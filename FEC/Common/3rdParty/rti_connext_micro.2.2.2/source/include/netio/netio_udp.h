/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_udp_h
#define netio_udp_h


/*e \file
  \brief NETIO Route API
*/
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef reda_string_h
#include "reda/reda_string.h"
#endif
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif
#ifndef netio_dll_h
#include "netio/netio_dll.h"
#endif
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_route_h
#include "netio/netio_route.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct UDP_InterfaceProperty
{
    struct NETIO_InterfaceProperty _parent;
};

#define UDP_InterfaceProperty_INITIALIZER \
{\
    NETIO_InterfaceProperty_INITIALIZER\
}

extern NETIODllVariable struct UDP_InterfaceFactoryProperty UDP_INTERFACE_FACTORY_PROPERTY_DEFAULT;

struct UDP_NatEntry
{
    struct NETIO_Address local_address;
    struct NETIO_Address public_address;
};

#define UDP_NatEntry_INITIALIZER \
{\
    NETIO_Address_INITIALIZER,\
    NETIO_Address_INITIALIZER,\
}

REDA_DEFINE_SEQUENCE(UDP_NatEntrySeq,struct UDP_NatEntry)

#define UDP_NatEntrySeq_INITIALIZER \
                        REDA_DEFINE_SEQUENCE_INITIALIZER(struct UDP_NatEntry)

struct UDP_InterfaceFactoryProperty
{
    struct NETIO_InterfaceFactoryProperty _parent;

    struct REDA_StringSeq allow_interface;

    struct REDA_StringSeq deny_interface;

    RTI_INT32 max_send_buffer;

    RTI_INT32 max_receive_buffer;

    RTI_INT32 multicast_ttl;

    RTI_BOOL is_default_interface;

    REDA_String_T multicast_interface;

    struct UDP_NatEntrySeq nat;
};

#define UDP_InterfaceFactoryProperty_INITIALIZER \
{\
    NETIO_InterfaceFactoryProperty_INITIALIZER,\
    REDA_StringSeq_INITIALIZER,\
    REDA_StringSeq_INITIALIZER,\
    (8*1024),\
    (8*1024),\
    1,\
    RTI_TRUE,\
    NULL,\
    UDP_NatEntrySeq_INITIALIZER \
}

#define UDP_INTERFACE_INTERFACE_ID RT_MKINTERFACEID(\
            RT_COMPONENT_CLASS_NETIO,RT_COMPONENT_INSTANCE_UDP)

NETIODllExport RTI_BOOL
UDP_InterfaceFactoryProperty_initialize(struct UDP_InterfaceFactoryProperty *);

NETIODllExport RTI_BOOL
UDP_InterfaceFactoryProperty_finalize(struct UDP_InterfaceFactoryProperty *);

NETIODllExport RTI_BOOL
UDP_InterfaceFactoryProperty_copy(struct UDP_InterfaceFactoryProperty *,
                                 struct UDP_InterfaceFactoryProperty *in);

NETIODllExport struct RT_ComponentFactoryI*
UDP_InterfaceFactory_get_interface(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* netio_udp_h */
