/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DiscoveryPlugin_pkg_h
#define DiscoveryPlugin_pkg_h

struct DPDE_DiscoveryPlugin
{
    /* --- Parent plugin --- */
    struct NDDS_Discovery_Plugin _parent;

    /* --- Builtin --- */
    DDS_Publisher *publisher;
    DDS_DataWriter *participant_writer;
    DDS_DataWriter *publication_writer;
    DDS_DataWriter *subscription_writer;

    DDS_Subscriber *subscriber;
    DDS_DataReader *participant_reader;
    DDS_DataReader *publication_reader;
    DDS_DataReader *subscription_reader;

    /*DDS_InstanceHandle_t *instance_handle; */

    struct OSAPI_NtpTime participant_liveliness_assert_period;
    struct OSAPI_NtpTime initial_participant_announcement_period;

    struct DPDE_DiscoveryPluginProperty properties;

    /* The announcement event, which may get updated as we receive announcements
     * from remote participants.
     */
    OSAPITimeoutHandle_t announcement_event;

    /* This is owned by the participant, not the plugin.  A pointer is
     * stored for convenience
     */
    struct DDS_ParticipantBuiltinTopicData *participant_builtin_data;

    struct DDS_PublicationBuiltinTopicData *publication_builtin_data;

    struct DDS_SubscriptionBuiltinTopicData *subscription_builtin_data;

    struct NDDS_Type_Plugin *pub_type_plugin;
    struct NDDS_Type_Plugin *sub_type_plugin;

    RTI_BOOL timer_created;
};

extern DDS_Boolean
DPDE_DiscoveryPlugin_add_peer(struct DPDE_DiscoveryPlugin *discovery_plugin,
                              DDS_DomainParticipant *const participant,
                              struct DDS_DomainParticipantQos *participant_qos,
                              const char *peer_addr);

#endif /* DiscoveryPlugin_pkg_h */
