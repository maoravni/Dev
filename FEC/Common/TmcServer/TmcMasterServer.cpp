/*
 * TmcMasterServer.cpp
 *
 *  Created on: 14 бреб 2012
 *      Author: maora
 */

#include "TmcMasterServer.h"
//#include "StatusLed.h"
#include "leds.h"
#include <string.h>
#include "logger.h"

#define M_DEFAULT_TMC_SERVER_PORT 523

uint32_t tmcmasterserver_stack_wm;
// the instance of the master server:
TmcMasterServer TmcMasterServer::s_instance;

TmcMasterServer::TmcMasterServer() :
        m_tcpConnector(M_DEFAULT_TMC_SERVER_PORT)
{
    reset();
}

TmcMasterServer::~TmcMasterServer()
{
}

void TmcMasterServer::reset()
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

bool TmcMasterServer::receiveAndExtractDataFromTcp()
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

    if (m_tcpConnector.receive(m_tcpReceiveBuffer.data + m_tcpReceiveBuffer.length, length, 50) == pdPASS)
    {
        m_tcpReceiveBuffer.length += length;

        m_isConnected = true;

        extractNextMessageFromBuffer();

        return true;
    }
    return false;
}

bool TmcMasterServer::extractNextMessageFromBuffer()
{
    int bytesToCopy;

    if (m_tcpReceiveBuffer.length == 0)
        return false;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "m_tcpReceiveBuffer.length=%d", m_tcpReceiveBuffer.length);

    // the number of bytes remaining in the buffer.
    int bytesRemainingInBuffer = m_tcpReceiveBuffer.length - m_tcpReceiveBuffer.position;
    int bytesNeededToCopyForFullStruct = sizeof(m_tmcMessageStruct) - m_messagePosition;
    bytesToCopy =
            (bytesRemainingInBuffer < bytesNeededToCopyForFullStruct) ?
                    bytesRemainingInBuffer : bytesNeededToCopyForFullStruct;

    // copy the bytes to the message
    memcpy(((char*) &m_tmcMessageStruct), m_tcpReceiveBuffer.data + m_tcpReceiveBuffer.position, bytesToCopy);

    // validate the message
    if (validateMessage())
    {
        // handle the message
        m_tmcMessageHandler.handleMessage(m_tmcMessageStruct);
    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Bad Message ID=%x SN=%x", m_tmcMessageStruct.header.id.full,
                m_tmcMessageStruct.header.sn);

        //TODO send error to the OPC on bad message
    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "old position: %d\n", m_tcpReceiveBuffer.position);
    m_tcpReceiveBuffer.position += m_tmcMessageStruct.header.length;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "new position: %d\n", m_tcpReceiveBuffer.position);

    // if the buffer is empty:
    if (m_tcpReceiveBuffer.position == m_tcpReceiveBuffer.length)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Buffer has been emptied");
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
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Buffer has been shifted. New Length: %d", m_tcpReceiveBuffer.length);
    }

    return true;
}

void TmcMasterServer::run()
{
    //CLogger::getInstance().setCurrentTaskMask(M_LOGGER_LEVEL_VERBOSE);

    led_light(LABL_TCP);
    //enterCritical();
    //vTaskSuspendAll();
    //while (1);

    for (;;)
    {
// reset the watchdog:
        IWDG_ReloadCounter();

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
        tmcmasterserver_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif
// receive the data from the net into the receive buffer
        if (!receiveAndExtractDataFromTcp())
        {
            if (m_isConnected && m_tcpConnector.isConnected() == false)
            {
                // receive the data from the net into the receive buffer
                //todo: extract data without handling it.
                while (receiveAndExtractDataFromTcp())
                    ;

                reset();
                m_isConnected = false;

                // send a disconnect message to the message handler.
                m_tmcMessageStruct.header.id.split.id = MSG_Disconnect;
                m_tmcMessageHandler.handleMessage(m_tmcMessageStruct);

                led_light(LABL_TCP);
            }
        }
    }
}

portBASE_TYPE TmcMasterServer::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    // TODO: Issue an error when task creation isn't successfull.
    portBASE_TYPE res;

    reset();

    // create the TCP connector
    if ((res = m_tcpConnector.create("TmcTcp", usStackDepth)) != pdPASS)
        return res;

    // create the message handler task:
    m_tmcMessageHandler.setBoardAddress(m_boardAddress);
    if ((res = m_tmcMessageHandler.create("TmcMsgHdl", configMINIMAL_STACK_SIZE * 2, uxPriority - 1)) != pdPASS)
        return res;

    return res;
}

TmcMasterServer& TmcMasterServer::getInstance()
{
    if (!s_instance.isValid())
    {
// Create the task.
        s_instance.create("TmcServer", configMINIMAL_STACK_SIZE + 50, 4);
    }

    return s_instance;
}

bool TmcMasterServer::validateMessage()
{
    // check that the source is 0x1 and the dest is 0x5.
    if (m_tmcMessageStruct.header.id.split.src != 0x1)
        return false;
    if (m_tmcMessageStruct.header.id.split.dst != 0x5)
        return false;

    // TODO: check a case where this is not the first message received, and the last request is not 0, yet the OPC sends a good message.
    // check that the serial number is as expected:
//    if (m_lastRequestSerialNumber == 0)
//        m_lastRequestSerialNumber = m_tmcMessageStruct.header.sn;
//    else if ((m_lastRequestSerialNumber + 2) != m_tmcMessageStruct.header.sn)
    if ((m_tmcMessageStruct.header.sn & 1) == 0)
        return false;
    // check that the serial number is odd:
//    if ((m_lastRequestSerialNumber & 1) == 0)
//        return false;

// save the serial number for future references:
    m_lastRequestSerialNumber = m_tmcMessageStruct.header.sn;

    return true;
}

/*void TmcMasterServer::extractMessagesFromBuffer()
 {
 //TODO: issue an error when the message contains previous transmission data, but the new buffer contains less bytes than the message requires.
 int bytesToCopy;

 M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "m_tcpReceiveBuffer.length=%d", m_tcpReceiveBuffer.length);

 // check if the remaining length of the buffer is bigger than the size of the message struct:
 while (m_tcpReceiveBuffer.length > m_tcpReceiveBuffer.position)
 {
 M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "before copy: m_messagePosition=%d", m_messagePosition);

 // the number of bytes remaining in the buffer.
 int bytesRemainingInBuffer = m_tcpReceiveBuffer.length - m_tcpReceiveBuffer.position;
 int bytesNeededToCopyForFullStruct = sizeof(m_tmcMessageStruct) - m_messagePosition;
 bytesToCopy =
 (bytesRemainingInBuffer < bytesNeededToCopyForFullStruct) ?
 bytesRemainingInBuffer : bytesNeededToCopyForFullStruct;

 // if the beginning of the message is filler, messagePosition is not zero - the memcpy will fill the rest of the
 // message.
 M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "bytesToCopy=%d m_tcpReceiveBuffer.position=%d", bytesToCopy,
 m_tcpReceiveBuffer.position);

 memcpy(((char*) &m_tmcMessageStruct + m_messagePosition), m_tcpReceiveBuffer.data + m_tcpReceiveBuffer.position,
 bytesToCopy);

 // check that we have the full size of the message:
 // first check if the (bytes copied including residue from previous iteraion) is enough to get the length:
 // (length is at position 4, size 2),
 // or the bytes copied are less than the complete message length:
 if ((bytesToCopy + m_messagePosition) < 6
 || (bytesToCopy + m_messagePosition) < m_tmcMessageStruct.header.length)
 {
 M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "break: btc=%d, mp=%d, mhl=%d", bytesToCopy, m_messagePosition,
 m_tmcMessageStruct.header.length);
 break;
 }

 M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "after copy: m_messagePosition=%d", m_messagePosition);
 m_tcpReceiveBuffer.position += (m_tmcMessageStruct.header.length - m_messagePosition);
 m_messagePosition = 0;

 M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,
 "m_tcpReceiveBuffer.position=%d .length=%d m_tmcMessageStruct.header.length=%d",
 m_tcpReceiveBuffer.position, m_tcpReceiveBuffer.length, m_tmcMessageStruct.header.length);

 // validate the message
 if (validateMessage())
 {
 // handle the message
 m_tmcMessageHandler.handleMessage(m_tmcMessageStruct);
 }
 else
 {
 M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Bad Message ID=%x SN=%x", m_tmcMessageStruct.header.id.full,
 m_tmcMessageStruct.header.sn);

 //TODO send error to the OPC on bad message
 }
 }

 // if we're here no full messages are left in the buffer.
 // copy the remaining bytes to the message struct:
 bytesToCopy = (m_tcpReceiveBuffer.length - m_tcpReceiveBuffer.position);
 if (bytesToCopy > 0)
 {
 memcpy(&m_tmcMessageStruct + m_messagePosition, m_tcpReceiveBuffer.data + m_tcpReceiveBuffer.position,
 bytesToCopy);
 m_messagePosition += bytesToCopy;
 M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "end of function: m_messagePosition=%d", m_messagePosition);
 }
 }*/

void TmcMasterServer::sendMessage(TmcMessageStruct& message)
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

//    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,
//            "sending message: message ID=%x length=%d sn=%d", message.header.id.split.id, message.header.length, message.header.sn);

    m_tcpConnector.send(&message, message.header.length);
}

void TmcMasterServer::sendLogMessage(std::string &str)
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

void TmcMasterServer::disconnectDetected()
{
    m_tmcMessageStruct.header.id.split.id = MSG_Disconnect;
    m_tmcMessageHandler.handleMessage(m_tmcMessageStruct);
}
