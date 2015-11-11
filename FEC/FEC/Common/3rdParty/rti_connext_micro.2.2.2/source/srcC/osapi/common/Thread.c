/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#include "osapi/osapi_thread.h"
#include "osapi/osapi_types.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_semaphore.h"
#include "osapi/osapi_log.h"

struct OSAPI_Thread
{
    OSAPI_ThreadHandle threadHandle;
    struct OSAPI_Semaphore *createSem;   /* to indicate successful creation of
                                         * thread */
    struct OSAPI_Semaphore *startSem;    /* pended on before userRoutine is run */
    OSAPI_ThreadRoutine userRoutine;     /* user's main routine to actually run */
    OSAPI_ThreadRoutine wakeupRoutine;   /* if needed, user supplied routine
                                         * used to unblock/wakeup 'userRoutine' 
                                         */
    struct OSAPI_ThreadInfo threadInfo;
};

RTI_BOOL OSAPI_Thread_g_return_NULL = RTI_FALSE;

/*** SOURCE_BEGIN ***/

void
OSAPI_Thread_delete(struct OSAPI_Thread *me)
{

    LOG_PRECONDITION(me == NULL,return)

    if (me->createSem)
    {
        OSAPI_Semaphore_delete(me->createSem);
    }

    if (me->startSem)
    {
        OSAPI_Semaphore_delete(me->startSem);
    }

    OSAPI_Heap_freeStructure(me);
}

struct OSAPI_Thread*
OSAPI_Thread_new(void)
{
    struct OSAPI_Thread *me;

    OSAPI_Heap_allocateStructure(&me, struct OSAPI_Thread);

    if (me == NULL)
    {
        return NULL;
    }

    me->threadHandle = 0;
    me->createSem = OSAPI_Semaphore_new();
    me->startSem = OSAPI_Semaphore_new();

    if (!me->createSem || !me->startSem)
    {
        OSAPI_Log_error_p4(OSAPI_LOG_THREAD_NEW_FAILED,
            OSAPI_LOG_PTR_HIGH(me->createSem),OSAPI_LOG_PTR_LOW(me->createSem),
            OSAPI_LOG_PTR_HIGH(me->startSem),OSAPI_LOG_PTR_LOW(me->startSem));
        OSAPI_Thread_delete(me);
        return NULL;
    }

    me->userRoutine = NULL;
    me->wakeupRoutine = NULL;

    me->threadInfo.stopThread = RTI_FALSE;
    me->threadInfo.userParam = NULL;

    return me;
}

/*
    this actually gets spawned when create() is called. we wrap user's routine w/in
    here to gain added control such as entrance/exit signals, pending, etc
*/
void*
OSAPI_Thread_exec(void *param)
{
    struct OSAPI_Thread *me = (struct OSAPI_Thread *)param;

    /* give semaphore to signal that we're spawned */
    if (!OSAPI_Semaphore_give(me->createSem))
    {
        OSAPI_Log_error(OSAPI_LOG_THREAD_EXEC_FAILED);
        return NULL;
        /* TODO: what else to do? */
    }

    /* take semaphore before we can begin */
    if (!OSAPI_Semaphore_take(me->startSem, OSAPI_SEMAPHORE_TIMEOUT_INFINITE, NULL))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_THREAD_EXEC_FAILED,
             OSAPI_LOG_PTR_HIGH(me->startSem),OSAPI_LOG_PTR_LOW(me->startSem));
        return NULL;
    }

    /* here we actually run the user's function */
    me->userRoutine(&me->threadInfo);

    /* give again as signal that we're really leaving */
    if (!OSAPI_Semaphore_give(me->createSem))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_THREAD_EXEC_FAILED,
             OSAPI_LOG_PTR_HIGH(me->createSem),OSAPI_LOG_PTR_LOW(me->createSem));
        return NULL;
        /* TODO: what else to do? */
    }

    return NULL;
}

/*
    call the user supplied wakeup function (if available) to unblock user's main routine
*/
RTI_BOOL
OSAPI_Thread_wakeup(struct OSAPI_Thread *me)
{
    LOG_PRECONDITION(me == NULL,return RTI_FALSE;)

    if (me->wakeupRoutine)
    {
        return me->wakeupRoutine(&me->threadInfo);
    }

    return RTI_TRUE;
}

/*
    when create() is called, preparation to execute the user routine is performed, but
    not yet run. user routine begins running immediately only when start() is called
*/
RTI_BOOL
OSAPI_Thread_start(struct OSAPI_Thread *me)
{
    LOG_PRECONDITION(me == NULL,return RTI_FALSE;)

    if (!OSAPI_Semaphore_give(me->startSem))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_THREAD_START_FAILED,
             OSAPI_LOG_PTR_HIGH(me->startSem),OSAPI_LOG_PTR_LOW(me->startSem));
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

/*
    destroy the thread. this includes terminating user's routine, killing the spawned
    thread, and deallocating all associated memory. thread exit is guaranteed when
    this function return RTI_TRUE

    note: user's routine must cooperate by checking threadInfo.stopThread and exit when
        it becomes RTI_TRUE
*/
RTI_BOOL
OSAPI_Thread_destroy(struct OSAPI_Thread *me)
{
    LOG_PRECONDITION(me == NULL,return RTI_FALSE;)

    /* start the thread if it was never started, otherwise it's a noop */
    if (!OSAPI_Thread_start(me))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_THREAD_DESTROY_FAILED,
             OSAPI_LOG_PTR_HIGH(me),OSAPI_LOG_PTR_LOW(me));
        return RTI_FALSE;
    }
    /* set the stop flag to true */
    me->threadInfo.stopThread = RTI_TRUE;
    /* wake up the thread, who should notice the stop flag state and exit */
    if (!OSAPI_Thread_wakeup(me))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_THREAD_DESTROY_FAILED,
             OSAPI_LOG_PTR_HIGH(me),OSAPI_LOG_PTR_LOW(me));
        return RTI_FALSE;
    }
    /* pend on semaphore to guarantee thread exit */
    if (!OSAPI_Semaphore_take(me->createSem,
                             OSAPI_SEMAPHORE_TIMEOUT_INFINITE, NULL))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_THREAD_DESTROY_FAILED,
             OSAPI_LOG_PTR_HIGH(me->createSem),OSAPI_LOG_PTR_LOW(me->createSem));
        return RTI_FALSE;
    }

    OSAPI_Thread_delete(me);

    return RTI_TRUE;
}
