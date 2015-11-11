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
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif
#ifndef dds_c_domain_h
#include "dds_c/dds_c_domain.h"
#endif

struct DDS_TopicDescriptionImpl
{
    const char *topic_name;
    const char *type_name;
    DDS_DomainParticipant *participant;
    void *owner;
};

/*** SOURCE_BEGIN ***/

void
DDS_TopicDescriptionImpl_initialize(struct DDS_TopicDescriptionImpl *td,
                                    const char *topic_name,
                                    const char *type_name,
                                    void *topic)
{
    td->topic_name = topic_name;
    td->type_name = type_name;
    td->owner = topic;
}

void
DDS_TopicDescriptionImpl_finalize(struct DDS_TopicDescriptionImpl *td)
{
}

/*******************************************************************************
 *                              Public API
 ******************************************************************************/
const char*
DDS_TopicDescription_get_type_name(DDS_TopicDescription * self)
{
    struct DDS_TopicDescriptionImpl *td = (struct DDS_TopicDescriptionImpl*)self;

    if (td == NULL)
    {
        return NULL;
    }

    return td->type_name;
}

const char*
DDS_TopicDescription_get_name(DDS_TopicDescription *self)
{
    struct DDS_TopicDescriptionImpl *td = (struct DDS_TopicDescriptionImpl*)self;

    if (td == NULL)
    {
        return NULL;
    }

    return td->topic_name;
}

DDS_DomainParticipant*
DDS_TopicDescription_get_participant(DDS_TopicDescription *self)
{
    struct DDS_TopicDescriptionImpl *td = (struct DDS_TopicDescriptionImpl*)self;

    if (td == NULL)
    {
        return NULL;
    }

    return td->participant;
}
