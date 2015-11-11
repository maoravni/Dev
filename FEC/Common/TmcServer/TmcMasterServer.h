/*
 * TmcMasterServer.h
 *
 *  Created on: 14 бреб 2012
 *      Author: maora
 */

#ifndef TMCMASTERSERVER_H_
#define TMCMASTERSERVER_H_

#include <AManagedTask.h>
#include <CTcpConnector.h>
#include "MessageStructs.h"
#include "TmcMessageHandler.h"
#include "PeriodicMonitorTask.h"
#include <string>

struct T_TcpReceiveBuffer
{
    char data[TCP_MSS*3];
    u16_t length;
    u16_t position;
};

/**
 * This is the TCP/IP server class for the master FEC board.
 */
class TmcMasterServer: public AManagedTask
{
private:
    static TmcMasterServer s_instance; //!< Instance of the master server singleton.

    CTcpConnector m_tcpConnector; //!< TCP Connector for the server

    TmcMessageStruct m_tmcMessageStruct; //!< The message going to/from the OPC.
    unsigned int m_lastRequestSerialNumber; //!< The last serial number of the request from the OPC.
    unsigned int m_lastReplySerialNumber; //!< The last serial number of the reply to the OPC.

    TmcMessageHandler m_tmcMessageHandler;

    T_TcpReceiveBuffer m_tcpReceiveBuffer;
    int m_currentTcpReceiveBuffer;
    int m_messagePosition; //!< holds the last filled position in the message, or 0 if the message is empty.

    char m_boardAddress;

    bool m_isConnected;

    CQueue m_receiveQueue;

    TmcMasterServer();
    bool receiveAndExtractDataFromTcp();

public:
    virtual ~TmcMasterServer();

    static TmcMasterServer& getInstance();

    /**
     * Task control function.
     */
    void run();

    /**
     * The framework calls this function before the task is created to give a chance to a managed task to perform its
     * own initialization code and stop the task creation process if needed.
     *
     * @param pcName specifies the task name.
     * @param usStackDepth specifies the task stack depth.
     * @param uxPriority specifies the task priority.
     *
     * @return pdTRUE if success, pdFALSE otherwise. If the method return pdFALSE the task creation process
     * is stopped and no FreeRTOS resource are allocated.
     */
    portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
            unsigned portBASE_TYPE uxPriority);

    /**
     * Validate the message to/from the OPC.
     * @return true if the message format is ok.
     */
    bool validateMessage();

    /**
     * Peel the messages from the buffer.
     * This function goes over the buffer received from the ethernet, and processes the messages one by one
     * until the end of the buffer is reached.
     */
    //void extractMessagesFromBuffer();

    /**
     * extract the first message in the buffer. Shift the buffer to the left when the first TCP MSS has been cleared.
     * @return true if there are more messages in the buffer.
     */
    bool extractNextMessageFromBuffer();

    /**
     * Send a message back to the server.
     * @param message the message to send to the server.
     */
    void sendMessage(TmcMessageStruct &message);
    void sendLogMessage(std::string &str);

    /**
     * reset all members of the master server class to default values.
     */
    void reset();

    /**
     * A disconnection from the SUM was detected - need to notify the message handler.
     */
    void disconnectDetected();
};

#endif /* TMCMASTERSERVER_H_ */
