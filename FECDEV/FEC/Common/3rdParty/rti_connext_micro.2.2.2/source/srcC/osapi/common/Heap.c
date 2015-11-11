/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#include "osapi/osapi_config.h"
#include "util/util_makeheader.h"
#include "osapi/osapi_dll.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_types.h"

RTI_SIZE_T OSAPI_Heap_g_allocatedByteCount = 0;

/*** SOURCE_BEGIN ***/

RTI_SIZE_T
OSAPI_Heap_getAllocatedByteCount(void)
{
    return OSAPI_Heap_g_allocatedByteCount;
}
