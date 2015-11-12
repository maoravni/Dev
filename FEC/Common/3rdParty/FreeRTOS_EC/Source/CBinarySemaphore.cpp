/**
 * CBinarySemaphore.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CBinarySemaphore.h"
#include <assert.h>

CBinarySemaphore::CBinarySemaphore() {

}

CBinarySemaphore::~CBinarySemaphore() {
}

CBinarySemaphore::CBinarySemaphore(xSemaphoreHandle handleSemaphore) {
	attach(handleSemaphore);
}

bool CBinarySemaphore::create() {
	xSemaphoreHandle handle;

	vSemaphoreCreateBinary(handle);

	if (handle != NULL)
		attach(handle);

	return isValid();
}
