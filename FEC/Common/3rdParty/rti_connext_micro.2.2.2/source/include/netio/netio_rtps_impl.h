/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_rtps_impl_h
#define netio_rtps_impl_h



#define RTPS_getWellKnownMulticastPort(domain_id, port_base, domain_id_gain, port_offset) \
    (port_base + (domain_id_gain*domain_id) + port_offset)

/* RTPS spec - PB + DG*domainId + PG*participantId + dX */
#define RTPS_getWellKnownUnicastPort(domain_id, participant_id, port_base, domain_id_gain, participant_id_gain, port_offset) \
    (port_base + (domain_id_gain*domain_id) + (participant_id_gain*participant_id) + port_offset)


#endif /* netio_rtps_impl_h */
