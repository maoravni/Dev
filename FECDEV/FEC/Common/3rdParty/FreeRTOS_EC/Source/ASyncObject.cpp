/**
 * ASyncObject.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "ASyncObject.h"
#include <assert.h>

ASyncObject::ASyncObject() {
	m_handleSemaphore = NULL;
}

ASyncObject::~ASyncObject() {
	if (isValid())
		deleteQueue();
}

void ASyncObject::deleteQueue() {
	if (m_handleSemaphore != NULL) {
		vQueueDelete(m_handleSemaphore);
		m_handleSemaphore = NULL;
	}
}

void ASyncObject::attach(xGenericHandle handle) {
	assert(handle != NULL);

	if (isValid()) {
		deleteQueue();
	}
	m_handleSemaphore = handle;
}
