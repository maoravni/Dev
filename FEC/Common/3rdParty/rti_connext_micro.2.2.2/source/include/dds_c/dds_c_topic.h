/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_topic_h
#define dds_c_topic_h



#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_sequence_h
#include "dds_c/dds_c_sequence.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* ================================================================= */
/*                       Basic Types                                 */
/* ================================================================= */

/*e \dref_InstanceId_t
 */
typedef struct DDS_BuiltinTopicKey_t DDS_InstanceId_t;


/* ================================================================= */
/*                       Status                                      */
/* ================================================================= */

/*i \dref_InconsistentTopicStatus
 */
struct DDS_InconsistentTopicStatus
{
    /*i \dref_InconsistentTopicStatus_total_count
     */
    DDS_Long total_count;

    /*i \dref_InconsistentTopicStatus_total_count_change
     */
    DDS_Long total_count_change;
};

/*ci \dref_InconsistentTopicStatus_INITIALIZER
 */
#define DDS_InconsistentTopicStatus_INITIALIZER { 0L, 0L }

/* ================================================================= */
/*                          QoS                                      */
/* ================================================================= */
struct DDS_TopicQos
{
    /*e \dref_TopicQos_deadline
     */
    struct DDS_DeadlineQosPolicy deadline;

    /*e \dref_TopicQos_liveliness
     */
    struct DDS_LivelinessQosPolicy liveliness;

    /*e \dref_TopicQos_history
     */
    struct DDS_HistoryQosPolicy history;

    /*e \dref_TopicQos_ownership
     */
    struct DDS_OwnershipQosPolicy ownership;

    /*i \dref_TopicQos_management
     */
    struct RTI_ManagementQosPolicy management;
};

/*ce \dref_TopicQos_initialize
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_TopicQos_initialize(struct DDS_TopicQos *self);

/*ce \dref_TopicQos_copy
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_TopicQos_copy(struct DDS_TopicQos *self,
        const struct DDS_TopicQos *source);

/*ce \dref_TopicQos_finalize
 */
DDSCDllExport DDS_ReturnCode_t
DDS_TopicQos_finalize(struct DDS_TopicQos *self);

/*ce \dref_TopicQos_INITIALIZER
 */
#define DDS_TopicQos_INITIALIZER   {   \
    DDS_DEADLINE_QOS_POLICY_DEFAULT,   \
    DDS_LIVELINESS_QOS_POLICY_DEFAULT, \
    DDS_HISTORY_QOS_POLICY_DEFAULT,    \
    DDS_OWNERSHIP_QOS_POLICY_DEFAULT,  \
    RTI_MANAGEMENT_QOS_POLICY_DEFAULT  \
}

/* ================================================================= */
/*                   DDS_TopicDescription                            */
/* ================================================================= */

/*ce \dref_TopicDescription
 */
typedef struct DDS_TopicDescriptionImpl DDS_TopicDescription;

/*ce \dref_TopicDescription_get_type_name
 */
DDSCDllExport const char*
DDS_TopicDescription_get_type_name(DDS_TopicDescription * self);

/*ce \dref_TopicDescription_get_name
 */
DDSCDllExport const char*
DDS_TopicDescription_get_name(DDS_TopicDescription * self);

/*ce \dref_TopicDescription_get_participant
 */
DDSCDllExport DDS_DomainParticipant*
DDS_TopicDescription_get_participant(DDS_TopicDescription * self);

/*ce \dref_Topic
 */
typedef struct DDS_TopicImpl DDS_Topic;

/* ================================================================= */
/*                       Listeners                                   */
/* ================================================================= */

/*ci \dref_TopicListener_InconsistentTopicCallback
 */
typedef void
(*DDS_TopicListener_InconsistentTopicCallback)(
        void *listener_data,
        DDS_Topic *topic,
        const struct DDS_InconsistentTopicStatus *status);

/*ci \dref_TopicListener
 */
struct DDS_TopicListener
{
    /*ce \dref_TopicListener_as_listener
     */
    struct DDS_Listener as_listener;

    /*ce \dref_TopicListener_on_inconsistent_topic
     */
    DDS_TopicListener_InconsistentTopicCallback on_inconsistent_topic;
};

/*ci \dref_TopicListener_INITIALIZER
 */
#define DDS_TopicListener_INITIALIZER \
{ DDS_Listener_INITIALIZER, (DDS_TopicListener_InconsistentTopicCallback)NULL}

/* ================================================================= */
/*                           Topic                                   */
/* ================================================================= */
/*ce \dref_Topic_as_entity
 */
DDSCDllExport DDS_Entity*
DDS_Topic_as_entity(DDS_Topic * self);

/*ce \dref_Topic_as_entity
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_enable(DDS_Entity *self);

/*ce \dref_Topic_as_topicdescription
 */
DDSCDllExport DDS_TopicDescription*
DDS_Topic_as_topicdescription(DDS_Topic * self);

/*ce \dref_Topic_narrow
 */
DDSCDllExport DDS_Topic*
DDS_Topic_narrow(DDS_TopicDescription *self);

/*ci \dref_Topic_get_inconsistent_topic_status
*/
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_get_inconsistent_topic_status(
        DDS_Topic *self,
        struct DDS_InconsistentTopicStatus *status);

/*ce \dref_Topic_set_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_set_qos(
        DDS_Topic *self,
        const struct DDS_TopicQos *qos);

/*ce \dref_Topic_get_qos
 */
/* #if INCLUDE_API_QOS */
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_get_qos(DDS_Topic * self,struct DDS_TopicQos *qos);


/*i \dref_Topic_get_qos_ref
 */
DDSCDllExport const struct DDS_TopicQos*
DDS_Topic_get_qos_ref(DDS_Topic * self);

/*ci \dref_Topic_set_listener
 */
/* #ifdef INCLUDE_API_LISTENER */
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_set_listener(
        DDS_Topic *self,
        const struct DDS_TopicListener *l,
        DDS_StatusMask mask);

/*ci \dref_Topic_get_listener
 */
/* #ifdef INCLUDE_API_LISTENER */
DDSCDllExport struct DDS_TopicListener
DDS_Topic_get_listener(DDS_Topic *self);


/*ci \dref_Topic_get_builtin_topic_key
 */
DDSCDllExport DDS_ReturnCode_t
DDS_Topic_get_builtin_topic_key(
        const DDS_Topic * self,
        DDS_BuiltinTopicKey_t *key);

/* ----------------------------------------------------------------- */

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_topic_h */
