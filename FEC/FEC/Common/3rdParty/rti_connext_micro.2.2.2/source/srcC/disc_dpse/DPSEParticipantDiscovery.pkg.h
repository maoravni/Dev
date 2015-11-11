/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DPSEParticipantDiscovery_pkg_h
#define DPSEParticipantDiscovery_pkg_h

extern OSAPITimeoutOp_t
DPSE_ParticipantDiscovery_assert_participant(
                                        struct OSAPITimeoutUserData *storage);

extern DDS_ReturnCode_t
DPSE_ParticipantDiscovery_schedule_fast_assertions(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        const struct DDS_ParticipantBuiltinTopicData *local_participant_data,
        DDS_Boolean new_event);

#endif /* DPSEParticipantDiscovery_pkg_h */
