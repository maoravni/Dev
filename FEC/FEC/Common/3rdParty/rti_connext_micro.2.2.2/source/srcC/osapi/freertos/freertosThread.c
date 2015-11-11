/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "osapi/osapi_config.h"
#include "util/util_makeheader.h"

#if OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS

#include "FreeRTOS.h"
#include "task.h"

#include "osapi/osapi_types.h"
#include "osapi/osapi_thread.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_semaphore.h"
#include "osapi/osapi_log.h"

#ifdef BUILDABLE_SOURCE
#include "../common/Thread.pkg.h"
#elif CMAKE_BUILD
#include "Thread.pkg.h"
#else
#include "../common/h/Thread.pkg.h"
#endif
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS
struct OSAPI_Thread *
OSAPI_Thread_create(const char *name,
                   const struct OSAPI_ThreadProperties *properties,
                   OSAPI_ThreadRoutine user_routine,
                   void *user_param, OSAPI_ThreadRoutine wakeup_routine)
{

    int stack = 0;
    int osErrorCode;            /* TODO : Add more logging of OS error code */

    int priority = tskIDLE_PRIORITY;

    portBASE_TYPE retcode;

    struct OSAPI_Thread *me = NULL;

    /* create the thread handle */
    me = OSAPI_Thread_new();
    if (!me)
    {
        LOG_printError("could not allocate space for thread\n");
        return NULL;
    }

    /* asign user's values */
    me->userRoutine = user_routine;
    me->wakeupRoutine = wakeup_routine;
    me->threadInfo.userParam = user_param;
    me->threadInfo.premptiveThread = RTI_TRUE;

    /* priority defaults to same as idle task */
    if (properties->priority > tskIDLE_PRIORITY)
    {
        priority = properties->priority;
    }

    if (properties->stackSize > 0)
    {
        stack = properties->stackSize / sizeof(portSTACK_TYPE);
    }

    retcode =
        xTaskCreate((pdTASK_CODE)OSAPI_Thread_exec, name, stack, &me->threadInfo, priority,
                    &me->threadHandle);
    if (pdTRUE != retcode)
    {
        LOG_printError("could not create thread (0x%x)\n", retcode);
        goto fail;
    }

    /* spawned thread gives semaphore immediately when started */
    if (!OSAPI_Semaphore_take(me->createSem, 
                              OSAPI_SEMAPHORE_TIMEOUT_INFINITE, NULL))
    {
        LOG_printError("could not take semaphore to confirm thread creation\n");
        goto fail;
    }

    return me;

    fail:

    OSAPI_Thread_delete(me);
    return NULL;
}

OSAPI_ThreadId
OSAPI_Thread_self()
{
    return xTaskGetCurrentTaskHandle();
}


void
OSAPI_Thread_sleep(RTI_UINT32 ms)
{
    vTaskDelay(ms / portTICK_RATE_MS);
}

#endif
