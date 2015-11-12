/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef History_pkg_h
#define History_pkg_h



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

struct WHSM_HistoryStatistics*
WHSM_History_get_stats(struct WHSM_History *self);

#endif /* History_pkg_h */
