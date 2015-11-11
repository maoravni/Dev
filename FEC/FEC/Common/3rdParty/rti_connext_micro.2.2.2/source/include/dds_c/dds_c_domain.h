/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_domain_h
#define dds_c_domain_h


#ifndef NULL
#define NULL 0
#endif
#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef reda_string_h
#include "reda/reda_string.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif
#ifndef dds_c_type_h
#include "dds_c/dds_c_type.h"
#endif
#ifndef dds_c_topic_h
#include "dds_c/dds_c_topic.h"
#endif
#ifndef dds_c_publication_h
#include "dds_c/dds_c_publication.h"
#endif
#ifndef dds_c_subscription_h
#include "dds_c/dds_c_subscription.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* ================================================================= */
/*                       Basic Types                                 */
/* ================================================================= */

/*e \dref_DomainId_t
 */
typedef DDS_DOMAINID_TYPE_NATIVE DDS_DomainId_t;

/* ----------------------------------------------------------------- */
/*                DISCOVERY_X (eXtension QoS)                        */
/* ----------------------------------------------------------------- */
/*e \dref_DiscoveryQosGroupDocs
 */

struct NDDS_Discovery_Property
{
    struct RT_ComponentProperty _parent;
};

#define NDDS_Discovery_Property_INITIALIZER \
{\
  RT_ComponentProperty_INITIALIZER\
}

/*e \dref_DISCOVERY_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_DISCOVERY_QOS_POLICY_NAME;

/*e \dref_DiscoveryComponent
 */
struct DDS_DiscoveryComponent
{
    /*e \dref_DiscoveryComponent_id
     */
    char name[RT_MAX_FACTORY_NAME];

    struct NDDS_Discovery_Property property;
};

#define DDS_DiscoveryComponent_INITIALIZER \
{\
  {0},\
  NDDS_Discovery_Property_INITIALIZER\
}

DDS_SEQUENCE(DDS_DiscoveryComponentSeq, struct DDS_DiscoveryComponent)

/*e \dref_DiscoveryQosPolicy
 */
struct DDS_DiscoveryQosPolicy
{
    /*e \dref_DiscoveryQosPolicy_initial_peers
     */
    struct DDS_StringSeq initial_peers;

    /*e \dref_DiscoveryQosPolicy_enabled_transports
     */
    struct DDS_StringSeq enabled_transports;

    /*e \dref_DiscoveryQosPolicy_discovery
     */
    struct DDS_DiscoveryComponent discovery;
};

/*i @ingroup DDSDiscoveryQosModule
 */
#define DDS_DISCOVERY_QOS_POLICY_DEFAULT { \
    DDS_SEQUENCE_INITIALIZER(DDS_String),\
    DDS_SEQUENCE_INITIALIZER(DDS_String),\
    DDS_DiscoveryComponent_INITIALIZER\
}

/*e \dref_UserTrafficQosPolicy
 */
struct DDS_UserTrafficQosPolicy
{
    /*e \dref_UserTrafficQosPolicy_enabled_transports
     */
    struct DDS_StringSeq enabled_transports;
};

/*i @ingroup DDSDiscoveryQosModule
 */
#define DDS_USERTRAFFIC_QOS_POLICY_DEFAULT { \
    DDS_SEQUENCE_INITIALIZER(DDS_String) \
}

/* ================================================================= */
/*                       Listeners                                   */
/* ================================================================= */

struct DDS_DomainParticipantQos;
struct DDS_PublisherQos;
struct DDS_SubscriberQos;
struct DDS_DataReaderQos;
struct DDS_DataWriterQos;
struct DDS_TopicQos;
struct DDS_DomainParticipantListener;
struct DDS_PublisherListener;
struct DDS_SubscriberListener;
struct DDS_DataReaderListener;
struct DDS_DataWriterListener;
struct DDS_TopicListener;

/* ----------------------------------------------------------------- */

/*ce \dref_DomainParticipantListener
 */
struct DDS_DomainParticipantListener
{
    /*ce \dref_DomainParticipantListener_as_topiclistener
     */
    struct DDS_TopicListener as_topiclistener;

    /*ce \dref_DomainParticipantListener_as_publisherlistener
     */
    struct DDS_PublisherListener as_publisherlistener;

    /*ce \dref_DomainParticipantListener_as_subscriberlistener
     */
    struct DDS_SubscriberListener as_subscriberlistener;
};

/*ce \dref_DomainParticipantListener_INITIALIZER
 */
#define DDS_DomainParticipantListener_INITIALIZER   \
{\
    DDS_TopicListener_INITIALIZER,    \
    DDS_PublisherListener_INITIALIZER, \
    DDS_SubscriberListener_INITIALIZER \
}

/* ================================================================= */
/*                          QoS                                      */
/* ================================================================= */

/* ====================== */
/*  DomainParticipantQos  */
/* ====================== */

/*e \dref_DomainParticipantQos
 */
struct DDS_DomainParticipantQos
{
    /*e \dref_DomainParticipantQos_entity_factory
     */
    struct DDS_EntityFactoryQosPolicy entity_factory;

    /*e \dref_DomainParticipantQos_discovery
     */
    struct DDS_DiscoveryQosPolicy discovery;

    /*e \dref_DomainParticipantQos_resource_limits
     */
    struct DDS_DomainParticipantResourceLimitsQosPolicy resource_limits;

    /*e \dref_DomainParticipantQos_event
     */
    struct DDS_EventQosPolicy event;

    /*e \dref_DomainParticipantQos_participant_name
     */
    struct DDS_EntityNameQosPolicy participant_name;

    /*e \dref_DomainParticipantQos_wire_protocol
     */
    struct DDS_WireProtocolQosPolicy protocol;

    /*e \dref_DomainParticipantQos_transports
     */
    struct DDS_TransportQosPolicy transports;

    /*e \dref_DomainParticipantQos_user_traffic
     */
    struct DDS_UserTrafficQosPolicy user_traffic;
};

/*ce \dref_DomainParticipantQos_INITIALIZER
 */
#define DDS_DomainParticipantQos_INITIALIZER                \
{                                                           \
  DDS_ENTITY_FACTORY_QOS_POLICY_DEFAULT,                    \
  DDS_DISCOVERY_QOS_POLICY_DEFAULT,                         \
  DDS_DOMAIN_PARTICIPANT_RESOURCE_LIMITS_QOS_POLICY_DEFAULT,\
  DDS_EVENT_QOS_POLICY_DEFAULT,                             \
  DDS_ENTITY_NAME_QOS_POLICY_DEFAULT,                       \
  DDS_WIRE_PROTOCOL_QOS_POLICY_DEFAULT,                     \
  DDS_TRANSPORT_QOS_POLICY_DEFAULT,                         \
  DDS_USERTRAFFIC_QOS_POLICY_DEFAULT                        \
}

/*ce \dref_DomainParticipantQos_initialize
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantQos_initialize(struct DDS_DomainParticipantQos *self);

/*ce \dref_DomainParticipantQos_copy
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantQos_copy(struct DDS_DomainParticipantQos *self,
                              const struct DDS_DomainParticipantQos *source);

/*ce \dref_DomainParticipantQos_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantQos_finalize(struct DDS_DomainParticipantQos *self);

/* ============================== */
/*  DomainParticipantFactoryQos  */
/* ============================== */

/*e \dref_DomainParticipantFactoryQos
 */
struct DDS_DomainParticipantFactoryQos
{
    /*e \dref_DomainParticipantFactoryQos_entity_factory
     */
    struct DDS_EntityFactoryQosPolicy entity_factory;

    /*e \dref_DomainParticipantFactoryQos_resource_limits
     */
    struct DDS_SystemResourceLimitsQosPolicy resource_limits;
};

/*ce \dref_DDS_DomainParticipantFactoryQos_INITIALIZER
 */
#define DDS_DDS_DomainParticipantFactoryQos_INITIALIZER     \
{\
  DDS_ENTITY_FACTORY_QOS_POLICY_DEFAULT,                    \
  DDS_SYSTEM_RESOURCE_LIMITS_QOS_POLICY_DEFAULT             \
}

/*ce \dref_DomainParticipantFactoryQos_initialize
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactoryQos_initialize(
        struct DDS_DomainParticipantFactoryQos *self);

/*ce \dref_DomainParticipantFactoryQos_copy
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactoryQos_copy(
        struct DDS_DomainParticipantFactoryQos *self,
        const struct DDS_DomainParticipantFactoryQos *source);

/*ce \dref_DomainParticipantFactoryQos_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactoryQos_finalize(
                        struct DDS_DomainParticipantFactoryQos *self);

/*ce \dref_DomainParticipantFactoryQos_INITIALIZER
 */
#define DDS_DomainParticipantFactoryQos_INITIALIZER \
{\
    DDS_ENTITY_FACTORY_QOS_POLICY_DEFAULT,\
    DDS_SYSTEM_RESOURCE_LIMITS_QOS_POLICY_DEFAULT\
}

/* ================================================================= */
/*                        DomainParticipant                          */
/* ================================================================= */

/*e \dref_TOPIC_QOS_DEFAULT
 */
extern DDSCDllVariable const struct DDS_TopicQos
    DDS_TOPIC_QOS_DEFAULT;

/*e \dref_PUBLISHER_QOS_DEFAULT
 */
extern DDSCDllVariable const struct DDS_PublisherQos
    DDS_PUBLISHER_QOS_DEFAULT;

/*e \dref_SUBSCRIBER_QOS_DEFAULT
 */
extern DDSCDllVariable const struct DDS_SubscriberQos
    DDS_SUBSCRIBER_QOS_DEFAULT;

/* ----------------------------------------------------------------- */

#define DDS_DomainParticipant_as_entity(domainPtr) \
            ((DDS_Entity*) domainPtr)

#ifdef DOXYGEN_DOCUMENTATION_ONLY

/*ce \dref_DomainParticipant_as_entity
 */
DDS_Entity *DDS_DomainParticipant_as_entity(DDS_DomainParticipant * domain);

#endif /*DOXYGEN_DOCUMENTATION_ONLY */

/* ================================== */
/*  Get and Set Default QoS routines  */
/* ================================== */

/*ce \dref_DomainParticipant_get_default_publisher_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_get_default_publisher_qos(
        DDS_DomainParticipant *self,
        struct DDS_PublisherQos *qos);

/*ce \dref_DomainParticipant_set_default_publisher_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_set_default_publisher_qos(
                    DDS_DomainParticipant *self,
                    const struct DDS_PublisherQos *qos);

/*ce \dref_DomainParticipant_get_default_subscriber_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_get_default_subscriber_qos(
        DDS_DomainParticipant *self,
        struct DDS_SubscriberQos *qos);

/*ce \dref_DomainParticipant_set_default_subscriber_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_set_default_subscriber_qos(
        DDS_DomainParticipant *self,
        const struct DDS_SubscriberQos *qos);

/*ce \dref_DomainParticipant_get_default_topic_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_get_default_topic_qos(
        DDS_DomainParticipant *self,
        struct DDS_TopicQos *qos);

/*ce \dref_DomainParticipant_set_default_topic_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_set_default_topic_qos(
        DDS_DomainParticipant *self,
        const struct DDS_TopicQos *qos);

/* ============================ */
/*  Create and Delete routines  */
/* ============================ */

/*ce \dref_DomainParticipant_create_publisher
 */
DDSCDllExport DDS_Publisher*
DDS_DomainParticipant_create_publisher(
        DDS_DomainParticipant * self,
        const struct DDS_PublisherQos *qos,
        const struct DDS_PublisherListener *listener,
        DDS_StatusMask mask);

/*ce \dref_DomainParticipant_delete_publisher
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_delete_publisher(
        DDS_DomainParticipant *self,
        DDS_Publisher *p);

/*ce \dref_DomainParticipant_create_subscriber
 */
DDSCDllExport DDS_Subscriber*
DDS_DomainParticipant_create_subscriber(
        DDS_DomainParticipant *self,
        const struct DDS_SubscriberQos *qos,
        const struct DDS_SubscriberListener *listener,
        DDS_StatusMask mask);

/*ce \dref_DomainParticipant_delete_subscriber
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_delete_subscriber(
        DDS_DomainParticipant *self,
        DDS_Subscriber * s);

/*ce \dref_DomainParticipant_create_topic
 */
DDSCDllExport DDS_Topic*
DDS_DomainParticipant_create_topic(
        DDS_DomainParticipant * self,
        const char *topic_name,
        const char *type_name,
        const struct DDS_TopicQos *qos,
        const struct DDS_TopicListener *listener,
        DDS_StatusMask mask);

/*ce \dref_DomainParticipant_delete_topic
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_delete_topic(
        DDS_DomainParticipant * self,
        DDS_Topic * topic);

/*ce \dref_DomainParticipant_delete_topic
 */
/* #if INCLUDE_API_LOOKUP */
DDSCDllExport DDS_Topic*
DDS_DomainParticipant_find_topic(DDS_DomainParticipant *self,
                                 const char *topic_name,
                                 const struct DDS_Duration_t *timeout);

/* ================ */
/*  Lookup routine  */
/* ================ */

/*ce \dref_DomainParticipant_lookup_topicdescription
 */
/* #if INCLUDE_API_LOOKUP */
DDSCDllExport DDS_TopicDescription*
DDS_DomainParticipant_lookup_topicdescription(
        DDS_DomainParticipant *self,
        const char *topic_name);


/* ================ */
/*  Misc. routines  */
/* ================ */

/*ci \dref_DomainParticipant_get_domain_id
 */
DDSCDllExport DDS_DomainId_t
DDS_DomainParticipant_get_domain_id(DDS_DomainParticipant *self);

/*ce \dref_DomainParticipant_delete_contained_entities
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_delete_contained_entities(DDS_DomainParticipant *self);

/*ci \dref_DomainParticipant_get_current_time
 */
/* #ifdef INCLUDE_API_UTILITY */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_get_current_time(
        DDS_DomainParticipant * self,
        struct DDS_Time_t *current_time);

struct DDS_ParticipantBuiltinTopicData;

/*ci \dref_DomainParticipant_get_discovered_participants
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_get_discovered_participants(
        DDS_DomainParticipant *self,
        struct DDS_InstanceHandleSeq *participant_handles);

/*ci \dref_DomainParticipant_get_discovered_participant_data
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_get_discovered_participant_data(
        DDS_DomainParticipant * self,
        struct DDS_ParticipantBuiltinTopicData *participant_data,
        const DDS_InstanceHandle_t * participant_handle);

/* ================================================================= */
/*   From DDS_Entity                                                 */
/* ================================================================= */

/* ========================== */
/*  Get and Set QoS routines  */
/* ========================== */

/*ci \dref_DomainParticipant_set_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_set_qos(
        DDS_DomainParticipant *self,
        const struct DDS_DomainParticipantQos *qos);

/*ci \dref_DomainParticipant_get_qos
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_get_qos(
        DDS_DomainParticipant *self,
        struct DDS_DomainParticipantQos *qos);

/*ci \dref_DomainParticipant_get_qos
 */
DDSCDllExport struct DDS_DomainParticipantQos*
DDS_DomainParticipant_get_qos_ref(DDS_DomainParticipant * self);

/* =============================== */
/*  Get and Set Listener routines  */
/* ============================== */

/*ci \dref_DomainParticipant_set_listener
 */
/* #ifdef INCLUDE_API_LISTENER */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_set_listener(
        DDS_DomainParticipant * self,
        const struct DDS_DomainParticipantListener *l,
        DDS_StatusMask mask);

/*ci \dref_DomainParticipant_get_listener
 */
/* #ifdef INCLUDE_API_LISTENER */
DDSCDllExport struct DDS_DomainParticipantListener
DDS_DomainParticipant_get_listener(DDS_DomainParticipant * self);

/*i \dref_DomainParticipant_get_listenerX
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_get_listenerX(
        DDS_DomainParticipant *self,
        struct DDS_DomainParticipantListener *listener);

DDSCDllExport struct OSAPI_System*
DDS_DomainParticipant_get_clock(DDS_DomainParticipant *const self);

DDSCDllExport OSAPI_Timer_t
DDS_DomainParticipant_get_timer(DDS_DomainParticipant *const self);

/*e \dref_DomainParticipant_register_type
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipant_register_type(DDS_DomainParticipant *participant,
                                    const char *type_name,
                                    struct NDDS_Type_Plugin *plugin);

/*e \dref_DomainParticipant_unregister_type
 */
DDSCDllExport struct NDDS_Type_Plugin*
DDS_DomainParticipant_unregister_type(DDS_DomainParticipant *participant,
                                      const char *type_name);

/* ================================================================= */
/*                       Factory                                     */
/* ================================================================= */

/*e \dref_PARTICIPANT_QOS_DEFAULT
 */
extern DDSCDllVariable const struct DDS_DomainParticipantFactoryQos
DDS_PARTICIPANT_FACTORY_QOS_DEFAULT;

/*e \dref_PARTICIPANT_QOS_DEFAULT
 */
extern DDSCDllVariable const struct DDS_DomainParticipantQos
DDS_PARTICIPANT_QOS_DEFAULT;

/*ce \dref_DomainParticipantFactory
 */
typedef struct DDS_DomainParticipantFactoryImpl DDS_DomainParticipantFactory;

/*ce \dref_DomainParticipantFactory_get_instance
 */
DDSCDllExport DDS_DomainParticipantFactory*
DDS_DomainParticipantFactory_get_instance(void);

#define DDS_TheParticipantFactory DDS_DomainParticipantFactory_get_instance()

/*ce \dref_DomainParticipantFactory_finalize_instance
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactory_finalize_instance(void);

/* ================================= */
/*  Set and Get Default QoS routines */
/* ================================= */

/*ce \dref_DomainParticipantFactory_set_default_participant_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactory_set_default_participant_qos(
        DDS_DomainParticipantFactory * self,
         const struct DDS_DomainParticipantQos *qos);

/*ce \dref_DomainParticipantFactory_get_default_participant_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactory_get_default_participant_qos(
        DDS_DomainParticipantFactory * self,
        struct DDS_DomainParticipantQos *qos);

/* =========================== */
/*  Create and Delete routines */
/* =========================== */

/*ce \dref_DomainParticipantFactory_create_participant
 */
DDSCDllExport DDS_DomainParticipant*
DDS_DomainParticipantFactory_create_participant(
        DDS_DomainParticipantFactory * self,
        DDS_DomainId_t domainId,
        const struct DDS_DomainParticipantQos *qos,
        const struct DDS_DomainParticipantListener *listener,
        DDS_StatusMask mask);

/*ce \dref_DomainParticipantFactory_delete_participant
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactory_delete_participant(
        DDS_DomainParticipantFactory * self,
         DDS_DomainParticipant * a_participant);

/* =============== */
/*  Lookup routine */
/* =============== */

/*ci \dref_DomainParticipantFactory_lookup_participant 
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_DomainParticipant*
DDS_DomainParticipantFactory_lookup_participant(
        DDS_DomainParticipantFactory *self,
        DDS_DomainId_t domainId);

/*ci \dref_DomainParticipantFactory_lookup_participant_by_builtin_topic_key
 */
DDSCDllExport DDS_DomainParticipant*
DDS_DomainParticipantFactory_lookup_participant_by_builtin_topic_key(
        DDS_DomainParticipantFactory *self,
        DDS_BuiltinTopicKey_t *key);

/*ci \dref_DomainParticipantFactory_lookup_entity
 */
DDSCDllExport DDS_Entity*
DDS_DomainParticipantFactory_lookup_entity(
        DDS_DomainParticipantFactory *self,
        DDS_BuiltinTopicKey_t *key);

/* ========================= */
/*  Get and Set Qos routines */
/* ========================= */

/*ce \dref_DomainParticipantFactory_get_qos
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactory_get_qos(
        DDS_DomainParticipantFactory *self,
        struct DDS_DomainParticipantFactoryQos *qos);

/*ce \dref_DomainParticipantFactory_set_qos
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DomainParticipantFactory_set_qos(
        DDS_DomainParticipantFactory *self,
        const struct DDS_DomainParticipantFactoryQos *qos);

/*ce \dref_DomainParticipantFactory_get_registry
 */
DDSCDllExport RT_Registry_T*
DDS_DomainParticipantFactory_get_registry(DDS_DomainParticipantFactory *self);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_domain_h */
