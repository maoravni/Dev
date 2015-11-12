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

#ifndef infrastructure_peer_h
#include "infrastructure.peer.h"
#endif
#ifndef dds_c_log_h
#include "dds_c/dds_c_log.h"
#endif

const struct DDS_PublisherQos DDS_PUBLISHER_QOS_DEFAULT =
    DDS_PublisherQos_INITIALIZER;

/*** SOURCE_BEGIN ***/

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_PublisherQos_copy(struct DDS_PublisherQos *out,
                      const struct DDS_PublisherQos *in)
{
    if ((out == NULL) || (in == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }


    *out = *in;

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_PublisherQos_initialize(struct DDS_PublisherQos * self)
{
    struct DDS_PublisherQos initVal = DDS_PublisherQos_INITIALIZER;

    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    *self = initVal;

    return DDS_RETCODE_OK;
}
#endif

DDS_ReturnCode_t
DDS_PublisherQos_finalize(struct DDS_PublisherQos * self)
{
    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    return DDS_RETCODE_OK;
}


DDS_Boolean
DDS_PublisherQos_is_equal(const struct DDS_PublisherQos *left,
                          const struct DDS_PublisherQos *right)
{
    if ((left == NULL) || (right == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_BOOLEAN_FALSE;
    }

    return !OSAPI_Memory_compare(left, right, sizeof(struct DDS_PublisherQos));
}


DDS_Boolean
DDS_PublisherQos_is_consistent(const struct DDS_PublisherQos *self)
{
    if (self == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}
