/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_route_h
#include "netio/netio_route.h"
#endif
#ifndef netio_loopback_h
#include "netio/netio_loopback.h"
#endif
#ifndef netio_udp_h
#include "netio/netio_udp.h"
#endif
#ifndef netio_rtps_h
#include "netio/netio_rtps.h"
#endif
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif
#ifndef reda_string_h
#include "reda/reda_string.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "Entity.pkg.h"
#include "QosPolicy.pkg.h"
#include "TopicDescription.pkg.h"
#include "TopicQos.pkg.h"
#include "Topic.pkg.h"
#include "Type.pkg.h"
#include "RtpsWellKnownPorts.pkg.h"
#include "DomainParticipantQos.pkg.h"
#include "DomainParticipantEvent.pkg.h"
#include "DataReaderImpl.pkg.h"
#include "DataReader.pkg.h"
#include "SubscriberQos.pkg.h"
#include "SubscriberImpl.pkg.h"
#include "DataWriterImpl.pkg.h"
#include "PublisherQos.pkg.h"
#include "PublisherImpl.pkg.h"
#include "RemoteEntity.pkg.h"
#include "RemoteEndpoint.pkg.h"
#include "RemoteParticipant.pkg.h"
#include "RemotePublication.pkg.h"
#include "RemoteSubscription.pkg.h"

struct NDDS_ParticipantConfig
{
    RT_Registry_T *registry;
    struct DDS_GUID_t guid;
    DB_Database_T db;
    DDS_Long instance;
};

/*i \dref_Publisher_Properties_INITIALIZER
 */

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

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DDS_DomainParticipant_db_init(struct DDS_DomainParticipantImpl *participant)
{
#define MAX_TABLE_LENGTH 16
    DB_ReturnCode_T dbrc;
    struct DB_TableProperty tbl_prop = DB_TableProperty_INITIALIZER;
    struct DB_DatabaseProperty db_property = DB_DatabaseProperty_INITIALIZER;
    DDS_ReturnCode_t ddsrc = DDS_RETCODE_ERROR;
    char tbl_name[MAX_TABLE_LENGTH];

    db_property.max_tables = 200;
    db_property.lock_mode = DB_LOCK_LEVEL_SHARED;
    participant->database = NULL;
    dbrc = DB_Database_create(&participant->database,"domain",
            &db_property,participant->db_lock);
    if (dbrc != DB_RETCODE_OK)
    {
        DDSC_Log_error_p1(LOG_FAIL_CREATE_DATABASE, dbrc);
        ddsrc = DDS_RETCODE_ERROR;
        goto done;
    }

    /*
     * Local entities
     */
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"to%d",participant->config.instance);
    tbl_prop.max_records =
               participant->qos.resource_limits.local_topic_allocation;
    dbrc = DB_Database_create_table(
            &participant->topic_table,participant->database,
            tbl_name,sizeof(struct DDS_TopicImpl),
            NULL,DDS_TopicImpl_compare,
            &tbl_prop);

    tbl_prop.max_records =
               participant->qos.resource_limits.local_type_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"ty%d",participant->config.instance);
    dbrc = DB_Database_create_table(
            &participant->type_table,participant->database,
            tbl_name,sizeof(struct DDS_TypeImpl),
            NULL,DDS_TypeImpl_compare,
            &tbl_prop);

    tbl_prop.max_records =
               participant->qos.resource_limits.local_publisher_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"p%d",participant->config.instance);
    dbrc = DB_Database_create_table(
            &participant->local_publisher_table,participant->database,
            tbl_name,sizeof(struct DDS_PublisherImpl),
            NULL,DDS_PublisherImpl_compare,
            &tbl_prop);

    tbl_prop.max_records =
               participant->qos.resource_limits.local_subscriber_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"s%d",participant->config.instance);
    dbrc = DB_Database_create_table(
            &participant->local_subscriber_table,participant->database,
            tbl_name,sizeof(struct DDS_SubscriberImpl),
            NULL,DDS_SubscriberImpl_compare,
            &tbl_prop);

    tbl_prop.max_records =
               participant->qos.resource_limits.local_reader_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"dr%d",participant->config.instance);
    dbrc = DB_Database_create_table(
            &participant->local_reader_table,participant->database,
            tbl_name,sizeof(struct DDS_DataReaderImpl),
            NULL,DDS_DataReaderImpl_compare,
            &tbl_prop);

    tbl_prop.max_records =
               participant->qos.resource_limits.local_writer_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"dw%d",participant->config.instance);
    dbrc = DB_Database_create_table(
            &participant->local_writer_table,participant->database,
            tbl_name,sizeof(struct DDS_DataWriterImpl),
            NULL,DDS_DataWriterImpl_compare,
            &tbl_prop);

    /*
     * Remote entities
     */
    tbl_prop.max_records =
               participant->qos.resource_limits.remote_participant_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"rem_p%d",participant->config.instance);
    dbrc = DB_Database_create_table(
            &participant->remote_participant_table,participant->database,
            tbl_name,sizeof(struct DDS_RemoteParticipantImpl),
            NULL,DDS_RemoteParticipantImpl_compare,
            &tbl_prop);

    tbl_prop.max_records =
               participant->qos.resource_limits.remote_reader_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"rem_s%d",participant->config.instance);
    dbrc = DB_Database_create_table(
            &participant->remote_subscriber_table,participant->database,
            tbl_name,sizeof(struct DDS_RemoteSubscriptionImpl),
            NULL,DDS_RemoteSubscriptionImpl_compare,
            &tbl_prop);

    tbl_prop.max_records =
               participant->qos.resource_limits.remote_writer_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"rem_dp%d",participant->config.instance);
    dbrc = DB_Database_create_table(
            &participant->remote_publisher_table,participant->database,
            tbl_name,sizeof(struct DDS_RemotePublicationImpl),
            NULL,DDS_RemotePublicationImpl_compare,
            &tbl_prop);

    ddsrc = DDS_RETCODE_OK;

done:
    return ddsrc;
}

struct DDS_BuiltinTopicKey_t
NDDS_DomainParticipant_get_key(const DDS_DomainParticipant *const self)
{
    struct DDS_BuiltinTopicKey_t key;
    DDS_InstanceHandle_t ih;

    ih = DDS_Entity_get_instance_handle(DDS_DomainParticipant_as_entity(self));
    OSAPI_Memory_copy(&key,ih.octet, 16);
#ifdef RTI_ENDIAN_LITTLE
     key.value[0] = NETIO_ntohl(key.value[0]);
     key.value[1] = NETIO_ntohl(key.value[1]);
     key.value[2] = NETIO_ntohl(key.value[2]);
     key.value[3] = NETIO_ntohl(key.value[3]);
#endif
    return key;
}

DDS_ReturnCode_t
DDS_DomainParticipant_initialize_builtin_data(
                struct DDS_DomainParticipantImpl *self,
                struct DDS_ParticipantBuiltinTopicData *data)
{
    DDS_ProtocolVersion_t protocol_version = DDS_PROTOCOLVERSION;

    data->key = NDDS_DomainParticipant_get_key(self);

    OSAPI_Stdio_snprintf(data->participant_name.name,
                        DDS_ENTITYNAME_QOS_NAME_MAX,
                        self->qos.participant_name.name);

    data->rtps_protocol_version = protocol_version;
    data->rtps_vendor_id.vendorId[0] = 0x01;
    /*data->rtps_vendor_id.vendorId[1] = 0x01;*/
    data->rtps_vendor_id.vendorId[1] = 0x0a;

    DDS_LocatorSeq_initialize(&data->metatraffic_unicast_locators);
    DDS_LocatorSeq_initialize(&data->metatraffic_multicast_locators);
    DDS_LocatorSeq_initialize(&data->default_unicast_locators);
    DDS_LocatorSeq_initialize(&data->default_multicast_locators);

    DDS_LocatorSeq_set_maximum(&data->metatraffic_multicast_locators,4);
    DDS_LocatorSeq_set_maximum(&data->metatraffic_unicast_locators,4);
    DDS_LocatorSeq_set_maximum(&data->default_multicast_locators,4);
    DDS_LocatorSeq_set_maximum(&data->default_unicast_locators,4);

    data->dds_builtin_endpoints = 0;

    data->product_version.major = 2;
    data->product_version.minor = 0;
    data->product_version.revision = 'c';
    data->product_version.release = 00;

    return DDS_RETCODE_OK;
}

DDS_InstanceHandle_t
DDS_DomainParticipantImpl_get_instance_handle(DDS_Entity *entity)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl*)entity;
    DDS_InstanceHandle_t retval;

    OSAPI_Memory_copy(retval.octet,&participant->config.guid,16);

    return retval;
}

DDS_ReturnCode_t
DDS_DomainParticipant_enable(DDS_Entity * self)
{
    DB_ReturnCode_T dbrc;
    struct DDS_DomainParticipantImpl *participant = NULL;
    struct DDS_PublisherImpl *local_publisher;
    struct DDS_SubscriberImpl *local_subscriber;
    DB_Cursor_T cursor = NULL;

    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    participant = (struct DDS_DomainParticipantImpl*)self;

    OSAPI_Mutex_take(participant->shared_lock);

    if (self->state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        OSAPI_Mutex_give(participant->shared_lock);
        return DDS_RETCODE_OK;
    }

    DDSC_LOG_DEBUG_P1(LOG_ENABLE_PARTICIPANT, participant->as_entity.entity_id);
    DDSC_LOG_PRINT_DEBUG("enable participant: %ld\n", participant->as_entity.entity_id);

    self->state = RTIDDS_ENTITY_STATE_ENABLED;

    /* Let the discovery plugin start so that locally created
     * endpoints gets announced
     */
    if (participant->disc_plugin != NULL)
    {
        NDDS_Discovery_Plugin_on_after_local_participant_enabled(
                participant->disc_plugin,
                participant,
                &participant->builtin_data);
    }

    cursor = NULL;
    dbrc = DB_Table_select_all_default(participant->local_subscriber_table,
                                      &cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor, (DB_Record_T*)&local_subscriber);
        if ((dbrc == DB_RETCODE_OK) && !DDS_SubscriberImpl_is_hidden(local_subscriber))
        {
            local_subscriber->as_entity.enable(&local_subscriber->as_entity);
        }
    } while (dbrc == DB_RETCODE_OK);

    DB_Cursor_finish(participant->local_subscriber_table,cursor);

    cursor = NULL;
    dbrc = DB_Table_select_all_default(participant->local_publisher_table,
                                      &cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor, (DB_Record_T*)&local_publisher);
        if ((dbrc == DB_RETCODE_OK) && !DDS_PublisherImpl_is_hidden(local_publisher))
        {
            local_publisher->as_entity.enable(&local_publisher->as_entity);
        }
    } while (dbrc == DB_RETCODE_OK);

    DB_Cursor_finish(participant->local_publisher_table,cursor);

    OSAPI_Mutex_give(participant->shared_lock);

    return DDS_RETCODE_OK;
}

#if 0
static void
print_locator(struct DDS_LocatorSeq *loc_seq)
{
    int i;
    for (i = 0; i < DDS_LocatorSeq_get_length(loc_seq); i++)
    {
        OSAPI_Stdio_printf("Address %d: %d/%d/%x\n",i,
                DDS_LocatorSeq_get_reference(loc_seq,i)->kind,
                DDS_LocatorSeq_get_reference(loc_seq,i)->port,
                *(RTI_INT32*)&DDS_LocatorSeq_get_reference(loc_seq,i)->address[0]);
    }
}
#endif

DDS_ReturnCode_t
DDS_DomainParticipant_finalize(DDS_DomainParticipant *self)
{
    struct DDS_DomainParticipantImpl *participant =
                                        (struct DDS_DomainParticipantImpl*)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
   DDS_BuiltinTopicKey_t key;
   DDS_UnsignedLong count;
   DB_ReturnCode_T dbrc = DB_RETCODE_ERROR;
   struct RT_ComponentFactory *c_factory;
   DB_Cursor_T cursor = NULL;
   struct DDS_RemoteParticipantImpl *rem_participant;
   struct DDS_RemotePublicationImpl *rem_pub;
   struct DDS_RemoteSubscriptionImpl *rem_sub;
   DDS_Long i,j;
   struct NETIO_Address src_address;
   NETIO_Interface_T *netio_intf;
   struct NETIO_AddressSeq routes = NETIO_AddressSeq_INITIALIZER;
   struct NETIO_NetmaskSeq netmasks = NETIO_NetmaskSeq_INITIALIZER;
   struct NETIO_RouteResolverProperty rte_prop = NETIO_RouteResolverProperty_INITIALIZER;

   if (participant->database != NULL)
   {
       DB_Database_lock(participant->database);
   }

   key = NDDS_DomainParticipant_get_key(self);

   if (participant->remote_participant_table)
   {
       cursor = NULL;
       dbrc = DB_Table_select_all_default(
                              participant->remote_participant_table,&cursor);
       do
       {
           dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&rem_participant);
           if (dbrc == DB_RETCODE_OK)
           {
               OSAPI_Timer_delete_timeout(rem_participant->timer,
                                   &rem_participant->lease_duration_event);
           }
       } while (dbrc == DB_RETCODE_OK);
       DB_Cursor_finish(participant->remote_participant_table,cursor);
   }

   if (self->disc_plugin != NULL)
   {
       retcode = NDDS_Discovery_Plugin_on_before_local_participant_deleted(
                   self->disc_plugin,
                   self,
                   &key);
   }

   /* Deleting a DP follows the following rules
    * 1. All local publisher must be deleted
    * 2. All local subscribers must be deleted
    * 3. The built-in readers must be deleted
    * 4. All threads (Sender/Receiver) must be deleted
    * 5. The Event thread must be deleted
    * 6. Generator and Interpreter must be deleted
    * 7. The OSAPI must be deleted
    */
   if (participant->local_publisher_table)
   {
       cursor = NULL;
       dbrc = DB_Table_select_all_default(
                              participant->local_publisher_table,&cursor);
       if (dbrc != DDS_RETCODE_OK)
       {
           goto done;
       }
       count = DB_Cursor_get_count(cursor);
       if (count > 0)
       {
           DDSC_Log_error_p1(LOG_PARTICIPANT_OWNS_PUBLISHERS, count);
           retcode = DDS_RETCODE_ENTITY_NOT_EMPTY;
           DB_Cursor_finish(participant->local_publisher_table,cursor);
           goto done;
       }
       DB_Cursor_finish(participant->local_publisher_table,cursor);
   }

   if (participant->local_subscriber_table)
   {
       cursor = NULL;
       dbrc = DB_Table_select_all_default(
                              participant->local_subscriber_table,&cursor);
       if (dbrc != DDS_RETCODE_OK)
       {
           goto done;
       }
       count = DB_Cursor_get_count(cursor);
       if (count > 0)
       {
           DDSC_Log_error_p1(LOG_PARTICIPANT_OWNS_SUBSCRIBERS, count);
           retcode = DDS_RETCODE_ENTITY_NOT_EMPTY;
           DB_Cursor_finish(participant->local_subscriber_table,cursor);
           goto done;
       }
       DB_Cursor_finish(participant->local_subscriber_table,cursor);
   }

#define delete_table(t_) \
if (participant->t_) \
{\
   dbrc = DB_Database_delete_table(participant->database,participant->t_);\
   if (dbrc != DB_RETCODE_OK) \
   {\
       retcode = DDS_RETCODE_ERROR;\
       goto done;\
   }\
}

   if (participant->remote_publisher_table)
   {
       cursor = NULL;
       dbrc = DB_Table_select_all_default(
                              participant->remote_publisher_table,&cursor);
       do
       {
           dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&rem_pub);
           if (dbrc == DB_RETCODE_OK)
           {
               DDS_PublicationBuiltinTopicData_finalize(&rem_pub->data);
               DB_Table_delete_record(
                       participant->remote_publisher_table,rem_pub);
           }
       } while (dbrc == DB_RETCODE_OK);
       DB_Cursor_finish(participant->remote_publisher_table,cursor);

   }

   if (participant->remote_subscriber_table)
   {
       cursor = NULL;
       dbrc = DB_Table_select_all_default(
                              participant->remote_subscriber_table,&cursor);
       do
       {
           dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&rem_sub);
           if (dbrc == DB_RETCODE_OK)
           {
               DDS_SubscriptionBuiltinTopicData_finalize(&rem_sub->data);
               DB_Table_delete_record(
                       participant->remote_subscriber_table,rem_sub);
           }
       } while (dbrc == DB_RETCODE_OK);
       DB_Cursor_finish(participant->remote_subscriber_table,cursor);
   }

   if (participant->remote_participant_table)
   {
       cursor = NULL;
       dbrc = DB_Table_select_all_default(
                              participant->remote_participant_table,&cursor);
       do
       {
           dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&rem_participant);
           if (dbrc == DB_RETCODE_OK)
           {
               NDDS_RemoteParticipantRecord_finalize(rem_participant,NULL);
               DB_Table_delete_record(
                       participant->remote_participant_table,rem_participant);
           }
       } while (dbrc == DB_RETCODE_OK);
       DB_Cursor_finish(participant->remote_participant_table,cursor);
   }

   /* All of our resources */
   if (participant->database)
   {
       delete_table(local_reader_table);
       delete_table(local_writer_table);
       delete_table(local_publisher_table);
       delete_table(local_subscriber_table);
       delete_table(remote_participant_table);
       delete_table(remote_publisher_table);
       delete_table(remote_subscriber_table);
       delete_table(topic_table);
       delete_table(type_table);
   }

   if (participant->finalize_lock)
   {
       OSAPI_Mutex_take(participant->finalize_lock);
   }

   /* It is safe to unlock the data base here. There are no shared resources
    * between the DDS and the transports, except the database. Since the
    * database is shared, avoid a deadlock as DDS sends messages to itself.
    */
   DB_Database_unlock(participant->database);

   NETIO_BindResolver_release_addresses(
           participant->bind_resolver,(struct REDA_StringSeq*)&participant->qos.transports.enabled_transports,NETIO_ROUTEKIND_META,
           (struct NETIO_AddressSeq*)&participant->builtin_data.metatraffic_multicast_locators);

   NETIO_BindResolver_release_addresses(
           participant->bind_resolver,(struct REDA_StringSeq*)&participant->qos.transports.enabled_transports,NETIO_ROUTEKIND_META,
           (struct NETIO_AddressSeq*)&participant->builtin_data.metatraffic_unicast_locators);

   NETIO_BindResolver_release_addresses(
           participant->bind_resolver,(struct REDA_StringSeq*)&participant->qos.transports.enabled_transports,NETIO_ROUTEKIND_USER,
           (struct NETIO_AddressSeq*)&participant->builtin_data.default_multicast_locators);

   NETIO_BindResolver_release_addresses(
           participant->bind_resolver,(struct REDA_StringSeq*)&participant->qos.transports.enabled_transports,NETIO_ROUTEKIND_USER,
           (struct NETIO_AddressSeq*)&participant->builtin_data.default_unicast_locators);

   DB_Database_lock(participant->database);

   NETIO_Address_init(&src_address,0);
   rte_prop.max_routes = participant->qos.resource_limits.max_destination_ports;
   NETIO_AddressSeq_set_maximum(&routes,rte_prop.max_routes);
   NETIO_AddressSeq_set_length(&routes,0);
   NETIO_NetmaskSeq_set_maximum(&netmasks,rte_prop.max_routes);
   NETIO_NetmaskSeq_set_length(&netmasks,0);

   for (i = 0; i < DDS_StringSeq_get_length(&participant->qos.transports.enabled_transports); ++i)
   {
       RT_ComponentFactoryId_T id;
       RT_ComponentFactoryId_clear(&id);
       RT_ComponentFactoryId_set_name(&id,*DDS_StringSeq_get_reference(&participant->qos.transports.enabled_transports,i));

       if (!NETIO_BindResolver_lookup_by_address(participant->bind_resolver,NETIO_ROUTEKIND_META,&id,&src_address,&netio_intf))
       {
           goto done;
       }

       if (!NETIO_BindResolver_delete_route(participant->bind_resolver,
                                   NETIO_ROUTEKIND_META,&id,&src_address,NULL))
       {
           goto done;
       }

       if (!NETIO_BindResolver_lookup_by_address(participant->bind_resolver,NETIO_ROUTEKIND_USER,&id,&src_address,&netio_intf))
       {
           goto done;
       }

       if (!NETIO_BindResolver_delete_route(participant->bind_resolver,
                                   NETIO_ROUTEKIND_USER,&id,&src_address,NULL))
       {
           goto done;
       }

       NETIO_AddressSeq_set_length(&routes,0);
       NETIO_NetmaskSeq_set_length(&netmasks,0);

       if (!NETIO_Interface_get_route_table(netio_intf,&routes,&netmasks))
       {
           goto done;
       }

       for (j = 0; j < NETIO_AddressSeq_get_length(&routes); ++j)
       {
           if (!NETIO_RouteResolver_delete_interface(participant->route_resolver,
                   netio_intf,
                   NETIO_AddressSeq_get_reference(&routes,j),
                   NETIO_NetmaskSeq_get_reference(&netmasks,j),
                   NULL))
           {
               goto done;
           }
       }

       c_factory = RT_Registry_lookup(participant->config.registry,
               *DDS_StringSeq_get_reference(
                          &participant->qos.transports.enabled_transports,i));
       if (c_factory == NULL)
       {
           goto done;
       }

       NETIO_InterfaceFactory_delete_component(c_factory,netio_intf);
   }

   if (participant->rtps_intf != NULL)
   {
       c_factory = RT_Registry_lookup(participant->config.registry,"rtps");
       if (c_factory != NULL)
       {
           NETIO_InterfaceFactory_delete_component(c_factory,
                                                   participant->rtps_intf);
       }
       participant->rtps_intf = NULL;
   }

   NETIO_AddressSeq_finalize(&routes);
   NETIO_NetmaskSeq_finalize(&netmasks);

   if (participant->bind_resolver != NULL)
   {
       if (!NETIO_BindResolver_delete(participant->bind_resolver))
       {
           goto done;
       }
       participant->bind_resolver = NULL;
   }

   if (participant->route_resolver != NULL)
   {
       if (!NETIO_RouteResolver_delete(participant->route_resolver))
       {
           goto done;
       }
       participant->route_resolver = NULL;
   }

   if (self->disc_plugin != NULL)
   {
       if (self->qos.discovery.discovery.name[0] != 0)
       {
           c_factory = RT_Registry_lookup(
                                        self->config.registry,
                                        self->qos.discovery.discovery.name);
           if (c_factory == NULL)
           {
               DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
               goto done;
           }
           DiscoveryComponentFactory_delete_component(c_factory,
                                                      self->disc_plugin);
           self->disc_plugin = NULL;
       }
   }

    DDS_DomainParticipantQos_finalize(&self->qos);
    DDS_PublisherQos_finalize(&self->_default_publisher_qos);
    DDS_SubscriberQos_finalize(&self->_default_subscriber_qos);
    DDS_ParticipantBuiltinTopicData_finalize(&self->builtin_data);

    if (participant->database != NULL)
    {
        DB_Database_unlock(participant->database);
        dbrc = DB_Database_delete(participant->database);
        if (dbrc != DB_RETCODE_OK)
        {
            DDSC_Log_error(LOG_FAIL_DELETE_DATABASE);
            retcode = DDS_RETCODE_ENTITY_NOT_EMPTY;
            goto done;
        }
        participant->database  = NULL;

        /* NOTE: If the db_lock is the shared lock, there is race condition where
            * the timer thread and the shared lock could end in a deadlock between
            * the system mutex and db_lock
            */
         if (self->timer)
         {
             OSAPI_Timer_delete(self->timer);
         }
        OSAPI_Mutex_delete(self->db_lock);
    }

    if (participant->finalize_lock)
    {
        OSAPI_Mutex_give(participant->finalize_lock);
        OSAPI_Mutex_delete(participant->finalize_lock);
    }

    DDS_DomainParticipantQos_finalize(&participant->qos);

    retcode = DDS_RETCODE_OK;

done:

	if ((retcode != DDS_RETCODE_OK) && (participant->database != NULL))
	{
	    DB_Database_unlock(participant->database);
	}

   return retcode;

#undef delete_table
}


DDS_DomainParticipant*
DDS_DomainParticipant_initialize(
                          struct DDS_DomainParticipantImpl *participant,
                          DDS_DomainParticipantFactory *factory,
                          DDS_DomainId_t domain_id,
                          const struct DDS_DomainParticipantQos *qos,
                          const struct DDS_DomainParticipantListener *listener,
                          DDS_StatusMask mask,
                          struct NDDS_ParticipantConfig *config,
                          struct OSAPI_Mutex *factory_lock)
{
    DDS_Boolean success = DDS_BOOLEAN_FALSE;
    DDS_ReturnCode_t rc;
    DDS_Long i,j;
#ifndef INCLUDE_API_QOS
    struct DDS_PublisherQos PUB_QOS_INIT = DDS_PublisherQos_INITIALIZER;
    struct DDS_SubscriberQos SUB_QOS_INIT = DDS_SubscriberQos_INITIALIZER;
#endif
    struct OSAPI_TimerProperty timer_property = OSAPI_TimerProperty_INITIALIZER;
    struct RT_ComponentFactory *c_factory;
    struct NDDS_Discovery_Property disc_property =
                                        NDDS_Discovery_Property_INITIALIZER;
    struct NDDS_Discovery_Listener disc_listener =
                                        NDDS_Discovery_Listener_INITIALIZE;
    struct LOOPInterfaceProperty lo_property =
                                        LOOPInterfaceProperty_INITIALIZER;
    struct UDP_InterfaceProperty udp_property = UDP_InterfaceProperty_INITIALIZER;
    struct NETIO_AddressSeq reserved_address = NETIO_AddressSeq_INITIALIZER;
    struct NETIO_BindResolverProperty rc_prop = NETIO_BindResolverProperty_INITIALIZER;
    struct NETIO_RouteResolverProperty rte_prop = NETIO_RouteResolverProperty_INITIALIZER;
    char tbl_name[MAX_TABLE_LENGTH];
    NETIO_Interface_T *netio_intf;
    struct NETIO_Address src_address;
    RTI_INT32 try_id,max_id;
    RTI_BOOL done = RTI_FALSE;
    struct NETIO_AddressSeq routes = NETIO_AddressSeq_INITIALIZER;
    struct NETIO_NetmaskSeq netmasks = NETIO_NetmaskSeq_INITIALIZER;
    struct RTPSInterfaceProperty rtps_property = RTPSInterfaceProperty_INITIALIZER;

    OSAPI_Memory_zero(participant,sizeof(struct DDS_DomainParticipantImpl));

    if (factory == NULL || qos == NULL)
    {
        goto done;
    }

    /* Save information which can be retrieved later. Need to validate it */
    if ((qos != &DDS_PARTICIPANT_QOS_DEFAULT) &&
        !DDS_DomainParticipantQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_PARTICIPANT_QOS);
        goto done;
    }

    if ((listener == NULL) && (mask != DDS_STATUS_MASK_NONE))
    {
        DDSC_Log_error(LOG_INCONSISTENT_LISTENER_AND_STATUS_MASK);
        goto done;
    }

    participant->finalize_lock = OSAPI_Mutex_new();
    if (participant->finalize_lock == NULL)
    {
        goto done;
    }

    participant->config = *config;
    participant->domain_id = domain_id;
    *((RTI_INT32*)&participant->config.guid.value[12])=NETIO_htonl(MIG_RTPS_OBJECT_ID_PARTICIPANT);

    DDS_EntityImpl_initialize(&participant->as_entity,
                              DDS_PARTICIPANT_ENTITY_KIND,
                              MIG_RTPS_OBJECT_ID_PARTICIPANT,
                              DDS_DomainParticipant_enable,
                              DDS_DomainParticipantImpl_get_instance_handle);

    participant->db_lock = OSAPI_Mutex_new();
    participant->database = NULL;
    participant->qos = DDS_PARTICIPANT_QOS_DEFAULT;
    participant->shared_lock = factory_lock;

    if (qos == &DDS_PARTICIPANT_QOS_DEFAULT)
    {
#if INCLUDE_API_QOS
        DDS_DomainParticipantFactory_get_default_participant_qos(
                                                    factory,&participant->qos);
#endif
    }
    else
    {
        if (DDS_DomainParticipantQos_copy(&participant->qos,qos) != DDS_RETCODE_OK)
        {
            goto done;
        }
    }

    /* NOTE:
     * Need to copy the RTPS parameters so that NETIO can use the info
     * to calculate RTPS ports to use. Although it does not waste a lot of
     * memory, it does it not a good thing.
     */
    participant->rtps_port_data.port_param = participant->qos.protocol.rtps_well_known_ports;
    participant->rtps_port_data.domain_id = participant->domain_id;

#ifdef INCLUDE_API_LISTENER
    rc = DDS_DomainParticipant_set_listener(participant, listener, mask);
    if (rc != DDS_RETCODE_OK)
    {
        DDSC_Log_error_p1(LOG_FAIL_SET_LISTENER, rc);
        goto done;
    }
#endif

#if INCLUDE_API_QOS
    DDS_PublisherQos_initialize(&participant->_default_publisher_qos);
    DDS_SubscriberQos_initialize(&participant->_default_subscriber_qos);
#else
    participant->_default_publisher_qos = PUB_QOS_INIT;
    participant->_default_subscriber_qos = SUB_QOS_INIT;
#endif

    /* Only one discovery plug-in is allowed, and it is okay with none
     */
    participant->disc_plugin = NULL;
    if (participant->qos.discovery.discovery.name[0] != 0)
    {
        c_factory = RT_Registry_lookup(participant->config.registry,
                                       participant->qos.discovery.discovery.name);
        if (c_factory == NULL)
        {
            goto done;
        }

        participant->disc_plugin = DiscoveryComponentFactory_create_component(
                c_factory,&disc_property._parent,&disc_listener._parent);
        if (participant->disc_plugin == NULL)
        {
            goto done;
        }
    }

    /* Initialize participant announcement data
     * The participant announcement relies on data collected from
     * the discovery plug-in(s)
     */
    DDS_ParticipantBuiltinTopicData_initialize(&participant->builtin_data);
    DDS_DomainParticipant_initialize_builtin_data(
                                    participant,&participant->builtin_data);

    /* Call the discovery plug-in before initialization, so the plug-in
     * can make any necessary changes to the plugin's QoS policies
     * (So the plug-in can allow for more resources, for example)
     * A discovery plug-in may alter the built-in topic-data, typically the
     * discovery locators are updated.
     */
    if (participant->disc_plugin != NULL)
    {
        NDDS_Discovery_Plugin_on_before_local_participant_created(
                                                participant->disc_plugin,
                                                participant,
                                                &participant->qos,
                                                &participant->builtin_data);
    }

    timer_property.max_entries = participant->qos.event.max_count;
    timer_property.max_slots = 128;

	participant->timer = OSAPI_Timer_new(&timer_property,participant->db_lock);
	if (participant->timer == NULL)
    {
        DDSC_Log_error(LOG_FAIL_CREATE_EVENT_GENERATOR);
        goto done;
    }

    /* Initialize the database */
    if (DDS_DomainParticipant_db_init(participant) != DDS_RETCODE_OK)
    {
        goto done;
    }

    /*
     * Resource table
     */
    rc_prop.max_routes = participant->qos.resource_limits.max_receive_ports +
            participant->qos.resource_limits.local_writer_allocation;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"rxrc%d",participant->config.instance);
    participant->bind_resolver = NETIO_BindResolver_new(participant->database,tbl_name,&rc_prop);
    if (participant->bind_resolver == NULL)
    {
        goto done;
    }

    rc_prop.max_routes = participant->qos.resource_limits.max_destination_ports;
    OSAPI_Stdio_snprintf(tbl_name,MAX_TABLE_LENGTH,"rte%d",participant->config.instance);
    participant->route_resolver = NETIO_RouteResolver_new(participant->database,tbl_name,&rte_prop);
    if (participant->route_resolver == NULL)
    {
        goto done;
    }

    /* Add the default transports, unless the user has already specified some
     */
    if (DDS_StringSeq_get_length(&participant->qos.transports.enabled_transports) == 0)
    {
        DDS_StringSeq_set_maximum(&participant->qos.transports.enabled_transports,2);
        DDS_StringSeq_set_length(&participant->qos.transports.enabled_transports,2);
        *DDS_StringSeq_get_reference(&participant->qos.transports.enabled_transports,0) =
                DDS_String_dup("_udp");
        *DDS_StringSeq_get_reference(&participant->qos.transports.enabled_transports,1) =
                DDS_String_dup("_intra");
    }

    /* Register each transport, create a component for address resolution
     */
    NETIO_Address_init(&src_address,0);
    NETIO_AddressSeq_set_maximum(&routes,rte_prop.max_routes);
    NETIO_AddressSeq_set_length(&routes,0);
    NETIO_NetmaskSeq_set_maximum(&netmasks,rte_prop.max_routes);
    NETIO_NetmaskSeq_set_length(&netmasks,0);

    for (i = 0; i < DDS_StringSeq_get_length(&participant->qos.transports.enabled_transports); ++i)
    {
        RT_ComponentFactoryId_T id;
        RT_ComponentFactoryId_clear(&id);
        RT_ComponentFactoryId_set_name(&id,*DDS_StringSeq_get_reference(&participant->qos.transports.enabled_transports,i));

        c_factory = RT_Registry_lookup(participant->config.registry,
                *DDS_StringSeq_get_reference(
                           &participant->qos.transports.enabled_transports,i));
        if (c_factory == NULL)
        {
            goto done;
        }

        netio_intf = NULL;
        if (RT_INTERFACE_INSTANCE(RT_ComponentFactory_get_id(c_factory)) == RT_COMPONENT_INSTANCE_INTRA)
        {
            lo_property._parent._parent.db = participant->database;
            lo_property._parent.max_routes = participant->qos.resource_limits.local_reader_allocation;
            lo_property._parent.max_binds = participant->qos.resource_limits.local_reader_allocation;
            netio_intf = NETIO_InterfaceFactory_create_component(c_factory,&lo_property._parent._parent,NULL);
        }
        else if (RT_INTERFACE_INSTANCE(RT_ComponentFactory_get_id(c_factory)) == RT_COMPONENT_INSTANCE_UDP)
        {
            udp_property._parent._parent.db = participant->database;
            udp_property._parent.max_routes = participant->qos.resource_limits.max_destination_ports;
            udp_property._parent.max_binds = participant->qos.resource_limits.max_receive_ports;
            netio_intf = NETIO_InterfaceFactory_create_component(c_factory,&udp_property._parent._parent,NULL);
        }
        else
        {
            /* TODO: Add suitable warning about unknown transport */
            continue;
        }

        if (netio_intf == NULL)
        {
            /* TODO: Add suitable warning about failure to create interface */
            continue;
        }

        if (!NETIO_BindResolver_add_route(participant->bind_resolver,
                NETIO_ROUTEKIND_META,&id,&src_address,netio_intf,
                NETIO_rtps_calculate_meta_port,&participant->rtps_port_data,NULL))
        {
            goto done;
        }

        if (!NETIO_BindResolver_add_route(participant->bind_resolver,
                NETIO_ROUTEKIND_USER,&id,&src_address,netio_intf,
                NETIO_rtps_calculate_user_port,&participant->rtps_port_data,NULL))
        {
            goto done;
        }

        NETIO_AddressSeq_set_length(&routes,0);
        NETIO_NetmaskSeq_set_length(&netmasks,0);

        if (!NETIO_Interface_get_route_table(netio_intf,&routes,&netmasks))
        {
            goto done;
        }

        for (j = 0; j < NETIO_AddressSeq_get_length(&routes); ++j)
        {
            if (!NETIO_RouteResolver_add_interface(participant->route_resolver,
                    netio_intf,
                    NETIO_AddressSeq_get_reference(&routes,j),
                    NETIO_NetmaskSeq_get_reference(&netmasks,j),
                    NULL))
            {
                goto done;
            }
        }
    }

    NETIO_AddressSeq_finalize(&routes);
    NETIO_NetmaskSeq_finalize(&netmasks);

    /* Add the default discovery transports unless the user has specified a list */
    if (DDS_StringSeq_get_length(&participant->qos.discovery.enabled_transports) == 0)
    {
        DDS_StringSeq_set_maximum(&participant->qos.discovery.enabled_transports,3);
        DDS_StringSeq_set_length(&participant->qos.discovery.enabled_transports,3);
        *DDS_StringSeq_get_reference(&participant->qos.discovery.enabled_transports,0) =
                DDS_String_dup("_udp://239.255.0.1");
        *DDS_StringSeq_get_reference(&participant->qos.discovery.enabled_transports,1) =
                DDS_String_dup("_udp://");
        *DDS_StringSeq_get_reference(&participant->qos.discovery.enabled_transports,2) =
                DDS_String_dup("_udp://127.0.0.1");
    }

    /* Add the default user-traffic transports unless the user has specified a list */
    if (DDS_StringSeq_get_length(&participant->qos.user_traffic.enabled_transports) == 0)
    {
        DDS_StringSeq_set_maximum(&participant->qos.user_traffic.enabled_transports,3);
        DDS_StringSeq_set_length(&participant->qos.user_traffic.enabled_transports,3);
        *DDS_StringSeq_get_reference(&participant->qos.user_traffic.enabled_transports,0) =
                DDS_String_dup("_udp://");
        *DDS_StringSeq_get_reference(&participant->qos.user_traffic.enabled_transports,1) =
                DDS_String_dup("_intra://");
        *DDS_StringSeq_get_reference(&participant->qos.user_traffic.enabled_transports,2) =
                DDS_String_dup("_udp://127.0.0.1");
    }

    /* Resolve addresses to listen to
     * 1. Create a list of candidate addresses based on the enabled transport list
     * 2. Determine the participant_id range
     * 3. For each port, try to reserve each address in the candidate list of
     *    transports
     * 4. If the reservation succeeds, add it to the rx_table
     * 5. If all candidate succeeds in reserving the address, done
     */
    NETIO_AddressSeq_set_maximum(&reserved_address,4);
    NETIO_AddressSeq_set_length(&reserved_address,0);

    NETIO_Address_init(&src_address,NETIO_ADDRESS_KIND_UDPv4);

    if (participant->qos.protocol.participant_id >= 0)
    {
        participant->participant_id = participant->qos.protocol.participant_id;
        try_id = participant->participant_id;
        max_id = try_id;
    }
    else
    {
        max_id = DDS_RtpsWellKnownPorts_get_max_participant_index(
                &participant->qos.protocol.rtps_well_known_ports);
        try_id = 0;
    }

    done = RTI_FALSE;

    for (i = try_id; (i <= max_id); ++i)
    {
        DDS_LocatorSeq_set_length(&participant->builtin_data.metatraffic_multicast_locators,0);
        DDS_LocatorSeq_set_length(&participant->builtin_data.metatraffic_unicast_locators,0);
        DDS_LocatorSeq_set_length(&participant->builtin_data.default_multicast_locators,0);
        DDS_LocatorSeq_set_length(&participant->builtin_data.default_unicast_locators,0);

        NETIO_AddressSeq_set_length(&reserved_address,0);
        if (!NETIO_BindResolver_reserve_addresses(
             participant->bind_resolver,i,
             (struct REDA_StringSeq*)&participant->qos.discovery.enabled_transports,
             NETIO_ROUTEKIND_META,
             &reserved_address,
             (struct NETIO_AddressSeq*)&participant->builtin_data.metatraffic_multicast_locators,
             (struct NETIO_AddressSeq*)&participant->builtin_data.metatraffic_unicast_locators))
        {
            continue;
        }

        NETIO_AddressSeq_set_length(&reserved_address,0);
        if (!NETIO_BindResolver_reserve_addresses(
             participant->bind_resolver,i,
             (struct REDA_StringSeq*)&participant->qos.user_traffic.enabled_transports,NETIO_ROUTEKIND_USER,
             &reserved_address,
             (struct NETIO_AddressSeq*)&participant->builtin_data.default_multicast_locators,
             (struct NETIO_AddressSeq*)&participant->builtin_data.default_unicast_locators))
        {
            continue;
        }

        break;
    }

    if (i > max_id)
    {
        goto done;
    }

    c_factory = RT_Registry_lookup(participant->config.registry,"rtps");
    if (c_factory == NULL)
    {
        goto done;
    }

    NETIO_Address_init(&src_address,NETIO_ADDRESS_KIND_GUID);

    {
        DDS_InstanceHandle_t instance_handle;
        instance_handle = DDS_Entity_get_instance_handle(DDS_DomainParticipant_as_entity(participant));
        NETIO_Address_set_guid(&src_address,0,instance_handle.octet);
    }

    rtps_property.intf_address =  src_address;
    rtps_property._parent._parent.db = participant->database;
    rtps_property._parent.max_routes = participant->qos.resource_limits.matching_writer_reader_pair_allocation;
    rtps_property._parent.max_binds = participant->qos.resource_limits.matching_reader_writer_pair_allocation;
    rtps_property.mode = RTPS_INTERFACEMODE_EXTERNAL_RECEIVER;

    participant->rtps_intf = NETIO_InterfaceFactory_create_component(c_factory,
                                    &rtps_property._parent._parent,NULL);
    if (participant->rtps_intf == NULL)
    {
        goto done;
    }

    participant->participant_id = i;

    if (participant->disc_plugin != NULL)
    {
      rc = NDDS_Discovery_Plugin_on_after_local_participant_created(
                                                   participant->disc_plugin,
                                                   participant,
                                                   &participant->builtin_data);
      if (rc != DDS_RETCODE_OK)
      {
          DDSC_Log_error(LOG_FAIL_INIT_DISCOVERY_PLUGIN);
          goto done;
      }
    }

    participant->as_entity.state = RTIDDS_ENTITY_STATE_CREATED;
    NETIO_AddressSeq_finalize(&reserved_address);

    success = DDS_BOOLEAN_TRUE;

done:

    if (!success && participant != NULL)
    {
        DDS_DomainParticipant_finalize(participant);
        participant = NULL;
    }

    return participant;
}

DDS_ReturnCode_t
DDS_DomainParticipant_register_type(DDS_DomainParticipant *participant,
                                    const char *type_name,
                                    struct NDDS_Type_Plugin *plugin)
{
    struct DDS_DomainParticipantImpl *self =
                            (struct DDS_DomainParticipantImpl *)participant;
    struct DDS_TypeImpl *a_type = NULL;
    DB_ReturnCode_T dbrc;
    DDS_ReturnCode_t rc = DDS_RETCODE_ERROR;

    if ((participant == NULL) || (type_name == NULL) || (plugin == NULL) ||
         (DDS_String_length(type_name) > MIG_RTPS_PATHNAME_LEN_MAX))
    {
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(self->database);

    dbrc = DB_Table_select_match(self->type_table,DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&a_type,(DB_Key_T)type_name);

    if (dbrc == DB_RETCODE_EXISTS)
    {
	rc = DDS_RETCODE_OK;
        goto done;
    }

    if (dbrc != DB_RETCODE_NOT_EXISTS)
    {
        rc = DDS_RETCODE_ERROR;
        goto done;
    }

    rc = DDS_RETCODE_ERROR;
    dbrc = DB_Table_create_record(self->type_table,(DB_Record_T*)&a_type);
    if (dbrc != DB_RETCODE_OK)
    {
        rc = DDS_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    if (!DDS_TypeImpl_initalize(a_type,type_name,plugin))
    {
        goto done;
    }

    dbrc = DB_Table_insert_record(self->type_table,(DB_Record_T*)a_type);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    rc = DDS_RETCODE_OK;

done:

    DB_Database_unlock(self->database);
    return rc;
}

struct NDDS_Type_Plugin*
DDS_DomainParticipant_unregister_type(DDS_DomainParticipant * participant,
                                      const char *type_name)
{
    struct DDS_DomainParticipantImpl *self =
                            (struct DDS_DomainParticipantImpl *)participant;
    struct DDS_TypeImpl *a_type = NULL;
    DB_ReturnCode_T dbrc;
    struct NDDS_Type_Plugin *type_plugin = NULL;

    DB_Database_lock(self->database);

    dbrc = DB_Table_select_match(self->type_table,DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&a_type,(DB_Key_T)type_name);

    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    if (DDS_TypeImpl_ref_count(a_type) != 0)
    {
        goto done;
    }

    a_type = NULL;
    dbrc = DB_Table_remove_record(self->type_table,
                                 (DB_Record_T*)&a_type,(DB_Key_T)type_name);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    type_plugin = a_type->plugin;

    dbrc = DB_Table_delete_record(self->type_table,(DB_Record_T*)a_type);
    if (dbrc != DB_RETCODE_OK)
    {
        type_plugin = NULL;
        goto done;
    }

done:

    DB_Database_unlock(self->database);
    return type_plugin;
}

/*******************************************************************************
 *                                    Public API
 ******************************************************************************/
DDS_ReturnCode_t
DDS_DomainParticipant_delete_contained_entities(DDS_DomainParticipant *self)
{
    struct DDS_DomainParticipantImpl *participant =
            (struct DDS_DomainParticipantImpl *)self;
    DB_ReturnCode_T dbrc;
    DDS_ReturnCode_t ddsrc = DDS_RETCODE_OK;
    DB_Cursor_T cursor;
    DDS_Topic *topic;
    DDS_Publisher *publisher;
    DDS_Subscriber *subscriber;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(participant->database);

    dbrc = DB_Table_select_all_default(participant->local_publisher_table,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor, (DB_Record_T*)&publisher);
        if (dbrc == DB_RETCODE_OK)
        {
            ddsrc = DDS_Publisher_delete_contained_entities(publisher);
            if ((ddsrc == DDS_RETCODE_OK) &&
                    !DDS_PublisherImpl_is_hidden(publisher))
            {
                ddsrc = DDS_DomainParticipant_delete_publisher(participant,
                        publisher);
            }
        }
    } while ((dbrc == DB_RETCODE_OK) && (ddsrc == DDS_RETCODE_OK));
    DB_Cursor_finish(participant->local_publisher_table,cursor);

    if ((dbrc != DB_RETCODE_NO_DATA) || (ddsrc != DDS_RETCODE_OK))
    {
        goto done;
    }

    /* Delete subscriber */
    cursor = NULL;
    dbrc = DB_Table_select_all_default(participant->local_subscriber_table,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor, (DB_Record_T*)&subscriber);
        if (dbrc == DB_RETCODE_OK)
        {
            ddsrc = DDS_Subscriber_delete_contained_entities(subscriber);
            if ((ddsrc == DDS_RETCODE_OK) &&
                    !DDS_SubscriberImpl_is_hidden(subscriber))
            {
                ddsrc = DDS_DomainParticipant_delete_subscriber(participant,
                        subscriber);
            }
        }
    } while ((dbrc == DB_RETCODE_OK) && (ddsrc == DDS_RETCODE_OK));
    DB_Cursor_finish(participant->local_subscriber_table,cursor);

    if ((dbrc != DB_RETCODE_NO_DATA) || (ddsrc != DDS_RETCODE_OK))
    {
        goto done;
    }

    /* Delete topics */
    cursor = NULL;
    dbrc = DB_Table_select_all_default(participant->topic_table,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor, (DB_Record_T*)&topic);
        if (dbrc == DB_RETCODE_OK)
        {
            if (!DDS_TopicImpl_is_hidden(topic))
            {
                ddsrc = DDS_DomainParticipant_delete_topic(self, topic);
            }
        }
    } while ((dbrc == DB_RETCODE_OK) && (ddsrc == DDS_RETCODE_OK));
    DB_Cursor_finish(participant->topic_table,cursor);

    if ((dbrc != DB_RETCODE_NO_DATA) || (ddsrc != DDS_RETCODE_OK))
    {
        goto done;
    }

    ddsrc = DDS_RETCODE_OK;

done:

    DB_Database_unlock(participant->database);

    return ddsrc;
}

DDS_Publisher*
DDS_DomainParticipant_create_publisher(DDS_DomainParticipant *self,
                                const struct DDS_PublisherQos *qos,
                                const struct DDS_PublisherListener *listener,
                                DDS_StatusMask mask)
{
    DDS_Publisher *retval = NULL;
    DB_ReturnCode_T dbrc;
    struct DDS_DomainParticipantImpl *participant =
            (struct DDS_DomainParticipantImpl *)self;
    struct DDS_PublisherImpl *publisher = NULL;
    struct NDDS_PublisherConfig pub_property = NDDS_PublisherConfig_INITIALIZER;
    DDS_Long object_id;

    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    DB_Database_lock(participant->database);

    object_id = OSAPI_System_get_next_object_id();
    if (object_id == -1)
    {
        goto done;
    }

    dbrc = DB_Table_create_record(participant->local_publisher_table,
            (DB_Record_T*)&publisher);

    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    pub_property.db = participant->database;
    pub_property.timer = participant->timer;
    pub_property.registry = participant->config.registry;
    pub_property.local_writer_table = participant->local_writer_table;
    pub_property.object_id_generator = OSAPI_System_get_next_object_id;
    pub_property.get_parent_handle = DDS_DomainParticipantImpl_get_instance_handle;
    pub_property.on_after_datawriter_created = DomainParticipantEvent_on_after_datawriter_created;
    pub_property.on_after_datawriter_enabled = DomainParticipantEvent_on_after_datawriter_enabled;
    pub_property.on_after_datawriter_modified = DomainParticipantEvent_on_after_datawriter_modified;
    pub_property.on_before_datawriter_deleted = DomainParticipantEvent_on_before_datawriter_deleted;
    pub_property.on_after_enabled = DomainParticipantEvent_on_after_publisher_enabled;
    pub_property.on_after_modified = DomainParticipantEvent_on_after_publisher_modified;
    pub_property.on_liveliness_lost = NDDS_DomainParticipant_on_liveliness_lost;
    pub_property.on_offered_deadline_missed = NDDS_DomainParticipant_on_offered_deadline_missed;
    pub_property.on_offered_incompatible_qos = NDDS_DomainParticipant_on_offered_incompatible_qos;
    pub_property.on_publication_matched = NDDS_DomainParticipant_on_publication_matched;
    pub_property.default_unicast = &participant->builtin_data.default_unicast_locators;
    pub_property.default_multicast = &participant->builtin_data.default_multicast_locators;
    pub_property.default_meta_multicast = &participant->builtin_data.metatraffic_multicast_locators;
    pub_property.default_meta_unicast = &participant->builtin_data.metatraffic_unicast_locators;
    pub_property.bind_resolver = participant->bind_resolver;
    pub_property.enabled_transports = &participant->qos.transports.enabled_transports;
    pub_property.route_resolver = participant->route_resolver;

    if (!DDS_PublisherImpl_initialize(publisher,self,qos,listener,mask,
                                      object_id,&pub_property))
    {
        (void)DB_Table_delete_record(participant->local_publisher_table,
                                     (DB_Record_T*)publisher);
        goto done;
    }

    dbrc = DB_Table_insert_record(participant->local_publisher_table,publisher);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    if ((self->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED) &&
            self->qos.entity_factory.autoenable_created_entities)
    {
        DDS_Entity_enable(DDS_Publisher_as_entity(publisher));
    }

    retval = publisher;

done:

    DB_Database_unlock(participant->database);

    return retval;
}

DDS_ReturnCode_t
DDS_DomainParticipant_delete_publisher(DDS_DomainParticipant *self,
        DDS_Publisher *publisher)
{
    struct DDS_PublisherImpl *pub_impl = (struct DDS_PublisherImpl*)publisher;
    DB_ReturnCode_T dbrc;
    DDS_ReturnCode_t ddsrc = DDS_RETCODE_ERROR;

    if ((self == NULL) || (publisher == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(self->database);

    if (!DDS_PublisherImpl_finalize(pub_impl))
    {
        goto done;
    }

    dbrc = DB_Table_delete_record(self->local_publisher_table,pub_impl);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    ddsrc = DDS_RETCODE_OK;

done:

    DB_Database_unlock(self->database);

    return ddsrc;
}

DDS_Subscriber*
DDS_DomainParticipant_create_subscriber(DDS_DomainParticipant *self,
                                const struct DDS_SubscriberQos *qos,
                                const struct DDS_SubscriberListener *listener,
        DDS_StatusMask mask)
{
    DDS_Subscriber *retval = NULL;
    DB_ReturnCode_T dbrc;
    struct DDS_DomainParticipantImpl *participant =
            (struct DDS_DomainParticipantImpl *)self;
    struct DDS_SubscriberImpl *subscriber = NULL;
    struct NDDS_SubscriberConfig sub_property =
            NDDS_SubscriberConfig_INITIALIZER;
    DDS_Long object_id;

    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    DB_Database_lock(participant->database);

    object_id = OSAPI_System_get_next_object_id();
    if (object_id == -1)
    {
        goto done;
    }

    dbrc = DB_Table_create_record(participant->local_subscriber_table,
            (DB_Record_T*)&subscriber);

    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    sub_property.db = participant->database;
    sub_property.timer = participant->timer;
    sub_property.registry = participant->config.registry;
    sub_property.local_reader_table = participant->local_reader_table;
    sub_property.object_id_generator = OSAPI_System_get_next_object_id;
    sub_property.get_parent_handle = DDS_DomainParticipantImpl_get_instance_handle;
    sub_property.on_after_datareader_created = DomainParticipantEvent_on_after_datareader_created;
    sub_property.on_after_datareader_enabled = DomainParticipantEvent_on_after_datareader_enabled;
    sub_property.on_after_datareader_modified = DomainParticipantEvent_on_after_datareader_modified;
    sub_property.on_before_datareader_deleted = DomainParticipantEvent_on_before_datareader_deleted;
    sub_property.on_after_enabled = DomainParticipantEvent_on_after_subscriber_enabled;
    sub_property.on_after_modified = DomainParticipantEvent_on_after_subscriber_modified;
    sub_property.on_requested_deadline_missed = NDDS_DomainParticipant_on_requested_deadline_missed;
    sub_property.on_requested_incompatible_qos = NDDS_DomainParticipant_on_requested_incompatible_qos;
    sub_property.on_sample_rejected = NDDS_DomainParticipant_on_sample_rejected;
    sub_property.on_liveliness_changed = NDDS_DomainParticipant_on_liveliness_changed;
    sub_property.on_data_available = NDDS_DomainParticipant_on_data_available;
    sub_property.on_subscription_matched = NDDS_DomainParticipant_on_subscription_matched;
    sub_property.on_sample_lost = NDDS_DomainParticipant_on_sample_lost;
    sub_property.default_unicast = &participant->builtin_data.default_unicast_locators;
    sub_property.default_multicast = &participant->builtin_data.default_multicast_locators;
    sub_property.default_meta_multicast = &participant->builtin_data.metatraffic_multicast_locators;
    sub_property.default_meta_unicast = &participant->builtin_data.metatraffic_unicast_locators;
    sub_property.bind_resolver = participant->bind_resolver;
    sub_property.enabled_transports = &participant->qos.transports.enabled_transports;
    sub_property.route_resolver = participant->route_resolver;

    if (!DDS_SubscriberImpl_initialize(subscriber,self,qos,listener,mask,
                                       object_id,&sub_property))
    {
        (void)DB_Table_delete_record(participant->local_subscriber_table,
                                     (DB_Record_T*)subscriber);
        goto done;
    }

    dbrc = DB_Table_insert_record(participant->local_subscriber_table,subscriber);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    if ((self->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED) &&
            self->qos.entity_factory.autoenable_created_entities)
    {
        DDS_Entity_enable(DDS_Subscriber_as_entity(subscriber));
    }

    retval = subscriber;

done:

    DB_Database_unlock(participant->database);

    return retval;
}

DDS_ReturnCode_t
DDS_DomainParticipant_delete_subscriber(DDS_DomainParticipant *self,
                                       DDS_Subscriber *publisher)
{
    struct DDS_SubscriberImpl *sub_impl = (struct DDS_SubscriberImpl*)publisher;
    DB_ReturnCode_T dbrc;
    DDS_ReturnCode_t ddsrc = DDS_RETCODE_ERROR;

    if ((self == NULL) || (publisher == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(self->database);

    if (!DDS_SubscriberImpl_finalize(sub_impl))
    {
        goto done;
    }

    dbrc = DB_Table_delete_record(self->local_subscriber_table,sub_impl);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    ddsrc = DDS_RETCODE_OK;

done:

    DB_Database_unlock(self->database);
    return ddsrc;
}

DDS_Topic*
DDS_DomainParticipant_create_topic(DDS_DomainParticipant *self,
                                   const char *topic_name,
                                   const char *type_name,
                                   const struct DDS_TopicQos *qos,
                                   const struct DDS_TopicListener *listener,
                                   DDS_StatusMask mask)
{
    DDS_Topic *retval = NULL;
    DB_ReturnCode_T dbrc;
    struct DDS_DomainParticipantImpl *participant =
            (struct DDS_DomainParticipantImpl *)self;
    struct DDS_TopicImpl *topic = NULL;
    struct NDDS_TopicConfig topic_config = NDDS_TopicConfig_INITIALIZER;
    struct DDS_TypeImpl *a_type = NULL;

    if ((self == NULL) || (topic_name == NULL) ||
        (type_name == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    DB_Database_lock(participant->database);

    dbrc = DB_Table_select_match(self->type_table,DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&a_type,(DB_Key_T)type_name);

    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    dbrc = DB_Table_create_record(self->topic_table,(DB_Record_T*)&topic);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    topic_config.db = participant->database;
    topic_config.on_inconsistent_topic = NDDS_DomainParticipant_on_inconsistent_topic;

    if (!DDS_TopicImpl_initialize(topic,
                                self,topic_name,a_type,
                                qos,listener,mask,
                                0,&topic_config))
    {
        goto done;
    }

    dbrc = DB_Table_insert_record(participant->topic_table,topic);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    if ((self->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED) &&
            self->qos.entity_factory.autoenable_created_entities)
    {
        if (DDS_Entity_enable(DDS_Topic_as_entity(topic)) != DDS_RETCODE_OK)
        {
            goto done;
        }
    }

    retval = topic;

done:

    DB_Database_unlock(participant->database);

    return retval;
}

DDS_ReturnCode_t
DDS_DomainParticipant_delete_topic(DDS_DomainParticipant *self,
                                   DDS_Topic *topic)
{
    DB_ReturnCode_T dbrc;
    DDS_ReturnCode_t ddsrc = DDS_RETCODE_ERROR;

    if ((self == NULL) || (topic == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(self->database);

    if (!DDS_TopicImpl_finalize(topic))
    {
        goto done;
    }

    dbrc = DB_Table_delete_record(self->topic_table,(DB_Record_T)topic);
    if (dbrc != DB_RETCODE_OK)
    {
        goto done;
    }

    ddsrc = DDS_RETCODE_OK;

done:

    DB_Database_unlock(self->database);

    return ddsrc;
}

#if INCLUDE_API_LOOKUP 
DDS_Topic*
DDS_DomainParticipant_find_topic(DDS_DomainParticipant *self,
                                 const char *topic_name,
                                 const struct DDS_Duration_t *timeout)
{
    DDS_Topic *topic = NULL;
    DB_ReturnCode_T dbrc;

    if ((self == NULL) || (topic_name == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    /* We do not support timeout. If the topic does not exist at call-time
     * we return NULL.
     * TODO: Need to reference count the topic, a topic found with find_topic
     * must also be deleted
     */
    DB_Database_lock(self->database);

    dbrc = DB_Table_select_match(self->topic_table,DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&topic,(DB_Key_T)topic_name);

    DB_Database_unlock(self->database);

    return topic;
}
#endif

#if INCLUDE_API_LOOKUP 
DDS_TopicDescription*
DDS_DomainParticipant_lookup_topicdescription(DDS_DomainParticipant *self,
                                              const char *topic_name)
{
    DDS_TopicDescription *topic_description = NULL;
    DDS_Topic *topic = NULL;

    if ((self == NULL) || (topic_name == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    topic = DDS_DomainParticipant_find_topic(self, topic_name, NULL);
    if (topic != NULL)
    {
        topic_description = DDS_Topic_as_topicdescription(topic);
    }

    return topic_description;
}
#endif

DDS_DomainId_t
DDS_DomainParticipant_get_domain_id(DDS_DomainParticipant * self)
{
    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return -1;
    }

    return self->domain_id;
}

OSAPI_Timer_t
DDS_DomainParticipant_get_timer(DDS_DomainParticipant * const self)
{
    return ((struct DDS_DomainParticipantImpl *)self)->timer;
}

/*******************************************************************************
 *                             OPTIONAL APIs
 ******************************************************************************/

#ifdef INCLUDE_API_LISTENER
DDS_ReturnCode_t
DDS_DomainParticipant_set_listener(DDS_DomainParticipant *self,
                    const struct DDS_DomainParticipantListener *l,
                    DDS_StatusMask mask)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
    struct DDS_DomainParticipantListener default_listener =
            DDS_DomainParticipantListener_INITIALIZER;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->shared_lock);

    if (l == NULL)
    {
        if (mask == DDS_STATUS_MASK_NONE)
        {
            self->mask = mask;
            self->listener = default_listener;
            retcode = DDS_RETCODE_OK;
        }
        goto done;
    }

    if (!DDS_Topic_listener_is_consistent(&l->as_topiclistener, mask))
    {
        DDSC_Log_error(LOG_INCONSISTENT_TOPIC_LISTENER);
        goto done;
    }

    if (!DDS_PublisherImpl_listener_is_consistent(&l->as_publisherlistener, mask))
    {
        DDSC_Log_error(LOG_INCONSISTENT_PUBLISHER_LISTENER);
        goto done;
    }

    if (!DDS_SubscriberImpl_listener_is_consistent(&l->as_subscriberlistener, mask))
    {
        DDSC_Log_error(LOG_INCONSISTENT_SUBSCRIBER_LISTENER);
        goto done;
    }

    self->listener = *l;
    self->mask = mask;

    retcode = DDS_RETCODE_OK;

done:

    OSAPI_Mutex_give(self->shared_lock);

    return retcode;
}

struct DDS_DomainParticipantListener
DDS_DomainParticipant_get_listener(DDS_DomainParticipant * self)
{
    struct DDS_DomainParticipantListener retval =
            DDS_DomainParticipantListener_INITIALIZER;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return retval;
    }

    OSAPI_Mutex_take(self->shared_lock);

    retval = self->listener;

    OSAPI_Mutex_give(self->shared_lock);

    return retval;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipant_get_default_publisher_qos(DDS_DomainParticipant * self,
        struct DDS_PublisherQos * qos)
{
    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->shared_lock);

    *qos = self->_default_publisher_qos;

    OSAPI_Mutex_give(self->shared_lock);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipant_set_default_publisher_qos(DDS_DomainParticipant * self,
        const struct DDS_PublisherQos *
        qos)
{
    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_PublisherQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_PUBLISHER_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    OSAPI_Mutex_take(self->shared_lock);

    self->_default_publisher_qos = *qos;

    OSAPI_Mutex_give(self->shared_lock);

    return DDS_RETCODE_OK;
}
#endif

DDS_ReturnCode_t
DDS_DomainParticipant_set_qos(DDS_DomainParticipant * self,
        const struct DDS_DomainParticipantQos * qos)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;

    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_DomainParticipantQos_immutable_is_equal(&self->qos, qos))
    {
        DDSC_Log_error(LOG_IMMUTABLE_PARTICIPANT_QOS);
        return DDS_RETCODE_IMMUTABLE_POLICY;
    }

    OSAPI_Mutex_take(self->shared_lock);

    if (DDS_Entity_is_enabled(&self->as_entity))
    {
        retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    self->qos = *qos;

done:

    OSAPI_Mutex_give(self->shared_lock);
    return retcode;
}

DDS_ReturnCode_t
DDS_DomainParticipant_get_qos(DDS_DomainParticipant * self,
                              struct DDS_DomainParticipantQos *qos)
{
    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->shared_lock);

    DDS_DomainParticipantQos_copy(qos,&self->qos);

    OSAPI_Mutex_give(self->shared_lock);

    return DDS_RETCODE_OK;
}

struct DDS_DomainParticipantQos*
DDS_DomainParticipant_get_qos_ref(DDS_DomainParticipant *self)
{
    return &self->qos;
}

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipant_get_default_subscriber_qos(DDS_DomainParticipant *self,
                                                 struct DDS_SubscriberQos *qos)
{
    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->shared_lock);

    *qos = self->_default_subscriber_qos;

    OSAPI_Mutex_give(self->shared_lock);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipant_set_default_subscriber_qos(DDS_DomainParticipant * self,
        const struct DDS_SubscriberQos
        * qos)
{
    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_SubscriberQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_SUBSCRIBER_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    OSAPI_Mutex_take(self->shared_lock);

    self->_default_subscriber_qos = *qos;

    OSAPI_Mutex_give(self->shared_lock);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipant_get_default_topic_qos(DDS_DomainParticipant * self,
        struct DDS_TopicQos * qos)
{
    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->shared_lock);

    *qos = self->_default_topic_qos;

    OSAPI_Mutex_give(self->shared_lock);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipant_set_default_topic_qos(DDS_DomainParticipant * self,
        const struct DDS_TopicQos * qos)
{
    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_TopicQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_TOPIC_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    OSAPI_Mutex_take(self->shared_lock);

    self->_default_topic_qos = *qos;

    OSAPI_Mutex_give(self->shared_lock);

    return DDS_RETCODE_OK;
}
#endif


#ifdef INCLUDE_API_UTILITY
DDS_ReturnCode_t
DDS_DomainParticipant_get_current_time(DDS_DomainParticipant * self,
        struct DDS_Time_t * current_time)
{
    struct OSAPI_NtpTime now;

    if ((self == NULL) || (current_time == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_System_get_time(&now);
    OSAPI_NtpTime_unpackToNanosec((&(current_time->sec)),
            (&(current_time->nanosec)), &now);

    return DDS_RETCODE_OK;
}
#endif

struct DDS_RemoteParticipantImpl;

#if INCLUDE_API_LOOKUP
DDS_ReturnCode_t
DDS_DomainParticipant_get_discovered_participants(DDS_DomainParticipant * self,
                              struct DDS_InstanceHandleSeq *participant_handles)
{
    struct DDS_DomainParticipantImpl *participant =
            (struct DDS_DomainParticipantImpl *)self;
    DDS_Long count;
    DB_ReturnCode_T dbrc;
    DDS_ReturnCode_t ddsrc = DDS_RETCODE_OK;
    DB_Cursor_T cursor = NULL;
    struct DDS_RemoteParticipantImpl *rem_participant;

    if ((self == NULL) || (participant_handles == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_Entity_is_enabled(&participant->as_entity))
    {
        return DDS_RETCODE_NOT_ENABLED;
    }

    DB_Database_lock(participant->database);

    dbrc = DB_Table_select_all_default(participant->remote_participant_table,&cursor);
    if (dbrc != DB_RETCODE_OK)
    {
        ddsrc = DDS_RETCODE_ERROR;
    }

    count = DB_Cursor_get_count(cursor);
    if (count == 0)
    {
        goto done;
    }

    if (!DDS_InstanceHandleSeq_set_maximum(participant_handles, count))
    {
        ddsrc = DDS_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    if (!DDS_InstanceHandleSeq_set_length(participant_handles, count))
    {
        ddsrc = DDS_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    count = 0;
    do
    {
        dbrc = DB_Cursor_get_next(cursor, (DB_Record_T*)&rem_participant);
#if 0
        TODO: Fix this when remote discovery has been implemented
        if ((dbrc == DB_RETCODE_OK) &&
            (rem_participant->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED))
        {
            DDS_InstanceHandle_from_rtps(DDS_InstanceHandleSeq_get_reference
                    (participant_handles, count),
                    (const struct MIGRtpsGuid *)
                    &rem_participant->data.key);
            ++count;
        }
#endif
    } while (dbrc == DDS_RETCODE_OK);
    DB_Cursor_finish(participant->remote_participant_table,cursor);

done:

    DB_Database_unlock(participant->database);

    return ddsrc;
}

DDS_ReturnCode_t
DDS_DomainParticipant_get_discovered_participant_data(
            DDS_DomainParticipant *self,
            struct DDS_ParticipantBuiltinTopicData *participant_data,
            const DDS_InstanceHandle_t *participant_handle)
{
    DDS_BuiltinTopicKey_t key;
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    DB_ReturnCode_T dbrc;
    struct DDS_RemoteParticipantImpl *rem_participant = NULL;
    struct DDS_DomainParticipantImpl *participant =
            (struct DDS_DomainParticipantImpl *)self;

    if ((self == NULL) || (participant_data == NULL) ||
            (participant_handle == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_Entity_is_enabled(&participant->as_entity))
    {
        return DDS_RETCODE_NOT_ENABLED;
    }

    DDS_InstanceHandle_to_rtps((struct MIGRtpsGuid *)&key, participant_handle);

    DB_Database_lock(participant->database);

    rem_participant = NULL;
    dbrc = DB_Table_select_match(participant->remote_participant_table,
                                      DB_TABLE_DEFAULT_INDEX,
                                      (DB_Record_T*)&rem_participant,
                                      (DB_Key_T)&key);

    if (dbrc != DB_RETCODE_OK)
    {
        retcode = DDS_RETCODE_ENTITY_NOT_EXIST;
        goto done;
    }
#if 0
    TODO:
    if (!DDS_ParticipantBuiltinTopicData_copy(participant_data,
                                              &rem_participant->data))
    {
        retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
    }
#endif

done:
    DB_Database_unlock(participant->database);

    return retcode;
}
#endif

