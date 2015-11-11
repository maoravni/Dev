/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_dll_h
#define osapi_dll_h



#if defined(RTI_WIN32) || defined(RTI_WINCE30)

#if defined(RTIME_DLL_EXPORT)
#ifndef RTI_osapi_DLL_EXPORT
#define RTI_osapi_DLL_EXPORT
#endif
#ifndef RTI_osapi_DLL_VARIABLE
#define RTI_osapi_DLL_VARIABLE
#endif
#endif

#if defined(RTI_osapi_DLL_EXPORT)
#define OSAPIDllExport __declspec( dllexport )
#else
#define OSAPIDllExport
#endif /* RTI_osapi_DLL_EXPORT */

#if defined(RTI_osapi_DLL_VARIABLE)
#if defined(RTI_osapi_DLL_EXPORT)
#define OSAPIDllVariable __declspec( dllexport )
#else
#define OSAPIDllVariable __declspec( dllimport )
#endif /* RTI_osapi_DLL_EXPORT */
#else
#define OSAPIDllVariable
#endif /* RTI_osapi_DLL_VARIABLE */
#else
#define OSAPIDllExport
#define OSAPIDllVariable
#endif /* RTI_WIN32 || RTI_WINCE30 */


#endif /* osapi_dll_h */
