/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_route_h
#include "netio/netio_route.h"
#endif
#include "netio/netio_loopback.h"
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "Entity.pkg.h"
#include "QosPolicy.pkg.h"
#include "TopicDescription.pkg.h"
#include "Topic.pkg.h"
#include "Type.pkg.h"
#include "DataReaderImpl.pkg.h"
#include "DataReader.pkg.h"
#include "DataReaderDiscovery.pkg.h"
#include "SubscriberQos.pkg.h"
#include "SubscriberImpl.pkg.h"
#include "DataWriterImpl.pkg.h"
#include "DataWriterDiscovery.pkg.h"
#include "PublisherQos.pkg.h"
#include "PublisherImpl.pkg.h"
#include "DomainParticipantQos.pkg.h"
#include "DomainParticipant.pkg.h"

/*** SOURCE_BEGIN ***/

/*******************************************************************************
 *                            Entity Listeners
 ******************************************************************************/
void
DomainParticipantEvent_on_after_publisher_modified(
                            DDS_Publisher *const publisher,
                            const struct DDS_PublisherQos *const qos,
                            const struct DDS_PublisherListener *const listener)
{
    /* NOT SUPPORTED */

    /*TODO:
     * - When a publisher is modified, discovery information for all
     *   local datawriters must be updated
     * - A rematch with all local and remote datawriters must be performed
     */
}

void
DomainParticipantEvent_on_after_publisher_enabled(
                            DDS_Publisher *const publisher,
                            const struct DDS_PublisherQos *const qos)
{
    /* NOT SUPPORTED */

    /* TODO:
     * - There is nothing to do when a publisher is enabled, there is
     *   a separate call-back for enabling a data-writer
     */
}

void
DomainParticipantEvent_on_after_subscriber_modified(
                        DDS_Subscriber *const subscriber,
                        const struct DDS_SubscriberQos *const qos,
                        const struct DDS_SubscriberListener *const listener)
{
    /* NOT SUPPORTED */

    /* TODO: Same as for publisher */
}

void
DomainParticipantEvent_on_after_subscriber_enabled(
                        DDS_Subscriber *const subscriber,
                        const struct DDS_SubscriberQos *const qos)
{
    /* NOT SUPPORTED */

    /* TODO: Same as for publisher */
}

void
DomainParticipantEvent_on_after_datareader_created(DDS_DataReader *const dw,
                        const struct DDS_DataReaderQos *const qos)
{
    /* NOT SUPPORTED */

    /* TODO: Nothing */
}

void
DomainParticipantEvent_on_after_datareader_enabled(
                        DDS_DataReader *const reader,
                        const struct DDS_DataReaderQos *const qos)
{
    DDS_DomainParticipant *participant;
    struct DDS_DataWriterImpl *local_writer;
    DB_Cursor_T dw_cursor = NULL;
    DB_ReturnCode_T dbrc;
    DDS_InstanceHandle_t ih;
    struct DDS_BuiltinTopicKey_t key;
    struct DDS_BuiltinTopicKey_t dr_key;

    participant = DDS_Subscriber_get_participant(
                                        DDS_DataReader_get_subscriber(reader));

    ih = DDS_Entity_get_instance_handle(DDS_DataWriter_as_entity(reader));
    DDS_BuiltinTopicKey_from_guid(&dr_key,&ih);

    DB_Database_lock(participant->database);

    (void)DB_Table_select_all(participant->local_writer_table,
                             DB_TABLE_DEFAULT_INDEX,&dw_cursor);
    do
    {
        dbrc = DB_Cursor_get_next(dw_cursor,(DB_Record_T*)&local_writer);
        if (dbrc == DB_RETCODE_OK)
        {
            ih = DDS_Entity_get_instance_handle(
                    DDS_DataWriter_as_entity(local_writer));
            DDS_BuiltinTopicKey_from_guid(&key,&ih);
            (void)DDS_DataReader_match_local_writer(reader,
                   &key,
                   DDS_TopicDescription_get_name(
                           DDS_Topic_as_topicdescription(local_writer->topic)),
                   DDS_TopicDescription_get_type_name(
                           DDS_Topic_as_topicdescription(local_writer->topic)),
                   &local_writer->qos);
            (void)DDS_DataWriter_match_local_reader(local_writer,
                   &dr_key,
                   DDS_TopicDescription_get_name(
                           DDS_Topic_as_topicdescription(reader->topic)),
                   DDS_TopicDescription_get_type_name(
                           DDS_Topic_as_topicdescription(reader->topic)),qos);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->local_writer_table,dw_cursor);

    if (participant->disc_plugin)
    {
        NDDS_Discovery_Plugin_on_after_local_datareader_enabled(
              participant->disc_plugin,participant,reader,qos);
    }

    DB_Database_unlock(participant->database);
}

void
DomainParticipantEvent_on_after_datareader_modified(DDS_DataReader *const dr,
                        const struct DDS_DataReaderQos *const qos,
                        const struct DDS_DataReaderListener *const listener)
{
    /* NOT SUPPORTED */

    /* TODO:
     * - Notify discovery plugins
     * - Perform matching with local and remote peers
     */
}

void
DomainParticipantEvent_on_before_datareader_deleted(DDS_DataReader *const reader)
{
    DDS_DomainParticipant *participant;
    DDS_InstanceHandle_t ih;
    struct DDS_DataWriterImpl *local_writer;
    DB_Cursor_T dw_cursor = NULL;
    DB_ReturnCode_T dbrc;
    struct DDS_BuiltinTopicKey_t dr_key;

    participant = DDS_Subscriber_get_participant(
                                        DDS_DataReader_get_subscriber(reader));

    ih = DDS_Entity_get_instance_handle(DDS_DataReader_as_entity(reader));
    DDS_BuiltinTopicKey_from_guid(&dr_key,&ih);

    DB_Database_lock(participant->database);

    (void)DB_Table_select_all(participant->local_writer_table,
                             DB_TABLE_DEFAULT_INDEX,&dw_cursor);
    do
    {
        dbrc = DB_Cursor_get_next(dw_cursor,(DB_Record_T*)&local_writer);
        if (dbrc == DB_RETCODE_OK)
        {
            (void)DDS_DataWriter_unmatch_local_reader(local_writer,
                   &dr_key,
                   &reader->qos);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->local_writer_table,dw_cursor);

    if (participant->disc_plugin)
    {
        NDDS_Discovery_Plugin_on_after_local_datareader_deleted(
              participant->disc_plugin,participant,&dr_key);
    }

    DB_Database_unlock(participant->database);
}

void
DomainParticipantEvent_on_after_datawriter_created(DDS_DataWriter *const dw,
                        const struct DDS_DataWriterQos *const qos)
{
    /* NOT SUPPORTED */

    /* TODO:
     * - Notify discovery plugins
     * - Perform matching with local and remote peers
     */
}

void
DomainParticipantEvent_on_after_datawriter_enabled(
                        DDS_DataWriter *const writer,
                        const struct DDS_DataWriterQos *const qos)
{
    DDS_DomainParticipant *participant;
    struct DDS_DataReaderImpl *local_reader;
    DB_Cursor_T dr_cursor = NULL;
    DB_ReturnCode_T dbrc;
    DDS_InstanceHandle_t ih;
    struct DDS_BuiltinTopicKey_t key;
    struct DDS_BuiltinTopicKey_t dw_key;
    RT_ComponentFactoryId_T id;
    struct NETIO_Address dw_address;
    NETIO_Interface_T *intf = NULL;

    participant = DDS_Publisher_get_participant(
                                        DDS_DataWriter_get_publisher(writer));
    ih = DDS_Entity_get_instance_handle(DDS_DataWriter_as_entity(writer));
    DDS_BuiltinTopicKey_from_guid(&dw_key,&ih);

    DB_Database_lock(participant->database);

    RT_ComponentFactoryId_clear(&id);
    RT_ComponentFactoryId_set_name(&id,"_intra");
    NETIO_Address_init(&dw_address,0);

    if (NETIO_BindResolver_lookup_by_address(participant->bind_resolver,
            NETIO_ROUTEKIND_USER,
            &id,
            &dw_address,
            &intf))
    {
        NETIO_Address_set_guid_from_key(&dw_address,0,&dw_key);
        (void)NETIO_BindResolver_add_route(participant->bind_resolver,
                NETIO_ROUTEKIND_USER,&id,&dw_address,intf,NULL,NULL,NULL);
    }

    (void)DB_Table_select_all(participant->local_reader_table,
                              DB_TABLE_DEFAULT_INDEX,&dr_cursor);
    do
    {
        dbrc = DB_Cursor_get_next(dr_cursor,(DB_Record_T*)&local_reader);
        if (dbrc == DB_RETCODE_OK)
        {
            ih = DDS_Entity_get_instance_handle(
                    DDS_DataReader_as_entity(local_reader));
            DDS_BuiltinTopicKey_from_guid(&key,&ih);
            (void)DDS_DataWriter_match_local_reader(writer,
                   &key,
                   DDS_TopicDescription_get_name(
                           DDS_Topic_as_topicdescription(local_reader->topic)),
                   DDS_TopicDescription_get_type_name(
                           DDS_Topic_as_topicdescription(local_reader->topic)),
                   &local_reader->qos);
            (void)DDS_DataReader_match_local_writer(local_reader,
                   &dw_key,
                   DDS_TopicDescription_get_name(
                           DDS_Topic_as_topicdescription(writer->topic)),
                   DDS_TopicDescription_get_type_name(
                           DDS_Topic_as_topicdescription(writer->topic)),qos);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->local_reader_table,dr_cursor);

    if (participant->disc_plugin)
    {
        NDDS_Discovery_Plugin_on_after_local_datawriter_enabled(
              participant->disc_plugin,participant,writer,qos);
    }

    DB_Database_unlock(participant->database);
}

void
DomainParticipantEvent_on_after_datawriter_modified(DDS_DataWriter *const dw,
                        const struct DDS_DataWriterQos *const qos,
                        const struct DDS_DataWriterListener *const listener)
{
    /* NOT SUPPORTED */

    /* TODO:
     * - Notify discovery plugins
     * - Perform matching with local and remote peers
     */
}

void
DomainParticipantEvent_on_before_datawriter_deleted(DDS_DataWriter *const writer)
{
    DDS_DomainParticipant *participant;
    DDS_InstanceHandle_t ih;
    struct DDS_BuiltinTopicKey_t dw_key;
    DB_Cursor_T dr_cursor = NULL;
    DB_ReturnCode_T dbrc;
    struct DDS_DataReaderImpl *local_reader;
    RT_ComponentFactoryId_T id;
    struct NETIO_Address dw_address;
    NETIO_Interface_T *intf = NULL;

    participant = DDS_Publisher_get_participant(
                                        DDS_DataWriter_get_publisher(writer));
    ih = DDS_Entity_get_instance_handle(DDS_DataWriter_as_entity(writer));
    DDS_BuiltinTopicKey_from_guid(&dw_key,&ih);

    DB_Database_lock(participant->database);

    RT_ComponentFactoryId_clear(&id);
    RT_ComponentFactoryId_set_name(&id,"_intra");
    NETIO_Address_init(&dw_address,0);

    if (NETIO_BindResolver_lookup_by_address(participant->bind_resolver,
            NETIO_ROUTEKIND_USER,
            &id,
            &dw_address,
            &intf))
    {
        NETIO_Address_set_guid_from_key(&dw_address,0,&dw_key);
        (void)NETIO_BindResolver_delete_route(participant->bind_resolver,
                NETIO_ROUTEKIND_USER,&id,&dw_address,NULL);
    }

    (void)DB_Table_select_all(participant->local_reader_table,
                             DB_TABLE_DEFAULT_INDEX,&dr_cursor);
    do
    {
        dbrc = DB_Cursor_get_next(dr_cursor,(DB_Record_T*)&local_reader);
        if (dbrc == DB_RETCODE_OK)
        {
            (void)DDS_DataReader_unmatch_local_writer(local_reader,
                   &dw_key,
                   &writer->qos);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->local_reader_table,dr_cursor);

    if (participant->disc_plugin)
    {
        NDDS_Discovery_Plugin_on_after_local_datawriter_deleted(
              participant->disc_plugin,participant,&dw_key);
    }

    DB_Database_unlock(participant->database);
}

/*******************************************************************************
 *                            LISTENER FORWARDERS APIs
 ******************************************************************************/
void
NDDS_DomainParticipant_on_offered_incompatible_qos(DDS_DomainParticipant * self,
                        DDS_DataWriter * writer,
                        const struct DDS_OfferedIncompatibleQosStatus *status)
{
    struct DDS_DomainParticipantImpl *dp =
                                    (struct DDS_DomainParticipantImpl *)self;

    if (dp->mask & DDS_OFFERED_INCOMPATIBLE_QOS_STATUS)
    {
        dp->listener.as_publisherlistener.as_datawriterlistener.
            on_offered_incompatible_qos(dp->listener.as_publisherlistener.
                                        as_datawriterlistener.as_listener.
                                        listener_data, writer, status);
    }
}

void
NDDS_DomainParticipant_on_publication_matched(DDS_DomainParticipant * self,
                        DDS_DataWriter * writer,
                        const struct DDS_PublicationMatchedStatus *status)
{
    struct DDS_DomainParticipantImpl *dp =
        (struct DDS_DomainParticipantImpl *)self;

    if (dp->mask & DDS_PUBLICATION_MATCHED_STATUS)
    {
        dp->listener.as_publisherlistener.as_datawriterlistener.
            on_publication_matched(dp->listener.as_publisherlistener.
                                   as_datawriterlistener.as_listener.
                                   listener_data, writer, status);
    }
}

/* DataReader Listener */
void
NDDS_DomainParticipant_on_requested_incompatible_qos(
                        DDS_DomainParticipant *self,
                        DDS_DataReader * reader,
                        const struct DDS_RequestedIncompatibleQosStatus *status)
{
    struct DDS_DomainParticipantImpl *dp =
        (struct DDS_DomainParticipantImpl *)self;

    if (dp->mask & DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS)
    {
        dp->listener.as_subscriberlistener.as_datareaderlistener.
            on_requested_incompatible_qos(self->listener.as_subscriberlistener.
                                          as_datareaderlistener.as_listener.
                                          listener_data, reader, status);
    }
}

void
NDDS_DomainParticipant_on_subscription_matched(DDS_DomainParticipant *self,
                        DDS_DataReader *reader,
                        const struct DDS_SubscriptionMatchedStatus *status)
{
    struct DDS_DomainParticipantImpl *dp =
                                    (struct DDS_DomainParticipantImpl *)self;

    if (dp->mask & DDS_SUBSCRIPTION_MATCHED_STATUS)
    {
        dp->listener.as_subscriberlistener.as_datareaderlistener.
            on_subscription_matched(self->listener.as_subscriberlistener.
                                    as_datareaderlistener.as_listener.
                                    listener_data, reader, status);
    }
}

void
NDDS_DomainParticipant_on_data_available(DDS_DomainParticipant *self,
                                         DDS_Subscriber *subscriber,
                                         DDS_DataReader *reader)
{
    struct DDS_DomainParticipantImpl *dp =
                                    (struct DDS_DomainParticipantImpl *)self;


    if (dp->mask & DDS_DATA_AVAILABLE_STATUS)
    {
        dp->listener.as_subscriberlistener.as_datareaderlistener.
            on_data_available(dp->listener.as_subscriberlistener.
                              as_datareaderlistener.as_listener.listener_data,
                              reader);
    }
}

/*******************************************************************************
 *                            OPTIONAL APIs
 ******************************************************************************/
#ifdef ENABLE_STATUS_LISTENER
void
NDDS_DomainParticipant_on_requested_deadline_missed(DDS_DomainParticipant *dp,
                        DDS_DataReader *reader,
                        const struct DDS_RequestedDeadlineMissedStatus *status)
{
    struct DDS_DomainParticipantImpl *self =
        (struct DDS_DomainParticipantImpl *)dp;

    if (self->mask & DDS_REQUESTED_DEADLINE_MISSED_STATUS)
    {
        self->listener.as_subscriberlistener.as_datareaderlistener.
            on_requested_deadline_missed(self->listener.as_subscriberlistener.
                                         as_datareaderlistener.as_listener.
                                         listener_data, reader, status);
    }
}

void
NDDS_DomainParticipant_on_liveliness_changed(DDS_DomainParticipant *dp,
                            DDS_DataReader * reader,
                            const struct DDS_LivelinessChangedStatus *status)
{
    struct DDS_DomainParticipantImpl *self =
                                       (struct DDS_DomainParticipantImpl *)dp;

    if (self->mask & DDS_LIVELINESS_LOST_STATUS)
    {
        self->listener.as_subscriberlistener.as_datareaderlistener.
            on_liveliness_changed(self->listener.as_subscriberlistener.
                                  as_datareaderlistener.as_listener.
                                  listener_data, reader, status);
    }
}

void
NDDS_DomainParticipant_on_sample_lost(DDS_DomainParticipant * dp,
                           DDS_DataReader *reader,
                           const struct DDS_SampleLostStatus *status)
{
    struct DDS_DomainParticipantImpl *self =
        (struct DDS_DomainParticipantImpl *)dp;

    if (self->mask & DDS_SAMPLE_LOST_STATUS)
    {
        self->listener.as_subscriberlistener.as_datareaderlistener.
            on_sample_lost(self->listener.as_subscriberlistener.
                           as_datareaderlistener.as_listener.listener_data,
                           reader, status);
    }
}

void
NDDS_DomainParticipant_on_sample_rejected(DDS_DomainParticipant *self,
                            DDS_DataReader *reader,
                            const struct DDS_SampleRejectedStatus *status)
{
    struct DDS_DomainParticipantImpl *dp =
        (struct DDS_DomainParticipantImpl *)self;

    if (dp->mask & DDS_SAMPLE_REJECTED_STATUS)
    {
        dp->listener.as_subscriberlistener.as_datareaderlistener.
            on_sample_rejected(self->listener.as_subscriberlistener.
                               as_datareaderlistener.as_listener.listener_data,
                               reader, status);
    }
}

/* Topic Listener */
void
NDDS_DomainParticipant_on_inconsistent_topic(DDS_DomainParticipant * self,
                              DDS_Topic * topic,
                              const struct DDS_InconsistentTopicStatus *status)
{
    struct DDS_DomainParticipantImpl *dp =
        (struct DDS_DomainParticipantImpl *)self;

    if (dp->mask & DDS_INCONSISTENT_TOPIC_STATUS)
    {
        dp->listener.as_topiclistener.on_inconsistent_topic(dp->listener.
                                                            as_topiclistener.
                                                            as_listener.
                                                            listener_data,
                                                            topic, status);
    }
}

/* DataWriter Listener */
void
NDDS_DomainParticipant_on_liveliness_lost(DDS_DomainParticipant * self,
                        DDS_DataWriter *writer,
                        const struct DDS_LivelinessLostStatus *status)
{
    struct DDS_DomainParticipantImpl *dp =
        (struct DDS_DomainParticipantImpl *)self;

    if (dp->mask & DDS_LIVELINESS_LOST_STATUS)
    {
        dp->listener.as_publisherlistener.as_datawriterlistener.
            on_liveliness_lost(dp->listener.as_publisherlistener.
                               as_datawriterlistener.as_listener.listener_data,
                               writer, status);
    }
}

void
NDDS_DomainParticipant_on_offered_deadline_missed(DDS_DomainParticipant * self,
                        DDS_DataWriter * writer,
                        const struct DDS_OfferedDeadlineMissedStatus *status)
{
    struct DDS_DomainParticipantImpl *dp =
        (struct DDS_DomainParticipantImpl *)self;

    if (dp->mask & DDS_OFFERED_DEADLINE_MISSED_STATUS)
    {
        dp->listener.as_publisherlistener.as_datawriterlistener.
            on_offered_deadline_missed(dp->listener.as_publisherlistener.
                                       as_datawriterlistener.as_listener.
                                       listener_data, writer, status);
    }
}
#endif

