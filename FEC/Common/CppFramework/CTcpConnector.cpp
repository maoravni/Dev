/*
 * CTcpConnector.cpp
 *
 *  Created on: 8 бреб 2012
 *      Author: maora
 */

#include "CTcpConnector.h"
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

//uint32_t ctcpconnector_stack_wm;
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

CTcpConnector::CTcpConnector(u16_t port)
{
    m_tcpNetConn = NULL;
    m_newTcpNetConn = NULL;
    m_port = port;
    m_lastNetBuf = NULL;
    m_lastSuccessfullSendTick = 0;
    m_deadConnection = false;
}

CTcpConnector::~CTcpConnector()
{
}

void CTcpConnector::closeConnection()
{
    T_TcpQueueItem queueItem;

    // empty the command queue:
//    T_CallbackQueueItem callbackQueueItem;
//    while (m_commandQueue.receive(&callbackQueueItem, 0) == pdPASS);
//    printf("close connection queue size: %d\n", m_commandQueue.messagesWaiting());

    // close the net connection
    unregisterNetconn(m_newTcpNetConn);
    netconn_close(m_newTcpNetConn);
    netconn_delete(m_newTcpNetConn);
    m_newTcpNetConn = NULL;

    xQueueReset(m_commandQueue);
    xQueueReset(m_receiveQueue);

    m_deadConnection = false;
}

void CTcpConnector::run()
{
    while (1)
    {
//#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
//        ctcpconnector_stack_wm = uxTaskGetStackHighWaterMark(0);
//#endif
        m_tcpNetConn = netconn_new_with_callback(NETCONN_TCP, lwipCallbackHandler);

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
            registerNetconn(m_newTcpNetConn, m_commandQueue);

//            m_newTcpNetConn->recv_timeout = 10;
            m_newTcpNetConn->pcb.tcp->so_options |= SOF_KEEPALIVE;

//            tcp_nagle_disable(m_newTcpNetConn->pcb.tcp);

//            led_dark(LABL_RESET);

            while (m_newTcpNetConn)
            {
                err_t netErr;
                T_CallbackQueueItem callbackQueueItem;
                while (m_commandQueue.receive(&callbackQueueItem, 50) == pdPASS)
                {
                    // check if the queue was full before we pulled the command from it,
                    // yield so that waiting tasks will have a chance to send to the queue.
                    if (m_commandQueue.messagesWaiting() >= (M_COMMAND_QUEUE_DEPTH - 1))
                        yield();
//                    printf("command queue size: %d\n", m_commandQueue.messagesWaiting()+1);
                    // check if no error occured on the connection:
//                    if (!GET_TCP_CONN_LAST_ERROR(m_newTcpNetConn) && !m_deadConnection)
//                    {
                    switch (callbackQueueItem.command)
                    {
                    case E_QueueCommandCommand_Send:
                        if (!GET_TCP_CONN_LAST_ERROR(m_newTcpNetConn) && !m_deadConnection)
                        {
                            netErr = netconn_write(callbackQueueItem.pNetconn, callbackQueueItem.payload.tcpQueueItem.data,
                                    callbackQueueItem.payload.tcpQueueItem.length, NETCONN_COPY);
//                            if (netErr != 0)
//                                printf("netErr: %d\n", netErr);
                        }
                        else
                            closeConnection();
                        break;
                    case E_QueueCommandCommand_Receive:
#if LWIP_VER == 14
                    netconn_recv(m_newTcpNetConn, &pNetBuf);
#else
                    pNetBuf = netconn_recv(m_newTcpNetConn);
#endif
                        while (m_receiveQueue.send(&pNetBuf, 200) != pdPASS)
                            int i = 0;
                        break;
                    case E_QueueCommandCommand_Disconnect:
                        closeConnection();
                        break;
                    }
//                    }
//                    else
//                    {
//                        closeConnection();
//                    }
                }
//                while (!GET_TCP_CONN_LAST_ERROR(m_newTcpNetConn) && !m_deadConnection)
//                {
//                    if (m_receiveQueue.receive)
//#if LWIP_VER == 14
//                    netconn_recv(m_newTcpNetConn, &pNetBuf);
//#else
//                    pNetBuf = netconn_recv(m_newTcpNetConn);
//#endif
////                    while (pNetBuf != NULL)
//                    if (pNetBuf != NULL)
//                    {
//                        if (m_receiveQueue.send(&pNetBuf, 10) != pdPASS)
//                        {
//                        }
//#if (LWIP_STATS == 1)
////                        printf("after receive pbufs: %d/%d\n", lwip_stats.memp[MEMP_PBUF_POOL].used,
////                                lwip_stats.memp[MEMP_PBUF_POOL].avail);
//
//#endif
//                        led_toggle(LABL_TCP);
//
////#if LWIP_VER == 14
////                        netconn_recv(m_newTcpNetConn, &pNetBuf);
////#else
////                        pNetBuf = netconn_recv(m_newTcpNetConn);
////#endif
//                    }
//                    if (!GET_TCP_CONN_LAST_ERROR(m_newTcpNetConn))
//                    {
//                        while (m_sendQueue.receive(&queueItem, 0) == pdPASS)
//                        {
//                            netconn_write(m_newTcpNetConn, queueItem.data, queueItem.length, NETCONN_COPY);
//                            if (m_deadConnection)
//                            {
//                                break;
//                            }
////                            while (m_newTcpNetConn->err == ERR_INPROGRESS);
//                            led_toggle(LABL_TCP);
//                        }
//                    }
////                }
//                if (GET_TCP_CONN_LAST_ERROR(m_newTcpNetConn) || m_deadConnection)
//                {
//#if LWIP_VER == 14
//                    netconn_recv(m_newTcpNetConn, &pNetBuf);
//#else
//                    pNetBuf = netconn_recv(m_newTcpNetConn);
//#endif
//                    while (pNetBuf != NULL)
//                    {
//                        while (m_receiveQueue.send(&pNetBuf, 10) != pdPASS)
//                        {
//                        }
//#if (LWIP_STATS == 1)
////                        printf("after receive pbufs (err): %d/%d\n", lwip_stats.memp[MEMP_PBUF_POOL].used,
////                                lwip_stats.memp[MEMP_PBUF_POOL].avail);
//
//#endif
//                        led_toggle(LABL_TCP);
//#if LWIP_VER == 14
//                        netconn_recv(m_newTcpNetConn, &pNetBuf);
//#else
//                        pNetBuf = netconn_recv(m_newTcpNetConn);
//#endif
//                    }
//                    closeConnection();
//                }
            }
        }
    }
}

void CTcpConnector::send(const void* message, u16_t length)
{
    T_CallbackQueueItem callbackQueueItem;

// if the TCP connection is not up, don't queue the message.
    if (m_newTcpNetConn)
    {
        // copy the data to the queue for sending
        memcpy(&callbackQueueItem.payload.tcpQueueItem.data, message, length);
        callbackQueueItem.payload.tcpQueueItem.length = length;
        callbackQueueItem.command = E_QueueCommandCommand_Send;
        callbackQueueItem.pNetconn = m_newTcpNetConn;
//        printf("CTCPConnector insert to callback queue. %d\n", m_commandQueue.messagesWaiting() + 1);
        while (m_newTcpNetConn && m_commandQueue.sendToBack(&callbackQueueItem, 200) != pdPASS)
        {
            if ((getTickCount() - m_lastSuccessfullSendTick) >= M_FULL_SEND_QUEUE_TIMEOUT)
                m_deadConnection = true;
            M_LOGGER_LOGF(M_LOGGER_LEVEL_FATAL, "TCP Send Queue is FULL!!!!");
            delay(200);
        }
        m_lastSuccessfullSendTick = getTickCount();
//        else
//        {
//        }
    }
}

portBASE_TYPE CTcpConnector::receive(void* message, u16_t &length, portBASE_TYPE ticksToWait)
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
    //printf("length: %d\n", length);

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

portBASE_TYPE CTcpConnector::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
// check if the queues are valid - if they are it means that the target task already connected a handle to
// the queues. If not, create new queues.
    if (!m_receiveQueue.isValid())
    {
        m_receiveQueue.create(M_COMMAND_QUEUE_DEPTH, sizeof(netbuf*));
    }

    if (!m_commandQueue.isValid())
    {
        m_commandQueue.create(M_COMMAND_QUEUE_DEPTH, sizeof(T_CallbackQueueItem));
    }

    return pdTRUE;
}

//portBASE_TYPE CTcpConnector::create(const portCHAR * const pcName, unsigned portSHORT usStackDepth)
//{
//    return CBaseConnector::create(pcName, usStackDepth, TCPIP_THREAD_PRIO - 1);
//}
//
void CTcpConnector::sendTaskControlFunc(void *pParams)
{
    static_cast<AManagedTask *>(pParams)->run();
}

ip_addr& CTcpConnector::getRemoteIpAddress()
{
    return m_newTcpNetConn->pcb.ip->remote_ip;
}
