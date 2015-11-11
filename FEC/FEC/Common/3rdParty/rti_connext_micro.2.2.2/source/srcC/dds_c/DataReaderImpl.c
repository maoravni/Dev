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


#include "QosPolicy.pkg.h"
#include "Entity.pkg.h"
#include "TopicDescription.pkg.h"
#include "Topic.pkg.h"
#include "Type.pkg.h"
#include "DataReaderQos.pkg.h"
#include "DataReaderEvent.pkg.h"
#include "DataReaderInterface.pkg.h"
#include "DataReader.pkg.h"


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

/*** SOURCE_BEGIN ***/

RTI_BOOL
DDS_DataReader_cdr_initialize(void *init_config, void *buffer)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)init_config;
    struct RTI_DataReaderSample *sample = (struct RTI_DataReaderSample*)buffer;

    if (!dr->type_plugin->create_sample(dr->type_plugin,
                                        &sample->hst_sample._user_data,
                                        dr->qos.type_support.plugin_data))
    {
        return RTI_FALSE;
    }
#if 0
    OSAPI_Heap_allocateStructure(&sample->hst_sample._info,
                                struct DDS_SampleInfo);
#endif

    return RTI_TRUE;
}

RTI_BOOL
DDS_DataReader_cdr_finalize(void *finalize_config, void *buffer)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)finalize_config;
    struct RTI_DataReaderSample *sample = (struct RTI_DataReaderSample*)buffer;

    if (!dr->type_plugin->delete_sample(dr->type_plugin,
                                        sample->hst_sample._user_data,
                                        dr->qos.type_support.plugin_data))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_INT32
DDS_DataReaderImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                           const DB_Record_T op1, void *op2, void *op3)
{
    struct DDS_DataReaderImpl *record_left = (struct DDS_DataReaderImpl*)op1;
    const DDS_Long *id_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        id_right = (const DDS_Long*)op2;
    }
    else
    {
        id_right = &((struct DDS_DataReaderImpl*)op2)->as_entity.entity_id;
    }

    return ((record_left->as_entity.entity_id == *id_right) ? 0 :
                 (record_left->as_entity.entity_id > *id_right ? 1 : -1));
}

DDS_Long
DDS_DataReader_get_objectid(DDS_DataReader * self)
{
    return self->object_id;
}

DDS_Boolean
DDS_DataReader_listener_is_consistent(const struct DDS_DataReaderListener *self,
                                      DDS_StatusMask mask)
{
#define check_listener(_l,_cb) \
if ((mask & _l) && (self->_cb == NULL)) {\
return DDS_BOOLEAN_FALSE;\
}
#ifdef ENABLE_STATUS_LISTENER
    check_listener(DDS_REQUESTED_DEADLINE_MISSED_STATUS,
                   on_requested_deadline_missed);

    check_listener(DDS_SAMPLE_LOST_STATUS, on_sample_lost);

    check_listener(DDS_SAMPLE_REJECTED_STATUS, on_sample_rejected);

    check_listener(DDS_LIVELINESS_CHANGED_STATUS, on_liveliness_changed);
#endif

    check_listener(DDS_DATA_AVAILABLE_STATUS, on_data_available);

    check_listener(DDS_SUBSCRIPTION_MATCHED_STATUS, on_subscription_matched);

    check_listener(DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS,
                   on_requested_incompatible_qos);


    return DDS_BOOLEAN_TRUE;

#undef check_listener
}

DDS_Boolean
DDS_DataReaderImpl_finalize(DDS_DataReader *self)
{
    struct DDS_DataReaderImpl *datareader = (struct DDS_DataReaderImpl *)self;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;
    struct RT_ComponentFactory *factory;
    NETIO_Interface_T *ext_intf;
    struct NETIO_Address from_address;
    NETIO_Interface_T *drio;
    DB_Cursor_T cursor = NULL;
    DB_ReturnCode_T dbrc;
    struct DataReaderBindEntry *drio_be;
    DDS_Long i,j;
    struct DDS_LocatorSeq *dr_locator_seq;
    struct DDS_LocatorSeq *dr_mc_locator_seq = NULL;
    NETIO_RouteKind_T bind_kind;

    LOG_printDebug("delete reader for topic: %s\n",DDS_TopicDescription_get_name(DDS_Topic_as_topicdescription(datareader->topic)));
    if (datareader->as_entity.entity_id & 0xc0)
    {
        dr_locator_seq = datareader->config.default_meta_unicast;
        dr_mc_locator_seq = datareader->config.default_meta_multicast;
        bind_kind = NETIO_ROUTEKIND_META;
    }
    else
    {
        dr_locator_seq = datareader->config.default_unicast;
        dr_mc_locator_seq = datareader->config.default_multicast;
        bind_kind = NETIO_ROUTEKIND_USER;
    }

#ifdef ENABLE_QOS_DEADLINE
    if (!DDS_Duration_is_infinite(&datareader->qos.deadline.period))
    {
        if (!OSAPI_Timer_delete_timeout(datareader->config.timer,
                        &datareader->deadline_event))
        {
            DDSC_Log_error_p1(LOG_FAIL_POST_DEADLINE_DATAREADER,
                               datareader->as_entity.entity_id);
            goto done;
        }
    }
#endif

    drio = datareader->dr_intf;
    if (drio != NULL)
    {
        cursor = NULL;
        dbrc = DB_Table_select_all(drio->_btable,DB_TABLE_DEFAULT_INDEX,&cursor);
        do
        {
            dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&drio_be);
            if (dbrc == DB_RETCODE_OK)
            {
                if (!NETIO_Interface_get_external_interface(
                        datareader->dr_intf,&drio_be->_parent.source,
                        &ext_intf,&from_address))
                {
                }
                NETIO_Interface_unbind_external(
                        datareader->rtps_intf,&drio_be->_parent.source,
                        ext_intf,&drio_be->_parent.source,NULL);

                NETIO_Interface_unbind(datareader->rtps_intf,
                        &drio_be->_parent.source,
                        datareader->rtps_intf,NULL);

                if (!NETIO_Interface_get_external_interface(
                        datareader->rtps_intf,&drio_be->_parent.source,
                        &ext_intf,&drio_be->_parent.source))
                {
                }

                j = DDS_LocatorSeq_get_length(dr_locator_seq);
                for (i = 0; i < j; i++)
                {
                    from_address = *(struct NETIO_Address*)
                              DDS_LocatorSeq_get_reference(dr_locator_seq,i);

                    NETIO_BindResolver_unbind(datareader->config.bind_resolver,
                            (struct REDA_StringSeq*)datareader->config.enabled_transports,
                            bind_kind,
                            &from_address,
                            ext_intf,
                            &drio_be->_parent.source);
                }

                j = DDS_LocatorSeq_get_length(dr_mc_locator_seq);
                for (i = 0; i < j; i++)
                {
                    from_address = *(struct NETIO_Address*)
                                              DDS_LocatorSeq_get_reference(dr_mc_locator_seq,i);
                    NETIO_BindResolver_unbind(datareader->config.bind_resolver,
                            (struct REDA_StringSeq*)datareader->config.enabled_transports,
                            bind_kind,
                            &from_address,
                            ext_intf,
                            &drio_be->_parent.source);
                }
            }
        } while (dbrc == DB_RETCODE_OK);
        if (cursor)
        {
            DB_Cursor_finish(drio->_btable,cursor);
        }
    }

    if (!DDS_Duration_is_infinite(&datareader->qos.deadline.period))
    {
        OSAPI_Timer_delete_timeout(datareader->config.timer,
                                  &datareader->deadline_event);
    }

    if (datareader->md5_stream)
    {
        CDR_Stream_Free(datareader->md5_stream);
    }

    if (self->_rh != NULL)
    {
        factory = RT_Registry_lookup(datareader->config.registry,"rh");
        if (factory == NULL)
        {
            DDSC_Log_error(LOG_NO_READER_HISTORY_FACTORY);
            goto done;
        }
        DDSHST_ReaderFactory_delete_component(factory, self->_rh);
        self->_rh = NULL;
    }

    if (datareader->dr_intf != NULL)
    {
        factory = RT_Registry_lookup(datareader->config.registry,"ri");
        if (factory == NULL)
        {
            DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
            goto done;
        }
        NETIO_InterfaceFactory_delete_component(factory,datareader->dr_intf);
        datareader->dr_intf = NULL;
    }

    if (datareader->rtps_intf != NULL)
    {
        factory = RT_Registry_lookup(datareader->config.registry,"rtps");
        if (factory == NULL)
        {
            DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
            goto done;
        }

        NETIO_InterfaceFactory_delete_component(factory,datareader->rtps_intf);
        datareader->rtps_intf = NULL;
    }

    if ((datareader->cdr_samples != NULL)
         && !REDA_BufferPool_delete(datareader->cdr_samples))
    {
        goto done;
    }

    DDS_DataReaderQos_finalize(&datareader->qos);

    retval = DDS_BOOLEAN_TRUE;

done:

    return retval;
}

DDS_InstanceHandle_t
DDS_DataReaderImpl_get_instance_handle(DDS_Entity *entity)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl*)entity;
    DDS_InstanceHandle_t retval;

    retval = dr->config.get_parent_handle((DDS_Entity*)dr->subscriber);
    *((DDS_Long*)&retval.octet[12]) = NETIO_htonl(dr->as_entity.entity_id);

    return retval;
}

DDS_Boolean
DDS_DataReaderImpl_initialize(struct DDS_DataReaderImpl *datareader,
                       DDS_Subscriber * subscriber,
                       DDS_TopicDescription * topic_description,
                       const struct DDS_DataReaderQos *qos,
                       const struct DDS_DataReaderListener *listener,
                       DDS_StatusMask mask,
                       DDS_Long object_id,
                       struct NDDS_DataReaderConfig *config)
{
#if !(INCLUDE_API_QOS)
    struct DDS_DataReaderQos init_val = DDS_DataReaderQos_INITIALIZER;
#endif
    DDS_ReturnCode_t retcode;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;
    DDS_Topic *topic;
    struct DDS_DataReaderInterfaceProperty drintf_property =
                                DDS_DataReaderInterfaceProperty_INITIALIZER;
    struct DDSHST_ReaderProperty rh_property =
                                        DDSHST_ReaderProperty_INITIALIZER;
    struct DDSHST_ReaderListener rh_listener =
                                        DDSHST_ReaderListener_INITIALIZE;
    DDS_InstanceHandle_t instance_handle;
    struct RT_ComponentFactory *factory;
    struct REDA_BufferPoolProperty pool_property =
                                            REDA_BufferPoolProperty_INITIALIZER;
    struct RTPSInterfaceProperty rtps_property = RTPSInterfaceProperty_INITIALIZER;

    OSAPI_Memory_zero(datareader,sizeof(struct DDS_DataReaderImpl));

    if ((qos != &DDS_DATAREADER_QOS_DEFAULT) &&
        !DDS_DataReaderQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DATAREADER_QOS);
        goto done;
    }

    /* Do we allow a listener to be NULL if the mask is set ? */
    if ((listener == NULL) && (mask != DDS_STATUS_MASK_NONE))
    {
        DDSC_Log_error(LOG_INCONSISTENT_LISTENER_AND_STATUS_MASK);
        goto done;
    }

    /* Initialize QoS */
    DDS_DataReaderQos_initialize(&datareader->qos);

    /* Initialize the DataReader */
    if (qos == &DDS_DATAREADER_QOS_DEFAULT)
    {
#if INCLUDE_API_QOS
        DDS_Subscriber_get_default_datareader_qos(subscriber, &datareader->qos);
#else
        datareader->qos = init_val;
#endif
    }
    else
    {
        DDS_DataReaderQos_copy(&datareader->qos, qos);
    }

    if (datareader->qos.protocol.rtps_object_id != DDS_RTPS_AUTO_ID)
    {
        object_id = datareader->qos.protocol.rtps_object_id;
    }

    /* pool for loaned sample and info sequence buffers */
    if (datareader->qos.history.kind == DDS_KEEP_ALL_HISTORY_QOS)
    {
        /* NOTE: assumes finite max_samples */
        datareader->read_seq_max = 
            datareader->qos.resource_limits.max_samples;
    }
    else /* KEEP_LAST */
    {
        /* NOTE: assumes finite max_instances */
        datareader->read_seq_max = 
            (datareader->qos.resource_limits.max_instances * 
             datareader->qos.history.depth);
    }

    topic = DDS_Topic_narrow(topic_description);
    if (topic == NULL)
    {
        DDSC_Log_error(LOG_FAIL_NARROW_TOPIC_DESCRIPTION_TO_TOPIC);
        goto done;
    }

    datareader->topic = topic;
    datareader->type_plugin =
                DDS_TypeImpl_get_plugin(DDS_Topic_get_type(datareader->topic));
    datareader->key_kind = datareader->type_plugin->get_key_kind(
                                 datareader->type_plugin,
                                 datareader->qos.type_support.plugin_data);
    if (datareader->key_kind == NDDS_TYPEPLUGIN_USER_KEY)
    {
        object_id = (object_id << 8) | MIG_RTPS_OBJECT_NORMAL_USER_CST_READER;
    }
    else if (datareader->key_kind == NDDS_TYPEPLUGIN_NO_KEY)
    {
        object_id = (object_id << 8) | MIG_RTPS_OBJECT_NORMAL_USER_SUBSCRIPTION;
    }

    DDS_EntityImpl_initialize(&datareader->as_entity,
            DDS_DATAREADER_ENTITY_KIND,
            object_id,
            DDS_DataReader_enable,
            DDS_DataReaderImpl_get_instance_handle);

	datareader->config = *config;
    datareader->subscriber = subscriber;
	
	retcode = DDS_DataReader_set_listener(datareader, listener, mask);
    if (retcode != DDS_RETCODE_OK)
    {
        DDSC_Log_error_p1(LOG_FAIL_SET_LISTENER, retcode);
        goto done;
    }

    datareader->md5_stream = NULL;

    OSAPI_NtpTime_setZero(&datareader->last_sample_recvd_time);

    pool_property.buffer_size = sizeof(struct RTI_DataReaderSample);
    pool_property.max_buffers = datareader->qos.resource_limits.max_samples;
    if (pool_property.max_buffers > 0)
    {
        pool_property.max_buffers++; /* for keep last purposes */
    }
    pool_property.flags |= REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC;
    datareader->cdr_samples = REDA_BufferPool_new(&pool_property,
                                    DDS_DataReader_cdr_initialize,
                                    datareader,
                                    DDS_DataReader_cdr_finalize,
                                    datareader);

    if (datareader->cdr_samples == NULL)
    {
        goto done;
    }

    rh_listener.listener_data = (void *)datareader;
    rh_listener.on_sample_removed = DDS_DataReaderEvent_on_sample_removed;
    rh_listener.on_data_available = DDS_DataReaderEvent_on_data_available;
    rh_listener.on_deadline_missed = DDS_DataReaderEvent_on_deadline_missed;
    rh_listener.on_sample_rejected = DDS_DataReaderEvent_on_hst_sample_rejected;
    rh_listener.on_sample_lost = DDS_DataReaderEvent_on_hst_sample_lost;

    factory = RT_Registry_lookup(datareader->config.registry,"rh");
    if (factory == NULL)
    {
        DDSC_Log_error(LOG_NO_READER_HISTORY_FACTORY);
        goto done;
    }

    rh_property.qos = &datareader->qos;
    rh_property.timer = datareader->config.timer;

    datareader->_rh = DDSHST_ReaderFactory_create_component(factory,
                                                        &rh_property._parent,
                                                        &rh_listener._parent);
    if (datareader->_rh == NULL)
    {
        DDSC_Log_error(LOG_FAIL_CREATE_READER_HISTORY);
        goto done;
    }

    /* DataWriter Interface */
    factory = RT_Registry_lookup(datareader->config.registry,"ri");
    if (factory == NULL)
    {
        DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
        goto done;
    }

    drintf_property._parent._parent.db = datareader->config.db;
    /* TODO:
     * Need a resource limit to specify the maximum number of destinations
     * for this writer. A destination is the {Destination GUID,Locator} tuple
     */
    drintf_property._parent.max_routes = datareader->qos.reader_resource_limits.max_remote_writers*2;
    drintf_property.datareader = datareader;
    instance_handle = DDS_DataReaderImpl_get_instance_handle(
                                                    (DDS_Entity*)datareader);

    NETIO_Address_set_guid(&drintf_property.intf_address,
                          0,instance_handle.octet);

    datareader->dr_intf = NETIO_InterfaceFactory_create_component(factory,
                                        &drintf_property._parent._parent,NULL);
    if (datareader->dr_intf == NULL)
    {
        DDSC_Log_error(LOG_FAIL_CREATE_WRITER_HISTORY);
        goto done;
    }

    factory = RT_Registry_lookup(
            datareader->config.registry,"rtps");
    if (factory == NULL)
    {
        DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
        goto done;
    }

    rtps_property.intf_address = drintf_property.intf_address;
    rtps_property._parent._parent.db = datareader->config.db;
    rtps_property.anonymous = datareader->qos.management.is_anonymous;
    rtps_property._parent.max_routes = 32;
    rtps_property.mode = RTPS_INTERFACEMODE_READER;
    rtps_property.reliable = (datareader->qos.reliability.kind == DDS_RELIABLE_RELIABILITY_QOS);

    datareader->rtps_intf = NETIO_InterfaceFactory_create_component(factory,
                                    &rtps_property._parent._parent,NULL);
    if (datareader->rtps_intf == NULL)
    {
        DDSC_Log_error(LOG_FAIL_CREATE_WRITER_HISTORY);
        goto done;
    }

    if (datareader->key_kind != NDDS_TYPEPLUGIN_NO_KEY)
    {
        datareader->md5_stream = CDR_Stream_Alloc(100);
        if (datareader->md5_stream == NULL)
        {
            DDSC_Log_error(LOG_FAIL_ALLOCATE);
            goto done;
        }
#ifdef RTI_ENDIAN_LITTLE
        if (!CDR_Stream_ByteswapSet(datareader->md5_stream, RTI_TRUE))
#else
        if (!CDR_Stream_ByteswapSet(datareader->md5_stream, RTI_FALSE))
#endif
        {
            DDSC_Log_error(LOG_FAIL_SET_BYTE_SWAPPING_OF_STREAM);
            goto done;
        }
    }

    datareader->as_entity.state = RTIDDS_ENTITY_STATE_CREATED;

    retval = DDS_BOOLEAN_TRUE;

done:

    if (!retval)
    {
        (void)DDS_DataReaderImpl_finalize(datareader);
    }

    return retval;
}

DDS_Boolean
DDS_DataReader_is_enabled(DDS_DataReader * self)
{
    return (self->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED);
}
