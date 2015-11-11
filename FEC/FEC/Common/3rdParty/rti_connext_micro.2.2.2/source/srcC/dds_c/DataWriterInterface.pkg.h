/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataWriterInterface_pkg_h
#define DataWriterInterface_pkg_h

struct DDS_DataWriterInterfaceProperty
{
    struct NETIO_InterfaceProperty _parent;
    struct NETIO_Address intf_address;
    struct DDS_DataWriterImpl *datawriter;
    RTI_UINT32 max_send_fanout;
};


#define DDS_DataWriterInterfaceProperty_INITIALIZER \
{\
    NETIO_InterfaceProperty_INITIALIZER,\
    NETIO_Address_INITIALIZER, \
    NULL, \
    8  \
}

struct DDS_DataWriterInterface
{
    struct NETIO_Interface _parent;
    struct DDS_DataWriterInterfaceProperty property;
    struct NETIO_InterfaceListener listener;
    struct DDS_DataWriterInterfaceFactory *factory;
    DB_Index_T rtable_intf_index;
    struct NETIO_AddressSeq send_dests_seq;
};

struct RT_ComponentFactoryI*
DDS_DataWriterInterfaceFactory_get_interface(void);

#endif /* DataWriterInterface_pkg_h */
