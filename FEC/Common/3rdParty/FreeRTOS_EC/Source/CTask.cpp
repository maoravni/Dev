/**
 * CTask.cpp
 *
 *  Created on: Jul 5, 2010
 *      Author: Stefano Oliveri (STF12.net)
 *      E-Mail: software@stf12.net
 */

#include "CTask.h"
#include <assert.h>


CTask::CTask() {
	m_handleTask = NULL;
}

CTask::CTask(xTaskHandle handleTask) {
	attach(handleTask);
}

CTask::~CTask() {
	if (isValid())
		deleteTask();
}

void CTask::attach(xGenericHandle handle) {
	assert(handle != NULL);

	if (isValid()) {
		deleteTask();
	}
	m_handleTask = handle;
}

portBASE_TYPE CTask::create(pdTASK_CODE pvTaskCode, const portCHAR * const pcName, unsigned portSHORT usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority)
{
	portBASE_TYPE res;
	xTaskHandle handle;
	res = xTaskCreate(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority, &handle);
	if (res == pdTRUE)
		attach(handle);

	return res;
}

portBASE_TYPE CTask::createRestricted(xTaskParameters *pxTaskDefinition)
{
	portBASE_TYPE res = pdFALSE;
#if( portUSING_MPU_WRAPPERS == 1 )
	xTaskHandle handle;
	res = xTaskCreateRestricted(pxTaskDefinition, &handle);
	if (res == pdTRUE)
		attach(handle);
#endif

	return res;
}


void CTask::deleteTask() {
	if (m_handleTask != NULL) {
#if ( INCLUDE_vTaskDelete == 1 )
		xTaskHandle handleTaskToDelete = m_handleTask;
		m_handleTask = NULL;
		vTaskDelete(handleTaskToDelete);
#else
		vTaskSuspend(m_handleTask);
#endif
		m_handleTask = NULL;
	}
}
