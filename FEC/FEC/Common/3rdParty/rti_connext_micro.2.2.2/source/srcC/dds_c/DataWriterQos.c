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
#ifndef netio_config_h
#include "netio/netio_config.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif

#include "QosPolicy.pkg.h"

const struct DDS_DataWriterQos DDS_DATAWRITER_QOS_DEFAULT =
    DDS_DataWriterQos_INITIALIZER;

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DDS_DataWriterQos_copy(struct DDS_DataWriterQos *out,
                       const struct DDS_DataWriterQos *in)
{
    if (out == NULL)
    {
        /*LOG_printError(DDS_LOG_BAD_PARAMETER_s_IS_NULL,"out"); */
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (in == NULL)
    {
        /*LOG_printError(DDS_LOG_BAD_PARAMETER_s_IS_NULL,"in"); */
        DDSC_Log_error(LOG_BAD_SECOND_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    out->deadline = in->deadline;
    out->liveliness = in->liveliness;
    out->history = in->history;
    out->resource_limits = in->resource_limits;
    out->ownership = in->ownership;
    out->ownership_strength = in->ownership_strength;
    out->reliability = in->reliability;
    out->protocol = in->protocol;
    out->type_support = in->type_support;
    out->ownership = in->ownership;
    out->management = in->management;
    out->durability = in->durability;

    DDS_StringSeq_copy(&out->transport.enabled_transports, &in->transport.enabled_transports);

    return DDS_RETCODE_OK;
}

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DataWriterQos_initialize(struct DDS_DataWriterQos * self)
{
struct DDS_DataWriterQos initVal = DDS_DATAWRITER_QOS_DEFAULT;

    if (self == NULL)
    {
        /*LOG_printError(DDS_LOG_BAD_PARAMETER_s_IS_NULL,"self"); */
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    *self = initVal;

    return DDS_RETCODE_OK;
}
#endif

DDS_ReturnCode_t
DDS_DataWriterQos_finalize(struct DDS_DataWriterQos * self)
{
    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DDS_StringSeq_finalize(&self->transport.enabled_transports);

    return DDS_RETCODE_OK;
}

DDS_Boolean
DDS_DataWriterQos_immutable_is_equal(const struct DDS_DataWriterQos *left,
                                     const struct DDS_DataWriterQos *right)
{
    if (!DDS_DeadlineQosPolicy_is_equal(&left->deadline,
                                        &right->deadline) ||
        !DDS_LivelinessQosPolicy_is_equal(&left->liveliness,
                                          &right->liveliness) ||
        !DDS_HistoryQosPolicy_is_equal(&left->history,
                                       &right->history) ||
        !DDS_ResourceLimitsQosPolicy_is_equal(&left->resource_limits,
                                              &right->resource_limits) ||
        !DDS_OwnershipQosPolicy_is_equal(&left->ownership,
                                         &right->ownership) ||
        !DDS_OwnershipStrengthQosPolicy_is_equal(&left->ownership_strength,
                                                 &right->ownership_strength) ||
        !DDS_TypeSupportQosPolicy_is_equal(&left->type_support,
                                           &right->type_support) ||
        !DDS_DataWriterProtocolQosPolicy_is_equal(&left->protocol,
                                                  &right->protocol) ||
        !DDS_ReliabilityQosPolicy_is_equal(&left->reliability,
                                           &right->reliability) ||
        !DDS_DurabilityQosPolicy_is_equal(&left->durability,
                                          &right->durability) ||
        !DDS_TransportQosPolicy_is_equal(&left->transport, &right->transport))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_DataWriterQos_is_consistent(const struct DDS_DataWriterQos *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

#if !RTPS_RELIABILITY
    if (self->reliability.kind == DDS_RELIABLE_RELIABILITY_QOS)
    {
#error "Reliability not enabled.  Please specify RTPS_RELIABILITY"
    }
#endif

    if (!DDS_DeadlineQosPolicy_is_consistent(&self->deadline))
    {
        /*LOG_printError("data writer deadline QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_DEADLINE_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LivelinessQosPolicy_is_consistent(&self->liveliness))
    {
        /*LOG_printError("data writer liveliness QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_LIVELINESS_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_HistoryQosPolicy_is_consistent(&self->history))
    {
        /*LOG_printError("data writer history QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_HISTORY_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ResourceLimitsQosPolicy_is_consistent(&self->resource_limits))
    {
        /*LOG_printError("data writer resource limits QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_RESOURCE_LIMITS_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DataWriterProtocolQosPolicy_is_consistent(&self->protocol))
    {
        /*LOG_printError("data writer protocol QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_DATAWRITER_PROTOCOL_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_TypeSupportQosPolicy_is_consistent(&self->type_support))
    {
        /*LOG_printError("data writer type support QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_TYPE_SUPPORT_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ReliabilityQosPolicy_is_consistent(&self->reliability))
    {
        /*LOG_printError("data writer reliability QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_RELIABILITY_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DurabilityQosPolicy_is_consistent(&self->durability))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DURABILITY_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipQosPolicy_is_consistent(&self->ownership))
    {
        /*LOG_printError("data writer ownership QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_OWNERSHIP_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipStrengthQosPolicy_is_consistent
        (&self->ownership_strength))
    {
        /*LOG_printError("data writer ownership strength QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_OWNERSHIP_STRENGTH_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_TransportQosPolicy_is_consistent(&self->transport))
    {
        /*LOG_printError("data writer transport QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_TRANSPORT_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if ((self->resource_limits.max_samples != DDS_LENGTH_UNLIMITED) &&
        (self->reliability.kind == DDS_RELIABLE_RELIABILITY_QOS &&
         self->protocol.rtps_reliable_writer.heartbeats_per_max_samples >
         self->resource_limits.max_samples))
    {
        /*LOG_printError("data writer heartbeats_per_max_samples > max_samples\n"); */
        DDSC_Log_error
            (LOG_INCONSISTENT_HEARTBEATS_PER_MAX_SAMPLES_AND_MAX_SAMPLES);
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}
