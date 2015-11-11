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
#ifndef dds_c_domain_h
#include "dds_c/dds_c_domain.h"
#endif

#include "Entity.pkg.h"
#include "QosPolicy.pkg.h"
#include "TopicDescription.pkg.h"
#include "Topic.pkg.h"
#include "Type.pkg.h"
#include "DomainFactoryQos.pkg.h"

const struct DDS_DomainParticipantFactoryQos DDS_PARTICIPANT_FACTORY_QOS_DEFAULT
    =                       DDS_DomainParticipantFactoryQos_INITIALIZER;

#define fail_with(__code) \
result = (__code);\
goto done;

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DDS_DomainParticipantFactoryQos_copy(
                        struct DDS_DomainParticipantFactoryQos *out,
                        const struct DDS_DomainParticipantFactoryQos *in)
{
    LOG_testPrecondition(out == NULL || in == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    *out = *in;

    return DDS_RETCODE_OK;
}

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_DomainParticipantFactoryQos_initialize(
                        struct DDS_DomainParticipantFactoryQos *self)
{
    struct DDS_DomainParticipantFactoryQos initVal =
                            DDS_DomainParticipantFactoryQos_INITIALIZER;

    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    *self = initVal;

    return DDS_RETCODE_OK;
}
#endif

DDS_ReturnCode_t
DDS_DomainParticipantFactoryQos_finalize(
                        struct DDS_DomainParticipantFactoryQos* self)
{
    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    return DDS_RETCODE_OK;
}

DDS_Boolean
DDS_DomainParticipantFactoryQos_immutable_is_equal(
                     const struct DDS_DomainParticipantFactoryQos *left,
                     const struct DDS_DomainParticipantFactoryQos *right)
{
    RTI_BOOL retval = RTI_FALSE;

    LOG_testPrecondition(left == NULL || right == NULL, return RTI_FALSE);

    retval =
        DDS_SystemResourceLimitsQosPolicy_immutable_is_equal(
                            &left->resource_limits, &right->resource_limits);

    return retval;
}

DDS_Boolean
DDS_DomainParticipantFactoryQos_is_equal(
                        const struct DDS_DomainParticipantFactoryQos *left,
                        const struct DDS_DomainParticipantFactoryQos *right)
{
    LOG_testPrecondition(left == NULL || right == NULL, return RTI_FALSE);

    return !OSAPI_Memory_compare(left, right,
                                sizeof(struct DDS_DomainParticipantFactoryQos));
}


DDS_Boolean
DDS_DomainParticipantFactoryQos_is_consistent(
                          const struct DDS_DomainParticipantFactoryQos *self)
{
    LOG_testPrecondition(self == NULL, return RTI_FALSE);

    return
        DDS_SystemResourceLimitsQosPolicy_is_consistent(&self->resource_limits);
}
