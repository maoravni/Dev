/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_config_h
#define dds_c_config_h



/* The global configuration is read first. Local definitions must
 * check if the global configuration has set an option or not. Note that
 * a module cannot disable a feature.
 */
#ifdef HAVE_GLOBAL_CONFIG
#include "rti_me_config.h"
#endif

/* RTPS_RELIABILITY
 *
 * Set RTPS_RELIABILITY to enable reliable RTPS communication.
 *
 */
#ifndef RTPS_RELIABILITY
#define RTPS_RELIABILITY  1
#endif

#ifndef INCLUDE_API_LOOKUP
#define INCLUDE_API_LOOKUP 1
#endif

#ifndef INCLUDE_API_QOS
#define INCLUDE_API_QOS 1
#endif

#ifndef ENABLE_STATUS_LISTENER
#define ENABLE_STATUS_LISTENER 1
#endif

#ifndef ENABLE_QOS_DEADLINE
#define ENABLE_QOS_DEADLINE 1
#endif

#ifndef INCLUDE_API_LISTENER
#define INCLUDE_API_LISTENER 1
#endif

#ifndef INCLUDE_API_INSTANCE
#define INCLUDE_API_INSTANCE 1
#endif

#ifndef ENABLE_READER_STATUS
#define ENABLE_READER_STATUS 1
#endif

#ifndef INCLUDE_API_DELETE
#define INCLUDE_API_DELETE 1
#endif


#endif /* dds_c_config_h */
