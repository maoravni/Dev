/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_string_impl_h
#define osapi_string_impl_h



#include <string.h>

#ifdef RTI_VXWORKS

#ifdef RTI_RTP
#include <strings.h>
#endif

#define OSAPI_Memory_copy(to, src, size) \
  (bcopy((const char *)src, (char *)to, size),1)

/* NOTE: we use bfill here as this avoids a function call.  On VxWorks,
   bzero(m,s) is a function implemented as bfill(m,s,0) */
#define OSAPI_Memory_zero(m, size) (bfill((char *)m, size, 0),(void *)m)
#define OSAPI_Memory_compare(l, r, size) bcmp((char *)l, (char *)r, size)
#define OSAPI_Memory_move(l,r,size) (bcopy((const char *)r, (char *)l, size),l)

#define OSAPI_String_length(__name) strlen(__name)
#define OSAPI_String_cmp(left_,right_) strcmp(left_,right_)

#elif SDCC
void *memcpy(void *s1, const void *s2, size_t n);
#else

#define OSAPI_Memory_copy memcpy
#define OSAPI_Memory_zero(m, size) memset(m, 0, size)
#define OSAPI_Memory_compare memcmp
#define OSAPI_Memory_move memmove

#define OSAPI_String_length(__name) strlen(__name)
#define OSAPI_String_cmp(left_,right_) strcmp(left_,right_)

#endif /* RTI_VXWORKS */


#endif /* osapi_string_impl_h */
