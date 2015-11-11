/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_sequenceNumber_h
#define reda_sequenceNumber_h


#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct REDA_Buffer;       /* forward declare to appease compiler */

/* Doxygen file & module documentation ==================================*/
/*e \file
    \brief Defines the REDA_SequenceNumber
*/

/* ----------------------------------------------------------------- */
/*e \defgroup REDA_SequenceNumberClass REDA_SequenceNumber
    \ingroup REDAModule
    \brief The functions in this module support the manipulation of 64-bit
    sequence numbers, including comparison and arithmetic operations.

    The impelmetation des not assume 64-bit processor architecture
    rather relies on 32-bit logic on the "low" and "high" part of the
    sequence number.
*/

/*e \ingroup REDA_SequenceNumberClass
  \brief The structure used to encode a 64-bit signed sequence number
*/
/*e 64-bit signed number. */
struct REDA_SequenceNumber
{
    /*e The most significant part of the sequence number */
    RTI_INT32 high;
    /*e The least significant part of the sequence number */
    RTI_UINT32 low;
};

/*e \ingroup REDA_SequenceNumberClass
  \brief Highest, most positive value for the high part of the seq num
*/
#define REDA_SEQUENCE_NUMBER_HIGH_MAX ((RTI_INT32)0x7fffffff)

/*e \ingroup REDA_SequenceNumberClass
  \brief Highest, most positive value for the low part of the seq num
*/
#define REDA_SEQUENCE_NUMBER_LOW_MAX ((RTI_UINT32)0xffffffff)

/*e \ingroup REDA_SequenceNumberClass
  \brief Highest, most positive value for the 64-bit sequence number
  Intended to be used as:
  \code
  struct REDA_SequenceNumber seqNum = REDA_SEQUENCE_NUMBER_MAX;
  \endcode
*/
#define REDA_SEQUENCE_NUMBER_MAX {REDA_SEQUENCE_NUMBER_HIGH_MAX, \
                                  REDA_SEQUENCE_NUMBER_LOW_MAX}

/*e \ingroup REDA_SequenceNumberClass
  \brief Zero value for the 64-bit sequence number
  Intended to be used as:
  \code
  struct REDA_SequenceNumber seqNum = REDA_SEQUENCE_NUMBER_ZERO;
  \endcode
*/
#define REDA_SEQUENCE_NUMBER_ZERO {0, 0}

/*e \ingroup REDA_SequenceNumberClass
  Unknown value (-1) for the 64-bit sequence number
  Intended to be used as:
  \code
  struct REDA_SequenceNumber seqNum = REDA_SEQUENCE_NUMBER_UNKNOWN;
  \endcode
*/
#define REDA_SEQUENCE_NUMBER_UNKNOWN \
  {(RTI_INT32)0xffffffff, (RTI_UINT32)0xffffffff}

/*e \ingroup REDA_SequenceNumberClass
  \brief Reset a sequence number to zero.
  It is safer and more portable than setting the individial fields.

  @post sequenceNumber set to REDA_SEQUENCE_NUMBER_ZERO.

  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_setZero(struct REDA_SequenceNumber *me);

/*e \ingroup REDA_SequenceNumberClass
  \brief Set a sequence number to the maximum value.
  It is safer and more portable than setting the individial fields.

  @post sequenceNumber set to REDA_SEQUENCE_NUMBER_MAX.
  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_setMaximum(struct REDA_SequenceNumber *me);

/*e \ingroup REDA_SequenceNumberClass
  \brief Compare two sequence numbers.

  @pre Both sequence numbers are valid.
  @post Both sequence numbers remain unchanged.

  @param sn \b In. Second sequence number to compare.

  @return
  <UL>
  <LI>If this > sn then return 1.
  <LI>If this < sn then return -1.
  <LI>If this == sn then return 0.
  </UL>

  @mtsafety oneThreadAtATime
*/
REDADllExport RTI_INT32
REDA_SequenceNumber_compare(const struct REDA_SequenceNumber *me,
                           const struct REDA_SequenceNumber *sn);

/*e \ingroup REDA_SequenceNumberClass
  \brief Whether a sequence number is unknown.

  @param me \b In. Sequence number to check.

  @return
  <UL>
  <LI>If sn is unknown, then return RTI_TRUE.
  </UL>

  @mtsafety oneThreadAtATime
*/
REDADllExport RTI_BOOL
REDA_SequenceNumber_isUnknown(const struct REDA_SequenceNumber *me);

/*e \ingroup REDA_SequenceNumberClass
  \brief Whether a sequence number is equal to zero.

  @param me \b In. Sequence number to check.

  @return
  <UL>
  <LI>If sn is zero, then return RTI_TRUE.
  </UL>

  @mtsafety oneThreadAtATime
*/
REDADllExport RTI_BOOL
REDA_SequenceNumber_isZero(const struct REDA_SequenceNumber *me);


/*e \ingroup REDA_SequenceNumberClass
  \brief Add two sequence numbers.

  @post answer contains sn1 + sn2.  sn1 and sn2 remain the same.

  @param answer \b Out. Sequence number to hold the sum.
  @param sn1 \b In. First sequence number to add.
  @param sn2 \b In. Second sequence number to add.

  @mtsafety ontThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_add(struct REDA_SequenceNumber *answer,
                       const struct REDA_SequenceNumber *sn1,
                       const struct REDA_SequenceNumber *sn2);

/*e \ingroup REDA_SequenceNumberClass
  \brief Subtract two sequence numbers.

  @post answer contains sn1 - sn2.  sn1 and sn2 remain the same.

  @param answer \b Out. Sequence number to hold the difference.
  @param sn1 \b In. number to subtract from.
  @param sn2 \b In. number to subtract.

  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_subtract(struct REDA_SequenceNumber *answer,
                            const struct REDA_SequenceNumber *sn1,
                            const struct REDA_SequenceNumber *sn2);

/*e \ingroup REDA_SequenceNumberClass
  \brief Increment a sequence number by one.

  @post this incremented by 1.

  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_plusplus(struct REDA_SequenceNumber *me);

/*e \ingroup REDA_SequenceNumberClass
  \brief Decrement a sequence number by one.

  @post sequenceNumber decremented by 1.
  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_minusminus(struct REDA_SequenceNumber *me);


/*e \ingroup REDA_SequenceNumberClass
  \brief Increment a sequence number by another sequence number.

  @post answer is incremented by sn.

  @param sn \b In. Sequence number to increment by.
  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_increment(struct REDA_SequenceNumber *me,
                             const struct REDA_SequenceNumber *sn);

/*e \ingroup REDA_SequenceNumberClass
  \brief Decrement a sequence number by another sequence number.

  @post answer is decremented by sn.

  @param sn \b In. Sequence number to decrement by.
  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_decrement(struct REDA_SequenceNumber *me,
                             const struct REDA_SequenceNumber *sn);

/*e \ingroup REDA_SequenceNumberClass
  \brief Get the max of two sequence numbers.

  @post answer is set to the max of the two sequence numbers.

  @param answer \b InOut. Max sequence number of the two.
  @param sn1 \b In. First sequence number.
  @param sn2 \b In. Second sequence number.
  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_max(struct REDA_SequenceNumber *answer,
                       const struct REDA_SequenceNumber *sn1,
                       const struct REDA_SequenceNumber *sn2);

/*e \ingroup REDA_SequenceNumberClass
  \brief Get the min of two sequence numbers.

  @post answer is set to the min of the two sequence numbers.

  @param answer \b InOut. Min sequence number of the two.
  @param sn1 \b In. First sequence number.
  @param sn2 \b In. Second sequence number.
  @mtsafety oneThreadAtATime
*/
REDADllExport void
REDA_SequenceNumber_min(struct REDA_SequenceNumber *answer,
                       const struct REDA_SequenceNumber *sn1,
                       const struct REDA_SequenceNumber *sn2);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

/* pick up hidden performance boosting macros and optimizations */
#include "reda/reda_sequenceNumber_impl.h"

#endif /* reda_sequenceNumber_h */
