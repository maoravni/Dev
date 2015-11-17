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

/*** SOURCE_BEGIN ***/

#ifdef T
#undef T
#endif
#define T DDS_ProtocolVersion
#define T_DEFAULT DDS_PROTOCOL_VERSION_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T

DDS_Boolean
DDS_ProtocolVersion_is_consistent(const struct DDS_ProtocolVersion *self)
{
    return (self == NULL ? DDS_BOOLEAN_FALSE : DDS_BOOLEAN_TRUE);
}