/**
 * @file AManagedTask.h
 * @class AManagedTask
 *
 * @date Aug 3, 2010
 * @author Stefano Oliveri
 *         E-Mail: software@stf12.net
 */

/**
 * @defgroup FreeRTOS_Managed FreeRTOS Managed Class Extension
 * @ingroup FreeRTOS_CPP_EXT
 * @{
 */

#ifndef AMANAGEDTASK_H_
#define AMANAGEDTASK_H_

#include "CTask.h"

class AManagedTask: public CTask {
	friend class CFreeRTOS;

	static AManagedTask *s_pManagedTaskListHead;
	AManagedTask *m_pNextManagedTask;

public:
	AManagedTask();
	virtual ~AManagedTask();


	portBASE_TYPE Create(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);
	virtual bool HardwareInit() { return true; }

	virtual void Run() =0;

	static void taskControlFunc(void *pParams);

private:
	void AddToManagedTask(AManagedTask *pTaskToAdd);
	void RemoveFromManagedTask(AManagedTask *pTaskToRemove);
	static bool InitHardwareForManagedTasks();
};


#endif /* AMANAGEDTASK_H_ */

/**
 * @}
 */

