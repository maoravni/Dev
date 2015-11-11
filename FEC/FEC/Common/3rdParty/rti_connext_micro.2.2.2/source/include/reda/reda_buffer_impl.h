/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_buffer_impl_h
#define reda_buffer_impl_h



#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif

#include "osapi/osapi_types.h"

#define REDA_BufferArray_getSize(answer, array, arrayCount) \
{ \
    RTI_INT32 i = 0; \
    for (*(answer) = 0; i < (arrayCount); ++i) { \
        *(answer) += ((array)[i]).length; \
    } \
}

#define REDA_Buffer_set(buf, ptr, len) \
{ \
    (buf)->length = (len); \
    (buf)->pointer = (ptr); \
}

#define REDA_Buffer_get(buf, ptr, len) \
{ \
    *len = (buf)->length; \
    *ptr = (buf)->pointer; \
}

#define REDA_Buffer_reset(buf) REDA_Buffer_set(buf, NULL, 0)


#endif /* reda_buffer_impl_h */
