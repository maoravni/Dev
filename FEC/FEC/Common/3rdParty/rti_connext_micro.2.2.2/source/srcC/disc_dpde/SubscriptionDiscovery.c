/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_domain_h
#include "dds_c/dds_c_domain.h"
#endif
#ifndef dds_c_subscription_h
#include "dds_c/dds_c_subscription.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif
#ifndef disc_dpde_dpdediscovery_h
#include "disc_dpde/disc_dpde_discovery_plugin.h"
#endif
#ifndef disc_dpde_log_h
#include "disc_dpde/disc_dpde_log.h"
#endif

#include "DiscoveryPlugin.pkg.h"
#include "SubscriptionBuiltinTopicDataPlugin.pkg.h"
#include "SubscriptionDiscovery.pkg.h"
#include "SubscriptionListener.pkg.h"

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DPDE_SubscriptionDiscovery_after_local_participant_created(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *local_participant_data)
{
    struct DPDE_DiscoveryPlugin *disc_plugin =
        (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    struct DDS_DataWriterQos writer_qos = DDS_DataWriterQos_INITIALIZER;
    struct DDS_DataReaderQos reader_qos = DDS_DataReaderQos_INITIALIZER;
    struct DDS_DataReaderListener pub_builtin_listener;
    struct DDS_DataWriterListener sub_builtin_listener;
    struct DDS_TopicQos topic_qos = DDS_TopicQos_INITIALIZER;

    DDS_Topic *topic = NULL;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    struct DDS_DomainParticipantQos *dp_qos;

    if (local_participant_data == NULL)
    {
        DPDE_Log_error(LOG_LOCAL_PARTICIPANT_HAS_INVALID_BUILTIN_DATA);
    }

    dp_qos = DDS_DomainParticipant_get_qos_ref(participant);

    disc_plugin->sub_type_plugin = DPDE_SubscriptionBuiltinTopicDataTypePlugin_get();

    if (DDS_RETCODE_OK !=
            DDS_DomainParticipant_register_type(participant,
                DDS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME,
                (struct NDDS_Type_Plugin *)disc_plugin->sub_type_plugin))
    {
        DPDE_Log_error(LOG_FAIL_REGISTER_SUBSCRIPTION_BUILTIN_TYPE);
        goto finally;
    }

    /* Builtin endpoints have well-known# object IDs */
    topic_qos.management.is_hidden = DDS_BOOLEAN_TRUE;

    topic = DDS_DomainParticipant_create_topic(participant,
                                    DDS_SUBSCRIPTION_BUILTIN_TOPIC_NAME,
                                    DDS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME,
                                    &topic_qos,
                                    NULL,
                                    DDS_STATUS_MASK_NONE);

    if (NULL == topic)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_SUBSCRIPTION_DISCOVERY_TOPIC);
        goto finally;
    }

    writer_qos.protocol.rtps_object_id =
        MIG_RTPS_OBJECT_ID_WRITER_SDP_SUBSCRIPTION;

    writer_qos.resource_limits.max_samples =
        dp_qos->resource_limits.local_reader_allocation;
    writer_qos.resource_limits.max_instances =
        writer_qos.resource_limits.max_samples;
    writer_qos.resource_limits.max_samples_per_instance = 1;
    writer_qos.history.depth = 1;
    writer_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    writer_qos.ownership.kind = DDS_SHARED_OWNERSHIP_QOS;
    writer_qos.durability.kind = DDS_TRANSIENT_LOCAL_DURABILITY_QOS;
    writer_qos.protocol.rtps_reliable_writer.heartbeats_per_max_samples = 0;
    writer_qos.protocol.rtps_reliable_writer.heartbeat_period.sec = 3;
    writer_qos.protocol.rtps_reliable_writer.heartbeat_period.nanosec = 0;
    writer_qos.deadline.period.sec = DDS_DURATION_INFINITE_SEC;
    writer_qos.deadline.period.nanosec = DDS_DURATION_INFINITE_NSEC;
    writer_qos.management.is_anonymous = DDS_BOOLEAN_FALSE;
    writer_qos.management.is_hidden = DDS_BOOLEAN_TRUE;

    DPDE_SubscriptionBuiltinDataWriterListener_initialize(
                                    disc_plugin, &sub_builtin_listener);

    disc_plugin->subscription_writer =
        DDS_Publisher_create_datawriter(disc_plugin->publisher,
                                        topic,
                                        &writer_qos,
                                        &sub_builtin_listener,
                                        DDS_PUBLICATION_MATCHED_STATUS);

    if (disc_plugin->subscription_writer == NULL)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_SUBSCRIPTION_DISCOVERY_WRITER);
        goto finally;
    }

    DPDE_SubscriptionBuiltinDataReaderListener_initialize(
                                    disc_plugin,
                                    &pub_builtin_listener);

    reader_qos.protocol.rtps_object_id =
        MIG_RTPS_OBJECT_ID_READER_SDP_SUBSCRIPTION;
    reader_qos.resource_limits.max_samples =
        dp_qos->resource_limits.remote_reader_allocation;
    reader_qos.resource_limits.max_instances =
        reader_qos.resource_limits.max_samples;
    reader_qos.resource_limits.max_samples_per_instance = 1;
    reader_qos.history.depth = 1;
    reader_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    reader_qos.ownership.kind = DDS_SHARED_OWNERSHIP_QOS;
    reader_qos.durability.kind = DDS_TRANSIENT_LOCAL_DURABILITY_QOS;
    reader_qos.reader_resource_limits.max_remote_writers =
        dp_qos->resource_limits.remote_participant_allocation;
    reader_qos.deadline.period.sec = DDS_DURATION_INFINITE_SEC;
    reader_qos.deadline.period.nanosec = DDS_DURATION_INFINITE_NSEC;

    reader_qos.management.is_anonymous = DDS_BOOLEAN_FALSE;
    reader_qos.management.is_hidden = DDS_BOOLEAN_TRUE;

    disc_plugin->subscription_reader =
        DDS_Subscriber_create_datareader(disc_plugin->subscriber,
                                         DDS_Topic_as_topicdescription(topic),
                                         &reader_qos, &pub_builtin_listener,
                                         DDS_STATUS_MASK_ALL);

    if (disc_plugin->subscription_reader == NULL)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_SUBSCRIPTION_DISCOVERY_READER);
        goto finally;
    }

    retcode = DDS_RETCODE_OK;

    finally:

    return retcode;
}

DDS_ReturnCode_t
DPDE_SubscriptionDiscovery_after_local_data_reader_deleted(
                    struct NDDS_Discovery_Plugin *const discovery_plugin,
                    DDS_DomainParticipant *const participant,
                    const struct DDS_BuiltinTopicKey_t *local_datareader_key)
{
    struct DPDE_DiscoveryPlugin *const dpde_plugin =
                    (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    DDS_InstanceHandle_t ih;
    DDS_ReturnCode_t retcode;

    /* Do not dispose builtin entities */
    if (local_datareader_key->value[3] & 0xc0)
    {
        return DDS_RETCODE_OK;
    }

    DDS_InstanceHandle_from_rtps(&ih,(struct MIGRtpsGuid *)local_datareader_key);

    retcode = DDS_DataWriter_dispose(
                                dpde_plugin->subscription_writer, NULL, &ih);

    if (retcode != DDS_RETCODE_OK)
    {
        DPDE_Log_error(LOG_FAIL_DISPOSE_SUBSCRIPTION);
    }

    return retcode;
}

DDS_ReturnCode_t
DPDE_SubscriptionDiscovery_after_local_data_reader_enabled(
                        struct NDDS_Discovery_Plugin *const discovery_plugin,
                        DDS_DomainParticipant *const participant,
                        DDS_DataReader *const data_reader,
                        const struct DDS_DataReaderQos *const qos)
{
    struct DDS_SubscriptionBuiltinTopicData *builtin_data;
    struct DPDE_DiscoveryPlugin *const dpde_plugin =
                    (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    struct MIGRtpsGuid guid;
    DDS_InstanceHandle_t instance_handle;

    /* Do not publish anything from our built-in endpoints */
    if ((data_reader == dpde_plugin->participant_reader) ||
        (data_reader == dpde_plugin->publication_reader) ||
        (data_reader == dpde_plugin->subscription_reader))
    {
        return DDS_RETCODE_OK;
    }

    /*TODO: Do not announce a local reader which can only be reached locally */

    builtin_data = dpde_plugin->subscription_builtin_data;

    instance_handle =
        DDS_Entity_get_instance_handle(DDS_DomainParticipant_as_entity
                                       (participant));
    DDS_InstanceHandle_to_rtps(&guid, &instance_handle);
    builtin_data->participant_key.value[0] = guid.prefix.hostId;
    builtin_data->participant_key.value[1] = guid.prefix.appId;
    builtin_data->participant_key.value[2] = guid.prefix.instanceId;
    builtin_data->participant_key.value[3] = guid.objectId;

    instance_handle =
        DDS_Entity_get_instance_handle(DDS_DataReader_as_entity(data_reader));
    DDS_InstanceHandle_to_rtps(&guid, &instance_handle);
    builtin_data->key.value[0] = guid.prefix.hostId;
    builtin_data->key.value[1] = guid.prefix.appId;
    builtin_data->key.value[2] = guid.prefix.instanceId;
    builtin_data->key.value[3] = guid.objectId;

    builtin_data->deadline = qos->deadline;
    builtin_data->liveliness = qos->liveliness;
    builtin_data->ownership = qos->ownership;
    builtin_data->reliability = qos->reliability;
    builtin_data->durability = qos->durability;

    DDS_String_replace(&builtin_data->topic_name,
                       DDS_TopicDescription_get_name
                       (DDS_DataReader_get_topicdescription
                        ((DDS_DataReader *) data_reader)));

    DDS_String_replace(&builtin_data->type_name,
                       DDS_TopicDescription_get_type_name
                       (DDS_DataReader_get_topicdescription
                        ((DDS_DataReader *) data_reader)));

    return DDS_DataWriter_write(dpde_plugin->subscription_writer,
                                (void *)builtin_data, &instance_handle);
}

DDS_ReturnCode_t
DPDE_SubscriptionDiscovery_assert_remote_builtin(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *dp_builtin_data)
{
    struct DDS_PublicationBuiltinTopicData *pub_builtin_data;
    struct DDS_SubscriptionBuiltinTopicData *sub_builtin_data;
    struct DPDE_DiscoveryPlugin *const dpde_plugin =
                            (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;

    pub_builtin_data = dpde_plugin->publication_builtin_data;
    pub_builtin_data->participant_key = dp_builtin_data->key;
    pub_builtin_data->key = dp_builtin_data->key;
    pub_builtin_data->key.value[3] = MIG_RTPS_OBJECT_ID_WRITER_SDP_SUBSCRIPTION;
    pub_builtin_data->reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    pub_builtin_data->ownership.kind = DDS_SHARED_OWNERSHIP_QOS;
    pub_builtin_data->liveliness.kind = DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS;
    pub_builtin_data->liveliness.lease_duration.sec = DDS_DURATION_INFINITE_SEC;
    pub_builtin_data->liveliness.lease_duration.nanosec = DDS_DURATION_INFINITE_NSEC;
    pub_builtin_data->deadline.period.sec = DDS_DURATION_INFINITE_SEC;
    pub_builtin_data->deadline.period.nanosec = DDS_DURATION_INFINITE_NSEC;
    pub_builtin_data->durability.kind = DDS_TRANSIENT_LOCAL_DURABILITY_QOS;

    DDS_String_replace(&pub_builtin_data->topic_name,
                       DDS_SUBSCRIPTION_BUILTIN_TOPIC_NAME);

    DDS_String_replace(&pub_builtin_data->type_name,
                       DDS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME);

    retcode = NDDS_DomainParticipant_assert_remote_publication(participant,
                                                 NULL,
                                                 pub_builtin_data,
                                                 NDDS_TYPEPLUGIN_GUID_KEY);
    if (retcode != DDS_RETCODE_OK)
    {
        DPDE_LOG_DEBUG(LOG_FAIL_ASSERT_REMOTE_PUBLICATION);
        retcode = DDS_RETCODE_ERROR;
        goto done;
    }

    sub_builtin_data = dpde_plugin->subscription_builtin_data;
    sub_builtin_data->participant_key = dp_builtin_data->key;
    sub_builtin_data->key = dp_builtin_data->key;
    sub_builtin_data->key.value[3] = MIG_RTPS_OBJECT_ID_READER_SDP_SUBSCRIPTION;
    sub_builtin_data->reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    sub_builtin_data->liveliness.kind = DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS;
    sub_builtin_data->ownership.kind = DDS_SHARED_OWNERSHIP_QOS;
    sub_builtin_data->liveliness.lease_duration.sec = DDS_DURATION_INFINITE_SEC;
    sub_builtin_data->liveliness.lease_duration.nanosec = DDS_DURATION_INFINITE_NSEC;
    sub_builtin_data->deadline.period.sec = DDS_DURATION_INFINITE_SEC;
    sub_builtin_data->deadline.period.nanosec = DDS_DURATION_INFINITE_NSEC;
    sub_builtin_data->durability.kind = DDS_TRANSIENT_LOCAL_DURABILITY_QOS;

    DDS_String_replace(&sub_builtin_data->topic_name,
                        DDS_SUBSCRIPTION_BUILTIN_TOPIC_NAME);

    DDS_String_replace(&sub_builtin_data->type_name,
                       DDS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME);

    retcode = NDDS_DomainParticipant_assert_remote_subscription(participant,
                                                NULL,
                                                sub_builtin_data,
                                                NDDS_TYPEPLUGIN_GUID_KEY);

    if (retcode != DDS_RETCODE_OK)
    {
        DPDE_LOG_DEBUG(LOG_FAIL_ASSERT_REMOTE_SUBSCRIPTION);
        retcode = DDS_RETCODE_ERROR;
        goto done;
    }

    retcode = DDS_RETCODE_OK;

    done:
    return retcode;
}
