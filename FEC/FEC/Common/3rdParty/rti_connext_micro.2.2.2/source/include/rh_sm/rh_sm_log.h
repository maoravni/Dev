/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rh_sm_log_h
#define rh_sm_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#define LOG_OUTSTANDING_SAMPLES                                      0x00000186
#define LOG_KEEP_ALL_HISTORY_NOT_SUPPORTED                           0x00000187
#define LOG_UNLIMITED_HISTORY_NOT_SUPPORTED                          0x00000188
#define LOG_MAX_SAMPLES_LESS_THAN_MAX_INSTANCES_TIMES_MAX_SAMPERINST 0x00000189
#define LOG_EXCLUSIVE_OWNERSHIP_NOT_SUPPORTED                        0x0000018a
#define LOG_FINITE_DEADLINE_NOT_SUPPORTED                            0x0000018b
#define LOG_FINITE_TIME_BASED_FILTER_NOT_SUPPORTED                   0x0000018c
#define LOG_RHSM_PROPERTY_NOT_INITIALIZED                            0x0000018d



#include "rh_sm/rh_sm_log_impl.h"

#endif /* rh_sm_log_h */
