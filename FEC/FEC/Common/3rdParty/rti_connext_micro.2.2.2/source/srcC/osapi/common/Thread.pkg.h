/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef Thread_pkg_h
#define Thread_pkg_h

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

extern void
OSAPI_Thread_delete(struct OSAPI_Thread *me);

struct OSAPI_Thread*
OSAPI_Thread_new(void);

extern void*
OSAPI_Thread_exec(void *param);

#endif /* Thread_pkg_h */
