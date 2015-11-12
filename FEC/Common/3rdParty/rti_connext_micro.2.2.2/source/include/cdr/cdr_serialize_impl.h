/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef cdr_serialize_impl_h
#define cdr_serialize_impl_h



#define CDR_alignUpwards(location, alignment) \
    ((((RTI_UINT32)(location)) + ((alignment) - 1)) & ~((alignment) - 1))

#define CDR_get_padSize(currentAlignment, align) \
  (((align) <= CDR_MAX_ALIGNMENT) ? \
    (CDR_alignUpwards((currentAlignment), (align)) - (currentAlignment)) : \
    (CDR_alignUpwards((currentAlignment), CDR_MAX_ALIGNMENT) - (currentAlignment)))

#define CDR_serialize_UnsignedShortMacro(/* RTI_INT8** */destBuffer, \
                                        /* unsigned short* */instance,\
                                        /* RTI_BOOL */ byteSwap)  \
  if (byteSwap) {                                    \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 1); \
    *((*destBuffer)++) = *((RTI_INT8*)instance    ); \
  } else {                                           \
    *(RTI_UINT16*)(*destBuffer) = *(RTI_UINT16*)instance; \
    (*destBuffer) += CDR_SHORT_SIZE; \
  }

#define CDR_deserialize_UnsignedShortMacro(/* RTI_INT8** */srcBuffer, \
                                          /* unsigned short* */instance,\
                                          /* RTI_BOOL */ byteSwap)  \
  if (byteSwap) {                                 \
    *((RTI_INT8*)instance + 1) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance    ) = *((*srcBuffer)++);  \
  } else {                                       \
    *instance = *(RTI_UINT16*)(*srcBuffer);      \
    (*srcBuffer) += CDR_SHORT_SIZE;         \
  }

#define CDR_serialize_UnsignedLongMacro(/* RTI_INT8** */destBuffer, \
                                       /* unsigned long* */instance,\
                                       /* RTI_BOOL */ byteSwap)  \
  if (byteSwap) {                                    \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 3); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 2); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 1); \
    *((*destBuffer)++) = *((RTI_INT8*)instance    ); \
  } else {                                           \
    *(RTI_UINT32*)(*destBuffer) = *(RTI_UINT32*)instance; \
    (*destBuffer) += CDR_LONG_SIZE; \
  }

#define CDR_deserialize_UnsignedLongMacro(/* RTI_INT8** */srcBuffer, \
                                         /* unsigned long* */instance,\
                                         /* RTI_BOOL */ byteSwap)  \
  if (byteSwap) {                                \
    *((RTI_INT8*)instance + 3) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 2) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 1) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance    ) = *((*srcBuffer)++);  \
  } else {                                       \
    *instance = *(RTI_UINT32*)(*srcBuffer);      \
    (*srcBuffer) += CDR_LONG_SIZE;          \
  }

#ifdef RTI_ENDIAN_LITTLE
#define CDR_serialize_UnsignedLongToBigEndianMacro(dst_,inst_) \
    *((*dst_)++) = *((RTI_INT8 *)(inst_) + 3);            \
    *((*dst_)++) = *((RTI_INT8 *)(inst_) + 2);            \
    *((*dst_)++) = *((RTI_INT8 *)(inst_) + 1);            \
    *((*dst_)++) = *((RTI_INT8 *)(inst_)    );          

#define CDR_deserialize_UnsignedLongFromBigEndianMacro(src_,inst_) \
  CDR_deserialize_UnsignedLongMacro(src_,inst_,RTI_TRUE)

#else 

#define CDR_serialize_UnsignedLongToBigEndianMacro(dst_,inst_) \
    CDR_serialize_UnsignedLongMacro(dst_,inst_,RTI_FALSE)

#define CDR_deserialize_UnsignedLongFromBigEndianMacro(src_,inst_) \
    CDR_deserialize_UnsignedLongMacro(src_,inst_,RTI_FALSE)

#endif

#define CDR_serialize_UnsignedLongLongMacro(/* RTI_INT8** */destBuffer, \
                                           /* unsigned long long* */instance,\
                                           /* RTI_BOOL */ byteSwap)  \
  if (byteSwap) {                                    \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 7); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 6); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 5); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 4); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 3); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 2); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 1); \
    *((*destBuffer)++) = *((RTI_INT8*)instance    ); \
  } else {                                           \
    *(RTI_UINT64*)(*destBuffer) = *(RTI_UINT64*)instance; \
    (*destBuffer) += CDR_LONG_LONG_SIZE; \
  }

#define CDR_deserialize_UnsignedLongLongMacro(/* RTI_INT8** */srcBuffer, \
                                             /* unsigned long long* */instance,\
                                             /* RTI_BOOL */ byteSwap)  \
  if (byteSwap) {                                \
    *((RTI_INT8*)instance + 7) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 6) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 5) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 4) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 3) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 2) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 1) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance    ) = *((*srcBuffer)++);  \
  } else {                                       \
    *instance = *(RTI_UINT64*)(*srcBuffer);      \
    (*srcBuffer) += CDR_LONG_LONG_SIZE;      \
  }

#ifdef RTI_HAVE_LONG_DOUBLE
#define CDR_serialize_LongDoubleMacro(/* RTI_INT8** */destBuffer, \
                                     /* long double* */instance, \
                                     /* RTI_BOOL */ byteSwap)  \
  if (byteSwap) {                                     \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 15); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 14); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 13); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 12); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 11); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 10); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 9);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 8);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 7);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 6);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 5);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 4);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 3);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 2);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 1);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance    );  \
  } else {                                            \
    *(RTI_DOUBLE128*)(*destBuffer) = *(RTI_DOUBLE128*)instance; \
    (*destBuffer) += CDR_LONG_DOUBLE_SIZE; \
  }

#define CDR_deserialize_LongDoubleMacro(/* RTI_INT8** */srcBuffer, \
                                       /* long double* */instance,\
                                       /* RTI_BOOL */ byteSwap)   \
  if (byteSwap) {                                    \
    *((RTI_INT8*)instance +15) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +14) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +13) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +12) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +11) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +10) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 9) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 8) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 7) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 6) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 5) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 4) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 3) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 2) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 1) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance    ) = *((*srcBuffer)++);  \
  } else {                                           \
    *instance = *(RTI_DOUBLE128*)(*srcBuffer);       \
    (*srcBuffer) += CDR_LONG_DOUBLE_SIZE;        \
  }
#else
#define CDR_serialize_LongDoubleMacro(/* RTI_INT8** */destBuffer, \
                                     /* long double* */instance, \
                                     /* RTI_BOOL */ byteSwap)  \
  if (byteSwap) {                                     \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 15); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 14); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 13); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 12); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 11); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 10); \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 9);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 8);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 7);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 6);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 5);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 4);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 3);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 2);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance + 1);  \
    *((*destBuffer)++) = *((RTI_INT8*)instance    );  \
  } else {                                            \
    *((RTI_UINT64*)(*destBuffer)   ) = *((RTI_UINT64*)instance   );\
    *((RTI_UINT64*)(*destBuffer) +1) = *((RTI_UINT64*)instance +1);\
    (*destBuffer) += CDR_LONG_DOUBLE_SIZE; \
  }

#define CDR_deserialize_LongDoubleMacro(/* RTI_INT8** */srcBuffer, \
                                       /* long double* */instance,\
                                       /* RTI_BOOL */ byteSwap)   \
  if (byteSwap) {                                    \
    *((RTI_INT8*)instance +15) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +14) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +13) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +12) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +11) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance +10) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 9) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 8) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 7) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 6) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 5) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 4) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 3) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 2) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance + 1) = *((*srcBuffer)++);  \
    *((RTI_INT8*)instance    ) = *((*srcBuffer)++);  \
  } else {                                           \
    *((RTI_UINT64*)instance   ) = *((RTI_UINT64*)(*srcBuffer)   );\
    *((RTI_UINT64*)instance +1) = *((RTI_UINT64*)(*srcBuffer) +1);\
    (*srcBuffer) += CDR_LONG_DOUBLE_SIZE;        \
  }
#endif

#ifdef RTI_SERIALIZE_MACRO
/*
 CDR serialization / deserialization implemented by macro expansion
 ( the other primative types are implemented using these macros,
   by macro definitions in serialize.ifc )
*/
#define CDR_serialize_UnsignedShort CDR_serialize_UnsignedShortMacro
#define CDR_deserialize_UnsignedShort CDR_deserialize_UnsignedShortMacro

#define CDR_serialize_UnsignedLong  CDR_serialize_UnsignedLongMacro
#define CDR_deserialize_UnsignedLong  CDR_deserialize_UnsignedLongMacro

#define CDR_serialize_UnsignedLongLong  CDR_serialize_UnsignedLongLongMacro
#define CDR_deserialize_UnsignedLongLong  CDR_deserialize_UnsignedLongLongMacro

#define CDR_serialize_LongDouble  CDR_serialize_LongDoubleMacro
#define CDR_deserialize_LongDouble  CDR_deserialize_LongDoubleMacro

#endif

#define CDR_Stream_serialize_UnsignedShortToBigEndian(me, in) (\
(((me)->endian == RTI_CDR_ENDIAN_BIG && (me)->needbyteswap == RTI_TRUE) ||  \
 ((me)->endian == RTI_CDR_ENDIAN_LITTLE && (me)->needbyteswap == RTI_FALSE)) ? \
    ((void)(*((me)->buffptr++) = *((RTI_INT8 *)(in) + 1),            \
            *((me)->buffptr++) = *((RTI_INT8 *)(in)    ))) :         \
    ((void)(*((me)->buffptr++) = *((RTI_INT8 *)(in)    ),            \
            *((me)->buffptr++) = *((RTI_INT8 *)(in) + 1)))           \
  )

#define CDR_Stream_deserialize_UnsignedShortFromBigEndian(me, out) (\
(((me)->endian == RTI_CDR_ENDIAN_BIG && (me)->needbyteswap == RTI_TRUE) ||  \
 ((me)->endian == RTI_CDR_ENDIAN_LITTLE && (me)->needbyteswap == RTI_FALSE)) ? \
    ((void)( *((RTI_INT8 *)(out) + 1) = *((me)->buffptr++),        \
        *((RTI_INT8 *)(out)    ) = *((me)->buffptr++)))   :        \
    ((void)(*((RTI_INT8 *)(out)    ) = *((me)->buffptr++),         \
       *((RTI_INT8 *)(out) + 1) = *((me)->buffptr++)))              \
  )

#define CDR_Stream_checkSize(me, size) \
  ((CDR_Stream_GetCurrentPositionOffset((me)) >        \
    (RTI_UINT32)((me)->length - (size))) ? RTI_FALSE : RTI_TRUE)

#define CDR_Stream_deserialize_4ByteFromBigEndianFromStream(me, out) ( \
(((me)->endian == RTI_CDR_ENDIAN_BIG && (me)->needbyteswap == RTI_TRUE) || \
 ((me)->endian == RTI_CDR_ENDIAN_LITTLE && (me)->needbyteswap == RTI_FALSE)) ? \
    ((void)( *((RTI_INT8 *)(out) + 3) = *((me)->buffptr++),        \
             *((RTI_INT8 *)(out) + 2) = *((me)->buffptr++),        \
             *((RTI_INT8 *)(out) + 1) = *((me)->buffptr++),        \
             *((RTI_INT8 *)(out)    ) = *((me)->buffptr++)))   :   \
    ((void)( *((RTI_INT8 *)(out)    ) = *((me)->buffptr++),        \
             *((RTI_INT8 *)(out) + 1) = *((me)->buffptr++),        \
             *((RTI_INT8 *)(out) + 2) = *((me)->buffptr++),        \
             *((RTI_INT8 *)(out) + 3) = *((me)->buffptr++)))       \
  )

#define CDR_Stream_serialize_4ByteToBigEndianFromStream(me, in) ( \
(((me)->endian == RTI_CDR_ENDIAN_BIG && (me)->needbyteswap == RTI_TRUE) ||  \
 ((me)->endian == RTI_CDR_ENDIAN_LITTLE && (me)->needbyteswap == RTI_FALSE)) ? \
    ((void)(*((me)->buffptr++) = *((RTI_INT8 *)(in) + 3),            \
            *((me)->buffptr++) = *((RTI_INT8 *)(in) + 2),            \
            *((me)->buffptr++) = *((RTI_INT8 *)(in) + 1),            \
            *((me)->buffptr++) = *((RTI_INT8 *)(in)    ))) :         \
    ((void)(*((me)->buffptr++) = *((RTI_INT8 *)(in)    ),            \
            *((me)->buffptr++) = *((RTI_INT8 *)(in) + 1),            \
            *((me)->buffptr++) = *((RTI_INT8 *)(in) + 2),            \
            *((me)->buffptr++) = *((RTI_INT8 *)(in) + 3)))           \
  )

#define CDR_Stream_serialize_UnsignedLongToBigEndian(me, in) \
  (CDR_Stream_checkSize((me), 4) ?                                \
   (CDR_Stream_serialize_4ByteToBigEndianFromStream(me, in), \
   RTI_TRUE) : RTI_FALSE)

#define CDR_Stream_deserialize_UnsignedLongFromBigEndian(me, in) \
  ( CDR_Stream_checkSize((me), 4) ?                                \
   (CDR_Stream_deserialize_4ByteFromBigEndianFromStream(me, in), \
   RTI_TRUE) : RTI_FALSE)


#define CDR_getPadSize(currentSize, align) \
  (((align) <= CDR_MAX_ALIGNMENT) ? \
    (CDR_alignUpwards((currentSize), (align)) - (currentSize)) : \
    (CDR_alignUpwards((currentSize), CDR_MAX_ALIGNMENT)    \
     - (currentSize)))

#define CDR_get2ByteMaxSizeSerialized(currentSize) \
  (CDR_getPadSize((currentSize), CDR_SHORT_ALIGN) + \
   CDR_SHORT_SIZE)

#define CDR_get4ByteMaxSizeSerialized(currentSize) \
  (CDR_getPadSize((currentSize), CDR_LONG_ALIGN) + \
   CDR_LONG_SIZE)

#define CDR_get8ByteMaxSizeSerialized(currentSize) \
  (CDR_getPadSize((currentSize), CDR_LONG_LONG_ALIGN) + \
   CDR_LONG_LONG_SIZE)

#define CDR_get16ByteMaxSizeSerialized(currentSize) \
  (CDR_getPadSize((currentSize), CDR_LONG_DOUBLE_ALIGN) + \
   CDR_LONG_DOUBLE_SIZE)

#define CDR_get1ByteMaxSizeSerialized(currentSize) (1)

#define CDR_Stream_serialize_1Byte(me, in)      \
    *((me)->buffptr++) = *((RTI_INT8 *)(in))

#define CDR_Stream_deserialize_1Byte(me, out)   \
    *((RTI_INT8 *)(out)) = *((me)->buffptr++)

#define CDR_get_maxSizeSerialized_String(currentSize, length) \
  (CDR_get4ByteMaxSizeSerialized((currentSize)) + \
   (CDR_CHAR_SIZE * (length)))

#define CDR_get_maxSizeSerialized_Wstring(currentSize, length) \
  (CDR_get4ByteMaxSizeSerialized((currentSize)) + \
   (CDR_WCHAR_SIZE * (length)))


#endif /* cdr_serialize_impl_h */
