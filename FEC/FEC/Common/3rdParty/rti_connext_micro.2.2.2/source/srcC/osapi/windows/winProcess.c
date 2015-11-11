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

#if OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS

#include <Windows.h>
#include "osapi/osapi_process.h"

#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS

/* ----------------------------------------------------------------- */
RTI_UINT32
OSAPI_Process_getId(void)
{
    return (RTI_UINT32)GetCurrentProcessId();
}

#endif
