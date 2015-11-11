/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef RemoteParticipant_pkg_h
#define RemoteParticipant_pkg_h

struct DDS_RemoteParticipantImpl
{
    struct NDDS_RemoteEntityImpl as_entity;
    struct DDS_ParticipantBuiltinTopicData data;
    OSAPITimeoutHandle_t lease_duration_event;
    OSAPI_Timer_t timer;
    struct OSAPI_NtpTime lease_duration;
    DDS_BuiltinTopicKey_t orig_key;
};

extern RTI_INT32
DDS_RemoteParticipantImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                           const DB_Record_T op1, void *op2, void *op3);

struct DDS_RemoteParticipantImpl*
NDDS_DomainParticipant_lookup_name(DDS_DomainParticipant *const participant,
                                   const char *name);

extern RTI_BOOL
NDDS_RemoteParticipantRecord_finalize(const void *record, void *param);

#endif /* RemoteParticipant_pkg_h */
