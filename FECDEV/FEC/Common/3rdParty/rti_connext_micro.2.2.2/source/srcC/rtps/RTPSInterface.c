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
#ifndef osapi_thread_h
#include "osapi/osapi_thread.h"
#endif
#ifndef osapi_stdio_h
#include "osapi/osapi_stdio.h"
#endif
#ifndef reda_buffer_h
#include "reda/reda_buffer.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef reda_indexer_h
#include "reda/reda_indexer.h"
#endif
#ifndef cdr_time_h
#include "cdr/cdr_time.h"
#endif
#ifndef db_api_h
#include "db/db_api.h"
#endif
#ifndef db_log_h
#include "db/db_log.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef netio_log_h
#include "netio/netio_log.h"
#endif
#ifndef netio_common_h
#include "netio/netio_common.h"
#endif
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_interface_h
#include "netio/netio_interface.h"
#endif
#ifndef rtps_config_h
#include "rtps/rtps_config.h"
#endif
#ifndef rtps_rtps_h
#include "rtps/rtps_rtps.h"
#endif
#ifndef rtps_log_h
#include "rtps/rtps_log.h"
#endif



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



RTI_PRIVATE struct NETIO_InterfaceI RTPSInterface_g_intf; /* fwd decl */

/*** SOURCE_BEGIN ***/

void
RTPSInterface_finalize(struct RTPSInterface *rtps_intf)
{
    DB_ReturnCode_T dbrc;
    DB_Cursor_T cursor;
    struct RTPSRouteEntry *route = NULL;
    struct RTPSBindEntry *bind = NULL;

    if (rtps_intf->property.mode == RTPS_INTERFACEMODE_WRITER)
    {
#if RTPS_RELIABILITY
        OSAPI_Timer_delete_timeout(rtps_intf->factory->timer,
            &rtps_intf->endpoint.writer.hbEvent);
        rtps_intf->endpoint.writer.hbEvent.epoch = -1;

        if (rtps_intf->endpoint.writer.send_window.samples != NULL)
        {
            OSAPI_Heap_freeArray(rtps_intf->endpoint.writer.send_window.samples);
        }
#endif
    }

    /* TODO: have NETIOMgr handle dest seqs */
    NETIO_AddressSeq_finalize(&rtps_intf->local_dest_seq);
    NETIO_AddressSeq_finalize(&rtps_intf->dwnstrm_dest_seq);

    OSAPI_Heap_freeArray(rtps_intf->peers);
    OSAPI_Heap_freeArray(rtps_intf->xports);

    if (rtps_intf->packet_buf != NULL)
    {
        OSAPI_Heap_free(rtps_intf->packet_buf);
    }
    DB_Table_delete_index(rtps_intf->_parent._rtable, 
                         rtps_intf->rtable_peer_index);
    DB_Table_delete_index(rtps_intf->_parent._rtable, 
                         rtps_intf->rtable_transport_index);
    DB_Table_delete_index(rtps_intf->_parent._btable, 
                         rtps_intf->btable_peer_index);

    /* clean up route table */
    dbrc = DB_Table_select_all_default(rtps_intf->_parent._rtable,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&route);
        if (dbrc == DB_RETCODE_OK)
        {
            dbrc = DB_Table_delete_record(rtps_intf->_parent._rtable,
                                         (DB_Record_T)route);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(rtps_intf->_parent._rtable, cursor);
    DB_Database_delete_table(rtps_intf->property._parent._parent.db,
                            rtps_intf->_parent._rtable);

    /* clean up bind table */
    dbrc = DB_Table_select_all_default(rtps_intf->_parent._btable,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&bind);
        if (dbrc == DB_RETCODE_OK)
        {
            dbrc = DB_Table_delete_record(rtps_intf->_parent._btable,
                                         (DB_Record_T)bind);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(rtps_intf->_parent._btable, cursor);
    DB_Database_delete_table(rtps_intf->property._parent._parent.db,
                            rtps_intf->_parent._btable);

    NETIO_Interface_finalize(&rtps_intf->_parent);
}

void
RTPSInterface_delete(struct RTPSInterface *rtps_intf)
{
    struct RTPSInterface *ext_intf = NULL;

    if (rtps_intf != NULL)
    {
        /* remove external interfaces into factory's ext_intf index */
        if (rtps_intf->property.intf_address.value.rtps_guid.object_id == 
                RTPS_PARTICIPANT_ID)
        {
            ext_intf = (struct RTPSInterface *)REDA_Indexer_remove_entry(
               rtps_intf->factory->ext_intf_index,
               &rtps_intf->property.intf_address);
        }
        RTPSInterface_finalize(rtps_intf);
        OSAPI_Heap_freeStructure(rtps_intf);
    }
}

RTI_PRIVATE RTI_INT32
RTPSInterface_compare_bind_peer(DB_Select_T opcode,
                                RTI_INT32 flags,
                                const DB_Record_T op1, 
                                void *op2, 
                                void *op3)
{
    struct RTPSBindEntry *left_record = (struct RTPSBindEntry *)op1;
    struct NETIO_Address *right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        right = (struct NETIO_Address*)op2;
    }
    else
    {
        right = &((struct RTPSBindEntry *)op2)->_parent.source;
    }

    return OSAPI_Memory_compare(&left_record->_parent.source, right,
                               sizeof(struct NETIO_Address));
}


RTI_PRIVATE RTI_INT32
RTPSInterface_compare_route_peer(DB_Select_T opcode,
                                 RTI_INT32 flags,
                                 const DB_Record_T op1, 
                                 void *op2, 
                                 void *op3)
{
    struct RTPSRouteEntry *left_record = (struct RTPSRouteEntry *)op1;
    struct NETIO_Address *right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        right = (struct NETIO_Address*)op2;
    }
    else
    {
        right = &((struct RTPSRouteEntry *)op2)->_parent.destination;
    }

    return OSAPI_Memory_compare(&left_record->_parent.destination, right,
                               sizeof(struct NETIO_Address));
}

RTI_PRIVATE RTI_INT32
RTPSInterface_compare_route_transport(DB_Select_T opcode,
                                      RTI_INT32 flags,
                                      const DB_Record_T op1, 
                                      void *op2, 
                                      void *op3)
{
    struct RTPSRouteEntry *left_record = (struct RTPSRouteEntry *)op1;
    NETIO_Interface_T *intf;
    struct NETIO_Address *intf_address;
    struct NETIO_Address *dest;
    RTI_INT32 priority;
    RTI_INT32 delta;

    /* this compare is only called by select_all, so op2 is always a route
       entry, never a key */
    intf = ((struct RTPSRouteEntry*)op2)->_parent.intf;
    intf_address = &((struct RTPSRouteEntry*)op2)->_parent.intf_address;
    dest = &((struct RTPSRouteEntry*)op2)->_parent.destination;
    priority = ((struct RTPSRouteEntry*)op2)->priority;

    delta = left_record->_parent.intf - intf;
    if (delta)
    {
        return delta;
    }

    delta = OSAPI_Memory_compare(&left_record->_parent.intf_address,
                                intf_address, sizeof(struct NETIO_Address));
    if (delta)
    {
        return delta;
    }

    delta = OSAPI_Memory_compare(&left_record->_parent.destination, dest,
                                 sizeof(struct NETIO_Address));
    if (delta)
    {
        return delta;
    }

    /* higher priority is ordered first */
    return left_record->priority - priority;
}


RTI_BOOL
RTPSInterface_initialize(struct RTPSInterface *rtps_intf,
                        struct RTPSInterfaceFactory *factory,
                        const struct RTPSInterfaceProperty *const property,
                        const struct NETIO_InterfaceListener *const listener)
{
    struct DB_TableProperty tbl_property = DB_TableProperty_INITIALIZER;
    char tbl_name[16];
    union RT_ComponentFactoryId id;
    struct DB_IndexProperty idx_property = DB_IndexProperty_INITIALIZER;

    if (!NETIO_Interface_initialize(&rtps_intf->_parent, /* netio */
                                   &RTPSInterface_g_intf, /* netio_intf */
                                   (property ? &property->_parent : NULL), /* property */
                                   (listener ? listener : NULL) /* listener */))
    {
        RTPS_Log_error(LOG_INTERFACE_FAIL_INITIALIZE);
        goto cleanup;
    }

    /* Route table */
    id._value = factory->_parent._id._value;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'r',factory->instance_counter);
    tbl_property.max_records = property->_parent.max_routes;
    tbl_property.max_indices = 2;
    tbl_property.max_cursors = 2; 

    if (DB_Database_create_table(&rtps_intf->_parent._rtable, /* table */
                                property->_parent._parent.db, /* db */
                                tbl_name, /* name */
                                sizeof(struct RTPSRouteEntry), /* record_size */
                                NULL, /* hash_fn */
                                NETIO_Interface_compare_route, /* compare_fn */
                                &tbl_property /* property */) != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_CREATE_TABLE);
        goto cleanup;
    }

    if (DB_Table_create_index(rtps_intf->_parent._rtable,
                            &rtps_intf->rtable_peer_index,
                            NULL,
                            RTPSInterface_compare_route_peer,
                            &idx_property) != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_CREATE_INDEX);
        goto cleanup;
    }

    if (DB_Table_create_index(rtps_intf->_parent._rtable,
                             &rtps_intf->rtable_transport_index,
                             NULL,
                             RTPSInterface_compare_route_transport,
                             &idx_property) != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_CREATE_INDEX);
        goto cleanup;
    }

    /* Bind table */
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'b',factory->instance_counter);
    tbl_property.max_records = property->max_transport_count; 
    tbl_property.max_cursors = 2; /* */

    if (DB_Database_create_table(&rtps_intf->_parent._btable,
                                property->_parent._parent.db,
                                tbl_name,
                                sizeof(struct RTPSBindEntry),
                                NULL,
                                NETIO_Interface_compare_bind,
                                &tbl_property) != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_CREATE_TABLE);
        goto cleanup;
    }

    if (DB_Table_create_index(rtps_intf->_parent._btable,
                             &rtps_intf->btable_peer_index,
                             NULL,
                             RTPSInterface_compare_bind_peer,
                             &idx_property) != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_CREATE_INDEX);
        goto cleanup;
    }

    rtps_intf->property = *property;
    rtps_intf->factory = factory;
    rtps_intf->_parent.state = NETIO_INTERFACESTATE_CREATED;

    /* Allocate peer and transport arrays */
    OSAPI_Heap_allocateArray(&rtps_intf->peers, 
                            property->max_peer_count, struct RTPSPeerEntry);
    OSAPI_Heap_allocateArray(&rtps_intf->xports, 
                            property->max_transport_count, struct RTPSXportEntry);
    if (rtps_intf->peers == NULL || rtps_intf->xports == NULL)
    {
        RTPS_Log_error(LOG_FAIL_ALLOCATE);
        goto cleanup;
    }

    /* packet */
    rtps_intf->packet_buf = 
        OSAPI_Heap_allocate(property->max_message_buffer_count, 
                           RTPS_LOCAL_PACKET_SIZE);
    if (rtps_intf->packet_buf == NULL)
    {
        RTPS_Log_error(LOG_FAIL_ALLOCATE);
        goto cleanup;
    }

    /* send destinations sequence */
    NETIO_AddressSeq_initialize(&rtps_intf->dwnstrm_dest_seq);
    NETIO_AddressSeq_set_maximum(&rtps_intf->dwnstrm_dest_seq,
                                 (RTI_INT32)property->max_send_fanout);

    /* single-length seq for locally generated sends */
    NETIO_AddressSeq_initialize(&rtps_intf->local_dest_seq);
    NETIO_AddressSeq_set_maximum(&rtps_intf->local_dest_seq, 1);
    NETIO_AddressSeq_set_length(&rtps_intf->local_dest_seq, 1);

    if (property->mode == RTPS_INTERFACEMODE_WRITER)
    {
        struct RTPSWriter *writer = &rtps_intf->endpoint.writer;

        REDA_SequenceNumber_setZero(&writer->firstSn);
        REDA_SequenceNumber_setZero(&writer->lastSn);
        writer->upstr_intf = NULL; /* assigned on bind_external() */
        writer->flags = RTPS_WRITER_FLAG_NONE;

        if (property->reliable)
        {
            writer->flags |= RTPS_WRITER_FLAG_RELIABLE;
        }

#if RTPS_RELIABILITY
        writer->hbEpoch = 0;
        writer->hbEvent.epoch = -1;
        writer->hbEvent._entry = NULL; /* TODO: verify setting ptr -1 is valid */
        writer->piggybackSampleCount = 0;
        writer->reliable_reader_count = 0;

        writer->hbPeriod = property->hb_period;
        writer->samplesPerHb = property->samples_per_hb;

        writer->send_window.max_length = property->window_size;
        writer->send_window.current_length = 0;
        writer->send_window.samples = NULL;
        
        if (property->window_size > 0)
        {
            OSAPI_Heap_allocateArray(&writer->send_window.samples,
                                property->window_size, struct RTPSWindowSample);
            if (writer->send_window.samples == NULL)
            {
                RTPS_Log_error(LOG_FAIL_ALLOCATE);
                goto cleanup;
            }

            writer->send_window.head_idx = 0;
            writer->send_window.tail_idx = 0;
            writer->send_window.flags = RTPS_WINDOW_FLAG_NONE;
        }

#endif

    } 
    else if (property->mode == RTPS_INTERFACEMODE_READER)
    {
        rtps_intf->endpoint.reader.flags = RTPS_READER_FLAG_NONE;
        if (property->reliable)
        {
            rtps_intf->endpoint.reader.flags |= RTPS_READER_FLAG_RELIABLE;
        }
#if RTPS_RELIABILITY
        rtps_intf->endpoint.reader.ackEpoch = 0;
#endif
    }

    /* Builtin DP discovery reader receives anonymously, from all writers with
       well-known Writer ID.  It wont be bound to any peers though, so need
       to assert this interface into bind table */
    if (rtps_intf->property.intf_address.value.rtps_guid.object_id == 
        MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT)
    {
        if (!NETIO_Interface_bind(&rtps_intf->_parent,
                                 &rtps_intf->property.intf_address,
                                 NULL, NULL))
        {
            RTPS_Log_error(LOG_INTERFACE_FAIL_BIND);
            return RTI_FALSE;
        }
    }

    return RTI_TRUE;

cleanup:
    RTPSInterface_finalize(rtps_intf);
    return RTI_FALSE; 
}


RTI_PRIVATE RTI_INT32
RTPS_compare_ext_intf(const void *const record,
                               RTI_BOOL key_is_record,
                               const void *const key)
{
    struct RTPSInterface *intf_left = (struct RTPSInterface *)record;
    struct NETIO_Address *addr_right = (struct NETIO_Address *)key;

    if (key_is_record)
    {
        addr_right = &((struct RTPSInterface *)key)->property.intf_address;
    }
    else
    {
        addr_right = (struct NETIO_Address *)key;
    }

    /* compare participant guid-prefix */
    return OSAPI_Memory_compare(&intf_left->property.intf_address.value,
                                   &addr_right->value,
                                   sizeof(struct NETIO_GuidPrefix));
}

struct RTPSInterface*
RTPSInterface_create(struct RTPSInterfaceFactory *factory,
                     const struct RTPSInterfaceProperty *const property,
                     const struct NETIO_InterfaceListener *const listener)
{
    struct RTPSInterface *rtps_intf = NULL;
    struct RTPSInterface *ext_intf = NULL;
    struct REDA_IndexerProperty index_prop = REDA_IndexerProperty_INITIALIZER;

    if (!factory->_initialized)
    {
        index_prop.max_entries = 16; /* same size as ext_intf_pool */ /* TODO: resource limit */
        factory->ext_intf_index = REDA_Indexer_new(
           RTPS_compare_ext_intf, NULL, &index_prop);
        if (factory->ext_intf_index == NULL)
        {
            RTPS_Log_error(LOG_INTERFACE_FAIL_INITIALIZE);
            return NULL;
        }

        factory->_initialized = RTI_TRUE;
    }

    OSAPI_Heap_allocateStructure(&rtps_intf, struct RTPSInterface);
    if (rtps_intf == NULL)
    {
        RTPS_Log_error(LOG_FAIL_ALLOCATE);
        return NULL;
    }

    if (!RTPSInterface_initialize(rtps_intf, factory, property, listener))
    {
        RTPS_Log_error(LOG_INTERFACE_FAIL_INITIALIZE);
        RTPSInterface_delete(rtps_intf);
        return NULL;
    }

    ++factory->instance_counter;

    /* assert external interfaces into factory's ext_intf index */
    if (property->intf_address.value.rtps_guid.object_id == RTPS_PARTICIPANT_ID)
    {
        ext_intf = (struct RTPSInterface *)REDA_Indexer_find_entry(
           factory->ext_intf_index, &rtps_intf->property.intf_address);
        if (ext_intf == NULL)
        {
            /* new external interface -> add intf to index */
            if (!REDA_Indexer_add_entry(factory->ext_intf_index, rtps_intf))
            {
            }
        }
    }

    return rtps_intf;
}

#define RTPS_ENABLE_SEND_ROUTE 0

#ifdef RTI_ENDIAN_LITTLE
#define RTPS_INTERFACE_ENDIAN_FLAG  MIG_RTPS_ENDIAN_FLAG
#else
#define RTPS_INTERFACE_ENDIAN_FLAG  MIG_RTPS_NO_FLAG
#endif


RTI_PRIVATE RTI_BOOL
RTPSSender_route_packet(struct RTPSInterface *intf,
                        NETIO_Packet_T *packet,
                        RTI_INT32 mode) 
{
    RTI_BOOL ok = RTI_TRUE;
    struct RTPSRouteEntry *route_entry = NULL;
    DB_Cursor_T db_cur;
    DB_ReturnCode_T db_rc;
    struct NETIO_Interface *send_intf = NULL;
    RTI_INT32 i, seq_len = 0;
    struct NETIO_AddressSeq *upstr_dest_seq = NULL;
    struct NETIO_Address *dest_addr = NULL;

    struct NETIO_Address *prev_dest = NULL;
    RTI_INT32 prev_prio = RTPS_ROUTE_PRIORITY_UNDEFINED;

    RTI_SIZE_T pkt_head, pkt_curr, pkt_tail;

    /* Precondition: DB Lock taken by callers (receive, send) */
    db_rc = DB_Table_select_all(intf->_parent._rtable,
                               intf->rtable_transport_index,
                               &db_cur);

    if (db_rc != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_SELECT_ALL);
        return RTI_FALSE;
    }

    upstr_dest_seq = packet->dests;
    packet->dests = &intf->dwnstrm_dest_seq;

    db_rc = DB_Cursor_get_next(db_cur,(DB_Record_T*)&route_entry);
    while (db_rc == DB_RETCODE_OK)
    {
#if 0
        /* Send to routes with highest priority */
        if (mode == RTPS_ROUTE_MODE_HIGHEST_PRIO)
        {
            /* skip routes to the same destination that have lesser priority */
            if (prev_dest == &route_entry->_parent.destination)
            {
                if (prev_prio > route_entry->priority)
                {
                    /* skip send to lower priority route */
                    goto nextRouteEntry;
                }
            }
            else 
            {
                /* first entry of new destination has highest priority */
                prev_prio = route_entry->priority;
            }
            prev_dest = &route_entry->_parent.destination;
        }
#endif
        for (i = 0; i < NETIO_AddressSeq_get_length(upstr_dest_seq); ++i)
        {
            dest_addr =  NETIO_AddressSeq_get_reference(upstr_dest_seq, i);

            /* route to matching (peer) destination */
            if (!OSAPI_Memory_compare(&route_entry->_parent.destination,
                                      dest_addr, sizeof(struct NETIO_Address)))
            {
                /* coalesce sends to the same interface */
                if (seq_len == (RTI_INT32)intf->property.max_send_fanout ||
                    route_entry->_parent.intf != send_intf)
                {
                    NETIO_Packet_save_positions_to(
                       packet, &pkt_head, &pkt_curr, &pkt_tail);
                    if (send_intf != NULL &&
                        !NETIO_Interface_send(send_intf,&intf->_parent,
                                              dest_addr, packet))
                    {
                        RTPS_Log_error(LOG_INTERFACE_FAIL_SEND);
                        ok = RTI_FALSE;
                    }
                    NETIO_Packet_restore_positions_from(
                       packet, pkt_head, pkt_curr, pkt_tail);

                    send_intf = route_entry->_parent.intf;
                    seq_len = 0;
                    NETIO_AddressSeq_set_length(packet->dests, 0);
                }

                /* matching destination --> add to send sequence */
                NETIO_AddressSeq_set_length(packet->dests, ++seq_len);
                *NETIO_AddressSeq_get_reference(packet->dests, seq_len-1) =
                    route_entry->_parent.intf_address;
                
            }
        }
 nextRouteEntry:
        db_rc = DB_Cursor_get_next(db_cur,(DB_Record_T*)&route_entry);
    }

    if (seq_len != 0)
    {
        NETIO_Packet_save_positions_to(packet, &pkt_head, &pkt_curr, &pkt_tail);
        /* dest_addr was set to valid addr in above loop */
        if (!NETIO_Interface_send(send_intf, &intf->_parent, dest_addr, packet))
        {
            RTPS_Log_error(LOG_INTERFACE_FAIL_SEND);
            ok = RTI_FALSE;
        }
        NETIO_Packet_restore_positions_from(packet, pkt_head, pkt_curr, pkt_tail);
    }

    /* restore packet's dests */
    packet->dests = upstr_dest_seq;

    DB_Cursor_finish(intf->_parent._rtable, db_cur);
    return ok;
}


RTI_BOOL
RTPSInterface_send(NETIO_Interface_T *netio_intf, /* RTPS self */
                   NETIO_Interface_T *source, /* upstream */
                   struct NETIO_Address *dest, /* RTPS peer */
                   NETIO_Packet_T *packet)
{
    struct RTPSInterface *self = (struct RTPSInterface *)netio_intf;
    RTI_BOOL ok = RTI_FALSE;
    union RTPS_MESSAGES *msg = NULL;
    RTPS_Entity reader_entity = RTPS_ENTITY_UNKNOWN;
    RTPS_Entity writer_entity = RTPS_ENTITY_UNKNOWN;
    RTI_SIZE_T payload_length;
    struct RTPSWriter *writer = NULL;
    RTI_UINT8 data_flags = 0;
    struct NETIO_Address *pkt_dst = NULL;

#if RTPS_RELIABILITY
    struct RTPSWindow *window = NULL;
#endif

    /* bad param check */
    if (netio_intf == NULL || source == NULL || packet == NULL || 
        packet->dests == NULL || NETIO_AddressSeq_get_length(packet->dests) == 0) 
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    DB_Database_lock(self->property._parent._parent.db);

    /* Only Writers send(); Readers send synchronously within receive(). 
    */
    if (self->_parent.state != NETIO_INTERFACESTATE_ENABLED)
    {
        RTPS_Log_error(LOG_INTERFACE_NOT_ENABLED);
        goto done;
    }

    /* RTPS reader don't currently send() */
    if (self->property.mode != RTPS_INTERFACEMODE_WRITER)
    {
        RTPS_Log_error(LOG_RTPS_READER_UNSUPPORTED);
        goto done;
    }

    writer = &self->endpoint.writer;
    window = &writer->send_window;

    /* TODO: RTPS local queue */
    /* TODO: cache source intf for each sample in local queue */
    if (writer->upstr_intf == NULL)
    {
        writer->upstr_intf = source;
    }
    else if (writer->upstr_intf != source)
    {
        RTPS_Log_error(LOG_INTERFACE_MISMATCH);
        goto done;
    }

    /* update first, last seq numbers */
    REDA_SequenceNumber_max(&writer->firstSn,
                            &writer->firstSn, &packet->info.first_available_sn);
    
#if RTPS_RELIABILITY

    /* This send will fail if the send window is full.
       If not full, the window will be updated to expect the next send to
       be for either the same or greater sequence-number.
       Note, sample is not added to window until downstream send is
       successful.
    */
    if (writer->send_window.max_length > 0)
    {
        RTI_BOOL pending_last_write, sn_greater_than_last;

        /* clean-up send-window samples no longer in queue */
        while ((window->current_length > 0) &&
               (REDA_SequenceNumber_compare(&writer->firstSn,
                                    &window->samples[window->head_idx].sn) > 0))
        {
            window->head_idx = (window->head_idx + 1) % window->max_length;
            --window->current_length;
        }

        /* quit if send window full */
        if (window->current_length == window->max_length)
        {
            RTPS_Log_error(LOG_RTPS_SEND_WINDOW_FULL);
            goto done;
        }

        /* This write may be for the same SN as the last write. Must check that
           this SN is the expected SN:
            - If pending for the same SN as last time, must check SN is indeed
              the same
            - If not pending for the same SN, must check this SN is greater
              than before
        */
        pending_last_write = 
            (window->flags & RTPS_WINDOW_FLAG_PENDING_LAST_WRITE);
        sn_greater_than_last = 
            REDA_SequenceNumber_compare(&packet->info.sn,
                                     &window->samples[window->tail_idx].sn) > 0;

        if (!(pending_last_write ^ sn_greater_than_last))
        {
            RTPS_Log_error(LOG_RTPS_FAIL_SEND_LAST_SN_OF_WRITE);
            goto done;
        }
    }
#endif

    REDA_SequenceNumber_max(&writer->lastSn, &writer->lastSn, &packet->info.sn);

    /* Create PDU
     *
     * Note that the PDU is built backwards: data messages are sent:
     * RTPS_HEADER + INFO_TS + DATA/GAP
     */

    /* set writer entity id */
    writer_entity = self->property.intf_address.value.guid.entity;

    /* TO CHECK: Check with Edward
     * If the object_id is a built-in entity, set the reader id, otherwise
     * it is left as 0
     */
    /* TODO: precondition of at least one destination */
    pkt_dst = NETIO_AddressSeq_get_reference(packet->dests, 0);
    if (pkt_dst->value.rtps_guid.object_id & 0xc0000000)
    {
        /* TODO: verify if any destination is for built-in entity, then all
                   destinations are for (same) built-in entity? */
        reader_entity = dest->value.guid.entity;
    }

    packet->source = self->property.intf_address;

    /* Get original payload length */
    payload_length = RTPS_DATA_SIZE(NETIO_Packet_get_payload_length(packet));

#if RTPS_RELIABILITY
    /* Reliable writer appends piggyback heartbeat */
    if (((self->endpoint.writer.flags & RTPS_WRITER_FLAG_RELIABLE) != 0) &&
        (self->endpoint.writer.samplesPerHb > 0))
    {
        self->endpoint.writer.piggybackSampleCount++;

        if (self->endpoint.writer.piggybackSampleCount == 
            self->endpoint.writer.samplesPerHb)
        {
            msg = (union RTPS_MESSAGES *)NETIO_Packet_get_tail(packet);
            self->endpoint.writer.hbEpoch++;
            NETIO_Packet_set_tail(packet, RTPS_HB_SIZE());
            RTPS_HB_SET(&msg->hb,RTPS_FLAGS_NONE,reader_entity,writer_entity,
                        self->endpoint.writer.firstSn,
                        self->endpoint.writer.lastSn,
                        self->endpoint.writer.hbEpoch);

            self->endpoint.writer.piggybackSampleCount = 0;
        }
    }
#endif
   
    /* DATA */
    /* TODO: verify unary minus of unsigned size is valid */
    NETIO_Packet_set_head(packet, 0-RTPS_DATA_SIZE(0));
    msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(packet);
    packet->info.protocol_id = NETIO_PROTOCOL_RTPS;

    if ((packet->info.rtps_flags & NETIO_RTPS_FLAGS_INLINEQOS) != 0)
    {
        data_flags |= RTPS_DATAFLAGS_Q;
    }

    if (packet->info.rtps_flags & NETIO_RTPS_FLAGS_DATA)
    {
        data_flags |= RTPS_DATAFLAGS_D;
    }

    RTPS_DATA_SET(&msg->data, data_flags, payload_length, RTPS_FLAGS_NONE, 
     RTPS_DATA_INLINEQOS_OFFSET, reader_entity, writer_entity, packet->info.sn);

    /* TODO: set timestamp as necessary; assume don't send zero time? */
    if (!OSAPI_NtpTime_isZero(&packet->info.timestamp))
    {
        /* INFO_TS */
        NETIO_Packet_set_head(packet,0-(RTPS_INFO_TS_SIZE(0)));
        msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(packet);
        RTPS_INFO_TS_SET(&msg->info_ts,
                         0,
                         packet->info.timestamp.sec,
                         packet->info.timestamp.frac);
    }
        
    /* RTPS Header */
    NETIO_Packet_set_head(packet, 0-(RTPS_HEADER_SIZE()));
    msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(packet);
    RTPS_HEADER_SET(&msg->header,&self->property.intf_address.value.guid.prefix);

#if RTPS_RELIABILITY
    /* Add sample to send window.  If not successful send, remove from send window. 
       The window expects each peer destination to acknowledge the sample 
       before removing it.
    */
    if (writer->send_window.max_length > 0)
    {
        window->samples[window->tail_idx].sn = packet->info.sn;
        window->samples[window->tail_idx].ack_count += 
            NETIO_AddressSeq_get_length(packet->dests);

        if (RTPSPacket_is_last_write_for_sn(packet))
        {
            /* advance tail, length */
            window->flags = RTPS_WINDOW_FLAG_NONE;
            window->tail_idx = (window->tail_idx + 1) % window->max_length;
            ++window->current_length;   
        }
        else
        {
            window->flags |= RTPS_WINDOW_FLAG_PENDING_LAST_WRITE; 
        }
    }
#endif

    if (!RTPSSender_route_packet(self, packet, RTPS_ROUTE_MODE_HIGHEST_PRIO)) 
    {
        RTPS_Log_error(LOG_RTPS_FAIL_ROUTE_PACKET);
        goto done;
    }

    ok = RTI_TRUE;
done:
    DB_Database_unlock(self->property._parent._parent.db);
    return ok;
}

RTI_BOOL
RTPSInterface_ack(NETIO_Interface_T *netio_intf,
                  struct NETIO_Address *source,
                  NETIO_PacketId_T *packet_id)
{
    /* RTPS samples do not require acknowledgement */
    RTPS_Log_error(LOG_INTERFACE_UNSUPPORTED);
    return RTI_FALSE;
}

/* TODO: add destination addr as param */
RTI_PRIVATE RTI_BOOL
RTPSInterface_request(NETIO_Interface_T *netio_intf,
                      struct NETIO_Address *source,
                      struct NETIO_Address *dest,
                      NETIO_Packet_T **packet,
                      NETIO_PacketId_T *packet_id)
{
    /* RTPS does not respond to requests */
    RTPS_Log_error(LOG_INTERFACE_UNSUPPORTED);
    return RTI_FALSE;
}

RTI_PRIVATE RTI_BOOL
RTPSInterface_return_loan(NETIO_Interface_T *netio_intf,
                          struct NETIO_Address *source,
                          NETIO_Packet_T *packet,
                          NETIO_PacketId_T *packet_id)
{
    /* RTPS does not loan samples */
    RTPS_Log_error(LOG_INTERFACE_UNSUPPORTED);
    return RTI_FALSE;
}

RTI_PRIVATE RTI_BOOL
RTPSInterface_discard(NETIO_Interface_T *netio_intf,
                      struct NETIO_Address *destination,
                      NETIO_PacketId_T *packet_id)
{
    struct RTPSInterface *self = (struct RTPSInterface *)netio_intf;

    if (packet_id == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    DB_Database_lock(self->property._parent._parent.db);

    if (self->property.mode == RTPS_INTERFACEMODE_WRITER &&
        (REDA_SequenceNumber_compare(packet_id, 
                                    &self->endpoint.writer.firstSn) > 0))
    {
        /* Assumption: upstream intf discards samples in-order */
        self->endpoint.writer.firstSn = *packet_id;
        REDA_SequenceNumber_plusplus(&self->endpoint.writer.firstSn);
    }

    DB_Database_unlock(self->property._parent._parent.db);

    return RTI_TRUE;
}

RTI_PRIVATE struct RTPSXportEntry*
RTPSInterface_assert_transport(struct RTPSInterface *self,
                               NETIO_Interface_T *via_intf, /* downstream intf */
                               struct NETIO_Address *via_addr) /* downstream addr */
{
    struct RTPSXportEntryKey xport_key;
    RTI_SIZE_T i;
    RTI_INT32 free_index = -1;

    xport_key.addr = *via_addr;
    xport_key.intf = via_intf;

    /* find existing entry; also find first free entry */
    for (i = 0; i < self->property.max_transport_count; ++i)
    {
        if ((free_index == -1) && (self->xports[i].ref_count == 0))
        {
            free_index = i;
        }
        if (OSAPI_Memory_compare(&xport_key, &self->xports[i],
                                sizeof(xport_key)) == 0)
        {
            /* found match */
            ++self->xports[i].ref_count;
            return (&self->xports[i]);
        }
    }

    if (free_index < 0) /* No free entries */
    {
        RTPS_Log_error(LOG_RTPS_OUT_OF_RESOURCES);
        return NULL;
    }

    /* Initialize new transport entry */
    self->xports[free_index].addr = *via_addr;
    self->xports[free_index].intf = via_intf;
    self->xports[free_index].ref_count = 1;
   
    return (&self->xports[free_index]);
}


/* TODO: keep peer state within route/bind tables */
RTI_PRIVATE struct RTPSPeerEntry*
RTPSInterface_assert_peer(struct RTPSInterface *self,
                          struct NETIO_Address *peer_addr,
                          RTI_UINT8 peer_reliable)
{
    RTI_SIZE_T i;
    RTI_INT32 free_index = -1;
    struct REDA_SequenceNumber SN_ZERO = {0,0};
    struct REDA_SequenceNumber SN_ONE = {0,1};

    /* Check existing peer */
    for (i = 0; i < self->property.max_peer_count; ++i)
    {
        if (free_index < 0 && self->peers[i].ref_count == 0)
        {
            free_index = i;
        } 
        /* Keyed on peer's address */
        if (self->peers[i].ref_count != 0 && 
            (OSAPI_Memory_compare(&self->peers[i].addr, peer_addr,
                                 sizeof(struct NETIO_Address)) == 0))
        {
            /* found existing peer, return immediately */
            ++self->peers[i].ref_count;
            return (&self->peers[i]);
        }
    }

    if (free_index < 0) /* No free entries */
    {
        RTPS_Log_error(LOG_RTPS_OUT_OF_RESOURCES);
        return NULL;
    }

    /* Initialize new peer entry */
    self->peers[free_index].ref_count = 1;
    self->peers[free_index].addr = *peer_addr;

    if (self->property.mode == RTPS_INTERFACEMODE_WRITER)
    {
        /* remote reader */
        self->peers[free_index].info.reader.reliable = peer_reliable;

#if RTPS_RELIABILITY
        if (peer_reliable && (self->peers[free_index].ref_count == 1))
        {
            ++self->endpoint.writer.reliable_reader_count; /* new peer reader */
        }
        self->peers[free_index].info.reader.epoch = 0;
        MIGRtpsBitmap_reset(&self->peers[free_index].info.reader.bitmap,
                            &SN_ZERO, MIG_RTPS_BITMAP_SIZE_MAX);
        
#endif

    }
    else if (self->property.mode == RTPS_INTERFACEMODE_READER)
    {
        /* remote writer */
        self->peers[free_index].info.writer.reliable = peer_reliable;
        MIGRtpsBitmap_reset(&self->peers[free_index].info.writer.bitmap,
                            &SN_ONE, MIG_RTPS_BITMAP_SIZE_MAX);

#if RTPS_RELIABILITY
        self->peers[free_index].info.writer.epoch = 0;
#endif

    }
    return (&self->peers[free_index]);
}

RTI_INT32
RTPSInterface_compute_route_priority(struct RTPSInterface *self, 
                                     struct NETIO_Address *via_addr)
{
    RTI_INT32 result = RTPS_ROUTE_PRIORITY_DEFAULT;

    /* IPv4 multicast address */
    if ((via_addr->kind == NETIO_ADDRESS_KIND_UDPv4) &&
        (via_addr->value.ipv4.address & 0xe0000000))
    {
        result = RTPS_ROUTE_PRIORITY_MULTICAST;
    }

    return result;
}

/*  Establishes logical route between source and peer.
      
 */
RTI_PRIVATE RTI_BOOL
RTPSInterface_add_route(NETIO_Interface_T *src_intf, /* RTPS self */
                        struct NETIO_Address *dst_addr, /* RTPS peer */
                        NETIO_Interface_T *via_intf, /* xport intf */
                        struct NETIO_Address *via_addr, /* xport addr (uni/multicast) */
                        struct NETIORouteProperty *property,
                        RTI_BOOL *existed)
{
    struct RTPSInterface *self = (struct RTPSInterface *)src_intf;
    struct RTPSInterface *ext_intf = NULL;
    struct RTPSPeerEntry *peer_entry = NULL;
    struct RTPSXportEntry *xport_entry = NULL;
    struct RTPSRouteEntry *route_entry = NULL;
    struct RTPSRouteProperty *route_property = NULL;
    struct NETIORouteEntryKey route_key;
    struct RTPSBindEntry *bind_entry = NULL;
    union RTPS_MESSAGES *msg = NULL;
    RTI_UINT8 is_reliable_peer = 0;
    DB_ReturnCode_T db_rc;

    /* bad param check */
    if (src_intf == NULL || dst_addr == NULL || 
        via_intf == NULL || via_addr == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    LOG_printDebug("RTPS route: fwd: src: %d/%x.%x.%x.%x ==> dst %d/%x.%x.%x.%x, via: %d/%x.%x.%x.%x\n",
            self->property.intf_address.port,
            self->property.intf_address.value.rtps_guid.host_id,
            self->property.intf_address.value.rtps_guid.app_id,
            self->property.intf_address.value.rtps_guid.instance_id,
            self->property.intf_address.value.rtps_guid.object_id,
            dst_addr->port,
            dst_addr->value.rtps_guid.host_id,
            dst_addr->value.rtps_guid.app_id,
            dst_addr->value.rtps_guid.instance_id,
            dst_addr->value.rtps_guid.object_id,
            via_addr->port,
            via_addr->value.rtps_guid.host_id,
            via_addr->value.rtps_guid.app_id,
            via_addr->value.rtps_guid.instance_id,
            via_addr->value.rtps_guid.object_id);

    /* first assert peer */
    if (property != NULL)
    {
        route_property = (struct RTPSRouteProperty*)property;
        is_reliable_peer = ((struct RTPSRouteProperty*)property)->reliable;
    }
    peer_entry = RTPSInterface_assert_peer(self, dst_addr, is_reliable_peer);
    if (peer_entry == NULL)
    {
        RTPS_Log_error(LOG_RTPS_FAIL_ASSERT_PEER);
        return RTI_FALSE;
    }

    /* Assert downstream (transport) interface */
    if (via_intf != NULL) /* assert transport, if available */
    {
        xport_entry = 
            RTPSInterface_assert_transport(self, via_intf, via_addr);
        if (xport_entry == NULL)
        {
            RTPS_Log_error(LOG_RTPS_FAIL_ASSERT_TRANSPORT);
            return RTI_FALSE;
        }
    }

    /* check for existing record */
    route_key.destination = *dst_addr; /* RTPS peer */
    route_key.intf = via_intf; /* xport intf */
    route_key.intf_address = *via_addr; /* xport addr */

    db_rc = DB_Table_select_match(self->_parent._rtable,
                                 DB_TABLE_DEFAULT_INDEX,
                                 (DB_Record_T *)&route_entry,
                                 (DB_Key_T)&route_key);
    if ((db_rc != DB_RETCODE_OK) && (db_rc != DB_RETCODE_NOT_EXISTS))
    {
        RTPS_Log_error(LOG_DB_FAIL_SELECT_MATCH);
        return RTI_FALSE;
    }

    if (db_rc == DB_RETCODE_OK)
    {
        if (existed != NULL)
        {
            *existed = RTI_TRUE;
        }
        return RTI_TRUE;
    }

    /* Assert route entry */
    db_rc = DB_Table_create_record(self->_parent._rtable, 
                                  (DB_Record_T *)&route_entry);
    if (db_rc != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_CREATE_RECORD);
        return RTI_FALSE;
    }

    /* Initialize route entry */
    route_entry->_parent.intf = via_intf;
    route_entry->_parent.intf_address = *via_addr;
    route_entry->_parent.destination = *dst_addr;
    route_entry->_parent.state = NETIO_ROUTESTATE_ACTIVE; 

    /* Compute and set route priority.
       Current priority order: multicast > unicast.
    */
    route_entry->priority = 
        RTPSInterface_compute_route_priority(self, via_addr);

    route_entry->peer_ref = peer_entry; 
    route_entry->xport_ref = xport_entry;
    
    if (route_property != NULL)
    {
        self->endpoint.writer.firstSn = route_property->first_sn;
        self->endpoint.writer.lastSn = route_property->last_sn;
    }

    LOG_printDebug("STATE[%d.%d -- %d.%d]\n",
            self->endpoint.writer.firstSn.high,
            self->endpoint.writer.firstSn.low,
            self->endpoint.writer.lastSn.high,
            self->endpoint.writer.lastSn.low);

    db_rc = DB_Table_insert_record(self->_parent._rtable,
                                  (DB_Record_T *)route_entry);
    if ((db_rc != DB_RETCODE_OK) && (db_rc != DB_RETCODE_EXISTS))
    {
        RTPS_Log_error(LOG_DB_FAIL_INSERT_RECORD);
        /* TODO: delete created record? */
        return RTI_FALSE;
    }

    /* Check if peer was asserted in external bind table */
    ext_intf = (struct RTPSInterface *)REDA_Indexer_find_entry(
       self->factory->ext_intf_index, &self->property.intf_address);
    if (ext_intf == NULL)
    {
        RTPS_Log_error(LOG_FAIL_GET_EXT_INTF);
        return RTI_FALSE;
    }
    db_rc = DB_Table_select_match(ext_intf->_parent._btable,
                                  ext_intf->btable_peer_index,
                                  (DB_Record_T*)&bind_entry,
                                  (DB_Key_T)dst_addr);
    if ((db_rc != DB_RETCODE_OK) && (db_rc != DB_RETCODE_NOT_EXISTS))
    {
        RTPS_Log_error(LOG_DB_FAIL_SELECT_MATCH);
        return RTI_FALSE;
    }
    if (db_rc == DB_RETCODE_OK)
    {
        /* may have called bind() before add_route(); need to assign peer */
        if (bind_entry->peer_ref == NULL)
        {
            bind_entry->peer_ref = peer_entry;
        }
    }

/* Reliable endpoints send preemptive HB or ACKNACK */
#if RTPS_RELIABILITY
    /* Stop if don't need to send preemptive HB or ACKNACK */
    if (!self->property.reliable || !is_reliable_peer)
    {
        return RTI_TRUE;
    }

    /* TODO: preemptive HB or ACKNACK between reliable interfaces */
    NETIO_Packet_initialize(&self->packet, self->packet_buf,
                           RTPS_LOCAL_PACKET_SIZE, 0, &self->local_dest_seq); 

    if (self->property.mode == RTPS_INTERFACEMODE_WRITER)
    {
        struct RTPSWriter *writer = &self->endpoint.writer;

        /* setup preemptive Heartbeat */
        NETIO_Packet_set_source(&self->packet, &self->property.intf_address);
        NETIO_Packet_set_head(&self->packet, 0-RTPS_HB_SIZE());
        msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(&self->packet);
        writer->hbEpoch++;
        RTPS_HB_SET(&msg->hb,RTPS_FLAGS_NONE,  
                    peer_entry->addr.value.guid.entity,
                    self->property.intf_address.value.guid.entity,
                    writer->firstSn, writer->lastSn, writer->hbEpoch);
    }
    else /* if (self->property.mode == RTPS_INTERFACEMODE_READER) */
    {
        /* setup preemptive ACKNACK */
        struct MIGRtpsBitmap bitmap;
        struct REDA_SequenceNumber SN_ZERO = REDA_SEQUENCE_NUMBER_ZERO;
        struct RTPSReader *reader = &self->endpoint.reader;

        MIGRtpsBitmap_reset(&bitmap, &SN_ZERO, 0);
        NETIO_Packet_set_source(&self->packet, &self->property.intf_address);
        NETIO_Packet_set_head(&self->packet, 0-RTPS_ACKNACK_SIZE(0));
        msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(&self->packet);
        reader->ackEpoch++;
        RTPS_ACKNACK_SET(&msg->acknack, 0, RTPS_ACKNACK_SIZE(0),
                         self->property.intf_address.value.guid.entity,
                         peer_entry->addr.value.guid.entity, &bitmap, 0);
    }

    /* RTPS Header */
    NETIO_Packet_set_head(&self->packet, 0-RTPS_HEADER_SIZE());
    msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(&self->packet);
    RTPS_HEADER_SET(&msg->header,&self->property.intf_address.value.guid.prefix);

    *NETIO_AddressSeq_get_reference(self->packet.dests, 0) = peer_entry->addr;
    if (!RTPSSender_route_packet(self, &self->packet, RTPS_ROUTE_MODE_ALL))
    {
        RTPS_Log_error(LOG_RTPS_FAIL_ROUTE_PACKET);
        return RTI_FALSE;
    }
#endif /* RTPS_RELIABILITY */
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RTPSInterface_delete_route(NETIO_Interface_T *netio_intf, /* RTPS self */
                          struct NETIO_Address *dst_addr, /* */
                          NETIO_Interface_T *via_intf,
                          struct NETIO_Address *via_addr,
                          RTI_BOOL *existed)
{
    struct RTPSInterface *self = (struct RTPSInterface *)netio_intf;
    struct RTPSRouteEntry *route_entry = NULL;
    struct NETIORouteEntryKey key;
    DB_ReturnCode_T db_rc;

    if (netio_intf == NULL || dst_addr == NULL || 
        via_intf == NULL || via_addr == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    key.destination = *dst_addr;
    key.intf = via_intf;
    key.intf_address = *via_addr;

    db_rc = DB_Table_remove_record(self->_parent._rtable,
                                  (DB_Record_T *)&route_entry,
                                  &key);
    if (db_rc != DB_RETCODE_OK)
    {
        if (db_rc == DB_RETCODE_NOT_EXISTS)
        {
            RTPS_LOG_WARNING(LOG_RTPS_ROUTE_NOT_EXIST);
            return RTI_TRUE;
        }
        RTPS_Log_error(LOG_DB_FAIL_REMOVE_RECORD);
        return RTI_FALSE;
    }

    route_entry->_parent.state = NETIO_ROUTESTATE_INACTIVE;

    /* Decrease peer's refCount (except for best-effort reader) */
    if (!self->property.anonymous &&
        !((self->property.mode == RTPS_INTERFACEMODE_READER) && 
          !self->property.reliable))
    {
        --route_entry->peer_ref->ref_count;

#if RTPS_RELIABILITY
        if (route_entry->peer_ref->info.reader.reliable &&
            self->property.reliable)
        {
            --self->endpoint.writer.reliable_reader_count;
        }
#endif
    }
    if (route_entry->xport_ref != NULL)
    {
        --route_entry->xport_ref->ref_count;
    }

    db_rc = DB_Table_delete_record(self->_parent._rtable, route_entry);
    if (db_rc != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_DELETE_RECORD);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

/* Asserts RTPS peer in external interface's receive/bind table */
RTI_PRIVATE RTI_BOOL
RTPSInterface_bind(NETIO_Interface_T *netio_intf, /* RTPS self */
                   struct NETIO_Address *src_addr, /* RTPS peer */
                   struct NETIOBindProperty *property,
                   RTI_BOOL *existed)
{
    struct RTPSInterface *self = (struct RTPSInterface *)netio_intf;
    struct RTPSInterface *ext_intf = NULL;
    struct RTPSRouteEntry *route_entry = NULL;
    struct RTPSBindEntry *bind_entry = NULL;
    struct RTPSPeerEntry *peer_entry = NULL;
    DB_ReturnCode_T db_rc;
    struct NETIOBindEntryKey bind_key;

    LOG_printDebug("RTPS bind: fwd: src: %d/%x.%x.%x.%x ==> dst %d/%x.%x.%x.%x\n",
            src_addr->port,
            src_addr->value.rtps_guid.host_id,
            src_addr->value.rtps_guid.app_id,
            src_addr->value.rtps_guid.instance_id,
            src_addr->value.rtps_guid.object_id);

    /* bad param check */
    if (netio_intf == NULL || src_addr == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    /* anonymous interfaces don't maintain peers */
    if (!self->property.anonymous) /* TODO: use flag, or derive from ID? */
    {
        db_rc = DB_Table_select_match(self->_parent._rtable,
                                     self->rtable_peer_index, 
                                     (DB_Record_T *)&route_entry,
                                     (DB_Key_T)src_addr);
        if ((db_rc != DB_RETCODE_OK) && (db_rc != DB_RETCODE_NOT_EXISTS))
        {
            RTPS_Log_error(LOG_DB_FAIL_SELECT_MATCH);
            return RTI_FALSE;
        }

        /* preexisting peer */
        if (db_rc == DB_RETCODE_OK)
        {
            peer_entry = route_entry->peer_ref;
        }
    }

    bind_key.source = *src_addr; /* peer addr */ 
    bind_key.destination = self->property.intf_address; /* self addr */

    ext_intf = (struct RTPSInterface *)REDA_Indexer_find_entry(
       self->factory->ext_intf_index, &self->property.intf_address);
    if (ext_intf == NULL)
    {
        RTPS_Log_error(LOG_FAIL_GET_EXT_INTF);
        return RTI_FALSE;
    }

    db_rc = DB_Table_select_match(
       ext_intf->_parent._btable, DB_TABLE_DEFAULT_INDEX,
       (DB_Record_T *)&bind_entry, (DB_Key_T)&bind_key);
    if ((db_rc != DB_RETCODE_OK) && (db_rc != DB_RETCODE_NOT_EXISTS))
    {
        RTPS_Log_error(LOG_DB_FAIL_SELECT_MATCH);
        return RTI_FALSE;
    }

    if (db_rc == DB_RETCODE_OK)
    {
        if (existed != NULL)
        {
            *existed = RTI_TRUE;
        }
        return RTI_TRUE;
    }

    db_rc = DB_Table_create_record(ext_intf->_parent._btable,
                                  (DB_Record_T *)&bind_entry);
    if (db_rc != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_CREATE_RECORD);
        return RTI_FALSE;
    }

    OSAPI_Memory_zero(bind_entry,sizeof(struct RTPSBindEntry));
    bind_entry->_parent.source = bind_key.source;
    bind_entry->_parent.destination = bind_key.destination;
    bind_entry->_parent.intf = netio_intf; /* self */
    bind_entry->_rtps_intf = self;
    bind_entry->peer_ref = peer_entry; /* NULL peer for anon endpoint */

    db_rc = DB_Table_insert_record(ext_intf->_parent._btable,
                                   (DB_Record_T *)bind_entry);
    if ((db_rc != DB_RETCODE_OK) && (db_rc != DB_RETCODE_EXISTS))
    {
        RTPS_Log_error(LOG_DB_FAIL_INSERT_RECORD);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RTPSInterface_unbind(NETIO_Interface_T *netio_intf, /* RTPS self */
                    struct NETIO_Address *src_addr, /* RTPS peer */
                    NETIO_Interface_T *dst_intf, /* RTPS self */
                    RTI_BOOL *existed)
{
    struct RTPSInterface *self = (struct RTPSInterface *)netio_intf;
    struct RTPSInterface *ext_intf = NULL;
    struct RTPSBindEntry *bind_entry = NULL;
    DB_ReturnCode_T db_rc;
    struct NETIOBindEntryKey bind_key;

    /* bad param check */
    if (netio_intf == NULL || src_addr == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    bind_key.source = *src_addr;
    bind_key.destination = self->property.intf_address;

    ext_intf = (struct RTPSInterface *)REDA_Indexer_find_entry(
       self->factory->ext_intf_index, &self->property.intf_address);
    if (ext_intf == NULL)
    {
        RTPS_Log_error(LOG_FAIL_GET_EXT_INTF);
        return RTI_FALSE;
    }

    db_rc = DB_Table_remove_record(ext_intf->_parent._btable,
                                   (DB_Record_T *)&bind_entry, &bind_key);
    if (db_rc == DB_RETCODE_NOT_EXISTS)
    {
        return RTI_TRUE;
    }

    if (db_rc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    /* For best-effort reader, decrease peer's refCount */
    if (!self->property.anonymous &&
        (self->property.mode == RTPS_INTERFACEMODE_READER) && 
        !self->property.reliable)
    {
        bind_entry->peer_ref->ref_count--;
    }

    bind_entry->_rtps_intf = NULL;
    bind_entry->peer_ref = NULL;

    if (DB_Table_delete_record(ext_intf->_parent._btable,
                               (DB_Record_T)bind_entry) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}


/* Asserts upstream intf/addr in local RTPS intf's bind table */
RTI_PRIVATE RTI_BOOL
RTPSInterface_bind_external(NETIO_Interface_T *src_intf, /* self intf */
                            struct NETIO_Address *src_addr, /* peer addr */
                            NETIO_Interface_T *dst_intf, /* upstream intf */
                            struct NETIO_Address *dst_addr, /* upstream addr */
                            struct NETIOBindProperty *property,
                            RTI_BOOL *existed)
{
    struct RTPSInterface *self = (struct RTPSInterface *)src_intf;
    struct RTPSBindEntry *bind_entry = NULL;
    DB_ReturnCode_T db_rc;
    struct NETIOBindEntryKey bind_key;

    /* bad param check */
    if (src_intf == NULL || src_addr == NULL || 
        dst_intf == NULL || dst_addr == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    bind_key.source = *src_addr;
    bind_key.destination = *dst_addr;

    /* assert in own bind table .. */
    db_rc = DB_Table_select_match(self->_parent._btable,
                                      DB_TABLE_DEFAULT_INDEX,
                                      (DB_Record_T *)&bind_entry,
                                      &bind_key);
    if ((db_rc != DB_RETCODE_OK) && (db_rc != DB_RETCODE_NOT_EXISTS))
    {
        return RTI_FALSE;
    }

    if (db_rc == DB_RETCODE_OK)
    {
        LOG_printDebug("record already existin bind on intef: %p\n",src_intf);
        if (existed != NULL)
        {
            *existed = RTI_TRUE;
        }
        return RTI_TRUE;
    }

    if (DB_Table_create_record(self->_parent._btable,
                              (DB_Record_T *)&bind_entry) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    OSAPI_Memory_zero(bind_entry, sizeof(struct RTPSBindEntry));
    bind_entry->_parent.source = *src_addr;
    bind_entry->_parent.destination = *dst_addr; 
    bind_entry->_parent.intf = dst_intf; /* upstream's interface */
    bind_entry->_rtps_intf = self;

    db_rc = DB_Table_insert_record(self->_parent._btable,
                                       (DB_Record_T *)bind_entry);
    if ((db_rc != DB_RETCODE_OK) && (db_rc != DB_RETCODE_EXISTS))
    {
        return RTI_FALSE;
    }

    LOG_printDebug("added bind on intef: %p\n",src_intf);

    if ((self->property.mode == RTPS_INTERFACEMODE_WRITER) &&
        self->property.reliable)
    {
        if (self->endpoint.writer.upstr_intf == NULL)
        {
            self->endpoint.writer.upstr_intf = dst_intf;
        }
        else if (self->endpoint.writer.upstr_intf != dst_intf)
        {
            RTPS_Log_error(LOG_INTERFACE_MISMATCH);
            return RTI_FALSE;
        }
    }

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RTPSInterface_unbind_external(NETIO_Interface_T *src_intf, /* self intf */
                              struct NETIO_Address *src_addr, /* self addr */
                              NETIO_Interface_T *dst_intf, /* upstream intf */
                              struct NETIO_Address *dst_addr, /* upstream addr */
                              RTI_BOOL *existed)
{
    struct RTPSInterface *self = (struct RTPSInterface *)src_intf;
    struct RTPSBindEntry *bind_entry = NULL;
    DB_ReturnCode_T db_rc;
    struct NETIOBindEntryKey bind_key;

    /* bad param check */
    if (src_intf == NULL || src_addr == NULL || 
        dst_intf == NULL || dst_addr == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    bind_key.source = *src_addr;
    bind_key.destination = *dst_addr;

    db_rc = DB_Table_remove_record(self->_parent._btable,
                                       (DB_Record_T *)&bind_entry,
                                       &bind_key);
    if (db_rc == DB_RETCODE_NOT_EXISTS)
    {
        RTPS_LOG_WARNING(LOG_RTPS_BIND_NOT_EXIST);
        return RTI_TRUE;
    }

    if (db_rc != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_REMOVE_RECORD);
        return RTI_FALSE;
    }

    bind_entry->_rtps_intf = NULL;
    bind_entry->peer_ref = NULL;

    db_rc = DB_Table_delete_record(self->_parent._btable,
                                       (DB_Record_T)bind_entry);
    if (db_rc != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_DELETE_RECORD);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

/* assigned to NETIO_Interface_get_external_interface() */
RTI_PRIVATE RTI_BOOL
RTPSInterface_get_external_interface(NETIO_Interface_T *netio_intf,
                                struct NETIO_Address *src_addr,
                                NETIO_Interface_T **dst_intf, /* out param */
                                struct NETIO_Address *dst_addr)
{
    struct RTPSInterface *self = (struct RTPSInterface *)netio_intf;
    struct RTPSInterface *ext_intf = NULL;

    ext_intf = (struct RTPSInterface *)REDA_Indexer_find_entry(
       self->factory->ext_intf_index, &self->property.intf_address);

    if (ext_intf == NULL)
    {
        return RTI_FALSE;
    }

    *dst_intf = &ext_intf->_parent;

    return RTI_TRUE;
}

RTI_BOOL
RTPSInterface_valid_pdu(NETIO_Interface_T *netio_intf, 
                        NETIO_Packet_T *packet)
{
    union RTPS_MESSAGES *rtps_msg;

    if (NETIO_Packet_get_payload_length(packet) < MIG_RTPS_HEADER_SIZE)
    {
        return RTI_FALSE;
    }

    rtps_msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(packet);

    if (rtps_msg->header.rtps != VALID_RTPS_HEADER)
    {
        return RTI_FALSE;
    }

    if (rtps_msg->header.protocol_version.major < 2)
    {
        return RTI_FALSE;
    }

    if (rtps_msg->header.protocol_version.minor < 1)
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RTPSReceiver_forward_upstream(struct RTPSInterface *intf,
                              RTI_BOOL *all_received,
                              NETIO_Packet_T *packet,
                              struct NETIO_Address *src_addr)
{
    DB_Cursor_T db_cur;
    DB_ReturnCode_T db_rc;
    struct RTPSBindEntry *bind_entry;
    RTI_SIZE_T pkt_head, pkt_curr, pkt_tail;

    packet->info.protocol_id = NETIO_PROTOCOL_RTPS;

    if (DB_Table_select_all_default(intf->_parent._btable, 
                                   &db_cur) != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_SELECT_ALL);
        return RTI_FALSE;
    }

    if (all_received != NULL)
    {
        *all_received = RTI_TRUE;
    }

    NETIO_Packet_save_positions_to(packet, &pkt_head, &pkt_curr, &pkt_tail);

    db_rc = DB_Cursor_get_next(db_cur, (DB_Record_T*)&bind_entry);
    while (db_rc == DB_RETCODE_OK)
    {
        if ((src_addr == NULL) ||
            NETIO_Address_compare(src_addr, &bind_entry->_parent.source) == 0)
        {
            if (!NETIO_Interface_receive(
               bind_entry->_parent.intf,
               &bind_entry->_parent.source,
               &bind_entry->_parent.destination,
               packet))
            {
                RTPS_Log_error(LOG_INTERFACE_FAIL_RECEIVE);
                if (all_received != NULL)
                {
                    *all_received = RTI_FALSE;
                }
            }

            NETIO_Packet_restore_positions_from(
               packet, pkt_head, pkt_curr, pkt_tail);
        }
        db_rc = DB_Cursor_get_next(db_cur, (DB_Record_T*)&bind_entry);
    }
    NETIO_Packet_restore_positions_from(packet, pkt_head, pkt_curr, pkt_tail);
    DB_Cursor_finish(intf->_parent._btable, db_cur);
    return RTI_TRUE;
}


RTI_PRIVATE RTI_BOOL
RTPSReceiver_process_gap(struct RTPSInterface *intf,
                         struct RTPSPeerEntry *peer_entry, 
                         NETIO_Packet_T *packet,
                         char *stream,
                         RTI_BOOL byte_swap)
{
    struct REDA_SequenceNumber old_lead, sn_start, sn_end, zero_sn;
    struct MIGRtpsBitmap bitmap, final_bitmap;
    struct RTPSRemoteWriter *writer;

    /* only unanonymous readers process GAPs */
    /* (anonymous writer only talks with anonymous readers,
       and being best-effort and stateless, anonymous writers never send GAPs */
    if (intf->property.mode != RTPS_INTERFACEMODE_READER ||
        intf->property.anonymous)
    {
        return RTI_TRUE;
    }

    writer = &peer_entry->info.writer;

    old_lead = writer->bitmap._lead;

    /* Precondition: stream pointing to GAP starting sn */
    CDR_deserialize_REDASequenceNumber(&stream, &sn_start, byte_swap);
    MIGRtpsBitmap_deserialize(&bitmap, (const char**)&stream, byte_swap);

    LOG_printDebug("Processing GAP, first uncommittable (%d,%u)\n",
                      bitmap._lead.high, bitmap._lead.low);

    /* update peer's state from GAP bitmap */
    if (REDA_SequenceNumber_compare(&sn_start, &writer->bitmap._lead) <= 0)
    {
        MIGRtpsBitmap_shift(&writer->bitmap, &bitmap._lead);
    }
    MIGRtpsBitmap_reset(&final_bitmap, &writer->bitmap._lead,
                        MIG_RTPS_BITMAP_SIZE_MAX);

    sn_end = bitmap._lead;
    REDA_SequenceNumber_minusminus(&sn_end);

    MIGRtpsBitmap_fill(&final_bitmap, &sn_start, &sn_end, RTI_TRUE);
    MIGRtpsBitmap_merge(&final_bitmap, &bitmap);
    MIGRtpsBitmap_merge(&writer->bitmap, &final_bitmap);

    /* update data-received bitmap */
    if (MIGRtpsBitmap_getFirstBit(&writer->bitmap, &zero_sn, RTI_FALSE))
    {
        MIGRtpsBitmap_shift(&writer->bitmap, &zero_sn);
    }
    else
    {
        zero_sn.high = 0;
        zero_sn.low = writer->bitmap._bitCount;
        REDA_SequenceNumber_increment(&zero_sn,&writer->bitmap._lead);
        MIGRtpsBitmap_reset(&writer->bitmap,&zero_sn,MIG_RTPS_BITMAP_SIZE_MAX);
    }

    if (REDA_SequenceNumber_compare(&old_lead, &writer->bitmap._lead) >= 0)
    {
        /* no samples to commit */
        return RTI_TRUE;
    }

    /* Commit (invalid) samples upstream. */
    /* Set invalid data flag, first uncommittable sn */
    packet->info.valid_data = 0;
    packet->info.committable_sn = writer->bitmap._lead;
    packet->info.rtps_flags |= NETIO_RTPS_FLAGS_COMMIT_DATA;
    if (!RTPSReceiver_forward_upstream(intf, NULL, packet, &peer_entry->addr))
    {
        RTPS_Log_error(LOG_RTPS_FAIL_RECEIVER_FORWARD_UPSTREAM);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}


RTI_PRIVATE RTI_BOOL
RTPSReceiver_process_acknack(struct RTPSInterface *intf,
                             struct RTPSPeerEntry *peer_entry,
                             char *stream,
                             RTI_INT8 flags,
                             RTI_BOOL byte_swap)
{
    RTI_BOOL ok = RTI_FALSE;

#if RTPS_RELIABILITY   /* returns false if !defined(RTPS_RELIABILITY) */

    struct RTPSWriter *writer = NULL;
    struct RTPSRemoteReader *reader = NULL;
    struct RTPSWindow *window = NULL;

    struct MIGRtpsBitmap bitmap;
    struct MIGRtpsBitmap resend_bitmap;
    REDA_Epoch_T epoch;
    struct REDA_SequenceNumber req_sn; 
    struct REDA_SequenceNumber gap_sn_end;
    RTI_BOOL resent = RTI_FALSE;
    NETIO_Packet_T *packet = NULL;
    RTI_SIZE_T payload_length;
    union RTPS_MESSAGES *msg = NULL;
    RTPS_Entity writer_entity;
    RTPS_Entity reader_entity;

    RTI_UINT8 data_flags = RTPS_DATAFLAGS_D;

    /* only unanonymous writers process ACKNACKs */
    if (intf->property.mode != RTPS_INTERFACEMODE_WRITER || 
        intf->property.anonymous)
    {
        return RTI_TRUE;
    }

    writer = &intf->endpoint.writer;
    reader = &peer_entry->info.reader;

    window = &writer->send_window;

    /* dont' process if writer is not reliable */
    if ((writer->flags & RTPS_WRITER_FLAG_RELIABLE) == 0)
    {
        return RTI_TRUE;
    }

    MIGRtpsBitmap_deserialize(&bitmap, (const char**)&stream, byte_swap);
    MIGRtpsEpoch_deserialize(&epoch, &stream, byte_swap);

    /* ignore old ACKNACK */
    if (!REDA_Epoch_isGreater(epoch, reader->epoch))
    {
        RTPS_LOG_DEBUG(LOG_EPOCH_LESS_THAN_LAST_EPOCH);
        return RTI_TRUE;
    }

    reader->epoch = epoch;

    RTPS_LOG_PRINT_DEBUG("RTPS processing ACKNACK: lead(%d,%u) bitcount(%d) epoch(%d)\n",
                        bitmap._lead.high, bitmap._lead.low, bitmap._bitCount, epoch);

    /* update send window if reader's lowest unacked SN has advanced */
    if ((writer->send_window.max_length > 0) &&
        (REDA_SequenceNumber_compare(&bitmap._lead, &reader->bitmap._lead) > 0))
    {
        RTI_INT32 i, iter;

        /* first, update ack_count in window */
        for (i = window->head_idx, iter = window->current_length; 
              (iter > 0) && (REDA_SequenceNumber_compare(&window->samples[i].sn,
                                                        &bitmap._lead) < 0); 
              i = (i + 1) % window->max_length, --iter)
        {
            if (((--window->samples[i].ack_count) == 0) &&
                /* don't ack SN that is waiting for more writes */
                !((window->flags & RTPS_WINDOW_FLAG_PENDING_LAST_WRITE) &&
                  (iter == 1)))
            {
                if (!NETIO_Interface_ack(writer->upstr_intf,
                                        &intf->property.intf_address,
                                        &window->samples[i].sn))
                {
                    RTPS_Log_error(LOG_INTERFACE_FAIL_ACK);
                    /* don't fail, continue ACKing */
                }
            }
        }

        /* then, update head index */
        while ((window->current_length > 0) &&
               (window->samples[window->head_idx].ack_count == 0))
        {   
            /* don't advance SN that is waiting for more writes */
            if ((window->current_length == 1) && 
                (window->flags & RTPS_WINDOW_FLAG_PENDING_LAST_WRITE))
            {
                break;
            }
            window->head_idx = (window->head_idx + 1) % window->max_length;
            --window->current_length;
        }
    }

    MIGRtpsBitmap_invert(&bitmap); /* 0's are now missing samples */
    if (REDA_SequenceNumber_compare(&bitmap._lead, &reader->bitmap._lead) >= 0)
    {
        MIGRtpsBitmap_copy(&resend_bitmap, &bitmap, MIG_RTPS_BITMAP_SIZE_MAX);
        MIGRtpsBitmap_merge(&resend_bitmap, &reader->bitmap);
        reader->bitmap = resend_bitmap;
    }
    else
    {
        MIGRtpsBitmap_merge(&reader->bitmap, &bitmap);
    }

    /* no resend needed for pure ACK */
    if ((MIGRtpsBitmap_getBitCount(&bitmap) == 0) &&
        ((flags & RTPS_ACKNACKFLAGS_F) != 0))
    {
        RTPS_LOG_DEBUG(LOG_RTPS_RECEIVE_PURE_ACK);
        return RTI_TRUE;
    }

    /* TODO: track ack-state per peer, notify writer queue when fully ack'd */

    /* verify upstream interface available for processing request */
    if (writer->upstr_intf == NULL)
    {
        RTPS_Log_error(LOG_RTPS_INVALID_UPSTREAM_INTERFACE);
        return RTI_TRUE;
    }

    writer_entity = intf->property.intf_address.value.guid.entity;
    reader_entity = peer_entry->addr.value.guid.entity;

    /* request and resend */
    MIGRtpsBitmap_truncate(&bitmap, &writer->lastSn);
    while ((MIGRtpsBitmap_getFirstBit(&bitmap, &req_sn, RTI_FALSE) &&
            REDA_SequenceNumber_compare(&req_sn, &writer->lastSn) <= 0))
    {
        if (!NETIO_Interface_request(writer->upstr_intf,
                                    &intf->property.intf_address,
                                    &peer_entry->addr,
                                    &packet,
                                    (NETIO_PacketId_T*)&req_sn))
        {
            RTPS_Log_error(LOG_INTERFACE_FAIL_REQUEST);

            /* try requesting next sample */
            MIGRtpsBitmap_setBit(&bitmap, &req_sn, RTI_TRUE);
            continue;
        }
        
        if (packet->info.valid_data)
        {
            /* valid data --> send DATA submsg */
            payload_length = 
                RTPS_DATA_SIZE(NETIO_Packet_get_payload_length(packet));

            LOG_printDebug("\tRequested DATA, sn(%d,%u) len(%d)\n",
                           req_sn.high, req_sn.low, payload_length);

            NETIO_Packet_set_head(packet, 0-RTPS_DATA_SIZE(0));
            msg = (union RTPS_MESSAGES*)NETIO_Packet_get_head(packet);
            /* TODO: verify packet's rtps_flags are to be used here */
            if ((packet->info.rtps_flags & NETIO_RTPS_FLAGS_INLINEQOS) != 0)
            {
                data_flags |= RTPS_DATAFLAGS_Q;
            }
            RTPS_DATA_SET(&msg->data, data_flags, payload_length, 
                          RTPS_FLAGS_NONE, RTPS_DATA_INLINEQOS_OFFSET, 
                          reader_entity, writer_entity, packet->info.sn);
        }
        else 
        {
            /* invalid data --> send GAP submsg */
            gap_sn_end = req_sn;
            REDA_SequenceNumber_plusplus(&gap_sn_end);
            MIGRtpsBitmap_reset(&resend_bitmap,&gap_sn_end,0);
            payload_length = RTPS_GAP_SIZE(resend_bitmap._bitCount);
            NETIO_Packet_set_head(packet, 0-payload_length);
            msg = (union RTPS_MESSAGES*)NETIO_Packet_get_head(packet);
            RTPS_GAP_SET(&msg->gap, payload_length, reader_entity, 
                         writer_entity, req_sn, &resend_bitmap);
        }

        /* INFO_DST */
        NETIO_Packet_set_head(packet, 0-(RTPS_INFO_DST_SIZE()));
        msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(packet);
        RTPS_INFO_DST_SET(&msg->info_dst, &peer_entry->addr.value.guid.prefix);

        /* RTPS Header */
        NETIO_Packet_set_head(packet, 0-(RTPS_HEADER_SIZE()));
        msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(packet);
        RTPS_HEADER_SET(&msg->header, 
                        &intf->property.intf_address.value.guid.prefix);

        /* Send downstream */
        packet->source = intf->property.intf_address;
        packet->dests = &intf->local_dest_seq;
        *NETIO_AddressSeq_get_reference(packet->dests, 0) = peer_entry->addr;
        if (!RTPSSender_route_packet(intf, packet, RTPS_ROUTE_MODE_ALL))
        {
            RTPS_Log_error(LOG_RTPS_FAIL_ROUTE_PACKET);
            goto done;
        }

        if (!NETIO_Interface_return_loan(writer->upstr_intf,
                                        &intf->property.intf_address,
                                        packet,
                                        (NETIO_PacketId_T*)&req_sn))
        {
            RTPS_Log_error(LOG_INTERFACE_FAIL_RETURN_LOAN);
            goto done;
        }
    
        /* update bitmap that sample was resent */
        resent = RTI_TRUE;
        MIGRtpsBitmap_setBit(&bitmap, &req_sn, RTI_TRUE);
    }

    /* piggyback HB */
    if (resent)
    {
        NETIO_Packet_initialize(&intf->packet, intf->packet_buf,
                              RTPS_LOCAL_PACKET_SIZE, 0, &intf->local_dest_seq); 
        NETIO_Packet_set_source(&intf->packet, &intf->property.intf_address);
        NETIO_Packet_set_head(&intf->packet, 0-RTPS_HB_SIZE());
        msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(&intf->packet);
        writer->hbEpoch++;
        RTPS_HB_SET(&msg->hb,RTPS_FLAGS_NONE,  
                    reader_entity, writer_entity,
                    writer->firstSn, writer->lastSn, writer->hbEpoch);

        /* RTPS Header */
        NETIO_Packet_set_head(&intf->packet, 0-RTPS_HEADER_SIZE());
        msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(&intf->packet);
        RTPS_HEADER_SET(&msg->header, 
                        &intf->property.intf_address.value.guid.prefix);

        *NETIO_AddressSeq_get_reference(intf->packet.dests,0) = peer_entry->addr;
        if (!RTPSSender_route_packet(intf, &intf->packet, RTPS_ROUTE_MODE_ALL))
        {
            RTPS_Log_error(LOG_RTPS_FAIL_ROUTE_PACKET);
            goto done;
        }
    }

    ok = RTI_TRUE;
done:
#endif /* RTPS_RELIABILITY */
    return ok;
}


RTI_PRIVATE RTI_BOOL
RTPSReceiver_process_heartbeat(struct RTPSInterface *intf,
                               struct RTPSPeerEntry *peer_entry,
                               char *stream,
                               RTI_INT8 flags,
                               RTI_BOOL byte_swap)
{
    struct RTPSReader *reader;
    struct RTPSRemoteWriter *writer;
    struct REDA_SequenceNumber sn_first, sn_last;
    REDA_Epoch_T epoch;
    struct MIGRtpsBitmap bitmap;
    RTI_SIZE_T payload_length;
    union RTPS_MESSAGES *msg;
    RTPS_Entity writer_entity;
    RTPS_Entity reader_entity;
    RTI_BOOL is_full_ack = RTI_FALSE;

    /* only unanonymous readers process HEARTBEATs */
    if (intf->property.mode != RTPS_INTERFACEMODE_READER ||
        intf->property.anonymous)
    {
        return RTI_TRUE;
    }

    reader = &intf->endpoint.reader;
    writer = &peer_entry->info.writer;

    /* liveliness assertion */
    if ((flags & RTPS_HBFLAGS_L) != 0)
    {
        /* invalid data, only asserting liveliness */
        intf->packet.info.valid_data = 0;
        intf->packet.info.rtps_flags = NETIO_RTPS_FLAGS_LIVELINESS;

        if (!RTPSReceiver_forward_upstream(intf, NULL, &intf->packet, 
                                           &peer_entry->addr))
        {
            RTPS_Log_error(LOG_RTPS_FAIL_RECEIVER_FORWARD_UPSTREAM);
            return RTI_FALSE;
        }

        return RTI_TRUE;
    }

#if RTPS_RELIABILITY
    /* dont' process further if reader is not reliable */
    if ((reader->flags & RTPS_READER_FLAG_RELIABLE) == 0)
    {
        return RTI_TRUE;
    }

    CDR_deserialize_REDASequenceNumber(&stream, &sn_first, byte_swap);
    CDR_deserialize_REDASequenceNumber(&stream, &sn_last, byte_swap);
    MIGRtpsEpoch_deserialize(&epoch, &stream, byte_swap);

    /* ignore old heartbeat */
    if (!REDA_Epoch_isGreater(epoch, writer->epoch))
    {
        RTPS_LOG_DEBUG(LOG_EPOCH_LESS_THAN_LAST_EPOCH);
        return RTI_TRUE;
    }

    writer->epoch = epoch;

    LOG_printDebug("Processing HEARTBEAT, first(%d,%u) last(%d,%u)\n",
                      sn_first.high, sn_first.low, sn_last.high, sn_last.low);

    if (REDA_SequenceNumber_compare(&sn_first, &writer->bitmap._lead) > 0)
    {
        /* Reader missed some samples --> notify upstream of lost data */
        struct REDA_SequenceNumber diff;

        REDA_SequenceNumber_subtract(&diff, &sn_first, &writer->bitmap._lead);
        intf->packet.info.lost_sample_count = diff.low;
        intf->packet.info.lost_sample_sn = writer->bitmap._lead;
        intf->packet.info.valid_data = 0;
        intf->packet.info.rtps_flags |= NETIO_RTPS_FLAGS_LOST_DATA;
        if (!RTPSReceiver_forward_upstream(intf, NULL, &intf->packet, 
                                           &peer_entry->addr))
        {
            RTPS_Log_error(LOG_RTPS_FAIL_RECEIVER_FORWARD_UPSTREAM);
            return RTI_FALSE;
        }

        if (!MIGRtpsBitmap_shift(&writer->bitmap, &sn_first))
        {
            RTPS_Log_error(LOG_FAIL_UPDATE_DATA_RECEIVED_BITMAP);
            return RTI_FALSE;
        }
        MIGRtpsBitmap_getFirstBit(&writer->bitmap, &diff, RTI_FALSE);
        MIGRtpsBitmap_shift(&writer->bitmap, &diff);
    }

    bitmap = writer->bitmap;
    MIGRtpsBitmap_truncate(&bitmap, &sn_last);

    is_full_ack = (MIGRtpsBitmap_getFirstBit(&bitmap, NULL, RTI_FALSE) ? 
        RTI_FALSE : RTI_TRUE);

    /* reader is up-to-date */
    if (((flags & RTPS_HBFLAGS_F) != 0) && is_full_ack)
    {
        /* no response needed */
        RTPS_LOG_DEBUG(LOG_RTPS_READER_NO_HB_RESPONSE);
        return RTI_TRUE;
    }

    /* Send ACKNACK */
    MIGRtpsBitmap_invert(&bitmap); /* over-wire format */
    payload_length = RTPS_ACKNACK_SIZE(bitmap._bitCount);
    NETIO_Packet_initialize(&intf->packet, intf->packet_buf,
                           RTPS_LOCAL_PACKET_SIZE, 0, &intf->local_dest_seq);
    intf->packet.source = intf->property.intf_address;
    NETIO_Packet_set_head(&intf->packet, 0-payload_length);
    msg = (union RTPS_MESSAGES*)NETIO_Packet_get_head(&intf->packet);
    writer_entity = peer_entry->addr.value.guid.entity;
    reader_entity = intf->property.intf_address.value.guid.entity;
    reader->ackEpoch++;
    RTPS_ACKNACK_SET(&msg->acknack, (is_full_ack ? RTPS_ACKNACKFLAGS_F : 0), 
                     payload_length, reader_entity, 
                     writer_entity, &bitmap, reader->ackEpoch);

    /* RTPS Header */
    NETIO_Packet_set_head(&intf->packet, 0-RTPS_HEADER_SIZE());
    msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(&intf->packet);
    RTPS_HEADER_SET(&msg->header,&intf->property.intf_address.value.guid.prefix);

    *NETIO_AddressSeq_get_reference(intf->packet.dests, 0) = peer_entry->addr;
    if (!RTPSSender_route_packet(intf, &intf->packet, RTPS_ROUTE_MODE_ALL))
    {
        RTPS_Log_error(LOG_RTPS_FAIL_ROUTE_PACKET);
        return RTI_FALSE;
    }

#endif /* RTPS_RELIABILITY */
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
RTPSReceiver_process_data(struct RTPSInterface *intf,
                          struct RTPSPeerEntry *peer_entry,
                          NETIO_Packet_T *packet,
                          RTI_INT8 flags,
                          RTI_UINT32 data_len,
                          RTI_BOOL byte_swap)
{
    RTI_BOOL ok = RTI_FALSE;
    struct REDA_SequenceNumber sn_commit, orig_lead;
    struct REDA_SequenceNumber diff = REDA_SEQUENCE_NUMBER_ZERO;
    RTI_BOOL received;
    struct RTPSReader *reader;
    struct RTPSRemoteWriter *writer;
    struct REDA_Buffer data_buf = REDA_BUFFER_INVALID;
    RTI_BOOL check_committal = RTI_FALSE;

    /* only readers process DATA */
    if (intf->property.mode != RTPS_INTERFACEMODE_READER)
    {
        return RTI_TRUE;
    }

    reader = &intf->endpoint.reader;

    /* unanonymous reader needs a valid peer_entry */
    if (!intf->property.anonymous && (peer_entry == NULL))
    {
        return RTI_TRUE;
    }

    writer = (!intf->property.anonymous ? &peer_entry->info.writer : NULL);
    

    LOG_printDebug("%p: Processing DATA, sn(%d,%u) \n",intf,packet->info.sn.high, packet->info.sn.low);

    if ((!intf->property.anonymous) &&
        (reader->flags & RTPS_READER_FLAG_RELIABLE) != 0) /* anon cannot be reliable */
    {
        LOG_printDebug("processing reliable data\n");
        /* check whether this sample was already received, or is out-of-range */    
        if (!MIGRtpsBitmap_getBit(&writer->bitmap, &received, &packet->info.sn))
        {
            RTPS_LOG_DEBUG(LOG_RTPS_DATA_OUT_OF_RANGE);
            return RTI_TRUE;
        }
        else if (received)
        {
            LOG_printDebug("already received reliable data\n");
            RTPS_LOG_DEBUG(LOG_RTPS_DATA_ALREADY_RECEIVED);
            return RTI_TRUE;
        }
        /* assert this sample as received (revert if fail later) */
        if (!MIGRtpsBitmap_setBit(&writer->bitmap, &packet->info.sn, RTI_TRUE))
        {
            LOG_printDebug("out of range data\n");

            RTPS_Log_error(LOG_RTPS_DATA_OUT_OF_RANGE);
            goto done;
        }
        MIGRtpsBitmap_getFirstBit(&writer->bitmap, &sn_commit, RTI_FALSE);

        /* flag for reverting bitmap committal, if upstream's receive fails */
        check_committal = RTI_TRUE; 
    }
    else  
    {
        LOG_printDebug("best effort data\n");

        sn_commit = packet->info.sn;
        REDA_SequenceNumber_plusplus(&sn_commit);

        if (!intf->property.anonymous)/* best-effort, not anonymous */
        {
            if (
                /* (NETIO_ntohl(peer_entry->addr.value.rtps_guid.object_id) != MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT) && */
                (REDA_SequenceNumber_compare(&packet->info.sn,&writer->bitmap._lead) < 0))
            {
                RTPS_LOG_DEBUG(LOG_RTPS_DATA_ALREADY_RECEIVED);
                return RTI_TRUE;
            }

            orig_lead = writer->bitmap._lead;
            MIGRtpsBitmap_reset(&writer->bitmap, &sn_commit, 0);

            if (REDA_SequenceNumber_compare(&packet->info.sn, &orig_lead) > 0)
            {
                /* lost some sample(s) */
                REDA_SequenceNumber_subtract(&diff, &packet->info.sn, &orig_lead);
                packet->info.lost_sample_count = diff.low;
                packet->info.lost_sample_sn = orig_lead;
                packet->info.rtps_flags |= NETIO_RTPS_FLAGS_LOST_DATA;
            }
        }
    }

    if (flags & RTPS_DATAFLAGS_D)
    {
        packet->info.valid_data = 1;
    }
    else
    {
        packet->info.valid_data = 0;
    }

    /* committable_sn is actually first uncommittable sn */
    packet->info.committable_sn = sn_commit;
    if ((flags & RTPS_DATAFLAGS_Q) != 0)
    {
        packet->info.rtps_flags |= NETIO_RTPS_FLAGS_INLINEQOS;
    }
    REDA_Buffer_set(&data_buf, NETIO_Packet_get_head(packet), data_len);
    packet->info.protocol_data.rtps_data.inline_data = &data_buf;
    packet->info.protocol_id = NETIO_PROTOCOL_RTPS;
    packet->info.rtps_flags |= NETIO_RTPS_FLAGS_DATA;

    /* forward upstream, to modules expecting msgs from src */
    if (!RTPSReceiver_forward_upstream(intf, &received, packet, 
                              (peer_entry == NULL ? NULL : &peer_entry->addr)))
    {
        RTPS_Log_error(LOG_RTPS_FAIL_RECEIVER_FORWARD_UPSTREAM);
        return RTI_FALSE;
    }

    /* reliable commit of successful reception */
    if (check_committal && received &&
        !MIGRtpsBitmap_shift(&writer->bitmap, &sn_commit))
    {
        RTPS_Log_error(LOG_FAIL_UPDATE_DATA_RECEIVED_BITMAP);
        goto done;
    }

    ok = RTI_TRUE;
 done:
    return ok;
}

/* receive()
 
   An Receives messages sent to its external (link) interface.
    
*/
RTI_PRIVATE RTI_BOOL
RTPSInterface_receive(NETIO_Interface_T *netio_intf, /* self's external intf */
                      struct NETIO_Address *source, /* downstream addr */
                      struct NETIO_Address *dst, /* self addr  */
                      NETIO_Packet_T *packet)
{
    struct RTPSInterface *self = (struct RTPSInterface *)netio_intf;
    RTI_BOOL ok = RTI_FALSE;
    struct RTPSBindEntry *bind_entry = NULL;
    DB_Cursor_T cursor = NULL;
    DB_ReturnCode_T db_rc;
    RTI_BOOL byte_swap;
    struct RTPS_Header *rtps_header = NULL;
    union RTPS_MESSAGES *rtps_msg = NULL;
    union RTPS_MESSAGES tmp_msg;
    char *stream = NULL; /* ptr to current submessage, for deserialization */
    struct NETIOBindEntryKey bind_key_low, bind_key_high;
    MIGRtpsObjectId OID_MAX = MIG_RTPS_OBJECT_ID_MAX;
    RTI_BOOL stateless_sender, stateless_receiver;
    RTI_SIZE_T submsg_offset, submsg_length;
    RTI_SIZE_T pkt_head, pkt_curr, pkt_tail;

    LOG_printDebug("RTPS: fwd: src: %d/%x.%x.%x.%x ==> dst %d/%x.%x.%x.%x, length (%d)\n",
            source->port,
            source->value.rtps_guid.host_id,
            source->value.rtps_guid.app_id,
            source->value.rtps_guid.instance_id,
            source->value.rtps_guid.object_id,
            dst->port,
            dst->value.rtps_guid.host_id,
            dst->value.rtps_guid.app_id,
            dst->value.rtps_guid.instance_id,
            dst->value.rtps_guid.object_id,
            NETIO_Packet_get_payload_length(packet));


    /* bad param check */
    if (netio_intf == NULL || packet == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    DB_Database_lock(self->property._parent._parent.db);

    /* intf must be enabled to receive */
    if (self->_parent.state != NETIO_INTERFACESTATE_ENABLED)
    {
        RTPS_LOG_WARNING(LOG_INTERFACE_NOT_ENABLED);
        ok = RTI_TRUE; /* dropped msg is not fatal */
        goto done;
    }

    if (!RTPSInterface_valid_pdu(netio_intf, packet))
    {
        RTPS_LOG_WARNING(LOG_RTPS_INVALID_PACKET);
        ok = RTI_TRUE; /* dropped msg is not fatal */
        goto done;
    }

    /* receive only over external intf */
    if (self->property.mode != RTPS_INTERFACEMODE_EXTERNAL_RECEIVER)
    {
        RTPS_LOG_WARNING(LOG_RTPS_UNSUPPORTED_INTERFACE);
        ok = RTI_TRUE; /* dropped msg is not fatal */
        goto done;
    }

    rtps_header = (struct RTPS_Header *)NETIO_Packet_get_head(packet);

    self->context.src.prefix = rtps_header->guid_prefix;

    /* context.dst defaults to reader's participant's GUID prefix */
    OSAPI_Memory_zero(&self->context.dst.entity, 
                      sizeof(struct NETIO_GuidEntity));
    OSAPI_Memory_copy(&self->context.dst.prefix, 
                      &self->property.intf_address.value.guid.prefix,
                      sizeof(struct NETIO_GuidPrefix));

    NETIO_Packet_set_head(packet, RTPS_HEADER_SIZE());
  
    packet->info.protocol_id = NETIO_PROTOCOL_RTPS;
    packet->info.protocol_data.rtps_data.guid_prefix = rtps_header->guid_prefix;

    /* process each submessage in packet */
    while (NETIO_Packet_get_payload_length(packet) > 0)
    {   
        LOG_printDebug("payload length(%d)\n",NETIO_Packet_get_payload_length(packet));
        /* parse submessage header: kind, flags, length */
        rtps_msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(packet);
        tmp_msg.submsg.kind = rtps_msg->submsg.kind;
        tmp_msg.submsg.flags = rtps_msg->submsg.flags;
        byte_swap = RTPSInterface_byte_swap(tmp_msg.submsg.flags);
        stream = (char *)&rtps_msg->submsg.length;

        if (tmp_msg.submsg.kind == RTPS_ACKNACK_KIND)
        {
            LOG_printDebug("Received ACKNACK\n");
        }

        /* TODO: don't need CDR representation; can/should not rely on RTPS */
        CDR_deserialize_UnsignedShort(&stream, &tmp_msg.submsg.length, byte_swap);
        submsg_length = (RTI_SIZE_T)tmp_msg.submsg.length;

        /* Handle zero-length submessages */
        if ((submsg_length == 0) && 
            ((tmp_msg.submsg.kind != RTPS_INFO_TS_KIND) && 
             (tmp_msg.submsg.kind != RTPS_PAD_KIND)))
        {
            /* length is remaining bytes */
            /* Precondition: payload_length not too large; won't be truncated */
            submsg_length = NETIO_Packet_get_payload_length(packet);
        }
        else
        {
            submsg_length += 4; /* account for submsg header */
        }

        /* Handle INFO submsgs */
        if (RTPSSubmessage_is_info(tmp_msg.submsg.kind))
        {
            switch (tmp_msg.submsg.kind)
            {
            case RTPS_INFO_DST_KIND:

                OSAPI_Memory_copy(&self->context.dst,
                                 &rtps_msg->info_dst.guid_prefix, 12);
                LOG_printDebug("Received INFO_DST\n");
                break;

            case RTPS_INFO_TS_KIND: 
   
                if ((tmp_msg.submsg.flags & RTPS_INFO_TSFLAGS_I) != 0)
                {
                    OSAPI_NtpTime_deserialize(&self->context.timestamp,
                                             &stream, byte_swap);
                }
                LOG_printDebug("Received INFO_TS\n");
                break;

            case RTPS_INFO_REPLY_KIND: 
            case RTPS_INFO_REPLY_IP4_KIND:

                /* TODO: add support */
                LOG_printDebug("Received INFO_REPLY\n");
                break;

            case RTPS_INFO_SRC_KIND:

                /* TODO: add support */
                LOG_printDebug("Received INFO_SRC\n");
                break;

            default:
                /* ignore but warn of received unknown submessages */
                RTPS_LOG_WARNING(LOG_RTPS_UNKNOWN_SUBMESSAGE);
                break;
            }
            NETIO_Packet_set_head(packet, submsg_length);
            continue;
        }

        /* Get DATA inlineQos, extraFlags, seq num */
        submsg_offset = 12;  /* includes submsg header */
        if (tmp_msg.submsg.kind == RTPS_DATA_KIND)
        {
            CDR_deserialize_UnsignedShort(
               &stream, &tmp_msg.data.flags, byte_swap);
            CDR_deserialize_UnsignedShort(
               &stream, &tmp_msg.data.inlineQosOffset, byte_swap);
            submsg_offset = 24;
        }

        /* Update context src/dst from submsg */
        if (RTPSSubmessage_from_writer(tmp_msg.submsg.kind))
        {
            OSAPI_Memory_copy(&self->context.dst.entity, stream, 4); stream += 4;
            OSAPI_Memory_copy(&self->context.src.entity, stream, 4); stream += 4;
        }
        else
        {
            OSAPI_Memory_copy(&self->context.src.entity, stream, 4); stream += 4;
            OSAPI_Memory_copy(&self->context.dst.entity, stream, 4); stream += 4;
        }

        if (tmp_msg.submsg.kind == RTPS_DATA_KIND)
        {
            CDR_deserialize_REDASequenceNumber(&stream, &packet->info.sn, byte_swap);
        }

        /* save positions after deserializing submsg headers, entity IDs; for
           DATA, also deserialized extra flags, inlineQoSOffset, and seq num.
        */
        NETIO_Packet_set_head(packet, submsg_offset);
        NETIO_Packet_save_positions_to(packet, &pkt_head, &pkt_curr, &pkt_tail);
        submsg_length -= submsg_offset;

        stateless_sender = RTPSInterface_addr_is_stateless_sender(&self->context.src);
        if (stateless_sender)
        {
            /* from stateless writer --> received by all stateless readers of
               corresponding entity ID */
            db_rc = DB_Table_select_all_default(self->_parent._btable, &cursor);
        }
        else 
        {
            NETIO_Address_set_guid(&bind_key_low.source, 0, &self->context.src);
            NETIO_Address_set_guid(&bind_key_low.destination, 0, &self->context.dst);
            bind_key_high = bind_key_low;
            
            /* Search full range if dest id is unknown */
            if (RTPSEntity_is_unknown(&self->context.dst.entity))
            {
                bind_key_high.destination.value.rtps_guid.host_id = OID_MAX;
                bind_key_high.destination.value.rtps_guid.app_id = OID_MAX;
                bind_key_high.destination.value.rtps_guid.instance_id = OID_MAX;
                bind_key_high.destination.value.rtps_guid.object_id = OID_MAX;
            }

            db_rc = DB_Table_select_range(
               self->_parent._btable, DB_TABLE_DEFAULT_INDEX, &cursor,
               (DB_Key_T)&bind_key_low, (DB_Key_T)&bind_key_high);
        }

        if (db_rc != DB_RETCODE_OK)
        {
            RTPS_LOG_WARNING(LOG_DB_FAIL_SELECT_RANGE);
            NETIO_Packet_set_head(packet, submsg_length);
            continue;
        }

        /* got cursor, not iterate */
        db_rc = DB_Cursor_get_next(cursor,(DB_Record_T*)&bind_entry);
        while (db_rc == DB_RETCODE_OK)
        {
            if (bind_entry->_rtps_intf == NULL)
            {
                /* already deleted, skip */
                goto nextEntry;
            }

            stateless_receiver = RTPSInterface_addr_is_stateless_receiver(
                 &bind_entry->_rtps_intf->property.intf_address.value.guid);

            /* skip this entry if:
                - interface is not enabled
                - stateless is talking with non-stateless
            */
            if ((bind_entry->_rtps_intf->_parent.state != 
                 NETIO_INTERFACESTATE_ENABLED) ||
                (stateless_sender ^ stateless_receiver))
            {
                goto nextEntry;
            }

            switch (tmp_msg.submsg.kind)
            {
            case RTPS_ACKNACK_KIND:

                if (!RTPSReceiver_process_acknack(
                   bind_entry->_rtps_intf,
                   bind_entry->peer_ref,
                   stream, 
                   tmp_msg.submsg.flags,
                   byte_swap))
                {
                    RTPS_Log_error(LOG_RTPS_FAIL_RECEIVE_ACKNACK);
                }
                break;

            case RTPS_DATA_KIND:

                if (!RTPSReceiver_process_data(
                   bind_entry->_rtps_intf, 
                   bind_entry->peer_ref,
                   packet, 
                   tmp_msg.submsg.flags,
                   submsg_length, 
                   byte_swap))
                {
                    RTPS_Log_error(LOG_RTPS_FAIL_RECEIVE_DATA);
                }
                break;

            case RTPS_GAP_KIND:

                if (!RTPSReceiver_process_gap(
                   bind_entry->_rtps_intf,
                   bind_entry->peer_ref,
                   packet,
                   stream, 
                   byte_swap))
                {
                    RTPS_Log_error(LOG_RTPS_FAIL_RECEIVE_GAP);
                }
                break;

            case RTPS_HEARTBEAT_KIND:

                if (!RTPSReceiver_process_heartbeat(
                   bind_entry->_rtps_intf, 
                   bind_entry->peer_ref,
                   stream, 
                   tmp_msg.submsg.flags, 
                   byte_swap))
                {
                    RTPS_Log_error(LOG_RTPS_FAIL_RECEIVE_HEARTBEAT);
                }
                break;

            default: 
                /* ignore and warn about unknown submessages */
                RTPS_LOG_WARNING(LOG_RTPS_UNKNOWN_SUBMESSAGE);
                break;
            }
 nextEntry:
            NETIO_Packet_restore_positions_from(packet,pkt_head,pkt_curr,pkt_tail);
            db_rc = DB_Cursor_get_next(cursor, (DB_Record_T*)&bind_entry);
        }
        DB_Cursor_finish(self->_parent._btable, cursor);
        NETIO_Packet_set_head(packet, submsg_length);
    }

    ok = RTI_TRUE;
done:
    DB_Database_unlock(self->property._parent._parent.db);
    return ok;
}


OSAPITimeoutOp_t
RTPSWriter_on_periodic_heartbeat(struct OSAPITimeoutUserData *storage)
{
#if RTPS_RELIABILITY
    OSAPITimeoutOp_t result = OSAPI_TIMEOUT_OP_AUTOMATIC;
    struct RTPSWriter *writer = NULL;
    struct RTPSInterface *self = (struct RTPSInterface *)storage->field[0];

    union RTPS_MESSAGES *msg = NULL;
    RTPS_Entity *dst_id = NULL;
    DB_Cursor_T cursor;
    DB_ReturnCode_T db_rc;
    struct RTPSRouteEntry *route_entry = NULL;
    RTPS_Entity writer_entity = RTPS_ENTITY_UNKNOWN;
    RTPS_Entity reader_entity = RTPS_ENTITY_UNKNOWN;

    DB_Database_lock(self->property._parent._parent.db);

    writer = &self->endpoint.writer;

    if (self->_parent.state != NETIO_INTERFACESTATE_ENABLED)
    {
        writer->flags &= ~RTPS_WRITER_FLAG_HB_EVENT_ACTIVE; 
        result =  OSAPI_TIMEOUT_OP_MANUAL; /* don't reschedule */
        goto done;
    }

    /* TODO: optimization, disable event when no reliable readers */

    /* don't send HB if queue is empty */
    if (REDA_SequenceNumber_isZero(&writer->firstSn))
    {
        goto done;
    }

    /* send periodic hb to each route entry */
    NETIO_Packet_initialize(&self->packet, self->packet_buf,
                           RTPS_LOCAL_PACKET_SIZE, 0, &self->local_dest_seq); 
    NETIO_Packet_set_source(&self->packet, &self->property.intf_address);
    NETIO_Packet_set_head(&self->packet, 0-RTPS_HB_SIZE());
    msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(&self->packet);

    /* save ref to dest entity ID; update for directed message */
    dst_id = &msg->hb.reader;
        
    /* iterate over route table, sending */
    writer_entity = self->property.intf_address.value.guid.entity;

    writer->hbEpoch++;
    RTPS_HB_SET(&msg->hb,RTPS_FLAGS_NONE,  
                reader_entity,writer_entity,
                writer->firstSn, writer->lastSn, writer->hbEpoch);

    /* RTPS Header */
    NETIO_Packet_set_head(&self->packet, 0-(RTPS_HEADER_SIZE()));
    msg = (union RTPS_MESSAGES *)NETIO_Packet_get_head(&self->packet);
    RTPS_HEADER_SET(&msg->header, &self->property.intf_address.value.guid.prefix);

    if (DB_Table_select_all_default(self->_parent._rtable, 
                                   &cursor) != DB_RETCODE_OK)
    {
        RTPS_Log_error(LOG_DB_FAIL_SELECT_ALL);
        goto done; /* reschedule */
    }

    db_rc = DB_Cursor_get_next(cursor, (DB_Record_T*)&route_entry);
    while (db_rc == DB_RETCODE_OK)
    {
        /* directed to each reliable reader */
        if (route_entry->peer_ref->info.reader.reliable)
        {
            /* directed to route's entity */
            *dst_id = route_entry->_parent.destination.value.guid.entity;
            *NETIO_AddressSeq_get_reference(self->packet.dests, 0) = 
                route_entry->_parent.intf_address;
            if (!NETIO_Interface_send(route_entry->_parent.intf,
                                     &self->_parent,
                                     &route_entry->_parent.intf_address,
                                     &self->packet))
            {
                RTPS_Log_error(LOG_FAIL_SEND_PERIODIC_HEARTBEAT);
                goto done; /* reschedule */
            }
        }
        db_rc = DB_Cursor_get_next(cursor, (DB_Record_T*)&route_entry);
    }
    DB_Cursor_finish(self->_parent._rtable, cursor);
done:
    DB_Database_unlock(self->property._parent._parent.db);
    return result;

#else
    /* event should not be running, stop */
    return OSAPI_TIMEOUT_OP_MANUAL;
#endif
}


/* set state to ENABLED or DISABLED */
RTI_PRIVATE RTI_BOOL
RTPSInterface_set_state(NETIO_Interface_T *netio_intf,
                        NETIO_InterfaceState_T state)
{
    struct RTPSInterface *self = (struct RTPSInterface *)netio_intf;
    struct RTPSInterface *ext_intf = NULL;

    /* bad param check */
    if (netio_intf == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return RTI_FALSE;
    }

    self->_parent.state = state;

    if (state == NETIO_INTERFACESTATE_ENABLED)
    {
        /* enable external interface */
        ext_intf = (struct RTPSInterface *)REDA_Indexer_find_entry(
           self->factory->ext_intf_index, &self->property.intf_address);
        if (ext_intf == NULL)
        {
            RTPS_Log_error(LOG_FAIL_GET_EXT_INTF);
            return RTI_FALSE;
        }
        ext_intf->_parent.state = NETIO_INTERFACESTATE_ENABLED;

#if RTPS_RELIABILITY
        /* activate periodic HB if necessary */
        if ((self->property.mode == RTPS_INTERFACEMODE_WRITER) &&
            (self->endpoint.writer.flags & RTPS_WRITER_FLAG_RELIABLE) &&
            !(self->endpoint.writer.flags & RTPS_WRITER_FLAG_HB_EVENT_ACTIVE))
        {
            struct OSAPITimeoutUserData storage;
            RTI_INT32 sec, ms;

            OSAPI_NtpTime_unpackToMillisec(
               &sec, &ms, &self->endpoint.writer.hbPeriod);
            storage.field[0] = (void *)self;

            if (!OSAPI_Timer_create_timeout(self->factory->timer,
                                           &self->endpoint.writer.hbEvent,
                                           ((sec * 1000) + ms),
                                           OSAPI_TIMER_PERIODIC,
                                           RTPSWriter_on_periodic_heartbeat,
                                           &storage))
            {
                RTPS_Log_error_p1(LOG_FAIL_POST_PERIODIC_HEARTBEAT, 0);
                return RTI_FALSE;
            }
            self->endpoint.writer.flags |= RTPS_WRITER_FLAG_HB_EVENT_ACTIVE;
        }
#endif
    }

    return RTI_TRUE;
}

/******************************************************************************
 *
 * RTPS Component Interface
 */
RTI_PRIVATE struct NETIO_InterfaceI RTPSInterface_g_intf =
{
    {DEFAULT_COMPONENT_RUN},
    RTPSInterface_send,                  /* send */
    RTPSInterface_ack,                   /* ack */
    RTPSInterface_request,               /* request */
    RTPSInterface_return_loan,           /* return_loan */
    RTPSInterface_discard,               /* discard */
    RTPSInterface_add_route,             /* add_route */
    RTPSInterface_delete_route,          /* delete_route */
    NULL,                                /* reserve_public_address */
    RTPSInterface_bind,                  /* bind */
    RTPSInterface_unbind,                /* unbind */
    RTPSInterface_receive,               /* receive */
    RTPSInterface_get_external_interface,/* get_external_interface */
    RTPSInterface_bind_external,         /* bind_external */
    RTPSInterface_unbind_external,       /* unbind_external */
    RTPSInterface_set_state,             /* set_state */
    NULL                                 /* release_public_address */
};

/* ------------------------------------------------------------------------ */
/*                   Plugin factory                                         */
/* ------------------------------------------------------------------------ */

RTI_PRIVATE RT_Component_T*
RTPSInterfaceFactory_create_component(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentProperty *property,
        struct RT_ComponentListener *listener)
{
    struct RTPSInterface *retval = NULL;

    if (factory == NULL || property == NULL)
    {
        RTPS_Log_error(LOG_FAIL_BAD_PARAMETER);
        return NULL;
    }

    retval = RTPSInterface_create(
            (struct RTPSInterfaceFactory*)factory,
            (const struct RTPSInterfaceProperty *const)property,
            (const struct NETIO_InterfaceListener *const)listener);

    return (retval == NULL ? NULL : &retval->_parent._parent);
}

RTI_PRIVATE void
RTPSInterfaceFactory_delete_component(struct RT_ComponentFactory *factory,
                                     RT_Component_T *component)
{
    struct RTPSInterfaceFactory *rtps_factory = 
        (struct RTPSInterfaceFactory*)factory;
    struct RTPSInterface *self = (struct RTPSInterface*)component;

    if (factory != NULL && component != NULL)
    {
        RTPSInterface_delete(self);
        --rtps_factory->instance_counter;
    }
}


RTI_PRIVATE struct RT_ComponentFactory*
RTPSInterfaceFactory_initialize(
   struct RT_ComponentFactoryProperty *property,
   struct RT_ComponentFactoryListener *listener)
{
    struct RTPSInterfaceFactory *factory = &RTPSInterfaceFactory_g_factory;
    struct OSAPI_TimerProperty timer_prop = OSAPI_TimerProperty_INITIALIZER;

    factory->_parent.intf = &RTPSInterfaceFactory_g_intf;
    factory->_parent._factory = NULL;
    factory->_parent._id._value._high = 0;
    factory->_parent._id._value._low = 0;

    factory->instance_counter = 0;
    factory->_initialized = RTI_FALSE;

    factory->timer = OSAPI_Timer_new(&timer_prop, NULL);
    if (factory->timer == NULL)
    {
        RTPS_Log_error(LOG_FAIL_CREATE_EVENT_TIMER);
        return NULL;
    }
    RTPSInterfaceFactory_g_factory._parent._factory = &factory->_parent;

    return &RTPSInterfaceFactory_g_factory._parent;
}

RTI_PRIVATE void
RTPSInterfaceFactory_finalize(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener)
{
    struct RTPSInterfaceFactory *rtps_factory = &RTPSInterfaceFactory_g_factory;

    if (factory == &rtps_factory->_parent)
    {
        if (rtps_factory->timer != NULL)
        {
            OSAPI_Timer_delete(rtps_factory->timer);
        }
        if (rtps_factory->_initialized)
        {
            (void)REDA_Indexer_delete(rtps_factory->ext_intf_index);
            rtps_factory->_initialized = RTI_FALSE;
        }
    }
}


RTI_PRIVATE struct RT_ComponentFactoryI RTPSInterfaceFactory_g_intf =
{
    RT_COMPONENT_FACTORY_ID_DEFAULT,
    RTPSInterfaceFactory_initialize,
    RTPSInterfaceFactory_finalize,
    RTPSInterfaceFactory_create_component,
    RTPSInterfaceFactory_delete_component
};

struct RT_ComponentFactoryI*
RTPSInterfaceFactory_get_interface(void)
{
    return &RTPSInterfaceFactory_g_intf;
}




