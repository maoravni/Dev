/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef domain_peer_h
#define domain_peer_h

extern DDS_ReturnCode_t
NDDS_RemotePublication_remove_internal(DDS_DomainParticipant *const self,
                                       const DDS_BuiltinTopicKey_t *const key,
                                       DDS_Boolean reset_entry);

extern DDS_ReturnCode_t
NDDS_RemoteSubscription_remove_internal(DDS_DomainParticipant *const self,
                                        const DDS_BuiltinTopicKey_t *key,
                                        DDS_Boolean reset_entry);

#endif /* domain_peer_h */
