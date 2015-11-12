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
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif

const struct DDS_TopicQos DDS_TOPIC_QOS_DEFAULT = DDS_TopicQos_INITIALIZER;

/*** SOURCE_BEGIN ***/

DDS_Boolean
DDS_TopicQos_is_consistent(const struct DDS_TopicQos *self)
{
    return DDS_BOOLEAN_TRUE;
}

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_TopicQos_copy(struct DDS_TopicQos * out, const struct DDS_TopicQos * in)
{
    LOG_testPrecondition(out == NULL || in == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);
    *out = *in;

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_TopicQos_initialize(struct DDS_TopicQos * self)
{
struct DDS_TopicQos initVal = DDS_TopicQos_INITIALIZER;
    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    *self = initVal;

    return DDS_RETCODE_OK;
}
#endif

DDS_ReturnCode_t
DDS_TopicQos_finalize(struct DDS_TopicQos * self)
{
    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    return DDS_RETCODE_OK;
}

DDS_Boolean
DDS_TopicQos_is_equal(const struct DDS_TopicQos * left,
                      const struct DDS_TopicQos * right)
{
    LOG_testPrecondition(left == NULL
                         || right == NULL, return DDS_BOOLEAN_FALSE);

    return !OSAPI_Memory_compare(left, right, sizeof(struct DDS_TopicQos));
}

DDS_Boolean
DDS_TopicQos_immutable_is_equal(const struct DDS_TopicQos *left,
                                const struct DDS_TopicQos *right)
{
    if (left == NULL)
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (right == NULL)
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DeadlineQosPolicy_is_equal(&left->deadline, &right->deadline))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LivelinessQosPolicy_is_equal(&left->liveliness,
                                          &right->liveliness))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_HistoryQosPolicy_is_equal(&left->history, &right->history))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipQosPolicy_is_equal(&left->ownership, &right->ownership))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}
