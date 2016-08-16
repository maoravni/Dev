/*
 * PsocHandler.cpp
 *
 *  Created on: 24 бреб 2013
 *      Author: maora
 */

//#include "PsocMessageStruct.h"
#include "PsocHandler.h"
#include <assert.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <PscServer/PscMasterServer.h>

extern "C" xSemaphoreHandle psocSpiResponseCompleteSemaphore;
extern "C" T_PsocSpiReceiveFrame psocSpiResponseBuffer;

PMHGetVersion g_getVersionHandler;
PMHKeepAlive g_keepAliveHandler;
PMHReadTemperaturePwmDISensors g_readTemperaturePwmDISensorsHandler;
PMHReadAnalogsFeedbacksErrors g_readAnalogsFeedbacksErrors;
PMHGetPsocStatus g_getPsocStatus;
PMHConfigTemperatureSensor g_configTemperatureSensor;
PMHGetCableID g_getCableId;
PMHWriteMultipleDevices g_writeMultipleDevices;
PMHStartErrorRecovery g_startErrorRecovery;
PMHStartBoardConfig g_startBoardConfig;
PMHEndBoardConfig g_endBoardConfig;
PMHConfigCurrentLoop g_configCurrentLoop;
PMHConfigDigitalIO g_configDigitalIO;
PMHConfigAnalogIO g_configAnalogIO;
PMHConfigChannelFunction g_configChannelFunction;
PMHStartBootloader g_startBootloader;

#ifdef WIN32
#define M_PSOC_REQUEST_PROLOGUE(frame) frame = popFrameFromPool(); \
                                if (frame == NULL) \
																                                { \
                                    M_LOGGER_LOGF(M_LOGGER_LEVEL_FATAL, "---No empty frame!---"); \
                                    return E_PsocSpiError_HandlerFrameOverwritten; \
																                                }
// no semaphore was popped, so we break and return an unexpected error.
#define M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame) frame->completeSemaphore = completeSemaphore; \
                                                if (frame->completeSemaphore == NULL) \
												                                                { \
                                                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d: No free semaphores available", m_psocIndex); \
                                                    return E_PsocSpiError_NoAvailableSemaphore; \
												                                                } \
                                                portBASE_TYPE semResult = frame->completeSemaphore->take(M_COMPLETE_SEMAPHORE_TIMEOUT); \
                                                frame->completeSemaphore = NULL; \
                                                if (semResult != pdTRUE) \
												                                                { \
                                                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d: No slave response, complete semaphore timeout", m_psocIndex); \
                                                    return E_PsocSpiError_NoSlaveResponse; \
												                                                }
#else
#define M_PSOC_REQUEST_PROLOGUE(frame) frame = popFrameFromPool(); \
                                if (frame == NULL) \
								                                { \
                                    M_LOGGER_LOGF(M_LOGGER_LEVEL_FATAL, "---No empty frame!---"); \
                                    return E_PsocSpiError_HandlerFrameOverwritten; \
								                                }
// no semaphore was popped, so we break and return an unexpected error.
#define M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame) frame->completeSemaphore = completeSemaphore; \
                                                if (frame->completeSemaphore == NULL) \
                                                { \
                                                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d: No free semaphores available", m_psocIndex); \
                                                    return E_PsocSpiError_NoAvailableSemaphore; \
                                                } \
                                                portBASE_TYPE semResult = frame->completeSemaphore->take(M_COMPLETE_SEMAPHORE_TIMEOUT); \
                                                frame->completeSemaphore = NULL; \
                                                if (semResult != pdTRUE) \
                                                { \
                                                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d: No slave response, complete semaphore timeout", m_psocIndex); \
                                                    return E_PsocSpiError_NoSlaveResponse; \
                                                }
#endif

CompleteSemaphorePool::CompleteSemaphorePool()
{
    m_semaphorePool.create(M_SEMAPHORE_POOL_SIZE, sizeof(CBinarySemaphore*));
    for (int i = 0; i < M_SEMAPHORE_POOL_SIZE; ++i)
    {
        m_semaphoreArray[i].create();
        m_semaphoreArray[i].take(10000);
        returnSemaphore(&m_semaphoreArray[i]);
    }
}

void CompleteSemaphorePool::reset()
{
    int i;
    for (i = 0; i < M_SEMAPHORE_POOL_SIZE; ++i)
    {
        popSemaphore(0);
    }
    for (i = 0; i < M_SEMAPHORE_POOL_SIZE; ++i)
    {
        m_semaphoreArray[i].take(0);
        returnSemaphore(&m_semaphoreArray[i]);
    }
}

SpiFramePool::SpiFramePool()
{
    m_spiFramePool.create(M_FRAME_ARRAY_SIZE, sizeof(T_SpiFrameStruct*));
    for (int i = 0; i < M_FRAME_ARRAY_SIZE; ++i)
    {
        returnFrame(&m_spiFrameArray[i]);
    }
}

void SpiFramePool::reset()
{
    int i;
    // we need to reset the frame pool queue, so we first pop every element, and then return it back.
    for (i = 0; i < M_FRAME_ARRAY_SIZE; ++i)
    {
        popFrame(0);
    }
    for (i = 0; i < M_FRAME_ARRAY_SIZE; ++i)
    {
        returnFrame(&m_spiFrameArray[i]);
    }
}

PsocHandler::PsocHandler()
{
    int i;
    m_gpioPort = NULL;
    m_gpioPin = 0;
    m_cableId = 0;

#ifdef PIPELINED
    m_requestFrame = NULL;
    m_replyFrame = NULL;
#else
    m_activeFrame = NULL;
#endif

//    m_spiFrameHandlerQueue.create(2, sizeof(T_SpiFrameStruct*));

    for (i = 0; i < E_PsocDeviceType_Size; ++i)
    {
        m_pendingUpdate[i] = E_DeviceWriteState_Idle;
    }

    memset(m_pwmOutputs, 0, sizeof(m_pwmOutputs));
    m_pendingUpdateCount = 0;
    m_digitalPwmOutputsVerifyErrorCount = 0;
    m_lastSerialNumber = 0;

    m_systemStatus = 0;
    m_errors = 0;
    m_warnings = 0;

    m_shadowPwmControlPulseCounter = 0;
    m_shadowPeakAccumulator[0] = 0;
    m_shadowPeakAccumulator[1] = 0;

    m_debugModeCableId = false;
    m_psocIsInReady = false;

    m_psocPrimaryFunction = E_PsocPrimaryFunction_Undefined;

//    m_psocCommState = E_PsocCommState_Disabled;
    setPsocCommState(E_PsocCommState_Disabled);

    resetOutputs();

    resetPeripherals();
}

PsocHandler::~PsocHandler()
{
}

E_PsocSpiError PsocHandler::execute()
{
    E_PsocSpiError psocError;

    assert(m_gpioPort != NULL);

    // TODO: If there's no pending frame, generate an empty frame.
#ifdef PIPELINED
//    assert(m_requestFrame != NULL);
#else
    assert(m_activeFrame != NULL);
#endif

    // handle incoming requests
#ifdef PIPELINED
    if (m_requestFrame != NULL)
#else
    if (m_activeFrame != NULL)
#endif
    {
        // prepare the next transmit frame
        // TODO: Check if the request frame is NULL, then no request is being made.
#ifdef PIPELINED
        if (m_requestFrame->messageHandler == NULL)
        {
            // clear the start byte, so the psoc will not reply to the message.
            m_requestFrame->transmitBuffer.header.start = 1;
            m_requestFrame->transmitBuffer.header.commandStatus = 0;
        }
#else
        if (m_activeFrame->messageHandler == NULL)
        {
            // clear the start byte, so the psoc will not reply to the message.
            m_activeFrame->transmitBuffer.header.start = 1;
            m_activeFrame->transmitBuffer.header.commandStatus = 0;
        }
#endif

        // lower the cs for the current psoc
        csLow();

//        m_activeFrame->serialNumber = m_activeFrame->transmitBuffer.header.serialNumber;
#ifdef WIN32
        psocError = E_PsocSpiError_Ok;
#else
#ifdef PIPELINED
        psocError = executeCommunication((uint8_t*) &m_requestFrame->transmitBuffer);
#else
        psocError = executeCommunication((uint8_t*) &m_activeFrame->transmitBuffer);
#endif
#endif

#ifdef PIPELINED
        if (m_replyFrame != NULL && m_replyFrame->messageHandler != NULL)
        {
            if (psocError == E_PsocSpiError_Ok)
            {
                m_replyFrame->responseStatus = m_replyFrame->messageHandler->handleReceiveBuffer(this,
                        &m_replyFrame->receiveBuffer, m_replyFrame->serialNumber);

                if (m_replyFrame->responseStatus != E_PsocSpiError_Ok)
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d error %x message Id %x", m_psocIndex,
                            m_replyFrame->responseStatus, m_replyFrame->messageHandler->getPsocCommand());
//                // check if the reply was a blocking request by looking at the semaphore:
//                if (m_activeFrame->completeSemaphore != NULL)
//                    m_activeFrame->completeSemaphore->give();
//                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d success message Id %x", m_psocIndex, m_replyFrame->messageHandler->getPsocCommand());
            }
            else
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d error %x message Id %x", m_psocIndex, psocError,
                        m_replyFrame->messageHandler->getPsocCommand());
                // TODO: Handle error on transmission.
            }
            // check if the reply was a blocking request by looking at the semaphore:
            if (m_replyFrame->completeSemaphore != NULL)
                m_replyFrame->completeSemaphore->give();
        }
        else
        {
            if (m_replyFrame != NULL && m_replyFrame->completeSemaphore != NULL)
                m_replyFrame->completeSemaphore->give();
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d error %x", m_psocIndex, psocError);
            // TODO: Handle error on transmission.
        }
#else
        if (psocError == E_PsocSpiError_Ok)
        {
            vTaskDelay(M_DELAY_BETWEEN_READING_REPLY);

            psocError = executeCommunication((uint8_t*) &m_activeFrame->transmitBuffer);

            if (psocError == E_PsocSpiError_Ok)
            {
                m_activeFrame->responseStatus = m_activeFrame->messageHandler->handleReceiveBuffer(this,
                        &m_activeFrame->receiveBuffer, m_activeFrame->serialNumber);

                if (m_activeFrame->responseStatus != E_PsocSpiError_Ok)
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d error %x", m_psocIndex, m_activeFrame->responseStatus);
//                // check if the reply was a blocking request by looking at the semaphore:
//                if (m_activeFrame->completeSemaphore != NULL)
//                    m_activeFrame->completeSemaphore->give();
            }
            else
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d error %x", m_psocIndex, psocError);
                // TODO: Handle error on transmission.
            }
            // check if the reply was a blocking request by looking at the semaphore:
            if (m_activeFrame->completeSemaphore != NULL)
            m_activeFrame->completeSemaphore->give();
        }
        else
        {
            if (m_activeFrame != NULL && m_activeFrame->completeSemaphore != NULL)
            m_activeFrame->completeSemaphore->give();
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d error %x", m_psocIndex, psocError);
            // TODO: Handle error on transmission.
        }
#endif
        // raise the cs back
        csHigh();
    }

    // handle previous reply
//    if (m_replyFrame != NULL)
//    {
//        // handle the data received
//        if (m_replyFrame->messageHandler != NULL)
//        {
//            m_replyFrame->responseStatus = m_replyFrame->messageHandler->handleReceiveBuffer(this,
//                    &m_replyFrame->receiveBuffer, m_replyFrame->serialNumber);
//
//            // check if the reply was a blocking request by looking at the semaphore:
//            if (m_replyFrame->useSemaphore)
//                m_replyFrame->completeSemaphore.give();
//        }
//    }
    // TODO: what if the execute task is not started?

//    m_spiFrameArray[((m_currentSpiFrame + 2) % M_FRAME_ARRAY_SIZE)].messageHandler = NULL;
//    if (freeFrame != NULL)
//        freeFrame->messageHandler = NULL;
//    printQueueState();

    ++m_lastSerialNumber;

#ifdef PIPELINED
    // rearrange the frame pointers:
    // the reply is now free:
    T_SpiFrameStruct *freeFrame = m_replyFrame;
    // the request is next to be replied
    m_replyFrame = m_requestFrame;
    m_requestFrame = NULL;
    if (freeFrame != NULL)
        m_spiFramePool.returnFrame(freeFrame);
#else
    // rearrange the frame pointers:
    T_SpiFrameStruct *freeFrame = m_activeFrame;
    m_activeFrame = NULL;
    if (freeFrame != NULL)
    m_spiFramePool.returnFrame(freeFrame);
#endif

    return psocError;
}

E_PsocSpiError PsocHandler::executeSingleFrameTransfer(uint8_t* transmitBuffer)
{
    E_PsocSpiError result;

    // lower the cs for the current psoc
    //csLow();

    result = executeCommunication(transmitBuffer);
    result = executeCommunication(transmitBuffer);

    // raise the cs back
    //csHigh();

    return result;
}

E_PsocSpiError PsocHandler::executeCommunication(uint8_t *transmitBuffer)
{
    static int NoSpiResponseError = 0;
    // start the DMA data transfer
    PSOC_fullDuplexTransfer(transmitBuffer, M_PSOC_SPI_FRAME_SIZE);

    // wait for the transfer to complete:
    if (xSemaphoreTake(psocSpiResponseCompleteSemaphore, M_SINGLE_FRAME_COMPLETE_TIMEOUT) == pdFALSE)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d: No SPI response, semaphore timeout", m_psocIndex);
        ++NoSpiResponseError;
        // if we encounter enough failures to read the SPI, we need to reset the DMA.
//        if (NoSpiResponseError >= M_NUMBER_OF_DMA_SPI_RETRIES)
//        {
//            // suspend all tasks
        vTaskSuspendAll();
        PSOC_setupSpiWithDma();
        xTaskResumeAll();
//        }
        return E_PsocSpiError_NoSpiResponse;
    }

    // check that the transfer is complete, and no timeout has occured:
    if (!psocSpiResponseBuffer.transferComplete)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d: No slave response, transfer not complete", m_psocIndex);
        return E_PsocSpiError_NoSlaveResponse;
    }

    if (!psocSpiResponseBuffer.crcOk)
    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d: CRC Error", m_psocIndex);
        return E_PsocSpiError_CrcError;
    }

    return E_PsocSpiError_Ok;
}

void PsocHandler::setPortAndPin(uint16_t index, GPIO_TypeDef* port, uint16_t pin)
{
    m_gpioPort = port;
    m_gpioPin = pin;
    m_psocIndex = index;
}

E_PsocSpiError PsocHandler::getVersion(CBinarySemaphore* completeSemaphore, T_PsocVersion& version)
{
    T_SpiFrameStruct* frame;

    M_PSOC_REQUEST_PROLOGUE(frame);

    frame->messageHandler = &g_getVersionHandler;
    g_getVersionHandler.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    assert(frame->completeSemaphore == NULL);
    frame->completeSemaphore = completeSemaphore;

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    memcpy(&version, &frame->receiveBuffer.data.psocVersion, sizeof(T_PsocVersion));
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Get Psoc Version: %d: firmware %d.%d.%d.%d --- icd %d.%d.%d.%d board %d",
            m_psocIndex, version.firmwareVersion.split.major, version.firmwareVersion.split.minor,
            version.firmwareVersion.split.build, version.firmwareVersion.split.revision,
            version.protocolVersion.split.major, version.protocolVersion.split.minor,
            version.protocolVersion.split.build, version.protocolVersion.split.revision, version.boardType);
    return frame->responseStatus;
}

E_PsocSpiError PsocHandler::getTemperaturePwmDiSensors()
{
    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

    // assign the semaphore, so that the handler will give it back when the transaction is complete:
    frame->completeSemaphore = NULL;
    frame->messageHandler = &g_readTemperaturePwmDISensorsHandler;
    // todo: pass board voltage
//    frame->transmitBuffer.data.writeMultipleDevices.voltage = Psc_GetBoardVoltage24V();
    g_readTemperaturePwmDISensorsHandler.prepareTransmitBufferOutputs(&frame->transmitBuffer, m_lastSerialNumber,
            m_pwmOutputs, m_analogOutputs, Psc_GetBoardVoltage24V());
    return E_PsocSpiError_Ok;
}

E_PsocSpiError PsocHandler::getAnalogsFeedbacksErrors()
{
    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

    // assign the semaphore, so that the handler will give it back when the transaction is complete:
    frame->completeSemaphore = NULL;
    frame->messageHandler = &g_readAnalogsFeedbacksErrors;
    // todo: pass board voltage
//    frame->transmitBuffer.data.psocRequestWithVoltage.voltage = Psc_GetBoardVoltage24V();
    g_readAnalogsFeedbacksErrors.prepareTransmitBufferOutputs(&frame->transmitBuffer, m_lastSerialNumber, m_pwmOutputs,
            m_analogOutputs, Psc_GetBoardVoltage24V());
    return E_PsocSpiError_Ok;
}

E_PsocSpiError PsocHandler::updatePsocPwmOutputs()
{
    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

    // assign the semaphore, so that the handler will give it back when the transaction is complete:
    frame->completeSemaphore = NULL;
    frame->messageHandler = &g_writeMultipleDevices;
    g_writeMultipleDevices.prepareTransmitBufferPwm(&frame->transmitBuffer, m_lastSerialNumber, m_pwmOutputs);

    return E_PsocSpiError_Ok;
}

E_PsocSpiError PsocHandler::updatePsocAnalogOutputs()
{
    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

    // assign the semaphore, so that the handler will give it back when the transaction is complete:
    frame->completeSemaphore = NULL;
    frame->messageHandler = &g_writeMultipleDevices;
    g_writeMultipleDevices.prepareTransmitBufferAnalogOuts(&frame->transmitBuffer, m_lastSerialNumber, m_analogOutputs);

    return E_PsocSpiError_Ok;
}

bool PsocHandler::verifyPsocOutputs()
{
    if (m_pendingUpdate[E_PsocDeviceType_DigitalPWM] == E_DeviceWriteState_Verify)
    {
        for (int i = 0; i < M_NUM_OF_PWM_OUTPUTS; ++i)
        {
//            if (m_pwmOutputs[i] != m_shadowTemperaturePwmDiValues.pwmOutputsReported[i])
            bool requestedOutputsNotZero = (m_pwmOutputs[i] != 0);
            bool psocOutputsNotZero = (m_shadowTemperaturePwmDiValues.pwmOutputsReported[i]);
            if (!psocOutputsNotZero && requestedOutputsNotZero)
            //if (psocOutputsNotZero != requestedOutputsNotZero)
            {
                // retry the write.
                addPendingUpdate(E_PsocDeviceType_DigitalPWM);
                return false;
            }
        }
        m_pendingUpdate[E_PsocDeviceType_DigitalPWM] = E_DeviceWriteState_Idle;
    }
//    if (m_pendingUpdate[E_PsocDeviceType_AnalogOutput] == E_DeviceWriteState_Verify)
//    {
//        for (int i = 0; i < M_NUM_OF_ANALOG_OUTPUTS; ++i)
//        {
//            if (m_analogOutputs[i] != m_shadowTemperaturePwmDiValues.pwmOutputsReported[i])
//            {
//                // retry the write.
//                addPendingUpdate(E_PsocDeviceType_DigitalPWM);
//                return false;
//            }
//        }
//        m_pendingUpdate[E_PsocDeviceType_DigitalPWM] = E_DeviceWriteState_Idle;
//    }
    return true;
}

E_PsocSpiError PsocHandler::configTemperatureSensor(CBinarySemaphore* completeSemaphore, int channelIndex,
        E_PsocTemperatureSensorType channelType, E_PsocTemperatureSensorFilterType filterType, float hardLimit,
        float corrA, float corrB, E_MissingDevicePriority priority)
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "configTemperatureSensor: psocIndex=%d channelIndex=%d hardLimit=%f missPrio=%d", m_psocIndex, channelIndex,
            hardLimit, priority);

    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

    // assign the semaphore, so that the handler will give it back when the transaction is complete:
//    frame->completeSem = &m_blockingRequestSem;
    frame->messageHandler = &g_configTemperatureSensor;
    frame->transmitBuffer.data.configTemperatureSensor.channelIndex = channelIndex;
    frame->transmitBuffer.data.configTemperatureSensor.sensorType = channelType;
    frame->transmitBuffer.data.configTemperatureSensor.hardLimit = hardLimit;
    frame->transmitBuffer.data.configTemperatureSensor.filterType = filterType;
    frame->transmitBuffer.data.configTemperatureSensor.aCoeff = corrA;
    frame->transmitBuffer.data.configTemperatureSensor.bCoeff = corrB;
    frame->transmitBuffer.data.configTemperatureSensor.missingSensorPriority = priority;
    g_configTemperatureSensor.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    return frame->responseStatus;
}

E_PsocSpiError PsocHandler::configCurrentLoop(CBinarySemaphore* completeSemaphore, int channelIndex,
        uint16_t minCurrentLimit, uint16_t maxCurrentLimit)
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "configCurrentLoop: psocIndex=%d channelIndex=%d minCurrentLimit=%d maxCurrentLimit=%d", m_psocIndex,
            channelIndex, minCurrentLimit, maxCurrentLimit);

    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

    // assign the semaphore, so that the handler will give it back when the transaction is complete:
//    frame->completeSem = &m_blockingRequestSem;
    frame->messageHandler = &g_configCurrentLoop;
    frame->transmitBuffer.data.configCurrentLoop.channelIndex = channelIndex;
    frame->transmitBuffer.data.configCurrentLoop.minCurrentLimit = minCurrentLimit;
    frame->transmitBuffer.data.configCurrentLoop.maxCurrentLimit = maxCurrentLimit;

    g_configCurrentLoop.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    return frame->responseStatus;
}

E_PsocSpiError PsocHandler::configAnalogIO(CBinarySemaphore* completeSemaphore, int channelIndex,
        uint16_t minCurrentLimit, uint16_t maxCurrentLimit)
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "configAnalogIO: psocIndex=%d channelIndex=%d minCurrentLimit=%d maxCurrentLimit=%d", m_psocIndex,
            channelIndex, minCurrentLimit, maxCurrentLimit);

    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

    // assign the semaphore, so that the handler will give it back when the transaction is complete:
//    frame->completeSem = &m_blockingRequestSem;
    frame->messageHandler = &g_configCurrentLoop;
    frame->transmitBuffer.data.configCurrentLoop.channelIndex = channelIndex;
    frame->transmitBuffer.data.configCurrentLoop.minCurrentLimit = minCurrentLimit;
    frame->transmitBuffer.data.configCurrentLoop.maxCurrentLimit = maxCurrentLimit;

    g_configCurrentLoop.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    return frame->responseStatus;
}

E_PsocSpiError PsocHandler::getCableID(CBinarySemaphore* completeSemaphore, uint8_t& cable1, uint8_t& cable2,
        uint8_t& cable3, uint8_t& errorBits)
{
    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

    // assign the semaphore, so that the handler will give it back when the transaction is complete:
//    frame->completeSem = &m_blockingRequestSem;
    frame->messageHandler = &g_getCableId;
    // TODO: Calc board voltage
    g_getCableId.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber, Psc_GetBoardVoltage24V());

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    T_PsocCableIdReply* payload = &frame->receiveBuffer.data.psocCableIdReply;
#ifdef DRYER_DEBUG
    cable1 = m_psocIndex + 1;
    cable2 = m_psocIndex + 1;
    cable3 = m_psocIndex + 1;
    m_cableId = m_psocIndex + 1;
#else
    cable1 = payload->output1Id;
    cable2 = payload->input1Id;
    cable3 = payload->input2Id;
    m_cableId = cable1;
#endif
    errorBits = payload->errorBits;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Get Psoc Cable ID: %d: %d %d %d", m_psocIndex, payload->input1Id,
            payload->input2Id, payload->output1Id);
    if (m_debugModeCableId)
    {
        cable1 = m_psocIndex + 1;
        cable2 = m_psocIndex + 1;
        cable3 = m_psocIndex + 1;
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Get ___Debug___ Psoc Cable ID: %d: %d %d %d", m_psocIndex, cable1, cable2,
                cable3);
    }

    return frame->responseStatus;
}

E_PsocSpiError PsocHandler::configDigitalIO(CBinarySemaphore* completeSemaphore, int channelIndex,
        E_DigitalIOType channelType, uint8_t riseStep, uint8_t fallStep, uint8_t riseTime, uint8_t fallTime)
{
    T_SpiFrameStruct* frame;

    M_PSOC_REQUEST_PROLOGUE(frame);

//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "configDigitalIO PSoC %d Channel %d ChannelType %d", m_psocIndex, channelIndex, channelType);
    frame->messageHandler = &g_configDigitalIO;
    frame->transmitBuffer.data.configDigitalIO.channelIndex = channelIndex;
    frame->transmitBuffer.data.configDigitalIO.channelType = channelType;
    frame->transmitBuffer.data.configDigitalIO.RiseStepSize = riseStep;
    frame->transmitBuffer.data.configDigitalIO.FallStepSize = fallStep;
    frame->transmitBuffer.data.configDigitalIO.RiseStepTickCount = riseTime;
    frame->transmitBuffer.data.configDigitalIO.FallStepTickCount = fallTime;

    g_configDigitalIO.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    return frame->responseStatus;

}

//int handlerLastTickCount;
void PsocHandler::updateShadowTemperaturePwmDiValues(T_ReadTemperaturePwmDISensors& sensorValues)
{
    handleSystemStatusBits(sensorValues.systemStatus);

    memcpy(&m_shadowTemperaturePwmDiValues, &sensorValues, sizeof(T_ReadTemperaturePwmDISensors));

    // count the number of times the requested output values are difference than reported:
    if (verifyPsocOutputs())
    {
        if (m_digitalPwmOutputsVerifyErrorCount >= M_ALLOWED_CONSECUTIVE_OUTPUT_DIFFERENCES)
            PscMasterServer::getInstance().sendError(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, 0);
        m_digitalPwmOutputsVerifyErrorCount = 0;
    }
    else
    {
        if (m_digitalPwmOutputsVerifyErrorCount < M_ALLOWED_CONSECUTIVE_OUTPUT_DIFFERENCES)
        {
            ++m_digitalPwmOutputsVerifyErrorCount;
            if (m_digitalPwmOutputsVerifyErrorCount >= M_ALLOWED_CONSECUTIVE_OUTPUT_DIFFERENCES)
                PscMasterServer::getInstance().sendError(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE,
                        E_PSSErrors_PsocOutputsNotOpen);
        }
    }
}

E_PsocSpiError PsocHandler::configChannelFunction(CBinarySemaphore* completeSemaphore,
        E_PsocPrimaryFunction channelFunction)
{
    T_SpiFrameStruct* frame;

    M_PSOC_REQUEST_PROLOGUE(frame);

    frame->messageHandler = &g_configChannelFunction;
    frame->transmitBuffer.data.configChannelFunction.primaryFunction = (uint8_t) channelFunction;
    g_configChannelFunction.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    return frame->responseStatus;
}

void PsocHandler::updateShadowAnalogsFeedbacksErrorsValues(T_ReadAnalogsFeedbacksErrors& sensorValues)
{
    handleSystemStatusBits(sensorValues.systemStatus);
    handleSystemErrorBits(sensorValues.systemErrors);
    handleSystemWarningBits(sensorValues.systemWarnings);

    memcpy(&m_shadowAnalogsFeedbacksErrorsValues, &sensorValues, sizeof(T_ReadAnalogsFeedbacksErrors));

    m_shadowPwmControlPulseCounter += m_shadowAnalogsFeedbacksErrorsValues.pwmCounts;
    m_shadowPeakAccumulator[0] += m_shadowAnalogsFeedbacksErrorsValues.peakCurrent[0] * m_shadowPwmControlPulseCounter;
    m_shadowPeakAccumulator[1] += m_shadowAnalogsFeedbacksErrorsValues.peakCurrent[1] * m_shadowPwmControlPulseCounter;
}

void PsocHandler::resetOutputs()
{
    int i;
    for (i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
        addPendingPwmOutputValue(i, 0);
    for (i = 0; i < M_NUM_OF_PSOC_ANALOG_OUTPUTS; ++i)
        addPendingAnalogOutputValue(i, 0);

    m_comFailures = 0;
    m_spiComFailures = 0;

}

void PsocHandler::resetCommunications()
{
    m_spiFramePool.reset();
//    m_completeSemaphorePool.reset();
#ifdef PIPELINED
    m_requestFrame = NULL;
    m_replyFrame = NULL;
#else
    m_activeFrame = NULL;
#endif
    setPsocCommState(E_PsocCommState_Enabled);
    //m_psocIsInReady = false;
}

void PsocHandler::resetPeripherals()
{
    m_temperaturePeripheral = NULL;
    m_pwmPeripheral = NULL;
    m_digitalOutPeripheral = NULL;
    m_digitalInPeripheral = NULL;
    m_analogInPeripheral = NULL;
    m_analogOutPeripheral = NULL;
    m_comFailures = 0;
    m_spiComFailures = 0;
    m_nextRequest = E_PsocRequest_ReadTemperaturePwmDi;
    setPsocCommState(E_PsocCommState_Disabled);
//    m_lastOutputsStatus = false;
}

void PsocHandler::enableDebugCableIdMode()
{
    m_debugModeCableId = true;
}

void PsocHandler::addPendingPwmOutputValue(int index, uint16_t value)
{
    m_pwmOutputs[index] = value;
    addPendingUpdate(E_PsocDeviceType_DigitalPWM);
}

void PsocHandler::addPendingAnalogOutputValue(int index, uint16_t value)
{
    if (value > 0)
        value += 50;
    if (value > 250)
        value = 250;
    m_analogOutputs[index] = value;
    addPendingUpdate(E_PsocDeviceType_AnalogOutput);
}

E_PsocSpiError PsocHandler::updatePendingOutputs()
{
    E_PsocSpiError result;
    E_PsocDeviceType deviceType = getPendingUpdate();

    switch (deviceType)
    {
    case E_PsocDeviceType_DigitalPWM:
//    case E_PsocDeviceType_H_BridgePWM:
        result = updatePsocPwmOutputs();
        break;
    case E_PsocDeviceType_AnalogOutput:
        result = updatePsocAnalogOutputs();
        break;
    }

    if (result == E_PsocSpiError_Ok)
        clearPendingUpdate(deviceType);
    return result;
}

E_PsocSpiError PsocHandler::startErrorRecovery(CBinarySemaphore* completeSemaphore)
{
    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

// assign the semaphore, so that the handler will give it back when the transaction is complete:
//    frame->completeSem = &m_blockingRequestSem;
    frame->messageHandler = &g_startErrorRecovery;
    g_startErrorRecovery.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    return frame->responseStatus;
}

E_PsocSpiError PsocHandler::startBoardConfig(CBinarySemaphore* completeSemaphore)
{
    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

// assign the semaphore, so that the handler will give it back when the transaction is complete:
//    frame->completeSem = &m_blockingRequestSem;
    frame->messageHandler = &g_startBoardConfig;
    g_startBoardConfig.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    m_psocIsInReady = false;

    return frame->responseStatus;
}

E_PsocSpiError PsocHandler::endBoardConfig(CBinarySemaphore* completeSemaphore)
{
    T_SpiFrameStruct* frame;
    M_PSOC_REQUEST_PROLOGUE(frame);

// assign the semaphore, so that the handler will give it back when the transaction is complete:
//    frame->completeSem = &m_blockingRequestSem;
    frame->messageHandler = &g_endBoardConfig;
    g_endBoardConfig.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    M_PSOC_REQUEST_BLOCKING_EPILOGUE(frame);

    m_psocIsInReady = true;

    return frame->responseStatus;
}

void PsocHandler::handleSystemStatusBits(uint16_t systemStatus)
{
#define M_CHECK_FOR_SYSTEM_STATUS_ERROR(PSOC_ERROR, PSS_ERROR) if (systemStatus & PSOC_ERROR) PscMasterServer::getInstance().sendError(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, PSS_ERROR)
#define M_CHECK_FOR_SYSTEM_STATUS_WARNING(PSOC_WARNING, PSS_WARNING) if (systemStatus & PSOC_WARNING) PscMasterServer::getInstance().sendWarning(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, PSS_WARNING)
    uint16_t systemStatusWarnings = systemStatus & M_STATUS_BITS_WARNING_MASK;
    uint16_t systemStatusErrors = systemStatus & M_STATUS_BITS_ERROR_MASK;
    if (isPsocIsInReady())
        systemStatusErrors |= systemStatus & M_STATUS_BITS_OPERATION_STATE;
//    uint16_t a = M_STATUS_BITS_WARNING_MASK;
//    uint16_t b = M_STATUS_BITS_ERROR_MASK;

    if (getPsocCommState() != E_PsocCommState_Enabled)
        return;

    if (m_systemStatus == systemStatus)
        return;

    if (systemStatusWarnings != (m_systemStatus & M_STATUS_BITS_WARNING_MASK))
    {
        if (systemStatusWarnings != 0)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_WARNING, "PSoC %d System Status Warnings %x", m_psocIndex,
                    systemStatusWarnings);
            M_CHECK_FOR_SYSTEM_STATUS_WARNING(E_PsocStatusBits_SystemModuleWarning,
                    E_PSSWarnings_PsocSystemModuleWarning);
            M_CHECK_FOR_SYSTEM_STATUS_WARNING(E_PsocStatusBits_TemperatureModuleWarning,
                    E_PSSWarnings_PsocTemperatureModuleWarning);
            M_CHECK_FOR_SYSTEM_STATUS_WARNING(E_PsocStatusBits_AnalogModuleWarning,
                    E_PSSWarnings_PsocAnalogModuleWarning);
            M_CHECK_FOR_SYSTEM_STATUS_WARNING(E_PsocStatusBits_MonitorModuleWarning,
                    E_PSSWarnings_PsocMonitorModuleWarning);
            M_CHECK_FOR_SYSTEM_STATUS_WARNING(E_PsocStatusBits_SafetyModuleWarning,
                    E_PSSWarnings_PsocSafetyModuleWarning);
        }
        else
        {
            if (m_systemStatus)
                M_LOGGER_LOGF(M_LOGGER_LEVEL_WARNING, "PSoC %d System Status No Warnings", m_psocIndex);
            PscMasterServer::getInstance().sendWarning(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, 0);
        }
    }
    if (systemStatusErrors != m_systemStatus & M_STATUS_BITS_ERROR_MASK)
    {
        if (systemStatusErrors != 0)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d System Status Errors %x", m_psocIndex, systemStatusErrors);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_AnalogModuleError, E_PSSErrors_PsocAnalogModuleError);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_SystemModuleError, E_PSSErrors_PsocSystemModuleError);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_TemperatureModuleError,
                    E_PSSErrors_PsocTemperatureModuleError);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_MonitorModuleError, E_PSSErrors_PsocMonitorModuleError);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_SafetyModuleError, E_PSSErrors_PsocSafetyError);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_PsocOffline, E_PSSErrors_PsocNotInOperationalState);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_SystemNotInitialized,
                    E_PSSErrors_PsocNotInOperationalState);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_BITFailed, E_PSSErrors_PsocNotInOperationalState);
            M_CHECK_FOR_SYSTEM_STATUS_ERROR(E_PsocStatusBits_SystemNotConfigured,
                    E_PSSErrors_PsocNotInOperationalState);
        }
        else
        {
            if (m_systemStatus)
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d System Status No Errors", m_psocIndex);
            PscMasterServer::getInstance().sendError(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, 0);
        }
    }
    m_systemStatus = systemStatus;
}

void PsocHandler::handleSystemErrorBits(uint16_t systemErrors)
{
//#define M_CHECK_FOR_SYSTEM_ERROR(PSOC_ERROR, PSS_ERROR) if (systemErrors & PSOC_ERROR) PscMasterServer::getInstance().sendError(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, PSS_ERROR)
#define M_CHECK_FOR_SYSTEM_ERROR(PSOC_ERROR, PSS_ERROR) if (errorsXored & PSOC_ERROR) raiseError(PSOC_ERROR, PSS_ERROR, (systemErrors & PSOC_ERROR) != 0)

    if (getPsocCommState() != E_PsocCommState_Enabled)
        return;

    uint16_t errorsXored = systemErrors ^ m_errors;

    if (errorsXored != 0)
    {
//        if (systemErrors != 0)
//        {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d System Error %x", m_psocIndex, systemErrors);
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Error Xor %x", errorsXored);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_SafetyRelayLow, E_PSSErrors_PsocSafetyError);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_HardTemperatureLimit, E_PSSErrors_DeviceExceedsHardLimits);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_CableIdNotMatching, E_PSSErrors_CableIdNotMatching);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_Pt100MissingSensor, E_PSSErrors_SensorMalfunction);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_Pt100CalResistorError, E_PSSErrors_SensorMalfunction);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_SoftTemperatureLimit, E_PSSErrors_DeviceExceedsSoftLimits);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_SsrOverheating, E_PSSErrors_SsrOverheating);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_MissingCurrentSensor, E_PSSErrors_SensorMalfunction);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_SsrAlwaysOn, E_PSSErrors_SsrAlwaysOn);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_OverCurrentLimit, E_PSSErrors_CurrentLoopAboveLimit);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_PrintBarLifted, E_PSSErrors_PrintBarLifted);
        M_CHECK_FOR_SYSTEM_ERROR(E_PsocErrorBits_LiquidDetectorError, E_PSSErrors_PsocLiquidDetectorError);
        //            M_CHECK_FOR_SYSTEM_WARNING(E_PsocWarningBits_MissingTemperatureSensor,
        //                    E_PSSWarnings_PsocMissingTemperatureSensor);
//        }
//        else
//        {
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d System Error No Errors", m_psocIndex);
//            PscMasterServer::getInstance().sendError(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, 0);
//        }
    }
    m_errors = systemErrors;
}

void PsocHandler::handleSystemWarningBits(uint16_t systemWarnings)
{
#define M_CHECK_FOR_SYSTEM_WARNING(PSOC_WARNING, PSS_WARNING) if (systemWarnings & PSOC_WARNING) PscMasterServer::getInstance().sendWarning(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, PSS_WARNING)

    if (getPsocCommState() != E_PsocCommState_Enabled)
        return;

    if (systemWarnings != m_warnings)
    {
        if (systemWarnings != 0)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_WARNING, "PSoC %d System Warning %x", m_psocIndex, systemWarnings);
//            M_CHECK_FOR_SYSTEM_WARNING(E_PsocWarningBits_LargeTemperatureVariation,
//                    E_PSSWarnings_PsocLargeTemperatureVariation);
//            M_CHECK_FOR_SYSTEM_WARNING(E_PsocWarningBits_MissingTemperatureSensor,
//                    E_PSSWarnings_PsocMissingTemperatureSensor);
            M_CHECK_FOR_SYSTEM_WARNING(E_PsocWarningBits_SsrAlwaysOff, E_PSSWarnings_PsocSsrAlwaysOff);
            M_CHECK_FOR_SYSTEM_WARNING(E_PsocWarningBits_SsrOverheating, E_PSSWarnings_PsocSsrHighTemperature);
        }
        else
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_WARNING, "PSoC %d System Error No Warnings", m_psocIndex);
            PscMasterServer::getInstance().sendWarning(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, 0);
        }
    }
    m_warnings = systemWarnings;
}

E_PsocSpiError PsocHandler::psocMessagePrologue(T_SpiFrameStruct* frame)
{
    frame = popFrameFromPool();
    if (frame == NULL)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_FATAL, "---No empty frame!---");
        return E_PsocSpiError_HandlerFrameOverwritten;
    }

    return E_PsocSpiError_Ok;
}

E_PsocSpiError PsocHandler::startPsocBootloader()
{
    T_SpiFrameStruct* frame;

    M_PSOC_REQUEST_PROLOGUE(frame);

    frame->messageHandler = &g_startBootloader;
    g_startBootloader.prepareTransmitBuffer(&frame->transmitBuffer, m_lastSerialNumber);

    E_PsocSpiError result = executeSingleFrameTransfer((uint8_t*) &frame->transmitBuffer);

    return result;
}

void PsocHandler::raiseError(E_PsocErrorBits psocError, E_PSSErrors pssError, bool state)
{
    uint16_t lastError = m_errors;

    if (state)
        m_errors |= psocError;
    else
        m_errors &= ~psocError;

    if (m_errors != lastError)
    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Sending PSoC error %x / PSS Error %x state %d for PSoC %d", psocError, pssError, state, m_psocIndex);
        PscMasterServer::getInstance().sendError(m_cableId, M_PSS_ID_NONE, M_PSS_ID_NONE, m_errors);
        delegateErrorToPeripheral(psocError, state);
    }
}

void PsocHandler::raiseWarning(E_PsocWarningBits warning, bool state)
{
}

void PsocHandler::raiseSystemStatus(E_PsocStatusBits statusBit, bool state)
{
}

void PsocHandler::delegateErrorToPeripheral(E_PsocErrorBits error, bool state)
{
#define M_DELEGATE_ERROR_TO_PERIPHERAL(p, e, s) if (p != NULL) p->raiseError(e, s)

    M_DELEGATE_ERROR_TO_PERIPHERAL(m_temperaturePeripheral, error, state);
    M_DELEGATE_ERROR_TO_PERIPHERAL(m_pwmPeripheral, error, state);
    M_DELEGATE_ERROR_TO_PERIPHERAL(m_analogOutPeripheral, error, state);
    M_DELEGATE_ERROR_TO_PERIPHERAL(m_digitalOutPeripheral, error, state);
    M_DELEGATE_ERROR_TO_PERIPHERAL(m_digitalInPeripheral, error, state);
    M_DELEGATE_ERROR_TO_PERIPHERAL(m_analogInPeripheral, error, state);
}

void PsocHandler::delegateWarningToPeripheral(E_PSSWarnings warning, bool state)
{
#define M_DELEGATE_WARNING_TO_PERIPHERAL(p, e, s) if (p != NULL) p->raiseWarning(e, s)

    M_DELEGATE_WARNING_TO_PERIPHERAL(m_temperaturePeripheral, warning, state);
    M_DELEGATE_WARNING_TO_PERIPHERAL(m_pwmPeripheral, warning, state);
    M_DELEGATE_WARNING_TO_PERIPHERAL(m_analogOutPeripheral, warning, state);
    M_DELEGATE_WARNING_TO_PERIPHERAL(m_digitalOutPeripheral, warning, state);
    M_DELEGATE_WARNING_TO_PERIPHERAL(m_digitalInPeripheral, warning, state);
    M_DELEGATE_WARNING_TO_PERIPHERAL(m_analogInPeripheral, warning, state);
}
