/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef winThread_pkg_h
#define winThread_pkg_h

struct OSAPI_Thread *
OSAPI_Thread_create(const char *name,
                   const struct OSAPI_ThreadProperties *properties,
                   OSAPI_ThreadRoutine userRoutine,
                   void *userParam, OSAPI_ThreadRoutine wakeupRoutine);

#endif /* winThread_pkg_h */
