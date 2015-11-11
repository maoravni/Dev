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

#include "QosPolicy.pkg.h"
#include "Entity.pkg.h"
#include "TopicDescription.pkg.h"
#include "Topic.pkg.h"
#include "Type.pkg.h"
#include "DataReaderQos.pkg.h"
#include "DataReaderEvent.pkg.h"
#include "DataReaderInterface.pkg.h"
#include "DataReaderImpl.pkg.h"

const DDS_InstanceStateMask DDS_ANY_INSTANCE_STATE =
    DDS_ALIVE_INSTANCE_STATE |
    DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE |
    DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE;

const DDS_ViewStateMask DDS_ANY_VIEW_STATE =
    DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE;

const DDS_SampleStateMask DDS_ANY_SAMPLE_STATE =
    DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE;

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DDS_DataReader_enable(DDS_Entity *self)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl*)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    struct OSAPITimeoutUserData storage;

    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    if (datareader->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        return DDS_RETCODE_OK;
    }

    if (!DDS_Entity_is_enabled(DDS_Subscriber_as_entity(datareader->subscriber)))
    {
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    DB_Database_lock(datareader->config.db);

    DDSC_LOG_DEBUG_P1(LOG_ENABLE_DATAREADER, datareader->object_id);

    datareader->as_entity.state = RTIDDS_ENTITY_STATE_ENABLED;

#if !ENABLE_DISCOVERY_MATCH_BUILTIN
    if (!(datareader->as_entity.entity_id & 0xc0) && datareader->config.on_after_enabled)
    {
        datareader->config.on_after_enabled((DDS_DataReader*)self,&datareader->qos);
    }
#else
    if (datareader->config.on_after_enabled)
    {
        datareader->config.on_after_enabled((DDS_DataRader*)self,&datareader->qos);
    }
#endif

    storage.field[0] = (void *)datareader;

#ifdef ENABLE_QOS_DEADLINE
    if (!DDS_Duration_is_infinite(&datareader->qos.deadline.period))
    {
        if (!OSAPI_Timer_create_timeout(datareader->config.timer,
                        &datareader->deadline_event,
                        DDS_Duration_to_ms(datareader->qos.deadline.period),
                        OSAPI_TIMER_PERIODIC,
                        DDS_DataReaderEvent_on_deadline_timeout,
                        &storage))
        {
            DDSC_Log_error_p1(LOG_FAIL_POST_DEADLINE_DATAREADER,
                               datareader->as_entity.entity_id);
            retcode = DDS_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }
    }
#endif

    NETIO_Interface_set_state(datareader->rtps_intf,NETIO_INTERFACESTATE_ENABLED);
    NETIO_Interface_set_state(datareader->dr_intf,NETIO_INTERFACESTATE_ENABLED);

done:

    DB_Database_unlock(datareader->config.db);

    return retcode;
}

DDS_TopicDescription*
DDS_DataReader_get_topicdescription(DDS_DataReader *self)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    if (datareader == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    return DDS_Topic_as_topicdescription(self->topic);
}

DDS_Topic*
DDS_DataReader_get_topic(DDS_DataReader *self)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    if (datareader == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    return self->topic;
}

DDS_Subscriber*
DDS_DataReader_get_subscriber(DDS_DataReader *self)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    if (datareader == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    return self->subscriber;
}


DDS_ReturnCode_t
DDS_DataReader_set_listener(DDS_DataReader *self,
                            const struct DDS_DataReaderListener *l,
                            DDS_StatusMask mask)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
    struct DDS_DataReaderListener default_listener =
                                            DDS_DataReaderListener_INITIALIZER;

    if (datareader == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(datareader->config.db);

    if (l == NULL)
    {
        if (mask == DDS_STATUS_MASK_NONE)
        {
            datareader->mask = mask;
            datareader->listener = default_listener;
            retcode = DDS_RETCODE_OK;
        }
        goto done;
    }

    if (!DDS_DataReader_listener_is_consistent(l, mask))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DATAREADER_LISTENER);
        goto done;
    }

    datareader->listener = *l;
    datareader->mask = mask;

    retcode = DDS_RETCODE_OK;

done:

    DB_Database_unlock(datareader->config.db);

    return retcode;
}

struct DDS_DataReaderListener
DDS_DataReader_get_listener(DDS_DataReader * self)
{
    struct DDS_DataReaderListener retval = DDS_DataReaderListener_INITIALIZER;
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    if (datareader == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return retval;
    }

    DB_Database_lock(datareader->config.db);

    retval = datareader->listener;

    DB_Database_unlock(datareader->config.db);

    return retval;
}

DDS_ReturnCode_t
DDS_DataReader_read_or_take(DDS_DataReader *self,
                            struct DDS_UntypedSampleSeq *received_data, 
                            struct DDS_SampleInfoSeq *info_seq,
                            DDS_Long max_samples,
                            const DDS_InstanceHandle_t *a_handle,
                            DDS_SampleStateMask sample_states,
                            DDS_ViewStateMask view_states,
                            DDS_InstanceStateMask instance_states,
                            DDS_Boolean take,
                            DDS_Boolean valid_handle)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_NO_DATA;
    DDS_Boolean loan = RTI_FALSE;

    void **sample_ptrs_array = NULL;
    struct DDS_SampleInfo *info_array = NULL;

    struct DDS_SampleInfo *sample_info = NULL;

    RTI_INT32 data_seq_max;
    DDS_Long sample_count = 0;
    RTI_INT32 i;

    if (datareader == NULL || received_data == NULL || info_seq == NULL || 
        (valid_handle && a_handle == NULL))
    {
        /* TODO: log msg */
        return DDS_RETCODE_BAD_PARAMETER;
    }

    /* values of len, max_len, and owns must be identical between seqs */
    if ((DDS_UntypedSampleSeq_get_maximum(received_data) != 
        DDS_SampleInfoSeq_get_maximum(info_seq)) || 

        (DDS_UntypedSampleSeq_get_length(received_data) != 
         DDS_SampleInfoSeq_get_length(info_seq)) ||

        (DDS_UntypedSampleSeq_has_ownership(received_data) !=     
         DDS_SampleInfoSeq_has_ownership(info_seq)))
    {
        /* TODO: log msg */
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    /* cannot read/take using sequence with loaned buffer */
    if (max_samples > 0 && !DDS_UntypedSampleSeq_has_ownership(received_data))
    {
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    data_seq_max = DDS_UntypedSampleSeq_get_maximum(received_data);

    /* cannot expect max_samples if sequence max not that large */
    if ((max_samples != DDS_LENGTH_UNLIMITED) && (data_seq_max > 0) && 
        (max_samples > data_seq_max))
    {
        /* TODO: log msg */
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    DB_Database_lock(datareader->config.db);

    /* set bound on max_samples returned by reader-history */
	if (max_samples == DDS_LENGTH_UNLIMITED || 
        max_samples > datareader->read_seq_max)
    {
        /* set finite max_samples */
		max_samples = datareader->read_seq_max;
    }

    /* whether to loan buffers to seqs */
    loan = (data_seq_max == 0 ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE);

    /* read/take from reader history */
    retcode = DDSHST_Reader_read_or_take(
       datareader->_rh, &sample_ptrs_array, &info_array, &sample_count, 
       (valid_handle ? a_handle : NULL),
       max_samples, sample_states, view_states, instance_states, take);

    if ((retcode == DDS_RETCODE_NO_DATA) || (retcode != DDS_RETCODE_OK))
    {
        /* TODO: log msg */
        goto done;
    }

    if (loan)
    {
        /* loan buffer of sample ptrs */
        if (!DDS_UntypedSampleSeq_loan_discontiguous(
           received_data, (void *)sample_ptrs_array, sample_count, sample_count))
        {
            /* TODO: log msg */
            goto done;
        }

        /* loan buffer of sample infos */
        if (!DDS_SampleInfoSeq_loan_contiguous(
           info_seq, (void *)info_array, sample_count, sample_count))
        {
            /* TODO: log msg */
            goto done;
        }
    }
    else /* !loan */
    {
		DDS_UntypedSampleSeq_set_length(received_data, sample_count);			
        DDS_SampleInfoSeq_set_length(info_seq, sample_count);

        for (i = 0; i < sample_count; ++i)
        {
            /* copy info */
            sample_info = DDS_SampleInfoSeq_get_reference(info_seq, i);
            *sample_info = info_array[i];
                
            /* copy data */
            if (sample_info->valid_data)
            {
                if (!datareader->type_plugin->copy_sample(
                   datareader->type_plugin,
                   DDS_UntypedSampleSeq_get_reference(received_data, i),
                   sample_ptrs_array[i],
                   datareader->qos.type_support.plugin_data))
                {
                    DDSC_Log_error(LOG_FAIL_COPY_DATA_TO_SAMPLE);
					retcode = DDS_RETCODE_ERROR;
                }
            }
        }

        /* done copying */
        (void)DDSHST_Reader_finish_read_or_take(
          datareader->_rh, &sample_ptrs_array, &info_array, sample_count, take);
    }

done:
    
    DB_Database_unlock(datareader->config.db);

    return retcode;
}

DDS_ReturnCode_t
DDS_DataReader_read_or_take_next_sample(DDS_DataReader *self,
                                        void *received_data,
                                        struct DDS_SampleInfo *sample_info,
                                        DDS_Boolean take)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_NO_DATA;

    void **sample_ptr_array = NULL;
    struct DDS_SampleInfo *info_array = NULL;
    DDS_Long sample_count;

    if ((datareader == NULL) || (received_data == NULL) ||(sample_info == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }
    
    DB_Database_lock(datareader->config.db);

    /* read/take from reader history */
    retcode = DDSHST_Reader_read_or_take(
       datareader->_rh, &sample_ptr_array, &info_array, &sample_count, NULL, 1,
       DDS_NOT_READ_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE, 
       take);

    if (retcode != DDS_RETCODE_OK)
    {
        /* TODO: log exception */
        goto done;
    }

    /* copy sample info */
    *sample_info = info_array[0];

    if (sample_info->valid_data)
    {
        /* copy user data */
        if (!self->type_plugin->copy_sample(datareader->type_plugin, 
                                      received_data, 
                                      sample_ptr_array[0],
                                      datareader->qos.type_support.plugin_data))
        {   
            DDSC_Log_error(LOG_FAIL_COPY_DATA_TO_SAMPLE);
            goto done;
        }
        retcode = DDS_RETCODE_OK;
    }

    /* return resources to reader history */
    (void)DDSHST_Reader_finish_read_or_take(datareader->_rh, &sample_ptr_array, 
                                            &info_array, 1, take);
    
done:
    DB_Database_unlock(datareader->config.db);

    return retcode;
}

DDS_ReturnCode_t
DDS_DataReader_read(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq,
        DDS_Long max_samples,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states)
{
    return DDS_DataReader_read_or_take(self, received_data, info_seq, 
        max_samples, NULL, sample_states, view_states, instance_states, 
        DDS_BOOLEAN_FALSE, DDS_BOOLEAN_FALSE);
}

DDS_ReturnCode_t
DDS_DataReader_take(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq,
        DDS_Long max_samples,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states)
{
    return DDS_DataReader_read_or_take(self, received_data, info_seq, 
        max_samples, NULL, sample_states, view_states, instance_states, 
        DDS_BOOLEAN_TRUE, DDS_BOOLEAN_FALSE);
}

DDS_ReturnCode_t
DDS_DataReader_read_instance(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq,
        DDS_Long max_samples,
        const DDS_InstanceHandle_t *a_handle,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states)
{
    return DDS_DataReader_read_or_take(self, received_data, info_seq, 
        max_samples, a_handle, sample_states, view_states, instance_states, 
        DDS_BOOLEAN_FALSE, DDS_BOOLEAN_TRUE);
}

DDS_ReturnCode_t
DDS_DataReader_take_instance(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq,
        DDS_Long max_samples,
        const DDS_InstanceHandle_t *a_handle,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states)
{
    return DDS_DataReader_read_or_take(self, received_data, info_seq, 
        max_samples, a_handle, sample_states, view_states, instance_states, 
        DDS_BOOLEAN_TRUE, DDS_BOOLEAN_TRUE);
}

DDS_ReturnCode_t
DDS_DataReader_read_next_sample(DDS_DataReader *self,
                                void *received_data,
                                struct DDS_SampleInfo *sample_info)
{
    return DDS_DataReader_read_or_take_next_sample(
       self, received_data, sample_info, DDS_BOOLEAN_FALSE /* take */);
}

DDS_ReturnCode_t
DDS_DataReader_take_next_sample(DDS_DataReader *self,
                                void *received_data,
                                struct DDS_SampleInfo *sample_info)
{
    return DDS_DataReader_read_or_take_next_sample(
       self, received_data, sample_info, DDS_BOOLEAN_TRUE /* take */);
}


DDS_ReturnCode_t
DDS_DataReader_return_loan(DDS_DataReader *self,
                           struct DDS_UntypedSampleSeq *received_data,
                           struct DDS_SampleInfoSeq *info_seq)
{
    DDS_ReturnCode_t retval;
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    void **sample_ptr_array = NULL;
    struct DDS_SampleInfo *info_array = NULL;
    DDS_Long sample_count;

    if (self == NULL || received_data == NULL || info_seq == NULL)
    {
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (DDS_UntypedSampleSeq_has_ownership(received_data) || 
        DDS_SampleInfoSeq_has_ownership(info_seq))
    {
        /* sequence(s) not loaned --> no side effects */
        return DDS_RETCODE_OK;
    }

    /* get (loaned) buffers from sequences. return_loan() will return them to
       the reader queue */
    sample_ptr_array = (void **)
        DDS_UntypedSampleSeq_get_buffer(received_data);
    info_array = (struct DDS_SampleInfo *)
        DDS_SampleInfoSeq_get_buffer(info_seq);
    sample_count = 
        DDS_SampleInfoSeq_get_length(info_seq);

    retval = DDSHST_Reader_return_loan(
       datareader->_rh, &sample_ptr_array, &info_array, sample_count);

    if (retval != DDS_RETCODE_OK)
    {
        /* TODO: log exception */
        return retval;
    }

    DDS_UntypedSampleSeq_unloan(received_data);
    DDS_SampleInfoSeq_unloan(info_seq);

    return DDS_RETCODE_OK;
}


DDS_InstanceHandle_t
DDS_DataReader_lookup_instance(DDS_DataReader *self,
                               const void *key_holder)
{
    DDS_InstanceHandle_t handle = DDS_HANDLE_NIL;
    DDS_KeyHash_t key_hash;
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return handle;
    }

    /* check key kind */
    if (datareader->type_plugin->get_key_kind(datareader->type_plugin, NULL) ==
        NDDS_TYPEPLUGIN_NO_KEY)
    {
        /* no key, not failure */
        return handle;
    }

    key_hash.length = MIG_RTPS_KEY_HASH_MAX_LENGTH;
    OSAPI_Memory_zero(key_hash.value, key_hash.length);
    OSAPI_Memory_zero(handle.octet, key_hash.length);
    CDR_Stream_Reset(datareader->md5_stream);

    if (!datareader->type_plugin->instance_to_keyhash(
       datareader->type_plugin, datareader->md5_stream, &key_hash, key_holder,  
       datareader->qos.type_support.plugin_data))
    {
        return handle;
    }

    OSAPI_Memory_copy(handle.octet, key_hash.value, key_hash.length);

    if (DDSHST_Reader_lookup_key(datareader->_rh, &handle) != NULL)
    {
        handle.is_valid = DDS_BOOLEAN_TRUE;
    }

    return handle;
}


DDS_ReturnCode_t
DDS_DataReader_liveliness_lost(DDS_DataReader *self,
                               DDS_InstanceHandle_t *publication_handle)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;
    struct DDSHST_ReaderEvent event;

    event.kind = DDSHST_READEREVENT_KIND_REMOTE_WRITER_DELETED;
    event.data.rw_deleted.rw_guid = *publication_handle;
    event.data.rw_deleted.rw_guid.is_valid = DDS_BOOLEAN_TRUE;

    if (self->_rh)
    {
        DDSHST_Reader_post_event(datareader->_rh, &event, NULL);
    }

    return DDS_RETCODE_OK;
}

/*******************************************************************************
 *                             OPTIONAL APIs
 ******************************************************************************/

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DataReader_set_qos(DDS_DataReader * self,
                       const struct DDS_DataReaderQos * qos)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    if ((datareader == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_DataReaderQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DATAREADER_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    DB_Database_lock(datareader->config.db);

    if (!DDS_DataReaderQos_immutable_is_equal(&datareader->qos, qos))
    {
        retcode = DDS_RETCODE_IMMUTABLE_POLICY;
        goto done;
    }

    retcode = DDS_DataReaderQos_finalize(&datareader->qos);
    if (retcode != DDS_RETCODE_OK)
    {
        goto done;
    }

    retcode = DDS_DataReaderQos_initialize(&datareader->qos);
    if (retcode != DDS_RETCODE_OK)
    {
        goto done;
    }

    retcode = DDS_DataReaderQos_copy(&datareader->qos, qos);

done:

    DB_Database_unlock(datareader->config.db);

    return retcode;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DataReader_get_qos(DDS_DataReader *self, struct DDS_DataReaderQos *qos)
{
    DDS_ReturnCode_t retcode;
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    if ((datareader == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(datareader->config.db);

    retcode = DDS_DataReaderQos_copy(qos, &self->qos);

    DB_Database_unlock(datareader->config.db);

    return retcode;
}
#endif

#if INCLUDE_API_LOOKUP
DDS_ReturnCode_t
DDS_DataReader_get_matched_publications(DDS_DataReader *self,
                            struct DDS_InstanceHandleSeq *publication_handles)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;

    if ((datareader == NULL) || (publication_handles == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_Entity_is_enabled(&datareader->as_entity))
    {
        return DDS_RETCODE_NOT_ENABLED;
    }

    DB_Database_lock(datareader->config.db);

#if 0
    count = DB_Table_get_row_count(db_cache->reader_state_table);

    if (!DDS_InstanceHandleSeq_set_maximum(publication_handles, count))
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_OUT_OF_RESOURCES;
    }

    if (!DDS_InstanceHandleSeq_set_length(publication_handles, count))
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    db_retcode =
        DB_Table_select_record_all(datareader->config.db_cache->reader_state_table,
                                  &handle);
    count = 0;
    db_retcode = DB_Table_select_next(handle, &reader_state);
    while (db_retcode != DB_RETCODE_NO_DATA)
    {
        if ((reader_state->reader == datareader) &&
            (reader_state->state != NDDS_DATAREADERSTATE_NOT_MATCHED))
        {
            DDS_InstanceHandle_from_rtps(DDS_InstanceHandleSeq_get_reference
                                         (publication_handles, count),
                                         (const struct MIGRtpsGuid
                                          *)&reader_state->remote_writer->data.
                                         key);
            ++count;
        }
        db_retcode = DB_Table_select_next(handle, &reader_state);
    }
    DB_Table_select_finish(handle);
#endif

    DB_Database_unlock(datareader->config.db);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_LOOKUP
DDS_ReturnCode_t
DDS_DataReader_get_matched_publication_data(DDS_DataReader *self,
                struct DDS_PublicationBuiltinTopicData *publication_data,
                const DDS_InstanceHandle_t *publication_handle)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;
    DDS_BuiltinTopicKey_t key;

    if ((datareader == NULL) || (publication_data == NULL) ||
        (publication_handle == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_Entity_is_enabled(&datareader->as_entity))
    {
        return DDS_RETCODE_NOT_ENABLED;
    }

    DDS_InstanceHandle_to_rtps((struct MIGRtpsGuid *)&key, publication_handle);

    DB_Database_lock(datareader->config.db);

#if 0
    struct DDS_RemotePublicationImpl *rem_publication;

    rem_publication = NULL;
    db_retcode = DB_Table_lookup_record((void **)&rem_publication,
                                       db_cache->remote_publisher_table,
                                       (DB_Key_T) & key, DB_RECORD_NONE);

    if (db_retcode != DB_RETCODE_OK)
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    if (!DDS_PublicationBuiltinTopicData_copy(publication_data,
                                              &rem_publication->data))
    {
        DB_Database_unlock(self->config.db_cache->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }
#endif

    DB_Database_unlock(datareader->config.db);
    return DDS_RETCODE_OK;
}
#endif

