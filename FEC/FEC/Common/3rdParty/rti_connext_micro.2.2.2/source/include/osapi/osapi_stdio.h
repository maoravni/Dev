/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_stdio_h
#define osapi_stdio_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
    \brief I/O utilities.
*/

/* \defgroup OSAPI_StdioClass OSAPI Standard I/O
    \ingroup OSAPIModule

  \brief OS-independent standard I/O utilities.
 */

/*e \ingroup OSAPI_StdioClass
  \brief Print string to standard output.

  @exception None.
*/
OSAPIDllExport void
OSAPI_Stdio_puts(const char *string);

/*e \ingroup OSAPI_StdioClass
  \brief Print formatted string to standard output.

  @exception None.

*/
OSAPIDllExport void
OSAPI_Stdio_printf(const char *format,...);

/*e \ingroup OSAPI_StdioClass

  \brief Print formatted string to buffer.

  @exception None.
*/
OSAPIDllExport RTI_INT32
OSAPI_Stdio_snprintf(char *s, RTI_SIZE_T n, const char *format,...);

#ifdef __cplusplus
}
#endif

#endif /* osapi_stdio_h */
