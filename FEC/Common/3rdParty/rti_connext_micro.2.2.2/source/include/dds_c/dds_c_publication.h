/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_publication_h
#define dds_c_publication_h



#ifndef db_api_h
#include "db/db_api.h"
#endif
#ifndef dds_c_topic_h
#include "dds_c/dds_c_topic.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif


/* ================================================================= */
/*                         Typedef for DataWriter                    */
/* ================================================================= */

/*ce \dref_DataWriter
 */
typedef struct DDS_DataWriterImpl DDS_DataWriter;

/* ================================================================= */
/*                         Typedef for Publisher                     */
/* ================================================================= */

/*ce \dref_Publisher
 */
typedef struct DDS_PublisherImpl DDS_Publisher;

/* TODO: implement DDS_Publisher_copy_from_topic_qos() */

/*i @ingroup DDSPublicationModule
  @brief Pointer to DDS_Publisher.
 */
typedef DDS_Publisher *DDS_Publisher_ptr;

DDS_SEQUENCE(DDS_PublisherSeq, DDS_Publisher_ptr)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*ce \dref_PublisherSeq
 */
struct DDS_PublisherSeq
{
};
#endif

/* ================================================================= */
/*                             Status                                */
/* ================================================================= */

/*i \dref_OfferedDeadlineMissedStatus
 */
struct DDS_OfferedDeadlineMissedStatus
{
    /*e \dref_OfferedDeadlineMissedStatus_total_count
     */
    DDS_Long total_count;

    /*e \dref_OfferedDeadlineMissedStatus_total_count_change
     */
    DDS_Long total_count_change;

    /*e \dref_OfferedDeadlineMissedStatus_last_instance_handle
     */
    DDS_InstanceHandle_t last_instance_handle;
};

/*ci \dref_OfferedDeadlineMissedStatus_INITIALIZER
 */
#define DDS_OfferedDeadlineMissedStatus_INITIALIZER \
                { 0L, 0L, DDS_HANDLE_NIL_NATIVE }

/* ----------------------------------------------------------------- */
/*e \dref_LivelinessLostStatus
 */
struct DDS_LivelinessLostStatus
{
    /*e \dref_LivelinessLostStatus_total_count
     */
    DDS_Long total_count;

    /*e \dref_LivelinessLostStatus_total_count_change
     */
    DDS_Long total_count_change;
};

/*ce \dref_LivelinessLostStatus_INITIALIZER
 */
#define DDS_LivelinessLostStatus_INITIALIZER { 0L, 0L }

/* ----------------------------------------------------------------- */
/*e \dref_OfferedIncompatibleQosStatus
 */
struct DDS_OfferedIncompatibleQosStatus
{
    /*e \dref_OfferedIncompatibleQosStatus_total_count
     */
    DDS_Long total_count;

    /*e \dref_OfferedIncompatibleQosStatus_total_count_change
     */
    DDS_Long total_count_change;

    /*e \dref_OfferedIncompatibleQosStatus_last_policy_id
     */
    DDS_QosPolicyId_t last_policy_id;

    /*e \dref_OfferedIncompatibleQosStatus_policies
     */
    struct DDS_QosPolicyCountSeq policies;
};

/*ce \dref_OfferedIncompatibleQosStatus_INITIALIZER
 */
#define DDS_OfferedIncompatibleQosStatus_INITIALIZER \
        { 0L, 0L,DDS_INVALID_QOS_POLICY_ID, DDS_SEQUENCE_INITIALIZER(struct DDS_QosPolicyCount) }

/* ----------------------------------------------------------------- */
/*e \dref_PublicationMatchedStatus
 */
struct DDS_PublicationMatchedStatus
{
    /*e \dref_PublicationMatchedStatus_total_count
     */
    DDS_Long total_count;

    /*e \dref_PublicationMatchedStatus_total_count_change
     */
    DDS_Long total_count_change;

    /*e \dref_PublicationMatchedStatus_current_count
     */
    DDS_Long current_count;

    /*e \dref_PublicationMatchedStatus_current_count_change
     */
    DDS_Long current_count_change;

    /*e \dref_PublicationMatchedStatus_last_subscription_handle
     */
    DDS_InstanceHandle_t last_subscription_handle;
};

/*ce \dref_PublicationMatchedStatus_INITIALIZER
 */
#define DDS_PublicationMatchedStatus_INITIALIZER \
        { 0L, 0L, 0L, 0L, DDS_HANDLE_NIL_NATIVE }

/* ------------------------------------------------------------------------ */

/*i \dref_ReliableWriterCacheEventCount
 */
struct DDS_ReliableWriterCacheEventCount
{
    /*i \dref_ReliableWriterCacheEventCount_total_count
     */
    DDS_Long total_count;

    /*i \dref_ReliableWriterCacheEventCount_total_count_change
     */
    DDS_Long total_count_change;
};

/*i @ingroup DDSWriterModule DDSStatusModule
 */
#define DDS_ReliableWriterCacheEventCount_INITIALIZER        { 0, 0 }

/*i \dref_ReliableWriterCacheChangedStatus
 */
struct DDS_ReliableWriterCacheChangedStatus
{
    /*i \dref_ReliableWriterCacheChangedStatus_empty_reliable_writer_cache
     */
    struct DDS_ReliableWriterCacheEventCount empty_reliable_writer_cache;

    /*i \dref_ReliableWriterCacheChangedStatus_full_reliable_writer_cache
     */
    struct DDS_ReliableWriterCacheEventCount full_reliable_writer_cache;

    /*i \dref_ReliableWriterCacheChangedStatus_low_watermark_reliable_writer_cache
     */
    struct DDS_ReliableWriterCacheEventCount
    low_watermark_reliable_writer_cache;

    /*i \dref_ReliableWriterCacheChangedStatus_high_watermark_reliable_writer_cache
     */
    struct DDS_ReliableWriterCacheEventCount
    high_watermark_reliable_writer_cache;

    /*i \dref_ReliableWriterCacheChangedStatus_unacknowledged_sample_count
     */
    DDS_Long unacknowledged_sample_count;
};

/*i \dref_ReliableWriterCacheChangedStatus_INITIALIZER
 */
#define DDS_ReliableWriterCacheChangedStatus_INITIALIZER \
        { DDS_ReliableWriterCacheEventCount_INITIALIZER,   \
          DDS_ReliableWriterCacheEventCount_INITIALIZER,   \
          DDS_ReliableWriterCacheEventCount_INITIALIZER,   \
          DDS_ReliableWriterCacheEventCount_INITIALIZER,   \
          0L }

/* ------------------------------------------------------------------------ */

/*i \dref_ReliableReaderActivityChangedStatus
 */
struct DDS_ReliableReaderActivityChangedStatus
{
    /*i \dref_ReliableReaderActivityChangedStatus_active_count
     */
    DDS_Long active_count;

    /*i \dref_ReliableReaderActivityChangedStatus_inactive_count
     */
    DDS_Long inactive_count;

    /*i \dref_ReliableReaderActivityChangedStatus_active_count_change
     */
    DDS_Long active_count_change;

    /*i \dref_ReliableReaderActivityChangedStatus_inactive_count_change
     */
    DDS_Long inactive_count_change;

    /*i \dref_ReliableReaderActivityChangedStatus_last_instance_handle
     */
    DDS_InstanceHandle_t last_instance_handle;
};

/*i \dref_ReliableReaderActivityChangedStatus_INITIALIZER
 */
#define DDS_ReliableReaderActivityChangedStatus_INITIALIZER \
        {0, 0, 0, 0,DDS_HANDLE_NIL_NATIVE}

/* ----------------------------------------------------------------- */
/* ================================================================= */
/*                          QoS                                      */
/* ================================================================= */

/*e \dref_DataWriterQos
 */
struct DDS_DataWriterQos
{
    /*e \dref_DataWriterQos_deadline
     */
    struct DDS_DeadlineQosPolicy deadline;

    /*e \dref_DataWriterQos_liveliness
     */
    struct DDS_LivelinessQosPolicy liveliness;

    /*e \dref_DataWriterQos_history
     */
    struct DDS_HistoryQosPolicy history;

    /*e \dref_DataWriterQos_resource_limits
     */
    struct DDS_ResourceLimitsQosPolicy resource_limits;

    /*e \dref_DataWriterQos_ownership
     */
    struct DDS_OwnershipQosPolicy ownership;

    /*e \dref_DataWriterQos_ownership_strength
     */
    struct DDS_OwnershipStrengthQosPolicy ownership_strength;

    /*e \dref_DataWriterQos_reliability
     */
    struct DDS_ReliabilityQosPolicy reliability;

    /*e \dref_DataWriterQos_durability
     */
    struct DDS_DurabilityQosPolicy durability;

    /* --- Extensions: ---------------------------------------------------- */

    /*e \dref_DataWriterQos_protocol
     */
    struct DDS_DataWriterProtocolQosPolicy protocol;

    /*i \dref_DataWriterQos_type_support
     */
    struct DDS_TypeSupportQosPolicy type_support;

    /*e \dref_DataWriterQos_transport
     */
    struct DDS_TransportQosPolicy transport;

    /*i \dref_DataWriterQos_management
     */
    struct RTI_ManagementQosPolicy management;
};

/*ce \dref_DataWriterQos_initialize
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriterQos_initialize(struct DDS_DataWriterQos *self);

/*ce \dref_DataWriterQos_copy
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriterQos_copy(struct DDS_DataWriterQos *self,
                       const struct DDS_DataWriterQos *source);

/*ce \dref_DataWriterQos_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriterQos_finalize(struct DDS_DataWriterQos *self);

/*ce \dref_DataWriterQos_INITIALIZER
 */
#define DDS_DataWriterQos_INITIALIZER   {                 \
    DDS_DEADLINE_QOS_POLICY_DEFAULT,                      \
    DDS_LIVELINESS_QOS_POLICY_DEFAULT,                    \
    DDS_HISTORY_QOS_POLICY_DEFAULT,                       \
    DDS_RESOURCE_LIMITS_QOS_POLICY_DEFAULT,               \
    DDS_OWNERSHIP_QOS_POLICY_DEFAULT,                     \
    DDS_OWNERSHIP_STRENGTH_QOS_POLICY_DEFAULT,            \
    DDS_RELIABILITY_QOS_POLICY_DEFAULT,                   \
    DDS_DURABILITY_QOS_POLICY_DEFAULT,                    \
    DDS_DATA_WRITER_PROTOCOL_QOS_POLICY_DEFAULT,          \
    DDS_TYPESUPPORT_QOS_POLICY_DEFAULT,                   \
    DDS_TRANSPORT_QOS_POLICY_DEFAULT,                     \
    RTI_MANAGEMENT_QOS_POLICY_DEFAULT                     \
}

/* ----------------------------------------------------------------- */
/*e \dref_PublisherQos
 */
struct DDS_PublisherQos
{
    /*e \dref_PublisherQos_entity_factory
     */
    struct DDS_EntityFactoryQosPolicy entity_factory;

    /*i \dref_PublisherQos_management
     */
    struct RTI_ManagementQosPolicy management;
};

/*ce \dref_PublisherQos_initialize
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_PublisherQos_initialize(struct DDS_PublisherQos *self);

/*ce \dref_PublisherQos_copy
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_PublisherQos_copy(struct DDS_PublisherQos *self,
                      const struct DDS_PublisherQos *source);

/*ce \dref_PublisherQos_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_PublisherQos_finalize(struct DDS_PublisherQos *self);

/*ce \dref_PublisherQos_INITIALIZER
 */
#define DDS_PublisherQos_INITIALIZER   {    \
    DDS_ENTITY_FACTORY_QOS_POLICY_DEFAULT,  \
    RTI_MANAGEMENT_QOS_POLICY_DEFAULT       \
}

/* ================================================================= */
/*                       Listeners                                   */
/* ================================================================= */

/*ci \dref_DataWriterListener_OfferedDeadlineMissedCallback
 */
typedef void
(*DDS_DataWriterListener_OfferedDeadlineMissedCallback)(
        void *listener_data,
        DDS_DataWriter* writer,
        const struct DDS_OfferedDeadlineMissedStatus* status);

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriterListener_LivelinessLostCallback
 */
typedef void
(*DDS_DataWriterListener_LivelinessLostCallback)(
        void *listener_data,
        DDS_DataWriter *writer,
        const struct DDS_LivelinessLostStatus *status);

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriterListener_OfferedIncompatibleQosCallback
 */
typedef void
(*DDS_DataWriterListener_OfferedIncompatibleQosCallback)(
        void *listener_data,
        DDS_DataWriter* writer,
        const struct DDS_OfferedIncompatibleQosStatus *status);

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriterListener_PublicationMatchedCallback
 */
typedef void (*DDS_DataWriterListener_PublicationMatchedCallback)(
        void *listener_data,
        DDS_DataWriter *writer,
        const struct DDS_PublicationMatchedStatus *status);

/* ------------------------------------------------------------------------ */
/*i \dref_DataWriterListener_ReliableWriterCacheChangedCallback
 */
typedef void
(*DDS_DataWriterListener_ReliableWriterCacheChangedCallback)(
        void *listener_data,
        DDS_DataWriter *writer,
        const struct DDS_ReliableWriterCacheChangedStatus *status);

/* ------------------------------------------------------------------------ */
/*i \dref_DataWriterListener_ReliableReaderActivityChangedCallback
 */
typedef void
(*DDS_DataWriterListener_ReliableReaderActivityChangedCallback)(
        void *listener_data,
        DDS_DataWriter * writer,
         const struct DDS_ReliableReaderActivityChangedStatus * status);

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriterListener
 */
struct DDS_DataWriterListener
{
    /*ce \dref_DataWriterListener_as_listener
     */
    struct DDS_Listener as_listener;

    /*ci \dref_DataWriterListener_on_offered_deadline_missed
     */
    DDS_DataWriterListener_OfferedDeadlineMissedCallback
    on_offered_deadline_missed;

    /*ce \dref_DataWriterListener_on_offered_incompatible_qos
     */
    DDS_DataWriterListener_OfferedIncompatibleQosCallback
    on_offered_incompatible_qos;

    /*ce \dref_DataWriterListener_on_liveliness_lost
     */
    DDS_DataWriterListener_LivelinessLostCallback on_liveliness_lost;

    /*ce \dref_DataWriterListener_on_publication_matched
     */
    DDS_DataWriterListener_PublicationMatchedCallback
    on_publication_matched;

    /*i \dref_DataWriterListener_on_reliable_writer_cache_changed
     */
    DDS_DataWriterListener_ReliableWriterCacheChangedCallback
    on_reliable_writer_cache_changed;

    /*i \dref_DataWriterListener_on_reliable_reader_activity_changed
     */
    DDS_DataWriterListener_ReliableReaderActivityChangedCallback
    on_reliable_reader_activity_changed;
};

/*ce \dref_DataWriterListener_INITIALIZER
 */
#define DDS_DataWriterListener_INITIALIZER  { \
  DDS_Listener_INITIALIZER, \
  (DDS_DataWriterListener_OfferedDeadlineMissedCallback)NULL, \
  (DDS_DataWriterListener_OfferedIncompatibleQosCallback)NULL, \
  (DDS_DataWriterListener_LivelinessLostCallback)NULL, \
  (DDS_DataWriterListener_PublicationMatchedCallback)NULL,\
  NULL,\
  NULL\
}

/* ----------------------------------------------------------------- */
/*ci \dref_PublisherListener
 */
struct DDS_PublisherListener
{
    /*ce \dref_PublisherListener_as_datawriterlistener
     */
    struct DDS_DataWriterListener as_datawriterlistener;
};

/*ci \dref_PublisherListener_INITIALIZER
 */
#define DDS_PublisherListener_INITIALIZER   { \
    DDS_DataWriterListener_INITIALIZER }

/* ================================================================= */
/*                          Publisher                                */
/* ================================================================= */

/* ----------------------------------------------------------------- */

/*e \dref_DATAWRITER_QOS_DEFAULT
 */
extern DDSCDllVariable const struct DDS_DataWriterQos
        DDS_DATAWRITER_QOS_DEFAULT;

/* ----------------------------------------------------------------- */
#define DDS_Publisher_as_entity(publisherPtr) \
            ((DDS_Entity*) publisherPtr)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*ce \dref_Publisher_as_entity
 */
    DDS_Entity *DDS_Publisher_as_entity(DDS_Publisher * publisher);
#endif                          /*DOXYGEN_DOCUMENTATION_ONLY */

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_get_default_datawriter_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_get_default_datawriter_qos(
        DDS_Publisher *self,
        struct DDS_DataWriterQos *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_set_default_datawriter_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_set_default_datawriter_qos(
        DDS_Publisher * self,
        const struct DDS_DataWriterQos
                                                 *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_create_datawriter
 */
DDSCDllExport DDS_DataWriter*
DDS_Publisher_create_datawriter(
        DDS_Publisher *self,
        DDS_Topic *topic,
        const struct DDS_DataWriterQos *qos,
        const struct DDS_DataWriterListener *listener,
        DDS_StatusMask mask);

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_delete_datawriter
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_enable(DDS_Entity *self);

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_delete_datawriter
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_delete_datawriter(
        DDS_Publisher *self,
        DDS_DataWriter *a_datawriter);

/* ----------------------------------------------------------------- */
/*ci \dref_Publisher_lookup_datawriter
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_DataWriter*
DDS_Publisher_lookup_datawriter(
         DDS_Publisher *self,
         const char *topic_name);

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_get_participant
 */
DDSCDllExport DDS_DomainParticipant*
DDS_Publisher_get_participant(DDS_Publisher * self);

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_delete_contained_entities
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_delete_contained_entities(DDS_Publisher * self);

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_set_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_set_qos(
        DDS_Publisher *self,
        const struct DDS_PublisherQos *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_Publisher_get_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_get_qos(
        DDS_Publisher *self,
        struct DDS_PublisherQos *qos);

/* ----------------------------------------------------------------- */
/*ci \dref_Publisher_set_listener
 */
/* called internally by Pub_new() */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_set_listener(
        DDS_Publisher * self,
        const struct DDS_PublisherListener *l,
        DDS_StatusMask mask);


/*ci \dref_Publisher_get_listener
 */
DDSCDllExport struct DDS_PublisherListener
DDS_Publisher_get_listener(DDS_Publisher * self);

/*ci \dref_Publisher_get_builtin_topic_key
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Publisher_get_builtin_topic_key(
        const DDS_Publisher *self,
        DDS_BuiltinTopicKey_t *key);

/* ----------------------------------------------------------------- */
/*i \dref_Publisher_lookup_datawriter_by_builtin_topic_key
 */
DDSCDllExport DDS_DataWriter*
DDS_Publisher_lookup_datawriter_by_builtin_topic_key(
        DDS_Publisher *self,
        DDS_BuiltinTopicKey_t* key);

/* ================================================================= */
/*                      Data Writer                                  */
/* ================================================================= */

/* ----------------------------------------------------------------- */
#define DDS_DataWriter_as_entity(dataWriterPtr) \
            ((DDS_Entity*) dataWriterPtr)

#ifdef DOXYGEN_DOCUMENTATION_ONLY
/*ce \dref_DataWriter_as_entity
 */
    DDS_Entity *DDS_DataWriter_as_entity(DDS_DataWriter * dataWriter);
#endif                          /*DOXYGEN_DOCUMENTATION_ONLY */

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriter_enable
*/
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_enable(DDS_Entity *self);

    /*ce \dref_DataWriter_assert_liveliness
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_assert_liveliness(DDS_DataWriter * self);

/* ----------------------------------------------------------------- */
/*ci \dref_DataWriter_get_matched_subscriptions
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_matched_subscriptions(
        DDS_DataWriter *self,
        struct DDS_InstanceHandleSeq *subscription_handles);

struct DDS_SubscriptionBuiltinTopicData;

/* ----------------------------------------------------------------- */
/*ci \dref_DataWriter_get_matched_subscription_data
 */
/* #ifdef INCLUDE_API_LOOKUP */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_matched_subscription_data(
        DDS_DataWriter *self,
        struct DDS_SubscriptionBuiltinTopicData *subscription_data,
        const DDS_InstanceHandle_t *subscription_handle);

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriter_get_topic
 */
DDSCDllExport DDS_Topic*
DDS_DataWriter_get_topic(DDS_DataWriter *self);

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriter_get_publisher
 */
DDSCDllExport DDS_Publisher*
DDS_DataWriter_get_publisher(DDS_DataWriter * self);

/* ----------------------------------------------------------------- */
/*ci \dref_DataWriter_get_liveliness_lost_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_liveliness_lost_status(
        DDS_DataWriter *self,
        struct DDS_LivelinessLostStatus *status);

/* ----------------------------------------------------------------- */
/*ci \dref_DataWriter_get_offered_deadline_missed_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_offered_deadline_missed_status(
        DDS_DataWriter *self,
        struct DDS_OfferedDeadlineMissedStatus *status);

/* ----------------------------------------------------------------- */
/*ci \dref_DataWriter_get_offered_incompatible_qos_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_offered_incompatible_qos_status(
        DDS_DataWriter *self,
        struct DDS_OfferedIncompatibleQosStatus *status);

/* ----------------------------------------------------------------- */
/*ci \dref_DataWriter_get_publication_matched_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_publication_matched_status(
        DDS_DataWriter *self,
        struct DDS_PublicationMatchedStatus *status);

/* ------------------------------------------------------------------------ */
/*i \dref_DataWriter_get_reliable_writer_cache_changed_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_reliable_writer_cache_changed_status(
        DDS_DataWriter *self,
        struct DDS_ReliableWriterCacheChangedStatus *status);

/* ------------------------------------------------------------------------ */
/*i \dref_DataWriter_get_reliable_reader_activity_changed_status
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_reliable_reader_activity_changed_status(
        DDS_DataWriter *self,
        struct DDS_ReliableReaderActivityChangedStatus *status);

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriter_set_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_set_qos(
        DDS_DataWriter *self,
        const struct DDS_DataWriterQos *qos);


/* ----------------------------------------------------------------- */
/*ce \dref_DataWriter_get_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_qos(DDS_DataWriter *self,
        struct DDS_DataWriterQos *qos);

/* ----------------------------------------------------------------- */
/*ce \dref_DataWriter_set_listener
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_set_listener(
        DDS_DataWriter * self,
        const struct DDS_DataWriterListener *l,
        DDS_StatusMask mask);


/*ci \dref_DataWriter_get_listener
 */
DDSCDllExport struct DDS_DataWriterListener
DDS_DataWriter_get_listener(DDS_DataWriter * self);

/*ci \dref_DataWriter_get_builtin_topic_key
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_get_builtin_topic_key(
        const DDS_DataWriter * self,
        DDS_BuiltinTopicKey_t * key);

/********************* Untyped Writer API ****************************/
/*e \dref_DataWriter_register_instance
 */
/* #ifdef INCLUDE_API_INSTANCE */
DDSCDllExport DDS_InstanceHandle_t
DDS_DataWriter_register_instance(
        DDS_DataWriter * self,
        const void *instance_data);

/*e \dref_DataWriter_register_instance_w_timestamp
 */
/* #ifdef INCLUDE_API_INSTANCE */
DDSCDllExport DDS_InstanceHandle_t
DDS_DataWriter_register_instance_w_timestamp(
        DDS_DataWriter * self,
        const void *instance_data,
        const struct DDS_Time_t *source_timestamp);

/*ce \dref_DataWriter_unregister_instance
 */
/* #ifdef INCLUDE_API_INSTANCE */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_unregister_instance(
        DDS_DataWriter *self,
        const void *instance_data,
        const DDS_InstanceHandle_t *handle);

/*ce \dref_DataWriter_unregister_instance_w_timestamp
 */
/* #ifdef INCLUDE_API_INSTANCE */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_unregister_instance_w_timestamp(
        DDS_DataWriter *self,
        const void *instance_data,
        const DDS_InstanceHandle_t *handle,
        const struct DDS_Time_t *source_timestamp);

/*ce \dref_DataWriter_dispose
 */
/* #ifdef INCLUDE_API_INSTANCE */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_dispose(
        DDS_DataWriter *self,
        const void *instance_data,
        const DDS_InstanceHandle_t *handle);

/*ce \dref_DataWriter_dispose_w_timestamp
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_dispose_w_timestamp(
        DDS_DataWriter *self,
        const void *instance_data,
        const DDS_InstanceHandle_t *handle,
        const struct DDS_Time_t *source_timestamp);

/*ce \dref_DataWriter_write
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_write(
        DDS_DataWriter *self,
        const void *instance_data,
        const DDS_InstanceHandle_t * handle);

/*ce \dref_DataWriter_write_w_timestamp
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_write_w_timestamp(
        DDS_DataWriter *self,
        const void *instance_data,
        const DDS_InstanceHandle_t *handle,
        const struct DDS_Time_t *source_timestamp);

/*ce \dref_DataWriter_write_w_params
 */
DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_write_w_params(
        DDS_DataWriter *self,
        const void *instance_data,
        struct DDS_WriteParams_t *params);

DDSCDllExport DDS_Long
DDS_DataWriter_get_liveliness_count(DDS_DataWriter * self);

/* ----------------------------------------------------------------- */

/****************** Additional Internal APIs **************************/

DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_advance_sn(DDS_DataWriter * self);

DDSCDllExport DDS_ReturnCode_t
DDS_DataWriter_add_anonymous_locator(DDS_DataWriter * self,
                                     struct DDS_Locator_t *locator);

DDSCDllExport DDS_Boolean
DDS_DataWriter_add_anonymous_route(DDS_DataWriter *datawriter,
                                   struct NETIO_Address *dst_reader,
                                   struct NETIO_Address *via_address);

/* ----------------------------------------------------------------- */

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#ifndef dds_c_w_history_plugin_h
#include "dds_c/dds_c_wh_plugin.h"
#endif


#endif /* dds_c_publication_h */
