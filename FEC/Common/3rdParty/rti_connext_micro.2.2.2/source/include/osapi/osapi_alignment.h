/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_alignment_h
#define osapi_alignment_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

/*e \file
  \brief Memory alignment utility functions
*/

/*e \defgroup OSAPIAlignmentClass OSAPI Alignment
    \ingroup OSAPIModule
  \brief Helps with architecture-specific alignment

  Note that alignment is necessary for the following reasons:
  <OL>
  <LI>Required alignment for certain data types: on certain architectures
      if certain types are not aligned correctly, there will be a bus-
      error; this is fatal.
      Typically the required alignment is 4 (int), 8 (double or
      void* on 64-bit architectures)
  <LI>Suggested alignment for certain types: on certain architectures,
      using data types that are not properly aligned will lead to a
      large drop in performance (because e.g., several bus-accesses are
      needed)
  <LI>Compilers/architectures will pack and align C-structures in a way
      that solves problems 1 and 2 for all C-structures.  The alignment
      is reflected in the behaviour of malloc()/calloc() and sizeof().
      The precise behaviour is complicated by the fact that many compilers
      can be coaxed into non-default behaviour through command-line
      options or instructions in the source (e.g., \#pragma pack).
      The code in this module will only function correctly if all source
      is compiled with the same alignment.
  <LI>For certain applications, it makes sense to further align data
      to achieve better performance.  Common use cases are:
      <UL>
        <LI>Alignment on a cache-line boundary
        <LI>Alignment on a page-boundary
        <LI>Alignment for I/O access
      </UL>
  </OL>

*/

/*e \ingroup OSAPIAlignmentClass
  @brief The OSAPIAlignment is the alignment in bytes; an address
  is aligned when it is a positive integer multiple of the alignment
*/
typedef RTI_INT32 OSAPIAlignment_T;


/*e \ingroup OSAPIAlignmentClass
  @brief Certain methods allow a default alignment: this should
  be an alignment that follows the "malloc" alignment of
  the architecture (aligned sufficiently to store any C-structure
  efficiently).
*/
#define OSAPI_ALIGNMENT_DEFAULT (-1)


#endif /* osapi_alignment_h */
