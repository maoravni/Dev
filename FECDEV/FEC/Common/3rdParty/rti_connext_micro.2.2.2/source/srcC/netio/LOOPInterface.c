/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "osapi/osapi_config.h"
#include "util/util_makeheader.h"
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef db_api_h
#include "db/db_api.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef netio_log_h
#include "netio/netio_log.h"
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

struct LOOPRouteEntry
{
    struct NETIORouteEntry _parent;
};

struct LOOPInterface
{
    struct NETIO_Interface _parent;
    struct NETIO_Address local_address;
    struct NETIO_Address global_address;
    struct LOOPInterfaceProperty property;
    RTI_INT32 index;
    RTI_UINT32 packet_tx;
    RTI_UINT32 packet_rx;
    RTI_UINT32 packet_drop;
    struct LOOPInterfaceFactory *factory;
};

struct LOOPInterfaceFactory
{
    struct RT_ComponentFactory _parent;
    RTI_INT32 instance_counter;
    RTI_BOOL _initialized;
};

RTI_PRIVATE struct NETIO_InterfaceI LOOPInterface_g_intf;

/*** SOURCE_BEGIN ***/

void
LOOPInterface_finalize(struct LOOPInterface *netio_intf)
{
    struct NETIORouteEntry *route = NULL;
    struct NETIOBindEntry *bind = NULL;
    DB_Cursor_T cursor = NULL;
    DB_ReturnCode_T dbrc;

    NETIO_Interface_finalize(&netio_intf->_parent);

    dbrc = DB_Table_select_all_default(netio_intf->_parent._rtable,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&route);
        if (dbrc == DB_RETCODE_OK)
        {
            dbrc = DB_Table_delete_record(
                                netio_intf->_parent._rtable,(DB_Record_T*)route);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(netio_intf->_parent._rtable,cursor);

    cursor = NULL;
    dbrc = DB_Table_select_all_default(netio_intf->_parent._btable,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&bind);
        if (dbrc == DB_RETCODE_OK)
        {
            dbrc = DB_Table_delete_record(
                                netio_intf->_parent._btable,(DB_Record_T*)bind);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(netio_intf->_parent._btable,cursor);

    DB_Database_delete_table(netio_intf->property._parent._parent.db,
                            netio_intf->_parent._rtable);

    DB_Database_delete_table(netio_intf->property._parent._parent.db,
                            netio_intf->_parent._btable);
}

void
LOOPInterface_delete(struct LOOPInterface *netio_intf)
{
    LOOPInterface_finalize(netio_intf);
    OSAPI_Heap_freeStructure(netio_intf);
}

RTI_BOOL
LOOPInterface_initialize(struct LOOPInterface *test_intf,
                   struct LOOPInterfaceFactory *factory,
                   const struct LOOPInterfaceProperty *const property,
                   const struct NETIO_InterfaceListener *const listener)
{
    struct DB_TableProperty tbl_property = DB_TableProperty_INITIALIZER;
    char tbl_name[16];
    union RT_ComponentFactoryId id;

    if (!NETIO_Interface_initialize(&test_intf->_parent,
                           &LOOPInterface_g_intf,
                           (property ? &property->_parent : NULL),
                           (listener ? listener : NULL)))
    {
        return RTI_FALSE;
    }

    id._value = factory->_parent._id._value;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'r',
                                      factory->instance_counter);

    tbl_property.max_records = property->_parent.max_routes;
    if (DB_Database_create_table(&test_intf->_parent._rtable,
                                property->_parent._parent.db,
                                &tbl_name[0],
                                sizeof(struct NETIORouteEntry),
                                NULL,
                                NETIO_Interface_compare_route,
                                &tbl_property) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    tbl_property.max_records = property->_parent.max_binds;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'b',
                                      factory->instance_counter);
    if (DB_Database_create_table(&test_intf->_parent._btable,
                                property->_parent._parent.db,
                                &tbl_name[0],
                                sizeof(struct NETIOBindEntry),
                                NULL,
                                NETIO_Interface_compare_bind,
                                &tbl_property) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    if (property)
    {
        test_intf->property = *property;
    }

    test_intf->index = factory->instance_counter;
    test_intf->factory = factory;
    ++factory->instance_counter;

    return RTI_TRUE;
}

struct LOOPInterface*
LOOPInterface_create(struct LOOPInterfaceFactory *factory,
                      const struct LOOPInterfaceProperty *const property,
                      const struct NETIO_InterfaceListener *const listener)
{
    struct LOOPInterface *lo_intf;

    LOG_PRECONDITION_P2((factory == NULL) ||
                        (property == NULL),
                        return NULL,
                        OSAPI_LOG_PTR_LOW(factory),
                        OSAPI_LOG_PTR_LOW(property))

    OSAPI_Heap_allocateStructure(&lo_intf,struct LOOPInterface);

    if (!LOOPInterface_initialize(lo_intf,
                                  factory,
                                  property,
                                  listener))
    {
        return NULL;
    }

    return lo_intf;
}



RTI_BOOL
LOOPInterface_sendI(struct LOOPInterface *self,
                   struct NETIO_Address *address,
                   NETIO_Packet_T *packet)
{
    DB_Cursor_T db_handle;
    DB_ReturnCode_T dbrc;
    struct NETIOBindEntry *bind_entry;
    struct NETIOBindEntryKey bind_key;
    RTI_INT32 i;

    for (i = 0; i < NETIO_AddressSeq_get_length(packet->dests); ++i)
    {
        bind_key.source = packet->source;
        bind_key.destination = *NETIO_AddressSeq_get_reference(packet->dests, i);

        if (self->property.mode == LOOP_INTERFACEMODE_LOOPBACK)
        {
            if (DB_Table_select_all_default(
                    self->_parent._btable,
                    &db_handle) != DB_RETCODE_OK)
            {
                return RTI_FALSE;
            }

            dbrc = DB_Cursor_get_next(db_handle,(DB_Record_T*)&bind_entry);
            while ((dbrc != DB_RETCODE_NO_DATA) &&
                    (dbrc != DB_RETCODE_BAD_PARAMETER))
            {
                if (!NETIO_Interface_compare_bind(
                        DB_SELECTOPCODE_EQUAL,
                        DB_SELECT_OP2_AS_KEY,
                        (struct NETIOBindEntryKey*)bind_entry,
                        &bind_key,
                        NULL))
                {
                    NETIO_Interface_receive(bind_entry->intf,
                            &bind_key.source,
                            address,
                            packet);
                }
                dbrc = DB_Cursor_get_next(db_handle,(DB_Record_T)&bind_entry);
            }
            DB_Cursor_finish(self->_parent._btable,db_handle);
        }
    }

    return RTI_TRUE;
}

RTI_BOOL
LOOPInterface_send(NETIO_Interface_T *netio_intf,
                   struct NETIO_Interface *source,
                   struct NETIO_Address *address,
                   NETIO_Packet_T *packet)
{
    struct LOOPInterface *self = (struct LOOPInterface *)netio_intf;

    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (source == NULL) ||
                        (address == NULL) ||
                        (packet == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(source),
                        OSAPI_LOG_PTR_LOW(address),
                        OSAPI_LOG_PTR_LOW(packet))

    return LOOPInterface_sendI(self, address, packet);
}

RTI_BOOL
LOOPInterface_ack(NETIO_Interface_T *netio_intf,
                       struct NETIO_Address *source,
                       NETIO_PacketId_T packet_id)
{
    LOG_PRECONDITION_P2((netio_intf == NULL) ||
                        (source == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(source))

    return RTI_FALSE;
}

RTI_BOOL
LOOPInterface_request(NETIO_Interface_T *netio_intf,
                           struct NETIO_Address *source,
                           NETIO_Packet_T **packet,
                           NETIO_PacketId_T packet_id)
{
    LOG_PRECONDITION_P3((netio_intf == NULL) ||
                        (source == NULL) ||
                        (packet == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(source),
                        OSAPI_LOG_PTR_LOW(packet))

    return RTI_FALSE;
}

RTI_BOOL
LOOPInterface_return_loan(NETIO_Interface_T *netio_intf,
                               struct NETIO_Address *source,
                               NETIO_Packet_T **packet,
                               NETIO_PacketId_T packet_id)
{
    LOG_PRECONDITION_P3((netio_intf == NULL) ||
                        (source == NULL) ||
                        (packet == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(source),
                        OSAPI_LOG_PTR_LOW(packet))

    return RTI_FALSE;
}

RTI_BOOL
LOOPInterface_discard(NETIO_Interface_T *netio_intf,
                           struct NETIO_Address *destination,
                           NETIO_PacketId_T packet_id)
{
    LOG_PRECONDITION_P2((netio_intf == NULL) ||
                        (destination == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(destination))

    return RTI_FALSE;
}

RTI_BOOL
LOOPInterface_add_route(NETIO_Interface_T *netio_intf,
                            struct NETIO_Address *dst_addr,
                            NETIO_Interface_T *via_intf,
                            struct NETIO_Address *via_addr,
                            struct NETIORouteProperty *property,
                            RTI_BOOL *existed)
{
    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (dst_addr == NULL) ||
                        (via_intf == NULL) ||
                        (via_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr),
                        OSAPI_LOG_PTR_LOW(via_intf),
                        OSAPI_LOG_PTR_LOW(via_addr))

    return RTI_TRUE;
}

RTI_BOOL
LOOPInterface_delete_route(NETIO_Interface_T *netio_intf,
                                struct NETIO_Address *dst_addr,
                                NETIO_Interface_T *via_intf,
                                struct NETIO_Address *via_addr,
                                RTI_BOOL *existed)
{
    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (dst_addr == NULL) ||
                        (via_intf == NULL) ||
                        (via_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr),
                        OSAPI_LOG_PTR_LOW(via_intf),
                        OSAPI_LOG_PTR_LOW(via_addr))

    return RTI_TRUE;
}

RTI_BOOL
LOOPInterface_bind(NETIO_Interface_T *netio_intf,
                   struct NETIO_Address *src_addr,
                   struct NETIOBindProperty *property,
                   RTI_BOOL *existed)
{
    LOG_PRECONDITION_P2((netio_intf == NULL) ||
                        (src_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(src_addr))

    return RTI_TRUE;
}

RTI_BOOL
LOOPInterface_unbind(NETIO_Interface_T *netio_intf,
                          struct NETIO_Address *src_addr,
                          NETIO_Interface_T *dst_intf,
                          RTI_BOOL *existed)
{
    LOG_PRECONDITION_P3((netio_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_intf == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_intf))

    return RTI_TRUE;
}

RTI_BOOL
LOOPInterface_bind_external(NETIO_Interface_T *src_intf,
                            struct NETIO_Address *src_addr,
                            NETIO_Interface_T *dst_intf,
                            struct NETIO_Address *dst_addr,
                            struct NETIOBindProperty *property,
                            RTI_BOOL *existed)
{
    struct LOOPInterface *self = (struct LOOPInterface *)src_intf;
    struct NETIOBindEntry *bind_entry = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIOBindEntryKey bind_key;

    LOG_PRECONDITION_P4((src_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_intf == NULL) ||
                        (dst_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(src_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr))

    bind_key.source = *src_addr;
    bind_key.destination = *dst_addr;

    dbrc = DB_Table_select_match(self->_parent._btable,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T *)&bind_entry,
                                &bind_key);
    if (dbrc == DB_RETCODE_OK)
    {
        return RTI_TRUE;
    }

    if (dbrc != DB_RETCODE_NOT_EXISTS)
    {
        return RTI_FALSE;
    }


    if (DB_Table_create_record(self->_parent._btable,
                              (DB_Record_T *)&bind_entry)
                              != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    OSAPI_Memory_zero(bind_entry,sizeof(struct NETIOBindEntry));
    bind_entry->source = *src_addr;
    bind_entry->destination = *dst_addr;
    bind_entry->intf = dst_intf;

    dbrc = DB_Table_insert_record(self->_parent._btable,
                                (DB_Record_T *)bind_entry);

    if ((dbrc != DB_RETCODE_OK) && (dbrc != DB_RETCODE_EXISTS))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
LOOPInterface_unbind_external(NETIO_Interface_T *src_intf,
                              struct NETIO_Address *src_addr,
                              NETIO_Interface_T *dst_intf,
                              struct NETIO_Address *dst_addr,
                              RTI_BOOL *existed)
{
    struct LOOPInterface *self = (struct LOOPInterface *)src_intf;
    struct NETIOBindEntry *bind_entry = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIOBindEntryKey bind_key;

    LOG_PRECONDITION_P4((src_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_intf == NULL) ||
                        (dst_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(src_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr))

    bind_key.source = *src_addr;
    bind_key.destination = *dst_addr;

    dbrc = DB_Table_select_match(self->_parent._btable,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T *)&bind_entry,
                                &bind_key);
    if (dbrc == DB_RETCODE_NOT_EXISTS)
    {
        return RTI_TRUE;
    }

    if (dbrc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    dbrc = DB_Table_delete_record(self->_parent._btable,
                                 (DB_Record_T *)bind_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
LOOPInterface_get_external_interface(NETIO_Interface_T *netio_intf,
                                     struct NETIO_Address *src_addr,
                                     NETIO_Interface_T **dst_intf,
                                     struct NETIO_Address *dst_addr)
{
    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_intf == NULL) ||
                        (dst_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr))

    *dst_intf = netio_intf;

    return RTI_TRUE;
}

RTI_BOOL
LOOPInterface_receive(NETIO_Interface_T *netio_intf,
                      struct NETIO_Address *src_addr,
                      struct NETIO_Address *dst_addr,
                      NETIO_Packet_T *packet)
{
    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_addr == NULL) ||
                        (packet == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_addr),
                        OSAPI_LOG_PTR_LOW(packet))

    return RTI_FALSE;
}

RTI_BOOL
LOOPInterface_set_state(NETIO_Interface_T *src_intf,
                        NETIO_InterfaceState_T state)
{
    LOG_PRECONDITION((src_intf == NULL),return RTI_FALSE)

    return RTI_FALSE;
}

RTI_BOOL
LOOPInterface_resolve_ipv4_adddress(
                        NETIO_Interface_T *netio_intf,
                        char *address_string,
                        struct NETIO_Address *address_value)
{
    NETIO_Address_init(address_value,0);

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
LOOPInterface_reserve_address(NETIO_Interface_T *self,
                              struct NETIO_AddressSeq *req_addr,
                              struct NETIO_AddressSeq *resvd_addr,
                              struct NETIOBindProperty *property)
{
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
LOOPInterface_release_address(NETIO_Interface_T *self,
                              struct NETIO_Address *address)
{
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
LOOPInterface_get_route_table(NETIO_Interface_T *netio_intf,
                             struct NETIO_AddressSeq *address,
                             struct NETIO_NetmaskSeq *netmask)
{
    RTI_INT32 max_size;
    RTI_INT32 cur_addr_len;
    struct NETIO_Address src_address;
    RTI_INT32 i;

    cur_addr_len = NETIO_AddressSeq_get_length(address);
    max_size = NETIO_AddressSeq_get_maximum(address) - cur_addr_len;

    if (max_size == 0)
    {
        return RTI_TRUE;
    }

    NETIO_Address_init(&src_address,NETIO_ADDRESS_KIND_GUID);
    NETIO_AddressSeq_set_length(address,cur_addr_len+1);
    *NETIO_AddressSeq_get_reference(address,cur_addr_len) = src_address;

    NETIO_NetmaskSeq_set_length(netmask,cur_addr_len+1);
    NETIO_NetmaskSeq_get_reference(netmask,cur_addr_len)->bits = 128;

    for (i = 0; i < 4; ++i)
    {
        NETIO_NetmaskSeq_get_reference(netmask,cur_addr_len)->mask[i] =
                                        src_address.value.as_int32.value[i];
    }

    return RTI_TRUE;
}

/******************************************************************************
 *
 * LOOP Component Interface
 */
RTI_PRIVATE struct NETIO_InterfaceI LOOPInterface_g_intf =
{
    {DEFAULT_COMPONENT_RUN},
    LOOPInterface_send,                 /* send */
    NULL,                               /* ack */
    NULL,                               /* request */
    NULL,                               /* return_loan */
    NULL,                               /* discard */
    LOOPInterface_add_route,            /* add_peer */
    LOOPInterface_delete_route,         /* delete_route */
    LOOPInterface_reserve_address,
    LOOPInterface_bind,
    LOOPInterface_unbind,
    LOOPInterface_receive,
    LOOPInterface_get_external_interface,
    LOOPInterface_bind_external,
    LOOPInterface_unbind_external,
    LOOPInterface_set_state,
    LOOPInterface_release_address,
    LOOPInterface_resolve_ipv4_adddress,
    LOOPInterface_get_route_table
};

/* ------------------------------------------------------------------------ */
/*                   Plugin factory                                         */
/* ------------------------------------------------------------------------ */

RTI_PRIVATE RT_Component_T*
LOOPInterfaceFactory_create_component(struct RT_ComponentFactory *factory,
                                      struct RT_ComponentProperty *property,
                                      struct RT_ComponentListener *listener)
{
    struct LOOPInterface *retval = NULL;

    retval = LOOPInterface_create(
            (struct LOOPInterfaceFactory*)factory,
            (const struct LOOPInterfaceProperty *const)property,
            (const struct NETIO_InterfaceListener *const)listener);

    return &retval->_parent._parent;
}

RTI_PRIVATE void
LOOPInterfaceFactory_delete_component(struct RT_ComponentFactory *factory,
                                      RT_Component_T *component)
{
    struct LOOPInterface *self = (struct LOOPInterface *)component;

    LOOPInterface_delete(self);
}

RTI_PRIVATE struct RT_ComponentFactory*
LOOPInterfaceFactory_initialize(struct RT_ComponentFactoryProperty*property,
                                struct RT_ComponentFactoryListener *listener);

RTI_PRIVATE void
LOOPInterfaceFactory_finalize(struct RT_ComponentFactory *factory,
                              struct RT_ComponentFactoryProperty **property,
                              struct RT_ComponentFactoryListener **listener);

RTI_PRIVATE struct RT_ComponentFactoryI LOOPInterfaceFactory_g_intf =
{
    INTRA_INTERFACE_INTERFACE_ID,
    LOOPInterfaceFactory_initialize,
    LOOPInterfaceFactory_finalize,
    LOOPInterfaceFactory_create_component,
    LOOPInterfaceFactory_delete_component
};

RTI_PRIVATE
struct LOOPInterfaceFactory LOOPInterfaceFactory_g_factory =
{
  {
     &LOOPInterfaceFactory_g_intf,
     NULL
  }
};

RTI_PRIVATE struct RT_ComponentFactory *
LOOPInterfaceFactory_initialize(struct RT_ComponentFactoryProperty *property,
                                struct RT_ComponentFactoryListener *listener)
{
    struct LOOPInterfaceFactory *factory = &LOOPInterfaceFactory_g_factory;

    LOOPInterfaceFactory_g_factory._parent._factory = &factory->_parent;
    factory->_initialized = RTI_FALSE;
    factory->instance_counter = 0;

    return &LOOPInterfaceFactory_g_factory._parent;
}

RTI_PRIVATE void
LOOPInterfaceFactory_finalize(struct RT_ComponentFactory *factory,
                              struct RT_ComponentFactoryProperty **property,
                              struct RT_ComponentFactoryListener **listener)
{
}

struct RT_ComponentFactoryI*
LOOPInterfaceFactory_get_interface(void)
{
    return &LOOPInterfaceFactory_g_intf;
}
