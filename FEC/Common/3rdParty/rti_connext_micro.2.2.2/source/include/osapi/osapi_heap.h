/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_heap_h
#define osapi_heap_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_alignment_h
#include "osapi/osapi_alignment.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define TYPE RTI_UINT32

/*e \file
 *
 * \brief Heap utilities
*/

/*e \defgroup OSAPI_HeapClass OSAPI Heap
 *  \ingroup OSAPIModule
 */

 /*e \ingroup OSAPI_HeapClass
 * \brief Allocates zero-initialized memory from the heap.
 *
 * \details
 *
 * The function allocates `count` elements of size `size`
 * and sets the memory region to 0.
 *
 * \param[in] count The number of elements to allocate.
 *
 * \param[in] size  The size of the element.
 *
 * \return. A pointer to a contiguous memory area guaranteed to be large
 * enough store `count` elements of size `size`.
 *
 * \sa \ref OSAPIPortingClass, \ref OSAPI_Heap_free
 */
OSAPIDllExport void*
OSAPI_Heap_allocate(RTI_INT32 count,RTI_INT32 size);

/*e \ingroup OSAPI_HeapClass
 *
 * \brief Frees memory allocated from the heap.
 *
 * \details
 *
 * The function frees memory previously allocated with \ref OSAPI_Heap_allocate
 * back to the heap.
 *
 * \param[in] ptr Pointer to region previous allocated
 *                 with \ref OSAPI_Heap_allocate
 *
 *
 * \sa \ref OSAPIPortingClass, \ref OSAPI_Heap_allocate
 */
OSAPIDllExport void
OSAPI_Heap_free(void *ptr);

/*e \ingroup OSAPI_HeapClass
    \brief Allocates space on the heap for a C structure and
    initializes the structure with 0's.

    Example:
    \code
    struct MyStruct *myStruct;
    OSAPI_Heap_allocateStructure(&myStruct, struct MyStruct);
    if (myStruct==NULL) {
        return;
    }
    \endcode

    The returned space must be freed with \ref OSAPI_Heap_freeStructure.
*/
OSAPIDllExport void
OSAPI_Heap_allocateStructure(TYPE ** structStoragePointer, TYPE);


/*e \ingroup OSAPI_HeapClass
    \brief Returns previously allocated
    (with \ref OSAPI_Heap_allocateStructure) space to the heap.

    @param [in] structStorage If NULL, no op.

    Example:
    \code
    char *c;
    OSAPI_Heap_allocateStructure(&c, char);
    OSAPI_Heap_freeStructure(c);
    \endcode
 */
OSAPIDllExport void
OSAPI_Heap_freeStructure(TYPE * structStorage);


/*e \ingroup OSAPI_HeapClass
    \brief A static method to allocate space on the heap for an array of C
    structures; the array is initialized with 0's.

    Example:
    \code
    struct MyElement *array;
    OSAPI_Heap_allocateStructure(&array, 7, struct MyElement);
    if (array==NULL) {
        return;
    }
    // elements of the array can be accessed as array[i]
    \endcode

    The returned space must be freed with \ref OSAPI_Heap_freeArray.
*/
OSAPIDllExport void
OSAPI_Heap_allocateArray(TYPE ** arrayStoragePointer, int elementCount,TYPE);


/*e \ingroup OSAPI_HeapClass
    \brief A static method to return space (that was previously
    allocated with \ref OSAPI_Heap_allocateArray) to the heap.

    Example:
    \code
    char *c;
    OSAPI_Heap_allocateArray(&c, 1, char);
    OSAPI_Heap_freeArray(c);
    \endcode

    @param arrayStorage \b In.  If NULL, no op.
 */
OSAPIDllExport void
OSAPI_Heap_freeArray(TYPE * arrayStorage);


/*e \ingroup OSAPI_HeapClass
    \brief A static method to allocate space on the heap for a string of up to
    a given size. The string is initialized with 0's.

    Example:
    \code
    char* myString;
    OSAPI_Heap_allocateString(&myString, 128);
    \endcode

    The returned space must be freed with \ref OSAPI_Heap_freeString.
 */
OSAPIDllExport void
OSAPI_Heap_allocateString(char **stringStoragePointer,RTI_UINT32 stringSize);

/*e \ingroup OSAPI_HeapClass
    \brief A static method to reallocate the space on the heap
    to hold a string of 'stringSize'.

    @pre *stringStoragePointer was allocated using OSAPI_Heap_allocateString()

    @return RTI_TRUE upon success; RTI_FALSE upon failure.

    @post Upon success, *stringStoragePointer contains a pointer
    to an area that can accomodate a string of size 'stringSize'
    (also the return value). Upon failure, leaves
    *stringStoragePointer unchanged.

    The returned space must be freed with \ref OSAPI_Heap_freeString.

    Example:
    \code
      char* myString = OSAPI_Heap_allocateString(32);
      OSAPI_Heap_reallocateString(&myString, 128);
    \endcode
 */
OSAPIDllExport RTI_BOOL
OSAPI_Heap_reallocateString(char **stringStoragePointer, RTI_UINT32 stringSize);

/*e \ingroup OSAPI_HeapClass
    \brief A static method to return space (that was previously
    allocated with \ref OSAPI_Heap_allocateString or
    \ref OSAPI_Heap_reallocateString) to the heap. 

    @param stringStorage \b In. If NULL, no op.
 */
OSAPIDllExport void
OSAPI_Heap_freeString(char *stringStorage);

/*e \ingroup OSAPI_HeapClass
  \brief A static method to allocate a block of memory of the provided 
  size from the heap.

  @post The returned address will be a multiple of 2^alignment; its
  memory will be initialized with 0's.

  @param buffer \b out
  @param size \b in. > 0
  @param alignment \b In. Power of 2 and >0 or OSAPI_ALIGNMENT_DEFAULT.
  If the alignment is OSAPI_ALIGNMENT_DEFAULT,
  the returned pointer will have "default alignment" (meaning that any 
  C-structure can start at the beginning of the buffer).

  The block must be returned to the heap with \ref OSAPI_Heap_freeBuffer.
*/
OSAPIDllExport char*
OSAPI_Heap_allocateBuffer(char **buffer,
                         RTI_UINT32 size,
                         OSAPIAlignment_T alignment);

/*e \ingroup OSAPI_HeapClass
  \brief A static method to return a block (that was previously
  allocated with \ref OSAPI_Heap_allocateBuffer) to the heap.

  @param buffer \b in
*/
OSAPIDllExport void
OSAPI_Heap_freeBuffer(void *buffer);

/*e \ingroup OSAPI_HeapClass
  \brief Return current allocated memory

  \mtsafe UNSAFE
*/
OSAPIDllVariable extern RTI_SIZE_T OSAPI_Heap_g_allocatedByteCount;

OSAPIDllExport RTI_SIZE_T
OSAPI_Heap_getAllocatedByteCount(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#include "osapi/osapi_heap_impl.h"

#endif /* osapi_heap_h */
