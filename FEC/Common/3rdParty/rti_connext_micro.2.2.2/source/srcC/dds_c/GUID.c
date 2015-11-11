/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

/*i @file
 * @ingroup DDSGUIDSupportModule
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
#ifndef dds_c_log_h
#include "dds_c/dds_c_log.h"
#endif

/* ------------------------------------------------------------------
 * Public Constants
 * ------------------------------------------------------------------ */

/* exported in infrastructure.ifc */
const struct DDS_GUID_t DDS_GUID_UNKNOWN =
    { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
const struct DDS_GUID_t DDS_GUID_AUTO =
    { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };

const struct DDS_GUID_t DDS_GUID_PREFIX_UNKNOWN =
    { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
const struct DDS_GUID_t DDS_GUID_PREFIX_AUTO =
    { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };

/*** SOURCE_BEGIN ***/

/* ------------------------------------------------------------------------
 * Public Methods
 * ------------------------------------------------------------------------ */

DDS_Boolean
DDS_GUID_equals(const struct DDS_GUID_t *self, const struct DDS_GUID_t *other)
{
    return (DDS_Boolean) (!OSAPI_Memory_compare(
       self->value, other->value, sizeof(DDS_GUID_UNKNOWN)));
}

void
DDS_GUID_copy(struct DDS_GUID_t *self, const struct DDS_GUID_t *other)
{
    OSAPI_Memory_copy(self->value, other->value, sizeof(self->value));
}

DDS_Long
DDS_GUID_compare(const struct DDS_GUID_t *self, const struct DDS_GUID_t *other)
{
    return OSAPI_Memory_compare(self->value, other->value, sizeof(self->value));
}

void
DDS_GUID_to_rtps(struct MIGRtpsGuid *other, const struct DDS_GUID_t *self)
{
#if RTI_ENDIAN_BIG
    OSAPI_Memory_copy(other, self->value, 16);
#else
    struct MIGRtpsGuid rtps_tmp;

    OSAPI_Memory_copy(&rtps_tmp, self->value, 16);

    other->prefix.hostId = NETIO_ntohl(rtps_tmp.prefix.hostId);
    other->prefix.appId = NETIO_ntohl(rtps_tmp.prefix.appId);
    other->prefix.instanceId = NETIO_ntohl(rtps_tmp.prefix.instanceId);
    other->objectId = NETIO_ntohl(rtps_tmp.objectId);
#endif
}

void
DDS_GUID_from_rtps(struct DDS_GUID_t *self, const struct MIGRtpsGuid *other)
{
#if RTI_ENDIAN_BIG
    OSAPI_Memory_copy(self->value, other, 16);
#else
    struct MIGRtpsGuid rtps_tmp;

    rtps_tmp.prefix.hostId = NETIO_htonl(other->prefix.hostId);
    rtps_tmp.prefix.appId = NETIO_htonl(other->prefix.appId);
    rtps_tmp.prefix.instanceId = NETIO_htonl(other->prefix.instanceId);
    rtps_tmp.objectId = NETIO_htonl(other->objectId);

    OSAPI_Memory_copy(self->value, &rtps_tmp, 16);
#endif
}


