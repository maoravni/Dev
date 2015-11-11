/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef util_makeheader_h
#include "util/util_makeheader.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef cdr_log_h
#include "cdr/cdr_log.h"
#endif

/*** SOURCE_BEGIN ***/

void
CDR_Stream_Free(struct CDR_Stream_t *cdrs)
{
    if (cdrs)
    {
        if (cdrs->realbuff)
        {
            OSAPI_Heap_freeArray(cdrs->realbuff);
        }
        OSAPI_Heap_freeStructure(cdrs);
    }
}

struct CDR_Stream_t*
CDR_Stream_Alloc(RTI_UINT32 buffsize)
{
    struct CDR_Stream_t *cdrs = NULL;
    struct CDR_Stream_t *returnCdrs = NULL;
    RTI_UINT8 lsb = 0;
    RTI_UINT8 align = CDR_MAX_ALIGNMENT - 1;

    LOG_testPrecondition(buffsize <= 0, return NULL);

    OSAPI_Heap_allocateStructure(&cdrs, struct CDR_Stream_t);
    if (cdrs == NULL)
    {
        CDR_Log_error(LOG_FAIL_ALLOCATE);
        goto finally;
    }

    /* allocate buffer memory (requested buffsize plus alignment overhead) */
    OSAPI_Heap_allocateArray(&cdrs->realbuff,
                            buffsize + (CDR_MAX_ALIGNMENT - 1),
                            char);
    if (cdrs->realbuff == NULL)
    {
        CDR_Log_error(LOG_FAIL_ALLOCATE);
        goto finally;
    }

    /* align allocated buffer to CDR_MAX_ALIGNMENT */
    cdrs->buffer = cdrs->realbuff;
    lsb = (RTI_UINT8)((RTI_UINT32)(cdrs->buffer));
    lsb = lsb & align;
    if (lsb != 0)
    {
        cdrs->buffer += (CDR_MAX_ALIGNMENT - lsb);
    }

    /* set buffer length to requested size */
    cdrs->length = buffsize;

    /* reset the stream pointer */
    cdrs->buffptr = cdrs->buffer;
    cdrs->alignBase = cdrs->buffer;
    cdrs->needbyteswap = CDR_BYTESWAP_INVALID;

    returnCdrs = cdrs;

finally:

    if (!returnCdrs)
    {
        CDR_Stream_Free(cdrs);
    }

    return returnCdrs;
}

void
CDR_Stream_Reset(struct CDR_Stream_t *cdrs)
{
    LOG_testPrecondition(cdrs == NULL, return);

    (cdrs)->buffptr = (cdrs)->buffer;
    (cdrs)->alignBase = (cdrs)->buffer;
}

RTI_BOOL
CDR_Stream_ByteswapSet(struct CDR_Stream_t *cdrs, RTI_BOOL littleEndian)
{
    LOG_testPrecondition(cdrs == NULL, return RTI_FALSE);

#ifdef RTI_ENDIAN_LITTLE
    cdrs->needbyteswap = littleEndian ? 0 : 1;
    cdrs->endian = RTI_CDR_ENDIAN_LITTLE;
#else
    cdrs->needbyteswap = littleEndian ? 1 : 0;
    cdrs->endian = RTI_CDR_ENDIAN_BIG;
#endif

    return RTI_TRUE;
}

RTI_UINT32
CDR_Stream_GetCurrentPositionOffset(struct CDR_Stream_t *cdrs)
{
    LOG_testPrecondition(cdrs == NULL, return 0);

    return (cdrs)->buffptr - (cdrs)->buffer;
}


RTI_BOOL
CDR_Stream_SetCurrentPositionOffset(struct CDR_Stream_t *cdrs,
                                      RTI_UINT32 num)
{
    LOG_testPrecondition(cdrs == NULL, return RTI_FALSE);

    /* unsigned num cannot be less than zero */
    if (num <= cdrs->length)
    {
        cdrs->buffptr = cdrs->buffer + num;
        return RTI_TRUE;
    }
    else
    {
        return RTI_FALSE;
    }
}

RTI_BOOL
CDR_Stream_IncrementCurrentPosition(struct CDR_Stream_t *me,
                                      RTI_INT32 amount)
{
    LOG_testPrecondition(me == NULL || me->buffptr == NULL, return RTI_FALSE);

    if (amount != 0)
    {
        if (amount > 0)
        {
            if (amount > (me->length - CDR_Stream_GetCurrentPositionOffset(me)))
            {
                return RTI_FALSE;
            }
        }
        else
        {
            if (-(amount) > CDR_Stream_GetCurrentPositionOffset(me))
            {
                return RTI_FALSE;
            }
        }
    
        me->buffptr += amount;
    }

    return RTI_TRUE;
}


RTI_BOOL
CDR_Stream_SetBuffer(struct CDR_Stream_t * me, char *buf, RTI_UINT32 length)
{
    LOG_testPrecondition(me == NULL, return RTI_FALSE);

    me->realbuff = buf;
    me->buffer = buf;
    me->alignBase = buf;
    me->buffptr = buf;
    me->length = length;

    return RTI_TRUE;
}
