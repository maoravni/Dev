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
#ifndef infrastructure_peer_h
#include "infrastructure.peer.h"
#endif

/* ------------------------------------------------------------------------
 * Public Constants
 * ------------------------------------------------------------------------ */
const DDS_InstanceHandle_t DDS_HANDLE_NIL = DDS_HANDLE_NIL_NATIVE;

/*** SOURCE_BEGIN ***/

/* ------------------------------------------------------------------------
 * Public Methods
 * ------------------------------------------------------------------------ */
DDS_Boolean
DDS_InstanceHandle_equals(const DDS_InstanceHandle_t * self,
                          const DDS_InstanceHandle_t * other)
{
    if (self->is_valid != other->is_valid)
    {
        return DDS_BOOLEAN_FALSE;
    }

    return !OSAPI_Memory_compare(self->octet, other->octet, 16);
}

DDS_Long
DDS_InstanceHandle_compare(const DDS_InstanceHandle_t * self,
                           const DDS_InstanceHandle_t * other)
{
    return OSAPI_Memory_compare(self->octet, other->octet, sizeof(self->octet));
}

void
DDS_InstanceHandle_to_rtps(struct MIGRtpsGuid *other,
                           const DDS_InstanceHandle_t * self)
{

#if RTI_ENDIAN_BIG
    OSAPI_Memory_copy(other, self->octet, 16);
#else
    struct MIGRtpsGuid rtps_tmp;

    OSAPI_Memory_copy(&rtps_tmp, self->octet, 16);

    other->prefix.hostId = NETIO_ntohl(rtps_tmp.prefix.hostId);
    other->prefix.appId = NETIO_ntohl(rtps_tmp.prefix.appId);
    other->prefix.instanceId = NETIO_ntohl(rtps_tmp.prefix.instanceId);
    other->objectId = NETIO_ntohl(rtps_tmp.objectId);
#endif
}

void
DDS_InstanceHandle_from_rtps(DDS_InstanceHandle_t * self,
                             const struct MIGRtpsGuid *other)
{
#if RTI_ENDIAN_BIG
    OSAPI_Memory_copy(self->octet, other, 16);
#else
    struct MIGRtpsGuid rtps_tmp;

    rtps_tmp.prefix.hostId = NETIO_htonl(other->prefix.hostId);
    rtps_tmp.prefix.appId = NETIO_htonl(other->prefix.appId);
    rtps_tmp.prefix.instanceId = NETIO_htonl(other->prefix.instanceId);
    rtps_tmp.objectId = NETIO_htonl(other->objectId);

    OSAPI_Memory_copy(self->octet, &rtps_tmp, 16);
#endif
    self->is_valid = DDS_BOOLEAN_TRUE;
}

/* ----------------------------------------------------------------- */
/* End $Id: InstanceHandle.c,v 1.2 2005/08/24 02:54:08 yi Exp $ */
