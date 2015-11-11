/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_config_h
#define osapi_config_h



/* OSAPI_PLATFORM_XXX
 * Defines platforms supported by OSAPI.
 *
 * The platform includes:
 * - OS API
 * - Hardware dependencies
 *
 * The platform specific OSAPI should include. Any HW dependencies to support
 * portable code. Likely candidates are clock/timer support.
 */
#define OSAPI_PLATFORM_POSIX      1
#define OSAPI_PLATFORM_STOS       2	/* not currently supported */
#define OSAPI_PLATFORM_FREERTOS   3
#define OSAPI_PLATFORM_VXWORKS    4
#define OSAPI_PLATFORM_WINDOWS    5
#define OSAPI_PLATFORM_STELLARIS  6

/* The global configuration is read first. Local definitions must
 * check if the global configuration has set an option or not. Note that
 * a module cannot disable a feature.
 */
#ifdef HAVE_GLOBAL_CONFIG
#include "rti_me_config.h"
#endif

/* If the platform has not been specified, attempt to determine it.
 */

#ifndef OSAPI_PLATFORM
#if defined(__APPLE__) && defined(__MACH__)
#define OSAPI_PLATFORM OSAPI_PLATFORM_POSIX
#elif defined(__linux__)
#define OSAPI_PLATFORM OSAPI_PLATFORM_POSIX
#elif defined(_MSC_VER) || defined(WIN32)
#define OSAPI_PLATFORM OSAPI_PLATFORM_WINDOWS
#elif defined(__vxworks)
#define OSAPI_PLATFORM OSAPI_PLATFORM_VXWORKS
#else
#error "Unable to determine OSAPI_PLATFORM. Please specify."
#endif
#endif

/* INCLUDE_POSIX
 *
 * Set INCLUDE_POSIX is the target platform used the POSIX API. Note that
 * 100% POSIX compliance is required, it is expected that nuances in the API
 * is reflected in the POSIX platform files. This is a coarse grained
 * inclusion.
 */
#if OSAPI_PLATFORM == OSAPI_PLATFORM_POSIX
#ifndef INCLUDE_POSIX
#define INCLUDE_POSIX
#endif
#ifndef RTI_UNIX
#define RTI_UNIX
#endif
#define HAVE_SOCKET_API
#define RTI_HAVE_FLOAT
/* #define RTI_SERIALIZE_MACRO */
/* Darwin/GCC */
#if defined(__APPLE__) && defined(__MACH__)
#include <malloc/malloc.h>
#define HAVE_MEMALIGN 0
#ifndef RTI_DARWIN
#define RTI_DARWIN
#endif /* RTI_DARWIN */
#if !defined(RTI_ENDIAN_BIG) && !defined(RTI_ENDIAN_LITTLE)
#ifdef __LITTLE_ENDIAN__
#define RTI_ENDIAN_LITTLE 1
#else
#define RTI_ENDIAN_BIG    1
#endif /* !defined(RTI_ENDIAN_BIG) && !defined(RTI_ENDIAN_LITTLE) */
#endif
/* Linux/GCC */
#elif defined(__linux__)
#ifndef _STDLIB_H
#include <stdlib.h>
#include <malloc.h>
#endif
#define HAVE_MEMALIGN 1
#ifndef RTI_LINUX
#define RTI_LINUX
#endif
#if !defined(RTI_ENDIAN_BIG) && !defined(RTI_ENDIAN_LITTLE)
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define RTI_ENDIAN_LITTLE 1
#elif __BYTE_ORDER == __BIG_ENDIAN
#define RTI_ENDIAN_BIG    1
#else
#error "Unable to determine endianess. Please specify RTI_ENDIAN_BIG RTI_ENDIAN_LITTLE"
#error
#endif
#endif /* !defined(RTI_ENDIAN_BIG) && !defined(RTI_ENDIAN_LITTLE) */
/* #define HAVE_RTI_POSIX_SCHED_POLICY */
#endif
#endif

/* Windows */
#if OSAPI_PLATFORM == OSAPI_PLATFORM_WINDOWS
#define _WINSOCKAPI_
#ifndef RTI_WIN32
#define RTI_WIN32
#endif
#define HAVE_SOCKET_API
#define RTI_HAVE_FLOAT
#if !defined(RTI_ENDIAN_BIG) && !defined(RTI_ENDIAN_LITTLE)
#ifdef _M_IX86
#define RTI_ENDIAN_LITTLE 1
#else
#error "Unable to determine endianess. Please specify RTI_ENDIAN_BIG RTI_ENDIAN_LITTLE"
#endif
#endif
#endif

/* VxWorks */
#if OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS
#ifndef RTI_USE_MUNCH
#define RTI_USE_MUNCH
#endif
#ifndef RTI_VXWORKS
#define RTI_VXWORKS
#endif
#ifndef RTI_RTP
/* #define RTI_RTP */
#endif
#define HAVE_SOCKET_API
#define RTI_HAVE_FLOAT
#if !defined(RTI_ENDIAN_BIG) && !defined(RTI_ENDIAN_LITTLE)
#ifdef __BIG_ENDIAN__
#define RTI_ENDIAN_BIG    1
#else
#define RTI_ENDIAN_LITTLE 1
#endif
#endif /* !defined(RTI_ENDIAN_BIG) && !defined(RTI_ENDIAN_LITTLE) */

#define VXWORKS_VERSION_6_8_OR_BETTER \
  (  ((VXWORKS_MAJOR_VERSION == 6) && (VXWORKS_MINOR_VERSION >= 8)) || \
     (VXWORKS_MAJOR_VERSION > 6) \
  )

#define VXWORKS_VERSION_6_3_OR_BETTER \
  (  ((VXWORKS_MAJOR_VERSION == 6) && (VXWORKS_MINOR_VERSION >= 3)) || \
     (VXWORKS_MAJOR_VERSION > 6) \
  )

#define VXWORKS_VERSION_6_0_OR_BETTER \
  (  ((VXWORKS_MAJOR_VERSION == 6) && (VXWORKS_MINOR_VERSION >= 0)) || \
     (VXWORKS_MAJOR_VERSION > 6) \
  )

#define VXWORKS_VERSION_5_4_OR_BETTER \
  (  ((VXWORKS_MAJOR_VERSION == 5) && (VXWORKS_MINOR_VERSION >= 5)) || \
     (VXWORKS_MAJOR_VERSION > 5) \
  )

#endif

/* Stellaris */
#if OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS
#define LWIP_NOSYS
#define RTI_ENDIAN_LITTLE 1
#endif

/* FreeRTOS */
#if OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS
#define HAVE_SOCKET_API
#define LWIP_SYS
#define RTI_ENDIAN_LITTLE 1
#endif


#endif /* osapi_config_h */
