/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rtps_rtps_h
#define rtps_rtps_h


#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef reda_epoch_h
#include "reda/reda_epoch.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef osapi_system_h
#include "osapi/osapi_system.h"
#endif
#ifndef rtps_dll_h
#include "rtps/rtps_dll.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef netio_common_h
#include "netio/netio_common.h"
#endif
#ifndef netio_common_h
#include "netio/netio_common.h"
#endif
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_route_h
#include "netio/netio_route.h"
#endif
#ifndef netio_interface_h
#include "netio/netio_interface.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif


/* Doxygen file-level documentation =====================================*/
/*e \file
    \brief RTPS protocol defined types, implemented in C.
*/



/* TODO: Document */
typedef RTI_UINT32 RTPS_ReturnCode_t;

/*e \dref_ReturnCode_t_RETCODE_OK    
 */
#define     RTPS_RETCODE_OK                     0

#define RTPS_RETCODE_START                      1000

/*e \dref_ReturnCode_t_RETCODE_ERROR
*/
#define RTPS_RETCODE_ERROR                      RTPS_RETCODE_START

/*e \dref_ReturnCode_t_RETCODE_UNSUPPORTED
 */
#define RTPS_RETCODE_UNSUPPORTED                RTPS_RETCODE_START + 1

/*e \dref_ReturnCode_t_RETCODE_BAD_PARAMETER
 */
#define RTPS_RETCODE_BAD_PARAMETER              RTPS_RETCODE_START + 2

/*e \dref_ReturnCode_t_RETCODE_PRECONDITION_NOT_MET
 */
#define RTPS_RETCODE_PRECONDITION_NOT_MET       RTPS_RETCODE_START + 3

/*e \dref_ReturnCode_t_RETCODE_OUT_OF_RESOURCES
 */
#define RTPS_RETCODE_OUT_OF_RESOURCES           RTPS_RETCODE_START + 4

/*e \dref_ReturnCode_t_RETCODE_ALREADY_DELETED
 */
#define RTPS_RETCODE_ALREADY_DELETED            RTPS_RETCODE_START + 5




/*e \defgroup MIGRtpsClass MIGRtps
  \ingroup MIGModule
  \brief The RTPS protocol defines many types.  This submodule is a concrete
  implementation of the data types necessary for RTPS communication.
  For detailed explanation, see RTPS protocol.
*/

typedef enum
{
    MIG_RTPS_APPKIND_UNKNOWN = 0x0,
    MIG_RTPS_APPKIND_APP = 0x01,
    MIG_RTPS_APPKIND_MANAGER = 0x02
} MIGRtpsAppKind;

typedef RTI_UINT32 MIGRtpsProductLine;

#define MIG_RTPS_PRODUCT_LINE_UNKNOWN 0x0
#define MIG_RTPS_PRODUCT_LINE_LITE    0x1

/*
can't use enum b/c this generates compiler warning when passed into de/serialization funcs
typedef enum {
    MIG_RTPS_PRODUCT_LINE_UNKNOWN = 0x0,
    MIG_RTPS_PRODUCT_LINE_LITE = 0x1
} MIGRtpsProductLine;
*/

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_HEADER_SIZE (20)       /*RTPS.2.0 */

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_SUBMESSAGE_HEADER_SIZE (4)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_SUBMESSAGE_OCTETS_TO_NEXT_HEADER_MAX (0xFFFF)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_SUBMESSAGE_SIZE_MIN (8)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_SUBMESSAGE_HEADER_SIZE_MAX (64)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_SUBMESSAGE_ALIGNMENT (4)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_IP_ADDRESS_INVALID (0)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_PORT_INVALID (0)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_TIMESTAMP_SEC_DEFAULT (0)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_TIMESTAMP_FRAC_DEFAULT (0)

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_PATHNAME_LEN_MAX (255)

/*e \ingroup MIGRtpsPidModule
  Used for participant GUID
*/
/*e \defgroup MIGRtpsParameterSequenceClass MIGRtpsParameterSequence
  \ingroup MIGModule
  \brief optional parameter sequence
  For detailed explanation, see RTPS protocol.
*/

/*e \defgroup MIGRtpsPidModule MIGRtpsPid
  \ingroup MIGRtpsModule
  @brief RTPS defined parameter ID.
*/

/*e \ingroup MIGRtpsPidModule
  RTPS parameter ID, used to describe RTPS objects.
*/
typedef RTI_UINT16 MIGRtpsParameterId;

/*e \ingroup MIGRtpsPidModule
  Mask for ParameterId bit indicating subspace where 
  unrecognized PIDs are either ignored or treated as 
  incompatible QoS
*/
#define MIG_RTPS_PID_INCOMPATIBLE_MASK              (0x4000)

/*e \ingroup MIGRtpsPidModule
  Pad for parameter values in case they don't end on a 4-byte boundary
*/
#define MIG_RTPS_PID_PAD                             (0x0000)

/*e \ingroup MIGRtpsPidModule
  Indicate end of parameter sequence.
*/
#define MIG_RTPS_PID_SENTINEL                        (0x0001)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::userDataQosPolicy : UserDataQosPolicy,
  Subscription::userDataQosPolicy : UserDataQosPolicy
  Application::userDataQosPolicy : UserDataQosPolicy
*/
#define MIG_RTPS_PID_USER_DATA                       (0x002c)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::topic : PathName, Subscription::topic : PathName
*/
#define MIG_RTPS_PID_TOPIC_NAME                      (0x0005)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::typeName : TypeName,
  Subscription::typeName : TypeName
*/
#define MIG_RTPS_PID_TYPE_NAME                       (0x0007)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::groupDataQosPolicy : GroupDataQosPolicy,
  Subscription::groupDataQosPolicy : GroupDataQosPolicy
*/
#define MIG_RTPS_PID_GROUP_DATA                       (0x002d)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::topicDataQosPolicy : TopicDataQosPolicy,
  Subscription::topicDataQosPolicy : TopicDataQosPolicy
*/
#define MIG_RTPS_PID_TOPIC_DATA                       (0x002e)

/*e \ingroup MIGRtpsPidModule
  Used for field Publicatoin::durabilityQosPolicy : DurabilityQosPolicy
  Used for field Subscription::durabilityQosPolicy : DurabilityQosPolicy
 */
#define MIG_RTPS_PID_DURABILITY         (0x001d)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::durabilityQosPolicy : DurabilityServiceQosPolicy
  Used for field Topic::durabilityQosPolicy : DurabilityServiceQosPolicy
 */
#define MIG_RTPS_PID_DURABILITY_SERVICE (0x001e)

/*e \ingroup MIGRtpsPidModule
  Used for field Subscription::deadlineQosPolicy : DeadlineQosPolicy
 */
#define MIG_RTPS_PID_DEADLINE         (0x0023)

/*e \ingroup MIGRtpsPidModule
  Used for field Subscription::latencyBudgetQosPolicy : LatencyBudgetQosPolicy
 */
#define MIG_RTPS_PID_LATENCY_BUDGET         (0x0027)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::livelinessQosPolicy : LivelinessQosPolicy
  Used for field Subscription::livelinessQosPolicy : LivelinessQosPolicy
 */
#define MIG_RTPS_PID_LIVELINESS         (0x001b)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::reliabilityOffered : RTI_UINT32
  Used for field Subscription::reliabilityRequested : RTI_UINT32
 */
#define MIG_RTPS_PID_RELIABILITY        (0x001a)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::lifespanQosPolicy : LifespanQosPolicy
 */
#define MIG_RTPS_PID_LIFESPAN           (0x002b)

/*e \ingroup MIGRtpsPidModule
  Used for field Subscription::destinationOrderQosPolicy : DestinationOrderQosPolicy
 */
#define MIG_RTPS_PID_DESTINATION_ORDER (0x0025)

/*e \ingroup MIGRtpsPidModule
  Used for field Topic::history : HistoryQosPolicy
 */
#define MIG_RTPS_PID_HISTORY           (0x0040)

/*e \ingroup MIGRtpsPidModule
  Used for field Topic::resourceLimits : ResourceLimitsQosPolicy
 */
#define MIG_RTPS_PID_RESOURCE_LIMITS   (0x0041)

/*e \ingroup MIGRtpsPidModule
  Used for field Subscription::ownershipQosPolicy : OwnershipQosPolicy
 */
#define MIG_RTPS_PID_OWNERSHIP         (0x001f)

/*e \ingroup MIGRtpsPidModule
  Used for field Publication::strength : long
*/
#define MIG_RTPS_PID_OWNERSHIP_STRENGTH (0x0006)

/*e \ingroup MIGRtpsPidModule
  Used for field Subscription::presentationQosPolicy : PresentationQosPolicy
 */
#define MIG_RTPS_PID_PRESENTATION         (0x0021)

/*e \ingroup MIGRtpsPidModule
  Used for field Subscription::partitionQosPolicy : PartitionQosPolicy
 */
#define MIG_RTPS_PID_PARTITION         (0x0029)

/*e \ingroup MIGRtpsPidModule
  Used for field Subscription::minimumSeparation : NtpTime, AKA
  Used for field Subscription::time_based_filter : TimeBasedFilterQosPolicy
 */
#define MIG_RTPS_PID_TIME_BASED_FILTER (0x0004)

/*e \ingroup MIGRtpsPidModule
  
 */
#define MIG_RTPS_PID_TRANSPORT_PRIORITY (0x0049)

/*e \ingroup MIGRtpsPidModule
  Used for field Application::protocolVersion : ProtocolVersion
 */
#define MIG_RTPS_PID_PROTOCOL_VERSION                (0x0015)

/*e \ingroup MIGRtpsPidModule
  Used for field Application::vendorId : VendorId
 */
#define MIG_RTPS_PID_VENDOR_ID                       (0x0016)

#define MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX   (4)

/*e \ingroup MIGRtpsPidModule
  Array of max size MIG_RTPS_PID_USERDATA_UNICAST_IPADDRESS_COUNT_MAX
 */
#define MIG_RTPS_PID_UNICAST_LOCATOR6     (0x002f)

/*e \ingroup MIGRtpsPidModule
  Array of max size MIG_RTPS_PID_USERDATA_MULTICAST_IPADDRESS_COUNT_MAX
 */
#define MIG_RTPS_PID_MULTICAST_LOCATOR6     (0x0030)

/*e \ingroup MIGRtpsPidModule
  Array of max size MIG_RTPS_PID_USERDATA_MULTICAST_IPADDRESS_COUNT_MAX
  Used for field Reader::userdataMulticastIPAddressList : IPAddress[]
 */
#define MIG_RTPS_PID_MULTICAST_IPADDRESS    (0x0011)

/*e \ingroup MIGRtpsPidModule
  Array of max size MIG_RTPS_PID_USERDATA_UNICAST_IPADDRESS_COUNT_MAX
 */
#define MIG_RTPS_PID_DEFAULT_UNICAST_LOCATOR6     (0x0031)

/*e \ingroup MIGRtpsPidModule
  Array of max size MIG_RTPS_PID_USERDATA_UNICAST_IPADDRESS_COUNT_MAX
 */
#define MIG_RTPS_PID_METATRAFFIC_UNICAST_LOCATOR6 (0x0032)

/*e \ingroup MIGRtpsPidModule
  Array of max size MIG_RTPS_PID_USERDATA_MULTICAST_IPADDRESS_COUNT_MAX
 */
#define MIG_RTPS_PID_METATRAFFIC_MULTICAST_LOCATOR6 (0x0033)

/*e \ingroup MIGRtpsPidModule
  Array of max size MIG_RTPS_PID_APP_IPADDRESS_COUNT_MAX
  Used for field Application::unicastIPAddressList : IPAddress[]
 */
#define MIG_RTPS_PID_DEFAULT_UNICAST_IPADDRESS     (0x000c)

/*e \ingroup MIGRtpsPidModule
  Used for field Application::userdataUnicastPort :Port
 */
#define MIG_RTPS_PID_DEFAULT_UNICAST_PORT          (0x000e)

/*e \ingroup MIGRtpsPidModule
  Used for field ParticipantProxy::metatrafficUnicastLocatorList.address : IPv4Address_t
 */
#define MIG_RTPS_PID_METATRAFFIC_UNICAST_IPADDRESS (0x0045)

/*e \ingroup MIGRtpsPidModule
  Used for field Application::metatrafficUnicastPort : Port
 */
#define MIG_RTPS_PID_METATRAFFIC_UNICAST_PORT     (0x000d)

/*e \ingroup MIGRtpsPidModule
  Array of max size MIG_RTPS_PID_METATRAFFIC_MULTICAST_IPADDRESS_COUNT_MAX
  Used for field Application::metatrafficMulticastIPAddressList: IPAddress[]
 */
#define MIG_RTPS_PID_METATRAFFIC_MULTICAST_IPADDRESS (0x000b)

/*e \ingroup MIGRtpsPidModule
 */
#define MIG_RTPS_PID_METATRAFFIC_MULTICAST_PORT (0x0046)

/*e \ingroup MIGRtpsPidModule
  boolean
 */
#define MIG_RTPS_PID_EXPECTS_INLINE_QOS         (0x0043)

/*e \ingroup MIGRtpsPidModule
  Used for field DomainParticipant::manualLivelinessCount : unsigned long
 */
#define MIG_RTPS_PID_MANUAL_LIVELINESS_COUNT    (0x0034)

/*e \ingroup MIGRtpsPidModule
 */
#define MIG_RTPS_PID_PARTICIPANT_BUILTIN_ENDPOINTS (0x0044)

/*e \ingroup MIGRtpsPidModule
  Used for field SDPParticipantData::leaseDuration NtpTime
*/
#define MIG_RTPS_PID_LEASE_DURATION                (0x0002)

/*e \ingroup MIGRtpsPidModule
  DEPRECATED: Used for TypeCode information
*/
#define MIG_RTPS_PID_TYPECODE                      (0x0047)

/*e \ingroup MIGRtpsPidModule
  Used for field Subscription::contentFilteredTopicProperties
 */
#define MIG_RTPS_PID_CONTENT_FILTER_PROPERTY         (0x0035)

/*e \ingroup MIGRtpsPidModule
  Used for field Participant::propertyList
 */
#define MIG_RTPS_PID_PROPERTY_LIST                    (0x0059)

/*e \ingroup MIGRtpsPidModule
  Used for participant GUID
*/
#define MIG_RTPS_PID_PARTICIPANT_GUID                 (0x0050)

/*e \ingroup MIGRtpsPidModule
  Used for participant entity ID
*/
#define MIG_RTPS_PID_PARTICIPANT_ENTITY_ID            (0x0051)

/*e \ingroup MIGRtpsPidModule
  Used for GROUP GUID
*/
#define MIG_RTPS_PID_GROUP_GUID                       (0x0052)

/*e \ingroup MIGRtpsPidModule
  Used for Group OID
*/
#define MIG_RTPS_PID_GROUP_ENTITY_ID                 (0x0053)

/*e \ingroup MIGRtpsParameterSequenceClass
  Indicate filter signature inline Qos
*/
#define MIG_RTPS_PID_FILTER_SIGNATURE (0x0055)

/*e \ingroup MIGRtpsParameterSequenceClass
  Indicate coherent set inline Qos
*/
#define MIG_RTPS_PID_COHERENT_SET     (0x0056)

/*e \ingroup MIGRtpsParameterSequenceClass
  Indicate directed write inline Qos
*/
#define MIG_RTPS_PID_DIRECTED_WRITE     (0x0057)

/*e \ingroup MIGRtpsParameterSequenceClass
  Indicate builtin endpoint set inline Qos, 
  PID_BUILTIN_ENDPOINT_SET from the RTPS spec
*/
#define MIG_RTPS_PID_BUILTIN_ENDPOINT_MASK     (0x0058)

/*e \ingroup MIGRtpsParameterSequenceClass
*/
#define MIG_RTPS_PID_ENDPOINT_GUID    (0x005A)

/*e \ingroup MIGRtpsParameterSequenceClass
  Indicate max sample serialized size inline Qos
*/
#define MIG_RTPS_PID_MAX_SAMPLE_SERIALIZED_SIZE     (0x0060)

/*e \ingroup MIGRtpsParameterSequenceClass
  Indicate entity name inline Qos
*/
#define MIG_RTPS_PID_ENTITY_NAME     (0x0062)

/*e \ingroup MIGRtpsParameterSequenceClass
*/
#define MIG_RTPS_PID_KEY_HASH     (0x0070)

/*e \ingroup MIGRtpsParameterSequenceClass
*/
#define MIG_RTPS_PID_STATUS_INFO    (0x0071)

#define MIG_RTPS_PID_SEND_QUEUE_SIZE_DEPRECATED                 (0x0013)

/*e \ingroup MIGRtpsClass */
typedef RTI_UINT32 MIGRtpsHostId;

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_HOST_ID_UNKNOWN (0x00000000)

/*e \ingroup MIGRtpsClass */
typedef RTI_UINT32 MIGRtpsAppId;
/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_APP_ID_UNKNOWN (0x00000000)

/*e \ingroup MIGRtpsClass */
typedef RTI_UINT32 MIGRtpsInstanceId;
/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_INSTANCE_ID_UNKNOWN (0x00000000)

/*e \ingroup MIGRtpsClass */
typedef RTI_UINT32 MIGRtpsObjectId;
/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_OBJECT_ID_UNKNOWN (0x00000000)
#define MIG_RTPS_OBJECT_ID_MAX     (0xFFFFFFFF)

/*e \ingroup MIGRtpsClass */
struct MIGRtpsGuidPrefix
{
    /*e */
    MIGRtpsHostId hostId;
    /*e */
    MIGRtpsAppId appId;
    /*e */
    MIGRtpsInstanceId instanceId;

};

typedef struct MIGRtpsGuidPrefix MIGRtpsParticipantId;

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_GUID_PREFIX_UNKNOWN { \
    MIG_RTPS_HOST_ID_UNKNOWN,   \
    MIG_RTPS_APP_ID_UNKNOWN,    \
    MIG_RTPS_INSTANCE_ID_UNKNOWN}

/*RTPS.2.0*/
/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_GUID_PREFIX_SIZE (12)

/*e \ingroup MIGRtpsClass */
struct MIGRtpsGuid
{
    /*e */
    struct MIGRtpsGuidPrefix prefix;
    /*e */
    MIGRtpsObjectId objectId;
};

/*e \ingroup MIGRtpsClass */
#define MIG_RTPS_GUID_UNKNOWN {   \
    MIG_RTPS_GUID_PREFIX_UNKNOWN,   \
    MIG_RTPS_OBJECT_ID_UNKNOWN}

/*e \ingroup DISCRtpsObjectIdModule
  Writer of Simple Discovery Protocol Participant variable
  (builtinSdpParticipantWriter) */
#define MIG_RTPS_OBJECT_ID_PARTICIPANT             (0x000001C1)
#define MIG_RTPS_OBJECT_ID_WRITER_SDP_PARTICIPANT  (0x000100C2)
#define MIG_RTPS_OBJECT_ID_WRITER_SDP_PUBLICATION  (0x000003C2)
#define MIG_RTPS_OBJECT_ID_WRITER_SDP_SUBSCRIPTION (0x000004C2)

/*e \ingroup DISCRtpsObjectIdModule
  Reader of Simple Discovery Protocol Participant variable
  (builtinSdpParticipantWriter) */
#define MIG_RTPS_OBJECT_ID_READER_SDP_PARTICIPANT  (0x000100C7)
#define MIG_RTPS_OBJECT_ID_READER_SDP_PUBLICATION  (0x000003C7)
#define MIG_RTPS_OBJECT_ID_READER_SDP_SUBSCRIPTION (0x000004C7)

#ifdef RTI_ENDIAN_LITTLE
#define RTPS_PARTICIPANT_ID 0xc1010000
#else
#define RTPS_PARTICIPANT_ID 0x000001c1
#endif

typedef RTI_UINT16 MIGRtpsDataFlags;
#define MIG_RTPS_DATA_FLAGS_NONE (0)

#define MIG_RTPS_DATA_INLINE_QOS_OFFSET_DEFAULT (16)
#define MIG_RTPS_KEY_HASH_MAX_LENGTH 16

typedef RTI_UINT8 RTICdrOctet;

struct MIGRtpsKeyHash
{
    RTICdrOctet value[MIG_RTPS_KEY_HASH_MAX_LENGTH];

    RTI_UINT32 length;
};

typedef struct MIGRtpsKeyHash DDS_KeyHash_t;

#define MIG_RTPS_KEY_HASH_NIL {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#define MIG_RTPS_KEY_HASH_DEFAULT {     \
MIG_RTPS_KEY_HASH_NIL,  /* value */\
MIG_RTPS_KEY_HASH_MAX_LENGTH/* length */\
}

extern RTPSDllVariable const char
    *RTPS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME;
extern RTPSDllVariable const char *RTPS_PARTICIPANT_BUILTIN_TOPIC_NAME;

extern RTPSDllVariable const char
    *RTPS_PUBLICATION_BUILTIN_TOPIC_TYPE_NAME;

extern RTPSDllVariable const char *RTPS_PUBLICATION_BUILTIN_TOPIC_NAME;

extern RTPSDllVariable const char
    *RTPS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME;

extern RTPSDllVariable const char *RTPS_SUBSCRIPTION_BUILTIN_TOPIC_NAME;

/* forward declaration */
struct REDA_Buffer;

/*e \ingroup MIGRtpsClass
  may be macroed for performance
  @pre valid args
  @see REDA_toStringBufferFunc MIG_RTPS_GUID_STRING_LENGTH
*/
RTPSDllExport const char*
MIGRtpsGuid_toString(const void *guid, struct REDA_Buffer *string);

/*e \ingroup MIGRtpsClass
  @see MIGRtpsGuid_toString
*/
#define MIG_RTPS_GUID_STRING_LENGTH (34)

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
RTPSDllExport void
MIGRtpsGuid_copy(
        struct MIGRtpsGuid *me,
        struct MIGRtpsGuid *src);

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
RTPSDllExport RTI_BOOL
MIGRtpsGuid_equals(struct MIGRtpsGuid *a, struct MIGRtpsGuid *b);

/*e \ingroup MIGRtpsClass
 may be macroed for performance
 */
RTPSDllExport RTI_BOOL
MIGRtpsGuid_prefix_equals(
        struct MIGRtpsGuid *a,
        struct MIGRtpsGuid *b);

/*e \ingroup MIGRtpsClass
 may be macroed for performance
 */
RTPSDllExport RTI_BOOL
MIGRtpsGuid_suffix_equals(
        struct MIGRtpsGuid *a,
        struct MIGRtpsGuid *b);

/*e \ingroup MIGRtpsClass */
typedef RTI_UINT16 MIGRtpsProtocolVersion;

/*e \ingroup MIGRtpsClass
  \brief Version of protocol this MIG module supports.
*/
/*#define MIG_RTPS_PROTOCOL_VERSION (0x0101) */
#define MIG_RTPS_PROTOCOL_VERSION (0X0200)


/*e \ingroup MIGRtpsClass
  \brief 1.0 version of protocol.

  This definition must always be defined so we always have a default value
  in the case that it is not specified by a remote node.
*/
#define MIG_RTPS_PROTOCOL_VERSION_1_0 (0x0100)
#define MIG_RTPS_PROTOCOL_VERSION_2_0 (0X0200)

/*e \ingroup MIGRtpsClass
  \brief Major version of protocol this MIG module supports.
*/
/* #define MIG_RTPS_PROTOCOL_VERSION_MAJOR (0x01) */
#define MIG_RTPS_PROTOCOL_VERSION_MAJOR (0x02)

/* TODO: This is duplicated in a higher level */
/*e \ingroup MIGRtpsClass
  \brief Minor version of protocol this MIG module supports.
*/
#define MIG_RTPS_PROTOCOL_VERSION_MINOR (0x01)  /* is 0x01 in NDDS 4.0 */

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
MIGRtpsProtocolVersion
MIGRtpsProtocolVersion_getMajor(MIGRtpsProtocolVersion *me);

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
MIGRtpsProtocolVersion
MIGRtpsProtocolVersion_getMinor(MIGRtpsProtocolVersion *me);

/*e \ingroup MIGRtpsClass */
typedef RTI_UINT16 MIGRtpsVendorId;

/*e \ingroup MIGRtpsClass
  RTI vendor ID major.
*/
#define MIG_RTPS_VENDOR_ID_MAJOR (0x01)

/*e \ingroup MIGRtpsClass
  RTI vensor ID minor.
*/
#define MIG_RTPS_VENDOR_ID_MINOR (0x01) /* note: NDDS 4.0 had 0x00 */

/*e \ingroup MIGRtpsClass
  Unknown/undeclared vendor.
*/
#define MIG_RTPS_VENDOR_ID_UNKNOWN (0x0000)

/*e \ingroup MIGRtpsClass
  RTI vendor ID
*/
#define MIG_RTPS_VENDOR_ID_RTI (0x0101)

#define MIG_RTPS_VENDOR_ID ((MIG_RTPS_VENDOR_ID_MAJOR << 8) | MIG_RTPS_VENDOR_ID_MINOR)

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
MIGRtpsVendorId
MIGRtpsVendorId_getMajor(MIGRtpsVendorId *me);

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
MIGRtpsVendorId
MIGRtpsVendorId_getMinor(MIGRtpsVendorId *me);

/*e \ingroup MIGRtpsClass
  \brief RTPS submessage types.
*/
typedef enum
{
    /*e padding */
    MIG_RTPS_PAD = 0x01,
    /*e CST variable */
    MIG_RTPS_VAR = 0x02,
    /*e P/S issue */
    MIG_RTPS_ISSUE = 0x03,
    /*e acknowledgement */
    MIG_RTPS_ACK = 0x06,
    /*e heartbeat */
    MIG_RTPS_HEARTBEAT = 0x07,
    /*e gap */
    MIG_RTPS_GAP = 0x08,
    /*e timestamp */
    MIG_RTPS_INFO_TS = 0x09,
    /*e message source */
    MIG_RTPS_INFO_SRC = 0x0c,
    /*e how to reply to sender using IPv4 */
    MIG_RTPS_INFO_REPLY_IP4 = 0x0d,     /*RTPS.2.0 */
    /*e destination of the message; intended recipient */
    MIG_RTPS_INFO_DST = 0x0e,
    /*e how to reply to sender using IPv6 */
    MIG_RTPS_INFO_REPLY = 0x0f, /*RTPS.2.0 */
    /*e identifier for the data fragments submessage */
    MIG_RTPS_DATA_FRAG = 0x10,  /*RTPS.2.0 */
    /*e identifier for the no key data fragments submessage */
    MIG_RTPS_NOKEY_DATA_FRAG = 0x11,    /*RTPS.2.0 */
    /*e identifier for the data fragments nack submessage */
    MIG_RTPS_NACK_FRAG = 0x12,  /*RTPS.2.0 */
    /*e identifier for the data fragments heartbeat submessage */
    MIG_RTPS_HEARTBEAT_FRAG = 0x13,     /*RTPS.2.0 */
    /*e data */
    MIG_RTPS_DATA = 0x15,       /* RTPS 2.1 */
    /*e data */
    MIG_RTPS_DATA_FRAG_X = 0x16,        /* RTPS 2.1 */
    /*e batch ACKNACK */
    MIG_RTPS_ACK_BATCH = 0x17,  /* RTPS 2.1 */
    /*e identifier for the data batch submessage */
    MIG_RTPS_DATA_BATCH = 0x18, /*RTPS.2.1 */
    /*e batch heartbeat */
    MIG_RTPS_HEARTBEAT_BATCH = 0x19     /*RTPS.2.1 */
} MIGRtpsSubmessageId;


/*e \ingroup MIGRtpsClass
  \brief Number of vendor neutral, common RTPS submessages.
  may be macroed for performance
*/
#define MIG_RTPS_SUBMESSAGE_ID_COUNT_MAX (26)

/*e \ingroup MIGRtpsClass
  \brief Get name for diagnosis

  may be macroed for performance
*/
const char*
MIGRtpsSubmessageId_toString(MIGRtpsSubmessageId id);

/*e \defgroup MIGRtpsSubmessageVarStatusInfoClass MIGRtpsSubmessageVarStatusInfo
  \ingroup MIGRtpsClass
  \brief VAR submessages may have status info bitfield, which impart more meaning
  to the contents.
*/
/*e \ingroup MIGRtpsSubmessageVarStatusInfoClass */
typedef RTI_UINT32 MIGRtpsStatusInfo;

/*e \ingroup MIGRtpsSubmessageVarStatusInfoClass */
#define MIG_RTPS_NO_STATUS_INFO (0x00000000)
#define MIG_RTPS_DISPOSE_STATUS_INFO (0x00000001)
#define MIG_RTPS_UNREGISTER_STATUS_INFO (0X00000002)

/*e \defgroup MIGRtpsSubmessageFlagClass MIGRtpsSubmessageFlag
  \ingroup MIGRtpsClass
  \brief Submessages may have flag bitfield, which impart more meaning
  to the contents.
*/

/*e \ingroup MIGRtpsSubmessageFlagClass */
#define MIG_RTPS_NO_FLAG (0x00)

/*e \ingroup MIGRtpsSubmessageFlagClass
  \brief tells whether this is a big endian/little endian message
*/
#define MIG_RTPS_ENDIAN_FLAG (0x01)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_INFO_REPLY
*/
#define MIG_RTPS_INFO_REPLY_M_FLAG (0x02)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_INFO_TS
*/
#define MIG_RTPS_INFO_TS_I_FLAG (0x02)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_ACK
*/
#define MIG_RTPS_ACK_F_FLAG (0x02)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_HEARTBEAT
*/
#define MIG_RTPS_HEARTBEAT_F_FLAG (0x02)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_HEARTBEAT
*/
#define MIG_RTPS_HEARTBEAT_L_FLAG (0x04)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_DATA
*/
#define MIG_RTPS_DATA_D_FLAG (0x04)

#define MIG_RTPS_DATA_Q_FLAG (0x02)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_ISSUE
*/
#define MIG_RTPS_ISSUE_P_FLAG (0x02)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_VAR
*/
#define MIG_RTPS_VAR_P_FLAG (0x02)

/*e \ingroup MIGRtpsSubmessageFlagClass
  @see MIGRtpsSubmessageId#MIG_RTPS_VAR
*/
#define MIG_RTPS_VAR_A_FLAG (0x04)

/*e \ingroup MIGRtpsSubmessageFlagClass
   @see MIGRtpsSubmessageId#MIG_RTPS_VAR
*/
#define MIG_RTPS_VAR_H_FLAG (0x08)
#define MIG_RTPS_VAR_K_FLAG (0x08)

/*e \ingroup MIGRtpsSubmessageFlagClass
   @see MIGRtpsSubmessageId#MIG_RTPS_VAR
*/
#define MIG_RTPS_VAR_U_FLAG (0x10)

/*e \ingroup MIGRtpsSubmessageFlagClass
  Check if there is protocol parameters in the submessage.

  There is protocol parameter only if A XOR P is true.

  @param flags \b In. Submessage flags.

  @return RTI_TRUE if there is protocol parameters. RTI_FALSE otherwise.
*/
RTPSDllExport RTI_BOOL
MIGRtpsVarSubmessage_hasProtocolParameters(char flags);

/*e \ingroup MIGRtpsClass
  \brief Denotes the built-in object types

  RTPSObject is categorized into (normal/reserved) x (user/meta) quadrant.
  Within each quadrant, there are several built-in (provided for at the RTPS
  protocol level) objects, such as application, publication/subscription,
  CST writer/reader.
*/
typedef enum
{
    MIG_RTPS_OBJECT_NORMAL_USER_UNKNOWN = 0x00,
    MIG_RTPS_OBJECT_NORMAL_USER_APPLICATION = 0x01,
    /*e Use this suffix for writers of data with a key. */
    MIG_RTPS_OBJECT_NORMAL_USER_CST_WRITER = 0x02,
    /*e Use this suffix for writers of data with no key. */
    MIG_RTPS_OBJECT_NORMAL_USER_PUBLICATION = 0x03,
    /*e Use this suffix for readers of data with no key. */
    MIG_RTPS_OBJECT_NORMAL_USER_SUBSCRIPTION = 0x04,
    /*e Use this suffix for readers of data with a key. */
    MIG_RTPS_OBJECT_NORMAL_USER_CST_READER = 0x07,
    MIG_RTPS_OBJECT_NORMAL_USER_VIRTUAL_SUBSCRIPTION = 0x3C,
    MIG_RTPS_OBJECT_NORMAL_USER_VIRTUAL_CST_READER = 0x3D,
    MIG_RTPS_OBJECT_RESERVED_USER_UNKNOWN = 0x40,
    MIG_RTPS_OBJECT_RESERVED_USER_APPLICATION = 0x41,
    MIG_RTPS_OBJECT_RESERVED_USER_CST_WRITER = 0x42,
    MIG_RTPS_OBJECT_RESERVED_USER_PUBLICATION = 0x43,
    MIG_RTPS_OBJECT_RESERVED_USER_SUBSCRIPTION = 0x44,
    MIG_RTPS_OBJECT_RESERVED_USER_CST_READER = 0x47,
    MIG_RTPS_OBJECT_NORMAL_META_UNKNOWN = 0x80,
    MIG_RTPS_OBJECT_NORMAL_META_APPLICATION = 0x81,
    MIG_RTPS_OBJECT_NORMAL_META_CST_WRITER = 0x82,
    MIG_RTPS_OBJECT_NORMAL_META_PUBLICATION = 0x83,
    MIG_RTPS_OBJECT_NORMAL_META_SUBSCRIPTION = 0x84,
    MIG_RTPS_OBJECT_NORMAL_META_CST_READER = 0x87,
    MIG_RTPS_OBJECT_RESERVED_META_UNKNOWN = 0xc0,
    MIG_RTPS_OBJECT_RESERVED_META_APPLICATION = 0xc1,
    MIG_RTPS_OBJECT_RESERVED_META_CST_WRITER = 0xc2,
    MIG_RTPS_OBJECT_RESERVED_META_PUBLICATION = 0xc3,
    MIG_RTPS_OBJECT_RESERVED_META_SUBSCRIPTION = 0xc4,
    MIG_RTPS_OBJECT_RESERVED_META_CST_READER = 0xc7
} MIGRtpsObjectSuffix;


/* ===========================================================================*/
/*                              RTPS types                                    */
/* ===========================================================================*/
/*e \dref_Locator_t_ADDRESS_LENGTH_MAX */
#define RTPS_LOCATOR_ADDRESS_LENGTH_MAX 16

/*e \dref_Locator_t
 */
struct RTPS_Locator_t
{
    /*e \dref_Locator_t_kind
     */
    RTI_INT32 kind;
    /*e \dref_Locator_t_port
     */
    RTI_UINT32 port;
    /*e \dref_Locator_t_address
     */
    RTI_UINT8 address[RTPS_LOCATOR_ADDRESS_LENGTH_MAX];
};


#define RTPS_LOCATOR_DEFAULT {\
-1,                               /* kind    */\
0,                                /* port    */\
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} /* address */\
}
#define RTPS_Locator RTPS_Locator_t

#define RTPS_Locator_is_udpv4(__locator) \
    (__locator->kind == RTPS_LOCATOR_KIND_UDPv4)



/*e \dref_DestinationKey
 */
struct RTPS_DestinationKey
{
    MIGRtpsObjectId object_id;
    struct RTPS_Locator locator;
    struct MIGRtpsGuid remote_guid;
};

struct RTPS_DestinationRecord
{
    struct RTPS_DestinationKey destination;

    /*TODO: This should be typedef'ed into a destination handle of some sorts */
    RTI_INT32 destination_id;
    RTI_INT32 ref_count;
};



/*e \dref_Locator_t
 */
struct RTPS_LocatorUdpv4_t
{
    /*e \dref_Locator_t_kind
     */
    RTI_INT32 kind;
    /*e \dref_Locator_t_port
     */
    RTI_UINT32 port;
    /*e \dref_Locator_t_address
     */
    RTI_UINT32 address;
};
#define RTPS_LOCATOR_UDPV4_DEFAULT {\
-1,                               /* kind    */\
0,                                /* port    */\
0                                 /* address */\
}

/*e
 * \dref_Locator_t_INVALID
 */
extern RTPSDllVariable const struct RTPS_Locator_t RTPS_LOCATOR_INVALID;

/*e
 * \dref_Locator_t_KIND_INVALID
 */
extern RTPSDllVariable const RTI_INT32 RTPS_LOCATOR_KIND_INVALID;

/*e
 * \dref_Locator_t_PORT_INVALID
 */
extern RTPSDllVariable const RTI_UINT32 RTPS_LOCATOR_PORT_INVALID;

/*e
 * \dref_Locator_t_ADDRESS_INVALID
 */
extern RTPSDllVariable const RTI_UINT8
    RTPS_LOCATOR_ADDRESS_INVALID[RTPS_LOCATOR_ADDRESS_LENGTH_MAX];

/*e
 * \dref_Locator_t_KIND_UDPv4
 */
extern RTPSDllVariable const RTI_INT32 RTPS_LOCATOR_KIND_UDPv4;

/*e
 * \dref_Locator_t_KIND_UDPv6
 */
extern RTPSDllVariable const RTI_INT32 RTPS_LOCATOR_KIND_UDPv6;

/*e
 * \dref_Locator_t_KIND_RESERVED
 */
extern RTPSDllVariable const RTI_INT32 RTPS_LOCATOR_KIND_RESERVED;

/*e
 * \dref_Locator_t_KIND_SHMEM
 */
extern RTPSDllVariable const RTI_INT32 RTPS_LOCATOR_KIND_SHMEM;

extern RTPSDllVariable const char
                *MIG_RTPS_SUBMESSAGE_ID_NAME[MIG_RTPS_SUBMESSAGE_ID_COUNT_MAX];

/* ===========================================================================*/
/*                           Rtps Object Bits                                 */
/* ===========================================================================*/

/*e \ingroup MIGRtpsClass
  \brief Meta traffic bit.
*/
#define MIG_RTPS_OBJECT_KIND_M_BIT (0x80)

/*e \ingroup MIGRtpsClass
  \brief Reserved object bit.
*/
#define MIG_RTPS_OBJECT_KIND_R_BIT (0x40)

/*e \ingroup MIGRtpsClass
  \brief Denotes reader or writer type.
*/
typedef enum
{
    MIG_RTPS_RW_TYPE_UNKNOWN,
    MIG_RTPS_RW_TYPE_READER,
    MIG_RTPS_RW_TYPE_WRITER
} MIGRtpsRWType;

/*e \ingroup MIGRtpsClass
  \brief Denotes reader or writer type.
*/
typedef enum
{
    MIG_RTPS_SAMPLE_TYPE_UNKNOWN,
    MIG_RTPS_SAMPLE_TYPE_CST,
    MIG_RTPS_SAMPLE_TYPE_PS
} MIGRtpsSampleType;

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
RTPSDllExport MIGRtpsRWType
MIGRtpsObjectSuffix_getRWType(MIGRtpsObjectSuffix osx);

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
RTPSDllExport MIGRtpsRWType
MIGRtpsObjectSuffix_getSampleType(MIGRtpsObjectSuffix osx);

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
RTPSDllExport MIGRtpsObjectSuffix
MIGRtpsObjectId_getObjectSuffix(MIGRtpsObjectId oid);

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
RTPSDllExport
MIGRtpsObjectId MIGRtpsObjectId_generate(
        int instanceId,
        MIGRtpsObjectSuffix osx);

/*e \ingroup MIGRtpsClass
  may be macroed for performance
*/
RTPSDllExport MIGRtpsObjectSuffix
MIGRtpsGuid_getObjectSuffix(struct MIGRtpsGuid *me);

/* forward declaration */
struct REDA_SequenceNumber;

/*e \defgroup MIGRtpsSequenceNumberClass MIGRtpsSequenceNumber
  \ingroup MIGRtpsClass
  \brief A logical derived class of \ref REDA_SequenceNumber, which exists solely
  to supply helper methods to deal with RTPS protocol aspect.
*/

/*e \ingroup MIGRtpsSequenceNumberClass
  \brief Read bits into the sequence number.

  @pre valid args
  @post sequenceNumber is packed and stream pointer is moved forward.

  @param me \b out. "this". Read into here.

  @param streamPtr \b InOut. Raw bytes containing the sequence number
  to be read.

  @param needBytesSwap \b In. RTI_TRUE if byte swapping is necessary,
  RTI_FALSE otherwise.

  @return RTI_TRUE on success, RTI_FALSE otherwise.

  @mtsafety Unsafe for read during modification.  Unsafe for concurrent
  modification unless the args are unchanging.
*/
/*
Public RTPSDllExport RTI_BOOL
MIGRtpsSequenceNumber_deserialize(struct REDA_SequenceNumber *me,
                  const char **stream, RTI_BOOL needByteSwap);
*/
/*e \ingroup MIGRtpsSequenceNumberClass
  \brief Calculate the distance between 2 sequence numbers.

  @pre valid args

  @return The distance, possibly saturated to
  MIG_RTPS_BITMAP_DISTANCE_MAX.  -1 on failure.

  @mtsafety unsafe if either sequence number modified concurrently.
*/
RTPSDllExport int
MIGRtpsSequenceNumber_getDistance(
        const struct REDA_SequenceNumber *s1,
        const struct REDA_SequenceNumber *s2);

/*e \ingroup MIGRtpsBitmapClass */
#define MIG_RTPS_BITMAP_32BITS_ARRAY_SIZE_MAX (8)       /* 256/32 = 8 */

/*e \defgroup MIGRtpsBitmapClass MIGRtpsBitmap
 \ingroup MIGRtpsClass
*/
struct MIGRtpsBitmap
{
    /* lead sequence */
    struct REDA_SequenceNumber _lead;
    /* The size of the bitmap in the number of bits. */
    RTI_INT32 _bitCount;
    /* The bitmap stored as an array of integers.
     * NOTE1: This must be unsigned in order to apply the >> operator.
     * NOTE2: There are two approaches to handle bits outside bitCount:
     * 1) They must always be zeroed.
     * 2) They can be anything.
     * The first approach puts the burden on otherwise simple
     * methods such as truncate to zero out truncated bits, whereas
     * the second approach puts the burden on methods such as shift
     * to make sure bits beyond the bitCount are not shifted in.
     * In our implementation, we have opted for the second approach. */
    RTI_UINT32 _32bits[MIG_RTPS_BITMAP_32BITS_ARRAY_SIZE_MAX];
};

/*e \ingroup MIGRtpsBitmapClass */
#define MIG_RTPS_BITMAP_SIZE_MAX              (256)

/*e \ingroup MIGRtpsBitmapClass
  Can only handle distance that doesn't overflow with int anyway.
*/
#define MIG_RTPS_BITMAP_DISTANCE_MAX          (0x7FFFFFFF)

/*e \ingroup MIGRtpsBitmapClass
  may be macroed for performance
  @pre valid arg
  @return Number of bytes if serialize were to be called
*/
int
MIGRtpsBitmap_getSerializedSize(const struct MIGRtpsBitmap *me);


/*e \ingroup MIGRtpsBitmapClass
  may be macroed for performance
  @pre valid arg
  @return Pointer to the lead sequence number.
  @mtsafety The returned pointer itself is always valid and MT safe, but
  the content may be unsafe if modified concurrently.
*/
struct REDA_SequenceNumber*
MIGRtpsBitmap_getLead(const struct MIGRtpsBitmap *me);

/*e \ingroup MIGRtpsBitmapClass
  may be macroed for performance
  @pre valid arg
  @return Number of bits the bitmap is describing
  @mtsafety unsafe if modified concurrently.
*/
RTI_INT32
MIGRtpsBitmap_getBitCount(const struct MIGRtpsBitmap *me);

/*e \ingroup MIGRtpsBitmapClass
  may be macroed for performance
  @pre valid args
  @post All bits are reset to zero.

  @param seqNum \b In. The new lead sequence number.
  @param bitCount \b In. The number of valid bits.
  Must be <= MIG_RTPS_BITMAP_SIZE_MAX.

  @mtsafety unsafe if modified or read concurrently.
*/
RTPSDllExport void
MIGRtpsBitmap_reset(
        struct MIGRtpsBitmap *me,
        const struct REDA_SequenceNumber *seqNum,
        RTI_INT32 bitCount);

/*e \ingroup MIGRtpsBitmapClass

  \brief Set specified bit in bitmap.

  @pre valid args
  @post The bit corresponding to the input sequence number is set to either
  0 or 1 as specified in input arg.

  @param num \b In. The sequence number of interest.
  @param bit \b In. RTI_TRUE if bit is to be turned on, RTI_FALSE otherwise.

  @return RTI_FALSE on failure, which includes the sequence not in bitmap.
  RTI_TRUE on success.

  @exception RTI_LOG_PRECONDITION_FAILURE

  @mtsafety unsafe.
*/
RTPSDllExport RTI_BOOL
MIGRtpsBitmap_setBit(
        struct MIGRtpsBitmap *me,
        const struct REDA_SequenceNumber *num,
        RTI_BOOL bit);

/*e \ingroup MIGRtpsBitmapClass

  \brief Get specified bit in bitmap.

  @pre valid args
  @post the bit out param is filled with logical value of bit of interest.

  @param bit \b Out. Pointer to the answer, whose value will be RTI_TRUE
  if bit is on and RTI_FALSE otherwise.
  @param num \b In. The sequence number of interest.

  @return RTI_FALSE on failure, which includes the sequence not in bitmap.
  RTI_TRUE on success.

  @exception RTI_LOG_PRECONDITION_FAILURE

  @mtsafety unsafe if modified concurrently.
*/
RTPSDllExport RTI_BOOL
MIGRtpsBitmap_getBit(
        const struct MIGRtpsBitmap *me,
        RTI_BOOL *bit,
        const struct REDA_SequenceNumber *num);

/*e \ingroup MIGRtpsBitmapClass

  Finds the first ocurrence of the specified bit in a bitmap.

  @pre valid args
  @post the position out param is filled with the sequence number.

  @param position \b Out. The position where the searchBit was first found.
  This will be the last sequence number contained in the bitmap + 1 if the
  searchBit is not found.
  @param searchBit \b In. The bit to look for.

  @return RTI_FALSE if the searchBit was not found, RTI_TRUE if found.

  @mtsafety unsafe if modified concurrently.
*/
RTPSDllExport RTI_BOOL
MIGRtpsBitmap_getFirstBit(
        const struct MIGRtpsBitmap *me,
        struct REDA_SequenceNumber *position,
        RTI_BOOL searchBit);

/*e \ingroup MIGRtpsBitmapClass

  \brief Fills the bitmap with the specified bit from [firstSeqNum, lastSeqNum].

  @pre valid args.
  @post Bitmap is adjusted.

  @param firstSeqNum \b In. The sequence number from which to start the fill.
  @param lastSeqNum \b In. The sequence number at which to end the fill.
  @param bit \b In. RTI_TRUE if bits are to be turned on, RTI_FALSE otherwise.

  @return RTI_TRUE on success, RTI_FALSE otherwise such as specifying a
  last sequence number smaller than the first sequence number.  However,
  specifying sequence numbers outside the range of the bitmap is valid.

  @mtsafety unsafe.
*/
RTPSDllExport RTI_BOOL
MIGRtpsBitmap_fill(
        struct MIGRtpsBitmap *me,
        const struct REDA_SequenceNumber *firstSeqNum,
        const struct REDA_SequenceNumber *lastSeqNum,
        RTI_BOOL bit);

/*e \ingroup MIGRtpsBitmapClass

  \brief Copies source bitmap into destination,
  but only specified number of bits.

  @pre valid args
  @post Bitmap is adjusted.

  @param source \b In. Source bitmap.
  @param length \b In. Number of bits to copy.  If length > bit count of
  source biwetmap, fill remainder with 0.

  @mtsafety unsafe.
*/
RTPSDllExport void
MIGRtpsBitmap_copy(
        struct MIGRtpsBitmap *me,
        const struct MIGRtpsBitmap *source,
        unsigned int length);

/*e \ingroup MIGRtpsBitmapClass

  \brief Merges source bitmap into destination.

  @pre valid args
  @post Bitmap is adjusted.

  @param source \b In. Source bitmap.

  @mtsafety unsafe.
*/
RTPSDllExport void
MIGRtpsBitmap_merge(
        struct MIGRtpsBitmap *me,
        const struct MIGRtpsBitmap *source);

/*e \ingroup MIGRtpsBitmapClass

  \brief Shifts bitmap forward only to specified sequence number,
  keeping bit count the same by filling in 0's as necessary.

  @post Bitmap is adjusted.  If seqNum == bitmap lead, noop.  If seqNum >
  any bit in bitmap, bitmap is 0 filled.

  @param seqNum \b In. New lead sequence number.

  @return RTI_TRUE on success, RTI_FALSE otherwise such as specifying a
  sequence number smaller than the current lead of the bitmap.

  @mtsafety unsafe.
*/
RTPSDllExport RTI_BOOL
MIGRtpsBitmap_shift(
        struct MIGRtpsBitmap *me,
        const struct REDA_SequenceNumber *seqNum);

/*e \ingroup MIGRtpsBitmapClass

  \brief Truncates the bitmap at the sepcified sequence number.

  @post Bitmap is adjusted.

  @param seqNum \b In. Throw away any bits whose sequence is greater than
  this. If seqNum < bitmap->_lead, bitmap's bitCount will be set to 0. If
  seqNum > bitmap->_lead, bitmap will remain unchanged.

  @return RTI_FALSE on failure, which includes the sequence not in bitmap.
  RTI_TRUE on success.

  @mtsafety unsafe.
*/
RTPSDllExport void
MIGRtpsBitmap_truncate(
        struct MIGRtpsBitmap *me,
        const struct REDA_SequenceNumber *seqNum);

/*e \ingroup MIGRtpsBitmapClass

  \brief Truncates the bitmap so that the number of zero it contains
  starting from startSn to the end (inclusive) equals zeroTotal.

  If the startSn is less than bitmap's lead, it is as if the startSn is the same
  as bitmaps's lead.
  If the startSn is larger than the last sn in bitmap, bitmap will remain
  unchanged.
  If the original bitmap doesn't have zeroTotal number of zeros between startSn
  and last sn, it will remain unchanged. Otherwise, it will be cut off just after
  the last zero that reaches the zeroTotal. That said, if the zeroTotal is zero,
  the bitmap will end at (startSn - 1), provided startSn is in bitmap range.

  @pre  valid param. totalZeros not negative.
  @post Bitmap is adjusted.

  @param startSn \b In. The sn from which to start counting zeros.
  @param totalZeros \b In. The total number of zeros that the truncated bitmap
  should have, counting from the startSn.

  @return RTI_FALSE on failure, which includes the sequence not in bitmap.
  RTI_TRUE on success.

  @mtsafety unsafe.
*/
RTPSDllExport void
MIGRtpsBitmap_truncateToZeroCount(
        struct MIGRtpsBitmap *me,
        const struct REDA_SequenceNumber *startSn,
        int totalZeros);

/*e \ingroup MIGRtpsBitmapClass

  \brief Inverts all bits of a bitmap.

  @post Bitmap is inverted.

  @mtsafety unsafe.
*/
RTPSDllExport void
MIGRtpsBitmap_invert(struct MIGRtpsBitmap *me);

/*e \ingroup MIGRtpsBitmapClass

  \brief Compresses bitmap changing the number of bits to eliminate trailing "1"
  bits.

  This function only changes the bitCount of the bitmap. It does not
  shift or change any bits. The purpose is to eliminate any trailing
  "1" bits which provide no information for many types of bitmaps.

  Simple implementation of RTPS protocol blindly acts on a bitmap in a
  sequential manner without looking ahead at higher sequence bits. For
  implementations whose behavior will be logically same for
  0111...111 as for 0, other than shorter execution time, by not
  processing the unnecessary 1's, this method cuts off the bitmap
  ahead of time before acting on it.

  As this functionality is provided only for optimization, i.e. an
  implementation of RTPS should behave correctly without using this,
  for efficiency the algorithm does not eliminate all trailing "1" bits
  rather it removes them to the closest 8-bit boundary.

  @post Bitmap is adjusted.

  @mtsafety unsafe.
*/
RTPSDllExport void
MIGRtpsBitmap_compressTrailingOnes(struct MIGRtpsBitmap *me);

/*e \ingroup MIGRtpsBitmapClass
  \brief Read bits into the bitmap.

  @post sequenceNumber is packed and stream pointer is moved forward.

  @param streamPtr \b InOut. Raw bytes containing the bitmap to be read.
  @param needBytesSwap \b In. RTI_TRUE if byte swapping is necessary,
  RTI_FALSE otherwise.

  @return RTI_TRUE on success, RTI_FALSE otherwise.

  @mtsafety Unsafe for read during modification.  Unsafe for concurrent
  modification unless the args are unchanging.
*/

RTPSDllExport void
MIGRtpsBitmap_serialize(
        const struct MIGRtpsBitmap *me,
        char **streamPtr,
        RTI_BOOL needByteSwap);

RTPSDllExport RTI_BOOL
MIGRtpsBitmap_deserialize(
        struct MIGRtpsBitmap *me,
        const char **streamPtr,
        RTI_BOOL needByteSwap);


/*e \defgroup MIGRtpsParameterSequenceClass MIGRtpsParameterSequence
  \ingroup MIGModule
  \brief optional parameter sequence
  For detailed explanation, see RTPS protocol.
*/

/*e \ingroup MIGRtpsParameterSequenceClass
  Indicate termination of parameter sequence.
*/
#define MIG_RTPS_PID_SENTINEL (0x0001)

/*e \ingroup MIGRtpsParameterSequenceClass
  \brief skip until the end of parameter sequence

  @pre valid args
  @post stream is moved until the end of parameter sequence.

  @param streamPtr \b InOut. Raw bytes containing parameter sequence
  to be read.

  @param needBytesSwap \b In. RTI_TRUE if byte swapping is necessary,
  RTI_FALSE otherwise.

  @return RTI_TRUE on success, RTI_FALSE otherwise.

  @mtsafety Unsafe for read during modification.  Unsafe for concurrent
  modification unless the args are unchanging.
*/
RTPSDllExport RTI_BOOL
MIGRtpsParameterSequence_skip(
        const char **stream,
        RTI_BOOL needByteSwap);


/* TODO: Documentation */
RTPSDllExport RTI_BOOL
MIGRtpsGuid_serialize(
    struct CDR_Stream_t *stream,
    const struct MIGRtpsGuid *guid,
    void * param);

RTPSDllExport RTI_BOOL
MIGRtpsGuid_deserialize(
    struct CDR_Stream_t *stream,
    struct MIGRtpsGuid *guid,
    void * param);

RTPSDllExport RTI_UINT32
MIGRtpsGuid_getMaxSizeSerialized(RTI_UINT32 size);

/* -------------------------------------------------------------------------- */
typedef struct OSAPI_NtpTime RTINtpTime;

RTPSDllExport RTI_BOOL
MIGRtps_serializeRtiNtpTime(
    struct CDR_Stream_t *stream,
    const RTINtpTime *time,
    void * param);

RTPSDllExport RTI_BOOL
MIGRtps_deserializeRtiNtpTime(
    struct CDR_Stream_t *stream,
    RTINtpTime *time,
    void * param);

RTPSDllExport RTI_UINT32
MIGRtps_getRtiNtpTimeMaxSizeSerialized(RTI_UINT32 size);

RTPSDllExport RTI_BOOL
MIGRtps_serialize2Octets(
    struct CDR_Stream_t *stream,
    const RTI_UINT16 * in,
    void * param);

RTPSDllExport RTI_BOOL
MIGRtps_deserialize2Octets(
    struct CDR_Stream_t *stream,
    RTI_UINT16 *out,
    void * param);

RTPSDllExport RTI_UINT32
MIGRtps_get2OctetsMaxSizeSerialized(RTI_UINT32 size);

RTPSDllExport RTI_BOOL
MIGRtps_serializeIpv6Locator(
    struct CDR_Stream_t *stream,
    const struct RTPS_Locator_t *loc,
    void * param);

RTPSDllExport RTI_BOOL
MIGRtps_deserializeIpv6Locator(
    struct CDR_Stream_t *stream,
    struct RTPS_Locator_t *loc,
    void * param);

RTPSDllExport RTI_UINT32
MIGRtps_getIpv6LocatorMaxSizeSerialized(RTI_UINT32 size);

/* RTPS Locator */
RTPSDllExport RTPS_ReturnCode_t
RTPS_Locator_copy(struct RTPS_Locator_t *out, const struct RTPS_Locator_t *in);

RTPSDllExport RTPS_ReturnCode_t
RTPS_Locator_initialize(struct RTPS_Locator_t *self);

RTPSDllExport RTPS_ReturnCode_t
RTPS_Locator_finalize(struct RTPS_Locator_t *self);

RTPSDllExport RTI_BOOL
RTPS_Locator_is_equal(const struct RTPS_Locator_t *left,
                      const struct RTPS_Locator_t *right);

RTPSDllExport RTI_BOOL
RTPS_Locator_is_consistent(const struct RTPS_Locator_t *self);


/*e \ingroup MIGRtpsPortModule
  Get well known port for multicast traffic

  RTPS defines well known ports based on the system wide
  port base number, and application specific port group number.
  We offer static helper methods to map them into well known port, using
  RTPS defined logic.

  @param domain_id \b In. Domain index of participant.
  @param port_base \b In. Domain base port.
  @param domain_id_gain \b In. Domain index gain.
  @param port_offset \b In. Domain base port offset.
*/
RTPSDllExport RTI_UINT32
MIGRtps_getWellKnownMulticastPort(
        int domain_id,
        RTI_UINT32 port_base,
        int domain_id_gain,
        int port_offset);

 /*e \ingroup MIGRtpsPortModule
  * Get well known port for unicast traffic
  * 
  * RTPS defines well known ports based on the system wide
  * port base number, and application specific port group number.
  * We offer static helper methods to map them into well known port, using
  * RTPS defined logic.
  * 
  * @param domain_id \b In. Domain index of participant.
  * @param port_base \b In. Domain base port.
  * @param domain_id_gain \b In. Domain index gain.
  * @param port_offset \b In. Domain base port offset.
  */
RTPSDllExport RTI_UINT32
MIGRtps_getWellKnownUnicastPort(
        int domain_id,
        int participant_id,
        RTI_UINT32 port_base,
        int domain_id_gain,
        int participant_id_gain,
        int port_offset);

/*e \ingroup REDAEpochClass
  @brief Definition of the type
  @todo consider making the epoch the LONGEST int that can be written
  atomically (e.g. long) on every platform
*/

RTPSDllExport RTI_BOOL
MIGRtpsEpoch_deserialize(REDA_Epoch_T * me,
                         const char **stream,
                         RTI_BOOL needByteSwap);

/**************/

typedef struct NETIO_Guid RTPS_GUID; 
typedef struct NETIO_GuidPrefix RTPS_GuidPrefix;
typedef struct NETIO_GuidEntity RTPS_Entity;

#define RTPS_GUIDPREFIX_UNKNOWN {0,0,0,0,0,0,0,0,0,0,0,0}
#define RTPS_ENTITY_UNKNOWN {{0,0,0,0}}

#define RTPSEntity_is_unknown(entity_) \
( \
    ((entity_)->value[0]== 0) && ((entity_)->value[1] == 0) && \
    ((entity_)->value[2] == 0) && ((entity_)->value[3] == 0) \
)


#define RTPS_ENTITYID_KIND_USER        0x0
#define RTPS_ENTITYID_KIND_BUILTIN     0x3
#define RTPS_ENTITYID_KIND_VENDOR      0x1

#define RTPS_ENTITYID_KIND_PARTICIPANT 0xc1

#define RTPS_ENTITYID_KIND_WRITER_B_K  0xc2
#define RTPS_ENTITYID_KIND_WRITER_B    0xc3
#define RTPS_ENTITYID_KIND_READER_B    0xc4
#define RTPS_ENTITYID_KIND_READER_B_K  0xc7

#define RTPS_ENTITYID_KIND_WRITER_U_K  0x2
#define RTPS_ENTITYID_KIND_WRITER_U    0x3
#define RTPS_ENTITYID_KIND_READER_U    0x4
#define RTPS_ENTITYID_KIND_READER_U_K  0x7


struct RTPS_Time {
    RTI_INT32 seconds;
    RTI_UINT32 fractions;
};

struct RTPS_Vendor {
    RTI_UINT8 vendor_id[2];
};

struct RTPS_SequenceNumber {
    RTI_INT32 high;
    RTI_UINT32 low;
};

#define RTPS_SEQUENCENUMBER_UNKNOWN {-1,0}

struct RTPS_FragmentNumber {
    RTI_UINT32 value;
};

struct RTPS_TopicKind {
    RTI_UINT32 kind;
};

#define RTPS_TOPICKIND_NOKEY 1
#define RTPS_TOPICKIND_KEYED 2

struct RTPS_ReliabilityKind {
    RTI_UINT32 kind;
};

#define RTPS_RELIABILITYKIND_BEST_EFFORT 1
#define RTPS_RELIABILITYKIND_RELIABLE    3

struct RTPS_Count {
    RTI_UINT32 value;
};

struct RTPS_ProtocolVersion {
    RTI_UINT8 major;
    RTI_UINT8 minor;
};

#define RTPS_PROTOCOLVERSION_1_0 {1,0}
#define RTPS_PROTOCOLVERSION_1_1 {1,1}
#define RTPS_PROTOCOLVERSION_2_0 {2,0}
#define RTPS_PROTOCOLVERSION_2_1 {2,1}
#define RTPS_PROTOCOLVERSION RTPS_PROTOCOLVERSION_2_1

struct RTPS_Header {
    RTI_UINT32 rtps;
    struct RTPS_ProtocolVersion protocol_version;
    struct RTPS_Vendor vendor_id;
    RTPS_GuidPrefix guid_prefix;
};

#define RTPS_HEADER_SIZE() sizeof(struct RTPS_Header)

#ifdef RTI_ENDIAN_LITTLE
#define VALID_RTPS_HEADER 0x53505452    /* RTPS backwards */
#else
#define VALID_RTPS_HEADER 0x52545053    /* RTPS in big endian */
#endif /* RTI_ENDIAN_LITTLE */


#define RTPSCdr_deserialize_guid_prefix(stream_,prefix_) \
{\
    CDR_deserialize_UnsignedLongFromBigEndian( \
       (stream_), &(prefix_)->hostId); \
    CDR_deserialize_UnsignedLongFromBigEndian( \
       (stream_), &(prefix_)->appId); \
    CDR_deserialize_UnsignedLongFromBigEndian( \
       (stream_), &(prefix_)->instanceId); \
}

#define RTPSCdr_serialize_guid_prefix(stream_,prefix_) \
{\
    CDR_serialize_UnsignedLongToBigEndian( \
       (stream_), &(prefix_)->hostId); \
    CDR_serialize_UnsignedLongToBigEndian( \
       (stream_), &(prefix_)->appId); \
    CDR_serialize_UnsignedLongToBigEndian( \
       (stream_), &(prefix_)->instanceId); \
}


#define RTPS_HEADER_SET(hdr_,prefix_)\
{\
    (hdr_)->rtps = VALID_RTPS_HEADER;\
    (hdr_)->protocol_version.major = MIG_RTPS_PROTOCOL_VERSION_MAJOR;\
    (hdr_)->protocol_version.minor = MIG_RTPS_PROTOCOL_VERSION_MINOR;\
    (hdr_)->vendor_id.vendor_id[0] = MIG_RTPS_VENDOR_ID_MAJOR;\
    (hdr_)->vendor_id.vendor_id[1] = MIG_RTPS_VENDOR_ID_MINOR;\
    (hdr_)->guid_prefix = *(prefix_);\
}

/******************************************************************************/
struct RTPS_ContentFilterProperty {
    char topic_name[256];
    char related_topic_name[256];
    char filter_name[256];
    char *expression;
    char **expression_parameters;
    RTI_INT16 value;
};

struct RTPS_ContentFilterInfo {
    RTI_UINT32 result;
    char **signature;
};

struct RTPS_Property {
    char *name;
    char *value;
};

struct RTPS_EntityName {
    char *name;
};

struct RTPS_OriginalWriterInfo {
    RTPS_GUID guid;
    struct RTPS_SequenceNumber sn;
    /* missing parameter list */
};

typedef RTI_UINT32 RTPS_BuiltinEndpointSet;
#define RTPS_BUILTINENDPOINT_PARTICIPANT_ANNOUNCER           (1<<0)
#define RTPS_BUILTINENDPOINT_PARTICIPANT_DETECTOR            (1<<1)
#define RTPS_BUILTINENDPOINT_PUBLICATION_ANNOUNCER           (1<<2)
#define RTPS_BUILTINENDPOINT_PUBLICATION_DETECTOR            (1<<3)
#define RTPS_BUILTINENDPOINT_SUBSCRIPTION_ANNOUNCER          (1<<4)
#define RTPS_BUILTINENDPOINT_SUBSCRIPTION_DETECTOR           (1<<5)
#define RTPS_BUILTINENDPOINT_PARTICIPANT_PROXY_ANNOUNCER     (1<<6)
#define RTPS_BUILTINENDPOINT_PARTICIPANT_PROXY_DETECTOR      (1<<7)
#define RTPS_BUILTINENDPOINT_PARTICIPANT_STATE_ANNOUNCER     (1<<8)
#define RTPS_BUILTINENDPOINT_PARTICIPANT_STATE_DETECTOR      (1<<9)
#define RTPS_BUILTINENDPOINT_PARTICIPANT_MESSAGE_DATA_WRITER (1<<10)
#define RTPS_BUILTINENDPOINT_PARTICIPANT_MESSAGE_DATA_READER (1<<11)

#define RTPS_PAD_KIND            (0x01)
#define RTPS_ACKNACK_KIND        (0x06)
#define RTPS_HEARTBEAT_KIND      (0x07)
#define RTPS_GAP_KIND            (0x08)
#define RTPS_INFO_TS_KIND        (0x09)
#define RTPS_INFO_SRC_KIND       (0x0c)
#define RTPS_INFO_REPLY_IP4_KIND (0x0d)
#define RTPS_INFO_DST_KIND       (0x0e)
#define RTPS_INFO_REPLY_KIND     (0x0f)
#define RTPS_NACK_FRAG_KIND      (0x12)
#define RTPS_HEARTBEAT_FRAG_KIND (0x13)
#define RTPS_DATA_KIND           (0x15)
#define RTPS_DATA_FRAG_KIND      (0x16)

struct RTPS_SubmsgHdr
{
    RTI_UINT8 kind;
    RTI_INT8 flags;
    RTI_UINT16 length;
};

#define RTPS_SUBMSG_FLAG_E 0x1

#ifdef RTI_ENDIAN_LITTLE
#define RTPS_SubmsgHdr_set(subhdr_,kind_,flags_,length_) \
{\
        (subhdr_)->kind = kind_;\
        (subhdr_)->flags = flags_ | RTPS_SUBMSG_FLAG_E;\
        (subhdr_)->length = length_ - 4;\
}
#else
#define RTPS_SubmsgHdr_set(subhdr_,kind_,flags_,length_) \
{\
        (subhdr_)->kind = kind_;\
        (subhdr_)->flags = flags_ & ~RTPS_SUBMSG_FLAG_E;\
        (subhdr_)->length = length_ - 4;\
}
#endif

#define RTPSSubmessage_is_info(kind_) \
((kind_ == RTPS_INFO_DST_KIND) || \
 (kind_ == RTPS_INFO_TS_KIND) || \
 (kind_ == RTPS_INFO_SRC_KIND) || \
 (kind_ == RTPS_INFO_REPLY_IP4_KIND) || \
 (kind_ == RTPS_INFO_REPLY_KIND))

#define RTPSSubmessage_from_reader(kind_) \
((kind_ == RTPS_ACKNACK_KIND))

#define RTPSSubmessage_from_writer(kind_) \
((kind_ == RTPS_GAP_KIND) || \
 (kind_ == RTPS_DATA_KIND) || \
 (kind_ == RTPS_HEARTBEAT_KIND))

/******************************************************************************/
struct RTPS_ACKNACK {
    struct RTPS_SubmsgHdr hdr;
    RTPS_Entity reader;
    RTPS_Entity writer;
    struct MIGRtpsBitmap bitmap; /* variable length serialized */
    RTI_UINT32 count;
};

#define RTPS_BITMAP_INT_COUNT(bitCount_) \
    ((bitCount_ + 31)/32)

#define RTPS_ACKNACKFLAGS_F 0x2
#define RTPS_ACKNACK_FINAL(flags) (flags & RTPS_ACKNACKFLAGS_F)

#define RTPS_ACKNACK_SIZE(bitCount_) \
( \
    (sizeof(struct RTPS_ACKNACK) - sizeof(struct MIGRtpsBitmap)) + \
    (sizeof(struct REDA_SequenceNumber)) + \
    4 + (4 * RTPS_BITMAP_INT_COUNT(bitCount_)) \
)

#define RTPS_ACKNACK_SET(ack_,flags_,length_,reader_,writer_,bitmap_,count_) \
{ \
    RTI_UINT32 int_count, *epoch_ref; \
    epoch_ref = &((ack_)->bitmap._32bits[0]); \
    RTPS_SubmsgHdr_set(&(ack_)->hdr,RTPS_ACKNACK_KIND,flags_,length_); \
    (ack_)->reader = reader_; \
    (ack_)->writer = writer_; \
    (ack_)->bitmap._lead = (bitmap_)->_lead; \
    (ack_)->bitmap._bitCount = (bitmap_)->_bitCount; \
    for (int_count = 0; \
         int_count < RTPS_BITMAP_INT_COUNT((bitmap_)->_bitCount); ++int_count) \
    { \
        (ack_)->bitmap._32bits[int_count] = (bitmap_)->_32bits[int_count]; \
        epoch_ref = &((ack_)->bitmap._32bits[int_count+1]); \
    } \
    *epoch_ref = count_; \
}

/******************************************************************************/
struct RTPS_DATA {
    struct RTPS_SubmsgHdr hdr;
    RTI_UINT16 flags;
    RTI_UINT16 inlineQosOffset;
    RTPS_Entity reader;
    RTPS_Entity writer;
    struct REDA_SequenceNumber sn; 
    /* inline qos */
    /* keyHash */
    /* status info */
    /* payload */
};

#define RTPS_DATA_SET(data_,flags_,length_,eflags_,qlength_,reader_,writer_,sn_) \
{ \
    RTPS_SubmsgHdr_set(&(data_)->hdr,RTPS_DATA_KIND,flags_,length_); \
    (data_)->flags = eflags_;\
    (data_)->inlineQosOffset = qlength_;\
    (data_)->reader = reader_;\
    (data_)->writer = writer_;\
    (data_)->sn = sn_;\
}

#define RTPS_FLAGS_NONE  0x0
#define RTPS_DATAFLAGS_Q 0x2
#define RTPS_DATAFLAGS_D 0x4
#define RTPS_DATAFLAGS_K 0x8

#define RTPS_DATA_INLINEQOS_OFFSET 16

#define RTPS_DATA_EMPTY(flags) (!((flags & RTPS_DATAFLAGS_D) && (flags & RTPS_DATAFLAGS_K)))
#define RTPS_DATA_SERIALIZED_DATA(flags) ((flags & RTPS_DATAFLAGS_D) && !(flags & RTPS_DATAFLAGS_K))
#define RTPS_DATA_SERIALIZED_KEY(flags) (!(flags & RTPS_DATAFLAGS_D) && (flags & RTPS_DATAFLAGS_K))
#define RTPS_DATA_INVALID(flags) ((flags & RTPS_DATAFLAGS_D) && (flags & RTPS_DATAFLAGS_K))

#define RTPS_DATA_SIZE(payload_size) (sizeof(struct RTPS_DATA)+payload_size)
/******************************************************************************/
struct RTPS_GAP {
    struct RTPS_SubmsgHdr hdr;
    RTPS_Entity reader;
    RTPS_Entity writer;
    struct REDA_SequenceNumber sn_start;
    struct MIGRtpsBitmap bitmap; /* variable length */
};

#define RTPS_GAP_SIZE(bitCount_) \
( \
    (sizeof(struct RTPS_GAP)) - (sizeof(struct MIGRtpsBitmap)) + \
    (sizeof(struct REDA_SequenceNumber)) + \
    4 + (4 * RTPS_BITMAP_INT_COUNT(bitCount_)) \
)

#define RTPS_GAP_SET(gap_,length_,reader_,writer_,sn_start_,bitmap_) \
{ \
    int int_count; \
    RTPS_SubmsgHdr_set(&(gap_)->hdr,RTPS_GAP_KIND,0,length_); \
    (gap_)->reader = reader_; \
    (gap_)->writer = writer_; \
    (gap_)->sn_start = sn_start_; \
    (gap_)->bitmap._lead = (bitmap_)->_lead; \
    (gap_)->bitmap._bitCount = (bitmap_)->_bitCount; \
    for (int_count = 0; \
         int_count < RTPS_BITMAP_INT_COUNT((bitmap_)->_bitCount); ++int_count) \
    { \
        (gap_)->bitmap._32bits[int_count] = (bitmap_)->_32bits[int_count]; \
    } \
}
/******************************************************************************/
struct RTPS_HEARTBEAT {
    struct RTPS_SubmsgHdr hdr;
    RTPS_Entity reader;
    RTPS_Entity writer;
    struct REDA_SequenceNumber sn_first; 
    struct REDA_SequenceNumber sn_last; 
    RTI_INT32 count;
};

#define RTPS_HB_SET(hb_,flags_,reader_,writer_,sn_first_,sn_last_,count_) \
{ \
    RTPS_SubmsgHdr_set(&(hb_)->hdr,RTPS_HEARTBEAT_KIND,flags_,RTPS_HB_SIZE());\
    (hb_)->reader = (reader_);\
    (hb_)->writer = (writer_);\
    (hb_)->sn_first = (sn_first_);\
    (hb_)->sn_last = (sn_last_);\
    (hb_)->count = (count_);\
}

#define RTPS_HBFLAGS_F 0x02
#define RTPS_HBFLAGS_L 0x04

#define RTPS_HB_SIZE() (sizeof(struct RTPS_HEARTBEAT))

/******************************************************************************/
struct RTPS_INFO_DST {
    struct RTPS_SubmsgHdr hdr;
    RTPS_GuidPrefix guid_prefix;
};

#define RTPS_INFO_DST_SIZE() (sizeof(struct RTPS_INFO_DST))

#define RTPS_INFO_DST_SET(info_,dst_) \
{ \
    RTPS_SubmsgHdr_set(&(info_)->hdr,RTPS_INFO_DST_KIND,0,RTPS_INFO_DST_SIZE());\
    (info_)->guid_prefix = *(dst_);\
}
/******************************************************************************/
struct RTPS_INFO_REPLY {
    struct RTPS_SubmsgHdr hdr;
    /* Add locator list */
};

#define RTPS_INFO_REPLYFLAGS_M 0x2

/******************************************************************************/
struct RTPS_INFO_SRC {
    struct RTPS_SubmsgHdr hdr;
    RTI_UINT32 unused;
    struct RTPS_ProtocolVersion version;
    struct RTPS_Vendor vendor;
    RTPS_GuidPrefix guid_prefix;
};

/******************************************************************************/
struct RTPS_INFO_TS {
    struct RTPS_SubmsgHdr hdr;
    struct RTPS_Time timestamp;
};

#define RTPS_INFO_TSFLAGS_I (0x2)
#define RTPS_INFO_TS_SIZE(i_) \
((i_) ? sizeof(struct RTPS_SubmsgHdr) : sizeof(struct RTPS_INFO_TS))

#define RTPS_INFO_TS_SET(info_ts_,flags_,sec_,frac_) \
{\
    RTPS_SubmsgHdr_set(&(info_ts_)->hdr,RTPS_INFO_TS_KIND,flags_,\
                       RTPS_INFO_TS_SIZE(flags_));\
    if (!((flags_) & RTPS_INFO_TSFLAGS_I))\
    {\
        (info_ts_)->timestamp.seconds = sec_;\
        (info_ts_)->timestamp.fractions = frac_;\
    }\
}

/******************************************************************************/
struct RTPS_INFO_PAD {
    struct RTPS_SubmsgHdr hdr;
};

/******************************************************************************/
struct RTPS_INFO_REPLY_IP4 {
    struct RTPS_SubmsgHdr hdr;
    /* unicast locator   sb*/
    /* multicast locator */
};

#define RTPS_INFO_REPLY_IP4FLAGS_M 0x2

/******************************************************************************/
union RTPS_MESSAGES
{
    struct RTPS_SubmsgHdr submsg;
    struct RTPS_Header header;

    struct RTPS_DATA data;
    struct RTPS_GAP gap;

    struct RTPS_HEARTBEAT hb;
    struct RTPS_ACKNACK acknack;
  
    struct RTPS_INFO_TS info_ts;
    struct RTPS_INFO_SRC info_src;
    struct RTPS_INFO_DST info_dst;
    struct RTPS_INFO_REPLY info_reply;

    struct RTPS_INFO_PAD pad;
};

/**************/

typedef enum
{
    RTPS_INTERFACEMODE_UNDEFINED,
    RTPS_INTERFACEMODE_WRITER,
    RTPS_INTERFACEMODE_READER,
    RTPS_INTERFACEMODE_EXTERNAL_RECEIVER
} RTPSInterfaceMode_t;

NETIODllExport struct RT_ComponentFactoryI*
RTPSInterfaceFactory_get_interface(void);

struct RTPSInterfaceProperty
{
    struct NETIO_InterfaceProperty _parent;
    RTPSInterfaceMode_t mode;
    struct NETIO_Address intf_address;

    RTI_INT32 max_peer_count; /* TODO: expose as resource limit */
    RTI_SIZE_T max_transport_count; /* TODO: expose as resource limit */
    RTI_SIZE_T max_message_buffer_count; /* TODO: expose as resource limit */
    RTI_SIZE_T max_send_fanout; /* TODO: expose as resource limit */

    RTI_UINT8 reliable;
    RTI_UINT8 anonymous;
    
    struct OSAPI_NtpTime hb_period; 
    RTI_SIZE_T samples_per_hb;
    RTI_UINT32 window_size;
};

#define RTPSInterfaceProperty_INITIALIZER \
{\
    NETIO_InterfaceProperty_INITIALIZER,\
    RTPS_INTERFACEMODE_UNDEFINED,\
    NETIO_Address_INITIALIZER, \
    32, /* max_peer_count */ \
    32, /* max_transport_count */ \
    4, /* max_message_buffer_count */ \
    8, /* max_send_fanout */ \
    0, /* reliable */ \
    0, /* anonymous */ \
    {3,0}, /* hb_period */ \
    8, /* samples_per_hb */ \
    0 /* window_size */ \
}

struct RTPSRouteProperty 
{
    struct NETIORouteProperty _parent; 

    /* peer reliability */
    RTI_UINT8 reliable;
    struct REDA_SequenceNumber first_sn;
    struct REDA_SequenceNumber last_sn;
};

#define RTPSRouteProperty_INITIALIZER \
{\
    NETIORouteProperty_INITIALIZER, \
    0, /* reliable */ \
    REDA_SEQUENCE_NUMBER_ZERO,\
    REDA_SEQUENCE_NUMBER_ZERO \
}

RTPSDllExport struct RT_ComponentFactoryI*
RTPSInterfaceFactory_get_interface(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#include "rtps/rtps_rtps_impl.h"

#endif /* rtps_rtps_h */
