/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef Topic_pkg_h
#define Topic_pkg_h

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

extern RTI_INT32
DDS_TopicImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                     const DB_Record_T op1, void *op2, void *op3);

extern DDS_Boolean
DDS_TopicImpl_is_hidden(DDS_Topic *self);

extern DDS_Boolean
DDS_Topic_listener_is_consistent(const struct DDS_TopicListener *self,
                                 DDS_StatusMask mask);

extern DDS_Boolean
DDS_TopicImpl_finalize(struct DDS_TopicImpl *topic);

extern DDS_Boolean
DDS_TopicImpl_initialize(struct DDS_TopicImpl *topic,
                         DDS_DomainParticipant *participant,
                         const char *topic_name,
                         DDS_Type *type,
                         const struct DDS_TopicQos *qos,
                         const struct DDS_TopicListener *listener,
                         DDS_StatusMask mask,
                         DDS_Long object_id,
                         struct NDDS_TopicConfig *config);

extern DDS_Boolean
DDS_Topic_is_compatible(DDS_Topic *left,
                        const char *right_topic_name,
                        const char *right_type_name);

extern const struct DDS_TopicQos*
DDS_Topic_get_qos_ref(DDS_Topic *topic);

extern DDS_Type*
DDS_Topic_get_type(DDS_Topic *topic);

#endif /* Topic_pkg_h */
