/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#include "osapi/osapi_time.h"
#include "osapi/osapi_types.h"
#include "osapi/osapi_string.h"
#include "cdr/cdr_serialize.h"
#include "cdr/cdr_stream.h"
#include "rtps/rtps_rtps.h"
#include "rtps/rtps_log.h"

/* Discovery constants.  TODO:  Move these someplace better */
const char *RTPS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME =
    "DDS_ParticipantBuiltinTopicData";
const char *RTPS_PARTICIPANT_BUILTIN_TOPIC_NAME = "DCPSParticipant";
const char *RTPS_PUBLICATION_BUILTIN_TOPIC_TYPE_NAME =
    "DDS_PublicationBuiltinTopicData";
const char *RTPS_PUBLICATION_BUILTIN_TOPIC_NAME = "DCPSPublication";
const char *RTPS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME =
    "DDS_SubscriptionBuiltinTopicData";
const char *RTPS_SUBSCRIPTION_BUILTIN_TOPIC_NAME = "DCPSSubscription";

/*** SOURCE_BEGIN ***/

RTI_BOOL
MIGRtpsGuidField_serialize(struct CDR_Stream_t *stream,
                           const RTI_UINT32 * field,
                           void * param)
{
    /* HostId, AppId, instanceId, or objectId */
    if (!CDR_Stream_serialize_UnsignedLongToBigEndian(stream, field))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
MIGRtpsGuidField_deserialize(struct CDR_Stream_t * stream,
                             const RTI_UINT32 * field,
                             void * param)
{
    /* HostId, AppId, instanceId, or objectId */
    if (!CDR_Stream_deserialize_UnsignedLongFromBigEndian(stream, field))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

/* TODO: Naming: why is this named as though guids are another object? */
RTI_BOOL
MIGRtpsGuid_serialize(struct CDR_Stream_t *stream,
                      const struct MIGRtpsGuid *guid,
                      void * param)
{
    /* HostId */
    if (!MIGRtpsGuidField_serialize(stream, &guid->prefix.hostId, NULL))
    {
        RTPS_Log_error(LOG_FAIL_SERIALIZE_HOST_ID);
        return RTI_FALSE;
    }

    /* AppId */
    if (!MIGRtpsGuidField_serialize(stream, &guid->prefix.appId, NULL))
    {
        RTPS_Log_error(LOG_FAIL_SERIALIZE_APP_ID);
        return RTI_FALSE;
    }

    /* InstanceId */
    if (!MIGRtpsGuidField_serialize(stream, &guid->prefix.instanceId, NULL))
    {
        RTPS_Log_error(LOG_FAIL_SERIALIZE_INSTANCE_ID);
        return RTI_FALSE;
    }

    /* ObjectId */
    if (!MIGRtpsGuidField_serialize(stream, &guid->objectId, NULL))
    {
        RTPS_Log_error(LOG_FAIL_SERIALIZE_OBJECT_ID);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
MIGRtpsGuid_deserialize(struct CDR_Stream_t * stream,
                        struct MIGRtpsGuid * guid,
                        void * param)
{
    /* HostId */
    if (!MIGRtpsGuidField_deserialize(stream, &guid->prefix.hostId, NULL))
    {
        return RTI_FALSE;
    }

    /* AppId */
    if (!MIGRtpsGuidField_deserialize(stream, &guid->prefix.appId, NULL))
    {
        return RTI_FALSE;
    }

    /* InstanceId */
    if (!MIGRtpsGuidField_deserialize(stream, &guid->prefix.instanceId, NULL))
    {
        return RTI_FALSE;
    }

    /* ObjectId */
    if (!MIGRtpsGuidField_deserialize(stream, &guid->objectId, NULL))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}


RTI_UINT32
MIGRtpsGuid_getMaxSizeSerialized(RTI_UINT32 size)
{
RTI_UINT32 origSize = size;

    /* Host Id */
    size += CDR_get4ByteMaxSizeSerialized(size);

    /* App Id */
    size += CDR_get4ByteMaxSizeSerialized(size);

    /* Instance Id */
    size += CDR_get4ByteMaxSizeSerialized(size);

    /* Object Id */
    size += CDR_get4ByteMaxSizeSerialized(size);

    return (size - origSize);
}

/* ----------------------------------------------------------------- */
RTI_BOOL
MIGRtps_serializeRtiNtpTime(struct CDR_Stream_t * stream,
                            const RTINtpTime * time,
                            void * param)
{
    if (!CDR_Stream_serialize_Long(stream, &time->sec))
    {
        return RTI_FALSE;
    }

    if (!CDR_Stream_serialize_UnsignedLong(stream, &time->frac))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}


RTI_BOOL
MIGRtps_deserializeRtiNtpTime(struct CDR_Stream_t * stream,
                              RTINtpTime * time,
                              void * param)
{
    if (!CDR_Stream_deserialize_Long(stream, &time->sec))
    {
        return RTI_FALSE;
    }

    if (!CDR_Stream_deserialize_UnsignedLong(stream, &time->frac))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_UINT32
MIGRtps_getRtiNtpTimeMaxSizeSerialized(RTI_UINT32 size)
{
RTI_UINT32 origSize = size;

    size += CDR_get_maxSizeSerialized_Long(size);
    size += CDR_get_maxSizeSerialized_UnsignedLong(size);

    return (size - origSize);
}

/* ----------------------------------------------------------------- */
RTI_BOOL
MIGRtps_serialize2Octets(struct CDR_Stream_t * stream,
                         const RTI_UINT16 * in,
                         void * param)
{
    if (!CDR_Stream_Align(stream, CDR_OCTET_ALIGN) ||
        !CDR_Stream_checkSize(stream, CDR_OCTET_SIZE * 2))
    {
        return RTI_FALSE;
    }

    CDR_Stream_serialize_UnsignedShortToBigEndian(stream, in);

    return RTI_TRUE;
}

RTI_BOOL
MIGRtps_deserialize2Octets(struct CDR_Stream_t * stream,
                           RTI_UINT16 * out,
                           void * param)
{
    if (!CDR_Stream_Align(stream, CDR_OCTET_ALIGN) ||
        !CDR_Stream_checkSize(stream, CDR_OCTET_SIZE * 2))
    {
        return RTI_FALSE;
    }

    CDR_Stream_deserialize_UnsignedShortFromBigEndian(stream, out);

    return RTI_TRUE;
}


/* TODO: Can we remove this? */
RTI_UINT32
MIGRtps_get2OctetsMaxSizeSerialized(RTI_UINT32 size)
{
RTI_UINT16 origSize = size;

    size += CDR_get1ByteMaxSizeSerialized(size);
    size += CDR_get1ByteMaxSizeSerialized(size);

    return (size - origSize);
}

/* ----------------------------------------------------------------- */
RTI_BOOL
MIGRtps_serializeIpv6Locator(struct CDR_Stream_t * stream,
                             const struct RTPS_Locator_t * loc,
                             void * param)
{
RTI_INT32 i;

    if (!CDR_Stream_serialize_Long(stream, &loc->kind))
    {
        return RTI_FALSE;
    }

    if (!CDR_Stream_serialize_Long(stream, &loc->port))
    {
        return RTI_FALSE;
    }

    for (i = 0; i < 16; ++i)
    {
        *(stream->buffptr++) = loc->address[i];
    }

    return RTI_TRUE;
}

RTI_BOOL
MIGRtps_deserializeIpv6Locator(struct CDR_Stream_t * stream,
                               struct RTPS_Locator_t * loc,
                               void * param)
{
RTI_INT32 i;
struct RTPS_LocatorUdpv4_t *udpv4_loc;

    if (!CDR_Stream_deserialize_Long(stream, &loc->kind))
    {
        return RTI_FALSE;
    }

    if (!CDR_Stream_deserialize_UnsignedLong(stream, &loc->port))
    {
        return RTI_FALSE;
    }

    if (loc->kind == RTPS_LOCATOR_KIND_UDPv6)
    {
        for (i = 0; i < 16; ++i)
        {
            loc->address[i] = *(stream->buffptr++);
        }
    }
    else if (loc->kind == RTPS_LOCATOR_KIND_UDPv4)
    {
        stream->buffptr = stream->buffptr + 12;
        OSAPI_Memory_zero(loc->address, 12);
        udpv4_loc = (struct RTPS_LocatorUdpv4_t *)loc;

        CDR_Stream_deserialize_4ByteFromBigEndianFromStream(stream,
                                                          &udpv4_loc->address);
    }

    return RTI_TRUE;
}

RTI_UINT32
MIGRtps_getIpv6LocatorMaxSizeSerialized(RTI_UINT32 size)
{
RTI_UINT32 origSize = size;

    /* NDDS_Transport_ClassId_t */
    size += CDR_get_maxSizeSerialized_Long(size);

    /* NDDS_Transport_Port_t */
    size += CDR_get_maxSizeSerialized_UnsignedLong(size);

    /* NDDS_Transport_Address_t */
    size += 16;

    return (size - origSize);
}
