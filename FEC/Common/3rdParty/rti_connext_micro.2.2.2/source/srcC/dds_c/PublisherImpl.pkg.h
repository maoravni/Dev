/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef PublisherImpl_pkg_h
#define PublisherImpl_pkg_h


#include "DataWriterImpl.pkg.h"

typedef void
(*PublisherLifeCycleListener_on_after_created)(DDS_Publisher *const writer,
                          const struct DDS_PublisherQos *const qos);

typedef void
(*PublisherLifeCycleListener_on_after_modified)(DDS_Publisher *const writer,
                          const struct DDS_PublisherQos *const qos,
                          const struct DDS_PublisherListener *const listener);

typedef void
(*PublisherLifeCycleListener_on_after_enabled)(DDS_Publisher *const writer,
                          const struct DDS_PublisherQos *const qos);

typedef void
(*PublisherLifeCycleListener_on_before_deleted)(DDS_Publisher *const writer);

typedef void
(*DomainParticipantListener_on_offered_deadline_missed)(
                       DDS_DomainParticipant *self,
                       DDS_DataWriter* writer,
                       const struct DDS_OfferedDeadlineMissedStatus* status);

typedef void
(*DomainParticipantListener_on_liveliness_lost)(
                       DDS_DomainParticipant *self,
                       DDS_DataWriter *writer,
                       const struct DDS_LivelinessLostStatus *status);

typedef void
(*DomainParticipantListener_on_offered_incompatible_qos)(
        DDS_DomainParticipant *self,
        DDS_DataWriter* writer,
        const struct DDS_OfferedIncompatibleQosStatus *status);

typedef void
(*DomainParticipantListener_on_publication_matched)(
        DDS_DomainParticipant *self,
        DDS_DataWriter *writer,
        const struct DDS_PublicationMatchedStatus *status);

struct NDDS_PublisherConfig
{
    DB_Database_T db;
    OSAPI_Timer_t timer;
    RT_Registry_T *registry;
    DB_Table_T local_writer_table;
    DataWriterLifeCycleListener_on_after_created on_after_datawriter_created;
    DataWriterLifeCycleListener_on_after_enabled on_after_datawriter_enabled;
    DataWriterLifeCycleListener_on_after_modified on_after_datawriter_modified;
    DataWriterLifeCycleListener_on_before_deleted on_before_datawriter_deleted;
    PublisherLifeCycleListener_on_after_enabled on_after_enabled;
    PublisherLifeCycleListener_on_after_modified on_after_modified;
    RTIDDS_EntityGetInstanceHandleFunction get_parent_handle;
    RTIDDS_ObjectIdGenerator object_id_generator;
    DomainParticipantListener_on_offered_deadline_missed on_offered_deadline_missed;
    DomainParticipantListener_on_liveliness_lost on_liveliness_lost;
    DomainParticipantListener_on_offered_incompatible_qos on_offered_incompatible_qos;
    DomainParticipantListener_on_publication_matched on_publication_matched;
    struct DDS_LocatorSeq *default_unicast;
    struct DDS_LocatorSeq *default_multicast;
    struct DDS_LocatorSeq *default_meta_unicast;
    struct DDS_LocatorSeq *default_meta_multicast;
    NETIO_BindResolver_T *bind_resolver;
    struct REDA_StringSeq *enabled_transports;
    NETIO_RouteResolver_T *route_resolver;
};


#define NDDS_PublisherConfig_INITIALIZER \
{ \
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

struct DDS_PublisherImpl
{
    struct DDS_EntityImpl as_entity;

    /*----- From create call -----*/
    struct DDS_PublisherQos qos;
    struct DDS_PublisherListener listener;
    DDS_StatusMask mask;
    DDS_DomainParticipant *participant;

    /*----- Support for API -----*/
    struct DDS_DataWriterQos default_qos;

    /*----- Internal management -----*/
    DDS_Long dw_count;

    /*----- Properties ------*/
    struct NDDS_PublisherConfig config;
};

extern RTI_INT32
DDS_PublisherImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                          const DB_Record_T op1, void *op2, void *op3);

extern DDS_Boolean
DDS_PublisherImpl_is_hidden(DDS_Publisher *self);

extern DDS_Boolean
DDS_PublisherImpl_finalize(struct DDS_PublisherImpl *self);

extern DDS_InstanceHandle_t
DDS_PublisherImpl_get_instance_handle(DDS_Entity *entity);

extern DDS_Boolean
DDS_PublisherImpl_initialize(struct DDS_PublisherImpl *publisher,
                             DDS_DomainParticipant *participant,
                             const struct DDS_PublisherQos *qos,
                             const struct DDS_PublisherListener *listener,
                             DDS_StatusMask mask,
                             DDS_Long object_id,
                             struct NDDS_PublisherConfig *config);

extern DDS_Boolean
DDS_PublisherImpl_listener_is_consistent(const struct DDS_PublisherListener *self,
                                         DDS_StatusMask mask);

#endif /* PublisherImpl_pkg_h */
