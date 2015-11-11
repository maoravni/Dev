/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_process_h
#define osapi_process_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
  \brief process related utility
*/

/*e \defgroup OSAPI_ProcessClass OSAPI Process
    \ingroup OSAPIModule

    \brief OS-independent encapsulation of an application process,
    in a separate address space.

 */

/*e \ingroup OSAPI_ProcessClass

  \brief process ID, which is unique for the application.

  \note this method internally generates exceptions,
  therefore the caller does not need to log an exception.
*/
OSAPIDllExport RTI_UINT32
OSAPI_Process_getId(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* osapi_process_h */
