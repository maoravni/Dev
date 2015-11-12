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
#include "disc_dpde/disc_dpde_discovery_plugin.h"
#endif
#ifndef disc_dpde_log_h
#include "disc_dpde/disc_dpde_log.h"
#endif

#include "DiscoveryPlugin.pkg.h"
#include "SubscriptionBuiltinTopicDataPlugin.pkg.h"
#include "SubscriptionDiscovery.pkg.h"

/*** SOURCE_BEGIN ***/

/* ------------------------------------------------------------------------ */
/*                Participant Discovery Listener                            */
/* ------------------------------------------------------------------------ */


RTI_PRIVATE void
DPDE_SubscriptionBuiltinDataReaderListener_on_requested_deadline_missed(
                    void *listener_data,
                    DDS_DataReader *reader,
                    const struct DDS_RequestedDeadlineMissedStatus *status)
{
}


/*ce \dref_SubscriptionBuiltinDataReaderListener_LivelinessChangedCallback
 */
RTI_PRIVATE void
DPDE_SubscriptionBuiltinDataReaderListener_on_liveliness_changed(
                    void *listener_data,
                    DDS_DataReader *reader,
                    const struct DDS_LivelinessChangedStatus *status)
{
}

/*ce \dref_SubscriptionBuiltinDataReaderListener_RequestedIncompatibleQosCallback
 */
RTI_PRIVATE void
DPDE_SubscriptionBuiltinDataReaderListener_on_requested_incompatible_qos(
                    void *listener_data,
                    DDS_DataReader *reader,
                    const struct DDS_RequestedIncompatibleQosStatus *status)
{

}


/*ce \dref_SubscriptionBuiltinDataReaderListener_SampleRejectedCallback
 */
RTI_PRIVATE void
DPDE_SubscriptionBuiltinDataReaderListener_on_sample_rejected(
                                void *listener_data,
                                DDS_DataReader *reader,
                                const struct DDS_SampleRejectedStatus *status)
{

}


/*ce \dref_SubscriptionBuiltinDataReaderListener_DataAvailableCallback
 */
RTI_PRIVATE void
DPDE_SubscriptionBuiltinDataReaderListener_on_data_available(
                                                        void *listener_data,
                                                        DDS_DataReader *reader)
{
    struct DPDE_DiscoveryPlugin *const dpde_plugin =
                                (struct DPDE_DiscoveryPlugin *)listener_data;
    DDS_ReturnCode_t retcode,retcode2;
    struct DDS_SubscriptionBuiltinTopicDataSeq data_seq =
                            DDS_SubscriptionBuiltinTopicDataSeq_INITIALIZER;
    struct DDS_SampleInfoSeq info_seq  =
                                DDS_SEQUENCE_INITIALIZER(struct DDS_SampleInfo);
    struct DDS_SubscriptionBuiltinTopicData *data = NULL;
    struct DDS_SampleInfo *info = NULL;
    DDS_Long i,l;

    do
    {
        retcode = DDS_DataReader_take(reader,
                (struct DDS_UntypedSampleSeq*)&data_seq,&info_seq,
                DDS_LENGTH_UNLIMITED,
                DDS_ANY_VIEW_STATE,DDS_ANY_SAMPLE_STATE,DDS_ANY_INSTANCE_STATE);

        if (retcode == DDS_RETCODE_NO_DATA)
        {
            break;
        }
        else if (retcode != DDS_RETCODE_OK)
        {
            DPDE_Log_error(LOG_FAIL_TAKE_SUBSCRIPTION_DISCOVERY_SAMPLE);
            break;
        }

        l = DDS_SubscriptionBuiltinTopicDataSeq_get_length(&data_seq);
        for (i = 0; i < l; ++i)
        {
            data = (struct DDS_SubscriptionBuiltinTopicData *)
                       DDS_UntypedSampleSeq_get_reference(
                                   (struct DDS_UntypedSampleSeq*)&data_seq,i);
            info = DDS_SampleInfoSeq_get_reference(&info_seq,i);

            if (info->valid_data)
            {
                data->participant_key = data->key;
                data->participant_key.value[3] = MIG_RTPS_OBJECT_ID_PARTICIPANT;
                retcode2 = NDDS_DomainParticipant_assert_remote_subscription(
                        DDS_Subscriber_get_participant(dpde_plugin->subscriber),
                        NULL,data, NDDS_TYPEPLUGIN_GUID_KEY);
                if (DDS_RETCODE_OK != retcode2)
                {
                    DPDE_LOG_WARNING(LOG_FAIL_ASSERT_REMOTE_SUBSCRIPTION);
                }
            }
            else
            {
                if ((info->instance_state == DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                        || (info->instance_state == DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE))
                {
                    NDDS_DomainParticipant_remove_remote_subscription(
                            DDS_Subscriber_get_participant(dpde_plugin->subscriber),
                            &data->key);
                }
                else
                {
                    DPDE_LOG_WARNING(LOG_INVALID_SUBSCRIPTION_DISCOVERY_SAMPLE);
                }
            }
        }
    } while (retcode == DDS_RETCODE_OK);
    DDS_DataReader_return_loan(reader,(struct DDS_UntypedSampleSeq*)&data_seq,&info_seq);
}

/*ce \dref_SubscriptionBuiltinDataReaderListener_SubscriptionMatchedCallback
 */
RTI_PRIVATE void
DPDE_SubscriptionBuiltinDataReaderListener_on_subscription_matched(
                            void *listener_data,
                            DDS_DataReader *reader,
                            const struct DDS_SubscriptionMatchedStatus *status)
{
}

/*ce \dref_SubscriptionBuiltinDataReaderListener_SampleLostCallback
 */
RTI_PRIVATE void
DPDE_SubscriptionBuiltinDataReaderListener_on_sample_lost(
                                                void *listener_data,
                                                DDS_DataReader *reader,
                                                const struct
                                                DDS_SampleLostStatus *status)
{
}

RTI_PRIVATE void
DPDE_SubscriptionBuiltinDataWriterListener_on_publication_matched(
                            void *listener_data,
                            DDS_DataWriter *reader,
                            const struct DDS_PublicationMatchedStatus *status)
{
}

void
DPDE_SubscriptionBuiltinDataReaderListener_initialize(
                                       struct DPDE_DiscoveryPlugin *plugin,
                                       struct DDS_DataReaderListener *listener)
{
    struct DDS_DataReaderListener DEFAULT_LISTENER = 
                                        DDS_DataReaderListener_INITIALIZER;

    *listener = DEFAULT_LISTENER;

    listener->as_listener.listener_data = plugin;

    listener->on_requested_deadline_missed =
       DPDE_SubscriptionBuiltinDataReaderListener_on_requested_deadline_missed;

    listener->on_requested_incompatible_qos =
       DPDE_SubscriptionBuiltinDataReaderListener_on_requested_incompatible_qos;

    listener->on_sample_rejected =
       DPDE_SubscriptionBuiltinDataReaderListener_on_sample_rejected;

    listener->on_liveliness_changed =
       DPDE_SubscriptionBuiltinDataReaderListener_on_liveliness_changed;

    listener->on_data_available =
       DPDE_SubscriptionBuiltinDataReaderListener_on_data_available;

    listener->on_subscription_matched =
       DPDE_SubscriptionBuiltinDataReaderListener_on_subscription_matched;

    listener->on_sample_lost =
       DPDE_SubscriptionBuiltinDataReaderListener_on_sample_lost;
}

void
DPDE_SubscriptionBuiltinDataWriterListener_initialize(
                                    struct DPDE_DiscoveryPlugin *plugin,
                                    struct DDS_DataWriterListener *listener)
{
    struct DDS_DataWriterListener INIT_LISTENER =
                                            DDS_DataWriterListener_INITIALIZER;
    *listener = INIT_LISTENER;

    listener->on_publication_matched =
        DPDE_SubscriptionBuiltinDataWriterListener_on_publication_matched;
}
