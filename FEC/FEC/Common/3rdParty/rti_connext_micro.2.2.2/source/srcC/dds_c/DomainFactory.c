/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef reda_circularlist_h
#include "reda/reda_circularlist.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef netio_common_h
#include "netio/netio_common.h"
#endif
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
#ifndef rtps_rtps_h
#include "rtps/rtps_rtps.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif

#include "Entity.pkg.h"
#include "DomainParticipantQos.pkg.h"
#include "DomainParticipant.pkg.h"
#include "DomainFactoryQos.pkg.h"
#include "DataWriterInterface.pkg.h"
#include "DataReaderInterface.pkg.h"

struct DDS_DomainParticipantEntry
{
    struct REDA_CircularListNode node;
    struct DDS_DomainParticipantImpl participant;
};

struct DDS_DomainParticipantFactoryImpl
{
    DDS_Boolean is_initialized;
    struct OSAPI_Mutex *factory_lock;
    struct DDS_DomainParticipantFactoryQos qos;
    struct DDS_DomainParticipantQos default_participant_qos;
    RT_Registry_T *registry;
    REDA_BufferPool_T participant_pool;
    REDA_CircularList_T participants;
    DDS_Boolean immutable_qos_enabled;
    DB_Database_T db;
    DDS_Long instance_counter;
};

RTI_PRIVATE struct DDS_DomainParticipantFactoryImpl DDS_g_DomainFactory =
{
        DDS_BOOLEAN_FALSE
};

struct UDP_InterfaceFactoryProperty DomainParticipantFactory_g_udp_property =
                                       UDP_InterfaceFactoryProperty_INITIALIZER;

/*** SOURCE_BEGIN ***/

DDS_DomainParticipantFactory*
DDS_DomainParticipantFactory_get_instance(void)
{
    struct DB_DatabaseProperty db_property = DB_DatabaseProperty_INITIALIZER;
    struct RT_ComponentFactoryProperty cf_property;
    struct RT_RegistryProperty rt_property =
                            RT_RegistryProperty_INITIALIZER;

    if (!DDS_g_DomainFactory.is_initialized)
    {
        DDS_g_DomainFactory.qos = DDS_PARTICIPANT_FACTORY_QOS_DEFAULT;
        DDS_g_DomainFactory.default_participant_qos = DDS_PARTICIPANT_QOS_DEFAULT;
        DDS_g_DomainFactory.is_initialized = DDS_BOOLEAN_TRUE;
        DDS_g_DomainFactory.participant_pool = NULL;
        DDS_g_DomainFactory.immutable_qos_enabled = DDS_BOOLEAN_FALSE;
        DDS_g_DomainFactory.registry = RT_Registry_get_instance();
        DDS_g_DomainFactory.instance_counter = 1;
        DDS_g_DomainFactory.factory_lock = OSAPI_Mutex_new();
        if (DDS_g_DomainFactory.factory_lock == NULL)
        {
            return NULL;
        }
        OSAPI_System_initialize();

        DDS_g_DomainFactory.db = NULL;
        /* Maximum number of factories to register */
        db_property.max_tables = 16;
        db_property.lock_mode = DB_LOCK_LEVEL_SHARED;
        if (DB_Database_create(&DDS_g_DomainFactory.db,"shared",&db_property,
                    DDS_g_DomainFactory.factory_lock) != DB_RETCODE_OK)
        {
            return NULL;
        }

        RT_Registry_get_property(DDS_g_DomainFactory.registry,&rt_property);
        rt_property.db = DDS_g_DomainFactory.db;
        rt_property.max_factories = 10;
        RT_Registry_set_property(DDS_g_DomainFactory.registry,&rt_property);

        if (!RT_Registry_register(DDS_g_DomainFactory.registry,
                                   "_intra",
                                   LOOPInterfaceFactory_get_interface(),
                                   &cf_property, NULL))
        {
            return NULL;
        }

        if (!RT_Registry_register(DDS_g_DomainFactory.registry,
                      "_udp",
                      UDP_InterfaceFactory_get_interface(),
                      &DomainParticipantFactory_g_udp_property._parent._parent,
                      NULL))
        {
            return NULL;
        }

        if (!RT_Registry_register(DDS_g_DomainFactory.registry,
                                   "rtps",
                                   RTPSInterfaceFactory_get_interface(),
                                   &cf_property,
                                   NULL))
        {
            return NULL;
        }

        if (!RT_Registry_register(DDS_g_DomainFactory.registry,
                "wi",
                DDS_DataWriterInterfaceFactory_get_interface(),
                NULL, NULL))
        {
            return NULL;
        }

        if (!RT_Registry_register(DDS_g_DomainFactory.registry,
                "ri",
                DDS_DataReaderInterfaceFactory_get_interface(),
                NULL, NULL))
        {
            return NULL;
        }

        DDS_g_DomainFactory.is_initialized = DDS_BOOLEAN_TRUE;
    }

    return &DDS_g_DomainFactory;
}

RT_Registry_T*
DDS_DomainParticipantFactory_get_registry(DDS_DomainParticipantFactory *self)
{
    return self->registry;
}

DDS_ReturnCode_t
DDS_DomainParticipantFactory_finalize_instance(void)
{
    if (!DDS_g_DomainFactory.is_initialized)
    {
        return DDS_RETCODE_OK;
    }

    DDS_g_DomainFactory.is_initialized = DDS_BOOLEAN_FALSE;

    if (DDS_g_DomainFactory.factory_lock)
    {
        OSAPI_Mutex_delete(DDS_g_DomainFactory.factory_lock);
        DDS_g_DomainFactory.factory_lock = NULL;
    }

    if (DDS_g_DomainFactory.participant_pool)
    {
        REDA_BufferPool_delete(DDS_g_DomainFactory.participant_pool);
        DDS_g_DomainFactory.participant_pool = NULL;
    }

    (void)RT_Registry_unregister(DDS_g_DomainFactory.registry,
                               "_intra",
                               NULL, NULL);

    (void)RT_Registry_unregister(DDS_g_DomainFactory.registry,
                               "rtps",
                               NULL, NULL);

    (void)RT_Registry_unregister(DDS_g_DomainFactory.registry,
                               "ri",
                               NULL, NULL);

    (void)RT_Registry_unregister(DDS_g_DomainFactory.registry,
                               "wi",
                               NULL, NULL);

    (void)RT_Registry_unregister(DDS_g_DomainFactory.registry,
                               "_udp",
                               NULL, NULL);

    (void)RT_Registry_finalize(DDS_g_DomainFactory.registry);

    if (DDS_g_DomainFactory.db != NULL)
    {
        DB_Database_delete(DDS_g_DomainFactory.db);
        DDS_g_DomainFactory.db = NULL;
    }

	OSAPI_System_finalize();

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
DDS_DomainParticipantFactory_get_qos(
        DDS_DomainParticipantFactory *self,
        struct DDS_DomainParticipantFactoryQos *qos)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;

    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->factory_lock);

    retcode = DDS_DomainParticipantFactoryQos_copy(qos, &self->qos);

    OSAPI_Mutex_give(self->factory_lock);

    return retcode;
}

DDS_ReturnCode_t
DDS_DomainParticipantFactory_set_qos(DDS_DomainParticipantFactory *self,
                             const struct DDS_DomainParticipantFactoryQos *qos)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;

    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->factory_lock);

    if ((self->immutable_qos_enabled) &&
        !DDS_DomainParticipantFactoryQos_immutable_is_equal(&self->qos, qos))
    {
        OSAPI_Mutex_give(self->factory_lock);
        return DDS_RETCODE_IMMUTABLE_POLICY;
    }

    retcode = DDS_DomainParticipantFactoryQos_copy(&self->qos, qos);

    OSAPI_Mutex_give(self->factory_lock);

    return retcode;
}

DDS_DomainParticipant*
DDS_DomainParticipantFactory_create_participant(
                    DDS_DomainParticipantFactory *self,
                    DDS_DomainId_t domain_id,
                    const struct DDS_DomainParticipantQos *qos,
                    const struct DDS_DomainParticipantListener *listener,
                    DDS_StatusMask mask)
{
    DDS_DomainParticipant *retval = NULL;
    struct REDA_BufferPoolProperty p_prop = REDA_BufferPoolProperty_INITIALIZER;
    struct DDS_DomainParticipantEntry *participant;
    struct NDDS_ParticipantConfig dp_cfg = NDDS_ParticipantConfig_INITIALIZER;
    DDS_ReturnCode_t ddsrc;
    struct MIGRtpsGuid guid;

    if ((self == NULL) || (domain_id < 0) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    if (((qos->protocol.rtps_host_id == DDS_RTPS_AUTO_ID) ||
        (qos->protocol.rtps_app_id == DDS_RTPS_AUTO_ID) ||
        (qos->protocol.rtps_instance_id == DDS_RTPS_AUTO_ID)) &&
        ((qos->protocol.rtps_host_id + qos->protocol.rtps_app_id +
         qos->protocol.rtps_instance_id) != 0))
    {
        return NULL;
    }

    OSAPI_Mutex_take(self->factory_lock);

    if (!self->immutable_qos_enabled)
    {
        REDA_CircularList_init(&self->participants);
        p_prop.buffer_size = sizeof(struct DDS_DomainParticipantEntry);
        p_prop.max_buffers = self->qos.resource_limits.max_participants;
        self->participant_pool = REDA_BufferPool_new(
                                            &p_prop,NULL,NULL,NULL,NULL);
        if (self->participant_pool == NULL)
        {
            OSAPI_Mutex_give(self->factory_lock);
            return NULL;
        }
        self->immutable_qos_enabled = DDS_BOOLEAN_TRUE;
    }

    participant = (struct DDS_DomainParticipantEntry*)
                             REDA_BufferPool_get_buffer(self->participant_pool);
    if (participant == NULL)
    {
        OSAPI_Mutex_give(self->factory_lock);
        return NULL;
    }

    dp_cfg.registry = self->registry;
    dp_cfg.db = NULL;
    dp_cfg.instance = DDS_g_DomainFactory.instance_counter++;

    /* Generate GUID for the participant
     */
    if ((qos->protocol.rtps_host_id == DDS_RTPS_AUTO_ID) &&
        (qos->protocol.rtps_app_id == DDS_RTPS_AUTO_ID) &&
        (qos->protocol.rtps_instance_id == DDS_RTPS_AUTO_ID))
    {
        struct OSAPI_SystemUUID uuid;
        OSAPI_System_generate_uuid(&uuid);
        OSAPI_Memory_copy(&dp_cfg.guid, &uuid.value, 12);
    }
    else
    {
        guid.prefix.hostId = qos->protocol.rtps_host_id;
        guid.prefix.appId = qos->protocol.rtps_app_id;
        guid.prefix.instanceId = qos->protocol.rtps_instance_id;
        /* Set by the participant itself */
        guid.objectId = 0;
        DDS_GUID_from_rtps(&dp_cfg.guid,&guid);
    }

    if (!DDS_DomainParticipant_initialize(&participant->participant,
                self,domain_id,qos,listener,mask,&dp_cfg,self->factory_lock))
    {
        REDA_BufferPool_return_buffer(self->participant_pool,participant);
        goto done;
    }

    if (self->qos.entity_factory.autoenable_created_entities)
    {
        ddsrc = DDS_Entity_enable(&participant->participant.as_entity);
        if (ddsrc != DDS_RETCODE_OK)
        {
            DDSC_Log_error_p1(LOG_FAIL_ENABLE_PARTICIPANT, ddsrc);
            goto done;
        }
    }

    REDA_CircularList_link_node_after(&self->participants,&participant->node);

    retval = &participant->participant;

done:
    OSAPI_Mutex_give(self->factory_lock);

    return retval;
}

DDS_ReturnCode_t
DDS_DomainParticipantFactory_delete_participant(
                                    DDS_DomainParticipantFactory *self,
                                    DDS_DomainParticipant *participant)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    struct DDS_DomainParticipantEntry *p_entry;
    struct REDA_CircularListNode *a_node;

    if ((self == NULL) || (participant == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    retcode = DDS_DomainParticipant_finalize(participant);
    if (retcode == DDS_RETCODE_OK)
    {
        OSAPI_Mutex_take(self->factory_lock);
        a_node = (struct REDA_CircularListNode *)participant - 1;
        p_entry = (struct DDS_DomainParticipantEntry*)a_node;
        REDA_CircularList_unlink_node(&p_entry->node);
        REDA_BufferPool_return_buffer(self->participant_pool,p_entry);
        OSAPI_Mutex_give(self->factory_lock);
    }

    return retcode;
}

/*******************************************************************************
 *                              OPTIONAL APIs
 ******************************************************************************/
#if INCLUDE_API_LOOKUP
DDS_DomainParticipant*
DDS_DomainParticipantFactory_lookup_participant(
                            DDS_DomainParticipantFactory *self,
                            DDS_DomainId_t domain_id)
{
    DDS_DomainParticipant *retval = NULL;
    struct DDS_DomainParticipantEntry *p_entry;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    OSAPI_Mutex_take(self->factory_lock);
    p_entry = (struct DDS_DomainParticipantEntry *)
                              REDA_CircularList_get_first(&self->participants);
    while (p_entry != (struct DDS_DomainParticipantEntry *)&self->participants)
    {
        if (DDS_DomainParticipant_get_domain_id(&p_entry->participant)
                == domain_id)
        {
            retval = &p_entry->participant;
            break;
        }
        p_entry = (struct DDS_DomainParticipantEntry *)
                                REDA_CircularListNode_get_next(&p_entry->node);
    }
    OSAPI_Mutex_give(self->factory_lock);

    return retval;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipantFactory_set_default_participant_qos(
                                DDS_DomainParticipantFactory *self,
                                const struct DDS_DomainParticipantQos *qos)
{
    DDS_ReturnCode_t retcode;

    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->factory_lock);

    retcode = DDS_DomainParticipantQos_copy(&self->default_participant_qos,qos);

    OSAPI_Mutex_give(self->factory_lock);

    return retcode;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipantFactory_get_default_participant_qos(
                            DDS_DomainParticipantFactory *self,
                            struct DDS_DomainParticipantQos * qos)
{
    DDS_ReturnCode_t retcode;

    if ((self == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Mutex_take(self->factory_lock);

    retcode = DDS_DomainParticipantQos_copy(qos,&self->default_participant_qos);

    OSAPI_Mutex_give(self->factory_lock);

    return retcode;
}
#endif
