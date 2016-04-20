/*
 * PsocManager.cpp
 *
 *  Created on: 20 бреб 2013
 *      Author: maora
 */

#include "PsocManager.h"
#include "PsocMessageStruct.h"
#include <Peripherals/PeripheralRepository.h>
#include <version.h>
#include <logger.h>
//#ifndef WIN32
//#include <arm_itm.h>
//#else
#define ITM_EVENT8(a,b)
//#endif

#define M_NUMBER_OF_RETRIES 10
#define M_DEFAULT_ROUND_ROBIN_DELAY 4

//PsocManager PsocManager::s_instance;

//extern "C" int g_txInterruptState;
//extern "C" int g_rxInterruptState;

//int lastTickCount;

T_PsocHandler::T_PsocHandler()
{
    cableId = -1;
//    m_psocPrimaryFunction = E_PsocPrimaryFunction_Undefined;
    psocHandler.setPsocCommState(E_PsocCommState_Disabled);
    reset();
}

void T_PsocHandler::reset()
{
    temperaturePeripheral = NULL;
    pwmPeripheral = NULL;
    digitalOutPeripheral = NULL;
    digitalInPeripheral = NULL;
    analogInPeripheral = NULL;
    analogOutPeripheral = NULL;
    comFailures = 0;
    spiComFailures = 0;
    nextRequest = E_PsocRequest_ReadTemperaturePwmDi;
    lastOutputsStatus = false;
//    psocHandler.reset();
}

void T_PsocHandler::resetPsocOutputs()
{
    if (psocHandler.getPsocCommState() != E_PsocCommState_Enabled)
        return;

    // reset all outputs on the PSoC:
    psocHandler.resetOutputs();

    // wait until all outputs have been written:
//    while (psocHandler.isPendingUpdate())
//    {
//        vTaskDelay(10);
//    }
}

PsocManager::PsocManager()
{
    m_currentPsoc = 0;
    m_roundRobinDelay = M_DEFAULT_ROUND_ROBIN_DELAY;
    m_numOfActivePsocs = 0;
    m_globalSpiComFailues = 0;

    m_blockingCallCompleteSemaphore.create();
    m_blockingCallCompleteSemaphore.take(0);

    m_semaphoreUsed.create();
    m_semaphoreUsed.give();

    m_totalNumberOfPsocs = Psc_GetNumberOfSlaves();
    m_isInBootloader = false;
    m_isSpiResetPending = false;

    m_lastRequest = E_PsocCommands_EmptyCommand;
    m_pBoardState = NULL;
    m_boardMode = E_BoardMode_Normal;
}

PsocManager::~PsocManager()
{
}

E_PsocSpiError PsocManager::getPsocCableID(uint8_t psocIndex, uint8_t& cable1, uint8_t& cable2, uint8_t& cable3,
        uint8_t& errorBits)
{
    PsocHandler* psocHandler;
    E_PsocSpiError spiResult;
    psocHandler = getPsocHandlerByIndex(psocIndex);

    if (psocHandler == NULL || psocHandler->getPsocCommState() == E_PsocCommState_Error)
    {
        cable1 = 0;
        cable2 = 0;
        cable3 = 0;
        return E_PsocSpiError_NoSlaveResponse;
    }
//        psocHandler->setPsocCommState(E_PsocCommState_Enabled);

    takeBlockingSemaphore();
    spiResult = psocHandler->getCableID(&m_blockingCallCompleteSemaphore, cable1, cable2, cable3, errorBits);
    giveBlockingSemaphore();

    if (spiResult != E_PsocSpiError_Ok)
    {
        psocHandler->setPsocCommState(E_PsocCommState_Error);
        cable1 = 0;
        cable2 = 0;
        cable3 = 0;
        return spiResult;
    }

    // verify cable ID's
    if (cable1 == cable2 && cable1 == cable3)
    {
        psocHandler->setCableID(cable1);
        if (cable1 == 0)
            psocHandler->setPsocCommState(E_PsocCommState_Disabled);
        else
            psocHandler->setPsocCommState(E_PsocCommState_Enabled);
    }
    // not all cable IDs are equal. So we find the most useful:
    else
    {
        if (cable1 != 0)
            psocHandler->setCableID(cable1);
        else if (cable2 != 0)
            psocHandler->setCableID(cable2);
        else if (cable3 != 0)
            psocHandler->setCableID(cable3);
    }

    return spiResult;
}

E_PsocSpiError PsocManager::getPsocVersion(uint8_t psocIndex, T_PsocVersion& version)
{
    PsocHandler* psocHandler;
    E_PsocSpiError spiResult;
    psocHandler = getPsocHandlerByIndex(psocIndex);

    if (psocHandler == NULL || psocHandler->getPsocCommState() == E_PsocCommState_Error)
    {
        version.firmwareVersion.full = 0;
        version.protocolVersion.full = 0;
    }
    else
    {
        takeBlockingSemaphore();
        spiResult = psocHandler->getVersion(&m_blockingCallCompleteSemaphore, version);
        giveBlockingSemaphore();
    }

    return spiResult;
}

PsocTemperaturePeripheral* PsocManager::getTemperaturePeripheralByIndex(int index)
{
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getTemperaturePeripheral();
}

PsocTemperaturePeripheral* PsocManager::getTemperaturePeripheralByCableId(int cableId)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getTemperaturePeripheral();
}

PsocPwmOutputPeripheral* PsocManager::getPwmPeripheralByCableId(int cableId)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getPwmPeripheral();
}

PsocPwmOutputPeripheral* PsocManager::getPwmPeripheralByIndex(int index)
{
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getPwmPeripheral();
}

PsocAnalogOutputPeripheral* PsocManager::getAnalogOutPeripheralByCableId(int cableId)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getAnalogOutPeripheral();
}

PsocAnalogOutputPeripheral* PsocManager::getAnalogOutPeripheralByIndex(int index)
{
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getAnalogOutPeripheral();
}

PsocDigitalOutputPeripheral* PsocManager::getDigitalOutPeripheralByCableId(int cableId)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getDigitalOutPeripheral();
}

PsocDigitalOutputPeripheral* PsocManager::getDigitalOutPeripheralByIndex(int index)
{
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getDigitalOutPeripheral();
}

PsocDigitalInputPeripheral* PsocManager::getDigitalInPeripheralByIndex(int index)
{
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getDigitalInPeripheral();
}

PsocDigitalInputPeripheral* PsocManager::getDigitalInPeripheralByCableId(int cableId)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getDigitalInPeripheral();
}

PsocAnalogInputsPeripheral* PsocManager::getAnalogInPeripheralByCableId(int cableId)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getAnalogInPeripheral();
}

PsocAnalogInputsPeripheral* PsocManager::getAnalogInPeripheralByIndex(int index)
{
    if (index == -1)
        return NULL;
    return m_psocHandlers[index].getAnalogInPeripheral();
}

void PsocManager::startShutdown()
{
    for (int i = 0; i < M_NUMBER_OF_PSOCS; ++i)
    {
        m_psocHandlers[i].setPsocIsInReady(false);
    }
}

// This method should never be called directly, since it's fucking up the psoc handler's semaphores.
// it should also always be called from within the PSoC Manager task.
void PsocManager::resetSpiCommunication()
{
#ifndef WIN32
    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Resetting SPI");
    // reset the SPI setup.
    PSOC_setupSpiWithDma();
    for (int i = 0; i < m_totalNumberOfPsocs; ++i)
    {
        m_psocHandlers[i].resetCommunications();
        m_psocHandlers[i].setPsocCommState(E_PsocCommState_Enabled);
        m_psocHandlers[i].m_comFailures = 0;
        m_psocHandlers[i].m_spiComFailures = 0;
    }
    downloadConfigurationToPsocs();
#endif
}

void PsocManager::execute(int psocIndex)
{
    E_PsocSpiError psocError;

    // update the next request now. In case a blocking request is performed, it will override this request.
    if (m_psocHandlers[psocIndex].isRequestPending())
    {
//        if (m_psocHandlers[psocIndex].isPendingUpdate())
//        {
//            m_psocHandlers[psocIndex].updatePendingOutputs();
//        }
//        else
//        {
            switch (m_psocHandlers[psocIndex].m_nextRequest)
            {
            case E_PsocRequest_ReadTemperaturePwmDi:
//                if (psocIndex == 1)
//                {
//                    printf("read delay: %d\n", getTickCount()-lastTickCount);
//                    lastTickCount = getTickCount();
//                }
                m_psocHandlers[psocIndex].getTemperaturePwmDiSensors();
                m_psocHandlers[psocIndex].m_nextRequest = E_PsocRequest_ReadAnalogsFeedbacksErrors;
                break;
            case E_PsocRequest_ReadAnalogsFeedbacksErrors:
                m_psocHandlers[psocIndex].getAnalogsFeedbacksErrors();
                m_psocHandlers[psocIndex].m_nextRequest = E_PsocRequest_ReadTemperaturePwmDi;
                break;
            }
//        }
    }

    // call the current psoc's execute
    psocError = m_psocHandlers[psocIndex].execute();
    if (psocError != E_PsocSpiError_Ok)
    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PSoC %d error %x", psocIndex, psocError);
        ++m_psocHandlers[psocIndex].m_comFailures;
        if (psocError == E_PsocSpiError_NoSpiResponse)
        {
            ++m_psocHandlers[psocIndex].m_spiComFailures;
            ++m_globalSpiComFailues;
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_FATAL, "DMA TX State %d RX State %d", g_txInterruptState, g_rxInterruptState);
        }
        if (m_psocHandlers[psocIndex].m_comFailures >= M_NUMBER_OF_RETRIES)
        {
            m_psocHandlers[psocIndex].setPsocCommState(E_PsocCommState_Error);
            if (m_psocHandlers[psocIndex].m_spiComFailures > 0)
            {
                //resetSpiCommunication();
                m_isSpiResetPending = true;
            }
        }
        if (m_globalSpiComFailues >= M_NUMBER_OF_RETRIES)
        {
//            resetSpiCommunication();
            m_isSpiResetPending = true;
        }
        // TODO: Move the board to error state.
    }
    else
    {
        m_psocHandlers[psocIndex].m_comFailures = 0;
        m_psocHandlers[psocIndex].m_spiComFailures = 0;
        m_globalSpiComFailues = 0;

        if (m_psocHandlers[psocIndex].isPsocIsInReady()
                && m_psocHandlers[psocIndex].getPsocPrimaryFunction() == E_PsocPrimaryFunction_Applicator)
        {
            if ((m_psocHandlers[psocIndex].getLastErrors() & E_PsocErrorBits_LiquidDetectorError))
                m_psocHandlers[psocIndex].getDigitalOutPeripheral()->getElementByIndex(3)->setValueValid(false);
            else
                m_psocHandlers[psocIndex].getDigitalOutPeripheral()->getElementByIndex(3)->setValueValid(true);

            //m_psocHandlers[psocIndex].digitalOutPeripheral->getElementByIndex(3)->setValueValid(((m_psocHandlers[psocIndex].getLastErrors() & E_PsocErrorBits_LiquidDetectorError) == 0));
        }

        // compare the current output status with the last iteration. If status has changed update the peripherals.
        bool currentOutputsStatus = m_psocHandlers[psocIndex].arePsocOutputsDisabled();

//        if (currentOutputsStatus != m_psocHandlers[psocIndex].lastOutputsStatus)
//        {
//            if (currentOutputsStatus)
//                int i = 0;
//            if (m_psocHandlers[psocIndex].pwmPeripheral != NULL)
//                m_psocHandlers[psocIndex].pwmPeripheral->setPsocOutputsEnabled(currentOutputsStatus);
//            if (m_psocHandlers[psocIndex].digitalOutPeripheral != NULL)
//                m_psocHandlers[psocIndex].digitalOutPeripheral->setPsocOutputsEnabled(currentOutputsStatus);
//            m_psocHandlers[psocIndex].lastOutputsStatus = currentOutputsStatus;
//        }
    }
}

void PsocManager::run()
{
    suspend();

    for (;;)
    {
        ITM_EVENT8(2, m_currentPsoc + 1);
        execute(m_currentPsoc);
        if (advanceToNextPsoc() && m_isSpiResetPending)
            resetSpiCommunication();
        // force a task switch in case a higher priority task is required.
//        yield();
        ITM_EVENT8(2, 0);
    }
}

bool PsocManager::hardwareInit()
{
    PSOC_setupSpiWithDma();

    if (Psc_BoardType == E_BoardType_Fec2)
    {
        m_psocHandlers[0].setPortAndPin(0, GPIOD, GPIO_Pin_2);
        m_psocHandlers[0].csHigh();
        m_psocHandlers[1].setPortAndPin(1, GPIOC, GPIO_Pin_6);
        m_psocHandlers[1].csHigh();
        m_psocHandlers[2].setPortAndPin(2, GPIOC, GPIO_Pin_7);
        m_psocHandlers[2].csHigh();
        m_psocHandlers[3].setPortAndPin(3, GPIOC, GPIO_Pin_8);
        m_psocHandlers[3].csHigh();
        m_psocHandlers[4].setPortAndPin(4, GPIOF, GPIO_Pin_0);
        m_psocHandlers[4].csHigh();
        m_psocHandlers[5].setPortAndPin(5, GPIOD, GPIO_Pin_15);
        m_psocHandlers[5].csHigh();
        m_psocHandlers[6].setPortAndPin(6, GPIOD, GPIO_Pin_6);
        m_psocHandlers[6].csHigh();
        m_psocHandlers[7].setPortAndPin(7, GPIOC, GPIO_Pin_9);
        m_psocHandlers[7].csHigh();
    }
    if (Psc_BoardType == E_BoardType_Fec3)
    {
        m_psocHandlers[0].setPortAndPin(0, GPIOD, GPIO_Pin_2);
        m_psocHandlers[0].csHigh();
        m_psocHandlers[1].setPortAndPin(1, GPIOC, GPIO_Pin_6);
        m_psocHandlers[1].csHigh();
        m_psocHandlers[2].setPortAndPin(2, GPIOC, GPIO_Pin_7);
        m_psocHandlers[2].csHigh();
        m_psocHandlers[3].setPortAndPin(3, GPIOC, GPIO_Pin_8);
        m_psocHandlers[3].csHigh();
        m_psocHandlers[4].setPortAndPin(4, GPIOE, GPIO_Pin_2);
        m_psocHandlers[4].csHigh();
        m_psocHandlers[5].setPortAndPin(5, GPIOD, GPIO_Pin_15);
        m_psocHandlers[5].csHigh();
        m_psocHandlers[6].setPortAndPin(6, GPIOD, GPIO_Pin_6);
        m_psocHandlers[6].csHigh();
        m_psocHandlers[7].setPortAndPin(7, GPIOC, GPIO_Pin_9);
        m_psocHandlers[7].csHigh();
        m_psocHandlers[8].setPortAndPin(8, GPIOD, GPIO_Pin_7);
        m_psocHandlers[8].csHigh();
        m_psocHandlers[9].setPortAndPin(9, GPIOB, GPIO_Pin_6);
        m_psocHandlers[9].csHigh();
    }

    return true;
}

bool PsocManager::initTemperaturePeripheralByCableId(int cableId, int pssId, int sampleInterval, int lpfWindow,
        int numberOfDevices)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return false;

    return initTemperaturePeripheralByIndex(index, pssId, sampleInterval, lpfWindow, numberOfDevices);
}

bool PsocManager::initTemperaturePeripheralByIndex(int index, int pssId, int sampleInterval, int lpfWindow,
        int numberOfDevices)
{
    if (index == -1)
        return false;

    if (m_psocHandlers[index].getTemperaturePeripheral() != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_psocHandlers[index].getTemperaturePeripheral()->getPssId() == 0)
        {
            m_psocHandlers[index].getTemperaturePeripheral()->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    PsocTemperaturePeripheral* tempPeriph = new PsocTemperaturePeripheral();
    m_psocHandlers[index].setTemperaturePeripheral(tempPeriph);
    PeripheralRepository::getInstance().addPeripheral(tempPeriph);

    if (sampleInterval == 0)
        sampleInterval = 100;

    tempPeriph->setPssId(pssId);
    tempPeriph->setPsocHandler(&m_psocHandlers[index]);
    tempPeriph->setUpdateInterval(sampleInterval);
    tempPeriph->setLpfWindow(lpfWindow);

    // CHECK: Do we need to delete the just created peripheral?
    if (tempPeriph->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PsocManager::initDigitalOutputPeripheralByCableId(int cableId, int pssId, int numberOfDevices)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return false;

    return initDigitalOutputPeripheralByIndex(index, pssId, numberOfDevices);
}

bool PsocManager::initDigitalOutputPeripheralByIndex(int index, int pssId, int numberOfDevices)
{
    if (index == -1)
        return false;

    if (m_psocHandlers[index].getDigitalOutPeripheral() != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_psocHandlers[index].getDigitalOutPeripheral()->getPssId() == 0)
        {
            m_psocHandlers[index].getDigitalOutPeripheral()->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    PsocDigitalOutputPeripheral* outPeriph = new PsocDigitalOutputPeripheral();
    m_psocHandlers[index].setDigitalOutPeripheral(outPeriph);
    PeripheralRepository::getInstance().addPeripheral(outPeriph);

    // we need to make sure that the PWM peripheral is instantiated, as it contains the channel type configuration
    // that should be downloaded to the PSoC when the configuration is done.
    if (initPwmPeripheralByIndex(index, 0, numberOfDevices) == false)
        return false;

    outPeriph->setPssId(pssId);
    outPeriph->setPsocHandler(&m_psocHandlers[index]);

    // CHECK: Do we need to delete the just created peripheral?
    if (outPeriph->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PsocManager::initDigitalInputPeripheralByCableId(int cableId, int pssId, int numberOfDevices)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return false;

    return initDigitalInputPeripheralByIndex(index, pssId, numberOfDevices);
}

bool PsocManager::initDigitalInputPeripheralByIndex(int index, int pssId, int numberOfDevices)
{
    if (index == -1)
        return false;

    if (m_psocHandlers[index].getDigitalInPeripheral() != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_psocHandlers[index].getDigitalInPeripheral()->getPssId() == 0)
        {
            m_psocHandlers[index].getDigitalInPeripheral()->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    PsocDigitalInputPeripheral* inPeriph = new PsocDigitalInputPeripheral();
    m_psocHandlers[index].setDigitalInPeripheral(inPeriph);
    PeripheralRepository::getInstance().addPeripheral(inPeriph);

    inPeriph->setPssId(pssId);
    inPeriph->setPsocHandler(&m_psocHandlers[index]);

    // CHECK: Do we need to delete the just created peripheral?
    if (inPeriph->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PsocManager::initAnalogInputPeripheralByCableId(int cableId, int pssId, int sampleInterval, int lpfWindow,
        int numberOfDevices)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return false;

    return initAnalogInputPeripheralByIndex(index, pssId, sampleInterval, lpfWindow, numberOfDevices);
}

bool PsocManager::initAnalogInputPeripheralByIndex(int index, int pssId, int sampleInterval, int lpfWindow,
        int numberOfDevices)
{
    if (index == -1)
        return false;

    if (m_psocHandlers[index].getAnalogInPeripheral() != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_psocHandlers[index].getAnalogInPeripheral()->getPssId() == 0)
        {
            m_psocHandlers[index].getAnalogInPeripheral()->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    PsocAnalogInputsPeripheral* inPeriph = new PsocAnalogInputsPeripheral();
    m_psocHandlers[index].setAnalogInPeripheral(inPeriph);
    PeripheralRepository::getInstance().addPeripheral(inPeriph);

    inPeriph->setPssId(pssId);
    inPeriph->setPsocHandler(&m_psocHandlers[index]);
    inPeriph->setUpdateInterval(sampleInterval);

    // CHECK: Do we need to delete the just created peripheral?
    if (inPeriph->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PsocManager::initPwmPeripheralByCableId(int cableId, int pssId, int numberOfDevices)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return false;

    return initPwmPeripheralByIndex(index, pssId, numberOfDevices);
}

bool PsocManager::initPwmPeripheralByIndex(int index, int pssId, int numberOfDevices)
{
    if (index == -1)
        return false;

    if (m_psocHandlers[index].getPwmPeripheral() != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_psocHandlers[index].getPwmPeripheral()->getPssId() == 0)
        {
            m_psocHandlers[index].getPwmPeripheral()->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    PsocPwmOutputPeripheral* pwmPeriph = new PsocPwmOutputPeripheral();
    m_psocHandlers[index].setPwmPeripheral(pwmPeriph);
    PeripheralRepository::getInstance().addPeripheral(pwmPeriph);

    pwmPeriph->setPssId(pssId);
    pwmPeriph->setPsocHandler(&m_psocHandlers[index]);

    // CHECK: Do we need to delete the just created peripheral?
    if (pwmPeriph->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PsocManager::initAnalogOutputPeripheralByCableId(int cableId, int pssId, int numberOfDevices)
{
    int index = getPsocIndexByCableId(cableId);
    if (index == -1)
        return false;

    return initAnalogOutputPeripheralByIndex(index, pssId, numberOfDevices);
}

bool PsocManager::initAnalogOutputPeripheralByIndex(int index, int pssId, int numberOfDevices)
{
    if (index == -1)
        return false;

    if (m_psocHandlers[index].getAnalogOutPeripheral() != NULL)
    {
        if (pssId == 0)
        {
            return true;
        }
        else if (m_psocHandlers[index].getAnalogOutPeripheral()->getPssId() == 0)
        {
            m_psocHandlers[index].getAnalogOutPeripheral()->setPssId(pssId);
            return true;
        }
        else
            return false;
    }

    PsocAnalogOutputPeripheral* analogOutPeriph = new PsocAnalogOutputPeripheral();
    m_psocHandlers[index].setAnalogOutPeripheral(analogOutPeriph);
    PeripheralRepository::getInstance().addPeripheral(analogOutPeriph);

    analogOutPeriph->setPssId(pssId);
    analogOutPeriph->setPsocHandler(&m_psocHandlers[index]);

    // CHECK: Do we need to delete the just created peripheral?
    if (analogOutPeriph->getElementCount() < numberOfDevices)
        return false;

    return true;
}

bool PsocManager::enablePsoc(int index)
{
    resume();
    m_isInBootloader = false;
    //++m_numOfActivePsocs;
    bool result;
    E_PsocSpiError spiError;
    T_PsocVersion psocVersion;
    uint8_t cable1, cable2, cable3, errors;
    if (m_psocHandlers[index].getPsocCommState() != E_PsocCommState_Enabled)
    {
        result = false;
        int numOfRetries = 0;
        do
        {
            // the psoc must enabled to try to communicate with it.
            // If communication fails we disable it again.
            m_psocHandlers[index].setPsocCommState(E_PsocCommState_Enabled);

            // first we need to check that the protocol versions match:
            do
            {
                spiError = getPsocVersion(index, psocVersion);
                if (spiError == E_PsocSpiError_Ok)
                {
                    if (psocVersion.firmwareVersion.full != 0 && psocVersion.protocolVersion.full != 0)
                        result = true;
                }
            } while (++numOfRetries < M_NUMBER_OF_RETRIES && result == false);

            if (result)
            {
                T_VersionUnion expectedIcdVersion;
                expectedIcdVersion.split.major = M_PSOC_ICD_VERSION_MAJOR;
                expectedIcdVersion.split.minor = M_PSOC_ICD_VERSION_MINOR;
                expectedIcdVersion.split.build = M_PSOC_ICD_VERSION_BUILD;
                expectedIcdVersion.split.revision = M_PSOC_ICD_VERSION_REVISION;

                if (psocVersion.protocolVersion.full != expectedIcdVersion.full || psocVersion.boardType != Psc_GetBoardType())
                    result = false;
                else
                {
                    result = false;
                    do
                    {
                        spiError = getPsocCableID(index, cable1, cable2, cable3, errors);
                        if (spiError == E_PsocSpiError_Ok/* && errors == 0*/)
                        {
//                            m_psocHandlers[index].resetPsocOutputs();
                            result = true;
                        }
//                        {
//                            // first we need
//                            if (cable1 == cable2 && cable1 == cable3)
//                            {
//                                m_psocHandlers[index].cableId = cable1;
//                                if (cable1 == 0)
//                                    m_psocHandlers[index].setPsocState(E_PsocCommState_Disabled);
//                                else
//                                    m_psocHandlers[index].setPsocState(E_PsocCommState_Enabled);
//                                result = true;
//                            }
//                        }
                    } while (++numOfRetries < M_NUMBER_OF_RETRIES && result == false);
                }
            }
        } while (++numOfRetries < M_NUMBER_OF_RETRIES && result == false);
    }
    if (result == false)
    {
        m_psocHandlers[index].setPsocCommState(E_PsocCommState_Error);
        --m_numOfActivePsocs;
    }
    if (m_numOfActivePsocs == 0)
    {
        suspend();
    }
    return result;
}

void PsocManager::initAllPsocs()
{
    int i;
//    suspend();

    delay(1000);

    m_totalNumberOfPsocs = Psc_GetNumberOfSlaves();
    m_numOfActivePsocs = m_totalNumberOfPsocs;
    PSOC_setupSpiWithDma();
    hardwareInit();

//    if (Psc_BoardType == E_BoardType_Fec3)
//    {
//        FEC3_PSOC_RESET_HIGH();
//        delay(500);
//        FEC3_PSOC_RESET_LOW();
//        delay(1000);
//    }

//    for (i = 0; i < m_totalNumberOfPsocs; ++i)
//    {
//        execute(i);
////        delay(10);
////        execute(i);
//    }
//
//    delay(2500);

//    resume();

    for (i = 0; i < m_totalNumberOfPsocs; ++i)
    {
        enablePsoc(i);
    }
}

void PsocManager::enableDebugCableIdMode()
{
    for (int i = 0; i < m_totalNumberOfPsocs; ++i)
    {
        m_psocHandlers[i].enableDebugCableIdMode();
        m_psocHandlers[i].setCableID(i + 1);
    }
}

/**
 * Return TRUE if finished the round-robin trip.
 */
bool PsocManager::advanceToNextPsoc()
{
    int previousPsoc = m_currentPsoc;
    do
    {
        m_currentPsoc = (m_currentPsoc + 1);
        if (m_currentPsoc >= m_totalNumberOfPsocs)
            m_currentPsoc -= m_totalNumberOfPsocs;
    } while (m_numOfActivePsocs != 0 && m_psocHandlers[m_currentPsoc].getPsocCommState() == E_PsocCommState_Error);
    // if the new index is the same as the previous index,
    // add a delay to let the PSoC handle the request.
    if (previousPsoc >= m_currentPsoc)
    {
        delay(m_roundRobinDelay);
        return true;
    }
    else
        return false;
//    if (m_numOfActivePsocs == 0)
//        suspend();
}

void PsocManager::reset()
{
//    m_numOfActivePsocs = 0;
//    suspend();

//    delay(2500);

    for (int i = 0; i < m_totalNumberOfPsocs; ++i)
    {
        // reset all outputs on the psoc.
        resetPsocOutputs(&m_psocHandlers[i]);
    }

    // this is not needed anymore, as the psoc's outputs are constantly written.
//    // wait for pending outputs to be reset
//    for (int i = 0; i < m_totalNumberOfPsocs; ++i)
//    {
//        while (m_psocHandlers[i].isPendingUpdate())
//            delay(10);
//    }

    for (int i = 0; i < m_totalNumberOfPsocs; ++i)
    {
        // reset the psoc handler:
        m_psocHandlers[i].resetPeripherals();
    }

//    PSOC_setupSpiWithDma();

//    initAllPsocs();
}

bool PsocManager::setPsocAllowedCableId(int index, uint64_t allowedMask)
{
    if (index >= m_totalNumberOfPsocs)
        return false;

    /*
     // if the psoc isn't enabled yet we need to enable it.
     if (m_psocHandlers[index].getPsocState() == E_PsocCommState_Disabled)
     // calling the get by index will also enable the PSoC.
     getPsocHandlerByIndex(index);
     */

    // check the state - if the psoc is not enabled it means that we can't communicate with it for some reason.
    // however this doesn't mean that we have an error, as we can assign an allowed mask for
    // all psocs, but use only some of them.
    if (m_psocHandlers[index].getPsocCommState() != E_PsocCommState_Enabled)
        return true;

    uint64_t actualCableMask = (1 << (m_psocHandlers[index].getCableID() - 1));

    // check the mask - if no bits are set it means the cable is not allowed for this PSoC.
    if ((allowedMask & actualCableMask == 0) && (actualCableMask != 0))
        return false;

    return true;
}

void PsocManager::downloadConfigurationToPsocs()
{
    // TODO: Check if this function can be called from the same psoc manager task.
    takeBlockingSemaphore();
    for (int i = 0; i < m_totalNumberOfPsocs; ++i)
    {
        // try to set the state to "Enabled". If there are peripherals defined the state would remain "Enabled".
        m_psocHandlers[i].setPsocCommState(E_PsocCommState_Enabled);
        if (m_psocHandlers[i].getPsocCommState() == E_PsocCommState_Enabled)
        {
            m_psocHandlers[i].startErrorRecovery(&m_blockingCallCompleteSemaphore);
            m_psocHandlers[i].startBoardConfig(&m_blockingCallCompleteSemaphore);
            if (m_psocHandlers[i].getTemperaturePeripheral() != NULL)
                m_psocHandlers[i].getTemperaturePeripheral()->downloadConfigurationToPsoc(
                        &m_blockingCallCompleteSemaphore);
            if (m_psocHandlers[i].getPwmPeripheral() != NULL)
                m_psocHandlers[i].getPwmPeripheral()->downloadConfigurationToPsoc(&m_blockingCallCompleteSemaphore);
            if (m_psocHandlers[i].getAnalogInPeripheral() != NULL)
                m_psocHandlers[i].getAnalogInPeripheral()->downloadConfigurationToPsoc(
                        &m_blockingCallCompleteSemaphore);
            m_psocHandlers[i].configChannelFunction(&m_blockingCallCompleteSemaphore,
                    m_psocHandlers[i].getPsocPrimaryFunction());
            m_psocHandlers[i].endBoardConfig(&m_blockingCallCompleteSemaphore);
            // TODO: In HW Valication mode, check if the Digital IO configuraiton hard-coded in the PSoC can be eliminated.
            // until then, we need to reconfigure the digital IOs:
            if (m_psocHandlers[i].getPsocPrimaryFunction() == E_PsocPrimaryFunction_HwValidation)
            {
                if (m_psocHandlers[i].getPwmPeripheral() != NULL)
                    m_psocHandlers[i].getPwmPeripheral()->downloadConfigurationToPsoc(&m_blockingCallCompleteSemaphore);
            }
        }

    }
    giveBlockingSemaphore();
}

bool PsocManager::setPsocChannelFunction(int cableId, E_PsocPrimaryFunction function)
{
//    m_psocHandlers[channelIndex].psocPrimaryFunction = function;
    PsocHandler* psocHandler;

    psocHandler = getPsocHandlerByCableId(cableId);

    if (psocHandler == NULL)
        return false;

    psocHandler->setPsocPrimaryFunction(function);

    return true;
}

E_PsocSpiError PsocManager::startPsocBootloader(uint8_t psocIndex)
{
    PsocHandler* psocHandler;
    E_PsocSpiError spiResult;

    if (!m_isInBootloader)
        return E_PsocSpiError_InvalidState;

    psocHandler = &m_psocHandlers[psocIndex];

    if (psocHandler == NULL)
        return E_PsocSpiError_NoSlaveResponse;

    psocHandler->csLow();

    spiResult = psocHandler->startPsocBootloader();

    psocHandler->csHigh();

    return spiResult;
}

E_PsocSpiError PsocManager::endPsocBootloader(uint8_t psocIndex)
{
    PsocHandler* psocHandler;
    psocHandler = &m_psocHandlers[psocIndex];

    if (psocHandler == NULL)
        return E_PsocSpiError_NoSlaveResponse;

    psocHandler->csHigh();

    return E_PsocSpiError_Ok;
}

E_PsocSpiError PsocManager::startBootloader()
{
    suspend();

    delay(2000);

    m_isInBootloader = true;

    return E_PsocSpiError_Ok;
}

void PsocManager::resetPsocOutputs(PsocHandler* psocHandler)
{
    if (psocHandler->getPsocCommState() != E_PsocCommState_Enabled)
        return;

    // reset all outputs on the PSoC:
    psocHandler->resetOutputs();

    // wait until all outputs have been written:
//    while (psocHandler->isPendingUpdate())
//    {
//        vTaskDelay(10);
//    }
}

void PsocManager::startRecovery()
{
    //resetSpiCommunication();
    //downloadConfigurationToPsocs();
    // TODO: Check if this function can be called from the same psoc manager task.
    takeBlockingSemaphore();
    for (int i = 0; i < m_totalNumberOfPsocs; ++i)
    {
        if (m_psocHandlers[i].getPsocCommState() == E_PsocCommState_Enabled)
        {
            m_psocHandlers[i].startErrorRecovery(&m_blockingCallCompleteSemaphore);
        }
    }
    giveBlockingSemaphore();
}
