/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataReaderImpl_pkg_h
#define DataReaderImpl_pkg_h

typedef void
(*DataReaderLifeCycleListener_on_after_created)(DDS_DataReader *const dw,
                        const struct DDS_DataReaderQos *const qos);

typedef void
(*DataReaderLifeCycleListener_on_after_modified)(DDS_DataReader *const dw,
                        const struct DDS_DataReaderQos *const qos,
                        const struct DDS_DataReaderListener *const listener);

typedef void
(*DataReaderLifeCycleListener_on_after_enabled)(
                        DDS_DataReader *const writer,
                        const struct DDS_DataReaderQos *const qos);

typedef void
(*DataReaderLifeCycleListener_on_before_deleted)(DDS_DataReader *const dw);

struct NDDS_DataReaderConfig
{
    DB_Database_T db;
    OSAPI_Timer_t timer;
    RT_Registry_T *registry;
    DataReaderLifeCycleListener_on_after_enabled on_after_enabled;
    DataReaderLifeCycleListener_on_after_modified on_after_modified;
    RTIDDS_EntityGetInstanceHandleFunction get_parent_handle;
    struct DDS_LocatorSeq *default_unicast;
    struct DDS_LocatorSeq *default_multicast;
    struct DDS_LocatorSeq *default_meta_unicast;
    struct DDS_LocatorSeq *default_meta_multicast;
    NETIO_BindResolver_T *bind_resolver;
    struct REDA_StringSeq *enabled_transports;
    NETIO_RouteResolver_T *route_resolver;
};


#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif

#define NDDS_DataReaderConfig_INITIALIZER \
{\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL,\
     NULL\
}


struct RTI_DataReaderSample
{
    struct RTI_DataReaderSample *_next;
    DDSHST_ReaderSample_T hst_sample;
};

typedef struct DDS_ReaderQueueElement
{
    struct DDS_SampleInfo info;
    void *data;
    DDS_Boolean is_valid;
} DDS_ReaderQueueElement_t;

struct DDS_DataReaderImpl
{
    struct DDS_EntityImpl as_entity;

    /*----- Object Key -----*/
    DDS_Long object_id;

    /*----- From create call -----*/
    struct DDS_DataReaderQos qos;
    struct DDS_DataReaderListener listener;
    DDS_StatusMask mask;

    /*----- Internal management -----*/
    DDS_Topic *topic;
    DDS_Subscriber *subscriber;
    struct NDDS_Type_Plugin *type_plugin;
    NDDS_TypePluginKeyKind key_kind;

    /*---- DDS ReaderState to manage DDS semantics ----*/
    struct OSAPI_NtpTime last_sample_recvd_time;
    OSAPITimeoutHandle_t deadline_event;
    OSAPITimeoutHandle_t liveliness_event;

    DDS_Long deadline_missed_total_count;
    DDS_Long deadline_missed_total_count_change;
    DDS_Long liveliness_changed_alive_count;
    DDS_Long liveliness_changed_not_alive_count;
    DDS_Long liveliness_changed_alive_count_change;
    DDS_Long liveliness_changed_not_alive_count_change;
    DDS_Long requested_incompatible_qos_total_count;
    DDS_Long requested_incompatible_qos_total_count_change;
    DDS_Long subscription_matched_total_count;
    DDS_Long subscription_matched_total_count_change;
    DDS_Long subscription_matched_current_count;
    DDS_Long subscription_matched_current_count_change;
    DDS_Long sample_lost_status_total_count;
    DDS_Long sample_lost_status_total_count_change;
    DDS_Long sample_rejected_status_total_count;
    DDS_Long sample_rejected_status_total_count_change;

    struct DDS_SampleRejectedStatus rejected_status;

    DDS_QosPolicyId_t   last_policy_id;

    struct CDR_Stream_t *md5_stream;
    struct CDR_Stream_t stream;

    struct DDSHST_Reader *_rh;

    NETIO_Interface_T *dr_intf;
    NETIO_Interface_T *rtps_intf;
    REDA_BufferPool_T cdr_samples;

    DDS_Long read_seq_max;

    struct NDDS_DataReaderConfig config;
};

extern RTI_INT32
DDS_DataReaderImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                           const DB_Record_T op1, void *op2, void *op3);

extern DDS_Long
DDS_DataReader_get_objectid(DDS_DataReader * self);

extern DDS_Boolean
DDS_DataReader_listener_is_consistent(const struct DDS_DataReaderListener *self,
                                      DDS_StatusMask mask);

extern DDS_Boolean
DDS_DataReaderImpl_finalize(DDS_DataReader *self);

extern DDS_Boolean
DDS_DataReaderImpl_initialize(struct DDS_DataReaderImpl *datareader,
                       DDS_Subscriber * subscriber,
                       DDS_TopicDescription * topic_description,
                       const struct DDS_DataReaderQos *qos,
                       const struct DDS_DataReaderListener *listener,
                       DDS_StatusMask mask,
                       DDS_Long object_id,
                       struct NDDS_DataReaderConfig *config);

extern DDS_Boolean
DDS_DataReader_is_enabled(DDS_DataReader * self);

#endif /* DataReaderImpl_pkg_h */
