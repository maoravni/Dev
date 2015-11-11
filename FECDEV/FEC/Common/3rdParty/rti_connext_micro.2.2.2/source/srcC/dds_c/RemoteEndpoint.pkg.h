/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef RemoteEndpoint_pkg_h
#define RemoteEndpoint_pkg_h

struct DDS_RemoteParticipantImpl*
NDDS_RemoteEndpoint_find_parent(DDS_DomainParticipant *self,
                   const DDS_BuiltinTopicKey_t *participant_key,
                   const char *participant_name,
                   const DDS_BuiltinTopicKey_t *endpoint_key);

#endif /* RemoteEndpoint_pkg_h */
