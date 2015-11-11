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
#include <vxWorks.h>
#include <semLib.h>

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
    SEM_ID mutexId;
};
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS
RTI_BOOL
OSAPI_Mutex_delete(struct OSAPI_Mutex *mutex)
{
    /* VxWorks Cert subset API does not include semDelete */
#ifndef RTI_CERT
    STATUS status;

    LOG_PRECONDITION((mutex == NULL),return RTI_FALSE)

    if (mutex->mutexId != NULL)
    {
        status = semDelete(mutex->mutexId);
        if (status != OK)
        {
            OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_DELETE_FAILED,status);
            return RTI_FALSE;
        }
    }

    OSAPI_Heap_freeStructure(mutex);
#endif
    return RTI_TRUE;
}


struct OSAPI_Mutex*
OSAPI_Mutex_new(void)
{
    struct OSAPI_Mutex *mutex = NULL;

    OSAPI_Heap_allocateStructure(&mutex, struct OSAPI_Mutex);

    if (mutex == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_MUTEX_NEW_FAILED);
        return NULL;
    }

    mutex->mutexId = semMCreate(SEM_Q_FIFO);
    if (mutex->mutexId == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_MUTEX_NEW_FAILED);
        (void)OSAPI_Mutex_delete(mutex);
        return NULL;
    }

    return mutex;
}


RTI_BOOL
OSAPI_Mutex_take(struct OSAPI_Mutex *mutex)
{
    STATUS status;

    LOG_PRECONDITION(mutex == NULL,return RTI_FALSE)

    status = semTake(mutex->mutexId, WAIT_FOREVER);
    if (status == OK)
    {
        return RTI_TRUE;
    }

    OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_TAKE_FAILED,status);

    return RTI_FALSE;
}


RTI_BOOL
OSAPI_Mutex_give(struct OSAPI_Mutex *mutex)
{
    STATUS status;

    LOG_PRECONDITION(mutex == NULL,return RTI_FALSE)

    status = semGive(mutex->mutexId);
    if (status == OK)
    {
        return RTI_TRUE;
    }

    OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_GIVE_FAILED,status);

    return RTI_FALSE;
}

#endif
