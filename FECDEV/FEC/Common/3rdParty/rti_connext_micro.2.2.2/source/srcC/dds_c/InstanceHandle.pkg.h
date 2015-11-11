/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef InstanceHandle_pkg_h
#define InstanceHandle_pkg_h

extern void
DDS_InstanceHandle_to_rtps(struct MIGRtpsGuid *other,
                           const DDS_InstanceHandle_t * self);

extern void
DDS_InstanceHandle_from_rtps(DDS_InstanceHandle_t * self,
                             const struct MIGRtpsGuid *other);

#endif /* InstanceHandle_pkg_h */
