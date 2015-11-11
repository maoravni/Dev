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

#if OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <windows.h>
#include <process.h>

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

#if OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS


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
            *nativePriority = THREAD_PRIORITY_LOWEST;
            return RTI_TRUE;
        case OSAPI_THREAD_PRIORITY_BELOW_NORMAL:
            *nativePriority = THREAD_PRIORITY_BELOW_NORMAL;
            return RTI_TRUE;
        case OSAPI_THREAD_PRIORITY_NORMAL:
            *nativePriority = THREAD_PRIORITY_NORMAL;
            return RTI_TRUE;
        case OSAPI_THREAD_PRIORITY_ABOVE_NORMAL:
            *nativePriority = THREAD_PRIORITY_ABOVE_NORMAL;
            return RTI_TRUE;
        case OSAPI_THREAD_PRIORITY_HIGH:
            *nativePriority = THREAD_PRIORITY_HIGHEST;
            return RTI_TRUE;
        default:
            OSAPI_Log_error_p1(OSAPI_LOG_SYSTEM_THREAD_PRIORITY_MAP_FAILED,priorityLevel);
            return RTI_FALSE;
    }
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
                   void *userParam, OSAPI_ThreadRoutine wakeupRoutine)
{
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

    me->threadHandle = (HANDLE)
        _beginthread((void (__cdecl *) (void *))OSAPI_Thread_exec,
                     properties->stackSize, (void *)me);
    if (me->threadHandle == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_THREAD_CREATE);
        goto fail;
    }

    /* spawned thread gives semaphore immediately when started */
    if (!OSAPI_Semaphore_take(me->createSem, OSAPI_SEMAPHORE_TIMEOUT_INFINITE, NULL))
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_THREAD_CREATE);
        goto fail;
    }

    return me;

fail:

    OSAPI_Thread_delete(me);
    return NULL;
}

OSAPI_ThreadId
OSAPI_Thread_self(void)
{
    return (HANDLE) GetCurrentThreadId();
}

void
OSAPI_Thread_sleep(RTI_INT32 ms)
{
    Sleep(ms);
}
#endif
