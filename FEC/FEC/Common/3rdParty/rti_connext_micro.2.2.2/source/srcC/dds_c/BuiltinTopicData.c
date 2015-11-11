/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
/*i @file
 * @ingroup DDSDomainModule
 */
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif

#include "infrastructure.peer.h"

/*** SOURCE_BEGIN ***/

/******************************************************************************
 *                     DDS_ParticipantBuiltinTopicData                        *
 ******************************************************************************/
#ifdef T
#undef T
#endif

#define T_map_cast RTI_BOOL(*)(T*)

#define T struct DDS_ParticipantBuiltinTopicData
#define T_initialize (T_map_cast)DDS_ParticipantBuiltinTopicData_initialize
#define T_finalize (T_map_cast)DDS_ParticipantBuiltinTopicData_finalize
#define T_copy DDS_ParticipantBuiltinTopicData_copy
#undef T_compare /*DDS_ParticipantBuiltinTopicData_compare*/
#define TSeq DDS_ParticipantBuiltinTopicDataSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq
#undef T_initialize
#undef T_finalize
#undef T_copy
#undef T_compare

#define T DDS_ParticipantBuiltinTopicData
#define T_copy DDS_ParticipantBuiltinTopicData_copy
#define T_finalize DDS_ParticipantBuiltinTopicData_finalize
#define T_initialize DDS_ParticipantBuiltinTopicData_initialize
#define T_is_equal DDS_ParticipantBuiltinTopicData_is_equal
#define T_DEFAULT DDS_ParticipantBuiltinTopicData_INITIALIZER
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT

DDS_Boolean
DDS_ParticipantBuiltinTopicData_is_equal(
                        const struct DDS_ParticipantBuiltinTopicData *left,
                        const struct DDS_ParticipantBuiltinTopicData *right)
{
    if ((left == NULL) || (right == NULL))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_BuiltinTopicKey_equals(&left->key, &right->key))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_EntityNameQosPolicy_is_equal(&left->participant_name,
                                          &right->participant_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (left->dds_builtin_endpoints != right->dds_builtin_endpoints)
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ProtocolVersion_is_equal(&left->rtps_protocol_version,
                                      &left->rtps_protocol_version))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_VendorId_is_equal(&left->rtps_vendor_id, &right->rtps_vendor_id))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_is_equal(&left->default_unicast_locators,
                                 &right->default_unicast_locators))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_is_equal(&left->metatraffic_unicast_locators,
                                 &right->metatraffic_unicast_locators))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_is_equal(&left->metatraffic_multicast_locators,
                                 &right->metatraffic_multicast_locators))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_Duration_compare(&left->liveliness_lease_duration,
                              &right->liveliness_lease_duration))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ProductVersion_is_equal(&left->product_version,
                                     &left->product_version))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_ParticipantBuiltinTopicData_initialize(
                                struct DDS_ParticipantBuiltinTopicData *self)
{
    struct DDS_ParticipantBuiltinTopicData init_val =
                                   DDS_ParticipantBuiltinTopicData_INITIALIZER;

    *self = init_val;

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_ParticipantBuiltinTopicData_finalize(
                                struct DDS_ParticipantBuiltinTopicData *self)
{
    if (self == NULL)
    {
        return(DDS_BOOLEAN_FALSE);
    }

    if (!DDS_LocatorSeq_finalize(&self->default_unicast_locators))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    if (!DDS_LocatorSeq_finalize(&self->metatraffic_unicast_locators))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    if (!DDS_LocatorSeq_finalize(&self->metatraffic_multicast_locators))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    if (!DDS_LocatorSeq_finalize(&self->default_multicast_locators))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_ParticipantBuiltinTopicData_copy(
                            struct DDS_ParticipantBuiltinTopicData *out,
                            const struct DDS_ParticipantBuiltinTopicData *in)
{
    if ((out == NULL) || (in == NULL))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    out->key = in->key;
    out->participant_name = in->participant_name;
    out->dds_builtin_endpoints = in->dds_builtin_endpoints;
    out->rtps_protocol_version = in->rtps_protocol_version;
    out->rtps_vendor_id = in->rtps_vendor_id;
    if (!DDS_LocatorSeq_copy(&out->default_unicast_locators,
                             &in->default_unicast_locators))
    {
        return(DDS_BOOLEAN_FALSE);
    }
    if (!DDS_LocatorSeq_copy(&out->metatraffic_unicast_locators,
                             &in->metatraffic_unicast_locators))
    {
        return(DDS_BOOLEAN_FALSE);
    }
    if (!DDS_LocatorSeq_copy(&out->metatraffic_multicast_locators,
                             &in->metatraffic_multicast_locators))
    {
        return(DDS_BOOLEAN_FALSE);
    }
    out->liveliness_lease_duration = in->liveliness_lease_duration;
    out->product_version = in->product_version;

    return DDS_BOOLEAN_TRUE;
}

void
DDS_ParticipantBuiltinTopicData_from_qos(struct DDS_DomainParticipantQos
                                         *participant_qos,
                                         struct DDS_ParticipantBuiltinTopicData
                                         *builtin_topic_data)
{
    builtin_topic_data->key.value[0] = participant_qos->protocol.rtps_host_id;
    builtin_topic_data->key.value[1] = participant_qos->protocol.rtps_app_id;
    builtin_topic_data->key.value[2] =
        participant_qos->protocol.rtps_instance_id;
    builtin_topic_data->key.value[3] = participant_qos->protocol.participant_id;

    strcpy(builtin_topic_data->participant_name.name,
           participant_qos->participant_name.name);
}

void
DDS_ParticipantBuiltinTopicData_to_qos(
                    struct DDS_ParticipantBuiltinTopicData *builtin_topic_data,
                    struct DDS_DomainParticipantQos *participant_qos)
{
    participant_qos->protocol.rtps_host_id = builtin_topic_data->key.value[0];
    participant_qos->protocol.rtps_app_id = builtin_topic_data->key.value[1];
    participant_qos->protocol.rtps_instance_id =
        builtin_topic_data->key.value[2];
    participant_qos->protocol.participant_id = builtin_topic_data->key.value[3];

    strcpy(participant_qos->participant_name.name,
           builtin_topic_data->participant_name.name);

}

/******************************************************************************
 *                     DDS_PublicationBuiltinTopicData                        *
 ******************************************************************************/
#ifdef T
#undef T
#endif

#define T struct DDS_PublicationBuiltinTopicData
#define T_initialize (T_map_cast)DDS_PublicationBuiltinTopicData_initialize
#define T_finalize (T_map_cast)DDS_PublicationBuiltinTopicData_finalize
#define T_copy DDS_PublicationBuiltinTopicData_copy
#undef T_compare /*DDS_PublicationBuiltinTopicData_compare*/
#define TSeq DDS_PublicationBuiltinTopicDataSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq
#undef T_initialize
#undef T_finalize
#undef T_copy
#undef T_compare

#define T DDS_PublicationBuiltinTopicData
#define T_copy DDS_PublicationBuiltinTopicData_copy
#define T_initialize DDS_PublicationBuiltinTopicData_initialize
#define T_finalize DDS_PublicationBuiltinTopicData_finalize
#define T_is_equal DDS_PublicationBuiltinTopicData_is_equal
#define T_DEFAULT DDS_PublicationBuiltinTopicData_INITIALIZER
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T_copy
#undef T_finalize
#undef T_is_equal
#undef T_initialize
#undef T_DEFAULT
#undef T

DDS_Boolean
DDS_PublicationBuiltinTopicData_is_equal(
                        const struct DDS_PublicationBuiltinTopicData *left,
                        const struct DDS_PublicationBuiltinTopicData *right)
{
    if ((left == NULL) || (right == NULL))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_BuiltinTopicKey_equals(&left->key, &right->key))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_BuiltinTopicKey_equals(&left->participant_key,
                                    &right->participant_key))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_String_cmp(left->topic_name, right->topic_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_String_cmp(left->type_name, right->type_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DeadlineQosPolicy_is_equal(&left->deadline, &right->deadline))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ReliabilityQosPolicy_is_equal(&left->reliability,
                                           &right->reliability))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipQosPolicy_is_equal(&left->ownership, &right->ownership))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DurabilityQosPolicy_is_equal(&left->durability, &right->durability))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipStrengthQosPolicy_is_equal(&left->ownership_strength,
                                                 &right->ownership_strength))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LivelinessQosPolicy_is_equal(&left->liveliness,
                                          &right->liveliness))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_is_equal(&left->unicast_locator,
                                 &right->unicast_locator))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;

}

DDS_Boolean
DDS_PublicationBuiltinTopicData_initialize(
                                struct DDS_PublicationBuiltinTopicData *self)
{
    struct DDS_PublicationBuiltinTopicData init_val =
                        DDS_PublicationBuiltinTopicData_INITIALIZER;

    *self = init_val;

    self->topic_name = DDS_String_alloc(MIG_RTPS_PATHNAME_LEN_MAX + 1);
    self->type_name = DDS_String_alloc(MIG_RTPS_PATHNAME_LEN_MAX + 1);

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_PublicationBuiltinTopicData_finalize(
                                struct DDS_PublicationBuiltinTopicData *self)
{
    if (self == NULL)
    {
        return (DDS_BOOLEAN_FALSE);
    }

    if (self->topic_name)
    {
        DDS_String_free(self->topic_name);
        self->topic_name = NULL;
    }

    if (self->type_name)
    {
        DDS_String_free(self->type_name);
        self->type_name = NULL;
    }

    if (!DDS_LocatorSeq_finalize(&self->unicast_locator))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_PublicationBuiltinTopicData_copy(
                              struct DDS_PublicationBuiltinTopicData *out,
                              const struct DDS_PublicationBuiltinTopicData *in)
{
    if ((out == NULL) || (in == NULL))
    {
        return (DDS_BOOLEAN_FALSE);
    }

    out->key = in->key;
    out->participant_key = in->participant_key;
    DDS_String_replace(&out->topic_name, in->topic_name);
    DDS_String_replace(&out->type_name, in->type_name);
    out->deadline = in->deadline;
    out->reliability = in->reliability;
    out->liveliness = in->liveliness;
    out->ownership = in->ownership;
    out->ownership_strength = in->ownership_strength;
    out->durability = in->durability;

    if (!DDS_LocatorSeq_copy(&out->unicast_locator, &in->unicast_locator))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    return DDS_BOOLEAN_TRUE;
}

void
DDS_PublicationBuiltinTopicData_from_qos(
                                struct DDS_PublicationBuiltinTopicData *data,
                                const struct DDS_PublisherQos *publisher_qos,
                                const struct DDS_DataWriterQos *datawriter_qos,
                                const char *const topic_name,
                                const char *const type_name,
                                DDS_BuiltinTopicKey_t p_key,
                                DDS_BuiltinTopicKey_t e_key)
{
    data->deadline = datawriter_qos->deadline;
    data->key = e_key;
    data->liveliness = datawriter_qos->liveliness;
    data->ownership = datawriter_qos->ownership;
    data->ownership_strength = datawriter_qos->ownership_strength;
    data->participant_key = p_key;
    data->reliability = datawriter_qos->reliability;
    DDS_String_replace(&data->topic_name, topic_name);
    DDS_String_replace(&data->type_name, type_name);
    data->durability = datawriter_qos->durability;
}

void
DDS_PublicationBuiltinTopicData_to_qos(
                            struct DDS_PublisherQos *publisher_qos,
                            struct DDS_DataWriterQos *datawriter_qos,
                            const struct DDS_PublicationBuiltinTopicData *data)
{
    /* TODO: Fill in */
}

/******************************************************************************
 *                     DDS_SubscriptionBuiltinTopicData                       *
 ******************************************************************************/
#define T struct DDS_SubscriptionBuiltinTopicData
#define T_initialize (T_map_cast)DDS_SubscriptionBuiltinTopicData_initialize
#define T_finalize (T_map_cast)DDS_SubscriptionBuiltinTopicData_finalize
#define T_copy DDS_SubscriptionBuiltinTopicData_copy
#undef T_compare /*DDS_SubscriptionBuiltinTopicData_compare*/
#define TSeq DDS_SubscriptionBuiltinTopicDataSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq
#undef T_initialize
#undef T_finalize
#undef T_copy
#undef T_compare

#define T DDS_SubscriptionBuiltinTopicData
#define T_copy DDS_SubscriptionBuiltinTopicData_copy
#define T_finalize DDS_SubscriptionBuiltinTopicData_finalize
#define T_initialize DDS_SubscriptionBuiltinTopicData_initialize
#define T_is_equal DDS_SubscriptionBuiltinTopicData_is_equal
#define T_DEFAULT DDS_SubscriptionBuiltinTopicData_INITIALIZER
#include "dds_c/dds_c_tpolicy_gen.h"
#undef T_copy
#undef T_finalize
#undef T_initialize
#undef T_is_equal
#undef T_DEFAULT

DDS_Boolean
DDS_SubscriptionBuiltinTopicData_is_equal(
                        const struct DDS_SubscriptionBuiltinTopicData *left,
                        const struct DDS_SubscriptionBuiltinTopicData *right)
{
    if ((left == NULL) || (right == NULL))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_BuiltinTopicKey_equals(&left->key, &right->key))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_BuiltinTopicKey_equals(&left->participant_key,
                                    &right->participant_key))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_String_cmp(left->topic_name, right->topic_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (DDS_String_cmp(left->type_name, right->type_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DeadlineQosPolicy_is_equal(&left->deadline, &right->deadline))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ReliabilityQosPolicy_is_equal(&left->reliability,
                                           &right->reliability))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipQosPolicy_is_equal(&left->ownership, &right->ownership))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LivelinessQosPolicy_is_equal(&left->liveliness,
                                          &right->liveliness))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DurabilityQosPolicy_is_equal(&left->durability, &right->durability))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_is_equal(&left->unicast_locator,
                                 &right->unicast_locator))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_is_equal(&left->multicast_locator,
                                 &right->multicast_locator))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;

}

DDS_Boolean
DDS_SubscriptionBuiltinTopicData_initialize(
                                struct DDS_SubscriptionBuiltinTopicData *self)
{
    struct DDS_SubscriptionBuiltinTopicData init_val =
            DDS_SubscriptionBuiltinTopicData_INITIALIZER;

    *self = init_val;

    self->topic_name = DDS_String_alloc(MIG_RTPS_PATHNAME_LEN_MAX + 1);
    self->type_name = DDS_String_alloc(MIG_RTPS_PATHNAME_LEN_MAX + 1);

    if (!DDS_LocatorSeq_set_maximum
        (&self->unicast_locator, MIG_RTPS_PID_USERDATA_IPADDRESS_COUNT_MAX))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_set_length(&self->unicast_locator, 0))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_set_maximum(&self->multicast_locator, 1))
    {
        return DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LocatorSeq_set_length(&self->multicast_locator, 0))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_SubscriptionBuiltinTopicData_finalize(
                                struct DDS_SubscriptionBuiltinTopicData *self)
{
    if (self == NULL)
    {
        return (DDS_BOOLEAN_FALSE);
    }

    if (self->topic_name)
    {
        DDS_String_free(self->topic_name);
        self->topic_name = NULL;
    }

    if (self->type_name)
    {
        DDS_String_free(self->type_name);
        self->type_name = NULL;
    }

    if (!DDS_LocatorSeq_finalize(&self->unicast_locator))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    if (!DDS_LocatorSeq_finalize(&self->multicast_locator))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_SubscriptionBuiltinTopicData_copy(
                            struct DDS_SubscriptionBuiltinTopicData *out,
                            const struct DDS_SubscriptionBuiltinTopicData * in)
{
    if ((out == NULL) || (in == NULL))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    out->key = in->key;
    out->participant_key = in->participant_key;
    DDS_String_replace(&out->topic_name, in->topic_name);
    DDS_String_replace(&out->type_name, in->type_name);
    out->deadline = in->deadline;
    out->reliability = in->reliability;
    out->liveliness = in->liveliness;
    out->ownership = in->ownership;
    out->durability = in->durability;

    if (!DDS_LocatorSeq_copy(&out->unicast_locator, &in->unicast_locator))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    if (!DDS_LocatorSeq_copy(&out->multicast_locator, &in->multicast_locator))
    {
        return(DDS_BOOLEAN_FALSE);
    }

    return DDS_BOOLEAN_TRUE;
}

void
DDS_SubscriptionBuiltinTopicData_from_qos(
                            struct DDS_SubscriptionBuiltinTopicData *data,
                            const struct DDS_PublisherQos *publisher_qos,
                            const struct DDS_DataWriterQos *datawriter_qos)
{
    /* TODO: Fill in */
}

void
DDS_SubscriptionBuiltinTopicData_to_qos(
                        struct DDS_SubscriberQos *subscriber_qos,
                        struct DDS_DataReaderQos *datareader_qos,
                        const struct DDS_SubscriptionBuiltinTopicData *data)
{
    /* TODO: Fill in */
}

#undef fail_with
