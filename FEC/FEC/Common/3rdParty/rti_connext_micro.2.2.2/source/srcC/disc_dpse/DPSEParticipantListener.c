/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
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
#include "DPSEDiscoveryPlugin.pkg.h"

/*** SOURCE_BEGIN ***/

/* ------------------------------------------------------------------------ */
/*                Participant Discovery Listener                            */
/* ------------------------------------------------------------------------ */
RTI_PRIVATE void
DPSE_ParticipantBuiltinDataReaderListener_on_requested_deadline_missed(
                        void *listener_data,
                        DDS_DataReader* reader,
                        const struct DDS_RequestedDeadlineMissedStatus *status)
{
}

/*ce \dref_ParticipantBuiltinDataReaderListener_LivelinessChangedCallback
 */
RTI_PRIVATE void
DPSE_ParticipantBuiltinDataReaderListener_on_liveliness_changed(
                            void *listener_data,
                            DDS_DataReader *reader,
                            const struct DDS_LivelinessChangedStatus*status)
{
}

/*ce \dref_ParticipantBuiltinDataReaderListener_RequestedIncompatibleQosCallback
 */
RTI_PRIVATE void
DPSE_ParticipantBuiltinDataReaderListener_on_requested_incompatible_qos(
                    void *listener_data,
                    DDS_DataReader *reader,
                    const struct DDS_RequestedIncompatibleQosStatus *status)
{
}

/*ce \dref_ParticipantBuiltinDataReaderListener_SampleRejectedCallback
 */
RTI_PRIVATE void
DPSE_ParticipantBuiltinDataReaderListener_on_sample_rejected(
                                void *listener_data,
                                DDS_DataReader *reader,
                                const struct DDS_SampleRejectedStatus *status)
{
}

/*ce \dref_ParticipantBuiltinDataReaderListener_DataAvailableCallback
 */
RTI_PRIVATE void
DPSE_ParticipantBuiltinDataReaderListener_on_data_available(
                                                        void *listener_data,
                                                        DDS_DataReader *reader)
{
    struct DPSE_DiscoveryPlugin *dpse_plugin =
                            (struct DPSE_DiscoveryPlugin *)listener_data;
    DDS_ReturnCode_t retcode;
    RTI_BOOL first_discovered = RTI_TRUE;
    DDS_Long i,j,k,l;
    DDS_DomainParticipant *participant = NULL;
    struct NETIO_Address dst_address;
    struct NETIO_Address dst_reader;
    struct DDS_Locator *a_locator;
    struct MIGRtpsGuid guid;
    DDS_InstanceHandle_t ih;
    struct DDS_ParticipantBuiltinTopicDataSeq data_seq =
                            DDS_ParticipantBuiltinTopicDataSeq_INITIALIZER;
    struct DDS_SampleInfoSeq info_seq  =
                                DDS_SEQUENCE_INITIALIZER(struct DDS_SampleInfo);
    struct DDS_ParticipantBuiltinTopicData *data = NULL;
    struct DDS_SampleInfo *info = NULL;
    struct DDS_BuiltinTopicKey_t key;

    NETIO_Address_init(&dst_reader,NETIO_ADDRESS_KIND_GUID);
    dst_reader.value.rtps_guid.object_id =
            NETIO_htonl(MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT);

    participant = DDS_Subscriber_get_participant(
                                    dpse_plugin->participant_subscriber);

    do
    {
        retcode = DDS_DataReader_take(reader,
                (struct DDS_UntypedSampleSeq*)&data_seq,&info_seq, DDS_LENGTH_UNLIMITED,
                DDS_ANY_VIEW_STATE,DDS_ANY_SAMPLE_STATE,DDS_ANY_INSTANCE_STATE);

        if (retcode == DDS_RETCODE_NO_DATA)
        {
            break;
        }
        else if (retcode != DDS_RETCODE_OK)
        {
            DPSE_Log_error(LOG_FAIL_TAKE_PARTICIPANT_DISCOVERY_SAMPLE);
            break;
        }

        ih = DDS_Entity_get_instance_handle(DDS_DomainParticipant_as_entity(participant));

        l = DDS_ParticipantBuiltinTopicDataSeq_get_length(&data_seq);
        for (k = 0; k < l; ++k)
        {
            data = (struct DDS_ParticipantBuiltinTopicData *)
                               DDS_UntypedSampleSeq_get_reference(
                                    (struct DDS_UntypedSampleSeq*)&data_seq, k);
            info = DDS_SampleInfoSeq_get_reference(&info_seq, k);

            if (info->valid_data)
            {
                DDS_InstanceHandle_to_rtps(&guid, &ih);
                if (MIGRtpsGuid_equals(&guid,(struct MIGRtpsGuid *)&data->key))
                {
                    LOG_printDebug("ignore ourselves\n");
                    continue;
                }

                /* New participant discovered */
                LOG_printDebug("detected participant: %s\n",data->participant_name.name);

                /* Assert the participant to update the QoS */
                for (i = 0; i < dpse_plugin->key_list_length; i++)
                {
                    if (DDS_BuiltinTopicKey_equals(&data->key,
                                                   &dpse_plugin->key_list[i]))
                    {
                        /* This participant has been discovered before; don't
                        *  make announcements.
                       */
                        first_discovered = RTI_FALSE;
                    }
                }

                if (first_discovered)
                {
                    LOG_printDebug("new discovery: %s\n",
                                   data->participant_name.name);

                    /* --- Enable the participant --- */
                    retcode = 
                        NDDS_DomainParticipant_enable_remote_participant_name(
                           participant, data);

                    if (DDS_RETCODE_OK != retcode)
                    {
                        DPSE_LOG_WARNING_P1(LOG_FAIL_ASSERT_REMOTE_PARTICIPANT,
                                            retcode);
                        continue;
                    }

                    NETIO_Address_init(&dst_reader,NETIO_ADDRESS_KIND_GUID);
                    dst_reader.port = 0;
                    dst_reader.value.rtps_guid.object_id =
                        NETIO_htonl(MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT);
                    j = DDS_LocatorSeq_get_length(
                       &data->metatraffic_unicast_locators);
                    for (i = 0; i < j; i++)
                    {
                        a_locator = DDS_LocatorSeq_get_reference(
                           &data->metatraffic_unicast_locators,i);
                        NETIO_Address_set_ipv4(&dst_address,a_locator->port,
                         ((struct NETIO_Address*)a_locator)->value.ipv4.address);
                        DDS_DataWriter_add_anonymous_route(
                           dpse_plugin->participant_writer,
                           &dst_reader,&dst_address);
                    }

                    /* If we successfully enabled the remote participant, we add 
                     * its key to our list.  If we didn't successfully enable 
                     * the participant, we may have discovered a participant 
                     * that was not asserted, which is not an error.
                     */
                    if (dpse_plugin->key_list_length == 
                        dpse_plugin->key_list_max)
                    {
                        DPSE_LOG_WARNING(
                           LOG_DISCOVER_MORE_THAN_MAX_REMOTE_PARTICIPANTS);
                    }

                    DDS_BuiltinTopicKey_copy_prefix(
                       &dpse_plugin->key_list[dpse_plugin->key_list_length],
                       &data->key);

                    DDS_BuiltinTopicKey_copy_suffix(
                       &dpse_plugin->key_list[dpse_plugin->key_list_length],
                       &data->key);

                    dpse_plugin->key_list_length++;

                    DPSE_ParticipantDiscovery_schedule_fast_assertions(
                       (struct NDDS_Discovery_Plugin*)dpse_plugin,
                       dpse_plugin->participant_builtin_data,
                       DDS_BOOLEAN_FALSE);
                }
                else
                {
                    /* Refresh the liveliness of a pre-existing
                       remote participant */
                    retcode = 
                        NDDS_DomainParticipant_refresh_remote_participant_liveliness(
                           participant, &data->key);
                    if (DDS_RETCODE_OK != retcode)
                    {
                        DPSE_LOG_WARNING(LOG_FAIL_REFRESH_REMOTE_PARTICIPANT);
                        continue;
                    }
                }
            }
            else
            {
                /* Check for disposed */
                if (info->instance_state == DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE ||
                    info->instance_state == DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE)
                {
                    DDS_InstanceHandle_to_rtps((struct MIGRtpsGuid *)&key,
                                               &info->instance_handle);

                    /* We've gotten a dispose message: reset the participant to be
                     * not alive in the databasel
                     */
                    NDDS_DomainParticipant_reset_remote_participant(participant, &key);
                }
                else
                {
                    DPSE_LOG_WARNING(LOG_INVALID_PARTICIPANT_DISCOVERY_SAMPLE);
                }
            }
        } /* for data_seq length */
    } while (retcode == DDS_RETCODE_OK);

    DDS_DataReader_return_loan(reader,(struct DDS_UntypedSampleSeq*)&data_seq,&info_seq);
}

/*ce \dref_ParticipantBuiltinDataReaderListener_SubscriptionMatchedCallback
 */
RTI_PRIVATE void
DPSE_ParticipantBuiltinDataReaderListener_on_subscription_matched(
                            void *listener_data,
                            DDS_DataReader *reader,
                            const struct DDS_SubscriptionMatchedStatus *status)
{
}


/*ce \dref_ParticipantBuiltinDataReaderListener_SampleLostCallback
 */
RTI_PRIVATE void
DPSE_ParticipantBuiltinDataReaderListener_on_sample_lost(void *listener_data,
                                    DDS_DataReader *reader,
                                    const struct DDS_SampleLostStatus *status)
{
}

void
DPSE_ParticipantBuiltinDataReaderListener_initialize(
                                       struct NDDS_Discovery_Plugin *plugin,
                                       struct DDS_DataReaderListener *listener)
{
    struct DDS_DataReaderListener DEFAULT_LISTENER = 
                                        DDS_DataReaderListener_INITIALIZER;

    *listener = DEFAULT_LISTENER;

    listener->as_listener.listener_data = plugin;

    listener->on_requested_deadline_missed =
        DPSE_ParticipantBuiltinDataReaderListener_on_requested_deadline_missed;

    listener->on_requested_incompatible_qos =
        DPSE_ParticipantBuiltinDataReaderListener_on_requested_incompatible_qos;

    listener->on_sample_rejected =
        DPSE_ParticipantBuiltinDataReaderListener_on_sample_rejected;

    listener->on_liveliness_changed =
        DPSE_ParticipantBuiltinDataReaderListener_on_liveliness_changed;

    listener->on_data_available =
        DPSE_ParticipantBuiltinDataReaderListener_on_data_available;

    listener->on_subscription_matched =
        DPSE_ParticipantBuiltinDataReaderListener_on_subscription_matched;

    listener->on_sample_lost =
        DPSE_ParticipantBuiltinDataReaderListener_on_sample_lost;
}

