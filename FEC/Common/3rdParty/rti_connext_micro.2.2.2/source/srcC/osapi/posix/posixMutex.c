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

#ifdef INCLUDE_POSIX

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>

#define RTI_INVALID_TID -1

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
    pthread_mutex_t *mutex;
};
#endif

/*** SOURCE_BEGIN ***/

#ifdef INCLUDE_POSIX

RTI_BOOL
OSAPI_Mutex_delete(OSAPI_Mutex_T *self)
{
    int rc;

    LOG_PRECONDITION(self == NULL,return RTI_FALSE)

    rc = pthread_mutex_destroy(self->mutex);
    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_DELETE_FAILED,rc);
        return RTI_FALSE;
    }

    if (self->mutex)
    {
        OSAPI_Heap_freeStructure(self->mutex);
    }

    OSAPI_Heap_freeStructure(self);

    return RTI_TRUE;
}

OSAPI_Mutex_T*
OSAPI_Mutex_new(void)
{
    pthread_mutexattr_t attr;
    struct OSAPI_Mutex *mutex = NULL;
    int rc;

    OSAPI_Heap_allocateStructure(&mutex, struct OSAPI_Mutex);

    if (mutex == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_MUTEX_NEW_FAILED);
        return NULL;
    }
#if defined(RTI_LINUX)
#define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
#endif

    mutex->mutex = NULL;

    if (!pthread_mutexattr_init(&attr) &&
        !pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE))
    {
        OSAPI_Heap_allocateStructure(&mutex->mutex, pthread_mutex_t);
    }

    if (mutex->mutex)
    {
        rc = pthread_mutex_init(mutex->mutex, &attr);
        if ( rc != 0)
        {
            OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_NEW_FAILED,rc);
            (void)OSAPI_Mutex_delete(mutex);
            return NULL;
        }
    }
    else
    {
        OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_NEW_FAILED,errno);
        (void)OSAPI_Mutex_delete(mutex);
        return NULL;
    }

    rc = pthread_mutexattr_destroy(&attr);
    if (rc != 0)
    {
        OSAPI_LOG_WARNING_P1(OSAPI_LOG_MUTEX_NEW_FAILED,rc)
    }

    return (OSAPI_Mutex_T*)mutex;
}

RTI_BOOL
OSAPI_Mutex_take(OSAPI_Mutex_T *self)
{
    int rc;

    LOG_PRECONDITION(self == NULL,return RTI_FALSE)

    rc = pthread_mutex_lock(self->mutex);
    if (rc == EINTR)
    {
        printf("-4 take\n");
    }

    if (rc == 0)
    {
        return RTI_TRUE;
    }

    OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_TAKE_FAILED,rc);

    return RTI_FALSE;
}

RTI_BOOL
OSAPI_Mutex_give(OSAPI_Mutex_T *self)
{
    int rc;

    LOG_PRECONDITION(self == NULL,return RTI_FALSE)

    rc = pthread_mutex_unlock(self->mutex);
    if (rc == EINTR)
    {
        printf("-4 give\n");
    }

    if (rc == 0)
    {
        return RTI_TRUE;
    }

    OSAPI_Log_error_p1(OSAPI_LOG_MUTEX_GIVE_FAILED,rc);

    return RTI_FALSE;
}
#endif
