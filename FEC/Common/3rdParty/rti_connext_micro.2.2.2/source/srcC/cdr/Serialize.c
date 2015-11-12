/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef util_makeheader_h
#include "util/util_makeheader.h"
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
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef cdr_serialize_h
#include "cdr/cdr_serialize.h"
#endif
#ifndef cdr_log_h
#include "cdr/cdr_log.h"
#endif

/*** SOURCE_BEGIN ***/

#ifndef RTI_SERIALIZE_MACRO

/* not using macro, so we have functions wrapping the macros
 * (size VS performance tradeoff) for base primative types: 
 * UnsignedShort, UnsignedLong, UnsignedLongLong, LongDouble
 */
void
CDR_serialize_UnsignedShort(char **destBuffer,
                            const RTI_UINT16 *instance,
                            RTI_BOOL byteSwap)
{
    CDR_serialize_UnsignedShortMacro(destBuffer, instance, byteSwap);
}

void
CDR_deserialize_UnsignedShort(char **srcBuffer,
                              RTI_UINT16 *instance,
                              RTI_BOOL byteSwap)
{
    CDR_deserialize_UnsignedShortMacro(srcBuffer, instance, byteSwap);
}

void
CDR_serialize_UnsignedLong(char **destBuffer,
                           const RTI_UINT32 *instance,
                           RTI_BOOL byteSwap)
{
    CDR_serialize_UnsignedLongMacro(destBuffer, instance, byteSwap);
}

void
CDR_deserialize_UnsignedLong(char **srcBuffer,
                             RTI_UINT32 *instance,
                             RTI_BOOL byteSwap)
{
    CDR_deserialize_UnsignedLongMacro(srcBuffer, instance, byteSwap);
}

void
CDR_serialize_UnsignedLongToBigEndian(char **destBuffer,
                                      const RTI_UINT32 *instance)
{
    CDR_serialize_UnsignedLongToBigEndianMacro(destBuffer, instance);
}

void
CDR_deserialize_UnsignedLongFromBigEndian(char **srcBuffer,
                                          RTI_UINT32 *instance)
{
    CDR_deserialize_UnsignedLongFromBigEndianMacro(srcBuffer, instance);
}

void
CDR_serialize_UnsignedLongLong(char **destBuffer,
                               const RTI_UINT64 *instance,
                               RTI_BOOL byteSwap)
{
    CDR_serialize_UnsignedLongLongMacro(destBuffer, instance, byteSwap);
}

void
CDR_deserialize_UnsignedLongLong(char **srcBuffer,
                                 RTI_UINT64 *instance,
                                 RTI_BOOL byteSwap)
{
    CDR_deserialize_UnsignedLongLongMacro(srcBuffer, instance, byteSwap);
}

void
CDR_serialize_LongDouble(char **destBuffer,
                         const RTI_DOUBLE128 *instance,
                         RTI_BOOL byteSwap)
{
    CDR_serialize_LongDoubleMacro(destBuffer, instance, byteSwap);
}

void
CDR_deserialize_LongDouble(char **srcBuffer,
                           RTI_DOUBLE128 *instance,
                           RTI_BOOL byteSwap)
{
    CDR_deserialize_LongDoubleMacro(srcBuffer, instance, byteSwap);
}

#endif
/*
 * serialization / deserialization for the base primative types
*/
RTI_BOOL
CDR_Stream_serialize_Char(struct CDR_Stream_t *cdrs, const RTI_INT8 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) < CDR_OCTET_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_Stream_serialize_1Byte(cdrs, instance);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_deserialize_Char(struct CDR_Stream_t * cdrs, RTI_INT8 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) < CDR_OCTET_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_Stream_deserialize_1Byte(cdrs, instance);
    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_serialize_UnsignedShort(struct CDR_Stream_t *cdrs,
                                   const RTI_UINT16 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_SHORT_ALIGN);

    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_UNSIGNED_SHORT_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_serialize_UnsignedShort(&cdrs->buffptr, instance, cdrs->needbyteswap);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_deserialize_UnsignedShort(struct CDR_Stream_t *cdrs,
                                     RTI_UINT16 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_SHORT_ALIGN);

    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_UNSIGNED_SHORT_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_deserialize_UnsignedShort(&cdrs->buffptr, instance, cdrs->needbyteswap);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_serialize_UnsignedLong(struct CDR_Stream_t *cdrs,
                                  const RTI_UINT32 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_LONG_ALIGN);

    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_UNSIGNED_LONG_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_serialize_UnsignedLong(&cdrs->buffptr, instance, cdrs->needbyteswap);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_deserialize_UnsignedLong(struct CDR_Stream_t *cdrs,
                                    RTI_UINT32 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_LONG_ALIGN);

    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_UNSIGNED_LONG_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_deserialize_UnsignedLong(&cdrs->buffptr, instance, cdrs->needbyteswap);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_serialize_UnsignedLongLong(struct CDR_Stream_t *cdrs,
                                      const RTI_UINT64 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_LONG_LONG_ALIGN);

    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_UNSIGNED_LONG_LONG_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_serialize_UnsignedLongLong(&cdrs->buffptr, instance, cdrs->needbyteswap);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_deserialize_UnsignedLongLong(struct CDR_Stream_t *cdrs,
                                        RTI_UINT64 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_LONG_LONG_ALIGN);

    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_UNSIGNED_LONG_LONG_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_deserialize_UnsignedLongLong(&cdrs->buffptr, instance, cdrs->needbyteswap);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_serialize_LongDouble(struct CDR_Stream_t * cdrs,
                                const RTI_DOUBLE128 * instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_LONG_DOUBLE_ALIGN);

    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_LONG_DOUBLE_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_serialize_LongDouble(&cdrs->buffptr, instance, cdrs->needbyteswap);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_deserialize_LongDouble(struct CDR_Stream_t * cdrs,
                                  RTI_DOUBLE128 *instance)
{
    LOG_testPrecondition(cdrs == NULL || instance == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_LONG_DOUBLE_ALIGN);

    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_LONG_DOUBLE_SIZE)
    {
        return RTI_FALSE;
    }

    CDR_deserialize_LongDouble(&cdrs->buffptr, instance, cdrs->needbyteswap);

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_serialize_String(struct CDR_Stream_t * cdrs,
                            const char *in,
                            RTI_UINT32 maximumLength)
{
    RTI_UINT32 length = 0;

    LOG_testPrecondition(cdrs == NULL, return RTI_FALSE);

    /* a NULL string pointer is sent as length zero, while an empty string
       is sent as length one and the terminating NUL character */
    if (in != NULL)
    {
        /* string length must include the terminating NUL character  */
        /* maximum length is EXCLUSIVE of terminating NUL character */
        length = REDA_String_length(in) + 1;
        if (length > maximumLength + 1)
        {
            return RTI_FALSE;
        }

    }

    if (!CDR_Stream_serialize_UnsignedLong(cdrs, &length))
    {
        return RTI_FALSE;
    }

    if (length == 0)
    {
        return RTI_TRUE;
    }

    /* check to make sure the CDR stream has enough space for the correct
     * length of string
     */
    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) < length)
    {
        return RTI_FALSE;
    }

    /* copy the string to stream buffer and increment stream location */
    OSAPI_Memory_copy(cdrs->buffptr, in, length);

    cdrs->buffptr += length;

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_deserialize_String(struct CDR_Stream_t * cdrs,
                              char *out,
                              RTI_UINT32 maximumLength)
{
    RTI_UINT32 length;

    LOG_testPrecondition(cdrs == NULL || out == NULL || maximumLength == 0,
                         return RTI_FALSE);

    /* get the string length */
    if (!CDR_Stream_deserialize_UnsignedLong(cdrs, &length))
    {
        return RTI_FALSE;
    }

    /* maximum length is EXCLUSIVE of terminating NUL character */
    if (length > maximumLength + 1)
    {
        return RTI_FALSE;
    }

    if (length == 0)
    {
        *out = '\0';
        return RTI_TRUE;
    }

    /* check to make sure the CDR stream has enough space for the correct
     * length of string
     */
    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) < length)
    {
        return RTI_FALSE;
    }

    /* copy the string to user buffer and increment stream location */
    OSAPI_Memory_copy(out, cdrs->buffptr, length);

    cdrs->buffptr += length;

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_serialize_Wstring(struct CDR_Stream_t * cdrs,
                             const RTI_UINT32 *in,
                             RTI_UINT32 maximumLength)
{
    RTI_UINT32 length;

    LOG_testPrecondition(cdrs == NULL || in == NULL, return RTI_FALSE);

    length = CDR_Wstring_length(in) + 1;

    /* maximum length is EXCLUSIVE of terminating NUL character */
    if ((maximumLength < 1) || (length > maximumLength + 1))
    {
        return RTI_FALSE;    
    }

    if (!CDR_Stream_serialize_UnsignedLong(cdrs, &length))
    {
        return RTI_FALSE;    
    }

    return CDR_Stream_serialize_PrimitiveArray(cdrs, (void *)in, length,
                                               CDR_WCHAR_TYPE);
}


RTI_BOOL
CDR_Stream_deserialize_Wstring(struct CDR_Stream_t * cdrs,
                               RTI_UINT32 *out,
                               RTI_UINT32 maximumLength)
{
    RTI_UINT32 length;

    LOG_testPrecondition(cdrs == NULL || out == NULL || maximumLength == 0,
                         return RTI_FALSE);

    if (!CDR_Stream_deserialize_UnsignedLong(cdrs, &length))
    {
	return RTI_FALSE;
    }

    /* maximum length is EXCLUSIVE of terminating NUL character */
    if (length > maximumLength + 1)
    {
	return RTI_FALSE;
    }

    if (length == 0)
    {
	return RTI_TRUE;
    }

    return CDR_Stream_deserialize_PrimitiveArray(cdrs, (void *)out, length,
                                                 CDR_WCHAR_TYPE);
}


RTI_BOOL
CDR_Stream_serialize_2OctetsBigEndian(struct CDR_Stream_t *cdrs,
                                      const RTI_UINT16 *in)
{
    LOG_testPrecondition(cdrs == NULL || in == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_SHORT_ALIGN);

    /* check to make sure the CDR stream has enough space for the data type */
    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_UNSIGNED_SHORT_SIZE)
    {
        return RTI_FALSE;
    }

    *(cdrs->buffptr++) = *((RTI_INT8 *)(in));
    *(cdrs->buffptr++) = *((RTI_INT8 *)(in) + 1);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_deserialize_2OctetsBigEndian(struct CDR_Stream_t *cdrs,
                                        RTI_UINT16 *out)
{
    char *ptr = (char *)out;

    LOG_testPrecondition(cdrs == NULL || out == NULL, return RTI_FALSE);

    /* check to make sure the CDR stream has enough space for the data type */
    CDR_Stream_Align(cdrs, CDR_SHORT_ALIGN);

    /* check to make sure the CDR stream has enough space for the data type */
    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) <
        CDR_UNSIGNED_SHORT_SIZE)
    {
        return RTI_FALSE;
    }

    ptr[0] = *(cdrs->buffptr++);
    ptr[1] = *(cdrs->buffptr++);

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_serialize_ByteArray(struct CDR_Stream_t *cdrs,
                               const unsigned char *inArray,
                               RTI_UINT32 length)
{
    LOG_testPrecondition(cdrs == NULL || inArray == NULL, return RTI_FALSE);

    if (length == 0)
    {
        return RTI_TRUE;
    }

    /* check to make sure the CDR stream has enough space for
     * the correct length of string
     */
    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) < length)
    {
        return RTI_FALSE;
    }

    /* copy the array to stream buffer and increment stream location */
    OSAPI_Memory_copy(cdrs->buffptr, inArray, length);
    cdrs->buffptr += length;

    return RTI_TRUE;
}

RTI_BOOL
CDR_Stream_deserialize_ByteArray(struct CDR_Stream_t * cdrs,
                                 unsigned char *out,
                                 RTI_UINT32 length)
{
    LOG_testPrecondition(cdrs == NULL || out == NULL, return RTI_FALSE);

    if (length == 0)
    {
        return RTI_TRUE;
    }

    /* check to make sure the CDR stream has enough space for the
     * correct length of string
     */
    if ((cdrs->length - (cdrs->buffptr - cdrs->buffer)) < length)
    {
        return RTI_FALSE;
    }

    /* copy the string to user buffer and increment stream location */
    OSAPI_Memory_copy(out, cdrs->buffptr, length);
    cdrs->buffptr += length;

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_serialize_StringArray(struct CDR_Stream_t *cdrs,
                                 const void* in, 
                                 RTI_UINT32 length,
                                 RTI_UINT32 maximumStringLength,
                                 CdrPrimitiveType type)
{
    RTI_UINT32 i;

    LOG_testPrecondition(cdrs == NULL || in == NULL || maximumStringLength == 0,
                         return RTI_FALSE);

    /* maximum length is EXCLUSIVE of terminating NUL character */
    if (type == CDR_CHAR_TYPE)
    {
        for (i=0; i<length ;i++)
        {
            if (!CDR_Stream_serialize_String(cdrs,
                                             ((char **)in)[i],
                                             maximumStringLength))
            {
                return RTI_FALSE;
            }
        }
    }
    else if (type == CDR_WCHAR_TYPE)
    {
        for (i=0; i<length; i++)
        {
            if (!CDR_Stream_serialize_Wstring(cdrs,
                                              ((RTI_UINT32 **)in)[i],
                                              maximumStringLength))
            {
                return RTI_FALSE;
            }
        }
    }
    else
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_deserialize_StringArray(struct CDR_Stream_t *cdrs,
                                   void *out,
                                   RTI_UINT32 length,
                                   RTI_UINT32 maximumStringLength,
                                   CdrPrimitiveType type)
{
    RTI_UINT32 i;

    LOG_testPrecondition(cdrs == NULL || out == NULL || maximumStringLength == 0,
                         return RTI_FALSE);

    /* maximum length is EXCLUSIVE of terminating NUL character */
    if (type == CDR_CHAR_TYPE)
    {
        for (i=0; i<length; i++)
        {
            if (!CDR_Stream_deserialize_String(cdrs,
                                               ((char **)out)[i],
                                               maximumStringLength))
            {
                return RTI_FALSE;
            }            
        }     
    }
    else if (type == CDR_WCHAR_TYPE)
    {
        for (i=0; i<length; i++)
        {
            if (!CDR_Stream_deserialize_Wstring(cdrs,
                                                ((RTI_UINT32 **)out)[i],
                                                maximumStringLength))
            {
                return RTI_FALSE;
            }
        }
    }
    else
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;      
}


RTI_UINT32
CDR_get_maxSizeSerialized_StringArray(RTI_UINT32 currentAlignment,
                                      RTI_UINT32 length,
                                      RTI_UINT32 maximumStringLength,
                                      CdrPrimitiveType type)
{
    RTI_UINT32 addSize = 0;

    /* maximum string length is EXCLUSIVE of terminating NUL character so add 1 */
    if (length > 0) {
        if (type==CDR_WCHAR_TYPE)
        {
            addSize = CDR_get_maxSizeSerialized_Wstring(currentAlignment,(maximumStringLength+1));
            addSize += CDR_get_maxSizeSerialized_Wstring(currentAlignment + addSize,(maximumStringLength+1))*(length-1);
        }
        else
        {
            addSize = CDR_get_maxSizeSerialized_String(currentAlignment,(maximumStringLength+1));
            addSize += CDR_get_maxSizeSerialized_String(currentAlignment + addSize,(maximumStringLength+1))*(length-1);
        }
    }

    return addSize;      
}


RTI_BOOL
CDR_Stream_serialize_PrimitiveArray(struct CDR_Stream_t *cdrs,
                                    const void* in,
                                    RTI_UINT32 length,
                                    CdrPrimitiveType type)
{
    RTI_UINT32 i, align, elementSize;
    RTI_UINT16 *in2 = NULL;
    RTI_UINT32 *in4 = NULL;
    RTI_UINT64 *in8 = NULL;
    RTI_DOUBLE64 *inDouble = NULL;
    RTI_DOUBLE128 *inLongDouble = NULL;

    LOG_testPrecondition(
        (cdrs == NULL) || (cdrs->buffptr == NULL) || (in == NULL),
        return RTI_FALSE);

    /* no need to check for alignment or swap order for 1-byte types:
       CDR_CHAR_TYPE, CDR_OCTET_TYPE, CDR_BOOLEAN_TYPE */
    if (CDR_Primitive_get_size(type) == 1)
    {
        if (!CDR_Stream_checkSize(cdrs, length))
        {
            return RTI_FALSE;
        }
        OSAPI_Memory_copy(cdrs->buffptr, in, length);
        cdrs->buffptr += length;
        return RTI_TRUE;
    }

    /* multi-byte types */
    align = CDR_Primitive_get_alignment(type);
    elementSize = CDR_Primitive_get_size(type);

    /* all multi-byte types have alignment greater than one */
    if (!CDR_Stream_Align(cdrs, align))
    {
        return RTI_FALSE;
    }

    if (!CDR_Stream_checkSize(cdrs, elementSize * length))
    {
        return RTI_FALSE;
    }

    /* if not byte swapping then can just copy memory */
#if (OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS) && (CPU_FAMILY==ARM)
    if ((!cdrs->needbyteswap) && (type != CDR_DOUBLE_TYPE))
#else
    if (!cdrs->needbyteswap)
#endif
    {
        OSAPI_Memory_copy(cdrs->buffptr, in, elementSize * length);
        cdrs->buffptr += elementSize * length;
    }
    else
    {
        switch (type)
        {
          case CDR_SHORT_TYPE:
          case CDR_UNSIGNED_SHORT_TYPE:
            in2 = (RTI_UINT16 *)in;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_serialize_UnsignedShort(cdrs, in2);
                ++in2;
            }
            break;
          case CDR_WCHAR_TYPE:       
          case CDR_LONG_TYPE:
          case CDR_UNSIGNED_LONG_TYPE:
          case CDR_FLOAT_TYPE:
          case CDR_ENUM_TYPE:
            in4 = (RTI_UINT32 *)in;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_serialize_UnsignedLong(cdrs, in4);
                ++in4;
            }
            break;
          case CDR_LONG_LONG_TYPE:
          case CDR_UNSIGNED_LONG_LONG_TYPE:
            in8 = (RTI_UINT64 *)in;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_serialize_UnsignedLongLong(cdrs, in8);
                ++in8;
            }
            break;
          case CDR_LONG_DOUBLE_TYPE:
            inLongDouble = (RTI_DOUBLE128 *)in;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_serialize_LongDouble(cdrs, inLongDouble);
                ++inLongDouble;
            }
            break;
          case CDR_DOUBLE_TYPE:
            inDouble = (RTI_DOUBLE64 *)in;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_serialize_Double(cdrs, inDouble);
                ++inDouble;
            }
            break;

          default:
            return RTI_FALSE; /* otherwise, ERROR */
        }
    }

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_deserialize_PrimitiveArray(struct CDR_Stream_t *cdrs,
                                      void* out,
                                      RTI_UINT32 length,
                                      CdrPrimitiveType type)
{
    RTI_UINT32 i, align, elementSize;
    RTI_UINT16 *out2 = NULL;
    RTI_UINT32 *out4 = NULL;
    RTI_UINT64 *out8 = NULL;
    RTI_DOUBLE64 *outDouble = NULL;
    RTI_DOUBLE128 *outLongDouble = NULL;

    LOG_testPrecondition(
        (cdrs == NULL) || (cdrs->buffptr == NULL) || (out == NULL),
        return RTI_FALSE);

    /* no need to check for alignment or swap order for 1-byte types:
       CDR_CHAR_TYPE, CDR_OCTET_TYPE, CDR_BOOLEAN_TYPE */
    if (CDR_Primitive_get_size(type) == 1)
    {
        if (!CDR_Stream_checkSize(cdrs, length))
        {
            return RTI_FALSE;
        }
        OSAPI_Memory_copy(out, cdrs->buffptr, length);
        cdrs->buffptr += length;
        return RTI_TRUE;
    }

    /* multi-byte types */
    align = CDR_Primitive_get_alignment(type);
    elementSize = CDR_Primitive_get_size(type);

    /* all multi-byte types have alignment greater than one */
    if (!CDR_Stream_Align(cdrs, align))
    {
        return RTI_FALSE;
    }

    if (!CDR_Stream_checkSize(cdrs, elementSize * length))
    {
        return RTI_FALSE;
    }

    /* if not byte swapping then can just copy memory */
#if (OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS) && (CPU_FAMILY==ARM)
    if ((!cdrs->needbyteswap) && (type != CDR_DOUBLE_TYPE))
#else
    if (!cdrs->needbyteswap)
#endif
    {
        OSAPI_Memory_copy(out, cdrs->buffptr, elementSize*length);
        cdrs->buffptr += elementSize * length;
    }
    else
    {
        switch (type)
        {
          case CDR_SHORT_TYPE:
          case CDR_UNSIGNED_SHORT_TYPE:
            out2 = (RTI_UINT16 *)out;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_deserialize_UnsignedShort(cdrs, out2);
                ++out2;
            }
            break;
          case CDR_WCHAR_TYPE:
          case CDR_LONG_TYPE:
          case CDR_UNSIGNED_LONG_TYPE:
          case CDR_FLOAT_TYPE:
          case CDR_ENUM_TYPE:
            out4 = (RTI_UINT32 *)out;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_deserialize_UnsignedLong(cdrs, out4);
                ++out4;
            }
            break;
          case CDR_LONG_LONG_TYPE:
          case CDR_UNSIGNED_LONG_LONG_TYPE:
            out8 = (RTI_UINT64 *)out;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_deserialize_UnsignedLongLong(cdrs, out8);
                ++out8;
            }
            break;
          case CDR_LONG_DOUBLE_TYPE:
            outLongDouble = (RTI_DOUBLE128 *)out;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_deserialize_LongDouble(cdrs, outLongDouble);
                ++outLongDouble;
            }
            break;
          case CDR_DOUBLE_TYPE:
            outDouble = (RTI_DOUBLE64 *)out;
            for (i = 0; i < length; ++i)
            {
                CDR_Stream_deserialize_Double(cdrs, outDouble);
                ++outDouble;
            }
            break;

          default:
            return RTI_FALSE; /* otherwise, ERROR */
        }
    }

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_serialize_NonPrimitiveArray(struct CDR_Stream_t *cdrs,
                                       const void* in,
                                       RTI_UINT32 length,
                                       RTI_UINT32 elementSize,
                                       CDR_Stream_SerializeFunction
                                         serializeFunction,
                                       void * param)
{
    RTI_UINT32 i;
    const char *arrayElement;

    LOG_testPrecondition(cdrs == NULL || cdrs->buffptr == NULL || in == NULL || 
                         elementSize < 0 || serializeFunction == NULL,
                         return RTI_FALSE);

    arrayElement = (const char *)in;
    for (i = 0; i < length; ++i)
    {
        if (!serializeFunction(cdrs, arrayElement, param))
        {
            return RTI_FALSE;
        }
        arrayElement += elementSize;
    }
    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_deserialize_NonPrimitiveArray(struct CDR_Stream_t *cdrs,
                                         void* out,
                                         RTI_UINT32 length,
                                         RTI_UINT32 elementSize,
                                         CDR_Stream_DeserializeFunction
                                           deserializeFunction,
                                         void * param)
{
    RTI_UINT32 i;
    char *arrayElement;

    LOG_testPrecondition(cdrs == NULL || cdrs->buffptr == NULL || out == NULL ||
                         elementSize < 0 || deserializeFunction == NULL,
                         return RTI_FALSE);

    arrayElement = (char *)out;
    for (i = 0; i < length; ++i)
    {
        if (!deserializeFunction(cdrs, arrayElement, param))
        {
            return RTI_FALSE;
        }
        arrayElement += elementSize;
    }
    return RTI_TRUE;
}


RTI_UINT32
CDR_get_maxSizeSerialized_NonPrimitiveArray(RTI_UINT32 currentAlignment,
                                            RTI_UINT32 length,
                                            CDR_Stream_GetSerializedSizeFunction
                                              getSerializedSizeFunc,
                                            void * param)
{
    RTI_UINT32 i = 0, addSize = 0, samplesInLoop, loopSize, loopCount;
    RTI_INT32 alignment, align[8];
    RTI_UINT32 sizeAlign[8];

    void * plugin = NULL; /* FIXME */

    LOG_testPrecondition(getSerializedSizeFunc == NULL, return RTI_FALSE);

    for (i=0; i<8; i++)
    {
        align[i] = -1;
        sizeAlign[i] = 0;
    }

    i=0;
    alignment = (currentAlignment % 8);

    while (align[alignment] < 0 && i < length)
    {
        align[alignment] = i;
        sizeAlign[alignment] = addSize;
        addSize += 
            getSerializedSizeFunc(plugin, currentAlignment + addSize, NULL);
        i++;
        alignment = (int)((currentAlignment + addSize)% 8);
    }

    if (i< length)
    {
        samplesInLoop = i - align[alignment];
        loopSize = addSize - sizeAlign[alignment];
        loopCount = (length - i)/samplesInLoop;
        addSize += (loopSize* loopCount);
        i += samplesInLoop*loopCount;
    }

    for(;i < length; ++i)
    {
        addSize += 
            getSerializedSizeFunc(plugin, currentAlignment + addSize, NULL);
    }
    return addSize;
}


RTI_BOOL
CDR_Stream_serialize_StringSequence(struct CDR_Stream_t *cdrs,
                                    const struct REDA_Sequence* in,
                                    RTI_UINT32 maximumStringLength,
                                    CdrPrimitiveType type)
{
    RTI_UINT32 i, length;
    const char * inStr;
    const RTI_UINT32 * inWStr;

    LOG_testPrecondition(
        (cdrs == NULL) || (cdrs->buffptr == NULL) || (in == NULL),
        return RTI_FALSE);

    length = REDA_Sequence_get_length(in);

    /* must have at least one element for non-zero sequence */
    if ((length != 0) && (REDA_Sequence_get_reference(in, 0) == NULL))
    {
        return RTI_FALSE;

    }

    /* serialize sequence length */
    if (!CDR_Stream_serialize_UnsignedLong(cdrs, &length))
    {
	return RTI_FALSE;
    }

    if (length == 0)
    {
	return RTI_TRUE;
    }

    if (type == CDR_CHAR_TYPE)
    {
        for (i=0; i<length ;i++)
        {
            inStr = *(const char **) REDA_Sequence_get_reference(in, i);
            if (!CDR_Stream_serialize_String(cdrs, inStr, maximumStringLength))
            {
                return RTI_FALSE;
            }
        }
    }
    else if (type == CDR_WCHAR_TYPE)
    {
        for (i=0; i<length; i++)
        {
            inWStr = *(const RTI_UINT32 **) REDA_Sequence_get_reference(in, i);
            if (!CDR_Stream_serialize_Wstring(cdrs, inWStr, maximumStringLength))
            {
                return RTI_FALSE;
            }
        }
    }
    else
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_deserialize_StringSequence(struct CDR_Stream_t *cdrs,
                                      struct REDA_Sequence* out,
                                      RTI_UINT32 maximumStringLength,
                                      CdrPrimitiveType type)
{
    RTI_UINT32 i, length;
    char * outStr;
    RTI_UINT32 * outWStr;

    LOG_testPrecondition(
        (cdrs == NULL) || (cdrs->buffptr == NULL) || (out == NULL),
        return RTI_FALSE);


    /* deserialize sequence length */
    if (!CDR_Stream_deserialize_UnsignedLong(cdrs, &length))
    {
	return RTI_FALSE;
    }

    if (length > REDA_Sequence_get_maximum(out))
    {
        return RTI_FALSE;

    }

    if (!REDA_Sequence_set_length(out, length))
    {
        return RTI_FALSE;
    }

    if (length == 0)
    {
        return RTI_TRUE;
    }

    if (type == CDR_CHAR_TYPE)
    {
        for (i=0; i<length; i++)
        {
            outStr = *(char **) REDA_Sequence_get_reference(out, i);
            if (!CDR_Stream_deserialize_String(cdrs, outStr, maximumStringLength))
            {
                return RTI_FALSE;
            }            
        }     
    }
    else if (type == CDR_WCHAR_TYPE)
    {
        for (i=0; i<length; i++)
        {
            outWStr = *(RTI_UINT32 **) REDA_Sequence_get_reference(out, i);
            if (!CDR_Stream_deserialize_Wstring(cdrs, outWStr, maximumStringLength))
            {
                return RTI_FALSE;
            }
        }
    }
    else
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;      
}


RTI_UINT32
CDR_get_maxSizeSerialized_StringSequence(RTI_UINT32 currentAlignment,
                                         RTI_UINT32 length,
                                         RTI_UINT32 maximumStringLength,
                                         CdrPrimitiveType type)
{
    RTI_UINT32 addSize;

    addSize = CDR_get_maxSizeSerialized_UnsignedLong(currentAlignment);

    /* maximum string length is EXCLUSIVE of terminating NUL character so add 1 */
    if (length > 0) {
        if (type==CDR_WCHAR_TYPE)
        {
            addSize += CDR_get_maxSizeSerialized_Wstring(currentAlignment + addSize,(maximumStringLength+1));
            addSize += CDR_get_maxSizeSerialized_Wstring(currentAlignment + addSize,(maximumStringLength+1))*(length-1);
        }
        else
        {
            addSize += CDR_get_maxSizeSerialized_String(currentAlignment + addSize,(maximumStringLength+1));
            addSize += CDR_get_maxSizeSerialized_String(currentAlignment + addSize,(maximumStringLength+1))*(length-1);
        }
    }

    return addSize;      
}


RTI_BOOL
CDR_Stream_serialize_PrimitiveSequence(struct CDR_Stream_t *cdrs,
                                       const struct REDA_Sequence* in,
                                       CdrPrimitiveType type)
{
    RTI_UINT32 i, align, elementSize, length;
    RTI_INT8 *in1 = NULL;
    RTI_UINT16 *in2 = NULL;
    RTI_UINT32 *in4 = NULL;
    RTI_UINT64 *in8 = NULL;
    RTI_DOUBLE64 *inDouble = NULL;
    RTI_DOUBLE128 *inLongDouble = NULL;
    char * buffer;

    LOG_testPrecondition(
        (cdrs == NULL) || (cdrs->buffptr == NULL) || (in == NULL),
        return RTI_FALSE);

    length = REDA_Sequence_get_length(in);
    /* PrimitiveSequences always use contiguous buffer */
    buffer = (char *) REDA_Sequence_get_buffer(in);

    /* must have at least one element for non-zero sequence */
    if ((length != 0) && (buffer == NULL))
    {
        return RTI_FALSE;

    }

    /* serialize sequence length */
    if (!CDR_Stream_serialize_UnsignedLong(cdrs, &length))
    {
	return RTI_FALSE;
    }

    if (length == 0)
    {
	return RTI_TRUE;
    }

    /* no need to check for alignment or swap order for 1-byte types:
       CDR_CHAR_TYPE, CDR_OCTET_TYPE, CDR_BOOLEAN_TYPE */
    if ((CDR_Primitive_get_size(type) == 1) &&
        !REDA_Sequence_has_discontiguous_buffer(in))
    {
        if (!CDR_Stream_checkSize(cdrs, length))
        {
            return RTI_FALSE;
        }
        OSAPI_Memory_copy(cdrs->buffptr, buffer, length);
        cdrs->buffptr += length;
        return RTI_TRUE;
    }

    align = CDR_Primitive_get_alignment(type);
    elementSize = CDR_Primitive_get_size(type);

    if (align > 1) {
        if (!CDR_Stream_Align(cdrs, align))
        {
            return RTI_FALSE;
        }
    }

    if (!CDR_Stream_checkSize(cdrs, elementSize * length))
    {
        return RTI_FALSE;
    }

    /* if not byte swapping and contiguous buffer then can just copy memory */
#if (OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS) && (CPU_FAMILY==ARM)
    if ((!cdrs->needbyteswap) && (type != CDR_DOUBLE_TYPE) &&
        !REDA_Sequence_has_discontiguous_buffer(in))
#else
    if ((!cdrs->needbyteswap) && !REDA_Sequence_has_discontiguous_buffer(in))
#endif
    {
        OSAPI_Memory_copy(cdrs->buffptr, buffer, elementSize * length);
        cdrs->buffptr += elementSize * length;
    }
    else
    {
        switch (type)
        {
          case CDR_CHAR_TYPE:
          case CDR_OCTET_TYPE:
          case CDR_BOOLEAN_TYPE:
            for (i = 0; i < length; ++i)
            {
                in1 = (RTI_INT8 *) REDA_Sequence_get_reference(in, i);
                CDR_Stream_serialize_Char(cdrs, in1);
            }
            break;
          case CDR_SHORT_TYPE:
          case CDR_UNSIGNED_SHORT_TYPE:
            for (i = 0; i < length; ++i)
            {
                in2 = (RTI_UINT16 *) REDA_Sequence_get_reference(in, i);
                CDR_Stream_serialize_UnsignedShort(cdrs, in2);
            }
            break;
          case CDR_WCHAR_TYPE:       
          case CDR_LONG_TYPE:
          case CDR_UNSIGNED_LONG_TYPE:
          case CDR_FLOAT_TYPE:
          case CDR_ENUM_TYPE:
            for (i = 0; i < length; ++i)
            {
                in4 = (RTI_UINT32 *) REDA_Sequence_get_reference(in, i);
                CDR_Stream_serialize_UnsignedLong(cdrs, in4);
            }
            break;
          case CDR_LONG_LONG_TYPE:
          case CDR_UNSIGNED_LONG_LONG_TYPE:
            for (i = 0; i < length; ++i)
            {
                in8 = (RTI_UINT64 *) REDA_Sequence_get_reference(in, i);
                CDR_Stream_serialize_UnsignedLongLong(cdrs, in8);
            }
            break;
          case CDR_LONG_DOUBLE_TYPE:
            for (i = 0; i < length; ++i)
            {
                inLongDouble = (RTI_DOUBLE128 *) REDA_Sequence_get_reference(in, i);
                CDR_Stream_serialize_LongDouble(cdrs, inLongDouble);
            }
            break;
          case CDR_DOUBLE_TYPE:
            for (i = 0; i < length; ++i)
            {
                inDouble = (RTI_DOUBLE64 *) REDA_Sequence_get_reference(in, i);
                CDR_Stream_serialize_Double(cdrs, inDouble);
            }
            break;

          default:
            return RTI_FALSE; /* otherwise, ERROR */
        }
    }

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_deserialize_PrimitiveSequence(struct CDR_Stream_t *cdrs,
                                         struct REDA_Sequence* out,
                                         CdrPrimitiveType type)
{
    RTI_INT32 i, align, elementSize, length;
    RTI_INT8 *out1 = NULL;
    RTI_UINT16 *out2 = NULL;
    RTI_UINT32 *out4 = NULL;
    RTI_UINT64 *out8 = NULL;
    RTI_DOUBLE64 *outDouble = NULL;
    RTI_DOUBLE128 *outLongDouble = NULL;
    char * buffer;

    LOG_testPrecondition(
        (cdrs == NULL) || (cdrs->buffptr == NULL) || (out == NULL),
        return RTI_FALSE);

    /* deserialize sequence length */
    if (!CDR_Stream_deserialize_Long(cdrs, &length))
    {
	return RTI_FALSE;
    }

    if (length > REDA_Sequence_get_maximum(out))
    {
        return RTI_FALSE;

    }

    if (!REDA_Sequence_set_length(out, length))
    {
        return RTI_FALSE;
    }

    if (length == 0)
    {
        return RTI_TRUE;
    }

    /* PrimitiveSequences always use contiguous buffer */
    buffer = (char *) REDA_Sequence_get_buffer(out);

    /* no need to check for alignment or swap order for 1-byte types:
       CDR_CHAR_TYPE, CDR_OCTET_TYPE, CDR_BOOLEAN_TYPE */
    if ((CDR_Primitive_get_size(type) == 1) &&
        !REDA_Sequence_has_discontiguous_buffer(out))
    {
        if (!CDR_Stream_checkSize(cdrs, length))
        {
            return RTI_FALSE;
        }
        OSAPI_Memory_copy(buffer, cdrs->buffptr, length);
        cdrs->buffptr += length;
        return RTI_TRUE;
    }

    align = CDR_Primitive_get_alignment(type);
    elementSize = CDR_Primitive_get_size(type);

    if (align > 1) {
        if (!CDR_Stream_Align(cdrs, align))
        {
            return RTI_FALSE;
        }
    }

    if (!CDR_Stream_checkSize(cdrs, elementSize * length))
    {
        return RTI_FALSE;
    }

    /* if not byte swapping and contiguous buffer then can just copy memory */
#if (OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS) && (CPU_FAMILY==ARM)
    if ((!cdrs->needbyteswap) && (type != CDR_DOUBLE_TYPE) &&
        !REDA_Sequence_has_discontiguous_buffer(out))
#else
    if ((!cdrs->needbyteswap) && !REDA_Sequence_has_discontiguous_buffer(out))
#endif
    {
        OSAPI_Memory_copy(buffer, cdrs->buffptr, elementSize*length);
        cdrs->buffptr += elementSize * length;
    }
    else
    {
        switch (type)
        {
          case CDR_CHAR_TYPE:
          case CDR_OCTET_TYPE:
          case CDR_BOOLEAN_TYPE:
            for (i = 0; i < length; ++i)
            {
                out1 = (RTI_INT8 *) REDA_Sequence_get_reference(out, i);
                CDR_Stream_deserialize_Char(cdrs, out1);
            }
            break;
          case CDR_SHORT_TYPE:
          case CDR_UNSIGNED_SHORT_TYPE:
            for (i = 0; i < length; ++i)
            {
                out2 = (RTI_UINT16 *) REDA_Sequence_get_reference(out, i);
                CDR_Stream_deserialize_UnsignedShort(cdrs, out2);
            }
            break;
          case CDR_WCHAR_TYPE:
          case CDR_LONG_TYPE:
          case CDR_UNSIGNED_LONG_TYPE:
          case CDR_FLOAT_TYPE:
          case CDR_ENUM_TYPE:
            for (i = 0; i < length; ++i)
            {
                out4 = (RTI_UINT32 *) REDA_Sequence_get_reference(out, i);
                CDR_Stream_deserialize_UnsignedLong(cdrs, out4);
            }
            break;
          case CDR_LONG_LONG_TYPE:
          case CDR_UNSIGNED_LONG_LONG_TYPE:
            for (i = 0; i < length; ++i)
            {
                out8 = (RTI_UINT64 *) REDA_Sequence_get_reference(out, i);
                CDR_Stream_deserialize_UnsignedLongLong(cdrs, out8);
            }
            break;
          case CDR_LONG_DOUBLE_TYPE:
            for (i = 0; i < length; ++i)
            {
                outLongDouble = (RTI_DOUBLE128 *) REDA_Sequence_get_reference(out, i);
                CDR_Stream_deserialize_LongDouble(cdrs, outLongDouble);
            }
            break;
          case CDR_DOUBLE_TYPE:
            for (i = 0; i < length; ++i)
            {
                outDouble = (RTI_DOUBLE64 *) REDA_Sequence_get_reference(out, i);
                CDR_Stream_deserialize_Double(cdrs, outDouble);
            }
            break;

          default:
            return RTI_FALSE; /* otherwise, ERROR */
        }
    }

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_serialize_NonPrimitiveSequence(struct CDR_Stream_t *cdrs,
                                          const struct REDA_Sequence* in,
                                          CDR_Stream_SerializeFunction
                                            serializeFunction,
                                          void * param)
{
    RTI_UINT32 i, length;
    void * buffer;

    LOG_testPrecondition(cdrs == NULL || cdrs->buffptr == NULL ||
                         serializeFunction == NULL, return RTI_FALSE);

    length = REDA_Sequence_get_length(in);

    if (!CDR_Stream_serialize_UnsignedLong(cdrs, &length))
    {
        return RTI_FALSE;
    }

    for (i = 0; i < length; ++i)
    {
        buffer = REDA_Sequence_get_reference(in, i);
        if (buffer == NULL) {
            return RTI_FALSE;
        }
        if (!serializeFunction(cdrs, buffer, param))
        {
            return RTI_FALSE;
        }
    }

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_deserialize_NonPrimitiveSequence(struct CDR_Stream_t *cdrs,
                                            struct REDA_Sequence* out,
                                            CDR_Stream_DeserializeFunction
                                              deserializeFunction,
                                            void * param)
{
    RTI_UINT32 i, length;
    void * buffer;

    LOG_testPrecondition(cdrs == NULL || cdrs->buffptr == NULL || 
                         out == NULL || deserializeFunction == NULL,
                         return RTI_FALSE);

    if (!CDR_Stream_deserialize_UnsignedLong(cdrs, &length))
    {
        return RTI_FALSE;
    }

    if (length > REDA_Sequence_get_maximum(out))
    {
        return RTI_FALSE;
    }

    if (!REDA_Sequence_set_length(out, length))
    {
        return RTI_FALSE;
    }

    if (length == 0)
    {
        return RTI_TRUE;
    }

    for (i = 0; i < length; ++i)
    {
        buffer = REDA_Sequence_get_reference(out, i);
        if (buffer == NULL) {
            return RTI_FALSE;
        }
        if (!deserializeFunction(cdrs, buffer, param))
        {
            return RTI_FALSE;
        }
    }

    return RTI_TRUE;
}


RTI_UINT32
CDR_get_maxSizeSerialized_NonPrimitiveSequence(RTI_UINT32 currentAlignment,
                                               RTI_UINT32 length,
                                               CDR_Stream_GetSerializedSizeFunction
                                                 getSerializedSizeFunc,
                                               void * param)
{
    RTI_UINT32 addSize;

    addSize = CDR_get_maxSizeSerialized_UnsignedLong(currentAlignment);

    addSize += CDR_get_maxSizeSerialized_NonPrimitiveArray(currentAlignment + addSize,
                                                           length,
                                                           getSerializedSizeFunc,
                                                           param);

    return addSize;
}

