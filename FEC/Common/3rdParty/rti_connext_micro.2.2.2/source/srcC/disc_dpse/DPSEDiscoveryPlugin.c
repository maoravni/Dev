/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef osapi_timer_h
#include "osapi/osapi_timer.h"
#endif
#ifndef osapi_system_h
#include "osapi/osapi_system.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif
#ifndef disc_dpse_dpsediscovery_h
#include "disc_dpse/disc_dpse_dpsediscovery.h"
#endif
#ifndef disc_dpse_log_h
#include "disc_dpse/disc_dpse_log.h"
#endif

#include "DPSECdr.pkg.h"
#include "DPSEParticipantBuiltinTopicData.pkg.h"
#include "DPSEParticipantDiscovery.pkg.h"
#include "DPSEParticipantListener.pkg.h"

/* ------------------------------------------------------------------------ */
/*                   DPSE Discovery plugin                                  */
/* ------------------------------------------------------------------------ */

struct DPSE_DiscoveryPlugin
{
    /* --- Parent plugin --- */
    struct NDDS_Discovery_Plugin _parent;

    /* --- Participant discovery writer --- */
    DDS_Publisher *participant_publisher;
    DDS_DataWriter *participant_writer;

    /* --- Participant discovery reader --- */
    DDS_Subscriber *participant_subscriber;
    DDS_DataReader *participant_reader;

    /*DDS_InstanceHandle_t *instance_handle; */

    struct OSAPI_NtpTime participant_liveliness_assert_period;
    struct OSAPI_NtpTime initial_participant_announcement_period;

    struct DPSE_DiscoveryPluginProperty properties;

    /* An array of all the IDs of remote entities that we have discovered.
     */
    DDS_BuiltinTopicKey_t *key_list;
    int key_list_length;
    int key_list_max;

    /* The announcement event, which may get updated as we receive announcements
     * from remote participants.
     */
    OSAPITimeoutHandle_t announcement_event;

    /* This is owned by the participant, not the plugin.  A pointer is 
     * stored for convenience
     */
    struct DDS_ParticipantBuiltinTopicData *participant_builtin_data;
};

struct DPSE_Discovery_Factory
{
    struct RT_ComponentFactory _parent;
    struct DPSE_DiscoveryPluginProperty property;
};

/*** SOURCE_BEGIN ***/

/* Copy a DPSE_DiscoveryPluginProperty structure.
 * 
 * This will allocate memory for a Packages list and multicast receive address
 * if the destination has not allocated those arrays.
 * 
 * pre: dst and src must not be NULL
 * post: the fields of dst will equal the fields of src.
 * return: 
 *     DDS_RETCODE_OK if no errors
 *     DDS_RETCODE_ERROR if there is a problem copying
 */
DDS_ReturnCode_t
DPSE_DiscoveryPluginProperty_copy(struct DPSE_DiscoveryPluginProperty *dst,
                                  struct DPSE_DiscoveryPluginProperty *src)
{
    /* --- Check that the parameters are not NULL --- */
    if (dst == NULL || src == NULL)
    {
        return DDS_RETCODE_ERROR;
    }

    /* --- Copy the assert period --- */
    dst->participant_liveliness_assert_period =
        src->participant_liveliness_assert_period;

    /* --- Copy the lease duration --- */
    dst->participant_liveliness_lease_duration =
        src->participant_liveliness_lease_duration;

    /* --- Copy the participant announcements --- */
    dst->initial_participant_announcements =
        src->initial_participant_announcements;

    /* --- Copy the initial announcement period --- */
    dst->initial_participant_announcement_period =
        src->initial_participant_announcement_period;

    return DDS_RETCODE_OK;
}

/* Finalize a DPSE_DiscoveryPluginProperty structure.
 * 
 * This will free memory for a Packages list and multicast receive address.
 * 
 * pre: properties must not be NULL
 * post: the memory used by the properties will be freed
 * return: returns DDS_RETCODE_OK
 */
DDS_ReturnCode_t
DPSE_DiscoveryPluginProperty_finalize(
                               struct DPSE_DiscoveryPluginProperty *properties)
{
    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
DPSE_RemoteParticipant_assert_builtin(
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const data)
{
    DDS_Boolean is_new;

    LOG_PRECONDITION((participant == NULL),return DDS_RETCODE_BAD_PARAMETER)


    return NDDS_DomainParticipant_assert_remote_participant(participant, data,
                                                            &is_new);
}

DDS_ReturnCode_t
DPSE_RemoteParticipant_assert(DDS_DomainParticipant * const participant,
                                   const char *rem_participant_name)
{
    struct DDS_ParticipantBuiltinTopicData remote_part_data =
                                  DDS_ParticipantBuiltinTopicData_INITIALIZER;
    DDS_ReturnCode_t retcode;

    LOG_PRECONDITION_P2((participant == NULL) || (rem_participant_name == NULL),
                        return DDS_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(participant),
                        OSAPI_LOG_PTR_LOW(rem_participant_name))

    strcpy(remote_part_data.participant_name.name, rem_participant_name);


    retcode = DPSE_RemoteParticipant_assert_builtin(participant,
                                                         &remote_part_data);

    DDS_ParticipantBuiltinTopicData_finalize(&remote_part_data);

    return retcode;
}

/*ce \dref_RemotePublication_assert
 */
DDS_ReturnCode_t
DPSE_RemotePublication_assert(DDS_DomainParticipant *const participant,
                      const char *const rem_participant_name,
                      const struct DDS_PublicationBuiltinTopicData *const data,
                      NDDS_TypePluginKeyKind key_kind)
{
    LOG_PRECONDITION_P5((participant == NULL) ||
                        (rem_participant_name == NULL) ||
                        (data == NULL) ||
                        (data->topic_name == NULL) ||
                        (data->type_name == NULL),
                        return DDS_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(participant),
                        OSAPI_LOG_PTR_LOW(rem_participant_name),
                        OSAPI_LOG_PTR_LOW(data),
                        OSAPI_LOG_PTR_LOW(data->topic_name),
                        OSAPI_LOG_PTR_LOW(data->type_name))

    return NDDS_DomainParticipant_assert_remote_publication(participant,
                                                            rem_participant_name,
                                                            data, key_kind);

}

/*ce \dref_RemoteSubscription_assert
 */
DDS_ReturnCode_t
DPSE_RemoteSubscription_assert(DDS_DomainParticipant *const participant,
                const char *const rem_participant_name,
                const struct DDS_SubscriptionBuiltinTopicData *const data,
                NDDS_TypePluginKeyKind key_kind)
{
    LOG_PRECONDITION_P5((participant == NULL) ||
                        (rem_participant_name == NULL) ||
                        (data == NULL) ||
                        (data->topic_name == NULL) ||
                        (data->type_name == NULL),
                        return DDS_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(participant),
                        OSAPI_LOG_PTR_LOW(rem_participant_name),
                        OSAPI_LOG_PTR_LOW(data),
                        OSAPI_LOG_PTR_LOW(data->topic_name),
                        OSAPI_LOG_PTR_LOW(data->type_name))

    return NDDS_DomainParticipant_assert_remote_subscription(participant,
                                                             rem_participant_name,
                                                             data,
                                                             key_kind);
}

DDS_ReturnCode_t
DPSE_DiscoveryPlugin_before_local_participant_created(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant * const participant,
        struct DDS_DomainParticipantQos *local_participant_qos,
        struct DDS_ParticipantBuiltinTopicData *participant_data_out)
{
    struct DPSE_DiscoveryPlugin *disc_plugin =
        (struct DPSE_DiscoveryPlugin *)discovery_plugin;

    local_participant_qos->resource_limits.local_writer_allocation =
        local_participant_qos->resource_limits.local_writer_allocation + 1;
    local_participant_qos->resource_limits.local_reader_allocation =
        local_participant_qos->resource_limits.local_reader_allocation + 1;
    local_participant_qos->resource_limits.local_publisher_allocation =
        local_participant_qos->resource_limits.local_publisher_allocation + 1;
    local_participant_qos->resource_limits.local_subscriber_allocation =
        local_participant_qos->resource_limits.local_subscriber_allocation + 1;
    local_participant_qos->resource_limits.local_topic_allocation =
        local_participant_qos->resource_limits.local_topic_allocation + 1;
    local_participant_qos->resource_limits.local_type_allocation =
        local_participant_qos->resource_limits.local_type_allocation + 1;
    local_participant_qos->resource_limits.remote_writer_allocation +=
      3 * local_participant_qos->resource_limits.remote_participant_allocation;
    local_participant_qos->resource_limits.remote_reader_allocation +=
      3 * local_participant_qos->resource_limits.remote_participant_allocation;

    /* --- Allocate a list of participant keys --- */
    OSAPI_Heap_allocateArray(&disc_plugin->key_list,
                            local_participant_qos->resource_limits.
                            remote_participant_allocation,
                            DDS_BuiltinTopicKey_t);

    disc_plugin->key_list_max =
        local_participant_qos->resource_limits.remote_participant_allocation;
    disc_plugin->key_list_length = 0;

    return DDS_RETCODE_OK;
}

/* The anonymous writer has a list of locators that it sends to.  Initialize
 * these locators from the discovery plugin's initial Packages list.
 */
DDS_Boolean
DPSE_DiscoveryPlugin_add_peer(struct DPSE_DiscoveryPlugin *discovery_plugin,
                              DDS_DomainParticipant *const participant,
                              struct DDS_DomainParticipantQos *participant_qos,
                              const char *Package_addr)
{
    DDS_Long i;
    DDS_Boolean isValidAddress = DDS_BOOLEAN_TRUE;
    struct NETIO_Address address;
    struct NETIO_Address dst_reader;

    NETIO_Address_init(&dst_reader,NETIO_ADDRESS_KIND_GUID);
    dst_reader.value.rtps_guid.object_id =
                        NETIO_htonl(MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT);

    isValidAddress = NETIO_Address_ipv4_string_to_address(&address, Package_addr);

    /* Check if the address was valid */
    if (!isValidAddress)
    {
        DPSE_LOG_WARNING(LOG_INVALID_ADDRESS_IN_Package_HOST_LIST);
        return DDS_BOOLEAN_FALSE;
    }

    address.kind = RTPS_LOCATOR_KIND_UDPv4;

    /* Calculate the ports */
    if ((address.value.ipv4.address >= 0xe0000000) &&
            (address.value.ipv4.address <= 0xefffffff))
    {
        address.port = MIGRtps_getWellKnownMulticastPort(
                DDS_DomainParticipant_get_domain_id(participant),
                participant_qos->protocol.rtps_well_known_ports.port_base,
                participant_qos->protocol.rtps_well_known_ports.domain_id_gain,
                participant_qos->protocol.
                rtps_well_known_ports.builtin_multicast_port_offset);
        /* Add route */
    }
    else
    {

        for (i = 0; i < 4; i++)
        {
            address.port = MIGRtps_getWellKnownUnicastPort(
                    DDS_DomainParticipant_get_domain_id(participant), i,
                    participant_qos->protocol.rtps_well_known_ports.port_base,
                    participant_qos->protocol.rtps_well_known_ports.domain_id_gain,
                    participant_qos->protocol.rtps_well_known_ports.participant_id_gain,
                    participant_qos->protocol.rtps_well_known_ports.builtin_unicast_port_offset);
            /*
            DPSE_LOG_DEBUG_P2(LOG_LOCATOR_ADDRESS_ADDED_TO_PROPERTIES,
                    address.value.ipv4.address, address.port) */
            DDS_DataWriter_add_anonymous_route(
                    discovery_plugin->participant_writer,
                    &dst_reader,
                    &address);

        }

    }
    return DDS_BOOLEAN_TRUE;
}

DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_participant_created(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const local_participant_data)
{
    struct DPSE_DiscoveryPlugin *disc_plugin =
        (struct DPSE_DiscoveryPlugin *)discovery_plugin;
    struct DDS_PublisherQos publisher_qos = DDS_PublisherQos_INITIALIZER;
    struct DDS_SubscriberQos subscriber_qos = DDS_SubscriberQos_INITIALIZER;
    struct DDS_DataWriterQos writer_qos = DDS_DataWriterQos_INITIALIZER;
    struct DDS_DataReaderQos reader_qos = DDS_DataReaderQos_INITIALIZER;
    struct NDDS_Type_Plugin *type_plugin;
    struct DDS_DataReaderListener participant_builtin_listener;
    DDS_Topic *topic = NULL;
    struct DDS_TopicQos topic_qos = DDS_TopicQos_INITIALIZER;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    struct DDS_DomainParticipantQos *dp_qos;
    DDS_Long Package_no,Package_length;
    struct NETIO_Address src_writer;
    struct NETIO_Address to_address;
    struct DDS_Locator *a_locator;
    DDS_Long peer_no,peer_length;

    NETIO_Address_init(&src_writer,NETIO_ADDRESS_KIND_GUID);
    src_writer.value.rtps_guid.object_id =
                        NETIO_htonl(MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT);

    if (local_participant_data == NULL)
    {
        DPSE_Log_error(LOG_LOCAL_PARTICIPANT_HAS_INVALID_BUILTIN_DATA);
    }

    disc_plugin->participant_builtin_data = local_participant_data;

    dp_qos = DDS_DomainParticipant_get_qos_ref(participant);

    /* Create endpoints */
    publisher_qos.management.is_hidden= DDS_BOOLEAN_TRUE;

    disc_plugin->participant_publisher = DDS_DomainParticipant_create_publisher(
                                                    participant,
                                                    &publisher_qos,
                                                    NULL,
                                                    DDS_STATUS_MASK_NONE);

    if (disc_plugin->participant_publisher == NULL)
    {
        DPSE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_PUBLISHER);
        goto finally;
    }

    subscriber_qos.management.is_hidden = DDS_BOOLEAN_TRUE;
    disc_plugin->participant_subscriber = DDS_DomainParticipant_create_subscriber(
                                                participant,
                                                &subscriber_qos,
                                                NULL,
                                                DDS_STATUS_MASK_NONE);

    if (disc_plugin->participant_subscriber == NULL)
    {
        DPSE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_SUBSCRIBER);
        goto finally;
    }

    type_plugin = DPSE_ParticipantBuiltinTopicDataTypePlugin_get();

    if (DDS_RETCODE_OK != DDS_DomainParticipant_register_type(
                                    participant,
                                    DDS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME,
                                    (struct NDDS_Type_Plugin *)type_plugin))
    {
        DPSE_Log_error(LOG_FAIL_REGISTER_PARTICIPANT_BUILTIN_TYPE);
        goto finally;
    }

    topic_qos.management.is_hidden = DDS_BOOLEAN_TRUE;
    topic = DDS_DomainParticipant_create_topic(participant,
                                    DDS_PARTICIPANT_BUILTIN_TOPIC_NAME,
                                    DDS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME,
                                    &topic_qos, NULL,
                                    DDS_STATUS_MASK_NONE);

    if (NULL == topic)
    {
        DPSE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_TOPIC);
        goto finally;
    }

    /* Builtin endpoints have well-known object IDs */
    writer_qos.protocol.rtps_object_id =
                                 MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT;

    writer_qos.resource_limits.max_instances = 1;
    writer_qos.resource_limits.max_samples = 1;
    writer_qos.resource_limits.max_samples_per_instance = 1;
    writer_qos.protocol.rtps_reliable_writer.heartbeats_per_max_samples = 0;
    writer_qos.liveliness.lease_duration.sec = DDS_DURATION_INFINITE_SEC;
    writer_qos.liveliness.lease_duration.nanosec = DDS_DURATION_INFINITE_NSEC;
    writer_qos.management.is_anonymous = DDS_BOOLEAN_TRUE;
    writer_qos.management.is_hidden = DDS_BOOLEAN_TRUE;
    writer_qos.ownership.kind = DDS_SHARED_OWNERSHIP_QOS;
    writer_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;

    disc_plugin->participant_writer = DDS_Publisher_create_datawriter(
                                        disc_plugin->participant_publisher,
                                        topic,
                                        &writer_qos,
                                        NULL,
                                        DDS_STATUS_MASK_NONE);
    if (disc_plugin->participant_writer == NULL)
    {
        DPSE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_WRITER);
        goto finally;
    }

    /* Parse and add the initial Package list to the announcer*/
    peer_length = DDS_StringSeq_get_length(&dp_qos->discovery.initial_peers);
    for (peer_no = 0; peer_no < peer_length; peer_no++)
    {
        DPSE_DiscoveryPlugin_add_peer(disc_plugin,
           participant,
           dp_qos,
           *DDS_StringSeq_get_reference(
                               &dp_qos->discovery.initial_peers,peer_no));
    }

    DPSE_ParticipantBuiltinDataReaderListener_initialize(
                            (struct NDDS_Discovery_Plugin*)disc_plugin,
                            &participant_builtin_listener);

    reader_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
    reader_qos.protocol.rtps_object_id =
        MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT;
    reader_qos.resource_limits.max_samples =
        dp_qos->resource_limits.remote_participant_allocation + 1;
    reader_qos.resource_limits.max_instances =
        reader_qos.resource_limits.max_samples;
    reader_qos.resource_limits.max_samples_per_instance = 1;
    reader_qos.reader_resource_limits.max_remote_writers =
        dp_qos->resource_limits.remote_participant_allocation;
    reader_qos.liveliness.kind = DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS;
    reader_qos.liveliness.lease_duration.sec = DDS_DURATION_INFINITE_SEC;
    reader_qos.liveliness.lease_duration.nanosec = DDS_DURATION_INFINITE_NSEC;
    reader_qos.management.is_anonymous = DDS_BOOLEAN_TRUE;
    reader_qos.management.is_hidden = DDS_BOOLEAN_TRUE;
    reader_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
    reader_qos.ownership.kind = DDS_SHARED_OWNERSHIP_QOS;

    disc_plugin->participant_reader =
        DDS_Subscriber_create_datareader(disc_plugin->participant_subscriber,
                                         DDS_Topic_as_topicdescription(topic),
                                         &reader_qos,
                                         &participant_builtin_listener,
                                         DDS_STATUS_MASK_ALL);
    if (disc_plugin->participant_reader == NULL)
    {
        DPSE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_READER);
        goto finally;

    }

    Package_length = DDS_LocatorSeq_get_length(
                        &local_participant_data->metatraffic_unicast_locators);
    for (Package_no = 0; Package_no < Package_length; Package_no++)
    {
        a_locator = DDS_LocatorSeq_get_reference(
                &local_participant_data->metatraffic_unicast_locators,Package_no);

        to_address = *(struct NETIO_Address*)a_locator;
        DDS_DataReader_add_anonymous_route(disc_plugin->participant_reader,
                &src_writer,
                &to_address);
    }

    /* Set discovery-specific information in the participant's builtin topic 
     * data.
     */
    local_participant_data->liveliness_lease_duration =
        disc_plugin->properties.participant_liveliness_lease_duration;

    retcode = DDS_RETCODE_OK;

finally:
    if (DDS_RETCODE_OK != retcode)
    {
        DPSE_Log_error(LOG_FAIL_INITIALIZE_DISCOVERY_PLUGIN);

        if (NULL != topic)
        {
            DDS_DomainParticipant_delete_topic(participant, topic);
        }

        if (NULL != disc_plugin->participant_publisher)
        {
            DDS_Publisher_delete_contained_entities(
                                        disc_plugin->participant_publisher);

            DDS_DomainParticipant_delete_publisher(
                                        participant,
                                        disc_plugin->participant_publisher);
        }

        if (NULL != disc_plugin->participant_subscriber)
        {
            DDS_Subscriber_delete_contained_entities(
                                    disc_plugin->participant_subscriber);

            DDS_DomainParticipant_delete_subscriber(participant,
                                    disc_plugin->participant_subscriber);
        }
    }

    return retcode;
}

/*i dref_Discovery_Plugin_AfterLocalParticipantEnabledCallback
 */
DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_participant_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const local_participant_data)
{
    struct DPSE_DiscoveryPlugin *disc_plugin =
        (struct DPSE_DiscoveryPlugin *)discovery_plugin;

    DDS_Entity_enable(DDS_Publisher_as_entity(disc_plugin->participant_publisher));
    DDS_Entity_enable(DDS_Subscriber_as_entity(disc_plugin->participant_subscriber));

    return DPSE_ParticipantDiscovery_schedule_fast_assertions(
                    discovery_plugin, local_participant_data, DDS_BOOLEAN_TRUE);
}

/*i dref_Discovery_Plugin_BeforeLocalParticipantDeletedCallback
 */
DDS_ReturnCode_t
DPSE_DiscoveryPlugin_announce_local_participant_deletion(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        const struct DDS_BuiltinTopicKey_t *local_participant_key)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    struct DPSE_DiscoveryPlugin *dpse_plugin =
        (struct DPSE_DiscoveryPlugin *)discovery_plugin;

    /* TODO: This needs to be tested */
    if (DDS_RETCODE_OK !=
        DDS_DataWriter_dispose(dpse_plugin->participant_writer,
                               dpse_plugin->participant_builtin_data,
                               &DDS_HANDLE_NIL))
    {
        DPSE_Log_error(LOG_FAIL_DISPOSE_PARTICIPANT);
        goto done;
    }

    retcode = DDS_RETCODE_OK;

done:
    return retcode;
}

/*i dref_Discovery_Plugin_BeforeLocalParticipantDeletedCallback
 */
DDS_ReturnCode_t
DPSE_DiscoveryPlugin_before_local_participant_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_BuiltinTopicKey_t *const local_participant_key)
{
    struct DPSE_DiscoveryPlugin *dpse_plugin =
        (struct DPSE_DiscoveryPlugin *)discovery_plugin;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    DDS_TopicDescription *topic;

    OSAPI_Timer_delete_timeout(DDS_DomainParticipant_get_timer(participant),
                              &dpse_plugin->announcement_event);
    /* It's possible that in an error condition, the participant was not
     * allocated, but the participant is being deleted due to the error.
     */
    if (dpse_plugin->participant_publisher != NULL)
    {

        /* It's possible that in an error condition, the data writer was
         * not allocated, and the participant is being deleted due to the error.
         */
        if (dpse_plugin->participant_writer != NULL)
        {
            if (DDS_RETCODE_OK !=
                DPSE_DiscoveryPlugin_announce_local_participant_deletion(
                                     discovery_plugin, local_participant_key))
            {
                DPSE_Log_error(LOG_FAIL_ANNOUNCE_LOCAL_PARTICIPANT_DELETION);
            }

            if (DDS_RETCODE_OK !=
                DDS_Publisher_delete_datawriter(dpse_plugin->participant_publisher,
                                                dpse_plugin->participant_writer))
            {
                DPSE_Log_error(LOG_FAIL_DELETE_PARTICIPANT_DISCOVERY_DATA_WRITER);
            }
            dpse_plugin->participant_writer = NULL;
        }

        if (DDS_RETCODE_OK != DDS_DomainParticipant_delete_publisher(participant,
                                        dpse_plugin->participant_publisher))
        {
            DPSE_Log_error(LOG_FAIL_DELETE_PARTICIPANT_DISCOVERY_PUBLISHER);
            goto done;
        }
        dpse_plugin->participant_publisher = NULL;
    }

    /* In an error condition, it's possible that the participant was never
     * fully-created, so the subscriber is NULL.
     */
    if (dpse_plugin->participant_subscriber != NULL)
    {
        /* In an error condition, it's possible that the participant was never
         * fully-created, so the reader is NULL.
         */
        if (dpse_plugin->participant_reader != NULL)
        {
            if (DDS_RETCODE_OK !=
                DDS_Subscriber_delete_datareader(dpse_plugin->participant_subscriber,
                                                 dpse_plugin->participant_reader))
            {
                DPSE_Log_error(LOG_FAIL_DELETE_PARTICIPANT_DISCOVERY_DATA_READER);
            }
            dpse_plugin->participant_reader = NULL;
        }

        if (DDS_RETCODE_OK !=
            DDS_DomainParticipant_delete_subscriber(participant,
                                        dpse_plugin->participant_subscriber))
        {
            DPSE_Log_error(LOG_FAIL_DELETE_PARTICIPANT_DISCOVERY_SUBSCRIBER);
            goto done;
        }
        dpse_plugin->participant_subscriber = NULL;
    }

    topic = DDS_DomainParticipant_lookup_topicdescription(participant,
            DDS_PARTICIPANT_BUILTIN_TOPIC_NAME);

    if (topic == NULL)
    {
        goto done;
    }

    if (DDS_RETCODE_OK != DDS_DomainParticipant_delete_topic(participant,DDS_Topic_narrow(topic)))
    {
        goto done;
    }

    (void)DDS_DomainParticipant_unregister_type(
                                participant,
                                DDS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME);

    retcode = DDS_RETCODE_OK;

done:
    return retcode;
}


/* writer-related activities */
/*i dref_Discovery_Plugin_AfterLocalDataWriterEnabledCallback
 */
DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_data_writer_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_DataWriter *const data_writer,
        const struct DDS_DataWriterQos *const qos)
{
    /* Do nothing */
    return DDS_RETCODE_OK;
}

/*i dref_Discovery_Plugin_AfterLocalDataWriterDeletedCallback
 */
DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_data_writer_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        const struct DDS_BuiltinTopicKey_t *const local_datawriter_key)
{
    /* Do nothing */
    return DDS_RETCODE_OK;
}

/* reader-related activities */
/*i dref_Discovery_Plugin_AfterLocalDataReaderEnabledCallback
 */
DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_data_reader_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_DataReader *const data_reader,
        const struct DDS_DataReaderQos *const qos)
{
    /* Do nothing */
    return DDS_RETCODE_OK;
}

/*i dref_Discovery_Plugin_AfterLocalDataReaderDeletedCallback
 */
DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_data_reader_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant* const participant,
        struct DDS_BuiltinTopicKey_t *const local_datareader_key)
{
    /* Do nothing */
    return DDS_RETCODE_OK;
}

void
DPSE_Plugin_remove_key_from_list(
                            struct DPSE_DiscoveryPlugin *disc_plugin,
                            DDS_BuiltinTopicKey_t *key)
{
    DDS_Long i, j = 0;
    DDS_BuiltinTopicKey_t initial_key = DDS_BuiltinTopicKey_t_INITIALIZER;

    for (i = 0; i < disc_plugin->key_list_length; i++)
    {
        if (DDS_BuiltinTopicKey_equals(&disc_plugin->key_list[i], key))
        {

            for (j = i; j < disc_plugin->key_list_length; j++)
            {
                /* Shift all non-empty fields left */
                if (i < disc_plugin->key_list_length - 1)
                {
                    disc_plugin->key_list[j] = disc_plugin->key_list[j + 1];
                }
                else
                {
                    /* Last field is now empty */
                    disc_plugin->key_list[j] = initial_key;
                }
            }
        }
    }

    disc_plugin->key_list_length--;
}

/*i \dref_Discovery_Plugin_on_remote_participant_liveliness_expired
 */
DDS_ReturnCode_t
DPSE_Plugin_after_participant_liveliness_expired(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const remote_participant_data)
{
    /* Remove from the array of remote participants and reorder */
    DPSE_Plugin_remove_key_from_list(
            (struct DPSE_DiscoveryPlugin*)discovery_plugin,
            &remote_participant_data->key);

    /* Reset with the core */
    return NDDS_DomainParticipant_reset_remote_participant(
                                    participant,&remote_participant_data->key);
}

void
DPSE_DiscoveryPlugin_delete(struct DPSE_DiscoveryPlugin *plugin)
{
    OSAPI_Heap_freeArray(plugin->key_list);
    plugin->key_list_max = 0;
    plugin->key_list_length = 0;

    DPSE_DiscoveryPluginProperty_finalize(&plugin->properties);

    OSAPI_Heap_freeStructure(plugin);
}

RTI_PRIVATE struct NDDS_DiscoveryI DPSE_DiscoveryPlugin_g_intf;

struct DPSE_DiscoveryPlugin*
DPSE_DiscoveryPlugin_create(struct DPSE_Discovery_Factory *factory,
                        const struct NDDS_Discovery_Property *const property,
                        const struct NDDS_Discovery_Listener *const listener)
{
    /* Variable declarations */
    struct DPSE_DiscoveryPlugin *new_plugin = NULL;
    DDS_Boolean error = DDS_BOOLEAN_TRUE;
    struct DPSE_DiscoveryPluginProperty *plugin_properties =
        &factory->property;

    if (property == NULL)
    {
        DPSE_Log_error(LOG_BAD_THIRD_PARAMETER_IS_NULL);
        goto finally;
    }

    /* --- Create the plugin --- */
    OSAPI_Heap_allocateStructure(&new_plugin, struct DPSE_DiscoveryPlugin);
    if (NULL == new_plugin)
    {
        /*LOG_printError("discovery plugin creation failure"); */
        DPSE_Log_error(LOG_FAIL_ALLOCATE);
        goto finally;
    }

    RT_Component_initialize(&new_plugin->_parent._parent,
                           &DPSE_DiscoveryPlugin_g_intf._parent,
                           0,
                           (property ? &property->_parent : NULL),
                           (listener ? &listener->_parent : NULL));

    /* --- Set properties --- */
    DPSE_DiscoveryPluginProperty_copy(&new_plugin->properties,
                                            plugin_properties);

    /* --- Fill in NtpTime structures --- */
    /* Fill in NtpTime structures with the periods set by the user in properties
     */
    OSAPI_NtpTime_packFromMillisec
       (&new_plugin->participant_liveliness_assert_period,
       new_plugin->properties.participant_liveliness_assert_period.sec,
       new_plugin->properties.participant_liveliness_assert_period.nanosec);

    OSAPI_NtpTime_packFromMillisec
       (&new_plugin->initial_participant_announcement_period,
       new_plugin->properties.initial_participant_announcement_period.sec,
       new_plugin->properties.initial_participant_announcement_period.nanosec);

    error = DDS_BOOLEAN_FALSE;

finally:
    if (DDS_BOOLEAN_TRUE == error)
    {
        DPSE_Log_error(LOG_FAIL_CREATE_DPSE_DISCOVERY_PLUGIN);
        if (NULL == new_plugin)
        {
            return NULL;
        }

        OSAPI_Heap_freeStructure(new_plugin);
        new_plugin = NULL;
    }

    return new_plugin;
}

RTI_PRIVATE struct NDDS_DiscoveryI DPSE_DiscoveryPlugin_g_intf =
{
    {DEFAULT_COMPONENT_RUN},
    DPSE_Plugin_after_participant_liveliness_expired,
    DPSE_DiscoveryPlugin_before_local_participant_created,
    DPSE_DiscoveryPlugin_after_local_participant_created,
    DPSE_DiscoveryPlugin_after_local_participant_enabled,
    DPSE_DiscoveryPlugin_before_local_participant_deleted,
    DPSE_DiscoveryPlugin_after_local_data_writer_enabled,
    DPSE_DiscoveryPlugin_after_local_data_writer_deleted,
    DPSE_DiscoveryPlugin_after_local_data_reader_enabled,
    DPSE_DiscoveryPlugin_after_local_data_reader_deleted,
    NULL
};

/* ------------------------------------------------------------------------ */
/*                   Plugin factory                                         */
/* ------------------------------------------------------------------------ */
RTI_PRIVATE void
DPSE_DiscoveryFactory_delete_component(struct RT_ComponentFactory *factory,
                                             RT_Component_T *component)
{
    struct DPSE_DiscoveryPlugin *self =
                            (struct DPSE_DiscoveryPlugin *)component;

    DPSE_DiscoveryPlugin_delete(self);
}

RTI_PRIVATE RT_Component_T*
DPSE_DiscoveryFactory_create_component(struct RT_ComponentFactory *factory,
                                        struct RT_ComponentProperty *property,
                                        struct RT_ComponentListener *listener)
{
    struct DPSE_DiscoveryPlugin *retval = NULL;

    retval = DPSE_DiscoveryPlugin_create(
                    (struct DPSE_Discovery_Factory*)factory,
                    (const struct NDDS_Discovery_Property *const)property,
                    (const struct NDDS_Discovery_Listener *const)listener);

    return &retval->_parent._parent;
}

RTI_PRIVATE struct RT_ComponentFactory*
DPSE_DiscoveryFactory_initialize(struct RT_ComponentFactoryProperty *property,
                                  struct RT_ComponentFactoryListener *listener);

RTI_PRIVATE void
DPSE_DiscoveryFactory_finalize(struct RT_ComponentFactory *factory,
                                struct RT_ComponentFactoryProperty **property,
                                struct RT_ComponentFactoryListener **listener);

RTI_PRIVATE
struct RT_ComponentFactoryI DPSE_Discovery_Factory_g_intf =
{
    RTI_DPSEDISCOVERY_INTERFACE_ID,
    DPSE_DiscoveryFactory_initialize,
    DPSE_DiscoveryFactory_finalize,
    DPSE_DiscoveryFactory_create_component,
    DPSE_DiscoveryFactory_delete_component
};

RTI_PRIVATE
struct DPSE_Discovery_Factory DPSE_Discovery_Factory_g_factory =
{
    {
            &DPSE_Discovery_Factory_g_intf,
            NULL
    },
    DPSE_DiscoveryPluginProperty_INITIALIZER
};

RTI_PRIVATE struct RT_ComponentFactory*
DPSE_DiscoveryFactory_initialize(struct RT_ComponentFactoryProperty *property,
                                  struct RT_ComponentFactoryListener *listener)
{
    DPSE_Discovery_Factory_g_factory._parent._factory =
        &DPSE_Discovery_Factory_g_factory._parent;

    DPSE_DiscoveryPluginProperty_copy
        (&DPSE_Discovery_Factory_g_factory.property,
         (struct DPSE_DiscoveryPluginProperty *)property);

    return &DPSE_Discovery_Factory_g_factory._parent;
}

RTI_PRIVATE void
DPSE_DiscoveryFactory_finalize(struct RT_ComponentFactory *factory,
                                struct RT_ComponentFactoryProperty **property,
                                struct RT_ComponentFactoryListener **listener)
{

}

struct RT_ComponentFactoryI*
DPSE_DiscoveryFactory_get_interface(void)
{
    return &DPSE_Discovery_Factory_g_intf;
}
