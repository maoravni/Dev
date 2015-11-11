/*
 * CTcpConnector.h
 *
 *  Created on: 8 бреб 2012
 *      Author: maora
 */

#ifndef CUDPCONNECTOR_H_
#define CUDPCONNECTOR_H_

#include "arch/cc.h"
#include "CBaseConnector.h"
#include "lwip/mem.h"
#include "lwip/api.h"
#include "CMutex.h"

struct T_UdpQueueItem
{
    char data[TCP_MSS];
    u16_t length;
};

/**
 * Class for TCP connections
 */
class CUdpConnector: public CBaseConnector
{
private:
    u16_t m_port;
    ip_addr m_ipAddress;

    netconn* m_udpNetConn;
    //netconn* m_newTcpNetConn;

    CQueue m_sendQueue; //!< A Queue for posting received data for the target task

public:
    CUdpConnector(u16_t port);
    virtual ~CUdpConnector();

    /**
     * Main task method
     */
    void run();

    /**
     * Send data
     * The base connector has a semaphore. When the connection is made, the semaphore would be
     * "Given", so the connecting task will block until a connection is made.
     */
    void send(const void* message, u16_t length);

    /** Receive data
     * This function wraps around queue.receive and netbuf data functions.
     * @param message pointer to memory to be filled.
     * @param length length of the data filled in memory.
     * @param ticksToWait ticks to wait until returning with an answer.
     * @return pdPASS if data was fetched from the queue.
     */
    portBASE_TYPE receive(void* message, u16_t &length, portBASE_TYPE ticksToWait);

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

//    bool isConnected()
//    {
//        return true;
//    }

    /**
     * Create the FreeRTOS task attached to this object.
     *
     * @param pcName name of the task. Need only for debug purpose.
     * @param usStackDepth size in word of the task stack.
     *
     * @sa CTask::Create
     */
    portBASE_TYPE create(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority = (TCPIP_THREAD_PRIO - 1));

    static void sendTaskControlFunc(void *pParams);

    void setPort(unsigned short port);
    void setIpAddress(ip_addr& ipAddress);
};

#endif /* CUDPCONNECTOR_H_ */
