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
#ifndef netio_interface_h
#include "netio/netio_interface.h"
#endif

/*** SOURCE_BEGIN ***/

RTI_INT32
NETIO_Interface_compare_route(DB_Select_T opcode,RTI_INT32 flags,
                             const DB_Record_T op1, void *op2, void *op3)
{
    return OSAPI_Memory_compare(op1,op2,sizeof(struct NETIORouteEntryKey));
}

RTI_INT32
NETIO_Interface_compare_bind(DB_Select_T opcode,RTI_INT32 flags,
                            const DB_Record_T op1, void *op2, void *op3)
{
    return OSAPI_Memory_compare(op1,op2,sizeof(struct NETIOBindEntryKey));
}

RTI_BOOL
NETIO_Interface_Table_name_from_id(char *tbl_name,
                                  union RT_ComponentFactoryId *id,
                                  char suffix,
                                  RTI_INT32 instance)
{
    RTI_UINT32 i,j,d;

    i = 0;
    while (id->_name._name[i])
    {
        tbl_name[i] = id->_name._name[i];
        ++i;
    }

    tbl_name[i++] = suffix;
    for (j = 28; (j >= 0) && (j <= 28); j -= 4, ++i)
    {
        d = ((instance >> j) & 0xf);
        tbl_name[i] = d + (d > 9 ? 'a' : '0');
    }
    tbl_name[i] = 0;

    return RTI_TRUE;
}

RTI_BOOL
NETIO_Interface_initialize(struct NETIO_Interface *netio,
                          struct NETIO_InterfaceI *netio_intf,
                          const struct NETIO_InterfaceProperty *const property,
                          const struct NETIO_InterfaceListener *const listener)
{
    RT_Component_initialize(&netio->_parent,
                           &netio_intf->_parent,
                           0,
                           (property ? &property->_parent : NULL),
                           (listener ? &listener->_parent : NULL));

    if (listener)
    {
        netio->_listener = *listener;
    }
    netio->_rtable = NULL;
    netio->_btable = NULL;

    return RTI_TRUE;
}

RTI_BOOL
NETIO_Interface_finalize(struct NETIO_Interface *netio)
{
    RT_Component_finalize(&netio->_parent);

    return RTI_TRUE;
}
