/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
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
#ifndef reda_string_h
#include "reda/reda_string.h"
#endif
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif
#ifndef cdr_cdr_type_h
#include "cdr/cdr_cdr_type.h"
#endif

/*** SOURCE_BEGIN ***/

/*==============================================================================
 * Built-In Sequences
 * ===========================================================================*/

#define T    CDR_Char
#define TSeq CDR_CharSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_Octet
#define TSeq CDR_OctetSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_Boolean
#define TSeq CDR_BooleanSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_Short
#define TSeq CDR_ShortSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_UnsignedShort
#define TSeq CDR_UnsignedShortSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_Long
#define TSeq CDR_LongSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_UnsignedLong
#define TSeq CDR_UnsignedLongSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_Enum
#define TSeq CDR_EnumSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_Wchar
#define TSeq CDR_WcharSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_LongLong
#define TSeq CDR_LongLongSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_UnsignedLongLong
#define TSeq CDR_UnsignedLongLongSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_Float
#define TSeq CDR_FloatSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_Double
#define TSeq CDR_DoubleSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T    CDR_LongDouble
#define TSeq CDR_LongDoubleSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

/*==============================================================================
 * Type Helper Functions
 * ===========================================================================*/

CDR_Wchar*
CDR_Wstring_alloc(RTI_SIZE_T length)
{
    CDR_Wchar * Wstring = NULL;

    OSAPI_Heap_allocateArray(&Wstring, length + 1, RTI_UINT32);

    return Wstring;
}


void
CDR_Wstring_free(CDR_Wchar * wstring)
{
    OSAPI_Heap_freeArray(wstring);
}


RTI_SIZE_T
CDR_Wstring_length(const CDR_Wchar * wstring)
{      
    RTI_UINT32 i = 0;

    while ((*wstring++) != '\0') i++;

    return (RTI_SIZE_T) i;
}

CDR_Wchar *
CDR_Wstring_dup(const CDR_Wchar * wstring)
{
    CDR_Wchar *clone = NULL;
    RTI_SIZE_T len;

    if (wstring != NULL)
    {
        len = CDR_Wstring_length(wstring);
        clone = CDR_Wstring_alloc(len);
        if (clone == NULL)
        {
            goto done;
        }
        OSAPI_Memory_copy(clone, wstring,((len + 1)* CDR_WCHAR_SIZE));
    }

done:
    return clone;
}


CDR_Wchar *
CDR_Wstring_replace(CDR_Wchar ** wstring_ptr, const CDR_Wchar * new_value)
{
    RTI_SIZE_T len;

    if (wstring_ptr == NULL)
    {
        return NULL;
    }

    if (new_value == NULL)
    {
        CDR_Wstring_free(*wstring_ptr);
        *wstring_ptr = NULL;
    }
    else
    {
        /* string length must include terminating NULL */
        len = CDR_Wstring_length(new_value) + 1;
        if (CDR_Wstring_length(*wstring_ptr) != len)
        {
            CDR_Wstring_free(*wstring_ptr);
            *wstring_ptr = CDR_Wstring_alloc(len);
            if (*wstring_ptr == NULL)
            {
                goto done;
            }

        }
        OSAPI_Memory_copy(*wstring_ptr, new_value, (len * CDR_WCHAR_SIZE));
    }

done:
    return *wstring_ptr;
}


RTI_INT32
CDR_Wstring_compare(const CDR_Wchar * left, const CDR_Wchar * right)
{
    RTI_SIZE_T r_len, l_len, cmp_len, i;

    if ((left == NULL) && (right == NULL))
        return 0;

    if (left == NULL)
        return 1;

    if (right == NULL)
        return -1;

    r_len = CDR_Wstring_length(right);
    l_len = CDR_Wstring_length(left);
    cmp_len = (r_len < l_len) ? r_len : l_len;
    for (i = 0; i < cmp_len; i++)
    {
        if (*left < *right)
        {
            return 1;
        }
        else if (*left++ > *right++)
        {
            return -1;
        }
    }
    return (r_len > cmp_len) ? 1 : ((l_len > cmp_len) ? -1 : 0);
}


RTI_BOOL
CDR_StringArray_initialize(void *value,
                           RTI_UINT32 length,
                           RTI_UINT32 maximumStringLength,
                           CdrPrimitiveType type)
{
    RTI_UINT32 i;
    char ** valueChar = NULL;    
    RTI_UINT32 ** valueWchar = NULL;
     
    if (type == CDR_CHAR_TYPE)
    {
        valueChar = (char **)value;
        for (i = 0;i < length; i++)
        {
            valueChar[i] = NULL;
            valueChar[i] = REDA_String_alloc(maximumStringLength);
            if (valueChar[i] == NULL)
            {
                return RTI_FALSE;
            }
        }
        return RTI_TRUE;
    } 
    else if (type==CDR_WCHAR_TYPE)
    {
        valueWchar = (RTI_UINT32 **)value;
        for (i = 0;i < length; i++)
        {
            valueWchar[i] = CDR_Wstring_alloc(maximumStringLength);
            if (valueWchar[i] == NULL)
            {
                return RTI_FALSE;
            }
        }
        return RTI_TRUE;
    }
    else 
    {
        return RTI_FALSE;
    }
}


RTI_BOOL
CDR_StringArray_finalize(void *value,
                         RTI_UINT32 length,
                         CdrPrimitiveType type)
{
    RTI_UINT32 i;
    char  ** valueChar=NULL;    
    RTI_UINT32 ** valueWchar=NULL;
    
    if (type==CDR_CHAR_TYPE)
    {
        valueChar=(char **)value;
        for (i=0;i<length;i++)
        {
            if (valueChar[i]!=NULL) 
            {
                REDA_String_free(valueChar[i]);
            }
            valueChar[i]=NULL;
        }
        return RTI_TRUE;
    }
    else if (type==CDR_WCHAR_TYPE)
    {
        valueWchar=(RTI_UINT32 **)value;
        for (i=0;i<length;i++)
        {
            if (valueWchar[i]!=NULL) 
            {
                CDR_Wstring_free(valueWchar[i]);
            }
            valueWchar[i]=NULL;
        }
        return RTI_TRUE;
    }
    else
    {
        return RTI_FALSE;
    }
}
 

RTI_BOOL
CDR_StringArray_copy(void *out, const void* in,
                     RTI_UINT32 length,
                     RTI_UINT32 maximumStringLength,
                     CdrPrimitiveType type)
{
    RTI_UINT32 i;
    
    if (type == CDR_CHAR_TYPE)
    {
        for (i=0; i<length; i++)
        {
            CDR_Primitive_copy_String(((char **)out)[i],
                                ((char **)in)[i],maximumStringLength);
        }
        return RTI_TRUE;      
    }
    else if (type == CDR_WCHAR_TYPE)
    {
        for (i=0; i<length; i++)
        {
            CDR_Primitive_copy_Wstring(((RTI_UINT32 **)out)[i],
                                 ((RTI_UINT32 **)in)[i],maximumStringLength);
        }
        return RTI_TRUE;      
    }
    else
    {
        return RTI_FALSE;
    }
}

/*==============================================================================
 * String Sequences
 * ===========================================================================*/

RTI_BOOL
CDR_String_initialize(CDR_String * string)
{
    if ((string != NULL) && (*string != NULL)) { 
        **string = '\0';
    }
    return RTI_TRUE;
}

RTI_BOOL
CDR_String_finalize(CDR_String * string)
{
    if (string != NULL) {
        REDA_String_free(*string);
        *string = NULL;
    }
    return RTI_TRUE;
}

RTI_SIZE_T
CDR_String_copy(CDR_String * dst, const CDR_String *src)
{
    RTI_SIZE_T len = 0;
    if (*dst)
    {
        len = OSAPI_String_length(*src)+1;
        CDR_Primitive_copy_String(*dst,*src,len);
        len--;
    }
    return len;
}


RTI_INT32
CDR_String_compare(CDR_String * left, const CDR_String *right)
{
    if ((left == NULL) && (right == NULL))
        return 0;

    if (left == NULL)
        return 1;

    if (right == NULL)
        return -1;

    return REDA_String_compare(*left, *right);
}


/* #define T_initialize REDA_String_alloc */
#define T_initialize CDR_String_initialize
#define T_finalize   CDR_String_finalize
#define T_copy       CDR_String_copy
#define T    CDR_String
#define TSeq CDR_StringSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq
#undef T_initialize
#undef T_finalize
#undef T_copy

RTI_BOOL
CDR_Wstring_initialize(CDR_Wstring * wstring)
{
    **wstring = 0;
    return RTI_TRUE;
}

RTI_BOOL
CDR_Wstring_finalize(CDR_Wstring * wstring)
{
    OSAPI_Heap_freeArray(*wstring);
    return RTI_TRUE;
}

CDRDllExport RTI_SIZE_T
CDR_Wstring_copy(CDR_Wstring * dst, const CDR_Wstring * src)
{
    RTI_SIZE_T len = 0;
    if (*dst)
    {
        len = CDR_Wstring_length(*src)+1;
        CDR_Primitive_copy_Wstring(*dst,*src,len);
        len--;
    }
    return len;
}

/* #define T_initialize CDR_Wstring_alloc */
#define T_initialize CDR_Wstring_initialize
#define T_finalize   CDR_Wstring_finalize
#define T_copy       CDR_Wstring_copy
#define T    CDR_Wstring
#define TSeq CDR_WstringSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq
#undef T_initialize
#undef T_finalize
#undef T_copy


