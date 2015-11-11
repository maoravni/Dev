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
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_route_h
#include "netio/netio_route.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "QosPolicy.pkg.h"
#include "Entity.pkg.h"
#include "TopicDescription.pkg.h"
#include "Topic.pkg.h"
#include "Type.pkg.h"
#include "DataWriterQos.pkg.h"
#include "DataWriterEvent.pkg.h"
#include "DataWriterInterface.pkg.h"
#include "DataWriterImpl.pkg.h"

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DDS_DataWriter_advance_sn(DDS_DataWriter *self)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    DB_Database_lock(datawriter->config.db);

    REDA_SequenceNumber_plusplus(&(datawriter->last_sn));

    DB_Database_unlock(datawriter->config.db);

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
DDS_DataWriter_assert_liveliness(DDS_DataWriter *self)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;
    DDS_ReturnCode_t rc;
    struct NETIO_Address destination = NETIO_Address_INITIALIZER;
    NETIO_Packet_T packet;
    struct NETIO_PacketInfo *pkt_info;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(datawriter->config.db);

    NETIO_Packet_initialize(&packet,NULL,100,50,NULL);
    pkt_info = NETIO_Packet_get_info(&packet);
    pkt_info->sn = self->last_sn;
    pkt_info->priority = 0;
    pkt_info->protocol_id = NETIO_PROTOCOL_INTRA;
    pkt_info->rtps_flags = NETIO_RTPS_FLAGS_LIVELINESS;

    if (!NETIO_Interface_send(datawriter->dw_intf,
                              datawriter->dw_intf,&destination,&packet))
    {
        rc = DDS_RETCODE_ERROR;
        goto done;
    }

    rc = DDS_RETCODE_OK;

done:

    DB_Database_unlock(datawriter->config.db);
    return rc;
}

DDS_ReturnCode_t
DDS_DataWriter_enable(DDS_Entity *self)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    struct OSAPITimeoutUserData storage;

    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    if (self->state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        return DDS_RETCODE_OK;
    }

    if (!DDS_Entity_is_enabled(DDS_Publisher_as_entity(datawriter->publisher)))
    {
        retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    DB_Database_lock(datawriter->config.db);

    DDSC_LOG_DEBUG_P1(LOG_ENABLE_DATAWRITER, datawriter->as_entity.entity_id);

    NETIO_Interface_set_state(datawriter->dw_intf,NETIO_INTERFACESTATE_ENABLED);

    NETIO_Interface_set_state(datawriter->rtps_intf,NETIO_INTERFACESTATE_ENABLED);

    datawriter->as_entity.state = RTIDDS_ENTITY_STATE_ENABLED;

    /* NOTE: If we match built-in entities as normal entities are matched,
     * then if the ignore API is implemented it would work the same for
     * internal and external entities
     */
#if !ENABLE_DISCOVERY_MATCH_BUILTIN
    if (!(datawriter->as_entity.entity_id & 0xc0) && datawriter->config.on_after_enabled)
    {
        datawriter->config.on_after_enabled((DDS_DataWriter*)self,&datawriter->qos);
    }
#else
    if (datawriter->config.on_after_enabled)
    {
        datawriter->config.on_after_enabled((DDS_DataWriter*)self,&datawriter->qos);
    }
#endif

    storage.field[0] = (void *)datawriter;

#ifdef ENABLE_QOS_DEADLINE
    if (!DDS_Duration_is_infinite(&datawriter->qos.deadline.period))
    {
        if (!OSAPI_Timer_create_timeout(datawriter->config.timer,
                        &datawriter->deadline_event,
                        DDS_Duration_to_ms(datawriter->qos.deadline.period),
                        OSAPI_TIMER_PERIODIC,
                        DDS_DataWriterEvent_on_deadline_expired,
                        &storage))
        {
            DDSC_Log_error_p1(LOG_FAIL_POST_DEADLINE_DATAWRITER,
                             datawriter->as_entity.entity_id);
            retcode = DDS_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }
    }
#endif

    if (!DDS_Duration_is_infinite(&datawriter->qos.liveliness.lease_duration))
    {
        if (!OSAPI_Timer_create_timeout(datawriter->config.timer,
                &datawriter->liveliness_event,
                DDS_Duration_to_ms(datawriter->qos.liveliness.lease_duration),
                OSAPI_TIMER_PERIODIC,
                DDS_DataWriterEvent_on_liveliness,
                &storage))
        {
            DDSC_Log_error_p1(LOG_FAIL_POST_LIVELINESS_DATAWRITER,
                               datawriter->as_entity.entity_id);
            retcode = DDS_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }
    }

done:

    DB_Database_unlock(datawriter->config.db);

    return retcode;
}

DDS_Topic*
DDS_DataWriter_get_topic(DDS_DataWriter *self)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    return datawriter->topic;
}

DDS_Publisher*
DDS_DataWriter_get_publisher(DDS_DataWriter *self)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    return datawriter->publisher;
}

DDS_ReturnCode_t
DDS_DataWriter_set_listener(DDS_DataWriter *self,
                            const struct DDS_DataWriterListener *l,
                            DDS_StatusMask mask)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
    struct DDS_DataWriterListener default_listener =
                                        DDS_DataWriterListener_INITIALIZER;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(datawriter->config.db);

    if (l == NULL)
    {
        if (mask == DDS_STATUS_MASK_NONE)
        {
            datawriter->mask = mask;
            datawriter->listener = default_listener;
            retcode = DDS_RETCODE_OK;
        }
        goto done;
    }

    if (!DDS_DataWriter_listener_is_consistent(l, mask))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DATAWRITER_LISTENER);
        goto done;
    }

    datawriter->listener = *l;
    datawriter->mask = mask;

    if (datawriter->config.on_after_modified)
    {
        datawriter->config.on_after_modified(self,NULL,&datawriter->listener);
    }

    retcode = DDS_RETCODE_OK;

done:

    DB_Database_unlock(datawriter->config.db);

    return retcode;
}

struct DDS_DataWriterListener
DDS_DataWriter_get_listener(DDS_DataWriter *self)
{
    struct DDS_DataWriterListener retval = DDS_DataWriterListener_INITIALIZER;
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if (datawriter == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return retval;
    }

    DB_Database_lock(datawriter->config.db);

    retval = self->listener;

    DB_Database_unlock(datawriter->config.db);

    return retval;
}


DDS_ReturnCode_t
DDS_DataWriter_write(DDS_DataWriter *self,
                     const void *instance_data,
                     const DDS_InstanceHandle_t *handle)
{
    struct DDS_WriteParams_t params = DDS_WRITEPARAMS_DEFAULT;
    struct OSAPI_NtpTime ntp_timestamp;

    if ((self == NULL) || (instance_data == NULL) || (handle == NULL))
    {
        /* TODO: log parameters */
        return DDS_RETCODE_BAD_PARAMETER;
    }

    params.handle = *handle;
    OSAPI_System_get_time(&ntp_timestamp);
    OSAPI_NtpTime_unpackToNanosec(&params.source_timestamp.sec, 
                                  &params.source_timestamp.nanosec,
                                  &ntp_timestamp);

    return DDS_DataWriter_write_w_params(self, instance_data, &params);
}

DDS_ReturnCode_t
DDS_DataWriter_write_w_timestamp(DDS_DataWriter *self,
                                 const void *instance_data,
                                 const DDS_InstanceHandle_t *handle,
                                 const struct DDS_Time_t *source_timestamp)
{
    struct DDS_WriteParams_t params = DDS_WRITEPARAMS_DEFAULT;

    if ((self == NULL) || (instance_data == NULL) || 
        (source_timestamp == NULL) || (handle == NULL))
    {
        /* TODO: log parameter */
        return DDS_RETCODE_BAD_PARAMETER;
    }

    params.handle = *handle;
    params.source_timestamp = *source_timestamp;

    return DDS_DataWriter_write_w_params(self, instance_data, &params);
}


DDS_ReturnCode_t
DDS_DataWriter_write_w_params(DDS_DataWriter *self,
                              const void *instance_data,
                              struct DDS_WriteParams_t *params)
{
    struct DDS_DataWriterImpl *writer = (struct DDS_DataWriterImpl *)self;
    struct NDDS_DataWriterSampleInfo sample_info;
    struct OSAPI_NtpTime next_deadline;
    struct OSAPI_NtpTime ntp_timestamp;
    DDS_ReturnCode_t retcode;

    if ((self == NULL) || (instance_data == NULL) || (params == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_NtpTime_packFromNanosec(&sample_info.timestamp, 
                                  params->source_timestamp.sec, 
                                  params->source_timestamp.nanosec);
    sample_info.statusInfo = MIG_RTPS_NO_STATUS_INFO;

    if (writer->type_plugin->get_key_kind(writer->type_plugin, NULL) ==
        NDDS_TYPEPLUGIN_USER_KEY)
    {
        sample_info.sendKeyHash = DDS_BOOLEAN_TRUE;
    }
    else
    {
        sample_info.sendKeyHash = DDS_BOOLEAN_FALSE;
    }

    DB_Database_lock(writer->config.db);

    /* Update so the deadline event does not fire */
    if (!DDS_Duration_is_infinite(&writer->qos.deadline.period))
    {
        OSAPI_NtpTime_packFromNanosec(&ntp_timestamp,
                                      params->source_timestamp.sec,
                                      params->source_timestamp.nanosec);
        OSAPI_NtpTime_add(&next_deadline, &ntp_timestamp,
                         &writer->topic_deadline);
        if (!OSAPI_Timer_update_timeout(writer->config.timer,
                         &writer->deadline_event,
                         DDS_Duration_to_ms(writer->qos.deadline.period)))
        {
            DDSC_Log_error_p1(LOG_FAIL_UPDATE_DEADLINE_DATAWRITER,
                               writer->as_entity.entity_id);
        }
    }

    retcode = DDS_DataWriter_write_untyped(writer,
                            instance_data, &params->handle, &sample_info);

    DB_Database_unlock(writer->config.db);

    return retcode;
}

#ifdef INCLUDE_API_INSTANCE
DDS_InstanceHandle_t
DDS_DataWriter_register_instance_w_timestamp(DDS_DataWriter *self,
                                    const void *instance_data,
                                    const struct DDS_Time_t *timestamp)
{
    DDS_InstanceHandle_t handle = DDS_HANDLE_NIL_NATIVE;
    struct OSAPI_NtpTime ntp_timestamp;
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return handle;
    }

    if (datawriter->type_plugin->get_key_kind == NDDS_TYPEPLUGIN_NO_KEY)
    {
        return handle;
    }

    OSAPI_NtpTime_packFromNanosec(&ntp_timestamp,
                                  timestamp->sec, timestamp->nanosec);

    DB_Database_lock(datawriter->config.db);

    /* register keyed type */
    if (DDS_DataWriter_register_key(datawriter, &handle, instance_data, 
                                    &ntp_timestamp) != DDS_RETCODE_OK)
    {
        DDSC_Log_error(LOG_FAIL_REGISTER_KEY);
    }
    DB_Database_unlock(datawriter->config.db);

    return handle;
}
#endif

#ifdef INCLUDE_API_INSTANCE
DDS_ReturnCode_t
DDS_DataWriter_unregister_instance_w_timestamp(DDS_DataWriter *self,
                           const void *instance_data,
                           const DDS_InstanceHandle_t *handle,
                           const struct DDS_Time_t *source_timestamp)
{
    struct NDDS_DataWriterSampleInfo sample_info;
    DDS_ReturnCode_t retcode;
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (datawriter->type_plugin->get_key_kind == NDDS_TYPEPLUGIN_NO_KEY)
    {
        return DDS_RETCODE_OK;
    }

    OSAPI_NtpTime_packFromNanosec(&sample_info.timestamp, 
                              source_timestamp->sec, source_timestamp->nanosec);
    sample_info.statusInfo = MIG_RTPS_UNREGISTER_STATUS_INFO;
    sample_info.sendKeyHash = DDS_BOOLEAN_TRUE;

    DB_Database_lock(datawriter->config.db);
    retcode = DDS_DataWriter_write_untyped(datawriter,
                                           instance_data,
                                           handle,
                                           &sample_info);
    DB_Database_unlock(datawriter->config.db);

    return retcode;
}
#endif

#ifdef INCLUDE_API_INSTANCE
DDS_ReturnCode_t
DDS_DataWriter_dispose(DDS_DataWriter *self,
                       const void *instance_data,
                       const DDS_InstanceHandle_t *handle)
{
    struct OSAPI_NtpTime ntp_timestamp;
    struct DDS_Time_t dds_timestamp;

    OSAPI_System_get_time(&ntp_timestamp);
    OSAPI_NtpTime_unpackToNanosec(&dds_timestamp.sec, &dds_timestamp.nanosec,
                                  &ntp_timestamp);

    return DDS_DataWriter_dispose_w_timestamp(
       self, instance_data, handle, &dds_timestamp);
}
#endif

#ifdef INCLUDE_API_INSTANCE
DDS_ReturnCode_t
DDS_DataWriter_dispose_w_timestamp(DDS_DataWriter *self,
                                   const void *instance_data,
                                   const DDS_InstanceHandle_t *handle,
                                   const struct DDS_Time_t *source_timestamp)
{
    struct NDDS_DataWriterSampleInfo sample_info;
    DDS_ReturnCode_t retcode;
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (datawriter->type_plugin->get_key_kind == NDDS_TYPEPLUGIN_NO_KEY)
    {
        return DDS_RETCODE_OK;
    }

    OSAPI_NtpTime_packFromNanosec(&sample_info.timestamp, 
                                  source_timestamp->sec, 
                                  source_timestamp->nanosec);
    sample_info.statusInfo = MIG_RTPS_DISPOSE_STATUS_INFO;
    sample_info.sendKeyHash = DDS_BOOLEAN_TRUE;

    DB_Database_lock(datawriter->config.db);

    retcode = DDS_DataWriter_write_untyped(datawriter,
                                           instance_data,
                                           handle,
                                           &sample_info);

    DB_Database_unlock(datawriter->config.db);

    return retcode;
}
#endif

/*******************************************************************************
 *                             OPTIONAL APIs
 ******************************************************************************/
#if INCLUDE_API_LOOKUP
DDS_ReturnCode_t
DDS_DataWriter_get_matched_subscriptions(DDS_DataWriter *self,
                            struct DDS_InstanceHandleSeq *subscription_handles)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if ((self == NULL) || (subscription_handles == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_Entity_is_enabled(&datawriter->as_entity))
    {
        return DDS_RETCODE_NOT_ENABLED;
    }

    return DDS_RETCODE_UNSUPPORTED;
#if 0
    struct NDDS_LocalWriterStateImpl *writer_state;
    DB_Database_lock(datawriter->config.db);

    dbrc = DB_Table_select_record_all(datawriter->config.db->writer_state_table,
                                     &handle);

    count = DB_Table_get_row_count(datawriter->config.db_cache->writer_state_table);

    if (!DDS_InstanceHandleSeq_set_maximum(subscription_handles, count))
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_OUT_OF_RESOURCES;
    }

    if (!DDS_InstanceHandleSeq_set_length(subscription_handles, count))
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    db_retcode =
        DB_Table_select_record_all(datawriter->config.db_cache->writer_state_table,
                                  &handle);
    count = 0;
    db_retcode = DB_Table_select_next(handle, &writer_state);
    while (db_retcode != DB_RETCODE_NO_DATA)
    {
        if ((writer_state->writer == datawriter) &&
            (writer_state->state != NDDS_DATAWRITERSTATE_NOT_MATCHED))
        {
            DDS_InstanceHandle_from_rtps(DDS_InstanceHandleSeq_get_reference
                                         (subscription_handles, count),
                                         (const struct MIGRtpsGuid
                                          *)&writer_state->remote_reader->data.
                                         key);
            count++;
        }
        db_retcode = DB_Table_select_next(handle, &writer_state);
    }
    DB_Table_select_finish(handle);
    if (db_retcode != DB_RETCODE_OK)
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    DB_Database_unlock(self->config.db_cache->database);
    return DDS_RETCODE_OK;
#endif
}
#endif

#if INCLUDE_API_LOOKUP
DDS_ReturnCode_t
DDS_DataWriter_get_matched_subscription_data(DDS_DataWriter * self,
            struct DDS_SubscriptionBuiltinTopicData *subscription_data,
            const DDS_InstanceHandle_t *subscription_handle)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if ((self == NULL) || (subscription_data == NULL) ||
        (subscription_handle == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_Entity_is_enabled(&datawriter->as_entity))
    {
        return DDS_RETCODE_NOT_ENABLED;
    }

    return DDS_RETCODE_UNSUPPORTED;

#if 0
    struct DDS_RemoteSubscriptionImpl *rem_subscription;
    DDS_BuiltinTopicKey_t key;

    DDS_InstanceHandle_to_rtps((struct MIGRtpsGuid *)&key, subscription_handle);

    DB_Database_lock(self->config.db_cache->database);

    rem_subscription = NULL;
    db_retcode = DB_Table_lookup_record((void **)&rem_subscription,
                                       datawriter->config.db_cache->
                                       remote_publisher_table, (DB_Key_T) & key,
                                       DB_RECORD_NONE);

    if (db_retcode != DB_RETCODE_OK)
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    if (!DDS_SubscriptionBuiltinTopicData_copy(subscription_data,
                                               &rem_subscription->data))
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    DB_Database_unlock(self->config.db_cache->database);
    return DDS_RETCODE_OK;
#endif
}
#endif


#ifdef INCLUDE_API_INSTANCE
DDS_InstanceHandle_t
DDS_DataWriter_register_instance(DDS_DataWriter * self,
                                 const void *instance_data)
{
    struct OSAPI_NtpTime ntp_timestamp;
    struct DDS_Time_t dds_timestamp;

    OSAPI_System_get_time(&ntp_timestamp);
    OSAPI_NtpTime_unpackToNanosec(&dds_timestamp.sec, &dds_timestamp.nanosec,
                                  &ntp_timestamp);

    return DDS_DataWriter_register_instance_w_timestamp(
       self, instance_data, &dds_timestamp);
}
#endif


#ifdef INCLUDE_API_INSTANCE
DDS_ReturnCode_t
DDS_DataWriter_unregister_instance(DDS_DataWriter * self,
                                   const void *instance_data,
                                   const DDS_InstanceHandle_t * handle)
{
    struct OSAPI_NtpTime ntp_timestamp;
    struct DDS_Time_t dds_timestamp;

    OSAPI_System_get_time(&ntp_timestamp);
    OSAPI_NtpTime_unpackToNanosec(&dds_timestamp.sec, &dds_timestamp.nanosec,
                                  &ntp_timestamp);

    return DDS_DataWriter_unregister_instance_w_timestamp(
       self, instance_data, handle, &dds_timestamp);
}
#endif


#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DataWriter_set_qos(DDS_DataWriter *self,
                       const struct DDS_DataWriterQos *qos)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_DataWriterQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DATAWRITER_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    DB_Database_lock(datawriter->config.db);

    if (DDS_Entity_is_enabled(DDS_DataWriter_as_entity(datawriter)))
    {
        retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    if (!DDS_DataWriterQos_immutable_is_equal(qos, &self->qos))
    {
        retcode = DDS_RETCODE_IMMUTABLE_POLICY;
        goto done;
    }

    DDS_DataWriterQos_copy(&self->qos, qos);

    retcode = DDS_RETCODE_OK;

    if (datawriter->config.on_after_modified)
    {
        datawriter->config.on_after_modified(self,qos,NULL);
    }
done:

    DB_Database_unlock(datawriter->config.db);

    return retcode;
}

DDS_ReturnCode_t
DDS_DataWriter_get_qos(DDS_DataWriter *self, struct DDS_DataWriterQos * qos)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (qos == NULL)
    {
        DDSC_Log_error(LOG_BAD_SECOND_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(datawriter->config.db);

    DDS_DataWriterQos_copy(qos, &self->qos);

    DB_Database_unlock(datawriter->config.db);

    return DDS_RETCODE_OK;
}
#endif
