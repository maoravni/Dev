/**
 * CQueue.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CQueue.h"


CQueue::CQueue() {
	m_handleQueue = NULL;
}

CQueue::~CQueue() {
	if (isValid())
		deleteQueue();
}

CQueue::CQueue(xQueueHandle handleQueue) {
	attach(handleQueue);
}

CQueue &CQueue::create(unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize) {
	assert(!isValid());

	xQueueHandle handle;

	handle = xQueueCreate(uxQueueLength, uxItemSize);
	if (handle != NULL)
		attach(handle);

	return *this;
}

void CQueue::attach(xGenericHandle handle) {
	assert(handle != NULL);

	if (isValid())
		deleteQueue();

	m_handleQueue = handle;
}

void CQueue::deleteQueue() {
	if (m_handleQueue != NULL) {
		vQueueDelete(m_handleQueue);
		m_handleQueue = NULL;
	}
}

