/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef Entity_pkg_h
#define Entity_pkg_h

typedef DDS_ReturnCode_t
(*RTIDDS_EntityEnableFunction)(DDS_Entity *self);

typedef DDS_InstanceHandle_t
(*RTIDDS_EntityGetInstanceHandleFunction)(DDS_Entity *self);

typedef DDS_Long
(*RTIDDS_ObjectIdGenerator)(void);

typedef enum
{
    RTIDDS_ENTITY_STATE_CREATED,
    RTIDDS_ENTITY_STATE_ENABLED,
    RTIDDS_ENTITY_STATE_REMOTE_CREATED,
    RTIDDS_ENTITY_STATE_REMOTE_ENABLED
} RTIDDS_EntityState;

struct DDS_EntityImpl
{
    DDS_Long entity_id;
    RTIDDS_EntityEnableFunction enable;
    RTIDDS_EntityGetInstanceHandleFunction get_instance_handle;
    RTIDDS_EntityState state;
    DDS_EntityKind_t kind;
};

extern void
DDS_EntityImpl_initialize(struct DDS_EntityImpl *entity,
        DDS_EntityKind_t kind,
        DDS_Long entity_id,
        RTIDDS_EntityEnableFunction enable,
        RTIDDS_EntityGetInstanceHandleFunction get_instance_handle);

extern void
DDS_EntityImpl_finalize(struct DDS_EntityImpl *entity);

#endif /* Entity_pkg_h */
