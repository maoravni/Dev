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
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_route_h
#include "netio/netio_route.h"
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
#include "DataWriterQos.pkg.h"
#include "DataWriterEvent.pkg.h"
#include "DataWriterInterface.pkg.h"

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

/*** SOURCE_BEGIN ***/

RTI_INT32
DDS_DataWriterImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                           const DB_Record_T op1, void *op2, void *op3)
{
    struct DDS_DataWriterImpl *record_left = (struct DDS_DataWriterImpl*)op1;
    const DDS_Long *id_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        id_right = (const DDS_Long*)op2;
    }
    else
    {
        id_right = &((struct DDS_DataWriterImpl*)op2)->as_entity.entity_id;
    }

    return ((record_left->as_entity.entity_id == *id_right) ? 0 :
                 (record_left->as_entity.entity_id > *id_right ? 1 : -1));
}

DDS_Long
DDS_DataWriter_get_objectid(DDS_DataWriter * self)
{
    return self->as_entity.entity_id;
}

DDS_Boolean
DDS_DataWriter_listener_is_consistent(const struct DDS_DataWriterListener *self,
                                      DDS_StatusMask mask)
{
#define check_listener(_l,_cb) \
if ((mask & _l) && (self->_cb == NULL)) {\
return DDS_BOOLEAN_FALSE;\
}

#ifdef ENABLE_STATUS_LISTENER
    check_listener(DDS_LIVELINESS_LOST_STATUS, on_liveliness_lost);

    check_listener(DDS_OFFERED_DEADLINE_MISSED_STATUS,
                   on_offered_deadline_missed);
#endif

    check_listener(DDS_OFFERED_INCOMPATIBLE_QOS_STATUS,
                   on_offered_incompatible_qos);
    check_listener(DDS_PUBLICATION_MATCHED_STATUS, on_publication_matched);

    return DDS_BOOLEAN_TRUE;

#undef check_listener
}

RTI_BOOL
DDS_DataWriter_cdr_initialize(void *init_config, void *buffer)
{
    struct RTI_TransformCDR_Sample *sample =
                                     (struct RTI_TransformCDR_Sample *)buffer;

    sample->payload = (char*)sample +
                       sizeof(struct RTI_TransformCDR_Sample);

    return RTI_TRUE;
}

RTI_BOOL
DDS_DataWriter_cdr_finalize(void *finalize_config, void *buffer)
{
    return RTI_TRUE;
}

DDS_Boolean
DDS_DataWriterImpl_finalize(DDS_DataWriter *self)
{
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;
    struct RT_ComponentFactory *factory;
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;
    NETIO_Interface_T *ext_intf;
    struct NETIO_Address from_address;
    DDS_Long i,j,k;
    NETIO_Interface_T *dwio;
    DB_Cursor_T cursor = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIORouteEntry *dwio_re;
    struct DDS_LocatorSeq *dr_locator_seq;
    NETIO_RouteKind_T recv_kind;
    NETIO_Interface_T *netio_intf;

    if (datawriter->as_entity.entity_id & 0xc0)
    {
        dr_locator_seq = datawriter->config.default_meta_unicast;
        recv_kind = NETIO_ROUTEKIND_META;
    }
    else
    {
        dr_locator_seq = datawriter->config.default_unicast;
        recv_kind = NETIO_ROUTEKIND_USER;
    }

    dwio = self->dw_intf;
    if (dwio != NULL)
    {
        cursor = NULL;
        dbrc = DB_Table_select_all(dwio->_rtable,DB_TABLE_DEFAULT_INDEX,&cursor);
        do
        {
            dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&dwio_re);
            if (dbrc == DB_RETCODE_OK)
            {
                if (!NETIO_Interface_get_external_interface(
                        datawriter->dw_intf,&dwio_re->destination,
                        &ext_intf,&from_address))
                {
                }
                NETIO_Interface_unbind_external(
                        datawriter->rtps_intf,&dwio_re->destination,
                        ext_intf,&dwio_re->destination,NULL);

                NETIO_Interface_unbind(datawriter->rtps_intf, &dwio_re->destination,
                        datawriter->rtps_intf,NULL);

                if (!NETIO_Interface_get_external_interface(
                        datawriter->rtps_intf,&dwio_re->destination,&ext_intf,&dwio_re->destination))
                {
                }

                for (k = 0; k < DDS_StringSeq_get_length(datawriter->config.enabled_transports); ++k)
                {
                    j = DDS_LocatorSeq_get_length(dr_locator_seq);
                    for (i = 0; i < j; i++)
                    {
                        RT_ComponentFactoryId_T id;
                        from_address = *(struct NETIO_Address*)
                                      DDS_LocatorSeq_get_reference(dr_locator_seq,i);
                        RT_ComponentFactoryId_clear(&id);
                        RT_ComponentFactoryId_set_name(&id,*DDS_StringSeq_get_reference(datawriter->config.enabled_transports,k));
                        if (!NETIO_BindResolver_lookup_by_address(
                                datawriter->config.bind_resolver,recv_kind,&id,
                                &from_address,&netio_intf))
                        {
                            continue;
                        }
                        if (!NETIO_Interface_unbind_external(
                                netio_intf,&from_address,ext_intf,
                                &dwio_re->destination,NULL))
                        {
                        }
                    }
                }
            }
        } while (dbrc == DB_RETCODE_OK);
        if (cursor)
        {
            DB_Cursor_finish(dwio->_rtable,cursor);
        }
    }

    if (datawriter->wh != NULL)
    {
        factory = RT_Registry_lookup(datawriter->config.registry,"wh");
        if (factory == NULL)
        {
            DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
            goto done;
        }
        DDSHST_WriterFactory_delete_component(factory, datawriter->wh);
        datawriter->wh = NULL;
    }


    if (datawriter->dw_intf != NULL)
    {
        factory = RT_Registry_lookup(datawriter->config.registry,"wi");
        if (factory == NULL)
        {
            DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
            goto done;
        }
        NETIO_InterfaceFactory_delete_component(factory,datawriter->dw_intf);
        datawriter->dw_intf = NULL;
    }

    if (datawriter->rtps_intf != NULL)
    {
        factory = RT_Registry_lookup(datawriter->config.registry,"rtps");
        if (factory == NULL)
        {
            DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
            goto done;
        }

        NETIO_InterfaceFactory_delete_component(factory,datawriter->rtps_intf);
        datawriter->rtps_intf = NULL;
    }

    if (!DDS_Duration_is_infinite(&datawriter->qos.deadline.period))
    {
        OSAPI_Timer_delete_timeout(datawriter->config.timer,
                                  &datawriter->deadline_event);
    }

    if (!DDS_Duration_is_infinite(&datawriter->qos.liveliness.lease_duration))
    {
        OSAPI_Timer_delete_timeout(datawriter->config.timer,
                                  &datawriter->liveliness_event);
    }

    if ((datawriter->cdr_samples != NULL)
         && !REDA_BufferPool_delete(datawriter->cdr_samples))
    {
        goto done;
    }

    if (self->md5Stream != NULL)
    {
        CDR_Stream_Free(self->md5Stream);
    }

    DDS_DataWriterQos_finalize(&self->qos);

    retval = DDS_BOOLEAN_TRUE;

done:

    return retval;
}

DDS_InstanceHandle_t
DDS_DataWriterImpl_get_instance_handle(DDS_Entity *entity)
{
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl*)entity;
    DDS_InstanceHandle_t retval;

    retval = dw->config.get_parent_handle((DDS_Entity*)dw->publisher);
    *((DDS_Long*)&retval.octet[12]) = NETIO_htonl(dw->as_entity.entity_id);

    return retval;
}

DDS_Boolean
DDS_DataWriterImpl_initialize(struct DDS_DataWriterImpl *datawriter,
                              DDS_Publisher *publisher,
                              DDS_Topic *topic,
                              const struct DDS_DataWriterQos *qos,
                              const struct DDS_DataWriterListener *listener,
                              DDS_StatusMask mask,
                              DDS_Long object_id,
                              const struct NDDS_DataWriterConfig *config)
{
#if !(INCLUDE_API_QOS)
    struct DDS_DataWriterQos init_val = DDS_DataWriterQos_INITIALIZER;
#endif
    DDS_ReturnCode_t retcode = DDS_BOOLEAN_FALSE;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;
    struct REDA_BufferPoolProperty cdr_property =
                                            REDA_BufferPoolProperty_INITIALIZER;
    struct DDSHST_WriterProperty wh_property =
                                DDSHST_WriterProperty_INITIALIZER;
    struct DDSHST_WriterListener wh_listener =
                                DDSHST_WriterListener_INITIALIZE;
    struct RT_ComponentFactory *factory;
    struct DDS_DataWriterInterfaceProperty dwintf_property =
                                DDS_DataWriterInterfaceProperty_INITIALIZER;
    DDS_InstanceHandle_t instance_handle;
    struct RTPSInterfaceProperty rtps_property = RTPSInterfaceProperty_INITIALIZER;
    DDS_UnsignedLong serialized_key_size;

    OSAPI_Memory_zero(datawriter,sizeof(struct DDS_DataWriterImpl));

    if ((qos != &DDS_DATAWRITER_QOS_DEFAULT) &&
        !DDS_DataWriterQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DATAWRITER_QOS);
        goto done;
    }

    /* Do we allow a listener to be NULL if the mask is set ? */
    if ((listener == NULL) && (mask != DDS_STATUS_MASK_NONE))
    {
        DDSC_Log_error(LOG_INCONSISTENT_LISTENER_AND_STATUS_MASK);
        goto done;
    }

    /* Initialize QoS */
    DDS_DataWriterQos_initialize(&datawriter->qos);

    if (qos == &DDS_DATAWRITER_QOS_DEFAULT)
    {
#if INCLUDE_API_QOS
        DDS_Publisher_get_default_datawriter_qos(publisher, &datawriter->qos);
#else
        datawriter->qos = init_val;
#endif
    }
    else
    {
        DDS_DataWriterQos_copy(&datawriter->qos, qos);
    }

    if (datawriter->qos.protocol.rtps_object_id != DDS_RTPS_AUTO_ID)
    {
        object_id = datawriter->qos.protocol.rtps_object_id;
    }

    datawriter->topic = topic;
    datawriter->type_plugin = DDS_TypeImpl_get_plugin(
                                    DDS_Topic_get_type(datawriter->topic));

    /*TODO: Needs to be updated with the new type-plugin */
    datawriter->key_kind = datawriter->type_plugin->get_key_kind(
                                datawriter->type_plugin,
                                datawriter->qos.type_support.plugin_data);

    if (datawriter->key_kind == NDDS_TYPEPLUGIN_USER_KEY)
    {
        object_id = (object_id << 8) | MIG_RTPS_OBJECT_NORMAL_USER_CST_WRITER;
    }
    else if (datawriter->key_kind == NDDS_TYPEPLUGIN_NO_KEY)
    {
        object_id = (object_id << 8) | MIG_RTPS_OBJECT_NORMAL_USER_PUBLICATION;
    }

    if (datawriter->key_kind == NDDS_TYPEPLUGIN_USER_KEY)
    {
        serialized_key_size = 
            datawriter->type_plugin->get_serialized_key_max_size(
                datawriter->type_plugin, 0, NULL);

        datawriter->md5Stream = CDR_Stream_Alloc(serialized_key_size);
        if (datawriter->md5Stream == NULL)
        {
            DDSC_Log_error(LOG_FAIL_ALLOCATE);
            goto done;
        }
#ifdef RTI_ENDIAN_LITTLE
        if (!CDR_Stream_ByteswapSet(datawriter->md5Stream, RTI_TRUE))
#else
        if (!CDR_Stream_ByteswapSet(datawriter->md5Stream, RTI_FALSE))
#endif
        {
            DDSC_Log_error(LOG_FAIL_SET_BYTE_SWAPPING_OF_STREAM);
            goto done;
        }
    }

    DDS_EntityImpl_initialize(&datawriter->as_entity,
            DDS_DATAWRITER_ENTITY_KIND,
            object_id,
            DDS_DataWriter_enable,
            DDS_DataWriterImpl_get_instance_handle);

    datawriter->config = *config;
    datawriter->publisher = publisher;

	retcode = DDS_DataWriter_set_listener(datawriter, listener, mask);
    if (retcode != DDS_RETCODE_OK)
    {
        DDSC_Log_error_p1(LOG_FAIL_SET_LISTENER, retcode);
        goto done;
    }
	
	REDA_SequenceNumber_setZero(&datawriter->last_sn);

    datawriter->max_cdr_serialized_length =
            datawriter->type_plugin->get_serialized_sample_max_size(
            		datawriter->type_plugin, 4,
            		datawriter->qos.type_support.plugin_data);
    datawriter->max_packet_length =
                        256 + datawriter->max_cdr_serialized_length + 32 + 128;

    cdr_property.buffer_size = sizeof(struct RTI_TransformCDR_Sample) +
                                    datawriter->max_packet_length;

    cdr_property.max_buffers = datawriter->qos.resource_limits.max_samples;
    cdr_property.flags |= REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC;
    datawriter->cdr_samples = REDA_BufferPool_new(&cdr_property,
                                    DDS_DataWriter_cdr_initialize,
                                    datawriter,
                                    DDS_DataWriter_cdr_finalize,
                                    NULL);

    if (datawriter->cdr_samples == NULL)
    {
        goto done;
    }

#ifdef RTI_ENDIAN_LITTLE
    if (!CDR_Stream_ByteswapSet(&datawriter->stream, RTI_TRUE))
#else
    if (!CDR_Stream_ByteswapSet(&datawriter->stream, RTI_FALSE))
#endif
    {
        DDSC_Log_error(LOG_FAIL_SET_BYTE_SWAPPING_OF_STREAM);
        goto done;
    }

    wh_listener.on_sample_removed = DDS_DataWriterEvent_on_sample_removed;
    wh_listener.on_key_removed = DDS_DataWriterEvent_on_key_removed;
    wh_listener.on_deadline_missed = DDS_DataWriterEvent_on_deadline_missed;
    wh_listener.listener_data = (void *)datawriter;

    factory = RT_Registry_lookup(datawriter->config.registry,"wh");
    if (factory == NULL)
    {
        DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
        goto done;
    }

    wh_property.qos = &datawriter->qos;
    datawriter->wh = DDSHST_WriterFactory_create_component(factory,
                                                       &wh_property._parent,
                                                       &wh_listener._parent);
    if (datawriter->wh == NULL)
    {
        DDSC_Log_error(LOG_FAIL_CREATE_WRITER_HISTORY);
        goto done;
    }

    /* DataWriter Interface */
    factory = RT_Registry_lookup(datawriter->config.registry,"wi");
    if (factory == NULL)
    {
        DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
        goto done;
    }

    dwintf_property._parent._parent.db = datawriter->config.db;

    /* TODO:
     * Need a resource limit to specify the maximum number of destinations
     * for this writer. A destination is the {Destination GUID,Locator} tuple
     */
    dwintf_property._parent.max_routes = 16;
    dwintf_property.datawriter = datawriter;

    instance_handle = DDS_DataWriterImpl_get_instance_handle(
                                                    (DDS_Entity*)datawriter);

    NETIO_Address_set_guid(&dwintf_property.intf_address,
                          0,instance_handle.octet);

    datawriter->dw_intf = NETIO_InterfaceFactory_create_component(factory,
                            &dwintf_property._parent._parent,NULL);
    if (datawriter->dw_intf == NULL)
    {
        DDSC_Log_error(LOG_FAIL_CREATE_WRITER_HISTORY);
        goto done;
    }

    factory = RT_Registry_lookup(datawriter->config.registry,"rtps");
    if (factory == NULL)
    {
        DDSC_Log_error(LOG_NO_WRITER_HISTORY_FACTORY);
        goto done;
    }

    rtps_property.intf_address = dwintf_property.intf_address;
    rtps_property._parent._parent.db = datawriter->config.db;
    rtps_property.anonymous = datawriter->qos.management.is_anonymous;
    /* TODO: ADD RESORUCE LIMIT */
    rtps_property._parent.max_routes = 32;
    rtps_property.mode = RTPS_INTERFACEMODE_WRITER;
    rtps_property.reliable = 
        (datawriter->qos.reliability.kind == DDS_RELIABLE_RELIABILITY_QOS);
    rtps_property.window_size = (RTI_UINT32)
        datawriter->qos.protocol.rtps_reliable_writer.max_send_window;

    DDS_Duration_to_ntp_time(
       &datawriter->qos.protocol.rtps_reliable_writer.heartbeat_period,
       &rtps_property.hb_period);

    if (datawriter->qos.protocol.rtps_reliable_writer.
        heartbeats_per_max_samples == 0)
    {
        rtps_property.samples_per_hb = 0;
    }
    else
    {
        if (datawriter->qos.resource_limits.max_samples == DDS_LENGTH_UNLIMITED)
        {
            rtps_property.samples_per_hb =
                100000000 /
                datawriter->qos.protocol.rtps_reliable_writer.
                heartbeats_per_max_samples;
        }
        else
        {
            rtps_property.samples_per_hb =
                datawriter->qos.resource_limits.max_samples /
                datawriter->qos.protocol.rtps_reliable_writer.
                heartbeats_per_max_samples;
        }
    }

    datawriter->rtps_intf = NETIO_InterfaceFactory_create_component(factory,
                                    &rtps_property._parent._parent,NULL);
    if (datawriter->rtps_intf == NULL)
    {
        DDSC_Log_error(LOG_FAIL_CREATE_WRITER_HISTORY);
        goto done;
    }

    datawriter->as_entity.state = RTIDDS_ENTITY_STATE_CREATED;

    retval = DDS_BOOLEAN_TRUE;

done:

    if (!retval)
    {
        (void)DDS_DataWriterImpl_finalize(datawriter);
    }

    return retval;
}

/* Note: this might be able to call the write_untyped call to make sure that
 * all liveliness processing happens in the same place. */

void
DDS_DataWriter_update_liveliness(DDS_DataWriter *self,
                                 struct OSAPI_NtpTime *timestamp)
{
    struct OSAPI_NtpTime next_liveliness;
    struct OSAPI_NtpTime now;
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;

    if (DDS_Duration_is_infinite(&datawriter->qos.liveliness.lease_duration))
    {
        return;
    }

    if (timestamp == NULL)
    {
        OSAPI_System_get_time(&now);
    }
    else
    {
        now = *timestamp;
    }

    OSAPI_NtpTime_add(&next_liveliness, &now, &datawriter->liveliness_time);

    if (!OSAPI_Timer_update_timeout(self->config.timer,
            &datawriter->liveliness_event,
            DDS_Duration_to_ms(datawriter->qos.liveliness.lease_duration)))
    {
        DDSC_Log_error_p1(LOG_FAIL_UPDATE_LIVELINESS_DATAWRITER,
                datawriter->as_entity.entity_id);
    }
}

DDS_ReturnCode_t
DDS_DataWriter_write_untyped(DDS_DataWriter *self,
                             const void *instance_data,
                             const DDS_InstanceHandle_t *handle,
                             struct NDDS_DataWriterSampleInfo *sample_info)
{
    struct DDS_DataWriterImpl *datawriter = (struct DDS_DataWriterImpl *)self;
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    struct RTI_TransformCDR_Sample *sample = NULL;
    DDSHST_WriterSampleEntryRef_T sample_entry;
    struct DDSHST_WriterState *wh_state;
    struct NETIO_Address destination = NETIO_Address_INITIALIZER;
    DDS_InstanceHandle_t actual_instance = DDS_HANDLE_NIL_NATIVE;
    struct NETIO_PacketInfo *pkt_info;
    DDS_KeyHash_t keyHashBuffer;
    DDS_UnsignedShort pid_id;
    DDS_UnsignedShort pid_length;

    if (!DDS_Entity_is_enabled(&self->as_entity))
    {
        return DDS_RETCODE_NOT_ENABLED;
    }

    DDS_DataWriter_update_liveliness(self, &sample_info->timestamp);

    /* Anonymous writers may republish the same sample, e.g for discovery
     */
    if (!self->qos.management.is_anonymous)
    {
        REDA_SequenceNumber_plusplus(&(self->last_sn));
    }

    if (datawriter->type_plugin->get_key_kind(self->type_plugin, NULL) ==
        NDDS_TYPEPLUGIN_NO_KEY)
    {
        actual_instance.is_valid = DDS_BOOLEAN_TRUE;
    }
    else if (datawriter->type_plugin->get_key_kind(
                datawriter->type_plugin, NULL) == NDDS_TYPEPLUGIN_USER_KEY)
    {
        if (DDS_InstanceHandle_is_nil(handle))
        {
            keyHashBuffer.length = MIG_RTPS_KEY_HASH_MAX_LENGTH;
            OSAPI_Memory_zero(keyHashBuffer.value, keyHashBuffer.length);
            if (!datawriter->type_plugin->instance_to_keyhash(
                    datawriter->type_plugin,
                    datawriter->md5Stream,
                    &keyHashBuffer,
                    instance_data,
                    datawriter->qos.type_support.plugin_data))
            {
                DDSC_Log_error(LOG_FAIL_INSTANCE_TO_KEYHASH);
                return DDS_RETCODE_PRECONDITION_NOT_MET;
            }
            OSAPI_Memory_copy(&actual_instance.octet, &keyHashBuffer.value,
                             keyHashBuffer.length);
            actual_instance.is_valid = DDS_BOOLEAN_TRUE;
        }
        else
        {
            actual_instance = *handle;
        }
    }
    else
    {
        /* NOTE: GUID instances are _always_ passed in */
        actual_instance = *handle;
    }

    /* If we need to save the sample, add it to the history queue. It would be
     * nice to store the same in case of best-effort and send it once also
     * for late joiners.
     */
    sample_entry = NULL;
    actual_instance.is_valid = DDS_BOOLEAN_TRUE;
    if ((self->qos.reliability.kind == DDS_RELIABLE_RELIABILITY_QOS)
         || !DDS_Duration_is_infinite(&datawriter->qos.deadline.period))
    {
        sample_entry = DDSHST_Writer_get_entry(self->wh, &actual_instance);
        if (sample_entry == NULL)
        {
            DDSC_Log_error(LOG_FAIL_GET_ENTRY_DUE_TO_RESOURCE_LIMITS);
            return DDS_RETCODE_OUT_OF_RESOURCES;
        }
    }

    sample = (struct RTI_TransformCDR_Sample*)
                                  REDA_BufferPool_get_buffer(self->cdr_samples);
    if (sample == NULL)
    {
        DDSC_Log_error(LOG_OUT_OF_SAMPLES);
        if (sample_entry != NULL)
        {
            DDSHST_Writer_return_entry(self->wh, sample_entry);
        }
        return DDS_RETCODE_OUT_OF_RESOURCES;
    }

    NETIO_Packet_initialize(&sample->packet,sample->payload,
                           datawriter->max_packet_length,128,NULL);
    pkt_info = NETIO_Packet_get_info(&sample->packet);

    /* First serialize inline qos */
    NETIO_Packet_set_head(&sample->packet,0-(datawriter->max_cdr_serialized_length+32));

    CDR_Stream_Reset(&self->stream);
    CDR_Stream_SetBuffer(&self->stream,
                           NETIO_Packet_get_head(&sample->packet),
                           (datawriter->max_cdr_serialized_length+32));
    pkt_info = NETIO_Packet_get_info(&sample->packet);

    keyHashBuffer.length = 0;
    if (sample_info->sendKeyHash)
    {
        keyHashBuffer.length = MIG_RTPS_KEY_HASH_MAX_LENGTH;
        OSAPI_Memory_zero(keyHashBuffer.value, keyHashBuffer.length);
        CDR_Stream_Reset(self->md5Stream);
        if (DDS_InstanceHandle_is_nil(handle))
        {
            if (!self->type_plugin->instance_to_keyhash(
                        self->type_plugin,self->md5Stream,&keyHashBuffer,
                        instance_data,self->qos.type_support.plugin_data))
            {
                DDSC_Log_error(LOG_FAIL_INSTANCE_TO_KEYHASH);
                return DDS_RETCODE_PRECONDITION_NOT_MET;
            }
        }
        else
        {
            keyHashBuffer = *(DDS_KeyHash_t *) handle;
            keyHashBuffer.length = 16;
        }

        pid_id = MIG_RTPS_PID_KEY_HASH;
        pid_length = keyHashBuffer.length;
        CDR_Stream_serialize_UnsignedShort(&self->stream, &pid_id);
        CDR_Stream_serialize_UnsignedShort(&self->stream, &pid_length);
        CDR_Stream_serialize_ByteArray(&self->stream,
                                     keyHashBuffer.value, keyHashBuffer.length);
    }

    if (sample_info->statusInfo &
        (MIG_RTPS_DISPOSE_STATUS_INFO | MIG_RTPS_UNREGISTER_STATUS_INFO))
    {
        pid_id = MIG_RTPS_PID_STATUS_INFO;
        pid_length = 4;
        CDR_Stream_serialize_UnsignedShort(&self->stream, &pid_id);
        CDR_Stream_serialize_UnsignedShort(&self->stream, &pid_length);
        CDR_Stream_serialize_4ByteToBigEndianFromStream(&self->stream,
                                                      &sample_info->statusInfo);
    }

    if (CDR_Stream_GetCurrentPositionOffset(&self->stream) > 0)
    {
        pid_id = MIG_RTPS_PID_SENTINEL;
        pid_length = 1;
        CDR_Stream_serialize_UnsignedShort(&self->stream, &pid_id);
        CDR_Stream_serialize_UnsignedShort(&self->stream, &pid_length);

        pkt_info->rtps_flags |= NETIO_RTPS_FLAGS_INLINEQOS;
    }

    if ((instance_data != NULL) &&
        (sample_info->statusInfo == MIG_RTPS_NO_STATUS_INFO))
    {
        CDR_Stream_serialize_header(&self->stream,
                            datawriter->key_kind == NDDS_TYPEPLUGIN_GUID_KEY);
        CDR_Stream_reset_alignment(&self->stream);

        if (DDS_BOOLEAN_TRUE !=
                self->type_plugin->serialize_data(
                		&self->stream, instance_data,
                		self->qos.type_support.plugin_data))
        {
            DDSC_Log_error(LOG_FAIL_SERIALIZE_DATA);
            return DDS_RETCODE_ERROR;
        }
        pkt_info->rtps_flags |= NETIO_RTPS_FLAGS_DATA;
    }

    if (sample_info->statusInfo & MIG_RTPS_DISPOSE_STATUS_INFO)
    {
        pkt_info->rtps_flags |= NETIO_RTPS_FLAGS_DISPOSE;
    }
    if (sample_info->statusInfo & MIG_RTPS_UNREGISTER_STATUS_INFO)
    {
        pkt_info->rtps_flags |= MIG_RTPS_UNREGISTER_STATUS_INFO;
    }

    NETIO_Packet_set_tail(&sample->packet,0-((datawriter->max_cdr_serialized_length+32) - CDR_Stream_GetCurrentPositionOffset(&self->stream)));

    /* General protocol data */
    pkt_info->sn = self->last_sn;
    pkt_info->priority = 0;
    pkt_info->timestamp = sample_info->timestamp;
    OSAPI_Memory_copy(&pkt_info->instance, actual_instance.octet, 16);

    /* Add intra-transport information. Since micro do not support multi-
     * encapsulation this is the only option. Since intra-transport is
     * a synchronous call the data pointer is valid on the receiving end
     */
    pkt_info->protocol_id = NETIO_PROTOCOL_INTRA;
    pkt_info->protocol_data.intra_info.user_data = (void*)instance_data;
    pkt_info->valid_data = (instance_data != NULL) &&
            (sample_info->statusInfo == MIG_RTPS_NO_STATUS_INFO);
    pkt_info->timestamp = sample_info->timestamp;
    NETIO_Packet_save_positions(&sample->packet);

    /* Note: data is currently not stored for best-effort. Thus,
     * when reliable, get the commitable sn the queue, for best
     * effort it is always the last sent.
     */
    if (sample_entry != NULL)
    {
        DDSHST_Writer_commit_entry(self->wh,sample_entry, &sample->_sample,
                                       &self->last_sn);
        wh_state = DDSHST_Writer_get_state(datawriter->wh);
        pkt_info->committable_sn = wh_state->high_sn;
        pkt_info->first_available_sn = wh_state->low_sn;
    }
    else
    {
        pkt_info->committable_sn = self->last_sn;
        pkt_info->first_available_sn = self->last_sn;
    }

    /* The protocol that commitable is everything up to last SN + 1*/
    REDA_SequenceNumber_plusplus(&pkt_info->committable_sn);

    if (!NETIO_Interface_send(datawriter->dw_intf,
                              datawriter->dw_intf,&destination,&sample->packet))
    {
        return DDS_RETCODE_ERROR;
    }

    if (sample_entry == NULL)
    {
        REDA_BufferPool_return_buffer(self->cdr_samples,sample);
    }

    return retcode;
}

DDS_ReturnCode_t
DDS_DataWriter_register_key(struct DDS_DataWriterImpl *datawriter, /* self */
                            DDS_InstanceHandle_t *handle, /* inout */
                            const void *instance_data, /* in */
                            const struct OSAPI_NtpTime *timestamp /* in */)
{
    DDS_KeyHash_t keyHashBuffer;
    DDSHST_ReturnCode_T wh_rc;

    if (datawriter->type_plugin->get_key_kind(
       datawriter->type_plugin, NULL) == NDDS_TYPEPLUGIN_GUID_KEY)
    {
        /* GUID keys */
        DDS_InstanceHandle_from_rtps(
           handle, (const struct MIGRtpsGuid *)instance_data);
    }
    else
    {
        /* user key */
        keyHashBuffer.length = MIG_RTPS_KEY_HASH_MAX_LENGTH;
        OSAPI_Memory_zero(keyHashBuffer.value, keyHashBuffer.length);

        if (!datawriter->type_plugin->instance_to_keyhash(
                datawriter->type_plugin, datawriter->md5Stream, &keyHashBuffer,
                instance_data, datawriter->qos.type_support.plugin_data))
            {
                DDSC_Log_error(LOG_FAIL_INSTANCE_TO_KEYHASH);
                return DDS_RETCODE_PRECONDITION_NOT_MET;
            }
        OSAPI_Memory_copy(&handle->octet,
                          &keyHashBuffer.value, keyHashBuffer.length);
        handle->is_valid = DDS_BOOLEAN_TRUE;
    }

    wh_rc = DDSHST_Writer_register_key(datawriter->wh, handle);
    if (wh_rc != DDSHST_RETCODE_SUCCESS)
    {
        /* DDSHST_RETCODE_NOSPACE */
        return DDS_RETCODE_OUT_OF_RESOURCES;
    }

    return DDS_RETCODE_OK;
}


