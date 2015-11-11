/**
 * CFreeRTOS.cpp
 *
 *  Created on: Jul 14, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CFreeRTOS.h"
#include "AManagedTask.h"

CFreeRTOS::CFreeRTOS() {

}

CFreeRTOS::~CFreeRTOS() {
}

bool CFreeRTOS::initHardwareForManagedTasks() {
	return AManagedTask::InitHardwareForManagedTasks();
}
