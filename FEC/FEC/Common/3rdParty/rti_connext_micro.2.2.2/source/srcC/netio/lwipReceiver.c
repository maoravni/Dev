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

#ifdef LWIP_NOSYS

#include <lwip/udp.h>

#include "util/util_makeheader.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_string.h"
#include "netio/netio_log.h"
#include "netio/netio_receiver.h"
#ifndef netio_log_h
#include "netio/netio_log.h"
#endif

struct NETIOReceiverEntryPort
{
    RTI_UINT32 address;
    NETIOPort port;
    struct udp_pcb *udp_pcb;
    struct NETIOMessageReceived *rx_msg;
    RTI_UINT32 rcv_count;
    RTI_UINT32 busy;
    struct pbuf *pbuf;
    struct ip_addr src_addr;
    RTI_INT16 src_port;
};

struct NETIOReceiverEntryPortTable
{
    int num;
    int max;
    struct NETIOReceiverEntryPort *entries;
};

/*Package*/
struct NETIOReceiver
{
    struct NETIOReceiverEntryPortTable entryPorts;

};

#endif

/*** SOURCE_BEGIN ***/

#ifdef LWIP_NOSYS

/* create a new receiver. properties can be NULL, in which case defaults 
 * will be used  
 */
struct NETIOReceiver *
NETIOReceiver_new(const struct NETIOReceiverProperties *properties)
{
    int i;
    struct NETIOReceiver *me;
    struct NETIOReceiverProperties defaultProperties =
        NETIO_RECEIVER_PROPERTIES_DEFAULT;

    if (properties == NULL)
    {
        properties = &defaultProperties;
    }

    OSAPI_Heap_allocateStructure(&me, struct NETIOReceiver);
    if (me == NULL)
    {
        NETIOLog_rti_error(LOG_FAIL_ALLOCATE);
        return NULL;
    }

    me->entryPorts.num = 0;
    me->entryPorts.max = properties->maxEntryPorts;
    OSAPI_Heap_allocateArray(&me->entryPorts.entries, me->entryPorts.max,
                            struct NETIOReceiverEntryPort);
    if (!me->entryPorts.entries)
    {
        goto fail;
    }
    for (i = 0; i < me->entryPorts.max; i++)
    {
        me->entryPorts.entries[i].udp_pcb = NULL;
    }

    return me;

    fail:

    NETIOReceiver_delete(me);

    return NULL;
}

void
NETIOReceiver_delete(struct NETIOReceiver *me)
{
    if (me)
    {
        if (me->entryPorts.entries)
        {
            OSAPI_Heap_freeArray(me->entryPorts.entries);
        }
        OSAPI_Heap_freeStructure(me);
    }
}

<<<<<<< .mine
NETIO_Address
=======
RTI_UINT32
>>>>>>> .r4007
NETIOReceiver_getEntryPortAddress(struct NETIOReceiver *me, int entryPortId)
{
    LOG_testPrecondition(me == NULL || entryPortId >= me->entryPorts.max,
                         return NETIO_ADDRESS_INVALID);

    return me->entryPorts.entries[entryPortId].address;
}

NETIOPort
NETIOReceiver_getEntryPortPort(struct NETIOReceiver *me, int entryPortId)
{
    LOG_testPrecondition(me == NULL || entryPortId >= me->entryPorts.max,
                         return NETIO_PORT_INVALID);

    return me->entryPorts.entries[entryPortId].port;
}

void
NETIOReceiver_receiveI(void *arg,
                       struct udp_pcb *upcb,
                       struct pbuf *p, struct ip_addr *addr, u16_t port)
{
    struct NETIOReceiverEntryPort *me = (struct NETIOReceiverEntryPort *)arg;
    OSAPILogEntryPayload payload;

    if (me->busy)
    {
        me->rcv_count++;
        pbuf_free(p);
        return;
    }

    if (me->pbuf)
    {
        pbuf_free(me->pbuf);
    }

    me->pbuf = p;
    me->rcv_count++;
    me->busy = 1;
    me->src_addr = *addr;
    me->src_port = port;
}

RTI_BOOL
NETIOReceiver_addEntryPort(struct NETIOReceiver *me,
                           int *entryPortIdOut,
<<<<<<< .mine
                           const NETIO_Address * interface,
=======
                           const RTI_UINT32 * interface,
>>>>>>> .r4007
                           const NETIOPort * port,
                           const struct NETIOEntryPortProperties *properties)
{
    RTI_BOOL retval = RTI_FALSE;
    int i;
    OSAPILogEntryPayload payload;
    struct udp_pcb *udp_port;
    struct ip_addr addr;

    LOG_testPrecondition(me == NULL ||
                         entryPortIdOut == NULL ||
                         interface == NULL ||
                         port == NULL || properties == NULL, return RTI_FALSE);

    if (me->entryPorts.num >= me->entryPorts.max)
    {
        NETIOLog_rti_error_1(LOG_EXCEED_MAX_ENTRY_PORTS, me->entryPorts.max);
        return RTI_FALSE;
    }

    udp_port = udp_new();

    if (udp_port == NULL)
    {
        NETIOLog_rti_error_1(LOG_FAIL_CREATE_SOCKET, errno);
        return RTI_FALSE;
    }

    OSAPI_Memory_zero(&addr, sizeof(struct ip_addr));

    addr.addr = (RTI_UINT32) NETIO_htonl(*interface);

    udp_bind(udp_port, &addr, *port);

    for (i = 0; (i < me->entryPorts.max) &&
         (me->entryPorts.entries[i].udp_pcb != NULL); i++) ;

    if (i == me->entryPorts.max)
    {
        goto failed;
    }

    *entryPortIdOut = i;
    me->entryPorts.entries[i].udp_pcb = udp_port;
    me->entryPorts.entries[i].address = *interface;
    me->entryPorts.entries[i].port = *port;
    me->entryPorts.entries[i].rcv_count = 0;
    me->entryPorts.entries[i].rx_msg = NULL;
    me->entryPorts.entries[i].busy = RTI_FALSE;
    me->entryPorts.entries[i].pbuf = NULL;

    me->entryPorts.num++;

    udp_recv(me->entryPorts.entries[i].udp_pcb,
             NETIOReceiver_receiveI, &me->entryPorts.entries[i]);

    retval = RTI_TRUE;

    failed:

    if (!retval)
    {
        udp_remove(udp_port);
    }

    return retval;
}

RTI_BOOL
NETIOReceiver_joinMulticastGroup(struct NETIOReceiver *me,
                                 int entryPortId,
                                 const RTI_UINT32 * multicastAddress)
{
    /* TODO: Add support for multi-cast using raw lwIP APIs */
    return RTI_FALSE;
}

RTI_BOOL
NETIOReceiver_receive(struct NETIOReceiver *me,
                      int entryPortId, struct NETIOMessageReceived *message)
{
    struct NETIOReceiverEntryPort *mep = &me->entryPorts.entries[entryPortId];
    char *dst_ptr;
    int copy_left = 0;
    struct pbuf *p = NULL;

    LOG_testPrecondition(me == NULL ||
                         message == NULL ||
                         entryPortId >= me->entryPorts.max, return RTI_FALSE);

    if (me->entryPorts.entries[entryPortId].busy)
    {
        message->buffer.length = mep->pbuf->tot_len;
        message->sourceAddress = NETIO_ntohl(mep->src_addr.addr);
        message->sourcePort = mep->src_port;
        message->entryPortId = entryPortId;
        copy_left = 0;
        p = mep->pbuf;
        do
        {
            OSAPI_Memory_copy(message->buffer.pointer + copy_left, p->payload,
                             p->len);
            copy_left += p->len;
            p = p->next;
        }
        while (copy_left < mep->pbuf->tot_len);
        me->entryPorts.entries[entryPortId].busy = RTI_FALSE;
        return RTI_TRUE;
    }
    else
    {
        return RTI_FALSE;
    }
}


void
NETIOReceiver_receive_clear(struct NETIOReceiver *me, int entryPortId)
{
}

RTI_INT32
NETIOReceiver_get_rx_count(const struct NETIOReceiver *me, int entryPortId)
{
    return me->entryPorts.entries[entryPortId].rcv_count;
}

RTI_BOOL
NETIOReceiver_unblockEntryPort(const struct NETIOReceiver *me, int entryPortId)
{
    return RTI_TRUE;
}

RTI_BOOL
NETIOReceiver_removeEntryPort(struct NETIOReceiver *me, int entryPortId)
{
    LOG_testPrecondition(me == NULL ||
                         entryPortId >= me->entryPorts.max, return RTI_FALSE);

    if (me->entryPorts.entries[entryPortId].udp_pcb == NULL)
    {
        NETIOLog_debug(LOG_ENTRY_PORT_NOT_EVEN_INITIALIZED_NOTHING_TO_REMOVE);
        return RTI_TRUE;
    }

    udp_remove(me->entryPorts.entries[entryPortId].udp_pcb);
    me->entryPorts.entries[entryPortId].udp_pcb = NULL;
    me->entryPorts.num--;

    return RTI_TRUE;
}

#endif
