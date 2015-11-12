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

#ifdef INCLUDE_POSIX
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "osapi/osapi_types.h"
#include "osapi/osapi_thread.h"
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

#ifdef INCLUDE_POSIX

RTI_PRIVATE int
OSAPI_Thread_getPolicyI(void)
{
    int sched_policy = SCHED_FIFO;
    int current_policy;
    struct sched_param dummy;
    int rc;


    /* If we're being spawned from a real-time thread, use the parent's policy
     * for the child instead of forcing SCHED_FIFO. */
    rc = pthread_getschedparam(pthread_self(), &current_policy, &dummy);
    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SYSTEM_THREAD_GET_POLICY_FAILED,rc);
    }
    else if ((current_policy == SCHED_RR) || (current_policy == SCHED_FIFO))
    {
        sched_policy = current_policy;
    }

    return sched_policy;
}

/*
    maps the osapi-defined priority levels to OS-agnostic priority values
*/
RTI_PRIVATE RTI_BOOL
OSAPI_mapToNativeThreadPriority(int *native_priority, int priority_level)
{
    int sched_policy;
    int min, max;

    if (priority_level >= 0)
    {
        *native_priority = priority_level;
        return RTI_TRUE;
    }

    sched_policy = OSAPI_Thread_getPolicyI();

#ifdef HAVE_RTI_POSIX_SCHED_POLICY
    /* If RTI_POSIX_SCHED_POLICY differs from the scheduling policy of my
     * application, log mismatch. This could be due to the fact to adjusting
     * scheduling policy previously failed. Continue calculating priority for
     * running scheduling priority */
    if (sched_policy != RTI_POSIX_SCHED_POLICY)
    {
        OSAPI_LOG_WARNING_P2(OSAPI_LOG_SYSTEM_THREAD_PRIORITY_MAP_FAILED,RTI_POSIX_SCHED_POLICY,sched_policy)
    }
#endif

    min = sched_get_priority_min(sched_policy);
    max = sched_get_priority_max(sched_policy);
    if (min == -1 || max == -1)
    {
        OSAPI_Log_error_p2(OSAPI_LOG_SYSTEM_THREAD_PRIORITY_MAP_FAILED,min,max);
        return RTI_FALSE;
    }

    /* posix thread gives us min and max, so we interpolate for the desired value 
     */
    *native_priority = min +
            ((max - min)*(priority_level*100)/OSAPI_THREAD_PRIORITY_HIGH)/100;

    OSAPI_LOG_DEBUG_P1(OSAPI_LOG_SYSTEM_THREAD_PRIORITY_MAP_FAILED,*native_priority)

    return RTI_TRUE;
}

/* Allocates needed memory and spawns the thread. the spawned thread pends
 * until start() is called before actually running user's routine. thread
 * spawning and pending states are guaranteed when RTI_TRUE is returned
 */
struct OSAPI_Thread*
OSAPI_Thread_create(const char *name,
                    const struct OSAPI_ThreadProperties *properties,
                    OSAPI_ThreadRoutine user_routine,
                    void *userParam, OSAPI_ThreadRoutine wakeup_routine)
{
    pthread_attr_t attr;
    int stack = 0;
    struct sched_param sch_param;
    int rc; /* TODO : Add more logging of OS error code */
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
    me->userRoutine = user_routine;
    me->wakeupRoutine = wakeup_routine;
    me->threadInfo.userParam = userParam;
    me->threadInfo.premptiveThread = RTI_TRUE;

    rc = pthread_attr_init(&attr);
    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc);
        goto fail;
    }

    rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc);
        goto fail;
    }

    /* Set the scheduling policy. This can fail on certain operating systems, due 
     * to the lack of superuser priviliges of the application. If this happens,
     * the default scheduling policy is used. This occurs i.e. on Linux: changing 
     * the policy from the default SCHED_OTHER to either SCHED_FIFO or SCHED_RR
     * require superuser privileges. */
    if (properties->options & OSAPI_THREAD_REALTIME_PRIORITY)
    {
        rc = pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
        if (rc != 0)
        {
            OSAPI_LOG_WARNING_P1(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc)
        }
#if defined(HAVE_RTI_POSIX_SCHED_POLICY)
        osErrorCode = pthread_attr_setschedpolicy(&attr, RTI_POSIX_SCHED_POLICY);
        if (osErrorCode != 0)
        {
            OSAPI_LOG_WARNING_P1(OSAPI_LOG_SYSTEM_THREAD_CREATE,osErrorCode)
        }
#else
        rc = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        if (rc != 0)
        {
            OSAPI_LOG_WARNING_P1(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc)
        }
#endif
    }

    /* Calculate and set the priority. THis should be done after the scheduling
     * policy has been adjusted. Setting the priority is scheduling policy
     * dependent. I.e. On Linux, Changing the priority for the default
     * SCHED_OTHER policy will fail.changing the priority is only applicable for
     * SCHED_FIFO and SCHED_RR scheduling policies. */
    if (!OSAPI_mapToNativeThreadPriority(&priority, properties->priority))
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SYSTEM_THREAD_CREATE,properties->priority);
        goto fail;
    }

    if (properties->priority != OSAPI_THREAD_PRIORITY_NORMAL)
    {
        /* only alter priority if non-normal is specified */
        sch_param.sched_priority = priority;

        rc = pthread_attr_setschedparam(&attr, &sch_param);
        if (rc != 0)
        {
            OSAPI_LOG_WARNING_P2(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc,
                                 properties->priority)
        }
        /* verify priority is actually set successfully */
        rc = pthread_attr_getschedparam(&attr, &sch_param);
        if (rc != 0)
        {
            OSAPI_LOG_WARNING_P1(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc)
        }
    }

    if (properties->stackSize > 0)
    {
        stack = properties->stackSize;
        rc = pthread_attr_setstacksize(&attr, stack);
        if (rc != 0)
        {
            OSAPI_Log_error_p1(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc);
            goto fail;
        }
    }

    rc = pthread_create(&(me->threadHandle),&attr, OSAPI_Thread_exec,(void *)me);

    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc);
        goto fail;
    }

    rc = pthread_attr_destroy(&attr);
    if (rc != 0)
    {
        OSAPI_LOG_WARNING_P1(OSAPI_LOG_SYSTEM_THREAD_CREATE,rc)
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
    return pthread_self();
}

void
OSAPI_Thread_sleep(RTI_UINT32 ms)
{
    usleep(ms * 1000);
}
#endif
