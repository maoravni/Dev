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
#ifndef disc_dpde_discovery_plugin_h
#include "disc_dpde/disc_dpde_discovery_plugin.h"
#endif
#ifndef disc_dpde_log_h
#include "disc_dpde/disc_dpde_log.h"
#endif

#include "ParticipantDiscovery.pkg.h"
#include "PublicationDiscovery.pkg.h"
#include "SubscriptionDiscovery.pkg.h"
#include "ParticipantBuiltinTopicDataPlugin.pkg.h"
#include "PublicationBuiltinTopicDataPlugin.pkg.h"
#include "SubscriptionBuiltinTopicDataPlugin.pkg.h"

/* ------------------------------------------------------------------------ */
/*                   DPDE Discovery plugin                                  */
/* ------------------------------------------------------------------------ */

struct DPDE_DiscoveryPlugin
{
    /* --- Parent plugin --- */
    struct NDDS_Discovery_Plugin _parent;

    /* --- Builtin --- */
    DDS_Publisher *publisher;
    DDS_DataWriter *participant_writer;
    DDS_DataWriter *publication_writer;
    DDS_DataWriter *subscription_writer;

    DDS_Subscriber *subscriber;
    DDS_DataReader *participant_reader;
    DDS_DataReader *publication_reader;
    DDS_DataReader *subscription_reader;

    /*DDS_InstanceHandle_t *instance_handle; */

    struct OSAPI_NtpTime participant_liveliness_assert_period;
    struct OSAPI_NtpTime initial_participant_announcement_period;

    struct DPDE_DiscoveryPluginProperty properties;

    /* The announcement event, which may get updated as we receive announcements
     * from remote participants.
     */
    OSAPITimeoutHandle_t announcement_event;

    /* This is owned by the participant, not the plugin.  A pointer is
     * stored for convenience
     */
    struct DDS_ParticipantBuiltinTopicData *participant_builtin_data;

    struct DDS_PublicationBuiltinTopicData *publication_builtin_data;

    struct DDS_SubscriptionBuiltinTopicData *subscription_builtin_data;

    struct NDDS_Type_Plugin *pub_type_plugin;
    struct NDDS_Type_Plugin *sub_type_plugin;

    RTI_BOOL timer_created;
};

struct DPDE_DiscoveryFactory
{
    struct RT_ComponentFactory _parent;
    struct DPDE_DiscoveryPluginProperty property;
};

/*** SOURCE_BEGIN ***/

/* Copy a DPDE_DiscoveryPluginProperty structure.
 * 
 * This will allocate memory for a peers list and multicast receive address
 * if the destination has not allocated those arrays.
 * 
 * pre: dst and src must not be NULL
 * post: the fields of dst will equal the fields of src.
 * return: 
 *     DDS_RETCODE_OK if no errors
 *     DDS_RETCODE_ERROR if there is a problem copying
 */
RTI_PRIVATE DDS_ReturnCode_t
DPDE_DiscoveryPluginProperty_copy(struct DPDE_DiscoveryPluginProperty *dst,
                                  struct DPDE_DiscoveryPluginProperty *src)
{
    if (dst == NULL || src == NULL)
    {
        return DDS_RETCODE_ERROR;
    }

    *dst = *src;

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
DPDE_DiscoveryPluginProperty_finalize(struct DPDE_DiscoveryPluginProperty *dst)
{
    return DDS_RETCODE_OK;
}

DDS_Boolean
DPDE_DiscoveryPlugin_add_peer(struct DPDE_DiscoveryPlugin *discovery_plugin,
                              DDS_DomainParticipant *const participant,
                              struct DDS_DomainParticipantQos *participant_qos,
                              const char *peer_addr)
{
    DDS_Long i;
    DDS_Boolean isValidAddress = DDS_BOOLEAN_TRUE;
    struct NETIO_Address address = NETIO_Address_INITIALIZER;
    struct NETIO_Address dst_reader = NETIO_Address_INITIALIZER;

    NETIO_Address_init(&dst_reader,NETIO_ADDRESS_KIND_GUID);
    dst_reader.value.rtps_guid.object_id =
                        NETIO_htonl(MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT);

    isValidAddress = NETIO_Address_ipv4_string_to_address(
                                        &address,peer_addr);

    /* Check if the address was valid */
    if (!isValidAddress)
    {
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
        DDS_DataWriter_add_anonymous_route(
                discovery_plugin->participant_writer,
                &dst_reader,
                &address);
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
            DDS_DataWriter_add_anonymous_route(
                    discovery_plugin->participant_writer,
                    &dst_reader,
                    &address);
        }
    }

    return DDS_BOOLEAN_TRUE;
}

RTI_PRIVATE DDS_ReturnCode_t
DPDE_DiscoveryPlugin_before_local_participant_created(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant* const participant,
        struct DDS_DomainParticipantQos *dp_qos,
        struct DDS_ParticipantBuiltinTopicData *participant_data_out)
{
    dp_qos->resource_limits.local_writer_allocation =
        dp_qos->resource_limits.local_writer_allocation + 3;
    dp_qos->resource_limits.local_reader_allocation =
            dp_qos->resource_limits.local_reader_allocation + 3;
    dp_qos->resource_limits.local_publisher_allocation =
        dp_qos->resource_limits.local_publisher_allocation + 1;
    dp_qos->resource_limits.local_subscriber_allocation =
        dp_qos->resource_limits.local_subscriber_allocation + 1;
    dp_qos->resource_limits.local_topic_allocation =
        dp_qos->resource_limits.local_topic_allocation + 3;
    dp_qos->resource_limits.local_type_allocation =
        dp_qos->resource_limits.local_type_allocation + 3;
    dp_qos->resource_limits.remote_writer_allocation +=
      3 * dp_qos->resource_limits.remote_participant_allocation;
    dp_qos->resource_limits.remote_reader_allocation +=
      3 * dp_qos->resource_limits.remote_participant_allocation;

    participant_data_out->dds_builtin_endpoints =
                                DDS_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER |
                                DDS_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR |
                                DDS_BUILTIN_ENDPOINT_PUBLICATION_ANNOUNCER |
                                DDS_BUILTIN_ENDPOINT_PUBLICATION_DETECTOR |
                                DDS_BUILTIN_ENDPOINT_SUBSCRIPTION_ANNOUNCER |
                                DDS_BUILTIN_ENDPOINT_SUBSCRIPTION_DETECTOR;

    return DDS_RETCODE_OK;
}

RTI_PRIVATE DDS_ReturnCode_t
DPDE_DiscoveryPlugin_after_local_participant_created(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const
        local_participant_data)
{
    struct DPDE_DiscoveryPlugin *const disc_plugin =
                        (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    struct DDS_PublisherQos publisher_qos = DDS_PublisherQos_INITIALIZER;
    struct DDS_SubscriberQos subscriber_qos = DDS_SubscriberQos_INITIALIZER;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    disc_plugin->participant_builtin_data = local_participant_data;

    publisher_qos.management.is_hidden = DDS_BOOLEAN_TRUE;
    disc_plugin->publisher = DDS_DomainParticipant_create_publisher(
                        participant,&publisher_qos,NULL,DDS_STATUS_MASK_NONE);
    if (disc_plugin->publisher == NULL)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_PUBLISHER);
        goto finally;
    }

    subscriber_qos.management.is_hidden = DDS_BOOLEAN_TRUE;
    disc_plugin->subscriber = DDS_DomainParticipant_create_subscriber(
                        participant,&subscriber_qos,NULL,DDS_STATUS_MASK_NONE);

    if (disc_plugin->subscriber == NULL)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_SUBSCRIBER);
        goto finally;
    }

    retcode = DPDE_ParticipantDiscovery_after_local_participant_created(
                    discovery_plugin, participant, local_participant_data);

    if (DDS_RETCODE_OK != retcode)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY);
        goto finally;
    }

    retcode = DPDE_PublicationDiscovery_after_local_participant_created(
                    discovery_plugin, participant, local_participant_data);

    if (DDS_RETCODE_OK != retcode)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_PUBLICATION_DISCOVERY);
        goto finally;
    }

    retcode = DPDE_SubscriptionDiscovery_after_local_participant_created(
                    discovery_plugin, participant, local_participant_data);

    if (DDS_RETCODE_OK != retcode)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_SUBSCRIPTION_DISCOVERY);
        goto finally;
    }

    DPDE_PublicationBuiltinTopicDataTypePlugin_create_sample(
                        disc_plugin->pub_type_plugin,
                        (void **)&disc_plugin->publication_builtin_data, NULL);

    if (disc_plugin->publication_builtin_data == NULL)
    {
        goto finally;
    }

    DPDE_SubscriptionBuiltinTopicDataTypePlugin_create_sample(
                        disc_plugin->sub_type_plugin,
                        (void **)&disc_plugin->subscription_builtin_data, NULL);

    if (disc_plugin->subscription_builtin_data == NULL)
    {
        goto finally;
    }

    retcode = DDS_RETCODE_OK;

finally:

    if (DDS_RETCODE_OK != retcode)
    {
        DPDE_Log_error(LOG_FAIL_INITIALIZE_DISCOVERY_PLUGIN);
    }

    return retcode;
}

/*i dref_Discovery_Plugin_AfterLocalParticipantEnabledCallback
 */
RTI_PRIVATE DDS_ReturnCode_t
DPDE_DiscoveryPlugin_after_local_participant_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const loc_dp_data)
{
    DDS_ReturnCode_t retcode;
    struct DPDE_DiscoveryPlugin *const disc_plugin =
                    (struct DPDE_DiscoveryPlugin *)discovery_plugin;

    DDS_Entity_enable(DDS_Publisher_as_entity(disc_plugin->publisher));
    DDS_Entity_enable(DDS_Subscriber_as_entity(disc_plugin->subscriber));

    retcode = DPDE_ParticipantDiscovery_schedule_fast_assertions(
                discovery_plugin,participant,loc_dp_data,DDS_BOOLEAN_TRUE);

    return retcode;
}

RTI_PRIVATE DDS_ReturnCode_t
DPDE_DiscoveryPlugin_shutdown(struct DPDE_DiscoveryPlugin *dpde_plugin,
                              DDS_DomainParticipant *participant)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    DDS_Topic *topic = NULL;
    DDS_TopicDescription *topic_desc = NULL;

    OSAPI_Timer_delete_timeout(
            DDS_DomainParticipant_get_timer(
                 DDS_Subscriber_get_participant(dpde_plugin->subscriber)),
                 &dpde_plugin->announcement_event);

    /* Cannot use delete contained entities */
    if (NULL != dpde_plugin->publisher)
    {
        retcode = DDS_Publisher_delete_datawriter(dpde_plugin->publisher,
                                        dpde_plugin->participant_writer);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
        dpde_plugin->participant_writer = NULL;

        retcode = DDS_Publisher_delete_datawriter(dpde_plugin->publisher,
                                        dpde_plugin->publication_writer);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
        dpde_plugin->publication_writer = NULL;

        retcode = DDS_Publisher_delete_datawriter(dpde_plugin->publisher,
                                        dpde_plugin->subscription_writer);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
        dpde_plugin->subscription_writer = NULL;

        retcode = DDS_DomainParticipant_delete_publisher(participant,
                                        dpde_plugin->publisher);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
        dpde_plugin->publisher = NULL;
    }

    if (NULL != dpde_plugin->subscriber)
    {
        retcode = DDS_Subscriber_delete_datareader(dpde_plugin->subscriber,
                                        dpde_plugin->participant_reader);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
        dpde_plugin->participant_reader = NULL;

        retcode = DDS_Subscriber_delete_datareader(dpde_plugin->subscriber,
                                        dpde_plugin->publication_reader);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
        dpde_plugin->publication_reader = NULL;

        retcode = DDS_Subscriber_delete_datareader(dpde_plugin->subscriber,
                                        dpde_plugin->subscription_reader);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
        dpde_plugin->subscription_reader = NULL;

        retcode = DDS_DomainParticipant_delete_subscriber(participant,
                                        dpde_plugin->subscriber);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
        dpde_plugin->subscriber = NULL;
    }

    topic_desc = DDS_DomainParticipant_lookup_topicdescription(
                            participant,DDS_SUBSCRIPTION_BUILTIN_TOPIC_NAME);
    if (topic_desc != NULL)
    {
        topic = DDS_Topic_narrow(topic_desc);
        retcode = DDS_DomainParticipant_delete_topic(participant, topic);
        if (retcode != DDS_RETCODE_OK)
        {
            DPDE_Log_error(LOG_FAIL_DELETE_SUBSCRIPTION_BUILTIN_TOPIC);
        }
    }

    topic_desc = DDS_DomainParticipant_lookup_topicdescription(
                            participant,DDS_PUBLICATION_BUILTIN_TOPIC_NAME);
    if (topic_desc != NULL)
    {
        topic = DDS_Topic_narrow(topic_desc);
        retcode = DDS_DomainParticipant_delete_topic(participant, topic);
        if (retcode != DDS_RETCODE_OK)
        {
            DPDE_Log_error(LOG_FAIL_DELETE_PUBLICATION_BUILTIN_TOPIC);
        }
    }

    topic_desc = DDS_DomainParticipant_lookup_topicdescription(
                            participant,DDS_PARTICIPANT_BUILTIN_TOPIC_NAME);
    if (topic_desc != NULL)
    {
        topic = DDS_Topic_narrow(topic_desc);
        retcode = DDS_DomainParticipant_delete_topic(participant, topic);
        if (retcode != DDS_RETCODE_OK)
        {
            DPDE_Log_error(LOG_FAIL_DELETE_PARTICIPANT_BUILTIN_TOPIC);
        }
    }

    if (dpde_plugin->publication_builtin_data != NULL)
    {
        DPDE_PublicationBuiltinTopicDataTypePlugin_delete_sample(
                dpde_plugin->pub_type_plugin,
                dpde_plugin->publication_builtin_data, NULL);
        dpde_plugin->publication_builtin_data = NULL;
    }

    if (dpde_plugin->subscription_builtin_data != NULL)
    {
        DPDE_SubscriptionBuiltinTopicDataTypePlugin_delete_sample(
                dpde_plugin->sub_type_plugin,
                dpde_plugin->subscription_builtin_data, NULL);
        dpde_plugin->subscription_builtin_data = NULL;
    }

    dpde_plugin->pub_type_plugin = NULL;
    dpde_plugin->sub_type_plugin = NULL;

    DPDE_DiscoveryPluginProperty_finalize(&dpde_plugin->properties);

    DDS_DomainParticipant_unregister_type(participant,
                                DDS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME);

    DDS_DomainParticipant_unregister_type(participant,
                                DDS_PUBLICATION_BUILTIN_TOPIC_TYPE_NAME);

    DDS_DomainParticipant_unregister_type(participant,
                                DDS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME);

    retcode = DDS_RETCODE_OK;

done:
    return retcode;
}

/*i dref_Discovery_Plugin_BeforeLocalParticipantDeletedCallback
 */
RTI_PRIVATE DDS_ReturnCode_t
DPDE_DiscoveryPlugin_before_local_participant_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant * const participant,
        struct DDS_BuiltinTopicKey_t *const local_participant_key)
{
    struct DPDE_DiscoveryPlugin *const DPDE_plugin =
        (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    DDS_InstanceHandle_t ih;

    ih = DDS_Entity_get_instance_handle(DDS_DomainParticipant_as_entity
                                        (participant));
    ih.is_valid = DDS_BOOLEAN_TRUE;
    retcode =
        DDS_DataWriter_dispose(DPDE_plugin->participant_writer, NULL, &ih);

    if (retcode != DDS_RETCODE_OK)
    {
        /*LOG_printError("dispose participant failed"); */
        DPDE_Log_error(LOG_FAIL_DISPOSE_PARTICIPANT);
        goto done;
    }

    retcode = DPDE_DiscoveryPlugin_shutdown(
            (struct DPDE_DiscoveryPlugin*)discovery_plugin,
            participant);

    done:

    return retcode;
}

/* writer-related activities */
/*i dref_Discovery_Plugin_AfterLocalDataWriterEnabledCallback
 */
RTI_PRIVATE DDS_ReturnCode_t
DPDE_DiscoveryPlugin_after_local_data_writer_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_DataWriter *const data_writer,
        const struct DDS_DataWriterQos *const qos)
{
    return DPDE_PublicationDiscovery_after_local_data_writer_enabled(
                            discovery_plugin, participant, data_writer, qos);
}

/*i dref_Discovery_Plugin_AfterLocalDataWriterDeletedCallback
 */
RTI_PRIVATE  DDS_ReturnCode_t
DPDE_DiscoveryPlugin_after_local_data_writer_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        const struct DDS_BuiltinTopicKey_t *const local_datawriter_key)
{
    return DPDE_PublicationDiscovery_after_local_data_writer_deleted(
                         discovery_plugin, participant, local_datawriter_key);
}

/* reader-related activities */
/*i dref_Discovery_Plugin_AfterLocalDataReaderEnabledCallback
 */
RTI_PRIVATE  DDS_ReturnCode_t
DPDE_DiscoveryPlugin_after_local_data_reader_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_DataReader *const data_reader,
        const struct DDS_DataReaderQos *const qos)
{
    return DPDE_SubscriptionDiscovery_after_local_data_reader_enabled(
                        discovery_plugin, participant, data_reader, qos);
}

/*i dref_Discovery_Plugin_AfterLocalDataReaderDeletedCallback
 */
RTI_PRIVATE  DDS_ReturnCode_t
DPDE_DiscoveryPlugin_after_local_data_reader_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant* const participant,
        struct DDS_BuiltinTopicKey_t *const local_datareader_key)
{
    return DPDE_SubscriptionDiscovery_after_local_data_reader_deleted(
                        discovery_plugin, participant, local_datareader_key);
}

/*i \dref_Discovery_Plugin_on_remote_participant_liveliness_expired
 */
RTI_PRIVATE  DDS_ReturnCode_t
DPDE_DiscoveryPlugin_after_participant_liveliness_expired(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const remote_participant_data)
{
    struct DPDE_DiscoveryPlugin *const disc_plugin =
                        (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    DDS_ReturnCode_t retcode;
    DDS_InstanceHandle_t publication_handle;
    DDS_BuiltinTopicKey_t key;

    /* Reset with the core */

    key = remote_participant_data->key;

    key.value[3] = MIG_RTPS_OBJECT_ID_WRITER_SDP_PUBLICATION;
    DDS_InstanceHandle_from_rtps(&publication_handle,
                                 (struct MIGRtpsGuid *)&key);
    retcode = DDS_DataReader_liveliness_lost(disc_plugin->publication_reader,
                                             &publication_handle);
    if (retcode != DDS_RETCODE_OK)
    {
        goto done;
    }

    key.value[3] = MIG_RTPS_OBJECT_ID_WRITER_SDP_SUBSCRIPTION;
    DDS_InstanceHandle_from_rtps(&publication_handle,
                                 (struct MIGRtpsGuid *)&key);
    retcode = DDS_DataReader_liveliness_lost(disc_plugin->subscription_reader,
                                             &publication_handle);
    if (retcode != DDS_RETCODE_OK)
    {
        goto done;
    }

    key.value[3] = MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT;
    DDS_InstanceHandle_from_rtps(&publication_handle,
                                 (struct MIGRtpsGuid *)&key);
    retcode = DDS_DataReader_liveliness_lost(disc_plugin->participant_reader,
                                             &publication_handle);
    if (retcode != DDS_RETCODE_OK)
    {
        goto done;
    }

    retcode = NDDS_DomainParticipant_remove_remote_participant(
                                   participant,&remote_participant_data->key);

done:
    return retcode;
}

/**************************** COMPONENT INTERFACE *****************************/

RTI_PRIVATE struct NDDS_DiscoveryI DPDE_DiscoveryPlugin_g_intf;

RTI_PRIVATE void
DPDE_DiscoveryPlugin_delete(struct DPDE_DiscoveryPlugin *plugin)
{
    OSAPI_Heap_freeStructure(plugin);
}

RTI_PRIVATE struct DPDE_DiscoveryPlugin *
DPDE_DiscoveryPlugin_create(
        struct DPDE_DiscoveryFactory *factory,
        const struct NDDS_Discovery_Property *const property,
        const struct NDDS_Discovery_Listener *const listener)
{
    /* Variable declarations */
    struct DPDE_DiscoveryPlugin *new_plugin = NULL;
    DDS_Boolean error = DDS_BOOLEAN_TRUE;
    struct DPDE_DiscoveryPluginProperty *plugin_properties =
        &factory->property;

    /* --- Create the plugin --- */
    OSAPI_Heap_allocateStructure(&new_plugin, struct DPDE_DiscoveryPlugin);

    if (NULL == new_plugin)
    {
        DPDE_Log_error(LOG_FAIL_ALLOCATE);
        goto finally;
    }

    new_plugin->timer_created = RTI_FALSE;

    RT_Component_initialize(&new_plugin->_parent._parent,
                           &DPDE_DiscoveryPlugin_g_intf._parent,
                           0,
                           (property ? &property->_parent : NULL),
                           (listener ? &listener->_parent : NULL));

    new_plugin->participant_builtin_data = NULL;
    new_plugin->participant_reader = NULL;
    new_plugin->participant_writer = NULL;
    new_plugin->pub_type_plugin = NULL;
    new_plugin->publication_builtin_data = NULL;
    new_plugin->publication_reader = NULL;
    new_plugin->publication_writer = NULL;
    new_plugin->publisher = NULL;
    new_plugin->sub_type_plugin = NULL;
    new_plugin->subscriber = NULL;
    new_plugin->subscription_reader = NULL;
    new_plugin->subscription_writer = NULL;
    new_plugin->subscription_builtin_data = NULL;

    /* --- Set properties --- */
    DPDE_DiscoveryPluginProperty_copy(&new_plugin->properties,
                                            plugin_properties);

    /* --- Fill in NtpTime structures --- */
    /* Fill in NtpTime structures with the periods set by the user in properties
     */
    OSAPI_NtpTime_packFromMillisec(
       &new_plugin->participant_liveliness_assert_period,
       new_plugin->properties.participant_liveliness_assert_period.sec,
       new_plugin->properties.participant_liveliness_assert_period.nanosec);

    OSAPI_NtpTime_packFromMillisec(
      &new_plugin->initial_participant_announcement_period,
      new_plugin->properties.initial_participant_announcement_period.sec,
      new_plugin->properties.initial_participant_announcement_period.nanosec);

    error = DDS_BOOLEAN_FALSE;

finally:

    if (DDS_BOOLEAN_TRUE == error)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_DPDE_DISCOVERY_PLUGIN);
        if (NULL == new_plugin)
        {
            return NULL;
        }

        OSAPI_Heap_freeStructure(new_plugin);

        new_plugin = NULL;
    }

    return new_plugin;
}

RTI_PRIVATE struct NDDS_DiscoveryI DPDE_DiscoveryPlugin_g_intf =
{
    {DEFAULT_COMPONENT_RUN},
    DPDE_DiscoveryPlugin_after_participant_liveliness_expired,
    DPDE_DiscoveryPlugin_before_local_participant_created,
    DPDE_DiscoveryPlugin_after_local_participant_created,
    DPDE_DiscoveryPlugin_after_local_participant_enabled,
    DPDE_DiscoveryPlugin_before_local_participant_deleted,
    DPDE_DiscoveryPlugin_after_local_data_writer_enabled,
    DPDE_DiscoveryPlugin_after_local_data_writer_deleted,
    DPDE_DiscoveryPlugin_after_local_data_reader_enabled,
    DPDE_DiscoveryPlugin_after_local_data_reader_deleted,
    NULL
};

/* ------------------------------------------------------------------------ */
/*                   Plugin factory                                         */
/* ------------------------------------------------------------------------ */
DDS_ReturnCode_t
DPDE_DiscoveryProperty_finalize(struct DPDE_DiscoveryPluginProperty *properties)
{
    return DDS_RETCODE_OK;
}

RTI_PRIVATE RT_Component_T*
DPDE_DiscoveryFactory_create_component(struct RT_ComponentFactory *factory,
                                       struct RT_ComponentProperty *property,
                                       struct RT_ComponentListener *listener)
{
    struct DPDE_DiscoveryPlugin *retval = NULL;

    retval = DPDE_DiscoveryPlugin_create(
                        (struct DPDE_DiscoveryFactory*)factory,
                        (const struct NDDS_Discovery_Property *const)property,
                        (const struct NDDS_Discovery_Listener *const)listener);

    return &retval->_parent._parent;
}

RTI_PRIVATE void
DPDE_DiscoveryFactory_delete_component(struct RT_ComponentFactory *factory,
                                       RT_Component_T *component)
{
    struct DPDE_DiscoveryPlugin *self = (struct DPDE_DiscoveryPlugin*)component;

    DPDE_DiscoveryPlugin_delete(self);
}

RTI_PRIVATE struct RT_ComponentFactory*
DPDE_DiscoveryFactory_initialize(struct RT_ComponentFactoryProperty*property,
                                 struct RT_ComponentFactoryListener *listener);

RTI_PRIVATE void
DPDE_DiscoveryFactory_finalize(struct RT_ComponentFactory *factory,
                               struct RT_ComponentFactoryProperty **property,
                               struct RT_ComponentFactoryListener **listener);

RTI_PRIVATE struct RT_ComponentFactoryI DPDE_DiscoveryFactory_g_intf =
{
    DPDE_DISCOVERY_INTERFACE_ID,
    DPDE_DiscoveryFactory_initialize,
    DPDE_DiscoveryFactory_finalize,
    DPDE_DiscoveryFactory_create_component,
    DPDE_DiscoveryFactory_delete_component
};

RTI_PRIVATE
struct DPDE_DiscoveryFactory DPDE_DiscoveryFactory_g_factory =
{
  {
     &DPDE_DiscoveryFactory_g_intf,
     NULL
  },
  DPDE_DiscoveryPluginProperty_INITIALIZER
};

RTI_PRIVATE struct RT_ComponentFactory*
DPDE_DiscoveryFactory_initialize(struct RT_ComponentFactoryProperty *property,
                                 struct RT_ComponentFactoryListener *listener)
{
    DPDE_DiscoveryFactory_g_factory._parent._factory =
        &DPDE_DiscoveryFactory_g_factory._parent;

    DPDE_DiscoveryPluginProperty_copy(
            &DPDE_DiscoveryFactory_g_factory.property,
            (struct DPDE_DiscoveryPluginProperty *)property);

    return &DPDE_DiscoveryFactory_g_factory._parent;
}

RTI_PRIVATE void
DPDE_DiscoveryFactory_finalize(struct RT_ComponentFactory *factory,
                               struct RT_ComponentFactoryProperty **property,
                               struct RT_ComponentFactoryListener **listener)
{
}

struct RT_ComponentFactoryI*
DPDE_DiscoveryFactory_get_interface(void)
{
    return &DPDE_DiscoveryFactory_g_intf;
}
