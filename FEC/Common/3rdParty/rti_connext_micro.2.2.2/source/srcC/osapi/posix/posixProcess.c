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

#ifdef INCLUDE_POSIX

#include <sys/types.h>
#include <unistd.h>

#ifndef osapi_process_h
#include "osapi/osapi_process.h"
#endif

#endif

/*** SOURCE_BEGIN ***/

#ifdef INCLUDE_POSIX
RTI_UINT32
OSAPI_Process_getId(void)
{
    return (RTI_UINT32)getpid();
}
#endif
