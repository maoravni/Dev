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
#include <limits.h>
#include <sysLib.h>
#include <semLib.h>
#include <tickLib.h>

#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif
#ifndef osapi_semaphore_h
#include "osapi/osapi_semaphore.h"
#endif

struct OSAPI_Semaphore
{
    SEM_ID semId;
};
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS
RTI_BOOL
OSAPI_Semaphore_delete(struct OSAPI_Semaphore *me)
{
     /* VxWorks Cert subset API does not include semDelete */
#ifndef RTI_CERT
    STATUS status;

    LOG_PRECONDITION(me == NULL,return RTI_FALSE)

    status = semDelete(me->semId);
    if (status != OK)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_DELETE_FAILED,status);
        return RTI_FALSE;
    }

    OSAPI_Heap_freeStructure(me);
#endif
    return RTI_TRUE;
}

struct OSAPI_Semaphore*
OSAPI_Semaphore_new(void)
{
    struct OSAPI_Semaphore *me;

    OSAPI_Heap_allocateStructure(&me, struct OSAPI_Semaphore);

    if (me == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_SEMAPHORE_NEW_FAILED);
        return NULL;
    }

    me->semId = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
    if (me->semId)
    {
        return me;
    }

    OSAPI_Log_error(OSAPI_LOG_SEMAPHORE_NEW_FAILED);
    (void)OSAPI_Semaphore_delete(me);
    return NULL;
}

/*
    attempt to take semaphore with the specified timeout (can be inifnite).
    if RTI_TRUE is returned, failReason will contain the error condition (ie, errno, etc).
    failReason is OS-specific (ie, not wrapped around by OSAPI-defined reasons)
*/
RTI_BOOL
OSAPI_Semaphore_take(struct OSAPI_Semaphore *me,
                    RTI_INT32 timeoutMs,
                    RTI_INT32 *failReason)
{
    STATUS result;
    RTI_UINT32 ticks, remainder;

    LOG_PRECONDITION(me == NULL,return RTI_FALSE)

    if (timeoutMs == OSAPI_SEMAPHORE_TIMEOUT_INFINITE)
    {
        ticks = WAIT_FOREVER;
    }
    else
    {
        /* Try to make this safe by keeping remainder. Then multiply remainder w/
         * ticksPerSec before dividing by 1000 */
#define MILLI_TO_1 1000
        ticks = (timeoutMs / MILLI_TO_1) * sysClkRateGet();
        remainder = timeoutMs % MILLI_TO_1;
        ticks += (remainder * sysClkRateGet()) / MILLI_TO_1;
#undef MILLI_TO_1
    }

    result = semTake(me->semId, ticks);
    if (failReason)
    {
        *failReason = result;
    }

    if (result == OK)
    {
        return RTI_TRUE;
    }

    OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_TAKE_FAILED,result);

    return RTI_FALSE;
}

RTI_BOOL
OSAPI_Semaphore_give(struct OSAPI_Semaphore * me)
{
    STATUS status;

    LOG_PRECONDITION(me == NULL,return RTI_FALSE)

    status = semGive(me->semId);
    if (status != OK)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_GIVE_FAILED,status);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

#endif
