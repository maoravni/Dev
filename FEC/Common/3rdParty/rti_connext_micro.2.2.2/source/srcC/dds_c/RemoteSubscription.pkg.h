/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef RemoteSubscription_pkg_h
#define RemoteSubscription_pkg_h

struct DDS_RemoteSubscriptionImpl
{
    struct NDDS_RemoteEntityImpl as_entity;
    /* The first field in the builtin topic data is the GUID, 
     * which is the key 
     */
    struct DDS_SubscriptionBuiltinTopicData data;
    DDS_BuiltinTopicKey_t orig_key;
};

extern DDS_ReturnCode_t
NDDS_RemoteSubscription_enable(DDS_DomainParticipant *const participant,
                               NDDS_RemoteEntity *entity,
                               const DDS_BuiltinTopicKey_t *key);

extern RTI_INT32
DDS_RemoteSubscriptionImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                                   const DB_Record_T op1, void *op2, void *op3);

extern DDS_ReturnCode_t
NDDS_RemoteSubscription_remove_internal(DDS_DomainParticipant *const self,
                                        const DDS_BuiltinTopicKey_t *key,
                                        DDS_Boolean reset_entry);

#endif /* RemoteSubscription_pkg_h */
