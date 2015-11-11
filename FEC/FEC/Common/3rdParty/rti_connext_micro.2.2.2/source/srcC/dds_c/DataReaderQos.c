/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef netio_config_h
#include "netio/netio_config.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif

#include "QosPolicy.pkg.h"

const struct DDS_DataReaderQos DDS_DATAREADER_QOS_DEFAULT =
    DDS_DataReaderQos_INITIALIZER;

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DDS_DataReaderQos_copy(struct DDS_DataReaderQos *out,
                       const struct DDS_DataReaderQos *in)
{
    LOG_testPrecondition(out == NULL || in == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    out->deadline = in->deadline;
    out->history = in->history;
    out->liveliness = in->liveliness;
    out->ownership = in->ownership;
    out->protocol = in->protocol;
    out->reliability = in->reliability;
    out->resource_limits = in->resource_limits;
    out->time_based_filter = in->time_based_filter;
    out->type_support = in->type_support;
    out->liveliness = in->liveliness;
    out->ownership = in->ownership;
    out->reader_resource_limits = in->reader_resource_limits;
    out->management = in->management;
    out->durability = in->durability;

    DDS_StringSeq_copy(&out->transport.enabled_transports, &in->transport.enabled_transports);

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
DDS_DataReaderQos_initialize(struct DDS_DataReaderQos * self)
{
struct DDS_DataReaderQos initVal = DDS_DataReaderQos_INITIALIZER;

    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    *self = initVal;

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
DDS_DataReaderQos_finalize(struct DDS_DataReaderQos * self)
{
    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    DDS_StringSeq_finalize(&self->transport.enabled_transports);

    return DDS_RETCODE_OK;
}

DDS_Boolean
DDS_DataReaderQos_immutable_is_equal(const struct DDS_DataReaderQos *left,
                                     const struct DDS_DataReaderQos *right)
{
    LOG_testPrecondition(left == NULL
                         || right == NULL, return DDS_BOOLEAN_FALSE);

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
        !DDS_TypeSupportQosPolicy_is_equal(&left->type_support,
                                           &right->type_support) ||
        !DDS_DataReaderProtocolQosPolicy_is_equal(&left->protocol,
                                                  &right->protocol) ||
        !DDS_ReliabilityQosPolicy_is_equal(&left->reliability,
                                           &right->reliability) ||
        !DDS_DurabilityQosPolicy_is_equal(&left->durability,
                                          &right->durability) ||
        !DDS_TransportQosPolicy_is_equal(&left->transport,
                                         &right->transport) ||
        !DDS_OwnershipQosPolicy_is_equal(&left->ownership, &right->ownership))
    {
        return DDS_BOOLEAN_FALSE;
    }
    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_DataReaderQos_is_equal(const struct DDS_DataReaderQos *left,
                           const struct DDS_DataReaderQos *right)
{
    if (!DDS_DataReaderQos_immutable_is_equal(left, right) ||
        !DDS_TimeBasedFilterQosPolicy_is_equal(&left->time_based_filter,
                                               &right->time_based_filter) ||
        !DDS_TypeSupportQosPolicy_is_equal(&left->type_support,
                                           &right->type_support) ||
        !DDS_DataReaderProtocolQosPolicy_is_equal(&left->protocol,
                                                  &right->protocol) ||
        !OSAPI_Memory_compare(&left->reader_resource_limits,
                             &right->reader_resource_limits,
                             sizeof(struct
                                    DDS_DataReaderResourceLimitsQosPolicy)))
    {
        return DDS_BOOLEAN_FALSE;
    }
    return DDS_BOOLEAN_TRUE;
}


DDS_Boolean
DDS_DataReaderQos_is_consistent(const struct DDS_DataReaderQos *self)
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
        /*LOG_printError("deadline QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_DEADLINE_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LivelinessQosPolicy_is_consistent(&self->liveliness))
    {
        /*LOG_printError("liveliness QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_LIVELINESS_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_HistoryQosPolicy_is_consistent(&self->history))
    {
        /*LOG_printError("history QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_HISTORY_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ResourceLimitsQosPolicy_is_consistent(&self->resource_limits))
    {
        /*LOG_printError("resource limits QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_RESOURCE_LIMITS_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipQosPolicy_is_consistent(&self->ownership))
    {
        /*LOG_printError("ownership QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_OWNERSHIP_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_TimeBasedFilterQosPolicy_is_consistent(&self->time_based_filter))
    {
        /*LOG_printError("time-based filter QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_TIME_BASED_FILTER_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_TypeSupportQosPolicy_is_consistent(&self->type_support))
    {
        /*LOG_printError("type support QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_TYPE_SUPPORT_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DataReaderProtocolQosPolicy_is_consistent(&self->protocol))
    {
        /*LOG_printError("data reader protocol QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_DATAREADER_PROTOCOL_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ReliabilityQosPolicy_is_consistent(&self->reliability))
    {
        /*LOG_printError("data reader reliability QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_RELIABILITY_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DurabilityQosPolicy_is_consistent(&self->durability))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DURABILITY_QOS);
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_TransportQosPolicy_is_consistent(&self->transport))
    {
        /*LOG_printError("data reader transport QoS policy is not consistent\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_TRANSPORT_QOS);
        return DDS_BOOLEAN_FALSE;
    }


#define VALUE_BETWEEN(v_,l_,u_) \
	((v_ != DDS_LENGTH_UNLIMITED) && (v_ >= l_) && (v_ <= u_))

    if (!(VALUE_BETWEEN(self->reader_resource_limits.max_remote_writers,
                        1, 100000000L) &&
          VALUE_BETWEEN(self->reader_resource_limits.
                        max_remote_writers_per_instance, 1, 100000000L)
          && VALUE_BETWEEN(self->reader_resource_limits.
                           max_samples_per_remote_writer, 1, 100000000L)))
    {
        return DDS_BOOLEAN_FALSE;
    }
#undef VALUE_BETWEEN

    /*---------- Additional inter-policy consistency checking ----------*/

    /* deadline must be >= minimum separation */
    if (DDS_Duration_compare
        (&self->deadline.period,
         &self->time_based_filter.minimum_separation) < 0)
    {
        /*LOG_printError("deadline QoS policy is not greater than "
         * "time-based filter minimum separation\n"); */
        DDSC_Log_error(LOG_INCONSISTENT_DEADLINE_AND_TBF_MIN_SEPARATION);
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}
