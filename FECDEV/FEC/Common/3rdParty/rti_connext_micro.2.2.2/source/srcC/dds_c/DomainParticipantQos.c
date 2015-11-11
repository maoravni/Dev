/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
/*i @file
 * @ingroup DDSDomainModule
 */
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif

const struct DDS_DomainParticipantQos DDS_PARTICIPANT_QOS_DEFAULT =
                                        DDS_DomainParticipantQos_INITIALIZER;

/*** SOURCE_BEGIN ***/

RTI_PRIVATE DDS_Boolean
DDS_DomainParticipantQos_discovery_is_equal(
                    const struct DDS_DiscoveryQosPolicy *left,
                    const struct DDS_DiscoveryQosPolicy *right)
{
    if (!DDS_StringSeq_is_equal(&left->initial_peers,
                                &right->initial_peers))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (OSAPI_Memory_compare(&left->discovery,&right->discovery,
                            sizeof(left->discovery)))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipantQos_copy(struct DDS_DomainParticipantQos *out,
                              const struct DDS_DomainParticipantQos *in)
{
    LOG_testPrecondition(out == NULL || in == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    out->entity_factory = in->entity_factory;

    if (!DDS_StringSeq_copy(&out->discovery.initial_peers,
                            &in->discovery.initial_peers))
    {
        return DDS_RETCODE_ERROR;
    }

    if (!DDS_StringSeq_copy(&out->discovery.enabled_transports,&in->discovery.enabled_transports))
    {
        return DDS_RETCODE_ERROR;
    }

    out->discovery.discovery = in->discovery.discovery;
    out->resource_limits = in->resource_limits;
    out->participant_name = in->participant_name;
    out->protocol = in->protocol;

    if (!DDS_StringSeq_copy(&out->transports.enabled_transports,
                             &in->transports.enabled_transports))
    {
        return DDS_RETCODE_ERROR;
    }

    if (!DDS_StringSeq_copy(&out->user_traffic.enabled_transports,
                             &in->user_traffic.enabled_transports))
    {
        return DDS_RETCODE_ERROR;
    }

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipantQos_initialize(struct DDS_DomainParticipantQos *self)
{
    struct DDS_DomainParticipantQos initVal = DDS_DomainParticipantQos_INITIALIZER;

    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    *self = initVal;

    return DDS_RETCODE_OK;
}
#endif


DDS_ReturnCode_t
DDS_DomainParticipantQos_finalize(struct DDS_DomainParticipantQos *self)
{
    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    DDS_StringSeq_finalize(&self->discovery.initial_peers);
    DDS_StringSeq_finalize(&self->transports.enabled_transports);
    DDS_StringSeq_finalize(&self->discovery.enabled_transports);
    DDS_StringSeq_finalize(&self->user_traffic.enabled_transports);

    return DDS_RETCODE_OK;
}

DDS_Boolean
DDS_DomainParticipantQos_immutable_is_equal(
                                const struct DDS_DomainParticipantQos *left,
                                const struct DDS_DomainParticipantQos *right)
{
    if (!DDS_DomainParticipantQos_discovery_is_equal(&left->discovery,
                                                     &right->discovery))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (OSAPI_Memory_compare(&left->resource_limits,&right->resource_limits,
                            sizeof(left->resource_limits)))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (OSAPI_Memory_compare(&left->participant_name,&right->participant_name,
                            sizeof(left->participant_name)))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (OSAPI_Memory_compare(&left->protocol,&right->protocol,
                            sizeof(left->protocol)))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_StringSeq_is_equal(&left->transports.enabled_transports,
                                &right->transports.enabled_transports))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_DomainParticipantQos_is_equal(const struct DDS_DomainParticipantQos *left,
                                  const struct DDS_DomainParticipantQos *right)
{
    if (left->entity_factory.autoenable_created_entities !=
        right->entity_factory.autoenable_created_entities)
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DomainParticipantQos_discovery_is_equal(&left->discovery,
                                                     &right->discovery))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (OSAPI_Memory_compare(&left->resource_limits,&right->resource_limits,
                            sizeof(left->resource_limits)))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (OSAPI_Memory_compare(&left->participant_name,&right->participant_name,
                            sizeof(left->participant_name)))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (OSAPI_Memory_compare(&left->protocol,&right->protocol,
                            sizeof(left->protocol)))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_StringSeq_is_equal(&left->transports.enabled_transports,
                                &right->transports.enabled_transports))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_DomainParticipantQos_is_consistent(
                                const struct DDS_DomainParticipantQos *self)
{
    const struct DDS_DomainParticipantResourceLimitsQosPolicy *resource;

    resource = &self->resource_limits;
    if ((resource->local_reader_allocation < 0) ||
            (resource->local_writer_allocation < 0) ||
            (resource->local_subscriber_allocation < 0) ||
            (resource->local_publisher_allocation < 0) ||
            (resource->local_topic_allocation < 0) ||
            (resource->local_type_allocation < 0) ||
            (resource->matching_reader_writer_pair_allocation < 0) ||
            (resource->matching_writer_reader_pair_allocation < 0) ||
            (resource->max_destination_ports < 0) ||
            (resource->max_gather_destinations < 0) ||
            (resource->max_receive_ports < 0) ||
            (resource->remote_participant_allocation < 0) ||
            (resource->remote_reader_allocation < 0) ||
            (resource->remote_writer_allocation < 0))
    {
        DDSC_Log_error(LOG_INCONSISTENT_RESOURCE_LIMITS_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (self->protocol.participant_id < -1)
    {
        DDSC_Log_error(LOG_INCONSISTENT_WIRE_PROTOCOL_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}
