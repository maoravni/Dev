/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_wh_plugin_h
#define dds_c_wh_plugin_h


#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef dds_c_topic_h
#include "dds_c/dds_c_topic.h"
#endif
#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef dds_c_discovery_h
#include "dds_c/dds_c_discovery.h"
#endif
#ifndef dds_c_type_h
#include "dds_c/dds_c_type.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    DDSHST_WRITEREVENT_KIND_DEADLINE_EXPIRED = 1
} DDSHST_WriterEventKind_T;

struct DDSHST_WriterEvent
{
    DDSHST_WriterEventKind_T kind;
};

struct DDSHST_Writer
{
    struct RT_Component _parent;
};

typedef struct DDSHST_Writer DDSHST_Writer_T;

struct DDSHST_WriterSampleEntry;
typedef struct DDSHST_WriterSampleEntry* DDSHST_WriterSampleEntryRef_T;
typedef struct DDSHST_WriterSample
{
    struct REDA_Buffer _user_data;
    struct REDA_Buffer _inline_data;
    struct OSAPI_NtpTime _timestamp;
    DDSHST_WriterSampleEntryRef_T _ref;
} DDSHST_WriterSample_T;

struct DDSHST_WriterState
{
    struct REDA_SequenceNumber low_sn;
    struct REDA_SequenceNumber high_sn;
};

struct DDSHST_WriterInfo
{
    RTI_UINT32 version;
};

struct DDSHST_WriterProperty
{
    struct RT_ComponentProperty _parent;
    struct DDS_DataWriterQos *qos;
};

#define DDSHST_WriterProperty_INITIALIZER \
{\
  RT_ComponentProperty_INITIALIZER,\
  NULL\
}

typedef void
(*DDSHST_WriterListener_OnSampleRemovedFunc)(
        void *listener_data,
        DDS_InstanceHandle_t *key,
        struct DDSHST_WriterSample * sample);

typedef void
(*DDSHST_WriterListener_OnDeadlineMissedFunc)(
        void *listener_data,
        DDS_InstanceHandle_t *key);

typedef void
(*DDSHST_WriterListener_OnKeyRemovedFunc)(
        void *listener_data,
        DDS_InstanceHandle_t *key);

struct DDSHST_WriterListener
{
    struct RT_ComponentListener _parent;
    DDSHST_WriterListener_OnSampleRemovedFunc on_sample_removed;
    DDSHST_WriterListener_OnKeyRemovedFunc on_key_removed;
    DDSHST_WriterListener_OnDeadlineMissedFunc on_deadline_missed;
    void *listener_data;
};

#define DDSHST_WriterListener_INITIALIZE {\
  RT_ComponentListener_INITIALIZER,\
  NULL,\
  NULL,\
  NULL,\
  NULL\
}

#define DDSHST_Writer_narrow(factory_) (((struct DDSHST_Writer*)(factory_))->intf)

/* ------------------------------ Life-cycle -------------------------------- */
typedef void
(*DDSHST_Writer_deleteFunc) (struct DDSHST_Writer * self);

typedef struct DDSHST_Writer*
(*DDSHST_Writer_createFunc) (
        const struct DDSHST_WriterProperty* const property,
        const struct DDSHST_WriterListener* const listener);

/* ------------------------------- Methods ---------------------------------- */

typedef DDSHST_WriterSampleEntryRef_T
(*DDSHST_Writer_get_entryFunc)(
        struct DDSHST_Writer * self,
        const DDS_InstanceHandle_t *const key);

typedef void
(*DDSHST_Writer_return_entryFunc)(
        struct DDSHST_Writer * self,
        DDSHST_WriterSampleEntryRef_T const entry);

typedef DDSHST_ReturnCode_T
(*DDSHST_Writer_commit_entryFunc)(
        struct DDSHST_Writer * self,
        DDSHST_WriterSampleEntryRef_T entry,
        DDSHST_WriterSample_T *const sample,
        const struct REDA_SequenceNumber *const sn);

typedef DDSHST_ReturnCode_T
(*DDSHST_Writer_commit_sampleFunc)(
        struct DDSHST_Writer * self,
        const struct REDA_SequenceNumber* const sn,
        DDS_Long sn_count);

typedef DDSHST_ReturnCode_T
(*DDSHST_Writer_request_sampleFunc)(
        struct DDSHST_Writer *self,
        struct DDSHST_WriterSample **sample,
        const struct REDA_SequenceNumber *const sn);

typedef DDSHST_ReturnCode_T
(*DDSHST_Writer_loan_finishedFunc)(
        struct DDSHST_Writer * self,
        const struct REDA_SequenceNumber* const sn);

typedef DDSHST_ReturnCode_T
(*DDSHST_Writer_ack_sampleFunc)(
        struct DDSHST_Writer *self,
        const struct REDA_SequenceNumber *const sn);

typedef DDSHST_ReturnCode_T
(*DDSHST_Writer_register_keyFunc)(
        struct DDSHST_Writer *self,
        const DDS_InstanceHandle_t *const key);

typedef DDSHST_ReturnCode_T
(*DDSHST_Writer_unregister_keyFunc)(
        struct DDSHST_Writer *self,
        const DDS_InstanceHandle_t *const key);

typedef void
(*DDSHST_Writer_update_stateFunc) (
        struct DDSHST_Writer *self,
        const struct OSAPI_NtpTime *const now);

typedef struct DDSHST_WriterState*
(*DDSHST_Writer_get_stateFunc)(struct DDSHST_Writer * self);

typedef struct DDSHST_WriterInfo*
(*DDSHST_Writer_get_infoFunc)(struct DDSHST_Writer * self);

typedef void
(*DDSHST_Writer_post_eventFunc)(struct DDSHST_Writer *self,
                                struct DDSHST_WriterEvent *event,
                                struct OSAPI_NtpTime * now);

struct DDSHST_WriterI
{
    struct RT_ComponentI _parent;
    DDSHST_Writer_get_entryFunc get_entry;
    DDSHST_Writer_return_entryFunc return_entry;
    DDSHST_Writer_commit_entryFunc commit_entry;
    DDSHST_Writer_commit_sampleFunc commit_sample;
    DDSHST_Writer_request_sampleFunc request_sample;
    DDSHST_Writer_ack_sampleFunc ack_sample;
    DDSHST_Writer_register_keyFunc register_key;
    DDSHST_Writer_unregister_keyFunc unregister_key;
    DDSHST_Writer_update_stateFunc update_state;
    DDSHST_Writer_loan_finishedFunc loan_finished;
    DDSHST_Writer_get_stateFunc get_state;
    DDSHST_Writer_get_infoFunc get_info;
    DDSHST_Writer_post_eventFunc post_event;
};

#define DDSHST_WriterFactory_create_component(f_,p_,l_) \
    (struct DDSHST_Writer*)((f_)->intf)->create_component(f_,p_,l_)

#define DDSHST_WriterFactory_delete_component(f_,c_) \
    ((f_)->intf)->delete_component(f_,(RT_Component_T*)(c_))

#define DDSHST_Writer_get_entry(self_,key_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->get_entry(self_,key_)

#define DDSHST_Writer_return_entry(self_,entry_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->return_entry(self_,entry_)

#define DDSHST_Writer_commit_entry(self_,entry_,sample_,sn_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->commit_entry(self_,entry_,\
            sample_,sn_)

#define DDSHST_Writer_commit_sample(self_,sn_,cnt_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->commit_entry(self_,sn_,cnt_)

#define DDSHST_Writer_request_sample(self_,sample_,sn_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->request_sample(self_,sample_,sn_)

#define DDSHST_Writer_loan_finished(self_,sn_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->loan_finished(self_,sn_)

#define DDSHST_Writer_ack_sample(self_,sn_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->ack_sample(self_,sn_)

#define DDSHST_Writer_register_key(self_,key_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->register_key(self_,key_)

#define DDSHST_Writer_unregister_key(self_,key_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->unregister_key(self_,key_)

#define DDSHST_Writer_update_state(self_,time_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->update_state(self_,time_)

#define DDSHST_Writer_get_state(self_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->get_state(self_)

#define DDSHST_Writer_get_info(self_) \
    ((struct DDSHST_WriterI*)(\
            (self_)->_parent._intf))->get_info(self_)

#define DDSHST_Writer_post_event(self_,event_,time_) \
        ((struct DDSHST_WriterI*)(\
                (self_)->_parent._intf))->post_event(self_,event_,time_)

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_wh_plugin_h */
