/*
 * PscMessageHandler.h
 *
 *  Created on: 4 Jul 2013
 *      Author: maora
 */

#ifndef PSCMESSAGEHANDLER_H_
#define PSCMESSAGEHANDLER_H_

#include "PscMessageStructs.h"
#include <CMessageTask.h>
#include "SubsystemInstantiator.h"
#include "BoardStateMachine.h"
#include <AppLoader.h>
#include <TaskPriorities.h>

#ifdef FEC2_BOARD
#include <Peripherals/PSoC/PsocManager.h>
#endif

//#define CMessageTestTask PscMessageHandler
#ifdef FEC2_BOARD
#define M_MESSAGE_HANDLER_DEFAULT_STACK_DEPTH 1000
#else
#define M_MESSAGE_HANDLER_DEFAULT_STACK_DEPTH 700
#endif

/**
 *
 * Class to handle messages coming from the OPC.
 */
class PscMessage: public CMessage
{
public:
    PscMessageStruct messageStruct;
};

class PscMessageHandler: public CMessageTask
{
public:
    enum
    {
        MESSAGE_QUEUE_SIZE = 0x10,
    };

private:
#ifdef PHANTOM_BOARD
#ifdef _NEW_IBD_CONTROL_
    //PhantomIbdZones m_zoneManager;
#else
    //PhantomZones m_zoneManager;
#endif
#endif
#ifdef FEC_BOARD
    //FecZones m_zoneManager;
#endif

    static PscMessageHandler* p_instance; //!< Instance of the message handler

#ifdef FEC2_BOARD
    PsocManager m_psocManager;
#endif

    AppLoaderBase *m_pAppLoader;

    BoardStateMachine m_boardState;
    E_BoardMode m_boardMode;
    //bool m_deleteControlsOnDisconnection;

    PscMessageStruct m_messages[MESSAGE_QUEUE_SIZE];
    unsigned long m_currentMessage;

    uint16_t m_cableId;
    uint16_t m_masterControllerId;
    uint16_t m_deviceType;
    uint16_t m_boardAddress;
    uint32_t m_usartBaudRate;
    uint32_t m_lastConfigurationHash;
    uint32_t m_lastConfigurationTimestamp;

    CBinarySemaphore m_postMessageSemaphore;

    DECLARE_MESSAGE_MAP(PscMessageHandler)

#define M_BEGIN_MESSAGE_MAP(a, b)
#define M_END_MESSAGE_MAP(a)
#define M_MESSAGE_MAP_ENTRY(a, b, c) void c (unsigned long param);
#include "GeneratedMessages.msg.h"

        /**
         * Send ACK Command to the OPC.
         * @param messageId Original message ID
         * @param sn serial number of the original message
         * @param controllerId
         * @param deviceType
         * @param zoneId
         * @param status
         */
    void sendAck(unsigned long messageId, unsigned long sn, uint16_t boardId, uint16_t pssId, E_AckStatus status);

    /**
     * Send Sequence Ended to the OPC.
     * @param messageId Original message ID
     * @param sn serial number of the original message
     * @param controllerId
     * @param deviceType
     * @param zoneId
     * @param status
     */
    void sendSeqEnded(unsigned long messageId, unsigned long sn, uint16_t boardId, uint16_t pssId, E_SeqEndedStatus status);

    void sendZoneStatusHandler(PscMessageStruct &message, uint16_t boardId, char deviceId, E_ZoneType zoneType);
    PscMessageHandler();

public:

    static PscMessageHandler* getInstance()
    {
        if (p_instance == NULL || !p_instance->isValid())
        {
            p_instance = new PscMessageHandler();
            // Create the task.
            p_instance->create("PscMsgHdl", M_MESSAGE_HANDLER_DEFAULT_STACK_DEPTH, M_TASK_PRIORITY_PSC_MESSAGE_HANDLER);
        }

        return p_instance;
    }

    virtual ~PscMessageHandler();

    /**
     * post a message to the message handler.
     */
    void handleMessage(PscMessageStruct& message);

    /**
     * Task control function. It implements the message loop.
     */
    void run();

    /**
     * A subclass overriding this methods should call the base implementation.
     * \sa AManagedTask::OnCreate
     *
     * @param pcName specifies the name assigned to the task for debug purpose.
     * @param usStackDepth specifies the task stack size in words.
     * @param uxPriority specifies the task priority.
     *
     */
    portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
            unsigned portBASE_TYPE uxPriority);

    /**
     * The framework calls this function before the scheduler start. The default implementation simply return true.
     * Override this method to perform task specific hardware initialization that does not use FreeRTOS API.
     *
     * @return true if success, false otherwise.
     */
    virtual bool hardwareInit();

    void setBoardAddress(char ba)
    {
        m_boardAddress = ba;
    }

    /**
     * Reset the state of the server to its default state, so we can reconnect again from the OPC.
     */
    void reset();

    /**
     * Enter emergency state
     */
    void stopOnEmr();

    /**
     * Recover from EMR/Error
     */
    void recoverFromEmrError();

    void instantiateZoneManager();

    /**
     * Perform board shutdown
     */
    void performBoardShutdown();

#ifdef FEC2_BOARD
    PsocManager* getPsocManager()
    {
        return &m_psocManager;
    }
#endif

private:
    void suspendScheduler();
    void resumeScheduler();

    friend class FecBoardConfiguration;
};

#endif /* PSCMESSAGEHANDLER_H_ */
