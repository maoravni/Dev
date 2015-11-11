/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef LOOPInterface_pkg_h
#define LOOPInterface_pkg_h

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

struct RT_ComponentFactoryI*
LOOPInterfaceFactory_get_interface(void);

#endif /* LOOPInterface_pkg_h */
