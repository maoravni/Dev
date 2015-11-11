/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataWriterImpl_pkg_h
#define DataWriterImpl_pkg_h

typedef void
(*DataWriterLifeCycleListener_on_after_created)(DDS_DataWriter *const dw,
                        const struct DDS_DataWriterQos *const qos);

typedef void
(*DataWriterLifeCycleListener_on_after_modified)(DDS_DataWriter *const dw,
                        const struct DDS_DataWriterQos *const qos,
                        const struct DDS_DataWriterListener *const listener);

typedef void
(*DataWriterLifeCycleListener_on_after_enabled)(
                        DDS_DataWriter *const writer,
                        const struct DDS_DataWriterQos *const qos);

typedef void
(*DataWriterLifeCycleListener_on_before_deleted)(DDS_DataWriter *const dw);

struct NDDS_DataWriterConfig
{
    DB_Database_T db;
    OSAPI_Timer_t timer;
    RT_Registry_T *registry;
    NETIO_BindResolver_T *bind_resolver;
    DataWriterLifeCycleListener_on_after_enabled on_after_enabled;
    DataWriterLifeCycleListener_on_after_modified on_after_modified;
    RTIDDS_EntityGetInstanceHandleFunction get_parent_handle;
    struct DDS_LocatorSeq *default_unicast;
    struct DDS_LocatorSeq *default_multicast;
    struct DDS_LocatorSeq *default_meta_unicast;
    struct DDS_LocatorSeq *default_meta_multicast;
    struct REDA_StringSeq *enabled_transports;
    NETIO_RouteResolver_T *route_resolver;
};


#define NDDS_DataWriterConfig_INITIALIZER \
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


struct DDS_DataWriterImpl
{
    struct DDS_EntityImpl as_entity;

    /*----- Support for DDS API -----*/
    struct DDS_DataWriterQos qos;
    struct DDS_DataWriterListener listener;
    DDS_StatusMask mask;

    /*----- Internal state management -----*/
    struct OSAPI_NtpTime topic_deadline;
    struct OSAPI_NtpTime liveliness_time;
    OSAPITimeoutHandle_t deadline_event;
    OSAPITimeoutHandle_t liveliness_event;

    DDS_Long deadline_missed_total_count;
    DDS_Long deadline_missed_total_count_change;
    DDS_Long liveliness_lost_total_count;
    DDS_Long liveliness_lost_total_count_change;
    DDS_Long offered_incompatible_qos_total_count;
    DDS_Long offered_incompatible_qos_total_count_change;
    DDS_QosPolicyId_t   last_policy_id;
    DDS_Long publication_matched_status_total_count;
    DDS_Long publication_matched_status_total_count_change;
    DDS_Long publication_matched_status_current_count;
    DDS_Long publication_matched_status_current_count_change;
    DDS_Topic *topic;
    DDS_Publisher *publisher;

    /* --- DDS History Cache --- */
    struct DDSHST_Writer *wh;

    /* --- Data-path interfaces --- */
    NETIO_Interface_T *dw_intf;
    NETIO_Interface_T *rtps_intf;

    /* --- Type-Plugin --- */
    struct NDDS_Type_Plugin *type_plugin;

    /* --- Samples --- */
    NDDS_TypePluginKeyKind key_kind;
    struct REDA_SequenceNumber last_sn;
    struct CDR_Stream_t stream;
    struct CDR_Stream_t *md5Stream;

    REDA_BufferPool_T cdr_samples;

    RTI_UINT32 max_cdr_serialized_length;
    RTI_UINT32 max_packet_length;

    NETIO_Packet_T lost_data_packet;
    /* --- Properties structure --- */
    struct NDDS_DataWriterConfig config;
};

struct NDDS_DataWriterSampleInfo
{
    struct OSAPI_NtpTime timestamp;
    MIGRtpsStatusInfo statusInfo;
    DDS_Boolean sendKeyHash;
};

struct RTI_TransformCDR_Sample
{
    struct DDSHST_WriterSample _sample;
    void *payload;
    DDS_Long _max_length;
    DDS_Long _max_config_length;
    NETIO_Packet_T packet;
};

extern RTI_INT32
DDS_DataWriterImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                           const DB_Record_T op1, void *op2, void *op3);

extern DDS_Long
DDS_DataWriter_get_objectid(DDS_DataWriter * self);

extern DDS_Boolean
DDS_DataWriter_listener_is_consistent(const struct DDS_DataWriterListener *self,
                                      DDS_StatusMask mask);

extern DDS_Boolean
DDS_DataWriterImpl_finalize(DDS_DataWriter *self);

extern DDS_Boolean
DDS_DataWriterImpl_initialize(struct DDS_DataWriterImpl *datawriter,
                              DDS_Publisher *publisher,
                              DDS_Topic *topic,
                              const struct DDS_DataWriterQos *qos,
                              const struct DDS_DataWriterListener *listener,
                              DDS_StatusMask mask,
                              DDS_Long object_id,
                              const struct NDDS_DataWriterConfig *config);

extern DDS_ReturnCode_t
DDS_DataWriter_write_untyped(DDS_DataWriter *self,
                             const void *instance_data,
                             const DDS_InstanceHandle_t *handle,
                             struct NDDS_DataWriterSampleInfo *sample_info);

extern DDS_ReturnCode_t
DDS_DataWriter_register_key(struct DDS_DataWriterImpl *datawriter, /* self */
                            DDS_InstanceHandle_t *handle, /* inout */
                            const void *instance_data, /* in */
                            const struct OSAPI_NtpTime *timestamp /* in */);

#endif /* DataWriterImpl_pkg_h */
