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
#ifndef osapi_timer_h
#include "osapi/osapi_timer.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif
#ifndef disc_dpse_dpsediscovery_h
#include "disc_dpde/disc_dpde_discovery_plugin.h"
#endif
#ifndef disc_dpde_log_h
#include "disc_dpde/disc_dpde_log.h"
#endif

#include "BuiltinCdr.pkg.h"
#include "DiscoveryPlugin.pkg.h"
#include "ParticipantBuiltinTopicDataPlugin.pkg.h"
#include "ParticipantListener.pkg.h"
#include "PublicationDiscovery.pkg.h"
#include "SubscriptionDiscovery.pkg.h"

/*** SOURCE_BEGIN ***/

/* Callback called when it is time to assert the local participant.
 *
 * The core participant does not know anything about how and where it is
 * announced. It is up to the plugin to handle this based on the confguration
 * of the discovery plugin
 */
RTI_PRIVATE OSAPITimeoutOp_t
DPDE_ParticipantDiscovery_assert_participant(struct OSAPITimeoutUserData *tdata)
{
    struct DPDE_DiscoveryPlugin *dpde_plugin =
                                (struct DPDE_DiscoveryPlugin *)tdata->field[0];
    void *local_participant_data = tdata->field[1];

    DPDE_LOG_DEBUG(LOG_ANNOUNCE_SELF_TO_REMOTE_PARTICIPANT);

    if (DDS_RETCODE_OK != DDS_DataWriter_write(dpde_plugin->participant_writer,
                                               local_participant_data,
                                               &DDS_HANDLE_NIL))
    {
        DPDE_Log_error(LOG_FAIL_ANNOUNCE_SELF_TO_REMOTE_PARTICIPANT);
    }

    /* If we are sending the initial three, we use the initial announcement
     * period to reschedule the event, otherwise use the regular announcement
     * interval
     */
    if (tdata->count[0] > 0)
    {
        tdata->count[0]--;
        return OSAPI_TIMEOUT_OP_AUTOMATIC;
    }

    if (RTI_TRUE != OSAPI_Timer_update_timeout(
            DDS_DomainParticipant_get_timer(
                    DDS_Subscriber_get_participant(dpde_plugin->subscriber)),
                    &dpde_plugin->announcement_event,
                    DDS_Duration_to_ms(dpde_plugin->properties.participant_liveliness_assert_period)))
    {
        DPDE_Log_error(LOG_FAIL_UPDATE_PARTICIPANT_ASSERTION);
    }

    return OSAPI_TIMEOUT_OP_MANUAL;
}

DDS_ReturnCode_t
DPDE_ParticipantDiscovery_schedule_fast_assertions(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *const participant,
        const struct DDS_ParticipantBuiltinTopicData *local_participant_data,
        DDS_Boolean new_event)
{
    struct DPDE_DiscoveryPlugin *const dpde_plugin =
                        (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    struct OSAPITimeoutUserData storage;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    DPDE_LOG_DEBUG(LOG_SCHEDULE_FAST_ASSERTIONS);

    if (new_event)
    {
        /* Advance the sequence number the first time */
        DDS_DataWriter_advance_sn(dpde_plugin->participant_writer);
    }

    /* Send out initial participant announcement */
    DDS_DataWriter_write(dpde_plugin->participant_writer,
                         (void *)local_participant_data, &DDS_HANDLE_NIL);

    storage.count[0] = 0;
    storage.count[1] = 0;
    storage.field[0] = (void *)dpde_plugin;
    storage.field[1] = (void *)local_participant_data;

    /* The number of initial announcements already sent is stored in the 
     * storage.count[0] variable passed to the event.  Since we have already
     * sent one announcement, we schedule sending the rest of the
     * initial announcements.
     */
    storage.count[0] =
            dpde_plugin->properties.initial_participant_announcements - 1;

    if (new_event)
    {
        if (RTI_TRUE !=
            OSAPI_Timer_create_timeout(
              DDS_DomainParticipant_get_timer(
                 DDS_Subscriber_get_participant(dpde_plugin->subscriber)),
                 &dpde_plugin->announcement_event,
                 DDS_Duration_to_ms(dpde_plugin->properties.initial_participant_announcement_period),
                 OSAPI_TIMER_PERIODIC,
                 DPDE_ParticipantDiscovery_assert_participant,
                 &storage))
        {
            DPDE_Log_error(LOG_FAIL_SCHEDULE_FAST_ASSERTIONS);
            goto finally;
        }
        dpde_plugin->timer_created = RTI_TRUE;
    }
    else
    {
        /* Confirm that the announcement has been posted */
        if (dpde_plugin->announcement_event.epoch > 0)
        {
            if (RTI_TRUE !=
                OSAPI_Timer_update_timeout(
                   DDS_DomainParticipant_get_timer(
                      DDS_Subscriber_get_participant(dpde_plugin->subscriber)),
                      &dpde_plugin->announcement_event,
                      DDS_Duration_to_ms(dpde_plugin->properties.participant_liveliness_assert_period)))
            {
                DPDE_Log_error(LOG_FAIL_UPDATE_PARTICIPANT_ASSERTION);
                goto finally;
            }
        }
    }

    retcode = DDS_RETCODE_OK;

    finally:
    return retcode;
}


DDS_ReturnCode_t
DPDE_ParticipantDiscovery_after_local_participant_created(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *local_participant_data)
{
    struct DPDE_DiscoveryPlugin *disc_plugin =
                        (struct DPDE_DiscoveryPlugin *)discovery_plugin;
    struct DDS_DataWriterQos writer_qos = DDS_DataWriterQos_INITIALIZER;
    struct DDS_DataReaderQos reader_qos = DDS_DataReaderQos_INITIALIZER;
    struct NDDS_Type_Plugin *type_plugin;
    struct DDS_DataReaderListener participant_builtin_listener;
    struct DDS_DomainParticipantQos *dp_qos = NULL;
    struct DDS_TopicQos topic_qos = DDS_TopicQos_INITIALIZER;
    DDS_Topic *topic = NULL;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    DDS_Long peer_no,peer_length;
    struct NETIO_Address src_writer = NETIO_Address_INITIALIZER;
    struct NETIO_Address to_address = NETIO_Address_INITIALIZER;
    struct DDS_Locator *a_locator;

    NETIO_Address_init(&src_writer,NETIO_ADDRESS_KIND_GUID);
    src_writer.value.rtps_guid.object_id =
                        NETIO_htonl(MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT);

    if (local_participant_data == NULL)
    {
        DPDE_Log_error(LOG_LOCAL_PARTICIPANT_HAS_INVALID_BUILTIN_DATA);
        goto finally;
    }

    type_plugin = DPDE_ParticipantBuiltinTopicDataTypePlugin_get();
    dp_qos = DDS_DomainParticipant_get_qos_ref(participant);

    if (DDS_RETCODE_OK != DDS_DomainParticipant_register_type(participant,
                                DDS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME,
                                (struct NDDS_Type_Plugin *)type_plugin))
    {
        DPDE_Log_error(LOG_FAIL_REGISTER_PARTICIPANT_BUILTIN_TYPE);
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
        DPDE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_TOPIC);
        goto finally;
    }

    /* Builtin endpoints have well-known object IDs */
    writer_qos.protocol.rtps_object_id =
                                     MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT;
    writer_qos.resource_limits.max_instances = 1;
    writer_qos.resource_limits.max_samples = 1;
    writer_qos.resource_limits.max_samples_per_instance = 1;
    writer_qos.liveliness.kind = DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS;
    writer_qos.liveliness.lease_duration.sec = DDS_DURATION_INFINITE_SEC;
    writer_qos.liveliness.lease_duration.nanosec = DDS_DURATION_INFINITE_NSEC;
    writer_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
    writer_qos.durability.kind = DDS_VOLATILE_DURABILITY_QOS;
    writer_qos.ownership.kind = DDS_SHARED_OWNERSHIP_QOS;
    writer_qos.protocol.rtps_reliable_writer.heartbeats_per_max_samples = 0;
    writer_qos.management.is_anonymous = DDS_BOOLEAN_TRUE;
    writer_qos.management.is_hidden = DDS_BOOLEAN_TRUE;

    disc_plugin->participant_writer = DDS_Publisher_create_datawriter(
           disc_plugin->publisher,topic,&writer_qos,NULL,DDS_STATUS_MASK_NONE);

    if (disc_plugin->participant_writer == NULL)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_WRITER);
        goto finally;
    }

    /* Parse and add the initial peer list to the announcer*/
    peer_length = DDS_StringSeq_get_length(&dp_qos->discovery.initial_peers);
    for (peer_no = 0; peer_no < peer_length; peer_no++)
    {
        DPDE_DiscoveryPlugin_add_peer(disc_plugin,
           participant,
           dp_qos,
           *DDS_StringSeq_get_reference(
                               &dp_qos->discovery.initial_peers,peer_no));
    }

    DPDE_ParticipantBuiltinDataReaderListener_initialize(
            (struct NDDS_Discovery_Plugin *)disc_plugin,
            &participant_builtin_listener);

    reader_qos.protocol.rtps_object_id =
        MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT;

    reader_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
    reader_qos.resource_limits.max_samples =
        dp_qos->resource_limits.remote_participant_allocation + 1;
    reader_qos.resource_limits.max_instances =
        reader_qos.resource_limits.max_samples;
    reader_qos.resource_limits.max_samples_per_instance = 1;
    reader_qos.liveliness.kind = DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS;
    reader_qos.liveliness.lease_duration.sec = DDS_DURATION_INFINITE_SEC;
    reader_qos.liveliness.lease_duration.nanosec = DDS_DURATION_INFINITE_NSEC;
    reader_qos.durability.kind = DDS_VOLATILE_DURABILITY_QOS;
    reader_qos.ownership.kind = DDS_SHARED_OWNERSHIP_QOS;
    reader_qos.reader_resource_limits.max_remote_writers =
        dp_qos->resource_limits.remote_participant_allocation;
    reader_qos.management.is_anonymous = DDS_BOOLEAN_TRUE;
    reader_qos.management.is_hidden = DDS_BOOLEAN_TRUE;

    disc_plugin->participant_reader = DDS_Subscriber_create_datareader(
                                         disc_plugin->subscriber,
                                         DDS_Topic_as_topicdescription(topic),
                                         &reader_qos,
                                         &participant_builtin_listener,
                                         DDS_STATUS_MASK_ALL);

    if (disc_plugin->participant_reader == NULL)
    {
        DPDE_Log_error(LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_READER);
        goto finally;
    }

    peer_length = DDS_LocatorSeq_get_length(
                        &local_participant_data->metatraffic_unicast_locators);
    for (peer_no = 0; peer_no < peer_length; peer_no++)
    {
        a_locator = DDS_LocatorSeq_get_reference(
                &local_participant_data->metatraffic_unicast_locators,peer_no);

        to_address = *(struct NETIO_Address*)a_locator;
        DDS_DataReader_add_anonymous_route(disc_plugin->participant_reader,
                &src_writer,
                &to_address);
    }

    peer_length = DDS_LocatorSeq_get_length(
                        &local_participant_data->metatraffic_multicast_locators);
    for (peer_no = 0; peer_no < peer_length; peer_no++)
    {
        a_locator = DDS_LocatorSeq_get_reference(
                &local_participant_data->metatraffic_multicast_locators,peer_no);

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

    return retcode;
}

void
DPDE_ParticipantDiscovery_assert_remote_participant(
                                struct DPDE_DiscoveryPlugin *dpde_plugin,
                                DDS_DomainParticipant *participant,
                                struct DDS_ParticipantBuiltinTopicData *data,
                                struct DDS_SampleInfo *info)
{
    DDS_Long i,j;
    struct MIGRtpsGuid guid;
    DDS_InstanceHandle_t ih;
    DDS_Boolean is_new;
    struct NETIO_Address dst_address;
    struct NETIO_Address dst_reader;
    struct DDS_Locator *a_locator;
    DDS_ReturnCode_t retcode;

    /* Ignore ourselves
     * TODO: Cleanup the mess that is
     * MIGRtpsGuid, InstanceHandle, DDS_GUID and BuiltinKey
     */
    ih = DDS_Entity_get_instance_handle(
                                DDS_DomainParticipant_as_entity(participant));
    DDS_InstanceHandle_to_rtps(&guid, &ih);
    if (MIGRtpsGuid_equals(&guid,
            (struct MIGRtpsGuid *)&data->key))
    {
        LOG_printDebug("ignore ourselves\n");
        return;
    }

    /* New participant discovered */
    /* Assert the participant to update the QoS */

    /* TODO: In this case we only allow participants that have been
     * asserted. However, this requires that the user asserts participants
     * A simple improvement is specify a filter based on the name for
     * participants which are allowed. This would not be compatible with
     * RTI DDS though, so we would then allow all participants.
     */
    LOG_printDebug("assert participant: %x.%x.%x.%x\n",
            data->key.value[0],
            data->key.value[1],
            data->key.value[2],
            data->key.value[3]);

    retcode = NDDS_DomainParticipant_assert_remote_participant(
                  participant,data,&is_new);

    if (DDS_RETCODE_OK != retcode)
    {
        DPDE_LOG_WARNING_P1(LOG_FAIL_ASSERT_REMOTE_PARTICIPANT, retcode);
        goto done;
    }

    if (is_new)
    {
        LOG_printDebug("detected new participant\n");

        retcode = NDDS_DomainParticipant_enable_remote_participant_guid(
                        participant, data);

        if (DDS_RETCODE_OK != retcode)
        {
            DPDE_LOG_WARNING_P1(LOG_FAIL_ENABLE_REMOTE_PARTICIPANT,
                                   retcode);
            goto done;
        }

        NETIO_Address_init(&dst_reader,NETIO_ADDRESS_KIND_GUID);
        dst_reader.port = 0;
        dst_reader.value.rtps_guid.object_id =
                NETIO_htonl(MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT);
        j = DDS_LocatorSeq_get_length(&data->metatraffic_unicast_locators);

        for (i = 0; i < j; i++)
        {
            a_locator = DDS_LocatorSeq_get_reference(
                                        &data->metatraffic_unicast_locators,i);
            NETIO_Address_init(&dst_address,0);
            NETIO_Address_set_ipv4(
             &dst_address,a_locator->port,((struct NETIO_Address*)a_locator)->value.ipv4.address);
            DDS_DataWriter_add_anonymous_route(
             dpde_plugin->participant_writer,&dst_reader,&dst_address);
        }

        if (dpde_plugin->timer_created)
        {
            DPDE_ParticipantDiscovery_schedule_fast_assertions(
                    (struct NDDS_Discovery_Plugin *)dpde_plugin,
                    participant,
                    dpde_plugin->participant_builtin_data,
                    DDS_BOOLEAN_FALSE);
        }

        /* Create the well-known discovery endpoints */
        retcode = DPDE_PublicationDiscovery_assert_remote_builtin(
                &dpde_plugin->_parent, participant,data);

        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }

        retcode = DPDE_SubscriptionDiscovery_assert_remote_builtin(
                &dpde_plugin->_parent, participant,data);
        if (retcode != DDS_RETCODE_OK)
        {
            goto done;
        }
    }

    retcode = NDDS_DomainParticipant_refresh_remote_participant_liveliness(
            participant, &data->key);

    if (DDS_RETCODE_OK != retcode)
    {
        DPDE_LOG_WARNING(LOG_FAIL_REFRESH_REMOTE_PARTICIPANT);
        goto done;
    }

done:
    return;
}
