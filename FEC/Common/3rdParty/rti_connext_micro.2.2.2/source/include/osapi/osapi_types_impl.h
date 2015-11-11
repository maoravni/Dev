/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_types_impl_h
#define osapi_types_impl_h



#ifdef __STDC__
#include <limits.h>
#include <stddef.h>

#if INT_MAX == 2147483647
typedef signed int RTI_INT32;
typedef unsigned int RTI_UINT32;
#else
#error "INT_MAX is unknown. Please define RTI_INT32/UINT32"
#endif /* INT_MAX */

#if SHRT_MAX == 32767
typedef signed short RTI_INT16;
typedef unsigned short RTI_UINT16;
#else
#error "SHRT_MAX is unknown. Please define RTI_INT16/UINT16"
#endif /* SHRT_MAX */

#if SCHAR_MAX == 127
typedef signed char RTI_INT8;
typedef unsigned char RTI_UINT8;
#else
#error "SHRT_MAX is unknown. Please define RTI_INT8/UINT8"
#endif /* SCHAR_MAX */

#ifdef __LONG_LONG_MAX__
 #if __LONG_LONG_MAX__ == 9223372036854775807
 typedef signed long long RTI_INT64;
 typedef unsigned long long RTI_UINT64;
 #else
 #error "__LONG_LONG_MAX__ does not support long long. Please define RTI_INT64/UINT64"
 #endif
#else
 #ifdef LLONG_MAX
  typedef signed long long RTI_INT64;
  typedef unsigned long long RTI_UINT64;
 #else
  #error "__LONG_LONG_MAX__ and LLONG_MAX are unknown. Please define RTI_INT64/UINT64"
 #endif
#endif

#ifdef RTI_HAVE_FLOAT
typedef float RTI_FLOAT32;
typedef double RTI_DOUBLE64;
#ifdef RTI_HAVE_LONG_DOUBLE
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 2))
typedef __float128 RTI_DOUBLE128;
#else
/* Warning: GNU long double is only 12 bytes
   Verify your implementation before enabling */
typedef long double RTI_DOUBLE128;
#endif
#else
typedef struct RTI_DOUBLE128 {
    char bytes[16];
} RTI_DOUBLE128;
#endif
#else
typedef unsigned int RTI_FLOAT32;
typedef unsigned long long RTI_DOUBLE64;
typedef struct RTI_DOUBLE128 {
    char bytes[16];
} RTI_DOUBLE128;
#endif

#elif defined(_M_IX86)
typedef signed int RTI_INT32;
typedef unsigned int RTI_UINT32;
typedef signed short RTI_INT16;
typedef unsigned short RTI_UINT16;
typedef signed char RTI_INT8;
typedef unsigned char RTI_UINT8;
typedef __int64 RTI_INT64;
typedef unsigned __int64 RTI_UINT64;

#ifdef RTI_HAVE_FLOAT
typedef float RTI_FLOAT32;
typedef double RTI_DOUBLE64;
#ifdef RTI_HAVE_LONG_DOUBLE
/* Warning: Microsoft Visual Studio long double is only 8 bytes
   Verify your implementation before enabling */
typedef long double RTI_DOUBLE128;
#else
typedef struct RTI_DOUBLE128 {
    char bytes[16];
} RTI_DOUBLE128;
#endif
#else
typedef unsigned int RTI_FLOAT32;
typedef unsigned __int64 RTI_DOUBLE64;
typedef struct RTI_DOUBLE128 {
    char bytes[16];
} RTI_DOUBLE128;
#endif

#else
#error "Non-standard compiler detected. Please define data-types"
#endif /* __STDC__ */


#endif /* osapi_types_impl_h */
