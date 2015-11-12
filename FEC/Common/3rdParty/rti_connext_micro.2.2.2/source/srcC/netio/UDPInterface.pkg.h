/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef UDPInterface_pkg_h
#define UDPInterface_pkg_h

struct UDPRouteEntry
{
    struct NETIORouteEntry _parent;
};

struct UDP_Interface
{
    struct NETIO_Interface _parent;
    struct UDP_InterfaceProperty property;
    struct UDP_InterfaceFactory *factory;
    int send_socket;
    struct sockaddr_in socket_addr;
    struct NETIO_InterfaceListener listener;
    DB_Table_T rx_thread_table;
    RTI_UINT32 ref_count;
    struct NETIO_Address mc_bind_address;
    RTI_BOOL multicast_enabled;

    /* NAT */
    DB_Table_T nat;
    DB_Index_T nat_local_idx;
    DB_Index_T nat_public_idx;
};

struct UDP_InterfaceFactory
{
    struct RT_ComponentFactory _parent;
    RTI_INT32 instance_counter;
    RTI_BOOL _initialized;
    struct UDP_InterfaceFactoryProperty *property;
    struct RT_ComponentFactoryListener listener;
};

struct UDPBindEntry
{
    struct NETIOBindEntry _parent;
};

struct UDPPortEntry
{
    struct NETIO_Address source;
    struct sockaddr_in sock_addr;
    NETIO_Packet_T _rx_buffer;
    struct OSAPI_Thread *_rx_thread;
    RTI_UINT32 _ref_count;
    struct UDP_Interface *_udp_intf;
    int _sock;
};

#endif /* UDPInterface_pkg_h */
