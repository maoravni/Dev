/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_infrastructure_h
#define dds_c_infrastructure_h



#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef osapi_thread_h
#include "osapi/osapi_thread.h"
#endif
#ifndef netio_rtps_h
#include "netio/netio_rtps.h"
#endif
#ifndef rtps_rtps_h
#include "rtps/rtps_rtps.h"
#endif
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_string_h
#include "dds_c/dds_c_string.h"
#endif
#ifndef dds_c_sequence_h
#include "dds_c/dds_c_sequence.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NULL
#define NULL 0
#endif

/* ================================================================= */
/*                        Micro DDS                                  */
/* ================================================================= */
#define RTI_MICRODDS
#define RTI_MICRODDS_MAJOR 2
#define RTI_MICRODDS_MINOR 0

/* ================================================================= */
/*                        Time Support                               */
/* ================================================================= */

/*e
 * \dref_TimeSupportGroupDocs
 */

/*e
 * \dref_Time_t
 */
struct DDS_Time_t
{
    /*e
     * \dref_TimeStamp_sec
     */
    DDS_Long sec;

    /*e
     * \dref_TimeStamp_nanosec
     */
    DDS_UnsignedLong nanosec;
};

#define DDS_Time_t_equals(l, r) \
    ((l).sec == (r).sec && (l).nanosec == (r).nanosec)
#define DDS_Time_t_greater_than(l, r) \
    (((l).sec > (r).sec) || \
     (((l).sec == (r).sec) && \
      ((l).nanosec > (r).nanosec)))
#define DDS_Time_t_less_than(l, r) \
    (((l).sec < (r).sec) || \
     (((l).sec == (r).sec) && \
      ((l).nanosec < (r).nanosec)))

/*e
 * \dref_Time_t_ZERO
 */
#define DDS_TIME_ZERO    { 0L, 0UL }

#define DDS_Time_is_zero(timePtr) \
        ((timePtr)->sec == 0L || (timePtr)->nanosec == 0UL)

#ifdef DOXYGEN_DOCUMENTATION_ONLY

/*e
 * \dref_Time_t_is_zero
 */
    DDS_Boolean DDS_Time_is_zero(const struct DDS_Time_t *time);

#endif  /*DOXYGEN_DOCUMENTATION_ONLY */

/*e
 * \dref_Time_t_INVALID_SEC
 */
extern DDSCDllVariable const DDS_Long DDS_TIME_INVALID_SEC;

/*i deprecated */
#define DDS_TIMESTAMP_INVALID_SEC DDS_TIME_INVALID_SEC

/*e
 * \dref_Time_t_INVALID_NSEC
 */
extern DDSCDllVariable const DDS_UnsignedLong DDS_TIME_INVALID_NSEC;

/*i deprecated */
#define DDS_TIMESTAMP_INVALID_NSEC DDS_TIME_INVALID_NSEC

/*e
 * \dref_Time_t_INVALID
 */
extern DDSCDllVariable const struct DDS_Time_t DDS_TIME_INVALID;

#define DDS_Time_is_invalid(timePtr) \
        ((timePtr)->sec < 0)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*e
 * \dref_Time_t_is_invalid
 */
    DDS_Boolean DDS_Time_is_invalid(const struct DDS_Time_t *time);
#endif

/* ================================================================= */
/*                             Duration                              */
/* ================================================================= */
/*e \dref_Duration_t
 */
struct DDS_Duration_t
{
    /*e \dref_TimeStamp_sec
     */
    DDS_Long sec;

    /*e \dref_TimeStamp_nanosec
     */
    DDS_UnsignedLong nanosec;
};

/*e \dref_Duration_t_INFINITE_SEC
 */
extern DDSCDllVariable const DDS_Long DDS_DURATION_INFINITE_SEC;

/*i deprecated */
#define DDS_DURATION_INFINITY_SEC DDS_DURATION_INFINITE_SEC

/*e \dref_Duration_t_INFINITE_NSEC
 */
extern DDSCDllVariable const DDS_UnsignedLong DDS_DURATION_INFINITE_NSEC;

/*i deprecated */
#define DDS_DURATION_INFINITY_NSEC DDS_DURATION_INFINITE_NSEC

/*e \dref_Duration_t_INFINITE
 */
extern DDSCDllVariable const struct DDS_Duration_t DDS_DURATION_INFINITE;

#define DDS_Duration_is_infinite(durationPtr) \
        ((durationPtr)->sec == DDS_DURATION_INFINITE_SEC && \
         (durationPtr)->nanosec == DDS_DURATION_INFINITE_NSEC)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*e \dref_Duration_t_is_infinite
 */
    DDS_Boolean DDS_Duration_is_infinite(const struct DDS_Duration_t *duration);
#endif

/*ce \dref_Duration_t_compare
 */
DDSCDllExport int
DDS_Duration_compare(const struct DDS_Duration_t *left,
                     const struct DDS_Duration_t *right);

DDSCDllExport void
DDS_Duration_to_ntp_time(const struct DDS_Duration_t *self,
                         struct OSAPI_NtpTime *dst);

#define DDS_Duration_copy(out, in) \
    ((out.sec = in.sec) && \
     (out.nanosec = in.nanosec))

#define DDS_Duration_equal(me, other) \
    ((me)->sec == (other)->sec && \
    (me)->nanosec == (other)->nanosec)

#define DDS_Duration_set(me, __sec,__nanosec) \
    (me)->sec = __sec;\
    (me)->nanosec = __nanosec

#define DDS_Duration_get(me, __sec,__nanosec) \
    __sec = (me)->sec;\
    __nanosec = (me)->nanosec

/*e \dref_Duration_t_ZERO_SEC
 */
extern DDSCDllVariable const DDS_Long DDS_DURATION_ZERO_SEC;

/*e \dref_Duration_t_ZERO_NSEC
 */
extern DDSCDllVariable const DDS_UnsignedLong DDS_DURATION_ZERO_NSEC;

/*e \dref_Duration_t_ZERO
 */
extern DDSCDllVariable const struct DDS_Duration_t DDS_DURATION_ZERO;

#define DDS_Duration_is_zero(durationPtr) \
        ((durationPtr)->sec == DDS_DURATION_ZERO_SEC && \
         (durationPtr)->nanosec == DDS_DURATION_ZERO_NSEC)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*e \dref_Duration_t_is_zero
 */
    DDS_Boolean DDS_Duration_is_zero(const struct DDS_Duration_t *duration);
#endif

/* ================================================================= */
/*                        Instance Handle                            */
/* ================================================================= */

/*e \dref_InstanceHandle_t
 */
typedef DDS_HANDLE_TYPE_NATIVE DDS_InstanceHandle_t;

DDS_SEQUENCE(DDS_InstanceHandleSeq, DDS_InstanceHandle_t)

#ifdef DOXYGEN_DOCUMENTATION_ONLY

/*i \dref_InstanceHandleSeq
 */
/* struct DDS_InstanceHandleSeq {};*/

#endif

/*e \dref_InstanceHandle_t_NIL
*/
extern DDSCDllVariable const DDS_InstanceHandle_t DDS_HANDLE_NIL;

/*e \dref_InstanceHandle_t_equals
 */
DDSCDllExport DDS_Boolean
DDS_InstanceHandle_equals(const DDS_InstanceHandle_t *self,
                          const DDS_InstanceHandle_t *other);

#define DDS_InstanceHandle_is_nil(handlePtr) \
        DDS_InstanceHandle_equals(handlePtr, &DDS_HANDLE_NIL)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*e \dref_InstanceHandle_t_is_nil
 */
    DDS_Boolean DDS_InstanceHandle_is_nil(const DDS_InstanceHandle_t * self);
#endif

DDSCDllExport void
DDS_InstanceHandle_from_rtps(DDS_InstanceHandle_t * self,
                             const struct MIGRtpsGuid *other);

DDSCDllExport void
DDS_InstanceHandle_to_rtps(struct MIGRtpsGuid *other,
                           const DDS_InstanceHandle_t * self);

DDSCDllExport DDS_Boolean
DDS_InstanceHandle_equals(const DDS_InstanceHandle_t * self,
                          const DDS_InstanceHandle_t * other);

DDSCDllExport DDS_Long
DDS_InstanceHandle_compare(const DDS_InstanceHandle_t * self,
                           const DDS_InstanceHandle_t * other);

/* ================================================================= */
/*                               GUID                                */
/* ================================================================= */

/*e
 * \dref_GUIDSupportGroupDocs
 */

/*e \dref_GUID_t
 */
struct DDS_GUID_t
{
    /*e \dref_GUID_t_value
     */
    DDS_Octet value[16];
};

#define DDS_GUID_INITIALIZER {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}

/*i \dref_GUID_t_AUTO
*/
extern DDSCDllVariable const struct DDS_GUID_t DDS_GUID_AUTO;

/*e \dref_GUID_t_UNKNOWN
*/
extern DDSCDllVariable const struct DDS_GUID_t DDS_GUID_UNKNOWN;

/*i \dref_GUID_t_PREFIX_UNKNOWN
*/
extern DDSCDllVariable const struct DDS_GUID_t DDS_GUID_PREFIX_UNKNOWN;

/*i \dref_GUID_t_PREFIX_UNKNOWN
 */
extern DDSCDllVariable const struct DDS_GUID_t DDS_GUID_PREFIX_AUTO;

/*e \dref_GUID_t_equals
 */
DDSCDllExport DDS_Boolean
DDS_GUID_equals(const struct DDS_GUID_t *self,
                const struct DDS_GUID_t *other);

/*e \dref_GUID_t_copy
 */
DDSCDllExport void
DDS_GUID_copy(struct DDS_GUID_t *self,
              const struct DDS_GUID_t *other);

/*i \dref_GUID_t_to_rtps
 @brief Convert from DDS GUID to RTPS GUID
 */
DDSCDllExport void 
DDS_GUID_to_rtps(struct MIGRtpsGuid *other,
                 const struct DDS_GUID_t *self);

/*i \dref_GUID_t_from_rtps
 @brief Convert from RTPS GUID to DDS GUID
 */
DDSCDllExport void 
DDS_GUID_from_rtps(struct DDS_GUID_t *self,
                   const struct MIGRtpsGuid *other);

/*i \dref_GUID_t_compare
 @brief Compare DDS_GUIDs
 */
DDSCDllExport DDS_Long
DDS_GUID_compare(const struct DDS_GUID_t *self,
                 const struct DDS_GUID_t *other);

/* ================================================================= */
/*                        Sequence Number Support                    */
/* ================================================================= */

/*e
 * \dref_SequenceNumberSupportGroupDocs
 */

/*e \dref_SequenceNumber_t
 */
struct DDS_SequenceNumber_t
{
    /*e \dref_SequenceNumber_t_high
    */
    DDS_Long high;

    /*e \dref_SequenceNumber_t_low
    */
    DDS_UnsignedLong low;
};


#define DDS_SEQUENCE_NUMBER_UNKNOWN    REDA_SEQUENCE_NUMBER_UNKNOWN

#define DDS_SEQUENCE_NUMBER_ZERO    REDA_SEQUENCE_NUMBER_ZERO

#define DDS_SEQUENCE_NUMBER_MAX    REDA_SEQUENCE_NUMBER_MAX

#define DDS_SequenceNumber_compare(sn1,sn2) \
    ((((sn1)->high) > ((sn2)->high)) ? 1 : \
     ((((sn1)->high) < ((sn2)->high)) ? -1 : \
      ((((sn1)->low) > ((sn2)->low)) ? 1 : \
       ((((sn1)->low) < ((sn2)->low)) ? -1 : 0))))

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*e \dref_SequenceNumber_t_compare
 */
    int DDS_SequenceNumber_compare(const struct DDS_SequenceNumber_t *sn1,
                                   const struct DDS_SequenceNumber_t *sn2);
#endif

/* ================================================================= */
/*                         Return Types                              */
/* ================================================================= */
/*e \dref_ReturnCodeGroupDocs
 */

/* ----------------------------------------------------------------- */
/*e \dref_ReturnCode_t
*/
typedef enum
{
    /*e \dref_ReturnCode_t_RETCODE_OK
     */
    DDS_RETCODE_OK = 0,

    /*e \dref_ReturnCode_t_RETCODE_ERROR
     */
    DDS_RETCODE_ERROR = 1,

    /*e \dref_ReturnCode_t_RETCODE_UNSUPPORTED
     */
    DDS_RETCODE_UNSUPPORTED = 2,

    /*e \dref_ReturnCode_t_RETCODE_BAD_PARAMETER
     */
    DDS_RETCODE_BAD_PARAMETER = 3,

    /*e \dref_ReturnCode_t_RETCODE_PRECONDITION_NOT_MET
     */
    DDS_RETCODE_PRECONDITION_NOT_MET = 4,

    /*e \dref_ReturnCode_t_RETCODE_OUT_OF_RESOURCES
     */
    DDS_RETCODE_OUT_OF_RESOURCES = 5,

    /*e \dref_ReturnCode_t_RETCODE_NOT_ENABLED
     */
    DDS_RETCODE_NOT_ENABLED = 6,

    /*e \dref_ReturnCode_t_RETCODE_IMMUTABLE_POLICY
     */
    DDS_RETCODE_IMMUTABLE_POLICY = 7,

    /*e \dref_ReturnCode_t_RETCODE_INCONSISTENT_POLICY
     */
    DDS_RETCODE_INCONSISTENT_POLICY = 8,

    /*e \dref_ReturnCode_t_RETCODE_ALREADY_DELETED
     */
    DDS_RETCODE_ALREADY_DELETED = 9,

    /*e \dref_ReturnCode_t_RETCODE_TIMEOUT
     */
    DDS_RETCODE_TIMEOUT = 10,

    /*e \dref_ReturnCode_t_RETCODE_NO_DATA
     */
    DDS_RETCODE_NO_DATA = 11,

    /*e \dref_ReturnCode_t_RETCODE_ILLEGAL_OPERATION
     */
    DDS_RETCODE_ILLEGAL_OPERATION = 12,

    /*e \dref_ReturnCode_t_RETCODE_ENTITY_NOT_EXIST
     */
    DDS_RETCODE_ENTITY_NOT_EXIST = 1000,

    /*e \dref_ReturnCode_t_RETCODE_ENTITY_NOT_EMPTY
     */
    DDS_RETCODE_ENTITY_NOT_EMPTY = 1001,

    /*e \dref_ReturnCode_t_RETCODE_ENTITY_LOCKED
     */
    DDS_RETCODE_ENTITY_LOCKED = 1002,

    /*e \dref_ReturnCode_t_RETCODE_ENTITY_DELETE_ERROR
     */
    DDS_RETCODE_ENTITY_DELETE_ERROR = 1003
} DDS_ReturnCode_t;


/* ================================================================= */
/*            VARIABLE LENGTH TYPES SUPPORT                          */
/* ================================================================= */
/*i @defgroup DDSVarLenType Full Variable Length Type Support
  @ingroup DDSCommonModule
 */
#define DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_FULL(T) \
 struct T; \
 DDSCDllExport DDS_Boolean T ## _initialize(struct T* self); \
 DDSCDllExport DDS_Boolean T ## _finalize(struct T* self); \
 DDSCDllExport DDS_Boolean T ## _copy(struct T* self, const struct T* from);\
 DDSCDllExport DDS_Boolean      T ## _is_equal(const struct T* self, const struct T* from)

/*i @defgroup DDSVarLenType Basic Variable Length Type Support
  @ingroup DDSCommonModule
 */
#define DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(T) \
 struct T; \
 DDSCDllExport DDS_Boolean      T ## _is_equal(const struct T* self, const struct T* from)

/*  DDSCDllExport DDS_ReturnCode_t T ## _copy(struct T* self, const struct T* from); */

/* ================================================================= */
/*                    Status Types                                   */
/* ================================================================= */

/*e \dref_StatusKindGroupDocs
 */

/*e \dref_StatusMask
 */
typedef DDS_UnsignedLong DDS_StatusMask;

/*e \dref_STATUS_MASK_NONE
 */
#define DDS_STATUS_MASK_NONE   ((DDS_StatusMask) 0)

/*e \dref_STATUS_MASK_ALL
 */
#define DDS_STATUS_MASK_ALL    (~DDS_STATUS_MASK_NONE)

/* ----------------------------------------------------------------- */
/*e \dref_StatusKind
 */
typedef enum
{
    /*i \dref_StatusKind_INCONSISTENT_TOPIC_STATUS
     */
    DDS_INCONSISTENT_TOPIC_STATUS = 0x0001 << 0,

    /*e \dref_StatusKind_OFFERED_DEADLINE_MISSED_STATUS
     */
    DDS_OFFERED_DEADLINE_MISSED_STATUS = 0x0001 << 1,

    /*e \dref_StatusKind_REQUESTED_DEADLINE_MISSED_STATUS
     */
    DDS_REQUESTED_DEADLINE_MISSED_STATUS = 0x0001 << 2,

    /*e \dref_StatusKind_OFFERED_INCOMPATIBLE_QOS_STATUS
     */
    DDS_OFFERED_INCOMPATIBLE_QOS_STATUS = 0x0001 << 5,

    /*e \dref_StatusKind_REQUESTED_INCOMPATIBLE_QOS_STATUS
     */
    DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS = 0x0001 << 6,

    /*e \dref_StatusKind_SAMPLE_LOST_STATUS
     */
    DDS_SAMPLE_LOST_STATUS = 0x0001 << 7,

    /*e \dref_StatusKind_SAMPLE_REJECTED_STATUS
     */
    DDS_SAMPLE_REJECTED_STATUS = 0x0001 << 8,

    /*i \dref_StatusKind_DATA_ON_READERS_STATUS
     */
    DDS_DATA_ON_READERS_STATUS = 0x0001 << 9,

    /*e \dref_StatusKind_DATA_AVAILABLE_STATUS
     */
    DDS_DATA_AVAILABLE_STATUS = 0x0001 << 10,

    /*e \dref_StatusKind_LIVELINESS_LOST_STATUS
     */
    DDS_LIVELINESS_LOST_STATUS = 0x0001 << 11,

    /*e \dref_StatusKind_LIVELINESS_CHANGED_STATUS
     */
    DDS_LIVELINESS_CHANGED_STATUS = 0x0001 << 12,

    /*e \dref_StatusKind_PUBLICATION_MATCHED_STATUS
     */
    DDS_PUBLICATION_MATCHED_STATUS = 0x0001 << 13,

    /*e \dref_StatusKind_SUBSCRIPTION_MATCHED_STATUS
     */
    DDS_SUBSCRIPTION_MATCHED_STATUS = 0x0001 << 14

    /* --- Begin extended statuses --- */
    /* The "right"-most 24 bits of the StatusMask are reserved
     * for standard statuses. The remaining 8 bits are for extended statuses.
     * DDS_RELIABLE_WRITER_CACHE_CHANGED_STATUS    = 0x00000001 << 24,
     */
} DDS_StatusKind;

/* ================================================================= */
/*                    Thread Setting                                 */
/* ================================================================= */
/*e \dref_ThreadSettingsGroupDocs
 */

/*e \dref_ThreadSettingsKindMask
 */
typedef DDS_UnsignedLong DDS_ThreadSettingsKindMask;

/*e \dref_ThreadSettingsKind_THREAD_SETTINGS_KIND_MASK_DEFAULT
 */
#define DDS_THREAD_SETTINGS_KIND_MASK_DEFAULT \
    ((DDS_ThreadSettingsKindMask)OSAPI_THREAD_DEFAULT_OPTIONS)

/*e \dref_ThreadSettingsKind
 */
typedef enum
{
    /*e \dref_ThreadSettingsKind_THREAD_SETTINGS_FLOATING_POINT
     */
    DDS_THREAD_SETTINGS_FLOATING_POINT = OSAPI_THREAD_FLOATING_POINT,

    /*e \dref_ThreadSettingsKind_THREAD_SETTINGS_STDIO
     */
    DDS_THREAD_SETTINGS_STDIO = OSAPI_THREAD_STDIO,

    /*e \dref_ThreadSettingsKind_THREAD_SETTINGS_REALTIME_PRIORITY
     */
    DDS_THREAD_SETTINGS_REALTIME_PRIORITY = OSAPI_THREAD_REALTIME_PRIORITY,

    /*e \dref_ThreadSettingsKind_THREAD_SETTINGS_PRIORITY_ENFORCE
     */
    DDS_THREAD_SETTINGS_PRIORITY_ENFORCE = OSAPI_THREAD_PRIORITY_ENFORCE,

    /*e \dref_ThreadSettingsKind_THREAD_SETTINGS_CANCEL_ASYNCHRONOUS
     */
    DDS_THREAD_SETTINGS_CANCEL_ASYNCHRONOUS =
            OSAPI_THREAD_CANCEL_ASYNCHRONOUS
} DDS_ThreadSettingsKind;

/*e \dref_ThreadSettings_t
 */
struct DDS_ThreadSettings
{
    /*e \dref_ThreadSettings_t_mask
     */
    DDS_ThreadSettingsKindMask mask;
    /*e \dref_ThreadSettings_t_priority
     */
    DDS_Long priority;
    /*e \dref_ThreadSettings_t_stack_size
     */
    DDS_Long stack_size;
};

/*i @ingroup DDSThreadSettingsModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_ThreadSettings);

#define DDS_ThreadSettings_t DDS_ThreadSettings
#define DDS_THREAD_SETTINGS_DEFAULT \
{ \
    DDS_THREAD_SETTINGS_KIND_MASK_DEFAULT, \
    OSAPI_THREAD_PRIORITY_NORMAL, \
    16*1024\
}

/* ================================================================= */
/*                QoS Types                                          */
/* ================================================================= */
/*e \dref_QosPoliciesGroupDocs
 */

/*i \dref_QosPolicyId_t
 */
/*i
 * Note that the value of these constants disagree with the values of
 * the corresponding parameter IDs in the RTPS protocol. This conflict
 * is unavoidable since these values are given in the DDS specification,
 * which is not tied to RTPS.
 */
typedef enum
{
    /*e \dref_QosPolicyId_t_INVALID_QOS_POLICY_ID
     */
    DDS_INVALID_QOS_POLICY_ID = 0,

    /*i \dref_QosPolicyId_t_USERDATA_QOS_POLICY_ID
     */
    DDS_USERDATA_QOS_POLICY_ID = 1,

    /*i \dref_QosPolicyId_t_DURABILITY_QOS_POLICY_ID
     */
    DDS_DURABILITY_QOS_POLICY_ID = 2,

    /*i \dref_QosPolicyId_t_PRESENTATION_QOS_POLICY_ID
     */
    DDS_PRESENTATION_QOS_POLICY_ID = 3,

    /*e \dref_QosPolicyId_t_DEADLINE_QOS_POLICY_ID
     */
    DDS_DEADLINE_QOS_POLICY_ID = 4,

    /*i \dref_QosPolicyId_t_LATENCYBUDGET_QOS_POLICY_ID
     */
    DDS_LATENCYBUDGET_QOS_POLICY_ID = 5,

    /*e \dref_QosPolicyId_t_OWNERSHIP_QOS_POLICY_ID
     */
    DDS_OWNERSHIP_QOS_POLICY_ID = 6,

    /*e \dref_QosPolicyId_t_OWNERSHIPSTRENGTH_QOS_POLICY_ID
     */
    DDS_OWNERSHIPSTRENGTH_QOS_POLICY_ID = 7,

    /*e \dref_QosPolicyId_t_LIVELINESS_QOS_POLICY_ID
     */
    DDS_LIVELINESS_QOS_POLICY_ID = 8,

    /*e \dref_QosPolicyId_t_TIMEBASEDFILTER_QOS_POLICY_ID
     */
    DDS_TIMEBASEDFILTER_QOS_POLICY_ID = 9,

    /*i \dref_QosPolicyId_t_PARTITION_QOS_POLICY_ID
     */
    DDS_PARTITION_QOS_POLICY_ID = 10,

    /*e \dref_QosPolicyId_t_RELIABILITY_QOS_POLICY_ID
     */
    DDS_RELIABILITY_QOS_POLICY_ID = 11,

    /*i \dref_QosPolicyId_t_DESTINATIONORDER_QOS_POLICY_ID
     */
    DDS_DESTINATIONORDER_QOS_POLICY_ID = 12,

    /*e \dref_QosPolicyId_t_HISTORY_QOS_POLICY_ID
     */
    DDS_HISTORY_QOS_POLICY_ID = 13,

    /*i \dref_QosPolicyId_t_RESOURCELIMITS_QOS_POLICY_ID
     */
    DDS_RESOURCELIMITS_QOS_POLICY_ID = 14,

    /*e \dref_QosPolicyId_t_ENTITYFACTORY_QOS_POLICY_ID
     */
    DDS_ENTITYFACTORY_QOS_POLICY_ID = 15,

    /*i \dref_QosPolicyId_t_WRITERDATALIFECYCLE_QOS_POLICY_ID
     */
    DDS_WRITERDATALIFECYCLE_QOS_POLICY_ID = 16,

    /*i \dref_QosPolicyId_t_READERDATALIFECYCLE_QOS_POLICY_ID
     */
    DDS_READERDATALIFECYCLE_QOS_POLICY_ID = 17,

    /*i \dref_QosPolicyId_t_TOPICDATA_QOS_POLICY_ID
     */
    DDS_TOPICDATA_QOS_POLICY_ID = 18,

    /*i \dref_QosPolicyId_t_GROUPDATA_QOS_POLICY_ID
     */
    DDS_GROUPDATA_QOS_POLICY_ID = 19,

    /*i \dref_QosPolicyId_t_TRANSPORTPRIORITY_QOS_POLICY_ID
     */
    DDS_TRANSPORTPRIORITY_QOS_POLICY_ID = 20,

    /*i \dref_QosPolicyId_t_LIFESPAN_QOS_POLICY_ID
     */
    DDS_LIFESPAN_QOS_POLICY_ID = 21,

    /*i \dref_QosPolicyId_t_DURABILITYSERVICE_QOS_POLICY_ID
     */
    DDS_DURABILITYSERVICE_QOS_POLICY_ID = 22,

    /* --- Extension QoS policies: --- */
    /*i
     * We start at 1000 to allow room for future policies added to
     * the \dds specification.
     */
    /*e \dref_QosPolicyId_t_WIREPROTOCOL_QOS_POLICY_ID
     */
    DDS_WIREPROTOCOL_QOS_POLICY_ID = 1000,

    /*e \dref_QosPolicyId_t_DISCOVERY_QOS_POLICY_ID
     */
    DDS_DISCOVERY_QOS_POLICY_ID = 1001,

    /*i \dref_QosPolicyId_t_DATAREADERRESOURCELIMITS_QOS_POLICY_IDNOTSUPPORTED
     */
    DDS_DATAREADERRESOURCELIMITS_QOS_POLICY_ID = 1003,

    /*i \dref_QosPolicyId_t_DATAWRITERRESOURCELIMITS_QOS_POLICY_IDNOTSUPPORTED
     */
    DDS_DATAWRITERRESOURCELIMITS_QOS_POLICY_ID = 1004,

    /*e \dref_QosPolicyId_t_DATAREADERPROTOCOL_QOS_POLICY_ID
     */
    DDS_DATAREADERPROTOCOL_QOS_POLICY_ID = 1005,

    /*e \dref_QosPolicyId_t_DATAWRITERPROTOCOL_QOS_POLICY_ID
     */
    DDS_DATAWRITERPROTOCOL_QOS_POLICY_ID = 1006,

    /*e \dref_QosPolicyId_t_DOMAINPARTICIPANTRESOURCELIMITS_QOS_POLICY_ID
     */
    DDS_DOMAINPARTICIPANTRESOURCELIMITS_QOS_POLICY_ID = 1007,

    /*e \dref_QosPolicyId_t_EVENT_QOS_POLICY_ID
     */
    DDS_EVENT_QOS_POLICY_ID = 1008,

    /*i \dref_QosPolicyId_t_DATABASE_QOS_POLICY_ID
     */
    DDS_DATABASE_QOS_POLICY_ID = 1009,

    /*i \dref_QosPolicyId_t_RECEIVERPOOL_QOS_POLICY_ID
     */
    DDS_RECEIVERPOOL_QOS_POLICY_ID = 1010,

    /*i \dref_QosPolicyId_t_DISCOVERYCONFIG_QOS_POLICY_ID
     */
    DDS_DISCOVERYCONFIG_QOS_POLICY_ID = 1011,

    /*i \dref_QosPolicyId_t_EXCLUSIVEAREA_QOS_POLICY_ID
     */
    DDS_EXCLUSIVEAREA_QOS_POLICY_ID = 1012,

    /*i \dref_QosPolicyId_t_USEROBJECT_QOS_POLICY_ID
     */
    DDS_USEROBJECT_QOS_POLICY_ID = 1013,

    /*e \dref_QosPolicyId_t_SYSTEMRESOURCELIMITS_QOS_POLICY_ID
     */
    DDS_SYSTEMRESOURCELIMITS_QOS_POLICY_ID = 1014,

    /*i \dref_QosPolicyId_t_TRANSPORTSELECTION_QOS_POLICY_ID
     */
    DDS_TRANSPORTSELECTION_QOS_POLICY_ID = 1015,

    /*i \dref_QosPolicyId_t_TRANSPORTUNICAST_QOS_POLICY_ID
     */
    DDS_TRANSPORTUNICAST_QOS_POLICY_ID = 1016,

    /*i \dref_QosPolicyId_t_TRANSPORTMULTICAST_QOS_POLICY_ID
     */
    DDS_TRANSPORTMULTICAST_QOS_POLICY_ID = 1017,

    /*i \dref_QosPolicyId_t_TRANSPORTBUILTIN_QOS_POLICY_ID
     */
    DDS_TRANSPORTBUILTIN_QOS_POLICY_ID = 1018,

    /*e \dref_QosPolicyId_t_TYPESUPPORT_QOS_POLICY_ID
     */
    DDS_TYPESUPPORT_QOS_POLICY_ID = 1019,

    /*i \dref_QosPolicyId_t_PROPERTY_QOS_POLICY_ID
     */
    DDS_PROPERTY_QOS_POLICY_ID = 1020,

    /*i \dref_QosPolicyId_t_PUBLISHMODE_QOS_POLICY_ID
     */
    DDS_PUBLISHMODE_QOS_POLICY_ID = 1021,

    /*i \dref_QosPolicyId_t_ASYNCHRONOUSPUBLISHER_QOS_POLICY_ID
     */
    DDS_ASYNCHRONOUSPUBLISHER_QOS_POLICY_ID = 1022,

    /*e \dref_QosPolicyId_t_ENTITYNAME_QOS_POLICY_ID
     */
    DDS_ENTITYNAME_QOS_POLICY_ID = 1023,

    /*i \dref_QosPolicyId_t_STATISTICS_QOS_POLICY_ID
     */
    DDS_STATISTICS_QOS_POLICY_ID = 1024,

    /*i \dref_QosPolicyId_t_SERVICE_QOS_POLICY_ID
     */
    DDS_SERVICE_QOS_POLICY_ID = 1025,

    /*i \dref_QosPolicyId_t_BATCHING_QOS_POLICY_ID
     */
    DDS_BATCHING_QOS_POLICY_ID = 1026
} DDS_QosPolicyId_t;

/* ----------------------------------------------------------------- */
/*i \dref_QosPolicyCount
 */
struct DDS_QosPolicyCount
{
    /*e \dref_QosPolicyCount_policy_id
     */
    DDS_QosPolicyId_t policy_id;

    /*e \dref_QosPolicyCount_count
     */
    DDS_Long count;
};

#define MICRO_DDS_SEQUENCE_MODULE
#ifdef MICRO_DDS_SEQUENCE_MODULE
DDS_SEQUENCE(DDS_QosPolicyCountSeq, struct DDS_QosPolicyCount)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*i \dref_QosPolicyCountSeq
 */
struct DDS_QosPolicyCountSeq
{
};
#endif
#endif

/* ================================================================= */
/*                         Entity Types                              */
/* ================================================================= */

/*e \dref_EntityKind_t
*/
typedef enum
{
    /*e \dref_EntityKind_t_UNKNOWN_ENTITY_KIND
     */
    DDS_UNKNOWN_ENTITY_KIND = 0,
    /*e \dref_EntityKind_t_PARTICIPANT_ENTITY_KIND
     */
    DDS_PARTICIPANT_ENTITY_KIND = 1,
    /*e \dref_EntityKind_t_PUBLISHER_ENTITY_KIND
     */
    DDS_PUBLISHER_ENTITY_KIND = 2,
    /*e \dref_EntityKind_t_SUBSCRIBER_ENTITY_KIND
     */
    DDS_SUBSCRIBER_ENTITY_KIND = 3,
    /*e \dref_EntityKind_t_TOPIC_ENTITY_KIND
     */
    DDS_TOPIC_ENTITY_KIND = 4,
    /*e \dref_EntityKind_t_DATAREADER_ENTITY_KIND
     */
    DDS_DATAREADER_ENTITY_KIND = 5,
    /*e \dref_EntityKind_t_DATAWRITER_ENTITY_KIND
     */
    DDS_DATAWRITER_ENTITY_KIND = 6
} DDS_EntityKind_t;

/* ----------------------------------------------------------------- */
/*                DEADLINE                                           */
/* ----------------------------------------------------------------- */
/*e \dref_DeadlineQosGroupDocs
 */

/*e \dref_DEADLINE_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_DEADLINE_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_DeadlineQosPolicy
 */
struct DDS_DeadlineQosPolicy
{
    /*e \dref_DeadlineQosPolicy_period
     */
    struct DDS_Duration_t period;
};

#define DDS_Duration_to_ms(dur_) (((dur_).sec*1000) + ((dur_).nanosec/1000000))

/*i @ingroup DDSDeadlineQosModule DDSQosTypesModule
 * Default is infinite
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_DeadlineQosPolicy);
#define DDS_DEADLINE_QOS_POLICY_DEFAULT  \
{{ 0x7fffffffL, 0xffffffffUL }}

/* ----------------------------------------------------------------- */
/*                OWNERSHIP                                          */
/* ----------------------------------------------------------------- */
/*e \dref_OwnershipQosGroupDocs
 */

/*e \dref_OWNERSHIP_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_OWNERSHIP_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_OwnershipQosPolicyKind
 */
typedef enum
{
    /*e \dref_OwnershipQosPolicyKind_SHARED_OWNERSHIP_QOS
     */
    DDS_SHARED_OWNERSHIP_QOS,

    /*e \dref_OwnershipQosPolicyKind_EXCLUSIVE_OWNERSHIP_QOS
     */
    DDS_EXCLUSIVE_OWNERSHIP_QOS
} DDS_OwnershipQosPolicyKind;

/* ----------------------------------------------------------------- */
/*e \dref_OwnershipQosPolicy
 */
struct DDS_OwnershipQosPolicy
{
    /*e \dref_OwnershipQosPolicy_kind
     */
    DDS_OwnershipQosPolicyKind kind;
};

/*i @ingroup DDSOwnershipQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_OwnershipQosPolicy);
#define DDS_OWNERSHIP_QOS_POLICY_DEFAULT { DDS_SHARED_OWNERSHIP_QOS }

/* ----------------------------------------------------------------- */
/*                OWNERSHIP_STRENGTH                                 */
/* ----------------------------------------------------------------- */
/*e \dref_OwnershipStrengthQosGroupDocs
 */

/*e \dref_OWNERSHIPSTRENGTH_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_OWNERSHIPSTRENGTH_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_OwnershipStrengthQosPolicy
 */
struct DDS_OwnershipStrengthQosPolicy
{
    /*e \dref_OwnershipStrengthQosPolicy_value
     */
    DDS_Long value;
};

/*i @ingroup DDSOwnershipStrengthQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_OwnershipStrengthQosPolicy);
#define DDS_OWNERSHIP_STRENGTH_QOS_POLICY_DEFAULT { 0L }

/* ----------------------------------------------------------------- */
/*                LIVELINESS                                         */
/* ----------------------------------------------------------------- */
/*e \dref_LivelinessQosGroupDocs
 */

/*e \dref_LIVELINESS_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_LIVELINESS_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_LivelinessQosPolicyKind
 */
typedef enum
{
    /*i \dref_LivelinessQosPolicyKind_AUTOMATIC_LIVELINESS_QOS
     */
    DDS_AUTOMATIC_LIVELINESS_QOS,

    /*i \dref_LivelinessQosPolicyKind_MANUAL_BY_PARTICIPANT_LIVELINESS_QOS
     */
    DDS_MANUAL_BY_PARTICIPANT_LIVELINESS_QOS,

    /*e \dref_LivelinessQosPolicyKind_MANUAL_BY_TOPIC_LIVELINESS_QOS
     */
    DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS
} DDS_LivelinessQosPolicyKind;

/* ----------------------------------------------------------------- */
/*e \dref_LivelinessQosPolicy
 */
struct DDS_LivelinessQosPolicy
{
    /*e \dref_LivelinessQosPolicy_kind
     */
    DDS_LivelinessQosPolicyKind kind;

    /*e \dref_LivelinessQosPolicy_lease_duration
     */
    struct DDS_Duration_t lease_duration;
};

/*i @ingroup DDSLivelinessQosModule DDSQosTypesModule
 * Default least duration is infinite
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_LivelinessQosPolicy);

#define DDS_LIVELINESS_QOS_POLICY_DEFAULT \
{ DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS, {0x7fffffff, 0xffffffffUL}}

/* ----------------------------------------------------------------- */
/*                TIME_BASED_FILTER                                    */
/* ----------------------------------------------------------------- */
/*i \dref_TimeBasedFilterQosGroupDocs
 */

/*i \dref_TIMEBASEDFILTER_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_TIMEBASEDFILTER_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*i \dref_TimeBasedFilterQosPolicy
 */
struct DDS_TimeBasedFilterQosPolicy
{
    /*e \dref_TimeBasedFilterQosPolicy_minimum_separation
     */
    struct DDS_Duration_t minimum_separation;
};

/*i @ingroup DDSTimeBasedFilterQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_TimeBasedFilterQosPolicy);
#define DDS_TIME_BASED_FILTER_QOS_POLICY_DEFAULT { {0L, 0UL} }

/* ----------------------------------------------------------------- */
/*                RELIABILITY                                       */
/* ----------------------------------------------------------------- */
/*e \dref_ReliabilityQosGroupDocs
 */

/*e \dref_RELIABILITY_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_RELIABILITY_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_ReliabilityQosPolicyKind
 */
typedef enum
{
    /*e \dref_ReliabilityQosPolicyKind_BEST_EFFORT_RELIABILITY_QOS
     */
    DDS_BEST_EFFORT_RELIABILITY_QOS = 0x01,

    /*e \dref_ReliabilityQosPolicyKind_RELIABLE_RELIABILITY_QOS
     */
    DDS_RELIABLE_RELIABILITY_QOS = 0x02     /* TODO: change to 0x03 to comply with RTPS spec */
} DDS_ReliabilityQosPolicyKind;

/* ----------------------------------------------------------------- */
/*e \dref_ReliabilityQosPolicy
 */
struct DDS_ReliabilityQosPolicy
{
    /*e \dref_ReliabilityQosPolicy_kind
     */
    DDS_ReliabilityQosPolicyKind kind;
    /*i \dref_ReliabilityQosPolicy_max_blocking_time
     */
    struct DDS_Duration_t max_blocking_time;
};

/*i @ingroup DDSReliabilityQosModule DDSQosTypesModule
 * Default reliability kind is different for Topic, Datawriter and DataReader.
 * Default reliability max blocking time is 100 milliseconds
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_ReliabilityQosPolicy);
#define DDS_RELIABILITY_QOS_POLICY_DEFAULT \
          { DDS_BEST_EFFORT_RELIABILITY_QOS, \
          {0L, 0UL} }


/* ----------------------------------------------------------------- */
/*                HISTORY                                            */
/* ----------------------------------------------------------------- */
/*e \dref_HistoryQosGroupDocs
 */

/*e \dref_HISTORY_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_HISTORY_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_HistoryQosPolicyKind
 */
typedef enum
{
    /*e \dref_HistoryQosPolicyKind_KEEP_LAST_HISTORY_QOS
     */
    DDS_KEEP_LAST_HISTORY_QOS,

    /*i \dref_HistoryQosPolicyKind_KEEP_ALL_HISTORY_QOS
     */
    DDS_KEEP_ALL_HISTORY_QOS
} DDS_HistoryQosPolicyKind;

/* ----------------------------------------------------------------- */
/*e \dref_HistoryQosPolicy
 */
struct DDS_HistoryQosPolicy
{
    /*e \dref_HistoryQosPolicy_kind
     */
    DDS_HistoryQosPolicyKind kind;

    /*e \dref_HistoryQosPolicy_depth
     */
    DDS_Long depth;
};

/*i @ingroup DDSHistoryQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_HistoryQosPolicy);
#define DDS_HISTORY_QOS_POLICY_DEFAULT { DDS_KEEP_LAST_HISTORY_QOS, \
                                         1L /* depth */ }

/* ----------------------------------------------------------------- */
/*                DURABILITY                                         */
/* ----------------------------------------------------------------- */
/*e \dref_DurabilityQosGroupDocs
*/

/*e \dref_DURABILITY_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_DURABILITY_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_DurabilityQosPolicyKind
*/
typedef enum
{
    /*e \dref_DurabilityQosPolicyKind_VOLATILE_DURABILITY_QOS
     */
    DDS_VOLATILE_DURABILITY_QOS,

    /*e \dref_DurabilityQosPolicyKind_TRANSIENT_LOCAL_DURABILITY_QOS
     */
    DDS_TRANSIENT_LOCAL_DURABILITY_QOS,

    /*i \dref_DurabilityQosPolicyKind_TRANSIENT_DURABILITY_QOS
     */
    DDS_TRANSIENT_DURABILITY_QOS,

    /*i \dref_DurabilityQosPolicyKind_PERSISTENT_DURABILITY_QOS
     */
    DDS_PERSISTENT_DURABILITY_QOS
} DDS_DurabilityQosPolicyKind;

/* ----------------------------------------------------------------- */
/*e \dref_DurabilityQosPolicy
 */
struct DDS_DurabilityQosPolicy
{
    /*e \dref_DurabilityQosPolicy_kind
     */
    DDS_DurabilityQosPolicyKind kind;
};

DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_DurabilityQosPolicy);

/*i @ingroup DDSDurabilityQosModule DDSQosModule
 */
#define DDS_DURABILITY_QOS_POLICY_DEFAULT \
{ DDS_VOLATILE_DURABILITY_QOS }

/* ----------------------------------------------------------------- */
/*                RESOURCE_LIMITS                                    */
/* ----------------------------------------------------------------- */
/*e \dref_ResourceLimitsQosGroupDocs
 */

/*e \dref_RESOURCELIMITS_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_RESOURCELIMITS_QOS_POLICY_NAME;

/*e \dref_LENGTH_UNLIMITED
 */
extern DDSCDllVariable const DDS_Long DDS_LENGTH_UNLIMITED;

/* ----------------------------------------------------------------- */
/*e \dref_ResourceLimitsQosPolicy
 */
struct DDS_ResourceLimitsQosPolicy
{
    /*e \dref_ResourceLimitsQosPolicy_max_samples
     */
    DDS_Long max_samples;

    /*e \dref_ResourceLimitsQosPolicy_max_instances
     */
    DDS_Long max_instances;

    /*e \dref_ResourceLimitsQosPolicy_max_samples_per_instance
     */
    DDS_Long max_samples_per_instance;
};

/*i @ingroup DDSResourceLimitsQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_ResourceLimitsQosPolicy);
#define DDS_RESOURCE_LIMITS_QOS_POLICY_DEFAULT { \
    1L, /* max_samples */ \
    1L, /* max_instances */ \
    1L, /* max_samples_per_instance */ \
}

/* ----------------------------------------------------------------- */
/*                DATAREADER_RESOURCE_LIMITS                         */
/* ----------------------------------------------------------------- */
/*e \dref_DataReaderResourceLimitsQosGroupDocs
 */

/*e \dref_DATAREADERRESOURCELIMITS_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const
                                 DDS_DATAREADERRESOURCELIMITS_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_DataReaderResourceLimitsQosPolicy
 */
struct DDS_DataReaderResourceLimitsQosPolicy
{
    /*e \dref_DataReaderResourceLimitsQosPolicy_max_remote_writers
     */
    DDS_Long max_remote_writers;

    /*e \dref_DataReaderResourceLimitsQosPolicy_max_remote_writers_per_instance
     */
    DDS_Long max_remote_writers_per_instance;

    /*e \dref_DataReaderResourceLimitsQosPolicy_max_samples_per_remote_writer
     */
    DDS_Long max_samples_per_remote_writer;

    /*e \dref_DataReaderResourceLimitsQosPolicy_max_outstanding_reads
     */
    DDS_Long max_outstanding_reads;
};

/*i @ingroup DDSResourceLimitsQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_DataReaderResourceLimitsQosPolicy);
#define DDS_DATAREADERRESOURCE_LIMITS_QOS_POLICY_DEFAULT { \
    1L, /* max_remote_writers */ \
    1L, /* max_remote_writers_per_instance */ \
    1L, /* max_samples_per_remote_writer */ \
    1L /* max_outstanding_reads */ \
}

/* ----------------------------------------------------------------- */
/*                ENTITY_FACTORY                                     */
/* ----------------------------------------------------------------- */
/*e \dref_EntityFactoryQosGroupDocs
 */

/*e \dref_ENTITYFACTORY_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_ENTITYFACTORY_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_EntityFactoryQosPolicy
 */
struct DDS_EntityFactoryQosPolicy
{
    /*e \dref_EntityFactoryQosPolicy_autoenable_created_entities
     */
    DDS_Boolean autoenable_created_entities;
};

/*i @ingroup DDSEntityFactoryQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_EntityFactoryQosPolicy);
#define DDS_ENTITY_FACTORY_QOS_POLICY_DEFAULT   { DDS_BOOLEAN_TRUE }

/* ----------------------------------------------------------------- */
/*                EXTENDED QOS SUPPORT                               */
/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/*e
 * \dref_ExtendedQosSupportGroupDocs
 */
#define DDS_LENGTH_UNLIMITED   (-1)

/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
/*                TYPESUPPORT                                        */
/* ----------------------------------------------------------------- */
/*i \dref_TypeSupportQosGroupDocs
*/

/*i \dref_TYPESUPPORT_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_TYPESUPPORT_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*i \dref_TypeSupportQosPolicy
 */
struct DDS_TypeSupportQosPolicy
{
    /*e \dref_TypeSupportQosPolicy_plugin_data
     */
    void *plugin_data;
};

/*i @ingroup DDSTypeSupportQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_TypeSupportQosPolicy);
#define DDS_TYPESUPPORT_QOS_POLICY_DEFAULT \
{ NULL }

/* ----------------------------------------------------------------- */
/*             SYSTEM_RESOURCE_LIMITS_X (eXtension QoS)              */
/* ----------------------------------------------------------------- */
/*e \dref_SystemResourceLimitsQosGroupDocs
 */

/*e \dref_SYSTEMRESOURCELIMITS_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_SYSTEMRESOURCELIMITS_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */

/*e \dref_SystemResourceLimitsQosPolicy
 */
struct DDS_SystemResourceLimitsQosPolicy
{
    /*e \dref_SystemResourceLimitsQosPolicy_max_participants
     */
    DDS_Long max_participants;
};

/*i @ingroup DDSSystemResourceLimitsQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_SystemResourceLimitsQosPolicy);
#define DDS_SYSTEM_RESOURCE_LIMITS_QOS_POLICY_DEFAULT { 1L }

/* ----------------------------------------------------------------- */
/*                WIRE_PROTOCOL_X (eXtension QoS)                    */
/* ----------------------------------------------------------------- */
/*e \dref_WireProtocolQosGroupDocs
 */

/*e \dref_RtpsWellKnownPorts_t
 */
#define DDS_RtpsWellKnownPorts NETIO_RtpsPortParam
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_RtpsWellKnownPorts);

/*e \dref_RTI_BACKWARDS_COMPATIBLE_RTPS_WELL_KNOWN_PORTS
 */
extern DDSCDllVariable struct DDS_RtpsWellKnownPorts_t
                            DDS_RTI_BACKWARDS_COMPATIBLE_RTPS_WELL_KNOWN_PORTS;

/*e \dref_INTEROPERABLE_RTPS_WELL_KNOWN_PORTS
 */
extern DDSCDllVariable struct DDS_RtpsWellKnownPorts_t
                                        DDS_INTEROPERABLE_RTPS_WELL_KNOWN_PORTS;

/*i @ingroup DDSWireProtocolQosModule
    these defaults are compatible with >= 4.2d
 */
#define DDS_RtpsWellKnownPorts_t DDS_RtpsWellKnownPorts
#define DDS_RTPS_WELL_KNOWN_PORTS_DEFAULT \
{ \
    7400, /* port_base */ \
    250, /* domain_id_gain */ \
    2, /* participant_id_gain */ \
    0, /* builtin_multicast_port_offset */ \
    10, /* builtin_unicast_port_offset */ \
    1, /* user_multicast_port_offset */ \
    11 /* user_unicast_port_offset */ \
}

/*e \dref_WIREPROTOCOL_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_WIREPROTOCOL_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_WireProtocolQosPolicy
 */
struct DDS_WireProtocolQosPolicy
{
    /*e \dref_WireProtocolQosPolicy_participant_id
     */
    DDS_Long participant_id;
    /*e \dref_WireProtocolQosPolicy_rtps_host_id
     */
    DDS_UnsignedLong rtps_host_id;

    /*e \dref_WireProtocolQosPolicy_rtps_app_id
     */
    DDS_UnsignedLong rtps_app_id;

    /*e \dref_WireProtocolQosPolicy_rtps_instance_id
     */
    DDS_UnsignedLong rtps_instance_id;
    /*e \dref_WireProtocolQosPolicy_rtps_well_known_ports
     */
    struct DDS_RtpsWellKnownPorts_t rtps_well_known_ports;
};

DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_WireProtocolQosPolicy);

/*e \dref_WireProtocolQosPolicy_RTPS_AUTO_ID
 */
    enum
    {
        /*e \dref_WireProtocolQosPolicy_RTPS_AUTO_ID
         */
        DDS_RTPS_AUTO_ID = 0
    };

/*i @ingroup DDSWireProtocolQosModule
 */
#define DDS_WIRE_PROTOCOL_QOS_POLICY_DEFAULT {      \
-1 /* auto participant_id */,                       \
DDS_RTPS_AUTO_ID /* rtps_host_id */,                \
DDS_RTPS_AUTO_ID /* rtps_app_id */,                 \
DDS_RTPS_AUTO_ID /* rtps_instance_id */,            \
DDS_RTPS_WELL_KNOWN_PORTS_DEFAULT /* rtps_well_known_ports */\
}

/* ----------------------------------------------------------------- */

#define DDS_Locator RTPS_Locator
#define DDS_Locator_t RTPS_Locator_t
#define DDS_LocatorUdpv4_t RTPS_LocatorUdpv4_t
#define DDS_LOCATOR_DEFAULT RTPS_LOCATOR_DEFAULT
#define DDS_LOCATOR_UDPV4_DEFAULT RTPS_LOCATOR_UDPV4_DEFAULT

#ifdef DOXYGEN_DOCUMENTATION_ONLY

/*e \dref_Locator_t
 */
struct DDS_Locator_t
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

#endif                          /*DOXYGEN_DOCUMENTATION_ONLY */


#define DDS_Locator_copy RTPS_Locator_copy
#define DDS_Locator_initialize RTPS_Locator_initialize
#define DDS_Locator_finalize RTPS_Locator_finalize
#define DDS_Locator_is_equal RTPS_Locator_is_equal
#define DDS_Locator_is_udpv4 RTPS_Locator_is_udpv4
#define DDS_Locator_is_consistent RTPS_Locator_is_consistent

DDS_SEQUENCE(DDS_LocatorSeq, struct RTPS_Locator)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*e \dref_LocatorSeq
 */
    struct DDS_LocatorSeq
    {
    };
#endif

/*e
 * \dref_Locator_t_INVALID
 */
#define DDS_LOCATOR_INVALID RTPS_LOCATOR_INVALID

/*e
 * \dref_Locator_t_DEFAULT
 */
#define DDS_LOCATOR_DEFAULT RTPS_LOCATOR_DEFAULT

/*e
 * \dref_Locator_t_KIND_INVALID
 */
#define DDS_LOCATOR_KIND_INVALID RTPS_LOCATOR_KIND_INVALID

/*e
 * \dref_Locator_t_PORT_INVALID
 */
#define DDS_LOCATOR_PORT_INVALID RTPS_LOCATOR_PORT_INVALID

/*e
 * \dref_Locator_t_ADDRESS_INVALID
 */
#define DDS_LOCATOR_ADDRESS_INVALID RTPS_LOCATOR_ADDRESS_INVALID

/*e
 * \dref_Locator_t_KIND_UDPv4
 */
#define DDS_LOCATOR_KIND_UDPv4 RTPS_LOCATOR_KIND_UDPv4

/*e
 * \dref_Locator_t_KIND_UDPv6
 */
#define DDS_LOCATOR_KIND_UDPv6 RTPS_LOCATOR_KIND_UDPv6

/*e
 * \dref_Locator_t_KIND_RESERVED
 */
#define DDS_LOCATOR_KIND_RESERVED RTPS_LOCATOR_KIND_RESERVED

/*e
 * \dref_Locator_t_KIND_SHMEM
 */
#define DDS_LOCATOR_KIND_SHMEM RTPS_LOCATOR_KIND_SHMEM

/* ----------------------------------------------------------------- */

/*e \dref_ProtocolVersion_t
 */
typedef struct DDS_ProtocolVersion
{
    /*e \dref_ProtocolVersion_t_major
     */
    DDS_Octet major;

    /*e \dref_ProtocolVersion_t_minor
     */
    DDS_Octet minor;
} DDS_ProtocolVersion_t;

DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_ProtocolVersion);

#define DDS_PROTOCOL_VERSION_DEFAULT { 0, 0 }

    /*e \dref_ProtocolVersion_t_PROTOCOLVERSION_1_0
     */
#define DDS_PROTOCOLVERSION_1_0 { 1, 0 }
    /*e \dref_ProtocolVersion_t_PROTOCOLVERSION_1_1
     */
#define DDS_PROTOCOLVERSION_1_1 { 1, 1 }
    /*e \dref_ProtocolVersion_t_PROTOCOLVERSION_1_2
     */
#define DDS_PROTOCOLVERSION_1_2 { 1, 2 }
    /*e \dref_ProtocolVersion_t_PROTOCOLVERSION
     */
#define DDS_PROTOCOLVERSION_2_0 { 2, 0 }
    /*e \dref_ProtocolVersion_t_PROTOCOLVERSION_2_1
     */
#define DDS_PROTOCOLVERSION_2_1 { 2, 1 }
    /*e \dref_ProtocolVersion_t_PROTOCOLVERSION
     */
#define DDS_PROTOCOLVERSION { 2, 1 }

/*e \dref_VendorId_t_LENGTH_MAX
 */
#define DDS_VENDOR_ID_LENGTH_MAX 2

/*e \dref_VendorId_t
 */
struct DDS_VendorId
{
    /*e \dref_VendorId_t_vendorId
     */
    DDS_Octet vendorId[DDS_VENDOR_ID_LENGTH_MAX];
};

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*e \dref_VendorId_t
 */
struct DDS_VendorId_t
{
    /*e \dref_VendorId_t_vendorId
     */
    DDS_Octet vendorId[DDS_VENDOR_ID_LENGTH_MAX];
};
#endif                          /*DOXYGEN_DOCUMENTATION_ONLY */


DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_VendorId);
#define DDS_VendorId_t DDS_VendorId
#define DDS_VENDOR_ID_DEFAULT { {0, 0} }

/*i \dref_VendorId_t_VENDORID_UNKNOWN
 */
#define DDS_VENDORID_UNKNOWN { {0, 0} }

/* ----------------------------------------------------------------- */

/*e \dref_ProductVersion_t
 */
struct DDS_ProductVersion
{
    /*e \dref_ProductVersion_t_major
     */
    DDS_Char major;
    /*e \dref_ProductVersion_t_minor
     */
    DDS_Char minor;
    /*e \dref_ProductVersion_t_release
     */
    DDS_Char release;
    /*e \dref_ProductVersion_t_revision
     */
    DDS_Char revision;
};


#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*e \dref_ProductVersion_t
 */
struct DDS_ProductVersion_t
{
    /*e \dref_ProductVersion_t_major
     */
    DDS_Char major;
    /*e \dref_ProductVersion_t_minor
     */
    DDS_Char minor;
    /*e \dref_ProductVersion_t_release
     */
    DDS_Char release;
    /*e \dref_ProductVersion_t_revision
     */
    DDS_Char revision;
};
#endif                          /*DOXYGEN_DOCUMENTATION_ONLY */

#define DDS_ProductVersion_t DDS_ProductVersion
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_ProductVersion);

/*e \dref_ProductVersion_t_UNKNOWN
 */
#define DDS_PRODUCTVERSION_UNKNOWN { 0, 0, '0', 0 }
#define DDS_PRODUCTVERSION_DEFAULT { 2, 0, 'c', 1 }


/* ----------------------------------------------------------------- */
/*               DATA_READER_PROTOCOL_X (eXtension QoS)              */
/* ----------------------------------------------------------------- */
/*e \dref_DataReaderProtocolQosGroupDocs
 */

/*e \dref_DATAREADERPROTOCOL_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_DATAREADERPROTOCOL_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_DataReaderProtocolQosPolicy
 */
struct DDS_DataReaderProtocolQosPolicy
{
    /*e \dref_DataReaderProtocolQosPolicy_rtps_object_id
     */
    DDS_UnsignedLong rtps_object_id;
};

/*i @ingroup DDSDurabilityQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_DataReaderProtocolQosPolicy);
#define DDS_DATA_READER_PROTOCOL_QOS_POLICY_DEFAULT \
{ \
  DDS_RTPS_AUTO_ID \
}

/* ----------------------------------------------------------------- */

/*e \dref_RtpsReliableWriterProtocol_t
 */
 struct DDS_RtpsReliableWriterProtocol_t
 {
    /*e \dref_RtpsReliableWriterProtocol_t_heartbeat_period
     */
    struct DDS_Duration_t heartbeat_period;

    /*e \dref_RtpsReliableWriterProtocol_t_heartbeats_per_max_samples
     */
    DDS_Long heartbeats_per_max_samples;

    /*e \dref_RtpsReliableWriterProtocol_t_max_send_window
     */
    DDS_Long max_send_window;
 };

/*i @ingroup DDSRtpsReliableWriterProtocolModule DDSQosModule
 */
#define DDS_RTPS_RELIABLE_WRITER_PROTOCOL_DEFAULT { \
    {3,0}, /* hb_period */ \
    8, /* hb_per_max_sampels */ \
    0 /* max_send_window */ \
}

/* ----------------------------------------------------------------- */
/*               DATA_WRITER_PROTOCOL_X (eXtension QoS)              */
/* ----------------------------------------------------------------- */
/*e \dref_DataWriterProtocolQosGroupDocs
 */

/*e \dref_DATAWRITERPROTOCOL_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_DATAWRITERPROTOCOL_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_DataWriterProtocolQosPolicy
 */
struct DDS_DataWriterProtocolQosPolicy
{
    /*e \dref_DataWriterProtocolQosPolicy_rtps_object_id
     */
    DDS_UnsignedLong rtps_object_id;

    /*e \dref_DataWriterProtocolQosPolicy_rtps_reliable_writer
     */
    struct DDS_RtpsReliableWriterProtocol_t rtps_reliable_writer;
};

/*i @ingroup DDSDataWriterProtocolQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_DataWriterProtocolQosPolicy);
#define DDS_DATA_WRITER_PROTOCOL_QOS_POLICY_DEFAULT \
{ DDS_RTPS_AUTO_ID, \
  DDS_RTPS_RELIABLE_WRITER_PROTOCOL_DEFAULT \
}

/* ----------------------------------------------------------------- */
/*                    TRANSPORT_QOS_POLICY (eXtension QoS)           */
/* ----------------------------------------------------------------- */
/*e \dref_TransportQosGroupDocs
 */

/*e \dref_TRANSPORT_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_TRANSPORT_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */
/*e \dref_TransportQosPolicy
 */
struct DDS_TransportQosPolicy
{
    /*e \dref_TransportQosPolicy_value
     */
    struct DDS_StringSeq enabled_transports;
};

/*i @ingroup DDTransportQosModule DDSQosTypesModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_TransportQosPolicy);
#define DDS_TRANSPORT_QOS_POLICY_DEFAULT \
{\
    DDS_SEQUENCE_INITIALIZER(DDS_String)\
}

/* ----------------------------------------------------------------- */
/*     DOMAIN_PARTICIPANT_RESOURCE_LIMITS_X (eXtension QoS)          */
/* ----------------------------------------------------------------- */
/*e \dref_DomainParticipantResourceLimitsQosGroupDocs
 */

/*e \dref_DOMAINPARTICIPANTRESOURCELIMITS_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const
                            DDS_DOMAINPARTICIPANTRESOURCELIMITS_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */

/*e \dref_DomainParticipantResourceLimitsQosPolicy
 */
struct DDS_DomainParticipantResourceLimitsQosPolicy
{
    /*e \dref_DomainParticipantResourceLimitsQosPolicy_local_writer_allocation
     */
    DDS_Long local_writer_allocation;
    /*e \dref_DomainParticipantResourceLimitsQosPolicy_local_reader_allocation
     */
    DDS_Long local_reader_allocation;
    /*e \dref_DomainParticipantResourceLimitsQosPolicy_local_publisher_allocation
     */
    DDS_Long local_publisher_allocation;
    /*e \dref_DomainParticipantResourceLimitsQosPolicy_local_subscriber_allocation
     */
    DDS_Long local_subscriber_allocation;
    /*e \dref_DomainParticipantResourceLimitsQosPolicy_local_topic_allocation
     */
    DDS_Long local_topic_allocation;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_local_type_allocation
     */
    DDS_Long local_type_allocation;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_remote_participant_allocation
     */
    DDS_Long remote_participant_allocation;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_remote_writer_allocation
     */
    DDS_Long remote_writer_allocation;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_remote_reader_allocation
     */
    DDS_Long remote_reader_allocation;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_matching_writer_reader_pair_allocation
     */
    DDS_Long matching_writer_reader_pair_allocation;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_matching_reader_writer_pair_allocation
     */
    DDS_Long matching_reader_writer_pair_allocation;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_max_receive_ports
     */
    DDS_Long max_receive_ports;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_max_destination_ports
     */
    DDS_Long max_destination_ports;

    /*e \dref_DomainParticipantResourceLimitsQosPolicy_max_gather_destinations
     */
    DDS_Long max_gather_destinations;
};

/*i @ingroup DDSDomainParticipantResourceLimitsQosModule
 * 
 * This constant is used below in
 * DDS_DOMAIN_PARTICIPANT_RESOURCE_LIMITS_QOS_POLICY_DEFAULT; other types
 * should not use it alone.
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_DomainParticipantResourceLimitsQosPolicy);
#define DDS_DomainParticipantResourceLimitsQosPolicy_MATCH_INIT (32L)

/*i @ingroup DDSDomainParticipantResourceLimitsQosModule
 * 
 * Several fields in this structure are actually never used. These fields
 * should be explicitly initialized when this structure is copied
 * from a lower layer structure to prevent the values from being left in
 * an uninitialized state. The unused fields include:
 * 
 * - local_publisher_allocation.max_count
 * - local_subscriber_allocation.max_count
 * - local_topic_allocation.max_count
 * - matching_writer_reader_pair_allocation.initial_count
 * - matching_writer_reader_pair_allocation.max_count
 * - matching_reader_writer_pair_allocation.initial_count
 * - matching_reader_writer_pair_allocation.max_count
 */

#define DDS_DOMAIN_PARTICIPANT_RESOURCE_LIMITS_QOS_POLICY_DEFAULT \
{1L, /*local_writer_allocation*/      \
 1L, /*local_reader_allocation*/      \
 1L, /*local_topic_allocation*/       \
 1L, /*local_type_allocation*/        \
 2L, /*remote_participant_allocation*/\
 1L, /*remote_publisher_allocation*/  \
 1L, /*remote_subscriber_allocation*/ \
 1L, /*remote_writer_allocation*/     \
 1L, /*remote_reader_allocation*/     \
 DDS_DomainParticipantResourceLimitsQosPolicy_MATCH_INIT, \
 DDS_DomainParticipantResourceLimitsQosPolicy_MATCH_INIT, \
 8L,\
 8L,\
 8,  /* max_gather_destinations */\
}

/* ----------------------------------------------------------------- */
/* ----------------------------------------------------------------- */
/*                      EVENT_X (eXtension QoS)                      */
/* ----------------------------------------------------------------- */
/*i \dref_EventQosGroupDocs
 */

/*i \dref_EVENT_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_EVENT_QOS_POLICY_NAME;

/* ----------------------------------------------------------------- */

/*i \dref_EventQosPolicy
 */
struct DDS_EventQosPolicy
{
    /*i \dref_EventQosPolicy_thread
     */
    struct DDS_ThreadSettings_t thread;
    /*i \dref_EventQosPolicy_initial_count
     */
    DDS_Long initial_count;
    /*i \dref_EventQosPolicy_max_count
     */
    DDS_Long max_count;
};

/*i @ingroup DDSEventQosModule
 */
DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_EventQosPolicy);
#define DDS_EVENT_QOS_POLICY_DEFAULT \
{ DDS_THREAD_SETTINGS_DEFAULT, \
  256, /* initial_count */ \
  256/* max_count: unlimited */ \
}

/* ----------------------------------------------------------------- */
#define DDS_BUILTIN_TOPIC_KEY_TYPE_NATIVE_LENGTH (4)

/*e \dref_BuiltinTopicKey_t
 */
typedef struct DDS_BuiltinTopicKey_t
{
  /*e \dref_BuiltinTopicKey_t_value
   */
  DDS_BUILTIN_TOPIC_KEY_TYPE_NATIVE value[DDS_BUILTIN_TOPIC_KEY_TYPE_NATIVE_LENGTH];
} DDS_BuiltinTopicKey_t;

/*i @ingroup BuiltinTopicGroupDocs
 */
#define DDS_BuiltinTopicKey_t_INITIALIZER { {0, 0, 0, 0} }

/*e \dref_BuiltinTopicKey_t_AUTO
 */
extern DDSCDllVariable const struct DDS_BuiltinTopicKey_t DDS_BUILTINTOPICKEY_AUTO;

/*e \dref_BuiltinTopicKey_t_UNKNOWN
 */
extern DDSCDllVariable const struct DDS_BuiltinTopicKey_t DDS_BUILTINTOPICKEY_UNKNOWN;

/*i \dref_BuiltinTopicKey_t_PREFIX_UNKNOWN
 */
extern DDSCDllVariable const struct DDS_BuiltinTopicKey_t DDS_BUILTINTOPICKEY_PREFIX_UNKNOWN;

/*i \dref_BuiltinTopicKey_t_PREFIX_UNKNOWN
 */
extern DDSCDllVariable const struct DDS_BuiltinTopicKey_t DDS_BUILTINTOPICKEY_PREFIX_AUTO;

/*i \ingroup BuiltinTopicGroupDocs_equals  
*/
DDSCDllExport DDS_Boolean
DDS_BuiltinTopicKey_equals(const DDS_BuiltinTopicKey_t *a,
                           const DDS_BuiltinTopicKey_t *b);

/*i \ingroup BuiltinTopicGroupDocs_prefix_equals  
 */
DDSCDllExport DDS_Boolean
DDS_BuiltinTopicKey_prefix_equals(const DDS_BuiltinTopicKey_t *a,
                                  const DDS_BuiltinTopicKey_t *b);

/*i \ingroup BuiltinTopicGroupDocs_suffix_equals  
 */
DDSCDllExport DDS_Boolean
DDS_BuiltinTopicKey_suffix_equals(const DDS_BuiltinTopicKey_t *a,
                                  const DDS_BuiltinTopicKey_t *b);

/*i \ingroup BuiltinTopicGroupDocs_copy_prefix  
 */
DDSCDllExport void
DDS_BuiltinTopicKey_copy_prefix(DDS_BuiltinTopicKey_t *a,
                                const DDS_BuiltinTopicKey_t *b);

/*i \ingroup BuiltinTopicGroupDocs_copy_suffix  
 */
DDSCDllExport void
DDS_BuiltinTopicKey_copy_suffix(DDS_BuiltinTopicKey_t *a,
                                const DDS_BuiltinTopicKey_t *b);

/*i \ingroup BuiltinTopicGroupDocs_from_guid
 */
DDSCDllExport void
DDS_BuiltinTopicKey_from_guid(DDS_BuiltinTopicKey_t *in,
                              const DDS_InstanceHandle_t *out);

/*i \ingroup BuiltinTopicGroupDocs_to_guid
 */
DDSCDllExport void
DDS_BuiltinTopicKey_to_guid(DDS_InstanceHandle_t *out,
                            const DDS_BuiltinTopicKey_t *in);

/* ----------------------------------------------------------------- */

/* ================================================================= */
/*                 Listeners                                         */
/* ================================================================= */
/*e \dref_EntityModuleDocs
 */

/*ce \dref_Listener
 */
struct DDS_Listener
{
    /*ce \dref_Listener_listener_data
     */
    void *listener_data;
};

/*ce \dref_Listener_INITIALIZER
 */
#define DDS_Listener_INITIALIZER    { NULL }

/* ================================================================= */
/*                         Entity typedef                            */
/* ================================================================= */

/*ce \dref_Entity
 */
typedef struct DDS_EntityImpl DDS_Entity;

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*ce \dref_DomainEntity
 */
typedef struct DDS_DomainEntityImpl DDS_DomainEntity;
#endif                          /*DOXYGEN_DOCUMENTATION_ONLY */

/* ================================================================= */
/*                            DDS_Entity                             */
/* ================================================================= */

/*ce \dref_Entity_enable
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Entity_enable(DDS_Entity * self);

/*ci \dref_Entity_is_enabled
 */
DDSCDllExport DDS_Boolean
DDS_Entity_is_enabled(DDS_Entity * self);

/*ci \dref_Entity_get_instance_handle
 */
DDSCDllExport DDS_InstanceHandle_t
DDS_Entity_get_instance_handle(DDS_Entity * self);

/*ci \dref_Entity_get_instance_handle
 */
DDSCDllExport DDS_EntityKind_t
DDS_Entity_get_entity_kind(DDS_Entity *self);

/* ----------------------------------------------------------------- */
/*                ENTITY_NAME                                        */
/* ----------------------------------------------------------------- */
/*e \dref_EntityNameQosGroupDocs
 */

/*e \dref_ENTITYNAME_QOS_POLICY_NAME
 */
extern DDSCDllVariable const char *const DDS_ENTITYNAME_QOS_POLICY_NAME;

/*e \dref_AUTO_NAME_ENTITY
 */
extern DDSCDllVariable const char *const DDS_AUTO_NAME_ENTITY;

/* ----------------------------------------------------------------- */
#define DDS_ENTITYNAME_QOS_NAME_MAX 255

/*e \dref_EntityNameQosPolicy
 */
struct DDS_EntityNameQosPolicy
{
    /*e \dref_EntityNameQosPolicy_name
     */
    char name[DDS_ENTITYNAME_QOS_NAME_MAX];
};

DDSC_VARIABLE_LENGTH_VALUE_TYPE_SUPPORT_BASIC(DDS_EntityNameQosPolicy);

/*i @ingroup DDSEntityNameQosModule DDSQosTypesModule
 */
#define DDS_ENTITY_NAME_QOS_POLICY_DEFAULT   { {'\0'} }

/* ================================================================= */
/*                            RTI_Management                         */
/* ================================================================= */

struct RTI_ManagementQosPolicy
{
    DDS_Boolean is_hidden;
    DDS_Boolean is_anonymous;
};

#define RTI_MANAGEMENT_QOS_POLICY_DEFAULT { \
	DDS_BOOLEAN_FALSE,\
    DDS_BOOLEAN_FALSE\
}

/* ================================================================= */
/*                            DDS_DomainEntity                       */
/* ================================================================= */

/*ce \dref_DomainParticipant
 */
typedef struct DDS_DomainParticipantImpl DDS_DomainParticipant;

/* ----------------------------------------------------------------- */

#define DDS_IntegerWithRange(__value, __low,__high) \
    ((__value) < (__low) ? -1 : ((__value) > (__high) ? 1 : 0))


/* ================================================================= */
/*                            DDSHST_History                           */
/* ================================================================= */
typedef enum
{
    DDSHST_RETCODE_ERROR = -1000,
    DDSHST_RETCODE_NOSPACE,
    DDSHST_RETCODE_EXISTS,
    DDSHST_RETCODE_NOT_EXISTS,
    DDSHST_RETCODE_INVALID_PROPERTY,
    DDSHST_RETCODE_SUCCESS = 0
} DDSHST_ReturnCode_T;

typedef enum
{
    DDSHST_REPLACE_POLICY_KIND_OLDEST,
    DDSHST_REPLACE_POLICY_KIND_NONE
} DDSHST_ReplacePolicyKind_T;


/* ----------------------------------------------------------------- */
/*                WRITE_PARAMS                                       */
/* ----------------------------------------------------------------- */

/*e \dref_SampleIdentity_t
 */
struct DDS_SampleIdentity_t 
{
    /*e \dref_SampleIdentity_t_writer_guid */
    struct DDS_GUID_t writer_guid;
    /*e \dref_SampleIdentity_t_sequence_number */
    struct DDS_SequenceNumber_t sequence_number;
};


/*ce \dref_SampleIdentity_t
 */
#define DDS_SAMPLE_IDENTITY_UNKNOWN \
{ \
    DDS_GUID_INITIALIZER, \
    DDS_SEQUENCE_NUMBER_UNKNOWN \
}

/*e \dref_WriteParams
 */
struct DDS_WriteParams_t 
{
    /*e \dref_WriteParams_identity
    */
    struct DDS_SampleIdentity_t identity;

    /*e \dref_WriteParams_related_sample_identity
    */
    struct DDS_SampleIdentity_t related_sample_identity;

    /*e \dref_WriteParams_source_timestamp
     */
    struct DDS_Time_t source_timestamp;

    /*e \dref_WriteParams_handle
     */
    DDS_InstanceHandle_t handle;
};


#define DDS_WRITEPARAMS_DEFAULT \
{ \
    DDS_SAMPLE_IDENTITY_UNKNOWN, \
    DDS_SAMPLE_IDENTITY_UNKNOWN, \
    DDS_TIME_ZERO, \
    DDS_HANDLE_NIL_NATIVE \
}

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* dds_c_infrastructure_h */
