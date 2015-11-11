/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_discovery_h
#define dds_c_discovery_h



#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif
#ifndef dds_c_domain_h
#include "dds_c/dds_c_domain.h"
#endif
#ifndef dds_c_type_h
#include "dds_c/dds_c_type.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define DDS_BUILTIN_ENDPOINT_PARTICIPANT_ANNOUNCER    0x00000001 << 0
#define DDS_BUILTIN_ENDPOINT_PARTICIPANT_DETECTOR     0x00000001 << 1
#define DDS_BUILTIN_ENDPOINT_PUBLICATION_ANNOUNCER    0x00000001 << 2
#define DDS_BUILTIN_ENDPOINT_PUBLICATION_DETECTOR     0x00000001 << 3
#define DDS_BUILTIN_ENDPOINT_SUBSCRIPTION_ANNOUNCER   0x00000001 << 4
#define DDS_BUILTIN_ENDPOINT_SUBSCRIPTION_DETECTOR    0x00000001 << 5

#define DDSC_PARTICIPANT_ADDRESS_COUNT_MAX MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX

/*e \dref_ParticipantBuiltInTopicGroupDocs
 */

#define DDS_BUILTIN_TOPIC_KEY_OBJECT_ID 3

/*e \dref_ParticipantBuiltinTopicData 
 */
struct DDS_ParticipantBuiltinTopicData
{
    /*e \dref_ParticipantBuiltinTopicData_key
     */
    struct DDS_BuiltinTopicKey_t key;       /* GUID must be first */

    /*e \dref_ParticipantBuiltinTopicData_participant_name
     */
    struct DDS_EntityNameQosPolicy participant_name;

    /*e \dref_ParticipantBuiltinTopicData_dds_builtin_endpoints
     */
    DDS_UnsignedLong dds_builtin_endpoints;

    /*e \dref_ParticipantBuiltinTopicData_rtps_protocol_version
     */
    DDS_ProtocolVersion_t rtps_protocol_version;

    /*e \dref_ParticipantBuiltinTopicData_rtps_vendor_id
     */
    struct DDS_VendorId_t rtps_vendor_id;

    /*e \dref_ParticipantBuiltinTopicData_default_unicast_locators
     */
    struct DDS_LocatorSeq default_unicast_locators;

    /*e \dref_ParticipantBuiltinTopicData_default_multicast_locators
     */
    struct DDS_LocatorSeq default_multicast_locators;

    /* Not documented in the full DDS product */
    struct DDS_LocatorSeq metatraffic_unicast_locators;

    /* Not documented in the full DDS product */
    struct DDS_LocatorSeq metatraffic_multicast_locators;

    /* Liveliness assert period! */
    /* Not documented in the full DDS product
     */
    struct DDS_Duration_t liveliness_lease_duration;

    /*e \dref_ParticipantBuiltinTopicData_product_version
     */
    struct DDS_ProductVersion_t product_version;

};
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_FULL(DDS_ParticipantBuiltinTopicData);

/*i \dref_DDS_ParticipantBuiltinTopicData_INITIALIZER
 */
#define DDS_ParticipantBuiltinTopicData_INITIALIZER { \
    DDS_BuiltinTopicKey_t_INITIALIZER, \
    DDS_ENTITY_NAME_QOS_POLICY_DEFAULT, \
    0, /* builtin endpoints */ \
    DDS_PROTOCOL_VERSION_DEFAULT, \
    DDS_VENDOR_ID_DEFAULT, \
    DDS_SEQUENCE_INITIALIZER(struct DDS_Locator), \
    DDS_SEQUENCE_INITIALIZER(struct DDS_Locator), \
    DDS_SEQUENCE_INITIALIZER(struct DDS_Locator), \
    DDS_SEQUENCE_INITIALIZER(struct DDS_Locator), \
    {100L,0L}, /* participant_liveliness_lease_duration */\
    DDS_PRODUCTVERSION_UNKNOWN   }

DDS_SEQUENCE(DDS_ParticipantBuiltinTopicDataSeq,
             struct DDS_ParticipantBuiltinTopicData)

#define DDS_ParticipantBuiltinTopicDataSeq_INITIALIZER \
DDS_SEQUENCE_INITIALIZER(struct DDS_ParticipantBuiltinTopicData)

/*e \dref_PublicationBuiltInTopicGroupDocs
 */

/*e \dref_PublicationBuiltinTopicData
 */
struct DDS_PublicationBuiltinTopicData
{
    /*e \dref_PublicationBuiltinTopicData_key
     */
    struct DDS_BuiltinTopicKey_t key;       /* Key GUID must be first */

    /*e \dref_PublicationBuiltinTopicData_participant_key
     */
    struct DDS_BuiltinTopicKey_t participant_key;

    /*e \dref_PublicationBuiltinTopicData_topic_name
     */
    char *topic_name;

    /*e \dref_PublicationBuiltinTopicData_type_name
     */
    char *type_name;

    /*e \dref_PublicationBuiltinTopicData_deadline
     */
    struct DDS_DeadlineQosPolicy deadline;

    /*e \dref_PublicationBuiltinTopicData_ownership
     */
    struct DDS_OwnershipQosPolicy ownership;

    /*e \dref_PublicationBuiltinTopicData_ownership_strength
     */
    struct DDS_OwnershipStrengthQosPolicy ownership_strength;

    /*e \dref_PublicationBuiltinTopicData_reliability
     */
    struct DDS_ReliabilityQosPolicy reliability;

    /*e \dref_PublicationBuiltinTopicData_liveliness
     */
    struct DDS_LivelinessQosPolicy liveliness;

    /*e \dref_PublicationBuiltinTopicData_durability
     */
    struct DDS_DurabilityQosPolicy durability;

    /*e \dref_PublicationBuiltinTopicData_unicast_locator
     */
    struct DDS_LocatorSeq unicast_locator;

    /*i \dref_PublicationBuiltinTopicData_send_queue_size
     * If making this field externally documented, add it to the print function
     */
    DDS_Long send_queue_size;
};

DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_FULL(DDS_PublicationBuiltinTopicData);

/*i \dref_DDS_PublicationBuiltinTopicData_INITIALIZER
 */
#define DDS_PublicationBuiltinTopicData_INITIALIZER { \
    DDS_BuiltinTopicKey_t_INITIALIZER, \
    DDS_BuiltinTopicKey_t_INITIALIZER, \
    NULL,\
    NULL,\
    DDS_DEADLINE_QOS_POLICY_DEFAULT, \
    DDS_OWNERSHIP_QOS_POLICY_DEFAULT,\
    DDS_OWNERSHIP_STRENGTH_QOS_POLICY_DEFAULT, \
    DDS_RELIABILITY_QOS_POLICY_DEFAULT,\
    DDS_LIVELINESS_QOS_POLICY_DEFAULT, \
    DDS_DURABILITY_QOS_POLICY_DEFAULT, \
    DDS_SEQUENCE_INITIALIZER(struct DDS_Locator),\
    256\
}

DDS_SEQUENCE(DDS_PublicationBuiltinTopicDataSeq,
             struct DDS_PublicationBuiltinTopicData)

#define DDS_PublicationBuiltinTopicDataSeq_INITIALIZER \
DDS_SEQUENCE_INITIALIZER(struct DDS_PublicationBuiltinTopicData)

/*e \dref_SubscriptionBuiltInTopicGroupDocs
 */

/*e \dref_SubscriptionBuiltinTopicData
 */
struct DDS_SubscriptionBuiltinTopicData
{
    /*e \dref_SubscriptionBuiltinTopicData_key
     * Key GUID must be first
     */
    struct DDS_BuiltinTopicKey_t key;

    /*e \dref_SubscriptionBuiltinTopicData_participant_key
     */
    struct DDS_BuiltinTopicKey_t participant_key;

    /*e \dref_SubscriptionBuiltinTopicData_topic_name
     */
    char *topic_name;

    /*e \dref_SubscriptionBuiltinTopicData_type_name
     */
    char *type_name;

    /*e \dref_SubscriptionBuiltinTopicData_deadline
     */
    struct DDS_DeadlineQosPolicy deadline;

    /*e \dref_SubscriptionBuiltinTopicData_ownership
     */
    struct DDS_OwnershipQosPolicy ownership;

    /*e \dref_SubscriptionBuiltinTopicData_reliability
     */
    struct DDS_ReliabilityQosPolicy reliability;

    /*e \dref_SubscriptionBuiltinTopicData_liveliness
     */
    struct DDS_LivelinessQosPolicy liveliness;

    /*e \dref_SubscriptionBuiltinTopicData_durability
     */
    struct DDS_DurabilityQosPolicy durability;

    /*e \dref_SubscriptionBuiltinTopicData_unicast_locator
     */
    struct DDS_LocatorSeq unicast_locator;

    /*e \dref_SubscriptionBuiltinTopicData_multicast_locator
     */
    struct DDS_LocatorSeq multicast_locator;
};

DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_FULL(DDS_SubscriptionBuiltinTopicData);

/*i \dref_DDS_SubscriptionBuiltinTopicData_INITIALIZER
 */
#define DDS_SubscriptionBuiltinTopicData_INITIALIZER { \
    DDS_BuiltinTopicKey_t_INITIALIZER, \
    DDS_BuiltinTopicKey_t_INITIALIZER, \
    NULL,\
    NULL,\
    DDS_DEADLINE_QOS_POLICY_DEFAULT, \
    DDS_OWNERSHIP_QOS_POLICY_DEFAULT ,\
    DDS_RELIABILITY_QOS_POLICY_DEFAULT,\
    DDS_LIVELINESS_QOS_POLICY_DEFAULT, \
    DDS_DURABILITY_QOS_POLICY_DEFAULT, \
    DDS_SEQUENCE_INITIALIZER(struct DDS_Locator),\
    DDS_SEQUENCE_INITIALIZER(struct DDS_Locator)\
}

DDS_SEQUENCE(DDS_SubscriptionBuiltinTopicDataSeq,
             struct DDS_SubscriptionBuiltinTopicData)

#define DDS_SubscriptionBuiltinTopicDataSeq_INITIALIZER \
DDS_SEQUENCE_INITIALIZER(struct DDS_SubscriptionBuiltinTopicData)

struct NDDS_RemoteEntityImpl;
typedef struct NDDS_RemoteEntityImpl NDDS_RemoteEntity;
struct DDS_RemotePublicationImpl;
typedef struct DDS_RemotePublicationImpl DDS_RemotePublication;
typedef struct DDS_RemoteSubscriptionImpl DDS_RemoteSubscription;
typedef struct DDS_RemoteParticipantImpl DDS_RemoteParticipant;

/*ci \dref_RemoteParticipant_assert
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_assert_remote_participant(
        DDS_DomainParticipant *const participant,
        struct DDS_ParticipantBuiltinTopicData *const data,
        DDS_Boolean *const is_new);

/*ci \dref_RemoteParticipant_liveliness_refresh
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_refresh_remote_participant_liveliness(
        DDS_DomainParticipant * const participant,
        const struct DDS_BuiltinTopicKey_t *const key);

/*ci \dref_RemoteParticipant_reset
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_reset_remote_participant(
        DDS_DomainParticipant *const participant,
        const struct DDS_BuiltinTopicKey_t *const key);

/*ci \dref_RemoteParticipant_remove
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_remove_remote_participant(
        DDS_DomainParticipant *const participant,
        const DDS_BuiltinTopicKey_t *const key);

/*ci \dref_RemoteParticipant_enable_name
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_enable_remote_participant_name(
        DDS_DomainParticipant * const participant,
        const struct DDS_ParticipantBuiltinTopicData *const data);

/*ci \dref_RemoteParticipant_enable_name
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_enable_remote_participant_guid(
         DDS_DomainParticipant * const participant,
         const struct DDS_ParticipantBuiltinTopicData *const data);

/*ci \dref_RemotePublication_assert
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_assert_remote_publication(
        DDS_DomainParticipant *const participant,
        const char *const participant_name,
        const struct DDS_PublicationBuiltinTopicData *const data,
       NDDS_TypePluginKeyKind key_kind);

/*ci \dref_RemotePublication_remove
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_remove_remote_publication(
        DDS_DomainParticipant *const participant,
        const DDS_BuiltinTopicKey_t *const key);

/*ci \dref_RemoteSubscription_assert
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_assert_remote_subscription(
        DDS_DomainParticipant* const participant,
        const char *const participant_name,
        const struct DDS_SubscriptionBuiltinTopicData *const data,
        NDDS_TypePluginKeyKind key_kind);

/*ci \dref_RemoteSubscription_remove
 */
DDSCDllExport DDS_ReturnCode_t
NDDS_DomainParticipant_remove_remote_subscription(
        DDS_DomainParticipant* const participant,
        const DDS_BuiltinTopicKey_t* const key);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_discovery_h */
