/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DPSEDiscoveryPlugin_pkg_h
#define DPSEDiscoveryPlugin_pkg_h

struct DPSE_DiscoveryPlugin
{
    /* --- Parent plugin --- */
    struct NDDS_Discovery_Plugin _parent;

    /* --- Participant discovery writer --- */
    DDS_Publisher *participant_publisher;
    DDS_DataWriter *participant_writer;

    /* --- Participant discovery reader --- */
    DDS_Subscriber *participant_subscriber;
    DDS_DataReader *participant_reader;

    /*DDS_InstanceHandle_t *instance_handle; */

    struct OSAPI_NtpTime participant_liveliness_assert_period;
    struct OSAPI_NtpTime initial_participant_announcement_period;

    struct DPSE_DiscoveryPluginProperty properties;

    /* An array of all the IDs of remote entities that we have discovered.
     */
    DDS_BuiltinTopicKey_t *key_list;
    int key_list_length;
    int key_list_max;

    /* The announcement event, which may get updated as we receive announcements
     * from remote participants.
     */
    OSAPITimeoutHandle_t announcement_event;

    /* This is owned by the participant, not the plugin.  A pointer is 
     * stored for convenience
     */
    struct DDS_ParticipantBuiltinTopicData *participant_builtin_data;
};

extern DDS_ReturnCode_t
DPSE_DiscoveryPluginProperty_copy(struct DPSE_DiscoveryPluginProperty *dst,
                                  struct DPSE_DiscoveryPluginProperty *src);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_before_local_participant_created(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant * const participant,
        struct DDS_DomainParticipantQos *local_participant_qos,
        struct DDS_ParticipantBuiltinTopicData *participant_data_out);

extern DDS_Boolean
DPSE_DiscoveryPlugin_add_peer(struct DPSE_DiscoveryPlugin *discovery_plugin,
                              DDS_DomainParticipant *const participant,
                              struct DDS_DomainParticipantQos *participant_qos,
                              const char *Package_addr);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_participant_created(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const local_participant_data);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_participant_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const local_participant_data);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_announce_local_participant_deletion(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        const struct DDS_BuiltinTopicKey_t *local_participant_key);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_before_local_participant_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_BuiltinTopicKey_t *const local_participant_key);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_data_writer_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_DataWriter *const data_writer,
        const struct DDS_DataWriterQos *const qos);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_data_writer_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        const struct DDS_BuiltinTopicKey_t *const local_datawriter_key);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_data_reader_enabled(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_DataReader *const data_reader,
        const struct DDS_DataReaderQos *const qos);

extern DDS_ReturnCode_t
DPSE_DiscoveryPlugin_after_local_data_reader_deleted(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant* const participant,
        struct DDS_BuiltinTopicKey_t *const local_datareader_key);

extern void
DPSE_Plugin_remove_key_from_list(
                            struct DPSE_DiscoveryPlugin *disc_plugin,
                            DDS_BuiltinTopicKey_t *key);

extern DDS_ReturnCode_t
DPSE_Plugin_after_participant_liveliness_expired(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const remote_participant_data);

#endif /* DPSEDiscoveryPlugin_pkg_h */
