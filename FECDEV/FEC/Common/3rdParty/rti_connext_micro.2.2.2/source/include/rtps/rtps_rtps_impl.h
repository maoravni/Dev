/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rtps_rtps_impl_h
#define rtps_rtps_impl_h



#ifndef reda_sequenceNumber_h
#include "reda/reda_sequenceNumber.h"
#endif

/* --------------------------------------------------------- */
#define MIGRtpsObjectSuffix_getRWType(osx) \
  ((((osx) & 0x3F) == 0x02 || ((osx) & 0x3F) == 0x03) ? \
   MIG_RTPS_RW_TYPE_WRITER :                            \
  ((((osx) & 0x3F) == 0x04 || ((osx) & 0x3F) == 0x07 || \
    ((osx) & 0x3F) == 0x3C || ((osx) & 0x3F) == 0x3D) ? \
   MIG_RTPS_RW_TYPE_READER : MIG_RTPS_RW_TYPE_UNKNOWN))

/* --------------------------------------------------------- */
#define MIGRtpsObjectSuffix_getSampleType(osx) \
  ((((osx) & 0x3F) == 0x02 || ((osx) & 0x3F) == 0x07 || \
    ((osx) & 0x3F) == 0x3D) ?                           \
   MIG_RTPS_SAMPLE_TYPE_CST :                           \
  ((((osx) & 0x3F) == 0x03 || ((osx) & 0x3F) == 0x04 || \
    ((osx) & 0x3F) == 0x3C) ? \
   MIG_RTPS_SAMPLE_TYPE_PS : MIG_RTPS_SAMPLE_TYPE_UNKNOWN))

/* --------------------------------------------------------- */

/* --------------------------------------------------------- */
#define MIGRtpsGuid_copy(to, from) \
  (to)->prefix.hostId   = (from)->prefix.hostId; \
  (to)->prefix.appId    = (from)->prefix.appId;  \
  (to)->prefix.instanceId    = (from)->prefix.instanceId;  \
  (to)->objectId = (from)->objectId

/* --------------------------------------------------------- */
#define MIGRtpsGuid_equals(a, b)   \
  ((a)->prefix.hostId     == (b)->prefix.hostId && \
   (a)->prefix.appId      == (b)->prefix.appId &&  \
   (a)->prefix.instanceId == (b)->prefix.instanceId &&  \
   (a)->objectId   == (b)->objectId)

#define MIGRtpsGuid_prefix_equals(a, b)   \
((a)->prefix.hostId     == (b)->prefix.hostId && \
(a)->prefix.appId      == (b)->prefix.appId &&  \
(a)->prefix.instanceId == (b)->prefix.instanceId)

#define MIGRtpsGuid_suffix_equals(a, b) \
((a)->objectId == (b)->objectId)

/* --------------------------------------------------------- */
#define MIGRtpsObjectId_getObjectSuffix(oid) \
  ((MIGRtpsObjectSuffix)((oid) & 0x000000ff))
/* --------------------------------------------------------- */
#define MIGRtpsObjectId_generate(instanceId, osx) \
  ((MIGRtpsObjectId)(((instanceId) << 8) | (osx)))
/* --------------------------------------------------------- */
#define MIGRtpsGuid_getObjectSuffix(guid) \
  ((MIGRtpsObjectSuffix)((guid)->objectId & 0x000000ff))
/* --------------------------------------------------------- */
#define MIGRtpsProtocolVersion_getMajor(version) (*(version) >> 8)
/* --------------------------------------------------------- */
#define MIGRtpsProtocolVersion_getMinor(version) (*(version) & 0x00ff)
/* --------------------------------------------------------- */
#define MIGRtpsVendorId_getMajor(vendor)         (*(vendor) >> 8)
/* --------------------------------------------------------- */
#define MIGRtpsVendorId_getMinor(vendor)         (*(vendor) & 0x00ff)

/* --------------------------------------------------------- */
#define MIGRtpsBitmap_getLead(a) (&(a)->_lead)
/* --------------------------------------------------------- */
#define MIGRtpsBitmap_getBitCount(a) (a)->_bitCount
/* --------------------------------------------------------- */
#define MIGRtpsBitmap_resetMacro(a, seqNum, bitCount)                     \
{                                                                    \
  register int MIGRtpsBitmap_g_resetI = 0;                           \
  (a)->_lead = *(seqNum);                                            \
  (a)->_bitCount = (bitCount);                                       \
  for (MIGRtpsBitmap_g_resetI=0;                                     \
       MIGRtpsBitmap_g_resetI<MIG_RTPS_BITMAP_32BITS_ARRAY_SIZE_MAX; \
       ++MIGRtpsBitmap_g_resetI) {                                   \
      (a)->_32bits[MIGRtpsBitmap_g_resetI] = 0;                      \
  }                                                                  \
}
#ifndef RTI_PRECONDITION_TEST
#define MIGRtpsBitmap_reset MIGRtpsBitmap_resetMacro
#endif

/* --------------------------------------------------------- */
#define MIGRtpsBitmap_getSerializedSize(bitmap) \
    12 + sizeof(RTI_INT32)*(((bitmap)->_bitCount + 31)/32)

#define MIGRtpsSubmessageId_toString(id) MIG_RTPS_SUBMESSAGE_ID_NAME[id]

/* --------------------------------------------------------- */

#define MIGRtpsVarSubmessage_hasProtocolParameters(flags) \
   ((((((flags) & MIG_RTPS_VAR_A_FLAG) != 0) && \
      (((flags) & MIG_RTPS_VAR_P_FLAG) == 0)) || \
     ((((flags) & MIG_RTPS_VAR_A_FLAG) == 0) && \
      (((flags) & MIG_RTPS_VAR_P_FLAG) != 0)))? RTI_TRUE : RTI_FALSE)

/* --------------------------------------------------------- */


/* RTPS spec - PB + DG*domainId + dX */
#define MIGRtps_getWellKnownMulticastPort(domain_id, port_base, domain_id_gain, port_offset) \
    (port_base + (domain_id_gain*domain_id) + port_offset)

/* RTPS spec - PB + DG*domainId + PG*participantId + dX */
#define MIGRtps_getWellKnownUnicastPort(domain_id, participant_id, port_base, domain_id_gain, participant_id_gain, port_offset) \
    (port_base + (domain_id_gain*domain_id) + (participant_id_gain*participant_id) + port_offset)


/* --------------------------------------------------------- */
#define MIGRtpsEpoch_deserialize(me, streamPtr, needByteSwap) \
    (                                                                 \
      needByteSwap ? (                                                \
        *(((char *)(me)) + 3) = *((*(streamPtr))++),                  \
        *(((char *)(me)) + 2) = *((*(streamPtr))++),                  \
        *(((char *)(me)) + 1) = *((*(streamPtr))++),                  \
        *(((char *)(me))    ) = *((*(streamPtr))++),                  \
        RTI_TRUE                                                      \
      ) : (                                                           \
        *(me) = *((REDA_Epoch_T*)(*(streamPtr))), *(streamPtr) += 4,     \
        RTI_TRUE                                                      \
      )                                                               \
    )



#endif /* rtps_rtps_impl_h */
