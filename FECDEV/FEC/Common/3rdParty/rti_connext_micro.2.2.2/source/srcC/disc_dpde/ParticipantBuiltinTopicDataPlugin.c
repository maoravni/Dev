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
#ifndef disc_dpde_dpdediscovery_h
#include "disc_dpde/disc_dpde_discovery_plugin.h"
#endif
#ifndef disc_dpde_log_h
#include "disc_dpde/disc_dpde_log.h"
#endif

#include "BuiltinCdr.pkg.h"

/*** SOURCE_BEGIN ***/

RTI_PRIVATE DDS_UnsignedLong
DPDE_ParticipantBuiltinTopicDataTypePlugin_get_parameters_max_size_serialized(
                                        DDS_UnsignedLong size,
                                        DDS_Long userDataMaximumLength,
                                        DDS_Long propertyListMaximumLength,
                                        DDS_Long propertyStringMaximumLength)
{
    DDS_UnsignedLong orig_sz = size;
    struct DDS_LocatorSeq locator = DDS_SEQUENCE_INITIALIZER(struct DDS_Locator_t);

    DDS_LocatorSeq_set_maximum(&locator,
                               MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX);

    DDS_LocatorSeq_set_length(&locator,
                              MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX);

    /*
     * Call get max size serialized on each parameter header and field
     * according to the type.
     * We assume there is a logical reset before serializing each value.
     * getParameterHeaderMaxSizeSerialize() take care of 4-byte alignment.
     * Sentinel parameter size is not included in the calculation.
     */
    CDR_Stream_getEncapsulationSize(size);

    size += DDS_CdrQosPolicy_getKeyMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getEntityNameQosPolicyMaxSizeSerialized(size);

    /* Builtin endpoint mask */
    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += CDR_get_maxSizeSerialized_UnsignedLong(0);

    /* promisciuity endpoint mask */
    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += CDR_get_maxSizeSerialized_UnsignedLong(0);

    /*  RTPS Protocol Version */
    size += DDS_CdrQosPolicy_getProtocolVersionMaxSerializedSize(size);

    /* RTPS Vendor Id */
    size += DDS_CdrQosPolicy_getVendorIdMaxSerializedSize(size);

    /* default uni-cast locators */
    size += DDS_CdrQosPolicy_getLocatorMaxSerializedSize(&locator, size);

    /* default meta-traffic unicast locator */
    size += DDS_CdrQosPolicy_getLocatorMaxSerializedSize(&locator, size);

    /* default meta-traffic multicast locator */
    size += DDS_CdrQosPolicy_getLocatorMaxSerializedSize(&locator, size);

    /* Lease duration */
    size += DDS_CdrQosPolicy_getLeaseDurationMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getProductVersionMaxSerializedSize(size);

    size = CDR_alignUpwards(size, 4);

    /* sentinel parameter header */
    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);

    DDS_LocatorSeq_finalize(&locator);

    return (size - orig_sz);
}

RTI_UINT32
DPDE_ParticipantBuiltinTopicDataTypePlugin_get_serialized_sample_max_size(
                                struct NDDS_Type_Plugin *plugin,
                                RTI_UINT32 current_alignment,
                                void *param)
{
    return (RTI_UINT32)DPDE_ParticipantBuiltinTopicDataTypePlugin_get_parameters_max_size_serialized(
            0, 0, 0, 0) + DDS_CdrQosPolicy_get_header_max_size_serialized(0);
}

RTI_PRIVATE RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_serialize(
                                                    struct CDR_Stream_t *stream,
                                                    const void *data,
                                                    void *param)
{
    DDS_Boolean ok = RTI_FALSE;
    DDS_UnsignedShort zeroLength = 0;
    DDS_UnsignedShort sentinel;
    struct DDS_ParticipantBuiltinTopicData *topic_data =
                                (struct DDS_ParticipantBuiltinTopicData *)data;

    if (!DDS_CdrQosPolicy_serializeKey(stream, &topic_data->key,
                                       MIG_RTPS_PID_PARTICIPANT_GUID))
    {
        return ok;
    }

    if (!DDS_CdrQosPolicy_serializeEntityNameQosPolicy(
        stream, &topic_data->participant_name, NULL))
    {
        return ok;
    }

    if (!DDS_Cdr_serializeFourByteParameter(
            stream, &topic_data->dds_builtin_endpoints,
            MIG_RTPS_PID_BUILTIN_ENDPOINT_MASK))
    {
        DPDE_Log_error(LOG_FAIL_SERIALIZE_BUILTIN_ENDPOINTS);
        return ok;
    }

    if (!DDS_CdrQosPolicy_serializeProtocolVersion(
            stream, &topic_data->rtps_protocol_version, NULL))
    {
        return ok;
    }

    if (!DDS_CdrQosPolicy_serializeVendorId(
            stream, &topic_data->rtps_vendor_id, NULL))
    {
        return ok;
    }

    if (!DDS_CdrQosPolicy_serializeLocator(
            stream, &topic_data->default_unicast_locators,
            MIG_RTPS_PID_DEFAULT_UNICAST_LOCATOR6))
    {
        return ok;
    }

    if (!DDS_CdrQosPolicy_serializeLocator(
            stream, &topic_data->metatraffic_unicast_locators,
            MIG_RTPS_PID_METATRAFFIC_UNICAST_LOCATOR6))
    {
        return ok;
    }

    if (!DDS_CdrQosPolicy_serializeLocator(
            stream, &topic_data->metatraffic_multicast_locators,
            MIG_RTPS_PID_METATRAFFIC_MULTICAST_LOCATOR6))
    {
        return ok;
    }

    if (!DDS_CdrQosPolicy_serializeLeaseDuration(
            stream, &topic_data->liveliness_lease_duration, NULL))
    {
        return ok;
    }

    if (!DDS_CdrQosPolicy_serializeProductVersion(
            stream, &topic_data->product_version, NULL))
    {
        return ok;
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

void
DPDE_BuiltinTopicParticipantDataPluginSupport_set_dflt_pv(
                        struct DDS_ParticipantBuiltinTopicData *topic_data)
{
}

/*
  Returns RTI_TRUE if there is a match in parameterId. Still need to set the
  ok to indicate if value is deserialized correctly.
*/
RTI_PRIVATE RTI_BOOL
DPDE_ParticipantBuiltinTopicData_deserialize_pv(RTI_BOOL *ok,
                            struct DDS_ParticipantBuiltinTopicData *topic_data,
                            struct CDR_Stream_t *stream,
                            DDS_UnsignedShort param_id,
                            DDS_UnsignedShort param_length)
{
    *ok = RTI_TRUE;

    switch (param_id)
    {
        case MIG_RTPS_PID_PARTICIPANT_GUID:
            *ok = DDS_CdrQosPolicy_deserializeKey(
                stream, &topic_data->key, NULL);
            break;
        case MIG_RTPS_PID_BUILTIN_ENDPOINT_MASK:
            if (!CDR_Stream_deserialize_UnsignedLong(
                    stream, &topic_data->dds_builtin_endpoints))
            {
                DPDE_Log_error(LOG_FAIL_DESERIALIZE_BUILTIN_ENDPOINTS);
                *ok = RTI_FALSE;
            }
            break;
        case MIG_RTPS_PID_PROTOCOL_VERSION:
            *ok = DDS_CdrQosPolicy_deserializeProtocolVersion(
                    stream, &topic_data->rtps_protocol_version, NULL);
            break;
        case MIG_RTPS_PID_VENDOR_ID:
            *ok = DDS_CdrQosPolicy_deserializeVendorId(
                    stream, &topic_data->rtps_vendor_id, NULL);
            break;
        case MIG_RTPS_PID_DEFAULT_UNICAST_LOCATOR6:
            *ok = DDS_CdrQosPolicy_deserializeLocator(
                    stream, &topic_data->default_unicast_locators, NULL);
            break;
        case MIG_RTPS_PID_METATRAFFIC_UNICAST_LOCATOR6:
            *ok = DDS_CdrQosPolicy_deserializeLocator(
                    stream, &topic_data->metatraffic_unicast_locators, NULL);
            break;
        case MIG_RTPS_PID_METATRAFFIC_MULTICAST_LOCATOR6:
            *ok = DDS_CdrQosPolicy_deserializeLocator(
                    stream, &topic_data->metatraffic_multicast_locators, NULL);
            break;
        case MIG_RTPS_PID_LEASE_DURATION:
            *ok = DDS_CdrQosPolicy_deserializeLeaseDuration(
                    stream, &topic_data->liveliness_lease_duration, NULL);
            break;
        case DISC_RTPS_PID_PRODUCT_VERSION:
            *ok = DDS_CdrQosPolicy_deserializeProductVersion(
                    stream, &topic_data->product_version, NULL);
            break;
        case DISC_RTPS_PID_ENTITY_NAME:
            *ok = DDS_CdrQosPolicy_deserializeEntityNameQosPolicy(
                    stream, &topic_data->participant_name, NULL);
            break;
        default:
            DPDE_LOG_WARNING_P1(
                    LOG_UNRECOGNIZED_PARTICIPANT_ANNOUNCEMENT_PARAMETER_ID,
                    param_id);
            return *ok;
    }

    return *ok;
}

RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_create_sample(
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

    DDS_LocatorSeq_set_maximum(
                        &participant_builtin_data->default_unicast_locators,
                        DDSC_PARTICIPANT_ADDRESS_COUNT_MAX);
    DDS_LocatorSeq_set_maximum(
                        &participant_builtin_data->metatraffic_unicast_locators,
                        DDSC_PARTICIPANT_ADDRESS_COUNT_MAX);
    DDS_LocatorSeq_set_maximum(
                        &participant_builtin_data->metatraffic_multicast_locators,
                        DDSC_PARTICIPANT_ADDRESS_COUNT_MAX);

    participant_builtin_data->dds_builtin_endpoints =
        DDS_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER |
        DDS_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR;

    return DDS_BOOLEAN_TRUE;
}

RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_delete_sample(
                                            struct NDDS_Type_Plugin *plugin,
                                            void *sample,
                                            void *param)
{
    DDS_ParticipantBuiltinTopicData_finalize(
                            (struct DDS_ParticipantBuiltinTopicData*)sample);

    OSAPI_Heap_freeStructure(sample);

    return DDS_BOOLEAN_TRUE;
}


RTI_PRIVATE NDDS_TypePluginKeyKind
DPDE_ParticipantBuiltinTopicDataTypePlugin_get_key_kind(
                                            struct NDDS_Type_Plugin* plugin,
                                            void *param)
{
    return NDDS_TYPEPLUGIN_GUID_KEY;
}

RTI_PRIVATE RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_copy_sample(
                                                struct NDDS_Type_Plugin *type,
                                                void *dst,
                                                const void *src,
                                                void *param)
{
    return (DDS_ParticipantBuiltinTopicData_copy(
            (struct DDS_ParticipantBuiltinTopicData *)dst,
            (struct DDS_ParticipantBuiltinTopicData *)src));
}

RTI_PRIVATE RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_deserialize(
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

    return DDS_Cdr_deserializeParameterSequence(topic_data, stream,
              (DDS_Cdr_SetDefaultParameterValuesFunction)
              DPDE_BuiltinTopicParticipantDataPluginSupport_set_dflt_pv,
              (DDS_Cdr_DeserializeParameterValueFunction)
              DPDE_ParticipantBuiltinTopicData_deserialize_pv);
}

RTI_PRIVATE RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_serializeKey(
                                        struct CDR_Stream_t *stream,
                                        const void *sample_key,
                                        void *param)
{
    return DDS_BOOLEAN_TRUE;
}


RTI_PRIVATE RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_deserializeKey(
                                        struct CDR_Stream_t *stream,
                                        UserDataKeyHolder_t sample_key,
                                        void *param)
{
    return DDS_BOOLEAN_TRUE;
}


RTI_PRIVATE RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_instanceToKeyHash(
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

    return DDS_BOOLEAN_TRUE;

}

RTI_PRIVATE struct NDDS_Type_Plugin DPDE_g_ParticipantBuiltinTopicDataTypePlugin =
{
    {0,0},
    NULL,
    NULL,
    0,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_serialize,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_deserialize,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_get_serialized_sample_max_size,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_serializeKey,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_deserializeKey,
    NULL,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_create_sample,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_delete_sample,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_copy_sample,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_get_key_kind,
    DPDE_ParticipantBuiltinTopicDataTypePlugin_instanceToKeyHash
};

struct NDDS_Type_Plugin*
DPDE_ParticipantBuiltinTopicDataTypePlugin_get()
{
    return &DPDE_g_ParticipantBuiltinTopicDataTypePlugin;
}
