/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#include "osapi/osapi_config.h"

#ifdef INCLUDE_POSIX
#include "osapi/osapi_log.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_string.h"
#endif

/*** SOURCE_BEGIN ***/

#ifdef INCLUDE_POSIX

void*
OSAPI_Heap_allocate(RTI_INT32 a,RTI_INT32 b)
{
    void *ptr;

    ptr = calloc(a, b);

#if defined(RTI_DARWIN)
    OSAPI_Heap_g_allocatedByteCount += malloc_size(ptr);
#elif defined(RTI_LINUX)
#if !defined(__ANDROID__)
    OSAPI_Heap_g_allocatedByteCount += malloc_usable_size(ptr);
#else
    OSAPI_Heap_g_allocatedByteCount += a*b;
#endif
#else
#error "OSAPI_Heap_allocateBufferInternal must be ported"
#endif

    return ptr;
}

/* ----------------------------------------------------------------- */
/* allocate the specified size of memory and align it as well */
void
OSAPI_Heap_allocateBufferInternal(char **buffer,
                                 RTI_UINT32 size,
                                 OSAPIAlignment_T alignment)
{
    LOG_PRECONDITION(buffer == NULL, return)
#if HAVE_MEMALIGN
    /* The function memalign() allocates size bytes and places the address of the 
     * allocated memory in *memptr. The address of the allocated memory will be a 
     * multiple of alignment, which must be a power of two and a multiple of
     * sizeof(void *). */
    if (alignment < 4)
    {
        alignment = 4;
    }
    *buffer = memalign(alignment, size);
#else
    if (alignment != OSAPI_ALIGNMENT_DEFAULT)
    {
        OSAPI_Log_error_p2(OSAPI_LOG_HEAP_INTERNAL_ALLOCATED_FAILED,size,alignment);
        *buffer = NULL;
        return;
    }
    *buffer = malloc(size);
#endif

    if (*buffer == NULL)
    {
        OSAPI_Log_error_p2(OSAPI_LOG_HEAP_INTERNAL_ALLOCATED_FAILED,size,alignment);
        return;
    }

    OSAPI_Memory_zero(*buffer, size);

#if defined(RTI_DARWIN)
    OSAPI_Heap_g_allocatedByteCount += malloc_size(*buffer);
#elif defined(RTI_LINUX)
#if !defined(__ANDROID__)
    OSAPI_Heap_g_allocatedByteCount += malloc_usable_size(*buffer);
#else
    OSAPI_Heap_g_allocatedByteCount += size;
#endif
#else
#error "OSAPI_Heap_allocateBufferInternal must be ported"
#endif
}

/* ----------------------------------------------------------------- */
/* free the previously allocated memory */
void
OSAPI_Heap_freeBuffer(void *buffer)
{
    LOG_PRECONDITION(buffer == NULL,return);
    free(buffer);
}

#endif

/* end of $Id: Heap.c,v 1.3 2005-05-07 01:37:45 ron Exp $ */
