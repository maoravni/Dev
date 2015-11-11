/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef PublicationDiscovery_pkg_h
#define PublicationDiscovery_pkg_h

extern DDS_ReturnCode_t
DPDE_PublicationDiscovery_after_local_participant_created(
            struct NDDS_Discovery_Plugin *const discovery_plugin,
            DDS_DomainParticipant *const participant,
            struct DDS_ParticipantBuiltinTopicData *local_participant_data);

extern DDS_ReturnCode_t
DPDE_PublicationDiscovery_after_local_data_writer_enabled(
                        struct NDDS_Discovery_Plugin *const discovery_plugin,
                        DDS_DomainParticipant *const participant,
                        DDS_DataWriter *const datawriter,
                        const struct DDS_DataWriterQos *const qos);

extern DDS_ReturnCode_t
DPDE_PublicationDiscovery_after_local_data_writer_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        const struct DDS_BuiltinTopicKey_t *local_datawriter_key);

extern DDS_ReturnCode_t
DPDE_PublicationDiscovery_assert_remote_builtin(
                    struct NDDS_Discovery_Plugin *const discovery_plugin,
                    DDS_DomainParticipant *const participant,
                    struct DDS_ParticipantBuiltinTopicData *dp_builtin_data);

#endif /* PublicationDiscovery_pkg_h */
