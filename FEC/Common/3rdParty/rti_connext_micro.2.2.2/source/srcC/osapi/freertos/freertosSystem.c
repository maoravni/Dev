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

#if OSAPI_PLATFORM ==  OSAPI_PLATFORM_FREERTOS

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
//#include <sys/time.h>


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
//#include "../common/System.pkg.h"
#elif CMAKE_BUILD
#include "System.pkg.h"
#else
//#include "../common/h/System.pkg.h"
#endif

#define OSAPISYSTEM_MAX_TIMERS          8
#define OSAPISYSTEM_TIMER_RESOLUTION	portTICK_RATE_MS

struct OSAPI_SystemTimerHandler
{
    OSAPI_TimerTickHandlerFunction handler;
    xTimerHandle timer_handle;
    void *param;
};

struct OSAPI_System
{
    RTI_BOOL is_initialized;
    struct OSAPI_SystemTimerHandler timer_handler[OSAPISYSTEM_MAX_TIMERS];
    RTI_BOOL thread_exit;
    RTI_BOOL thread_exited;
    xTaskHandle timer_tid;
    RTI_INT32 timer_count;
};

RTI_PRIVATE struct OSAPI_System OSAPI_System_g = { RTI_FALSE };

RTI_PRIVATE struct OSAPI_System *OSAPI_System_g_system = &OSAPI_System_g;

static int initLock = 0; /* lock for timer initialization */
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM ==  OSAPI_PLATFORM_FREERTOS

RTI_BOOL
OSAPI_System_get_time(OSAPI_NtpTime * now)
{
#if 0   /* not portable if gettimeofday is unavailable */
    struct timeval currentTime;

    if (gettimeofday(&currentTime, NULL) == -1)
    {
        OSAPILog_write(LOG_RTI_ERROR | LOG_OSAPI | LOG_FAIL_GET_CLOCK_TIME,
                       NULL);
        return;
    }

    OSAPI_NtpTime_packFromMicrosec(now, currentTime.tv_sec, currentTime.tv_usec);
#endif
    return RTI_TRUE;
}

void
OSAPI_System_on_timer_callback(xTimerHandle timerHandle)
{
RTI_INT32 i = 0;

    if (!OSAPI_System_g_system->is_initialized)
    {
        return;
    }

    i = (RTI_INT32) pvTimerGetTimerID(timerHandle);

    /* check valid index */
    if (i < 0 ||
        i >= OSAPISYSTEM_MAX_TIMERS ||
        timerHandle != OSAPI_System_g_system->timer_handler[i].timer_handle)
    {
        return;
    }

    /* execute timer handler */
    if (OSAPI_System_g_system->timer_handler[i].handler)
    {
        OSAPI_System_g_system->timer_handler[i].handler(OSAPI_System_g_system->
                                                       timer_handler[i].param);
    }
}



void
OSAPI_SystemFreeRtos_initialize(struct OSAPI_System *self)
{
    if (!self->is_initialized)
    {
        while (__sync_lock_test_and_set(&initLock, 1)) ;

        if (!self->is_initialized)
        {
    RTI_INT32 i;
            for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
            {
                self->timer_handler[i].handler = NULL;
                self->timer_handler[i].param = NULL;
                self->timer_handler[i].timer_handle = NULL;
            }
            self->thread_exited = RTI_FALSE;
            self->thread_exit = RTI_FALSE;
            self->timer_count = 0;
            self->is_initialized = RTI_TRUE;
            initLock = 0;
        }
    }
}

RTI_PRIVATE void
OSAPI_SystemFreeRtos_finalize(struct OSAPI_System *self)
{
    RTI_INT32 i;

    for (i = 0; self->timer_count && (i < OSAPISYSTEM_MAX_TIMERS); ++i)
    {
        if (self->timer_handler[i].handler != NULL)
        {
            /* stop, delete timer */
        }
        self->timer_handler[i].handler = NULL;
        self->timer_handler[i].param = NULL;
        --self->timer_count;
    }
}

RTI_BOOL
OSAPI_System_start_timer(OSAPI_Timer_t self,
                        OSAPI_TimerTickHandlerFunction tick_handler)
{
RTI_INT32 i;
RTI_BOOL ok = RTI_FALSE;

    traceDEBUG_LINE(__LINE__);
    OSAPI_SystemFreeRtos_initialize(OSAPI_System_g_system);

    while (__sync_lock_test_and_set(&initLock, 1)) ;

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        if (OSAPI_System_g_system->timer_handler[i].handler == NULL)
        {
            break;
        }
    }
    if (i == OSAPISYSTEM_MAX_TIMERS)
    {
        goto done;
    }

    OSAPI_System_g_system->timer_handler[i].handler = tick_handler;
    OSAPI_System_g_system->timer_handler[i].param = self;

    OSAPI_System_g_system->timer_handler[i].timer_handle =
        xTimerCreate("", OSAPISYSTEM_TIMER_RESOLUTION, pdTRUE /* periodic */ ,
                     (void *)i, OSAPI_System_on_timer_callback);
    if (OSAPI_System_g_system->timer_handler[i].timer_handle == NULL)
    {
        goto done;
    }

    if (pdTRUE !=
        xTimerStart(OSAPI_System_g_system->timer_handler[i].timer_handle,
                    portMAX_DELAY))
    {
        goto done;
    }

    ++OSAPI_System_g_system->timer_count;

    ok = RTI_TRUE;

    done:
    initLock = 0;
    return ok;
}

RTI_BOOL
OSAPI_System_stop_timer(OSAPI_Timer_t self)
{
RTI_INT32 i;
RTI_BOOL ok = RTI_FALSE;

    if (!OSAPI_System_g_system->is_initialized)
    {
        return RTI_FALSE;
    }

    while (__sync_lock_test_and_set(&initLock, 1)) ;

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        if (OSAPI_System_g_system->timer_handler[i].param == self)
        {
            if (pdTRUE !=
                xTimerStop(OSAPI_System_g_system->timer_handler[i].timer_handle,
                           portMAX_DELAY))
            {
                goto done;
            }
            if (pdTRUE !=
                xTimerDelete(OSAPI_System_g_system->timer_handler[i].
                             timer_handle, portMAX_DELAY))
            {
                goto done;
            }
            OSAPI_System_g_system->timer_handler[i].timer_handle = NULL;
            OSAPI_System_g_system->timer_handler[i].handler = NULL;
            OSAPI_System_g_system->timer_handler[i].param = NULL;
            --OSAPI_System_g_system->timer_count;
        }
    }
    ok = RTI_TRUE;
    done:
    initLock = 0;
    return ok;
}

RTI_INT32
OSAPI_System_get_timer_resolution(void)
{
    return OSAPISYSTEM_TIMER_RESOLUTION;
}

RTI_BOOL
OSAPI_System_initialize(void)
{
    OSAPI_SystemFreeRtos_initialize(OSAPI_System_g_system);
    return RTI_TRUE;
}

RTI_BOOL
OSAPI_System_finalize(void)
{
    OSAPI_SystemFreeRtos_finalize(OSAPI_System_g_system);
}

void
OSAPI_System_generate_uuid(struct OSAPI_SystemUUID *uuid_out)
{
    /* TODO: use system-specific uuid generation */
    OSAPI_NtpTime now;
    OSAPI_System_get_time(&now);
    uuid_out->value[0] = (RTI_UINT32)now.sec;
    uuid_out->value[1] = (RTI_UINT32)now.frac;
    uuid_out->value[2] = (RTI_UINT32)&OSAPI_System_g;
    uuid_out->value[3] = (RTI_UINT32)now.frac;
}

#endif
