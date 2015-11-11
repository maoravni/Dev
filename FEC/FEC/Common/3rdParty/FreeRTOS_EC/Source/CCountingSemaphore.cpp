/**
 * CCountingSemaphore.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CCountingSemaphore.h"
#include <assert.h>

CCountingSemaphore::CCountingSemaphore() {

}

CCountingSemaphore::~CCountingSemaphore() {
}

CCountingSemaphore::CCountingSemaphore(xSemaphoreHandle handleSemaphore) {
	attach(handleSemaphore);
}

bool CCountingSemaphore::create(unsigned portBASE_TYPE uxMaxCount, unsigned portBASE_TYPE uxInitialCount) {

#if (configUSE_COUNTING_SEMAPHORES == 1)
	xSemaphoreHandle handle = NULL;
	handle = xSemaphoreCreateCounting(uxMaxCount, uxInitialCount);

	if (handle != NULL)
		attach(handle);
#endif

	return isValid();
}
