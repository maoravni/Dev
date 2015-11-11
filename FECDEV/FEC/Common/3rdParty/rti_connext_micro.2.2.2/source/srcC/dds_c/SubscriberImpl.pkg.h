/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef SubscriberImpl_pkg_h
#define SubscriberImpl_pkg_h


#include "DataReaderImpl.pkg.h"

typedef void
(*SubscriberLifeCycleListener_on_after_modified)(
                        DDS_Subscriber *const subscriber,
                        const struct DDS_SubscriberQos *const qos,
                        const struct DDS_SubscriberListener *const listener);

typedef void
(*SubscriberLifeCycleListener_on_after_enabled)(
                        DDS_Subscriber *const reader,
                        const struct DDS_SubscriberQos *const qos);

typedef void
(*SubscriberLifeCycleListener_on_before_deleted)(
                         DDS_Subscriber *const reader);

typedef void
(*DomainParticipantListener_on_data_on_readers)(
                           DDS_DomainParticipant *self,
                           DDS_Subscriber *sub);

typedef void
(*DomainParticipantListener_on_requested_deadline_missed)(
                        DDS_DomainParticipant *self,
                        DDS_DataReader *reader,
                        const struct DDS_RequestedDeadlineMissedStatus *status);

typedef void
(*DomainParticipantListener_on_requested_incompatible_qos)(
                        DDS_DomainParticipant *self,
                        DDS_DataReader *reader,
                        const struct DDS_RequestedIncompatibleQosStatus *status);

typedef void
(*DomainParticipantListener_on_sample_rejected)(
                        DDS_DomainParticipant *self,
                        DDS_DataReader *reader,
                        const struct DDS_SampleRejectedStatus *status);

typedef void
(*DomainParticipantListener_on_liveliness_changed)(
                        DDS_DomainParticipant *self,
                        DDS_DataReader *reader,
                        const struct DDS_LivelinessChangedStatus *status);

typedef void
(*DomainParticipantListener_on_data_available)(
                        DDS_DomainParticipant *self,
                        DDS_Subscriber *subscriber,
                        DDS_DataReader *reader);

typedef void
(*DomainParticipantListener_on_subscription_matched)(
                           DDS_DomainParticipant *self,
                           DDS_DataReader *reader,
                           const struct DDS_SubscriptionMatchedStatus *status);

typedef void
(*DomainParticipantListener_on_sample_lost)(
                           DDS_DomainParticipant *self,
                           DDS_DataReader *reader,
                           const struct DDS_SampleLostStatus *status);

struct NDDS_SubscriberConfig
{
    DB_Database_T db;
    OSAPI_Timer_t timer;
    RT_Registry_T *registry;
    DB_Table_T local_reader_table;
    DataReaderLifeCycleListener_on_after_created on_after_datareader_created;
    DataReaderLifeCycleListener_on_after_enabled on_after_datareader_enabled;
    DataReaderLifeCycleListener_on_after_modified on_after_datareader_modified;
    DataReaderLifeCycleListener_on_before_deleted on_before_datareader_deleted;
    SubscriberLifeCycleListener_on_after_enabled on_after_enabled;
    SubscriberLifeCycleListener_on_after_modified on_after_modified;
    RTIDDS_EntityGetInstanceHandleFunction get_parent_handle;
    RTIDDS_ObjectIdGenerator object_id_generator;
    DomainParticipantListener_on_data_on_readers on_data_on_readers;
    DomainParticipantListener_on_requested_deadline_missed on_requested_deadline_missed;
    DomainParticipantListener_on_requested_incompatible_qos on_requested_incompatible_qos;
    DomainParticipantListener_on_sample_rejected on_sample_rejected;
    DomainParticipantListener_on_liveliness_changed on_liveliness_changed;
    DomainParticipantListener_on_data_available on_data_available;
    DomainParticipantListener_on_subscription_matched on_subscription_matched;
    DomainParticipantListener_on_sample_lost on_sample_lost;
    struct DDS_LocatorSeq *default_unicast;
    struct DDS_LocatorSeq *default_multicast;
    struct DDS_LocatorSeq *default_meta_unicast;
    struct DDS_LocatorSeq *default_meta_multicast;
    NETIO_BindResolver_T *bind_resolver;
    struct REDA_StringSeq *enabled_transports;
    NETIO_RouteResolver_T *route_resolver;
};


#define NDDS_SubscriberConfig_INITIALIZER \
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
    NULL,\
    NULL,\
    NULL,\
    NULL,\
    NULL \
}

struct DDS_SubscriberImpl
{
    struct DDS_EntityImpl as_entity;

    /*----- From create call -----*/
    struct DDS_SubscriberQos qos;
    struct DDS_SubscriberListener listener;
    DDS_StatusMask mask;
    DDS_DomainParticipant *participant;

    /*----- Support for API -----*/
    struct DDS_DataReaderQos default_qos;

    /*----- Internal management -----*/
    DDS_Long dr_count;


    /*----- Properties -----*/
    struct NDDS_SubscriberConfig config;
};

extern RTI_INT32
DDS_SubscriberImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                               const DB_Record_T op1, void *op2, void *op3);

extern DDS_Boolean
DDS_SubscriberImpl_is_hidden(struct DDS_SubscriberImpl * self);

extern DDS_Boolean
DDS_SubscriberImpl_finalize(struct DDS_SubscriberImpl * self);

extern DDS_InstanceHandle_t
DDS_SubscriberImpl_get_instance_handle(DDS_Entity *entity);

extern DDS_Boolean
DDS_SubscriberImpl_initialize(struct DDS_SubscriberImpl *subscriber,
                              DDS_DomainParticipant *participant,
                              const struct DDS_SubscriberQos *qos,
                              const struct DDS_SubscriberListener *listener,
                              DDS_StatusMask mask,
                              DDS_Long object_id,
                              struct NDDS_SubscriberConfig *config);

extern DDS_Boolean
DDS_SubscriberImpl_listener_is_consistent(const struct DDS_SubscriberListener *self,
                                          DDS_StatusMask mask);

#endif /* SubscriberImpl_pkg_h */
