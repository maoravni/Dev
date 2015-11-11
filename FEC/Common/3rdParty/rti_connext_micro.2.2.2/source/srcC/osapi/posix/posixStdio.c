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

#if OSAPI_PLATFORM ==  OSAPI_PLATFORM_POSIX
#include <stdio.h>
#include <stdarg.h>
#include "osapi/osapi_types.h"

#if defined(__ANDROID__)
#include <android/log.h>
#endif
#endif

/*** SOURCE_BEGIN ***/

#define OSAPI_LOG_TAG "com.rti.rtime"

#if OSAPI_PLATFORM ==  OSAPI_PLATFORM_POSIX
void
OSAPI_Stdio_puts(const char *string)
{
#if !defined(NDEBUG)
#if defined(__ANDROID__)
    (void)__android_log_print(ANDROID_LOG_DEBUG,OSAPI_LOG_TAG,string);
#else
    printf("%s",string);
#endif
#endif
}

void
OSAPI_Stdio_printf(const char *format,...)
{
    va_list arglist;
    va_start(arglist, format);

    #if !defined(NDEBUG)
#if defined(__ANDROID__)
    (void)__android_log_vprint(ANDROID_LOG_DEBUG,OSAPI_LOG_TAG,format,arglist);
#else
    (void)vprintf(format,arglist);
#endif
#endif

    va_end(arglist);
}

RTI_INT32
OSAPI_Stdio_snprintf(char *s, RTI_SIZE_T n, const char *format,...)
{
    RTI_INT32 len;

    va_list arglist;

    va_start(arglist, format);

    len = vsnprintf(s,n,format,arglist);

    va_end(arglist);

    return len;
}

#endif
