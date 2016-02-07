/*
 * CTcpConnector.cpp
 *
 *
 *  Created on: 8 бреб 2012
 *      Author: maora
 */

#include "CTcpConnectorSocket.h"
#include "netconf.h"
#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <Win32MissingDefines.h>
#define __root 
#else
#include <stm32f2xx.h>
#endif

//#include "StatusLed.h"
#include <string.h>
#include <logger.h>
#ifndef WIN32
#include "lwip/tcp.h"
#include "lwip/stats.h"
#include <lwip/sockets.h>
#endif
#include "lwipCallback.h"
#include <StatusLed.h>

#ifdef WIN32
#endif

//uint32_t ctcpconnector_stack_wm;
//CMutex CTcpConnector::m_tcpConnectorMutex;

//extern __IO uint8_t EthLinkStatus;
extern "C" struct netif xnetif; /* network interface structure */

#define M_DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 2 )
#define M_FULL_SEND_QUEUE_TIMEOUT 10000
#define M_SEND_QUEUE_DEPTH 20
#define M_RECEIVE_QUEUE_DEPTH 20

#if (LWIP_VER == 14)
#define GET_TCP_CONN_LAST_ERROR(conn) ERR_IS_FATAL(conn->last_err)
#else
#define GET_TCP_CONN_LAST_ERROR(conn) ERR_IS_FATAL(conn->err)
#endif

//struct T_TcpQueueItemu
//{
//    char data[150];
//    u16_t length;
//};

CTcpConnectorSocket::CTcpConnectorSocket(u16_t port)
{
    m_tcpSocket = NULL;
    m_newTcpSocket = NULL;
    m_port = port;
    m_lastSuccessfullSendTick = 0;
    m_deadConnection = false;
}

CTcpConnectorSocket::~CTcpConnectorSocket()
{
}

void CTcpConnectorSocket::closeConnection()
{
    T_TcpQueueItem queueItem;

    // empty the command queue:
//    T_CallbackQueueItem callbackQueueItem;
//    while (m_commandQueue.receive(&callbackQueueItem, 0) == pdPASS);
//    printf("close connection queue size: %d\n", m_commandQueue.messagesWaiting());

// close the net connection
    lwip_close(m_newTcpSocket);
    m_newTcpSocket = 0;

    // empty the send queue:
//    while (m_sendQueue.receive(&queueItem, 0) == pdPASS)
//        ;
    m_sendQueue.reset();
    m_receiveQueue.reset();

    m_deadConnection = false;
}

//int maxMessageSize = 0;
void CTcpConnectorSocket::run()
{
    int size;
    int optval;
    int sockError;
    int retVal;
    struct sockaddr_in address, remotehost;
    struct timeval tv;

    while (1)
    {
        /* create a TCP socket */
        if ((m_tcpSocket = lwip_socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
			retVal = lwip_getsockopt(m_tcpSocket, SOL_SOCKET, SO_ERROR, (char*)&sockError, (socklen_t*)&size);
			printf("can not create socket");
            return;
        }

        /* bind to port 80 at any interface */
        address.sin_family = AF_INET;
        address.sin_port = htons(m_port);
        address.sin_addr.s_addr = INADDR_ANY;

        // start listening for connections
        if (lwip_bind(m_tcpSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
        {
            printf("can not bind socket");
            return;
        }

        /* listen for incoming connections (TCP listen backlog = 5) */
        lwip_listen(m_tcpSocket, 2);

        size = sizeof(remotehost);

        T_TcpQueueItem queueItem;

        while (1)
        {
            m_newTcpSocket = lwip_accept(m_tcpSocket, (struct sockaddr *) &remotehost, (socklen_t *) &size);
#ifdef WIN32
            m_remoteIp.S_un.S_addr = remotehost.sin_addr.s_addr;
#else
            m_remoteIp.s_addr = remotehost.sin_addr.s_addr;
#endif

            // set a timeout interval on the socket:
//            tv.tv_sec = 10;
//            tv.tv_usec = 10000;
//            lwip_setsockopt(m_newTcpSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));
#ifndef WIN32
            optval = 10;
            lwip_setsockopt(m_newTcpSocket, SOL_SOCKET, SO_RCVTIMEO, (char*) &optval, sizeof(optval));
#else
            u_long iMode = 1;
            ioctlsocket(m_newTcpSocket, FIONBIO, &iMode);
#define MSG_DONTWAIT 0
#endif

            // turn on keep alive:
            optval = 1;
            lwip_setsockopt(m_newTcpSocket, SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval));

//            m_newTcpNetConn->recv_timeout = 10;
//            m_newTcpNetConn->pcb.tcp->so_options |= SOF_KEEPALIVE;

//            tcp_nagle_disable(m_newTcpNetConn->pcb.tcp);

//            led_dark(LABL_RESET);

            while (m_newTcpSocket)
            {
                retVal = lwip_getsockopt(m_newTcpSocket, SOL_SOCKET, SO_ERROR, (char*) &sockError, (socklen_t*) &size);
                while (retVal == 0 && sockError <= 0 && !m_deadConnection)
                {
                    do
                    {
                        retVal = lwip_recv(m_newTcpSocket, queueItem.data, sizeof(queueItem.data), MSG_DONTWAIT);
                        if (retVal > 0)
                        {
                            queueItem.length = retVal;
                            while (m_receiveQueue.send(&queueItem, 10) != pdPASS)
                            {
                            }
#if (LWIP_STATS == 1)
//                        printf("after receive pbufs: %d/%d\n", lwip_stats.memp[MEMP_PBUF_POOL].used,
//                                lwip_stats.memp[MEMP_PBUF_POOL].avail);

#endif
                            led_toggle(LABL_TCP);

                        }
                    } while (retVal > 0);

                    retVal = lwip_getsockopt(m_newTcpSocket, SOL_SOCKET, SO_ERROR, (char*) &sockError,
                            (socklen_t*) &size);
                    if (sockError == 11)
                        sockError = 0;
                    if (retVal == 0 && sockError <= 0)
                    {
                        while (m_sendQueue.receive(&queueItem, 10) == pdPASS)
                        {
                            retVal = lwip_send(m_newTcpSocket, queueItem.data, queueItem.length, 0);
                            if (m_deadConnection)
                            {
                                break;
                            }
//                            while (m_newTcpNetConn->err == ERR_INPROGRESS);
                            led_toggle(LABL_TCP);
                        }
                    }
                    // get the error for the next loop iteration.
                    retVal = lwip_getsockopt(m_newTcpSocket, SOL_SOCKET, SO_ERROR, (char*) &sockError,
                            (socklen_t*) &size);
                }
                // if there's an error on the socket, empty buffers:
                retVal = lwip_getsockopt(m_newTcpSocket, SOL_SOCKET, SO_ERROR, (char*) &sockError, (socklen_t*) &size);
                if (retVal != 0 || sockError > 0 || m_deadConnection)
                {
                    do
                    {
                        retVal = lwip_recv(m_newTcpSocket, queueItem.data, sizeof(queueItem.data), MSG_DONTWAIT);
                        if (retVal > 0)
                        {
                            queueItem.length = retVal;
                            while (m_receiveQueue.send(&queueItem, 10) != pdPASS)
                            {
                            }
#if (LWIP_STATS == 1)
//                        printf("after receive pbufs: %d/%d\n", lwip_stats.memp[MEMP_PBUF_POOL].used,
//                                lwip_stats.memp[MEMP_PBUF_POOL].avail);

#endif
                            led_toggle(LABL_TCP);

                        }
                    } while (retVal > 0);
                    closeConnection();
                }
            }
        }
    }
}

void CTcpConnectorSocket::send(const void* message, u16_t length)
{
    T_TcpQueueItem queueItem;
// if the TCP connection is not up, don't queue the message.
    if (m_newTcpSocket)
    {
        // copy the data to the queue for sending
        memcpy(&queueItem.data, message, length);
        queueItem.length = length;
        while (m_newTcpSocket != 0 && m_sendQueue.sendToBack(&queueItem, 200) != pdPASS)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "TCP Send Queue is full (%d messages).", m_sendQueue.messagesWaiting());
            while (m_newTcpSocket != 0 && m_sendQueue.messagesWaiting() != 0)
            {
                delay(10);
            }
            M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "TCP Send Queue was emptied.");
//            if ((getTickCount() - m_lastSuccessfullSendTick) >= M_FULL_SEND_QUEUE_TIMEOUT)
//                m_deadConnection = true;
        }
//        else
//        {
//            m_lastSuccessfullSendTick = getTickCount();
//        }
    }
}

portBASE_TYPE CTcpConnectorSocket::receive(void* message, u16_t &length, portBASE_TYPE ticksToWait)
{
    T_TcpQueueItem queueItem;
    portBASE_TYPE res;

    // if we still have data in the last netbuf, we don't want to get
    // the next netbuf from the socket.
    if ((res = m_receiveQueue.receive(&queueItem, ticksToWait)) != pdPASS)
        return res;

    memcpy(message, queueItem.data, queueItem.length);
    length = queueItem.length;

#if (LWIP_STATS == 1)
//    printf("after free pbufs: %d/%d\n", lwip_stats.memp[MEMP_PBUF_POOL].used, lwip_stats.memp[MEMP_PBUF_POOL].avail);
#endif

    return pdPASS;
}

portBASE_TYPE CTcpConnectorSocket::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
// check if the queues are valid - if they are it means that the target task already connected a handle to
// the queues. If not, create new queues.
    if (!m_receiveQueue.isValid())
    {
        m_receiveQueue.create(M_RECEIVE_QUEUE_DEPTH, sizeof(T_TcpQueueItem));
    }

    if (!m_sendQueue.isValid())
    {
        m_sendQueue.create(M_SEND_QUEUE_DEPTH, sizeof(T_TcpQueueItem));
    }

    return pdTRUE;
}

//portBASE_TYPE CTcpConnector::create(const portCHAR * const pcName, unsigned portSHORT usStackDepth)
//{
//    return CBaseConnector::create(pcName, usStackDepth, TCPIP_THREAD_PRIO - 1);
//}
//
void CTcpConnectorSocket::sendTaskControlFunc(void *pParams)
{
    static_cast<AManagedTask *>(pParams)->run();
}

in_addr& CTcpConnectorSocket::getRemoteIpAddress()
{
    return m_remoteIp;
}
