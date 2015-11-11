/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rt_log_h
#define rt_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#define RT_LOG_REGISTRY_SET_IMMUTABLE_PROPERTY  1
#define RT_LOG_REGISTRY_INIT_FAILURE            2
#define RT_LOG_REGISTRY_ENTRY_EXISTS            3
#define RT_LOG_REGISTRY_REGISTER_FAILURE        4



#include "rt/rt_log_impl.h"

#endif /* rt_log_h */
