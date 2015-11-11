/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DomainParticipant_pkg_h
#define DomainParticipant_pkg_h

struct NDDS_ParticipantConfig
{
    RT_Registry_T *registry;
    struct DDS_GUID_t guid;
    DB_Database_T db;
    DDS_Long instance;
};


#define NDDS_ParticipantConfig_INITIALIZER \
{ \
    NULL,\
    DDS_GUID_INITIALIZER,\
    NULL\
}

struct DDS_DomainParticipantImpl
{
    struct DDS_EntityImpl as_entity;

    struct DDS_DomainParticipantQos qos;
    struct DDS_DomainParticipantListener listener;
    DDS_StatusMask mask;
    DDS_DomainId_t domain_id;
    struct DDS_PublisherQos _default_publisher_qos;
    struct DDS_SubscriberQos _default_subscriber_qos;
    struct DDS_TopicQos _default_topic_qos;

    DB_Database_T database;
    DB_Table_T local_publisher_table;
    DB_Table_T local_subscriber_table;
    DB_Table_T local_reader_table;
    DB_Table_T local_writer_table;
    DB_Table_T remote_participant_table;
    DB_Table_T remote_publisher_table;
    DB_Table_T remote_subscriber_table;
    DB_Table_T topic_table;
    DB_Table_T type_table;

    struct NDDS_Discovery_Plugin *disc_plugin;

    struct OSAPI_Mutex *shared_lock;
    struct OSAPI_Mutex *db_lock;
    struct OSAPI_Mutex *finalize_lock;
    OSAPI_Timer_t timer;
    RTI_UINT32 object_id;
    DDS_Long participant_id;

    struct DDS_ParticipantBuiltinTopicData builtin_data;

    NETIO_BindResolver_T *bind_resolver;
    NETIO_RouteResolver_T *route_resolver;

    NETIO_Interface_T *rtps_intf;

    struct NDDS_ParticipantConfig config;

    struct NETIO_RtpsPortData rtps_port_data;
};

extern DDS_InstanceHandle_t
DDS_DomainParticipantImpl_get_instance_handle(DDS_Entity *entity);

extern DDS_ReturnCode_t
DDS_DomainParticipant_finalize(DDS_DomainParticipant *self);

extern DDS_DomainParticipant*
DDS_DomainParticipant_initialize(
                          struct DDS_DomainParticipantImpl *participant,
                          DDS_DomainParticipantFactory *factory,
                          DDS_DomainId_t domain_id,
                          const struct DDS_DomainParticipantQos *qos,
                          const struct DDS_DomainParticipantListener *listener,
                          DDS_StatusMask mask,
                          struct NDDS_ParticipantConfig *config,
                          struct OSAPI_Mutex *factory_lock);

#endif /* DomainParticipant_pkg_h */
