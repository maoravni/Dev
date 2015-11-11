/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef disc_dpse_dpsediscovery_h
#define disc_dpse_dpsediscovery_h


#ifndef disc_dpse_dll_h
#include "disc_dpse/disc_dpse_dll.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* TODO:  TYPEDEF THIS */
struct DPSE_DiscoveryPlugin;

/*i \file 
    \brief Dynamic Participant/Static Endpoint Discovery Plugin.
*/

/*e \defgroup DPSE_DiscoveryPluginModule Plugin Module
  \ingroup NDDS_DPSE_Discovery
  \brief Dynamic Participant/Static Endpoint Discovery Plugin

  The Plugin APIs are used to configure and instantiate the RTI
  Connext Micro Dynamic Participant/Static Endpoint
  Discovery Plugin.

*/

/*e \ingroup NDDS_Discovery_Static_PluginModule

  \brief Properties structure to tune the workings of the DPSE Discovery Plugin.
*/

/*e \dref_DiscoveryPluginGroupDocs
 */

/*e \dref_DPSE_Discovery_PluginProperty
*/
struct DPSE_DiscoveryPluginProperty
{
    struct RT_ComponentFactoryProperty _parent;

    /*e \dref_DPSE_Discovery_PluginProperty_participant_liveliness_assert_period
     */
    struct DDS_Duration_t participant_liveliness_assert_period;

    /*e \dref_DPSE_Discovery_PluginProperty_participant_liveliness_lease_duration
     */
    struct DDS_Duration_t participant_liveliness_lease_duration;

    /*e \dref_DPSE_Discovery_PluginProperty_initial_participant_announcements
     */
    DDS_Long initial_participant_announcements;

    /*e \dref_DPSE_Discovery_PluginProperty_initial_participant_announcement_period
     */
    struct DDS_Duration_t initial_participant_announcement_period;
};

#define RTI_DPSEDISCOVERY_INTERFACE_ID \
  RT_MKINTERFACEID(RT_COMPONENT_CLASS_DISCOVERY,\
                   RT_COMPONENT_INSTANCE_DISCOVERY_DPSE)

/*i \dref_DPSE_DiscoveryPluginProperty_INITIALIZER
 */
#define DPSE_DiscoveryPluginProperty_INITIALIZER { \
    RT_ComponentFactoryProperty_INITIALIZER,\
    {30,0},  \
    {100,0}, \
    5,       \
    {1,0},   \
    }

/*ce \dref_DPSE_DiscoveryPluginProperty_finalize
 */
DISC_DPSEDllExport DDS_ReturnCode_t
DPSE_DiscoveryPluginProperty_finalize(
                            struct DPSE_DiscoveryPluginProperty *properties);

/*ce \dref_NDDS_DPSE_RemoteParticipant_assert_builtin
 */
DISC_DPSEDllExport DDS_ReturnCode_t
DPSE_RemoteParticipant_assert_builtin(DDS_DomainParticipant *const participant,
                           struct DDS_ParticipantBuiltinTopicData *const data);

/*ce \dref_RemoteParticipant_assert
 */
DISC_DPSEDllExport DDS_ReturnCode_t
DPSE_RemoteParticipant_assert(DDS_DomainParticipant *const participant,
                              const char *rem_participant_name);

/*ce \dref_RemotePublication_assert
 */
DISC_DPSEDllExport DDS_ReturnCode_t
DPSE_RemotePublication_assert(DDS_DomainParticipant * const participant,
                      const char *const rem_participant_name,
                      const struct DDS_PublicationBuiltinTopicData *const data,
                      NDDS_TypePluginKeyKind key_kind);

/*ce \dref_RemoteSubscription_assert
 */
DISC_DPSEDllExport DDS_ReturnCode_t
DPSE_RemoteSubscription_assert(DDS_DomainParticipant * const participant,
                   const char *const rem_participant_name,
                   const struct DDS_SubscriptionBuiltinTopicData *const data,
                   NDDS_TypePluginKeyKind key_kind);

/* -------------------------------------------------------------------------- */
/* Plugin factory 
 */
/*ce \dref_Plugin_Factory_get_instance
 */
DISC_DPSEDllExport struct RT_ComponentFactoryI*
DPSE_DiscoveryFactory_get_interface(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* disc_dpse_dpsediscovery_h */
