/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef cdr_cdr_type_h
#define cdr_cdr_type_h



#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif 
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif 
#ifndef cdr_dll_h
#include "cdr/cdr_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* ================================================================= */
/*                       CDR Primitive Types                         */
/* ================================================================= */

/*e
 * \dref_CdrGroupDocs
 */
/*i
 * TODO: Make sure the types actually map to the expected sizes on the 
 * different platform
 */
/*e
 * \dref_Char
 */
typedef RTI_INT8 CDR_Char;

/*e
 * \dref_Octet
 */
typedef RTI_UINT8 CDR_Octet;

/*e
 * \dref_Boolean
 */
typedef RTI_UINT8 CDR_Boolean;

/*e
 * \dref_Short
 */
typedef RTI_INT16 CDR_Short;

/*e
 * \dref_UnsignedShort
 */
typedef RTI_UINT16 CDR_UnsignedShort;

/*e
 * \dref_Long
 */
typedef RTI_INT32 CDR_Long;

/*e
 * \dref_UnsignedLong
 */
typedef RTI_UINT32 CDR_UnsignedLong;

/*e
 * \dref_Enum
 */
typedef RTI_UINT32 CDR_Enum;

/*e
 * \dref_Wchar
 */
typedef RTI_UINT32 CDR_Wchar;

/*e
 * \dref_LongLong
 */
typedef RTI_INT64 CDR_LongLong;

/*e
 * \dref_UnsignedLongLong
 */
typedef RTI_UINT64 CDR_UnsignedLongLong;

/*e
 * \dref_Float
 */
typedef RTI_FLOAT32 CDR_Float;

/*e
 * \dref_Double
 */
typedef RTI_DOUBLE64 CDR_Double;

/*e
 * \dref_LongDouble
 */
typedef RTI_DOUBLE128 CDR_LongDouble;

/*e
 * \dref_String
 */
typedef char *CDR_String;

/*e
 * \dref_Wstring
 */
typedef RTI_UINT32 *CDR_Wstring;

/* ================================================================= */
/*                 CDR Primitive Type Sequences                      */
/* ================================================================= */

REDA_DEFINE_SEQUENCE(CDR_CharSeq, CDR_Char)
REDA_DEFINE_SEQUENCE(CDR_OctetSeq, CDR_Octet)
REDA_DEFINE_SEQUENCE(CDR_BooleanSeq, CDR_Boolean)
REDA_DEFINE_SEQUENCE(CDR_ShortSeq, CDR_Short)
REDA_DEFINE_SEQUENCE(CDR_UnsignedShortSeq, CDR_UnsignedShort)
REDA_DEFINE_SEQUENCE(CDR_LongSeq, CDR_Long)
REDA_DEFINE_SEQUENCE(CDR_UnsignedLongSeq, CDR_UnsignedLong)
REDA_DEFINE_SEQUENCE(CDR_EnumSeq, CDR_Enum)
REDA_DEFINE_SEQUENCE(CDR_WcharSeq, CDR_Wchar)
REDA_DEFINE_SEQUENCE(CDR_LongLongSeq, CDR_LongLong)
REDA_DEFINE_SEQUENCE(CDR_UnsignedLongLongSeq, CDR_UnsignedLongLong)
REDA_DEFINE_SEQUENCE(CDR_FloatSeq, CDR_Float)
REDA_DEFINE_SEQUENCE(CDR_DoubleSeq, CDR_Double)
REDA_DEFINE_SEQUENCE(CDR_LongDoubleSeq, CDR_LongDouble)
REDA_DEFINE_SEQUENCE(CDR_StringSeq, CDR_String)
REDA_DEFINE_SEQUENCE(CDR_WstringSeq, CDR_Wstring)

/* ================================================================= */
/*          CDR Primitive Type Enumeration and Attributes            */
/* ================================================================= */

typedef enum {
    CDR_CHAR_TYPE,
    CDR_WCHAR_TYPE,
    CDR_OCTET_TYPE,
    CDR_SHORT_TYPE,
    CDR_UNSIGNED_SHORT_TYPE,
    CDR_LONG_TYPE,
    CDR_UNSIGNED_LONG_TYPE,
    CDR_LONG_LONG_TYPE,
    CDR_UNSIGNED_LONG_LONG_TYPE,
    CDR_FLOAT_TYPE,
    CDR_DOUBLE_TYPE,
    CDR_LONG_DOUBLE_TYPE,
    CDR_BOOLEAN_TYPE,
    CDR_ENUM_TYPE,
    CDR_INVALID_TYPE
} CdrPrimitiveType;

#define CDR_BOOLEAN_SIZE      1
#define CDR_OCTET_SIZE        1
#define CDR_CHAR_SIZE         1
#define CDR_SHORT_SIZE        2
#define CDR_UNSIGNED_SHORT_SIZE 2
#define CDR_LONG_SIZE         4
#define CDR_UNSIGNED_LONG_SIZE  4
#define CDR_FLOAT_SIZE        4
#define CDR_WCHAR_SIZE        4
#define CDR_ENUM_SIZE         4
#define CDR_LONG_LONG_SIZE    8
#define CDR_UNSIGNED_LONG_LONG_SIZE 8
#define CDR_DOUBLE_SIZE       8
#define CDR_LONG_DOUBLE_SIZE  16

#define CDR_OCTET_ALIGN       1
#define CDR_SHORT_ALIGN       2
#define CDR_LONG_ALIGN        4
#define CDR_FLOAT_ALIGN       4
#define CDR_LONG_LONG_ALIGN   8
#define CDR_DOUBLE_ALIGN      8
#define CDR_LONG_DOUBLE_ALIGN 8

#define CDR_Primitive_get_alignment(type)    \
  ((((type) == CDR_CHAR_TYPE) ||                  \
    ((type) == CDR_OCTET_TYPE) ||                 \
    ((type) == CDR_BOOLEAN_TYPE)) ? 1 :           \
   ((((type) == CDR_SHORT_TYPE) ||                \
     ((type) == CDR_UNSIGNED_SHORT_TYPE)) ? 2 :   \
    ((((type) == CDR_WCHAR_TYPE) ||               \
      ((type) == CDR_LONG_TYPE) ||                \
      ((type) == CDR_UNSIGNED_LONG_TYPE) ||       \
      ((type) == CDR_FLOAT_TYPE) ||               \
      ((type) == CDR_ENUM_TYPE)) ? 4 :            \
     ((((type) == CDR_LONG_LONG_TYPE) ||          \
       ((type) == CDR_UNSIGNED_LONG_LONG_TYPE) || \
       ((type) == CDR_DOUBLE_TYPE) ||             \
       ((type) == CDR_LONG_DOUBLE_TYPE)) ? 8 : 0))))

#define CDR_Primitive_get_size(type)         \
  ((((type) == CDR_CHAR_TYPE) ||                  \
    ((type) == CDR_OCTET_TYPE) ||                 \
    ((type) == CDR_BOOLEAN_TYPE)) ? 1U :          \
   ((((type) == CDR_SHORT_TYPE) ||                \
     ((type) == CDR_UNSIGNED_SHORT_TYPE)) ? 2U :  \
    ((((type) == CDR_WCHAR_TYPE)||                \
      ((type) == CDR_LONG_TYPE) ||                \
      ((type) == CDR_UNSIGNED_LONG_TYPE) ||       \
      ((type) == CDR_FLOAT_TYPE) ||               \
      ((type) == CDR_ENUM_TYPE)) ? 4U :           \
     ((((type) == CDR_LONG_LONG_TYPE) ||          \
       ((type) == CDR_UNSIGNED_LONG_LONG_TYPE) || \
       ((type) == CDR_DOUBLE_TYPE)) ? 8U :        \
      ((type) == CDR_LONG_DOUBLE_TYPE) ? 16U : 0U))))
      
/* ================================================================= */
/*               Type Specific Initialization & Copy                 */
/* ================================================================= */
/* not all types are atomic so we define macros to emulate as needed */
#define CDR_Primitive_init_Char(value)              *(value) = 0
#define CDR_Primitive_init_Octet(value)             *(value) = 0
#define CDR_Primitive_init_Boolean(value)           *(value) = 0
#define CDR_Primitive_init_Short(value)             *(value) = 0
#define CDR_Primitive_init_UnsignedShort(value)     *(value) = 0
#define CDR_Primitive_init_Long(value)              *(value) = 0
#define CDR_Primitive_init_UnsignedLong(value)      *(value) = 0
#define CDR_Primitive_init_Wchar(value)             *(value) = 0
#define CDR_Primitive_init_Enum(value)              *(value) = 0
#define CDR_Primitive_init_Float(value)             *(value) = 0
#define CDR_Primitive_init_LongLong(value)          *(value) = 0
#define CDR_Primitive_init_UnsignedLongLong(value)  *(value) = 0
#define CDR_Primitive_init_Double(value)            *(value) = 0
#ifdef RTI_HAVE_LONG_DOUBLE
#define CDR_Primitive_init_LongDouble(value)        *(value) = 0
#else
#define CDR_Primitive_init_LongDouble(value) \
    OSAPI_Memory_zero(value,sizeof(RTI_DOUBLE128))
#endif
#define CDR_Primitive_init_String(value,maxsize) \
    OSAPI_Memory_zero(value,maxsize)
#define CDR_Primitive_init_Wstring(value,maxsize) \
    OSAPI_Memory_zero(value,((maxsize)*CDR_WCHAR_SIZE))
#define CDR_Primitive_init_Array(value,maxsize) \
    OSAPI_Memory_zero(value,maxsize)

#define CDR_Primitive_copy_Char(dst,src)            *(dst) = *(src)
#define CDR_Primitive_copy_Octet(dst,src)           *(dst) = *(src)
#define CDR_Primitive_copy_Boolean(dst,src)         *(dst) = *(src)
#define CDR_Primitive_copy_Short(dst,src)           *(dst) = *(src)
#define CDR_Primitive_copy_UnsignedShort(dst,src)   *(dst) = *(src)
#define CDR_Primitive_copy_Long(dst,src)            *(dst) = *(src)
#define CDR_Primitive_copy_UnsignedLong(dst,src)    *(dst) = *(src)
#define CDR_Primitive_copy_Wchar(dst,src)           *(dst) = *(src)
#define CDR_Primitive_copy_Enum(dst,src)            *(dst) = *(src)
#define CDR_Primitive_copy_Float(dst,src)           *(dst) = *(src)
#define CDR_Primitive_copy_LongLong(dst,src)        *(dst) = *(src)
#define CDR_Primitive_copy_UnsignedLongLong(dst,src) *(dst) = *(src)
#define CDR_Primitive_copy_Double(dst,src)          *(dst) = *(src)
#ifdef RTI_HAVE_LONG_DOUBLE
#define CDR_Primitive_copy_LongDouble(dst,src)      *(dst) = *(src)
#else
#define CDR_Primitive_copy_LongDouble(dst,src) \
    OSAPI_Memory_copy((void*)dst,(void*)src,sizeof(RTI_DOUBLE128))
#endif
#define CDR_Primitive_copy_String(dst,src,maxsize) \
    OSAPI_Memory_copy((void*)dst,(void*)src,maxsize)
#define CDR_Primitive_copy_Wstring(dst,src,maxsize) \
    OSAPI_Memory_copy((void*)dst,(void*)src,((maxsize)*CDR_WCHAR_SIZE))
#define CDR_Primitive_copy_Array(dst,src,maxsize) \
    OSAPI_Memory_copy((void*)dst,(void*)src,maxsize)

/*==============================================================================
 * Type Helper Functions
 * ===========================================================================*/

CDRDllExport CDR_Wchar*
CDR_Wstring_alloc(RTI_SIZE_T length);

CDRDllExport void
CDR_Wstring_free(CDR_Wchar * wstring);

CDRDllExport RTI_SIZE_T
CDR_Wstring_length(const CDR_Wchar * wstring);

CDRDllExport CDR_Wchar *
CDR_Wstring_dup(const CDR_Wchar * wstring);

CDRDllExport CDR_Wchar *
CDR_Wstring_replace(CDR_Wchar ** wstring_ptr, const CDR_Wchar * new_value);

CDRDllExport RTI_INT32
CDR_Wstring_compare(const CDR_Wchar * left, const CDR_Wchar * right);

CDRDllExport RTI_BOOL
CDR_StringArray_initialize(void *value, RTI_UINT32 length,
                           RTI_UINT32 maximumStringLength,
                           CdrPrimitiveType type);

CDRDllExport RTI_BOOL
CDR_StringArray_finalize(void *value, RTI_UINT32 length,
                         CdrPrimitiveType type);

CDRDllExport RTI_BOOL
CDR_StringArray_copy(void *out, const void* in,
                     RTI_UINT32 length,
                     RTI_UINT32 maximumStringLength,
                     CdrPrimitiveType type);

/*==============================================================================
 * String Sequence support wrapper functions to convert from String* to Char*
 * ===========================================================================*/

CDRDllExport RTI_BOOL
CDR_String_initialize(CDR_String * string);

CDRDllExport RTI_BOOL
CDR_String_finalize(CDR_String * string);

CDRDllExport RTI_SIZE_T
CDR_String_copy(CDR_String * dst, const CDR_String *src);

CDRDllExport RTI_INT32
CDR_String_compare(CDR_String * left, const CDR_String *right);

CDRDllExport RTI_BOOL
CDR_Wstring_initialize(CDR_Wstring * wstring);

CDRDllExport RTI_BOOL
CDR_Wstring_finalize(CDR_Wstring * wstring);

CDRDllExport RTI_SIZE_T
CDR_Wstring_copy(CDR_Wstring * left, const CDR_Wstring * right);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* cdr_cdr_type_h */
