/**
 * CMutex.cpp
 *
 *  Created on: Jul 14, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CMutex.h"

CMutex::CMutex() {

}

CMutex::~CMutex() {
}

CMutex::CMutex(xSemaphoreHandle handleSemaphore) {
	attach(handleSemaphore);
}

bool CMutex::create() {

#if ( configUSE_MUTEXES == 1 )

	xSemaphoreHandle handle;

	handle = xSemaphoreCreateMutex();

	if (handle != NULL)
		attach(handle);
#endif

	return isValid();
}

