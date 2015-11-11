/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_sequenceNumber_impl_h
#define reda_sequenceNumber_impl_h



#define REDA_SequenceNumber_setZero(sequenceNumber) \
  (sequenceNumber)->high = 0; \
  (sequenceNumber)->low = 0

#define REDA_SequenceNumber_setMaximum(sequenceNumber) \
  (sequenceNumber)->high = REDA_SEQUENCE_NUMBER_HIGH_MAX; \
  (sequenceNumber)->low = REDA_SEQUENCE_NUMBER_LOW_MAX

#define REDA_SequenceNumber_compare(sequenceNumber1, sequenceNumber2) \
  ((((sequenceNumber1)->high) > ((sequenceNumber2)->high)) ? 1 : \
   ((((sequenceNumber1)->high) < ((sequenceNumber2)->high)) ? -1 : \
    ((((sequenceNumber1)->low) > ((sequenceNumber2)->low)) ? 1 : \
     ((((sequenceNumber1)->low) < ((sequenceNumber2)->low)) ? -1 : 0))))

#define REDA_SequenceNumber_isUnknown(sn_) \
    (((sn_)->high == 0xffffffff) && ((sn_)->low == 0xffffffff))

#define REDA_SequenceNumber_isZero(sn_) \
    (((sn_)->high == 0) && ((sn_)->low == 0))

#define REDA_SequenceNumber_add(answer, sn1, sn2) \
{ \
  (answer)->high = (sn1)->high + (sn2)->high; \
  (answer)->low = (sn1)->low + (sn2)->low; \
  if (((answer)->low < (sn1)->low) || ((answer)->low < (sn2)->low)) { \
      ++(answer)->high; \
  } \
}

#define REDA_SequenceNumber_subtract(answer, sn1, sn2) \
{ \
 (answer)->high  = (sn1)->high - (sn2)->high; \
 (answer)->low = (sn1)->low - (sn2)->low; \
 if ((answer)->low > (sn1)->low) { \
     --(answer)->high; \
  } \
}

#define REDA_SequenceNumber_plusplus(sequenceNumber) \
  ((++(sequenceNumber)->low) == 0) ? ++(sequenceNumber)->high : 0

#define REDA_SequenceNumber_minusminus(sequenceNumber) \
{ \
  RTI_UINT32 originalLow = (sequenceNumber)->low; \
  --(sequenceNumber)->low; \
  if (((sequenceNumber)->low > originalLow)) { --(sequenceNumber)->high; } \
}

#define REDA_SequenceNumber_increment(answer, sequenceNumber) \
{ \
  RTI_UINT32 currentLow = (answer)->low; \
  (answer)->high += (sequenceNumber)->high; \
  (answer)->low += (sequenceNumber)->low; \
  if (((answer)->low < (sequenceNumber)->low) || \
      ((answer)->low < currentLow)) { \
      ++(answer)->high; \
  } \
}

#define REDA_SequenceNumber_decrement(answer, sequenceNumber) \
{ \
  RTI_UINT32 currentLow = (answer)->low; \
  (answer)->high -= (sequenceNumber)->high; \
  (answer)->low -= (sequenceNumber)->low; \
  if (((answer)->low > currentLow)) { --(answer)->high; } \
}

#define REDA_SequenceNumber_max(answer, sn1, sn2) \
{ \
  if (REDA_SequenceNumber_compare((sn1), (sn2)) > 0) { \
    (answer)->high = (sn1)->high; (answer)->low = (sn1)->low; \
  } else { \
    (answer)->high = (sn2)->high; (answer)->low = (sn2)->low; \
  } \
}

#define REDA_SequenceNumber_min(answer, sn1, sn2) \
{ \
  if (REDA_SequenceNumber_compare((sn1), (sn2)) < 0) { \
    (answer)->high = (sn1)->high; (answer)->low = (sn1)->low; \
  } else { \
    (answer)->high = (sn2)->high; (answer)->low = (sn2)->low; \
  } \
}


#endif /* reda_sequenceNumber_impl_h */
