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
#ifndef osapi_timer_h
#include "osapi/osapi_timer.h"
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
#include "DataWriterImpl.pkg.h"
#include "DataWriterQos.pkg.h"
#include "DataWriterInterface.pkg.h"
#include "PublisherEvent.pkg.h"

/*** SOURCE_BEGIN ***/

void
DDS_DataWriterEvent_on_incompatible_qos(struct DDS_DataWriterImpl *dw,
                                        const DDS_BuiltinTopicKey_t *key)
{
    struct DDS_OfferedIncompatibleQosStatus status;

    ++dw->offered_incompatible_qos_total_count;
    dw->offered_incompatible_qos_total_count_change = 1;
    status.total_count = dw->offered_incompatible_qos_total_count;
    status.total_count_change = dw->offered_incompatible_qos_total_count_change;
    status.last_policy_id = dw->last_policy_id;

    if ((dw->mask & DDS_OFFERED_INCOMPATIBLE_QOS_STATUS) &&
            dw->listener.on_offered_incompatible_qos)
    {
        dw->listener.on_offered_incompatible_qos(
            dw->listener.as_listener.listener_data,
            dw, &status);
    }
    else
    {
        DDS_PublisherEvent_on_offered_incompatible_qos(
                                        dw->publisher,dw,&status);
    }
}

void
DDS_DataWriterEvent_on_publication_matched(struct DDS_DataWriterImpl *dw,
                                           const DDS_BuiltinTopicKey_t *key,
                                           DDS_Boolean route_existed,
                                           DDS_Boolean matched)
{
    struct DDS_PublicationMatchedStatus pms;

    if ((route_existed) && matched)
    {
        return;
    }

    DDS_InstanceHandle_from_rtps(&pms.last_subscription_handle,
                                 (struct MIGRtpsGuid*)key);

    if (!route_existed && matched)
    {
        ++dw->publication_matched_status_current_count;
        ++dw->publication_matched_status_current_count_change;
        ++dw->publication_matched_status_total_count;
        dw->publication_matched_status_total_count_change = 1;
        pms.current_count = dw->publication_matched_status_current_count;
        pms.current_count_change = 1;
        pms.total_count = dw->publication_matched_status_total_count;
        pms.total_count_change = 1;

        if ((dw->mask & DDS_PUBLICATION_MATCHED_STATUS) &&
                dw->listener.on_publication_matched)
        {
            dw->listener.on_publication_matched(
                    dw->listener.as_listener.listener_data,dw,&pms);
        }
        else
        {
            DDS_PublisherEvent_on_publication_matched(
                                             dw->publisher,dw,&pms);
        }
    }
    else if (route_existed && !matched)
    {
        --dw->publication_matched_status_current_count;
        --dw->publication_matched_status_current_count_change;
        pms.current_count = dw->publication_matched_status_current_count;
        pms.current_count_change = -1;
        pms.total_count = dw->publication_matched_status_total_count_change;

        if ((dw->mask & DDS_PUBLICATION_MATCHED_STATUS) &&
                dw->listener.on_publication_matched)
        {
            dw->listener.on_publication_matched(
                    dw->listener.as_listener.listener_data,dw,&pms);
        }
        else
        {
            DDS_PublisherEvent_on_publication_matched(dw->publisher,dw,&pms);
        }
    }
}

#ifdef ENABLE_QOS_DEADLINE
RTI_PRIVATE void
DDS_DataWriterEvent_deadline_missed(struct DDS_DataWriterImpl *datawriter,
                                    DDS_InstanceHandle_t *ih)
{
    struct DDS_OfferedDeadlineMissedStatus dms;

    ++datawriter->deadline_missed_total_count;
    ++datawriter->deadline_missed_total_count_change;

    dms.total_count = datawriter->deadline_missed_total_count;
    dms.total_count_change = 1;
    dms.last_instance_handle = *ih;

#ifdef ENABLE_STATUS_LISTENER
    if (datawriter->mask & DDS_OFFERED_DEADLINE_MISSED_STATUS)
    {
        datawriter->listener.on_offered_deadline_missed(
                    datawriter->listener.as_listener.listener_data,
                    datawriter,
                    &dms);
    }
    else
    {
        (void)DDS_PublisherEvent_on_offered_deadline_missed(datawriter->publisher,
                                                      datawriter,
                                                      &dms);
    }
#endif
}
#endif

OSAPITimeoutOp_t
DDS_DataWriterEvent_on_liveliness(struct OSAPITimeoutUserData *storage)
{
    struct DDS_DataWriterImpl *datawriter;
#ifdef ENABLE_STATUS_LISTENER
    struct DDS_LivelinessLostStatus lls;
#endif
    datawriter = (struct DDS_DataWriterImpl *)storage->field[0];

    DB_Database_lock(datawriter->config.db);

    ++datawriter->liveliness_lost_total_count;
    ++datawriter->liveliness_lost_total_count_change;

    lls.total_count = datawriter->liveliness_lost_total_count;
    lls.total_count_change = 1;

#ifdef ENABLE_STATUS_LISTENER
    if (datawriter->mask & DDS_LIVELINESS_LOST_STATUS)
    {
        datawriter->listener.on_liveliness_lost(datawriter->listener.
                                                as_listener.listener_data,
                                                datawriter,
                                                &lls);
    }
    else
    {
        (void)DDS_PublisherEvent_on_liveliness_lost(
                                    datawriter->publisher, datawriter,&lls);
    }
#endif

    DB_Database_unlock(datawriter->config.db);

    return OSAPI_TIMEOUT_OP_AUTOMATIC;
}

void
DDS_DataWriterEvent_on_sample_removed(void *config,
                                      DDS_InstanceHandle_t *key,
                                      struct DDSHST_WriterSample *sample)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)config;

    struct RTI_TransformCDR_Sample *cdr_sample =
                                    (struct RTI_TransformCDR_Sample *)sample;

    REDA_BufferPool_return_buffer(datawriter->cdr_samples,cdr_sample);
}

void
DDS_DataWriterEvent_on_key_removed(void *config, DDS_InstanceHandle_t * key)
{
}

void
DDS_DataWriterEvent_on_deadline_missed(void *listener_data,
                                        DDS_InstanceHandle_t *key)
{
    struct DDS_DataWriterImpl *datawriter =
            (struct DDS_DataWriterImpl *)listener_data;

    DDS_DataWriterEvent_deadline_missed(datawriter,key);
}

OSAPITimeoutOp_t
DDS_DataWriterEvent_on_deadline_expired(struct OSAPITimeoutUserData *storage)
{
    struct DDS_DataWriterImpl *datawriter =
                                (struct DDS_DataWriterImpl *)storage->field[0];
    struct DDSHST_WriterEvent event;
    struct OSAPI_NtpTime now;

    OSAPI_System_get_time(&now);

    event.kind = DDSHST_WRITEREVENT_KIND_DEADLINE_EXPIRED;

    DB_Database_lock(datawriter->config.db);

    DDSHST_Writer_post_event(datawriter->wh, &event, &now);

    DB_Database_unlock(datawriter->config.db);

    return OSAPI_TIMEOUT_OP_AUTOMATIC;
}
