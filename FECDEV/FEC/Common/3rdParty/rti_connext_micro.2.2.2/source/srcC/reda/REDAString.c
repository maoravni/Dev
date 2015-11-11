/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#ifndef osapi_config_h
#include "osapi/osapi_config.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef reda_log_h
#include "reda/reda_log.h"
#endif
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif
#ifndef reda_string_h
#include "reda/reda_string.h"
#endif

/*** SOURCE_BEGIN ***/

RTI_PRIVATE RTI_BOOL
REDA_StringSeq_element_init(char **e)
{
    *e = NULL;
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
REDA_StringSeq_element_finalize(char **e)
{
    REDA_String_free(*e);
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
REDA_StringSeq_element_copy(char **dst,const char **src)
{
    if (*dst != NULL)
    {
        REDA_String_free(*dst);
    }
    *dst = REDA_String_dup(*src);
    if (*dst == NULL)
    {
        return RTI_FALSE;
    }
    return RTI_TRUE;
}

RTI_PRIVATE RTI_INT32
REDA_StringSeq_element_compare(const char **left,const char **right)
{
    return REDA_String_compare(*left,*right);
}

#define T char*
#define T_initialize REDA_StringSeq_element_init
#define T_finalize REDA_StringSeq_element_finalize
#define T_copy REDA_StringSeq_element_copy
#define T_compare REDA_StringSeq_element_compare
#define TSeq REDA_StringSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef T_initialize
#undef T_finalize
#undef T_copy
#undef T_compare
#undef TSeq

char*
REDA_String_alloc(RTI_SIZE_T length)
{
    char *string = NULL;

    LOG_PRECONDITION((length == 0) || (length == RTI_SIZE_INVALID),return NULL)

    OSAPI_Heap_allocateString(&string, length);

    return string;
}

void
REDA_String_free(char *string)
{
    LOG_PRECONDITION(string == NULL,return)

    OSAPI_Heap_freeString(string);
}

char*
REDA_String_dup(const char *string)
{
    char *clone = NULL;
    RTI_SIZE_T len;

    LOG_PRECONDITION(string == NULL,return NULL)

    len = OSAPI_String_length(string)+1;

    clone = REDA_String_alloc(len);
    if (clone == NULL)
    {
        REDA_Log_error(REDA_LOG_STRING_ALLOC_FAILED);
        goto done;
    }

    OSAPI_Memory_copy(clone,string,len);

done:
    return clone;
}

char*
REDA_String_replace(char **string_ptr, const char *new_value)
{
    char *result = NULL;
    RTI_SIZE_T len;
#ifdef RTI_CERT
    char *old_string_ptr = NULL;
#endif

    LOG_PRECONDITION(string_ptr == NULL,return NULL)

    if (new_value == NULL)
    {
        REDA_String_free(*string_ptr);
        *string_ptr = result = NULL;
    }
    else
    {
        len = OSAPI_String_length(new_value)+1;
#ifdef RTI_CERT
        old_string_ptr = *string_ptr;
        *string_ptr = (char *)OSAPI_Heap_allocate(1, len);
#else
        *string_ptr = (char*)OSAPI_Heap_realloc(*string_ptr,len);
#endif
        if (*string_ptr)
        {
            OSAPI_Memory_copy(*string_ptr,new_value,len);
            result = *string_ptr;
        }
#ifdef RTI_CERT
        REDA_String_free(old_string_ptr);
#endif
    }

    return result;
}

RTI_SIZE_T
REDA_String_length(const char *string)
{
    LOG_PRECONDITION(string == NULL,return RTI_SIZE_INVALID)

    return OSAPI_String_length(string);
}

RTI_INT32
REDA_String_compare(const char *left, const char *right)
{

    if ((left == NULL) && (right == NULL))
        return 0;

    if (left == NULL)
        return 1;

    if (right == NULL)
        return -1;

    return OSAPI_String_cmp(left, right);
}

