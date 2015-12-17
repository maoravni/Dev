/*
 * PsocHandler.h
 *
 *  Created on: 24 бреб 2013
 *      Author: maora
 */

#ifndef PSOCHANDLER_H_
#define PSOCHANDLER_H_

#include "PsocMessageStruct.h"
#include <PscServer/PscMessageStructs.h>
#include <psocSpiWithDma.h>
#include "PsocMessageHandlers.h"
#include <logger.h>

#include "PsocTemperaturePeripheral.h"
#include "PsocPwmOutputPeripheral.h"
#include "PsocAnalogOutputPeripheral.h"
#include "PsocDigitalOutputPeripheral.h"
#include "PsocDigitalInputPeripheral.h"
#include "PsocAnalogInputsPeripheral.h"

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif

#define PIPELINED

#ifdef PIPELINED
#define M_FRAME_ARRAY_SIZE 2
#else
#define M_FRAME_ARRAY_SIZE 1
#endif
#define M_SEMAPHORE_POOL_SIZE 1

#define M_SINGLE_FRAME_COMPLETE_TIMEOUT 5
#define M_NUMBER_OF_DMA_SPI_RETRIES 10
#define M_DELAY_BETWEEN_READING_REPLY 4
#define M_NUMBER_OF_PSOCS 10
#define M_COMPLETE_SEMAPHORE_TIMEOUT M_SINGLE_FRAME_COMPLETE_TIMEOUT*M_NUMBER_OF_DMA_SPI_RETRIES*M_DELAY_BETWEEN_READING_REPLY*M_NUMBER_OF_PSOCS*2
#define M_FRAME_POP_FROM_QUEUE_TIMEOUT M_COMPLETE_SEMAPHORE_TIMEOUT*2
#define M_DEFAULT_PSOC_SPI_TIMEOUT 500
#define M_ALLOWED_CONSECUTIVE_OUTPUT_DIFFERENCES 10

enum E_PsocRequest
{
    E_PsocRequest_ReadTemperaturePwmDi, E_PsocRequest_ReadAnalogsFeedbacksErrors
};

enum E_PsocCommState
{
    E_PsocCommState_Disabled, E_PsocCommState_Enabled, E_PsocCommState_Error
};

enum E_DeviceWriteState
{
    E_DeviceWriteState_Idle, E_DeviceWriteState_Pending, E_DeviceWriteState_Verify
};

struct T_SpiFrameStruct
{
    CBinarySemaphore* completeSemaphore;
//    bool useSemaphore;
    T_MessagePacket transmitBuffer;
    T_MessagePacket receiveBuffer;
    PsocMessageHandlerBase *messageHandler;
    E_PsocSpiError responseStatus;
    uint8_t serialNumber;
    T_SpiFrameStruct()
    {
//        completeSemaphore.create();
//        completeSemaphore.take(10000);
        completeSemaphore = NULL;
        messageHandler = NULL;
        serialNumber = 0;
        memset(&transmitBuffer, 0, sizeof(T_MessagePacket));
        memset(&receiveBuffer, 0, sizeof(T_MessagePacket));
        responseStatus = E_PsocSpiError_UnknownError;
//        useSemaphore = false;
    }
};

class SpiFramePool
{
private:
    T_SpiFrameStruct m_spiFrameArray[M_FRAME_ARRAY_SIZE];
    CQueue m_spiFramePool;
public:
    SpiFramePool();
    void reset();
    T_SpiFrameStruct* popFrame(portTickType timeout = M_FRAME_POP_FROM_QUEUE_TIMEOUT)
    {
        T_SpiFrameStruct* frame = NULL;
        portBASE_TYPE result;
        result = m_spiFramePool.receive(&frame, timeout);
        if (result != pdTRUE)
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC SPI Frame Pool: Pop Frame Timeout");
//        printf("pop %x\n", frame);
        return frame;
    }
    void returnFrame(T_SpiFrameStruct* frame)
    {
        m_spiFramePool.send(&frame, 0);
    }
};

class CompleteSemaphorePool
{
private:
    CBinarySemaphore m_semaphoreArray[M_SEMAPHORE_POOL_SIZE];
    CQueue m_semaphorePool;
public:
    CompleteSemaphorePool();
    void reset();
    CBinarySemaphore* popSemaphore(portTickType timeout = M_FRAME_POP_FROM_QUEUE_TIMEOUT)
    {
        CBinarySemaphore* semaphore = NULL;
        portBASE_TYPE result;
        result = m_semaphorePool.receive(&semaphore, timeout);
        if (result != pdTRUE)
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC Complete Semaphore Pool: Pop Semaphore Timeout");
//        printf("pop %x\n", frame);
        return semaphore;
    }
    void returnSemaphore(CBinarySemaphore* semaphore)
    {
        m_semaphorePool.send(&semaphore, 0);
    }
};

class PsocHandler
{
private:
    GPIO_TypeDef* m_gpioPort;
    uint16_t m_gpioPin;
    uint16_t m_psocIndex;
    uint16_t m_cableId;
    //CBinarySemaphore m_blockingRequestSem;
    uint8_t m_lastSerialNumber;

    uint16_t m_systemStatus;
    uint16_t m_errors;
    uint16_t m_warnings;

    SpiFramePool m_spiFramePool;
//    CompleteSemaphorePool m_completeSemaphorePool;
#ifdef PIPELINED
    T_SpiFrameStruct* m_requestFrame;
    T_SpiFrameStruct* m_replyFrame;
#else
    T_SpiFrameStruct* m_activeFrame;
#endif

// Local copies of the sensor values of the PSoC.
    T_ReadTemperaturePwmDISensors m_shadowTemperaturePwmDiValues;
    T_ReadAnalogsFeedbacksErrors m_shadowAnalogsFeedbacksErrorsValues;
    uint32_t m_shadowPwmControlPulseCounter;
    uint32_t m_shadowPeakAccumulator[2];
    uint8_t m_pwmOutputs[M_NUM_OF_PWM_OUTPUTS];
    uint8_t m_analogOutputs[M_NUM_OF_ANALOG_OUTPUTS];
    E_DeviceWriteState m_pendingUpdate[E_PsocDeviceType_Size];
    int m_pendingUpdateCount;
    int m_digitalPwmOutputsVerifyErrorCount;

    E_PsocCommState m_psocCommState;

    bool m_debugModeCableId;
    bool m_psocIsInReady;

    E_PsocPrimaryFunction m_psocPrimaryFunction;

    PsocTemperaturePeripheral* m_temperaturePeripheral;
    PsocPwmOutputPeripheral* m_pwmPeripheral;
    PsocAnalogOutputPeripheral* m_analogOutPeripheral;
    PsocDigitalOutputPeripheral* m_digitalOutPeripheral;
    PsocDigitalInputPeripheral* m_digitalInPeripheral;
    PsocAnalogInputsPeripheral* m_analogInPeripheral;

public:
    uint8_t m_comFailures; //!< Number of communication failures.
    uint8_t m_spiComFailures; //!< Number of failures originating from local SPI peripheral.
    E_PsocRequest m_nextRequest;

public:
    PsocHandler();
    virtual ~PsocHandler();

    void setPortAndPin(uint16_t index, GPIO_TypeDef* port, uint16_t pin);

    void resetOutputs();
    void resetPeripherals();
    void resetCommunications();

    /*** PSOC Blocking Messages ***/
    E_PsocSpiError getVersion(CBinarySemaphore* completeSemaphore, T_PsocVersion& version);
    E_PsocSpiError getCableID(CBinarySemaphore* completeSemaphore, uint8_t &cable1, uint8_t &cable2, uint8_t &cable3,
            uint8_t &errorBits);
    E_PsocSpiError startErrorRecovery(CBinarySemaphore* completeSemaphore);
    E_PsocSpiError startBoardConfig(CBinarySemaphore* completeSemaphore);
    E_PsocSpiError endBoardConfig(CBinarySemaphore* completeSemaphore);
    E_PsocSpiError configTemperatureSensor(CBinarySemaphore* completeSemaphore, int channelIndex,
            E_PsocTemperatureSensorType channelType, E_PsocTemperatureSensorFilterType filterType, float hardLimit,
            float corrA, float corrB);
    E_PsocSpiError configCurrentLoop(CBinarySemaphore* completeSemaphore, int channelIndex, uint16_t minCurrentLimit,
            uint16_t maxCurrentLimit);
    E_PsocSpiError configDigitalIO(CBinarySemaphore* completeSemaphore, int channelIndex, E_DigitalIOType channelType,
            uint8_t riseStep, uint8_t fallStep, uint8_t riseTime, uint8_t fallTime);
    E_PsocSpiError configAnalogIO(CBinarySemaphore* completeSemaphore, int channelIndex, uint16_t minCurrentLimit,
            uint16_t maxCurrentLimit);
    E_PsocSpiError configChannelFunction(CBinarySemaphore* completeSemaphore, E_PsocPrimaryFunction channelFunction);

    E_PsocSpiError startPsocBootloader();

    void raiseError(E_PsocErrorBits psocError, E_PSSErrors pssError, bool state);
    void raiseWarning(E_PsocWarningBits warning, bool state);
    void raiseSystemStatus(E_PsocStatusBits statusBit, bool state);

    //    E_PsocSpiError keepAlive();
//    E_PsocSpiError getPsocStatus(E_PsocStatusBits &status);
//    E_PsocSpiError setErrorsMask(uint16_t mask);
//    E_PsocSpiError setWarningsMask(uint16_t mask);
//    E_PsocSpiError getErrors();
//    E_PsocSpiError getWarnings();
//    E_PsocSpiError configPwm(int channelIndex, E_PsocPwmType channelType, int channelGroup);
//    E_PsocSpiError configDigitalIO(int channelIndex, bool isInput, E_PsocDigitalIoDriveMode driveMode);
//    E_SpiError configAnalogIO(int channelIndex, );
//    E_SpiError write
//    E_PsocSpiError getDeviceErrors(E_PsocDeviceType deviceType, int channelIndex);
//    E_PsocSpiError getDeviceWarnings(E_PsocDeviceType deviceType, int channelIndex);

    uint16_t getPsocIndex() const
    {
        return m_psocIndex;
    }

    void setPsocIndex(uint16_t psocIndex)
    {
        m_psocIndex = psocIndex;
    }

    bool isRequestPending()
    {
#ifdef PIPELINED
        return (m_requestFrame == NULL);
#else
        return (m_activeFrame == NULL);
#endif
    }

    E_PsocCommState getPsocCommState() const
    {
        return m_psocCommState;
    }

    void setPsocCommState(E_PsocCommState psocState)
    {
        switch (psocState)
        {
        case E_PsocCommState_Error:
        case E_PsocCommState_Disabled:
            m_psocCommState = psocState;
            break;
        case E_PsocCommState_Enabled:
            if (m_temperaturePeripheral != NULL || m_pwmPeripheral != NULL || m_analogOutPeripheral != NULL
                    || m_digitalOutPeripheral != NULL || m_digitalInPeripheral != NULL || m_analogInPeripheral != NULL)
                m_psocCommState = E_PsocCommState_Enabled;
            else
                m_psocCommState = E_PsocCommState_Disabled;
            break;
        }
    }

    void enableDebugCableIdMode();

    uint16_t getLastErrors() const
    {
        return m_errors;
    }

    uint8_t getLastSerialNumber() const
    {
        return m_lastSerialNumber;
    }

    uint16_t getLastSystemStatus() const
    {
        return m_systemStatus;
    }

    bool isPsocIsInReady() const
    {
        return m_psocIsInReady;
    }

    void setPsocIsInReady(bool psocIsInReady)
    {
        m_psocIsInReady = psocIsInReady;
    }

    void csLow()
    {
        m_gpioPort->BSRRH = m_gpioPin;
    }
    void csHigh()
    {
        m_gpioPort->BSRRL = m_gpioPin;
    }

    E_PsocPrimaryFunction getPsocPrimaryFunction() const
    {
        return m_psocPrimaryFunction;
    }

    void setPsocPrimaryFunction(E_PsocPrimaryFunction psocPrimaryFunction)
    {
        m_psocPrimaryFunction = psocPrimaryFunction;
    }

    uint16_t getCableID() const
    {
        return m_cableId;
    }

    void setCableID(uint16_t cableId)
    {
        m_cableId = cableId;
    }

    PsocAnalogInputsPeripheral* getAnalogInPeripheral()
    {
        return m_analogInPeripheral;
    }

    void setAnalogInPeripheral(PsocAnalogInputsPeripheral* analogInPeripheral)
    {
        m_analogInPeripheral = analogInPeripheral;
    }

    PsocAnalogOutputPeripheral* getAnalogOutPeripheral()
    {
        return m_analogOutPeripheral;
    }

    void setAnalogOutPeripheral(PsocAnalogOutputPeripheral* analogOutPeripheral)
    {
        m_analogOutPeripheral = analogOutPeripheral;
    }

    PsocDigitalInputPeripheral* getDigitalInPeripheral()
    {
        return m_digitalInPeripheral;
    }

    void setDigitalInPeripheral(PsocDigitalInputPeripheral* digitalInPeripheral)
    {
        m_digitalInPeripheral = digitalInPeripheral;
    }

    PsocDigitalOutputPeripheral* getDigitalOutPeripheral()
    {
        return m_digitalOutPeripheral;
    }

    void setDigitalOutPeripheral(PsocDigitalOutputPeripheral* digitalOutPeripheral)
    {
        m_digitalOutPeripheral = digitalOutPeripheral;
    }

    PsocPwmOutputPeripheral* getPwmPeripheral()
    {
        return m_pwmPeripheral;
    }

    void setPwmPeripheral(PsocPwmOutputPeripheral* pwmPeripheral)
    {
        m_pwmPeripheral = pwmPeripheral;
    }

    PsocTemperaturePeripheral* getTemperaturePeripheral()
    {
        return m_temperaturePeripheral;
    }

    void setTemperaturePeripheral(PsocTemperaturePeripheral* temperaturePeripheral)
    {
        m_temperaturePeripheral = temperaturePeripheral;
    }

    //    void printQueueState();
private:
    E_PsocSpiError execute();
    E_PsocSpiError executeCommunication(uint8_t *transmitBuffer);
    E_PsocSpiError executeSingleFrameTransfer(uint8_t *transmitBuffer);

    /*** PSOC messages for dealing with continous update of sensor values ***/
    E_PsocSpiError getTemperaturePwmDiSensors();
    E_PsocSpiError getAnalogsFeedbacksErrors();

    E_PsocSpiError updatePendingOutputs();
    E_PsocSpiError updatePsocPwmOutputs();
    E_PsocSpiError updatePsocAnalogOutputs();
    bool verifyPsocOutputs();

    E_PsocSpiError psocMessagePrologue(T_SpiFrameStruct* frame);

    void updateShadowTemperaturePwmDiValues(T_ReadTemperaturePwmDISensors &sensorValues);
    void updateShadowAnalogsFeedbacksErrorsValues(T_ReadAnalogsFeedbacksErrors &sensorValues);

    void handleSystemStatusBits(uint16_t systemStatus);
    void handleSystemErrorBits(uint16_t systemErrors);
    void handleSystemWarningBits(uint16_t systemWarnings);
    bool arePsocOutputsDisabled()
    {
        return (m_psocIsInReady && ((m_systemStatus & M_STATUS_BITS_ERROR_MASK) != 0));
    }

//    inline int getNextFrameIndex()
//    {
//        return ((m_currentSpiFrame + 1) % M_FRAME_ARRAY_SIZE);
//    }
//    inline T_SpiFrameStruct* getReplyFrame()
//    {
//        T_SpiFrameStruct* result;
//        printQueueState();
//        result = &m_spiFrameArray[m_currentSpiFrame];
//        return (result->messageHandler) ? result : NULL;
//    }
//    inline T_SpiFrameStruct* getRequestFrame()
//    {
//        T_SpiFrameStruct* result;
//        printQueueState();
//        result = &m_spiFrameArray[((m_currentSpiFrame + 1) % M_FRAME_ARRAY_SIZE)];
//        return (result->messageHandler) ? result : NULL;
//    }
    inline T_SpiFrameStruct* popFrameFromPool()
    {
        T_SpiFrameStruct* result;
        result = m_spiFramePool.popFrame(M_FRAME_POP_FROM_QUEUE_TIMEOUT);
//        assert(m_activeFrame == NULL);
//        assert(result != NULL);
        if (result == NULL)
            return NULL;
#ifdef PIPELINED
        m_requestFrame = result;
        return m_requestFrame;
#else
        m_activeFrame = result;
        return m_activeFrame;
#endif
    }
//    inline T_SpiFrameStruct* getReplyToHandleFrame()
//    {
//        T_SpiFrameStruct* result;
//        printQueueState();
//        result = &m_spiFrameArray[((m_currentSpiFrame + 3) % M_FRAME_ARRAY_SIZE)];
//        return (!result->messageHandler) ? result : NULL;
//    }

    float getLocalTemperatureSensorValue(int index)
    {
        return m_shadowTemperaturePwmDiValues.temperatureSensors[index];
    }
    uint8_t getLocalTemperatureErrorBits()
    {
        return m_shadowTemperaturePwmDiValues.temperatureErrorBits;
    }
    uint16_t getLocalPwmOutputValue(int index)
    {
        return m_shadowTemperaturePwmDiValues.pwmOutputsReported[index];
    }
    uint16_t getLocalAnalogInValue(int index)
    {
        return m_shadowAnalogsFeedbacksErrorsValues.analogIn[index];
    }
    uint16_t getLocalPeakCurrentValue(int index)
    {
        return m_shadowAnalogsFeedbacksErrorsValues.peakCurrent[index];
    }
    uint32_t getLocalPwmControlPulseCounter()
    {
        uint32_t result = m_shadowPwmControlPulseCounter;
        m_shadowPwmControlPulseCounter = 0;
        return result;
    }
    uint32_t getLocalEnergyCounter(int index)
    {
        uint32_t result = m_shadowPeakAccumulator[index];
        m_shadowPeakAccumulator[index] = 0;
        return result;
    }
//    uint16_t getLocalBridgeCurrentValue(int index)
//    {
//        return m_shadowAnalogsFeedbacksErrorsValues.bridgeCurrents[index];
//    }
//    uint16_t getLocalCurrentLoopValue(int index)
//    {
//        return m_shadowAnalogsFeedbacksErrorsValues.currentLoop[index];
//    }
    uint16_t getLocalDigitalInputsValue()
    {
        return m_shadowTemperaturePwmDiValues.digitalInputBits;
    }

    void addPendingPwmOutputValue(int index, uint16_t value);
    void addPendingAnalogOutputValue(int index, uint16_t value);

    void addPendingUpdate(E_PsocDeviceType deviceType)
    {
        if (m_pendingUpdate[deviceType] != E_DeviceWriteState_Pending)
            m_pendingUpdateCount += 1;
        m_pendingUpdate[deviceType] = E_DeviceWriteState_Pending;
    }

    bool isPendingUpdate()
    {
        return (m_pendingUpdateCount != 0);
    }

    E_PsocDeviceType getPendingUpdate()
    {
        if (m_pendingUpdateCount == 0)
            return E_PsocDeviceType_Size;

        for (int i = 0; i < E_PsocDeviceType_Size; ++i)
        {
            if (m_pendingUpdate[i] == E_DeviceWriteState_Pending)
                return (E_PsocDeviceType) i;
        }
        return E_PsocDeviceType_Size;
    }

    void clearPendingUpdate(E_PsocDeviceType deviceType)
    {
        if (m_pendingUpdate[deviceType] == E_DeviceWriteState_Pending)
            m_pendingUpdateCount -= 1;
        m_pendingUpdate[deviceType] = E_DeviceWriteState_Verify;
    }

    void delegateErrorToPeripheral(E_PsocErrorBits error, bool state);
    void delegateWarningToPeripheral(E_PSSWarnings warning, bool state);

    friend class PMHReadTemperaturePwmDISensors;
    friend class PMHReadAnalogsFeedbacksErrors;
    friend class PsocTemperaturePeripheral;
    friend class PsocPwmOutputPeripheral;
    friend class PsocAnalogOutputPeripheral;
    friend class PsocDigitalOutputPeripheral;
    friend class PsocDigitalInputPeripheral;
    friend class PsocAnalogInputsPeripheral;
    friend class PsocCurrentInputsPeripheral;
    friend class PsocManager;
    friend class T_PsocHandler;
};

#endif /* PSOCHANDLER_H_ */

