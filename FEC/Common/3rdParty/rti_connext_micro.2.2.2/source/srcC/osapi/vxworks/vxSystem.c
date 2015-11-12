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
#include <vxWorks.h>
#if defined(RTI_RTP)
  #include <unistd.h>
  #include <rtpLibCommon.h>
  #include <errno.h>
#else
  #include <taskLib.h>
#endif
#include <stdlib.h>
#include <limits.h>
#include <sysLib.h>
#include <tickLib.h>
#include <objLib.h>
#include <time.h>

#include "osapi/osapi_heap.h"
#include "osapi/osapi_mutex.h"
#include "osapi/osapi_system.h"
#include "osapi/osapi_thread.h"
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#ifdef BUILDABLE_SOURCE
#include "../common/System.pkg.h"
#else
#include "System.pkg.h"
#endif

#ifndef OSAPISYSTEM_MAX_TIMERS
#define OSAPISYSTEM_MAX_TIMERS          8
#endif

#ifndef OSAPISYSTEM_TIMER_RESOLUTION
#define OSAPISYSTEM_TIMER_RESOLUTION    (1000/60)
#endif

struct OSAPI_SystemTimerHandler
{
    OSAPI_TimerTickHandlerFunction handler;
    void *param;
};

struct OSAPI_SystemVxWorks
{
    struct OSAPI_System _parent;
    struct OSAPI_Mutex *mutex;
    struct OSAPI_SystemTimerHandler timer_handler[OSAPISYSTEM_MAX_TIMERS];
    struct OSAPI_Thread *timer_thread;
    RTI_INT32 timer_count;
    struct timespec resolution;
};

RTI_PRIVATE struct OSAPI_SystemVxWorks OSAPI_System_g = {OSAPI_System_INITIALIZER};
RTI_PRIVATE struct OSAPI_SystemVxWorks *OSAPI_System_g_system = &OSAPI_System_g;
RTI_UINT32 OSAPI_System_g_size = sizeof(struct OSAPI_SystemVxWorks);

#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS
RTI_PRIVATE RTI_BOOL
OSAPI_System_lock(void)
{
    return OSAPI_Mutex_take(OSAPI_System_g_system->mutex);
}

RTI_PRIVATE RTI_BOOL
OSAPI_System_unlock(void)
{
    return OSAPI_Mutex_give(OSAPI_System_g_system->mutex);
}

RTI_PRIVATE RTI_BOOL
OSAPI_SystemVxWorks_timer_thread(struct OSAPI_ThreadInfo* thread_param)
{
    struct OSAPI_SystemVxWorks *self =
        (struct OSAPI_SystemVxWorks *)thread_param->userParam;
    RTI_INT32 i, j;

#if VXWORKS_VERSION_6_3_OR_BETTER  
    struct timespec this_iter_start_time, next_iter_start_time;
#endif

    while (!thread_param->stopThread)
    {
        (void)OSAPI_System_lock();

#if VXWORKS_VERSION_6_3_OR_BETTER  
        /* determine time of next iteration */
        if (clock_gettime(CLOCK_REALTIME, &this_iter_start_time) == -1)
        {
            return RTI_FALSE;
        }
      
        next_iter_start_time = this_iter_start_time;
        next_iter_start_time.tv_nsec += self->resolution.tv_nsec;
        if (next_iter_start_time.tv_nsec < this_iter_start_time.tv_nsec)
        {
            ++next_iter_start_time.tv_sec;
        }
#endif

        for (i = 0, j = self->timer_count; (i < OSAPISYSTEM_MAX_TIMERS) && j;
             ++i)
        {
            if (self->timer_handler[i].handler)
            {
                j--;
                self->timer_handler[i].handler(self->timer_handler[i].param);
            }
        }

        (void)OSAPI_System_unlock();

#if VXWORKS_VERSION_6_3_OR_BETTER
        if (clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, 
                            &next_iter_start_time, NULL) == -1)
        {
            return RTI_FALSE;
        }
#else
        taskDelay(1);
#endif
    }

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
OSAPI_SystemVxWorks_initialize(struct OSAPI_SystemVxWorks *self)
{
    RTI_INT32 i;

    if (self->_parent.is_initialized)
    {
        return RTI_TRUE;
    }

    /* NOTE: VxWorks 6.8+ SMP is not locked, not yet multi task safe */
#if !defined(RTI_RTP) && !(VXWORKS_VERSION_6_8_OR_BETTER)
    taskLock();
#endif

    if (self->_parent.is_initialized)
    {
#if !defined(RTI_RTP) && !(VXWORKS_VERSION_6_8_OR_BETTER)
        taskUnlock();
#endif
        return RTI_TRUE;
    }

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        self->timer_handler[i].handler = NULL;
        self->timer_handler[i].param = NULL;
    }

    self->timer_count = 0;

    if (clock_getres(CLOCK_REALTIME, &self->resolution) == -1)
    {
#if !defined(RTI_RTP) && !(VXWORKS_VERSION_6_8_OR_BETTER)
        taskUnlock();
#endif
        return RTI_FALSE;
    }

    self->mutex = OSAPI_Mutex_new();
    if (self->mutex == NULL)
    {
#if !defined(RTI_RTP) && !(VXWORKS_VERSION_6_8_OR_BETTER)
        taskUnlock();
#endif
        return RTI_FALSE;
    }

    self->timer_thread = OSAPI_Thread_create("osapi_timer",
            &self->_parent.property.timer_property.thread,
            OSAPI_SystemVxWorks_timer_thread,(void *)self,NULL);

    if (self->timer_thread == NULL)
    {
#if !defined(RTI_RTP) && !(VXWORKS_VERSION_6_8_OR_BETTER)
        taskUnlock();
#endif
        return RTI_FALSE;
    }

    if (!OSAPI_Thread_start(self->timer_thread))
    {
#if !defined(RTI_RTP) && !(VXWORKS_VERSION_6_8_OR_BETTER)
        taskUnlock();
#endif
        return RTI_FALSE;
    }

    self->_parent.is_initialized = RTI_TRUE;

#if !defined(RTI_RTP) && !(VXWORKS_VERSION_6_8_OR_BETTER)
    taskUnlock();
#endif
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
OSAPI_SystemVxWorks_finalize(struct OSAPI_SystemVxWorks *self)
{
    RTI_INT32 i;

    if (!self->_parent.is_initialized)
    {
        return RTI_TRUE;
    }

    if (!OSAPI_System_lock())
    {
        return RTI_FALSE;
    }

    if (!OSAPI_Thread_destroy(self->timer_thread))
    {
        return RTI_FALSE;
    }

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        self->timer_handler[i].handler = NULL;
        self->timer_handler[i].param = NULL;
    }

    self->timer_count = 0;

    if (!OSAPI_System_unlock())
    {
        return RTI_FALSE;
    }

    if (!OSAPI_Mutex_delete(self->mutex))
    {
        return RTI_FALSE;
    }

    self->_parent.is_initialized = RTI_FALSE;

    return RTI_TRUE;
}

/******************************************************************************
 *  Public API
 ******************************************************************************/
RTI_BOOL
OSAPI_System_get_time(OSAPI_NtpTime *now)
{
    struct timespec clk_time;

    LOG_PRECONDITION(now == NULL,return RTI_FALSE)

    if (clock_gettime(CLOCK_REALTIME, &clk_time) == -1)
    {
        return RTI_FALSE;
    }

    OSAPI_NtpTime_packFromNanosec(now, clk_time.tv_sec, clk_time.tv_nsec);

    return RTI_TRUE;
}


RTI_BOOL
OSAPI_System_start_timer(OSAPI_Timer_t self,
                        OSAPI_TimerTickHandlerFunction tick_handler)
{
    RTI_INT32 i;

    LOG_PRECONDITION_P4((self == NULL) || (tick_handler == NULL),
            return RTI_FALSE,
            OSAPI_LOG_PTR_HIGH(self),OSAPI_LOG_PTR_LOW(self),
            OSAPI_LOG_PTR_HIGH(tick_handler),OSAPI_LOG_PTR_LOW(tick_handler))

    if (!OSAPI_System_g_system->_parent.is_initialized)
    {
        if (!OSAPI_SystemVxWorks_initialize(OSAPI_System_g_system))
        {
            OSAPI_Log_error(OSAPI_LOG_SYSTEM_START_TIMER_FAILED);
            return RTI_FALSE;
        }
    }

    if (!OSAPI_System_lock())
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_START_TIMER_FAILED);
        return RTI_FALSE;
    }

    if (OSAPI_System_g_system->timer_count == OSAPISYSTEM_MAX_TIMERS)
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_START_TIMER_FAILED);
        (void)OSAPI_System_unlock();
        return RTI_FALSE;
    }

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        if (OSAPI_System_g_system->timer_handler[i].handler == NULL)
        {
            break;
        }
    }

    if (i == OSAPISYSTEM_MAX_TIMERS)
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_START_TIMER_FAILED);
        (void)OSAPI_System_unlock();
        return RTI_FALSE;
    }

    OSAPI_System_g_system->timer_handler[i].handler = tick_handler;
    OSAPI_System_g_system->timer_handler[i].param = self;
    ++OSAPI_System_g_system->timer_count;

    return OSAPI_System_unlock();
}

RTI_BOOL
OSAPI_System_stop_timer(OSAPI_Timer_t self)
{
    RTI_INT32 i;

    LOG_PRECONDITION_P2(self == NULL,return RTI_FALSE,
            OSAPI_LOG_PTR_HIGH(self),OSAPI_LOG_PTR_LOW(self))

    if (!OSAPI_System_g_system->_parent.is_initialized)
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_START_TIMER_FAILED);
        return RTI_FALSE;
    }

    if (!OSAPI_System_lock())
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_START_TIMER_FAILED);
        return RTI_FALSE;
    }

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        if (OSAPI_System_g_system->timer_handler[i].param == self)
        {
            OSAPI_System_g_system->timer_handler[i].handler = NULL;
            OSAPI_System_g_system->timer_handler[i].param = NULL;
            --OSAPI_System_g_system->timer_count;
        }
    }

    if (!OSAPI_System_unlock())
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_START_TIMER_FAILED);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_INT32
OSAPI_System_get_timer_resolution(void)
{
    struct timespec clk_res;
    if (clock_getres(CLOCK_REALTIME, &clk_res) == -1)
    {
        return -1;
    }
    /* return in milliseconds */
    return (RTI_INT32)clk_res.tv_nsec / 1000000;
}

RTI_BOOL
OSAPI_System_initialize(void)
{
    return OSAPI_SystemVxWorks_initialize(OSAPI_System_g_system);
}


RTI_BOOL
OSAPI_System_finalize(void)
{
    return OSAPI_SystemVxWorks_finalize(OSAPI_System_g_system);
}

void
OSAPI_System_generate_uuid(struct OSAPI_SystemUUID *uuid_out)
{
    OSAPI_NtpTime now;

    LOG_PRECONDITION(uuid_out == NULL,return)

    /* TODO: use system-specific uuid generation */
    OSAPI_System_get_time(&now);
    uuid_out->value[0] = (RTI_UINT32)now.sec;
    uuid_out->value[1] = (RTI_UINT32)now.frac;
    uuid_out->value[2] = (RTI_UINT32)&OSAPI_System_g;
    uuid_out->value[3] = (RTI_UINT32)now.frac;
}

struct OSAPI_System*
OSAPI_System_get_instance(void)
{
    return &OSAPI_System_g._parent;
}

#endif









