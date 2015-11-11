/**
 * CRecursiveMutex.cpp
 *
 *  Created on: Jul 14, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CRecursiveMutex.h"

CRecursiveMutex::CRecursiveMutex() {

}

CRecursiveMutex::CRecursiveMutex(xSemaphoreHandle handleSemaphore) {
	attach(handleSemaphore);
}

CRecursiveMutex::~CRecursiveMutex() {
}

bool CRecursiveMutex::Create() {
#if (configUSE_RECURSIVE_MUTEXES == 1 && configUSE_MUTEXES ==1)
	xSemaphoreHandle handle = xSemaphoreCreateRecursiveMutex();
	if (handle != NULL)
		attach(handle);
#endif

	return isValid();
}

