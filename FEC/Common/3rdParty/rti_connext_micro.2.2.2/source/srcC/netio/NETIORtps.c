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

#ifndef netio_rtps_h
#include "netio/netio_rtps.h"
#endif

RTI_PRIVATE RTI_BOOL
NETIO_rtps_calculate_port(void *param,RTI_UINT32 port_base,RTI_INT32 index,struct NETIO_Address *address,RTI_BOOL is_user)
{
    struct NETIO_RtpsPortData *port_data = (struct NETIO_RtpsPortData *)param;
    RTI_UINT32 actual_port;

    if (port_base == 0)
    {
        actual_port = port_data->port_param.port_base;
    }
    else
    {
        actual_port = port_base;
    }

    if ((address->kind == NETIO_ADDRESS_KIND_UDPv4) &&
            ((address->value.ipv4.address >= 0xe0000000) &&
             (address->value.ipv4.address <= 0xefffffff)))
    {
        if (is_user)
        {
            address->port = RTPS_getWellKnownMulticastPort(
                    port_data->domain_id,actual_port,
                    port_data->port_param.domain_id_gain,
                    port_data->port_param.user_multicast_port_offset);
            return RTI_TRUE;
        }

        address->port = RTPS_getWellKnownMulticastPort(
                port_data->domain_id,actual_port,
                port_data->port_param.domain_id_gain,
                port_data->port_param.builtin_multicast_port_offset);
        return RTI_TRUE;
    }

    if (is_user)
    {
        address->port = RTPS_getWellKnownUnicastPort(
                port_data->domain_id,index,actual_port,
                port_data->port_param.domain_id_gain,
                port_data->port_param.participant_id_gain,
                port_data->port_param.user_unicast_port_offset);
        return RTI_TRUE;
    }

    address->port = RTPS_getWellKnownUnicastPort(
            port_data->domain_id,index,actual_port,
            port_data->port_param.domain_id_gain,
            port_data->port_param.participant_id_gain,
            port_data->port_param.builtin_unicast_port_offset);
    return RTI_TRUE;
}

RTI_BOOL
NETIO_rtps_calculate_user_port(void *param,RTI_UINT32 port_base,RTI_INT32 index,struct NETIO_Address *address)
{
    return NETIO_rtps_calculate_port(param,port_base,index,address,RTI_TRUE);
}

RTI_BOOL
NETIO_rtps_calculate_meta_port(void *param,RTI_UINT32 port_base,RTI_INT32 index,struct NETIO_Address *address)
{
    return NETIO_rtps_calculate_port(param,port_base,index,address,RTI_FALSE);
}
