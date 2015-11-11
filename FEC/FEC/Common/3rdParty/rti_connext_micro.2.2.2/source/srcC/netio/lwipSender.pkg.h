/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef lwipSender_pkg_h
#define lwipSender_pkg_h

extern RTI_BOOL
NETIOSender_send(struct NETIOSender *me,
                 int *failReason,
                 int *toArray,
                 int toArraySize,
                 const struct REDA_Buffer *buffer, int bufferCount);

extern RTI_BOOL
NETIOSender_queryCapability(struct NETIOSender *me,
                            int *messageSizeMaxOut,
                            int *gatherSendBufferCountMaxOut);

extern RTI_BOOL
NETIOSender_getDestinationId(struct NETIOSender *me,
                             int *destinationIdOut,
                             const RTI_UINT32 * address,
                             const NETIOPort * port);

extern RTI_BOOL
NETIOSender_addDestination(struct NETIOSender *me,
                           int *destinationIdOut,
                           const RTI_UINT32 * address, const NETIOPort * port);

extern void
NETIOSender_removeDestination(struct NETIOSender *me, int destinationId);

extern void
NETIOSender_delete(struct NETIOSender *me);

#endif /* lwipSender_pkg_h */
