/*
 * PsocManager.h
 *
 *  Created on: 20 бреб 2013
 *      Author: maora
 */

#ifndef PSOCMANAGER_H_
#define PSOCMANAGER_H_

#include "PsocMessageStruct.h"
#include <psocSpiWithDma.h>
#include "PsocHandler.h"
#include <AManagedTask.h>
#include "PsocTemperaturePeripheral.h"
#include "PsocPwmOutputPeripheral.h"
#include "PsocAnalogOutputPeripheral.h"
#include "PsocDigitalOutputPeripheral.h"
#include "PsocDigitalInputPeripheral.h"
#include "PsocAnalogInputsPeripheral.h"
#include <PscServer/BoardStateMachine.h>

#define M_MAX_NUMBER_OF_PSOC 10

class T_PsocHandler
{
public:
//    E_PsocState state;
    uint8_t cableId;
//    E_PsocPrimaryFunction psocPrimaryFunction;
    uint8_t comFailures; //!< Number of communication failures.
    uint8_t spiComFailures;
    PsocHandler psocHandler;
    E_PsocRequest nextRequest;
    bool lastOutputsStatus;
    PsocTemperaturePeripheral* temperaturePeripheral;
    PsocPwmOutputPeripheral* pwmPeripheral;
    PsocAnalogOutputPeripheral* analogOutPeripheral;
    PsocDigitalOutputPeripheral* digitalOutPeripheral;
    PsocDigitalInputPeripheral* digitalInPeripheral;
    PsocAnalogInputsPeripheral* analogInPeripheral;
    T_PsocHandler();
    void reset();
    void resetPsocOutputs();
};

class PsocManager: public AManagedTask
{
private:
//    static PsocManager s_instance;

    E_PsocCommands m_lastRequest;

    uint8_t m_totalNumberOfPsocs;
    uint8_t m_currentPsoc;
    uint8_t m_roundRobinDelay;
    uint8_t m_numOfActivePsocs;
    int m_globalSpiComFailues;
    bool m_isInBootloader;
    bool m_isSpiResetPending;

    PsocHandler m_psocHandlers[M_MAX_NUMBER_OF_PSOC];

    /// This semaphore is used to signal to the blocking function call that the data is ready.
    CBinarySemaphore m_blockingCallCompleteSemaphore;

    /// This semaphore is used to indicate that there's already a method blocking blocking call complete semaphore.
    CBinarySemaphore m_semaphoreUsed;

    BoardStateMachine* m_pBoardState;
    E_BoardMode m_boardMode;

public:
    PsocManager();
    virtual ~PsocManager();

//    static inline PsocManager& getInstance()
//    {
//        return s_instance;
//    }

    void startShutdown();
    void reset();
    // Run a single cycle of communication with the psocs:
    void execute(int psocIndex);

    /**
     * Task control loop.
     */
    virtual void run();

    /**
     * The framework calls this function before the scheduler start. The default implementation simply return true.
     * Override this method to perform task specific hardware initialization that does not use FreeRTOS API.
     *
     * @return true if success, false otherwise.
     */
    virtual bool hardwareInit();

    bool setPsocAllowedCableId(int index, uint64_t mask);

    bool initTemperaturePeripheralByCableId(int cableId, int pssId, int sampleInterval, int lpfWindow,
            int numberOfDevices);
    bool initPwmPeripheralByCableId(int cableId, int pssId, int numberOfDevices);
    bool initAnalogOutputPeripheralByCableId(int cableId, int pssId, int numberOfDevices);
    bool initDigitalOutputPeripheralByCableId(int cableId, int pssId, int numberOfDevices);
    bool initDigitalInputPeripheralByCableId(int cableId, int pssId, int numberOfDevices);
    bool initAnalogInputPeripheralByCableId(int cableId, int pssId, int sampleInterval, int lpfWindow, int numberOfDevices);

    bool initTemperaturePeripheralByIndex(int index, int pssId, int sampleInterval, int lpfWindow, int numberOfDevices);
    bool initPwmPeripheralByIndex(int index, int pssId, int numberOfDevices);
    bool initAnalogOutputPeripheralByIndex(int index, int pssId, int numberOfDevices);
    bool initDigitalOutputPeripheralByIndex(int index, int pssId, int numberOfDevices);
    bool initDigitalInputPeripheralByIndex(int index, int pssId, int numberOfDevices);
    bool initAnalogInputPeripheralByIndex(int index, int pssId, int sampleInterval, int lpfWindow, int numberOfDevices);

    PsocTemperaturePeripheral* getTemperaturePeripheralByCableId(int cableId);
    PsocPwmOutputPeripheral* getPwmPeripheralByCableId(int cableId);
    PsocAnalogOutputPeripheral* getAnalogOutPeripheralByCableId(int cableId);
    PsocDigitalOutputPeripheral* getDigitalOutPeripheralByCableId(int cableId);
    PsocDigitalInputPeripheral* getDigitalInPeripheralByCableId(int cableId);
    PsocAnalogInputsPeripheral* getAnalogInPeripheralByCableId(int cableId);

    PsocTemperaturePeripheral* getTemperaturePeripheralByIndex(int index);
    PsocPwmOutputPeripheral* getPwmPeripheralByIndex(int index);
    PsocAnalogOutputPeripheral* getAnalogOutPeripheralByIndex(int index);
    PsocDigitalOutputPeripheral* getDigitalOutPeripheralByIndex(int index);
    PsocDigitalInputPeripheral* getDigitalInPeripheralByIndex(int index);
    PsocAnalogInputsPeripheral* getAnalogInPeripheralByIndex(int index);

    bool enablePsoc(int index);
    void initAllPsocs();
    void downloadConfigurationToPsocs();
    bool setPsocChannelFunction(int cableId, E_PsocPrimaryFunction function);

    void enableDebugCableIdMode();

    E_PsocSpiError getPsocCableID(uint8_t psocIndex, uint8_t &cable1, uint8_t &cable2, uint8_t &cable3,
            uint8_t &errorBits);
    E_PsocSpiError getPsocVersion(uint8_t psocIndex, T_PsocVersion& version);

    BoardStateMachine* getBoardState() const
    {
        return m_pBoardState;
    }

    void setBoardState(BoardStateMachine* pBoardState)
    {
        m_pBoardState = pBoardState;
    }

    E_PsocSpiError startBootloader();
    void startRecovery();

    E_BoardMode getBoardMode() const
    {
        return m_boardMode;
    }

    void setBoardMode(E_BoardMode boardMode)
    {
        m_boardMode = boardMode;
    }

private:
    // TODO: initialize the hardware lines for the psoc.
    void initializeGpio();
    // TODO: initialize the DMA and the SPI.
    void initializeDmaSpi();

    void sendBuffer(int psocIndex, uint8_t *buffer, int bufferLength);
    E_PsocSpiError receiveBuffer(int psocIndex, uint8_t* buffer, int &bufferLength);
    E_PsocSpiError startPsocBootloader(uint8_t psocIndex);
    E_PsocSpiError endPsocBootloader(uint8_t psocIndex);

    /**
     * Returns pointer to a PsocHandler. The handler is found by its index.
     * Returns NULL when the handler is not found, or is in error.
     * @param index
     * @return
     */
    PsocHandler* getPsocHandlerByIndex(int index)
    {
        if (index >= m_totalNumberOfPsocs)
            return NULL;
        if (m_psocHandlers[index].getPsocCommState() == E_PsocCommState_Error)
            return NULL;
        return &m_psocHandlers[index];
    }

    PsocHandler* getPsocHandlerByCableId(int cableId)
    {
        int index = getPsocIndexByCableId(cableId);
        if (index == -1)
            return NULL;
        return &m_psocHandlers[index];
    }

    int getPsocIndexByCableId(int cableId)
    {
        if (m_boardMode == E_BoardMode_HwValidation)
        {
            --cableId;
            if (m_psocHandlers[cableId].getPsocCommState() != E_PsocCommState_Error)
                return cableId;
        }
        else
        {
            for (int i = 0; i < m_totalNumberOfPsocs; ++i)
            {
                if (m_psocHandlers[i].getPsocCommState() != E_PsocCommState_Error
                        && m_psocHandlers[i].getCableID() == cableId)
                    return i;
            }
        }
        return -1;
    }

    bool advanceToNextPsoc();
    void resetSpiCommunication();

    void takeBlockingSemaphore()
    {
        m_semaphoreUsed.take(M_COMPLETE_SEMAPHORE_TIMEOUT);
    }
    void giveBlockingSemaphore()
    {
        m_semaphoreUsed.give();
    }

    void setPsocCsLow(int index)
    {
        PsocHandler* psocHandler = &m_psocHandlers[index];
        if (psocHandler != NULL)
            psocHandler->csLow();
    }

    void setPsocCsHigh(int index)
    {
        PsocHandler* psocHandler = &m_psocHandlers[index];
        if (psocHandler != NULL)
            psocHandler->csHigh();
    }

    void resetPsocOutputs(PsocHandler* psocHandler);

	virtual portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) { return pdTRUE; }

    friend class PsocAppLoader;
};

#endif /* PSOCMANAGER_H_ */
