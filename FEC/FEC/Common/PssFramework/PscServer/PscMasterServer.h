/*
 * PscMasterServer.h
 *
 *  Created on: 4 Jul 2013
 *      Author: maora
 */

#ifndef PSCMASTERSERVER_H_
#define PSCMASTERSERVER_H_

#include <stdint.h>
#include <CTcpConnector.h>
#include <lwipopts.h>
#include "PscMessageStructs.h"
#include "PscMessageHandler.h"

struct T_TcpReceiveBuffer
{
    char data[TCP_MSS * 3];
    u16_t length;
    u16_t position;
};

#define M_PSC_ICD_ID 0x5
#define M_OPC_ICD_ID 0x1

/**
 * This is the TCP/IP server class for the master FEC board.
 */
class PscMasterServer: public AManagedTask
{
private:
    static PscMasterServer s_instance; //!< Instance of the master server singleton.

    CTcpConnector *m_pTcpConnector; //!< TCP Connector for the server

    PscMessageStruct m_messageStruct; //!< The message going to/from the OPC.
    unsigned int m_lastRequestSerialNumber; //!< The last serial number of the request from the OPC.
    unsigned int m_lastReplySerialNumber; //!< The last serial number of the reply to the OPC.

    PscMessageHandler *m_messageHandler;

    T_TcpReceiveBuffer m_tcpReceiveBuffer;
    int m_currentTcpReceiveBuffer;
    int m_messagePosition; //!< holds the last filled position in the message, or 0 if the message is empty.

    char m_boardAddress;

    bool m_isConnected;

    CQueue m_receiveQueue;

    PscMasterServer();
    bool receiveAndExtractDataFromTcp();

public:
    virtual ~PscMasterServer();

    static PscMasterServer& getInstance();

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
    void sendMessage(PscMessageStruct &message);
    void sendLogMessage(std::string &str);
//    void sendMonitorNotificationMessage(uint16_t pssId, E_DataTypes dataType, int16_t value);

    /**
     * reset all members of the master server class to default values.
     */
    void reset();

    /**
     * A disconnection from the SUM was detected - need to notify the message handler.
     */
    void disconnectDetected();

    /**
     * Force kill the connection
     */
    void killConnection();

    /**
     * Send sequence ended message to the OPC.
     * @param messageId
     * @param sn
     * @param boardId
     * @param pssId
     * @param status
     */
    void sendSeqEnded(unsigned long messageId, unsigned long sn, uint16_t boardId, uint16_t pssId, E_SeqEndedStatus status);

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

    void sendError(uint16_t boardId, uint16_t pssId, uint16_t secondaryPssId, uint32_t errors);
    void sendWarning(uint16_t boardId, uint16_t pssId, uint16_t secondaryPssId, uint32_t warnings);

};

#endif /* PSCMASTERSERVER_H_ */
