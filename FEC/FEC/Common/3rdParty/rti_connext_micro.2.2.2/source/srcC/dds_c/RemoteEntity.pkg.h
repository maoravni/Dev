/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef RemoteEntity_pkg_h
#define RemoteEntity_pkg_h

typedef DDS_ReturnCode_t
(*NDDS_RemoteEntityEnableFunction)(DDS_DomainParticipant *const participant,
                                   NDDS_RemoteEntity *self,
                                   const DDS_BuiltinTopicKey_t *new_key);

struct NDDS_RemoteEntityImpl
{
    NDDS_RemoteEntityEnableFunction enable_func;
    RTIDDS_EntityState state;
    DDS_EntityKind_t kind;
    const struct NDDS_Discovery_Plugin *plugin;
};

extern DDS_Boolean
NDDS_RemoteEntity_is_enabled(struct NDDS_RemoteEntityImpl *entity);

#endif /* RemoteEntity_pkg_h */
