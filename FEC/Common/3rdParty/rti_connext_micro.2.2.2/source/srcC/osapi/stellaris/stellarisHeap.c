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

#if OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS

#include "osapi/osapi_heap.h"
#include "osapi/osapi_string.h"

#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_STELLARIS
/* ----------------------------------------------------------------- */
/* allocate the specified size of memory and align it as well */
void
OSAPI_Heap_allocateBufferInternal(char **buffer,
                                 RTI_UINT32 size,
                                 OSAPIAlignment_T alignment)
{
    *buffer = (char *)calloc(1, size);
}

/* ----------------------------------------------------------------- */
/* free the previously allocated memory */
void
OSAPI_Heap_freeBuffer(char *buffer)
{
    free(buffer);
}

/*--------------------------------------------------------------------------*/
/* change the length of the string - truncated if shorter, uninitialized if longer */
RTI_BOOL
OSAPI_Heap_reallocateString(char **stringStoragePointer,
                           RTI_UINT32 stringSize)
{
    char *result = (char *)realloc(*stringStoragePointer, stringSize + 1);

    if (result != NULL)
    {
        result[stringSize] = '\0';
        *stringStoragePointer = result;
    }

    return result != NULL;
}
#endif

/* end of $Id: Heap.c,v 1.3 2005-05-07 01:37:45 ron Exp $ */
