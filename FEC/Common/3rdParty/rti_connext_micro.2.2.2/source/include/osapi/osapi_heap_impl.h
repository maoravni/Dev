/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_heap_impl_h
#define osapi_heap_impl_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif

#include "osapi/osapi_config.h"

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#else
#include <stdlib.h>
#endif

#if OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS
#ifndef INC_FREERTOS_H
#include "FreeRTOS.h"
#endif
#define OSAPI_Heap_allocate(a, b) pvPortMalloc(((size_t)a*b));OSAPI_Heap_g_allocatedByteCount+=a*b
#define OSAPI_Heap_free(a) vPortFree(a)
#elif (OSAPI_PLATFORM == OSAPI_PLATFORM_POSIX)
/* This function has been moved to posixHeap.c
 * #define OSAPI_Heap_allocate(a, b) OSAPI_Heap_allocate_func(a, b)
 */
#define OSAPI_Heap_free(a) free(a)
#define OSAPI_Heap_realloc(ptr_,size_) realloc(ptr_,size_)
#else
#define OSAPI_Heap_allocate(a, b) calloc(a, b);OSAPI_Heap_g_allocatedByteCount+=a*b
#ifdef RTI_CERT
#define OSAPI_Heap_free(a) 
#else
#define OSAPI_Heap_free(a) free(a)
#endif
#ifdef RTI_CERT
#define OSAPI_Heap_realloc(ptr_,size_)
#else
#define OSAPI_Heap_realloc(ptr_,size_) realloc(ptr_,size_)
#endif
#endif

/*--------------------------------------------------------------------------*/
/* This needs to stay a MACRO; not for performance reasons but because
   it is the only way to pass a "type" into the "function"
 */
#define OSAPI_Heap_allocateStructure(destPointer, destType) \
    *(destPointer) = (destType *)OSAPI_Heap_allocate(1, sizeof(destType))
/*--------------------------------------------------------------------------*/
#define OSAPI_Heap_freeStructure(dest) \
    if ((dest)!=NULL) OSAPI_Heap_free(dest)
/*--------------------------------------------------------------------------*/
/* This needs to stay a MACRO; not for performance reasons but because
   it is the only way to pass a "type" into the "function"
 */
#define OSAPI_Heap_allocateArray(destPointer, elementCount, elementType)    \
    *(destPointer) = (elementType *)OSAPI_Heap_allocate(elementCount, \
                     sizeof(elementType))
/*--------------------------------------------------------------------------*/
#define OSAPI_Heap_freeArray(dest) \
    if ((dest)!=NULL) OSAPI_Heap_free(dest)
/*--------------------------------------------------------------------------*/
#define OSAPI_Heap_allocateString(stringStoragePointer, stringSize) \
    *(stringStoragePointer) = (char *)OSAPI_Heap_allocate(stringSize+1, 1)
/*--------------------------------------------------------------------------*/
#define OSAPI_Heap_freeString(stringStorage) \
    if ((stringStorage) != NULL) OSAPI_Heap_free(stringStorage)
/*--------------------------------------------------------------------------*/


OSAPIDllExport void
OSAPI_Heap_allocateBufferInternal(char **buffer,
                                 RTI_UINT32 size,
                                 OSAPIAlignment_T alignment);

#define OSAPI_Heap_allocateBuffer OSAPI_Heap_allocateBufferInternal

#endif /* osapi_heap_impl_h */
