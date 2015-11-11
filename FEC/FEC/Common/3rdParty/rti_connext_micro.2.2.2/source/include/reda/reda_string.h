/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_string_h
#define reda_string_h



#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif

#include "reda/reda_sequence.h"

#ifdef __cplusplus
extern "C"
{
#endif

REDA_DEFINE_SEQUENCE(REDA_StringSeq, char*)

#define REDA_StringSeq_INITIALIZER REDA_DEFINE_SEQUENCE_INITIALIZER(char*)

typedef char* REDA_String_T;

REDADllExport char*
REDA_String_alloc(RTI_SIZE_T length);

REDADllExport void
REDA_String_free(char *string);

REDADllExport RTI_SIZE_T
REDA_String_length(const char *string);

REDADllExport char*
REDA_String_dup(const char *string);

REDADllExport char*
REDA_String_replace(char **string_ptr, const char *new_value);

REDADllExport RTI_INT32
REDA_String_compare(const char *left, const char *right);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* reda_string_h */
