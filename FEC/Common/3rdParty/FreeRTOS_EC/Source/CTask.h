/**
 * @file CTask.h
 * @class CTask
 * @ingroup FreeRTOS_Wrapper
 *
 * The CTask class wraps a native FreeRTOS task handle (xTaskHandle). To create a task instance
 * an object of this class and then call its CTask::Create method like showed in the following sample:
 * \code
 * CTask g_aTask // global task declaration.
 *
 * void main()
 * {
 * 	g_aTask.Create(task_control_func, "Led2", configMINIMAL_STACK_SIZE, mainFLASH_TASK_PRIORITY);
 *
 * 	CFreeRTOS::StartScheduler();
 * }
 * \endcode
 *
 * Usually the application subclass this class to create an application specific task object.
 * See the CHelloWorld class for an example.
 *
 * \sa <a href="http://www.freertos.org/taskandcr.html">task documentation</a> in the FreeRTOS web site.
 *
 * @date Jul 5, 2010
 * @author Stefano Oliveri (STF12.net)
 *         E-Mail: software@stf12.net
 */

#ifndef CTASK_H_
#define CTASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "IFreeRTOSProtocol.h"

class CTask: public IFreeRTOSObj {

	/**
	 * Specifies the native FreeRTOS handle managed by an instance of this class.
	 */
	xTaskHandle m_handleTask;

public:
	/**
	 * The default constructor. It initializes the object without attaching it to a native FreeRTOS handle.
	 * Call the Create method to allocate a task handle and attach it to the object.
	 */
	CTask();

	/**
	 * Create a CTask instance and attach it to a valid handle.
	 *
	 * @param handleTask a valid task handle.
	 */
	CTask(xTaskHandle handleTask);

	/**
	 * Delete the native FreeRTOS task.
	 */
	virtual ~CTask();

	/**
	 * Retrieve the native FreeRTOS task handle attached to this object.
	 *
	 * @return the native FreeRTOS task handle attached to this object.
	 */
	inline operator xTaskHandle() const { return m_handleTask; }

	// FreeRTOS API Wrappers

	/**
	 * Create a FreeRTOS task and attach its handle to the receiver object.
	 *
	 * \sa <a href="http://www.freertos.org/a00125.html">xTaskCreate</a> FreeRTOS API function.
	 */
	portBASE_TYPE create(pdTASK_CODE pvTaskCode, const portCHAR * const pcName, unsigned portSHORT usStackDepth, void *pvParameters, unsigned portBASE_TYPE uxPriority);

	/**
	 * Create a Memory Protection Unit (MPU) restricted task and attach its handle to the receiver object.
	 *
	 * \sa <a href="http://www.freertos.org/xTaskCreateRestricted.html">xTaskCreateRestricted</a> FreeRTOS API function.
	 */
	portBASE_TYPE createRestricted(xTaskParameters *pxTaskDefinition);

	/**
	 * \sa <a href="http://www.freertos.org/a00126.html">vTaskDelete</a>  FreeRTOS API function.
	 */
	void deleteTask();

	/**
	 * \sa <a href="http://www.freertos.org/a00127.html">vTaskDelay</a>  FreeRTOS API function.
	 */
	inline static void delay(portTickType xTicksToDelay);

	/**
	 * \sa <a href="http://www.freertos.org/vtaskdelayuntil.html">vTaskDelayUntil</a>  FreeRTOS API function.
	 */
    inline static void delayUntil(portTickType *pxPreviousWakeTime, portTickType xTimeIncrement);

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskYIELD">taskYIELD</a>  FreeRTOS API function.
     */
    inline static void yield() { taskYIELD(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskENTER_CRITICAL">taskENTER_CRITICAL</a>  FreeRTOS API function.
     */
    inline static void enterCritical() { taskENTER_CRITICAL(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskEXIT_CRITICAL">taskEXIT_CRITICAL</a>  FreeRTOS API function.
     */
    inline static void exitCritical() { taskEXIT_CRITICAL(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskDISABLE_INTERRUPTS">taskDISABLE_INTERRUPTS</a>  FreeRTOS API function.
     */
    inline static void disableInterrupt() { taskDISABLE_INTERRUPTS(); }

    /**
     * \sa <a href="http://www.freertos.org/a00020.html#taskENABLE_INTERRUPTS">taskENABLE_INTERRUPTS</a>  FreeRTOS API function.
     */
    inline static void enableInterrupt() { taskENABLE_INTERRUPTS(); }

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetCurrentTaskHandle">xTaskGetCurrentTaskHandle</a>  FreeRTOS API function.
     */
    inline static xTaskHandle getCurrentTaskHandle();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetTickCount">xTaskGetTickCount</a>  FreeRTOS API function.
     */
    inline static portTickType getTickCount();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#xTaskGetSchedulerState">xTaskGetSchedulerState</a>  FreeRTOS API function.
     */
    inline static portBASE_TYPE getSchedulerState();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#usTaskGetNumberOfTasks">uxTaskGetNumberOfTasks</a>  FreeRTOS API function.
     */
    inline static unsigned portBASE_TYPE getNumberOfTasks();

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#vTaskList">vTaskList</a>  FreeRTOS API function.
     */
    inline static void getTaskList(portCHAR *pcWriteBuffer);

    /**
     * \sa <a href="http://www.freertos.org/a00021.html#vTaskGetRunTimeStats">vTaskGetRunTimeStats</a>  FreeRTOS API function.
     */
    inline static void getRunTimeStats(portCHAR *pcWriteBuffer);

    /**
     * \sa <a href="http://www.freertos.org/a00128.html">uxTaskPriorityGet</a>  FreeRTOS API function.
     */
    inline unsigned portBASE_TYPE priorityGet() const;

    /**
     * \sa <a href="http://www.freertos.org/a00129.html">vTaskPrioritySet</a>  FreeRTOS API function.
     */
    inline void prioritySet(unsigned portBASE_TYPE uxNewPriority);

    /**
     * \sa <a href="http://www.freertos.org/a00130.html">vTaskSuspend</a>  FreeRTOS API function.
     */
    inline void suspend();

    /**
     * \sa <a href="http://www.freertos.org/a00131.html">vTaskResume</a>  FreeRTOS API function.
     */
    inline void resume();

    /**
     * \sa <a href="http://www.freertos.org/taskresumefromisr.html">xTaskResumeFromISR</a>  FreeRTOS API function.
     */
    inline portBASE_TYPE resumeFromISR();

    /**
     * \sa <a href="http://www.freertos.org/vTaskSetApplicationTag.html">vTaskSetApplicationTaskTag</a>  FreeRTOS API function.
     */
    inline void setApplicationTag(pdTASK_HOOK_CODE pxTagValue);

    /**
     * \sa <a href="http://www.freertos.org/xTaskCallApplicationTaskHook.html">xTaskCallApplicationTaskHook</a>  FreeRTOS API function.
     */
    inline portBASE_TYPE callApplicationTaskHook(void *pvParameter);

    /**
     * \sa <a href="http://www.freertos.org/vTaskAllocateMPURegions.html">vTaskAllocateMPURegions</a>  FreeRTOS API function.
     */
    inline void allocateMPURegions(const xMemoryRegion * const xRegions);

    // FreeRTOS class extension.
	inline bool isValid() const;
	void attach(xGenericHandle handle);
	inline xGenericHandle detach();
};

// inline method
////////////////

inline
void CTask::delay(portTickType xTicksToDelay) {
#if ( INCLUDE_vTaskDelay == 1 )
	vTaskDelay(xTicksToDelay);
#endif
}

inline
void CTask::delayUntil(portTickType *pxPreviousWakeTime, portTickType xTimeIncrement) {
#if ( INCLUDE_vTaskDelayUntil == 1 )
	vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
#endif
}

inline
xTaskHandle CTask::getCurrentTaskHandle() {
	return xTaskGetCurrentTaskHandle();
}

inline
portTickType CTask::getTickCount() {
	return xTaskGetTickCount();
}

inline
portBASE_TYPE CTask::getSchedulerState() {
#if ( INCLUDE_xTaskGetSchedulerState == 1 )
	return xTaskGetSchedulerState();
#else
	return 0;
#endif
}

inline
unsigned portBASE_TYPE CTask::getNumberOfTasks() {
	return uxTaskGetNumberOfTasks();
}

inline
void CTask::getTaskList(portCHAR *pcWriteBuffer) {
#if ( configUSE_TRACE_FACILITY == 1 )
	vTaskList((portCHAR*)pcWriteBuffer);
#endif
}

inline
void CTask::getRunTimeStats(portCHAR *pcWriteBuffer) {
#if (configGENERATE_RUN_TIME_STATS == 1)
	vTaskGetRunTimeStats((portCHAR*)pcWriteBuffer);
#endif
}

inline
unsigned portBASE_TYPE CTask::priorityGet() const {
#if ( INCLUDE_uxTaskPriorityGet == 1 )
	return uxTaskPriorityGet(m_handleTask);
#else
	return -1;
#endif
}

inline
void CTask::prioritySet(unsigned portBASE_TYPE uxNewPriority) {
#if ( INCLUDE_vTaskPrioritySet == 1 )
	vTaskPrioritySet(m_handleTask, uxNewPriority);
#endif
}

inline
void CTask::suspend() {
#if ( INCLUDE_vTaskSuspend == 1 )
	vTaskSuspend(m_handleTask);
#endif
}

inline
void CTask::resume() {
#if ( INCLUDE_vTaskSuspend == 1 )
	vTaskResume(m_handleTask);
#endif
}

inline
portBASE_TYPE CTask::resumeFromISR() {
#if ( ( INCLUDE_xTaskResumeFromISR == 1 ) && ( INCLUDE_vTaskSuspend == 1 ) )
	return xTaskResumeFromISR(m_handleTask);
#else
	return pdFALSE;
#endif
}

inline
void CTask::setApplicationTag(pdTASK_HOOK_CODE pxTagValue) {
#if ( configUSE_APPLICATION_TASK_TAG == 1 )
	vTaskSetApplicationTaskTag(m_handleTask, pxTagValue);
#endif
}

inline
portBASE_TYPE CTask::callApplicationTaskHook(void *pvParameter) {
#if ( configUSE_APPLICATION_TASK_TAG == 1 )
	return xTaskCallApplicationTaskHook(m_handleTask, pvParameter);
#else
	return 0;
#endif
}

inline
void CTask::allocateMPURegions(const xMemoryRegion * const xRegions) {
#if ( portUSING_MPU_WRAPPERS == 1 )
	vTaskAllocateMPURegions(m_handleTask, xRegions);
#endif
}

inline
bool CTask::isValid() const{
	return m_handleTask != NULL;
}

inline
xGenericHandle CTask::detach() {
	xTaskHandle res = m_handleTask;
	m_handleTask = NULL;
	return res;
}

#endif /* CTASK_H_ */
