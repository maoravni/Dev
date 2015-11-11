/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef cdr_serialize_h
#define cdr_serialize_h



#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif
#ifndef reda_sequenceNumber_h
#include "reda/reda_sequenceNumber.h"
#endif
#ifndef cdr_dll_h
#include "cdr/cdr_dll.h"
#endif
#ifndef cdr_cdr_type_h
#include "cdr/cdr_cdr_type.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef cdr_serialize_impl_h
#include "cdr/cdr_serialize_impl.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* The size is the same for signed or unsigned longs and shorts */
#define CDR_get_maxSizeSerialized_Octet            CDR_get1ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_Char             CDR_get1ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_Boolean          CDR_get1ByteMaxSizeSerialized

#define CDR_get_maxSizeSerialized_Short            CDR_get2ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_UnsignedShort    CDR_get2ByteMaxSizeSerialized

#define CDR_get_maxSizeSerialized_Long             CDR_get4ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_UnsignedLong     CDR_get4ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_Wchar            CDR_get4ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_Enum             CDR_get4ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_Float            CDR_get4ByteMaxSizeSerialized

#define CDR_get_maxSizeSerialized_LongLong         CDR_get8ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_UnsignedLongLong CDR_get8ByteMaxSizeSerialized
#define CDR_get_maxSizeSerialized_Double           CDR_get8ByteMaxSizeSerialized

#define CDR_get_maxSizeSerialized_LongDouble       CDR_get16ByteMaxSizeSerialized

/*
 Declare serialization / deserialization for base primative types:
 UnsignedShort (2 bytes), UnsignedLong (4 bytes), UnsignedLongLong (8 bytes),
 LongDouble (16 bytes)
*/
CDRDllExport void
CDR_serialize_UnsignedShort(char **destBuffer, const RTI_UINT16 * instance,
                           RTI_BOOL byteSwap);

CDRDllExport void
CDR_deserialize_UnsignedShort(char **srcBuffer, RTI_UINT16 * instance,
                             RTI_BOOL byteSwap);

CDRDllExport void
CDR_serialize_UnsignedLong(char **destBuffer, const RTI_UINT32 * instance,
                          RTI_BOOL byteSwap);

CDRDllExport void
CDR_deserialize_UnsignedLong(char **srcBuffer, RTI_UINT32 * instance,
                            RTI_BOOL byteSwap);

CDRDllExport void
CDR_serialize_UnsignedLongToBigEndian(char **destBuffer, const RTI_UINT32 * instance);

CDRDllExport void
CDR_deserialize_UnsignedLongFromBigEndian(char **srcBuffer, RTI_UINT32 * instance);

CDRDllExport void
CDR_serialize_UnsignedLongLong(char **destBuffer, const RTI_UINT64 * instance,
                              RTI_BOOL byteSwap);

CDRDllExport void
CDR_deserialize_UnsignedLongLong(char **srcBuffer, RTI_UINT64 * instance,
                                RTI_BOOL byteSwap);

CDRDllExport void
CDR_serialize_LongDouble(char **destBuffer, const RTI_DOUBLE128 * instance,
                        RTI_BOOL byteSwap);

CDRDllExport void
CDR_deserialize_LongDouble(char **srcBuffer, RTI_DOUBLE128 * instance,
                          RTI_BOOL byteSwap);

CDRDllExport void
CDR_serialize_REDASequenceNumber(char **destBuffer,
                                const struct REDA_SequenceNumber *instance,
                                RTI_BOOL byteSwap);

CDRDllExport void
CDR_deserialize_REDASequenceNumber(char **srcBuffer,
                                  struct REDA_SequenceNumber *instance,
                                  RTI_BOOL byteSwap);

/*
 Define serialization / deserialization macros for primative types that are
 implemented by equaivalent sized base primative types
*/
#define CDR_serialize_Short(__buf,__inst,_b) \
    CDR_serialize_UnsignedShort(__buf,((RTI_UINT16*)__inst),_b)

#define CDR_deserialize_Short(__buf,__inst,_b) \
    CDR_deserialize_UnsignedShort(__buf,((RTI_UINT16*)__inst),_b)

#define CDR_serialize_Wchar(__buf,__inst,_b) \
    CDR_serialize_UnsignedShort(__buf,((RTI_UINT16*)__inst),_b)

#define CDR_deserialize_Wchar(__buf,__inst,_b) \
    CDR_deserialize_UnsignedShort(__buf,((RTI_UINT16*)__inst),_b)

#define CDR_serialize_Enum(__buf,__inst,_b) \
    CDR_serialize_UnsignedShort(__buf,((RTI_UINT16*)__inst),_b)

#define CDR_deserialize_Enum(__buf,__inst,_b) \
    CDR_deserialize_UnsignedShort(__buf,((RTI_UINT16*)__inst),_b)

#define CDR_serialize_Long(__buf,__inst,_b) \
    CDR_serialize_UnsignedLong(__buf,((RTI_UINT32*)__inst),_b)

#define CDR_deserialize_Long(__buf,__inst,_b) \
    CDR_deserialize_UnsignedLong(__buf,((RTI_UINT32*)__inst),_b)

#define CDR_serialize_Float(__buf,__inst,_b) \
    CDR_serialize_UnsignedLong(__buf,((RTI_UINT32*)__inst),_b)

#define CDR_deserialize_Float(__buf,__inst,_b) \
    CDR_deserialize_UnsignedLong(__buf,((RTI_UINT32*)__inst),_b)

#define CDR_serialize_LongLong(__s,__i,_b) \
    CDR_serialize_UnsignedLongLong(__s,((RTI_UINT64*)__i),_b)

#define CDR_deserialize_LongLong(__s,__i) \
    CDR_deserialize_UnsignedLongLong(__s,((RTI_UINT64*)__i))

#define CDR_serialize_Double(__buf,__inst,_b) \
    CDR_serialize_UnsignedLongLong(__buf,((RTI_UINT64*)__inst),_b)

#define CDR_deserialize_Double(__buf,__inst,_b) \
    CDR_deserialize_UnsignedLongLong(__buf,((RTI_UINT64*)__inst),_b)

#define CDR_serialize_REDASequenceNumber(__buf,__inst,_b) \
    CDR_serialize_Long(__buf, (RTI_UINT32*)&(__inst)->high, _b); \
    CDR_serialize_UnsignedLong(__buf, (RTI_UINT32*)&(__inst)->low, _b)

#define CDR_deserialize_REDASequenceNumber(__buf,__inst,_b) \
    CDR_deserialize_Long(__buf, (RTI_UINT32*)&(__inst)->high, _b); \
    CDR_deserialize_UnsignedLong(__buf, (RTI_UINT32*)&(__inst)->low, _b)

/*
 Declare stream serialization / deserialization for base primative types:
 Char (1 byte), UnsignedShort (2 bytes), UnsignedLong (4 bytes),
 UnsignedLongLong (8 bytes), LongDouble (16 bytes)
*/
CDRDllExport RTI_BOOL
CDR_Stream_serialize_Char(struct CDR_Stream_t *cdrs, const RTI_INT8 * instance);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_Char(struct CDR_Stream_t *cdrs, RTI_INT8 * instance);

CDRDllExport RTI_BOOL
CDR_Stream_serialize_UnsignedShort(struct CDR_Stream_t *cdrs,
                                   const RTI_UINT16 * instance);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_UnsignedShort(struct CDR_Stream_t *cdrs,
                                     RTI_UINT16 * instance);

CDRDllExport RTI_BOOL
CDR_Stream_serialize_UnsignedLong(struct CDR_Stream_t *cdrs,
                                  const RTI_UINT32 * instance);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_UnsignedLong(struct CDR_Stream_t *cdrs,
                                    RTI_UINT32 * instance);

CDRDllExport RTI_BOOL
CDR_Stream_serialize_UnsignedLongLong(struct CDR_Stream_t *cdrs,
                                      const RTI_UINT64 * instance);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_UnsignedLongLong(struct CDR_Stream_t *cdrs,
                                        RTI_UINT64 * instance);

CDRDllExport RTI_BOOL
CDR_Stream_serialize_LongDouble(struct CDR_Stream_t *cdrs,
                                const RTI_DOUBLE128 * in);

CDRDllExport  RTI_BOOL
CDR_Stream_deserialize_LongDouble(struct CDR_Stream_t *cdrs,
                                  RTI_DOUBLE128 * out);

CDRDllExport RTI_BOOL
CDR_Stream_serialize_String(struct CDR_Stream_t *cdrs,
                            const char *in, RTI_UINT32 maximumLength);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_String(struct CDR_Stream_t *cdrs,
                              char *out, RTI_UINT32 maximumLength);

CDRDllExport RTI_BOOL
CDR_Stream_serialize_Wstring(struct CDR_Stream_t *cdrs,
                           const RTI_UINT32 *in, RTI_UINT32 maximumLength);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_Wstring(struct CDR_Stream_t *cdrs,
                             RTI_UINT32 *out, RTI_UINT32 maximumLength);

/* serialize 2 bytes in big endian, regardless of host endianness */
CDRDllExport RTI_BOOL
CDR_Stream_serialize_2OctetsBigEndian(struct CDR_Stream_t *cdrs,
                                    const RTI_UINT16 * in);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_2OctetsBigEndian(struct CDR_Stream_t *cdrs,
                                      RTI_UINT16 * out);

CDRDllExport RTI_BOOL
CDR_Stream_serialize_REDASequenceNumber(struct CDR_Stream_t *cdrs,
                                         const struct REDA_SequenceNumber *in);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_REDASequenceNumber(struct CDR_Stream_t *cdrs,
                                          struct REDA_SequenceNumber *out);

/*
 Define serialization / deserialization stream macros for primative types that
 are implemented by equaivalent sized base primative types
*/
#define CDR_Stream_serialize_Boolean(__s,__i) \
    CDR_Stream_serialize_Char(__s,((RTI_INT8*)__i))

#define CDR_Stream_deserialize_Boolean(__s,__i) \
    CDR_Stream_deserialize_Char(__s,((RTI_INT8*)__i))

#define CDR_Stream_serialize_Octet(__s,__i) \
    CDR_Stream_serialize_Char(__s,((RTI_INT8*)__i))

#define CDR_Stream_deserialize_Octet(__s,__i) \
    CDR_Stream_deserialize_Char(__s,((RTI_INT8*)__i))

#define CDR_Stream_serialize_Short(__s,__i) \
    CDR_Stream_serialize_UnsignedShort(__s,((RTI_UINT16*)__i))

#define CDR_Stream_deserialize_Short(__s,__i) \
    CDR_Stream_deserialize_UnsignedShort(__s,((RTI_UINT16*)__i))

#define CDR_Stream_serialize_Wchar(__s,__i) \
    CDR_Stream_serialize_UnsignedLong(__s,(RTI_UINT32*)__i)

#define CDR_Stream_deserialize_Wchar(__s,__i) \
    CDR_Stream_deserialize_UnsignedLong(__s,(RTI_UINT32*)__i)

#define CDR_Stream_serialize_Enum(__s,__i) \
    CDR_Stream_serialize_UnsignedLong(__s,(RTI_UINT32*)__i)

#define CDR_Stream_deserialize_Enum(__s,__i) \
    CDR_Stream_deserialize_UnsignedLong(__s,(RTI_UINT32*)__i)

#define CDR_Stream_serialize_Long(__s,__i) \
    CDR_Stream_serialize_UnsignedLong(__s,((RTI_UINT32*)__i))

#define CDR_Stream_deserialize_Long(__s,__i) \
    CDR_Stream_deserialize_UnsignedLong(__s,((RTI_UINT32*)__i))

#define CDR_Stream_serialize_Float(__s,__i) \
    CDR_Stream_serialize_UnsignedLong(__s,((RTI_UINT32*)__i))

#define CDR_Stream_deserialize_Float(__s,__i) \
    CDR_Stream_deserialize_UnsignedLong(__s,((RTI_UINT32*)__i))

#define CDR_Stream_serialize_LongLong(__s,__i) \
    CDR_Stream_serialize_UnsignedLongLong(__s,((RTI_UINT64*)__i))

#define CDR_Stream_deserialize_LongLong(__s,__i) \
    CDR_Stream_deserialize_UnsignedLongLong(__s,((RTI_UINT64*)__i))

#define CDR_Stream_serialize_Double(__s,__i) \
    CDR_Stream_serialize_UnsignedLongLong(__s,((RTI_UINT64*)__i))

#define CDR_Stream_deserialize_Double(__s,__i) \
    CDR_Stream_deserialize_UnsignedLongLong(__s,((RTI_UINT64*)__i))

#define CDR_Stream_serialize_REDASequenceNumber(__s,__i) \
    (CDR_Stream_serialize_Long(__s, (RTI_UINT32*)&(__i)->high) && \
     CDR_Stream_serialize_UnsignedLong(__s, (RTI_UINT32*)&(__i)->low))

#define CDR_Stream_deserialize_REDASequenceNumber(__s,__i) \
    (CDR_Stream_deserialize_Long(__s, (RTI_UINT32*)&(__i)->high) && \
     CDR_Stream_deserialize_UnsignedLong(__s, (RTI_UINT32*)&(__i)->low))

typedef RTI_BOOL
(*CDR_Stream_SerializeFunction) (struct CDR_Stream_t * stream,
                                 const void *sample,
                                 void *param);
                                                      
typedef RTI_BOOL
(*CDR_Stream_DeserializeFunction) (struct CDR_Stream_t * stream,
                                   void *sample,
                                   void *param);

struct NDDS_Type_Plugin;            /* forward declaration */

typedef RTI_UINT32
(*CDR_Stream_GetSerializedSizeFunction)(struct NDDS_Type_Plugin *plugin,
                                        RTI_UINT32 current_alignment,
                                        void *param);

/*
 * Array and Sequence Serialization / Deserialization
*/

CDRDllExport RTI_BOOL
CDR_Stream_serialize_ByteArray(struct CDR_Stream_t *cdrs,
                             const unsigned char *inArray,
                             RTI_UINT32 length);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_ByteArray(struct CDR_Stream_t *cdrs,
                               unsigned char *out,
                               RTI_UINT32 length);

#define CDR_get_maxSizeSerialized_ByteArray(currentAlignment, length, type) \
  (length)

CDRDllExport  RTI_BOOL
CDR_Stream_serialize_StringArray(struct CDR_Stream_t *cdrs,
                               const void* in, 
                               RTI_UINT32 length,
                               RTI_UINT32 maximumStringLength,
                               CdrPrimitiveType type);

CDRDllExport  RTI_BOOL
CDR_Stream_deserialize_StringArray(struct CDR_Stream_t *cdrs,
                                 void *out,
                                 RTI_UINT32 length,
                                 RTI_UINT32 maximumStringLength,
                                 CdrPrimitiveType type);

CDRDllExport  RTI_UINT32
CDR_get_maxSizeSerialized_StringArray(RTI_UINT32 currentAlignment,
                                    RTI_UINT32 length,
                                    RTI_UINT32 maximumStringLength,
                                    CdrPrimitiveType type);

CDRDllExport  RTI_BOOL
CDR_Stream_serialize_PrimitiveArray(struct CDR_Stream_t *cdrs,
                                  const void* in,
                                  RTI_UINT32 length,
                                  CdrPrimitiveType type);

CDRDllExport  RTI_BOOL
CDR_Stream_deserialize_PrimitiveArray(struct CDR_Stream_t *cdrs,
                                    void* out,
                                    RTI_UINT32 length,
                                    CdrPrimitiveType type);

#define CDR_get_maxSizeSerialized_PrimitiveArray(currentAlignment, length, type) \
  ((length != 0) ? \
   ((CDR_get_padSize((currentAlignment), CDR_Primitive_get_size((type))) + \
     (CDR_Primitive_get_size((type)) * (length)))) : \
   0)

CDRDllExport  RTI_BOOL
CDR_Stream_serialize_NonPrimitiveArray(struct CDR_Stream_t *stream,
                                       const void* in,
                                       RTI_UINT32 length,
                                       RTI_UINT32 elementSize,
                                       CDR_Stream_SerializeFunction
                                         serializeFunction,
                                       void * param);

CDRDllExport  RTI_BOOL
CDR_Stream_deserialize_NonPrimitiveArray(struct CDR_Stream_t *stream,
                                         void* out,
                                         RTI_UINT32 length,
                                         RTI_UINT32 elementSize,
                                         CDR_Stream_DeserializeFunction
                                           deserializeFunction,
                                         void * param);

CDRDllExport  RTI_UINT32
CDR_get_maxSizeSerialized_NonPrimitiveArray(RTI_UINT32 currentAlignment,
                                            RTI_UINT32 length,
                                            CDR_Stream_GetSerializedSizeFunction
                                              getSerializedSizeFunc,
                                            void * param);

CDRDllExport  RTI_BOOL
CDR_Stream_serialize_StringSequence(struct CDR_Stream_t *cdrs,
                                    const struct REDA_Sequence* in,
                                    RTI_UINT32 maximumStringLength,
                                    CdrPrimitiveType type);

CDRDllExport  RTI_BOOL
CDR_Stream_deserialize_StringSequence(struct CDR_Stream_t *cdrs,
                                      struct REDA_Sequence* out,
                                      RTI_UINT32 maximumStringLength,
                                      CdrPrimitiveType type);

CDRDllExport  RTI_UINT32
CDR_get_maxSizeSerialized_StringSequence(RTI_UINT32 currentAlignment,
                                         RTI_UINT32 length,
                                         RTI_UINT32 maximumStringLength,
                                         CdrPrimitiveType type);

CDRDllExport  RTI_BOOL
CDR_Stream_serialize_PrimitiveSequence(struct CDR_Stream_t *stream,
                                       const struct REDA_Sequence* in,
                                       CdrPrimitiveType type);

CDRDllExport  RTI_BOOL
CDR_Stream_deserialize_PrimitiveSequence(struct CDR_Stream_t *stream,
                                         struct REDA_Sequence* out,
                                         CdrPrimitiveType type);

#define CDR_get_maxSizeSerialized_PrimitiveSequence(currentAlignment, length, type) \
  (CDR_get_maxSizeSerialized_UnsignedLong((currentAlignment)) + \
       CDR_get_maxSizeSerialized_PrimitiveArray((currentAlignment) + \
           CDR_get_maxSizeSerialized_UnsignedLong((currentAlignment)), (length), (type)))

CDRDllExport  RTI_BOOL
CDR_Stream_serialize_NonPrimitiveSequence(struct CDR_Stream_t *stream,
                                          const struct REDA_Sequence* in,
                                          CDR_Stream_SerializeFunction
                                            serializeFunction,
                                          void * param);

CDRDllExport  RTI_BOOL
CDR_Stream_deserialize_NonPrimitiveSequence(struct CDR_Stream_t *stream,
                                            struct REDA_Sequence* out,
                                            CDR_Stream_DeserializeFunction
                                              deserializeFunction,
                                            void * param);

CDRDllExport  RTI_UINT32
CDR_get_maxSizeSerialized_NonPrimitiveSequence(RTI_UINT32 currentAlignment,
                                               RTI_UINT32 length,
                                               CDR_Stream_GetSerializedSizeFunction
                                                 getSerializedSizeFunc,
                                               void * param);
#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* cdr_serialize_h */
