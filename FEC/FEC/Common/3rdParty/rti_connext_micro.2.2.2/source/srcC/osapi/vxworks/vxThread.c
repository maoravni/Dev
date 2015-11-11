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

#if OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <vxWorks.h>
#include <taskLib.h>
#ifdef RTI_RTP
  #include <taskLibCommon.h>
  #include <tlsLib.h>
#else
  #include <taskVarLib.h>
#endif
#include <sysLib.h>
#include <tickLib.h>

#include "osapi/osapi_thread.h"
#include "osapi/osapi_types.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_semaphore.h"
#include "osapi/osapi_log.h"

#ifdef BUILDABLE_SOURCE
#include "../common/Thread.pkg.h"
#else
#include "Thread.pkg.h"
#endif

#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS

RTI_PRIVATE RTI_BOOL
OSAPI_mapToNativeThreadPriority(RTI_INT32 *nativePriority,
                                RTI_INT32 priorityLevel)
{
    if (priorityLevel >= 0)
    {
        *nativePriority = priorityLevel;
        return RTI_TRUE;
    }

    /* we center our priority around the typical 100 */
    switch (priorityLevel)
    {
        case OSAPI_THREAD_PRIORITY_LOW:
            *nativePriority = 120;
            return RTI_TRUE;
        case OSAPI_THREAD_PRIORITY_BELOW_NORMAL:
            *nativePriority = 110;
            return RTI_TRUE;
        case OSAPI_THREAD_PRIORITY_NORMAL:
            *nativePriority = 100;
            return RTI_TRUE;
        case OSAPI_THREAD_PRIORITY_ABOVE_NORMAL:
            *nativePriority = 71;
            return RTI_TRUE;
        case OSAPI_THREAD_PRIORITY_HIGH:
            *nativePriority = 68;
            return RTI_TRUE;
        default:
            OSAPI_Log_error_p1(OSAPI_LOG_SYSTEM_THREAD_PRIORITY_MAP_FAILED,priorityLevel);
            return RTI_FALSE;
    }

    return RTI_TRUE;
}

/*
    allocates needed memory and spawns the thread. the spawned thread pends
    until start() is called before actually running user's routine. thread
    spawning and pending states are guaranteed when RTI_TRUE is returned
*/
struct OSAPI_Thread *
OSAPI_Thread_create(const char *name,
                   const struct OSAPI_ThreadProperties *properties,
                   OSAPI_ThreadRoutine userRoutine,
                   void *userParam,
                   OSAPI_ThreadRoutine wakeupRoutine)
{
    int os_options = 0;
    int priority;
    struct OSAPI_Thread *me;

    LOG_PRECONDITION(properties == NULL, return NULL)

    /* create the thread handle */
    me = OSAPI_Thread_new();
    if (me == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_THREAD_CREATE);
        return NULL;
    }

    /* asign user's values */
    me->userRoutine = userRoutine;
    me->wakeupRoutine = wakeupRoutine;
    me->threadInfo.userParam = userParam;
    me->threadInfo.premptiveThread = RTI_TRUE;

    /* Calculate native priority */
    if (!OSAPI_mapToNativeThreadPriority(&priority, properties->priority))
    {
        LOG_printError("could not convert thread priority of %d\n",
                       properties->priority);
        goto fail;
    }

    /* user indicates they will be doing fp, setup vxworks thread state correctly
     */
    if (properties->options & OSAPI_THREAD_FLOATING_POINT)
    {
        os_options |= VX_FP_TASK;
    }
#if !defined(RTI_RTP)
#if defined (RTI_LOG_ERROR)
    /* RTI_LOG_ERROR is the lowest form of debugging, indicating there's always a
     * chance that we'll be printing in the thread - we automatically enabled
     * VX_STDIO regardless of their thread option */
    os_options |= VX_STDIO;
#else
    /* user indicates they will be doing stdio, setup vxworks thread state
     * correctly */
    if (properties->options & OSAPI_THREAD_STDIO)
    {
        os_options |= VX_STDIO;
    }
#endif /* RTI_LOG_ERROR */
#endif /* !RTI_RTP */

    me->threadHandle = taskSpawn((char *)name,
                                 priority,
                                 os_options,
                                 properties->stackSize,
                                 (FUNCPTR) OSAPI_Thread_exec,
                                 (int)me, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (me->threadHandle == ERROR)
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_THREAD_CREATE);
        goto fail;
    }

    /* spawned thread gives semaphore immediately when started */
    if (!OSAPI_Semaphore_take(me->createSem, 
                              OSAPI_SEMAPHORE_TIMEOUT_INFINITE, NULL))
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_THREAD_CREATE);
        goto fail;
    }

    return me;

fail:

    (void)OSAPI_Thread_delete(me);
    return NULL;
}

OSAPI_ThreadId
OSAPI_Thread_self(void)
{
#if defined(RTI_RTP)
    return getpid();
#else
    return taskIdSelf();
#endif
}

void
OSAPI_Thread_sleep(RTI_UINT32 ms)
{
    taskDelay(((sysClkRateGet() * ms) + 500) / 1000);
}

#endif

















