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

#if OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "drivers/set_pinout.h"

#include "osapi/osapi_types.h"
#include "osapi/osapi_thread.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_semaphore.h"
#include "osapi/osapi_log.h"

#ifdef BUILDABLE_SOURCE
#include "../common/Thread.pkg.h"
#elif defined(CMAKE_BUILD)
#include "Thread.pkg.h"
#else
#include "../common/h/Thread.pkg.h"
#endif

#ifndef OSAPISTELLARIS_MAX_THREADS
#define OSAPISTELLARIS_MAX_THREADS 32
#endif

#define OSAPISTELLARIS_IDLE_THREAD_ID 0x7fffffff

struct OSAPIStellarisScheduler
{
    RTI_UINT32 is_initialized;
    struct OSAPI_Thread *threads[OSAPISTELLARIS_MAX_THREADS];
    RTI_UINT32 thread_count;
    RTI_UINT32 current_thread;
};


RTI_PRIVATE struct OSAPIStellarisScheduler OSAPIStellaris_g_scheduler = { 0 };
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS
RTI_PRIVATE void
OSAPIStellarisScheduler_initialize(struct OSAPIStellarisScheduler *self)
{
    RTI_UINT32 i;

    for (i = 0; i < OSAPISTELLARIS_MAX_THREADS; ++i)
    {
        OSAPIStellaris_g_scheduler.threads[i] = NULL;
    }

    OSAPIStellaris_g_scheduler.thread_count = 0;
    OSAPIStellaris_g_scheduler.current_thread = OSAPISTELLARIS_IDLE_THREAD_ID;
    OSAPIStellaris_g_scheduler.is_initialized = 1;
}

void
OSAPI_ThreadScheduler_run(void)
{
    RTI_UINT32 i, c;

    for (i = 0, c = 0; (i < OSAPISTELLARIS_MAX_THREADS) &&
         (c < OSAPIStellaris_g_scheduler.thread_count); ++i)
    {
        if (OSAPIStellaris_g_scheduler.threads[i] != NULL)
        {
            OSAPIStellaris_g_scheduler.current_thread = i;

            OSAPIStellaris_g_scheduler.threads[i]->threadInfo.premptiveThread =
                RTI_FALSE;

            OSAPI_Thread_exec(OSAPIStellaris_g_scheduler.threads[i]);

            ++c;
        }
    }
}

struct OSAPI_Thread*
OSAPI_Thread_create(const char *name,
                   const struct OSAPI_ThreadProperties *properties,
                   OSAPI_ThreadRoutine user_routine,
                   void *user_param, OSAPI_ThreadRoutine wakeup_routine)
{
    struct OSAPI_Thread *self;
    RTI_UINT32 i;

    if (!OSAPIStellaris_g_scheduler.is_initialized)
    {
        OSAPIStellarisScheduler_initialize(&OSAPIStellaris_g_scheduler);
    }

    for (i = 0; i < OSAPISTELLARIS_MAX_THREADS; ++i)
    {
        if (OSAPIStellaris_g_scheduler.threads[i] == NULL)
        {
            break;
        }
    }

    if (i == OSAPISTELLARIS_MAX_THREADS)
    {
        return NULL;
    }

    self = OSAPI_Thread_new();
    if (!self)
    {
        return NULL;
    }

    self->userRoutine = user_routine;
    self->wakeupRoutine = wakeup_routine;
    self->threadInfo.userParam = user_param;
    self->threadInfo.premptiveThread = RTI_FALSE;

    if (!OSAPI_Semaphore_give(self->startSem))
    {
        OSAPI_Thread_delete(self);
        return NULL;
    }

    self->threadInfo.premptiveThread = RTI_FALSE;

    OSAPI_Thread_exec(self);

    if (!OSAPI_Semaphore_take(self->createSem, 
                              OSAPI_SEMAPHORE_TIMEOUT_INFINITE, NULL))
    {
        OSAPI_Thread_delete(self);
        return NULL;
    }

    OSAPIStellaris_g_scheduler.threads[i] = self;
    ++OSAPIStellaris_g_scheduler.thread_count;


    return self;
}


OSAPI_ThreadId
OSAPI_Thread_self()
{
    return OSAPIStellaris_g_scheduler.current_thread;
}

extern volatile int g_sleep;

void
OSAPI_Thread_sleep(RTI_UINT32 ms)
{
    g_sleep = ms / 10;

    while (g_sleep) ;
}

#endif
