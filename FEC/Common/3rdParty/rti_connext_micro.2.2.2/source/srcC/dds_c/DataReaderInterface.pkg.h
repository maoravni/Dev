/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataReaderInterface_pkg_h
#define DataReaderInterface_pkg_h

struct DDS_DataReaderInterfaceProperty
{
    struct NETIO_InterfaceProperty _parent;
    struct NETIO_Address intf_address;
    struct DDS_DataReaderImpl *datareader;
};


#define DDS_DataReaderInterfaceProperty_INITIALIZER \
{\
    NETIO_InterfaceProperty_INITIALIZER,\
    NETIO_Address_INITIALIZER \
}

typedef enum
{
    REMOTE_WRITERSTATE_NOT_ALIVE,
    REMOTE_WRITERSTATE_ALIVE
} RemoteWriterState_t;

struct DataReaderRouteEntry
{
    struct NETIORouteEntry _parent;
};

struct DataReaderBindEntry
{
    struct NETIOBindEntry _parent;
    RemoteWriterState_t writer_state;
    RTI_UINT32 activity_count;
    RTI_UINT32 last_activity_count;
    RTI_UINT32 strength;
};

struct DDS_DataReaderInterface
{
    struct NETIO_Interface _parent;
    struct DDS_DataReaderInterfaceProperty property;
    struct NETIO_InterfaceListener listener;
    struct DDS_DataReaderInterfaceFactory *factory;
    NETIO_InterfaceState_T state;
};

struct RT_ComponentFactoryI*
DDS_DataReaderInterfaceFactory_get_interface(void);

#endif /* DataReaderInterface_pkg_h */
