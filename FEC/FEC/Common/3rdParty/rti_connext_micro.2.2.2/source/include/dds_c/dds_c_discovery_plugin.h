/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_discovery_plugin_h
#define dds_c_discovery_plugin_h



#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_domain_h
#include "dds_c/dds_c_domain.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern DDSCDllVariable const char *DDS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME;
extern DDSCDllVariable const char *DDS_PARTICIPANT_BUILTIN_TOPIC_NAME;
extern DDSCDllVariable const char *DDS_PUBLICATION_BUILTIN_TOPIC_TYPE_NAME;
extern DDSCDllVariable const char *DDS_PUBLICATION_BUILTIN_TOPIC_NAME;
extern DDSCDllVariable const char *DDS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME;
extern DDSCDllVariable const char *DDS_SUBSCRIPTION_BUILTIN_TOPIC_NAME;

/*i @file
 @ingroup DDSDiscoveryModule
 @brief Defines the \dds discovery package
*/
struct NDDS_Discovery_Plugin
{
    struct RT_Component _parent;
};

struct NDDS_Discovery_Listener
{
    struct RT_ComponentListener _parent;
};

#define NDDS_Discovery_Listener_INITIALIZE {\
  RT_ComponentListener_INITIALIZER\
}

#define NDDSDiscovery_narrow(factory_) (((struct NDDS_Discovery_Plugin*)(factory_))->intf)

/* ------------------------------ Life-cycle -------------------------------- */
typedef void (*NDDSDiscovery_deleteFunc) (struct NDDS_Discovery_Plugin * self);

typedef struct NDDS_Discovery_Plugin*
(*NDDS_Discovery_Plugin_createFunc) (
                        const struct NDDS_Discovery_Property *const property,
                        const struct NDDS_Discovery_Listener *const listener);

/*i dref_Discovery_Plugin_BeforeLocalParticipantCreatedCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_BeforeLocalParticipantCreatedCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_DomainParticipantQos *participant_qos,
        struct DDS_ParticipantBuiltinTopicData *local_builtin_data_out);

/*i dref_Discovery_Plugin_AfterLocalParticipantCreatedCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_AfterLocalParticipantCreatedCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const local_participant_data);

/*i dref_Discovery_Plugin_AfterLocalParticipantEnabledCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_AfterLocalParticipantEnabledCallback)(
        struct NDDS_Discovery_Plugin* const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const local_participant_data);

/*i dref_Discovery_Plugin_BeforeLocalParticipantDeletedCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_BeforeLocalParticipantDeletedCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_BuiltinTopicKey_t *const local_participant_key);

/*i dref_Discovery_Plugin_BeforeLocalParticipantDeletedCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_AnnounceLocalParticipantLivelinessCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
         DDS_DomainParticipant *const participant,
         struct DDS_BuiltinTopicKey_t *const local_participant_key);

/* writer-related activities */
/*i dref_Discovery_Plugin_AfterLocalDataWriterEnabledCallback
*/
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_AfterLocalDataWriterEnabledCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_DataWriter *const data_writer,
        const struct DDS_DataWriterQos *const qos);

/*i dref_Discovery_Plugin_AfterLocalDataWriterDeletedCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_AfterLocalDataWriterDeletedCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        const struct DDS_BuiltinTopicKey_t *const local_datawriter_key);

/* reader-related activities */
/*i dref_Discovery_Plugin_AfterLocalDataReaderEnabledCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_AfterLocalDataReaderEnabledCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_DataReader *const data_reader,
        const struct DDS_DataReaderQos *const qos);

/*i dref_Discovery_Plugin_AfterLocalDataReaderDeletedCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_AfterLocalDataReaderDeletedCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        struct DDS_BuiltinTopicKey_t *const local_datareader_key);

/*i dref_Discovery_Plugin_BeforeLocalParticipantCreatedCallback
 */
typedef DDS_ReturnCode_t
(*NDDS_Discovery_Plugin_RemoteParticipantLivelinessExpiredCallback)(
        struct NDDS_Discovery_Plugin * const discovery_plugin,
        DDS_DomainParticipant * const participant,
        struct DDS_ParticipantBuiltinTopicData *const remote_participant_data);

/*i dref_Discovery_Plugin_InterfaceAllowedCallback
 */
typedef DDS_Boolean
(*NDDS_Discovery_Plugin_OnInterfaceDetectedCallback)(
        struct NDDS_Discovery_Plugin *const discovery_plugin,
        DDS_DomainParticipant *const participant,
        DDS_Long transport_kind,
        const char *const ifname);

/* ================================================================= */
/*                       Type Plugin Interface                       */
/* ================================================================= */
struct NDDS_DiscoveryI
{
    struct RT_ComponentI _parent;

    /*i \dref_Discovery_Plugin_on_remote_participant_liveliness_expired
     */
    NDDS_Discovery_Plugin_RemoteParticipantLivelinessExpiredCallback
    on_remote_participant_liveliness_expired;

    /*i \dref_Discovery_Plugin_on_before_local_participant_created
     */
    NDDS_Discovery_Plugin_BeforeLocalParticipantCreatedCallback
    on_before_local_participant_created;

    /*i \dref_Discovery_Plugin_on_after_local_participant_created
     */
    NDDS_Discovery_Plugin_AfterLocalParticipantCreatedCallback
    on_after_local_participant_created;

    /*i \dref_Discovery_Plugin_on_after_local_participant_enabled
     */
    NDDS_Discovery_Plugin_AfterLocalParticipantEnabledCallback
    on_after_local_participant_enabled;

    /*i \dref_Discovery_Plugin_on_before_local_participant_deleted
     */
    NDDS_Discovery_Plugin_BeforeLocalParticipantDeletedCallback
    on_before_local_participant_deleted;

    /* writer-related activities */
    /*i \dref_Discovery_Plugin_on_after_local_datawriter_enabled
     */
    NDDS_Discovery_Plugin_AfterLocalDataWriterEnabledCallback
    on_after_local_datawriter_enabled;

    /*i \dref_Discovery_Plugin_on_after_local_datawriter_deleted
     */
    NDDS_Discovery_Plugin_AfterLocalDataWriterDeletedCallback
    on_after_local_datawriter_deleted;

    /* reader-related activities */
    /*i \dref_Discovery_Plugin_on_after_local_datareader_enabled
     */
    NDDS_Discovery_Plugin_AfterLocalDataReaderEnabledCallback
    on_after_local_datareader_enabled;

    /*i \dref_Discovery_Plugin_on_after_local_datareader_deleted
     */
    NDDS_Discovery_Plugin_AfterLocalDataReaderDeletedCallback
    on_after_local_datareader_deleted;

    /*i \dref_Discovery_Plugin_on_interface_detected
     */
    NDDS_Discovery_Plugin_OnInterfaceDetectedCallback
    on_interface_detected;
};



#define DiscoveryComponentFactory_create_component(f_,p_,l_) \
    (struct NDDS_Discovery_Plugin*)((f_)->intf)->create_component(f_,p_,l_)

#define DiscoveryComponentFactory_delete_component(f_,c_) \
    ((f_)->intf)->delete_component(f_,(RT_Component_T*)(c_))



/*i dref_Discovery_Plugin_BeforeLocalParticipantCreatedCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_before_local_participant_created(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *participant,
        struct DDS_DomainParticipantQos *local_participant_qos,
        struct DDS_ParticipantBuiltinTopicData *local_builtin_data_out);

/*i dref_Discovery_Plugin_AfterLocalParticipantCreatedCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_after_local_participant_created(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant* participant,
        struct DDS_ParticipantBuiltinTopicData *local_participant_data);

/*i dref_Discovery_Plugin_AfterLocalParticipantEnabledCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_after_local_participant_enabled(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant* participant,
        const struct DDS_ParticipantBuiltinTopicData *local_participant_data);

/*i dref_Discovery_Plugin_BeforeLocalParticipantDeletedCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_before_local_participant_deleted(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant* participant,
        const struct DDS_BuiltinTopicKey_t *local_participant_key);

/*i dref_Discovery_Plugin_BeforeLocalParticipantDeletedCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_announce_local_participant_liveliness(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *participant,
        const struct DDS_BuiltinTopicKey_t *local_participant_key);

/* writer-related activities */
/*i dref_Discovery_Plugin_AfterLocalDataWriterEnabledCallback
*/
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_after_local_datawriter_enabled(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *participant,
        const DDS_DataWriter *const data_writer,
        const struct DDS_DataWriterQos *const qos);

/*i dref_Discovery_Plugin_AfterLocalDataWriterDeletedCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_after_local_datawriter_deleted(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *participant,
        const struct DDS_BuiltinTopicKey_t *local_datawriter_key);

/* reader-related activities */
/*i dref_Discovery_Plugin_AfterLocalDataReaderEnabledCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_after_local_datareader_enabled(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *participant,
        const DDS_DataReader *const data_reader,
        const struct DDS_DataReaderQos *const qos);

/*i dref_Discovery_Plugin_AfterLocalDataReaderDeletedCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_after_local_datareader_deleted(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant* participant,
        const struct DDS_BuiltinTopicKey_t *local_datareader_key);

/*i dref_Discovery_Plugin_BeforeLocalParticipantCreatedCallback
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_Discovery_Plugin_on_remote_participant_liveliness_expired(
        const struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *participant,
        struct DDS_ParticipantBuiltinTopicData *remote_participant_data);

/*i     dref_Discovery_Plugin_InterfaceAllowedCallback
 */
DDSCDllExport DDS_Boolean
NDDS_Discovery_Plugin_on_interface_detected(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        DDS_DomainParticipant *participant,
        DDS_Long transport_kind,
        const char *ifname);

/*----------------------------------- */
#define NDDS_Discovery_Plugin_on_before_local_participant_created(\
        self_,p_,q_,b_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_before_local_participant_created(self_,p_,q_,b_)

#define NDDS_Discovery_Plugin_on_after_local_participant_created(\
        self_,p_,data_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_after_local_participant_created(self_,p_,data_)

#define NDDS_Discovery_Plugin_on_after_local_participant_enabled(\
        self_,p_,data_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_after_local_participant_enabled(self_,p_,data_)

#define NDDS_Discovery_Plugin_on_before_local_participant_deleted(\
        self_,p_,key_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_before_local_participant_deleted(self_,p_,key_)

#define NDDS_Discovery_Plugin_on_announce_local_participant_liveliness(\
        self_,p_,key_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_announce_local_participant_liveliness(self_,p_,key_)

#define NDDS_Discovery_Plugin_on_after_local_datawriter_enabled(\
        self_,p_,dw_,dq_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_after_local_datawriter_enabled(self_,p_,dw_,dq_)

#define NDDS_Discovery_Plugin_on_after_local_datawriter_deleted(\
        self_,p_,dwkey_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_after_local_datawriter_deleted(self_,p_,dwkey_)

#define NDDS_Discovery_Plugin_on_after_local_datareader_enabled(\
        self_,p_,dr_,dq_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_after_local_datareader_enabled(self_,p_,dr_,dq_)

#define NDDS_Discovery_Plugin_on_after_local_datareader_deleted(self_,p_,drkey)\
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_after_local_datareader_deleted(self_,p_,drkey)

#define NDDS_Discovery_Plugin_on_remote_participant_liveliness_expired(\
        self_,p_,data_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_remote_participant_liveliness_expired(self_,p_,data_)

#define NDDS_Discovery_Plugin_on_interface_detected(self_,p_,tkind_,ifn_) \
((struct NDDS_DiscoveryI*)((self_)->_parent._intf))->\
  on_interface_detected(self_,p_,tkind_,ifn_)

/*ce \dref_Discovery_match_local_writer
 */
DDSCDllExport DDS_DomainParticipant*
NDDS_DiscoveryUtils_get_participant(const struct NDDS_Discovery_Plugin *plugin);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_discovery_plugin_h */
