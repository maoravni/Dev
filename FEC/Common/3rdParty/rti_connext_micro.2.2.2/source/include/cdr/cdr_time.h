/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef cdr_time_h
#define cdr_time_h


#ifndef cdr_serialize_h
#include "cdr/cdr_serialize.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif


#define OSAPI_NtpTime_serialize(ntpTime, destBuffer) \
  CDR_serialize_Long(destBuffer, &(ntpTime)->sec); \
  CDR_serialize_UnsignedLong(destBuffer, &(ntpTime)->frac);

#define OSAPI_NtpTime_deserialize(ntpTime, srcBuffer, byteSwap) \
  CDR_deserialize_Long(srcBuffer, &(ntpTime)->sec, byteSwap); \
  CDR_deserialize_UnsignedLong(srcBuffer, &(ntpTime)->frac, byteSwap);

#define OSAPI_NtpTime_serializeToStream(ntpTime, cdrs) \
  (CDR_Stream_serialize_Long(cdrs, &(ntpTime)->sec) && \
   CDR_Stream_serialize_UnsignedLong(cdrs, &(ntpTime)->frac))

#define OSAPI_NtpTime_deserializeFromStream(ntpTime, cdrs) \
  (CDR_Stream_deserialize_Long(cdrs, &(ntpTime)->sec) && \
   CDR_Stream_deserialize_UnsignedLong(cdrs, &(ntpTime)->frac))

#endif /* cdr_time_h */
