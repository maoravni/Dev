/**
 * AManagedTask.cpp
 *
 *  Created on: Aug 3, 2010
 *      Author: Stefano Oliveri
 */

#include "AManagedTask.h"
#include "CFreeRTOS.h"

AManagedTask* AManagedTask::s_pManagedTaskListHead = NULL;

AManagedTask::AManagedTask() {
	m_pNextManagedTask = NULL;
	AddToManagedTask(this);
}

AManagedTask::~AManagedTask() {
	RemoveFromManagedTask(this);
}

portBASE_TYPE AManagedTask::create(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) {
	return onCreate(pcName, usStackDepth, uxPriority) &&
			CTask::create(AManagedTask::taskControlFunc, pcName, usStackDepth, this, uxPriority);
}

void AManagedTask::taskControlFunc(void *pParams) {
	static_cast<AManagedTask *>(pParams)->run();
}

void AManagedTask::AddToManagedTask(AManagedTask *pTaskToAdd) {
	enterCritical();
	// is the first element?
	if (s_pManagedTaskListHead == NULL) {
		s_pManagedTaskListHead = this;
	}
	else {
		// add the task to the head of the managed task list.
		m_pNextManagedTask = s_pManagedTaskListHead;
		s_pManagedTaskListHead = this;
	}
	exitCritical();
}

void AManagedTask::RemoveFromManagedTask(AManagedTask *pTaskToRemove) {
	// search for the task to remove in the managed task list
	// is it the first element?
	enterCritical();
	if (pTaskToRemove == s_pManagedTaskListHead) {
		s_pManagedTaskListHead = pTaskToRemove->m_pNextManagedTask;
		exitCritical();
	}
	else {
		exitCritical();
		CFreeRTOS::suspendAllTasks();
		AManagedTask *pTask = s_pManagedTaskListHead;
		while (pTask != NULL && pTask->m_pNextManagedTask != pTaskToRemove) {
			pTask = pTask->m_pNextManagedTask;
		}
		if (pTask != NULL) { // Check to be sure that the task is in the list
			// remove the thask from the list
			pTask->m_pNextManagedTask = pTaskToRemove->m_pNextManagedTask;
		}
		CFreeRTOS::resumeAllTasks();
	}
}

bool AManagedTask::InitHardwareForManagedTasks() {
	bool bRes = true;
	for (AManagedTask *pTask=s_pManagedTaskListHead; pTask!=NULL; pTask=pTask->m_pNextManagedTask)
		bRes &= pTask->hardwareInit();

	return bRes;
}
