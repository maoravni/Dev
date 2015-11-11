/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_log_h
#include "dds_c/dds_c_log.h"
#endif

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

/*** SOURCE_BEGIN ***/

void
DDS_EntityImpl_initialize(struct DDS_EntityImpl *entity,
        DDS_EntityKind_t kind,
        DDS_Long entity_id,
        RTIDDS_EntityEnableFunction enable,
        RTIDDS_EntityGetInstanceHandleFunction get_instance_handle)
{
    entity->enable = enable;
    entity->get_instance_handle = get_instance_handle;
    entity->kind = kind;
    entity->entity_id = entity_id;
    entity->state = RTIDDS_ENTITY_STATE_CREATED;
}

void
DDS_EntityImpl_finalize(struct DDS_EntityImpl *entity)
{
}

/*******************************************************************************
 *
 *                            Public API
 *
 ******************************************************************************/
DDS_ReturnCode_t
DDS_Entity_enable(DDS_Entity *self)
{
    struct DDS_EntityImpl *entity = (struct DDS_EntityImpl *)self;

    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    if (entity->state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        return DDS_RETCODE_OK;
    }

    return entity->enable(self);
}

DDS_Boolean
DDS_Entity_is_enabled(DDS_Entity *self)
{
    struct DDS_EntityImpl *entity = (struct DDS_EntityImpl *)self;

    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    return (entity->state == RTIDDS_ENTITY_STATE_ENABLED);
}

DDS_InstanceHandle_t
DDS_Entity_get_instance_handle(DDS_Entity *self)
{
    struct DDS_EntityImpl *entity = (struct DDS_EntityImpl *)self;
    DDS_InstanceHandle_t retval = DDS_HANDLE_NIL;

    if (self != NULL)
    {
        retval = entity->get_instance_handle(self);
    }

    return retval;
}

DDS_EntityKind_t
DDS_Entity_get_entity_kind(DDS_Entity *self)
{
    struct DDS_EntityImpl *entity = (struct DDS_EntityImpl *)self;

    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    return entity->kind;
}
