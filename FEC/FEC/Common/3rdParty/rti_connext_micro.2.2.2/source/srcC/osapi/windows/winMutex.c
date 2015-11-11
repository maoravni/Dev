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
#include <windows.h>
#include <malloc.h>

#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

struct OSAPI_Mutex
{
    HANDLE hmutex;
};

#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS

RTI_BOOL
OSAPI_Mutex_delete(struct OSAPI_Mutex *mutex)
{
    DWORD ec;

    LOG_PRECONDITION((mutex == NULL),return RTI_FALSE)

    if (!CloseHandle(mutex->hmutex))
    {
        ec = GetLastError();
        OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_DELETE_FAILED,ec);
        return RTI_FALSE;
    }

    OSAPI_Heap_freeStructure(mutex);

    return RTI_TRUE;
}


struct OSAPI_Mutex *
OSAPI_Mutex_new(void)
{
    struct OSAPI_Mutex *mutex = NULL;
    DWORD ec;

    OSAPI_Heap_allocateStructure(&mutex, struct OSAPI_Mutex);

    if (mutex == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_MUTEX_NEW_FAILED);
        return NULL;
    }

    mutex->hmutex = CreateMutex(NULL, FALSE, NULL);
    if (mutex->hmutex == NULL)
    {
        ec = GetLastError();
        OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_NEW_FAILED,ec);
        (void)OSAPI_Mutex_delete(mutex);
        return NULL;
    }

    return mutex;
}

RTI_BOOL
OSAPI_Mutex_take(struct OSAPI_Mutex *mutex)
{
    DWORD rc;

    LOG_PRECONDITION(mutex == NULL,return RTI_FALSE)

    rc = WaitForSingleObject(mutex->hmutex, INFINITE);
    if (rc == WAIT_OBJECT_0)
    {
        return RTI_TRUE;
    }

    OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_TAKE_FAILED,rc);

    return RTI_FALSE;
}

RTI_BOOL
OSAPI_Mutex_give(struct OSAPI_Mutex *mutex)
{
    DWORD ec;

    LOG_PRECONDITION(mutex == NULL,return RTI_FALSE)

    if (ReleaseMutex(mutex->hmutex))
    {
        return RTI_TRUE;
    }

    ec = GetLastError();

    OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_GIVE_FAILED,ec);

    return RTI_FALSE;
}

#endif
