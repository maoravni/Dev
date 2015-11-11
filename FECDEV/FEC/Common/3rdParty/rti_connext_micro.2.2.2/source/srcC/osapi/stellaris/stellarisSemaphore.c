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

#include "osapi/osapi_heap.h"
#include "osapi/osapi_mutex.h"
#include "osapi/osapi_semaphore.h"

struct OSAPI_Semaphore
{
    RTI_UINT32 count;
};

static struct OSAPI_Semaphore OSAPI_Semaphore_g_shared = { 0 };
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS
void
OSAPI_Semaphore_delete(struct OSAPI_Semaphore *self)
{
}

struct OSAPI_Semaphore *
OSAPI_Semaphore_new(void)
{
    return &OSAPI_Semaphore_g_shared;
}

RTI_BOOL
OSAPI_Semaphore_take(struct OSAPI_Semaphore *self,
                    RTI_INT32 timeoutMs,
                    RTI_INT32 *failReason)
{
    if (self != &OSAPI_Semaphore_g_shared)
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
OSAPI_Semaphore_give(struct OSAPI_Semaphore *self)
{
    if (self != &OSAPI_Semaphore_g_shared)
    {
        return RTI_FALSE;
    }
    return RTI_TRUE;
}

#endif
