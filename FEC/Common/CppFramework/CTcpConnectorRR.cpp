/*
 * CTcpConnector.cpp
 *
 *
 *  Created on: 8 бреб 2012
 *      Author: maora
 */

#include "CTcpConnectorRR.h"
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
#include "lwip/tcp.h"
#include "lwip/stats.h"
#include <lwip/sockets.h>
#include <StatusLed.h>
#include "lwipCallback.h"

uint32_t ctcpconnector_stack_wm;
//CMutex CTcpConnector::m_tcpConnectorMutex;

//extern __IO uint8_t EthLinkStatus;
extern "C" struct netif xnetif; /* network interface structure */

#define M_DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 2 )
#define M_FULL_SEND_QUEUE_TIMEOUT 10000
#define M_COMMAND_QUEUE_DEPTH MEMP_NUM_NETBUF - 1

#if (LWIP_VER == 14)
#define GET_TCP_CONN_LAST_ERROR(conn) ERR_IS_FATAL(conn->last_err)
#else
#define GET_TCP_CONN_LAST_ERROR(conn) ERR_IS_FATAL(conn->err)
#endif

//struct T_TcpQueueItem
//{
//    char data[150];
//    u16_t length;
//};

CTcpConnectorRR::CTcpConnectorRR(u16_t port)
{
    m_tcpNetConn = NULL;
    m_newTcpNetConn = NULL;
    m_port = port;
    m_lastNetBuf = NULL;
    m_lastSuccessfullSendTick = 0;
    m_deadConnection = false;
}

CTcpConnectorRR::~CTcpConnectorRR()
{
}

void CTcpConnectorRR::closeConnection()
{
    T_TcpQueueItem queueItem;

    // empty the command queue:
//    T_CallbackQueueItem callbackQueueItem;
//    while (m_commandQueue.receive(&callbackQueueItem, 0) == pdPASS);
//    printf("close connection queue size: %d\n", m_commandQueue.messagesWaiting());

// close the net connection
    netconn_close(m_newTcpNetConn);
    netconn_delete(m_newTcpNetConn);
    m_newTcpNetConn = NULL;

    // empty the send queue:
//    while (m_sendQueue.receive(&queueItem, 0) == pdPASS)
//        ;
    m_sendQueue.reset();
    m_receiveQueue.reset();

    m_deadConnection = false;
}

int maxMessageSize = 0;
void CTcpConnectorRR::run()
{
    int netError;
    while (1)
    {
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
        ctcpconnector_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif
        m_tcpNetConn = netconn_new(NETCONN_TCP);

        // start listening for connections
        netconn_bind(m_tcpNetConn, IP_ADDR_ANY, m_port);
        netconn_listen(m_tcpNetConn);

        netbuf *pNetBuf = NULL;
//        void* pData;
//        u16_t len;
        T_TcpQueueItem queueItem;

        while (1)
        {

#if LWIP_VER == 14
            netconn_accept(m_tcpNetConn, &m_newTcpNetConn);
#else
            m_newTcpNetConn = netconn_accept(m_tcpNetConn);
#endif
            m_newTcpNetConn->recv_timeout = 10;
            m_newTcpNetConn->pcb.tcp->so_options |= SOF_KEEPALIVE;

//            tcp_nagle_disable(m_newTcpNetConn->pcb.tcp);

//            led_dark(LABL_RESET);

            while (m_newTcpNetConn)
            {
                while (!GET_TCP_CONN_LAST_ERROR(m_newTcpNetConn) && !m_deadConnection)
                {
#if LWIP_VER == 14
                    netconn_recv(m_newTcpNetConn, &pNetBuf);
#else
                    pNetBuf = netconn_recv(m_newTcpNetConn);
#endif
//                    while (pNetBuf != NULL)
                    if (pNetBuf != NULL)
                    {
                        while (m_receiveQueue.send(&pNetBuf, 10) != pdPASS)
                        {
                        }
#if (LWIP_STATS == 1)
//                        printf("after receive pbufs: %d/%d\n", lwip_stats.memp[MEMP_PBUF_POOL].used,
//                                lwip_stats.memp[MEMP_PBUF_POOL].avail);

#endif
                        led_toggle(LABL_TCP);

//#if LWIP_VER == 14
//                        netconn_recv(m_newTcpNetConn, &pNetBuf);
//#else
//                        pNetBuf = netconn_recv(m_newTcpNetConn);
//#endif
                    }
                    if (!GET_TCP_CONN_LAST_ERROR(m_newTcpNetConn))
                    {
                        while (m_sendQueue.receive(&queueItem, 0) == pdPASS)
                        {
                            netError = netconn_write(m_newTcpNetConn, queueItem.data, queueItem.length, NETCONN_COPY);
                            if (netError != 0)
                                printf("netError %d\n", netError);
                            if (m_deadConnection)
                            {
                                break;
                            }
//                            while (m_newTcpNetConn->err == ERR_INPROGRESS);
                            led_toggle(LABL_TCP);
                        }
                    }
                }
                if (GET_TCP_CONN_LAST_ERROR(m_newTcpNetConn) || m_deadConnection)
                {
#if LWIP_VER == 14
                    netconn_recv(m_newTcpNetConn, &pNetBuf);
#else
                    pNetBuf = netconn_recv(m_newTcpNetConn);
#endif
                    while (pNetBuf != NULL)
                    {
                        while (m_receiveQueue.send(&pNetBuf, 10) != pdPASS)
                        {
                        }
#if (LWIP_STATS == 1)
//                        printf("after receive pbufs (err): %d/%d\n", lwip_stats.memp[MEMP_PBUF_POOL].used,
//                                lwip_stats.memp[MEMP_PBUF_POOL].avail);

#endif
                        led_toggle(LABL_TCP);
#if LWIP_VER == 14
                        netconn_recv(m_newTcpNetConn, &pNetBuf);
#else
                        pNetBuf = netconn_recv(m_newTcpNetConn);
#endif
                    }
                    closeConnection();
                }
            }
        }
    }
}

void CTcpConnectorRR::send(const void* message, u16_t length)
{
    T_TcpQueueItem queueItem;
// if the TCP connection is not up, don't queue the message.
    if (m_newTcpNetConn)
    {
        // copy the data to the queue for sending
        memcpy(&queueItem.data, message, length);
        queueItem.length = length;
        while (m_newTcpNetConn != NULL && m_sendQueue.sendToBack(&queueItem, 200) != pdPASS)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "TCP Send Queue is full (%d messages).", m_sendQueue.messagesWaiting());
            while (m_newTcpNetConn != NULL && m_sendQueue.messagesWaiting() != 0)
            {
                delay(10);
            }
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "TCP Send Queue was emptied.");
//            if ((getTickCount() - m_lastSuccessfullSendTick) >= M_FULL_SEND_QUEUE_TIMEOUT)
//                m_deadConnection = true;
        }
//        else
//        {
//            m_lastSuccessfullSendTick = getTickCount();
//        }
    }
}

portBASE_TYPE CTcpConnectorRR::receive(void* message, u16_t &length, portBASE_TYPE ticksToWait)
{
    netbuf* pNetBuf;
    portBASE_TYPE res;

    // if we still have data in the last netbuf, we don't want to get
    // the next netbuf from the socket.
    if (m_lastNetBuf == NULL)
    {
        if ((res = m_receiveQueue.receive(&pNetBuf, ticksToWait)) != pdPASS)
            return res;
        m_lastNetBuf = pNetBuf;
    }

    if (m_lastNetBuf->p->tot_len > PBUF_POOL_BUFSIZE)
        printf("Buffer larger than copy buffer! (%d)\n", m_lastNetBuf->p->tot_len);

    // copy the current netbuf to the receiver end.
    //length = netbuf_copy(m_lastNetBuf, message, PBUF_POOL_BUFSIZE);
    length = netbuf_copy(m_lastNetBuf, message, TCP_MSS);

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
//    printf("after free pbufs: %d/%d\n", lwip_stats.memp[MEMP_PBUF_POOL].used, lwip_stats.memp[MEMP_PBUF_POOL].avail);
#endif

    return pdPASS;
}

portBASE_TYPE CTcpConnectorRR::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
// check if the queues are valid - if they are it means that the target task already connected a handle to
// the queues. If not, create new queues.
    if (!m_receiveQueue.isValid())
    {
        m_receiveQueue.create(M_COMMAND_QUEUE_DEPTH, sizeof(netbuf*));
    }

    if (!m_sendQueue.isValid())
    {
        m_sendQueue.create(M_COMMAND_QUEUE_DEPTH, sizeof(T_TcpQueueItem));
    }

    return pdTRUE;
}

//portBASE_TYPE CTcpConnector::create(const portCHAR * const pcName, unsigned portSHORT usStackDepth)
//{
//    return CBaseConnector::create(pcName, usStackDepth, TCPIP_THREAD_PRIO - 1);
//}
//
void CTcpConnectorRR::sendTaskControlFunc(void *pParams)
{
    static_cast<AManagedTask *>(pParams)->run();
}

ip_addr& CTcpConnectorRR::getRemoteIpAddress()
{
    return m_newTcpNetConn->pcb.ip->remote_ip;
}
