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

#include "Entity.pkg.h"
#include "DataReaderQos.pkg.h"
#include "DataReaderImpl.pkg.h"
#include "DataReaderEvent.pkg.h"
#include "DataReader.pkg.h"

struct DDS_DataReaderInterfaceProperty
{
    struct NETIO_InterfaceProperty _parent;
    struct NETIO_Address intf_address;
    struct DDS_DataReaderImpl *datareader;
};


#define DDS_DataReaderInterfaceProperty_INITIALIZER \
{\
    NETIO_InterfaceProperty_INITIALIZER,\
    NETIO_Address_INITIALIZER \
}


typedef enum
{
    REMOTE_WRITERSTATE_NOT_ALIVE,
    REMOTE_WRITERSTATE_ALIVE
} RemoteWriterState_t;

struct DataReaderRouteEntry
{
    struct NETIORouteEntry _parent;
};

struct DataReaderBindEntry
{
    struct NETIOBindEntry _parent;
    RemoteWriterState_t writer_state;
    RTI_UINT32 activity_count;
    RTI_UINT32 last_activity_count;
    RTI_UINT32 strength;
};

struct DDS_DataReaderInterfaceFactory;
struct DDS_DataReaderInterface
{
    struct NETIO_Interface _parent;
    struct DDS_DataReaderInterfaceProperty property;
    struct NETIO_InterfaceListener listener;
    struct DDS_DataReaderInterfaceFactory *factory;
    NETIO_InterfaceState_T state;
};

struct DDS_DataReaderInterfaceFactory
{
    struct RT_ComponentFactory _parent;
    struct DDS_DataReaderInterface _external;
    RTI_INT32 instance_counter;
    RTI_BOOL _initialized;
};

RTI_PRIVATE struct NETIO_InterfaceI DDS_DataReaderInterface_g_intf;

/*** SOURCE_BEGIN ***/

RTI_PRIVATE OSAPITimeoutOp_t
DDS_DataReaderInterface_on_liveliness_timeout(struct OSAPITimeoutUserData *storage)
{
    struct DDS_DataReaderInterface *self =
                (struct DDS_DataReaderInterface *)storage->field[0];
    DB_Cursor_T bind_cursor = NULL;
    DB_ReturnCode_T dbrc;
    struct DataReaderBindEntry *bind_entry;

    DB_Database_lock(self->property.datareader->config.db);

    dbrc = DB_Table_select_all(
                    self->_parent._btable,DB_TABLE_DEFAULT_INDEX,&bind_cursor);
    do
    {
        dbrc = DB_Cursor_get_next(bind_cursor,(DB_Record_T*)&bind_entry);
        if (dbrc == DB_RETCODE_OK)
        {
            if ((bind_entry->activity_count == bind_entry->last_activity_count) &&
                (bind_entry->writer_state == REMOTE_WRITERSTATE_ALIVE))
            {
                DDS_DataReaderEvent_on_liveliness_lost(
                        self->property.datareader,&bind_entry->_parent.source);
                bind_entry->writer_state = REMOTE_WRITERSTATE_NOT_ALIVE;
            }
            bind_entry->last_activity_count = bind_entry->activity_count;
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(self->_parent._btable,bind_cursor);

    DB_Database_unlock(self->property.datareader->config.db);

    return OSAPI_TIMEOUT_OP_AUTOMATIC;
}

RTI_PRIVATE RTI_BOOL
DDS_DataReaderInterface_initialize(struct DDS_DataReaderInterface *self,
                struct DDS_DataReaderInterfaceFactory *factory,
                const struct DDS_DataReaderInterfaceProperty *const property,
                const struct NETIO_InterfaceListener *const listener)
{
    struct DB_TableProperty tbl_property = DB_TableProperty_INITIALIZER;
    char tbl_name[16];
    union RT_ComponentFactoryId id;

    if (!NETIO_Interface_initialize(&self->_parent,
                           &DDS_DataReaderInterface_g_intf,
                           (property ? &property->_parent : NULL),
                           (listener ? listener : NULL)))
    {
        return RTI_FALSE;
    }

    if (property)
    {
        self->property = *property;
    }

    id._value = factory->_parent._id._value;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'r',
                                      factory->instance_counter);

    tbl_property.max_records = property->_parent.max_routes;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'b',
                                      factory->instance_counter);
    if (DB_Database_create_table(&self->_parent._btable,
            property->_parent._parent.db,&tbl_name[0],sizeof(struct DataReaderBindEntry),
            NULL,NETIO_Interface_compare_bind,&tbl_property) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    if (property)
    {
        self->property = *property;
    }

    self->state = NETIO_INTERFACESTATE_CREATED;

    self->factory = factory;
    ++factory->instance_counter;

    self->property = *property;

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
DDS_DataReaderInterface_finalize(struct DDS_DataReaderInterface *drintf)
{
    struct DataReaderBindEntry *bind;
    DB_Cursor_T cursor = NULL;
    DB_ReturnCode_T dbrc;

    NETIO_Interface_finalize(&drintf->_parent);

    if (!DDS_Duration_is_infinite(
            &drintf->property.datareader->qos.liveliness.lease_duration))
    {
        OSAPI_Timer_delete_timeout(drintf->property.datareader->config.timer,
                            &drintf->property.datareader->liveliness_event);
    }

    cursor = NULL;
    dbrc = DB_Table_select_all_default(drintf->_parent._btable,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&bind);
        if (dbrc == DB_RETCODE_OK)
        {
            dbrc = DB_Table_delete_record(
                    drintf->_parent._btable,(DB_Record_T*)bind);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(drintf->_parent._btable,cursor);

    DB_Database_delete_table(drintf->property._parent._parent.db,
            drintf->_parent._btable);

    return RTI_TRUE;
}

/* PUBLIC API */

void
DDS_DataReaderInterface_delete(struct DDS_DataReaderInterface *netio_intf)
{
    DDS_DataReaderInterface_finalize(netio_intf);
    OSAPI_Heap_freeStructure(netio_intf);
}

struct DDS_DataReaderInterface*
DDS_DataReaderInterface_create(struct DDS_DataReaderInterfaceFactory *factory,
                  const struct DDS_DataReaderInterfaceProperty *const property,
                  const struct NETIO_InterfaceListener *const listener)
{
    struct DDS_DataReaderInterface *dw_intf = NULL;

    OSAPI_Heap_allocateStructure(&dw_intf, struct DDS_DataReaderInterface);
    if (dw_intf == NULL)
    {
        return NULL;
    }

    if (!DDS_DataReaderInterface_initialize(dw_intf,factory,property,listener))
    {
        DDS_DataReaderInterface_delete(dw_intf);
        return NULL;
    }

    return dw_intf;
}

RTI_BOOL
DDS_DataReaderInterface_send(NETIO_Interface_T *self,
                             NETIO_Interface_T *src_intf,
                             struct NETIO_Address *destination,
                             NETIO_Packet_T *packet)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_receive(NETIO_Interface_T *netio,
                                struct NETIO_Address *src_addr,
                                struct NETIO_Address *dst_addr,
                                NETIO_Packet_T *packet)
{
    struct DDS_DataReaderInterface *drio =
                                      (struct DDS_DataReaderInterface *)netio;
    struct DDS_DataReaderImpl *datareader = drio->property.datareader;
    RTI_BOOL retval = RTI_FALSE;
    struct DDS_SampleInfo *sample_info;
    NDDS_TypePluginKeyKind key_kind;
    DDS_InstanceHandle_t instance_handle = DDS_HANDLE_NIL;
    DDS_InstanceHandle_t publisher_handle = DDS_HANDLE_NIL;
    DDS_UnsignedShort pid_id;
    DDS_UnsignedShort pid_length;
    DDS_UnsignedLong status_info = 0;
    DDS_Boolean key_found = DDS_BOOLEAN_FALSE;
    void *plugin_data;
    DDS_KeyHash_t key_hash;
    struct RTI_DataReaderSample *sample;
    DDSHST_ReaderSampleEntryRef_T sample_entry;
    struct DDS_SampleInfo temp_info;
    struct NETIO_PacketInfo *pkt_info;
    RTI_BOOL has_inline_qos = RTI_FALSE;
    struct DataReaderBindEntry *bind_entry = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIOBindEntryKey bind_key;
    DDS_InstanceHandle_t actual_instance = DDS_HANDLE_NIL_NATIVE;
    DDS_Boolean sample_dropped;
    RTI_UINT32 cur_stream_pos;

    /* Make sure we do not read any data before the reader is enabled. This
     * check is added in case we receive data on a port that is shared
     */
    if (!DDS_DataReader_is_enabled(datareader))
    {
        return RTI_TRUE;
    }

    DB_Database_lock(datareader->config.db);

    bind_key.source = *src_addr;
    bind_key.destination = drio->property.intf_address;

    dbrc = DB_Table_select_match(drio->_parent._btable,DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T *)&bind_entry,&bind_key);
    if (dbrc == DB_RETCODE_NOT_EXISTS)
    {
        goto done;
    }

    ++bind_entry->activity_count;

    pkt_info = NETIO_Packet_get_info(packet);

    /* process Lost Data first */
    if (pkt_info->rtps_flags & NETIO_RTPS_FLAGS_LOST_DATA)
    {
        LOG_printDebug("DRT: NETIO_RTPS_FLAGS_LOST_DATA\n");

        DDS_DataReaderEvent_on_sample_lost(datareader,
                src_addr,&pkt_info->lost_sample_sn,
                pkt_info->lost_sample_count);
        if (bind_entry->writer_state == REMOTE_WRITERSTATE_NOT_ALIVE)
        {
            DDS_DataReaderEvent_on_liveliness_detected(datareader,src_addr);
            bind_entry->writer_state = REMOTE_WRITERSTATE_ALIVE;
        }
    }

    /* commit (invalid) data  */
    OSAPI_Memory_copy(&publisher_handle.octet,&src_addr->value.guid,16);
    publisher_handle.is_valid = DDS_BOOLEAN_TRUE;

    if (!pkt_info->valid_data && 
        (pkt_info->rtps_flags & NETIO_RTPS_FLAGS_COMMIT_DATA))
    {
        LOG_printDebug("DRT: COMMIT, NO DATA\n");
        if (DDSHST_Reader_commit_sample(datareader->_rh,
                        &publisher_handle,
                        &pkt_info->committable_sn,bind_entry->strength) !=
            DDSHST_RETCODE_SUCCESS)
        {
            DDSC_Log_error(LOG_FAIL_COMMIT_SAMPLE);
            goto done;
        }
    }

    if (pkt_info->rtps_flags & NETIO_RTPS_FLAGS_INLINEQOS)
    {
        LOG_printDebug("DRT: INLINE QOS\n");
        has_inline_qos = RTI_TRUE;
    }

    if (!has_inline_qos && !pkt_info->valid_data)
    {
        /* no valid data or inline qos to process */
        retval = RTI_TRUE;
        goto done;
    }

    /* packet's inline_data is buffer containing inline Qos and/or data */
    CDR_Stream_Reset(&datareader->stream);
#ifdef RTI_ENDIAN_LITTLE
    CDR_Stream_ByteswapSet(&datareader->stream, RTI_TRUE);
#else
    CDR_Stream_ByteswapSet(&datareader->stream, RTI_FALSE);
#endif

    if ((pkt_info->protocol_id == NETIO_PROTOCOL_INTRA) &&
         datareader->type_plugin->get_key_kind(datareader->type_plugin,
                         datareader->qos.type_support.plugin_data) ==
                         NDDS_TYPEPLUGIN_USER_KEY)
    {
        DDS_KeyHash_t keyHashBuffer;
        RTI_BOOL result;
        result = datareader->type_plugin->instance_to_keyhash(
                datareader->type_plugin,&datareader->stream,&keyHashBuffer,
                pkt_info->protocol_data.intra_info.user_data,
                datareader->qos.type_support.plugin_data);
        CDR_Stream_deserialize_ByteArray(&datareader->stream,
                                       instance_handle.octet, 16);
        has_inline_qos = RTI_FALSE;
    }

    if (has_inline_qos || (pkt_info->protocol_id == NETIO_PROTOCOL_RTPS))
    {
        CDR_Stream_SetBuffer(
                &datareader->stream,
                pkt_info->protocol_data.rtps_data.inline_data->pointer,
                pkt_info->protocol_data.rtps_data.inline_data->length);
    }

    if (has_inline_qos)
    {
        do
        {
            CDR_Stream_deserialize_UnsignedShort(&datareader->stream, &pid_id);
            CDR_Stream_deserialize_UnsignedShort(&datareader->stream, &pid_length);

            LOG_printDebug("Processing inline QoS, id(%d) length(%d)\n",
                           pid_id, pid_length);

            switch (pid_id)
            {   
            case MIG_RTPS_PID_SENTINEL:
                break;

            case MIG_RTPS_PID_KEY_HASH:
                LOG_printDebug("Found key-hash\n"); 
                DDSC_LOG_DEBUG(LOG_FOUND_KEY_HASH);
                CDR_Stream_deserialize_ByteArray(&datareader->stream,
                                               instance_handle.octet, 16);
                key_found = DDS_BOOLEAN_TRUE;
                break;

            case MIG_RTPS_PID_STATUS_INFO:
                CDR_Stream_deserialize_4ByteFromBigEndianFromStream(
                   &datareader->stream, &status_info);
                DDSC_LOG_DEBUG_P1(LOG_STATUS_INFO, status_info);
                DDSC_LOG_PRINT_DEBUG("Status info (0x%08x):", status_info);
                if (status_info & MIG_RTPS_DISPOSE_STATUS_INFO)
                {
                    DDSC_LOG_PRINT_DEBUG(" DISPOSE");
                }
                if (status_info & MIG_RTPS_UNREGISTER_STATUS_INFO)
                {
                    DDSC_LOG_PRINT_DEBUG(" UNREGISTER");
                }
                LOG_printDebug("\n");
                break;

            default:
                LOG_printDebug("\tignored pid(%d)\n",pid_id);
                CDR_Stream_IncrementCurrentPosition(&datareader->stream, 
                                                      pid_length);
                break;
            }
        }
        while (pid_id != MIG_RTPS_PID_SENTINEL);
    }

    /* by here, datareader stream points to start of data-payload */

    if (pkt_info->rtps_flags & NETIO_RTPS_FLAGS_LIVELINESS)
    {
        LOG_printDebug("DRT: NETIO_RTPS_FLAGS_LIVELINESS\n");

        if (bind_entry->writer_state == REMOTE_WRITERSTATE_NOT_ALIVE)
        {
            DDS_DataReaderEvent_on_liveliness_detected(datareader,src_addr);
            bind_entry->writer_state = REMOTE_WRITERSTATE_ALIVE;
        }
    }

    /* If the user has installed this callback, let the user choose
     * determine whether to keep the sample or not. it is very
     * important to note that the filtering is done _before_ any
     * resources are allocated.
     */
    if ((datareader->listener.on_before_sample_deserialize) &&
        (pkt_info->protocol_id == NETIO_PROTOCOL_RTPS) &&
        packet->info.valid_data)
    {
        sample_dropped = DDS_BOOLEAN_FALSE;
        cur_stream_pos = CDR_Stream_GetCurrentPositionOffset(&datareader->stream);

        if (!datareader->listener.on_before_sample_deserialize(
                datareader->listener.as_listener.listener_data,
                datareader,
                datareader->type_plugin,
                &datareader->stream,
                &sample_dropped))
        {
            goto done;
        }

        if (sample_dropped)
        {
            if (DDSHST_Reader_commit_sample(datareader->_rh,
                        &publisher_handle,&pkt_info->committable_sn,bind_entry->strength) !=
                                DDSHST_RETCODE_SUCCESS)
            {
                DDSC_Log_error(LOG_FAIL_COMMIT_SAMPLE);
                goto done;
            }

            if (bind_entry->writer_state == REMOTE_WRITERSTATE_NOT_ALIVE)
            {
                DDS_DataReaderEvent_on_liveliness_detected(datareader,src_addr);
                bind_entry->writer_state = REMOTE_WRITERSTATE_ALIVE;
            }
            goto success;
        }
        CDR_Stream_SetCurrentPositionOffset(&datareader->stream,cur_stream_pos);
    }

    sample = (struct RTI_DataReaderSample*)
                            REDA_BufferPool_get_buffer(datareader->cdr_samples);
    if (sample == NULL)
    {
        DDSC_Log_error(LOG_FAIL_GET_CDR_SAMPLE);
        goto done;
    }

    sample->hst_sample._param = sample;

    if ((pkt_info->protocol_id == NETIO_PROTOCOL_INTRA) &&
            (pkt_info->rtps_flags & NETIO_RTPS_FLAGS_DATA))
    {
        OSAPI_Memory_copy(&instance_handle.octet, &pkt_info->instance, 16);
        instance_handle.is_valid = DDS_BOOLEAN_TRUE;
        status_info = pkt_info->rtps_flags;
    }
    else if ((pkt_info->protocol_id == NETIO_PROTOCOL_RTPS) &&
              packet->info.valid_data)
    {
        plugin_data = datareader->qos.type_support.plugin_data;

        /* data encapsulation header */
        CDR_Stream_deserialize_header(&datareader->stream);
        CDR_Stream_reset_alignment(&datareader->stream);

        if (DDS_BOOLEAN_TRUE !=
                datareader->type_plugin->deserialize_data(
                                            &datareader->stream,
                                            sample->hst_sample._user_data,
                                            plugin_data))
        {
            DDSC_Log_error(LOG_FAIL_DESERIALIZE_DATA);
            goto done;
        }
        key_kind = datareader->type_plugin->get_key_kind(
                                        datareader->type_plugin,plugin_data);

        switch (key_kind)
        {
        case NDDS_TYPEPLUGIN_NO_KEY:
            actual_instance.is_valid = DDS_BOOLEAN_TRUE;
            break;
        case NDDS_TYPEPLUGIN_GUID_KEY:
        case NDDS_TYPEPLUGIN_USER_KEY:
            if (!key_found)
            {
                DDSC_LOG_DEBUG(LOG_EXPECTED_KEY_NOT_FOUND_COMPUTING_KEY);
                key_hash.length = MIG_RTPS_KEY_HASH_MAX_LENGTH;
                OSAPI_Memory_zero(key_hash.value, key_hash.length);
                OSAPI_Memory_zero(instance_handle.octet, key_hash.length);
                CDR_Stream_Reset(datareader->md5_stream);
                datareader->type_plugin->
                instance_to_keyhash(datareader->type_plugin,
                        datareader->md5_stream, &key_hash,
                        sample->hst_sample._user_data,
                        plugin_data);
                OSAPI_Memory_copy(instance_handle.octet, key_hash.value,
                        key_hash.length);
            }
            break;
        default:
            /* all cases covered above */
            break;
        }
    }

    if ((pkt_info->protocol_id == NETIO_PROTOCOL_RTPS) &&
            datareader->qos.management.is_anonymous)
    {
        OSAPI_Memory_copy(publisher_handle.octet,
                         &pkt_info->protocol_data.rtps_data.guid_prefix, 12);
    }
    else
    {
        OSAPI_Memory_copy(publisher_handle.octet, &src_addr->value.guid, 16);
    }
    publisher_handle.is_valid = DDS_BOOLEAN_TRUE;
    instance_handle.is_valid = DDS_BOOLEAN_TRUE;

    /* If the user has installed theon_before_sample_commit listener,
     * let the user determine if the sample is committed.
     */
    if (datareader->listener.on_before_sample_commit && packet->info.valid_data)
    {
        sample_dropped = DDS_BOOLEAN_FALSE;
        if (pkt_info->protocol_id == NETIO_PROTOCOL_INTRA)
        {
            if (!datareader->listener.on_before_sample_commit(
                    datareader->listener.as_listener.listener_data,
                    datareader,
                    pkt_info->protocol_data.intra_info.user_data,
                    &sample_dropped))
            {
                goto done;
            }
        }
        else
        {
            if (!datareader->listener.on_before_sample_commit(
                    datareader->listener.as_listener.listener_data,
                    datareader,
                    sample->hst_sample._user_data,
                    &sample_dropped))
            {
                goto done;
            }
        }

        if (sample_dropped)
        {
            if (DDSHST_Reader_commit_sample(datareader->_rh,
                    &publisher_handle,
                    &pkt_info->committable_sn,bind_entry->strength) != DDSHST_RETCODE_SUCCESS)
            {
                DDSC_Log_error(LOG_FAIL_COMMIT_SAMPLE);
                goto done;
            }

            if (bind_entry->writer_state == REMOTE_WRITERSTATE_NOT_ALIVE)
            {
                DDS_DataReaderEvent_on_liveliness_detected(datareader,src_addr);
                bind_entry->writer_state = REMOTE_WRITERSTATE_ALIVE;
            }
            REDA_BufferPool_return_buffer(datareader->cdr_samples, sample);
            goto success;
        }
    }

    /* To support ownership get_entry may return
     * - NULL && !owner => OK, just commit all the samples for this writer
     * - NULL && owner, error, resource problem
     * - !NULL && owner => OK
     * - !NULL && !owner => Should never happen
     *
     * The strength of the write is passed in because it is the current
     * strength, otherwise the queue must request the strength. Thus, a mutable
     * strength is already supported (when a route is updated the strength
     * in the bind entry is updated)
     *
     * Note that when a remote writer looses liveliness an event is posted to
     * the queue, allowing the queue to update the current owner
     */
    sample_entry = DDSHST_Reader_get_entry(datareader->_rh,
                                            &publisher_handle,
                                            &instance_handle,
                                            &sample->hst_sample._info,
                                            bind_entry->strength);
    if (sample_entry == NULL)
    {
        REDA_BufferPool_return_buffer(datareader->cdr_samples, sample);
        DDSC_Log_error(LOG_FAIL_GET_ENTRY_DUE_TO_RESOURCE_LIMITS);
        goto done;
    }

    sample_info = sample->hst_sample._info;

    if (pkt_info->protocol_id == NETIO_PROTOCOL_INTRA)
    {
        sample_info->valid_data = (pkt_info->rtps_flags & NETIO_RTPS_FLAGS_DATA);
        if (sample_info->valid_data)
        {
            datareader->type_plugin->copy_sample(datareader->type_plugin,
                    sample->hst_sample._user_data,
                    pkt_info->protocol_data.intra_info.user_data,
                    datareader->qos.type_support.plugin_data);
        }
    }
    else if (pkt_info->protocol_id == NETIO_PROTOCOL_RTPS)
    {
        sample_info->valid_data = packet->info.valid_data;
    }
    else
    {
        goto done;
    }

    OSAPI_NtpTime_unpackToNanosec(&sample_info->source_timestamp.sec,
                                 &sample_info->source_timestamp.nanosec,
                                 &pkt_info->timestamp);
    sample_info->instance_handle = instance_handle;
    sample_info->instance_handle.is_valid = DDS_BOOLEAN_TRUE;
    sample_info->publication_handle = publisher_handle;
    sample_info->publication_handle.is_valid = DDS_BOOLEAN_TRUE;

    /* TODO: Check that last time-stamp received is correct */
    OSAPI_NtpTime_unpackToNanosec(&sample_info->reception_timestamp.sec,
                                 &sample_info->reception_timestamp.nanosec,
                                 &datareader->last_sample_recvd_time);

    sample_info->publication_sequence_number =
                             *((struct DDS_SequenceNumber_t *)&pkt_info->sn);

    sample_info->reception_sequence_number =
                             *((struct DDS_SequenceNumber_t *)&pkt_info->sn);

    if (sample_info->valid_data)
    {
        LOG_printDebug("DRT: COMMIT VALID DATA\n");

        if (DDSHST_Reader_commit_entry(datareader->_rh,
                sample_entry,&sample->hst_sample,&pkt_info->sn) !=
                        DDSHST_RETCODE_SUCCESS)
        {
            REDA_BufferPool_return_buffer(datareader->cdr_samples, sample);
            DDSC_Log_error(LOG_FAIL_COMMIT_ENTRY);
            goto done;
        }
        /*
        firstUncommittableSn = pkt_info->committable_sn;
        REDA_SequenceNumber_plusplus(&firstUncommittableSn); */
#if 0
        if (*((RTI_INT32*)&(drio->property.intf_address.value.guid.guid[12]))
                == NETIO_ntohl(MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT))
        {
            REDA_SequenceNumber_plusplus(&firstUncommittableSn);
        }
#endif
        /* packet's committable_sn is actually the first uncommittable sn */
        if (DDSHST_Reader_commit_sample(datareader->_rh,
                &publisher_handle,&pkt_info->committable_sn,bind_entry->strength) !=
                                            DDSHST_RETCODE_SUCCESS)
        {
            DDSC_Log_error(LOG_FAIL_COMMIT_SAMPLE);
            goto done;
        }

        if (bind_entry->writer_state == REMOTE_WRITERSTATE_NOT_ALIVE)
        {
            DDS_DataReaderEvent_on_liveliness_detected(datareader,src_addr);
            bind_entry->writer_state = REMOTE_WRITERSTATE_ALIVE;
        }
    }
 
    /* Update the state of the sample */
    temp_info = *sample_info;
    if (!sample_info->valid_data)
    {
        REDA_BufferPool_return_buffer(datareader->cdr_samples, sample);
        DDSHST_Reader_return_entry(datareader->_rh, sample_entry);
    }

    if (status_info & MIG_RTPS_UNREGISTER_STATUS_INFO)
    {
        LOG_printDebug("DRT: COMMIT, MIG_RTPS_UNREGISTER_STATUS_INFO\n");
        if (DDSHST_Reader_unregister_key(datareader->_rh,&temp_info) !=
                                            DDSHST_RETCODE_SUCCESS)
        {
            /* TODO: log exception */
        }
    }

    if (status_info & MIG_RTPS_DISPOSE_STATUS_INFO)
    {
        LOG_printDebug("DRT: COMMIT, MIG_RTPS_DISPOSE_STATUS_INFO\n");

        if (DDSHST_Reader_dispose(datareader->_rh,&temp_info) !=
                                            DDSHST_RETCODE_SUCCESS)
        {
            /* TODO: log exception */
        }
    }

success:
    retval = RTI_TRUE;

done:

    DB_Database_unlock(datareader->config.db);

    return retval;
}

RTI_BOOL
DDS_DataReaderInterface_ack(NETIO_Interface_T *self,
                            struct NETIO_Address *source,
                            NETIO_PacketId_T packet_id)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_request(NETIO_Interface_T *self,
                                struct NETIO_Address *source,
                                struct NETIO_Address *dest,
                                NETIO_Packet_T *packet,
                                NETIO_PacketId_T packet_id)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_return_loan(NETIO_Interface_T *self,
                                    struct NETIO_Address *source,
                                    NETIO_Packet_T **packet,
                                    NETIO_PacketId_T packet_id)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_discard(NETIO_Interface_T *self,
                                struct NETIO_Address *destination,
                                NETIO_PacketId_T packet_id)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_add_route(NETIO_Interface_T *self,
                                  struct NETIO_Address *dst_addr,
                                  NETIO_Interface_T *via_intf,
                                  struct NETIO_Address *via_addr,
                                  struct NETIORouteProperty *property,
                                  RTI_BOOL *existed)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_delete_route(NETIO_Interface_T *self,
                                     struct NETIO_Address *dst_addr,
                                     NETIO_Interface_T *via_intf,
                                     struct NETIO_Address *via_addr,
                                     RTI_BOOL *existed)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_bind(NETIO_Interface_T *netio,
                             struct NETIO_Address *src_addr,
                             struct NETIOBindProperty *property,
                             RTI_BOOL *existed)
{
    struct DDS_DataReaderInterface *drintf =
                                    (struct DDS_DataReaderInterface *)netio;
    struct DataReaderBindEntry *bind_entry = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIOBindEntryKey bind_key;

    bind_key.source = *src_addr;
    bind_key.destination = drintf->property.intf_address;

    dbrc = DB_Table_select_match(drintf->_parent._btable,DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T *)&bind_entry,&bind_key);
    if (dbrc == DB_RETCODE_OK)
    {
        return RTI_TRUE;
    }

    if (dbrc != DB_RETCODE_NOT_EXISTS)
    {
        return RTI_FALSE;
    }

    if (DB_Table_create_record(drintf->_parent._btable,
                              (DB_Record_T *)&bind_entry) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    OSAPI_Memory_zero(bind_entry,sizeof(struct NETIOBindEntry));
    bind_entry->_parent.source = *src_addr;
    bind_entry->_parent.destination = drintf->property.intf_address;
    bind_entry->_parent.intf = netio;
    bind_entry->last_activity_count = 0;
    bind_entry->activity_count = 0;
    bind_entry->writer_state = REMOTE_WRITERSTATE_ALIVE;
    if (property != NULL)
    {
        bind_entry->strength = property->strength;
    }
    else
    {
        bind_entry->strength = 0;
    }

    dbrc = DB_Table_insert_record(drintf->_parent._btable,
                                (DB_Record_T *)bind_entry);

    if ((dbrc != DB_RETCODE_OK) && (dbrc != DB_RETCODE_EXISTS))
    {
        return RTI_FALSE;
    }

    DDS_DataReaderEvent_on_liveliness_detected(
            drintf->property.datareader,&bind_entry->_parent.source);

    return RTI_TRUE;
}

RTI_BOOL
DDS_DataReaderInterface_unbind(NETIO_Interface_T *netio,
                        struct NETIO_Address *src_addr,
                        NETIO_Interface_T *dst_intf,
                        RTI_BOOL *existed)
{
    struct DDS_DataReaderInterface *drintf =
                                    (struct DDS_DataReaderInterface *)netio;
    struct DataReaderBindEntry *bind_entry = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIOBindEntryKey bind_key;
    DDS_InstanceHandle_t ih;

    bind_key.source = *src_addr;
    bind_key.destination = drintf->property.intf_address;

    dbrc = DB_Table_select_match(drintf->_parent._btable,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T *)&bind_entry,
                                &bind_key);
    if (dbrc == DB_RETCODE_NOT_EXISTS)
    {
        if (existed)
        {
            *existed = RTI_FALSE;
        }
        return RTI_TRUE;
    }

    if (dbrc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    if (existed)
    {
        *existed = RTI_TRUE;
    }

    dbrc = DB_Table_delete_record(drintf->_parent._btable,
                                 (DB_Record_T *)bind_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    OSAPI_Memory_copy(&ih.octet,&src_addr->value.guid,sizeof(src_addr->value.guid));
    ih.is_valid = DDS_BOOLEAN_TRUE;
    DDS_DataReader_liveliness_lost(drintf->property.datareader,&ih);

    return RTI_TRUE;
}

RTI_BOOL
DDS_DataReaderInterface_bind_external(NETIO_Interface_T *dst_intf,
                                      struct NETIO_Address *dst_addr,
                                      NETIO_Interface_T *src_intf,
                                      struct NETIO_Address *src_addr,
                                      struct NETIOBindProperty *property,
                                      RTI_BOOL *existed)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_unbind_external(NETIO_Interface_T *dst_intf,
                                        struct NETIO_Address *dst_addr,
                                        NETIO_Interface_T *src_intf,
                                        struct NETIO_Address *src_addr,
                                        RTI_BOOL *existed)
{
    return RTI_FALSE;
}

RTI_BOOL
DDS_DataReaderInterface_get_external_interface(NETIO_Interface_T *netio_intf,
                                               struct NETIO_Address *src_addr,
                                               NETIO_Interface_T **dst_intf,
                                               struct NETIO_Address *dst_addr)
{
    *dst_intf = netio_intf;
    return RTI_TRUE;
}

RTI_BOOL
DDS_DataReaderInterface_set_state(NETIO_Interface_T *netio,
                                  NETIO_InterfaceState_T state)
{

    struct DDS_DataReaderInterface *drio = (struct DDS_DataReaderInterface *)netio;
    struct OSAPITimeoutUserData storage;

    if ((drio->state != NETIO_INTERFACESTATE_CREATED) ||
         state != NETIO_INTERFACESTATE_ENABLED)
    {
        return RTI_TRUE;
    }

    /* start liveliness timer */
    storage.field[0] = (void *)drio;
    if (!DDS_Duration_is_infinite(&drio->property.datareader->qos.liveliness.lease_duration))
    {
        if (!OSAPI_Timer_create_timeout(drio->property.datareader->config.timer,
                  &drio->property.datareader->liveliness_event,
                  DDS_Duration_to_ms(drio->property.datareader->qos.liveliness.lease_duration),
                  OSAPI_TIMER_PERIODIC,
                  DDS_DataReaderInterface_on_liveliness_timeout,
                  &storage))
        {
            return RTI_FALSE;
        }
    }
    return RTI_TRUE;
}

RTI_PRIVATE struct NETIO_InterfaceI DDS_DataReaderInterface_g_intf =
{
    {DEFAULT_COMPONENT_RUN},
    DDS_DataReaderInterface_send, /* NETIO_Interface_sendFunc send */
    NULL, /* NETIO_Interface_ackFunc ack; */
    NULL, /* NETIO_Interface_requestFunc request; */
    NULL, /* NETIO_Interface_return_loanFunc return_loan; */
    NULL, /* NETIO_Interface_discardFunc discard; */
    DDS_DataReaderInterface_add_route, /* NETIO_Interface_add_routeFunc add_peer; */
    DDS_DataReaderInterface_delete_route, /* NETIO_Interface_delete_routeFunc remove_peer; */
    NULL, /* NETIO_Interface_get_local_addressFunc get_local_address; */
    DDS_DataReaderInterface_bind,
    DDS_DataReaderInterface_unbind,
    DDS_DataReaderInterface_receive,
    DDS_DataReaderInterface_get_external_interface,
    DDS_DataReaderInterface_bind_external,
    DDS_DataReaderInterface_unbind_external,
    DDS_DataReaderInterface_set_state,
    NULL
};

/* ------------------------------------------------------------------------ */
/*                   Plugin factory                                         */
/* ------------------------------------------------------------------------ */

RTI_PRIVATE RT_Component_T*
DDS_DataReaderInterfaceFactory_create_component(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentProperty *property,
        struct RT_ComponentListener *listener)
{
    struct DDS_DataReaderInterface *retval = NULL;

    retval = DDS_DataReaderInterface_create(
            (struct DDS_DataReaderInterfaceFactory*)factory,
            (const struct DDS_DataReaderInterfaceProperty *const)property,
            (const struct NETIO_InterfaceListener *const)listener);

    if (retval != NULL)
    {
        return &retval->_parent._parent;
    }
    else
    {
        return NULL;
    }
}

RTI_PRIVATE void
DDS_DataReaderInterfaceFactory_delete_component(
                                        struct RT_ComponentFactory *factory,
                                        RT_Component_T *component)
{
    struct DDS_DataReaderInterface *self =
                                    (struct DDS_DataReaderInterface*)component;

    DDS_DataReaderInterface_delete(self);
}

RTI_PRIVATE struct RT_ComponentFactory*
DDS_DataReaderInterfaceFactory_initialize(
        struct RT_ComponentFactoryProperty*property,
        struct RT_ComponentFactoryListener *listener);

RTI_PRIVATE void
DDS_DataReaderInterfaceFactory_finalize(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener);

RTI_PRIVATE struct RT_ComponentFactoryI DDS_DataReaderInterfaceFactory_g_intf =
{
    0, /* TODO: Define constant */
    DDS_DataReaderInterfaceFactory_initialize,
    DDS_DataReaderInterfaceFactory_finalize,
    DDS_DataReaderInterfaceFactory_create_component,
    DDS_DataReaderInterfaceFactory_delete_component
};

RTI_PRIVATE
struct DDS_DataReaderInterfaceFactory DDS_DataReaderInterfaceFactory_g_factory =
{
  {
     &DDS_DataReaderInterfaceFactory_g_intf,
     NULL
  }
};

RTI_PRIVATE struct RT_ComponentFactory*
DDS_DataReaderInterfaceFactory_initialize(
        struct RT_ComponentFactoryProperty *property,
        struct RT_ComponentFactoryListener *listener)
{
    struct DDS_DataReaderInterfaceFactory *factory =
                                    &DDS_DataReaderInterfaceFactory_g_factory;

    factory->instance_counter = 0;
    factory->_initialized = RTI_FALSE;
    DDS_DataReaderInterfaceFactory_g_factory._parent._factory = &factory->_parent;

    return &DDS_DataReaderInterfaceFactory_g_factory._parent;
}

RTI_PRIVATE void
DDS_DataReaderInterfaceFactory_finalize(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener)
{
    struct DDS_DataReaderInterfaceFactory *c_factory =
                            (struct DDS_DataReaderInterfaceFactory *)factory;

    c_factory->instance_counter = 0;
}

struct RT_ComponentFactoryI*
DDS_DataReaderInterfaceFactory_get_interface(void)
{
    return &DDS_DataReaderInterfaceFactory_g_intf;
}

