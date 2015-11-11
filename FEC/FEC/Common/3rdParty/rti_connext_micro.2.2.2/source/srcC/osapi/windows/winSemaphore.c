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
#include <limits.h>
#include <windows.h>

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
    HANDLE handle;              /* native OS representation */
};

#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS
RTI_BOOL
OSAPI_Semaphore_delete(OSAPI_Semaphore_T *me)
{
    DWORD ec;

    LOG_PRECONDITION(me == NULL,return RTI_FALSE)

    if (!CloseHandle(me->handle))
    {
        ec = GetLastError();
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_DELETE_FAILED,ec);
        return RTI_FALSE;

    }

    OSAPI_Heap_freeStructure(me);

    return RTI_TRUE;
}

OSAPI_Semaphore_T*
OSAPI_Semaphore_new(void)
{
    struct OSAPI_Semaphore *me;

    OSAPI_Heap_allocateStructure(&me, struct OSAPI_Semaphore);

    if (me == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_SEMAPHORE_NEW_FAILED);
        return NULL;
    }

    me->handle = CreateSemaphore(NULL,  /* security attributes */
                                 0,       /* empty */
                                 1,       /* maximum count */
                                 NULL /* unamed */ );
    if (me->handle != NULL)
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
OSAPI_Semaphore_take(OSAPI_Semaphore_T *me,
                    RTI_INT32 timeoutMs,
                    RTI_INT32 *failReason)
{
    DWORD status;
    RTI_BOOL result;

    LOG_PRECONDITION(me == NULL,return RTI_FALSE)

    status = WaitForSingleObject(me->handle, timeoutMs);

    switch (status)
    {
        /* WAIT_ABANDONED means the thread owing the mutex exited
         * This should not happen
         */
        case WAIT_ABANDONED:
            OSAPI_LOG_WARNING_P1(OSAPI_LOG_SEMAPHORE_TAKE_FAILED,result)
        case WAIT_OBJECT_0:
            result = RTI_TRUE;
            break;
        default:
			result = RTI_FALSE;
    }

    if (failReason)
    {
        *failReason = status;
    }

    return result;
}

RTI_BOOL
OSAPI_Semaphore_give(OSAPI_Semaphore_T *me)
{
    DWORD e;
	LONG pc=0;

    LOG_PRECONDITION(me == NULL,return RTI_FALSE)

    /* Too many posts are ok for notification NT returns ERROR_TOO_MANY_POSTS */
    if (!ReleaseSemaphore(me->handle, 1, &pc))
    {
        e = GetLastError();
        if (e != ERROR_TOO_MANY_POSTS)
        {
            OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_GIVE_FAILED,e);
            return RTI_FALSE;
        }
	}

    return RTI_TRUE;
}

#endif
