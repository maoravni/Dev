/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_interface_h
#define netio_interface_h


/*e \file
  \brief NETIO Interface API
*/

/*e \defgroup NETIO_InterfaceClass NETIO Interface
    \ingroup NETIOModule

    \brief NETIO Interface API
 */
#ifndef netio_dll_h
#include "netio/netio_dll.h"
#endif

#ifndef osapi_config_h
#include "osapi/osapi_config.h"
#endif

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef reda_sequenceNumber_h
#include "reda/reda_sequenceNumber.h"
#endif

#ifndef db_api_h
#include "db/db_api.h"
#endif

#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif

#ifndef netio_address_h
#include "netio/netio_address.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define NETIO_TRANSPORT_INTERFACE_FLAG_UP          (0x1)

/*e \ingroup NETIOTransportInterfaceType
  \brief This interface supports broadcast
 */
#define NETIO_TRANSPORT_INTERFACE_FLAG_BROADCAST   (0x2)

/*e \ingroup NETIOTransportInterfaceType
  \brief This interface supports a loopback interface
 */
#define NETIO_TRANSPORT_INTERFACE_FLAG_LOOPBACK    (0x4)

/*e \ingroup NETIOTransportInterfaceType
  \brief This interface is a point-to-point connection.
 */
#define NETIO_TRANSPORT_INTERFACE_FLAG_POINTOPOINT (0x8)

/*e \ingroup NETIOTransportInterfaceType
  \brief This interface supports multicast.
*/
#define NETIO_TRANSPORT_INTERFACE_FLAG_MULTICAST   (0x10)

/*e \ingroup NETIOTransportInterfaceType
  \brief This interface has all resources allocated.
*/
#define NETIO_TRANSPORT_INTERFACE_FLAG_RUNNING     (0x20)

#define NETIO_PROTOCOL_INTRA        1
#define NETIO_PROTOCOL_RTPS         2
#define NETIO_PROTOCOL_UDP          3

#define NETIO_RTPS_FLAGS_DISPOSE     (0x1)
#define NETIO_RTPS_FLAGS_UNREGISTER  (0x2)
#define NETIO_RTPS_FLAGS_LIVELINESS  (0x4)
#define NETIO_RTPS_FLAGS_INLINEQOS   (0x8)
#define NETIO_RTPS_FLAGS_LOST_DATA   (0x10)
#define NETIO_RTPS_FLAGS_COMMIT_DATA (0x20)
#define NETIO_RTPS_FLAGS_DATA        (0x40)
#define NETIO_RTPS_FLAGS_LAST_WRITE_FOR_SN (0x80)

struct NETIO_RtpsInfo
{
    struct REDA_Buffer *inline_data;
    struct NETIO_GuidPrefix guid_prefix;
};

struct NETIO_IntraInfo
{
    const void *user_data;
    struct REDA_Buffer *inline_data;
};

#define NETIO_IntraInfo_INITIALIZER \
{ \
    NULL, /* user_data */ \
    NULL, /* inline_data */ \
}

struct NETIO_PacketInfo
{
    struct REDA_SequenceNumber sn;
    struct OSAPI_NtpTime timestamp;
    RTI_UINT8 priority;
    RTI_UINT8 valid_data;
    struct NETIO_Guid instance; 
    RTI_UINT8 rtps_flags;
    struct REDA_SequenceNumber committable_sn;
    struct REDA_SequenceNumber first_available_sn;
    struct REDA_SequenceNumber lost_sample_sn;
    RTI_UINT32 lost_sample_count;
    RTI_INT32 protocol_id;
    struct
    {
        struct NETIO_IntraInfo intra_info;
        struct NETIO_RtpsInfo rtps_data;
    } protocol_data;
};

#define NETIO_PacketInfo_INITIALIZER \
{ \
    REDA_SEQUENCE_NUMBER_ZERO, /* sn */\
    OSAPI_NTP_TIME_ZERO, /* timestamp */ \
    0, /* priority */ \
    0, /* valid_data */ \
    NETIO_ADDRESS_GUID_UNKNOWN, /* instance */ \
    0, /* flags */ \
    REDA_SEQUENCE_NUMBER_ZERO, /* committable_sn */ \
    REDA_SEQUENCE_NUMBER_ZERO, /* first_available_sn */ \
    REDA_SEQUENCE_NUMBER_ZERO, /* lost_sample_sn */ \
    0, /* lost sample count */ \
    0 /* protocol_id */\
}

struct NETIO_Packet
{
    struct NETIO_Address source;
    char *buffer;
    RTI_SIZE_T length;
    RTI_SIZE_T max_length;
    RTI_SIZE_T head_pos;
    RTI_SIZE_T current_pos; 
    RTI_SIZE_T tail_pos;
    RTI_SIZE_T saved_head_pos;
    RTI_SIZE_T saved_current_pos; 
    RTI_SIZE_T saved_tail_pos;
    RTI_SIZE_T end_pos;
    struct NETIO_PacketInfo info;

    /* sequence of destinations for single packet.  Memory owned by sender. */
    struct NETIO_AddressSeq *dests;
};

/* TODO: make packet mem cpy */
/* TODO: align current_hdr/tail_pos to 4 */

typedef struct NETIO_Packet NETIO_Packet_T;

/* TODO: what is this packetId currently?  is this SN? */
typedef struct REDA_SequenceNumber NETIO_PacketId_T;

#define NETIO_Packet_INITIALIZER \
{\
    NETIO_Address_INITIALIZER,\
    NULL, /* buffer */\
    0, /* length */\
    0, /* max_length */\
    0, /* head_pos */\
    0, /* current_pos */\
    0, /* tail_pos */\
    0, /* saved_head_pos */\
    0, /* saved_current_pos */\
    0, /* saved_tail_pos */\
    0, /* end_pos */\
    NETIO_PacketInfo_INITIALIZER,\
    NULL\
}

NETIODllExport RTI_BOOL
NETIO_Packet_set_head(NETIO_Packet_T *packet,RTI_INT32 delta);

NETIODllExport RTI_BOOL
NETIO_Packet_set_current(NETIO_Packet_T *packet,RTI_INT32 delta);

NETIODllExport RTI_BOOL
NETIO_Packet_set_tail(NETIO_Packet_T *packet,RTI_INT32 delta);

NETIODllExport void*
NETIO_Packet_get_head(NETIO_Packet_T *packet);

NETIODllExport void*
NETIO_Packet_get_current(NETIO_Packet_T *packet);

NETIODllExport void*
NETIO_Packet_get_tail(NETIO_Packet_T *packet);

NETIODllExport RTI_BOOL
NETIO_Packet_initialize(NETIO_Packet_T *packet,
                       void *init_buffer,
                       RTI_SIZE_T init_length,
                       RTI_SIZE_T trailer_length,
                       struct NETIO_AddressSeq *dest_seq);

NETIODllExport RTI_BOOL
NETIO_Packet_finalize(NETIO_Packet_T *packet);

NETIODllExport struct NETIO_PacketInfo*
NETIO_Packet_get_info(NETIO_Packet_T *packet);

NETIODllExport void
NETIO_Packet_get_source(NETIO_Packet_T *packet,struct NETIO_Address *src);

NETIODllExport void
NETIO_Packet_set_source(NETIO_Packet_T *packet,struct NETIO_Address *src);

NETIODllExport RTI_SIZE_T
NETIO_Packet_get_length(NETIO_Packet_T *packet);

NETIODllExport RTI_SIZE_T
NETIO_Packet_get_max_length(NETIO_Packet_T *packet);

NETIODllExport RTI_SIZE_T
NETIO_Packet_get_payload_length(NETIO_Packet_T *packet);

NETIODllExport void
NETIO_Packet_save_positions(NETIO_Packet_T *packet);

NETIODllExport void
NETIO_Packet_restore_positions(NETIO_Packet_T *packet);

NETIODllExport void
NETIO_Packet_save_positions_to(NETIO_Packet_T *packet,
                               RTI_SIZE_T *head,
                               RTI_SIZE_T *current,
                               RTI_SIZE_T *tail);

NETIODllExport void
NETIO_Packet_restore_positions_from(NETIO_Packet_T *packet,
                                    RTI_SIZE_T head,
                                    RTI_SIZE_T current,
                                    RTI_SIZE_T tail);

/******************************************************************************/


/*e \ingroup NETIO_InterfaceClass
 *
 *  Abstract NETIO property object
 */
struct NETIO_InterfaceProperty
{
    struct RT_ComponentProperty _parent;
    RTI_INT32 max_routes;
    RTI_INT32 max_binds;
};

/*e \ingroup NETIO_InterfaceClass
 *
 */
#define NETIO_InterfaceProperty_INITIALIZER \
{\
    RT_ComponentProperty_INITIALIZER,\
    0,\
    0\
}

/*e \ingroup NETIO_InterfaceClass
 *
 *  Abstract NETIO listener
 */
struct NETIO_InterfaceListener
{
    struct RT_ComponentListener _parent;
};

#define NETIO_InterfaceListener_INITIALIZE {\
    RT_ComponentListener_INITIALIZER \
}

/*e \ingroup NETIO_InterfaceClass
 *
 *  Abstract NETIO property object
 */
struct NETIO_InterfaceFactoryProperty
{
    struct RT_ComponentFactoryProperty _parent;
    RTI_INT32 max_routes;
};

/*e \ingroup NETIO_InterfaceClass
 *
 */
#define NETIO_InterfaceFactoryProperty_INITIALIZER \
{\
    RT_ComponentFactoryProperty_INITIALIZER,\
    0\
}

typedef enum
{
    NETIO_INTERFACESTATE_CREATED,
    NETIO_INTERFACESTATE_ENABLED,
    NETIO_INTERFACESTATE_DISABLED
} NETIO_InterfaceState_T;

/*e \ingroup NETIO_InterfaceClass
 *
 *  Abstract NETIO interface object
 */
struct NETIO_Interface
{
    struct RT_Component _parent;
    DB_Table_T _rtable;
    DB_Table_T _btable;
    struct NETIO_InterfaceListener _listener;
    struct NETIO_Address local_address;
    NETIO_InterfaceState_T state;
};

typedef struct NETIO_Interface NETIO_Interface_T;

struct NETIOBindProperty
{
    RTI_UINT32 strength;
};

#define NETIOBindProperty_INITIALIZER \
{\
    0\
}

struct NETIORouteProperty
{
    RTI_INT32 _dummy;
};

#define NETIORouteProperty_INITIALIZER \
{\
    0 \
}

struct NETIO_InterfaceI;

NETIODllExport RTI_BOOL
NETIO_Interface_Table_name_from_id(char *tbl_name,
                                  union RT_ComponentFactoryId *id,
                                  char suffix,
                                  RTI_INT32 instance);

NETIODllExport RTI_BOOL
NETIO_Interface_initialize(struct NETIO_Interface *netio,
                          struct NETIO_InterfaceI *netio_intf,
                          const struct NETIO_InterfaceProperty *const property,
                          const struct NETIO_InterfaceListener *const listener);

NETIODllExport RTI_BOOL
NETIO_Interface_finalize(struct NETIO_Interface *netio);

NETIODllExport RTI_INT32
NETIO_Interface_compare_route(DB_Select_T opcode,RTI_INT32 flags,
        const DB_Record_T op1, void *op2, void *op3);

NETIODllExport RTI_INT32
NETIO_Interface_compare_bind(DB_Select_T opcode,RTI_INT32 flags,
        const DB_Record_T op1, void *op2, void *op3);

/*e \ingroup NETIO_InterfaceClass
 */

typedef RTI_BOOL
(*NETIO_Interface_sendFunc)(
        NETIO_Interface_T *self,
        struct NETIO_Interface *source,
        struct NETIO_Address *destination,
        NETIO_Packet_T *packet);

/*e \ingroup NETIO_InterfaceClass
 */
typedef RTI_BOOL
(*NETIO_Interface_receiveFunc)(
        NETIO_Interface_T *netio_intf,
        struct NETIO_Address *src_addr,
        struct NETIO_Address *dst_addr,
        NETIO_Packet_T *packet);

/*e \ingroup NETIO_InterfaceClass
 */
typedef RTI_BOOL
(*NETIO_Interface_ackFunc)(
        NETIO_Interface_T *self,
        struct NETIO_Address *source,
        NETIO_PacketId_T *packet_id);

typedef RTI_BOOL
(*NETIO_Interface_requestFunc)(
        NETIO_Interface_T *self,
        struct NETIO_Address *source,
        struct NETIO_Address *dest,
        NETIO_Packet_T **packet,
        NETIO_PacketId_T *packet_id);

typedef RTI_BOOL
(*NETIO_Interface_return_loanFunc)(
        NETIO_Interface_T *self,
        struct NETIO_Address *source,
        NETIO_Packet_T *packet,
        NETIO_PacketId_T *packet_id);

typedef RTI_BOOL
(*NETIO_Interface_discardFunc)(
        NETIO_Interface_T *self,
        struct NETIO_Address *destination,
        NETIO_PacketId_T *packet_id);

typedef RTI_BOOL
(*NETIO_Interface_add_routeFunc)(
        NETIO_Interface_T *self,
        struct NETIO_Address *dst_addr,
        NETIO_Interface_T *via_intf,
        struct NETIO_Address *via_addr,
        struct NETIORouteProperty *property,
        RTI_BOOL *existed);

typedef RTI_BOOL
(*NETIO_Interface_delete_routeFunc)(
        NETIO_Interface_T *self,
        struct NETIO_Address *dst_addr,
        NETIO_Interface_T *via_intf,
        struct NETIO_Address *via_addr,
        RTI_BOOL *existed);

typedef RTI_BOOL
(*NETIO_Interface_bindFunc)(NETIO_Interface_T *netio_intf,
                           struct NETIO_Address *src_addr,
                           struct NETIOBindProperty *property,
                           RTI_BOOL *existed);

typedef RTI_BOOL
(*NETIO_Interface_unbindFunc)(NETIO_Interface_T *netio_intf,
                             struct NETIO_Address *src_addr,
                             NETIO_Interface_T *dst_intf,
                             RTI_BOOL *existed);

typedef RTI_BOOL
(*NETIO_Interface_bind_externalFunc)(NETIO_Interface_T *src_intf,
                                    struct NETIO_Address *src_addr,
                                    NETIO_Interface_T *dst_intf,
                                    struct NETIO_Address *dst_addr,
                                    struct NETIOBindProperty *property,
                                    RTI_BOOL *existed);

typedef RTI_BOOL
(*NETIO_Interface_unbind_externalFunc)(NETIO_Interface_T *src_intf,
                                      struct NETIO_Address *src_addr,
                                      NETIO_Interface_T *dst_intf,
                                      struct NETIO_Address *dst_addr,
                                      RTI_BOOL *existed);

typedef RTI_BOOL
(*NETIO_Interface_get_external_interfaceFunc)(NETIO_Interface_T *netio_intf,
                                             struct NETIO_Address *src_addr,
                                             NETIO_Interface_T **dst_intf,
                                             struct NETIO_Address *dst_addr);

typedef RTI_BOOL
(*NETIO_Interface_reserve_addressFunc)(NETIO_Interface_T *self,
                                          struct NETIO_AddressSeq *req_address,
                                          struct NETIO_AddressSeq *rsvd_address,
                                          struct NETIOBindProperty *property);

typedef RTI_BOOL
(*NETIO_Interface_release_addressFunc)(NETIO_Interface_T *self,
                                    struct NETIO_Address *src_addr);

typedef RTI_BOOL
(*NETIO_Interface_set_stateFunc)(NETIO_Interface_T *netio_intf,
                                NETIO_InterfaceState_T state);

typedef RTI_BOOL
(*NETIO_Interface_resolve_adddressFunc)(NETIO_Interface_T *netio_intf,
                                        char *address_string,
                                        struct NETIO_Address *address_value);

typedef RTI_BOOL
(*NETIO_Interface_get_route_tableFunc)(NETIO_Interface_T *netio_intf,
                                        struct NETIO_AddressSeq *address,
                                        struct NETIO_NetmaskSeq *netmask);

/*e \ingroup NETIO_InterfaceClass
 *
 *  Abstract NETIO interface class
 */
struct NETIO_InterfaceI
{
    struct RT_ComponentI _parent;
    NETIO_Interface_sendFunc send;
    NETIO_Interface_ackFunc ack;
    NETIO_Interface_requestFunc request;
    NETIO_Interface_return_loanFunc return_loan;
    NETIO_Interface_discardFunc discard;
    NETIO_Interface_add_routeFunc add_route;
    NETIO_Interface_delete_routeFunc delete_route;
    NETIO_Interface_reserve_addressFunc reserve_address;
    NETIO_Interface_bindFunc bind;
    NETIO_Interface_unbindFunc unbind;
    NETIO_Interface_receiveFunc receive;
    NETIO_Interface_get_external_interfaceFunc get_external_interface;
    NETIO_Interface_bind_externalFunc bind_external;
    NETIO_Interface_unbind_externalFunc unbind_external;
    NETIO_Interface_set_stateFunc set_state;
    NETIO_Interface_release_addressFunc release_address;
    NETIO_Interface_resolve_adddressFunc resolve_address;
    NETIO_Interface_get_route_tableFunc get_route_table;
};

#define NETIO_InterfaceFactory_create_component(f_,p_,l_) \
        (NETIO_Interface_T*)((f_)->intf->create_component(\
                (f_),p_,l_))

#define NETIO_InterfaceFactory_delete_component(f_,intf_) \
        (f_)->intf->delete_component(f_,(&(intf_)->_parent))

#define NETIO_Interface_send(self_,src_intf_,dst_addr_,pkt_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->send(self_,src_intf_,dst_addr_,pkt_)

#define NETIO_Interface_ack(self_,src_intf_,sn_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->ack(self_,src_intf_,sn_)

#define NETIO_Interface_request(self_,src_,dst_,pkt_,pktid_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->request(self_,src_,dst_,pkt_,pktid_)

#define NETIO_Interface_return_loan(self_,src_,pkt_,pkt_id_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->return_loan(self_,src_,pkt_,pkt_id_)

#define NETIO_Interface_discard(self_,dst_,pkt_id) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->discard(self_,dst_,pkt_id)

#define NETIO_Interface_add_route(self_,dst_,intf_,intf_addr,prop_,ex_) \
    ((struct NETIO_InterfaceI*)(\
     (self_)->_parent._intf))->add_route(self_,dst_,intf_,intf_addr,prop_,ex_)

#define NETIO_Interface_delete_route(self_,dst_,intf_,intf_addr,exist_) \
    ((struct NETIO_InterfaceI*)(\
     (self_)->_parent._intf))->delete_route(self_,dst_,intf_,intf_addr,exist_)

#define NETIO_Interface_reserve_address(self_,src_addr_,pub_addr_,prop_)\
((struct NETIO_InterfaceI*)(\
(self_)->_parent._intf))->reserve_address(self_,src_addr_,pub_addr_,prop_)

#define NETIO_Interface_release_address(self_,src_addr_)\
((struct NETIO_InterfaceI*)(\
(self_)->_parent._intf))->release_address(self_,src_addr_)

#define NETIO_Interface_bind(self_,src_,prop_,ex_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->bind(self_,src_,prop_,ex_)

#define NETIO_Interface_unbind(self_,src_,dst_,exist_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->unbind(self_,src_,dst_,exist_)

#define NETIO_Interface_bind_external(src_intf_,src_adr_,dst_intf_,dst_adr_,p_,e_) \
((struct NETIO_InterfaceI*)(\
 (src_intf_)->_parent._intf))->bind_external(src_intf_,src_adr_,dst_intf_,dst_adr_,p_,e_)

#define NETIO_Interface_unbind_external(src_intf_,src_adr_,dst_adr_,dst_intf_,exist_) \
((struct NETIO_InterfaceI*)(\
 (src_intf_)->_parent._intf))->unbind_external(src_intf_,src_adr_,dst_adr_,dst_intf_,exist_)

#define NETIO_Interface_receive(self_,src_,dst_,p_) \
    ((struct NETIO_InterfaceI*)(\
            (self_)->_parent._intf))->receive(self_,src_,dst_,p_)

#define NETIO_Interface_get_external_interface(self_,src_,ul_,dst_) \
    ((struct NETIO_InterfaceI*)(\
       (self_)->_parent._intf))->get_external_interface(self_,src_,ul_,dst_)

#define NETIO_Interface_set_state(self_,state_) \
    ((struct NETIO_InterfaceI*)(\
       (self_)->_parent._intf))->set_state(self_,state_)

#define NETIO_Interface_resolve_address(self_,address_,resolved_) \
    ((struct NETIO_InterfaceI*)(\
       (self_)->_parent._intf))->resolve_address(self_,address_,resolved_)

#define NETIO_Interface_get_route_table(self_,address_,netmask_) \
    ((struct NETIO_InterfaceI*)(\
       (self_)->_parent._intf))->get_route_table(self_,address_,netmask_)

#ifdef __cplusplus
}
#endif


#endif /* netio_interface_h */
