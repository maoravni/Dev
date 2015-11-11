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

#if OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS

#include <stdlib.h>
#include <vxWorks.h>
#include <memLib.h>
#include "osapi/osapi_heap.h"
#include "osapi/osapi_string.h"
#include "osapi/osapi_log.h"
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_VXWORKS
/* ----------------------------------------------------------------- */
/* allocate the specified size of memory and align it as well */
void
OSAPI_Heap_allocateBufferInternal(char **buffer,
                                 RTI_UINT32 size,
                                 OSAPIAlignment_T alignment)
{
    LOG_PRECONDITION(buffer == NULL, return)

    if (alignment == OSAPI_ALIGNMENT_DEFAULT)
    {
        alignment = sizeof(void*);
    }

    /* From the documentation of memalign: This routine allocates a buffer of
     * size <size> from the system memory partition.  Additionally, it insures
     * that the allocated buffer begins on a memory address evenly divisible by
     * the specified alignment parameter.  The alignment parameter must be a
     * power of 2 THIS DOCUMENTATION IS MISLEADING: the address where the buffer
     * starts is NOT an even multiple; but just a multiple...so the call below is
     * NOT too conservative. */
    *buffer = memalign(alignment, size);
    if (*buffer == NULL)
    {
        OSAPI_Log_error_p2(OSAPI_LOG_HEAP_INTERNAL_ALLOCATED_FAILED,size,alignment);
        return;
    }

    OSAPI_Memory_zero(*buffer, size);
}

/* ----------------------------------------------------------------- */
/* free the previously allocated memory */
void
OSAPI_Heap_freeBuffer(void *buffer)
{
    LOG_PRECONDITION(buffer == NULL,return);

#ifndef RTI_CERT
    free(buffer); 
#endif
}

#endif

/* end of $Id: Heap.c,v 1.3 2005-05-07 01:37:45 ron Exp $ */





