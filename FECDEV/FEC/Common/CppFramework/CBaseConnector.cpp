/*
 * CBaseConnector.cpp
 *
 *  Created on: 8 бреб 2012
 *      Author: maora
 */

#include "CBaseConnector.h"

CBaseConnector::~CBaseConnector()
{
}

CBaseConnector::CBaseConnector()
{
}

void CBaseConnector::attachReceiveQueueHandle(xQueueHandle queue)
{
	m_receiveQueue.attach(queue);
}

xQueueHandle CBaseConnector::getReceiveQueueHandle()
{
	return (xQueueHandle) m_receiveQueue;
}

