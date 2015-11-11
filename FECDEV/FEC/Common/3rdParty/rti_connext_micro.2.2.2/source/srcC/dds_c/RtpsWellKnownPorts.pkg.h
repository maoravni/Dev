/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef RtpsWellKnownPorts_pkg_h
#define RtpsWellKnownPorts_pkg_h

extern DDS_Boolean
DDS_RtpsWellKnownPorts_is_consistent(
                                 const struct DDS_RtpsWellKnownPorts_t *self);

extern DDS_Long
DDS_RtpsWellKnownPorts_get_max_participant_index(
                    const struct DDS_RtpsWellKnownPorts_t *rtpsWellKnownPorts);

#endif /* RtpsWellKnownPorts_pkg_h */
