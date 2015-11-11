/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_rh_plugin_h
#define dds_c_rh_plugin_h


#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef dds_c_discovery_h
#include "dds_c/dds_c_discovery.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    DDSHST_READEREVENT_KIND_LIVELINESS_LOST = 1,
    DDSHST_READEREVENT_KIND_LIVELINESS_DETECTED,
    DDSHST_READEREVENT_KIND_REMOTE_WRITER_DELETED,
    DDSHST_READEREVENT_KIND_DEADLINE_EXPIRED
} DDSHST_ReaderEventKind_T;

struct DDSHST_ReaderEvent
{
    DDSHST_ReaderEventKind_T kind;
    union
    {
        struct
        {
            DDS_InstanceHandle_t rw_guid;
            DDS_InstanceHandle_t instance;
        } liveliness;
        struct
        {
            DDS_InstanceHandle_t rw_guid;
        } rw_deleted;
    } data;
};

struct DDSHST_Reader
{
    struct RT_Component _parent;
};

typedef struct DDSHST_Reader DDSHST_Reader_T;

struct DDSHST_ReaderSampleEntry;
typedef struct DDSHST_ReaderSampleEntry* DDSHST_ReaderSampleEntryRef_T;

struct DDSHST_ReaderKeyEntry;
typedef struct DDSHST_ReaderKeyEntry* DDSHST_ReaderKeyEntryRef_T;

typedef struct DDSHST_ReaderSample
{
    void *_user_data;
    void *_param;
    struct DDS_SampleInfo *_info;
    DDSHST_ReaderSampleEntryRef_T _ref;
} DDSHST_ReaderSample_T;

DDS_SEQUENCE(DDSHST_ReaderSample_TSeq, DDSHST_ReaderSample_T)

struct DDSHST_ReaderProperty
{
    struct RT_ComponentProperty _parent;
    struct DDS_DataReaderQos *qos;
    OSAPI_Timer_t timer;
};

#define DDSHST_ReaderProperty_INITIALIZER \
{\
    RT_ComponentProperty_INITIALIZER,\
    NULL,\
    NULL \
}

struct DDSHST_ReaderInfo
{
    RTI_UINT32 version;
};

typedef void
(*DDSHST_ReaderListener_OnSampleRemovedFunc)(
        struct DDSHST_Reader *rh,
        void *listener_data,
        DDS_InstanceHandle_t *key,
        DDSHST_ReaderSample_T *sample);

typedef void
(*DDSHST_ReaderListener_OnKeyRemovedFunc)(
        struct DDSHST_Reader *rh,
        void *listener_data,
        DDS_InstanceHandle_t *key);

typedef void
(*DDSHST_ReaderListener_OnDeadlineMissedFunc)(
        struct DDSHST_Reader *rh,
        void *listener_data,
        DDS_InstanceHandle_t *key);

typedef void
(*DDSHST_ReaderListener_OnDataAvailableFunc)(
        struct DDSHST_Reader *rh,
        void *listener_data,
        DDS_InstanceHandle_t *key,
        DDSHST_ReaderSample_T *sample);

typedef void
(*DDSHST_ReaderListener_OnSampleRejectedFunc)(
        struct DDSHST_Reader *rh,
        void *listener_data,
        DDS_InstanceHandle_t *key,
        DDS_SampleRejectedStatusKind reason);

typedef void
(*DDSHST_ReaderListener_OnSampleLostFunc)(
        struct DDSHST_Reader *rh,
        void *listener_data);

struct DDSHST_ReaderListener
{
    struct RT_ComponentListener _parent;
    DDSHST_ReaderListener_OnSampleRemovedFunc on_sample_removed;
    DDSHST_ReaderListener_OnKeyRemovedFunc on_key_removed;
    DDSHST_ReaderListener_OnDataAvailableFunc on_data_available;
    DDSHST_ReaderListener_OnDeadlineMissedFunc on_deadline_missed;
    DDSHST_ReaderListener_OnSampleRejectedFunc on_sample_rejected;
    DDSHST_ReaderListener_OnSampleLostFunc on_sample_lost;
    void *listener_data;
};

#define DDSHST_ReaderListener_INITIALIZE {\
        RT_ComponentListener_INITIALIZER,\
        NULL,\
        NULL,\
        NULL,\
        NULL,\
        NULL,\
        NULL,\
        NULL\
}

#define DDSHST_Reader_narrow(factory_) (((struct DDSHST_Reader*)(factory_))->intf)

    /* ------------------------------ Life-cycle -------------------------------- */

typedef void
(*DDSHST_Reader_deleteFunc)(struct DDSHST_Reader * self);

typedef struct DDSHST_Reader*
(*DDSHST_Reader_createFunc)(
        const struct DDSHST_ReaderProperty *const property,
        const struct DDSHST_ReaderListener *const listener);

    /* ----------------------------- Operations --------------------------------- */

typedef DDSHST_ReaderSampleEntryRef_T
(*DDSHST_Reader_get_entryFunc)(
        struct DDSHST_Reader *self,
        DDS_InstanceHandle_t *rw,
        DDS_InstanceHandle_t *key,
        struct DDS_SampleInfo **sample_info,
        DDS_Long strength);

typedef void
(*DDSHST_Reader_return_entryFunc)(
        struct DDSHST_Reader * self,
        DDSHST_ReaderSampleEntryRef_T entry);

typedef DDSHST_ReturnCode_T
(*DDSHST_Reader_commit_entryFunc)(
        struct DDSHST_Reader *self,
        DDSHST_ReaderSampleEntryRef_T entry,
        DDSHST_ReaderSample_T *sample,
        struct REDA_SequenceNumber *sn);

typedef DDSHST_ReturnCode_T
(*DDSHST_Reader_commit_sampleFunc)(
        struct DDSHST_Reader *self,
        DDS_InstanceHandle_t *rw,
        struct REDA_SequenceNumber *sn,
        DDS_Long strength);

typedef DDSHST_ReturnCode_T
(*DDSHST_Reader_register_keyFunc)(
        struct DDSHST_Reader *self,
        DDS_InstanceHandle_t *key);

typedef DDSHST_ReturnCode_T
(*DDSHST_Reader_unregister_keyFunc)(
        struct DDSHST_Reader *self,
        struct DDS_SampleInfo *sample_info);

typedef void
(*DDSHST_Reader_post_eventFunc)(
        struct DDSHST_Reader *self,
        struct DDSHST_ReaderEvent *event,
        struct OSAPI_NtpTime * now);

typedef DDS_ReturnCode_t
(*DDSHST_Reader_read_or_takeFunc)(
        struct DDSHST_Reader *self,
        void ***sample_ptr_array,
        struct DDS_SampleInfo **info_array,
        DDS_Long *sample_count,
        const DDS_InstanceHandle_t *a_handle,
        DDS_Long max_samples,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states,
        DDS_Boolean take);

typedef DDS_ReturnCode_t
(*DDSHST_Reader_read_or_take_sampleFunc)(
        struct DDSHST_Reader *self,
        struct DDSHST_ReaderSample_TSeq* sample_seq,
        DDS_Long max_samples,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states,
        DDS_Boolean take);

typedef DDS_ReturnCode_t
(*DDSHST_Reader_finish_read_or_takeFunc)(
        struct DDSHST_Reader *self,
        void ***sample_ptr_array,
        struct DDS_SampleInfo **info_array,
        DDS_Long sample_count,
        DDS_Boolean taken);

typedef DDS_ReturnCode_t
(*DDSHST_Reader_return_loan_sampleFunc)(
        struct DDSHST_Reader *self,
        struct DDSHST_ReaderSample_TSeq* sample_seq,
        DDS_Boolean take);

typedef DDS_ReturnCode_t
(*DDSHST_Reader_disposeFunc)(
        struct DDSHST_Reader *self,
        struct DDS_SampleInfo *sample_info);

typedef struct DDSHST_ReaderInfo*
(*DDSHST_Reader_get_infoFunc) (struct DDSHST_Reader* self);

typedef DDSHST_ReaderKeyEntryRef_T
(*DDSHST_Reader_lookup_keyFunc) (struct DDSHST_Reader* self,
                                 const DDS_InstanceHandle_t *const key);

struct DDSHST_ReaderI
{
    struct RT_ComponentI _parent;
    DDSHST_Reader_get_entryFunc get_entry;
    DDSHST_Reader_return_entryFunc return_entry;
    DDSHST_Reader_commit_entryFunc commit_entry;
    DDSHST_Reader_commit_sampleFunc commit_sample;
    DDSHST_Reader_read_or_takeFunc read_or_take;
    DDSHST_Reader_finish_read_or_takeFunc finish_read_or_take;
    DDSHST_Reader_register_keyFunc register_key;
    DDSHST_Reader_unregister_keyFunc unregister_key;
    DDSHST_Reader_post_eventFunc post_event;
    DDSHST_Reader_disposeFunc dispose;
    DDSHST_Reader_get_infoFunc get_info;
    DDSHST_Reader_lookup_keyFunc lookup_key;
};

#define DDSHST_ReaderFactory_create_component(f_,p_,l_) \
(struct DDSHST_Reader*)((f_)->intf)->create_component(f_,p_,l_)

#define DDSHST_ReaderFactory_delete_component(f_,c_) \
        ((f_)->intf)->delete_component(f_,(RT_Component_T*)(c_))

#define DDSHST_Reader_get_entry(self_,rw_,key_,si_,strength_) \
        ((struct DDSHST_ReaderI*)((self_)->_parent._intf))->get_entry(\
                self_,rw_,key_,si_,strength_)

#define DDSHST_Reader_return_entry(self_,entry_) \
        ((struct DDSHST_ReaderI*)((self_)->_parent._intf))->return_entry(\
                self_,entry_)

#define DDSHST_Reader_commit_entry(self_,entry_,sample_,sn_) \
        ((struct DDSHST_ReaderI*)((self_)->_parent._intf))->commit_entry(\
                self_,entry_,\
                sample_,sn_)

#define DDSHST_Reader_commit_sample(self_,rw_,sn_,s_) \
        ((struct DDSHST_ReaderI*)((self_)->_parent._intf))->commit_sample(\
                self_,rw_,sn_,s_)

#define DDSHST_Reader_register_key(self_,key_) \
        ((struct DDSHST_ReaderI*)((self_)->_parent._intf))->register_key(\
                self_,key_)

#define DDSHST_Reader_unregister_key(self_,si_) \
        ((struct DDSHST_ReaderI*)((self_)->_parent._intf))->unregister_key(\
                self_,si_)

#define DDSHST_Reader_read_or_take(self_,\
        sample_arr_,info_arr_,count_,handle_,max_samples_,sample_states_,view_states_,\
        instance_states_,take_) \
        ((struct DDSHST_ReaderI*)(\
                (self_)->_parent._intf))->read_or_take(self_,\
                sample_arr_,info_arr_,count_,handle_,max_samples_,\
                sample_states_,view_states_,instance_states_,take_)

#define DDSHST_Reader_finish_read_or_take(self_,sample_seq_,info_seq_,count_,take_) \
        ((struct DDSHST_ReaderI*)(\
                (self_)->_parent._intf))->finish_read_or_take(self_,\
                        sample_seq_,info_seq_,count_,take_)

#define DDSHST_Reader_return_loan(self_,sample_seq_,info_seq_,count_) \
        ((struct DDSHST_ReaderI*)(\
                (self_)->_parent._intf))->finish_read_or_take(self_,\
                     sample_seq_,info_seq_,count_,DDS_BOOLEAN_FALSE)

#define DDSHST_Reader_post_event(self_,event_,time_) \
        ((struct DDSHST_ReaderI*)(\
                (self_)->_parent._intf))->post_event(self_,event_,time_)

#define DDSHST_Reader_dispose(self_,si_) \
        ((struct DDSHST_ReaderI*)(\
                (self_)->_parent._intf))->dispose(self_,si_)

#define DDSHST_Reader_get_info(self_) \
        ((struct DDSHST_ReaderI*)(\
                (self_)->_parent._intf))->get_info(self_)

#define DDSHST_Reader_lookup_key(self_,key_) \
        ((struct DDSHST_ReaderI*)(\
                (self_)->_parent._intf))->lookup_key(self_,key_)



#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* dds_c_rh_plugin_h */
