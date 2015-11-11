/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_dll_h
#define netio_dll_h



#if defined(RTI_WIN32) || defined(RTI_WINCE30)

#if defined(RTIME_DLL_EXPORT)
#ifndef RTI_netio_DLL_EXPORT
#define RTI_netio_DLL_EXPORT
#endif
#ifndef RTI_netio_DLL_VARIABLE
#define RTI_netio_DLL_VARIABLE
#endif
#endif

#if defined(RTI_netio_DLL_EXPORT)
#define NETIODllExport __declspec( dllexport )
#else
#define NETIODllExport
#endif /* RTI_netio_DLL_EXPORT */

#if defined(RTI_netio_DLL_VARIABLE)
#if defined(RTI_netio_DLL_EXPORT)
#define NETIODllVariable __declspec( dllexport )
#else
#define NETIODllVariable __declspec( dllimport )
#endif /* RTI_netio_DLL_EXPORT */
#else
#define NETIODllVariable
#endif /* RTI_netio_DLL_VARIABLE */
#else
#define NETIODllExport
#define NETIODllVariable
#endif /* RTI_WIN32 || RTI_WINCE30 */

#endif /* netio_dll_h */
