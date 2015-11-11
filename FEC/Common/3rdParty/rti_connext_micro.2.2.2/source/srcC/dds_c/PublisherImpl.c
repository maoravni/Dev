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
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "Entity.pkg.h"
#include "PublisherQos.pkg.h"

#include "DataWriterImpl.pkg.h"

#include "DataWriterQos.pkg.h"

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

/*i \dref_Publisher_Properties_INITIALIZER
 */

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

/*** SOURCE_BEGIN ***/

RTI_INT32
DDS_PublisherImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                          const DB_Record_T op1, void *op2, void *op3)
{
    struct DDS_PublisherImpl *record_left = (struct DDS_PublisherImpl*)op1;
    const DDS_Long *id_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        id_right = (const DDS_Long*)op2;
    }
    else
    {
        id_right = &((struct DDS_PublisherImpl*)op2)->as_entity.entity_id;
    }

    return ((record_left->as_entity.entity_id == *id_right) ? 0 :
                 (record_left->as_entity.entity_id > *id_right ? 1 : -1));
}

DDS_Boolean
DDS_PublisherImpl_is_hidden(DDS_Publisher *self)
{
    return self->qos.management.is_hidden;
}

DDS_Boolean
DDS_PublisherImpl_finalize(struct DDS_PublisherImpl *self)
{
    if (self->dw_count != 0)
    {
        DDSC_Log_error_p1(LOG_PUBLISHER_OWNS_DATAWRITERS, self->dw_count);
        return DDS_BOOLEAN_FALSE;
    }

    DDS_DataWriterQos_finalize(&self->default_qos);
    DDS_PublisherQos_finalize(&self->qos);

    /*TODO: When an object-id generation method is determined, release it */

    return DDS_BOOLEAN_TRUE;
}

DDS_InstanceHandle_t
DDS_PublisherImpl_get_instance_handle(DDS_Entity *entity)
{
    struct DDS_PublisherImpl *pub = (struct DDS_PublisherImpl*)entity;
    DDS_InstanceHandle_t retval;

    retval = pub->config.get_parent_handle((DDS_Entity*)pub->participant);
    *((DDS_Long*)&retval.octet[12]) = NETIO_htonl(pub->as_entity.entity_id);

    return retval;
}

DDS_Boolean
DDS_PublisherImpl_initialize(struct DDS_PublisherImpl *publisher,
                             DDS_DomainParticipant *participant,
                             const struct DDS_PublisherQos *qos,
                             const struct DDS_PublisherListener *listener,
                             DDS_StatusMask mask,
                             DDS_Long object_id,
                             struct NDDS_PublisherConfig *config)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;

#if !(INCLUDE_API_QOS)
    struct DDS_PublisherQos DEFAULT_PUB_QOS = DDS_PublisherQos_INITIALIZER;
    struct DDS_DataWriterQos DEFAULT_DW_QOS = DDS_DataWriterQos_INITIALIZER;
#endif

    if ((qos != &DDS_PUBLISHER_QOS_DEFAULT) &&
        !DDS_PublisherQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_PUBLISHER_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    /* Do we allow a listener to be NULL if the mask is set ? */
    if ((listener == NULL) && (mask != DDS_STATUS_MASK_NONE))
    {
        DDSC_Log_error(LOG_INCONSISTENT_LISTENER_AND_STATUS_MASK);
        return DDS_BOOLEAN_FALSE;
    }

#if INCLUDE_API_QOS
    DDS_DataWriterQos_initialize(&publisher->default_qos);
#else
    publisher->default_qos = DEFAULT_DW_QOS;
#endif

    /* Initialize the Publisher */
    if (qos == &DDS_PUBLISHER_QOS_DEFAULT)
    {
#if INCLUDE_API_QOS
        DDS_DomainParticipant_get_default_publisher_qos(participant,
                                                        &publisher->qos);
#else
        /* must be DDS-default QoS */
        publisher->qos = DEFAULT_PUB_QOS;
#endif
    }
    else
    {
        publisher->qos = *qos;
    }

    DDS_EntityImpl_initialize(&publisher->as_entity,
                          DDS_PUBLISHER_ENTITY_KIND,
                          object_id,
                          DDS_Publisher_enable,
                          DDS_PublisherImpl_get_instance_handle);

    publisher->config = *config;
    publisher->dw_count = 0;
    publisher->participant = participant;

    retcode = DDS_Publisher_set_listener(publisher, listener, mask);
    if (retcode != DDS_RETCODE_OK)
    {
        DDSC_Log_error_p1(LOG_FAIL_SET_LISTENER, retcode);
        goto done;
    }

    retval = DDS_BOOLEAN_TRUE;

done:

    if (!retval)
    {
        DDS_PublisherImpl_finalize(publisher);
    }

    return retval;
}

DDS_Boolean
DDS_PublisherImpl_listener_is_consistent(const struct DDS_PublisherListener *self,
                                         DDS_StatusMask mask)
{
    const struct DDS_DataWriterListener *dw_listener = NULL;

    dw_listener = &self->as_datawriterlistener;

    return DDS_DataWriter_listener_is_consistent(dw_listener, mask);
}
