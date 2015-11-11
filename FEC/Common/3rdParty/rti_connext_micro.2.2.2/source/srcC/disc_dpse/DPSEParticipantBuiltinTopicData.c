/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef cdr_encapsulation_h
#include "cdr/cdr_encapsulation.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef disc_dpse_dpsediscovery_h
#include "disc_dpse/disc_dpse_dpsediscovery.h"
#endif
#ifndef disc_dpse_log_h
#include "disc_dpse/disc_dpse_log.h"
#endif

#include "DPSECdr.pkg.h"

#define CDR_PARAMETER_ALIGNMENT 4

/*** SOURCE_BEGIN ***/

RTI_PRIVATE DDS_Boolean
DPSE_ParticipantBuiltinTopicDataTypePlugin_insertParameterLength(
                                            struct CDR_Stream_t *stream,
                                            DDS_Long parameterBeginPosition,
                                            DDS_UnsignedShort parameterLength,
                                            RTI_BOOL parameterSuccess)
{
    DDS_Long currentPosition;
    RTI_BOOL ok = RTI_TRUE;

    LOG_testPrecondition(stream == NULL ||
                         (stream->buffer + parameterBeginPosition) 
                          < stream->alignBase,
                         return RTI_FALSE);

    if (parameterSuccess)
    {
        currentPosition = CDR_Stream_GetCurrentPositionOffset(stream);

        ok = CDR_Stream_SetCurrentPositionOffset(stream,
                parameterBeginPosition + CDR_SHORT_SIZE)
               && CDR_Stream_serialize_UnsignedShort(stream, &parameterLength);

        if (ok)
        {
            CDR_Stream_SetCurrentPositionOffset(stream, currentPosition);
            return RTI_TRUE;
        }
    }

    /* undo this parameter */
    CDR_Stream_SetCurrentPositionOffset(stream, parameterBeginPosition);

    return RTI_FALSE;
}

RTI_PRIVATE RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeFourByteParameter(
                                                struct CDR_Stream_t* stream,
                                                const void *in,
                                                DDS_UnsignedShort parameterId)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength;
    DDS_Long parameterBeginPosition, valueBeginPosition, valueEndPosition;
    DDS_Long parameterSize;

    LOG_testPrecondition(stream == NULL || in == NULL, return RTI_FALSE);

    parameterBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter Id */
    ok = ok && CDR_Stream_serialize_UnsignedShort(stream, &parameterId);

    /* skip parameter length */
    ok = ok && CDR_Stream_IncrementCurrentPosition(stream, CDR_SHORT_SIZE);

    /* parameter value parameter has to be 4-byte aligned */
    valueBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);
    parameterSize = 4U;

#ifdef TODO
    ok = ok && CDR_Stream_CheckSize(stream, parameterSize);
#endif
    if (ok)
    {
        CDR_Stream_serialize_Long(stream, (long *)in);
    }

    CDR_Stream_Align(stream, CDR_PARAMETER_ALIGNMENT);
    valueEndPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter length */
    parameterLength = valueEndPosition - valueBeginPosition;

    return DPSE_ParticipantBuiltinTopicDataTypePlugin_insertParameterLength(
                stream,parameterBeginPosition,parameterLength,ok);
}

RTI_PRIVATE RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
                                struct CDR_Stream_t *stream,
                                const void *in,
                                CDR_Stream_SerializeFunction serializeFunction,
                                DDS_UnsignedShort parameterId)
{
    RTI_BOOL ok = RTI_TRUE;
    struct REDA_Buffer state;
    DDS_UnsignedShort parameterLength;
    DDS_Long parameterBeginPosition, valueBeginPosition, valueEndPosition;

    LOG_testPrecondition(stream == NULL || in == NULL ||
                         serializeFunction == NULL, return RTI_FALSE);

    parameterBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter Id */
    ok = ok && CDR_Stream_serialize_UnsignedShort(stream, &parameterId);

    /* skip parameter length */
    ok = ok
        && CDR_Stream_IncrementCurrentPosition(stream, CDR_SHORT_SIZE);

    /* parameter value parameter has to be 4-byte aligned */
    valueBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);
    ok = ok && serializeFunction(stream, in, NULL);
    CDR_Stream_Align(stream, CDR_PARAMETER_ALIGNMENT);
    valueEndPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter length */
    parameterLength = valueEndPosition - valueBeginPosition;

    return DPSE_ParticipantBuiltinTopicDataTypePlugin_insertParameterLength(
                stream,parameterBeginPosition,parameterLength,ok);
}

RTI_PRIVATE DDS_UnsignedLong
DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(
                                                    DDS_UnsignedLong size)
{
    DDS_UnsignedLong origSize = size;

    /* align to 4 bytes boundary */
    size = CDR_alignUpwards(size, 4);  /* TODO: Put this default alignment in a #define somewhere */

    /* paramterId */
    size += CDR_get_maxSizeSerialized_UnsignedShort(size);

    /* paramterLength */
    size += CDR_get_maxSizeSerialized_UnsignedShort(size);

    return (size - origSize);
}

RTI_PRIVATE DDS_UnsignedLong
DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameters_max_size_serialized(
                                        DDS_UnsignedLong size,
                                        DDS_Long userDataMaximumLength,
                                        DDS_Long propertyListMaximumLength,
                                        DDS_Long propertyStringMaximumLength)
{
    DDS_UnsignedLong origSize = size;
    DDS_Long i = 0;

    /*
     * Call get max size serialized on each parameter header and field
     * according to the type.
     * We assume there is a logical reset before serializing each value.
     * getParameterHeaderMaxSizeSerialize() take care of 4-byte alignment.
     * Sentinel parameter size is not included in the calculation.
     */
    CDR_Stream_getEncapsulationSize(size);

    /* Guid */
    size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
    size += MIGRtpsGuid_getMaxSizeSerialized(0);

    /* Builtin endpoint mask */
    size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
    size += CDR_get_maxSizeSerialized_UnsignedLong(0);

    size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
    size += MIGRtps_get2OctetsMaxSizeSerialized(0);

    size += MIGRtps_get2OctetsMaxSizeSerialized(0);

    for (i = 0; i < MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX; ++i)
    {
        size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
        size += MIGRtps_getIpv6LocatorMaxSizeSerialized(0);
    }

    for (i = 0; i < MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX; ++i)
    {
        size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
        size += MIGRtps_getIpv6LocatorMaxSizeSerialized(0);
    }

    for (i = 0; i < MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX; ++i)
    {
        size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
        size += MIGRtps_getIpv6LocatorMaxSizeSerialized(0);
    }

    /* Lease duration */
    size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
    size += MIGRtps_getRtiNtpTimeMaxSizeSerialized(0);

    size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
    size += DPSE_Builtin_getProductVersionMaxSizeSerialized(size);

    size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
    size += CDR_get4ByteMaxSizeSerialized(0);

    size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);
    size += DPSE_Builtin_getEntityNameQosPolicyMaxSizeSerialized(size);

    /* since parameter can be serialized in any order, we need to make sure
     * that the next parameter is taken into account */
    size = CDR_alignUpwards(size, 4);  /* TODO put this default alignment in a #define somewhere */

    /* sentinel parameter header */
    size += DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameter_header_max_size_serialized(size);

    return (size - origSize);
}

RTI_PRIVATE DDS_UnsignedLong
DPSE_ParticipantBuiltinTopicDataTypePlugin_get_header_max_size_serialized(
                                                        DDS_UnsignedLong size)
{
    DDS_Long origSize = size;

    /* align to 4 bytes boundary */
    size = CDR_alignUpwards(size, 4);  /* TODO: Put this default alignment in a #define somewhere */

    CDR_Stream_getEncapsulationSize(size);

    return size - origSize;

}

RTI_PRIVATE DDS_UnsignedLong
DPSE_ParticipantBuiltinTopicDataTypePlugin_get_serialized_sample_max_size(
                                            struct NDDS_Type_Plugin *plugin,
                                            DDS_UnsignedLong current_alignment,
                                            void *param)
{
    return DPSE_ParticipantBuiltinTopicDataTypePlugin_get_parameters_max_size_serialized(
            0, 0, 0, 0) +
            DPSE_ParticipantBuiltinTopicDataTypePlugin_get_header_max_size_serialized(0);
}

void
DPSE_ParticipantBuiltinTopicDataTypePlugin_ipv6_from_ipv4(
                                        struct DDS_Locator_t *ipv6Locator,
                                        struct DDS_LocatorUdpv4_t *ipv4Locator)
{
    RTI_UINT32 tmp = ipv4Locator->address;

    OSAPI_Memory_zero(ipv6Locator->address, 12);

    /* This does not need to be byte swapped because we always store the locator
     * in network order.
     */
    ipv6Locator->address[12] = (RTI_UINT8)(tmp >> 24);
    ipv6Locator->address[13] = (RTI_UINT8)((tmp & 0x00FF0000) >> 16);
    ipv6Locator->address[14] = (RTI_UINT8)((tmp & 0x0000FF00) >> 8);
    ipv6Locator->address[15] = (RTI_UINT8)((tmp & 0x000000FF));
}

RTI_PRIVATE RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_serialize(
                                                struct CDR_Stream_t *stream,
                                                const void *data,
                                                void *param)
{
    DDS_Boolean ok = RTI_FALSE;
    DDS_UnsignedShort zeroLength = 0;
    DDS_Long i = 0;
    struct MIGRtpsGuid guid;
    MIGRtpsProtocolVersion protocolVersion;
    MIGRtpsVendorId vendorId;
    struct OSAPI_NtpTime serializeTime;
    RTI_UINT16 sentinel;
    struct DDS_Locator_t generic_loc;
    struct DDS_ParticipantBuiltinTopicData *topic_data =
                            (struct DDS_ParticipantBuiltinTopicData *)data;

    /* Copy key into MIG GUID */
    /* TODO: Can I do a straight copy of these rather than field-by-field? */
    guid.prefix.hostId = topic_data->key.value[0];
    guid.prefix.appId = topic_data->key.value[1];
    guid.prefix.instanceId = topic_data->key.value[2];
    guid.objectId = topic_data->key.value[3];

    /* Serialize the participant guid (the participant key) */
    if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
            stream, &guid,(CDR_Stream_SerializeFunction) MIGRtpsGuid_serialize,
            MIG_RTPS_PID_PARTICIPANT_GUID))        /* TODO:  Do we need this in MIG? */
    {
        /*LOG_printError("Serialization of GUID failed\n"); */
        DPSE_Log_error(LOG_FAIL_SERIALIZE_GUID);
        return ok;
    }

    if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeFourByteParameter
        (stream, &topic_data->dds_builtin_endpoints,
         MIG_RTPS_PID_BUILTIN_ENDPOINT_MASK))
    {
        /*LOG_printError("Serialization of builtin endpoints failed\n"); */
        DPSE_Log_error(LOG_FAIL_SERIALIZE_BUILTIN_ENDPOINTS);
        return ok;
    }

    /* --- Protocol version --- */
    protocolVersion = (topic_data->rtps_protocol_version.major << 8) |
        topic_data->rtps_protocol_version.minor;

    /* Serialize the participant parameter */
    if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
            stream,
            &protocolVersion,
            (CDR_Stream_SerializeFunction) MIGRtps_serialize2Octets,
            MIG_RTPS_PID_PROTOCOL_VERSION))
    {
        /*LOG_printError("Serialization of protocol version failed"); */
        DPSE_Log_error(LOG_FAIL_SERIALIZE_PROTOCOL_VERSION);
        return ok;
    }

    vendorId = (topic_data->rtps_vendor_id.vendorId[0] << 8) |
        topic_data->rtps_vendor_id.vendorId[1];

    if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
            stream,
            &vendorId,
            (CDR_Stream_SerializeFunction) MIGRtps_serialize2Octets,
            MIG_RTPS_PID_VENDOR_ID))
    {
        /*LOG_printError("Serialization of vendor id failed\n"); */
        DPSE_Log_error(LOG_FAIL_SERIALIZE_VENDOR_ID);
        return ok;
    }

    for (i = 0;
         i < DDS_LocatorSeq_get_length(&topic_data->default_unicast_locators);
         ++i)
    {

        generic_loc =
            *DDS_LocatorSeq_get_reference(&topic_data->default_unicast_locators,
                                          i);

        if (generic_loc.kind == RTPS_LOCATOR_KIND_UDPv4)
        {

            struct DDS_LocatorUdpv4_t *loc = (struct DDS_LocatorUdpv4_t *)&generic_loc;
            DPSE_ParticipantBuiltinTopicDataTypePlugin_ipv6_from_ipv4(&generic_loc, loc);
        }
        if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
                stream,&generic_loc,
                (CDR_Stream_SerializeFunction)MIGRtps_serializeIpv6Locator,
                MIG_RTPS_PID_DEFAULT_UNICAST_LOCATOR6))
        {
            /*LOG_printError("Serialize of default unicast loc failed\n"); */
            DPSE_Log_error(LOG_FAIL_SERIALIZE_DEFAULT_UNICAST_LOCATOR);
            return ok;
        }
    }

    for (i = 0;
         i < DDS_LocatorSeq_get_length(&topic_data->metatraffic_unicast_locators);
         ++i)
    {
        generic_loc = *DDS_LocatorSeq_get_reference(
                                &topic_data->metatraffic_unicast_locators, i);

        if (generic_loc.kind == RTPS_LOCATOR_KIND_UDPv4)
        {
            struct DDS_LocatorUdpv4_t *loc = (struct DDS_LocatorUdpv4_t *)&generic_loc;
            DPSE_ParticipantBuiltinTopicDataTypePlugin_ipv6_from_ipv4(&generic_loc, loc);
        }

        if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
                stream,&generic_loc,
                (CDR_Stream_SerializeFunction) MIGRtps_serializeIpv6Locator,
                MIG_RTPS_PID_METATRAFFIC_UNICAST_LOCATOR6))
        {
            DPSE_Log_error(LOG_FAIL_SERIALIZE_METATRAFFIC_UNICAST_LOCATOR);
            return ok;
        }
    }

    for (i = 0;
         i <
         DDS_LocatorSeq_get_length(&topic_data->metatraffic_multicast_locators);
         ++i)
    {
        generic_loc = *DDS_LocatorSeq_get_reference(
                               &topic_data->metatraffic_multicast_locators, i);

        if (generic_loc.kind == RTPS_LOCATOR_KIND_UDPv4)
        {

            struct DDS_LocatorUdpv4_t *loc = (struct DDS_LocatorUdpv4_t *)&generic_loc;
            DPSE_ParticipantBuiltinTopicDataTypePlugin_ipv6_from_ipv4(&generic_loc, loc);
        }

        if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
                stream,&generic_loc,
                (CDR_Stream_SerializeFunction) MIGRtps_serializeIpv6Locator,
                MIG_RTPS_PID_METATRAFFIC_MULTICAST_LOCATOR6))
        {
            DPSE_Log_error(LOG_FAIL_SERIALIZE_METATRAFFIC_MULTICAST_LOCATOR);
            return ok;
        }
    }

    /* Lease duration */
    OSAPI_NtpTime_packFromNanosec(&serializeTime,
                                 topic_data->liveliness_lease_duration.sec,
                                 topic_data->liveliness_lease_duration.nanosec);

    if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
            stream,
            &serializeTime,
            (CDR_Stream_SerializeFunction) MIGRtps_serializeRtiNtpTime,
            MIG_RTPS_PID_LEASE_DURATION))
    {
        /*LOG_printError("serialization of participant lease duration failed\n"); */
        DPSE_Log_error(LOG_FAIL_SERIALIZE_PARTICIPANT_LEASE_DURATION);
        return ok;
    }

    /* Product version */
    if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
            stream,
            &topic_data->product_version,
            (CDR_Stream_SerializeFunction)DPSE_Builtin_serializeProductVersion,
            DISC_RTPS_PID_PRODUCT_VERSION))
    {
        /*LOG_printError("serialization of product version failed\n"); */
        DPSE_Log_error(LOG_FAIL_SERIALIZE_PRODUCT_VERSION);
        return ok;
    }

    /* TODO: Do we need to add the promiscuity kind?  Should always be default */
#if 0
    /* Plugin promiscuity kind */
    if (topic_data->parameter->pluginPromiscuityKind !=
        PRES_DISCOVER_MATCHING_REMOTE_ENTITIES_PROMISCUITY)
    {
        /* Serialize only if it is a non-default value */
        if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeFourByteParameter(stream, &topic_data->parameter->pluginPromiscuityKind, MIG_RTPS_PID_PLUGIN_PROMISCUITY_KIND))
        {
            return RTI_FALSE;
        }
    }
#endif

    /* Participant name */
    if (topic_data->participant_name.name != NULL)
    {
        /* RTPS default for participant name is NULL.
         * Serialize only if it is a non-default i.e. non-NULL value */
        if (!DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeNonPrimitiveParameter(
                stream,
                &topic_data->participant_name,
                (CDR_Stream_SerializeFunction)DPSE_Builtin_serializeEntityNameQosPolicy,
                DISC_RTPS_PID_ENTITY_NAME))
        {
            /*LOG_printError("serialization of participant name failed\n"); */
            DPSE_Log_error(LOG_FAIL_SERIALIZE_PARTICIPANT_NAME);
            return ok;
        }
    }

    sentinel = MIG_RTPS_PID_SENTINEL;
    if (DDS_BOOLEAN_TRUE != CDR_Stream_serialize_UnsignedShort(stream, &sentinel))
    {
        return ok;
    }

    if (DDS_BOOLEAN_TRUE !=
        CDR_Stream_serialize_UnsignedShort(stream, &zeroLength))
    {
        return ok;
    }

    ok = DDS_BOOLEAN_TRUE;

    return ok;
}

/*i \ingroup DDS_ParticipantBuiltinTopicData_TypePluginModule
  Prototype for deserializing parameter value of the given parameterId.

  @param ok \b Out. Indicate if parameter value is correctly deserialized.
  @param parameter \b Out. Pointer to the parameter to which value should be
  deserialized.
  @param stream \b InOut. Stream for deserialization.
  @param parameterId \b In. Parameter ID of the value to be deserialized.
  @param parameterLength \b In. Length of parameter value.
  @param deserializeOption \b In. deserialize option obtained from
  reader property.

  @return Return RTI_TRUE if there is a match in parameterId, and ok is set to
  RTI_TRUE if deserialization of the parameter value is successful.
  Otherwise, return RTI_FALSE.
  Note that returning RTI_TRUE does not mean that the
  parameter value is correctly deserialized.
*/
typedef RTI_BOOL
(*DDS_ParticipantBuiltinTopicData_TypePluginDeserializeParameterValueFunction)(
        RTI_BOOL * ok, void *parameters, struct CDR_Stream_t * stream,
        DDS_UnsignedShort parameterId, DDS_UnsignedShort parameterLength);

/*i \ingroup DDS_ParticipantBuiltinTopicData_TypePluginModule
  Prototype for setting parameter to default value.

  @param parameter \b Out. Setup parameter with default values.
*/
typedef void
(*DDS_ParticipantBuiltinTopicData_TypePluginSetDefaultParameterValuesFunction)(void *parameter);

RTI_PRIVATE RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_deserializeParameterSequence(
        void *parameter,
        struct CDR_Stream_t *stream,
        DDS_ParticipantBuiltinTopicData_TypePluginSetDefaultParameterValuesFunction
        setDefaultParameterValuesFnc,
        DDS_ParticipantBuiltinTopicData_TypePluginDeserializeParameterValueFunction
        deserializeParameterValueFnc)
{
    DDS_UnsignedShort parameterId, parameterLength;
    RTI_BOOL ok = RTI_TRUE;
    struct REDA_Buffer state;
    DDS_Long origPosition;

    LOG_testPrecondition((parameter == NULL) || (stream == NULL) ||
                         (setDefaultParameterValuesFnc == NULL) ||
                         (deserializeParameterValueFnc == NULL),
                         return RTI_FALSE);

    /* set parameter to default value */
    setDefaultParameterValuesFnc(parameter);

    /* parameter id and length */
    ok = ok && CDR_Stream_deserialize_UnsignedShort(stream, &parameterId);
    ok = ok && CDR_Stream_deserialize_UnsignedShort(stream, &parameterLength);

    while (ok && parameterId != MIG_RTPS_PID_SENTINEL)
    {
        origPosition = CDR_Stream_GetCurrentPositionOffset(stream);

        /* logical reset of stream per parameter value */
        deserializeParameterValueFnc(&ok, parameter,
                                     stream, parameterId, parameterLength);

        /* skip to next parameter */
        ok = ok
            && CDR_Stream_SetCurrentPositionOffset(
                                        stream,origPosition + parameterLength);

        /* parameter id and length */
        ok = ok && CDR_Stream_deserialize_UnsignedShort(stream, &parameterId);
        ok = ok && CDR_Stream_deserialize_UnsignedShort(stream, &parameterLength);
    }

    return ok;
}

RTI_PRIVATE void
DPSE_BuiltinTopicParticipantDataPluginSupport_setDefaultParameterValues(
                            struct DDS_ParticipantBuiltinTopicData *topic_data)
{
}

/*
  Returns RTI_TRUE if there is a match in parameterId. Still need to set the
  ok to indicate if value is deserialized correctly.
  
*/
RTI_PRIVATE RTI_BOOL
DPSE_ParticipantBuiltinTopicData_deserializeParameterValue(RTI_BOOL *ok,
        struct DDS_ParticipantBuiltinTopicData *topic_data,
        struct CDR_Stream_t *stream,
        DDS_UnsignedShort parameterId,
        DDS_UnsignedShort parameterLength)
{
    struct DDS_Locator_t *locator;
    struct MIGRtpsGuid guid;

    *ok = RTI_TRUE;

    /* check each field to see if the paramterId matches */
    switch (parameterId)
    {
        case MIG_RTPS_PID_PARTICIPANT_GUID:
            if (!MIGRtpsGuid_deserialize(stream, &guid, NULL))
            {
                DPSE_Log_error(LOG_FAIL_DESERIALIZE_GUID);
                *ok = RTI_FALSE;
            }

            topic_data->key.value[0] = guid.prefix.hostId;
            topic_data->key.value[1] = guid.prefix.appId;
            topic_data->key.value[2] = guid.prefix.instanceId;
            topic_data->key.value[3] = guid.objectId;

            break;
        case MIG_RTPS_PID_BUILTIN_ENDPOINT_MASK:
            if (!CDR_Stream_deserialize_UnsignedLong
                (stream, &topic_data->dds_builtin_endpoints))
            {
                DPSE_LOG_DEBUG(LOG_FAIL_DESERIALIZE_BUILTIN_ENDPOINTS);
                *ok = RTI_FALSE;
            }
            break;
        case MIG_RTPS_PID_PROTOCOL_VERSION:
        {
            DDS_UnsignedShort protocolVersion;
            if (!MIGRtps_deserialize2Octets(stream, &protocolVersion, NULL))
            {
                DPSE_LOG_DEBUG(LOG_FAIL_DESERIALIZE_PROTOCOL_VERSION);
                *ok = RTI_FALSE;
            }
            topic_data->rtps_protocol_version.major = protocolVersion >> 8;
            topic_data->rtps_protocol_version.minor = protocolVersion & 0x00ff;
            break;
        }
        case MIG_RTPS_PID_VENDOR_ID:
        {
            MIGRtpsVendorId vendorId;

            if (!MIGRtps_deserialize2Octets(stream, &vendorId, NULL))
            {
                DPSE_LOG_DEBUG(LOG_FAIL_DESERIALIZE_VENDOR_ID);
                *ok = RTI_FALSE;
            }
            topic_data->rtps_vendor_id.vendorId[0] =
                MIGRtpsVendorId_getMajor(&vendorId);
            topic_data->rtps_vendor_id.vendorId[1] =
                MIGRtpsVendorId_getMinor(&vendorId);

            break;
        }
        case MIG_RTPS_PID_DEFAULT_UNICAST_LOCATOR6:
            if (DDS_LocatorSeq_get_length(&topic_data->default_unicast_locators)
                >= MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX)
            {
                *ok = RTI_FALSE;
                DPSE_Log_error_p1(LOG_EXCEED_MAX_LOCATORS,
                                         DDS_LocatorSeq_get_length
                                         (&topic_data->
                                          default_unicast_locators));
            }
            else
            {
                /* TODO: Check if the length is > the max */
                DDS_LocatorSeq_set_length(&topic_data->default_unicast_locators,
                                          DDS_LocatorSeq_get_length
                                          (&topic_data->
                                           default_unicast_locators) + 1);
                locator = DDS_LocatorSeq_get_reference(
                        &topic_data->default_unicast_locators,
                        DDS_LocatorSeq_get_length(
                                &topic_data->default_unicast_locators) - 1);
                *ok = *ok && MIGRtps_deserializeIpv6Locator(stream, locator, NULL);
            }
            break;
        case MIG_RTPS_PID_METATRAFFIC_UNICAST_LOCATOR6:
            if (DDS_LocatorSeq_get_length(
                    &topic_data->metatraffic_unicast_locators) >=
                    MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX)
            {
                *ok = RTI_FALSE;
            }
            else
            {
                DDS_LocatorSeq_set_length(
                        &topic_data->metatraffic_unicast_locators,
                        DDS_LocatorSeq_get_length(
                                &topic_data->metatraffic_unicast_locators) + 1);
                locator = DDS_LocatorSeq_get_reference(
                        &topic_data->metatraffic_unicast_locators,
                        DDS_LocatorSeq_get_length(
                              &topic_data->metatraffic_unicast_locators) - 1);
                *ok = *ok && MIGRtps_deserializeIpv6Locator(stream, locator, NULL);
            }
            break;
        case MIG_RTPS_PID_METATRAFFIC_MULTICAST_LOCATOR6:
            if (DDS_LocatorSeq_get_length(
                    &topic_data->metatraffic_multicast_locators) >=
                    MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX)
            {
                *ok = RTI_FALSE;
            }
            else
            {
                DDS_LocatorSeq_set_length(
                        &topic_data->metatraffic_multicast_locators,
                        DDS_LocatorSeq_get_length(
                            &topic_data->metatraffic_multicast_locators) + 1);
                locator = DDS_LocatorSeq_get_reference(
                        &topic_data->metatraffic_multicast_locators,
                        DDS_LocatorSeq_get_length(
                             &topic_data->metatraffic_multicast_locators) - 1);
                *ok = *ok && MIGRtps_deserializeIpv6Locator(stream, locator, NULL);
            }
            break;
        case MIG_RTPS_PID_LEASE_DURATION:
        {
            struct OSAPI_NtpTime time;

            if (!MIGRtps_deserializeRtiNtpTime(stream, &time, NULL))
            {
                *ok = RTI_FALSE;
            }

            OSAPI_NtpTime_unpackToNanosec(
                    &topic_data->liveliness_lease_duration.sec,
                    &topic_data->liveliness_lease_duration.nanosec, &time);
        }
            break;
        case DISC_RTPS_PID_PRODUCT_VERSION:
        {
            MIGRtpsVendorId vendorId;

            vendorId = (topic_data->rtps_vendor_id.vendorId[0] << 8) |
                        topic_data->rtps_vendor_id.vendorId[1];

            if (vendorId == MIG_RTPS_VENDOR_ID_RTI)
            {
                if (!DPSE_Builtin_deserializeProductVersion(
                                    stream,&topic_data->product_version, NULL))
                {
                    *ok = RTI_FALSE;
                }
            }
            break;
        }
            /* TODO:  Do we need to add the plugin promiscuity kind? */
#if 0
        case DISC_RTPS_PID_PLUGIN_PROMISCUITY_KIND:
            /* Plugin Promiscuity kind */
            if (topic_data->parameter->vendorId == MIG_RTPS_VENDOR_ID_RTI)
            {
                /* Check for product version here, if we make any changes to this 
                 * parameter in a new version
                 */
                if (!RTICDR_Stream_deserialize_Enum
                    (stream, &topic_data->parameter->pluginPromiscuityKind))
                {
                    *ok = RTI_FALSE;
                }
            }
            break;
#endif
        case DISC_RTPS_PID_ENTITY_NAME:
            if (!DPSE_Builtin_deserializeEntityNameQosPolicy(
                                stream, &topic_data->participant_name,NULL))
            {
                DPSE_Log_error(LOG_FAIL_DESERIALIZE_ENTITY_NAME);
                *ok = RTI_FALSE;
            }
            break;
        default:
            DPSE_LOG_WARNING_P1(LOG_UNRECOGNIZED_PARTICIPANT_ANNOUNCEMENT_PARAMETER_ID,
                 parameterId);
            return *ok;
    }

    return *ok;
}

RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_create_sample(
                                        struct NDDS_Type_Plugin* plugin,
                                        void **sample,
                                        void *param)
{
    struct DDS_ParticipantBuiltinTopicData *participant_builtin_data = NULL;

    OSAPI_Heap_allocateStructure(sample, struct DDS_ParticipantBuiltinTopicData);

    if (sample == NULL)
    {
        return DDS_BOOLEAN_FALSE;
    }
    participant_builtin_data = (*sample);

    DDS_ParticipantBuiltinTopicData_initialize(participant_builtin_data);

    DDS_LocatorSeq_set_maximum
        (&participant_builtin_data->default_unicast_locators,
         DDSC_PARTICIPANT_ADDRESS_COUNT_MAX);
    DDS_LocatorSeq_set_maximum
        (&participant_builtin_data->metatraffic_unicast_locators,
         DDSC_PARTICIPANT_ADDRESS_COUNT_MAX);
    DDS_LocatorSeq_set_maximum
        (&participant_builtin_data->metatraffic_multicast_locators,
         DDSC_PARTICIPANT_ADDRESS_COUNT_MAX);

    participant_builtin_data->dds_builtin_endpoints =
                                DDS_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER |
                                DDS_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR;

    return DDS_BOOLEAN_TRUE;
}


RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_delete_sample(
                    struct NDDS_Type_Plugin *plugin, void *sample,void *param)
{
    struct DDS_ParticipantBuiltinTopicData *builtin_data =
                            (struct DDS_ParticipantBuiltinTopicData *)sample;

    if (sample == NULL)
    {
        return DDS_BOOLEAN_FALSE;
    }

    DDS_LocatorSeq_finalize(&builtin_data->default_unicast_locators);
    DDS_LocatorSeq_finalize(&builtin_data->metatraffic_unicast_locators);
    DDS_LocatorSeq_finalize(&builtin_data->metatraffic_multicast_locators);


    /* TODO:  Any fields need to be freed? */
    OSAPI_Heap_freeStructure(builtin_data);

    return DDS_BOOLEAN_TRUE;
}


NDDS_TypePluginKeyKind
DPSE_ParticipantBuiltinTopicDataTypePlugin_get_key_kind(
                                struct NDDS_Type_Plugin* plugin, void *param)
{
    return NDDS_TYPEPLUGIN_GUID_KEY;
}

RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_copy_sample(
        struct NDDS_Type_Plugin *type,
        void *dst,
        const void *src,
        void *param)
{

    struct DDS_ParticipantBuiltinTopicData *destination =
        (struct DDS_ParticipantBuiltinTopicData *)dst;
    struct DDS_ParticipantBuiltinTopicData *source =
        (struct DDS_ParticipantBuiltinTopicData *)src;
    DDS_Long i = 0;

    if (dst == NULL)
    {
        DPSE_Log_error(LOG_BAD_SECOND_PARAMETER_IS_NULL);
        return DDS_BOOLEAN_FALSE;
    }
    if (src == NULL)
    {
        /*LOG_printError("invalid argument\n"); */
        DPSE_Log_error(LOG_BAD_THIRD_PARAMETER_IS_NULL);
        return DDS_BOOLEAN_FALSE;
    }

    for (i = 0; i < DDS_BUILTIN_TOPIC_KEY_TYPE_NATIVE_LENGTH; i++)
    {
        destination->key.value[i] = source->key.value[i];
    }

    strncpy(destination->participant_name.name,
            source->participant_name.name, 255); /* TODO:  Make a constant */

    destination->dds_builtin_endpoints = source->dds_builtin_endpoints;

    destination->rtps_protocol_version.major =
        source->rtps_protocol_version.major;
    destination->rtps_protocol_version.minor =
        source->rtps_protocol_version.minor;

    for (i = 0; i < DDS_VENDOR_ID_LENGTH_MAX; i++)
    {
        destination->rtps_vendor_id.vendorId[i] =
            source->rtps_vendor_id.vendorId[i];
    }

    /* --- Default unicast locator list --- */
    DDS_LocatorSeq_copy(&destination->default_unicast_locators,
                        &source->default_unicast_locators);

    /* --- Metatraffic unicast locator list --- */
    DDS_LocatorSeq_copy(&destination->metatraffic_unicast_locators,
                        &source->metatraffic_unicast_locators);

    /* --- Metatraffic multicast locator list --- */
    DDS_LocatorSeq_copy(&destination->metatraffic_multicast_locators,
                        &source->metatraffic_multicast_locators);

    destination->liveliness_lease_duration.sec =
        source->liveliness_lease_duration.sec;
    destination->liveliness_lease_duration.nanosec =
        source->liveliness_lease_duration.nanosec;

    destination->product_version.major = source->product_version.major;
    destination->product_version.minor = source->product_version.minor;
    destination->product_version.release = source->product_version.release;
    destination->product_version.revision = source->product_version.revision;

    return DDS_BOOLEAN_TRUE;
}


RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_deserialize(
                                                struct CDR_Stream_t *stream,
                                                void *sample,
                                                void *param)
{
    struct DDS_ParticipantBuiltinTopicData *topic_data =
        (struct DDS_ParticipantBuiltinTopicData *)sample;

    /* This is a new sample: empty out all sequences */
    DDS_LocatorSeq_set_length(&topic_data->default_unicast_locators, 0);
    DDS_LocatorSeq_set_length(&topic_data->metatraffic_unicast_locators, 0);
    DDS_LocatorSeq_set_length(&topic_data->metatraffic_multicast_locators, 0);

    return
        DPSE_ParticipantBuiltinTopicDataTypePlugin_deserializeParameterSequence
        (topic_data, stream,
         (DDS_ParticipantBuiltinTopicData_TypePluginSetDefaultParameterValuesFunction)
         DPSE_BuiltinTopicParticipantDataPluginSupport_setDefaultParameterValues,
         (DDS_ParticipantBuiltinTopicData_TypePluginDeserializeParameterValueFunction)
         DPSE_ParticipantBuiltinTopicData_deserializeParameterValue);
}

RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeKey(
                                     struct CDR_Stream_t *stream,
                                     const void *sample_key,
                                     void *param)
{
    return DDS_BOOLEAN_TRUE;
}

RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_deserializeKey(
                                    struct CDR_Stream_t *stream,
                                    const UserDataKeyHolder_t sample_key,
                                    void *param)
{
    return DDS_BOOLEAN_TRUE;
}

RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_instanceToKeyHash(
                                    struct NDDS_Type_Plugin *plugin,
                                    struct CDR_Stream_t *stream,
                                    DDS_KeyHash_t *keyHash,
                                    const void *instance,
                                    void *param)
{
    struct DDS_ParticipantBuiltinTopicData *topic_data =
        (struct DDS_ParticipantBuiltinTopicData *)instance;

    struct MIGRtpsGuid rtps;

    DDS_GUID_to_rtps(&rtps, (struct DDS_GUID_t *)&topic_data->key);

    OSAPI_Memory_copy(keyHash->value, &rtps, 16);

#if 0
    DDS_GUID_to_rtps((struct MIGRtpsGuid *)keyHash->value,
                     (struct DDS_GUID_t *)&topic_data->key);
#endif

    return DDS_BOOLEAN_TRUE;

}

RTI_PRIVATE struct NDDS_Type_Plugin DPSE_g_ParticipantBuiltinTopicDataTypePlugin =
{
    {0,0},
    NULL,
    NULL,
    0,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_serialize,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_deserialize,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_get_serialized_sample_max_size,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeKey,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_deserializeKey,
    NULL,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_create_sample,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_delete_sample,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_copy_sample,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_get_key_kind,
    DPSE_ParticipantBuiltinTopicDataTypePlugin_instanceToKeyHash
};

struct NDDS_Type_Plugin*
DPSE_ParticipantBuiltinTopicDataTypePlugin_get()
{
    return &DPSE_g_ParticipantBuiltinTopicDataTypePlugin;
}
