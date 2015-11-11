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

#if OSAPI_PLATFORM ==  OSAPI_PLATFORM_WINDOWS

#include <stdlib.h>
#include <windows.h>

#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef osapi_thread_h
#include "osapi/osapi_thread.h"
#endif
#ifndef osapi_system_h
#include "osapi/osapi_system.h"
#endif
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#ifdef BUILDABLE_SOURCE
#include "../common/System.pkg.h"
#else
#include "System.pkg.h"
#endif

#define OSAPISYSTEM_MAX_TIMERS          8
#define OSAPISYSTEM_TIMER_RESOLUTION    10

struct OSAPI_SystemTimerHandler
{
    OSAPI_TimerTickHandlerFunction handler;
    void *param;
};

struct OSAPI_SystemWin
{
    struct OSAPI_System _parent;
    struct OSAPI_SystemTimerHandler timer_handler[OSAPISYSTEM_MAX_TIMERS];
    RTI_INT32 timer_count;
    struct OSAPI_Mutex *mutex;
    struct OSAPI_Thread *timer_thread;
};

RTI_PRIVATE struct OSAPI_SystemWin OSAPI_System_g = { OSAPI_System_INITIALIZER };
RTI_PRIVATE struct OSAPI_SystemWin *OSAPI_System_g_system = &OSAPI_System_g;
RTI_UINT32 OSAPI_System_g_size = sizeof(struct OSAPI_SystemWin);

#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS

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
OSAPI_SystemWin_timer_thread(struct OSAPI_ThreadInfo *thread_param)
{
	struct OSAPI_SystemWin *self = (struct OSAPI_SystemWin *)thread_param->userParam;
    RTI_INT32 i, j;

    while (!thread_param->stopThread)
    {
        (void)OSAPI_System_lock();
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
        Sleep(OSAPISYSTEM_TIMER_RESOLUTION);
    }

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
OSAPI_SystemWin_initialize(struct OSAPI_SystemWin *self)
{
    RTI_INT32 i;
    LONG prev_value;

    if (self->_parent.is_initialized)
    {
        return RTI_TRUE;
    }

    prev_value = InterlockedExchange(
                        &OSAPI_System_g_system->_parent.is_initialized,1);
    if (prev_value != 0)
    {
        while (!OSAPI_System_g_system->_parent.is_initialized);
        return RTI_TRUE;
    }

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        self->timer_handler[i].handler = NULL;
        self->timer_handler[i].param = NULL;
    }

    self->timer_count = 0;

    self->mutex = OSAPI_Mutex_new();
    if (self->mutex == NULL)
    {
         return RTI_FALSE;
    }

    self->timer_thread = OSAPI_Thread_create("osapi_timer",
                    &self->_parent.property.timer_property.thread,
                    OSAPI_SystemWin_timer_thread,(void *)self,NULL);

    if (self->timer_thread == NULL)
    {
        return RTI_FALSE;
    }

    if (!OSAPI_Thread_start(self->timer_thread))
    {
        return RTI_FALSE;
    }

    self->_parent.is_initialized = RTI_TRUE;

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
OSAPI_SystemWin_finalize(struct OSAPI_SystemWin *self)
{
    RTI_INT32 i;

    if (!self->_parent.is_initialized)
    {
        return RTI_FALSE;
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
#define RTIDRT_EPOCHFILETIME (116444736000000000i64)
    FILETIME ft;
    ULARGE_INTEGER li;
    __int64 t;
    RTI_INT32 sec;
    RTI_INT32 usec;

    LOG_PRECONDITION(now == NULL,return RTI_FALSE)

    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    t = li.QuadPart;
    t -= RTIDRT_EPOCHFILETIME;
    t /= 10;
    sec = (long)(t / 1000000);
    usec = (long)(t % 1000000);

    OSAPI_NtpTime_packFromMicrosec(now, sec, usec);
#undef RTIDRT_EPOCHFILETIME

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
        if (!OSAPI_SystemWin_initialize(OSAPI_System_g_system))
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
    return OSAPISYSTEM_TIMER_RESOLUTION;
}

RTI_BOOL
OSAPI_System_initialize(void)
{
    return OSAPI_SystemWin_initialize(OSAPI_System_g_system);
}

RTI_BOOL
OSAPI_System_finalize(void)
{
    return OSAPI_SystemWin_finalize(OSAPI_System_g_system);
}

void
OSAPI_System_generate_uuid(struct OSAPI_SystemUUID *uuid_out)
{
    OSAPI_NtpTime now;

    LOG_PRECONDITION(uuid_out == NULL,return)

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
