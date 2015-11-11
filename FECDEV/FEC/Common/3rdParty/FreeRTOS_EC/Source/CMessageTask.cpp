/*
 * CMessageTask.cpp
 *
 *  Created on: Sep 20, 2010
 *      Author: oliveris
 */

#include <stdio.h>
#include "CMessageTask.h"


const CMessageTask::message_map_entry_t CMessageTask::s_message_map[] = {
		{ NULL_MSG, NULL }
};

CMessageTask::CMessageTask()
{
	m_pDelegate = NULL;
	m_nTimeOut = portMAX_DELAY;
}

CMessageTask::~CMessageTask() {
}

portBASE_TYPE CMessageTask::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) {
	portBASE_TYPE res;
	res = m_queue.create(CMessageTask::MESSAGE_QUEUE_SIZE, sizeof(char[20])).isValid();

	return res;
}

void CMessageTask::run() {

	// call task specific initialization code.
	 if ( !onInit() ) {
#if INCLUDE_vTaskDelete == 1
		 deleteTask();
		 return;
#else
	 // The task must not start due to initialization problem, but the FreeRTOS
	 // vTaskDelete function is not defined by configuration file. Suspend the task.
		 Suspend();
#endif
	 }

	CMessage msg;
	for (;;) {

		if ( m_queue.receive(&msg, m_nTimeOut) == pdTRUE ) {
			// Message Handling routine

			// Call the delegate, if one, before try to dispatch the event
			if (m_pDelegate) m_pDelegate->OnHandleEvent(msg);
			DispatchMessage(msg);
			// Call the delegate, if one, after tried to dispatch the event
			if (m_pDelegate) m_pDelegate->DidHandleEvent(msg);
		}
		else {
			// TODOFreeRTOS_EC: STF - timeout expired.
			onTimeOut();
		}
	}
}

void CMessageTask::DispatchMessage(const CMessage &msg) {
	for (int i=0; s_message_map[i].id!=NULL_MSG; ++i) {
		if (s_message_map[i].id == msg.m_nId) {
			(this->*s_message_map[i].MessageHandler)(msg.wParam);
			break;
		}
	}
}

bool CMessageTask::postMessage(CMessage *pMsg, portTickType nTicksToWait/*=portMAX_DELAY*/) {
	return (bool)m_queue.send(pMsg, nTicksToWait);
}

bool CMessageTask::postMessageFromISR(CMessage *pMsg, portBASE_TYPE *pxHigherPriorityTaskWoken) {
	return (bool)m_queue.sendFromISR(pMsg, pxHigherPriorityTaskWoken);
}

bool CMessageTask::sendMessage(CMessage *pMsg, portTickType nTicksToWait/*=portMAX_DELAY*/) {
	return (bool)m_queue.sendToFront(pMsg, nTicksToWait);
}

bool CMessageTask::sendFromISR(CMessage *pMsg, portBASE_TYPE * pxHigherPriorityTaskWoken) {
	return (bool)m_queue.sendToFrontFromISR(pMsg, pxHigherPriorityTaskWoken);
}
