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

#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN 8
#endif

#include <lwip/udp.h>

#include "util/util_makeheader.h"
#include "osapi/osapi_string.h"
#include "osapi/osapi_heap.h"
#include "netio/netio_sender.h"
#ifndef netio_log_h
#include "netio/netio_log.h"
#endif

#include "netio/netio_log.h"

/* ================================================================= */
/*                Table: Destination Table                           */
/* ----------------------------------------------------------------- */

struct NETIOSenderDestinationTableRecord
{
    int count;
    struct ip_addr addr;
    u16_t port;
};

struct NETIOSenderDestinationTable
{
    int numDestinations;
    int maxDestinations;
    struct NETIOSenderDestinationTableRecord *records;
};

/* ================================================================= */
/*                       NETIOSender                                 */
/* ----------------------------------------------------------------- */

struct NETIOSender
{
    struct udp_pcb *pcb;
    struct NETIOSenderDestinationTable destinations;
};
#endif


/*** SOURCE_BEGIN ***/

#ifdef LWIP_NOSYS

RTI_BOOL
NETIOSender_send(struct NETIOSender *me,
                 int *failReason,
                 int *toArray,
                 int toArraySize,
                 const struct REDA_Buffer *buffer, int bufferCount)
{
    RTI_BOOL ok = RTI_FALSE;
    int toCount = 0;
    int destinationTableIndex = 0;
    int i = 0;
    int bytesToSend = 0;
    /* struct pbuf bufferArray[MSG_MAXIOVLEN]; */
    struct pbuf *pbuf, *prev_pbuf = NULL;

    LOG_testPrecondition(me == NULL ||
                         failReason == NULL ||
                         (toArraySize && toArray == NULL) ||
                         (bufferCount && buffer == NULL), goto finally);

    REDA_BufferArray_getSize(&bytesToSend, buffer, bufferCount);

    for (i = 0; i < bufferCount; ++i)
    {
        pbuf = pbuf_alloc(PBUF_TRANSPORT, buffer[i].length, PBUF_REF);
        pbuf->payload = buffer[i].pointer;
        if (prev_pbuf != NULL)
        {
            pbuf_cat(prev_pbuf, pbuf);
        }
        else
        {
            prev_pbuf = pbuf;
        }
    }

    for (toCount = 0; toCount < toArraySize; toCount++)
    {
        destinationTableIndex = toArray[toCount];
        udp_sendto(me->pcb,
                   prev_pbuf,
                   &me->destinations.records[destinationTableIndex].addr,
                   me->destinations.records[destinationTableIndex].port);
    }

    pbuf_free(prev_pbuf);

    ok = RTI_TRUE;

    finally:

    return ok;
}

RTI_BOOL
NETIOSender_queryCapability(struct NETIOSender *me,
                            int *messageSizeMaxOut,
                            int *gatherSendBufferCountMaxOut)
{
    LOG_testPrecondition(me == NULL ||
                         messageSizeMaxOut == NULL ||
                         gatherSendBufferCountMaxOut == NULL, return RTI_FALSE);

/* make it a little faster... we shouldn't calculate everytime */
#define MESSAGE_SIZE_MAX_OUT 65536      /* 64 * 1024 */
#define GATHER_SEND_BUFFER_COUNT_MAX_OUT 16

    /*me->transport->properties->messageSizeMax; */
    *messageSizeMaxOut = MESSAGE_SIZE_MAX_OUT;

    /* me->transport->properties->gatherSendBufferCountMax; */
    *gatherSendBufferCountMaxOut = GATHER_SEND_BUFFER_COUNT_MAX_OUT;

    return RTI_TRUE;
}

RTI_BOOL
NETIOSender_getDestinationId(struct NETIOSender *me,
                             int *destinationIdOut,
                             const RTI_UINT32 * address,
                             const NETIOPort * port)
{
    int i = 0, num = 0;

    LOG_testPrecondition(me == NULL
                         || destinationIdOut == NULL, return RTI_FALSE);

    /* TODO: will need to add reference count! */
    for (i = 0; num < me->destinations.numDestinations &&
         i < me->destinations.maxDestinations; i++)
    {
        if (me->destinations.records[i].count > 0)
        {
            num++;
            if (me->destinations.records[i].addr.addr == NETIO_htonl(*address)
                && me->destinations.records[i].port == *port)
            {
                *destinationIdOut = i;
                return RTI_TRUE;
            }
        }
    }

    return RTI_FALSE;
}

RTI_BOOL
NETIOSender_addDestination(struct NETIOSender *me,
                           int *destinationIdOut,
                           const RTI_UINT32 * address, const NETIOPort * port)
{
    int i = 0;
    OSAPILogEntryPayload payload;
    struct pbuf *pbuf;

    LOG_testPrecondition(me == NULL
                         || destinationIdOut == NULL, return RTI_FALSE);

    if (NETIOSender_getDestinationId(me, destinationIdOut, address, port))
    {
        me->destinations.records[*destinationIdOut].count++;
        return RTI_TRUE;
    }

    for (i = 0; (i < me->destinations.maxDestinations) &&
         (me->destinations.records[i].count > 0); i++) ;

    if (i == me->destinations.maxDestinations)
    {
        NETIOLog_rti_error_2
            (LOG_FAIL_ADD_NEW_ENTRY_DESTINATION_TABLE_ALREADY_FULL, *address,
             *port);
        return RTI_FALSE;
    }

    me->destinations.records[i].addr.addr = NETIO_htonl(*address);
    me->destinations.records[i].port = 9;       /* 9 is a well-known "discard" port */

    pbuf = pbuf_alloc(PBUF_TRANSPORT, NETIO_PING_MSG_SIZE, PBUF_RAM);
    memcpy(pbuf->payload, NETIO_PING_MSG, NETIO_PING_MSG_SIZE);

    if (!udp_sendto(me->pcb, pbuf,
                    &me->destinations.records[i].addr,
                    me->destinations.records[i].port))
    {
        NETIOLog_warning_3(LOG_FAIL_SEND_PING, *address, *port, errno);
    }

    me->destinations.records[i].port = *port;
    me->destinations.records[i].count = 1;
    me->destinations.numDestinations++;
    *destinationIdOut = i;

    return RTI_TRUE;
}

void
NETIOSender_removeDestination(struct NETIOSender *me, int destinationId)
{
    LOG_testPrecondition(me == NULL ||
                         destinationId >= me->destinations.maxDestinations ||
                         me->destinations.records[destinationId].count <= 0,
                         return);

    me->destinations.records[destinationId].count--;

    if (me->destinations.records[destinationId].count <= 0)
    {
        me->destinations.numDestinations--;
    }
}

void
NETIOSender_delete(struct NETIOSender *me)
{
    if (me == NULL)
    {
        return;
    }

    if (me->destinations.records)
    {
        OSAPI_Heap_freeArray(me->destinations.records);
    }

    if (me->pcb != NULL)
    {
        udp_remove(me->pcb);
    }

    OSAPI_Heap_freeStructure(me);
}

struct NETIOSender *
NETIOSender_new(const struct NETIOSenderProperties *properties)
{
    struct NETIOSender *me = NULL;
    int i = 0;
    struct NETIOSenderProperties defaultProperties =
        NETIO_SENDER_PROPERTIES_DEFAULT;

    OSAPI_Heap_allocateStructure(&me, struct NETIOSender);
    if (me == NULL)
    {
        NETIOLog_rti_error(LOG_FAIL_ALLOCATE);
        return NULL;
    }
    me->destinations.records = NULL;
    me->pcb = udp_new();

    if (properties == NULL)
    {
        properties = &defaultProperties;
    }

    me->destinations.numDestinations = 0;
    me->destinations.maxDestinations = properties->destinationTableMax;

    OSAPI_Heap_allocateArray(&me->destinations.records,
                            me->destinations.maxDestinations,
                            struct NETIOSenderDestinationTableRecord);
    if (me->destinations.records == NULL)
    {
        NETIOLog_rti_error(LOG_FAIL_ALLOCATE);
        goto fail;
    }

    for (i = 0; i < me->destinations.maxDestinations; i++)
    {
        OSAPI_Memory_zero(&me->destinations.records[i].addr,
                         sizeof(me->destinations.records[i].addr));
        me->destinations.records[i].count = 0;
    }

    return me;

    fail:

    NETIOSender_delete(me);

    return NULL;
}

#endif
