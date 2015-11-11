/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_subscription_h
#define dds_c_subscription_h


#ifndef dds_c_type_h
#include "dds_c/dds_c_type.h"
#endif

#ifndef dds_c_topic_h
#include "dds_c/dds_c_topic.h"
#endif

#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* ================================================================= */
/*                         Typedef for DataReader                    */
/* ================================================================= */

/*ce \dref_DataReader
 */
typedef struct DDS_DataReaderImpl DDS_DataReader;


/* ================================================================= */
/*                         Typedef for Subscriber                    */
/* ================================================================= */

/*ce \dref_Subscriber
 */
typedef struct DDS_SubscriberImpl DDS_Subscriber;

/* TODO: implement DDS_Subscriber_copy_from_topic_qos() */

/* ================================================================= */
/*	                      Status                                 */
/* ================================================================= */

/* -------------------------------------- */
/*    Requested Deadline Missed Status    */
/* -------------------------------------- */

/*i \dref_RequestedDeadlineMissedStatus
 */
struct DDS_RequestedDeadlineMissedStatus
{
    /*e \dref_RequestedDeadlineMissedStatus_total_count
     */
    DDS_Long total_count;

    /*e \dref_RequestedDeadlineMissedStatus_total_count_change
     */
    DDS_Long total_count_change;

    /*e \dref_RequestedDeadlineMissedStatus_last_instance_handle
     */
    DDS_InstanceHandle_t last_instance_handle;
};

/*ci \dref_RequestedDeadlineMissedStatus_INITIALIZER
 */
#define DDS_RequestedDeadlineMissedStatus_INITIALIZER \
        { 0L, 0L, DDS_HANDLE_NIL_NATIVE }

/*ci \dref_RequestedDeadlineMissedStatus_initialize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_RequestedDeadlineMissedStatus_initialize(
        struct DDS_RequestedDeadlineMissedStatus *self);

/*ci \dref_RequestedDeadlineMissedStatus_copy
 */
DDSCDllExport DDS_ReturnCode_t
DDS_RequestedDeadlineMissedStatus_copy(
        struct DDS_RequestedDeadlineMissedStatus *self,
        const struct DDS_RequestedDeadlineMissedStatus *source);

/*ci \dref_RequestedDeadlineMissedStatus_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_RequestedDeadlineMissedStatus_finalize(
        struct DDS_RequestedDeadlineMissedStatus *self);

/* ------------------------------- */
/*    Liveliness Changed Status    */
/* ------------------------------- */

/*e \dref_LivelinessChangedStatus
 */
struct DDS_LivelinessChangedStatus
{

    /*e \dref_LivelinessChangedStatus_alive_count
     */
    DDS_Long alive_count;

    /*e \dref_LivelinessChangedStatus_not_alive_count
     */
    DDS_Long not_alive_count;

    /*e \dref_LivelinessChangedStatus_alive_count_change
     */
    DDS_Long alive_count_change;

    /*e \dref_LivelinessChangedStatus_not_alive_count_change
     */
    DDS_Long not_alive_count_change;

    /*e \dref_LivelinessChangedStatus_last_publication_handle
     */
    DDS_InstanceHandle_t last_publication_handle;
};

/*ce \dref_LivelinessChangedStatus_INITIALIZER
 */
#define DDS_LivelinessChangedStatus_INITIALIZER { 0L, 0L, 0L, 0L, \
    DDS_HANDLE_NIL_NATIVE }

/*ci \dref_LivelinessChangedStatus_initialize
 */
/* TODO: implementation */
DDSCDllExport DDS_ReturnCode_t
DDS_LivelinessChangedStatus_initialize(
        struct DDS_LivelinessChangedStatus *self);

/*ci \dref_LivelinessChangedStatus_copy
 */
/* TODO: implementation */
DDSCDllExport DDS_ReturnCode_t
DDS_LivelinessChangedStatus_copy(
        struct DDS_LivelinessChangedStatus *self,
        const struct DDS_LivelinessChangedStatus *source);

/*ci \dref_LivelinessChangedStatus_finalize
 */
/* TODO: implementation */
DDSCDllExport DDS_ReturnCode_t
DDS_LivelinessChangedStatus_finalize(struct DDS_LivelinessChangedStatus *self);

/* --------------------------------------- */
/*    Requested Incompatible QoS Status    */
/* --------------------------------------- */

/*e \dref_RequestedIncompatibleQosStatus
 */
struct DDS_RequestedIncompatibleQosStatus
{
    /*e \dref_RequestedIncompatibleQosStatus_total_count
     */
    DDS_Long total_count;

    /*e \dref_RequestedIncompatibleQosStatus_total_count_change
     */
    DDS_Long total_count_change;

    /*e \dref_RequestedIncompatibleQosStatus_last_policy_id
     */
    DDS_QosPolicyId_t last_policy_id;

    /*e \dref_RequestedIncompatibleQosStatus_policies
     */
    struct DDS_QosPolicyCountSeq policies;
};

/*ce \dref_RequestedIncompatibleQosStatus_INITIALIZER
 */
#define DDS_RequestedIncompatibleQosStatus_INITIALIZER \
        { 0L, 0L, DDS_INVALID_QOS_POLICY_ID, DDS_SEQUENCE_INITIALIZER(struct DDS_QosPolicyCount) }

/*ci \dref_RequestedIncompatibleQosStatus_initialize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_RequestedIncompatibleQosStatus_initialize(
        struct DDS_RequestedIncompatibleQosStatus *self);

/*ci \dref_RequestedIncompatibleQosStatus_copy
 */
DDSCDllExport DDS_ReturnCode_t
DDS_RequestedIncompatibleQosStatus_copy(
        struct DDS_RequestedIncompatibleQosStatus *self,
        const struct DDS_RequestedIncompatibleQosStatus *source);

/*ci \dref_RequestedIncompatibleQosStatus_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_RequestedIncompatibleQosStatus_finalize(
        struct DDS_RequestedIncompatibleQosStatus *self);

/* ------------------------- */
/*   Sample Lost Status      */
/* ------------------------- */

/*i \dref_SampleLostStatus
 */
struct DDS_SampleLostStatus
{
    /*e  \dref_SampleLostStatus_total_count
     */
    DDS_Long total_count;

    /*e  \dref_SampleLostStatus_total_count_change
     */
    DDS_Long total_count_change;
};

/*ci \dref_SampleLostStatus_INITIALIZER
 */
#define DDS_SampleLostStatus_INITIALIZER { 0L, 0L }

/*ci \dref_SampleLostStatus_initialize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SampleLostStatus_initialize(struct DDS_SampleLostStatus *self);

/*ci \dref_SampleLostStatus_copy
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SampleLostStatus_copy(
        struct DDS_SampleLostStatus *self,
        const struct DDS_SampleLostStatus *source);

/*ci \dref_SampleLostStatus_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SampleLostStatus_finalize(
        struct DDS_SampleLostStatus *self);

/* ----------------------------------- */
/*  Sample Rejected Status Kind enum   */
/* ----------------------------------- */

/*i \dref_SampleRejectedStatusKind
 */
typedef enum
{
    /*e \dref_SampleRejectedStatusKind_NOT_REJECTED
     */
    DDS_NOT_REJECTED,

    /*e \dref_SampleRejectedStatusKind_REJECTED_BY_INSTANCES_LIMIT
     */
    DDS_REJECTED_BY_INSTANCES_LIMIT,

    /*e \dref_SampleRejectedStatusKind_REJECTED_BY_SAMPLES_LIMIT
     */
    DDS_REJECTED_BY_SAMPLES_LIMIT,

    /*e \dref_SampleRejectedStatusKind_REJECTED_BY_SAMPLES_PER_INSTANCE_LIMIT
     */
    DDS_REJECTED_BY_SAMPLES_PER_INSTANCE_LIMIT,

    /*e \dref_SampleRejectedStatusKind_REJECTED_BY_REMOTE_WRITERS_LIMIT
     */
    DDS_REJECTED_BY_REMOTE_WRITERS_LIMIT,

    /*e \dref_SampleRejectedStatusKind_REJECTED_BY_REMOTE_WRITERS_PER_INSTANCE_LIMIT
     */
    DDS_REJECTED_BY_REMOTE_WRITERS_PER_INSTANCE_LIMIT,

    /*e \dref_SampleRejectedStatusKind_REJECTED_BY_SAMPLES_PER_REMOTE_WRITER_LIMIT
     */
    DDS_REJECTED_BY_SAMPLES_PER_REMOTE_WRITER_LIMIT
} DDS_SampleRejectedStatusKind;

/*i deprecated */
#define DDS_REJECTED_BY_INSTANCE_LIMIT DDS_REJECTED_BY_INSTANCES_LIMIT

/* --------------------------------- */
/*  Sample Rejected Status struct    */
/* --------------------------------- */

/*i \dref_SampleRejectedStatusTODO
 */
struct DDS_SampleRejectedStatus
{

    /*e \dref_SampleRejectedStatus_total_count
     */
    DDS_Long total_count;

    /*e \dref_SampleRejectedStatus_total_count_change
     */
    DDS_Long total_count_change;

    /*e \dref_SampleRejectedStatus_last_reason
     */
    DDS_SampleRejectedStatusKind last_reason;

    /*e \dref_SampleRejectedStatus_last_instance_handle
     */
    DDS_InstanceHandle_t last_instance_handle;
};

/*ci \dref_SampleRejectedStatus_INITIALIZERTODO
 */
#define DDS_SampleRejectedStatus_INITIALIZER \
        { 0L, 0L, DDS_NOT_REJECTED,DDS_HANDLE_NIL_NATIVE }

/*ci \dref_SampleRejectedStatus_initializeTODO
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SampleRejectedStatus_initialize(struct DDS_SampleRejectedStatus *self);

/*ci \dref_SampleRejectedStatus_copy
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SampleRejectedStatus_copy(
        struct DDS_SampleRejectedStatus *self,
        const struct DDS_SampleRejectedStatus *source);

/*ci \dref_SampleRejectedStatus_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SampleRejectedStatus_finalize(struct DDS_SampleRejectedStatus *self);

/* --------------------------------- */
/*  Subscription Match Status struct */
/* --------------------------------- */

/*e \dref_SubscriptionMatchedStatus
 */
struct DDS_SubscriptionMatchedStatus
{
    /*e \dref_SubscriptionMatchedStatus_total_count
     */
    DDS_Long total_count;

    /*e \dref_SubscriptionMatchedStatus_total_count_change
     */
    DDS_Long total_count_change;

    /*e \dref_SubscriptionMatchedStatus_current_count
     */
    DDS_Long current_count;

    /*e \dref_SubscriptionMatchedStatus_current_count_change
     */
    DDS_Long current_count_change;

    /*e \dref_SubscriptionMatchedStatus_last_publication_handle
     */
    DDS_InstanceHandle_t last_publication_handle;
};

/*ce \dref_SubscriptionMatchedStatus_INITIALIZER
 */
#define DDS_SubscriptionMatchedStatus_INITIALIZER \
        { 0L, 0L, 0L, 0L, DDS_HANDLE_NIL_NATIVE}

/*ci \dref_SubscriptionMatchedStatus_initialize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SubscriptionMatchedStatus_initialize(
        struct DDS_SubscriptionMatchedStatus *self);

/*ci \dref_SubscriptionMatchedStatus_copy
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SubscriptionMatchedStatus_copy(
        struct DDS_SubscriptionMatchedStatus *self,
        const struct DDS_SubscriptionMatchedStatus *source);

/*ci \dref_SubscriptionMatchedStatus_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SubscriptionMatchedStatus_finalize(
        struct DDS_SubscriptionMatchedStatus *self);

/* ================================================================= */
/*                      Sample States                                */
/* ================================================================= */

/* ---------------------------- */
/*    Sample State Kind enum    */
/* ---------------------------- */

/*e \dref_SampleStateKind
 */
typedef enum
{
    /*e \dref_SampleStateKind_READ_SAMPLE_STATE
     */
    DDS_READ_SAMPLE_STATE = 0x0001 << 0,

    /*e \dref_SampleStateKind_NOT_READ_SAMPLE_STATE
     */
    DDS_NOT_READ_SAMPLE_STATE = 0x0001 << 1
} DDS_SampleStateKind;

/* ---------------------------- */
/* Sample State Mask            */
/* ---------------------------- */

/*e \dref_SampleStateMask
 */
typedef DDS_UnsignedLong DDS_SampleStateMask;

/*e \dref_ANY_SAMPLE_STATE
 */
extern DDSCDllVariable const DDS_SampleStateMask DDS_ANY_SAMPLE_STATE;

/* ================================================================= */
/*                           View States                             */
/* ================================================================= */

/* ---------------------------- */
/*    View State Kind enum      */
/* ---------------------------- */

/*e \dref_ViewStateKind
 */
typedef enum
{
    /*e \dref_ViewStateKind_NEW_VIEW_STATE
     */
    DDS_NEW_VIEW_STATE = 0x0001 << 0,

    /*e \dref_ViewStateKind_NOT_NEW_VIEW_STATE
     */
    DDS_NOT_NEW_VIEW_STATE = 0x0001 << 1
} DDS_ViewStateKind;

/* ---------------------------- */
/* View State Mask              */
/* ---------------------------- */

/*e \dref_ViewStateMask
 */
 typedef DDS_UnsignedLong DDS_ViewStateMask;

/*e \dref_ANY_VIEW_STATE
 */
 extern DDSCDllVariable const DDS_ViewStateMask DDS_ANY_VIEW_STATE;

/* ================================================================= */
/*                           Instance States                         */
/* ================================================================= */

/* ---------------------------- */
/*    Instance State Kind enum  */
/* ---------------------------- */

/*e \dref_InstanceStateKind
 */
 typedef enum
 {
     /*e \dref_InstanceStateKind_ALIVE_INSTANCE_STATE
      */
     DDS_ALIVE_INSTANCE_STATE = 0x0001 << 0,

     /*e \dref_InstanceStateKind_NOT_ALIVE_DISPOSED_INSTANCE_STATE
      */
     DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE = 0x0001 << 1,

     /*e \dref_InstanceStateKind_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE
      */
     DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE = 0x0001 << 2
 } DDS_InstanceStateKind;

/* ---------------------------- */
/* Instance State Mask          */
/* ---------------------------- */

/*e \dref_InstanceStateMask
 */
typedef DDS_UnsignedLong DDS_InstanceStateMask;

/*e \dref_ANY_INSTANCE_STATE
 */
extern DDSCDllVariable const DDS_InstanceStateMask DDS_ANY_INSTANCE_STATE;

/*e \dref_NOT_ALIVE_INSTANCE_STATE
 */
extern DDSCDllVariable const DDS_InstanceStateMask DDS_NOT_ALIVE_INSTANCE_STATE;


/* ================================================================= */
/*                           Sample Info                             */
/* ================================================================= */

/* --------------------------- */
/*    Sample Info struct       */
/* --------------------------- */

/*e \dref_SampleInfo
 */
struct DDS_SampleInfo
{
    /*e \dref_SampleInfo_sample_state
     */
    DDS_SampleStateKind sample_state;

    /*e \dref_SampleInfo_view_state
     */
    DDS_ViewStateKind view_state;

    /*e \dref_SampleInfo_instance_state
     */
    DDS_InstanceStateKind instance_state;

    /*e \dref_SampleInfo_source_timestamp
     */
    struct DDS_Time_t source_timestamp;

    /*e \dref_SampleInfo_instance_handle
     */
    DDS_InstanceHandle_t instance_handle;

    /*e \dref_SampleInfo_publication_handle
     */
    DDS_InstanceHandle_t publication_handle;

    /*i \dref_SampleInfo_disposed_generation_count
     */
    DDS_Long disposed_generation_count;

    /*i \dref_SampleInfo_no_writers_generation_count
     */
    DDS_Long no_writers_generation_count;

    /*i \dref_SampleInfo_sample_rank
     */
    DDS_Long sample_rank;

    /*i \dref_SampleInfo_generation_rank
     */
    DDS_Long generation_rank;

    /*i \dref_SampleInfo_absolute_generation_rank
     */
    DDS_Long absolute_generation_rank;

    /*e \dref_SampleInfo_valid_data
     */
    DDS_Boolean valid_data;

    /*e \dref_SampleInfo_reception_timestamp
     */
    struct DDS_Time_t reception_timestamp;

    /*i \dref_SampleInfo_publication_sequence_number
     */
    struct DDS_SequenceNumber_t publication_sequence_number;

    /*i \dref_SampleInfo_reception_sequence_number
     */
    struct DDS_SequenceNumber_t reception_sequence_number;

    /*i Reserved, for internal use 
     */
    void *reserved_data;
};

DDS_SEQUENCE(DDS_SampleInfoSeq, struct DDS_SampleInfo)

#ifdef DOXYGEN_DOCUMENTATION_ONLY

/*i \dref_SampleInfoSeq
 */
    struct DDS_SampleInfoSeq
    {
    };

#endif


/* ================================================================= */
/*                          QoS                                      */
/* ================================================================= */

/* --------------------- */
/* DataReader QoS struct */
/* --------------------- */

/*e \dref_DataReaderQos
 */
struct DDS_DataReaderQos
{
    /*e \dref_DataReaderQos_deadline
     */
    struct DDS_DeadlineQosPolicy deadline;

    /*e \dref_DataReaderQos_liveliness
     */
    struct DDS_LivelinessQosPolicy liveliness;

    /*e \dref_DataReaderQos_history
     */
    struct DDS_HistoryQosPolicy history;

    /*e \dref_DataReaderQos_resource_limits
     */
    struct DDS_ResourceLimitsQosPolicy resource_limits;

    /*e \dref_DataReaderQos_ownership
     */
    struct DDS_OwnershipQosPolicy ownership;

    /*i \dref_DataReaderQos_time_based_filter
     */
    struct DDS_TimeBasedFilterQosPolicy time_based_filter;

    /*e \dref_DataReaderQos_reliability
     */
    struct DDS_ReliabilityQosPolicy reliability;

    /*e \dref_DataReaderQos_durability
     */
    struct DDS_DurabilityQosPolicy durability;

    /* --- Extensions: ---------------------------------------------------- */
    /*i \dref_DataWriterQos_type_support
     */
    struct DDS_TypeSupportQosPolicy type_support;

    /*e \dref_DataReaderQos_protocol
     */
    struct DDS_DataReaderProtocolQosPolicy protocol;

    /*e \dref_DataReaderQos_transport
     */
    struct DDS_TransportQosPolicy transport;

    /*e \dref_DataReaderQos_reader_resource_limits
     */
    struct DDS_DataReaderResourceLimitsQosPolicy reader_resource_limits;

    /*i \dref_DataReaderQos_management
     */
    struct RTI_ManagementQosPolicy management;
};

/*ci \dref_DataReaderQos_initialize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReaderQos_initialize(struct DDS_DataReaderQos *self);

/*ci \dref_DataReaderQos_copy
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReaderQos_copy(
        struct DDS_DataReaderQos *self,
        const struct DDS_DataReaderQos *source);

/*ce \dref_DataReaderQos_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReaderQos_finalize(struct DDS_DataReaderQos *self);

/*ce \dref_DataReaderQos_INITIALIZER
 */
#define DDS_DataReaderQos_INITIALIZER {                   \
    DDS_DEADLINE_QOS_POLICY_DEFAULT,                      \
    DDS_LIVELINESS_QOS_POLICY_DEFAULT,                    \
    DDS_HISTORY_QOS_POLICY_DEFAULT,                       \
    DDS_RESOURCE_LIMITS_QOS_POLICY_DEFAULT,               \
    DDS_OWNERSHIP_QOS_POLICY_DEFAULT,                     \
    DDS_TIME_BASED_FILTER_QOS_POLICY_DEFAULT,             \
    DDS_RELIABILITY_QOS_POLICY_DEFAULT,                   \
    DDS_DURABILITY_QOS_POLICY_DEFAULT,                    \
    DDS_TYPESUPPORT_QOS_POLICY_DEFAULT,                   \
    DDS_DATA_READER_PROTOCOL_QOS_POLICY_DEFAULT,          \
    DDS_TRANSPORT_QOS_POLICY_DEFAULT,                     \
    DDS_DATAREADERRESOURCE_LIMITS_QOS_POLICY_DEFAULT,     \
    RTI_MANAGEMENT_QOS_POLICY_DEFAULT                     \
}

/* --------------------- */
/* Subscriber QoS struct */
/* --------------------- */

/*e \dref_SubscriberQos
 */
struct DDS_SubscriberQos
{
    /*e \dref_SubscriberQos_entity_factory
     */
    struct DDS_EntityFactoryQosPolicy entity_factory;

    /*i \dref_SubscriberQos_management
     */
    struct RTI_ManagementQosPolicy management;
};

/*ci \dref_SubscriberQos_initialize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SubscriberQos_initialize(struct DDS_SubscriberQos *self);

/*ci \dref_SubscriberQos_copy
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SubscriberQos_copy(
        struct DDS_SubscriberQos *self,
        const struct DDS_SubscriberQos *source);

/*ce \dref_SubscriberQos_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_SubscriberQos_finalize(struct DDS_SubscriberQos *self);

/*ce \dref_SubscriberQos_INITIALIZER
 */
#define DDS_SubscriberQos_INITIALIZER   {   \
    DDS_ENTITY_FACTORY_QOS_POLICY_DEFAULT,  \
    RTI_MANAGEMENT_QOS_POLICY_DEFAULT       \
}

/* ================================================================= */
/*                       Listeners                                   */
/* ================================================================= */

/*ci @ingroup DDSReaderModule
  @brief Pointer to DDS_DataReader
 */
typedef DDS_DataReader *DDS_DataReader_ptr;

/*ci @ingroup DDSReaderModule
  @brief Note: can't use DDS_DataReader* !!
  
  The sequence definition uses a macro that will prepend 'const' to the type
  name in certain cases. The C++ compiler will interpret this 'const'
  incorrectly without the intervening typedef (it will consider the element
  type to be a pointer to a constant datareader, when what is desired in a
  constant pointer to a non-constant datareader).
*/
DDS_SEQUENCE(DDS_DataReaderSeq, DDS_DataReader_ptr)

#ifdef DOXYGEN_DOCUMENTATION_ONLY

/*ce \dref_DataReaderSeq
 */
    struct DDS_DataReaderSeq
    {
    };
#endif

/* -------------------------------------- */
/* DataReader Listener callback typedefs  */
/* -------------------------------------- */

/*ci \dref_DataReaderListener_RequestedDeadlineMissedCallback
 */
typedef void
(*DDS_DataReaderListener_RequestedDeadlineMissedCallback)(
        void *listener_data,
        DDS_DataReader* reader,
        const struct DDS_RequestedDeadlineMissedStatus *status);


/*ce \dref_DataReaderListener_LivelinessChangedCallback
 */
typedef void
(*DDS_DataReaderListener_LivelinessChangedCallback)(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_LivelinessChangedStatus *status);

/*ce \dref_DataReaderListener_RequestedIncompatibleQosCallback
 */
typedef void
(*DDS_DataReaderListener_RequestedIncompatibleQosCallback)(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_RequestedIncompatibleQosStatus *status);

/*ci \dref_DataReaderListener_SampleRejectedCallbackTODO
 */
typedef void
(*DDS_DataReaderListener_SampleRejectedCallback)(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_SampleRejectedStatus *status);

/*ce \dref_DataReaderListener_DataAvailableCallback
 */
typedef void
(*DDS_DataReaderListener_DataAvailableCallback)(
        void *listener_data,
        DDS_DataReader *reader);

/*ce \dref_DataReaderListener_SubscriptionMatchedCallback
 */
typedef void
(*DDS_DataReaderListener_SubscriptionMatchedCallback)(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_SubscriptionMatchedStatus *status);

/*ci \dref_DataReaderListener_SampleLostCallback
 */
typedef void
(*DDS_DataReaderListener_SampleLostCallback)(
        void *listener_data,
        DDS_DataReader *reader,
        const struct DDS_SampleLostStatus *status);

/*ce \dref_DataReaderListener_BeforeSampleDeserializeCallback
 */
typedef DDS_Boolean
(*DDS_DataReaderListener_BeforeSampleDeserializeCallback)(
        void *listener_data,
        DDS_DataReader *reader,
        struct NDDS_Type_Plugin *plugin,
        struct CDR_Stream_t *stream,
        DDS_Boolean *dropped);

/*ce \dref_DataReaderListener_BeforeSampleCommitCallback
 */
typedef DDS_Boolean
(*DDS_DataReaderListener_BeforeSampleCommitCallback)(
        void *listener_data,
        DDS_DataReader *reader,
        const void *const sample,
        DDS_Boolean *dropped);

/* -------------------------- */
/* DataReader Listener struct */
/* -------------------------- */

/*ce \dref_DataReaderListener
 */
struct DDS_DataReaderListener
{
    /*ce \dref_DataReaderListener_as_listener
     */
    struct DDS_Listener as_listener;

    /*ci \dref_DataReaderListener_on_requested_deadline_missed
     */
    DDS_DataReaderListener_RequestedDeadlineMissedCallback
    on_requested_deadline_missed;

    /*ce \dref_DataReaderListener_on_requested_incompatible_qos
     */
    DDS_DataReaderListener_RequestedIncompatibleQosCallback
    on_requested_incompatible_qos;

    /*i \dref_DataReaderListener_on_sample_rejected
     */
    DDS_DataReaderListener_SampleRejectedCallback
    on_sample_rejected;

    /*ce \dref_DataReaderListener_on_liveliness_changed
     */
    DDS_DataReaderListener_LivelinessChangedCallback
    on_liveliness_changed;

    /*ce \dref_DataReaderListener_on_data_available
     */
    DDS_DataReaderListener_DataAvailableCallback
    on_data_available;

    /*ce \dref_DataReaderListener_on_subscription_matched
     */
    DDS_DataReaderListener_SubscriptionMatchedCallback
    on_subscription_matched;

    /*ci \dref_DataReaderListener_on_sample_lost
     */
    DDS_DataReaderListener_SampleLostCallback
    on_sample_lost;

    /*ce \dref_DataReaderListener_on_before_sample_deserialize
     */
    DDS_DataReaderListener_BeforeSampleDeserializeCallback
    on_before_sample_deserialize;

    /*ce \dref_DataReaderListener_on_before_sample_commit
     */
    DDS_DataReaderListener_BeforeSampleCommitCallback
    on_before_sample_commit;
};

/*ce \dref_DataReaderListener_INITIALIZER
 */
#define DDS_DataReaderListener_INITIALIZER  { \
  DDS_Listener_INITIALIZER, \
  (DDS_DataReaderListener_RequestedDeadlineMissedCallback)NULL, \
  (DDS_DataReaderListener_RequestedIncompatibleQosCallback)NULL, \
  (DDS_DataReaderListener_SampleRejectedCallback)NULL, \
  (DDS_DataReaderListener_LivelinessChangedCallback)NULL, \
  (DDS_DataReaderListener_DataAvailableCallback)NULL, \
  (DDS_DataReaderListener_SubscriptionMatchedCallback)NULL, \
  (DDS_DataReaderListener_SampleLostCallback)NULL,\
  (DDS_DataReaderListener_BeforeSampleDeserializeCallback)NULL,\
  (DDS_DataReaderListener_BeforeSampleCommitCallback)NULL \
}

/* -------------------------------------- */
/* Subscriber Listener callback typedefs  */
/* -------------------------------------- */

/*ci \dref_SubscriberListener_DataOnReadersCallback
 */
typedef void
(*DDS_SubscriberListener_DataOnReadersCallback)(
        void *listener_data,
        DDS_Subscriber *sub);


/* ----------------------------- */
/* Subscriber Listener struct    */
/* ----------------------------- */

/*ce \dref_SubscriberListener
 */
struct DDS_SubscriberListener
{

    /*ce \dref_SubscriberListener_as_datareaderlistener
     */
    struct DDS_DataReaderListener as_datareaderlistener;

    /*ci \dref_SubscriberListener_on_data_on_readers
     */
    DDS_SubscriberListener_DataOnReadersCallback on_data_on_readers;
};

/*ce \dref_SubscriberListener_INITIALIZER
 */
#define DDS_SubscriberListener_INITIALIZER  \
    { DDS_DataReaderListener_INITIALIZER, \
      (DDS_SubscriberListener_DataOnReadersCallback)NULL }

/* ================================================================= */
/*                           Subscriber                              */
/* ================================================================= */

/*e \dref_DATAREADER_QOS_DEFAULT
 */
extern DDSCDllVariable const struct DDS_DataReaderQos
    DDS_DATAREADER_QOS_DEFAULT;

/*e \dref_DATAREADER_QOS_USE_TOPIC_QOSTODO
 */
extern DDSCDllVariable const struct DDS_DataReaderQos
    DDS_DATAREADER_QOS_USE_TOPIC_QOS;

/* ----------------------------------------------------------------- */
#define DDS_Subscriber_as_entity(subscriberPtr) \
            ((DDS_Entity*) subscriberPtr)

/* ----------------------------------------------------------------- */
#ifdef DOXYGEN_DOCUMENTATION_ONLY

/*ce \dref_Subscriber_as_entity
 */
    DDS_Entity *DDS_Subscriber_as_entity(DDS_Subscriber * subscriber);

#endif                          /*DOXYGEN_DOCUMENTATION_ONLY */

/* ----------------------------------------------------------------- */
/*ce \dref_Subscriber_get_default_datareader_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_get_default_datareader_qos(
        DDS_Subscriber *self,
        struct DDS_DataReaderQos *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_Subscriber_set_default_datareader_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_set_default_datareader_qos(
        DDS_Subscriber *self,
        const struct DDS_DataReaderQos *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_Subscriber_create_datareader
 */
DDSCDllExport DDS_DataReader*
DDS_Subscriber_create_datareader(DDS_Subscriber *self,
        DDS_TopicDescription *topic,
        const struct DDS_DataReaderQos *qos,
        const struct DDS_DataReaderListener *listener,
        DDS_StatusMask mask);

/* ----------------------------------------------------------------- */
/*ce \dref_Subscriber_delete_datareader
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_delete_datareader(
        DDS_Subscriber *self,
        DDS_DataReader *a_datareader);

/* ----------------------------------------------------------------- */
/*ce \dref_Subscriber_enable
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_enable(DDS_Entity *self);

/* ----------------------------------------------------------------- */
/*ce \dref_Subscriber_delete_contained_entities
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_delete_contained_entities(DDS_Subscriber * self);

/* ----------------------------------------------------------------- */
/*ci \dref_Subscriber_lookup_datareader
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_DataReader*
DDS_Subscriber_lookup_datareader(
        DDS_Subscriber *self,
        const char *topic_name);

/* ----------------------------------------------------------------- */
/*ci \dref_Subscriber_get_datareaders
 */
/* TODO: implement? */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_get_datareaders(
        DDS_Subscriber * self,
        struct DDS_DataReaderSeq *readers,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states);

/* ----------------------------------------------------------------- */
/*ci \dref_Subscriber_notify_datareaders
 */
/* TODO: implement? */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_notify_datareaders(DDS_Subscriber * self);

/* ----------------------------------------------------------------- */
/*ce \dref_Subscriber_get_participant
 */
DDSCDllExport DDS_DomainParticipant*
DDS_Subscriber_get_participant(DDS_Subscriber * self);


/* ----------------------------------------------------------------- */
/*ce  \dref_Subscriber_set_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_set_qos(
        DDS_Subscriber* self,
        const struct DDS_SubscriberQos *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_Subscriber_get_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_get_qos(
        DDS_Subscriber *self,
        struct DDS_SubscriberQos *qos);

/* ----------------------------------------------------------------- */
/*ci \dref_Subscriber_set_listener
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_set_listener(
        DDS_Subscriber * self,
        const struct DDS_SubscriberListener *l,
        DDS_StatusMask mask);

/*ci \dref_Subscriber_get_listener
 */
DDSCDllExport struct DDS_SubscriberListener
DDS_Subscriber_get_listener(DDS_Subscriber * self);

/*ci \dref_Subscriber_get_builtin_topic_key
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Subscriber_get_builtin_topic_key(
        const DDS_Subscriber * self,
        DDS_BuiltinTopicKey_t * key);

/* ----------------------------------------------------------------- */
/*ci \dref_Subscriber_lookup_datareader_by_builtin_topic_key
 */
/* NOT IMPLEMENTED */
DDSCDllExport DDS_DataReader*
DDS_Subscriber_lookup_datareader_by_builtin_topic_key(
        DDS_Subscriber *self,
        DDS_BuiltinTopicKey_t *key);

/* ================================================================= */
/*                           Data Reader                             */
/* ================================================================= */
#define DDS_DataReader_as_entity(dataReaderPtr) \
            ((DDS_Entity*) dataReaderPtr)

/* ----------------------------------------------------------------- */
#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*ce \dref_DataReader_as_entity
 */
DDS_Entity *
DDS_DataReader_as_entity(DDS_DataReader * dataReader);
#endif                          /*DOXYGEN_DOCUMENTATION_ONLY */

/* ----------------------------------------------------------------- */
/*ce \dref_DataReader_get_topicdescription
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_enable(DDS_Entity *self);

/* ----------------------------------------------------------------- */
/*ci \dref_DataReader_get_matched_publications
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_matched_publications(
        DDS_DataReader *self,
        struct DDS_InstanceHandleSeq *publication_handles);

struct DDS_PublicationBuiltinTopicData;
/* ----------------------------------------------------------------- */
/*ci \dref_DataReader_get_matched_publication_data
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_matched_publication_data(
        DDS_DataReader *self,
        struct DDS_PublicationBuiltinTopicData *publication_data,
        const DDS_InstanceHandle_t *publication_handle);

/* ----------------------------------------------------------------- */
/*ce \dref_DataReader_get_topicdescription
 */
DDSCDllExport DDS_TopicDescription*
DDS_DataReader_get_topicdescription(DDS_DataReader * self);

/* ----------------------------------------------------------------- */
/*ce \dref_DataReader_get_subscriber
 */
DDSCDllExport DDS_Subscriber*
DDS_DataReader_get_subscriber(DDS_DataReader * self);


/* ----------------------------------------------------------------- */
/*i \dref_DataReader_get_sample_rejected_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_sample_rejected_status(
        DDS_DataReader *self,
        struct DDS_SampleRejectedStatus *status);

/* ----------------------------------------------------------------- */
/*i \dref_DataReader_get_liveliness_changed_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_liveliness_changed_status(
        DDS_DataReader *self,
        struct DDS_LivelinessChangedStatus *status);

/* ----------------------------------------------------------------- */
/*i \dref_DataReader_get_requested_deadline_missed_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_requested_deadline_missed_status(
        DDS_DataReader *self,
        struct DDS_RequestedDeadlineMissedStatus *status);

/* ----------------------------------------------------------------- */
/*i \dref_DataReader_get_requested_incompatible_qos_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_requested_incompatible_qos_status(
        DDS_DataReader *self,
        struct DDS_RequestedIncompatibleQosStatus *status);

/* ----------------------------------------------------------------- */
/*i \dref_DataReader_get_subscription_matched_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_subscription_matched_status(
        DDS_DataReader *self,
        struct DDS_SubscriptionMatchedStatus *status);

/* ----------------------------------------------------------------- */
/*i \dref_DataReader_get_sample_lost_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_sample_lost_status(
        DDS_DataReader *self,
        struct DDS_SampleLostStatus *status);

/* ----------------------------------------------------------------- */
/*ce \dref_DataReader_set_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_set_qos(
        DDS_DataReader* self,
        const struct DDS_DataReaderQos *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_DataReader_get_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_get_qos(DDS_DataReader *self,
        struct DDS_DataReaderQos *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_DataReader_set_listener
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_set_listener(
        DDS_DataReader * self,
        const struct DDS_DataReaderListener *l,
        DDS_StatusMask mask);


/*ce \dref_DataReader_get_listener
 */
DDSCDllExport struct DDS_DataReaderListener
DDS_DataReader_get_listener(DDS_DataReader *self);

/* ----------------------------------------------------------------- */

/****************** Properties Internal API **************************/

/* ================================================================= */
/*                       Subscriber Properties                       */
/* ================================================================= */

struct NDDS_Subscriber_Properties
{
    DDS_Boolean is_visible;
};

/*i \dref_Subscriber_Properties_INITIALIZER
 */
#define NDDS_Subscriber_Properties_INITIALIZER { \
DDS_BOOLEAN_TRUE }

/* ================================================================= */
/*                       DataReader Properties                       */
/* ================================================================= */


struct NDDS_DataReader_Properties
{
    int dummy;                  /* otherwise Windows doesn't compile */
};

/*i \dref_DataReader_Properties_INITIALIZER
 */
#define NDDS_DataReader_Properties_INITIALIZER {0}

#include "dds_c/dds_c_rh_plugin.h"

#define DDS_UntypedSampleSeq                      REDA_Sequence
#define DDS_UntypedSampleSeq_initialize           REDA_Sequence_initialize
#define DDS_UntypedSampleSeq_finalize             REDA_Sequence_finalize
#define DDS_UntypedSampleSeq_get_maximum          REDA_Sequence_get_maximum
#define DDS_UntypedSampleSeq_set_maximum          REDA_Sequence_set_maximum
#define DDS_UntypedSampleSeq_get_length           REDA_Sequence_get_length
#define DDS_UntypedSampleSeq_set_length           REDA_Sequence_set_length
#define DDS_UntypedSampleSeq_get_reference        REDA_Sequence_get_reference
#define DDS_UntypedSampleSeq_copy                 REDA_Sequence_copy
#define DDS_UntypedSampleSeq_is_equal             REDA_Sequence_is_equal
#define DDS_UntypedSampleSeq_loan_contiguous      REDA_Sequence_loan_contiguous
#define DDS_UntypedSampleSeq_loan_discontiguous   REDA_Sequence_loan_discontiguous
#define DDS_UntypedSampleSeq_unloan               REDA_Sequence_unloan
#define DDS_UntypedSampleSeq_has_ownership        REDA_Sequence_has_ownership
#define DDS_UntypedSampleSeq_get_buffer           REDA_Sequence_get_buffer
#define DDS_UntypedSampleSeq_set_buffer           REDA_Sequence_set_buffer
#define DDS_UntypedSampleSeq_has_discontiguous_buffer REDA_Sequence_has_discontiguous_buffer


/********************* (Untyped) DataReader API ****************************/
/*ce \dref_DataReader_read
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_read(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq,
        DDS_Long max_samples,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states);

/*ce \dref_DataReader_take
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_take(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq,
        DDS_Long max_samples,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states);

/*ce \dref_DataReader_read_next_sample
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_read_next_sample(
        DDS_DataReader *self,
        void *received_data,
        struct DDS_SampleInfo *sample_info);

/*ce \dref_DataReader_take_next_sample
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_take_next_sample(
        DDS_DataReader *self,
        void *received_data,
        struct DDS_SampleInfo *sample_info);

/*ce \dref_DataReader_read_instance
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_read_instance(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq,
        DDS_Long max_samples,
        const DDS_InstanceHandle_t *a_handle,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states);

/*ce \dref_DataReader_take_instance
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_take_instance(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq,
        DDS_Long max_samples,
        const DDS_InstanceHandle_t *a_handle,
        DDS_SampleStateMask sample_states,
        DDS_ViewStateMask view_states,
        DDS_InstanceStateMask instance_states);


/*ce \dref_DataReader_return_loan
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_return_loan(
        DDS_DataReader *self,
        struct DDS_UntypedSampleSeq *received_data,
        struct DDS_SampleInfoSeq *info_seq);

/*ce \dref_DataReader_lookup_instance
 */
DDSCDllExport DDS_InstanceHandle_t
DDS_DataReader_lookup_instance(DDS_DataReader *self,
                               const void *key_holder);

/*i \dref_DataReader_liveliness_lost
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataReader_liveliness_lost(
        DDS_DataReader *self,
        DDS_InstanceHandle_t *publication_handle);

/*i \dref_DataReader_add_anonymous_route
 */
DDSCDllExport DDS_Boolean
DDS_DataReader_add_anonymous_route(DDS_DataReader *self,
                                   struct NETIO_Address *src_writer,
                                   struct NETIO_Address *from_address);

/* ----------------------------------------------------------------- */


#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_subscription_h */
