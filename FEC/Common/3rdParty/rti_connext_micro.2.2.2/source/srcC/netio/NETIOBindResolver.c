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
#ifndef reda_string_h
#include "reda/reda_string.h"
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

struct NETIO_BindResolver
{
    struct NETIO_BindResolverProperty property;
    DB_Table_T route_table;
    DB_Database_T db;
};

struct NETIO_RouteTableRecord
{
    struct NETIO_Address address;
    RTI_UINT32 netmask;
    NETIO_Interface_T *intf;
};

struct NETIO_RouteTable
{
    struct NETIO_RouteResolverProperty property;
    DB_Table_T route_table;
    DB_Database_T db;
};

struct NETIO_RouteEntryKey
{
    NETIO_RouteKind_T kind;
    RT_ComponentFactoryId_T id;
    struct NETIO_Address address;
};

struct NETIO_RouteEntry
{
    struct NETIO_RouteEntryKey key;
    NETIO_Interface_T *intf;
    NETIO_PortCalculateFunc_T port_resolve;
    void *port_resolve_param;
};

/*** SOURCE_BEGIN ***/


RTI_PRIVATE RTI_INT32
NETIO_BindResolver_compare_entry(DB_Select_T opcode,RTI_INT32 flags,
                               const DB_Record_T op1, void *op2, void *op3)
{
    struct NETIO_RouteEntry *record_left = (struct NETIO_RouteEntry*)op1;
    const struct NETIO_RouteEntryKey *id_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        id_right = (const struct NETIO_RouteEntryKey *)op2;
    }
    else
    {
        id_right = &((struct NETIO_RouteEntry*)op2)->key;
    }

    if (record_left->key.kind > id_right->kind)
    {
        return 1;
    }

    if (record_left->key.kind < id_right->kind)
    {
        return -1;
    }

    if (record_left->key.id._value._high > id_right->id._value._high)
    {
        return 1;
    }

    if (record_left->key.id._value._high < id_right->id._value._high)
    {
        return -1;
    }

    if (record_left->key.id._value._low > id_right->id._value._low)
    {
        return 1;
    }

    if (record_left->key.id._value._low < id_right->id._value._low)
    {
        return -1;
    }

    if (record_left->key.address.port > id_right->address.port)
    {
        return 1;
    }

    if (record_left->key.address.port < id_right->address.port)
    {
        return -1;
    }

    if (record_left->key.address.value.rtps_guid.host_id > id_right->address.value.rtps_guid.host_id)
    {
        return 1;
    }

    if (record_left->key.address.value.rtps_guid.host_id < id_right->address.value.rtps_guid.host_id)
    {
        return -1;
    }

    if (record_left->key.address.value.rtps_guid.app_id > id_right->address.value.rtps_guid.app_id)
    {
        return 1;
    }

    if (record_left->key.address.value.rtps_guid.app_id < id_right->address.value.rtps_guid.app_id)
    {
        return -1;
    }

    if (record_left->key.address.value.rtps_guid.instance_id > id_right->address.value.rtps_guid.instance_id)
    {
        return 1;
    }

    if (record_left->key.address.value.rtps_guid.instance_id < id_right->address.value.rtps_guid.instance_id)
    {
        return -1;
    }

    if (record_left->key.address.value.rtps_guid.object_id > id_right->address.value.rtps_guid.object_id)
    {
        return 1;
    }

    if (record_left->key.address.value.rtps_guid.object_id < id_right->address.value.rtps_guid.object_id)
    {
        return -1;
    }

    return 0;
}

NETIO_BindResolver_T*
NETIO_BindResolver_new(DB_Database_T db,
                     const char* const name,
                     struct NETIO_BindResolverProperty *const property)
{
    NETIO_BindResolver_T *rtable = NULL;
    NETIO_BindResolver_T *rval = NULL;
    struct DB_TableProperty tbl_property = DB_TableProperty_INITIALIZER;
    DB_ReturnCode_T dbrc;

    LOG_PRECONDITION_P3((db == NULL) || (name == NULL) || (property == NULL),
                        return NULL,
                        OSAPI_LOG_PTR_LOW(db),
                        OSAPI_LOG_PTR_LOW(name),
                        OSAPI_LOG_PTR_LOW(property))

    OSAPI_Heap_allocateStructure(&rtable,struct NETIO_BindResolver);
    if (rtable == NULL)
    {
        NETIO_Log_error(NETIO_LOG_ROUTETABLE_NEW_ALLOC_FAILED);
        return NULL;
    }

    rtable->property = *property;
    rtable->db = db;

    tbl_property.max_cursors = 1;
    tbl_property.max_indices = 1;
    tbl_property.max_records = rtable->property.max_routes;

    dbrc = DB_Database_create_table(&rtable->route_table,
            db,name,sizeof(struct NETIO_RouteEntry),
            NULL,NETIO_BindResolver_compare_entry,&tbl_property);

    if (dbrc != DB_RETCODE_OK)
    {
        NETIO_Log_error_p1(NETIO_LOG_ROUTETABLE_TABLE_CREATE_FAILED,dbrc);
        goto done;
    }

    rval = rtable;

done:
    return rval;
}

RTI_BOOL
NETIO_BindResolver_delete(NETIO_BindResolver_T* r_table)
{
    DB_ReturnCode_T dbrc;

    LOG_PRECONDITION((r_table == NULL),return RTI_FALSE)

    dbrc = DB_Database_delete_table(r_table->db,r_table->route_table);
    if (dbrc != DB_RETCODE_OK)
    {
        NETIO_Log_error_p1(NETIO_LOG_ROUTETABLE_TABLE_DELETE_FAILED,dbrc);
        return RTI_FALSE;
    }

    OSAPI_Heap_freeStructure(r_table);

    return RTI_TRUE;
}

RTI_BOOL
NETIO_BindResolver_add_route(NETIO_BindResolver_T *r_table,
                           NETIO_RouteKind_T kind,
                           RT_ComponentFactoryId_T *id,
                           struct NETIO_Address *address,
                           NETIO_Interface_T *intf,
                           NETIO_PortCalculateFunc_T port_resolve,
                           void *port_resolve_param,
                           RTI_BOOL *exists)
{
    struct NETIO_RouteEntry *r_entry = NULL;
    DB_ReturnCode_T dbrc;
    RTI_BOOL rval = RTI_FALSE;
    struct NETIO_RouteEntryKey key;

    LOG_PRECONDITION_P4((r_table == NULL) || (id == NULL) ||
                        (address == NULL) || (intf == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(r_table),
                        OSAPI_LOG_PTR_LOW(id),
                        OSAPI_LOG_PTR_LOW(address),
                        OSAPI_LOG_PTR_LOW(intf))

    if (exists)
    {
        *exists = RTI_FALSE;
    }

    RT_ComponentFactoryId_clear(&key.id);
    key.id = *id;
    key.address = *address;
    key.kind = kind;

    dbrc = DB_Table_select_match(r_table->route_table,
            DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&r_entry,(DB_Key_T)&key);
    if (dbrc == DB_RETCODE_OK)
    {
        if (exists)
        {
            *exists = RTI_TRUE;
        }
        rval = RTI_TRUE;
        goto done;
    }

    dbrc = DB_Table_create_record(r_table->route_table,(DB_Record_T*)&r_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        NETIO_Log_error_p1(NETIO_LOG_ROUTETABLE_ROUTE_CREATE_FAILED,dbrc);
        goto done;
    }

    r_entry->key = key;
    r_entry->intf = intf;
    r_entry->port_resolve = port_resolve;
    r_entry->port_resolve_param = port_resolve_param;

    dbrc = DB_Table_insert_record(r_table->route_table,(DB_Record_T)r_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        NETIO_Log_error_p1(NETIO_LOG_ROUTETABLE_ROUTE_CREATE_FAILED,dbrc);
        goto done;
    }

    LOG_printDebug("added bind route: %d/%s/%d/%d/%x\n",kind,key.id._name._name,key.address.kind,key.address.port,key.address.value.ipv4.address);

    rval = RTI_TRUE;

done:
    return rval;
}

RTI_BOOL
NETIO_BindResolver_delete_route(NETIO_BindResolver_T *r_table,
                              NETIO_RouteKind_T kind,
                              RT_ComponentFactoryId_T *id,
                              struct NETIO_Address *address,
                              RTI_BOOL *existed)
{
    struct NETIO_RouteEntry *r_entry = NULL;
    DB_ReturnCode_T dbrc;
    RTI_BOOL rval = RTI_FALSE;
    struct NETIO_RouteEntryKey key;

    LOG_PRECONDITION_P3((r_table == NULL) || (id == NULL) ||
                        (address == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(r_table),
                        OSAPI_LOG_PTR_LOW(id),
                        OSAPI_LOG_PTR_LOW(address))

    if (existed)
    {
        *existed = RTI_FALSE;
    }


    RT_ComponentFactoryId_clear(&key.id);
    key.id = *id;
    key.address = *address;
    key.kind = kind;

    LOG_printDebug("trying to delete route: %d/%s/%d/%d/%x\n",kind,key.id._name._name,key.address.kind,key.address.port,key.address.value.ipv4.address);

    dbrc = DB_Table_remove_record(r_table->route_table,
                                 (DB_Record_T*)&r_entry,(DB_Key_T)&key);
    if (dbrc == DB_RETCODE_NOT_EXISTS)
    {
        if (existed)
        {
            *existed = RTI_FALSE;
        }
        rval = RTI_TRUE;
        goto done;
    }

    if (dbrc != DB_RETCODE_OK)
    {
        NETIO_Log_error_p1(NETIO_LOG_ROUTETABLE_ROUTE_CREATE_FAILED,dbrc);
        goto done;
    }

    if (existed)
    {
        *existed = RTI_TRUE;
    }

    dbrc = DB_Table_delete_record(r_table->route_table,(DB_Record_T)r_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        NETIO_Log_error_p1(NETIO_LOG_ROUTETABLE_ROUTE_CREATE_FAILED,dbrc);
    }
    
    LOG_printDebug("delete bind route: %d/%s/%d/%d/%x\n",kind,key.id._name._name,key.address.kind,key.address.port,key.address.value.ipv4.address);

    rval = RTI_TRUE;

done:
    return rval;
}

RTI_BOOL
NETIO_BindResolver_lookup_by_address(NETIO_BindResolver_T* r_table,
                                      NETIO_RouteKind_T kind,
                                      RT_ComponentFactoryId_T *id,
                                      struct NETIO_Address *address,
                                      NETIO_Interface_T **intf)
{
    struct NETIO_RouteEntry *r_entry = NULL;
    DB_ReturnCode_T dbrc;
    RTI_BOOL rval = RTI_FALSE;
    struct NETIO_RouteEntryKey key;

    key.id = *id;
    key.address = *address;
    key.kind = kind;

    LOG_printDebug("lookup: %d/%s/%d/%d/%x\n",kind,key.id._name._name,key.address.kind,key.address.port,key.address.value.ipv4.address);

    dbrc = DB_Table_select_match(r_table->route_table,
            DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&r_entry,(DB_Key_T)&key);

    if (dbrc != DB_RETCODE_OK)
    {
        LOG_printDebug("route does not exist %d/%s/%d/%d/%x\n",kind,key.id._name._name,key.address.kind,key.address.port,key.address.value.ipv4.address);
        rval = RTI_FALSE;
        goto done;
    }

    *intf = r_entry->intf;
    rval = RTI_TRUE;

done:
    return rval;
}

RTI_BOOL
NETIO_BindResolver_lookup_by_name(NETIO_BindResolver_T* r_table,
                                  NETIO_RouteKind_T kind,
                                  RTI_INT32 try_index,
                                  const char *name,
                                  RT_ComponentFactoryId_T *out_id,
                                  NETIO_Interface_T **intf,
                                  struct NETIO_AddressSeq *resolved)
{
    struct NETIO_RouteEntry *r_entry = NULL;
    DB_ReturnCode_T dbrc;
    RTI_BOOL rval = RTI_FALSE;
    struct NETIO_RouteEntryKey key;
    RTI_INT32 len,max_len;
    const char *c_ptr;
    RT_ComponentFactoryId_T id;
    RTI_UINT32 base_port = 0;
    char address_string[1024];
    RTI_INT32 port_index,index;

    LOG_PRECONDITION_P4((r_table == NULL) || (name == NULL) ||
                        (intf == NULL) || (resolved == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(r_table),
                        OSAPI_LOG_PTR_LOW(name),
                        OSAPI_LOG_PTR_LOW(intf),
                        OSAPI_LOG_PTR_LOW(resolved))

    /*
     * Algorithm:
     *
     * An address is on the form
     * [index @]<interface>://[<address>[/port]]
     *
     * - break address string into
     *
     * - A sequence of index ranges (0 if none is given)
     * - interface_name (must be given)
     * - address_string
     * - a port number (0) if none is given
     *
     * - Lookup intf for address 0 (default for any address) based on ifname
     *
     * For each {index,address,port} do:
     *  port = resolve_port(param,port_base,index)
     *  call intf->resolve(port,address_string,resolved);
     * done
     *
     * The result of this is
     *  - the interface
     *  - A sequence of addresses (kind,port,address)
     *
     *  NOTE: This is just address resolution and does not do anything other
     *  then resolve address.
     */
    len = NETIO_AddressSeq_get_length(resolved);
    max_len = NETIO_AddressSeq_get_maximum(resolved);

    if (len == max_len)
    {
        return RTI_TRUE;
    }

    c_ptr = name;
    RT_ComponentFactoryId_clear(&id);

    address_string[0] = 0;

    if (!NETIO_Address_parse(name,&base_port,&index,&id,address_string))
    {
        goto done;
    }

    key.id = id;
    NETIO_Address_init(&key.address,0);
    key.kind = kind;

    dbrc = DB_Table_select_match(r_table->route_table,
            DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&r_entry,(DB_Key_T)&key);
    if (dbrc != DB_RETCODE_OK)
    {
        rval = RTI_FALSE;
        goto done;
    }

    if (try_index != -1)
    {
        index = try_index;
    }
    else
    {
        try_index = 0;
    }

    for (port_index = try_index; (port_index <= index) && (len < max_len); ++port_index)
    {
        len = NETIO_AddressSeq_get_length(resolved);
        if (len < 0)
        {
            rval = RTI_FALSE;
            goto done;
        }
        if (!NETIO_AddressSeq_set_length(resolved,len+1))
        {
            rval = RTI_FALSE;
            goto done;
        }

        if (port_index == try_index)
        {
            rval = NETIO_Interface_resolve_address(r_entry->intf,
                                address_string,
                                NETIO_AddressSeq_get_reference(resolved,len));
            if (!rval)
            {
                goto done;
            }
        }
        else
        {
            *NETIO_AddressSeq_get_reference(resolved,len) =
                            *NETIO_AddressSeq_get_reference(resolved,len-1);
        }

        if (r_entry->port_resolve)
        {
            r_entry->port_resolve(r_entry->port_resolve_param,
                                  base_port,port_index,
                                  NETIO_AddressSeq_get_reference(resolved,len));
        }
        else
        {
            NETIO_AddressSeq_get_reference(resolved,len)->port = base_port;
        }

        len += 1;
    }

    *intf = r_entry->intf;
    *out_id = id;

done:
    return rval;
}

RTI_BOOL
NETIO_BindResolver_reserve_addresses(NETIO_BindResolver_T* r_table,
                                     RTI_INT32 try_index,
                                     struct REDA_StringSeq *transport,
                                     NETIO_RouteKind_T kind,
                                     struct NETIO_AddressSeq *reserved_address,
                                     struct NETIO_AddressSeq *mc_locator_seq,
                                     struct NETIO_AddressSeq *uc_locator_seq)
{
    struct NETIO_AddressSeq candidate_address = NETIO_AddressSeq_INITIALIZER;
    NETIO_Interface_T *netio_intf;
    RTI_INT32 i,k,l;
    RTI_BOOL retval = RTI_FALSE;
    RTI_INT32 mc_len;
    RTI_INT32 uc_len;
    RT_ComponentFactoryId_T id;

    if (!NETIO_AddressSeq_set_maximum(&candidate_address,4))
    {
        return RTI_FALSE;
    }
    NETIO_AddressSeq_set_length(&candidate_address,0);
    NETIO_AddressSeq_set_length(reserved_address,0);
    l = 0;
    for (i = 0; i < REDA_StringSeq_get_length(transport); ++i)
    {
        NETIO_AddressSeq_set_length(&candidate_address,0);
        RT_ComponentFactoryId_clear(&id);
        if (!NETIO_BindResolver_lookup_by_name(r_table,kind,try_index,
                                    *REDA_StringSeq_get_reference(transport,i),
                                    &id,&netio_intf,&candidate_address))
        {
            goto done;
        }

        if (!NETIO_Interface_reserve_address(netio_intf,
                                &candidate_address,reserved_address,NULL))
        {
            for (k = 0; k < NETIO_AddressSeq_get_length(reserved_address); ++k)
            {
                NETIO_Interface_release_address(netio_intf,
                          NETIO_AddressSeq_get_reference(reserved_address,k));
            }
            goto done;
        }

        /* Add the resolved entry to the resource table
         * NOTE: It is not necessary with a port resolver for specific
         * addresses
         */
        mc_len = NETIO_AddressSeq_get_length(mc_locator_seq);
        uc_len = NETIO_AddressSeq_get_length(uc_locator_seq);
        for (k = l; k < NETIO_AddressSeq_get_length(reserved_address); ++k,l++)
        {
            if (!NETIO_BindResolver_add_route(r_table,
                    kind,
                    &id,
                    NETIO_AddressSeq_get_reference(reserved_address,k),
                    netio_intf,NULL,NULL,NULL))
            {
                goto done;
            }
            if ((NETIO_AddressSeq_get_reference(reserved_address,k)->kind == NETIO_ADDRESS_KIND_UDPv4) &&
                    ((NETIO_AddressSeq_get_reference(reserved_address,k)->value.ipv4.address >= 0xe0000000) &&
                     (NETIO_AddressSeq_get_reference(reserved_address,k)->value.ipv4.address <= 0xefffffff)))
            {
                if (NETIO_AddressSeq_set_length(mc_locator_seq,mc_len+1))
                {
                    *NETIO_AddressSeq_get_reference(mc_locator_seq, mc_len) =
                        *NETIO_AddressSeq_get_reference(reserved_address,k);
                    ++mc_len;
                }
            }
            else
            {
                if (NETIO_AddressSeq_set_length(uc_locator_seq,uc_len+1))
                {
                    *NETIO_AddressSeq_get_reference(uc_locator_seq, uc_len) =
                            *NETIO_AddressSeq_get_reference(reserved_address,k);
                    ++uc_len;
                }
            }
        }
    }

    retval = RTI_TRUE;

done:
    if (!NETIO_AddressSeq_finalize(&candidate_address))
    {
        return RTI_FALSE;
    }

    return retval;
}

RTI_BOOL
NETIO_BindResolver_release_addresses(NETIO_BindResolver_T* r_table,
                                      struct REDA_StringSeq *transport,
                                      NETIO_RouteKind_T kind,
                                      struct NETIO_AddressSeq *locator_seq)
{
    struct NETIO_Address netio_address;
    NETIO_Interface_T *netio_intf;
    RTI_INT32 i,j;
    RTI_BOOL retval = RTI_FALSE;
    RTI_INT32 len;
    RT_ComponentFactoryId_T id;

    len = NETIO_AddressSeq_get_length(locator_seq);
    for (i = 0; i < len; ++i)
    {
        netio_address = *NETIO_AddressSeq_get_reference(locator_seq,i);
        for (j = 0; j < REDA_StringSeq_get_length(transport); ++j)
        {
            RT_ComponentFactoryId_clear(&id);
            RT_ComponentFactoryId_set_name(&id,*REDA_StringSeq_get_reference(transport,j));
            if (NETIO_BindResolver_lookup_by_address(r_table,kind,&id,&netio_address,&netio_intf))
            {
                (void)NETIO_Interface_release_address(netio_intf,&netio_address);
                (void)NETIO_BindResolver_delete_route(r_table,kind,&id,&netio_address,NULL);
            }
        }
    }

    retval = RTI_TRUE;
done:
    return retval;
}

RTI_BOOL
NETIO_BindResolver_bind(NETIO_BindResolver_T* r_table,
                        struct REDA_StringSeq *transport,
                        NETIO_RouteKind_T kind,
                        struct NETIO_Address *from_address,
                        NETIO_Interface_T *to_intf,
                        struct NETIO_Address *to_address)
{
    NETIO_Interface_T *netio_intf;
    RTI_INT32 k;
    RT_ComponentFactoryId_T id;
    RTI_BOOL brc = RTI_FALSE;

    for (k = 0; k < REDA_StringSeq_get_length(transport); ++k)
    {
        RT_ComponentFactoryId_clear(&id);
        RT_ComponentFactoryId_set_name(&id,*REDA_StringSeq_get_reference(transport,k));
        if (!NETIO_BindResolver_lookup_by_address(r_table,kind,&id,from_address,&netio_intf))
        {
            continue;
        }
        LOG_printDebug("bind_resolved: %d %d/%d/%x.%x.%x.%x => %d/%d/%x.%x.%x.%x\n\n",
                    kind,
                    from_address->kind,from_address->port,
                    from_address->value.rtps_guid.host_id,
                    from_address->value.rtps_guid.app_id,
                    from_address->value.rtps_guid.instance_id,
                    from_address->value.rtps_guid.object_id,
                    to_address->kind,to_address->port,
                    to_address->value.rtps_guid.host_id,
                    to_address->value.rtps_guid.app_id,
                    to_address->value.rtps_guid.instance_id,
                    to_address->value.rtps_guid.object_id);

        (void)NETIO_Interface_bind_external(
                netio_intf,from_address,to_intf,to_address,
                NULL,NULL);
    }

    brc = RTI_TRUE;

done:
    return brc;
}

RTI_BOOL
NETIO_BindResolver_unbind(NETIO_BindResolver_T* r_table,
                          struct REDA_StringSeq *transport,
                          NETIO_RouteKind_T kind,
                          struct NETIO_Address *from_address,
                          NETIO_Interface_T *to_intf,
                          struct NETIO_Address *to_address)
{
    NETIO_Interface_T *netio_intf;
    RTI_INT32 k;
    RT_ComponentFactoryId_T id;
    RTI_BOOL brc = RTI_FALSE;

    LOG_printDebug("unbind: %d %d/%d/%x.%x.%x.%x => %d/%d/%x.%x.%x.%x\n\n",
                kind,
                from_address->kind,from_address->port,
                from_address->value.rtps_guid.host_id,
                from_address->value.rtps_guid.app_id,
                from_address->value.rtps_guid.instance_id,
                from_address->value.rtps_guid.object_id,
                to_address->kind,to_address->port,
                to_address->value.rtps_guid.host_id,
                to_address->value.rtps_guid.app_id,
                to_address->value.rtps_guid.instance_id,
                to_address->value.rtps_guid.object_id);

    for (k = 0; k < REDA_StringSeq_get_length(transport); ++k)
    {
        RT_ComponentFactoryId_clear(&id);
        RT_ComponentFactoryId_set_name(&id,*REDA_StringSeq_get_reference(transport,k));
        if (!NETIO_BindResolver_lookup_by_address(r_table,kind,&id,from_address,&netio_intf))
        {
#if 0
            LOG_printDebug("didn't find address: %d/%d/%d/%x\n",
                    kind,from_address->kind,from_address->port,from_address->value.ipv4.address);
#endif
            continue;
        }
        LOG_printDebug("unbind_external address: %d/%d/%d/%x\n",
                kind,from_address->kind,from_address->port,from_address->value.ipv4.address);
        (void)NETIO_Interface_unbind_external(
                netio_intf,from_address,to_intf,to_address,
                NULL);
    }

    brc = RTI_TRUE;

done:
    return brc;
}

