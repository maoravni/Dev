/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_log_h
#define reda_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#define REDA_LOG_BUFFERPOOL_OUT_OF_RESOURCES                    1
#define REDA_LOG_BUFFERPOOL_BUFFER_INITIALIZATION_FAILED        2

#define REDA_LOG_SEQUENCE_ALLOC_FAILED                          3
#define REDA_LOG_SEQUENCE_COPY_FAILED                           4
#define REDA_LOG_SEQUENCE_SET_MAX_FAILED                        5
#define REDA_LOG_SEQUENCE_INVALID_COMPARE                       6

#define REDA_LOG_STRING_ALLOC_FAILED                            7



#include "reda/reda_log_impl.h"

#endif /* reda_log_h */
