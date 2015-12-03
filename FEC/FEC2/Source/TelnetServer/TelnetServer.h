/*
 * TelnetServer.h
 *
 *  Created on: 5 Jun 2013
 *      Author: maora
 */

#ifndef TELNETSERVER_H_
#define TELNETSERVER_H_

#include <AManagedTask.h>
#include <lwip/api.h>
#include <CTcpConnectorRR.h>
#include <portmacro.h>
#include "CommandInterpreter.h"

class TelnetServer: public AManagedTask
{
    static TelnetServer s_instance; //!< Instance of the telnet server singleton.

    CTcpConnectorRR m_tcpConnector;

    CQueue m_tcpReceiveQueue;

    char m_telnetMessage[TCP_MSS];

//    netconn* m_tcpNetConn;
//    netconn* m_newTcpNetConn;

    CommandInterpreter m_commandInterpreter;

    TelnetServer();

public:
    virtual ~TelnetServer();

    static TelnetServer& getInstance();

    /**
     * Task control loop.
     */
    virtual void run();

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
    virtual portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);

};

#endif /* TELNETSERVER_H_ */
