/*
 send* PscMasterServer.cpp
 *
 *  Created on: 4 Jul 2013
 *      Author: maora
 */

#include "PscMasterServer.h"
#include <string.h>
#include <logger.h>
#include <leds.h>
#include <PscSubsystem.h>
#include <StatusLed.h>

#define M_DEFAULT_TMC_SERVER_PORT 5000

#define M_PSC_MASTER_SERVER_STACK_SIZE 600

// the instance of the master server:
PscMasterServer PscMasterServer::s_instance;

PscMasterServer::PscMasterServer() /*:
 m_tcpConnector(M_DEFAULT_TMC_SERVER_PORT)*/
{
    m_pTcpConnector = NULL;
    reset();
}

PscMasterServer::~PscMasterServer()
{
}

void PscMasterServer::reset()
{
    m_currentTcpReceiveBuffer = 0;
    m_messagePosition = 0;
    memset(&m_tcpReceiveBuffer, 0, sizeof(T_TcpReceiveBuffer));

    m_lastRequestSerialNumber = 0;
    m_lastReplySerialNumber = 0;

    // TODO: use a configurable board ID (From EEPROM? From Dip Switches?)
    m_boardAddress = 5;

    m_isConnected = false;
    //TODO: impelement eeprom class to read and write configurations to memory.

}

bool PscMasterServer::receiveAndExtractDataFromTcp()
{
    u16_t length;
    // receive the data from the net into the receive buffer
    if (m_tcpReceiveBuffer.length > TCP_MSS * 2)
    {
        while (m_tcpReceiveBuffer.length > TCP_MSS * 2)
        {
            extractNextMessageFromBuffer();
        }
    }
    else
        extractNextMessageFromBuffer();

    if (m_pTcpConnector->receive(m_tcpReceiveBuffer.data + m_tcpReceiveBuffer.length, length, 50) == pdPASS)
    {
        m_tcpReceiveBuffer.length += length;

//        m_isConnected = true;
        // send a connected message to the message handler.
//        m_messageStruct.header.id.split.id = MSG_Connect;
//        PscMessageHandler::getInstance()->handleMessage(m_messageStruct);

        extractNextMessageFromBuffer();

        return true;
    }
    return false;
}

bool PscMasterServer::extractNextMessageFromBuffer()
{
    int bytesToCopy;

    if (m_tcpReceiveBuffer.length == 0)
        return false;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "m_tcpReceiveBuffer.length=%d", m_tcpReceiveBuffer.length);

    // the number of bytes remaining in the buffer.
    int bytesRemainingInBuffer = m_tcpReceiveBuffer.length - m_tcpReceiveBuffer.position;
    int bytesNeededToCopyForFullStruct = sizeof(m_messageStruct) - m_messagePosition;
    bytesToCopy =
            (bytesRemainingInBuffer < bytesNeededToCopyForFullStruct) ?
                    bytesRemainingInBuffer : bytesNeededToCopyForFullStruct;

    // copy the bytes to the message
    memcpy(((char*) &m_messageStruct), m_tcpReceiveBuffer.data + m_tcpReceiveBuffer.position, bytesToCopy);

    // validate the message
    if (validateMessage())
    {
        // handle the message
        PscMessageHandler::getInstance()->handleMessage(m_messageStruct);
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Bad Message ID=%x SN=%x", m_messageStruct.header.id.full,
                m_messageStruct.header.sn);

        //TODO send error to the OPC on bad message
        sendAck(0, 0, Psc_ControllerId, 0, E_AckStatus_InvalidMessage);
        m_tcpReceiveBuffer.position += 1;
        return false;
    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "old position: %d", m_tcpReceiveBuffer.position);
    m_tcpReceiveBuffer.position += m_messageStruct.header.length;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "new position: %d", m_tcpReceiveBuffer.position);

    // if the buffer is empty:
    if (m_tcpReceiveBuffer.position == m_tcpReceiveBuffer.length)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Buffer has been emptied");
        m_tcpReceiveBuffer.position = 0;
        m_tcpReceiveBuffer.length = 0;
        // return false as there are no more messages in the buffer.
        return false;
    }
    // if we have crossed into the second buffer 'page', we clear the first page and copy the rest of the buffer to the beginning.
    if (m_tcpReceiveBuffer.position >= TCP_MSS)
    {
        bytesRemainingInBuffer = m_tcpReceiveBuffer.length - m_tcpReceiveBuffer.position;
        memmove(m_tcpReceiveBuffer.data, m_tcpReceiveBuffer.data + m_tcpReceiveBuffer.position, bytesRemainingInBuffer);
        m_tcpReceiveBuffer.position = 0;
        m_tcpReceiveBuffer.length = bytesRemainingInBuffer;
        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Buffer has been shifted. New Length: %d", m_tcpReceiveBuffer.length);
    }

    return true;
}

void PscMasterServer::run()
{
    //CLogger::getInstance().setCurrentTaskMask(M_LOGGER_LEVEL_VERBOSE);

    led_light(LABL_TCP);
    //enterCritical();
    //vTaskSuspendAll();
    //while (1);

    for (;;)
    {
#ifdef USE_IWDG
// reset the watchdog:
        IWDG_ReloadCounter();
#endif

        while (m_pTcpConnector->isConnected() == false)
            delay(10);

        // TOOD: Find a better way of notifiying the server on a connection.
        if (m_isConnected == false)
        {
            m_isConnected = true;
            StatusLed::getInstance().setSpeed(E_AliveLedSpeed_Connected);
            CLogger::getInstance().setOutputUdpIpAddress(m_pTcpConnector->getRemoteIpAddress());
            m_messageStruct.header.id.split.id = MSG_Connect;
            PscMessageHandler::getInstance()->handleMessage(m_messageStruct);
        }

// receive the data from the net into the receive buffer
        if (!receiveAndExtractDataFromTcp())
        {
            if (m_isConnected && m_pTcpConnector->isConnected() == false)
            {
                // receive the data from the net into the receive buffer
                //todo: extract data without handling it.
                while (receiveAndExtractDataFromTcp())
                    ;

                // send a disconnect message to the message handler.
                m_messageStruct.header.id.split.id = MSG_Disconnect;
                PscMessageHandler::getInstance()->handleMessage(m_messageStruct);

                reset();
                m_isConnected = false;
                StatusLed::getInstance().setSpeed(E_AliveLedSpeed_Disconnected);

                led_light(LABL_TCP);
            }
        }
    }
}

portBASE_TYPE PscMasterServer::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    // TODO: Issue an error when task creation isn't successfull.
    portBASE_TYPE res;

    uint16_t cableId;

    while ((cableId = Psc_GetCableId()) == 0xffff);

    uint16_t port = M_DEFAULT_TMC_SERVER_PORT + cableId;
    m_pTcpConnector = new CTcpConnectorSocket(port);

    reset();

    // create the TCP connector
    if ((res = m_pTcpConnector->create("PscTcp", usStackDepth, M_TASK_PRIORITY_TCP_CONNECTOR_LOW_PRIORITY, M_TASK_PRIORITY_TCP_CONNECTOR_HIGH_PRIORITY)) != pdPASS)
        return res;

    CLogger::getInstance().enableOutputUdp(true);

    // create the message handler task:
    //PscMessageHandler::getInstance() = (PscMessageHandler::getInstance());
//    if ((res = PscMessageHandler::getInstance()->create("PscMsgHdl", M_MESSAGE_HANDLER_DEFAULT_STACK_DEPTH, uxPriority)) != pdPASS)
//        return res;
    PscMessageHandler::getInstance()->setBoardAddress(m_boardAddress);

    return res;
}

PscMasterServer& PscMasterServer::getInstance()
{
    if (!s_instance.isValid())
    {
// Create the task.
        s_instance.create("PscServer", M_PSC_MASTER_SERVER_STACK_SIZE, M_TASK_PRIORITY_PSC_MASTER_SERVER);
    }

    return s_instance;
}

bool PscMasterServer::validateMessage()
{
    // check that the source is 0x1 and the dest is 0x5.
    if (m_messageStruct.header.id.split.src != M_OPC_ICD_ID)
        return false;
    if (m_messageStruct.header.id.split.dst != M_PSC_ICD_ID)
        return false;

    if ((m_messageStruct.header.sn & 1) == 0)
        return false;

// save the serial number for future references:
    m_lastRequestSerialNumber = m_messageStruct.header.sn;

    return true;
}

void PscMasterServer::sendMessage(PscMessageStruct& message)
{
    if (!m_isConnected)
        return;

    // first assign the correct serial number to the message:
    enterCritical();

    m_lastReplySerialNumber += 2;
    message.header.sn = m_lastReplySerialNumber;

    exitCritical();

    message.header.id.split.src = m_boardAddress;
    message.header.id.split.dst = 0x1;
    message.header.crc = 0;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "sending message: message ID=%x length=%d sn=%d", message.header.id.split.id,
            message.header.length, message.header.sn);

    m_pTcpConnector->send(&message, message.header.length);
}

void PscMasterServer::sendLogMessage(std::string &str)
{
    return;

    /*
     TmcMessageStruct tmcMsg;

     if (!m_isConnected)
     return;

     // first assign the correct serial number to the message:
     enterCritical();

     m_lastReplySerialNumber += 2;
     tmcMsg.header.sn = m_lastReplySerialNumber;

     exitCritical();

     tmcMsg.header.id.split.id = 0x127;
     tmcMsg.payload.pSSLogMsg.arrayLength = str.length();
     tmcMsg.header.id.split.src = m_boardAddress;
     tmcMsg.header.id.split.dst = 0x1;
     tmcMsg.header.length = tmcMsg.payload.pSSLogMsg.arrayLength+sizeof(tmcMsg.header) + sizeof(tmcMsg.payload.pSSLogMsg.arrayLength);

     //    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,
     //            "sending message: message ID=%x length=%d sn=%d", message.header.id.split.id, message.header.length, message.header.sn);

     m_tcpConnector.send(&tmcMsg, sizeof(tmcMsg.header)+sizeof(tmcMsg.payload.pSSLogMsg.arrayLength));
     m_tcpConnector.send(str.c_str(), str.length());
     */
}

//void PscMasterServer::sendMonitorNotificationMessage(uint16_t pssId, E_DataTypes dataType, int16_t value)
//{
//    // static instance for the master server.
//    PscMessageStruct replyMessage;
//
////    replyMessage.header.id.split.id = MSG_MonitorDataNotification;
////    replyMessage.payload.pSSMonitoringDataNotificationMsg.cableId = Psc_ControllerId;
////    replyMessage.payload.pSSMonitoringDataNotificationMsg.zoneId = (uint8_t)pssId;
////    replyMessage.payload.pSSMonitoringDataNotificationMsg.dataSpecification = dataType;
////    replyMessage.payload.pSSMonitoringDataNotificationMsg.dataLength = 2;
////    *((U32*) replyMessage.payload.pSSMonitoringDataNotificationMsg.data) = value;
////    replyMessage.header.length = sizeof(replyMessage.header)
////            + sizeof(replyMessage.payload.pSSMonitoringDataNotificationMsg)
////            - sizeof(replyMessage.payload.pSSMonitoringDataNotificationMsg.data) + 2;
//
////    sendMessage(replyMessage);
//}

void PscMasterServer::disconnectDetected()
{
    m_messageStruct.header.id.split.id = MSG_Disconnect;
    PscMessageHandler::getInstance()->handleMessage(m_messageStruct);
}

void PscMasterServer::killConnection()
{
    m_pTcpConnector->killConnection();
}

void PscMasterServer::sendSeqEnded(unsigned long messageId, unsigned long sn, uint16_t boardId, uint16_t pssId,
        E_SeqEndedStatus status)
{
    if (sn == 0)
        return;
    //TODO: make sure that only one thread is calling this function, since the reply is static.
    PscMessageStruct reply;
    reply.header.id.split.src = m_boardAddress;
    reply.header.id.split.dst = M_OPC_ICD_ID;
    reply.header.id.split.id = MSG_SeqEnded;
    reply.header.length = sizeof(reply.header) + sizeof(reply.payload.pSSSeqEndedMsg);
    // TODO: Fix the hardcoded message ID.
    reply.payload.pSSSeqEndedMsg.originalRequestMsgId = 0x01050000 | messageId;
    reply.payload.pSSSeqEndedMsg.originalRequestSN = sn;
    reply.payload.pSSSeqEndedMsg.cableId = boardId;
    reply.payload.pSSSeqEndedMsg.pssId = pssId;
    reply.payload.pSSSeqEndedMsg.status = status;

    int logLevel = M_LOGGER_LEVEL_DEBUG;
    if (status != E_AckStatus_Success)
        logLevel = M_LOGGER_LEVEL_WARNING;

    M_LOGGER_LOGF(logLevel, "Sequence Ended ID 0x%x, controller=%d, zone={[PSSID:%d]}, status=%d sn=%d",
            messageId, boardId, pssId, status, sn);
    sendMessage(reply);
}

void PscMasterServer::sendAck(unsigned long messageId, unsigned long sn, uint16_t boardId, uint16_t pssId,
        E_AckStatus status)
{

    if (sn == 0)
        return;

    //TODO: make sure that only one thread is calling this function, since the reply is static.
    PscMessageStruct reply;
    reply.header.id.split.src = m_boardAddress;
    reply.header.id.split.dst = M_OPC_ICD_ID;
    reply.header.id.split.id = MSG_Ack;
    reply.header.length = sizeof(reply.header) + sizeof(reply.payload.pSSAckMsg);
    reply.payload.pSSAckMsg.originalRequestMsgId = messageId;
    reply.payload.pSSAckMsg.originalRequestSN = sn;
    reply.payload.pSSAckMsg.cableId = boardId;
    reply.payload.pSSAckMsg.pssId = pssId;
    reply.payload.pSSAckMsg.status = status;

    int logLevel = M_LOGGER_LEVEL_DEBUG;
    if (status != E_AckStatus_Success)
        logLevel = M_LOGGER_LEVEL_ERROR;

    M_LOGGER_LOGF(logLevel, "Ack Message ID 0x%x, controller=%d, zone={[PSSID:%d]}, status=%d sn=%d",
            messageId, boardId, pssId, status, sn);

    sendMessage(reply);
}

void PscMasterServer::sendError(uint16_t boardId, uint16_t pssId, uint16_t secondaryPssId, uint32_t errors)
{
    PscMessageStruct reply;
    PSSErrorNotificationWithSecondaryMsg &payload = reply.payload.pSSErrorNotificationWithSecondaryMsg;

    //TODO: Check if the board address should be hard-coded.
    reply.header.id.split.src = 0x5;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_ErrorNotificationWithSecondary;
    reply.header.length = sizeof(reply.header) + sizeof(payload);
    payload.cableId = boardId;
    payload.pssId = pssId;
    payload.secondaryPssId = secondaryPssId;
    payload.errors = errors;

    int logLevel = M_LOGGER_LEVEL_ERROR;
    if (errors == 0)
        logLevel = M_LOGGER_LEVEL_DEBUG;

    M_LOGGER_LOGF(logLevel, "Error notification sent: controller=%d, pssId={[PSSID:%d]}, secondary={[PSSID:%d]}, error=%x", boardId,
            pssId, secondaryPssId, errors);

    sendMessage(reply);
}

void PscMasterServer::sendWarning(uint16_t boardId, uint16_t pssId, uint16_t secondaryPssId, uint32_t warnings)
{
    PscMessageStruct reply;
    PSSWarningNotificationWithSecondaryMsg &payload = reply.payload.pSSWarningNotificationWithSecondaryMsg;

    //TODO: Check if the board address should be hard-coded.
    reply.header.id.split.src = 0x5;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_WarningNotificationWithSecondary;
    reply.header.length = sizeof(reply.header) + sizeof(payload);
    payload.cableId = boardId;
    payload.pssId = pssId;
    payload.secondaryPssId = secondaryPssId;
    payload.warnings = warnings;

    int logLevel = M_LOGGER_LEVEL_WARNING;
    if (warnings == 0)
        logLevel = M_LOGGER_LEVEL_DEBUG;

    M_LOGGER_LOGF(logLevel, "Warning notification sent: controller=%d, pssId={[PSSID:%d]}, secondary={[PSSID:%d]}, error=%x",
            boardId, pssId, secondaryPssId, warnings);

    sendMessage(reply);
}

