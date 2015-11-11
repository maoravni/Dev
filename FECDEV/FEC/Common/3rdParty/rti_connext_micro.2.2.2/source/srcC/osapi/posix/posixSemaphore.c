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

#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

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
    pthread_mutex_t sem_mutex;     /* control mutex for the semaphore */
    pthread_cond_t sem_given; /* wake up threads blocked on empty sem */
    RTI_UINT32 sem_count;          /* count, for non-native semaphores */
};

#endif

/*** SOURCE_BEGIN ***/

#ifdef INCLUDE_POSIX
RTI_BOOL
OSAPI_Semaphore_delete(OSAPI_Semaphore_T *self)
{
    int rc;

    LOG_PRECONDITION(self == NULL,return RTI_FALSE)

    rc = pthread_mutex_destroy(&self->sem_mutex);
    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_DELETE_FAILED,rc);
        return RTI_FALSE;
    }

    rc = pthread_cond_destroy(&self->sem_given);
    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_DELETE_FAILED,rc);
        return RTI_FALSE;
    }

    OSAPI_Heap_freeStructure(self);

    return RTI_TRUE;
}

OSAPI_Semaphore_T*
OSAPI_Semaphore_new(void)
{
    struct OSAPI_Semaphore *me;
    int rc;

    OSAPI_Heap_allocateStructure(&me, struct OSAPI_Semaphore);

    if (me == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_SEMAPHORE_NEW_FAILED);
        return NULL;
    }

    rc = pthread_cond_init(&me->sem_given, NULL);
    if (rc != 0)
    {
        goto failure;
    }

    rc = pthread_mutex_init(&me->sem_mutex, NULL);
    if (rc == 0)
    {
        me->sem_count = 0;
        return me;
    }

failure:
    OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_NEW_FAILED,rc);
    (void)OSAPI_Semaphore_delete(me);
    return NULL;
}

/*
    attempt to take semaphore with the specified timeout (can be inifnite).
    if RTI_TRUE is returned, failReason will contain the error condition
    (ie, errno, etc). failReason is OS-specific (ie, not wrapped around by
    OSAPI-defined reasons)
*/
RTI_BOOL
OSAPI_Semaphore_take(OSAPI_Semaphore_T *self,RTI_INT32 timeoutMs,
                    RTI_INT32 *fail_reason)
{
    int status;
    RTI_BOOL result;
    struct timespec abstime;
    struct timeval currtime;

    LOG_PRECONDITION(self == NULL,return RTI_FALSE)

    if (fail_reason)
    {
        *fail_reason = 0;
    }

    status = pthread_mutex_lock(&self->sem_mutex);
    if (status != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_TAKE_FAILED,status);
        return RTI_FALSE;
    }

    if (self->sem_count > 0)
    {
        /* semaphore already available */
        self->sem_count--;
        result = RTI_TRUE;
    }
    else
    {
        /* semaphore not available, will begin to sleep up to timeout */
        if (timeoutMs == OSAPI_SEMAPHORE_TIMEOUT_INFINITE)
        {
            /* wait forever */
            status = pthread_cond_wait(&self->sem_given, &self->sem_mutex);
        }
        else
        {
            /* calculate how long to wait in terms of absolute time */

            status = gettimeofday(&currtime, NULL);
            if (status == 0)
            {
                RTI_UINT32 nanos =
                    (currtime.tv_usec * OSAPI_NTP_TIME_NSEC_PER_USEC) +
                    ((timeoutMs % OSAPI_NTP_TIME_MSEC_PER_SEC) *
                            OSAPI_NTP_TIME_NSEC_PER_MSEC);

                abstime.tv_sec = (currtime.tv_sec) +
                    (timeoutMs / OSAPI_NTP_TIME_MSEC_PER_SEC) +
                                (nanos / OSAPI_NTP_TIME_NSEC_PER_SEC);
                abstime.tv_nsec = nanos % OSAPI_NTP_TIME_NSEC_PER_SEC;
                /* 
                 * this wouldn't work... we must give secs all its props and take away
                 * from nsecs abstime.tv_sec = (currtime.tv_sec) + (timeoutMs /
                 * MILLI_TO_1); abstime.tv_nsec = (currtime.tv_usec * MICRO_TO_NANO) +
                 * ((timeoutMs % MILLI_TO_1) * MILLI_TO_NANO); */
                status = pthread_cond_timedwait(&self->sem_given,
                                                &self->sem_mutex, &abstime);
            }
        }

        if (status == 0)
        {
            self->sem_count--;
            result = RTI_TRUE;
        }
        else
        {
            if (fail_reason)
            {
                *fail_reason = status;
            }
            OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_TAKE_FAILED,status);
            result = RTI_FALSE;
        }
    }

    status = pthread_mutex_unlock(&self->sem_mutex);

    if (status != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_TAKE_FAILED,status);
        return RTI_FALSE;
    }

    return result;
}

RTI_BOOL
OSAPI_Semaphore_give(OSAPI_Semaphore_T *self)
{
    RTI_BOOL result = RTI_FALSE;
    int rc;

    LOG_PRECONDITION(self == NULL,return RTI_FALSE)

    /* start by locking the control mutex, then verify that we can/need to give
     * the semaphore. */
    rc = pthread_mutex_lock(&self->sem_mutex);
    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_GIVE_FAILED,rc);
        return RTI_FALSE;
    }

    /* Binary semaphore: don't re-increment if full (but return success) */
    if (self->sem_count == 0)
    {
        /* OK, give the semaphore and signal via the condition variable. */
        self->sem_count++;
        rc = pthread_cond_signal(&self->sem_given);
        if (rc != 0)
        {
            OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_GIVE_FAILED,rc);
        }
    }

    rc = pthread_mutex_unlock(&self->sem_mutex);
    if (rc != 0)
    {
        OSAPI_Log_error_p1(OSAPI_LOG_SEMAPHORE_GIVE_FAILED,rc);
        return RTI_FALSE;
    }
    else
    {
        result = RTI_TRUE;
    }

    return result;
}

#endif
