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
//#include "drivers/kitronix320x240x16_ssd2119_8bit.h"
#include "drivers/set_pinout.h"

#include "osapi/osapi_heap.h"
#include "osapi/osapi_mutex.h"
#include "osapi/osapi_system.h"
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#ifdef BUILDABLE_SOURCE
#include "../common/System.pkg.h"
#elif CMAKE_BUILD
#include "System.pkg.h"
#else
#include "../common/h/System.pkg.h"
#endif

#define OSAPISYSTEM_MAX_TIMERS          8
#define OSAPISYSTEM_TIMER_RESOLUTION    10

struct OSAPI_SystemTimerHandler
{
    OSAPI_TimerTickHandlerFunction handler;
    void *param;
};

struct OSAPI_SystemStellaris
{
    struct OSAPI_System _parent;
    struct OSAPI_SystemTimerHandler timer_handler[OSAPISYSTEM_MAX_TIMERS];
    RTI_BOOL thread_exit;
    RTI_BOOL thread_exited;
    RTI_INT32 timer_count;
    RTI_INT32 tick_high;
    RTI_UINT32 tick_low;
};


RTI_PRIVATE struct OSAPI_System OSAPI_System_g = { RTI_FALSE };

RTI_PRIVATE struct OSAPI_System *OSAPI_System_g_system = &OSAPI_System_g;

#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS

void
OSAPI_System_get_time(OSAPI_NtpTime * now)
{
    RTI_INT32 sec1,sec2;
    RTI_UINT32 msec;

    sec1 = OSAPI_System_g_system->tick_high;
    msec = OSAPI_System_g_system->tick_low * OSAPISYSTEM_TIMER_RESOLUTION;
    sec2 = OSAPI_System_g_system->tick_high;

    /* Always move forward */
    if (sec1 == sec2)
    {
        OSAPI_NtpTime_packFromMillisec(now, sec1, msec);
    }
    else
    {
        OSAPI_NtpTime_packFromMillisec(now, sec2, 0);
    }
}

RTI_PRIVATE void
OSAPI_System_timer_thread(struct OSAPI_System *self)
{
    RTI_INT32 i, j;

    for (i = 0, j = self->timer_count; (i < OSAPISYSTEM_MAX_TIMERS) && j; ++i)
    {
        if (self->timer_handler[i].handler)
        {
            j--;
            self->timer_handler[i].handler(self->timer_handler[i].param);
        }
    }
    ++self->tick_low;
    if (self->tick_low == (1000 / OSAPISYSTEM_TIMER_RESOLUTION))
    {
        ++self->tick_high;
        self->tick_low = 0;
    }
}

void
OSAPI_System_initialize(struct OSAPI_System *self)
{
    RTI_INT32 i;

    IntMasterDisable();

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        self->timer_handler[i].handler = NULL;
        self->timer_handler[i].param = NULL;
    }
    self->thread_exited = RTI_FALSE;
    self->thread_exit = RTI_FALSE;
    self->timer_count = 0;
    self->tick_high = 0;
    self->tick_low = 0;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    //IntMasterEnable();
    TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
    TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 100);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
    OSAPI_System_g_system->is_initialized = RTI_TRUE;
    IntMasterEnable();
}

void
OSAPI_System_finalize(struct OSAPI_System *self)
{
    RTI_INT32 i;

    self->thread_exit = RTI_TRUE;

    while (!self->thread_exited) ;

    for (i = 0; i < OSAPISYSTEM_MAX_TIMERS; ++i)
    {
        self->timer_handler[i].handler = NULL;
        self->timer_handler[i].param = NULL;
    }
    self->thread_exited = RTI_FALSE;
    self->thread_exit = RTI_FALSE;
    self->timer_count = 0;
}

volatile int g_sleep = 0;

void
SleepIntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    if (g_sleep)
    {
        --g_sleep;
    }

    OSAPI_System_timer_thread(OSAPI_System_g_system);
}

RTI_BOOL
OSAPI_System_start_timer(OSAPI_Timer_t self,
                        OSAPI_TimerTickHandlerFunction tick_handler)
{
    RTI_INT32 i;

    if (!OSAPI_System_g_system->is_initialized)
    {
        if (!OSAPI_System_g_system->is_initialized)
        {
            OSAPI_System_initialize(OSAPI_System_g_system);
        }
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
        return RTI_FALSE;
    }

    OSAPI_System_g_system->timer_handler[i].handler = tick_handler;
    OSAPI_System_g_system->timer_handler[i].param = self;
    ++OSAPI_System_g_system->timer_count;

    return RTI_TRUE;
}

RTI_BOOL
OSAPI_System_stop_timer(OSAPI_Timer_t self)
{
    RTI_INT32 i;

    if (!OSAPI_System_g_system->is_initialized)
    {
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

    return RTI_TRUE;
}

RTI_INT32
OSAPI_System_get_timer_resolution(void)
{
    return OSAPISYSTEM_TIMER_RESOLUTION;
}

void
OSAPI_System_initialize(void)
{
}

void
OSAPI_System_finalize(void)
{
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
