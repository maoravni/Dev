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

#if OSAPI_PLATFORM ==  OSAPI_PLATFORM_STELLARIS

#include <stdio.h>
#include <stdarg.h>

#include "osapi/osapi_types.h"
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM ==  OSAPI_PLATFORM_STELLARIS

void
OSAPI_Stdio_puts(const char *string)
{
#if !defined(NDEBUG)
#endif
}

void
OSAPI_Stdio_printf(const char *format,...)
{
#if !defined(NDEBUG)
#endif
}

RTI_UINT32
OSAPI_Stdio_snprintf(char *s, RTI_SIZE_T n, const char *format,...)
{
    RTI_UINT32 len = 0;

    return len;
}

#endif
