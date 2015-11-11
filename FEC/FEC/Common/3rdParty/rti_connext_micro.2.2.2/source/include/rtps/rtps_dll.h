/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rtps_dll_h
#define rtps_dll_h



#if defined(RTI_WIN32) || defined(RTI_WINCE30)

#if defined(RTIME_DLL_EXPORT)
#ifndef RTI_rtps_DLL_EXPORT
#define RTI_rtps_DLL_EXPORT
#endif
#ifndef RTI_rtps_DLL_VARIABLE
#define RTI_rtps_DLL_VARIABLE
#endif
#endif

#if defined(RTI_rtps_DLL_EXPORT)
#define RTPSDllExport __declspec( dllexport )
#else
#define RTPSDllExport
#endif /* RTI_RTPS_DLL_EXPORT */

#if defined(RTI_rtps_DLL_VARIABLE)
#if defined(RTI_rtps_DLL_EXPORT)
#define RTPSDllVariable __declspec( dllexport )
#else
#define RTPSDllVariable __declspec( dllimport )
#endif /* RTI_RTPS_DLL_EXPORT */
#else
#define RTPSDllVariable
#endif /* RTI_RTPS_DLL_VARIABLE */
#else
#define RTPSDllExport
#define RTPSDllVariable
#endif /* RTI_WIN32 || RTI_WINCE30 */


#endif /* rtps_dll_h */
