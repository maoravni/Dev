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

#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef netio_log_h
#include "netio/netio_log.h"
#endif
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_route_h
#include "netio/netio_route.h"
#endif
#ifndef netio_interface_h
#include "netio/netio_interface.h"
#endif

/*** SOURCE_BEGIN ***/

void
NETIO_Packet_save_positions(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return)

    packet->saved_head_pos = packet->head_pos;
    packet->saved_current_pos = packet->current_pos;
    packet->saved_tail_pos = packet->tail_pos;
}

void
NETIO_Packet_restore_positions(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return)

    packet->head_pos = packet->saved_head_pos;
    packet->current_pos = packet->saved_current_pos;
    packet->tail_pos = packet->saved_tail_pos;
}


RTI_BOOL
NETIO_Packet_set_head(NETIO_Packet_T *packet,RTI_INT32 delta)
{
    LOG_PRECONDITION((packet == NULL),return RTI_FALSE)

    packet->head_pos += delta;

    if (packet->head_pos > packet->tail_pos)
    {
        packet->head_pos = packet->tail_pos;
        return RTI_FALSE;
    }


    return RTI_TRUE;
}

RTI_BOOL
NETIO_Packet_set_current(NETIO_Packet_T *packet,RTI_INT32 delta)
{
    LOG_PRECONDITION((packet == NULL),return RTI_FALSE)

    packet->current_pos += delta;
    if (packet->current_pos < packet->head_pos) 
    {
        packet->current_pos = packet->head_pos;
        return RTI_FALSE;
    }
    if (packet->current_pos > packet->tail_pos)
    {
        packet->current_pos = packet->tail_pos;
        return RTI_FALSE;
    }
    return RTI_TRUE;
}


RTI_BOOL
NETIO_Packet_set_tail(NETIO_Packet_T *packet,RTI_INT32 delta)
{
    LOG_PRECONDITION((packet == NULL),return RTI_FALSE)

    packet->tail_pos += delta;
    if (packet->head_pos > packet->tail_pos)
    {
        packet->tail_pos = packet->head_pos;
        return RTI_FALSE;
    }
    return RTI_TRUE;
}

void*
NETIO_Packet_get_head(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return NULL)

    return (void*)&packet->buffer[packet->head_pos];
}

void*
NETIO_Packet_get_current(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return NULL)

    return (void*)&packet->buffer[packet->current_pos];
}


void*
NETIO_Packet_get_tail(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return NULL)

    return (void*)&packet->buffer[packet->tail_pos];
}

RTI_SIZE_T
NETIO_Packet_get_length(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return RTI_SIZE_INVALID)

    return packet->length;
}

RTI_SIZE_T
NETIO_Packet_get_payload_length(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return RTI_SIZE_INVALID)

    return packet->tail_pos - packet->head_pos;
}

RTI_SIZE_T
NETIO_Packet_get_max_length(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return RTI_SIZE_INVALID)

    return packet->max_length;
}

RTI_BOOL
NETIO_Packet_initialize(NETIO_Packet_T *packet,
                       void *init_buffer,
                       RTI_SIZE_T init_length,
                       RTI_SIZE_T trailer_length,
                       struct NETIO_AddressSeq *dest_seq)
{
    NETIO_Packet_T packet_init = NETIO_Packet_INITIALIZER;

    LOG_PRECONDITION_P2((packet == NULL) ||
                        (init_buffer == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(packet),
                        OSAPI_LOG_PTR_LOW(init_buffer))

    *packet = packet_init;

    if (trailer_length > init_length)
    {
        return RTI_FALSE;
    }

    if (init_buffer)
    {
        packet->buffer = init_buffer;
        packet->max_length = init_length;
    }

    /* 0 indexed */
    packet->current_pos = init_length - trailer_length;
    packet->head_pos =  packet->current_pos;
    packet->tail_pos =  packet->current_pos;
    packet->end_pos =  init_length;

    /* TODO: have dest seq managed from NETIO */
    packet->dests = dest_seq;

    return RTI_TRUE;
}

RTI_BOOL
NETIO_Packet_finalize(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return RTI_FALSE)

    return RTI_TRUE;
}

struct NETIO_PacketInfo*
NETIO_Packet_get_info(NETIO_Packet_T *packet)
{
    LOG_PRECONDITION((packet == NULL),return NULL)

    return &packet->info;
}

void
NETIO_Packet_get_source(NETIO_Packet_T *packet,struct NETIO_Address *src)
{
    LOG_PRECONDITION((packet == NULL),return)

    *src = packet->source;
}

void
NETIO_Packet_set_source(NETIO_Packet_T *packet,struct NETIO_Address *src)
{
    LOG_PRECONDITION((packet == NULL),return)

    packet->source = *src;
}


void
NETIO_Packet_save_positions_to(NETIO_Packet_T *packet,
                               RTI_SIZE_T *head,
                               RTI_SIZE_T *current, 
                               RTI_SIZE_T *tail)
{
    LOG_PRECONDITION((packet == NULL) || (head == NULL) || 
                     (current == NULL) || (tail == NULL), return)

    *head = packet->head_pos;
    *current = packet->current_pos;
    *tail = packet->tail_pos; 
}


void
NETIO_Packet_restore_positions_from(NETIO_Packet_T *packet,
                                    RTI_SIZE_T head,
                                    RTI_SIZE_T current, 
                                    RTI_SIZE_T tail)
{
    LOG_PRECONDITION((packet == NULL), return)

    packet->head_pos = head;
    packet->current_pos = current;
    packet->tail_pos = tail; 
}
