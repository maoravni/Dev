/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef freertosSemaphore_pkg_h
#define freertosSemaphore_pkg_h

extern void
OSAPI_Semaphore_delete(struct OSAPI_Semaphore *self);

struct OSAPI_Semaphore *
OSAPI_Semaphore_new(void);

extern RTI_BOOL
OSAPI_Semaphore_take(struct OSAPI_Semaphore *self, int timeoutMs, int *failReason);

extern RTI_BOOL
OSAPI_Semaphore_give(struct OSAPI_Semaphore *self);

#endif /* freertosSemaphore_pkg_h */
