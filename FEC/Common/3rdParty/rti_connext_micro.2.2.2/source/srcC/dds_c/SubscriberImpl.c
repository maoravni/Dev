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

#include "DataReaderImpl.pkg.h"

#include "DataReader.pkg.h"
#include "SubscriberEvent.pkg.h"
#include "SubscriberQos.pkg.h"
#include "DataReaderQos.pkg.h"
#include "DataReaderEvent.pkg.h"

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

/*i \dref_Subscriber_Properties_INITIALIZER
 */

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

/*** SOURCE_BEGIN ***/

RTI_INT32
DDS_SubscriberImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                               const DB_Record_T op1, void *op2, void *op3)
{
    struct DDS_SubscriberImpl *record_left = (struct DDS_SubscriberImpl*)op1;
    const DDS_Long *id_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        id_right = (const DDS_Long*)op2;
    }
    else
    {
        id_right = &((struct DDS_SubscriberImpl*)op2)->as_entity.entity_id;
    }

    return ((record_left->as_entity.entity_id == *id_right) ? 0 :
                 (record_left->as_entity.entity_id > *id_right ? 1 : -1));
}

DDS_Boolean
DDS_SubscriberImpl_is_hidden(struct DDS_SubscriberImpl * self)
{
    return self->qos.management.is_hidden;
}

DDS_Boolean
DDS_SubscriberImpl_finalize(struct DDS_SubscriberImpl * self)
{

    if (self->dr_count != 0)
    {
        DDSC_Log_error_p1(LOG_SUBSCRIBER_OWNS_DATAREADERS, self->dr_count);
        return DDS_BOOLEAN_FALSE;
    }

    DDS_DataReaderQos_finalize(&self->default_qos);
    DDS_SubscriberQos_finalize(&self->qos);

    return DDS_BOOLEAN_TRUE;
}

DDS_InstanceHandle_t
DDS_SubscriberImpl_get_instance_handle(DDS_Entity *entity)
{
    struct DDS_SubscriberImpl *sub = (struct DDS_SubscriberImpl*)entity;
    DDS_InstanceHandle_t retval;

    retval = sub->config.get_parent_handle((DDS_Entity*)sub->participant);
    *((DDS_Long*)&retval.octet[12]) = NETIO_htonl(sub->as_entity.entity_id);

    return retval;
}

DDS_Boolean
DDS_SubscriberImpl_initialize(struct DDS_SubscriberImpl *subscriber,
                              DDS_DomainParticipant *participant,
                              const struct DDS_SubscriberQos *qos,
                              const struct DDS_SubscriberListener *listener,
                              DDS_StatusMask mask,
                              DDS_Long object_id,
                              struct NDDS_SubscriberConfig *config)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;

#if !(INCLUDE_API_QOS)
    struct DDS_SubscriberQos DEFAULT_SUB_QOS = DDS_SubscriberQos_INITIALIZER;
    struct DDS_DataReaderQos DEFAULT_DR_QOS = DDS_DataReaderQos_INITIALIZER;
#endif

    if ((qos != &DDS_SUBSCRIBER_QOS_DEFAULT)
        && !DDS_SubscriberQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_SUBSCRIBER_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    /* Do we allow a listener to be NULL if the mask is set ? */
    if ((listener == NULL) && (mask != DDS_STATUS_MASK_NONE))
    {
        DDSC_Log_error(LOG_INCONSISTENT_LISTENER_AND_STATUS_MASK);
        return DDS_BOOLEAN_FALSE;
    }

#if INCLUDE_API_QOS
    DDS_DataReaderQos_initialize(&subscriber->default_qos);
#else
    subscriber->default_qos = DEFAULT_DR_QOS;
#endif

    /* Initialize the Subscriber */
    if (qos == &DDS_SUBSCRIBER_QOS_DEFAULT)
    {
#if INCLUDE_API_QOS
        /* TODO: How to get the default qos without the parent ? */
        DDS_DomainParticipant_get_default_subscriber_qos(participant,
                                                         &subscriber->qos);
#else
        subscriber->qos = DEFAULT_QOS;
#endif
    }
    else
    {
        subscriber->qos = *qos;
    }

    DDS_EntityImpl_initialize(&subscriber->as_entity,
                          DDS_SUBSCRIBER_ENTITY_KIND,
                          object_id,
                          DDS_Subscriber_enable,
                          DDS_SubscriberImpl_get_instance_handle);

    subscriber->config = *config;
    subscriber->dr_count = 0;
    subscriber->participant = participant;

    retcode = DDS_Subscriber_set_listener(subscriber, listener, mask);
    if (retcode != DDS_RETCODE_OK)
    {
        DDSC_Log_error_p1(LOG_FAIL_SET_LISTENER, retcode);
        goto done;
    }

    retval = DDS_BOOLEAN_TRUE;

done:

    if (!retval)
    {
        DDS_SubscriberImpl_finalize(subscriber);
    }

    return retval;
}

DDS_Boolean
DDS_SubscriberImpl_listener_is_consistent(const struct DDS_SubscriberListener *self,
                                          DDS_StatusMask mask)
{
    const struct DDS_DataReaderListener *dr_listener = NULL;

    dr_listener = &self->as_datareaderlistener;

    if ((mask & DDS_DATA_ON_READERS_STATUS) && self->on_data_on_readers == NULL)
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_DataReader_listener_is_consistent(dr_listener, mask);
}

