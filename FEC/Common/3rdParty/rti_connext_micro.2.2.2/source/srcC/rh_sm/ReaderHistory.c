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
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef reda_buffer_h
#include "reda/reda_buffer.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef reda_circularlist_h
#include "reda/reda_circularlist.h"
#endif
#ifndef reda_indexer_h
#include "reda/reda_indexer.h"
#endif
#ifndef reda_sequenceNumber_h
#include "reda/reda_sequenceNumber.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef dds_c_common_impl
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef rh_sm_log_h
#include "rh_sm/rh_sm_log.h"
#endif

#include "rh_sm/rh_sm_history.h"

const struct DDSHST_ReaderListener RTI_DEFAULT_READERHISTORY_LISTENER =
                                            DDSHST_ReaderListener_INITIALIZE;

const struct RHSM_HistoryProperty RTI_DEFAULT_SMREADERHISTORY_PROPERTY =
                                            RHSM_HistoryProperty_INITIALIZER;

RTI_PRIVATE struct DDSHST_ReaderI RHSM_HistoryI_g_intf;



#ifndef reda_indexer_h
#include "reda/reda_indexer.h"
#endif

struct RHSM_HistoryKeyEntry;
struct RHSM_HistoryRWEntry;

struct RHSM_HistorySample
{
    struct REDA_CircularListNode _node;
    struct RHSM_HistorySample *_ordered_prev;
    struct RHSM_HistorySample *_ordered_next;
    DDSHST_ReaderSample_T *_sample;
    struct REDA_SequenceNumber _sn;
    struct RHSM_HistoryKeyEntry * _key_entry;
    RTI_UINT16 _loan_count;
    RTI_UINT16 _flags;
    struct RHSM_HistoryRWEntry *_pend_rw_entry;
    DDS_SampleStateMask sample_state;
    struct DDS_SampleInfo _info;
};

#define RHSM_HISTORY_FLAG_SAMPLE_TAKEN 0x01

struct RHSM_HistoryKeyEntry
{
    struct REDA_CircularListNode _node;
    DDS_InstanceHandle_t _key;
    REDA_CircularList_T _samples;
    RTI_INT32 _size;
    DDS_InstanceStateMask _instance_state;
    DDS_ViewStateMask _view_state;
    struct RHSM_HistoryRWEntry **_writers;
    RTI_INT32 _writer_count;
    struct RHSM_HistoryRWEntry *_current_owner;
    struct REDA_CircularListMixInNode _deadline_entry;
    RTI_INT32 last_update_period;
};

struct RHSM_HistoryRWEntry
{
    struct REDA_CircularListNode _node;
    DDS_InstanceHandle_t _key;
    REDA_CircularList_T _samples;
    RTI_INT32 _size;
    DDS_Long  _strength;
    RTI_INT32 _ownership_count;
};

struct RHSM_History
{
    struct DDSHST_Reader _parent;
    struct RHSM_HistoryProperty _property;
    const struct DDS_DataReaderQos *_qos;
    struct DDSHST_ReaderListener _listener;
    struct RHSM_HistorySample _s_ordered;
    struct DDSHST_ReaderInfo _info;
    REDA_CircularList_T _keys_in_use;
    REDA_CircularList_T _rw_in_use;
    REDA_Indexer_T *_key_index;
    REDA_Indexer_T *_rw_index;
    REDA_BufferPool_T _key_pool;
    REDA_BufferPool_T _rw_pool;
    REDA_BufferPool_T _sample_pool;
    REDA_BufferPool_T _sample_info_pool;
    REDA_BufferPool_T _sample_ptr_pool;
    REDA_CircularList_T _deadline_timer;
    RTI_INT32 _current_period;
    RTI_BOOL deadline_enabled;
    struct DDS_SampleRejectedStatus rejected_status;
    struct DDS_SampleLostStatus lost_status;
};



/*** SOURCE_BEGIN ***/

/*******************************************************************************
 *                                RTI_PRIVATE API
 ******************************************************************************/
RTI_PRIVATE RTI_INT32
RHSM_History_compare_key_record(const void *const record,
                                RTI_BOOL key_is_record,
                                const void *const key)
{
    struct RHSM_HistoryKeyEntry *record_left =
                                        (struct RHSM_HistoryKeyEntry*)record;
    const DDS_InstanceHandle_t *key_right;

    if (key_is_record)
    {
        key_right = &((struct RHSM_HistoryKeyEntry*)key)->_key;
    }
    else
    {
        key_right = (const DDS_InstanceHandle_t*)key;
    }

    return DDS_InstanceHandle_compare(&record_left->_key,key_right);
}

RTI_PRIVATE RTI_INT32
RHSM_History_compare_rw_record(const void *const record,
                               RTI_BOOL key_is_record,
                               const void *const key)
{
    struct RHSM_HistoryRWEntry *record_left =
                                        (struct RHSM_HistoryRWEntry*)record;
    const DDS_InstanceHandle_t *key_right;

    if (key_is_record)
    {
        key_right = &((struct RHSM_HistoryRWEntry*)key)->_key;
    }
    else
    {
        key_right = (const DDS_InstanceHandle_t*)key;
    }

    return DDS_InstanceHandle_compare(&record_left->_key,key_right);
}

RTI_PRIVATE void
RHSM_HistoryRWEntry_initialize(struct RHSM_HistoryRWEntry *rw_entry,
                               DDS_InstanceHandle_t *key)
{
    REDA_CircularListNode_init(&rw_entry->_node);
    rw_entry->_key = *key;
    REDA_CircularList_init(&rw_entry->_samples);
    rw_entry->_size = 0;
    rw_entry->_strength = 0;
    rw_entry->_ownership_count = 0;
}

RTI_PRIVATE void
RHSM_History_init_ordered_list(struct RHSM_HistorySample *list)
{
    list->_ordered_next = list;
    list->_ordered_prev = list;
}

RTI_PRIVATE DDS_Boolean
RHSM_History_ordered_list_is_empty(struct RHSM_HistorySample *list)
{
    return (list->_ordered_next == list ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE);
}

RTI_PRIVATE RTI_BOOL
RHSM_History_initialize_key(struct RHSM_History *self,
                            struct RHSM_HistoryKeyEntry *key)
{
    RTI_INT32 i;

    key->_size = 0;
    REDA_CircularList_init(&key->_samples);
    key->_key = DDS_HANDLE_NIL;
    key->_current_owner = NULL;
    key->_writer_count = 0;
    key->last_update_period = self->_current_period;

    if (key->_writers)
    {
        for (i = 0;
                i < self->_qos->reader_resource_limits.max_remote_writers_per_instance;
                ++i)
        {
            key->_writers[i] = NULL;
        }
    }

    REDA_CircularListNode_init(&key->_node);
    REDA_CircularListMixInNode_init(&key->_deadline_entry,key);

    return RTI_TRUE;
}

RTI_PRIVATE void
RHSM_History_insert_ordered_sample_after(
                                struct RHSM_HistorySample *after,
                                struct RHSM_HistorySample *sample)
{
    sample->_ordered_next = after->_ordered_next;
    sample->_ordered_next->_ordered_prev = sample;
    sample->_ordered_prev = after;
    after->_ordered_next = sample;
}

RTI_PRIVATE void
RHSM_History_unlink_ordered_sample(struct RHSM_HistorySample *sample)
{
    sample->_ordered_prev->_ordered_next = sample->_ordered_next;
    sample->_ordered_next->_ordered_prev = sample->_ordered_prev;
}

RTI_PRIVATE RTI_BOOL
RHSM_History_free_or_delete_sample(struct RHSM_History *self,
                                   DDS_InstanceHandle_t *key,
                                   struct RHSM_HistorySample *sample)
{
    if (sample->_loan_count != 0)
    {
        /* never delete loaned sample */
        return RTI_FALSE;
    }

    REDA_CircularList_unlink_node(&sample->_node);

    if ((sample->_ordered_next != NULL) && (sample->_ordered_prev != NULL))
    {
        RHSM_History_unlink_ordered_sample(sample);
    }

    if (sample->_sample && self->_listener.on_sample_removed)
    {
        self->_listener.on_sample_removed((struct DDSHST_Reader *)self,
                                          self->_listener.listener_data,
                                          key, sample->_sample);
    }

    if ((sample->sample_state == DDS_NOT_READ_SAMPLE_STATE) &&
        (self->_listener.on_sample_lost != NULL))
    {
        self->_listener.on_sample_lost((struct DDSHST_Reader*)self,
                                        self->_listener.listener_data);
    }

    sample->_sample = NULL;
    REDA_BufferPool_return_buffer(self->_sample_pool,sample);

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RHSM_History_delete_sample_list(struct RHSM_History *self,
                                REDA_CircularList_T *samples)
{
    struct RHSM_HistorySample *sample = NULL;
    struct RHSM_HistorySample *sample2 = NULL;

    sample = (struct RHSM_HistorySample*)REDA_CircularList_get_first(samples);
    while (!REDA_CircularList_node_at_head(samples,&sample->_node))
    {
        sample2 = (struct RHSM_HistorySample*)REDA_CircularListNode_get_next(
                                                                &sample->_node);
        if (!RHSM_History_free_or_delete_sample(self,
                                                &sample->_info.instance_handle,
                                                sample))
        {
            return RTI_FALSE;
        }
        sample = sample2;
    }
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RHSM_History_delete_key(struct RHSM_History *self,
                        struct RHSM_HistoryKeyEntry *key)
{
    if (!RHSM_History_delete_sample_list(self,&key->_samples))
    {
        return RTI_FALSE;
    }

    if (self->_listener.on_key_removed)
    {
        self->_listener.on_key_removed((struct DDSHST_Reader *)self,
                                       self->_listener.listener_data,
                                       &key->_key);
    }

    REDA_CircularList_unlink_node(
                        (struct REDA_CircularListNode*)&key->_deadline_entry);
    REDA_CircularList_unlink_node(&key->_node);
    REDA_Indexer_remove_entry(self->_key_index,&key->_key);
    REDA_BufferPool_return_buffer(self->_key_pool,key);

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RHSM_History_delete_rw(struct RHSM_History *self,
                       struct RHSM_HistoryRWEntry *rw)
{
    if (!RHSM_History_delete_sample_list(self,&rw->_samples))
    {
        return RTI_FALSE;
    }

    REDA_CircularList_unlink_node(&rw->_node);
    REDA_Indexer_remove_entry(self->_rw_index,&rw->_key);
    REDA_BufferPool_return_buffer(self->_rw_pool,rw);

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RHSM_History_prune_samples(struct RHSM_History *self,
                           struct RHSM_HistoryKeyEntry *const key_entry)
{
    struct RHSM_HistorySample *entry1;
    RTI_BOOL retval = RTI_FALSE;

    /* PRECONDITION: samples' linked-list in sorted order */
    entry1 = (struct RHSM_HistorySample *)REDA_CircularList_get_first(
                                                        &key_entry->_samples);
    while (!REDA_CircularList_node_at_head(&key_entry->_samples,&entry1->_node))
    {
        if (entry1->_loan_count == 0)
        {
            (void)RHSM_History_free_or_delete_sample(
               self, &key_entry->_key, entry1);
            --key_entry->_size;
            retval = RTI_TRUE;
            goto done;
        }
        entry1 = (struct RHSM_HistorySample *)REDA_CircularListNode_get_next(
                                                             &entry1->_node);
    }

done:
    return retval;
}

RTI_PRIVATE void
RHSM_History_update_committed(struct RHSM_History *self)
{
    if (self->_listener.on_data_available)
    {
        self->_listener.on_data_available((struct DDSHST_Reader *)self,
                                          self->_listener.listener_data,
                                          NULL, NULL);
    }
}

RTI_PRIVATE void
RHSM_History_add_meta_sample(struct RHSM_History *self,
                             struct RHSM_HistoryKeyEntry *key,
                             const DDS_InstanceHandle_t *publisher_handle,
                             DDS_InstanceStateKind instance_state,
                             struct OSAPI_NtpTime *now)
{
    struct DDS_SampleInfo *sample_info;
    struct RHSM_HistorySample *new_entry = NULL;
    DDS_Boolean at_max_history_depth;

    sample_info = NULL;

    at_max_history_depth = 
        (self->_qos->history.kind == DDS_KEEP_LAST_HISTORY_QOS &&
         key->_size == self->_qos->history.depth) ?
        DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;

    if (at_max_history_depth || 
        (key->_size == self->_qos->resource_limits.max_samples_per_instance))
    {
        if (!RHSM_History_prune_samples(self, key))
        {
            return;
        }
    }

    new_entry = REDA_BufferPool_get_buffer(self->_sample_pool);
    if (new_entry == NULL)
    {
        return;
    }

    new_entry->_key_entry = key;
    new_entry->_pend_rw_entry = NULL;
    new_entry->_ordered_next = NULL;
    new_entry->_ordered_prev = NULL;
    REDA_CircularListNode_init(&new_entry->_node);
    new_entry->_sample = NULL;
    new_entry->_flags = 0;

    ++key->_size;

    sample_info = &new_entry->_info;

    sample_info->sample_state = DDS_NOT_READ_SAMPLE_STATE;
    sample_info->view_state = key->_view_state;
    key->_instance_state = instance_state;
    sample_info->instance_state = key->_instance_state;
    new_entry->sample_state = sample_info->sample_state;

    OSAPI_NtpTime_unpackToNanosec(&sample_info->source_timestamp.sec,
                                 &sample_info->source_timestamp.nanosec, now);

    sample_info->instance_handle = key->_key;
    sample_info->instance_handle.is_valid = DDS_BOOLEAN_TRUE;
    sample_info->publication_handle = *publisher_handle;
    sample_info->publication_handle.is_valid = DDS_BOOLEAN_TRUE;

    /* None of the counters are supported */
    sample_info->disposed_generation_count = 0;
    sample_info->no_writers_generation_count = 0;
    sample_info->sample_rank = 0;
    sample_info->generation_rank = 0;
    sample_info->absolute_generation_rank = 0;

    /* This is a meta-sample */
    sample_info->valid_data = DDS_BOOLEAN_FALSE;

    OSAPI_NtpTime_unpackToNanosec(&sample_info->reception_timestamp.sec,
                                 &sample_info->reception_timestamp.nanosec,
                                 now);

    REDA_CircularList_append(&key->_samples,&new_entry->_node);

    RHSM_History_insert_ordered_sample_after(self->_s_ordered._ordered_prev,
                                             new_entry);

    RHSM_History_update_committed(self);
}

RTI_PRIVATE struct RHSM_HistoryRWEntry*
RHSM_History_find_rw(struct RHSM_History *self,
                     const DDS_InstanceHandle_t *const rw)
{
    return (struct RHSM_HistoryRWEntry*)REDA_Indexer_find_entry(
                                                        self->_rw_index,rw);
}

RTI_PRIVATE void
RHSM_History_remove_remote_writer(struct RHSM_History *self,
                                  struct RHSM_HistoryKeyEntry *key_entry,
                                  RTI_INT32 index)
{
    RTI_INT32 i;

    if (index < (key_entry->_writer_count-1))
    {
        /* move all indices down one spot */
        for (i = index; i < (key_entry->_writer_count-1); i++)
        {
            key_entry->_writers[i] = key_entry->_writers[i+1];
        }
        if (key_entry->_writers[i] == key_entry->_current_owner)
        {
            --key_entry->_writers[i]->_ownership_count;
        }
        key_entry->_writers[i] = NULL;
    }
    else
    {
        /* delete the last index */
        if (key_entry->_writers[index] == key_entry->_current_owner)
        {
            --key_entry->_writers[index]->_ownership_count;
        }
        key_entry->_writers[index] = NULL;
    }

    --key_entry->_writer_count;
}

RTI_PRIVATE void
RHSM_History_add_remote_writer(struct RHSM_History *self,
                               struct RHSM_HistoryKeyEntry *key_entry,
                               struct RHSM_HistoryRWEntry *rw_entry,
                               RTI_INT32 index)
{
    RTI_INT32 i;

    if (index <= (key_entry->_writer_count-1))
    {
        for (i = key_entry->_writer_count; i > index; --i)
        {
            key_entry->_writers[i] = key_entry->_writers[i-1];
        }
    }

    key_entry->_writers[index] = rw_entry;
    ++key_entry->_writer_count;
}

/*
 * Handle liveliness detected on a data-writer. Liveliness detected on a writer
 * will cause instances written by that writer be to alive.
 */
RTI_PRIVATE void
RHSM_History_update_liveliness(struct RHSM_History *self,
                               const struct DDSHST_ReaderEvent *const event,
                               struct OSAPI_NtpTime *now)
{
    struct DDS_SampleInfo *sample_info;
    RTI_INT32 i, j, live_count;
    struct RHSM_HistoryRWEntry *rw_entry;
    RTI_INT32 active_instances;
    struct RHSM_HistoryKeyEntry *key_entry;
    RTI_INT32 owner_index = -1;

    rw_entry = (struct RHSM_HistoryRWEntry *)REDA_Indexer_find_entry(
                             self->_rw_index,&event->data.liveliness.rw_guid);
    if (rw_entry == NULL)
    {
        return;
    }

    sample_info = NULL;

    /* This is very inefficient, but since this is assumed to happen
     * infrequently, and we are concerned with memory footprint this
     * is a reasonable choice for this particular queue implementation.
     */
    active_instances = REDA_Indexer_get_count(self->_key_index);
    for (i = 0; i < active_instances; ++i)
    {
        key_entry = (struct RHSM_HistoryKeyEntry*)
                                    REDA_Indexer_get_entry(self->_key_index,i);

        if ((key_entry->_instance_state == DDS_ALIVE_INSTANCE_STATE) &&
             (event->kind == DDSHST_READEREVENT_KIND_LIVELINESS_DETECTED))
        {
            continue;
        }

        live_count = 0;
        for (j = 0; j < key_entry->_writer_count; ++j)
        {
            if ((event->kind == DDSHST_READEREVENT_KIND_LIVELINESS_DETECTED) &&
                DDS_InstanceHandle_equals(&event->data.liveliness.rw_guid,
                                          &key_entry->_writers[j]->_key))
            {
                if (((self->_qos->ownership.kind == DDS_EXCLUSIVE_OWNERSHIP_QOS)
                        && (rw_entry == key_entry->_current_owner)) ||
                      (self->_qos->ownership.kind == DDS_SHARED_OWNERSHIP_QOS))
                {
                    RHSM_History_add_meta_sample(
                            self,
                            key_entry,
                            &event->data.liveliness.rw_guid,
                            DDS_ALIVE_INSTANCE_STATE,
                            now);
                }
                break;
            }
            else if ((event->kind == DDSHST_READEREVENT_KIND_LIVELINESS_LOST) &&
                     DDS_InstanceHandle_equals(&event->data.liveliness.rw_guid,
                                                &key_entry->_writers[j]->_key))
            {
                owner_index = j;
                break;
            }
        }

        if ((owner_index != -1) &&
            (event->kind == DDSHST_READEREVENT_KIND_LIVELINESS_LOST))
        {
            RHSM_History_remove_remote_writer(self,key_entry,owner_index);
            if (key_entry->_writer_count > 0)
            {
                if (self->_qos->ownership.kind == DDS_EXCLUSIVE_OWNERSHIP_QOS)
                {
                    key_entry->_current_owner = key_entry->_writers[0];
                    ++key_entry->_current_owner->_ownership_count;
                }
            }
            else
            {
                key_entry->_current_owner = NULL;
                RHSM_History_add_meta_sample(self,
                        key_entry,
                        &event->data.liveliness.rw_guid,
                        DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE,
                        now);
            }
        }
        owner_index = -1;
    }
}

RTI_PRIVATE void
RHSM_History_prune_rw(struct DDSHST_Reader *rh,
                      struct RHSM_HistoryRWEntry *rw_entry,
                      RTI_BOOL init_key)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    RTI_INT32 i, j;
    RTI_INT32 active_instances;
    struct RHSM_HistoryKeyEntry *key_entry;
    RTI_BOOL is_owner = RTI_FALSE;
    struct OSAPI_NtpTime now;

    OSAPI_System_get_time(&now);

    /* This is very inefficient, but since this is assumed to happen
     * infrequently, and we are concerned with memory footprint this
     * is a reasonable choice for this particular queue implementation.
     */

    active_instances = REDA_Indexer_get_count(self->_key_index);
    for (i = 0; i < active_instances; ++i)
    {
        key_entry = (struct RHSM_HistoryKeyEntry*)
                                  REDA_Indexer_get_entry(self->_key_index,i);
        for (j = 0; j < key_entry->_writer_count; ++j)
        {
            if (rw_entry == key_entry->_writers[j])
            {
                is_owner = (rw_entry == key_entry->_current_owner ? RTI_TRUE : RTI_FALSE);
                RHSM_History_remove_remote_writer(self,key_entry,j);
                break;
            }
        }

        if ((rw_entry->_key.octet[15] & 0xc0) && init_key)
        {

            RHSM_History_delete_key(self, key_entry);
            active_instances = REDA_Indexer_get_count(self->_key_index);
        }

        if (key_entry->_writer_count > 0)
        {
            if (self->_qos->ownership.kind == DDS_EXCLUSIVE_OWNERSHIP_QOS)
            {
                if (is_owner)
                {
                    key_entry->_current_owner = key_entry->_writers[0];
                    ++key_entry->_current_owner->_ownership_count;
                }
            }
        }
        else
        {
            key_entry->_current_owner = NULL;
            RHSM_History_add_meta_sample(self,
                    key_entry,
                    &key_entry->_key,
                    DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE,
                    &now);
        }
    }

    RHSM_History_delete_rw(self,rw_entry);
}

RTI_PRIVATE RTI_BOOL
RHSM_History_prune_rw_instance(struct DDSHST_Reader *rh,RTI_INT32 rank)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    RTI_INT32 i;
    RTI_INT32 active_writers;
    RTI_INT32 active_instances;
    RTI_BOOL retval = RTI_FALSE;
    struct RHSM_HistoryRWEntry *rw_entry = NULL;

    active_writers = REDA_Indexer_get_count(self->_rw_index);
    active_instances = REDA_Indexer_get_count(self->_key_index);
    for (i = 0; i < active_writers; ++i)
    {
        rw_entry = (struct RHSM_HistoryRWEntry*)
                                    REDA_Indexer_get_entry(self->_rw_index,i);

        if (rw_entry->_ownership_count == 0)
        {
            RHSM_History_prune_rw(rh,rw_entry,RTI_FALSE);
            retval = RTI_TRUE;
            break;
        }
    }

    return retval;
}

/* TIMEOUTS:
 * NOTE: Timeouts are async and to other code paths and must thus protect
 * any data-structure access
 */
#ifdef ENABLE_QOS_DEADLINE
RTI_PRIVATE void
RHSM_History_on_deadline_expired(struct RHSM_History *self,struct OSAPI_NtpTime *now)
{
    struct RHSM_HistoryKeyEntry *key_entry;
    struct REDA_CircularListMixInNode *a_node;
    struct REDA_CircularListMixInNode *last_node;
    RTI_BOOL check_complete = RTI_FALSE;

    a_node = (struct REDA_CircularListMixInNode*)
                           REDA_CircularList_get_first(&self->_deadline_timer);

    if (a_node == (struct REDA_CircularListMixInNode *)
            REDA_CircularList_get_last(&self->_deadline_timer) &&
            !REDA_CircularList_node_at_head(&self->_deadline_timer,
                                    (struct REDA_CircularListNode *)a_node))
    {
        last_node = (struct REDA_CircularListMixInNode*)&self->_deadline_timer;
    }
    else if (!REDA_CircularList_node_at_head(&self->_deadline_timer,
                                    (struct REDA_CircularListNode*)a_node))
    {
        last_node = (struct REDA_CircularListMixInNode*)
                           REDA_CircularList_get_last(&self->_deadline_timer);
    }
    else
    {
        return;
    }

    while (!check_complete)
    {
        key_entry = REDA_CircularListMixInNode_get_data(a_node);
        if (key_entry->last_update_period == (self->_current_period+1))
        {
            break;
        }
        if (self->_qos->ownership.kind == DDS_EXCLUSIVE_OWNERSHIP_QOS)
        {
            /* Check if there are still writers in case the writer was
             * removed due to loss of liveliness
             */
            if (key_entry->_writer_count > 0)
            {
                RHSM_History_remove_remote_writer(self,key_entry,0);
            }

            if (key_entry->_writer_count > 0)
            {
                key_entry->_current_owner = key_entry->_writers[0];
                ++key_entry->_current_owner->_ownership_count;
            }
            else
            {
                key_entry->_current_owner = NULL;
            }
        }
        if (key_entry->_writer_count == 0)
        {
            RHSM_History_add_meta_sample(self,
                    key_entry,&key_entry->_key,
                    DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE,now);
        }
        ++key_entry->last_update_period;
        if (self->_listener.on_deadline_missed)
        {
            self->_listener.on_deadline_missed((struct DDSHST_Reader*)self,
                               self->_listener.listener_data,&key_entry->_key);
        }
        if (a_node == last_node)
        {
            check_complete = RTI_TRUE;
            REDA_CircularList_unlink_node(
                 (struct REDA_CircularListNode *)&key_entry->_deadline_entry);

            REDA_CircularList_append(&self->_deadline_timer,
                 (struct REDA_CircularListNode *)&key_entry->_deadline_entry);
        }
        else
        {
            a_node = (struct REDA_CircularListMixInNode *)
                                        REDA_CircularListNode_get_next(a_node);
            if (a_node == (struct REDA_CircularListMixInNode*)
                                                        &self->_deadline_timer)
            {
                check_complete = RTI_TRUE;
            }
        }
    }
    ++self->_current_period;
}
#endif

/*******************************************************************************
 *                                Public API
 ******************************************************************************/
RTI_PRIVATE void
RHSM_History_delete(struct RHSM_History *self)
{
    struct RHSM_HistoryKeyEntry *key_entry1,*key_entry2;
    struct RHSM_HistoryRWEntry *rw_entry1,*rw_entry2;

    key_entry1 = (struct RHSM_HistoryKeyEntry*)
                             REDA_CircularList_get_first(&self->_keys_in_use);

    while (!REDA_CircularList_node_at_head(&self->_keys_in_use,&key_entry1->_node))
    {
        key_entry2 = (struct RHSM_HistoryKeyEntry*)
                            REDA_CircularListNode_get_next(&key_entry1->_node);

        if (!RHSM_History_delete_key(self, key_entry1))
        {
            RHSM_Log_error(LOG_OUTSTANDING_SAMPLES);
            return;
        }
        key_entry1 = key_entry2;
    }

    if (self->_key_pool != NULL)
    {
        if (!REDA_BufferPool_delete(self->_key_pool))
        {
            RHSM_Log_error(LOG_OUTSTANDING_SAMPLES);
            return;
        }
    }

    rw_entry1 = (struct RHSM_HistoryRWEntry*)
                                REDA_CircularList_get_first(&self->_rw_in_use);

    while (!REDA_CircularList_node_at_head(&self->_rw_in_use,&rw_entry1->_node))
    {
        rw_entry2 = (struct RHSM_HistoryRWEntry*)REDA_CircularListNode_get_next(
                                                        &rw_entry1->_node);

        if (!RHSM_History_delete_rw(self, rw_entry1))
        {
            RHSM_Log_error(LOG_OUTSTANDING_SAMPLES);
            return;
        }
        rw_entry1 = rw_entry2;
    }

    if (self->_rw_pool != NULL)
    {
        if (!REDA_BufferPool_delete(self->_rw_pool))
        {
            return;
        }
    }

    if (self->_sample_pool != NULL)
    {
        if (!REDA_BufferPool_delete(self->_sample_pool))
        {
            return;
        }
    }

    if (self->_key_index != NULL)
    {
        if (!REDA_Indexer_delete(self->_key_index))
        {
            return;
        }
    }

    if (self->_rw_index != NULL)
    {
        if (!REDA_Indexer_delete(self->_rw_index))
        {
            return;
        }
    }

    if (self->_sample_info_pool != NULL)
    {
        REDA_BufferPool_delete(self->_sample_info_pool);
    }

    if (self->_sample_ptr_pool != NULL)
    {
        REDA_BufferPool_delete(self->_sample_ptr_pool);
    }

    OSAPI_Heap_freeStructure(self);
}

RTI_BOOL
RHSM_HistoryKeyEntry_initialize(void *initialize_param, void *buffer)
{
    struct RHSM_History *history = (struct RHSM_History*)initialize_param;
    struct RHSM_HistoryKeyEntry *key_entry = (struct RHSM_HistoryKeyEntry*)buffer;
    RTI_INT32 j;

    OSAPI_Heap_allocateArray(&key_entry->_writers,
        history->_qos->reader_resource_limits.max_remote_writers_per_instance,
        struct RHSM_HistoryRWEntry*);

    if (key_entry->_writers == NULL)
    {
        return RTI_FALSE;
    }

    RHSM_History_initialize_key(history, key_entry);

    for (j = 0; j < history->_qos->reader_resource_limits.max_remote_writers_per_instance; ++j)
    {
        key_entry->_writers[j] = NULL;
    }

    return RTI_TRUE;
}

RTI_BOOL
RHSM_HistoryKeyEntry_finalize(void *initialize_param, void *buffer)
{
    struct RHSM_HistoryKeyEntry *key_entry = (struct RHSM_HistoryKeyEntry*)buffer;

    if (key_entry->_writers != NULL)
    {
        OSAPI_Heap_freeArray(key_entry->_writers);
    }
    return RTI_TRUE;
}

RTI_PRIVATE struct RHSM_History*
RHSM_History_create(const struct RHSM_HistoryProperty *const property,
                    const struct DDSHST_ReaderListener *const listener)
{
    struct RHSM_History *history = NULL;
    struct RHSM_History *retval = NULL;
    const struct DDS_DataReaderQos *qos;
    DDS_Long read_seq_max;
    struct REDA_BufferPoolProperty pool_property =
                                        REDA_BufferPoolProperty_INITIALIZER;
    struct REDA_IndexerProperty index_prop = REDA_IndexerProperty_INITIALIZER;

    qos = property->_parent.qos;

    if (qos == NULL)
    {
        RHSM_Log_error(LOG_RHSM_PROPERTY_NOT_INITIALIZED);
        goto done;
    }

    if (qos->history.kind != DDS_KEEP_LAST_HISTORY_QOS)
    {
        RHSM_Log_error(LOG_KEEP_ALL_HISTORY_NOT_SUPPORTED);
        goto done;
    }

    if ((qos->resource_limits.max_instances == DDS_LENGTH_UNLIMITED) ||
        (qos->resource_limits.max_samples == DDS_LENGTH_UNLIMITED) ||
        (qos->resource_limits.max_samples_per_instance == DDS_LENGTH_UNLIMITED))
    {
        RHSM_Log_error(LOG_UNLIMITED_HISTORY_NOT_SUPPORTED);
        goto done;
    }

    if (qos->resource_limits.max_samples <
        (qos->resource_limits.max_instances
         * qos->resource_limits.max_samples_per_instance))
    {
        RHSM_Log_error(
           LOG_MAX_SAMPLES_LESS_THAN_MAX_INSTANCES_TIMES_MAX_SAMPERINST);
        goto done;
    }

    if (!DDS_Duration_is_zero(&qos->time_based_filter.minimum_separation))
    {
        RHSM_Log_error(LOG_FINITE_TIME_BASED_FILTER_NOT_SUPPORTED);
        goto done;
    }

    OSAPI_Heap_allocateStructure(&history, struct RHSM_History);

    if (history == NULL)
    {
        goto done;
    }

    RT_Component_initialize(&history->_parent._parent,
                           &RHSM_HistoryI_g_intf._parent,
                           0,
                           (property ? &property->_parent._parent : NULL),
                           (listener ? &listener->_parent : NULL));

    history->_qos = qos;

    if (property)
    {
        history->_property = *(struct RHSM_HistoryProperty*)property;
    }
    else
    {
        history->_property = RTI_DEFAULT_SMREADERHISTORY_PROPERTY;
    }

    if (listener)
    {
        history->_listener = *listener;
    }
    else
    {
        history->_listener = RTI_DEFAULT_READERHISTORY_LISTENER;
    }

    RHSM_History_init_ordered_list(&history->_s_ordered);

    pool_property.buffer_size = sizeof(struct RHSM_HistoryRWEntry);
    pool_property.max_buffers =
                history->_qos->reader_resource_limits.max_remote_writers;

    history->_rw_pool = REDA_BufferPool_new(&pool_property,NULL,NULL,NULL,NULL);
    if (history->_rw_pool == NULL)
    {
        goto done;
    }

    index_prop.max_entries = history->_qos->reader_resource_limits.max_remote_writers;
    history->_rw_index = REDA_Indexer_new(RHSM_History_compare_rw_record,NULL,&index_prop);
    if (history->_rw_index == NULL)
    {
        goto done;
    }

    pool_property.buffer_size = sizeof(struct RHSM_HistoryKeyEntry);
    pool_property.max_buffers = history->_qos->resource_limits.max_instances;
    history->_key_pool = REDA_BufferPool_new(&pool_property,
            RHSM_HistoryKeyEntry_initialize,(void*)history,
            RHSM_HistoryKeyEntry_finalize,(void*)history);
    if (history->_key_pool == NULL)
    {
        goto done;
    }

    index_prop.max_entries = history->_qos->resource_limits.max_instances;
    history->_key_index = REDA_Indexer_new(RHSM_History_compare_key_record,NULL,&index_prop);
    if (history->_key_index == NULL)
    {
        goto done;
    }

    pool_property.buffer_size = sizeof(struct RHSM_HistorySample);
    pool_property.max_buffers = history->_qos->resource_limits.max_samples;
    history->_sample_pool = REDA_BufferPool_new(&pool_property,NULL,NULL,NULL,NULL);
    if (history->_sample_pool == NULL)
    {
        goto done;
    }

    REDA_CircularList_init(&history->_keys_in_use);
    REDA_CircularList_init(&history->_rw_in_use);
    REDA_CircularList_init(&history->_deadline_timer);

    OSAPI_Memory_zero(&history->rejected_status,sizeof(history->rejected_status));
    OSAPI_Memory_zero(&history->lost_status,sizeof(history->lost_status));

    history->_info.version = RHSM_HISTORY_VERSION;

    /* pool of SampleInfo's, for read/take with loans */
    if (qos->history.kind == DDS_KEEP_ALL_HISTORY_QOS)
    {
        /* NOTE: assumes finite max_samples */
        read_seq_max = qos->resource_limits.max_samples;
    }
    else 
    {
        /* KEEP_LAST */
        /* PRECONDITION: finite max_instances */
        read_seq_max = 
            (qos->resource_limits.max_instances * qos->history.depth);
    }

    /* allocate internal sample and sample_info sequences */
    pool_property.max_buffers = 
        qos->reader_resource_limits.max_outstanding_reads;

    /* buffers of loaned sample infos */
    pool_property.buffer_size = sizeof(struct DDS_SampleInfo) * read_seq_max;
    history->_sample_info_pool =
        REDA_BufferPool_new(&pool_property, NULL, NULL, NULL, NULL);
    if (history->_sample_info_pool == NULL)
    {
        /* TODO: pool creation error */
        goto done;
    }

    /* buffers of pointers to loaned sample */
    pool_property.buffer_size = sizeof(void *) * read_seq_max;
    history->_sample_ptr_pool =
        REDA_BufferPool_new(&pool_property, NULL, NULL, NULL, NULL);
    if (history->_sample_ptr_pool == NULL)
    {
        /* TODO: pool creation error */
        goto done;
    }

#ifdef ENABLE_QOS_DEADLINE
    if (!DDS_Duration_is_infinite(&history->_qos->deadline.period))
    {
        history->deadline_enabled = RTI_TRUE;
    }
    else
    {
        history->deadline_enabled = RTI_FALSE;
    }
#else
    history->deadline_enabled = RTI_FALSE;
#endif

  retval = history;

  done:

    if ((retval == NULL) && (history != NULL))
    {
        RHSM_History_delete(history);
    }

    return retval;
}

RTI_PRIVATE struct RHSM_HistoryKeyEntry *
RHSM_History_find_key(struct RHSM_History *self,
                      const DDS_InstanceHandle_t *const key)
{
    return (struct RHSM_HistoryKeyEntry*)REDA_Indexer_find_entry(
                                                        self->_key_index,key);
}

RTI_PRIVATE DDSHST_ReaderKeyEntryRef_T
RHSM_History_lookup_key(struct DDSHST_Reader *rh,
                        const DDS_InstanceHandle_t *const key)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    return (DDSHST_ReaderKeyEntryRef_T)RHSM_History_find_key(self, key);
}

RTI_PRIVATE RTI_BOOL
RHSM_History_writer_is_stronger(DDS_InstanceHandle_t *lw,DDS_Long ls,
                                DDS_InstanceHandle_t *rw,DDS_Long rs)
{
    return (ls > rs) || ((ls == rs) && DDS_InstanceHandle_compare(lw,rw) < 0);
}


RTI_PRIVATE DDSHST_ReaderSampleEntryRef_T
RHSM_History_get_entry(struct DDSHST_Reader *rh,
                              DDS_InstanceHandle_t *rw,
                              DDS_InstanceHandle_t *key,
                              struct DDS_SampleInfo **sample_info,
                              DDS_Long strength)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    struct RHSM_HistorySample *new_entry = NULL;
    struct RHSM_HistoryKeyEntry *key_entry = NULL;
    struct RHSM_HistoryRWEntry *rw_entry = NULL;
    struct RHSM_HistoryRWEntry *kicked_rw_entry = NULL;
    RTI_INT32 i;
    RTI_BOOL key_allocated = RTI_FALSE;
    RTI_BOOL rw_allocated = RTI_FALSE;
    RTI_BOOL allocate_writer = RTI_FALSE;
    RTI_BOOL is_strongest = RTI_FALSE;
    DDS_Boolean at_max_history_depth;
    RTI_INT32 writer_index = -1;
    RTI_BOOL writers_full = RTI_FALSE;
    RTI_BOOL writer_found = RTI_FALSE;
    RTI_INT32 writer_found_index = -1;
    DDS_SampleRejectedStatusKind last_reason;

    LOG_printDebug("get_entry for %d/%d/%d\n",rw->octet[0],strength,key->octet[0]);

    last_reason = DDS_NOT_REJECTED;
    key_entry = RHSM_History_find_key(self, key);
    if (key_entry == NULL)
    {
        key_entry = (struct RHSM_HistoryKeyEntry*)REDA_BufferPool_get_buffer(
                                                            self->_key_pool);
        if (key_entry == NULL)
        {
            last_reason = DDS_REJECTED_BY_INSTANCES_LIMIT;
            goto failure;
        }
        RHSM_History_initialize_key(self, key_entry);
        key_entry->_key = *key;
        key_entry->_view_state = DDS_NEW_VIEW_STATE;
        key_allocated = RTI_TRUE;
        REDA_CircularList_append(&self->_deadline_timer,
                  (struct REDA_CircularListNode *)&key_entry->_deadline_entry);
    }

    rw_entry = RHSM_History_find_rw(self, rw);

    writers_full =
         (key_entry->_writer_count == self->_qos->reader_resource_limits.max_remote_writers_per_instance ?
                 RTI_TRUE : RTI_FALSE);

    if (self->_qos->ownership.kind == DDS_SHARED_OWNERSHIP_QOS)
    {
        if ((rw_entry == NULL) && !writers_full)
        {
            writer_index = key_entry->_writer_count;
            allocate_writer = RTI_TRUE;
        }
        else if (rw_entry != NULL)
        {
            for (i = 0; i <  key_entry->_writer_count; i++)
            {
                if (key_entry->_writers[i] == rw_entry)
                {
                    break;
                }
            }
            if ((i == key_entry->_writer_count) && !writers_full)
            {
                writer_index = key_entry->_writer_count;
            }
        }
    }
    else if ((rw_entry == NULL) || (key_entry->_current_owner != rw_entry))
    {
        /* To get here:
         * Writer either found or not, check if it is the strongest or
         * stronger than an existing writer and we need to take action
         */
        LOG_printDebug("check owner: %d\n");
        for (i = 0; i < key_entry->_writer_count; ++i)
        {
            LOG_printDebug("%d, %d\n",
                    key_entry->_writers[i]->_key.octet[0],
                    key_entry->_writers[i]->_strength);
        }
        if (key_entry->_writer_count == 0)
        {
            writer_index = 0;
            is_strongest = RTI_TRUE;
            allocate_writer = RTI_TRUE;
        }
        else
        {
            is_strongest = RTI_FALSE;
            for (i = 0; i <  key_entry->_writer_count; i++)
            {
                if ((writer_index == -1) &&
                        RHSM_History_writer_is_stronger(rw,strength,
                         &key_entry->_writers[i]->_key,
                         key_entry->_writers[i]->_strength))
                {
                    LOG_printDebug("found stronger writer [%d/%d] at %d\n",
                            rw->octet[0],strength,i);
                    writer_index = i;
                    if (rw_entry == NULL)
                    {
                        LOG_printDebug("allocate writer\n");
                        allocate_writer = RTI_TRUE;
                        is_strongest = RTI_TRUE;
                        break;
                    }
                }

                if ((rw_entry != NULL) && (key_entry->_writers[i] == rw_entry))
                {
                    writer_found = RTI_TRUE;
                    writer_found_index = i;
                    LOG_printDebug("writer found @ %d\n",writer_found_index);
                }
            }

            if (writer_found && (writer_index != -1))
            {
                if (writer_found_index > writer_index)
                {
                    RHSM_History_remove_remote_writer(self,key_entry,
                                                      writer_found_index);
                }
                else
                {
                    writer_index = -1;
                }
            }

            LOG_printDebug("writer index = %d\n",writer_index);

            /* If it is not the strongest, but there is room, keep it
             * around anyway
             */
            if ((!writers_full) && !allocate_writer &&
                (rw_entry == NULL) && !writer_found)
            {
                writer_index = key_entry->_writer_count;
                allocate_writer = RTI_TRUE;
                LOG_printDebug("allocate entry anyway @ %d\n",writer_index);
            }
        }
    }

    if ((rw_entry == NULL) && allocate_writer)
    {
        rw_entry = REDA_BufferPool_get_buffer(self->_rw_pool);
        if (rw_entry == NULL)
        {
            if (is_strongest && RHSM_History_prune_rw_instance(rh,writer_index))
            {
                rw_entry = REDA_BufferPool_get_buffer(self->_rw_pool);
            }
        }
        if (rw_entry != NULL)
        {
            RHSM_HistoryRWEntry_initialize(rw_entry,rw);
            rw_allocated = RTI_TRUE;
            rw_entry->_strength = strength;
        }
        else
        {
            last_reason = DDS_REJECTED_BY_REMOTE_WRITERS_LIMIT;
            goto failure;
        }
    }
    else if (rw_entry == NULL)
    {
        last_reason = DDS_REJECTED_BY_REMOTE_WRITERS_PER_INSTANCE_LIMIT;
        goto failure;
    }

    /* At this point, key_entry and rw_entry are both good */
    if (writer_index != -1)
    {
        if (self->_qos->ownership.kind == DDS_SHARED_OWNERSHIP_QOS)
        {
            key_entry->_writers[writer_index] = rw_entry;
            ++key_entry->_writer_count;
        }
        else
        {
            LOG_printDebug("ownership update\n");
            if (key_entry->_writer_count == self->_qos->reader_resource_limits.max_remote_writers_per_instance)
            {
                kicked_rw_entry = key_entry->_writers[key_entry->_writer_count-1];
                LOG_printDebug("kick out writer %d\n",kicked_rw_entry->_key.octet[0]);
                RHSM_History_prune_rw(rh,kicked_rw_entry,RTI_TRUE);
            }
            RHSM_History_add_remote_writer(self,key_entry,rw_entry,writer_index);
            rw_entry->_strength = strength;
            if (writer_index == 0)
            {
                if (key_entry->_current_owner != NULL)
                {
                    --key_entry->_current_owner->_ownership_count;
                }
                key_entry->_current_owner = rw_entry;
                ++rw_entry->_ownership_count;
                LOG_printDebug("new strongest writer for %d: %d/%d/%d,\n",
                        key_entry->_key.octet[0],
                        rw_entry->_key.octet[0],
                        rw_entry->_ownership_count,
                        rw_entry->_strength);
            }
            LOG_printDebug("current ownership for: %d\n",key_entry->_key.octet[0]);
            for (i = 0; i < key_entry->_writer_count; ++i)
            {
                LOG_printDebug("%d, %d\n",
                        key_entry->_writers[i]->_key.octet[0],
                        key_entry->_writers[i]->_strength);
            }
        }
    }

    at_max_history_depth = 
        (self->_qos->history.kind == DDS_KEEP_LAST_HISTORY_QOS &&
         key_entry->_size == self->_qos->history.depth) ?
        DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;

    if (at_max_history_depth || 
        (key_entry->_size ==
         self->_qos->resource_limits.max_samples_per_instance))
    {
        if (!RHSM_History_prune_samples(self, key_entry))
        {
            last_reason = DDS_REJECTED_BY_SAMPLES_PER_INSTANCE_LIMIT;
            goto failure;
        }
    }

    new_entry = REDA_BufferPool_get_buffer(self->_sample_pool);
    if (new_entry == NULL)
    {
        last_reason = DDS_REJECTED_BY_SAMPLES_LIMIT;
        goto failure;
    }

    if (key_allocated)
    {
        REDA_CircularList_prepend(&self->_keys_in_use,&key_entry->_node);
        REDA_Indexer_add_entry(self->_key_index,key_entry);
    }
    if (rw_allocated)
    {
        REDA_CircularList_prepend(&self->_rw_in_use,&rw_entry->_node);
        REDA_Indexer_add_entry(self->_rw_index,rw_entry);
    }

    new_entry->_key_entry = key_entry;
    new_entry->_pend_rw_entry = rw_entry;
    new_entry->_loan_count = 0;
    new_entry->_flags = 0;
    REDA_CircularListNode_init(&new_entry->_node);
    new_entry->_ordered_next = NULL;
    new_entry->_ordered_prev = NULL;
    ++key_entry->_size;
    ++rw_entry->_size;
    new_entry->_sample = NULL;

    new_entry->_info.instance_handle = *key;
    new_entry->_info.publication_handle = *rw;

    *sample_info = &new_entry->_info;

    return (DDSHST_ReaderSampleEntryRef_T) new_entry;

failure:

    if (self->_listener.on_sample_rejected)
    {
        self->_listener.on_sample_rejected(
                        rh,self->_listener.listener_data,key,last_reason);
    }

    if (key_allocated)
    {
        REDA_BufferPool_return_buffer(self->_key_pool,key_entry);
    }

    if (rw_allocated)
    {
        REDA_BufferPool_return_buffer(self->_rw_pool,rw_entry);
    }

    return NULL;
}

RTI_PRIVATE void
RHSM_History_return_entry(struct DDSHST_Reader *rh,
                                 DDSHST_ReaderSampleEntryRef_T entry)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    struct RHSM_HistorySample *new_entry = (struct RHSM_HistorySample *)entry;

    --new_entry->_key_entry->_size;
    REDA_BufferPool_return_buffer(self->_sample_pool,new_entry);
}

RTI_PRIVATE DDSHST_ReturnCode_T
RHSM_History_commit_entry(struct DDSHST_Reader *rh,
                                 DDSHST_ReaderSampleEntryRef_T entry,
                                 DDSHST_ReaderSample_T *sample,
                                 struct REDA_SequenceNumber *sn)
{
    struct RHSM_HistorySample *new_entry =
                                    (struct RHSM_HistorySample *)entry;
    struct RHSM_HistorySample *s1_entry;
    struct RHSM_HistoryRWEntry *rw_entry = NULL;

    rw_entry = new_entry->_pend_rw_entry;
    new_entry->_sn = *sn;
    new_entry->_info.reception_sequence_number =
                                            *(struct DDS_SequenceNumber_t *)sn;
    new_entry->_sample = sample;

    if (REDA_CircularList_is_empty(&rw_entry->_samples))
    {
        REDA_CircularList_append(&rw_entry->_samples,&new_entry->_node);
        return DDSHST_RETCODE_SUCCESS;
    }

    s1_entry = (struct RHSM_HistorySample*)REDA_CircularList_get_last(
                                                        &rw_entry->_samples);
    if (REDA_SequenceNumber_compare(&s1_entry->_sn, sn) > 0)
    {
        s1_entry = (struct RHSM_HistorySample*)REDA_CircularList_get_first(
                                                        &rw_entry->_samples);
        while (!REDA_CircularList_node_at_head(&rw_entry->_samples,&s1_entry->_node))
        {
            if (REDA_SequenceNumber_compare(&s1_entry->_sn, sn) > 0)
            {
                break;
            }
            s1_entry = (struct RHSM_HistorySample*)
                              REDA_CircularListNode_get_next(&s1_entry->_node);
        }
        REDA_CircularList_append(&s1_entry->_node,&new_entry->_node);
    }
    else
    {
        REDA_CircularList_link_node_after(REDA_CircularList_get_last(
                &rw_entry->_samples),&new_entry->_node);
    }

    return DDSHST_RETCODE_SUCCESS;
}

RTI_PRIVATE DDS_Boolean
RHSM_History_writer_is_owner(struct RHSM_History *self,
                             struct RHSM_HistoryKeyEntry *key_entry,
                             struct RHSM_HistoryRWEntry *rw_entry,
                             DDS_Long strength)
{
    if (self->_property._parent.qos->ownership.kind == DDS_SHARED_OWNERSHIP_QOS)
    {
        return DDS_BOOLEAN_TRUE;

    }

    if (key_entry->_current_owner == rw_entry)
    {
        return DDS_BOOLEAN_TRUE;
    }

    return DDS_BOOLEAN_FALSE;
}

RTI_PRIVATE DDSHST_ReturnCode_T
RHSM_History_commit_sample(struct DDSHST_Reader *rh,
                                  DDS_InstanceHandle_t *rw,
                                  struct REDA_SequenceNumber *sn,
                                  DDS_Long strength)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    struct RHSM_HistoryRWEntry *rw_entry = NULL;
    struct RHSM_HistorySample *s1_entry, *s2_entry;
    DDS_Boolean new_samples = DDS_BOOLEAN_FALSE;
    DDS_Boolean is_owner;

    /* Commit all samples [oldest,sn> for the specific writer
     * Committing samples means moving samples into the sample committed list,
     * and ordered according to the presentation and destination order qos
     */
    rw_entry = RHSM_History_find_rw(self, rw);

    if (rw_entry == NULL)
    {
        return DDSHST_RETCODE_SUCCESS;
    }

    rw_entry->_strength = strength;
    s1_entry = (struct RHSM_HistorySample*)REDA_CircularList_get_first(
                                                        &rw_entry->_samples);

    while (!REDA_CircularList_node_at_head(&rw_entry->_samples,&s1_entry->_node))
    {
        s2_entry = (struct RHSM_HistorySample*)REDA_CircularListNode_get_next(
                                                            &s1_entry->_node);
        if (REDA_SequenceNumber_compare(&s1_entry->_sn, sn) < 0)
        {
            REDA_CircularList_unlink_node(&s1_entry->_node);
            --rw_entry->_size;
            /* Determine if this writer is indeed the strongest writer for
             * this instance
             */
            is_owner = RHSM_History_writer_is_owner(
                    self,s1_entry->_key_entry,rw_entry,strength);
            if (is_owner)
            {
                s1_entry->sample_state = DDS_NOT_READ_SAMPLE_STATE;
                s1_entry->_sample->_info->disposed_generation_count = 0;
                s1_entry->_sample->_info->no_writers_generation_count = 0;
                s1_entry->_sample->_info->sample_rank = 0;
                s1_entry->_sample->_info->generation_rank = 0;
                s1_entry->_sample->_info->absolute_generation_rank = 0;
                if (self->deadline_enabled &&
                    (s1_entry->_key_entry->last_update_period == self->_current_period))
                {
                    REDA_CircularList_unlink_node(
                            (struct REDA_CircularListNode *)&s1_entry->_key_entry->_deadline_entry);
                    REDA_CircularList_append(&self->_deadline_timer,
                        (struct REDA_CircularListNode *)&s1_entry->_key_entry->_deadline_entry);
                    s1_entry->_key_entry->last_update_period = self->_current_period + 1;
                }
                s1_entry->_key_entry->_instance_state = DDS_ALIVE_INSTANCE_STATE;
                s1_entry->_sample->_ref = (DDSHST_ReaderSampleEntryRef_T)s1_entry;
                REDA_CircularList_append(&s1_entry->_key_entry->_samples, &s1_entry->_node);
                RHSM_History_insert_ordered_sample_after(self->_s_ordered._ordered_prev,s1_entry);
                s1_entry->_pend_rw_entry = NULL;
                new_samples = DDS_BOOLEAN_TRUE;
            }
            else
            {
                if (!RHSM_History_free_or_delete_sample(self,
                                    &s1_entry->_key_entry->_key,
                                    s1_entry))
                {

                }
                --s1_entry->_key_entry->_size;
            }
        }
        s1_entry = s2_entry;
    }

    if (new_samples)
    {
        RHSM_History_update_committed(self);
    }

    return DDSHST_RETCODE_SUCCESS;
}

RTI_PRIVATE DDSHST_ReturnCode_T
RHSM_History_register_key(struct DDSHST_Reader * rh,
                                 DDS_InstanceHandle_t * key)
{
    return DDSHST_RETCODE_ERROR;
}

RTI_PRIVATE DDSHST_ReturnCode_T
RHSM_History_unregister_key(struct DDSHST_Reader *rh,
                            struct DDS_SampleInfo *sample_info)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    struct RHSM_HistoryKeyEntry *key_entry;
    struct RHSM_HistoryRWEntry *rw_entry;
    RTI_INT32 i;
    struct OSAPI_NtpTime now;

    OSAPI_NtpTime_packFromNanosec(&now,
                                 sample_info->reception_timestamp.sec,
                                 sample_info->reception_timestamp.nanosec);

    key_entry = RHSM_History_find_key(self,&sample_info->instance_handle);
    if (key_entry == NULL)
    {
        return DDSHST_RETCODE_SUCCESS;
    }

    rw_entry = RHSM_History_find_rw(self,&sample_info->publication_handle);
    if (rw_entry == NULL)
    {
        return DDSHST_RETCODE_SUCCESS;
    }

    if (self->_qos->ownership.kind == DDS_EXCLUSIVE_OWNERSHIP_QOS)
    {
        if (key_entry->_current_owner != rw_entry)
        {
            return DDSHST_RETCODE_SUCCESS;
        }
        RHSM_History_remove_remote_writer(self,key_entry,0);
    }
    else
    {
        for (i = 0; key_entry->_writer_count; ++i)
        {
            if (key_entry->_writers[i] == rw_entry)
            {
                RHSM_History_remove_remote_writer(self,key_entry,i);
                break;
            }
        }
    }

    if (key_entry->_writer_count > 0)
    {
        if (self->_qos->ownership.kind == DDS_EXCLUSIVE_OWNERSHIP_QOS)
        {
            key_entry->_current_owner = key_entry->_writers[0];
            ++key_entry->_current_owner->_ownership_count;
        }
    }
    else
    {
        key_entry->_current_owner = NULL;
        RHSM_History_add_meta_sample(self,
                key_entry,
                &sample_info->publication_handle,
                DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE,
                &now);
    }

    return DDSHST_RETCODE_SUCCESS;
}

RTI_PRIVATE DDS_ReturnCode_t
RHSM_History_dispose(struct DDSHST_Reader *rh,
                     struct DDS_SampleInfo *sample_info)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    struct RHSM_HistoryKeyEntry *key_entry;
    struct RHSM_HistoryRWEntry *rw_entry;
    struct OSAPI_NtpTime now;

    OSAPI_NtpTime_packFromNanosec(&now,
                                 sample_info->reception_timestamp.sec,
                                 sample_info->reception_timestamp.nanosec);

    key_entry = RHSM_History_find_key(self,&sample_info->instance_handle);
    if (key_entry == NULL)
    {
        return DDSHST_RETCODE_SUCCESS;
    }

    if (self->_qos->ownership.kind == DDS_EXCLUSIVE_OWNERSHIP_QOS)
    {
        rw_entry = RHSM_History_find_rw(self,&sample_info->publication_handle);
        if (rw_entry == NULL)
        {
            return DDSHST_RETCODE_SUCCESS;
        }
        if (key_entry->_current_owner != rw_entry)
        {
            return DDSHST_RETCODE_SUCCESS;
        }
        --rw_entry->_ownership_count;
    }

    RHSM_History_add_meta_sample(self,
                                 key_entry,
                                 &sample_info->publication_handle,
                                 DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE,
                                 &now);

    if (!RHSM_History_delete_key(self,key_entry))
    {
        return DDSHST_RETCODE_ERROR;
    }

    return DDSHST_RETCODE_SUCCESS;
}

#define RHSM_HistorySampleEntry_can_be_read_or_taken(entry_,self_,ss_,is_,vs_) \
((!((entry_)->_flags & RHSM_HISTORY_FLAG_SAMPLE_TAKEN)) && \
 ((entry_)->sample_state & (ss_)) && \
 ((entry_)->_key_entry->_instance_state & (is_)) && \
 ((entry_)->_key_entry->_view_state & (vs_)))

RTI_PRIVATE DDS_ReturnCode_t
RHSM_History_read_or_take(struct DDSHST_Reader * rh,
                          void ***sample_ptr_array,
                          struct DDS_SampleInfo **info_array,
                          DDS_Long *sample_count,
                          const DDS_InstanceHandle_t *handle,
                          DDS_Long max_samples,
                          DDS_SampleStateMask sample_states,
                          DDS_ViewStateMask view_states,
                          DDS_InstanceStateMask instance_states,
                          DDS_Boolean take)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    struct RHSM_HistorySample *entry = NULL;
    struct RHSM_HistorySample *last_entry = NULL;
    struct RHSM_HistoryKeyEntry *instance_entry = NULL;
    RTI_INT32 i;

    if ((rh == NULL) || (sample_ptr_array == NULL) || (info_array == NULL) || 
        (sample_count == NULL) || ((handle != NULL) && (!handle->is_valid)) )
    {
        return DDS_RETCODE_BAD_PARAMETER;
    }

    /* get/check instance */
    if (handle != NULL)
    {
        instance_entry = RHSM_History_find_key(self, handle);
        if (instance_entry == NULL)
        {
            /* instance does not exist --> bad parameter */
            return DDS_RETCODE_BAD_PARAMETER;
        }
    }

    *sample_count = 0;

    if (instance_entry != NULL)
    {
        /* read/take for particular instance, from instance's sample list */
        if (REDA_CircularList_is_empty(&instance_entry->_samples))
        {
            return DDS_RETCODE_NO_DATA;
        }
        entry = (struct RHSM_HistorySample*)
                        REDA_CircularList_get_first(&instance_entry->_samples);
        last_entry = (struct RHSM_HistorySample*)&instance_entry->_samples;
    }
    else
    {
        /* not particular instance, so read/take from general sample list */
        if (RHSM_History_ordered_list_is_empty(&self->_s_ordered))
        {
            return DDS_RETCODE_NO_DATA;
        }
        entry = (struct RHSM_HistorySample*)self->_s_ordered._ordered_next;
        last_entry = (struct RHSM_HistorySample*)&self->_s_ordered;
    }

    *sample_ptr_array = (void **)
        REDA_BufferPool_get_buffer(self->_sample_ptr_pool);
    if (*sample_ptr_array == NULL)
    {
        /* TODO: log exception */
        return DDS_RETCODE_NO_DATA; 
    }

    *info_array = (struct DDS_SampleInfo *)
        REDA_BufferPool_get_buffer(self->_sample_info_pool);
    if (*info_array == NULL)
    {
        /* TODO: log excpetion */
        return DDS_RETCODE_NO_DATA;
    }

    /* Copy to sequence, sample by sample */

    /* circular list iteration, until seq full or list fully iterated */
    while ((entry != last_entry) && (*sample_count < max_samples))
    {
        /* for untaken samples, check sample/instance/view states */

        if (RHSM_HistorySampleEntry_can_be_read_or_taken(
           entry, self, sample_states, instance_states, view_states))
        {
            if (entry->_sample != NULL)
            {   
                (*sample_ptr_array)[*sample_count] = entry->_sample->_user_data;
            }
            else 
            {
                (*sample_ptr_array)[*sample_count] = NULL;
            }
          
            /* update entry as in-use, possibly loaned and/or taken */
            entry->_flags |= (take ? RHSM_HISTORY_FLAG_SAMPLE_TAKEN : 0);
            ++entry->_loan_count;

            /* update entry's sample info, then set sample info ref */
            entry->_info.instance_state = entry->_key_entry->_instance_state;
            entry->_info.view_state = entry->_key_entry->_view_state;
            entry->_info.sample_state = entry->sample_state;
            entry->_info.reserved_data = (void *)entry;

            /* copy entry's sampleInfo, as view/instance states will be updated */
            (*info_array)[*sample_count] = entry->_info;
            ++(*sample_count);

            /* update sample state upon read/take */
            entry->_info.sample_state = DDS_READ_SAMPLE_STATE;
            entry->sample_state = DDS_READ_SAMPLE_STATE;

            /* if taken, remove from committed list, leave entry as in-use */
            if (take)
            {
                RHSM_History_unlink_ordered_sample(entry);
            }
        }

        if (instance_entry != NULL)
        {
            entry = (struct RHSM_HistorySample*)REDA_CircularListNode_get_next(
                                                                &entry->_node);
        }
        else
        {
            entry = entry->_ordered_next;
        }
    }

    /* update view state, only after samples read/taken */
    for (i = 0; i < *sample_count; ++i)
    {
        entry = (struct RHSM_HistorySample *)((*info_array)[i].reserved_data);
        entry->_key_entry->_view_state = DDS_NOT_NEW_VIEW_STATE;
    }

    if (*sample_count == 0)
    {
        /* no data --> return internal buffers */
        REDA_BufferPool_return_buffer(self->_sample_ptr_pool, *sample_ptr_array);
        REDA_BufferPool_return_buffer(self->_sample_info_pool, *info_array);
        *sample_ptr_array = NULL;
        *info_array = NULL;
    }

    return ((*sample_count == 0) ? DDS_RETCODE_NO_DATA : DDS_RETCODE_OK);
}

RTI_PRIVATE DDS_ReturnCode_t
RHSM_History_finish_read_or_take(struct DDSHST_Reader * rh,
                                 void ***sample_ptr_array,
                                 struct DDS_SampleInfo **info_array,
                                 DDS_Long sample_count,
                                 DDS_Boolean taken)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    DDS_Long i;
    struct RHSM_HistorySample *entry = NULL;

    for (i = 0; i < sample_count; ++i)
    {
        entry = (struct RHSM_HistorySample*)((*info_array)[i].reserved_data);
        /*  || !entry->_in_use */
        if (entry == NULL)
        {
            continue;
        }

        --entry->_loan_count;

        /* remove taken sample with no loans */
        if (entry->_loan_count == 0 &&
            (entry->_flags & RHSM_HISTORY_FLAG_SAMPLE_TAKEN))
        {
            RHSM_History_free_or_delete_sample(self,
                                &entry->_key_entry->_key,
                                entry);
            --entry->_key_entry->_size;
        }
    }

    /* return sample_ptr and info arrays to pools */
    if (*sample_ptr_array != NULL)
    {
        REDA_BufferPool_return_buffer(self->_sample_ptr_pool, *sample_ptr_array);
        *sample_ptr_array = NULL;
    }
    if (*info_array != NULL)
    {
        REDA_BufferPool_return_buffer(self->_sample_info_pool, *info_array);
        *info_array = NULL;
    }

    return DDS_RETCODE_OK;
}


RTI_PRIVATE void
RHSM_History_post_event(struct DDSHST_Reader *rh,
                               struct DDSHST_ReaderEvent *event,
                               struct OSAPI_NtpTime *now)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;
    struct RHSM_HistoryRWEntry *rw_entry = NULL;

    switch (event->kind)
    {
        case DDSHST_READEREVENT_KIND_LIVELINESS_LOST:
        case DDSHST_READEREVENT_KIND_LIVELINESS_DETECTED:
            RHSM_History_update_liveliness(
                                        (struct RHSM_History*)self,event,now);
            break;
        case DDSHST_READEREVENT_KIND_REMOTE_WRITER_DELETED:
            rw_entry = RHSM_History_find_rw(self,&event->data.rw_deleted.rw_guid);
            if (rw_entry != NULL)
            {
                RHSM_History_prune_rw(rh,rw_entry,RTI_TRUE);
            }
            break;
        case DDSHST_READEREVENT_KIND_DEADLINE_EXPIRED:
            RHSM_History_on_deadline_expired(self,now);
            break;
        default:
            break;
    }
}

RTI_PRIVATE struct DDSHST_ReaderInfo *
RHSM_History_get_info(struct DDSHST_Reader *rh)
{
    struct RHSM_History *self = (struct RHSM_History *)rh;

    return &self->_info;
}

/*******************************************************************************
 *                                Plugin API
 ******************************************************************************/

RTI_PRIVATE struct DDSHST_ReaderI RHSM_HistoryI_g_intf = {
    {DEFAULT_COMPONENT_RUN},
    RHSM_History_get_entry,
    RHSM_History_return_entry,
    RHSM_History_commit_entry,
    RHSM_History_commit_sample,
    RHSM_History_read_or_take,
    RHSM_History_finish_read_or_take,
    RHSM_History_register_key,
    RHSM_History_unregister_key,
    RHSM_History_post_event,
    RHSM_History_dispose,
    RHSM_History_get_info,
    RHSM_History_lookup_key
};

RTI_PRIVATE RT_Component_T*
RHSM_HistoryFactory_create_component(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentProperty *property,
        struct RT_ComponentListener *listener)
{
    struct RHSM_History *retval = NULL;

    retval =
        RHSM_History_create((const struct RHSM_HistoryProperty *
                                    const)property,
                                   (const struct DDSHST_ReaderListener *
                                    const)listener);
    return &retval->_parent._parent;
}

RTI_PRIVATE void
RHSM_HistoryFactory_delete_component(struct RT_ComponentFactory *factory,
                                            RT_Component_T * component)
{
    RHSM_History_delete((struct RHSM_History *)component);
}

static struct RT_ComponentFactory *
RHSM_HistoryFactory_initialize(
        struct RT_ComponentFactoryProperty *property,
        struct RT_ComponentFactoryListener *listener);

static void
RHSM_HistoryFactory_finalize(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener);

struct RT_ComponentFactoryI RHSM_HistoryFactory_g_intf =
{
    RHSM_HISTORY_INTERFACE_ID,
    RHSM_HistoryFactory_initialize,
    RHSM_HistoryFactory_finalize,
    RHSM_HistoryFactory_create_component,
    RHSM_HistoryFactory_delete_component
};

static struct RT_ComponentFactory RHSM_HistoryFactory_g_factory =
{
    &RHSM_HistoryFactory_g_intf,
    NULL
};

struct RT_ComponentFactory*
RHSM_HistoryFactory_initialize(
        struct RT_ComponentFactoryProperty *property,
        struct RT_ComponentFactoryListener *listener)
{
    RHSM_HistoryFactory_g_factory._factory =
        &RHSM_HistoryFactory_g_factory;
    return &RHSM_HistoryFactory_g_factory;
}

void
RHSM_HistoryFactory_finalize(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener)
{

}

struct RT_ComponentFactoryI *
RHSM_HistoryFactory_get_interface(void)
{
    return &RHSM_HistoryFactory_g_intf;
}
