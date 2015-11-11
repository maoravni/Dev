/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_buffer_h
#define reda_buffer_h


#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
    \brief Defines the REDA_Buffer interface
*/

/*e \defgroup REDA_BufferClass REDA_Buffer
    \ingroup REDAModule
    \brief REDA buffers are equivalent to iovec buffers supported by the
    sendmsg() socket API, or the WSABUF array used to do gather send on
    Windows.

*/

/*e \ingroup REDA_BufferClass
  \brief Generic buffer to hold data, described by a pointer to the data, as
  well as the length of the data.

  This structure deliberately mirrors
  windows WSABUF so we can cast the passed in structure directly without
  copying the fields into a strucuture on the stack.

  @see REDA_BufferArray_getSize
*/
struct REDA_Buffer
{
    /*e Length of the buffer pointed to by pointer. */
    RTI_UINT32 length;
    /*e Pre-allocated (by the caller) buffer.  */
    char *pointer;
};


/*e \ingroup REDA_BufferClass
  \brief static method to calculate the total data length in a gather buffer

  @todo Needs to be renamed
  Calculates the total size of an array of REDA_Buffer elements.

  @pre valid answer, array, and arrayCount.

  @param answer \b Out. Pointer to the integer sum of the lengths of all the
  buffers.
  @param array \b In. Iovec array, that is array of REDA_Buffer elements
  @param arrayCount \b In. Number of elements in the array.
*/
REDADllExport void
REDA_BufferArray_getSize(RTI_UINT32 *answer,
                        const struct REDA_Buffer *array,
                        RTI_UINT32 arrayCount);

/*e \ingroup REDA_BufferClass
  \brief Set the elements of a REDA_Buffer.
  It is safer and more portable than setting the individial fields.

  @pre buf answer and array.

  @param pointer \b In.  Underlying memory for the buffer
  @param length \n In.  The length of the underlying memory for the buffer
*/
REDADllExport void
REDA_Buffer_set(struct REDA_Buffer *me,char *pointer,RTI_UINT32 length);

REDADllExport void
REDA_Buffer_get(struct REDA_Buffer *me,char **pointer, RTI_UINT32 *length);

/*e \ingroup REDA_BufferClass
  \brief Reset the elements of a REDA_Buffer.
  It is safer and more portable than setting the individial fields.

  @pre Valid buffer.
  @post buffer set to REDA_BUFFER_INVALID
*/
REDADllExport void
REDA_Buffer_reset(struct REDA_Buffer *me);


/*e \ingroup REDA_BufferClass
  \brief An invalid buffer, useful for initialization as indicated by
  the usage:
  \code
  struct REDA_Buffer buf = REDA_BUFFER_INVALID;
  \endcode
*/
#define REDA_BUFFER_INVALID {0, NULL}


/*e \ingroup REDA_BufferClass
  \brief Convert to string for printing during log messages and diagnostics.

  @pre pre-allocated buffer big enough to hold the answer and valid
  ptr. The function should verify this and either fail if there is not
  enough space, or else truncate the output.

  @post On success, the buffer holds the string and the length field
  of the buffer is the numbers of characters (including the
  terminating '\0'). The length field will never exceed the one passed
  in. If fewer bytes are written than the available then it will be
  shortened to reflect the true length of the string.  On failure, the
  REDA_Buffer is left unchanged.

  @param ptr \b. In. "this" The object to be stringified.

  @param buffer \b InOut. Pointer to a single REDA_Buffer that
  satisfies the pre-condition. The function will use this to determine
  where to write and the maximum number of bytes to write. REDA_Buffer.

  @exception RTI_LOG_PRECONDITION_FAILURE

  @return On success it returns the pointer to the character array
  underlying the REDA_Buffer \e buffer. This character array should
  contain the string representation of \e ptr. On failure the function
  should return NULL.

  @mtsafety should be implemented as a re-entrant function which will
  be automatically thread-safe provided that it is not invoked
  concurrently on the same buffer.
*/
typedef char *(*REDA_toStringBufferFunc) (const void *ptr,
                                          struct REDA_Buffer * buffer);

/* pick up hidden performance boosting macros and optimizations */

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#include "reda/reda_buffer_impl.h"

#endif /* reda_buffer_h */
