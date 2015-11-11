/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_rtps_h
#define netio_rtps_h


#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef reda_epoch_h
#include "reda/reda_epoch.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef osapi_system_h
#include "osapi/osapi_system.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef netio_common_h
#include "netio/netio_common.h"
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

#ifdef __cplusplus
extern "C"
{
#endif

struct NETIO_RtpsPortParam
{
    /*e \dref_RtpsWellKnownPorts_t_port_base
     */
    RTI_INT32 port_base;

    /*e \dref_RtpsWellKnownPorts_t_domain_id_gain
     */
    RTI_INT32 domain_id_gain;

    /*e \dref_RtpsWellKnownPorts_t_participant_id_gain
     */
    RTI_INT32 participant_id_gain;

    /*e \dref_RtpsWellKnownPorts_t_builtin_multicast_port_offset
     */
    RTI_INT32 builtin_multicast_port_offset;

    /*e \dref_RtpsWellKnownPorts_t_builtin_unicast_port_offset
     */
    RTI_INT32 builtin_unicast_port_offset;

    /*e \dref_RtpsWellKnownPorts_t_user_multicast_port_offset
     */
    RTI_INT32 user_multicast_port_offset;

    /*e \dref_RtpsWellKnownPorts_t_user_unicast_port_offset
     */
    RTI_INT32 user_unicast_port_offset;
};

struct NETIO_RtpsPortData
{
    /*e \dref_RtpsWellKnownPorts_t_domain_id
     */
    RTI_INT32 domain_id;

    /*e \dref_RtpsWellKnownPorts_t_port_param
     */
    struct NETIO_RtpsPortParam port_param;
};

NETIODllExport RTI_BOOL
NETIO_rtps_calculate_user_port(void *param,RTI_UINT32 port_base,RTI_INT32 index,struct NETIO_Address *address);

NETIODllExport RTI_BOOL
NETIO_rtps_calculate_meta_port(void *param,RTI_UINT32 port_base,RTI_INT32 index,struct NETIO_Address *address);


#include "netio/netio_rtps_impl.h"

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* netio_rtps_h */
