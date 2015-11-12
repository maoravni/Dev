/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_string_h
#define osapi_string_h



#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
    \brief Memory management utilities.
*/

/* \defgroup OSAPI_MemoryClass OSAPI Memory
    \ingroup OSAPIModule

  \brief These are utilities for OS-independent memory manipulation such
  as filling and copying.
 */

/*e \ingroup OSAPI_MemoryClass
  \brief Copies size bytes from non-overlapping src to dest.

  @pre Valid dest and src >= size, where size >= 0.  The src
  and dst memory regions should NOT overlap or the results will
  be undetermined.

  @post size number of bytes from in [dest, dest+size) are identical to
  [src, src+size).

  @param dest \b Out. Pointer to destination memory region.
  @param src \b In. Pointer to source memory region.
  @param size \b In. Number of bytes to copy starting at dest.

  @exception None.

  @mtsafety src cannot be modified concurrently; dest cannot be
  not modified or read concurrently.
*/
OSAPIDllExport void
OSAPI_Memory_copy(void *dest,const void *src, int size);


/*e \ingroup OSAPI_MemoryClass
  \brief Sets memory region to 0.

  @pre Valid memory >= size, where size >= 0.

  @post [mem, mem+size) filled with 0.

  @param mem \b Out. Pointer to memory region to be modified.
  @param size \b In. Number of bytes to fill with zeroes, starting at mem.

  @exception None.

  @mtsafety mem cannot be modified or read concurrently.
*/
OSAPIDllExport void
OSAPI_Memory_zero(void *mem, int size);


/*e \ingroup OSAPI_MemoryClass
  \brief Byte-wise compare of two memory regions.

  @pre Valid left and right >= 0, where size >= 0.

  @post left and right unchanged.

  @param left \b In. Pointer to logical "left" side of the inequality test.
  @param right \b In. Pointer to logical "right" side of the inequality test.
  @param size \b In. Number of bytes to compare, starting from left to right.

  @return less than, equal to, or greater than  0, according to whether left is
  lexicographically less than, equal to, or greater than right when
  taken as unsigned characters.

  @exception none

  @mtsafety neither left nor right can be modified concurrently.
*/
OSAPIDllExport int
OSAPI_Memory_compare(const void *left,const void *right, int size);

/*e \ingroup OSAPI_MemoryClass
  \brief Moves size bytes from non-overlapping src to dest.

  @pre Valid dest and src >= size, where size >= 0.  The src
  and dst memory regions may overlap.

  @post size number of bytes from in [dest, dest+size) are identical to
  [src, src+size).

  @param dest \b Out. Pointer to destination memory region.
  @param src \b In. Pointer to source memory region.
  @param size \b In. Number of bytes to copy starting at dest.

  @exception None.

  @mtsafety src cannot be modified concurrently; dest cannot be
  not modified or read concurrently.
*/
OSAPIDllExport void*
OSAPI_Memory_move(const void *left,const void *right, RTI_SIZE_T size);

/* pick up hidden performance boosting macros and optimizations */
#ifdef __cplusplus
}                               /* extern "C" */
#endif


#include "osapi/osapi_string_impl.h"

#endif /* osapi_string_h */
