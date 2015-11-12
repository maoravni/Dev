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

#include "Entity.pkg.h"
#include "DataWriterImpl.pkg.h"
#include "DataWriterQos.pkg.h"
#include "DataWriterEvent.pkg.h"

struct DDS_DataWriterInterfaceProperty
{
    struct NETIO_InterfaceProperty _parent;
    struct NETIO_Address intf_address;
    struct DDS_DataWriterImpl *datawriter;
    RTI_UINT32 max_send_fanout;
};


#define DDS_DataWriterInterfaceProperty_INITIALIZER \
{\
    NETIO_InterfaceProperty_INITIALIZER,\
    NETIO_Address_INITIALIZER, \
    NULL, \
    8  \
}


struct DDS_DataWriterInterfaceFactory;
struct DDS_DataWriterInterface
{
    struct NETIO_Interface _parent;
    struct DDS_DataWriterInterfaceProperty property;
    struct NETIO_InterfaceListener listener;
    struct DDS_DataWriterInterfaceFactory *factory;
    DB_Index_T rtable_intf_index;
    struct NETIO_AddressSeq send_dests_seq;
};

struct DDS_DataWriterInterfaceFactory
{
    struct RT_ComponentFactory _parent;
    RTI_INT32 instance_counter;
    RTI_BOOL _initialized;
};


RTI_PRIVATE struct NETIO_InterfaceI DDS_DataWriterInterface_g_intf;

/*** SOURCE_BEGIN ***/

RTI_INT32
DDSDataWriterInterface_compare_route_intf(DB_Select_T opcode,RTI_INT32 flags,
                                    const DB_Record_T op1, void *op2, void *op3)
{
    struct NETIORouteEntry *left_record = (struct NETIORouteEntry *)op1;

    struct NETIO_Address dest;
    NETIO_Interface_T *intf = NULL;
    struct NETIO_Address intf_addr;
    RTI_INT32 result;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        dest = ((struct NETIORouteEntryKey *)op2)->destination;
        intf = ((struct NETIORouteEntryKey *)op2)->intf;
        intf_addr = ((struct NETIORouteEntryKey *)op2)->intf_address;
    }
    else
    {
        dest = ((struct NETIORouteEntry *)op2)->destination;
        intf = ((struct NETIORouteEntry *)op2)->intf;
        intf_addr = ((struct NETIORouteEntry *)op2)->intf_address;
    }

    /* intf --> intf_addr --> dest */
    result = OSAPI_Memory_compare(left_record->intf, intf,
                                  sizeof(NETIO_Interface_T));
    if (result)
    {
        return result;
    }

    result = OSAPI_Memory_compare(&left_record->intf_address, &intf_addr,
                                  sizeof(struct NETIO_Address));
    if (result)
    {
        return result;
    }

    return OSAPI_Memory_compare(&left_record->destination, &dest,
                                sizeof(struct NETIO_Address));
}


RTI_BOOL
DDS_DataWriterInterface_initialize(struct DDS_DataWriterInterface *self,
                struct DDS_DataWriterInterfaceFactory *factory,
                const struct DDS_DataWriterInterfaceProperty *const property,
                const struct NETIO_InterfaceListener *const listener)
{
    struct DB_TableProperty tbl_property = DB_TableProperty_INITIALIZER;
    struct DB_IndexProperty idx_property = DB_IndexProperty_INITIALIZER;
    char tbl_name[16];
    union RT_ComponentFactoryId id;

    if (!NETIO_Interface_initialize(&self->_parent,
                           &DDS_DataWriterInterface_g_intf,
                           (property ? &property->_parent : NULL),
                           (listener ? listener : NULL)))
    {
        return RTI_FALSE;
    }

    id._value = factory->_parent._id._value;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'r',factory->instance_counter);

    tbl_property.max_records = property->_parent.max_routes;
    tbl_property.max_indices = 1;
    if (DB_Database_create_table(&self->_parent._rtable,
                                property->_parent._parent.db,
                                &tbl_name[0],
                                sizeof(struct NETIORouteEntry),
                                NULL,
                                NETIO_Interface_compare_route,
                                &tbl_property) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    /* index on route interfaces */
    if (DB_Table_create_index(self->_parent._rtable,
                             &self->rtable_intf_index,
                             NULL,
                             DDSDataWriterInterface_compare_route_intf,
                             &idx_property) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    self->property = *property;
    self->factory = factory;

    /* destination sequence for send() */
    NETIO_AddressSeq_initialize(&self->send_dests_seq);
    NETIO_AddressSeq_set_maximum(&self->send_dests_seq, 
                                property->max_send_fanout);

    return RTI_TRUE;
}

RTI_BOOL
DDS_DataWriterInterface_finalize(struct DDS_DataWriterInterface *netio_intf)
{
    struct NETIORouteEntry *route = NULL;
    DB_Cursor_T cursor = NULL;
    DB_ReturnCode_T dbrc;

    NETIO_Interface_finalize(&netio_intf->_parent);

    dbrc = DB_Table_select_all_default(netio_intf->_parent._rtable,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&route);
        if (dbrc == DB_RETCODE_OK)
        {
            dbrc = DB_Table_delete_record(netio_intf->_parent._rtable,(DB_Record_T*)route);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(netio_intf->_parent._rtable,cursor);

    DB_Table_delete_index(netio_intf->_parent._rtable,
                         netio_intf->rtable_intf_index);

    (void)DB_Database_delete_table(netio_intf->property._parent._parent.db,
                            netio_intf->_parent._rtable);

    NETIO_AddressSeq_finalize(&netio_intf->send_dests_seq);

    return RTI_TRUE;
}

void
DDS_DataWriterInterface_delete(struct DDS_DataWriterInterface *netio_intf)
{
    DDS_DataWriterInterface_finalize(netio_intf);
    OSAPI_Heap_freeStructure(netio_intf);
}

struct DDS_DataWriterInterface*
DDS_DataWriterInterface_create(struct DDS_DataWriterInterfaceFactory *factory,
                  const struct DDS_DataWriterInterfaceProperty *const property,
                  const struct NETIO_InterfaceListener *const listener)
{
    struct DDS_DataWriterInterface *dw_intf = NULL;

    OSAPI_Heap_allocateStructure(&dw_intf, struct DDS_DataWriterInterface);
    if (dw_intf == NULL)
    {
        return NULL;
    }

    if (!DDS_DataWriterInterface_initialize(dw_intf,factory,property,listener))
    {
        DDS_DataWriterInterface_delete(dw_intf);
        return NULL;
    }

    ++factory->instance_counter;

    return dw_intf;
}

#define NETIO_SEND_TRACE(n,si,a,p)

RTI_BOOL
DDS_DataWriterInterface_send(NETIO_Interface_T *netio,
                             NETIO_Interface_T *src_intf,
                             struct NETIO_Address *address,
                             NETIO_Packet_T *packet)
{
    struct DDS_DataWriterInterface *dwio =
                                       (struct DDS_DataWriterInterface *)netio;
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl *)dwio->property.datawriter;
    DB_Cursor_T cursor = NULL;
    struct NETIORouteEntry *route_entry;
    DB_ReturnCode_T dbrc;
    RTI_BOOL retval = RTI_TRUE;
    NETIO_Interface_T *send_intf = NULL;
    RTI_INT32 seq_len = 0;
    struct NETIO_Address ADDR_UNKNOWN = NETIO_Address_INITIALIZER;
    RTI_BOOL is_unknown_addr;
    RTI_SIZE_T pkt_head, pkt_curr, pkt_tail;
    
    packet->source = dwio->property.intf_address;
    packet->dests = &dwio->send_dests_seq;

    NETIO_SEND_TRACE(netio,src_intf,address,packet);

    LOG_printDebug("DataWriter: send topic: %s\n",
            DDS_TopicDescription_get_name(DDS_Topic_as_topicdescription(dw->topic)));

    NETIO_AddressSeq_set_length(&dwio->send_dests_seq, 0);
    seq_len = 0;

    is_unknown_addr = (NETIO_Address_compare(address, &ADDR_UNKNOWN) == 0) ?
                       RTI_TRUE : RTI_FALSE;

    dbrc = DB_Table_select_all(dwio->_parent._rtable,
                              dwio->rtable_intf_index, &cursor);
    dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&route_entry);
    while (dbrc == DB_RETCODE_OK)
    {
        if (is_unknown_addr || /* broadcast to unknown dest */
                !OSAPI_Memory_compare(&route_entry->destination, address,
                        sizeof(address)))
        {
            if ((route_entry->intf != send_intf) ||
                (seq_len == (RTI_INT32)dwio->property.max_send_fanout))
            {
                NETIO_Packet_save_positions_to(
                        packet, &pkt_head, &pkt_curr, &pkt_tail);
                packet->info.protocol_id = NETIO_PROTOCOL_INTRA;
                if (send_intf && !NETIO_Interface_send(send_intf,
                        netio,
                        &route_entry->intf_address,
                        packet))
                {
                    retval = RTI_FALSE;
                }
                NETIO_Packet_restore_positions_from(
                        packet, pkt_head, pkt_curr, pkt_tail);

                send_intf = route_entry->intf;
                seq_len = 0;
                NETIO_AddressSeq_set_length(&dwio->send_dests_seq, seq_len);
            }

            NETIO_AddressSeq_set_length(&dwio->send_dests_seq, ++seq_len);
            *NETIO_AddressSeq_get_reference(&dwio->send_dests_seq, seq_len - 1) =
                    route_entry->destination;

            LOG_printDebug("DataWriter: route topic: %s/%d\n",
                    DDS_TopicDescription_get_name(DDS_Topic_as_topicdescription(dw->topic)),is_unknown_addr);
        }
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&route_entry);
    }
    DB_Cursor_finish(dwio->_parent._rtable, cursor);
    
    if (seq_len != 0)
    {
        packet->info.rtps_flags |= NETIO_RTPS_FLAGS_LAST_WRITE_FOR_SN;
        NETIO_Packet_save_positions_to(packet, &pkt_head, &pkt_curr, &pkt_tail);
        packet->info.protocol_id = NETIO_PROTOCOL_INTRA;
        if (!NETIO_Interface_send(send_intf, netio, &route_entry->intf_address,
                                  packet))
        {
            retval = RTI_FALSE;
        }
        NETIO_Packet_restore_positions_from(packet, pkt_head, pkt_curr, pkt_tail);
    }
    
    return retval;
}

#define NETIO_RECEIVE_TRACE(n,sa,da,p) \
        LOG_printDebug("receive\n")

RTI_BOOL
DDS_DataWriterInterface_receive(NETIO_Interface_T *netio_intf,
                                struct NETIO_Address *src_addr,
                                struct NETIO_Address *dst_addr,
                                NETIO_Packet_T *packet)
{
    NETIO_RECEIVE_TRACE(netio_intf,src_addr,dst_addr,packet);
    return RTI_FALSE;
}

#define NETIO_ACK_TRACE(n,src,pid) \
        LOG_printDebug("ack (%d,%u)\n",pid->high,pid->low)

RTI_BOOL
DDS_DataWriterInterface_ack(NETIO_Interface_T *netio,
                            struct NETIO_Address *source,
                            NETIO_PacketId_T *packet_id)
{
    NETIO_ACK_TRACE(netio,source,packet_id);
    return RTI_TRUE;
}

#define NETIO_REQUEST_TRACE(n,src,pkt,pid) \
        LOG_printDebug("request\n")

RTI_BOOL
DDS_DataWriterInterface_request(NETIO_Interface_T *netio,
                                struct NETIO_Address *source,
                                struct NETIO_Address *dest,
                                NETIO_Packet_T **packet,
                                NETIO_PacketId_T *packet_id)
{
    struct DDS_DataWriterInterface *dwio = (struct DDS_DataWriterInterface *)netio;
    DDSHST_WriterSample_T *a_sample = NULL;
    struct REDA_SequenceNumber sn;
    struct RTI_TransformCDR_Sample *cdr_sample;
    
    /* return FALSE upon unrecoverable failure */
    /* otherwise, returns TRUE, whether data valid or invalid */

    NETIO_REQUEST_TRACE(netio,source,packet,packet_id);

    DB_Database_lock(dwio->property.datawriter->config.db);

    sn = *(struct REDA_SequenceNumber*)packet_id;
    if (DDSHST_Writer_request_sample(dwio->property.datawriter->wh,
                                        &a_sample, &sn)
                                        != DDSHST_RETCODE_SUCCESS)
    {
        *packet = &dwio->property.datawriter->lost_data_packet;
        dwio->property.datawriter->lost_data_packet.info.valid_data = 0;
    }
    else
    {
        cdr_sample = (struct RTI_TransformCDR_Sample *)a_sample;
        *packet = &cdr_sample->packet;
    }

    DB_Database_unlock(dwio->property.datawriter->config.db);
    return RTI_TRUE;
}

#define NETIO_RETURN_LOAN_TRACE(n,src,pkt,pid) \
        LOG_printDebug("return loan\n")

RTI_BOOL
DDS_DataWriterInterface_return_loan(NETIO_Interface_T *netio,
                                    struct NETIO_Address *source,
                                    NETIO_Packet_T *packet,
                                    NETIO_PacketId_T *packet_id)
{
    struct DDS_DataWriterInterface *dwio =
                                       (struct DDS_DataWriterInterface *)netio;
    RTI_BOOL retval = RTI_FALSE;
    struct REDA_SequenceNumber sn;

    NETIO_RETURN_LOAN_TRACE(n,src,pkt,pid);

    if (packet == &dwio->property.datawriter->lost_data_packet)
    {
        return RTI_TRUE;
    }
    NETIO_Packet_restore_positions(packet);

    DB_Database_lock(dwio->property.datawriter->config.db);

    sn = *(struct REDA_SequenceNumber*)packet_id;
    if (DDSHST_Writer_loan_finished(dwio->property.datawriter->wh, &sn) !=
            DDSHST_RETCODE_SUCCESS)
    {
        DDSC_Log_error(LOG_FAIL_WH_LOAN_FINISHED);
        goto done;
    }

    retval = RTI_TRUE;

done:

    DB_Database_unlock(dwio->property.datawriter->config.db);
    return retval;
}

#define NETIO_DISCARD_TRACE(n,dst,pid) \
    LOG_printDebug("discard")

RTI_BOOL
DDS_DataWriterInterface_discard(NETIO_Interface_T *self,
                                struct NETIO_Address *destination,
                                NETIO_PacketId_T *packet_id)
{

    NETIO_DISCARD_TRACE(self,destination,packet_id);

    return RTI_FALSE;
}

#define NETIO_ADD_ROUTE_TRACE(i,dst,via_i,via_a,p_,e_) \
    LOG_printDebug("add_route")

RTI_BOOL
DDS_DataWriterInterface_add_route(NETIO_Interface_T *intf,
                                  struct NETIO_Address *dst_addr,
                                  NETIO_Interface_T *via_intf,
                                  struct NETIO_Address *via_addr,
                                  struct NETIORouteProperty *property,
                                  RTI_BOOL *existed)
{
    struct DDS_DataWriterInterface *self = (struct DDS_DataWriterInterface *)intf;
    struct NETIORouteEntry *route_entry = NULL;
    struct NETIORouteEntryKey route_key;
    DB_ReturnCode_T dbrc;

    NETIO_ADD_ROUTE_TRACE(intf,dst_addr,via_intf,via_addr,property,existed);

    route_key.destination = *dst_addr;
    route_key.intf = via_intf;
    route_key.intf_address = *via_addr;

    dbrc = DB_Table_select_match(self->_parent._rtable,
                                DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&route_entry,
                                (DB_Key_T)&route_key);

    if (dbrc == DB_RETCODE_NOT_EXISTS)
    {
        if (existed)
        {
            *existed = RTI_FALSE;
        }

        if (DB_Table_create_record(self->_parent._rtable,
                                  (DB_Record_T *)&route_entry) != DB_RETCODE_OK)
        {
            return RTI_FALSE;
        }

        route_entry->destination = *dst_addr;
        route_entry->intf = via_intf;
        route_entry->intf_address = *via_addr;

        dbrc = DB_Table_insert_record(self->_parent._rtable,
                                           (DB_Record_T *)route_entry);
        if (dbrc != DB_RETCODE_OK)
        {
            return RTI_FALSE;
        }
    }
    else if (dbrc == DB_RETCODE_EXISTS)
    {
        if (existed)
        {
            *existed = RTI_TRUE;
        }
    }
    else
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
DDS_DataWriterInterface_delete_route(NETIO_Interface_T *netio,
                                     struct NETIO_Address *dst_addr,
                                     NETIO_Interface_T *via_intf,
                                     struct NETIO_Address *via_addr,
                                     RTI_BOOL *existed)
{
    struct DDS_DataWriterInterface *self = (struct DDS_DataWriterInterface *)netio;
    struct NETIORouteEntry *route_entry = NULL;
    struct NETIORouteEntryKey key;
    DB_ReturnCode_T db_retcode;

    key.destination = *dst_addr;
    key.intf = via_intf;
    key.intf_address = *via_addr;

    db_retcode = DB_Table_remove_record(self->_parent._rtable,
                                       (DB_Record_T *)&route_entry,
                                       &key);
    if (db_retcode == DB_RETCODE_NOT_EXISTS)
    {
        if (existed)
        {
            *existed = RTI_FALSE;
        }
        return RTI_TRUE;
    }

    if (existed)
    {
        *existed = RTI_TRUE;
    }

    if (db_retcode != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    db_retcode = DB_Table_delete_record(self->_parent._rtable,route_entry);
    if (db_retcode != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

#define NETIO_GET_EXTERNAL_INTERFACE_TRACE(i,src,dst,dsta) \
    LOG_printDebug("get_external_interface")

RTI_BOOL
DDS_DataWriterInterface_get_external_interface(NETIO_Interface_T *netio_intf,
                                               struct NETIO_Address *src_addr,
                                               NETIO_Interface_T **dst_intf,
                                               struct NETIO_Address *dst_addr)
{
    NETIO_GET_EXTERNAL_INTERFACE_TRACE(netio_intf,src_addr,dst_intf,dst_addr);

    *dst_intf = netio_intf;

    return RTI_TRUE;
}

RTI_BOOL
DDS_DataWriterInterface_set_state(NETIO_Interface_T *netio,
                                  NETIO_InterfaceState_T state)
{
    return RTI_TRUE;
}

RTI_PRIVATE struct NETIO_InterfaceI DDS_DataWriterInterface_g_intf =
{
    {DEFAULT_COMPONENT_RUN},
    DDS_DataWriterInterface_send,           /* send */
    DDS_DataWriterInterface_ack,            /* ack */
    DDS_DataWriterInterface_request,        /* request */
    DDS_DataWriterInterface_return_loan,    /* return_loan*/
    DDS_DataWriterInterface_discard,        /* discard */
    DDS_DataWriterInterface_add_route,      /* add_route */
    DDS_DataWriterInterface_delete_route,   /* remove_peer */
    NULL,                                   /* reserve_public_address */
    NULL,                                   /* bind */
    NULL,                                   /* unbind */
    DDS_DataWriterInterface_receive,        /* receive */
    DDS_DataWriterInterface_get_external_interface, /* get_external interface */
    NULL,                                   /* external_bind */
    NULL,                                    /* external_unbind */
    DDS_DataWriterInterface_set_state,
    NULL
};

/* ------------------------------------------------------------------------ */
/*                   Plugin factory                                         */
/* ------------------------------------------------------------------------ */

RTI_PRIVATE RT_Component_T*
DDS_DataWriterInterfaceFactory_create_component(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentProperty *property,
        struct RT_ComponentListener *listener)
{
    struct DDS_DataWriterInterface *retval = NULL;

    retval = DDS_DataWriterInterface_create(
            (struct DDS_DataWriterInterfaceFactory*)factory,
            (const struct DDS_DataWriterInterfaceProperty *const)property,
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
DDS_DataWriterInterfaceFactory_delete_component(
                                        struct RT_ComponentFactory *factory,
                                        RT_Component_T *component)
{
    struct DDS_DataWriterInterface *self =
                                    (struct DDS_DataWriterInterface*)component;

    DDS_DataWriterInterface_delete(self);
}

RTI_PRIVATE struct RT_ComponentFactory*
DDS_DataWriterInterfaceFactory_initialize(
        struct RT_ComponentFactoryProperty*property,
        struct RT_ComponentFactoryListener *listener);

RTI_PRIVATE void
DDS_DataWriterInterfaceFactory_finalize(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener);

RTI_PRIVATE struct RT_ComponentFactoryI DDS_DataWriterInterfaceFactory_g_intf =
{
    0, /* TODO: Define constant */
    DDS_DataWriterInterfaceFactory_initialize,
    DDS_DataWriterInterfaceFactory_finalize,
    DDS_DataWriterInterfaceFactory_create_component,
    DDS_DataWriterInterfaceFactory_delete_component
};

RTI_PRIVATE
struct DDS_DataWriterInterfaceFactory DDS_DataWriterInterfaceFactory_g_factory =
{
  {
     &DDS_DataWriterInterfaceFactory_g_intf,
     NULL
  }
};

RTI_PRIVATE struct RT_ComponentFactory*
DDS_DataWriterInterfaceFactory_initialize(
        struct RT_ComponentFactoryProperty *property,
        struct RT_ComponentFactoryListener *listener)
{
    struct DDS_DataWriterInterfaceFactory *factory =
                                    &DDS_DataWriterInterfaceFactory_g_factory;

    factory->instance_counter = 0;
    factory->_initialized = RTI_FALSE;
    DDS_DataWriterInterfaceFactory_g_factory._parent._factory = &factory->_parent;

    return &DDS_DataWriterInterfaceFactory_g_factory._parent;
}

RTI_PRIVATE void
DDS_DataWriterInterfaceFactory_finalize(
        struct RT_ComponentFactory *c_factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener)
{
    struct DDS_DataWriterInterfaceFactory *factory =
            (struct DDS_DataWriterInterfaceFactory *)c_factory;

    factory->instance_counter = 0;
}

struct RT_ComponentFactoryI*
DDS_DataWriterInterfaceFactory_get_interface(void)
{
    return &DDS_DataWriterInterfaceFactory_g_intf;
}

