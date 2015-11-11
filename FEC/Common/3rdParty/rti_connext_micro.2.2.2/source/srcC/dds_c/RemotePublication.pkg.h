/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef RemotePublication_pkg_h
#define RemotePublication_pkg_h

struct DDS_RemotePublicationImpl
{
    struct NDDS_RemoteEntityImpl as_entity;
    struct DDS_PublicationBuiltinTopicData data;
    DDS_UnsignedLong liveliness_count;
    DDS_BuiltinTopicKey_t orig_key;
};

extern DDS_ReturnCode_t
NDDS_RemotePublication_enable(DDS_DomainParticipant *const participant,
                              NDDS_RemoteEntity *entity,
                              const DDS_BuiltinTopicKey_t *new_key);

extern RTI_INT32
DDS_RemotePublicationImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                                  const DB_Record_T op1, void *op2, void *op3);

extern DDS_Boolean
NDDS_RemotePublication_remove_entry(DDS_DomainParticipant *const self,
                        struct DDS_RemoteParticipantImpl *remote_participant,
                        struct DDS_RemotePublicationImpl *rem_publication,
                        DDS_Boolean reset_entry);

extern DDS_ReturnCode_t
NDDS_RemotePublication_remove_internal(DDS_DomainParticipant *const self,
                                       const DDS_BuiltinTopicKey_t *const key,
                                       DDS_Boolean reset_entry);

#endif /* RemotePublication_pkg_h */
