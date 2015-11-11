/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_thread_h
#define osapi_thread_h


#include "osapi/osapi_config.h"
#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
  \brief Thread API
*/

/*e \defgroup OSAPI_ThreadClass OSAPI Thread
 *  \ingroup OSAPIModule
*/

/*e \ingroup OSAPI_ThreadClass

    \brief Abstract Thread API.
*/
#ifdef INCLUDE_POSIX
#include <pthread.h>
#define OSAPI_ThreadHandle pthread_t
#define OSAPI_ThreadId pthread_t
#elif OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS
#include <windows.h>
#include <process.h>
#define OSAPI_ThreadHandle HANDLE
#define OSAPI_ThreadId HANDLE
#elif OSAPI_PLATFORM == OSAPI_PLATFORM_STOS
#define OSAPI_ThreadHandle RTI_INT32
#define OSAPI_ThreadId RTI_INT32
#elif OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS
#define OSAPI_ThreadHandle RTI_INT32
#define OSAPI_ThreadId RTI_INT32
#elif OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS
#ifndef INC_FREERTOS_H
#include "FreeRTOS.h"
#endif
#ifndef TASK_H
#include "task.h"
#endif
#define OSAPI_ThreadHandle xTaskHandle
#define OSAPI_ThreadId xTaskHandle
#elif OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS
#define OSAPI_ThreadId int
#define OSAPI_ThreadHandle int
#else
#error "thread handle not ported"
#endif

/* these are #defined so users can pass in any int they want for priority as well
 *  i.e., OS-native thread priorities
 */
#if OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS
#define OSAPI_THREAD_PRIORITY_LOW               1
#define OSAPI_THREAD_PRIORITY_BELOW_NORMAL      2
#define OSAPI_THREAD_PRIORITY_NORMAL            3
#define OSAPI_THREAD_PRIORITY_ABOVE_NORMAL      4
#define OSAPI_THREAD_PRIORITY_HIGH              5
#else
#define OSAPI_THREAD_PRIORITY_LOW               -1
#define OSAPI_THREAD_PRIORITY_BELOW_NORMAL      -2
#define OSAPI_THREAD_PRIORITY_NORMAL            -3
#define OSAPI_THREAD_PRIORITY_ABOVE_NORMAL      -4
#define OSAPI_THREAD_PRIORITY_HIGH              -5
#endif

/*e \ingroup OSAPI_ThreadClass
 *  Thread options
 */
typedef RTI_UINT32 OSAPI_ThreadOptions;

/*e \ingroup OSAPI_ThreadClass
  Use only the default options the OS gives you.
*/
#define OSAPI_THREAD_DEFAULT_OPTIONS     0x00
/*e \ingroup OSAPI_ThreadClass
  Support floating point.
*/
#define OSAPI_THREAD_FLOATING_POINT      0x01
/*e \ingroup OSAPI_ThreadClass
  Support standard I/O.
*/
#define OSAPI_THREAD_STDIO               0x02
/*e \ingroup OSAPI_ThreadClass
  Run in real-time priority mode.
*/
#define OSAPI_THREAD_REALTIME_PRIORITY   0x08
/*e \ingroup OSAPI_ThreadClass
  Insist on the specified priority and fail if the OS doesn't like it.
*/
#define OSAPI_THREAD_PRIORITY_ENFORCE    0x10
/*e \ingroup OSAPI_ThreadClass
  Support the ability to asynchronously cancel the thread.
*/
#define OSAPI_THREAD_CANCEL_ASYNCHRONOUS 0x20

/*e \ingroup OSAPI_ThreadClass
 *
 */
struct OSAPI_ThreadProperties
{
    /*e Hint on required stack-size */
    RTI_UINT32 stackSize;

    /*e Hint on required priority */
    RTI_INT32 priority;

    OSAPI_ThreadOptions options;
};

/*e \ingroup OSAPI_ThreadClass
 * Initializer for thread properties
 */
#define OSAPI_THREAD_PROPERTIES_DEFAULT \
{ \
    16*1024, \
    OSAPI_THREAD_PRIORITY_NORMAL, \
    OSAPI_THREAD_DEFAULT_OPTIONS \
}

#define OSAPI_ThreadProperties_INITIALIZER OSAPI_THREAD_PROPERTIES_DEFAULT
/*e
 * \ingroup OSAPI_ThreadClass
 * \brief Thread info
 *
 */
struct OSAPI_ThreadInfo
{
    /*e Stop executing thread */
    RTI_BOOL stopThread;

    /*e Whether or not the created thread is preemptive */
    RTI_BOOL premptiveThread;

    /*e Parameter passed by thread creator. Passed to thread. */
    void *userParam;
};

/*e \ingroup OSAPI_ThreadClass
 *
 * \brief Thread task signature.
 */
typedef RTI_BOOL(*OSAPI_ThreadRoutine) (struct OSAPI_ThreadInfo * threadInfo);

/*e \ingroup OSAPI_ThreadClass
 *
 * \brief Abstract thread class.
 */
struct OSAPI_Thread;

/*i \ingroup OSAPI_ThreadClass
 *
 *  \brief Wakeup a specific thread.
 *
 *  \return RTI_TRUE on success, RTI_FALSE on failure.
 *
 *  \sa \ref OSAPIPortingClass, \ref OSAPI_Thread_create
 */
OSAPIDllExport RTI_BOOL
OSAPI_Thread_wakeup(struct OSAPI_Thread *me);

/*i \ingroup OSAPI_ThreadClass
 *
 *  \brief Start a specific thread.
 *
 *  \return RTI_TRUE on success, RTI_FALSE on failure.
 *
 *  \sa \ref OSAPIPortingClass, \ref OSAPI_Thread_create
 */
OSAPIDllExport RTI_BOOL
OSAPI_Thread_start(struct OSAPI_Thread *me);

/*e \ingroup OSAPI_ThreadClass
 *
 *  \brief Destroy a specific thread.
 *  \param [in] self Thread.
 *  \return RTI_TRUE on success, RTI_FALSE on failure.
 *
 *  \sa \ref OSAPIPortingClass, \ref OSAPI_Thread_create
 */
OSAPIDllExport RTI_BOOL
OSAPI_Thread_destroy(struct OSAPI_Thread *self);

/*e \ingroup OSAPI_ThreadClass
 *
 *  \brief Suspend a thread for a specified amount of time.
 *
 *  \param [in] ms Sleep time.
 *
 *  \sa \ref OSAPIPortingClass
 */
OSAPIDllExport void
OSAPI_Thread_sleep(RTI_UINT32 ms);

/*e \ingroup OSAPI_ThreadClass
 *
 *  \brief Create a thread.
 *
 *  \param [in] name The name of the thread.
 *
 *  \param [in] properties Thread properties. These properties are hints.
 *
 *  \param [in] userRoutine  Thread task. The thread task cannot assume that
 *                          it can block; thus it must be written such that
 *                          it can be called repeatedly.
 *
 *  \param [in] userParam Parameters passed to the thread task.
 *
 *  \param [in] wakeupRoutine Routine to wakeup a thread, called to delete
 *                            a thread.
 *
 *  \return Handle to stopped thread on success, NULL on failure.
 *
 *  \sa \ref OSAPIPortingClass, \ref OSAPI_Thread_destroy
 */
OSAPIDllExport struct OSAPI_Thread*
OSAPI_Thread_create(const char *name,
                   const struct OSAPI_ThreadProperties *properties,
                   OSAPI_ThreadRoutine userRoutine,
                   void *userParam,
                   OSAPI_ThreadRoutine wakeupRoutine);


/*e \ingroup OSAPI_ThreadClass
 *
 *  \brief Return thread ID.
 *
 *  \return thread ID of the calling thread.
 *
 *  \sa \ref OSAPIPortingClass, \ref OSAPI_Thread_create
 */
OSAPIDllExport OSAPI_ThreadId
OSAPI_Thread_self(void);

OSAPIDllExport void
OSAPI_ThreadScheduler_run(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* osapi_thread_h */
