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

#include "QosPolicy.pkg.h"
#include "RtpsWellKnownPorts.pkg.h"

/*** SOURCE_BEGIN ***/
#ifdef T
#undef T
#endif
#define T DDS_DurabilityQosPolicy
#define T_DEFAULT DDS_DURABILITY_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_DurabilityQosPolicy_is_consistent(const struct DDS_DurabilityQosPolicy *self)
{
    if ((self->kind == DDS_VOLATILE_DURABILITY_QOS) ||
        (self->kind == DDS_TRANSIENT_LOCAL_DURABILITY_QOS))
    {
        return DDS_BOOLEAN_TRUE;
    }
    return DDS_BOOLEAN_FALSE;
}

DDS_Boolean
DDS_DurabilityQosPolicy_is_compatible(const struct DDS_DurabilityQosPolicy *request,
                                      const struct DDS_DurabilityQosPolicy *offered)
{
    return (request->kind <= offered->kind);
}

#ifdef T
#undef T
#endif
#define T DDS_DeadlineQosPolicy
#define T_DEFAULT DDS_DEADLINE_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_DeadlineQosPolicy_is_consistent(const struct DDS_DeadlineQosPolicy *self)
{
    const struct DDS_Duration_t DDS_DURATION_YEAR = { 31536000L, 0UL };
    const struct DDS_Duration_t DDS_DURATION_NANOSEC = { 0L, 1UL };

    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    if ((DDS_Duration_compare(&self->period, &DDS_DURATION_NANOSEC) < 0
         || DDS_Duration_compare(&self->period, &DDS_DURATION_YEAR) > 0)
        && DDS_Duration_compare(&self->period, &DDS_DURATION_INFINITE) != 0)
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_DeadlineQosPolicy_is_compatible(const struct DDS_DeadlineQosPolicy *request,
                                    const struct DDS_DeadlineQosPolicy *offered)
{
    if (DDS_Duration_compare(&request->period,
                             &DDS_DURATION_INFINITE) &&
        (DDS_Duration_compare(&request->period, &offered->period) < 0))
    {
        return DDS_BOOLEAN_FALSE;
    }
    return DDS_BOOLEAN_TRUE;
}


/*************************
    DDS_HistoryQosPolicy
*************************/
#define T DDS_HistoryQosPolicy
#define T_DEFAULT DDS_HISTORY_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal


DDS_Boolean
DDS_HistoryQosPolicy_is_consistent(const struct DDS_HistoryQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    if ((self->kind != DDS_KEEP_LAST_HISTORY_QOS) &&
        (self->kind != DDS_KEEP_ALL_HISTORY_QOS))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return ((self->depth > 0) && (self->depth < 1000000));
}

/******************************************
    DDS_SystemResourceLimitsQosPolicy
******************************************/
#define T DDS_SystemResourceLimitsQosPolicy
#define T_DEFAULT DDS_SYSTEM_RESOURCE_LIMITS_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_SystemResourceLimitsQosPolicy_immutable_is_equal(
                    const struct DDS_SystemResourceLimitsQosPolicy *left,
                    const struct DDS_SystemResourceLimitsQosPolicy *right)
{
    LOG_testPrecondition(left == NULL
                         || right == NULL, return DDS_BOOLEAN_FALSE);
    return (left->max_participants == right->max_participants);
}

DDS_Boolean
DDS_SystemResourceLimitsQosPolicy_is_consistent(
                    const struct DDS_SystemResourceLimitsQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);
    return (self->max_participants > 0);
}

/**************************************************
    DDS_ResourceLimitsQosPolicy
**************************************************/
#define T DDS_ResourceLimitsQosPolicy
#define T_DEFAULT DDS_RESOURCE_LIMITS_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_ResourceLimitsQosPolicy_is_consistent(
                                const struct DDS_ResourceLimitsQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    if ((self->max_samples < 1) || (self->max_instances < 1) ||
        (self->max_samples_per_instance < 1) ||
        (self->max_samples <
         (self->max_instances * self->max_samples_per_instance)))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}


/**************************************************
    DDS_OwnershipQosPolicy
**************************************************/
#define T DDS_OwnershipQosPolicy
#define T_DEFAULT DDS_OWNERSHIP_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_OwnershipQosPolicy_is_consistent(const struct DDS_OwnershipQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);
    return ((self->kind == DDS_SHARED_OWNERSHIP_QOS) ||
            (self->kind == DDS_EXCLUSIVE_OWNERSHIP_QOS));
}

DDS_Boolean
DDS_OwnershipQosPolicy_is_compatible(const struct DDS_OwnershipQosPolicy *request,
                                     const struct DDS_OwnershipQosPolicy *offered)
{
    return (request->kind == offered->kind);
}

/**************************************************
    DDS_OwnershipStrengthQosPolicy
**************************************************/
#define T DDS_OwnershipStrengthQosPolicy
#define T_DEFAULT DDS_OWNERSHIP_STRENGTH_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_OwnershipStrengthQosPolicy_is_consistent(
                            const struct DDS_OwnershipStrengthQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);
    return (self->value >= 0);
}


/**************************************************
    DDS_LivelinessQosPolicy
**************************************************/
#define T DDS_LivelinessQosPolicy
#define T_DEFAULT DDS_LIVELINESS_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_LivelinessQosPolicy_is_consistent(
                                    const struct DDS_LivelinessQosPolicy *self)
{
    const struct DDS_Duration_t DDS_DURATION_YEAR = { 31536000L, 0UL };

    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    if (self->kind != DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS)
    {
        return DDS_BOOLEAN_FALSE;
    }

    if ((DDS_Duration_compare(&self->lease_duration, &DDS_DURATION_ZERO) < 0 ||
         DDS_Duration_compare(&self->lease_duration, &DDS_DURATION_YEAR) > 0) &&
        DDS_Duration_compare(&self->lease_duration, &DDS_DURATION_INFINITE))
    {
        return DDS_BOOLEAN_FALSE;
    }
    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_LivelinessQosPolicy_is_compatible(const struct DDS_LivelinessQosPolicy *request,
                                      const struct DDS_LivelinessQosPolicy *offered)
{
    /*(request->kind <= offered->kind) && */
    return DDS_Duration_is_infinite(&request->lease_duration) ||
            ((request->kind <= offered->kind) &&
            (DDS_Duration_compare(&request->lease_duration,
                                 &offered->lease_duration) >= 0));
}

/**************************************************
    DDS_TimeBasedFilterQosPolicy
**************************************************/
#define T DDS_TimeBasedFilterQosPolicy
#define T_DEFAULT DDS_TIME_BASED_FILTER_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_TimeBasedFilterQosPolicy_is_consistent(
                               const struct DDS_TimeBasedFilterQosPolicy *self)
{
    const struct DDS_Duration_t DDS_DURATION_YEAR = { 31536000L, 0UL };

    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    if (DDS_Duration_compare(&self->minimum_separation, &DDS_DURATION_ZERO) < 0
        || DDS_Duration_compare(&self->minimum_separation,
                                &DDS_DURATION_YEAR) > 0)
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}


/**************************************************
    DDS_ReliabilityQosPolicy
**************************************************/
#define T DDS_ReliabilityQosPolicy
#define T_DEFAULT DDS_RELIABILITY_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_ReliabilityQosPolicy_is_consistent(
                                   const struct DDS_ReliabilityQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    if ((self->kind != DDS_RELIABLE_RELIABILITY_QOS) &&
        (self->kind != DDS_BEST_EFFORT_RELIABILITY_QOS))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_ReliabilityQosPolicy_is_compatible(const struct DDS_ReliabilityQosPolicy *reader,
                                       const struct DDS_ReliabilityQosPolicy *writer)
{
    if ((reader->kind == DDS_RELIABLE_RELIABILITY_QOS) &&
        (writer->kind != DDS_RELIABLE_RELIABILITY_QOS))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

/**************************************************
    DDS_TypeSupportQosPolicy
**************************************************/
#define T DDS_TypeSupportQosPolicy
#define T_DEFAULT DDS_TYPESUPPORT_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_TypeSupportQosPolicy_is_consistent(
                                    const struct DDS_TypeSupportQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);
    return DDS_BOOLEAN_TRUE;
}


/**************************************************
    DDS_DataWriterProtocolQosPolicy
**************************************************/
#define T DDS_DataWriterProtocolQosPolicy
#define T_DEFAULT DDS_DATA_WRITER_PROTOCOL_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_DataWriterProtocolQosPolicy_is_consistent(
                            const struct DDS_DataWriterProtocolQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    /* No restrictions on  rtps_object_id */
    return DDS_RtpsReliableWriterProtocol_is_consistent(
                                        &self->rtps_reliable_writer);
}

/**************************************************
    DDS_DataReaderResourceLimitsQosPolicy
**************************************************/
#define T DDS_DataReaderResourceLimitsQosPolicy
#define T_DEFAULT DDS_DATAREADERRESOURCE_LIMITS_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_DataReaderResourceLimitsQosPolicy_is_consistent(
                    const struct DDS_DataReaderResourceLimitsQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    if ((self->max_remote_writers < 1) || (self->max_remote_writers > 1000000)
        || (self->max_samples_per_remote_writer < 1)
        || (self->max_samples_per_remote_writer > 100000000))
    {

        return DDS_BOOLEAN_FALSE;

    }

    return DDS_BOOLEAN_TRUE;
}


/**************************************************
    DDS_TransportQosPolicy
**************************************************/
#define T DDS_TransportQosPolicy
#define T_DEFAULT DDS_TRANSPORT_QOS_POLICY_DEFAULT
#define T_is_equal DDS_TransportQosPolicy_is_equal
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_TransportQosPolicy_is_equal(const struct DDS_TransportQosPolicy *left,
                                const struct DDS_TransportQosPolicy *right)
{
    return DDS_StringSeq_is_equal(&left->enabled_transports, &right->enabled_transports);
}

DDS_Boolean
DDS_TransportQosPolicy_is_consistent(const struct DDS_TransportQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);
    return DDS_BOOLEAN_TRUE;
}


/**************************************************
    DDS_RtpsReliableWriterProtocol_t
**************************************************/
#define T DDS_RtpsReliableWriterProtocol_t
#define T_DEFAULT DDS_RTPS_RELIABLE_WRITER_PROTOCOL_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

/* Commenting out so makeheader won't add this to a .h file while the rest of the file
 * is #if 0'd out.  if this is used in the future, uncomment the function name. 
 */
DDS_Boolean
DDS_RtpsReliableWriterProtocol_is_consistent(
                          const struct DDS_RtpsReliableWriterProtocol_t *self)
{
    const struct DDS_Duration_t DDS_DURATION_NANOSEC = { 0L, 1UL };
    const struct DDS_Duration_t DDS_DURATION_YEAR = { 31536000L, 0UL };

    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    /* heartbeat_period */
    if (DDS_Duration_compare(&self->heartbeat_period,
                             &DDS_DURATION_NANOSEC) < 0 ||
        DDS_Duration_compare(&self->heartbeat_period, &DDS_DURATION_YEAR) > 0)
    {
        return DDS_BOOLEAN_FALSE;
    }

    /* heartbeats_per_max_samples */
    if (self->heartbeats_per_max_samples < 0 ||
        self->heartbeats_per_max_samples > 100000000)
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

/**************************************************
    DDS_EntityNameQosPolicy
**************************************************/
#define T DDS_EntityNameQosPolicy
#define T_DEFAULT DDS_ENTITY_NAME_QOS_POLICY_DEFAULT
#define T_is_equal DDS_EntityNameQosPolicy_is_equal
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_EntityNameQosPolicy_is_equal(const struct DDS_EntityNameQosPolicy *left,
                                 const struct DDS_EntityNameQosPolicy *right)
{
    return !DDS_String_cmp(left->name, right->name);
}

DDS_Boolean
DDS_EntityNameQosPolicy_is_consistent(
                                 const struct DDS_EntityNameQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);
    return DDS_BOOLEAN_TRUE;
}

/**************************************************
    DDS_DataReaderProtocolQosPolicy
**************************************************/
#define T DDS_DataReaderProtocolQosPolicy
#define T_DEFAULT DDS_DATA_READER_PROTOCOL_QOS_POLICY_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT
#undef _initialize
#undef _finalize
#undef _copy
#undef _is_equal

DDS_Boolean
DDS_DataReaderProtocolQosPolicy_is_consistent(
                            const struct DDS_DataReaderProtocolQosPolicy *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);
    return DDS_BOOLEAN_TRUE;
}
