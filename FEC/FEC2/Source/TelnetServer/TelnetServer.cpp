/*
 * TelnetServer.cpp
 *
 *  Created on: 5 Jun 2013
 *      Author: maora
 */

#include "TelnetServer.h"
#include <hmi.h>

#define M_DEFAULT_TELNET_PORT 23
#define M_TELNET_STACK_SIZE configMINIMAL_STACK_SIZE

TelnetServer TelnetServer::s_instance;

TelnetServer::TelnetServer() :
        m_tcpConnector(M_DEFAULT_TELNET_PORT)
{

}

TelnetServer::~TelnetServer()
{
}

void TelnetServer::run()
{
    u16_t length;
//    led_toggle(LABL_RESET);
    while (1)
    {
        while (m_tcpConnector.isConnected())
        {
            if (m_tcpConnector.receive(m_telnetMessage, length, 50) == pdPASS)
            {
                m_tcpConnector.send(m_telnetMessage, length);
                m_commandInterpreter.interpretCommand(m_telnetMessage);
            }
        }
        delay(50);
    }
}

TelnetServer& TelnetServer::getInstance()
{
    if (!s_instance.isValid())
    {
// Create the task.
        s_instance.create("TelnetServer", configMINIMAL_STACK_SIZE, 4);
    }

    return s_instance;
}

portBASE_TYPE TelnetServer::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    if ((m_tcpConnector.create("Telnet TCP", M_TELNET_STACK_SIZE, uxPriority-1, uxPriority-1 )) != pdPASS)
        return pdFAIL;

    m_tcpReceiveQueue.attach(m_tcpConnector.getReceiveQueueHandle());

    return pdPASS;
}
