/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef ReaderHistory_pkg_h
#define ReaderHistory_pkg_h



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


#endif /* ReaderHistory_pkg_h */
