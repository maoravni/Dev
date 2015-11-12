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

/*** SOURCE_BEGIN ***/

#ifdef T
#undef T
#endif
#define T DDS_RtpsWellKnownPorts
#define T_DEFAULT DDS_RTPS_WELL_KNOWN_PORTS_DEFAULT
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T

DDS_Boolean
DDS_RtpsWellKnownPorts_is_consistent(
                                 const struct DDS_RtpsWellKnownPorts_t *self)
{
    LOG_testPrecondition(self == NULL, return DDS_BOOLEAN_FALSE);

    if ((self->port_base < 1) ||
        (self->domain_id_gain < 1) ||
        (self->participant_id_gain < 1) ||
        (self->builtin_multicast_port_offset < 0) ||
        (self->builtin_unicast_port_offset < 0) ||
        (self->user_multicast_port_offset < 0) ||
        (self->user_unicast_port_offset < 0))
    {

        return DDS_BOOLEAN_FALSE;
    }
    return DDS_BOOLEAN_TRUE;
}

/*i @brief get max participant index for a given set of cached well known ports */
DDS_Long
DDS_RtpsWellKnownPorts_get_max_participant_index(
                    const struct DDS_RtpsWellKnownPorts_t *rtpsWellKnownPorts)
{
    DDS_Long maxParticipantIndex = 0;
    const DDS_Long PORT_MAX = 65535;

    /* Compute max participant index */
    maxParticipantIndex =
        ((rtpsWellKnownPorts->domain_id_gain >
        rtpsWellKnownPorts->participant_id_gain) ?
                rtpsWellKnownPorts->domain_id_gain /
                rtpsWellKnownPorts->participant_id_gain :
                (PORT_MAX - rtpsWellKnownPorts-> port_base) /
                rtpsWellKnownPorts->participant_id_gain) - 1;

    /* Above is overselective for some mappings, fix for RTPS2.x standard mapping here */
    if (OSAPI_Utility_max(rtpsWellKnownPorts->builtin_unicast_port_offset,
                         rtpsWellKnownPorts->user_unicast_port_offset) >
        rtpsWellKnownPorts->participant_id_gain)
    {
        maxParticipantIndex -=
            OSAPI_Utility_max(rtpsWellKnownPorts->builtin_unicast_port_offset,
                             rtpsWellKnownPorts->user_unicast_port_offset) /
            rtpsWellKnownPorts->participant_id_gain;
    }

    return maxParticipantIndex;
}
