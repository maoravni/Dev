/*
 * CUdpConnector.cpp
 *
 *  Created on: 8 бреб 2012
 *      Author: maora
 */

#include "CUdpConnector.h"
#include "netconf.h"
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif
//#include "StatusLed.h"
#include "hmi.h"
#include <string.h>
#include <logger.h>
#include <lwip/tcp.h>
#include <lwip/stats.h>
#include "lwip/api.h"
#include <GnrlCfgDB.h>
#include <dg_in_ctrl.h>
#include <PscSubsystem.h>

#define DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 2 )

#if (LWIP_VER == 14)
#define GET_UDP_CONN_LAST_ERROR() ERR_IS_FATAL(m_udpNetConn->last_err)
#else
#define GET_UDP_CONN_LAST_ERROR() ERR_IS_FATAL(m_udpNetConn->err)
#endif


CUdpConnector::CUdpConnector(u16_t port)
{
    m_udpNetConn = NULL;
    m_port = port;
    IP4_ADDR(&m_ipAddress, 0, 0, 0, 0);
}

CUdpConnector::~CUdpConnector()
{
}

void CUdpConnector::run()
{
    delay(1000);
    while (1)
    {
        m_udpNetConn = netconn_new(NETCONN_UDP);
        //ip_addr multicastAddress;

        //U8 lowByte = Psc_GetLSBIpAddress();

        //IP4_ADDR(&multicastAddress, 239, 0, 0, lowByte);

        netbuf *pNetBuf = netbuf_new();

        netconn_bind(m_udpNetConn, IP_ADDR_ANY, m_port);
        //netconn_join_leave_group(m_udpNetConn, &multicastAddress, IP_ADDR_ANY, NETCONN_JOIN);
        void* pData;
//        u16_t len;
        T_UdpQueueItem queueItem;

        while (!GET_UDP_CONN_LAST_ERROR())
        {
//                    while ((pNetBuf = netconn_recv(m_newTcpNetConn)) != NULL)
//                    {
//                        while (m_receiveQueue.send(&pNetBuf, 10) != pdPASS)
//                        {
//                        }
//#if (LWIP_STATS == 1)
//                        printf("after receive pbufs: %d/%d\n", lwip_stats.memp[13].used, lwip_stats.memp[13].avail);
//#endif
//                        led_toggle (LABL_TCP);
//
//                    }
            while (m_sendQueue.receive(&queueItem, 10) == pdPASS)
            {
                if (m_ipAddress.addr == 0)
                    continue;
                pData = netbuf_alloc(pNetBuf, queueItem.length);
                if (pData != NULL)
                {
                    memcpy(pData, queueItem.data, queueItem.length);
                    netconn_sendto(m_udpNetConn, pNetBuf, &m_ipAddress, m_port);
                    //netconn_send(m_udpNetConn, pNetBuf);
                    netbuf_free(pNetBuf);
                    led_toggle(LABL_UDP);
                }
            }
            if (GET_UDP_CONN_LAST_ERROR())
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
        }
        netconn_close(m_udpNetConn);
        netconn_delete(m_udpNetConn);
        m_udpNetConn = NULL;
    }
}

void CUdpConnector::send(const void* message, u16_t length)
{
    T_UdpQueueItem queueItem;
// if the TCP connection is not up, don't queue the message.
    if (m_udpNetConn)
    {
        // copy the data to the queue for sending
        memcpy(&queueItem.data, message, length);
        queueItem.length = length;
        m_sendQueue.sendToBack(&queueItem, 50);
    }
}

portBASE_TYPE CUdpConnector::receive(void* message, u16_t &length, portBASE_TYPE ticksToWait)
{
    netbuf* pNetBuf;
    portBASE_TYPE res;

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

portBASE_TYPE CUdpConnector::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
// check if the queues are valid - if they are it means that the target task already connected a handle to
// the queues. If not, create new queues.
    if (!m_receiveQueue.isValid())
    {
        m_receiveQueue.create(MEMP_NUM_NETBUF - 1, sizeof(netbuf*));
    }

    if (!m_sendQueue.isValid())
    {
        m_sendQueue.create(2, sizeof(T_UdpQueueItem));
    }

    return pdTRUE;
}

portBASE_TYPE CUdpConnector::create(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority)
{
    return CBaseConnector::create(pcName, usStackDepth, uxPriority);
}

void CUdpConnector::sendTaskControlFunc(void *pParams)
{
    static_cast<AManagedTask *>(pParams)->run();
}

void CUdpConnector::setPort(unsigned short port)
{
    m_port = port;
}

void CUdpConnector::setIpAddress(ip_addr& ipAddress)
{
    memcpy(&m_ipAddress, &ipAddress, sizeof(ip_addr));
//    m_ipAddress = ipAddress;
}
