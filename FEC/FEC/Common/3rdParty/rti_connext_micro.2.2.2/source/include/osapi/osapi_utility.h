/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_utility_h
#define osapi_utility_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
  @brief Simple, static utility functions that you do not want to write
  over and over again in different modules.
*/

/*e \defgroup OSAPI_UtilityClass OSAPI Utility
 * \ingroup OSAPIModule
*/

/*e \ingroup OSAPI_UtilityClass
  \brief Namespace for utility functions.
*/
/*e \ingroup OSAPI_UtilityClass
  \brief Static function to return the greater of two values.
*/
OSAPIDllExport RTI_INT32
OSAPI_Utility_max(RTI_INT32 l, RTI_INT32 r);

/*e \ingroup OSAPI_UtilityClass
  \brief Static function to return the lesser of two values.
*/
OSAPIDllExport RTI_INT32
OSAPI_Utility_min(RTI_INT32 l, RTI_INT32 r);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#include "osapi/osapi_utility_impl.h"

#endif /* osapi_utility_h */
