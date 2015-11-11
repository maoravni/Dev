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

#include "inc/hw_types.h"
#include "driverlib/interrupt.h"

#include "osapi/osapi_types.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_mutex.h"

struct OSAPI_Mutex
{
    RTI_INT8 lock;
};

static struct OSAPI_Mutex OSAPI_Mutex_g_shared = { 0 };
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS
RTI_BOOL
OSAPI_Mutex_delete(struct OSAPI_Mutex *mutex)
{
    if (mutex != &OSAPI_Mutex_g_shared)
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

struct OSAPI_Mutex*
OSAPI_Mutex_new(void)
{
    return &OSAPI_Mutex_g_shared;
}

RTI_BOOL
OSAPI_Mutex_take(struct OSAPI_Mutex *mutex)
{
    if (mutex != &OSAPI_Mutex_g_shared)
    {
        return RTI_FALSE;
    }

    IntMasterDisable();

    OSAPI_Mutex_g_shared.lock = 1;

    return RTI_TRUE;
}


RTI_BOOL
OSAPI_Mutex_give(struct OSAPI_Mutex *mutex)
{
    if (mutex != &OSAPI_Mutex_g_shared)
    {
        return RTI_FALSE;
    }

    OSAPI_Mutex_g_shared.lock = 0;

    IntMasterEnable();

    return RTI_TRUE;
}
#endif
