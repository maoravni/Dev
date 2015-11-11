/**
 * @file   ASyncObject.h
 * @class  ASyncObject
 * @ingroup FreeRTOS_Wrapper
 *
 * Abstract base class for all task synchronization object.
 *
 * @date   Jul 13, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef ASEMAPHORE_H_
#define ASEMAPHORE_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "IFreeRTOSProtocol.h"

class ASyncObject: public IFreeRTOSObj {
protected:
    xSemaphoreHandle m_handleSemaphore;

public:
	ASyncObject();
	virtual ~ASyncObject();

	inline operator xSemaphoreHandle() const { return m_handleSemaphore; }

	void deleteQueue();
	inline bool isValid()const;
	void attach(xGenericHandle handle);
	inline xGenericHandle detach();

	virtual portBASE_TYPE take(portTickType xBlockTime) =0;
	virtual portBASE_TYPE give() =0;
};

// inline method

inline
bool ASyncObject::isValid() const {
	return m_handleSemaphore != NULL;
}

inline
xGenericHandle ASyncObject::detach() {
	xSemaphoreHandle res = m_handleSemaphore;
	m_handleSemaphore = NULL;
	return res;
}

#endif /* ASEMAPHORE_H_ */

