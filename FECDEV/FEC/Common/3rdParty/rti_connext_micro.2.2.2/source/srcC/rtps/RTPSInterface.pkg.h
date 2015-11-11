/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef RTPSInterface_pkg_h
#define RTPSInterface_pkg_h



/******************************************************************************/
/* Writer flags: bitMask of Writer QoS/state */
typedef RTI_UINT32 RTPSWriterFlags;
#define RTPS_WRITER_FLAG_NONE      0x00
#define RTPS_WRITER_FLAG_RELIABLE  0x01
#define RTPS_WRITER_FLAG_HB_EVENT_ACTIVE 0x02

/* Reader flags: bitMask of Reader QoS/state */
typedef RTI_UINT32 RTPSReaderFlags;
#define RTPS_READER_FLAG_NONE      0x00
#define RTPS_READER_FLAG_RELIABLE  0x01


struct RTPSRemoteReader
{
    RTI_UINT8 reliable;
     
#if RTPS_RELIABILITY
    struct MIGRtpsBitmap bitmap; /* ackBitmap */
    REDA_Epoch_T epoch; /* lastAckEpoch */
#endif

};

RTI_SIZE_T RTPSRemoteReader_g_size = sizeof(struct RTPSRemoteReader);


struct RTPSRemoteWriter
{
    RTI_UINT8 reliable; 
    struct MIGRtpsBitmap bitmap; /* dataReceived (used by both BE and SR) */

#if RTPS_RELIABILITY
    REDA_Epoch_T epoch; /* lastHbEpoch */
#endif

};
RTI_SIZE_T RTPSRemoteWriter_g_size = sizeof(struct RTPSRemoteWriter);

typedef struct REDA_SequenceNumber RTPSSampleId_t;

struct RTPSWindowSample 
{
    struct REDA_SequenceNumber sn; /* RTPS SN (same as DDS) */
    RTI_UINT16 ack_count;
    RTI_UINT8 flags;
};


struct RTPSWindow
{
    struct RTPSWindowSample *samples; /* circular buffer array */

    /* Two indices:
       1) Head is first valid entry
       2) Tail is last valid entry
     
       When empty, all three indices are the same.
       When full, Head and Free are same; Tail is (Head - 1) % max_length
       Otherwise, Free is (Tail + 1) % max_length
    */
    RTI_SIZE_T head_idx; 
    RTI_SIZE_T tail_idx; 

    /* Number of valid samples in window */
    RTI_UINT16 current_length;

    /* Max number of samples in window */
    RTI_UINT32 max_length;

    RTI_UINT8 flags;
};

#define RTPS_WINDOW_FLAG_NONE                   (0x00)
#define RTPS_WINDOW_FLAG_PENDING_LAST_WRITE     (0x01)

#define RTPSPacket_is_last_write_for_sn(pkt_) \
((pkt_)->info.rtps_flags & NETIO_RTPS_FLAGS_LAST_WRITE_FOR_SN)


/* Local Writer State */
struct RTPSWriter
{
    NETIO_Interface_T *upstr_intf; /* upstream interface to request */ 
    RTPSWriterFlags flags;
    struct REDA_SequenceNumber firstSn;
    struct REDA_SequenceNumber lastSn;

    struct RTPSWindow send_window;

#if RTPS_RELIABILITY
    RTI_UINT32 hbEpoch;
    struct OSAPI_NtpTime hbPeriod;
    RTI_INT32 samplesPerHb;
    RTI_INT32 piggybackSampleCount;
    OSAPITimeoutHandle_t hbEvent; /* periodic HB */
    RTI_SIZE_T reliable_reader_count;
#endif
};

RTI_SIZE_T RTPSWriter_g_size = sizeof(struct RTPSWriter);


/* Local Reader State */
struct RTPSReader
{
    RTPSReaderFlags flags;

#if RTPS_RELIABILITY
    RTI_UINT32 ackEpoch;
#endif
};

RTI_SIZE_T RTPSReader_g_size = sizeof(struct RTPSReader);

/*************************************/
struct RTPSXportEntryKey
{
    struct NETIO_Interface *intf; /* transport interface */
    struct NETIO_Address addr; /* transport address */
};

struct RTPSXportEntry
{
    struct NETIO_Interface *intf; /* transport interface */
    struct NETIO_Address addr; /* transport address */
    RTI_UINT32 ref_count;
};

RTI_SIZE_T RTPSXportEntry_g_size = sizeof(struct RTPSXportEntry);


struct RTPSPeerEntry
{
    struct NETIO_Address addr; /* peer address */
    RTI_UINT32 ref_count;
    union {
        struct RTPSRemoteReader reader;
        struct RTPSRemoteWriter writer;
    } info;
};

RTI_SIZE_T RTPSPeerEntry_g_size = sizeof(struct RTPSPeerEntry);


struct RTPSReceiveContext
{
    RTPS_GUID src;
    RTPS_GUID dst;
    struct OSAPI_NtpTime timestamp;
};

/*************************************/
struct RTPSInterface
{
    struct NETIO_Interface _parent;
    struct RTPSInterfaceProperty property;
    struct RTPSInterfaceFactory *factory;
    struct NETIO_InterfaceListener listener;

    /* indexes for route table */
    DB_Index_T rtable_peer_index;
    DB_Index_T rtable_transport_index;
    DB_Index_T btable_peer_index;

    struct RTPSPeerEntry *peers; /* ref to peers array */
    struct RTPSXportEntry *xports; /* ref to transports array */

    struct RTPSReceiveContext context; 

    NETIO_Packet_T packet; /* scratch packet */
    void *packet_buf; /* scratch packet's buffer */
    
    union
    {
        struct RTPSWriter writer;
        struct RTPSReader reader;
    } endpoint;

    /* send() sequence of destinations */
    struct NETIO_AddressSeq local_dest_seq;
    struct NETIO_AddressSeq dwnstrm_dest_seq;
};

RTI_SIZE_T RTPSInterface_g_size = sizeof(struct RTPSInterface);

/*************************************/
struct RTPSInterfaceFactory
{
    struct RT_ComponentFactory _parent;
    struct RTPSInterface _external; /* */
    OSAPI_Timer_t timer;
    struct OSAPI_System *clock;

    /* matched remote reader state */
    RTI_INT32 maxRemoteEndpointCount;
    RTI_INT32 maxLocalEndpointCount;

    RTI_INT32 instance_counter;
    RTI_BOOL _initialized;

    REDA_Indexer_T *ext_intf_index;
};

RTI_SIZE_T RTPSInterfaceFactory_g_size = sizeof(struct RTPSInterfaceFactory);

/* forward decl */
RTI_PRIVATE struct RT_ComponentFactoryI RTPSInterfaceFactory_g_intf;

RTI_PRIVATE
struct RTPSInterfaceFactory RTPSInterfaceFactory_g_factory;

/*************************************/

struct RTPSRouteEntry
{
    struct NETIORouteEntry _parent;

    /* ptr to peer state entry */
    struct RTPSPeerEntry *peer_ref;

    /* ptr to transport state entry */
    struct RTPSXportEntry *xport_ref; 

    /* route priority.  Higher priorities get routed to first */
    RTI_INT32 priority;
};

RTI_SIZE_T RTPSRouteEntry_g_size = sizeof(struct RTPSRouteEntry);

#define RTPS_ROUTE_PRIORITY_UNDEFINED   0
#define RTPS_ROUTE_PRIORITY_DEFAULT     1
#define RTPS_ROUTE_PRIORITY_MULTICAST   2

#define RTPS_ROUTE_MODE_ALL 0
#define RTPS_ROUTE_MODE_HIGHEST_PRIO 1

struct RTPSBindProperty
{
    struct NETIOBindProperty _parent;

    RTI_BOOL is_reliable;
};

struct RTPSBindEntry
{
    struct NETIOBindEntry _parent;
    struct RTPSInterface *_rtps_intf;   
    struct RTPSPeerEntry *peer_ref;
};

RTI_SIZE_T RTPSBindEntry_g_size = sizeof(struct RTPSBindEntry);

/**************************************/
/* Max size of locally created packet.
   Needs to be large enough for downstream transport headers. */
#define RTPS_DOWNSTREAM_HEADER_MAX_SIZE (128)

#define RTPS_LOCAL_PACKET_SIZE \
(RTPS_DOWNSTREAM_HEADER_MAX_SIZE + sizeof(struct RTPS_Header) + \
 (3 * sizeof(union RTPS_MESSAGES)))


/*************************************/

#define RTPSInterface_addr_is_stateless_sender(addr_) \
    (*((RTI_INT32*)&((addr_)->entity)) == \
        NETIO_ntohl(MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT))

#define RTPSInterface_addr_is_stateless_receiver(addr_) \
    (*((RTI_INT32*)&((addr_)->entity)) == \
        NETIO_ntohl(MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT))

#ifdef RTI_ENDIAN_LITTLE
#define RTPSInterface_byte_swap(flags_) \
    ((flags_ & MIG_RTPS_ENDIAN_FLAG) == 0)
#else
#define RTPSInterface_byte_swap(flags_) \
    ((flags_ & MIG_RTPS_ENDIAN_FLAG) != 0)    
#endif

/*************************************/


#endif /* RTPSInterface_pkg_h */
