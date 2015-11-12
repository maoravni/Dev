/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_epoch_impl_h
#define reda_epoch_impl_h


/* (((value) - (me)) >= REDA_EPOCH_HALF_VALUE) */
#define REDA_Epoch_isGreater(me, value) (me > value)

/* (((me) - (value)) >= REDA_EPOCH_HALF_VALUE) */
#define REDA_Epoch_isLess(me, value) (me < value)


#endif /* reda_epoch_impl_h */
