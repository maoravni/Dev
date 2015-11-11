/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef disc_dpde_discovery_plugin_h
#define disc_dpde_discovery_plugin_h


#ifndef disc_dpde_dll_h
#include "disc_dpde/disc_dpde_dll.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct DPDE_DiscoveryPlugin;
typedef struct DPDE_DiscoveryPlugin DPDE_DiscoveryPlugin_T;

/*i \file 
    \brief Dynamic Participant/Static Endpoint Discovery Plugin.
*/

/*e \defgroup NDDS_DPDE_Discovery_PluginModule Plugin Module
  \ingroup NDDS_DPDE_Discovery
  \brief Dynamic Participant Dynamic Endpoint Discovery Plugin

  The Plugin APIs are used to configure and instantiate the RTI 
  Connext Micro Dynamic Participant Dynamic Endpoint Discovery Plugin.
*/

/*e \ingroup NDDS_Discovery_Static_PluginModule

  \brief Properties structure to tune the workings of the DPDE Discovery Plugin.
*/

/*e \dref_DiscoveryPluginGroupDocs
 */

/*e \dref_DPDE_Discovery_PluginProperty
*/
struct DPDE_DiscoveryPluginProperty
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

    /*e \dref_DPDE_Discovery_PluginProperty_accept_unknown_peers
     */
    DDS_Boolean accept_unknown_peers;
};

#define DPDE_DISCOVERY_INTERFACE_ID \
  RT_MKINTERFACEID(RT_COMPONENT_CLASS_DISCOVERY,\
                   RT_COMPONENT_INSTANCE_DISCOVERY_DPDE)

/*i \dref_NDDS_DPDE_Discovery_PluginProperty_INITIALIZER
 */
#define DPDE_DiscoveryPluginProperty_INITIALIZER \
{\
    RT_ComponentFactoryProperty_INITIALIZER,\
    {30,0},  \
    {100,0}, \
    5,       \
    {1,0},   \
    DDS_BOOLEAN_TRUE,\
}

/*ce \dref_NDDS_DPDE_Discovery_PluginProperty_finalize
 */
DISC_DPDEDllExport DDS_ReturnCode_t
DPDE_DiscoveryPluginProperty_finalize(struct DPDE_DiscoveryPluginProperty *dst);

/* -------------------------------------------------------------------------- */
/* Plugin factory 
 */
/*ce \dref_Plugin_Factory_get_instance
 */
DISC_DPDEDllExport struct RT_ComponentFactoryI*
DPDE_DiscoveryFactory_get_interface(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* disc_dpde_discovery_plugin_h */
