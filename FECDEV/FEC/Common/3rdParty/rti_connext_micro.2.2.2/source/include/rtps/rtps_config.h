/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rtps_config_h
#define rtps_config_h



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


#endif /* rtps_config_h */
