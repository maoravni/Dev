/**
 * @file CQueue.h
 * @class CQueue
 * @ingroup FreeRTOS_Wrapper
 *
 * The CQueue class wraps a native FreeRTOS queue handle (xQueueHandle).
 * To create a queue instance an object of this class and then call its CQueue::Create method
 * like showed in the following sample:
 * \code
 * CQueue g_aQueue // global task declaration.
 *
 * void taskControlFunc(void *pParams)
 * {
 * 	unsigned char *Message;
 * 	g_aQueue.Create(nQueueSize, sizeof(queueItemT));
 *
 * 	for (;;) {
 * 		g_aQueue.Receive(&Message, portMAX_DELAY ) != pdTRUE)
 * 		// do something
 * }
 * \endcode
 *
 * See the CLcdTask for an example.
 *
 * \sa <a href="http://www.freertos.org/Inter-Task-Communication.html">queue documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 12, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */


#ifndef CQUEUE_H_
#define CQUEUE_H_

#include <assert.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "IFreeRTOSProtocol.h"


class CQueue: public IFreeRTOSObj {
	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	xQueueHandle m_handleQueue;

public:
	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a queue handle and attach it to the object.
	 */
	CQueue();

	/**
	 * Create a CQueue instance and attach it to a valid handle.
	 *
	 * @param handleQueue a valid queue handle.
	 */
	CQueue(xQueueHandle handleQueue);

	/**
	 * Delete the native FreeRTOS queue.
	 */
	virtual ~CQueue();

	/**
	 * Retrieve the native FreeRTOS queue handle attached to this object.
	 *
	 * @return the native FreeRTOS queue handle attached to this object.
	 */
	inline operator xQueueHandle() const { return m_handleQueue; }

	inline bool isValid() const;
	void attach(xGenericHandle handle);
	inline xGenericHandle detach();

	// FreeRTOS API Wrappers

	/**
	 * \sa <a href="http://www.freertos.org/a00116.html">xQueueCreate</a> FreeRTOS API function.
	 */
	CQueue &create(unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize);

	/**
	 * \sa <a href="http://www.freertos.org/a00018.html#vQueueDelete">vQueueDelete</a> FreeRTOS API function.
	 */
	void deleteQueue();

	/**
	 * \sa <a href="http://www.freertos.org/a00018.html#ucQueueMessagesWaiting">uxQueueMessagesWaiting</a> FreeRTOS API function.
	 */
	inline unsigned portBASE_TYPE messagesWaiting();

	/**
	 * \sa <a href="http://www.freertos.org/a00117.html">xQueueSend</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE send(const void * pvItemToQueue, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToBack.html">SendToBack</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE sendToBack(const void * pvItemToQueue, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToFront.html">xQueueSendToFront</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE sendToFront(const void * pvItemToQueue, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/a00118.html">xQueueReceive</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE receive(void *pvBuffer, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/xQueuePeek.html">xQueuePeek</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE peek(void *pvBuffer, portTickType xTicksToWait);

	/**
	 * \sa <a href="http://www.freertos.org/a00119.html">xQueueSendFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE sendFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToBackFromISR.html">xQueueSendToBackFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE sendToBackFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/xQueueSendToFrontFromISR.html">xQueueSendToFrontFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE sendToFrontFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/a00120.html">xQueueReceiveFromISR</a> FreeRTOS API function.
	 */
	inline portBASE_TYPE receiveFromISR(void *pvBuffer, portBASE_TYPE *pxTaskWoken);

	/**
	 * \sa <a href="http://www.freertos.org/vQueueAddToRegistry.html">vQueueAddToRegistry</a> FreeRTOS API function.
	 */
	inline void addToRegistry(portCHAR *pcQueueName);

    inline void reset();
//    inline portBASE_TYPE isQueueEmpty();
//    inline portBASE_TYPE isQueueFull();
};

// inline method implementation

inline
portBASE_TYPE CQueue::send(const void * pvItemToQueue, portTickType xTicksToWait) {
	return xQueueSend(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
portBASE_TYPE CQueue::sendFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken) {
	return xQueueSendFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
unsigned portBASE_TYPE CQueue::messagesWaiting() {
	assert(isValid());

	return uxQueueMessagesWaiting(m_handleQueue);
}

inline
portBASE_TYPE CQueue::sendToBack(const void * pvItemToQueue, portTickType xTicksToWait) {
	assert(isValid());

	return xQueueSendToBack(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
portBASE_TYPE CQueue::sendToFront(const void * pvItemToQueue, portTickType xTicksToWait) {
	assert(isValid());

	return xQueueSendToFront(m_handleQueue, pvItemToQueue, xTicksToWait);
}

inline
portBASE_TYPE CQueue::receive(void *pvBuffer, portTickType xTicksToWait) {
	assert(isValid());

	return xQueueReceive(m_handleQueue, pvBuffer, xTicksToWait);
}

inline
portBASE_TYPE CQueue::peek(void *pvBuffer, portTickType xTicksToWait) {
	assert(isValid());

	return xQueuePeek(m_handleQueue, pvBuffer, xTicksToWait);
}

inline
portBASE_TYPE CQueue::sendToBackFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken) {
	assert(isValid());

	return xQueueSendToBackFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
portBASE_TYPE CQueue::sendToFrontFromISR(const void * pvItemToQueue, portBASE_TYPE *pxHigherPriorityTaskWoken) {
	assert(isValid());

	return xQueueSendToFrontFromISR(m_handleQueue, pvItemToQueue, pxHigherPriorityTaskWoken);
}

inline
portBASE_TYPE CQueue::receiveFromISR(void *pvBuffer, portBASE_TYPE *pxTaskWoken) {
	assert(isValid());

	return xQueueReceiveFromISR(m_handleQueue, pvBuffer, pxTaskWoken);
}

inline
void CQueue::addToRegistry(portCHAR *pcQueueName) {
#if (configQUEUE_REGISTRY_SIZE > 0)
	assert(isValid());

	vQueueAddToRegistry(m_handleQueue, pcQueueName);
#endif
}

inline
bool CQueue::isValid() const{
	return m_handleQueue != NULL;
}

inline
xGenericHandle CQueue::detach() {
	xQueueHandle res = m_handleQueue;
	m_handleQueue = NULL;
	return res;
}

inline void CQueue::reset()
{
    xQueueReset(m_handleQueue);
}

//inline portBASE_TYPE CQueue::isQueueEmpty()
//{
//    return prvIsQueueEmpty(m_handleQueue);
//}
//
//inline portBASE_TYPE CQueue::isQueueFull()
//{
//    return prvIsQueueFull(m_handleQueue);
//}

#endif /* CQUEUE_H_ */

