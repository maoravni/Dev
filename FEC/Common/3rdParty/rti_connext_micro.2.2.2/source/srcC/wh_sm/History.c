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
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef osapi_system_h
#include "osapi/osapi_system.h"
#endif
#ifndef reda_buffer_h
#include "reda/reda_buffer.h"
#endif
#ifndef reda_circularlist_h
#include "reda/reda_circularlist.h"
#endif
#ifndef reda_sequenceNumber_h
#include "reda/reda_sequenceNumber.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef wh_sm_log_h
#include "wh_sm/wh_sm_log.h"
#endif

#include "wh_sm/wh_sm_history.h"

const struct DDSHST_WriterListener
    RTI_DEFAULT_SMWRITERHISTORY_LISTENER = DDSHST_WriterListener_INITIALIZE;

const struct WHSM_HistoryProperty
    RTI_DEFAULT_SMWRITERHISTORY_PROPERTY =
    WHSM_HistoryProperty_INITIALIZER;

RTI_PRIVATE struct DDSHST_WriterI WHSM_HistoryI_g_intf;



struct WHSM_HistoryStatistics
{
    RTI_UINT32 collision_count;
};

#define RTI_SMWRITERHISTORY_SAMPLE_STATE_FREE   0x1
#define RTI_SMWRITERHISTORY_SAMPLE_STATE_LOANED 0x2
#define RTI_SMWRITERHISTORY_SAMPLE_STATE_ACKED  0x4

#define RTI_SMWRITERHISTORY_KEY_STATE_FREE      0x1
#define RTI_SMWRITERHISTORY_KEY_STATE_LOANED    0x2
#define RTI_SMWRITERHISTORY_KEY_STATE_ACKED     0x4

struct WHSM_HistorySampleEntry
{
    struct REDA_CircularListNode _node;
    struct DDSHST_WriterSample *_sample;
    struct REDA_SequenceNumber _sn;
    DDS_UnsignedLong _key_index;
    DDS_Octet _state;
};

/* WHSM_HistoryKeyEntry
 *
 * Store information about each unique key registered or detected
 * Each key has a list of samples for that key.
 */
struct WHSM_HistoryKeyEntry
{
    DDS_InstanceHandle_t _key;
    REDA_CircularList_T _samples;
    DDS_Long _size;
    DDS_Long _index;
    DDS_Octet _state;
    RTI_INT32 last_update_period;
    struct REDA_CircularListMixInNode _deadline_entry;
};

struct WHSM_HistoryWindowEntry
{
    struct WHSM_HistorySampleEntry *_sentry;
};

/* WHSM_History
 *
 * Data structure that represents the history cache.
 */

struct WHSM_History
{
    struct DDSHST_Writer _parent;
    struct WHSM_HistoryKeyEntry **keys;
    struct WHSM_HistoryProperty _property;
    const struct DDS_DataWriterQos *_qos;
    struct DDSHST_WriterListener _listener;
    REDA_CircularList_T _free_sample_list;
    struct DDSHST_WriterState _state;
    struct DDSHST_WriterInfo _info;
    struct WHSM_HistoryWindowEntry *window;
    DDS_Long _free_win_slot_idx;
    struct WHSM_HistoryStatistics _stat;
    REDA_CircularList_T _deadline_timer;
    RTI_INT32 _current_period;
    RTI_BOOL deadline_enabled;
};


/*** SOURCE_BEGIN ***/

/*******************************************************************************
 *                                RTI_PRIVATE API
 ******************************************************************************/

RTI_INT32
WHSM_History_find_slot(struct WHSM_History *self,
                              const struct REDA_SequenceNumber *const sn,
                              DDS_Boolean find_empty_slot)
{
    RTI_INT32 slot = -1;
    RTI_INT32 ll, ul, result;

    if (self->_qos->resource_limits.max_instances == 1)
    {
        slot = sn->low % self->_qos->resource_limits.max_samples;
        if (!((find_empty_slot && (self->window[slot]._sentry == NULL)) ||
              (!find_empty_slot && (self->window[slot]._sentry != NULL) &&
               !REDA_SequenceNumber_compare(&self->window[slot]._sentry->_sn,
                                           sn))))
        {
            slot = -1;
        }
    }
    else if (find_empty_slot &&
             (self->_free_win_slot_idx !=
              self->_qos->resource_limits.max_samples))
    {
        slot = ((char *)(&self->window[self->_free_win_slot_idx]) -
                (char *)(&self->window[0])) /
            sizeof(struct WHSM_HistoryWindowEntry);
        ++self->_free_win_slot_idx;
    }
    else if (!find_empty_slot)
    {
        ll = 0;
        ul = self->_free_win_slot_idx - 1;
        while (ll <= ul)
        {
            slot = ll + (ul - ll) / 2;
            result = REDA_SequenceNumber_compare(sn,
                                                &self->window[slot]._sentry->
                                                _sn);
            if (result == 0)
            {
                break;
            }
            else if (result < 0)
            {
                ul = slot - 1;
            }
            else
            {
                ll = slot + 1;
            }
        }
        slot = (ll <= ul ? slot : -1);
    }

    return slot;
}

DDS_Boolean
WHSM_History_initialize_key(struct WHSM_History *self,
                            struct WHSM_HistoryKeyEntry *key)
{
    DDS_InstanceHandle_t null_handle = DDS_HANDLE_NIL;

    key->_state = RTI_SMWRITERHISTORY_KEY_STATE_FREE;
    key->_size = 0;
    REDA_CircularList_init(&key->_samples);
    key->_key = null_handle;
    REDA_CircularListMixInNode_init(&key->_deadline_entry,key);

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
WHSM_History_free_or_delete_sample(struct WHSM_History *self,
                            DDS_InstanceHandle_t * key,
                            struct WHSM_HistorySampleEntry* sample,
                            DDS_Boolean delete)
{
    REDA_CircularList_unlink_node(&sample->_node);

    if (self->_listener.on_sample_removed)
    {
        self->_listener.on_sample_removed(self->_listener.listener_data,
                                          key, sample->_sample);
    }

    if (delete)
    {
        OSAPI_Heap_freeStructure(sample);
    }
    else
    {
        REDA_CircularList_append(&self->_free_sample_list,&sample->_node);
        sample->_state = RTI_SMWRITERHISTORY_SAMPLE_STATE_FREE;
    }

    return DDSHST_RETCODE_SUCCESS;
}

DDS_Boolean
WHSM_History_delete_key(struct WHSM_History *self,
                        struct WHSM_HistoryKeyEntry *key)
{
    struct WHSM_HistorySampleEntry *sample = NULL;
    struct WHSM_HistorySampleEntry *sample2 = NULL;

    sample = (struct WHSM_HistorySampleEntry*)
                                  REDA_CircularList_get_first(&key->_samples);

    while (!REDA_CircularList_node_at_head(&key->_samples,&sample->_node))
    {
        sample2 = (struct WHSM_HistorySampleEntry*)
                                REDA_CircularListNode_get_next(&sample->_node);
        WHSM_History_free_or_delete_sample(self, &key->_key, sample,
                                                            DDS_BOOLEAN_TRUE);
        --key->_size;
        sample = sample2;
    }

    if (self->_listener.on_key_removed)
    {
        self->_listener.on_key_removed(self->_listener.listener_data,
                                       &key->_key);
    }

    REDA_CircularList_unlink_node(
                        (struct REDA_CircularListNode*)&key->_deadline_entry);

    OSAPI_Heap_freeStructure(key);

    return DDSHST_RETCODE_SUCCESS;
}

DDS_Boolean
WHSM_History_prune_samples(struct WHSM_History * self,
                           struct WHSM_HistoryKeyEntry * const w_entry)
{
    struct WHSM_HistorySampleEntry *s1_entry;
    RTI_INT32 win_slot, slot_index;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;

    s1_entry = (struct WHSM_HistorySampleEntry*)
                            REDA_CircularList_get_first(&w_entry->_samples);
    while (!REDA_CircularList_node_at_head(&w_entry->_samples,&s1_entry->_node))
    {
        if (!(s1_entry->_state & RTI_SMWRITERHISTORY_SAMPLE_STATE_LOANED))
        {
            WHSM_History_free_or_delete_sample(self,
                                               &w_entry->_key, s1_entry,
                                               DDS_BOOLEAN_FALSE /* delete */);
            if (self->window != NULL)
            {
                win_slot =
                    WHSM_History_find_slot(self, &s1_entry->_sn,
                                           DDS_BOOLEAN_FALSE);
                if (win_slot != -1)
                {
                    if (self->_qos->resource_limits.max_instances > 1)
                    {
                        slot_index = ((char *)(&self->window[win_slot]) -
                                      (char *)(&self->window[0])) /
                            sizeof(struct WHSM_HistoryWindowEntry);
                        if (slot_index != self->_free_win_slot_idx - 1)
                        {
                            RTI_INT32 i;
                            for (i = slot_index;
                                 i < self->_free_win_slot_idx - 1; ++i)
                            {
                                self->window[i] = self->window[i + 1];
                            }
                            self->window[self->_free_win_slot_idx - 1]._sentry =
                                NULL;
                            ++self->_stat.collision_count;
                        }
                        --self->_free_win_slot_idx;
                    }
                    else
                    {
                        self->window[win_slot]._sentry = NULL;
                    }
                }
            }
            --w_entry->_size;
            /* exit after freeing one */
            retval = DDS_BOOLEAN_TRUE;
            goto done;
        }
        s1_entry = (struct WHSM_HistorySampleEntry*)
                              REDA_CircularListNode_get_next(&s1_entry->_node);
    }

    done:

    return retval;
}

struct WHSM_HistoryStatistics*
WHSM_History_get_stats(struct WHSM_History *self)
{
    return &self->_stat;
}

struct WHSM_HistorySampleEntry *
WHSM_History_find_entry(struct WHSM_History *self,
                               const struct REDA_SequenceNumber *const sn)
{
    struct WHSM_HistorySampleEntry *s1_entry = NULL;
    struct WHSM_HistoryKeyEntry *w_entry = NULL;
    DDS_Long i;
    RTI_INT32 win_slot;

    if (self->window != NULL)
    {
        win_slot = WHSM_History_find_slot(self, sn, DDS_BOOLEAN_FALSE);
        if (win_slot != -1)
        {
            s1_entry = self->window[win_slot]._sentry;
        }
    }
    else
    {
        for (i = 0; i < self->_qos->resource_limits.max_instances; ++i)
        {
            w_entry = self->keys[i];
            if (!(w_entry->_state & RTI_SMWRITERHISTORY_KEY_STATE_FREE))
            {
                s1_entry =(struct WHSM_HistorySampleEntry*)
                                REDA_CircularList_get_first(&w_entry->_samples);

                while (!REDA_CircularList_node_at_head(&w_entry->_samples,
                                                       &s1_entry->_node))
                {
                    if (!REDA_SequenceNumber_compare(&s1_entry->_sn, sn))
                    {
                        i = self->_qos->resource_limits.max_instances;
                        break;
                    }
                    s1_entry = (struct WHSM_HistorySampleEntry*)
                            REDA_CircularListNode_get_next(&s1_entry->_node);
                }
                if (REDA_CircularList_node_at_head(&w_entry->_samples,
                                                   &s1_entry->_node))
                {
                    s1_entry = NULL;
                }
            }
        }
    }

    return s1_entry;
}

struct WHSM_HistoryKeyEntry *
WHSM_History_find_key(struct WHSM_History *self,
                             const DDS_InstanceHandle_t * const key,
                             DDS_Long *free)
{
    DDS_Long i;
    struct WHSM_HistoryKeyEntry *retval = NULL;

    if (free)
    {
        *free = -1;
    }
    for (i = 0; i < self->_qos->resource_limits.max_instances; ++i)
    {
        if ((self->keys[i]->_state & RTI_SMWRITERHISTORY_KEY_STATE_FREE)
            && (free && (*free == -1)))
        {
            *free = i;
        }
        if (!(self->keys[i]->_state & RTI_SMWRITERHISTORY_KEY_STATE_FREE)
            && DDS_InstanceHandle_equals(&self->keys[i]->_key, key))
        {
            retval = self->keys[i];
            break;
        }
    }

    return retval;
}

#ifdef ENABLE_QOS_DEADLINE
RTI_PRIVATE void
WHSM_History_on_deadline_expired(struct WHSM_History *self,struct OSAPI_NtpTime *now)
{
    struct WHSM_HistoryKeyEntry *key_entry;
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
        ++key_entry->last_update_period;
        if (self->_listener.on_deadline_missed)
        {
            self->_listener.on_deadline_missed(
                    self->_listener.listener_data,&key_entry->_key);
        }
        if (a_node == last_node)
        {
            check_complete = RTI_TRUE;
            REDA_CircularList_unlink_node(
                   (struct REDA_CircularListNode*)&key_entry->_deadline_entry);
            REDA_CircularList_append(&self->_deadline_timer,
                   (struct REDA_CircularListNode *)&key_entry->_deadline_entry);
        }
        else
        {
            a_node = (struct REDA_CircularListMixInNode *)REDA_CircularListNode_get_next(a_node);
            if (a_node == (struct REDA_CircularListMixInNode*)&self->_deadline_timer)
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
DDSHST_ReturnCode_T
WHSM_History_delete(struct WHSM_History * self)
{
    DDS_Long i;
    struct WHSM_HistorySampleEntry *sample = NULL, *sample2;

    for (i = 0; i < self->_qos->resource_limits.max_instances; ++i)
    {
        if (!(self->keys[i]->_state & RTI_SMWRITERHISTORY_KEY_STATE_FREE))
        {
            WHSM_History_delete_key(self, self->keys[i]);
        }
        else
        {
            OSAPI_Heap_freeStructure(self->keys[i]);
        }
    }
    OSAPI_Heap_freeArray(self->keys);

    sample = (struct WHSM_HistorySampleEntry*)
                         REDA_CircularList_get_first(&self->_free_sample_list);

    while (!REDA_CircularList_node_at_head(&self->_free_sample_list,
                                           &sample->_node))
    {
        sample2 = (struct WHSM_HistorySampleEntry*)
                                REDA_CircularListNode_get_next(&sample->_node);
        OSAPI_Heap_freeStructure(sample);
        sample = sample2;
    }

    if (self->window != NULL)
    {
        OSAPI_Heap_freeArray(self->window);
    }

    OSAPI_Heap_freeStructure(self);

    return DDSHST_RETCODE_SUCCESS;
}

struct WHSM_History*
WHSM_History_create(const struct WHSM_HistoryProperty *const property,
                    const struct DDSHST_WriterListener *const listener)
{
    struct WHSM_History *history = NULL;
    struct WHSM_History *retval = NULL;
    DDS_Long i;
    struct WHSM_HistorySampleEntry *sample = NULL;
    struct REDA_SequenceNumber unknown = REDA_SEQUENCE_NUMBER_UNKNOWN;
    const struct DDS_DataWriterQos *qos;

    qos = property->_parent.qos;

    if (qos->history.kind != DDS_KEEP_LAST_HISTORY_QOS)
    {
        /*LOG_printError("Keep all history not supported\n"); */
        WHSM_Log_error(LOG_KEEP_ALL_HISTORY_NOT_SUPPORTED);
        goto done;
    }

#if 0
    /*TODO: Should we check this, or just ignore depth */
    if (qos->history.depth != qos->resource_limits.max_samples)
    {
        LOG_printError("depth != qos->resource_limits.max_samples\n");
        goto done;
    }
#endif
    if ((qos->resource_limits.max_instances == DDS_LENGTH_UNLIMITED) ||
        (qos->resource_limits.max_samples == DDS_LENGTH_UNLIMITED) ||
        (qos->resource_limits.max_samples_per_instance == DDS_LENGTH_UNLIMITED))
    {
        /*LOG_printError("Unlimited history not supported\n"); */
        WHSM_Log_error(LOG_UNLIMITED_HISTORY_NOT_SUPPORTED);
        goto done;
    }

    if (qos->resource_limits.max_samples <
        (qos->resource_limits.max_instances
         * qos->resource_limits.max_samples_per_instance))
    {
        /*LOG_printError("max_samples < max_instances*max_samples_per_instance\n"); */
        WHSM_Log_error
            (LOG_MAX_SAMPLES_LESS_THAN_MAX_INSTANCES_TIMES_MAX_SAMPERINST);
        goto done;
    }

    OSAPI_Heap_allocateStructure(&history, struct WHSM_History);
    if (history == NULL)
    {
        goto done;
    }

    RT_Component_initialize(&history->_parent._parent,
                           &WHSM_HistoryI_g_intf._parent,
                           0,
                           (property ? &property->_parent._parent : NULL),
                           (listener ? &listener->_parent : NULL));

    history->keys = NULL;
    REDA_CircularList_init(&history->_free_sample_list);
    history->_qos = qos;

    if (property)
    {
        history->_property = *property;
    }
    else
    {
        history->_property = RTI_DEFAULT_SMWRITERHISTORY_PROPERTY;
    }

    if (listener)
    {
        history->_listener = *listener;
    }
    else
    {
        history->_listener = RTI_DEFAULT_SMWRITERHISTORY_LISTENER;
    }

    OSAPI_Heap_allocateArray(&history->keys,
                            history->_qos->resource_limits.max_instances,
                            struct WHSM_HistoryKeyEntry *);

    if (history->keys == NULL)
    {
        goto done;
    }

    if (history->_property.sample_sort_treshold >
        history->_qos->resource_limits.max_samples)
    {
        history->window = NULL;
    }
    else
    {
        OSAPI_Heap_allocateArray(&history->window,
                                history->_qos->resource_limits.max_samples,
                                struct WHSM_HistoryWindowEntry);

        if (history->window == NULL)
        {
            goto done;
        }
        for (i = 0; i < history->_qos->resource_limits.max_samples; ++i)
        {
            history->window[i]._sentry = NULL;
        }
        history->_free_win_slot_idx = 0;
    }

    for (i = 0; i < history->_qos->resource_limits.max_instances; ++i)
    {
    OSAPI_Heap_allocateStructure(&history->keys[i],
                                struct WHSM_HistoryKeyEntry);
        if (history->keys[i] == NULL)
        {
            goto done;
        }
        WHSM_History_initialize_key(history, history->keys[i]);
        history->keys[i]->_index = i;
    }

    for (i = 0; i < history->_qos->resource_limits.max_samples; ++i)
    {
    OSAPI_Heap_allocateStructure(&sample, struct WHSM_HistorySampleEntry);
        if (sample == NULL)
        {
            goto done;
        }
        sample->_sample = 0;
        sample->_state = RTI_SMWRITERHISTORY_SAMPLE_STATE_FREE;
        sample->_sn = unknown;
        REDA_CircularList_link_node_after(&history->_free_sample_list,
                                         &sample->_node);
    }

    REDA_SequenceNumber_setZero(&history->_state.high_sn);
    REDA_SequenceNumber_setMaximum(&history->_state.low_sn);
    OSAPI_Memory_zero(&history->_stat,
                     sizeof(struct WHSM_HistoryStatistics));
    history->_info.version = WHSM_HISTORY_VERSION;

    REDA_CircularList_init(&history->_deadline_timer);

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
        WHSM_History_delete(history);
    }

    return retval;
}

/***************************** get_entry **************************************/

DDSHST_WriterSampleEntryRef_T
WHSM_History_get_entry(struct DDSHST_Writer * wh,
                       const DDS_InstanceHandle_t *const key)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    struct WHSM_HistorySampleEntry *new_entry = NULL;
    struct WHSM_HistoryKeyEntry *key_entry = NULL;
    DDS_Long free = -1;
    DDS_Boolean at_max_history_depth;

    key_entry = WHSM_History_find_key(self, key, &free);

    if ((key_entry == NULL) && (free == -1))
    {
        return NULL;
    }

    if (key_entry == NULL)
    {
        key_entry = self->keys[free];
        WHSM_History_initialize_key(self, key_entry);
        key_entry->_state &= ~RTI_SMWRITERHISTORY_KEY_STATE_FREE;
        key_entry->_key = *key;
        key_entry->_index = free;
        REDA_CircularList_append(&self->_deadline_timer,
               (struct REDA_CircularListNode *)&key_entry->_deadline_entry);
    }

    at_max_history_depth = 
        (self->_qos->history.kind == DDS_KEEP_LAST_HISTORY_QOS &&
         key_entry->_size == self->_qos->history.depth) ? 
        DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;

    if (at_max_history_depth ||
        (key_entry->_size ==
         self->_qos->resource_limits.max_samples_per_instance))
    {
        if (!WHSM_History_prune_samples(self, key_entry))
        {
            return NULL;
        }
    }

    if (REDA_CircularList_is_empty(&self->_free_sample_list))
    {
        return NULL;
    }

    new_entry = (struct WHSM_HistorySampleEntry*)
                         REDA_CircularList_get_first(&self->_free_sample_list);
    REDA_CircularList_unlink_node(&new_entry->_node);
    new_entry->_key_index = key_entry->_index;
    new_entry->_state = RTI_SMWRITERHISTORY_SAMPLE_STATE_FREE;
    ++key_entry->_size;

    return (DDSHST_WriterSampleEntryRef_T) new_entry;

}

/**************************** return_entry ************************************/

void
WHSM_History_return_entry(struct DDSHST_Writer *wh,
                                 DDSHST_WriterSampleEntryRef_T const entry)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    struct WHSM_HistorySampleEntry *new_entry =
        (struct WHSM_HistorySampleEntry *)entry;

    new_entry->_state = RTI_SMWRITERHISTORY_SAMPLE_STATE_FREE;
    REDA_CircularList_prepend(&self->_free_sample_list,&new_entry->_node);
    --self->keys[new_entry->_key_index]->_size;
}

/***************************** commit_entry ***********************************/

DDSHST_ReturnCode_T
WHSM_History_commit_entry(struct DDSHST_Writer *wh,
                                 DDSHST_WriterSampleEntryRef_T const entry,
                                 DDSHST_WriterSample_T * const sample,
                                 const struct REDA_SequenceNumber *const sn)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    struct WHSM_HistorySampleEntry *new_entry =
        (struct WHSM_HistorySampleEntry *)entry;
    struct WHSM_HistorySampleEntry *s1_entry;
    struct WHSM_HistoryKeyEntry *w_entry = NULL;
    RTI_INT32 win_slot;

    new_entry->_sn = *sn;
    new_entry->_sample = sample;
    w_entry = self->keys[new_entry->_key_index];

    s1_entry = (struct WHSM_HistorySampleEntry*)
                                REDA_CircularList_get_last(&w_entry->_samples);
    if ((s1_entry ==
         (struct WHSM_HistorySampleEntry *)&w_entry->_samples)
        || (REDA_SequenceNumber_compare(&s1_entry->_sn, sn) < 0))
    {
        REDA_CircularList_link_node_after(&s1_entry->_node, &new_entry->_node);
        if (self->window != NULL)
        {
            win_slot = WHSM_History_find_slot(self, sn, DDS_BOOLEAN_TRUE);
            self->window[win_slot]._sentry = new_entry;
        }
        if (w_entry->last_update_period == self->_current_period)
        {
            REDA_CircularList_unlink_node(
                    (struct REDA_CircularListNode *)&w_entry->_deadline_entry);
            REDA_CircularList_append(&self->_deadline_timer,
                    (struct REDA_CircularListNode *)&w_entry->_deadline_entry);
            w_entry->last_update_period = self->_current_period + 1;
        }

        return DDSHST_RETCODE_SUCCESS;
    }

    return DDSHST_RETCODE_ERROR;
}

/***************************** request_entry **********************************/

DDSHST_ReturnCode_T
WHSM_History_request_sample(struct DDSHST_Writer * wh,
                                   struct DDSHST_WriterSample **sample,
                                   const struct REDA_SequenceNumber *const sn)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    struct WHSM_HistorySampleEntry *s1_entry = NULL;
    DDSHST_ReturnCode_T retval = DDSHST_RETCODE_NOT_EXISTS;

    s1_entry = WHSM_History_find_entry(self, sn);

    if (s1_entry != NULL)
    {
        *sample = s1_entry->_sample;
        (*sample)->_ref = (DDSHST_WriterSampleEntryRef_T) s1_entry;
        s1_entry->_state |= RTI_SMWRITERHISTORY_SAMPLE_STATE_LOANED;
        retval = DDSHST_RETCODE_SUCCESS;
    }

    return retval;
}

/************************** sample_loan_finished ******************************/

DDSHST_ReturnCode_T
WHSM_History_sample_loan_finished(struct DDSHST_Writer *wh,
                                const struct REDA_SequenceNumber *const sn)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    struct WHSM_HistorySampleEntry *s1_entry = NULL;
    DDSHST_ReturnCode_T retval = DDSHST_RETCODE_NOT_EXISTS;

    s1_entry = WHSM_History_find_entry(self, sn);

    if (s1_entry != NULL)
    {
        s1_entry->_state &= ~RTI_SMWRITERHISTORY_SAMPLE_STATE_LOANED;
        if (s1_entry->_state & RTI_SMWRITERHISTORY_SAMPLE_STATE_ACKED)
        {
            REDA_CircularList_unlink_node(&s1_entry->_node);
            WHSM_History_return_entry(wh,
                                      (DDSHST_WriterSampleEntryRef_T)s1_entry);
            s1_entry->_state &= ~RTI_SMWRITERHISTORY_SAMPLE_STATE_ACKED;
        }
        retval = DDSHST_RETCODE_SUCCESS;
    }

    return retval;
}

/******************************** ack_sample **********************************/

DDSHST_ReturnCode_T
WHSM_History_ack_sample(struct DDSHST_Writer *wh,
                               const struct REDA_SequenceNumber *const sn)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    struct WHSM_HistorySampleEntry *s1_entry;
    DDSHST_ReturnCode_T retval = DDSHST_RETCODE_NOT_EXISTS;

    s1_entry = WHSM_History_find_entry(self, sn);

    if (s1_entry != NULL)
    {
        if (!(s1_entry->_state & RTI_SMWRITERHISTORY_SAMPLE_STATE_LOANED))
        {
            REDA_CircularList_unlink_node(&s1_entry->_node);
            WHSM_History_return_entry(wh,
                                      (DDSHST_WriterSampleEntryRef_T)s1_entry);
        }
        else
        {
            s1_entry->_state |= RTI_SMWRITERHISTORY_SAMPLE_STATE_ACKED;
        }
        retval = DDSHST_RETCODE_SUCCESS;
    }

    return retval;
}

/******************************** register_key ********************************/

DDSHST_ReturnCode_T
WHSM_History_register_key(struct DDSHST_Writer *wh,
                                 const DDS_InstanceHandle_t *const key)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    DDS_Long free = -1;
    struct WHSM_HistoryKeyEntry *key_entry;

    key_entry = WHSM_History_find_key(self, key, &free);

    if (key_entry != NULL)
    {
        return DDSHST_RETCODE_SUCCESS;
    }

    if (free == -1)
    {
        return DDSHST_RETCODE_NOSPACE;
    }

    WHSM_History_initialize_key(self, self->keys[free]);
    self->keys[free]->_key = *key;
    self->keys[free]->_index = free;
    self->keys[free]->_state &= ~RTI_SMWRITERHISTORY_KEY_STATE_FREE;

    return DDSHST_RETCODE_SUCCESS;
}

/****************************** unregister_key ********************************/

DDSHST_ReturnCode_T
WHSM_History_unregister_key(struct DDSHST_Writer *wh,
                                   const DDS_InstanceHandle_t *const key)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    struct WHSM_HistorySampleEntry *sample = NULL;
    struct WHSM_HistorySampleEntry *sample2 = NULL;
    struct WHSM_HistoryKeyEntry *key_entry;

    key_entry = WHSM_History_find_key(self, key, NULL);

    if (key_entry == NULL)
    {
        return DDSHST_RETCODE_NOT_EXISTS;
    }

    sample = (struct WHSM_HistorySampleEntry*)
                              REDA_CircularList_get_first(&key_entry->_samples);

    while (!REDA_CircularList_node_at_head(&key_entry->_samples,&sample->_node))
    {
        sample2 = (struct WHSM_HistorySampleEntry*)
                                REDA_CircularListNode_get_next(&sample->_node);
        WHSM_History_free_or_delete_sample(self, &key_entry->_key,
                                                  sample, DDS_BOOLEAN_FALSE);
        sample = sample2;
    }

    if (self->_listener.on_key_removed)
    {
        self->_listener.on_key_removed(self->_listener.listener_data,
                                       &key_entry->_key);
    }

    REDA_CircularList_unlink_node((struct REDA_CircularListNode *)
                                                &key_entry->_deadline_entry);

    key_entry->_state = RTI_SMWRITERHISTORY_KEY_STATE_FREE;

    return DDSHST_RETCODE_SUCCESS;
}

/******************************** update_state ********************************/

void
WHSM_History_update_state(struct DDSHST_Writer *self,
                                 const struct OSAPI_NtpTime *const now)
{
    /* TODO: This is intended for asynchronous updates such as deadlines
     * and lifespan updates.
     */
}

/********************************* get_state **********************************/

struct DDSHST_WriterState *
WHSM_History_get_state(struct DDSHST_Writer *wh)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;
    struct WHSM_HistorySampleEntry *s1_entry;
    struct WHSM_HistoryKeyEntry *w_entry = NULL;
    DDS_Long i;
    DDS_Boolean empty = DDS_BOOLEAN_TRUE;

    REDA_SequenceNumber_setZero(&self->_state.high_sn);
    REDA_SequenceNumber_setMaximum(&self->_state.low_sn);

    for (i = 0; i < self->_qos->resource_limits.max_instances; ++i)
    {
        w_entry = self->keys[i];
        if (!(w_entry->_state & RTI_SMWRITERHISTORY_KEY_STATE_FREE))
        {
            s1_entry = (struct WHSM_HistorySampleEntry*)
                                                       w_entry->_samples._next;
            if (s1_entry != (struct WHSM_HistorySampleEntry*)
                                                       &w_entry->_samples)
            {
                empty = DDS_BOOLEAN_FALSE;
                if (REDA_SequenceNumber_compare(&self->_state.low_sn,
                                               &s1_entry->_sn) > 0)
                {
                    self->_state.low_sn = s1_entry->_sn;
                }

                s1_entry = (struct WHSM_HistorySampleEntry *)
                                                       w_entry->_samples._prev;
                if (REDA_SequenceNumber_compare(&self->_state.high_sn,
                                               &s1_entry->_sn) < 0)
                {
                    self->_state.high_sn = s1_entry->_sn;
                }
            }
        }
    }

    if (empty)
    {
        REDA_SequenceNumber_setZero(&self->_state.low_sn);
    }

    return &self->_state;
}

/********************************** get_info **********************************/

struct DDSHST_WriterInfo *
WHSM_History_get_info(struct DDSHST_Writer *wh)
{
    struct WHSM_History *self = (struct WHSM_History *)wh;

    return &self->_info;
}

RTI_PRIVATE void
WHSM_History_post_event(struct DDSHST_Writer *rh,
                               struct DDSHST_WriterEvent *event,
                               struct OSAPI_NtpTime *now)
{
    struct WHSM_History *self = (struct WHSM_History *)rh;

    switch (event->kind)
    {
        case DDSHST_WRITEREVENT_KIND_DEADLINE_EXPIRED:
            WHSM_History_on_deadline_expired(self,now);
            break;
        default:
            break;
    }
}

/*********************** MicroDDS COMPONENT Interface *************************/

RTI_PRIVATE struct DDSHST_WriterI WHSM_HistoryI_g_intf =
{
    {DEFAULT_COMPONENT_RUN},
    WHSM_History_get_entry,
    WHSM_History_return_entry,
    WHSM_History_commit_entry,
    NULL,
    WHSM_History_request_sample,
    WHSM_History_ack_sample,
    WHSM_History_register_key,
    WHSM_History_unregister_key,
    WHSM_History_update_state,
    WHSM_History_sample_loan_finished,
    WHSM_History_get_state,
    WHSM_History_get_info,
    WHSM_History_post_event
};

RTI_PRIVATE RT_Component_T*
WHSM_HistoryFactory_create_component(struct RT_ComponentFactory *factory,
                                     struct RT_ComponentProperty *property,
                                     struct RT_ComponentListener *listener)
{
    struct WHSM_History *retval = NULL;

    retval =
        WHSM_History_create(
                        (const struct WHSM_HistoryProperty * const)property,
                        (const struct DDSHST_WriterListener *const)listener);
    return &retval->_parent._parent;
}

RTI_PRIVATE void
WHSM_HistoryFactory_delete_component(struct RT_ComponentFactory *factory,
                                     RT_Component_T *component)
{
    struct WHSM_History *self = (struct WHSM_History *)component;

    WHSM_History_delete(self);
}

static struct RT_ComponentFactory *
WHSM_HistoryFactory_initialize(struct RT_ComponentFactoryProperty *property,
                               struct RT_ComponentFactoryListener *listener);

static void
WHSM_HistoryFactory_finalize(struct RT_ComponentFactory *factory,
                             struct RT_ComponentFactoryProperty **property,
                             struct RT_ComponentFactoryListener **listener);

struct RT_ComponentFactoryI WHSM_HistoryFactory_g_intf = {
    WHSM_HISTORY_INTERFACE_ID,
    WHSM_HistoryFactory_initialize,
    WHSM_HistoryFactory_finalize,
    WHSM_HistoryFactory_create_component,
    WHSM_HistoryFactory_delete_component
};

static struct RT_ComponentFactory WHSM_HistoryFactory_g_factory = {
    &WHSM_HistoryFactory_g_intf,
    NULL
};

struct RT_ComponentFactory *
WHSM_HistoryFactory_initialize(
                                struct RT_ComponentFactoryProperty *property,
                                struct RT_ComponentFactoryListener *listener)
{
    WHSM_HistoryFactory_g_factory._factory =
        &WHSM_HistoryFactory_g_factory;
    return &WHSM_HistoryFactory_g_factory;
}

void
WHSM_HistoryFactory_finalize(struct RT_ComponentFactory *factory,
                    struct RT_ComponentFactoryProperty **property,
                    struct RT_ComponentFactoryListener **listener)
{

}

struct RT_ComponentFactoryI *
WHSM_HistoryFactory_get_interface(void)
{
    return &WHSM_HistoryFactory_g_intf;
}
