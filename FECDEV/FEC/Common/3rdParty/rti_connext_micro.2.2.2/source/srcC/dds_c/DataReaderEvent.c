/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "Entity.pkg.h"
#include "QosPolicy.pkg.h"
#include "TopicDescription.pkg.h"
#include "Topic.pkg.h"
#include "Type.pkg.h"
#include "DataReaderQos.pkg.h"
#include "DataReaderInterface.pkg.h"
#include "DataReader.pkg.h"
#include "DataReaderImpl.pkg.h"
#include "SubscriberEvent.pkg.h"

/*** SOURCE_BEGIN ***/

void
DDS_DataReaderEvent_on_incompatible_qos(struct DDS_DataReaderImpl *dr,
                                        const DDS_BuiltinTopicKey_t *key)
{
    struct DDS_RequestedIncompatibleQosStatus status;

    ++dr->requested_incompatible_qos_total_count;
    dr->requested_incompatible_qos_total_count_change = 1;

    status.last_policy_id = dr->last_policy_id;
    status.total_count = dr->requested_incompatible_qos_total_count;
    status.total_count_change = dr->requested_incompatible_qos_total_count_change;

    if ((dr->mask & DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS) &&
            dr->listener.on_requested_deadline_missed)
    {
        dr->listener.on_requested_incompatible_qos(
            dr->listener.as_listener.listener_data,
            dr, &status);
    }
    else
    {
        DDS_SubscriberEvent_on_requested_incompatible_qos(
                                        dr->subscriber,dr,&status);
    }
}

void
DDS_DataReaderEvent_on_subscription_matched(struct DDS_DataReaderImpl *dr,
                                           const DDS_BuiltinTopicKey_t *key,
                                           DDS_Boolean route_existed,
                                           DDS_Boolean matched)
{
    struct DDS_SubscriptionMatchedStatus pms;

    if ((route_existed) && matched)
    {
        return;
    }

    DDS_InstanceHandle_from_rtps(&pms.last_publication_handle,
                                 (struct MIGRtpsGuid*)key);

    if (!route_existed && matched)
    {
        ++dr->subscription_matched_current_count;
        ++dr->subscription_matched_current_count_change;
        ++dr->subscription_matched_total_count;
        pms.current_count = dr->subscription_matched_current_count;
        pms.current_count_change = 1;
        pms.total_count = dr->subscription_matched_total_count;
        pms.total_count_change = 1;

        if ((dr->mask & DDS_SUBSCRIPTION_MATCHED_STATUS) &&
                dr->listener.on_subscription_matched)
        {
            dr->listener.on_subscription_matched(
                    dr->listener.as_listener.listener_data,dr,&pms);
        }
        else
        {
            DDS_SubscriberEvent_on_subscription_matched(
                                             dr->subscriber,dr,&pms);
        }
    }
    else if (route_existed && !matched)
    {
        --dr->subscription_matched_current_count;
        --dr->subscription_matched_current_count_change;
        pms.current_count = dr->subscription_matched_current_count;
        pms.current_count_change = -1;
        pms.total_count = dr->subscription_matched_total_count;
        pms.total_count_change = -1;

        if ((dr->mask & DDS_SUBSCRIPTION_MATCHED_STATUS) &&
                dr->listener.on_subscription_matched)
        {
            dr->listener.on_subscription_matched(
                    dr->listener.as_listener.listener_data,dr,&pms);
        }
        else
        {
            DDS_SubscriberEvent_on_subscription_matched(dr->subscriber,dr,&pms);
        }
    }
}

void
DDS_DataReaderEvent_on_data_available(struct DDSHST_Reader *rh,
                                      void *listener_data,
                                      DDS_InstanceHandle_t * key,
                                      DDSHST_ReaderSample_T * sample)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)listener_data;

    if (dr->mask & DDS_DATA_AVAILABLE_STATUS)
    {
        dr->listener.on_data_available(
                                dr->listener.as_listener.listener_data, dr);
    }
    else
    {
        DDS_SubscriberEvent_on_data_available(
                                       DDS_DataReader_get_subscriber(dr), dr);
    }
}

void
DDS_DataReaderEvent_on_sample_removed(struct DDSHST_Reader *rh,
                                        void *listener_data,
                                        DDS_InstanceHandle_t *key,
                                        DDSHST_ReaderSample_T *sample)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)listener_data;

    REDA_BufferPool_return_buffer(dr->cdr_samples,sample->_param);
}

void
DDS_DataReaderEvent_on_hst_sample_rejected(
                        struct DDSHST_Reader *rh,
                        void *listener_data,
                        DDS_InstanceHandle_t *key,
                        DDS_SampleRejectedStatusKind reason)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)listener_data;
    struct DDS_SampleRejectedStatus status;

    ++dr->sample_rejected_status_total_count;
    ++dr->sample_rejected_status_total_count_change;

#ifdef ENABLE_STATUS_LISTENER
    status.last_reason = reason;
    status.total_count = dr->sample_rejected_status_total_count;
    status.total_count_change = 1;
    status.last_instance_handle = *key;

    if (dr->mask & DDS_SAMPLE_LOST_STATUS)
    {
        dr->listener.on_sample_rejected(
                            dr->listener.as_listener.listener_data,dr,&status);
    }
    else
    {
        DDS_SubscriberEvent_on_sample_rejected(dr->subscriber, dr,&status);
    }
#endif
}

void
DDS_DataReaderEvent_sample_lost(struct DDS_DataReaderImpl *dr)
{
    struct DDS_SampleLostStatus sample_lost_info;

    sample_lost_info.total_count = dr->sample_lost_status_total_count;
    sample_lost_info.total_count_change = dr->sample_lost_status_total_count_change;

#ifdef ENABLE_STATUS_LISTENER
    if (dr->mask & DDS_SAMPLE_LOST_STATUS)
    {
        dr->listener.on_sample_lost(
                  dr->listener.as_listener.listener_data,dr,&sample_lost_info);
    }
    else
    {
        DDS_SubscriberEvent_on_sample_lost(dr->subscriber,dr,&sample_lost_info);
    }
#endif
}

void
DDS_DataReaderEvent_on_hst_sample_lost(
        struct DDSHST_Reader *rh,
        void *listener_data)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)listener_data;

    ++dr->sample_lost_status_total_count;
    dr->sample_lost_status_total_count_change = 1;

    DDS_DataReaderEvent_sample_lost(dr);
}

void
DDS_DataReaderEvent_on_sample_lost(struct DDS_DataReaderImpl *dr,
                                   struct NETIO_Address *source,
                                   struct REDA_SequenceNumber *first_sn,
                                   RTI_UINT32 count)
{
    if ((count == 0) || (source->value.rtps_guid.object_id ==
                         MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT))
    {
        return;
    }

    dr->sample_lost_status_total_count += count;
    dr->sample_lost_status_total_count_change = count;

    DDS_DataReaderEvent_sample_lost(dr);
}

RTI_BOOL
DDS_DataReaderEvent_on_commit_data(struct DDS_DataReaderImpl *datareader,
                                   struct NETIO_Address *source,
                                   struct REDA_SequenceNumber *first_sn,
                                   RTI_UINT32 count)
{
    DDS_InstanceHandle_t publisher_handle = DDS_HANDLE_NIL;

    OSAPI_Memory_copy(&publisher_handle.octet,&source->value.guid,16);
    publisher_handle.is_valid = DDS_BOOLEAN_TRUE;

    if (DDSHST_Reader_commit_sample(datareader->_rh, &publisher_handle,
                                first_sn,0) != DDSHST_RETCODE_SUCCESS)
    {
        DDSC_Log_error(LOG_FAIL_COMMIT_SAMPLE);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

void
DDS_DataReaderEvent_on_liveliness_changed(struct DDS_DataReaderImpl *datareader,
                                          DDS_InstanceHandle_t *instance)
{
    struct DDS_LivelinessChangedStatus status;
    RTI_BOOL event_consumed = DDS_BOOLEAN_FALSE;

    status.alive_count = datareader->liveliness_changed_alive_count;
    status.alive_count_change = datareader->liveliness_changed_alive_count_change;
    status.not_alive_count = datareader->liveliness_changed_not_alive_count;
    status.not_alive_count_change = datareader->liveliness_changed_not_alive_count_change;
    status.last_publication_handle = *instance;

#ifdef ENABLE_STATUS_LISTENER
    if ((datareader->mask & DDS_LIVELINESS_CHANGED_STATUS) &&
        datareader->listener.on_liveliness_changed)
    {
        datareader->listener.on_liveliness_changed(
                datareader->listener.as_listener.listener_data,datareader,&status);
        event_consumed = DDS_BOOLEAN_TRUE;
    }
    else
    {
        event_consumed = DDS_SubscriberEvent_on_liveliness_changed(
                                datareader->subscriber,datareader,&status);
    }

    if (event_consumed)
    {
        datareader->liveliness_changed_alive_count_change = 0;
        datareader->liveliness_changed_not_alive_count_change = 0;
    }
#endif
}

void
DDS_DataReaderEvent_on_liveliness_detected(struct DDS_DataReaderImpl *datareader,
                                           struct NETIO_Address *source)
{
    DDS_InstanceHandle_t publisher_handle = DDS_HANDLE_NIL;
    struct DDSHST_ReaderEvent event;
    struct OSAPI_NtpTime now;

    OSAPI_System_get_time(&now);

    OSAPI_Memory_copy(&publisher_handle.octet,&source->value.guid,16);
    publisher_handle.is_valid = DDS_BOOLEAN_TRUE;

    if (datareader->liveliness_changed_not_alive_count > 0)
    {
        --datareader->liveliness_changed_not_alive_count_change;
        --datareader->liveliness_changed_not_alive_count;
    }
    ++datareader->liveliness_changed_alive_count_change;
    ++datareader->liveliness_changed_alive_count;

    event.kind = DDSHST_READEREVENT_KIND_LIVELINESS_DETECTED;
    event.data.liveliness.rw_guid = publisher_handle;
    event.data.liveliness.instance.is_valid = DDS_BOOLEAN_FALSE;

    DDSHST_Reader_post_event(datareader->_rh, &event, &now);

    DDS_DataReaderEvent_on_liveliness_changed(datareader,&publisher_handle);
}

void
DDS_DataReaderEvent_on_liveliness_lost(struct DDS_DataReaderImpl *datareader,
                                       struct NETIO_Address *source)
{
    DDS_InstanceHandle_t publisher_handle = DDS_HANDLE_NIL;
    struct DDSHST_ReaderEvent event;
    struct OSAPI_NtpTime now;

    OSAPI_System_get_time(&now);

    OSAPI_Memory_copy(&publisher_handle.octet,&source->value.guid,16);
    publisher_handle.is_valid = DDS_BOOLEAN_TRUE;

    --datareader->liveliness_changed_alive_count;
    --datareader->liveliness_changed_alive_count_change;
    ++datareader->liveliness_changed_not_alive_count_change;
    ++datareader->liveliness_changed_not_alive_count;

    event.kind = DDSHST_READEREVENT_KIND_LIVELINESS_LOST;
    event.data.liveliness.rw_guid = publisher_handle;
    event.data.liveliness.instance.is_valid = DDS_BOOLEAN_FALSE;

    DDSHST_Reader_post_event(datareader->_rh, &event, &now);

    DDS_DataReaderEvent_on_liveliness_changed(datareader,&publisher_handle);
}

#ifdef ENABLE_QOS_DEADLINE
void
DDS_DataReaderEvent_deadline_missed(struct DDS_DataReaderImpl *datareader,
                                    DDS_InstanceHandle_t *ih)
{
    DDS_Boolean event_consumed = DDS_BOOLEAN_FALSE;
    struct DDS_RequestedDeadlineMissedStatus dms;

    LOG_printDebug("Deadline missed for reader %ld\n", datareader->as_entity.entity_id);

    ++datareader->deadline_missed_total_count;
    ++datareader->deadline_missed_total_count_change;

    dms.total_count = datareader->deadline_missed_total_count;
    dms.total_count_change = datareader->deadline_missed_total_count_change;
    dms.last_instance_handle = *ih;

#ifdef ENABLE_STATUS_LISTENER
    if (datareader->mask & DDS_REQUESTED_DEADLINE_MISSED_STATUS)
    {
        datareader->listener.on_requested_deadline_missed(
                datareader->listener.as_listener.listener_data,
                datareader,
                &dms);
        event_consumed = DDS_BOOLEAN_TRUE;
    }
    else
    {
        event_consumed =
            DDS_SubscriberEvent_on_requested_deadline_missed(
                            datareader->subscriber,
                            datareader,
                            &dms);
    }
#endif

    /* TODO: This has been moved to the queue
     * NDDS_DataReaderState_reassign_ownership(datareader);
     */
    if (event_consumed)
    {
        datareader->deadline_missed_total_count_change = 0;
    }
}

void
DDS_DataReaderEvent_on_deadline_missed(struct DDSHST_Reader *rh,
                                        void *listener_data,
                                        DDS_InstanceHandle_t *key)
{
    struct DDS_DataReaderImpl *datareader =
            (struct DDS_DataReaderImpl *)listener_data;

    DDS_DataReaderEvent_deadline_missed(datareader,key);
}

OSAPITimeoutOp_t
DDS_DataReaderEvent_on_deadline_timeout(struct OSAPITimeoutUserData *storage)
{
    struct DDS_DataReaderImpl *datareader =
                                (struct DDS_DataReaderImpl *)storage->field[0];
    struct DDSHST_ReaderEvent event;
    struct OSAPI_NtpTime now;

    OSAPI_System_get_time(&now);

    event.kind = DDSHST_READEREVENT_KIND_DEADLINE_EXPIRED;

    DB_Database_lock(datareader->config.db);

    DDSHST_Reader_post_event(datareader->_rh, &event, &now);

    DB_Database_unlock(datareader->config.db);

    return OSAPI_TIMEOUT_OP_AUTOMATIC;
}
#endif




