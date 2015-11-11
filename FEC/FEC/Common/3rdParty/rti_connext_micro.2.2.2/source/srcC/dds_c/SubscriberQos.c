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

#ifndef infrastructure_peer_h
#include "infrastructure.peer.h"
#endif

const struct DDS_SubscriberQos DDS_SUBSCRIBER_QOS_DEFAULT =
    DDS_SubscriberQos_INITIALIZER;

/*** SOURCE_BEGIN ***/

DDS_Boolean
DDS_SubscriberQos_is_consistent(const struct DDS_SubscriberQos *self)
{
    return DDS_BOOLEAN_TRUE;
}

DDS_ReturnCode_t
DDS_SubscriberQos_copy(struct DDS_SubscriberQos * out,
                       const struct DDS_SubscriberQos * in)
{
    LOG_testPrecondition(out == NULL || in == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);
    *out = *in;

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
DDS_SubscriberQos_initialize(struct DDS_SubscriberQos * self)
{
struct DDS_SubscriberQos initVal = DDS_SubscriberQos_INITIALIZER;
    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    *self = initVal;

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
DDS_SubscriberQos_finalize(struct DDS_SubscriberQos * self)
{
    LOG_testPrecondition(self == NULL, return DDS_RETCODE_PRECONDITION_NOT_MET);

    return DDS_RETCODE_OK;
}

DDS_Boolean
DDS_SubscriberQos_is_equal(const struct DDS_SubscriberQos * left,
                           const struct DDS_SubscriberQos * right)
{
    LOG_testPrecondition(left == NULL
                         || right == NULL, return DDS_BOOLEAN_FALSE);

    return !OSAPI_Memory_compare(left, right, sizeof(struct DDS_SubscriberQos));
}
