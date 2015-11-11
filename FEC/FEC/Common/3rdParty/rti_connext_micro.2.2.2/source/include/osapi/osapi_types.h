/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_types_h
#define osapi_types_h



#include "osapi/osapi_config.h"

#include "osapi/osapi_types_impl.h"

typedef RTI_INT32 RTI_BOOL;

#define RTI_FALSE   ((RTI_BOOL) 0)
#define RTI_TRUE    ((RTI_BOOL) 1)

#ifdef HAVE_SIZE_T
typedef size_t RTI_SIZE_T;
#define RTI_SIZE_INVALID 0xffffffff
#else
typedef RTI_UINT32 RTI_SIZE_T;
#define RTI_SIZE_INVALID 0xffffffff
#endif


#endif /* osapi_types_h */
