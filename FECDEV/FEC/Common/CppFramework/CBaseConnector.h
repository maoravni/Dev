/*
 * CBaseConnector.h
 *
 *  Created on: 8 бреб 2012
 *      Author: maora
 */

#ifndef CBASECONNECTOR_H_
#define CBASECONNECTOR_H_

#include <CQueue.h>
#include <AManagedTask.h>
#include <CBinarySemaphore.h>

/**
 * This class implements a base class for all connector (communication) types
 */
class CBaseConnector : public AManagedTask
{
protected:
	/**
	 * A Queue for posting received data for the target task
	 */
	CQueue m_receiveQueue;

	/**
	 * A Semaphore for notifying the target task that a connection has been made.
	 */
	//- maora 12 Dec 2012: Remove the binary semaphore.
	//CBinarySemaphore m_binarySemaphore;

public:
	/**
	 * Constructor.
	 * @param receiveQueueHandle Handle to the receive queue for the target.
	 */
	CBaseConnector();
	virtual ~CBaseConnector();

	void attachReceiveQueueHandle(xQueueHandle queue);
	xQueueHandle getReceiveQueueHandle();
	/**
	 * return the FreeRTOS handle of the semaphore
	 */
	//inline operator xSemaphoreHandle() const { return (xSemaphoreHandle)m_binarySemaphore; }

};

#endif /* CBASECONNECTOR_H_ */
