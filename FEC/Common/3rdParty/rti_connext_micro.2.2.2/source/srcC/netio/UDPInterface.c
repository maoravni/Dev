/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "osapi/osapi_config.h"

#ifdef HAVE_SOCKET_API
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#if !defined(RTI_WIN32)
//#include <unistd.h>
#endif
//#include <fcntl.h>
#ifdef RTI_VXWORKS
#ifndef MSG_MAXIOVLEN           /* No longer defined in VxWorks 6.x */
#include <net/uio.h>
/* Never define MSG_MAXIOVLEN to be greater than 16, since we use it
   to size data allocations made on the stack! */
#if defined(UIO_MAXIOV) && (UIO_MAXIOV <= 16)
#define MSG_MAXIOVLEN (UIO_MAXIOV)
#else /* no UIO_MAXIOV, or defined to be >16 */
#define MSG_MAXIOVLEN (16)
#endif /* no UIO_MAXIOV, or defined to be >16 */
#endif /* no MSG_MAXIOVLEN */
#elif defined(RTI_SOL2) /* vxworks */
#include <sys/sockio.h>
#elif defined(RTI_LINUX)
#include <linux/sockios.h>
#include <sys/ioctl.h>
#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN   128     /* Should never be larger than 128 */
#endif /* MSG_MAXIOVLEN */
#elif defined(RTI_DARWIN)
#include <sys/ioctl.h>
#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN   IOV_MAX /* Defined in /usr/include/linux/uio.h */
#endif /* MSG_MAXIOVLEN */
#elif defined(RTI_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <netioapi.h>
#endif /* RTI_VXWORKS */

#if defined(RTI_VXWORKS)
#include <ioLib.h>
#include <ioctl.h>
#include <hostLib.h>
#include <netinet/tcp.h>
#elif defined(RTI_UNIX)
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netdb.h>
#if defined(RTI_LINUX)
#include <sys/ioctl.h>
#endif
#endif

#include "util/util_makeheader.h"

#if defined(LWIP_SYS)
#define RTI_NETIO_GATHER_SEND
#include "osapi/osapi_semaphore.h"
#endif

#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef osapi_alignment_h
#include "osapi/osapi_alignment.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_thread_h
#include "osapi/osapi_thread.h"
#endif
#ifndef reda_string_h
#include "reda/reda_string.h"
#endif
#ifndef db_api_h
#include "db/db_api.h"
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
#ifndef netio_route_h
#include "netio/netio_route.h"
#endif
#ifndef netio_udp_h
#include "netio/netio_udp.h"
#endif

NETIODllVariable
struct UDP_InterfaceFactoryProperty UDP_INTERFACE_FACTORY_PROPERTY_DEFAULT =
                                        UDP_InterfaceFactoryProperty_INITIALIZER;

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

RTI_PRIVATE struct NETIO_InterfaceI UDP_Interface_g_intf;

RTI_PRIVATE RTI_BOOL
UDP_Interface_receive(NETIO_Interface_T *netio_intf,
                     struct NETIO_Address *source,
                     NETIO_Packet_T *packet);

//#ifdef RTI_WIN32
#define IFNAMSIZ 1024
//#endif
struct UDP_NetworkIfInfo
{
    RTI_BOOL mc_enabled;
    RTI_UINT32 intf_address;
    RTI_UINT32 intf_netmask;
    char lastname[IFNAMSIZ];
    char fname[IFNAMSIZ];
};

#define UDP_NetworkIfInfo_INITIALIZER \
{\
    RTI_FALSE,\
    0,\
    0,\
    {0},\
    {0}\
}

#ifdef LWIP_SYS
#define NETIO_HOST_INTERFACE_BUFFER_INIT 1024
struct UDP_IfContext
{
    RTI_BOOL initialized;
    RTI_INT32 interfaceBufferSize;
};
#define UDP_IFCONTEXT_INITIALIZER \
{\
    RTI_FALSE,\
    NETIO_HOST_INTERFACE_BUFFER_INIT\
}
#endif

#if defined(RTI_UNIX) || defined(RTI_VXWORKS) /* || defined(RTI_WIN32) */
#define NETIO_HOST_INTERFACE_BUFFER_INIT 1024
struct UDP_IfContext
{
    RTI_BOOL initialized;
    RTI_INT32 interfaceBufferSize;
    char interfaceBuffer[NETIO_HOST_INTERFACE_BUFFER_INIT];
    char *bufferPointer;
    struct ifconf ifc;
    int s;
};

#define UDP_IFCONTEXT_INITIALIZER \
{\
    RTI_FALSE,\
    NETIO_HOST_INTERFACE_BUFFER_INIT\
}
#elif defined (RTI_WIN32)
#define NETIO_HOST_INTERFACE_BUFFER_INIT 1024
#define MAX_FRIENDLY_NAME 255
typedef DWORD(WINAPI * GETADAPTERSADDRESSES_FUNC)
    (ULONG Family,
     DWORD Flags,
     PVOID Reserved,
     PIP_ADAPTER_ADDRESSES pAdapterAddresses, PULONG pOutBufLen);

struct UDP_IfContext
{
    RTI_BOOL initialized;
    RTI_INT32 interfaceBufferSize;
    HINSTANCE hinstIphlpapiLib;
    GETADAPTERSADDRESSES_FUNC myGetAdaptersAddresses;
    IP_ADAPTER_ADDRESSES *interfaceBuffer;
    IP_ADAPTER_ADDRESSES *ifbufIter;
    char addressString[INET6_ADDRSTRLEN];
};

#define UDP_IFCONTEXT_INITIALIZER \
{\
    RTI_FALSE,\
    NETIO_HOST_INTERFACE_BUFFER_INIT\
}
#endif

#ifdef RTI_VXWORKS
#define NETIO_ioctl(s, option, pointer) ioctl(s, option, (int)(pointer))
#else
#define NETIO_ioctl ioctl
#endif /* RTI_VXWORKS */

#endif

/*** SOURCE_BEGIN ***/

#define T struct UDP_NatEntry
#define TSeq UDP_NatEntrySeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

RTI_PRIVATE RTI_BOOL
UDP_Interface_is_multicast(const struct NETIO_Address *const address)
{
    return ((address->value.ipv4.address >= 0xe0000000) &&
            (address->value.ipv4.address <= 0xefffffff));
}

#ifdef HAVE_SOCKET_API

RTI_PRIVATE RTI_INT32
UDP_Interface_compare_nat_entry(DB_Select_T opcode,RTI_INT32 flags,
                                const DB_Record_T op1, void *op2, void *op3)
{
    struct UDP_NatEntry *left = (struct UDP_NatEntry*)op1;
    const struct UDP_NatEntry *right = (struct UDP_NatEntry*)op2;
    RTI_INT32 cval;

    cval = NETIO_Address_compare(&left->local_address,&right->local_address);
    if (cval)
    {
        return cval;
    }

    return NETIO_Address_compare(&left->public_address,&right->public_address);
}

RTI_PRIVATE RTI_INT32
UDP_Interface_compare_nat_local(DB_Select_T opcode,RTI_INT32 flags,
                                const DB_Record_T op1, void *op2, void *op3)
{
    struct UDP_NatEntry *left = (struct UDP_NatEntry*)op1;
    const struct NETIO_Address *right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        right = (const struct NETIO_Address *)op2;
    }
    else
    {
        right = &((struct UDP_NatEntry*)op2)->local_address;
    }

    return NETIO_Address_compare(&left->local_address,right);
}

RTI_PRIVATE RTI_INT32
UDP_Interface_compare_nat_public(DB_Select_T opcode,RTI_INT32 flags,
                                const DB_Record_T op1, void *op2, void *op3)
{
    struct UDP_NatEntry *left = (struct UDP_NatEntry*)op1;
    const struct NETIO_Address *right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        right = (const struct NETIO_Address *)op2;
    }
    else
    {
        right = &((struct UDP_NatEntry*)op2)->public_address;
    }

    return NETIO_Address_compare(&left->public_address,right);
}


RTI_PRIVATE RTI_INT32
UDP_Interface_compare_port(DB_Select_T opcode,RTI_INT32 flags,
                            const DB_Record_T op1, void *op2, void *op3)
{
    struct UDPPortEntry *record_left = (struct UDPPortEntry*)op1;
    const struct NETIO_Address *id_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        id_right = (const struct NETIO_Address *)op2;
    }
    else
    {
        id_right = &((struct UDPPortEntry*)op2)->source;
    }

    if (record_left->source.port > id_right->port)
    {
        return 1;
    }

    if (record_left->source.port < id_right->port)
    {
        return -1;
    }

    if (!UDP_Interface_is_multicast(&record_left->source))
    {
        return 0;
    }

    if (record_left->source.value.ipv4.address > id_right->value.ipv4.address)
    {
        return 1;
    }

    if (record_left->source.value.ipv4.address < id_right->value.ipv4.address)
    {
        return -1;
    }

    return 0;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_enable_multicast_loopback(struct UDP_Interface *udp_intf)
{
    RTI_UINT8 option;
    RTI_BOOL retval = RTI_FALSE;
    struct in_addr multicast_if;

#if !(OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS)
    /* set the multicast loopback option,0 = disable, 1 = enable */
    option = 1;

    if (netiosock_setsockopt(udp_intf->send_socket,
            IPPROTO_IP, IP_MULTICAST_LOOP, (void*)&option, sizeof(option)) < 0)
    {
        NETIO_Log_error_p2(LOG_FAIL_SOCKET_OPTIONING_IP_MULTICAST_LOOP,
                option, errno);
        goto done;
    }
#endif

    /* set the multicast send intf option:
     * Bind outgoing multicast address to a single NIC. In case multiple
     * NICs are allowed for the UDP interface, the first NIC that supports
     * multicast is used.
     */
    multicast_if.s_addr = NETIO_htonl(udp_intf->mc_bind_address.value.ipv4.address);
    if (netiosock_setsockopt(udp_intf->send_socket, IPPROTO_IP, IP_MULTICAST_IF,
            (char *)&multicast_if, sizeof(multicast_if)) < 0)
    {
        NETIO_Log_error_p2(LOG_FAIL_SOCKET_OPTIONING_IP_MULTICAST_IF,
                multicast_if.s_addr, errno);
        goto done;
    }

    retval = RTI_TRUE;

done:
    return retval;
}

RTI_PRIVATE int
UDP_Interface_create_socket(struct UDP_Interface *udp_intf)
{
    int s;
    RTI_UINT8 option;
    struct NETIO_Address address;

    NETIO_Address_init(&address,0);

    s = netiosock_socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1)
    {
        NETIO_Log_error_p1(LOG_FAIL_CREATE_SOCKET, errno);
        return -1;
    }

#if defined(RTI_UNIX)
    if (fcntl(s, F_SETFD, FD_CLOEXEC) == -1)
    {
        NETIO_Log_error_p1(LOG_FAIL_FCNTL_FD_CLOEXEC, errno);
        goto fail;
    }
#endif

#if !(OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS)
    if (netiosock_setsockopt(s, SOL_SOCKET, SO_SNDBUF,
                 (char *)&udp_intf->factory->property->max_send_buffer,
                 sizeof(udp_intf->factory->property->max_send_buffer)) < 0)
    {
        NETIO_Log_error_p2(LOG_FAIL_SOCKET_OPTIONING_SO_SNDBUF,
                udp_intf->factory->property->max_send_buffer, errno);
        goto fail;
    }
#endif

    /* set the multicast ttl option, 0-255 */
    option = (char)udp_intf->factory->property->multicast_ttl;
    if (netiosock_setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&option,
            sizeof(option)) < 0)
    {
        NETIO_Log_error_p2(LOG_FAIL_SOCKET_OPTIONING_IP_MULTICAST_TTL,
                option, errno);
        goto fail;
    }

    return s;

fail:

    netiosock_close(s);

    return -1;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_receive_thread(struct OSAPI_ThreadInfo *thread_info)
{
    struct sockaddr_in ip_src;
    struct UDPPortEntry *port_entry =
            (struct UDPPortEntry *)thread_info->userParam;
    struct NETIO_Packet packet;
#ifdef RTI_VXWORKS
    int ip_len = sizeof(struct sockaddr_in);
#else
    socklen_t ip_len = sizeof(struct sockaddr_in);
#endif
#if defined(RTI_WIN32) || defined(RTI_WINCE)
    int flags = 0;              /* don't care about flag, but can't provide NULL */
#endif
#if defined(RTI_WIN32)
    WSABUF mbuf;
#endif
    RTI_INT32 rx_len;

    LOG_printDebug("thread started for port: %d\n",port_entry->source.port);
    while (!thread_info->stopThread)
    {
#if defined(RTI_WIN32)
        mbuf.len = port_entry->_rx_buffer.max_length;
        mbuf.buf = port_entry->_rx_buffer.buffer;
        if (WSARecvFrom((SOCKET)port_entry->_sock,
                (WSABUF*)&mbuf, 1, &rx_len, &flags,
                (struct sockaddr *)&ip_src, &ip_len, NULL, NULL) != 0)
        {
            port_entry->_rx_buffer.length = -1;
        }
#else
        LOG_printDebug("wait for data: %d\n",port_entry->source.port);
        rx_len = netiosock_recvfrom(port_entry->_sock,
                port_entry->_rx_buffer.buffer,
                port_entry->_rx_buffer.max_length,
                0,
                (struct sockaddr *)&ip_src,
                &ip_len);
#endif
        LOG_printDebug("received data on %d,%d/%x %d\n",OSAPI_Thread_self(),port_entry->source.port,
                port_entry->source.value.ipv4.address,rx_len);

        if (rx_len < 0)
        {
            LOG_printDebug("continue receiving ....\n");
            continue;
        }

        if (rx_len <= NETIO_PING_MSG_SIZE)
        {
            LOG_printDebug("ping\n");
            continue;
        }

        LOG_printDebug("process message\n");

        port_entry->_rx_buffer.length = (RTI_SIZE_T)rx_len;

        NETIO_Packet_initialize(&packet,port_entry->_rx_buffer.buffer,
                port_entry->_rx_buffer.length,0,NULL);
        NETIO_Packet_set_head(&packet,0-port_entry->_rx_buffer.length);

        if (!UDP_Interface_receive(&port_entry->_udp_intf->_parent,
                                  &port_entry->source,
                                  &packet))
        {
            /* TODO: log excpeption */
        }
        LOG_printDebug("processed message\n");
    }

    LOG_printDebug("thread exited for port: %d\n",port_entry->source.port);

    return RTI_TRUE;
}

RTI_BOOL
UDP_Interface_interface_allowed(struct UDP_Interface *src_intf,
                               char *lastname)
{
    RTI_INT32 j;
    char *a_string;

    /* Check if interface appears in the allowed  list */
    if (REDA_StringSeq_get_length(
                        &src_intf->factory->property->allow_interface) > 0)
    {
        for (j = 0; j < REDA_StringSeq_get_length(
                    &src_intf->factory->property->allow_interface); ++j)
        {
            a_string = *REDA_StringSeq_get_reference(
                    &src_intf->factory->property->allow_interface,j);
            if (!OSAPI_String_cmp(a_string,lastname))
            {
                return RTI_TRUE;
            }
        }

        if (j == REDA_StringSeq_get_length(
                            &src_intf->factory->property->allow_interface))
        {
            return RTI_FALSE;
        }
    }

    /* Allowed, check if it is the deny list */
    if (REDA_StringSeq_get_length(&src_intf->factory->property->deny_interface) > 0)
    {
        for (j = 0; j < REDA_StringSeq_get_length(&src_intf->factory->property->deny_interface); ++j)
        {
            a_string = *REDA_StringSeq_get_reference(
                        &src_intf->factory->property->deny_interface,j);
            if (!OSAPI_String_cmp(a_string,lastname))
            {
                return RTI_FALSE;
            }
        }
        if (j == REDA_StringSeq_get_length(&src_intf->factory->property->deny_interface))
        {
            return RTI_TRUE;
        }
    }

    return RTI_TRUE;
}

void
UDP_Interface_finalize(struct UDP_Interface *netio_intf)
{
    struct UDP_NatEntry nat_key;
    struct UDP_NatEntry *nat_record;
    struct UDP_NatEntrySeq *nat_seq;
    DB_ReturnCode_T dbrc;
    RTI_INT32 i,len;

    len = UDP_NatEntrySeq_get_length(&netio_intf->factory->property->nat);
    if (len > 0)
    {
        nat_seq = &netio_intf->factory->property->nat;
        for (i = 0; i < len; i++)
        {
            nat_key = *UDP_NatEntrySeq_get_reference(nat_seq,i);
            dbrc = DB_Table_remove_record(netio_intf->nat,
                                          (DB_Record_T*)&nat_record,
                                          (DB_Key_T*)&nat_key);
            if (dbrc != DB_RETCODE_OK)
            {
            }

            dbrc = DB_Table_delete_record(netio_intf->nat,nat_record);
            if (dbrc != DB_RETCODE_OK)
            {
            }
        }

        if (DB_Table_delete_index(netio_intf->nat,
                                  netio_intf->nat_local_idx) != DB_RETCODE_OK)
        {
        }

        if (DB_Table_delete_index(netio_intf->nat,
                                  netio_intf->nat_public_idx) != DB_RETCODE_OK)
        {
        }
    }

    if (netio_intf->send_socket != -1)
    {
        netiosock_close(netio_intf->send_socket);
        netio_intf->send_socket = -1;
    }

    NETIO_Interface_finalize(&netio_intf->_parent);
}

void
UDP_Interface_delete(struct UDP_Interface *netio_intf)
{

    if (netio_intf)
    {
        UDP_Interface_finalize(netio_intf);
        OSAPI_Heap_freeStructure(netio_intf);
    }
}

RTI_PRIVATE  RTI_BOOL
UDP_Interface_wakeup_receive_thread(struct OSAPI_ThreadInfo *thread_info)
{
    struct UDPPortEntry *port_entry = (struct UDPPortEntry *)thread_info->userParam;
    struct sockaddr_in sock_addr;
    RTI_SIZE_T bytes_sent;
    RTI_INT32 i;

    LOG_printDebug("UDP_Interface_wakeup_receive_thread: %d\n",port_entry->_sock);

    for (i = 0; i < 20; ++i)
    {
        OSAPI_Memory_zero(&sock_addr, sizeof(struct sockaddr_in));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = NETIO_htonl(port_entry->source.value.ipv4.address);
        sock_addr.sin_port = NETIO_htons((RTI_UINT16)port_entry->source.port);

        bytes_sent = netiosock_sendto(port_entry->_udp_intf->send_socket,
                NETIO_PING_MSG, NETIO_PING_MSG_SIZE, 0,
                (struct sockaddr *)&sock_addr, sizeof(sock_addr));
        LOG_printDebug("sent %d bytes to %x/%x\n",bytes_sent,(RTI_UINT16)port_entry->source.port,port_entry->source.value.ipv4.address);
    }

    return RTI_TRUE;
}

RTI_PRIVATE struct UDPPortEntry*
UDP_Interface_create_bind_entry(struct UDP_Interface *src_intf,
                                struct NETIO_Address *src_addr,
                                struct NETIOBindProperty *property,
                                RTI_BOOL *existed)
{
    struct UDP_Interface *self = (struct UDP_Interface *)src_intf;
    struct UDPPortEntry *port_entry = NULL;
    DB_ReturnCode_T dbrc;
    struct OSAPI_ThreadProperties properties = OSAPI_THREAD_PROPERTIES_DEFAULT;
#if defined(RTI_WIN32) || defined(RTI_WINCE)
    int flags = 0;       /* don't care about flag, but can't provide NULL */
#endif
    struct sockaddr_in sock_addr;
    int _sock;
    const int YES = 1;
    struct ip_mreq imr;
    int rc;

    if (existed)
    {
        *existed = RTI_FALSE;
    }

    dbrc = DB_Table_select_match(self->rx_thread_table,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&port_entry,
                                (DB_Key_T)src_addr);
    if (dbrc == DB_RETCODE_OK)
    {
        if (existed)
        {
            *existed = RTI_TRUE;
        }
        return port_entry;
    }

    _sock = netiosock_socket(AF_INET, SOCK_DGRAM, 0);
    if (_sock == -1)
    {
        NETIO_Log_error_p1(LOG_FAIL_CREATE_SOCKET, errno);
        return NULL;
    }

#if defined(RTI_UNIX) || defined(RTI_LYNX)
    if (fcntl(_sock, F_SETFD, FD_CLOEXEC) == -1)
    {
        NETIO_Log_error_p1(LOG_FAIL_FCNTL_FD_CLOEXEC, errno);
        return NULL;
    }
#endif

    /* Check that the specified socket can be reserved */
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = NETIO_htons((RTI_UINT16)src_addr->port);

    /*(address = (RTI_UINT32) * intf; */
    if (UDP_Interface_is_multicast(src_addr))
    {
#if defined(RTI_IRIX) || defined(RTI_LYNX) || \
    (defined(RTI_VXWORKS) && VXWORKS_VERSION_5_4_OR_BETTER) \
    || defined(RTI_QNX) || (defined(RTI_INTY) && defined(RTI_IPEAK)) || defined(RTI_DARWIN)
        if (netiosock_setsockopt(_sock, SOL_SOCKET, SO_REUSEPORT, (const char *)&YES, sizeof(int)) != 0)
#else
        if (netiosock_setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&YES, sizeof(int)) != 0)
#endif
        {
            NETIO_Log_error(LOG_FAIL_REUSE_MULTICAST_PORT);
            return NULL;
        }
#ifdef RTI_WIN32
        sock_addr.sin_addr.s_addr = NETIO_htonl(INADDR_ANY);
#else
        sock_addr.sin_addr.s_addr = NETIO_htonl(src_addr->value.ipv4.address);
#endif
    }
    else
    {
        sock_addr.sin_addr.s_addr = NETIO_htonl(INADDR_ANY);
    }

    if (netiosock_bind(_sock,(struct sockaddr *)&sock_addr,
                        sizeof(struct sockaddr_in)) < 0)
    {
        NETIO_LOG_WARNING_P3(LOG_FAIL_SOCKET_BINDING,
                             src_addr->value.ipv4.address,
                             src_addr->port, errno)
        return NULL;
    }

    if (UDP_Interface_is_multicast(src_addr))
    {
        imr.imr_interface.s_addr = NETIO_htonl(self->mc_bind_address.value.ipv4.address);
        imr.imr_multiaddr.s_addr = NETIO_htonl(src_addr->value.ipv4.address);

        rc = netiosock_setsockopt(_sock,IPPROTO_IP, IP_ADD_MEMBERSHIP,
                (void *)&imr, sizeof(imr));
        if (rc < 0)
        {
            NETIO_Log_error_p1(LOG_FAIL_SOCKET_OPTIONING_IP_MULTICAST_IF,rc);
            (void)DB_Table_delete_record(self->_parent._btable,
                    (DB_Record_T *)port_entry);
            return NULL;
        }
    }

    /* Create bind entry */
    dbrc = DB_Table_create_record(self->rx_thread_table,
                              (DB_Record_T *)&port_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        return NULL;
    }

    OSAPI_Memory_zero(port_entry,sizeof(struct UDPPortEntry));
    port_entry->source = *src_addr;
    port_entry->_udp_intf = self;
    port_entry->_ref_count = 0;
    port_entry->sock_addr = sock_addr;
    port_entry->_sock = _sock;
    port_entry->_rx_thread = NULL;
    port_entry->_rx_buffer.max_length =
            port_entry->_udp_intf->factory->property->max_receive_buffer;
    port_entry->_rx_buffer.length = 0;

    OSAPI_Heap_allocateBuffer(&port_entry->_rx_buffer.buffer,
            port_entry->_rx_buffer.max_length,OSAPI_ALIGNMENT_DEFAULT);

    if (port_entry->_rx_buffer.buffer == NULL)
    {
        return NULL;
    }

    if (netiosock_setsockopt(port_entry->_sock,
            SOL_SOCKET, SO_RCVBUF,(char *)&port_entry->_rx_buffer.max_length,
            sizeof(port_entry->_rx_buffer.max_length)) < 0)
    {
        NETIO_Log_error_p2(LOG_FAIL_SOCKET_OPTIONING_SO_RCVBUF,
                port_entry->_rx_buffer.max_length, errno);
        return NULL;
    }

    dbrc = DB_Table_insert_record(self->rx_thread_table,
                                 (DB_Record_T *)port_entry);
    if ((dbrc != DB_RETCODE_OK) && (dbrc != DB_RETCODE_EXISTS))
    {
        (void)DB_Table_delete_record(self->rx_thread_table,
                                    (DB_Record_T *)port_entry);
        return NULL;
    }

    LOG_printDebug("created port entry: %d/%d/%x\n",
                src_addr->kind,src_addr->port,src_addr->value.ipv4.address);

    port_entry->_rx_thread = OSAPI_Thread_create(
                    "udp_rx",
                    &properties,
                    UDP_Interface_receive_thread,
                    (void*)port_entry,
                    UDP_Interface_wakeup_receive_thread);

    if (port_entry->_rx_thread == NULL)
    {
        netiosock_close(_sock);
        (void)DB_Table_delete_record(self->rx_thread_table,
                                    (DB_Record_T *)port_entry);
        return NULL;
    }

    return port_entry;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_bind_addresses(struct UDP_Interface *src_intf,
                             RTI_INT32 start_index,
                             struct NETIO_AddressSeq *addresses,
                             struct NETIOBindProperty *property)
{
    RTI_BOOL retval = RTI_FALSE;
    RTI_INT32 i,length;
    struct NETIO_Address *an_address;
    struct UDPPortEntry *port_entry;
    RTI_BOOL port_existed;
    struct UDP_NatEntry *nat_entry;
    DB_ReturnCode_T dbrc;
    struct NETIO_Address src_address;


    length = NETIO_AddressSeq_get_length(addresses);
    for (i = start_index; i < length; ++i)
    {
        an_address = NETIO_AddressSeq_get_reference(addresses,i);

        src_address = *an_address;
        if (src_intf->nat)
        {
            dbrc = DB_Table_select_match(src_intf->nat,
                    src_intf->nat_public_idx,
                    (DB_Record_T*)&nat_entry,an_address);

            if (dbrc == DB_RETCODE_OK)
            {
                src_address = nat_entry->local_address;
            }
        }

        port_existed = RTI_FALSE;
        port_entry = UDP_Interface_create_bind_entry(
                                   src_intf,&src_address,property,&port_existed);
        if (port_entry == NULL)
        {
            break;
        }

        if (port_existed && UDP_Interface_is_multicast(&src_address))
        {
            if (i < (length -1))
            {
                *NETIO_AddressSeq_get_reference(addresses,i) =
                                *NETIO_AddressSeq_get_reference(addresses,i+1);
            }
            /* Need redo the bind for the element index */
            --i;
            --length;
            NETIO_AddressSeq_set_length(addresses,length);
        }
    }

    if (i == length)
    {
        retval = RTI_TRUE;
    }

    return retval;
}


#if defined(RTI_UNIX) || defined(RTI_VXWORKS) /* || defined(RTI_WIN32) */
RTI_PRIVATE RTI_BOOL
UDP_Interface_get_next_interface(struct UDP_Interface *src_intf,
                                 struct UDP_IfContext *context,
                                 struct UDP_NetworkIfInfo *ifdata,
                                 RTI_BOOL *next_if_found)
{
    struct ifreq ifrcopy, *ifr;
    int ifrSize = sizeof(struct ifreq);
    char *cptr = NULL;

    if (!context->initialized)
    {
        OSAPI_Memory_zero(context->interfaceBuffer,NETIO_HOST_INTERFACE_BUFFER_INIT);

        context->s = netiosock_socket(AF_INET, SOCK_DGRAM, 0);
        if (context->s == -1)
        {
            NETIO_Log_error_p1(LOG_FAIL_CREATE_SOCKET, errno);
            return RTI_FALSE;
        }

        context->ifc.ifc_buf = context->interfaceBuffer;
        context->ifc.ifc_len = context->interfaceBufferSize;
        if (NETIO_ioctl(context->s, SIOCGIFCONF, &context->ifc) == -1)
        {
            NETIO_Log_error_p1(LOG_FAIL_IOCTL_SIOCGIFCONF, errno);
            return RTI_FALSE;
        }
        context->bufferPointer = context->interfaceBuffer;
        ifdata->lastname[0] = 0;
        ifdata->fname[0] = 0;
        context->initialized = RTI_TRUE;
    }

    *next_if_found = RTI_FALSE;
    while (!*next_if_found &&
           (context->bufferPointer < (context->interfaceBuffer + context->ifc.ifc_len)))
    {
        OSAPI_Memory_zero(&ifrcopy, sizeof(struct ifreq));
        ifr = (struct ifreq *)context->bufferPointer;

#if defined(RTI_VXWORKS)
        ifrSize = ifr->ifr_addr.sa_len + sizeof(ifr->ifr_name);
        if (ifrSize < sizeof(struct ifreq))
        {
            ifrSize = sizeof(struct ifreq);
        }
#elif defined(RTI_DARWIN) && defined(_SIZEOF_ADDR_IFREQ)
        ifrSize = _SIZEOF_ADDR_IFREQ(*ifr);
#else
        ifrSize = sizeof(struct ifreq);
#endif /* RTI_VXWORKS */

        context->bufferPointer += ifrSize;       /* for the next interface */
        OSAPI_Memory_copy(&ifrcopy, ifr, sizeof(struct ifreq));

        if (ifrcopy.ifr_addr.sa_family != AF_INET)
        {
            /* only want IPv4 */
            continue;
        }

        /* filter out alias */
        cptr = strrchr(ifrcopy.ifr_name, ':');
        if (cptr != NULL)
        {
            /* replace possible colon alias designator with terminating NULL */
            *cptr = 0;
        }

        if (REDA_String_compare(ifdata->lastname,ifrcopy.ifr_name) == 0)
        {
            /* name already exists, alias! */
            continue;
        }

        OSAPI_Memory_copy(ifdata->lastname, ifrcopy.ifr_name, IFNAMSIZ);

        /* now get flags */
        if (NETIO_ioctl(context->s, SIOCGIFFLAGS, &ifrcopy) == -1)
        {
            NETIO_Log_error_p1(LOG_FAIL_IOCTL_SIOCGIFFLAGS, errno);
            netiosock_close(!context->s);
            return RTI_FALSE;
        }

        if (!(ifrcopy.ifr_flags & IFF_UP))
        {
            continue;
        }

        ifdata->mc_enabled = (ifrcopy.ifr_flags & IFF_MULTICAST ? RTI_TRUE : RTI_FALSE);
        ifdata->intf_address = NETIO_ntohl(((struct sockaddr_in*)&(ifrcopy.ifr_addr))->sin_addr.s_addr);

        if (NETIO_ioctl(context->s, SIOCGIFNETMASK, &ifrcopy) == -1)
        {
            NETIO_Log_error_p1(LOG_FAIL_IOCTL_SIOCGIFFLAGS, errno);
            return RTI_FALSE;
        }

        ifdata->intf_netmask = NETIO_ntohl(((struct sockaddr_in*)&(ifrcopy.ifr_addr))->sin_addr.s_addr);

        *next_if_found = RTI_TRUE;
    }

    if (*next_if_found)
    {
        return RTI_TRUE;
    }

    return RTI_FALSE;
}

RTI_PRIVATE void
UDP_Interface_get_next_interface_done(struct UDP_Interface *src_intf,
                                      struct UDP_IfContext *context)
{
    if (context->s != -1)
    {
        if (netiosock_close(context->s) == -1)
        {
            NETIO_Log_error_p1(LOG_FAIL_CLOSE_SOCKET, errno);
        }
    }
}

#elif defined (LWIP_SYS)
RTI_BOOL
UDP_Interface_get_next_interface(struct UDP_Interface *src_intf,
                                 struct UDP_IfContext *context,
                                 struct UDP_NetworkIfInfo *ifdata,
                                 RTI_BOOL *next_if_found)
{
    return RTI_FALSE;
}

RTI_PRIVATE void
UDP_Interface_get_next_interface_done(struct UDP_Interface *src_intf,
                                      struct UDP_IfContext *context)
{
}

#elif defined (RTI_WIN32)
RTI_PRIVATE RTI_BOOL
UDP_Interface_get_next_interface(struct UDP_Interface *src_intf,
                                 struct UDP_IfContext *context,
                                 struct UDP_NetworkIfInfo *ifdata,
                                 RTI_BOOL *next_if_found)
{
    const int interfaceInfoSize = sizeof(IP_ADAPTER_ADDRESSES);
    const int NUM_RETRIES = 5;
    ULONG retVal;
    RTI_INT32 i=0;
    char addressString[INET6_ADDRSTRLEN];

    if (!context->initialized)
    {
        context->myGetAdaptersAddresses = NULL;
        context->interfaceBuffer = NULL;
        context->ifbufIter = NULL;
        context->hinstIphlpapiLib = NULL;
        ifdata->lastname[0] = 0;
        ifdata->fname[0] = 0;

        /* run-time dynamic linking to avoid failure on some Windows OS versions */
        context->hinstIphlpapiLib = LoadLibrary("iphlpapi");
        if (context->hinstIphlpapiLib != NULL)
        {
            context->myGetAdaptersAddresses = (GETADAPTERSADDRESSES_FUNC)
             GetProcAddress(context->hinstIphlpapiLib, "GetAdaptersAddresses");
        }
        else
        {
            return RTI_FALSE;
        }

        /* if we can't get the function, return an empty list */
        if (context->myGetAdaptersAddresses == NULL)
        {
            return RTI_FALSE;
        }

        /* Get needed size of buffer */
        context->interfaceBufferSize = 0;
        if (context->myGetAdaptersAddresses(AF_INET, 0, NULL,
                      context->interfaceBuffer,
                      &context->interfaceBufferSize) != ERROR_BUFFER_OVERFLOW)
        {
            return RTI_FALSE;
        }

        retry_alloc:
        ++i;
        if (context->interfaceBuffer)
        {
            OSAPI_Heap_freeBuffer((char *)context->interfaceBuffer);
        }
        OSAPI_Heap_allocateBuffer((char **)&context->interfaceBuffer, context->interfaceBufferSize, 1);

        if (context->interfaceBuffer == NULL)
        {
            return RTI_FALSE;
        }

        retVal = context->myGetAdaptersAddresses(AF_INET, 0, NULL, context->interfaceBuffer,
                &context->interfaceBufferSize);
        if (retVal == ERROR_BUFFER_OVERFLOW && i < NUM_RETRIES)
        {
            goto retry_alloc;
        }
        else if (retVal != 0)
        {
            return RTI_FALSE;
        }
        context->ifbufIter = context->interfaceBuffer;
        context->initialized = RTI_TRUE;
    }

    *next_if_found = RTI_FALSE;
    while (!*next_if_found && (context->ifbufIter != NULL))
    {
        if (context->ifbufIter->IfType == IF_TYPE_TUNNEL)
        {
            /* ignore tunnels (there are many when using IPv6) */
            context->ifbufIter = context->ifbufIter->Next;
            continue;
        }

        if ((context->ifbufIter->OperStatus != IfOperStatusUp) &&
            (context->ifbufIter->OperStatus != IfOperStatusDormant))
        {
            context->ifbufIter = context->ifbufIter->Next;
            continue;
        }

        wcstombs(ifdata->fname,context->ifbufIter->FriendlyName,IFNAMSIZ);
        OSAPI_Memory_copy(ifdata->lastname, context->ifbufIter->AdapterName, 
			              (1 + OSAPI_String_length(context->ifbufIter->AdapterName)));

        if (context->ifbufIter->FirstUnicastAddress == NULL)
        {
            /* ignore interfaces with no address */
            context->ifbufIter = context->ifbufIter->Next;
            continue;
        }

        if (getnameinfo(context->ifbufIter->FirstUnicastAddress->Address.lpSockaddr,
                        context->ifbufIter->FirstUnicastAddress->Address.iSockaddrLength,
                        addressString, INET6_ADDRSTRLEN, NULL, 0,
                        NI_NUMERICHOST) != 0)
        {
            NETIO_Log_error_p1(LOG_GETNAME_FAILED, WSAGetLastError());
            return RTI_FALSE;
        }

        ifdata->mc_enabled = (context->ifbufIter->Flags & IP_ADAPTER_NO_MULTICAST ? RTI_FALSE : RTI_TRUE);

        /* Only one multicast interface is supported. It should be the same
         * as the interface configured by the OS to use for multicast
         * if none is specified the OS default is used.
         */
        ifdata->intf_address = NETIO_ntohl(inet_addr(addressString));
        ifdata->intf_netmask = 0xff000000;

        *next_if_found = RTI_TRUE;
        context->ifbufIter = context->ifbufIter->Next;
    }

    if (*next_if_found)
    {
        return RTI_TRUE;
    }

    return RTI_FALSE;
}

RTI_PRIVATE void
UDP_Interface_get_next_interface_done(struct UDP_Interface *src_intf,
                                      struct UDP_IfContext *context)
{
    if (context->hinstIphlpapiLib != NULL)
    {
        FreeLibrary(context->hinstIphlpapiLib);
    }

    if (context->interfaceBuffer != NULL)
    {
        OSAPI_Heap_freeBuffer((char *)context->interfaceBuffer);
    }
}

#else
#error "UDP_Interface_read_interface_list() not ported"
#endif

RTI_PRIVATE RTI_BOOL
UDP_Interface_read_interface_list(struct UDP_Interface *src_intf,
                                 struct NETIO_AddressSeq *req_addr,
                                 struct NETIO_AddressSeq *rcvd_addr,
                                 struct NETIOBindProperty *property)
{
    RTI_BOOL ok = RTI_FALSE;
    RTI_INT32 max_size;
    RTI_INT32 i,k,j;
    RTI_INT32 cur_addr_len;
    RTI_INT32 start_index;
    struct NETIO_Address src_address;
    struct NETIO_Address *an_address;
    struct UDP_IfContext if_context = UDP_IFCONTEXT_INITIALIZER;
    struct UDP_NetworkIfInfo if_data = UDP_NetworkIfInfo_INITIALIZER;
    RTI_BOOL if_found;
    RTI_INT32 lb_index = -1;
    struct UDP_NatEntry *nat_entry;
    DB_ReturnCode_T dbrc;

    cur_addr_len = NETIO_AddressSeq_get_length(rcvd_addr);
    start_index = cur_addr_len;
    max_size = NETIO_AddressSeq_get_maximum(rcvd_addr) - cur_addr_len;

    if (max_size == 0)
    {
        return RTI_TRUE;
    }

    NETIO_Address_init(&src_address,0);

    i = 0;

    while (UDP_Interface_get_next_interface(src_intf,&if_context,&if_data,&if_found) && (i < max_size))
    {
        if (!if_found)
        {
            break;
        }

        if (!UDP_Interface_interface_allowed(src_intf,if_data.lastname))
        {
#ifdef RTI_WIN32
            if (!UDP_Interface_interface_allowed(src_intf,if_data.fname))
            {
                continue;
            }
#else
            continue;
#endif
        }

        if ((src_intf->factory->property->multicast_interface != NULL) &&
            (src_intf->mc_bind_address.value.ipv4.address == 0) &&
            if_data.mc_enabled)
        {
            if (!REDA_String_compare(if_data.lastname,
                            src_intf->factory->property->multicast_interface))
            {
                src_intf->mc_bind_address.
                                    value.ipv4.address = if_data.intf_address;
            }
#ifdef RTI_WIN32
            else if (!REDA_String_compare(if_data.fname,
                            src_intf->factory->property->multicast_interface))
            {
                src_intf->mc_bind_address.
                                    value.ipv4.address = if_data.intf_address;
            }
#endif
        }

        /* NOTE:
         * At this point we have an interface which are allowed per the
         * allow/deny lists. The next step is to check the requested application
         * list.
         *
         * - If the request list is empty, there is nothing to be done
         * - For each entry in the request list, check:
         *   - The entry _must_ have a port assigned
         *   - If the address is unassigned, add the interface's unicast
         *     address
         *   - If the address is assigned and is unicast, only add it if
         *     it matches the interface address
         *  -  If the address is assigned and is multicast, only add if it
         *     the interface supports multi-cast, multi-cast is enabled
         */
        for (k = 0; k < NETIO_AddressSeq_get_length(req_addr); ++k)
        {
            an_address = NETIO_AddressSeq_get_reference(req_addr,k);

            if (an_address->kind != NETIO_ADDRESS_KIND_UDPv4)
            {
                continue;
            }
            if ((an_address->port < 1024) || (an_address->port > 65535))
            {
                continue;
            }

            if (((an_address->value.ipv4.address & 0x7f000000) == 0x7f000000) &&
                 (lb_index == -1))
            {
                lb_index = k;
            }
            src_address.kind = NETIO_ADDRESS_KIND_UDPv4;
            src_address.port = an_address->port;
            if (UDP_Interface_is_multicast(an_address))
            {
                /* multicast */
                if (!if_data.mc_enabled)
                {
                    continue;
                }
                src_address.value.ipv4.address = an_address->value.ipv4.address;
                src_intf->multicast_enabled = RTI_TRUE;
            }
            else
            {
                /* unicast */
                if (an_address->value.ipv4.address == 0)
                {
                    /* Use the interface address */
                    src_address.value.ipv4.address = if_data.intf_address;
                    if ((if_data.intf_address & 0x7f000000) == 0x7f000000)
                    {
                        /* Don't add lb intf */
                        lb_index = -2;
                    }
                }
                else if (an_address->value.ipv4.address == if_data.intf_address)
                {
                    if ((if_data.intf_address & 0x7f000000) == 0x7f000000)
                    {
                        /* Don't add lb intf */
                        lb_index = -2;
                    }
                    /* If the interface address is specified, it must be the
                     * same as the interface address. This allows us to filter
                     * out addresses that are listed but does not exist
                     */
                    src_address.value.ipv4.address = an_address->value.ipv4.address;
                }
                else
                {
                    /* Invalid address, ignore it */
                    continue;
                }
            }

            /* Don't duplicate addresses */
            for (j = 0; j < NETIO_AddressSeq_get_length(rcvd_addr); ++j)
            {
                if ((NETIO_AddressSeq_get_reference(rcvd_addr,j)->value.ipv4.address == src_address.value.ipv4.address) &&
                        (NETIO_AddressSeq_get_reference(rcvd_addr,j)->port == src_address.port))
                {
                    break;
                }
            }
            if (j < NETIO_AddressSeq_get_length(rcvd_addr))
            {
                continue;
            }

            NETIO_AddressSeq_set_length(rcvd_addr,cur_addr_len+i+1);

            /* Check if there is a NAT entry for this local address. If that
             * is case the case, return the public address
             */
            if (src_intf->nat)
            {
                dbrc = DB_Table_select_match(src_intf->nat,
                                             src_intf->nat_local_idx,
                                             (DB_Record_T*)&nat_entry,
                                             &src_address);
                if (dbrc == DB_RETCODE_OK)
                {
                    src_address = nat_entry->public_address;
                }
            }
            *NETIO_AddressSeq_get_reference(rcvd_addr,cur_addr_len+i) =
                                            src_address;
            ++i;
        }
    }

    UDP_Interface_get_next_interface_done(src_intf,&if_context);

    if (src_intf->multicast_enabled)
    {
        if (!UDP_Interface_enable_multicast_loopback(src_intf))
        {
            goto finally;
        }
    }

#if 0
#if defined (RTI_WIN32)
    cur_addr_len = NETIO_AddressSeq_get_length(rcvd_addr);
    max_size = NETIO_AddressSeq_get_maximum(rcvd_addr) - cur_addr_len;
    if ((lb_index >=0) && (max_size > 0))
    {
        an_address = NETIO_AddressSeq_get_reference(req_addr,lb_index);
        src_address.kind = NETIO_ADDRESS_KIND_UDPv4;
        src_address.port = an_address->port;
        src_address.value.ipv4.address = an_address->value.ipv4.address;

        if (src_intf->nat)
        {
            dbrc = DB_Table_select_match(src_intf->nat,
                                         src_intf->nat_local_idx,
                                         (DB_Record_T*)&nat_entry,
                                         &src_address);
            if (dbrc == DB_RETCODE_OK)
            {
                src_address = nat_entry->public_address;
            }
        }

        NETIO_AddressSeq_set_length(rcvd_addr,cur_addr_len+1);
        *NETIO_AddressSeq_get_reference(rcvd_addr,cur_addr_len) = src_address;
    }
#endif
#endif

    /* Bind will do the final filterng or reserved addresses */
    if (!UDP_Interface_bind_addresses(src_intf,start_index,rcvd_addr,property))
    {
        goto finally;
    }

    ok = RTI_TRUE;

finally:

    return ok;
}

RTI_BOOL
UDP_Interface_initialize(struct UDP_Interface *udp_intf,
                        struct UDP_InterfaceFactory *factory,
                        const struct UDP_InterfaceProperty *const property,
                        const struct NETIO_InterfaceListener *const listener)
{
    struct DB_TableProperty tbl_prop = DB_TableProperty_INITIALIZER;
    char tbl_name[16];
    union RT_ComponentFactoryId id;
    struct DB_IndexProperty idx_prop = DB_IndexProperty_INITIALIZER;
    RTI_INT32 i,len;
    struct UDP_NatEntry *nat_record;
    struct UDP_NatEntrySeq *nat_seq;
    DB_ReturnCode_T dbrc;

    udp_intf->send_socket = -1;

    if (property)
    {
        udp_intf->property = *property;
    }

    if (!NETIO_Interface_initialize(&udp_intf->_parent,
                           &UDP_Interface_g_intf,
                           (property ? &property->_parent : NULL),
                           (listener ? listener : NULL)))
    {
        return RTI_FALSE;
    }

    id._value = factory->_parent._id._value;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'r',
                                      factory->instance_counter);

    tbl_prop.max_records = property->_parent.max_routes;
    if (DB_Database_create_table(&udp_intf->_parent._rtable,
                                property->_parent._parent.db,
                                &tbl_name[0],
                                sizeof(struct UDPRouteEntry),
                                NULL,
                                NETIO_Interface_compare_route,
                                &tbl_prop) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    tbl_prop.max_records = property->_parent.max_routes;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'b',
                                      factory->instance_counter);

    if (DB_Database_create_table(&udp_intf->_parent._btable,
                                property->_parent._parent.db,
                                &tbl_name[0],
                                sizeof(struct UDPBindEntry),
                                NULL,
                                NETIO_Interface_compare_bind,
                                &tbl_prop) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    tbl_prop.max_records = property->_parent.max_routes;
    NETIO_Interface_Table_name_from_id(tbl_name,&id,'p',
                                       factory->instance_counter);

    udp_intf->rx_thread_table = NULL;
    if (DB_Database_create_table(&udp_intf->rx_thread_table,
                                property->_parent._parent.db,
                                &tbl_name[0],
                                sizeof(struct UDPPortEntry),
                                NULL,
                                UDP_Interface_compare_port,
                                &tbl_prop) != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    udp_intf->nat = NULL;
    udp_intf->nat_local_idx = NULL;
    udp_intf->nat_public_idx = NULL;

    len = UDP_NatEntrySeq_get_length(&factory->property->nat);
    if (len > 0)
    {
        nat_seq = &factory->property->nat;
        tbl_prop.max_indices = 2;
        tbl_prop.max_records = len;
        NETIO_Interface_Table_name_from_id(tbl_name,&id,'n',
                                           factory->instance_counter);
        if (DB_Database_create_table(&udp_intf->nat,
                property->_parent._parent.db,
                &tbl_name[0],
                sizeof(struct UDP_NatEntry),
                NULL,
                UDP_Interface_compare_nat_entry,
                &tbl_prop) != DB_RETCODE_OK)
        {
            return RTI_FALSE;
        }

        if (DB_Table_create_index(udp_intf->nat,
                                  &udp_intf->nat_local_idx,
                                  NULL,UDP_Interface_compare_nat_local,
                                  &idx_prop) != DB_RETCODE_OK)
        {
            return RTI_FALSE;
        }

        if (DB_Table_create_index(udp_intf->nat,
                                  &udp_intf->nat_public_idx,
                                  NULL,UDP_Interface_compare_nat_public,
                                  &idx_prop) != DB_RETCODE_OK)
        {
            return RTI_FALSE;
        }

        for (i = 0; i < len; i++)
        {
            nat_record = NULL;
            dbrc = DB_Table_create_record(udp_intf->nat,
                                          (DB_Record_T*)&nat_record);
            if (dbrc != DB_RETCODE_OK)
            {
                return RTI_FALSE;
            }
            *nat_record = *UDP_NatEntrySeq_get_reference(nat_seq,i);
            dbrc = DB_Table_insert_record(udp_intf->nat,nat_record);
            if (dbrc != DB_RETCODE_OK)
            {
                return RTI_FALSE;
            }
        }
    }

    udp_intf->factory = factory;
    ++factory->instance_counter;

    udp_intf->send_socket = UDP_Interface_create_socket(udp_intf);
    if (udp_intf->send_socket == -1)
    {
        return RTI_FALSE;
    }

    NETIO_Address_init(&udp_intf->mc_bind_address,NETIO_ADDRESS_KIND_UDPv4);
    udp_intf->multicast_enabled = RTI_FALSE;

    return RTI_TRUE;
}

/*******************************************************************************
 *                               PUBLIC API
 ******************************************************************************/
RTI_BOOL
UDP_InterfaceFactoryProperty_initialize(struct UDP_InterfaceFactoryProperty *p)
{
    struct UDP_InterfaceFactoryProperty init =
                            UDP_InterfaceFactoryProperty_INITIALIZER;

    *p = init;

    return RTI_TRUE;
}

RTI_BOOL
UDP_InterfaceFactoryProperty_finalize(struct UDP_InterfaceFactoryProperty *p)
{
    REDA_StringSeq_finalize(&p->allow_interface);
    REDA_StringSeq_finalize(&p->deny_interface);
    if (p->multicast_interface != NULL)
    {
        REDA_String_free(p->multicast_interface);
        p->multicast_interface = NULL;

    }
    UDP_NatEntrySeq_finalize(&p->nat);

    return RTI_TRUE;
}

RTI_BOOL
UDP_InterfaceFactoryProperty_copy(struct UDP_InterfaceFactoryProperty *out,
                                 struct UDP_InterfaceFactoryProperty *in)
{
    return RTI_FALSE;
}

struct UDP_Interface*
UDP_Interface_create(struct UDP_InterfaceFactory *factory,
                      const struct UDP_InterfaceProperty *const property,
                      const struct NETIO_InterfaceListener *const listener)
{
    struct UDP_Interface *udp_intf = NULL;

    LOG_PRECONDITION_P2((factory == NULL) ||
                        (property == NULL),
                        return NULL,
                        OSAPI_LOG_PTR_LOW(factory),
                        OSAPI_LOG_PTR_LOW(property))

    OSAPI_Heap_allocateStructure(&udp_intf, struct UDP_Interface);
    if (udp_intf == NULL)
    {
        return NULL;
    }

    if (!UDP_Interface_initialize(udp_intf,factory,property,NULL))
    {
        return NULL;
    }

    ++factory->instance_counter;

    return udp_intf;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_send(NETIO_Interface_T *netio_intf,
                  struct NETIO_Interface *source,
                  struct NETIO_Address *address,
                  NETIO_Packet_T *packet)
{
    struct UDP_Interface *self = (struct UDP_Interface *)netio_intf;
    struct sockaddr_in sock_addr;
    RTI_SIZE_T bytes_sent;
    RTI_INT32 i;
    struct NETIO_Address *dest_addr = NULL;

    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (source == NULL) ||
                        (packet == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(source),
                        OSAPI_LOG_PTR_LOW(address),
                        OSAPI_LOG_PTR_LOW(packet))

    /* NOTE: There are some fields in sock_add that gives a warning in valgrind
     */
    OSAPI_Memory_zero(&sock_addr,sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;

    /* no destinations to which to send */
    if (packet->dests == NULL)
    {
        return RTI_TRUE;
    }

    for (i = 0 ; i < NETIO_AddressSeq_get_length(packet->dests); ++i)
    {
        dest_addr = NETIO_AddressSeq_get_reference(packet->dests, i);
        sock_addr.sin_port = NETIO_htons((RTI_UINT16)dest_addr->port);
        sock_addr.sin_addr.s_addr = NETIO_htonl(dest_addr->value.ipv4.address);

        bytes_sent = netiosock_sendto(self->send_socket,
                                  NETIO_Packet_get_head(packet),
                                  NETIO_Packet_get_payload_length(packet), 0,
                                  (struct sockaddr *)(&sock_addr),
                                  sizeof(struct sockaddr_in));

        LOG_printDebug("UDP wrote: %d bytes to %d/%x.%x.%x.%x\n",bytes_sent,
                            (RTI_UINT16)dest_addr->port,
                            dest_addr->value.rtps_guid.host_id,
                            dest_addr->value.rtps_guid.app_id,
                            dest_addr->value.rtps_guid.instance_id,
                            dest_addr->value.rtps_guid.object_id);
    }

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_ack(NETIO_Interface_T *netio_intf,
                       struct NETIO_Address *source,
                       NETIO_PacketId_T *packet_id)
{
    LOG_PRECONDITION_P2((netio_intf == NULL) ||
                        (source == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(source))

    return RTI_FALSE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_request(NETIO_Interface_T *netio_intf,
                           struct NETIO_Address *source,
                           struct NETIO_Address *dest,
                           NETIO_Packet_T **packet,
                           NETIO_PacketId_T *packet_id)
{
    LOG_PRECONDITION_P3((netio_intf == NULL) ||
                        (source == NULL) ||
                        (packet == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(source),
                        OSAPI_LOG_PTR_LOW(packet))

    return RTI_FALSE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_return_loan(NETIO_Interface_T *netio_intf,
                               struct NETIO_Address *source,
                               NETIO_Packet_T *packet,
                               NETIO_PacketId_T *packet_id)
{
    LOG_PRECONDITION_P3((netio_intf == NULL) ||
                        (source == NULL) ||
                        (packet == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(source),
                        OSAPI_LOG_PTR_LOW(packet))

    return RTI_FALSE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_discard(NETIO_Interface_T *netio_intf,
                           struct NETIO_Address *destination,
                           NETIO_PacketId_T *packet_id)
{
    LOG_PRECONDITION_P2((netio_intf == NULL) ||
                        (destination == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(destination))

    return RTI_FALSE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_add_route(NETIO_Interface_T *netio_intf,
                       struct NETIO_Address *dst_addr,
                       NETIO_Interface_T *via_intf,
                       struct NETIO_Address *via_addr,
                       struct NETIORouteProperty *property,
                       RTI_BOOL *existed)
{
    NETIO_Packet_T ping_packet = NETIO_Packet_INITIALIZER;

    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (dst_addr == NULL) ||
                        (via_intf == NULL) ||
                        (via_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr),
                        OSAPI_LOG_PTR_LOW(via_intf),
                        OSAPI_LOG_PTR_LOW(via_addr))

    ping_packet.buffer = NETIO_PING_MSG;
    ping_packet.length = NETIO_PING_MSG_SIZE;

    UDP_Interface_send(netio_intf,netio_intf,dst_addr,&ping_packet);

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_delete_route(NETIO_Interface_T *netio_intf,
                          struct NETIO_Address *dst_addr,
                          NETIO_Interface_T *via_intf,
                          struct NETIO_Address *via_addr,
                          RTI_BOOL *existed)
{
    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (dst_addr == NULL) ||
                        (via_intf == NULL) ||
                        (via_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr),
                        OSAPI_LOG_PTR_LOW(via_intf),
                        OSAPI_LOG_PTR_LOW(via_addr))

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_reserve_address(NETIO_Interface_T *self,
                                    struct NETIO_AddressSeq *req_addr,
                                    struct NETIO_AddressSeq *resvd_addr,
                                    struct NETIOBindProperty *property)
{
    struct UDP_Interface *src_intf = (struct UDP_Interface *)self;

    if (!UDP_Interface_read_interface_list(
                                    src_intf,req_addr,resvd_addr,property))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_release_address(NETIO_Interface_T *self,
                              struct NETIO_Address *address)
{
    struct UDP_Interface *src_intf = (struct UDP_Interface *)self;
    struct UDPPortEntry *port_entry = NULL;
    DB_ReturnCode_T dbrc;
    RTI_BOOL retval = RTI_FALSE;
    struct UDP_NatEntry *nat_entry;
    struct NETIO_Address src_address;

    src_address = *address;

    if (src_intf->nat)
    {
        dbrc = DB_Table_select_match(src_intf->nat,
                src_intf->nat_public_idx,
                (DB_Record_T*)&nat_entry,address);

        if (dbrc == DB_RETCODE_OK)
        {
            src_address = nat_entry->local_address;
        }
    }

    LOG_printDebug("release address: %d/%d/%x\n",
            src_address.kind,src_address.port,src_address.value.ipv4.address);

    DB_Database_lock(src_intf->property._parent._parent.db);

    dbrc = DB_Table_select_match(src_intf->rx_thread_table,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&port_entry,
                                (DB_Key_T)&src_address);

    if (dbrc == DB_RETCODE_NOT_EXISTS)
    {
        LOG_printDebug("release address not found: %d/%d/%x\n",
             src_address.kind,src_address.port,src_address.value.ipv4.address);
        goto done;
    }

    if (dbrc != DB_RETCODE_OK)
    {
        LOG_printDebug("release address some error %d: %d/%d/%x\n",dbrc,
             src_address.kind,src_address.port,src_address.value.ipv4.address);
        goto done;
    }

    if (port_entry->_ref_count != 0)
    {
        LOG_printDebug("release address in use: count = %d, %d/%d/%x\n",
            port_entry->_ref_count,
            src_address.kind,src_address.port,src_address.value.ipv4.address);
        goto done;
    }

    LOG_printDebug("delete entry: %d/%d/%x\n",port_entry->source.kind,port_entry->source.port,port_entry->source.value.ipv4.address);

    dbrc = DB_Table_remove_record(src_intf->rx_thread_table,
                                  (DB_Record_T*)&port_entry,
                                  (DB_Key_T)&src_address);

    if (dbrc != DB_RETCODE_OK)
    {
        LOG_printDebug("release address not found: %d/%d/%x\n",
            src_address.kind,src_address.port,src_address.value.ipv4.address);
        goto done;
    }


    DB_Database_unlock(src_intf->property._parent._parent.db);

    if (port_entry->_rx_thread != NULL)
    {
        OSAPI_Thread_destroy(port_entry->_rx_thread);
        port_entry->_rx_thread = NULL;
    }

    if (port_entry->_rx_buffer.buffer != NULL)
    {
        OSAPI_Heap_freeBuffer(port_entry->_rx_buffer.buffer);
        port_entry->_rx_buffer.buffer = NULL;
    }

    if (UDP_Interface_is_multicast(&port_entry->source))
    {
        struct ip_mreq imr;
        int rc;
        imr.imr_interface.s_addr = NETIO_htonl(src_intf->mc_bind_address.value.ipv4.address);
        imr.imr_multiaddr.s_addr = NETIO_htonl(port_entry->source.value.ipv4.address);
        rc = netiosock_setsockopt(port_entry->_sock,IPPROTO_IP,
                                  IP_DROP_MEMBERSHIP,
                                  (void *)&imr, sizeof(imr));
        if (rc < 0)
        {
            NETIO_Log_error_p1(LOG_FAIL_SOCKET_OPTIONING_IP_MULTICAST_IF,rc);
        }
    }

    netiosock_close(port_entry->_sock);

    port_entry->_sock = -1;

    DB_Database_lock(src_intf->property._parent._parent.db);

    dbrc = DB_Table_delete_record(src_intf->rx_thread_table,
                                (DB_Record_T *)port_entry);

    if (dbrc == DB_RETCODE_OK)
    {
        LOG_printDebug("deleted port entry: %d/%d/%x\n",port_entry->source.kind,port_entry->source.port,port_entry->source.value.ipv4.address);
    }
    else
    {
        LOG_printDebug("failed to port entry: %d/%d/%x\n",port_entry->source.kind,port_entry->source.port,port_entry->source.value.ipv4.address);
    }

    retval = (dbrc == DB_RETCODE_OK ? RTI_TRUE : RTI_FALSE);

done:

    DB_Database_unlock(src_intf->property._parent._parent.db);

    return retval;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_bind(NETIO_Interface_T *netio_intf,
                  struct NETIO_Address *src_addr,
                  struct NETIOBindProperty *property,
                  RTI_BOOL *existed)
{
    LOG_PRECONDITION_P2((netio_intf == NULL) ||
                        (src_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(src_addr))

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_unbind(NETIO_Interface_T *netio_intf,
                    struct NETIO_Address *src_addr,
                    NETIO_Interface_T *dst_intf,
                    RTI_BOOL *existed)
{
    LOG_PRECONDITION_P3((netio_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_intf == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_intf))

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_bind_external(NETIO_Interface_T *self,
                           struct NETIO_Address *src_addr,
                           NETIO_Interface_T *dst_intf,
                           struct NETIO_Address *dst_addr,
                           struct NETIOBindProperty *property,
                           RTI_BOOL *existed)
{
    struct UDP_Interface *src_intf = (struct UDP_Interface *)self;
    struct UDPBindEntry *bind_entry = NULL;
    struct UDPPortEntry *port_entry = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIOBindEntryKey bind_key;
    struct UDP_NatEntry *nat_entry;
    struct NETIO_Address src_address;

    LOG_PRECONDITION_P4((src_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_intf == NULL) ||
                        (dst_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(src_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr))

    LOG_printDebug("UDP_Interface_bind_external: %d/%x.%x.%x.%x ==> "
            "dst %d/%x.%x.%x.%x\n",
            src_addr->port,
            src_addr->value.rtps_guid.host_id,
            src_addr->value.rtps_guid.app_id,
            src_addr->value.rtps_guid.instance_id,
            src_addr->value.rtps_guid.object_id,
            dst_addr->port,
            dst_addr->value.rtps_guid.host_id,
            dst_addr->value.rtps_guid.app_id,
            dst_addr->value.rtps_guid.instance_id,
            dst_addr->value.rtps_guid.object_id);

    if (existed)
    {
        *existed = RTI_FALSE;
    }

    src_address = *src_addr;
    if (src_intf->nat)
    {
        dbrc = DB_Table_select_match(src_intf->nat,
                src_intf->nat_public_idx,
                (DB_Record_T*)&nat_entry,src_addr);

        if (dbrc == DB_RETCODE_OK)
        {
            src_address = nat_entry->local_address;
        }
    }
    /* Find the receive entry */
    dbrc = DB_Table_select_match(src_intf->rx_thread_table,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&port_entry,
                                (DB_Key_T)&src_address);
    if (dbrc != DB_RETCODE_OK)
    {
        LOG_printDebug("address not found\n");
        return RTI_FALSE;
    }

    bind_key.source = src_address;
    bind_key.destination = *dst_addr;

    dbrc = DB_Table_select_match(src_intf->_parent._btable,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&bind_entry,
                                (DB_Key_T)&bind_key);
    if (dbrc == DB_RETCODE_OK)
    {
        if (existed)
        {
            *existed = RTI_TRUE;
        }
        return RTI_TRUE;
    }

    dbrc = DB_Table_create_record(src_intf->_parent._btable,
                                 (DB_Record_T*)&bind_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        LOG_printDebug("failed to create record\n");
        return RTI_FALSE;
    }

    bind_entry->_parent.source = src_address;
    bind_entry->_parent.destination = *dst_addr;
    bind_entry->_parent.intf = dst_intf;

    dbrc = DB_Table_insert_record(src_intf->_parent._btable,
                                 (DB_Record_T*)bind_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        (void)DB_Table_delete_record(src_intf->_parent._btable,
                                     (DB_Record_T*)bind_entry);
        LOG_printDebug("failed to insert record\n");
        return RTI_FALSE;
    }

    LOG_printDebug("UDP bind_external succeeded: %d/%x.%x.%x.%x ==> dst %d/%x.%x.%x.%x\n",
            bind_entry->_parent.source.port,
            bind_entry->_parent.source.value.rtps_guid.host_id,
            bind_entry->_parent.source.value.rtps_guid.app_id,
            bind_entry->_parent.source.value.rtps_guid.instance_id,
            bind_entry->_parent.source.value.rtps_guid.object_id,
            bind_entry->_parent.destination.port,
            bind_entry->_parent.destination.value.rtps_guid.host_id,
            bind_entry->_parent.destination.value.rtps_guid.app_id,
            bind_entry->_parent.destination.value.rtps_guid.instance_id,
            bind_entry->_parent.destination.value.rtps_guid.object_id);


    ++port_entry->_ref_count;

    LOG_printDebug("port %d/%d/%x bound %d times\n",src_addr->kind,src_addr->port,src_addr->value.ipv4.address,port_entry->_ref_count);
    if (port_entry->_ref_count > 1)
    {
        return RTI_TRUE;
    }

    return OSAPI_Thread_start(port_entry->_rx_thread);
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_unbind_external(NETIO_Interface_T *src_intf,
                             struct NETIO_Address *src_addr,
                             NETIO_Interface_T *dst_intf,
                             struct NETIO_Address *dst_addr,
                             RTI_BOOL *existed)
{
    struct UDP_Interface *self = (struct UDP_Interface *)src_intf;
    struct UDPBindEntry *bind_entry = NULL;
    DB_ReturnCode_T dbrc;
    struct NETIOBindEntryKey bind_key;
    struct UDPPortEntry *port_entry = NULL;
    struct UDP_NatEntry *nat_entry;
    struct NETIO_Address src_address;

    LOG_PRECONDITION_P4((src_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_intf == NULL) ||
                        (dst_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(src_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr))

    LOG_printDebug("UDP unbind_external: %d/%x.%x.%x.%x ==>"
            "dst %d/%x.%x.%x.%x\n",
            src_addr->port,
            src_addr->value.rtps_guid.host_id,
            src_addr->value.rtps_guid.app_id,
            src_addr->value.rtps_guid.instance_id,
            src_addr->value.rtps_guid.object_id,
            dst_addr->port,
            dst_addr->value.rtps_guid.host_id,
            dst_addr->value.rtps_guid.app_id,
            dst_addr->value.rtps_guid.instance_id,
            dst_addr->value.rtps_guid.object_id);

    src_address = *src_addr;
    if (self->nat)
    {
        dbrc = DB_Table_select_match(self->nat,
                self->nat_public_idx,
                (DB_Record_T*)&nat_entry,src_addr);

        if (dbrc == DB_RETCODE_OK)
        {
            src_address = nat_entry->local_address;
        }
    }

    bind_key.source = src_address;
    bind_key.destination = *dst_addr;

    dbrc = DB_Table_remove_record(self->_parent._btable,
                                (DB_Record_T*)&bind_entry,
                                (DB_Key_T)&bind_key);
    if (dbrc == DB_RETCODE_NOT_EXISTS)
    {
        LOG_printDebug("UDP Address %d/%d/%d/%x does not exist\n",
                1,src_addr->kind,src_addr->port,src_addr->value.ipv4.address);
        return RTI_TRUE;
    }

    if (dbrc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    (void)DB_Table_delete_record(self->_parent._btable,
                                 (DB_Record_T*)bind_entry);
    LOG_printDebug("UDP unbind_external, deleted bind: %d/%x.%x.%x.%x "
                   "==> dst %d/%x.%x.%x.%x\n",
            src_addr->port,
            src_addr->value.rtps_guid.host_id,
            src_addr->value.rtps_guid.app_id,
            src_addr->value.rtps_guid.instance_id,
            src_addr->value.rtps_guid.object_id,
            dst_addr->port,
            dst_addr->value.rtps_guid.host_id,
            dst_addr->value.rtps_guid.app_id,
            dst_addr->value.rtps_guid.instance_id,
            dst_addr->value.rtps_guid.object_id);

    /* Find the receive entry */
    dbrc = DB_Table_select_match(self->rx_thread_table,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&port_entry,
                                (DB_Key_T)&src_address);
    if (dbrc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    --port_entry->_ref_count;

    LOG_printDebug("port %d/%d/%x unbound,  %d left\n",src_addr->kind,src_addr->port,src_addr->value.ipv4.address,port_entry->_ref_count);

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_get_link_interface(NETIO_Interface_T *netio_intf,
                                struct NETIO_Address *src_addr,
                                NETIO_Interface_T **dst_intf,
                                struct NETIO_Address *dst_addr)
{
    LOG_PRECONDITION_P4((netio_intf == NULL) ||
                        (src_addr == NULL) ||
                        (dst_intf == NULL) ||
                        (dst_addr == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(netio_intf),
                        OSAPI_LOG_PTR_LOW(src_addr),
                        OSAPI_LOG_PTR_LOW(dst_intf),
                        OSAPI_LOG_PTR_LOW(dst_addr))

    *dst_intf = netio_intf;

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_receive(NETIO_Interface_T *netio_intf,
                     struct NETIO_Address *source,
                     NETIO_Packet_T *packet)
{
    struct UDP_Interface *self = (struct UDP_Interface *)netio_intf;
    DB_Cursor_T cursor = NULL;
    struct UDPBindEntry *bind_entry;
    DB_ReturnCode_T dbrc;

    DB_Database_lock(self->property._parent._parent.db);

    dbrc = DB_Table_select_all(self->_parent._btable,
                              DB_TABLE_DEFAULT_INDEX,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&bind_entry);
        if (dbrc == DB_RETCODE_OK)
        {
#if 0
            (bind_entry->destination.value.ipv4.address ==
                    bind_key.destination.value.ipv4.address))
#endif

            if ((bind_entry->_parent.source.value.ipv4.address ==
                    source->value.ipv4.address) &&
                 (bind_entry->_parent.source.port == source->port))
            {
                LOG_printDebug("UDP: fwd: src: %d/%x.%x.%x.%x ==> "
                               "dst %d/%x.%x.%x.%x\n",
                    bind_entry->_parent.source.port,
                    bind_entry->_parent.source.value.rtps_guid.host_id,
                    bind_entry->_parent.source.value.rtps_guid.app_id,
                    bind_entry->_parent.source.value.rtps_guid.instance_id,
                    bind_entry->_parent.source.value.rtps_guid.object_id,
                    bind_entry->_parent.destination.port,
                    bind_entry->_parent.destination.value.rtps_guid.host_id,
                    bind_entry->_parent.destination.value.rtps_guid.app_id,
                    bind_entry->_parent.destination.value.rtps_guid.instance_id,
                    bind_entry->_parent.destination.value.rtps_guid.object_id);
                NETIO_Interface_receive(bind_entry->_parent.intf,
                                       &bind_entry->_parent.destination,
                                       &bind_entry->_parent.destination,
                                       packet);
            }
        }
    } while (dbrc == DB_RETCODE_OK);
    if (cursor != NULL)
    {
        DB_Cursor_finish(self->_parent._btable,cursor);
    }

    DB_Database_unlock(self->property._parent._parent.db);

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_set_state(NETIO_Interface_T *src_intf,
                        NETIO_InterfaceState_T state)
{
    LOG_PRECONDITION((src_intf == NULL),return RTI_FALSE)

    return RTI_FALSE;
}

RTI_BOOL
UDP_Interface_resolve_ipv4_adddress(
                        NETIO_Interface_T *netio_intf,
                        char *address_string,
                        struct NETIO_Address *address_value)
{
    if (address_string[0] == 0)
    {
        NETIO_Address_init(address_value,NETIO_ADDRESS_KIND_UDPv4);
        return RTI_TRUE;
    }

    return NETIO_Address_ipv4_string_to_address(address_value,address_string);
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_add_route_entry(struct NETIO_AddressSeq *address_seq,
                             struct NETIO_NetmaskSeq *netmask_seq,
                             RTI_INT32 address,
                             RTI_INT32 netmask,
                             RTI_INT32 netmask_bits)
{
    struct NETIO_Address src_address;
    struct NETIO_Netmask src_netmask;
    RTI_INT32 len;

    NETIO_Address_init(&src_address,NETIO_ADDRESS_KIND_UDPv4);

    len = NETIO_AddressSeq_get_length(address_seq);
    if (len >= NETIO_AddressSeq_get_maximum(address_seq))
    {
        return RTI_FALSE;
    }

    NETIO_AddressSeq_set_length(address_seq,len+1);
    NETIO_NetmaskSeq_set_length(netmask_seq,len+1);

    src_address.value.ipv4.address = address;
    *NETIO_AddressSeq_get_reference(address_seq,len) = src_address;
    src_netmask.bits = netmask_bits;
    src_netmask.mask[0] = netmask;
    *NETIO_NetmaskSeq_get_reference(netmask_seq,len) = src_netmask;

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
UDP_Interface_get_route_table(NETIO_Interface_T *netio_intf,
                             struct NETIO_AddressSeq *address,
                             struct NETIO_NetmaskSeq *netmask)
{
    RTI_BOOL ok = RTI_FALSE;
    struct UDP_Interface *src_intf = (struct UDP_Interface*)netio_intf;
    RTI_INT32 max_size;
    RTI_INT32 i;
    RTI_INT32 cur_addr_len;
    RTI_BOOL has_mc = RTI_FALSE;
    struct UDP_IfContext if_context = UDP_IFCONTEXT_INITIALIZER;
    struct UDP_NetworkIfInfo if_data = UDP_NetworkIfInfo_INITIALIZER;
    RTI_BOOL if_found;

    cur_addr_len = NETIO_AddressSeq_get_length(address);
    max_size = NETIO_AddressSeq_get_maximum(address) - cur_addr_len;

    if (max_size == 0)
    {
        return RTI_TRUE;
    }

    if (src_intf->factory->property->is_default_interface)
    {

        UDP_Interface_add_route_entry(address,netmask,0,0,0);
        --max_size;
    }

    i = 0;
    while (UDP_Interface_get_next_interface(src_intf,
                        &if_context,&if_data,&if_found) && (i < max_size))
    {
        if (!if_found)
        {
            break;
        }

        if (!UDP_Interface_interface_allowed(src_intf,if_data.lastname))
        {
#ifdef RTI_WIN32
            if (!UDP_Interface_interface_allowed(src_intf,if_data.fname))
            {
                continue;
            }
#else
            continue;
#endif
        }

        if (!has_mc && if_data.mc_enabled)
        {
            has_mc = RTI_TRUE;
        }

        UDP_Interface_add_route_entry(address,netmask,
                                if_data.intf_address,if_data.intf_netmask,32);

        ++i;
    }

    if (has_mc)
    {
        UDP_Interface_add_route_entry(address,netmask,0xef000001,0xe0000000,32);
    }

    UDP_Interface_get_next_interface_done(src_intf,&if_context);

    ok = RTI_TRUE;

    return ok;

}


/******************************************************************************
 *
 * UDP Component Interface
 */
RTI_PRIVATE struct NETIO_InterfaceI UDP_Interface_g_intf =
{
    {DEFAULT_COMPONENT_RUN},
    UDP_Interface_send,                  /* send */
    UDP_Interface_ack,                   /* ack */
    UDP_Interface_request,               /* request */
    UDP_Interface_return_loan,           /* return_loan */
    UDP_Interface_discard,               /* discard */
    UDP_Interface_add_route,             /* add_route */
    UDP_Interface_delete_route,          /* delete_route */
    UDP_Interface_reserve_address,/* get_global_address */
    UDP_Interface_bind,                  /* bind */
    UDP_Interface_unbind,                /* unbind */
    NULL, /* UDP_Interface_receive */    /* receive */
    UDP_Interface_get_link_interface,    /* get_external_interface */
    UDP_Interface_bind_external,         /* bind_external */
    UDP_Interface_unbind_external,       /* unbind_external */
    UDP_Interface_set_state,
    UDP_Interface_release_address,
    UDP_Interface_resolve_ipv4_adddress,
    UDP_Interface_get_route_table
};

/* ------------------------------------------------------------------------ */
/*                   Plugin factory                                         */
/* ------------------------------------------------------------------------ */

RTI_PRIVATE RT_Component_T*
UDP_InterfaceFactory_create_component(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentProperty *property,
        struct RT_ComponentListener *listener)
{
    struct UDP_Interface *retval = NULL;

    retval = UDP_Interface_create(
            (struct UDP_InterfaceFactory*)factory,
            (const struct UDP_InterfaceProperty *const)property,
            (const struct NETIO_InterfaceListener *const)listener);

    return &retval->_parent._parent;
}

RTI_PRIVATE void
UDP_InterfaceFactory_delete_component(struct RT_ComponentFactory *factory,
                                     RT_Component_T *component)
{
    struct UDP_Interface *self = (struct UDP_Interface*)component;

    UDP_Interface_delete(self);
}

RTI_PRIVATE struct RT_ComponentFactory*
UDP_InterfaceFactory_initialize(
        struct RT_ComponentFactoryProperty* property,
        struct RT_ComponentFactoryListener *listener);

RTI_PRIVATE void
UDP_InterfaceFactory_finalize(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener);

RTI_PRIVATE struct RT_ComponentFactoryI UDP_InterfaceFactory_g_intf =
{
    UDP_INTERFACE_INTERFACE_ID,
    UDP_InterfaceFactory_initialize,
    UDP_InterfaceFactory_finalize,
    UDP_InterfaceFactory_create_component,
    UDP_InterfaceFactory_delete_component
};

RTI_PRIVATE
struct UDP_InterfaceFactory UDP_InterfaceFactory_g_factory =
{
  {
     &UDP_InterfaceFactory_g_intf,
     NULL
  }
};

RTI_PRIVATE struct RT_ComponentFactory*
UDP_InterfaceFactory_initialize(
        struct RT_ComponentFactoryProperty *property,
        struct RT_ComponentFactoryListener *listener)
{
    struct UDP_InterfaceFactory *factory = &UDP_InterfaceFactory_g_factory;
    struct UDP_InterfaceFactoryProperty *f_property =
                            (struct UDP_InterfaceFactoryProperty*)property;
    UDP_InterfaceFactory_g_factory._parent._factory =
                                &UDP_InterfaceFactory_g_factory._parent;

    factory->_initialized = RTI_FALSE;
    factory->instance_counter = 0;
    factory->property = f_property;
    if (listener)
    {
        factory->listener = *listener;
    }

#if defined(RTI_WIN32)
    OSAPISocketModule_init();
#endif

    return &UDP_InterfaceFactory_g_factory._parent;
}

RTI_PRIVATE void
UDP_InterfaceFactory_finalize(
        struct RT_ComponentFactory *factory,
        struct RT_ComponentFactoryProperty **property,
        struct RT_ComponentFactoryListener **listener)
{

}

struct RT_ComponentFactoryI*
UDP_InterfaceFactory_get_interface(void)
{
    return &UDP_InterfaceFactory_g_intf;
}

#endif
