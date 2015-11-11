/*
 * TmcMessageHandler.h
 *
 *  Created on: 18 бреб 2012
 *      Author: maora
 */

#ifndef TMCMESSAGEHANDLER_H_
#define TMCMESSAGEHANDLER_H_

#include <CMessageTask.h>
#include "MessageStructs.h"
#include "PeriodicMonitorTask.h"
#ifdef PHANTOM_BOARD
#ifdef _NEW_IBD_CONTROL_
#include "PhantomIbdZones.h"
#else
#include "PhantomZones.h"
#endif
#endif
#include <CCountingSemaphore.h>
#include <CBinarySemaphore.h>
#include <MiddlewareRegisterMonitor.h>

#define CMessageTestTask TmcMessageHandler

/**
 *
 * Class to handle messages coming from the OPC.
 */

class TmcMessage : public CMessage
{
public:
    TmcMessageStruct messageStruct;
};

class TmcMessageHandler: public CMessageTask
{
public:
    enum {
        MESSAGE_QUEUE_SIZE = 0x8,
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

    ZoneManager* m_pZoneManager;
    MiddlewareRegisterMonitor m_registerMonitor;

    TmcMessageStruct m_tmcMessages[MESSAGE_QUEUE_SIZE];
    unsigned long m_currentMessage;
    //TmcMessageStruct m_replyMessage; //!< Struct for the reply message.

    //TODO: Should these parameters be read from non-volatile memory?
    char m_boardId;
    char m_masterControllerId;
    char m_deviceType;
    char m_boardAddress;

    CBinarySemaphore m_postMessageSemaphore;

    DECLARE_MESSAGE_MAP(TmcMessageHandler)

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
    void sendAck(unsigned long messageId, unsigned long sn, char controllerId, char zoneId, int status);

    /**
     * Send Sequence Ended to the OPC.
     * @param messageId Original message ID
     * @param sn serial number of the original message
     * @param controllerId
     * @param deviceType
     * @param zoneId
     * @param status
     */
    void sendSeqEnded(unsigned long messageId, unsigned long sn, char controllerId, char zoneId, int status);

    void sendZoneStatusHandler(TmcMessageStruct &message, char boardId, char deviceId, E_ZoneType zoneType);

public:

    TmcMessageHandler();
    virtual ~TmcMessageHandler();

    /**
     * post a message to the message handler.
     */
    void handleMessage(TmcMessageStruct& message);

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
    portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);

    /**
     * The framework calls this function before the scheduler start. The default implementation simply return true.
     * Override this method to perform task specific hardware initialization that does not use FreeRTOS API.
     *
     * @return true if success, false otherwise.
     */
    virtual bool hardwareInit();

    void setBoardAddress(char ba) {m_boardAddress = ba;}

    /**
     * Reset the state of the server to its default state, so we can reconnect again from the OPC.
     */
    void reset();

    void instantiateZoneManager();


};

#endif /* TMCMESSAGEHANDLER_H_ */
