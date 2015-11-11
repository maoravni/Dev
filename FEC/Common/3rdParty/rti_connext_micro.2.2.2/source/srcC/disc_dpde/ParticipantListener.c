/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
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
#include "ParticipantDiscovery.pkg.h"
#include "PublicationDiscovery.pkg.h"
#include "SubscriptionDiscovery.pkg.h"

/*** SOURCE_BEGIN ***/

/* ------------------------------------------------------------------------ */
/*                Participant Discovery Listener                            */
/* ------------------------------------------------------------------------ */
RTI_PRIVATE void
DPDE_ParticipantBuiltinDataReaderListener_on_requested_deadline_missed(
                        void *listener_data,
                        DDS_DataReader* reader,
                        const struct DDS_RequestedDeadlineMissedStatus *status)
{

}

/*ce \dref_ParticipantBuiltinDataReaderListener_LivelinessChangedCallback
 */
RTI_PRIVATE void
DPDE_ParticipantBuiltinDataReaderListener_on_liveliness_changed(
                    void *listener_data,
                    DDS_DataReader *reader,
                    const struct DDS_LivelinessChangedStatus *status)
{

}

/*ce \dref_ParticipantBuiltinDataReaderListener_RequestedIncompatibleQosCallback
 */
RTI_PRIVATE void
DPDE_ParticipantBuiltinDataReaderListener_on_requested_incompatible_qos(
                    void *listener_data,
                    DDS_DataReader *reader,
                    const struct DDS_RequestedIncompatibleQosStatus *status)
{

}

/*ce \dref_ParticipantBuiltinDataReaderListener_SampleRejectedCallback
 */
RTI_PRIVATE void
DPDE_ParticipantBuiltinDataReaderListener_on_sample_rejected(
                    void *listener_data,
                    DDS_DataReader *reader,
                    const struct DDS_SampleRejectedStatus *status)
{

}

/*ce \dref_ParticipantBuiltinDataReaderListener_DataAvailableCallback
 */
RTI_PRIVATE void
DPDE_ParticipantBuiltinDataReaderListener_on_data_available(
                                                        void *listener_data,
                                                        DDS_DataReader *reader)
{
    DDS_ReturnCode_t retcode;
    struct DPDE_DiscoveryPlugin *dpde_plugin =
                                (struct DPDE_DiscoveryPlugin *)listener_data;
    DDS_Long i,l;
    DDS_DomainParticipant *participant = NULL;
    struct DDS_ParticipantBuiltinTopicDataSeq data_seq =
                            DDS_ParticipantBuiltinTopicDataSeq_INITIALIZER;
    struct DDS_SampleInfoSeq info_seq  =
                                DDS_SEQUENCE_INITIALIZER(struct DDS_SampleInfo);
    struct DDS_ParticipantBuiltinTopicData *data = NULL;
    struct DDS_SampleInfo *info = NULL;
    struct DDS_BuiltinTopicKey_t key;

    participant = DDS_Subscriber_get_participant(dpde_plugin->subscriber);

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
            DPDE_Log_error(LOG_FAIL_TAKE_PARTICIPANT_DISCOVERY_SAMPLE);
            break;
        }

        l = DDS_ParticipantBuiltinTopicDataSeq_get_length(&data_seq);
        for (i = 0; i < l; ++i)
        {
            data = (struct DDS_ParticipantBuiltinTopicData *)
                               DDS_UntypedSampleSeq_get_reference(
                                    (struct DDS_UntypedSampleSeq*)&data_seq,i);
            info = DDS_SampleInfoSeq_get_reference(&info_seq,i);

            if (info->valid_data)
            {
                DPDE_ParticipantDiscovery_assert_remote_participant(
                                            dpde_plugin,participant,data,info);
            }
            else
            {
                if (info->instance_state == DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE
                    || info->instance_state == DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE)
                {
                    DDS_InstanceHandle_to_rtps((struct MIGRtpsGuid *)&key,
                                               &info->instance_handle);
                    /* TODO: Need an option to select whether to remove or reset
                     * the participant
                     */
                    NDDS_DomainParticipant_remove_remote_participant(
                                                            participant, &key);
                }
                else
                {
                    DPDE_LOG_WARNING(LOG_INVALID_PARTICIPANT_DISCOVERY_SAMPLE);
                }
            }
        }
    } while (retcode == DDS_RETCODE_OK);

    DDS_DataReader_return_loan(reader,(struct DDS_UntypedSampleSeq*)&data_seq,&info_seq);
}

/*ce \dref_ParticipantBuiltinDataReaderListener_SubscriptionMatchedCallback
 */
RTI_PRIVATE void
DPDE_ParticipantBuiltinDataReaderListener_on_subscription_matched(
                        void *listener_data,
                        DDS_DataReader *reader,
                        const struct DDS_SubscriptionMatchedStatus *status)
{
}

/*ce \dref_ParticipantBuiltinDataReaderListener_SampleLostCallback
 */
RTI_PRIVATE void
DPDE_ParticipantBuiltinDataReaderListener_on_sample_lost(
                        void *listener_data,
                        DDS_DataReader * reader,
                        const struct DDS_SampleLostStatus *status)
{

}

void
DPDE_ParticipantBuiltinDataReaderListener_initialize(
                        struct NDDS_Discovery_Plugin *plugin,
                        struct DDS_DataReaderListener *listener)
{
    struct DDS_DataReaderListener DEFAULT_LISTENER = 
                                        DDS_DataReaderListener_INITIALIZER;

    *listener = DEFAULT_LISTENER;

    listener->as_listener.listener_data = plugin;

    listener->on_requested_deadline_missed =
        DPDE_ParticipantBuiltinDataReaderListener_on_requested_deadline_missed;

    listener->on_requested_incompatible_qos =
        DPDE_ParticipantBuiltinDataReaderListener_on_requested_incompatible_qos;

    listener->on_sample_rejected =
        DPDE_ParticipantBuiltinDataReaderListener_on_sample_rejected;

    listener->on_liveliness_changed =
        DPDE_ParticipantBuiltinDataReaderListener_on_liveliness_changed;

    listener->on_data_available =
        DPDE_ParticipantBuiltinDataReaderListener_on_data_available;

    listener->on_subscription_matched =
        DPDE_ParticipantBuiltinDataReaderListener_on_subscription_matched;

    listener->on_sample_lost =
        DPDE_ParticipantBuiltinDataReaderListener_on_sample_lost;
}
