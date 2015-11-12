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

struct NETIO_RouteResolverRecord
{
    struct NETIO_Address address;
    struct NETIO_Netmask netmask;
    NETIO_Interface_T *intf;
};

struct NETIO_RouteResolver
{
    struct NETIO_RouteResolverProperty property;
    DB_Table_T route_table;
    DB_Database_T db;
};

/* NOTE: 33 entries, 0-32 bits! */
RTI_PRIVATE RTI_INT32 NETIO_masks[]=
{
        0x00000000,0x00000001,0x00000003,0x00000007,
        0x0000000f,0x0000001f,0x0000003f,0x0000007f,
        0x000000ff,0x000001ff,0x000003ff,0x000007ff,
        0x00000fff,0x00001fff,0x00003fff,0x00007fff,
        0x0000ffff,0x0001ffff,0x0003ffff,0x0007ffff,
        0x000fffff,0x001fffff,0x003fffff,0x007fffff,
        0x00ffffff,0x01ffffff,0x03ffffff,0x07ffffff,
        0x0fffffff,0x1fffffff,0x3fffffff,0x7fffffff,
        0xffffffff
};

#define NETIO_calculate_mask_index(bits_,i_) \
(bits_ >= ((i_+1)*NETIO_NETMASK_MASK_BITS_PER_UNIT) ? \
        NETIO_NETMASK_MASK_BITS_PER_UNIT : (bits_ % 33))

/*** SOURCE_BEGIN ***/

RTI_PRIVATE RTI_INT32
NETIO_RouteResolver_compare_entry(DB_Select_T opcode,RTI_INT32 flags,
                               const DB_Record_T op1, void *op2, void *op3)
{
    struct NETIO_RouteResolverRecord *left = (struct NETIO_RouteResolverRecord*)op1;
    struct NETIO_RouteResolverRecord *right = (struct NETIO_RouteResolverRecord*)op2;
    RTI_INT32 diff;
    RTI_INT32 left_mask_index;
    RTI_INT32 left_mask_value;
    RTI_INT32 i,j;

    diff = left->address.kind - right->address.kind;
    if (diff)
    {
        return diff;
    }

    if (left->netmask.bits > right->netmask.bits)
    {
        return -1;
    }

    if (left->netmask.bits < right->netmask.bits)
    {
        return 1;
    }

    /* Netmask is of same size, use address to sort */
    j = left->netmask.bits <= 32 ? 1 : (left->netmask.bits / NETIO_NETMASK_MASK_BITS_PER_UNIT);
    for (i = 0; i < j; ++i)
    {
        left_mask_index = NETIO_calculate_mask_index(left->netmask.bits,i);
        left_mask_value = left->netmask.mask[i] & NETIO_masks[left_mask_index];
        diff = ((left->address.value.as_int32.value[i] & left_mask_value) -
                (right->address.value.as_int32.value[i] & left_mask_value));

        if (diff || (!diff && (left->netmask.bits <= (32*(i+1)))))
        {
            break;
        }
    }

    return ((diff < 0) ? 1 : (diff > 0 ? -1 : 0));
}

NETIO_RouteResolver_T*
NETIO_RouteResolver_new(DB_Database_T db,
                     const char* const name,
                     struct NETIO_RouteResolverProperty *const property)
{
    NETIO_RouteResolver_T *rtable = NULL;
    NETIO_RouteResolver_T *rval = NULL;
    struct DB_TableProperty tbl_property = DB_TableProperty_INITIALIZER;
    DB_ReturnCode_T dbrc;

    LOG_PRECONDITION_P3((db == NULL) || (name == NULL) || (property == NULL),
                        return NULL,
                        OSAPI_LOG_PTR_LOW(db),
                        OSAPI_LOG_PTR_LOW(name),
                        OSAPI_LOG_PTR_LOW(property))

    OSAPI_Heap_allocateStructure(&rtable,struct NETIO_RouteResolver);
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
            db,name,sizeof(struct NETIO_RouteResolverRecord),
            NULL,NETIO_RouteResolver_compare_entry,&tbl_property);

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
NETIO_RouteResolver_delete(NETIO_RouteResolver_T *r_table)
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

NETIODllExport RTI_BOOL
NETIO_RouteResolver_add_interface(NETIO_RouteResolver_T *r_table,
                               NETIO_Interface_T *intf,
                               struct NETIO_Address *address,
                               struct NETIO_Netmask *netmask,
                               RTI_BOOL *exists)
{
    struct NETIO_RouteResolverRecord *r_entry = NULL;
    DB_ReturnCode_T dbrc;
    RTI_BOOL rval = RTI_FALSE;
    struct NETIO_RouteResolverRecord key;

    LOG_PRECONDITION_P3((r_table == NULL) || (address == NULL) || (intf == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(r_table),
                        OSAPI_LOG_PTR_LOW(address),
                        OSAPI_LOG_PTR_LOW(intf))

    if (exists)
    {
        *exists = RTI_FALSE;
    }

    key.address = *address;
    key.netmask = *netmask;
    key.intf = intf;

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

    *r_entry = key;

    dbrc = DB_Table_insert_record(r_table->route_table,(DB_Record_T)r_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        NETIO_Log_error_p1(NETIO_LOG_ROUTETABLE_ROUTE_CREATE_FAILED,dbrc);
        goto done;
    }

    rval = RTI_TRUE;

done:
    return rval;
}

NETIODllExport RTI_BOOL
NETIO_RouteResolver_delete_interface(NETIO_RouteResolver_T *r_table,
                                  NETIO_Interface_T *intf,
                                  struct NETIO_Address *address,
                                  struct NETIO_Netmask *netmask,
                                  RTI_BOOL *exists)
{
    struct NETIO_RouteResolverRecord key;
    struct NETIO_RouteResolverRecord *r_entry = NULL;
    DB_ReturnCode_T dbrc;

    key.address = *address;
    key.netmask = *netmask;
    key.intf = intf;

    dbrc = DB_Table_remove_record(r_table->route_table,
                    (DB_Record_T*)&r_entry,(DB_Key_T)&key);
    if (dbrc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    dbrc = DB_Table_delete_record(r_table->route_table,(DB_Record_T)r_entry);

    return (dbrc == DB_RETCODE_OK);
}


RTI_PRIVATE RTI_BOOL
NETIO_address_match(struct NETIO_Address *adddress,struct NETIO_RouteResolverRecord *r_entry)
{
    RTI_INT32 diff;
    RTI_INT32 left_mask_index;
    RTI_INT32 left_mask_value;
    RTI_INT32 i,j;

    if (r_entry->address.kind != adddress->kind)
    {
        return RTI_FALSE;
    }

    j = r_entry->netmask.bits <= 32 ? 1 : (r_entry->netmask.bits / NETIO_NETMASK_MASK_BITS_PER_UNIT);
    for (i = 0; i < j; ++i)
    {
        left_mask_index = NETIO_calculate_mask_index(r_entry->netmask.bits,i);
        left_mask_value = r_entry->netmask.mask[i] & NETIO_masks[left_mask_index];
        diff = ((r_entry->address.value.as_int32.value[i] & left_mask_value) -
                (adddress->value.as_int32.value[i] & left_mask_value));

        if (diff || (!diff && (r_entry->netmask.bits <= (32*(i+1)))))
        {
            break;
        }
    }

    return diff ? RTI_FALSE : RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
NETIO_RouteResolver_route_update(NETIO_RouteResolver_T *r_table,
                              NETIO_Interface_T *src_intf,
                              struct NETIO_Address *dst_reader,
                              struct NETIO_Address *via_address,
                              struct NETIORouteProperty *property,
                              RTI_BOOL *route_existed,
                              RTI_BOOL add_route)
{
    DB_Cursor_T cursor = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIO_RouteResolverRecord *r_entry;
    RTI_BOOL brc = RTI_FALSE;

    if (route_existed)
    {
        *route_existed = RTI_FALSE;
    }
    (void)DB_Table_select_all(r_table->route_table,DB_TABLE_DEFAULT_INDEX,&cursor);
    dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&r_entry);
    while (dbrc == DB_RETCODE_OK)
    {
        if (NETIO_address_match(via_address,r_entry))
        {
            if (add_route)
            {
                brc = NETIO_Interface_add_route(
                        (NETIO_Interface_T*)src_intf,dst_reader,
                        r_entry->intf,via_address,property,route_existed);
            }
            else
            {
                brc = NETIO_Interface_delete_route(
                        (NETIO_Interface_T*)src_intf,dst_reader,
                        r_entry->intf,via_address,route_existed);
            }
            break;
        }
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&r_entry);
    }

    brc = RTI_TRUE;

    if (cursor != NULL)
    {
        DB_Cursor_finish(r_table->route_table,cursor);
    }

    return brc;
}

void
NETIO_RouteResolver_print_route_table(NETIO_RouteResolver_T *r_table)
{
    DB_Cursor_T cursor = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIO_RouteResolverRecord *r_entry;

    (void)DB_Table_select_all(r_table->route_table,DB_TABLE_DEFAULT_INDEX,&cursor);
    dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&r_entry);
    while (dbrc == DB_RETCODE_OK)
    {
        OSAPI_Stdio_printf("kind=%d,port=%d,address=%d/%d/%d/%d,mask_bits=%d,mask=%x/%x/%x/%x\n",
        r_entry->address.kind,
        r_entry->address.port,
        r_entry->address.value.as_int32.value[0],
        r_entry->address.value.as_int32.value[1],
        r_entry->address.value.as_int32.value[2],
        r_entry->address.value.as_int32.value[3],
        r_entry->netmask.bits,
        r_entry->netmask.mask[0],
        r_entry->netmask.mask[1],
        r_entry->netmask.mask[2],
        r_entry->netmask.mask[3]);
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&r_entry);
    }
    if (cursor != NULL)
    {
        DB_Cursor_finish(r_table->route_table,cursor);
    }
}

RTI_BOOL
NETIO_RouteResolver_add_route(NETIO_RouteResolver_T *r_table,
                           NETIO_Interface_T *src_intf,
                           struct NETIO_Address *dst_reader,
                           struct NETIO_Address *via_address,
                           struct NETIORouteProperty *property,
                           RTI_BOOL *route_existed)
{
    return NETIO_RouteResolver_route_update(
            r_table,src_intf,dst_reader,via_address,property,route_existed,RTI_TRUE);
}

RTI_BOOL
NETIO_RouteResolver_delete_route(NETIO_RouteResolver_T *r_table,
                              NETIO_Interface_T *src_intf,
                              struct NETIO_Address *dst_reader,
                              struct NETIO_Address *via_address,
                              RTI_BOOL *route_existed)
{
    return NETIO_RouteResolver_route_update(
            r_table,src_intf,dst_reader,via_address,NULL,route_existed,RTI_FALSE);
}

