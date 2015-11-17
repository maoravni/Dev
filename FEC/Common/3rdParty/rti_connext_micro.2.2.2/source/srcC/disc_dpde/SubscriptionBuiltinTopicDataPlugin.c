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

#ifndef disc_dpde_discovery_plugin_h
#include "disc_dpde/disc_dpde_discovery_plugin.h"
#endif
#ifndef disc_dpde_log_h
#include "disc_dpde/disc_dpde_log.h"
#endif

#include "BuiltinCdr.pkg.h"

/*** SOURCE_BEGIN ***/

/* ------------------------------------------------------------------------ */
/*                   Builtin type plugin                                    */
/* ------------------------------------------------------------------------ */
RTI_PRIVATE DDS_UnsignedLong
DPDE_SubscriptionBuiltinTopicDataTypePlugin_get_parameters_max_size_serialized(
                                        DDS_UnsignedLong size,
                                        DDS_Long userDataMaximumLength,
                                        DDS_Long propertyListMaximumLength,
                                        DDS_Long propertyStringMaximumLength)
{
    DDS_UnsignedLong origSize = size;
    struct DDS_LocatorSeq locator = DDS_SEQUENCE_INITIALIZER(struct DDS_Locator_t);

    DDS_LocatorSeq_set_maximum(&locator,
                               MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX);

    DDS_LocatorSeq_set_length(&locator,
                              MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX);

    /* Call get max size serialized on each parameter header and field
     * according to the type.
     * We assume there is a logical reset before serializing each value.
     * getParameterHeaderMaxSizeSerialize() take care of 4-byte alignment.
     * Sentinel parameter size is not included in the calculation.
     */
    CDR_Stream_getEncapsulationSize(size);

    size += DDS_CdrQosPolicy_getKeyMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getKeyMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getTopicNameMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getTypeNameMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getDeadlineMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getOwnershipMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getReliabilityMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getLivelinessMaxSerializedSize(size);

    size += DDS_CdrQosPolicy_getDurabilityMaxSerializedSize(size);

    /*-- Unicast --*/
    DDS_LocatorSeq_set_maximum(&locator,
                               MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX);

    DDS_LocatorSeq_set_length(&locator,
                              MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX);
    size += DDS_CdrQosPolicy_getLocatorMaxSerializedSize(&locator, size);

    /*-- Multicast --*/
    DDS_LocatorSeq_set_maximum(&locator,
                               MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX);

    DDS_LocatorSeq_set_length(&locator,
                              MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX);
    size += DDS_CdrQosPolicy_getLocatorMaxSerializedSize(&locator, size);

    size = CDR_alignUpwards(size, 4);

    /* Add size for the sentinel parameter header */
    size += DDS_CdrQosPolicy_get_header_max_size_serialized(size);

    DDS_LocatorSeq_finalize(&locator);

    return (size - origSize);
}

RTI_UINT32
DPDE_SubscriptionBuiltinTopicDataTypePlugin_get_serialized_sample_max_size(
            struct NDDS_Type_Plugin *plugin,
            RTI_UINT32 current_alignment,
            void *param)
{
    return (RTI_UINT32)
            DPDE_SubscriptionBuiltinTopicDataTypePlugin_get_parameters_max_size_serialized(0, 0, 0, 0) + DDS_CdrQosPolicy_get_header_max_size_serialized(0);
}

RTI_PRIVATE RTI_BOOL
DPDE_SubscriptionBuiltinTopicDataTypePlugin_serialize(struct CDR_Stream_t *stream,
                                                      const void *data,
                                                      void *param)
{
    DDS_Boolean ok = RTI_FALSE;
    DDS_UnsignedShort zeroLength = 0;
    unsigned short sentinel;
    struct DDS_SubscriptionBuiltinTopicData *topic_data =
                            (struct DDS_SubscriptionBuiltinTopicData *)data;

    /*---------------------- Serialize the endpoint guid ---------------------*/
    if (!DDS_CdrQosPolicy_serializeKey(stream,
                &topic_data->key,MIG_RTPS_PID_ENDPOINT_GUID))
    {
        return ok;
    }

    /*---------------------- Serialize the participant guid ------------------*/
    if (!DDS_CdrQosPolicy_serializeKey(stream,
                &topic_data->participant_key,MIG_RTPS_PID_PARTICIPANT_GUID))
    {
        return ok;
    }

    /*---------------------- Serialize the topic name ------------------------*/
    if (!DDS_CdrQosPolicy_serializeTopicName(stream,
                                             topic_data->topic_name, NULL))
    {
        return ok;
    }

    /*---------------------- Serialize the type name -------------------------*/
    if (!DDS_CdrQosPolicy_serializeTypeName(stream,
                                            topic_data->type_name, NULL))
    {
        return ok;
    }

    /*------------------------ Serialize  deadline ---------------------------*/
    if (!DDS_CdrQosPolicy_serializeDeadline(stream,
                                            &topic_data->deadline, NULL))
    {
        return ok;
    }

    /*------------------------ Serialize ownership ---------------------------*/
    if (!DDS_CdrQosPolicy_serializeOwnership(stream,
                                             &topic_data->ownership, NULL))
    {
        return ok;
    }

    /*----------------------- Serialize reliability --------------------------*/
    if (!DDS_CdrQosPolicy_serializeReliability(stream,
                                               &topic_data->reliability, NULL))
    {
        return ok;
    }

    /*----------------------- Serialize liveliness ---------------------------*/
    if (!DDS_CdrQosPolicy_serializeLiveliness(stream,
                                              &topic_data->liveliness, NULL))
    {
        return ok;
    }

    /*------------------------ Serialize  durability -------------------------*/
    if (!DDS_CdrQosPolicy_serializeDurability(stream,
                                            &topic_data->durability, NULL))
    {
        return ok;
    }

    /*----------------------- Serialize unicast locators ---------------------*/
    if (!DDS_CdrQosPolicy_serializeLocator(stream,
                &topic_data->unicast_locator,MIG_RTPS_PID_UNICAST_LOCATOR6))
    {
        return ok;
    }

    /*----------------------- Serialize multicast locators -------------------*/
    if (!DDS_CdrQosPolicy_serializeLocator(stream,
                &topic_data->multicast_locator,MIG_RTPS_PID_MULTICAST_LOCATOR6))
    {
        return ok;
    }

    /*------------------------ Serialize sentinel ----------------------------*/

    sentinel = MIG_RTPS_PID_SENTINEL;
    if (DDS_BOOLEAN_TRUE !=
                CDR_Stream_serialize_UnsignedShort(stream, &sentinel))
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

/*
  Returns RTI_TRUE if there is a match in parameterId. Still need to set the
  ok to indicate if value is deserialized correctly.
*/
RTI_PRIVATE RTI_BOOL
DPDE_SubscriptionBuiltinTopicData_deserialize_pv(RTI_BOOL *ok,
        struct DDS_SubscriptionBuiltinTopicData *topic_data,
        struct CDR_Stream_t *stream,
        DDS_UnsignedShort parameterId,
        DDS_UnsignedShort parameterLength)
{
    /* check each field to see if the paramterId matches */
    switch (parameterId)
    {
        case MIG_RTPS_PID_PARTICIPANT_GUID:
            *ok = DDS_CdrQosPolicy_deserializeKey(
                stream, &topic_data->participant_key, NULL);
            break;
        case MIG_RTPS_PID_ENDPOINT_GUID:
            *ok = DDS_CdrQosPolicy_deserializeKey(
                stream, &topic_data->key, NULL);
            break;
        case MIG_RTPS_PID_TOPIC_NAME:
            *ok = DDS_CdrQosPolicy_deserializeTopicName(
                stream, topic_data->topic_name, NULL);
            break;
        case MIG_RTPS_PID_TYPE_NAME:
            *ok = DDS_CdrQosPolicy_deserializeTypeName(
                stream, topic_data->type_name, NULL);
            break;
        case MIG_RTPS_PID_DEADLINE:
            *ok = DDS_CdrQosPolicy_deserializeDeadline(
                stream, &topic_data->deadline, NULL);
            break;
        case MIG_RTPS_PID_OWNERSHIP:
            *ok = DDS_CdrQosPolicy_deserializeOwnership(
                stream, &topic_data->ownership, NULL);
            break;
        case MIG_RTPS_PID_RELIABILITY:
            *ok = DDS_CdrQosPolicy_deserializeReliability(
                stream, &topic_data->reliability, NULL);
            break;
        case MIG_RTPS_PID_LIVELINESS:
            *ok = DDS_CdrQosPolicy_deserializeLiveliness(
                stream, &topic_data->liveliness, NULL);
            break;
        case MIG_RTPS_PID_DURABILITY:
            *ok = DDS_CdrQosPolicy_deserializeDurability(
                stream, &topic_data->durability, NULL);
            break;
        case MIG_RTPS_PID_UNICAST_LOCATOR6:
            *ok = DDS_CdrQosPolicy_deserializeLocator(
                    stream, &topic_data->unicast_locator, NULL);
            break;
        case MIG_RTPS_PID_MULTICAST_LOCATOR6:
            *ok = DDS_CdrQosPolicy_deserializeLocator(
                    stream, &topic_data->multicast_locator, NULL);
            break;
        default:
            DPDE_LOG_WARNING_P1(
                    LOG_UNRECOGNIZED_SUBSCRIPTION_ANNOUNCEMENT_PARAMETER_ID,
                    parameterId);
            return *ok;
    }

    return *ok;
}

RTI_BOOL
DPDE_SubscriptionBuiltinTopicDataTypePlugin_create_sample(
                                            struct NDDS_Type_Plugin *plugin,
                                            void **sample,
                                            void *param)
{
    OSAPI_Heap_allocateStructure(sample,
                                 struct DDS_SubscriptionBuiltinTopicData);

    if (*sample == NULL)
    {
        return DDS_BOOLEAN_FALSE;
    }

    DDS_SubscriptionBuiltinTopicData_initialize(
                (struct DDS_SubscriptionBuiltinTopicData *)(*sample));

    return DDS_BOOLEAN_TRUE;
}


RTI_BOOL
DPDE_SubscriptionBuiltinTopicDataTypePlugin_delete_sample(
                                            struct NDDS_Type_Plugin *plugin,
                                            void *sample,
                                            void *param)
{
    DDS_SubscriptionBuiltinTopicData_finalize(
                        (struct DDS_SubscriptionBuiltinTopicData *)sample);

    OSAPI_Heap_freeStructure(sample);

    return DDS_BOOLEAN_TRUE;
}

RTI_PRIVATE NDDS_TypePluginKeyKind
DPDE_SubscriptionBuiltinTopicDataTypePlugin_get_key_kind(
                                            struct NDDS_Type_Plugin* plugin,
                                            void *param)
{
    return NDDS_TYPEPLUGIN_GUID_KEY;
}

RTI_BOOL
DPDE_SubscriptionBuiltinTopicDataTypePlugin_copy_sample(
        struct NDDS_Type_Plugin *type,
        void *dst,
        const void *src,
        void *param)
{
    DDS_SubscriptionBuiltinTopicData_copy(
            (struct DDS_SubscriptionBuiltinTopicData*)dst,
            (struct DDS_SubscriptionBuiltinTopicData*)src);

    return DDS_BOOLEAN_TRUE;
}

RTI_PRIVATE void
DPDE_SubscriptionBuiltinTopicDataPluginTypeSupport_set_dflt_pv(
        struct DDS_SubscriptionBuiltinTopicData *topic_data)
{
}

RTI_PRIVATE RTI_BOOL
DPDE_SubscriptionBuiltinTopicDataTypePlugin_deserialize(
                                                struct CDR_Stream_t *stream,
                                                void *sample,
                                                void *param)
{
    struct DDS_SubscriptionBuiltinTopicData *topic_data =
                            (struct DDS_SubscriptionBuiltinTopicData *)sample;

    DDS_LocatorSeq_set_length(&topic_data->unicast_locator, 0);
    DDS_LocatorSeq_set_length(&topic_data->multicast_locator, 0);

    return
        DDS_Cdr_deserializeParameterSequence(
                topic_data, stream,
                (DDS_Cdr_SetDefaultParameterValuesFunction)
                DPDE_SubscriptionBuiltinTopicDataPluginTypeSupport_set_dflt_pv,
                (DDS_Cdr_DeserializeParameterValueFunction)
                DPDE_SubscriptionBuiltinTopicData_deserialize_pv);

    return DDS_BOOLEAN_FALSE;
}

RTI_PRIVATE  RTI_BOOL
DPDE_SubscriptionBuiltinTopicDataTypePlugin_serializeKey(
                                                struct CDR_Stream_t *stream,
                                                const void* sample_key,
                                                void *param)
{
    return DDS_BOOLEAN_TRUE;
}

RTI_PRIVATE RTI_BOOL
DPDE_SubscriptionBuiltinTopicDataTypePlugin_deserializeKey(
                                                struct CDR_Stream_t *stream,
                                                UserDataKeyHolder_t sample_key,
                                                void *param)
{
    return DDS_BOOLEAN_TRUE;
}

RTI_PRIVATE RTI_BOOL
DPDE_SubscriptionBuiltinTopicDataTypePlugin_instanceToKeyHash(
                                        struct NDDS_Type_Plugin *plugin,
                                        struct CDR_Stream_t *stream,
                                        DDS_KeyHash_t *keyHash,
                                        const void *instance,
                                        void *param)
{
    struct DDS_SubscriptionBuiltinTopicData *topic_data =
        (struct DDS_SubscriptionBuiltinTopicData *)instance;

    struct MIGRtpsGuid rtps;

    DDS_GUID_to_rtps(&rtps, (struct DDS_GUID_t *)&topic_data->key);

    OSAPI_Memory_copy(keyHash->value, &rtps, 16);

    return DDS_BOOLEAN_TRUE;
}

RTI_PRIVATE struct NDDS_Type_Plugin DPDE_g_SubscriptionBuiltinTopicDataTypePlugin =
{
    {0,0},
    NULL,
    NULL,
    0,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_serialize,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_deserialize,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_get_serialized_sample_max_size,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_serializeKey,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_deserializeKey,
    NULL,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_create_sample,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_delete_sample,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_copy_sample,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_get_key_kind,
    DPDE_SubscriptionBuiltinTopicDataTypePlugin_instanceToKeyHash
};

struct NDDS_Type_Plugin*
DPDE_SubscriptionBuiltinTopicDataTypePlugin_get()
{
    return &DPDE_g_SubscriptionBuiltinTopicDataTypePlugin;
}