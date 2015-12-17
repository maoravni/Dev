/*
 * CUdpConnectorSocket.cpp
 *
 *  Created on: 8 бреб 2012
 *      Author: maora
 */

#include "CUdpConnectorSocket.h"
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
#include <lwip/tcp.h>
#include <lwip/stats.h>
#include "lwip/api.h"
#include <lwip/sockets.h>
#endif
#include <dg_in_ctrl.h>
#include <PscSubsystem.h>
#include <leds.h>

#define DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 2 )

#if (LWIP_VER == 14)
#define GET_UDP_CONN_LAST_ERROR() ERR_IS_FATAL(m_udpNetConn->last_err)
#else
#define GET_UDP_CONN_LAST_ERROR() ERR_IS_FATAL(m_udpNetConn->err)
#endif

CUdpConnectorSocket::CUdpConnectorSocket(u16_t port)
{
    m_udpSocket = 0;
    m_port = port;
#ifdef WIN32
    m_ipAddress.S_un.S_addr = 0;
#else
    m_ipAddress.s_addr = 0;
#endif
}

CUdpConnectorSocket::~CUdpConnectorSocket()
{
}

void CUdpConnectorSocket::run()
{
    struct sockaddr_in address, remotehost;
    int sockError;
    int retVal;
    int size;

    delay(1000);
    while (1)
    {
        m_udpSocket = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        address.sin_family = AF_INET;
        address.sin_port = htons(m_port);
        address.sin_addr.s_addr = INADDR_ANY;

        if (lwip_bind(m_udpSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
        {
            printf("can not bind socket");
            return;
        }

        void* pData;
//        u16_t len;
        T_UdpQueueItem queueItem;

        retVal = lwip_getsockopt(m_udpSocket, SOL_SOCKET, SO_ERROR, (char*) &sockError, (socklen_t*) &size);
        while (retVal == 0 && sockError <= 0)
        {
            while (m_sendQueue.receive(&queueItem, 10) == pdPASS)
            {
                if (m_ipAddress.s_addr == 0)
                    continue;

#ifndef WIN32
                remotehost.sin_len = sizeof(remotehost);
#endif
                remotehost.sin_family = AF_INET;
                remotehost.sin_addr.s_addr = m_ipAddress.s_addr;
                remotehost.sin_port = htons(m_port);

                lwip_sendto(m_udpSocket, queueItem.data, queueItem.length, 0, (struct sockaddr*)&remotehost, sizeof(remotehost));

                led_toggle(LABL_UDP);
            }
            retVal = lwip_getsockopt(m_udpSocket, SOL_SOCKET, SO_ERROR, (char*) &sockError, (socklen_t*) &size);
            if (retVal != 0 || sockError > 0)
            {
//                    while ((pNetBuf = netconn_recv(m_newTcpNetConn)) != NULL)
//                    {
//                        while (m_receiveQueue.send(&pNetBuf, 10) != pdPASS)
//                        {
//                        }
//#if (LWIP_STATS == 1)
//                        printf("after receive pbufs (err): %d/%d\n", lwip_stats.memp[13].used,
//                                lwip_stats.memp[13].avail);
//#endif
//                        led_toggle(LABL_TCP);
//                    }
            }
            // get the error for the next iteration.
            retVal = lwip_getsockopt(m_udpSocket, SOL_SOCKET, SO_ERROR, (char*) &sockError, (socklen_t*) &size);
        }
        lwip_close(m_udpSocket);
        m_udpSocket = NULL;
    }
}

portBASE_TYPE CUdpConnectorSocket::send(const void* message, u16_t length)
{
    T_UdpQueueItem queueItem;
// if the TCP connection is not up, don't queue the message.
    if (m_udpSocket)
    {
        // copy the data to the queue for sending
        memcpy(&queueItem.data, message, length);
        queueItem.length = length;
        return m_sendQueue.sendToBack(&queueItem, 50);
    }
}

portBASE_TYPE CUdpConnectorSocket::receive(void* message, u16_t &length, portBASE_TYPE ticksToWait)
{
/*
    netbuf* pNetBuf;
    portBASE_TYPE res;
*/

    /*
     // the next netbuf from the socket.
     if (m_lastNetBuf == NULL)
     {
     if ((res = m_receiveQueue.receive(&pNetBuf, ticksToWait)) != pdPASS)
     return res;
     m_lastNetBuf = pNetBuf;
     }

     // copy the current netbuf to the receiver end.
     length = netbuf_copy(m_lastNetBuf, message, PBUF_POOL_BUFSIZE);

     // try to move to the next buffer
     // if netbuf_next returned -1 it means there is no next part,
     // so we can delete the netbuf.
     if (netbuf_next(m_lastNetBuf) == -1)
     {
     netbuf_delete(m_lastNetBuf);
     m_lastNetBuf = NULL;
     }
     else
     {
     puts("multiple pbufs in netbuf");
     }

     #if (LWIP_STATS == 1)
     printf("after free pbufs: %d/%d\n", lwip_stats.memp[13].used, lwip_stats.memp[13].avail);
     #endif

     */
    return pdPASS;
}

portBASE_TYPE CUdpConnectorSocket::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
// check if the queues are valid - if they are it means that the target task already connected a handle to
// the queues. If not, create new queues.
/*
    if (!m_receiveQueue.isValid())
    {
        m_receiveQueue.create(MEMP_NUM_NETBUF - 1, sizeof(netbuf*));
    }
*/

    if (!m_sendQueue.isValid())
    {
        m_sendQueue.create(16, sizeof(T_UdpQueueItem));
    }

    return pdTRUE;
}

portBASE_TYPE CUdpConnectorSocket::create(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    return CBaseConnector::create(pcName, usStackDepth, uxPriority);
}

void CUdpConnectorSocket::sendTaskControlFunc(void *pParams)
{
    static_cast<AManagedTask *>(pParams)->run();
}

void CUdpConnectorSocket::setPort(unsigned short port)
{
    m_port = port;
}

void CUdpConnectorSocket::setIpAddress(in_addr& ipAddress)
{
    memcpy(&m_ipAddress, &ipAddress, sizeof(in_addr));
//    m_ipAddress = ipAddress;
}
