/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef infrastructure_peer_h
#define infrastructure_peer_h

extern void DDS_Duration_to_ntp_time(const struct DDS_Duration_t *self,
                                   struct OSAPI_NtpTime *dst);

extern void DDS_Duration_from_ntp_time(struct DDS_Duration_t *self,
                                     const struct OSAPI_NtpTime *src);

extern DDS_Boolean
DDS_RtpsWellKnownPorts_is_consistent(
                                 const struct DDS_RtpsWellKnownPorts_t *self);

extern DDS_Boolean
DDS_ThreadSettings_is_consistent(const struct DDS_ThreadSettings *self);

#endif /* infrastructure_peer_h */
