/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_loopback_h
#define netio_loopback_h


/*e \file
  \brief NETIO Route API
*/

#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif

#ifndef netio_dll_h
#include "netio/netio_dll.h"
#endif

#ifndef osapi_config_h
#include "osapi/osapi_config.h"
#endif

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
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

typedef enum
{
    LOOP_INTERFACEMODE_UNDEFINED,
    LOOP_INTERFACEMODE_LOOPBACK
} LOOPInterfaceMode_t;


struct LOOPInterfaceAsyncSendEntry
{
    RTI_BOOL is_valid;
    struct NETIO_Address address;
    NETIO_Packet_T *packet;
};

struct LOOPInterfaceAsyncSendState
{
    struct LOOPInterfaceAsyncSendEntry *buf; /* array of sends */
    RTI_SIZE_T buf_add_index;
    RTI_SIZE_T buf_take_index;
    RTI_SIZE_T buf_len;
    struct OSAPI_Mutex *buf_lock;

    struct OSAPI_Thread *send_thread;
    struct OSAPI_Mutex *send_signal;
    RTI_BOOL send_running;
};

struct LOOPInterfaceProperty
{
    struct NETIO_InterfaceProperty _parent;
    LOOPInterfaceMode_t mode;
    RTI_BOOL async_send;
};

#define LOOPInterfaceProperty_INITIALIZER \
{\
    NETIO_InterfaceProperty_INITIALIZER,\
    LOOP_INTERFACEMODE_LOOPBACK,\
    RTI_FALSE /* async_send */ \
}

#define INTRA_INTERFACE_INTERFACE_ID RT_MKINTERFACEID(\
            RT_COMPONENT_CLASS_NETIO,RT_COMPONENT_INSTANCE_INTRA)

NETIODllExport struct RT_ComponentFactoryI*
LOOPInterfaceFactory_get_interface(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* netio_loopback_h */
