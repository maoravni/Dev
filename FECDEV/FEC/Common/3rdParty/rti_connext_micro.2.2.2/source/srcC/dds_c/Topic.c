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
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "Entity.pkg.h"
#include "TopicDescription.pkg.h"
#include "TopicQos.pkg.h"
#include "Type.pkg.h"

/*******************************************************************************
 *                              Internal/Peer API
 ******************************************************************************/
typedef void
(*DomainParticipantListener_on_inconsistent_topic)(
        DDS_DomainParticipant *participant,
        DDS_Topic *topic,
        const struct DDS_InconsistentTopicStatus *status);

struct NDDS_TopicConfig
{
    DDS_Boolean is_anonymous;
    DB_Database_T db;
    RTIDDS_EntityGetInstanceHandleFunction get_parent_handle;
    DomainParticipantListener_on_inconsistent_topic on_inconsistent_topic;
};


#define NDDS_TopicConfig_INITIALIZER \
{\
     DDS_BOOLEAN_FALSE,\
     NULL,\
     0\
}


struct DDS_TopicImpl
{
    struct DDS_EntityImpl as_entity;
    char topic_name[MIG_RTPS_PATHNAME_LEN_MAX];
    struct DDS_TopicDescriptionImpl as_topicdescription;
    struct DDS_TopicQos qos;
    struct DDS_TopicListener listener;
    DDS_Type *type;
    DDS_StatusMask mask;
    struct DDS_InconsistentTopicStatus inconsistent_status;
    struct NDDS_TopicConfig config;
    DDS_DomainParticipant *participant;
};

/*** SOURCE_BEGIN ***/

RTI_INT32
DDS_TopicImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                     const DB_Record_T op1, void *op2, void *op3)
{
    struct DDS_TopicImpl *record_left = (struct DDS_TopicImpl*)op1;
    const char *name_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        name_right = (const char*)op2;
    }
    else
    {
        name_right = ((struct DDS_TopicImpl*)op2)->topic_name;
    }

    return strcmp(record_left->topic_name,name_right);
}

DDS_Boolean
DDS_TopicImpl_is_hidden(DDS_Topic *self)
{
    return self->qos.management.is_hidden;
}

DDS_Boolean
DDS_Topic_listener_is_consistent(const struct DDS_TopicListener *self,
                                 DDS_StatusMask mask)
{
    if ((self == NULL) && (mask != DDS_STATUS_MASK_NONE))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if ((mask & DDS_INCONSISTENT_TOPIC_STATUS) &&
        (self->on_inconsistent_topic == NULL))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_TopicImpl_finalize(struct DDS_TopicImpl *topic)
{
    DDS_EntityImpl_finalize(&topic->as_entity);
    DDS_TopicDescriptionImpl_finalize(&topic->as_topicdescription);
    DDS_TypeImpl_ref_minus(topic->type);

    return DDS_BOOLEAN_TRUE;
}

DDS_InstanceHandle_t
DDS_TopicImpl_get_instance_handle(DDS_Entity *entity)
{
    struct DDS_TopicImpl *topic = (struct DDS_TopicImpl*)entity;
    DDS_InstanceHandle_t retval;

    retval = topic->config.get_parent_handle((DDS_Entity*)topic->participant);
    *((DDS_Long*)&retval.octet[12]) = NETIO_htonl(topic->as_entity.entity_id);

    return retval;
}

DDS_Boolean
DDS_TopicImpl_initialize(struct DDS_TopicImpl *topic,
                         DDS_DomainParticipant *participant,
                         const char *topic_name,
                         DDS_Type *type,
                         const struct DDS_TopicQos *qos,
                         const struct DDS_TopicListener *listener,
                         DDS_StatusMask mask,
                         DDS_Long object_id,
                         struct NDDS_TopicConfig *config)
{
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;

    if (!DDS_Topic_listener_is_consistent(listener,mask))
    {
        goto done;
    }

    OSAPI_Memory_copy((void *)topic->topic_name, (const void *)topic_name,
                     DDS_String_length(topic_name) + 1);

    DDS_EntityImpl_initialize(&topic->as_entity,
                          DDS_TOPIC_ENTITY_KIND,
                          object_id,
                          DDS_Topic_enable,
                          DDS_TopicImpl_get_instance_handle);

    DDS_TopicDescriptionImpl_initialize(&topic->as_topicdescription,
                        (const char *)&topic->topic_name,
                        DDS_TypeImpl_get_type_name_reference(type),
                        topic);

    DDS_TypeImpl_ref_plus(type);
    topic->config = *config;
    topic->type = type;
    topic->qos = *qos;
    topic->inconsistent_status.total_count = 0;
    topic->inconsistent_status.total_count_change = 0;
    topic->mask = mask;
    topic->participant = participant;


    if (listener)
    {
        topic->listener = *listener;
    }

    retval = DDS_BOOLEAN_TRUE;

done:
    return retval;
}

DDS_ReturnCode_t
DDS_Topic_enable(DDS_Entity *self)
{
    struct DDS_TopicImpl *topic = (struct DDS_TopicImpl*)self;

    if (topic->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        return DDS_RETCODE_OK;
    }

    topic->as_entity.state = RTIDDS_ENTITY_STATE_ENABLED;

    return DDS_RETCODE_OK;
}

#ifdef ENABLE_STATUS_LISTENER
void
DDS_Topic_on_inconsistent_topic(DDS_Topic *self)
{
    struct DDS_TopicImpl *topic = (struct DDS_TopicImpl *)self;

    ++topic->inconsistent_status.total_count;
    topic->inconsistent_status.total_count_change = 1;

    if (topic->mask & DDS_INCONSISTENT_TOPIC_STATUS)
    {
        topic->listener.on_inconsistent_topic(topic->listener.as_listener.
                                              listener_data, topic,
                                              &topic->inconsistent_status);
    }
    else
    {
        topic->config.on_inconsistent_topic(topic->participant,
                                            topic,
                                            &topic->inconsistent_status);
    }
}
#endif

/*******************************************************************************
 *                                Public API
 ******************************************************************************/
DDS_Entity*
DDS_Topic_as_entity(DDS_Topic *self)
{
    struct DDS_TopicImpl *topic = (struct DDS_TopicImpl*)self;

    LOG_PRECONDITION((topic == NULL),return NULL)

    return &topic->as_entity;
}

DDS_TopicDescription*
DDS_Topic_as_topicdescription(DDS_Topic *self)
{
    struct DDS_TopicImpl *topic = (struct DDS_TopicImpl*)self;

    LOG_PRECONDITION((topic == NULL),return NULL)

    return &topic->as_topicdescription;
}

DDS_Topic*
DDS_Topic_narrow(DDS_TopicDescription *self)
{
    LOG_PRECONDITION((self == NULL),return NULL)

    return (DDS_Topic*)self->owner;
}

#ifdef INCLUDE_API_LISTENER 
DDS_ReturnCode_t
DDS_Topic_set_listener(DDS_Topic *self,
                       const struct DDS_TopicListener *listener,
                       DDS_StatusMask mask)
{
    struct DDS_TopicImpl *topic = (struct DDS_TopicImpl*)self;

    LOG_PRECONDITION((topic == NULL),return DDS_RETCODE_BAD_PARAMETER)

    if (!DDS_Topic_listener_is_consistent(listener, mask))
    {
        DDSC_Log_error(LOG_INCONSISTENT_TOPIC_LISTENER);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    DB_Database_lock(topic->config.db);

    if (listener)
    {
        topic->listener = *listener;
    }
    topic->mask = mask;

    DB_Database_unlock(topic->config.db);

    return DDS_RETCODE_OK;
}
#endif

DDS_Boolean
DDS_Topic_is_compatible(DDS_Topic *left,
                        const char *right_topic_name,
                        const char *right_type_name)
{
    const char *left_topic_name;
    const char *left_type_name;
    struct DDS_InconsistentTopicStatus status;

    left_topic_name =
        DDS_TopicDescription_get_name(DDS_Topic_as_topicdescription(left));

    left_type_name =
        DDS_TopicDescription_get_type_name(DDS_Topic_as_topicdescription(left));

    if (left_topic_name == NULL || right_topic_name == NULL)
    {
        DDSC_Log_error(LOG_ATTEMPT_TO_MATCH_NULL_TOPIC);
        return DDS_BOOLEAN_FALSE;
    }
    if (left_type_name == NULL || right_type_name == NULL)
    {
        DDSC_Log_error(LOG_ATTEMPT_TO_MATCH_NULL_TYPE);
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_String_cmp(left_topic_name, right_topic_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_String_cmp(left_type_name, right_type_name))
    {
        DDSC_LOG_DEBUG(LOG_TOPICS_SAME_BUT_TYPES_DIFFERENT);
        if (left->listener.on_inconsistent_topic)
        {
            left->listener.on_inconsistent_topic(
                    left->listener.as_listener.listener_data,
                    left,
                    &status);
        }
        return DDS_BOOLEAN_FALSE;
    }
    return DDS_BOOLEAN_TRUE;
}

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Topic_get_qos(DDS_Topic *self, struct DDS_TopicQos *qos)
{
    struct DDS_TopicImpl *topic = (struct DDS_TopicImpl*)self;

    LOG_PRECONDITION_P2(((self == NULL) || (qos == NULL)),
                        return DDS_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(self),
                        OSAPI_LOG_PTR_LOW(qos))

    DB_Database_lock(topic->config.db);

    *qos = self->qos;

    DB_Database_unlock(topic->config.db);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Topic_set_qos(DDS_Topic *self, const struct DDS_TopicQos *qos)
{
    struct DDS_TopicImpl *topic = (struct DDS_TopicImpl*)self;

    LOG_PRECONDITION_P2(((self == NULL) || (qos == NULL)),
                        return DDS_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(self),
                        OSAPI_LOG_PTR_LOW(qos))

    if (!DDS_TopicQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_TOPIC_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    DB_Database_lock(topic->config.db);

    if (!DDS_TopicQos_immutable_is_equal(qos, &topic->qos))
    {
        DB_Database_unlock(topic->config.db);
        return DDS_RETCODE_IMMUTABLE_POLICY;
    }

    self->qos = *qos;

    DB_Database_unlock(topic->config.db);

    return DDS_RETCODE_OK;
}
#endif

const struct DDS_TopicQos*
DDS_Topic_get_qos_ref(DDS_Topic *topic)
{
    LOG_PRECONDITION(topic == NULL,return NULL)

    return &topic->qos;
}

DDS_Type*
DDS_Topic_get_type(DDS_Topic *topic)
{
    LOG_PRECONDITION(topic == NULL,return NULL)

    return topic->type;
}

#ifdef INCLUDE_API_LISTENER
struct DDS_TopicListener
DDS_Topic_get_listener(DDS_Topic *topic)
{
    struct DDS_TopicListener retval = DDS_TopicListener_INITIALIZER;

    LOG_PRECONDITION(topic == NULL,return retval)

    DB_Database_lock(topic->config.db);

    retval = topic->listener;

    DB_Database_unlock(topic->config.db);

    return retval;
}
#endif
