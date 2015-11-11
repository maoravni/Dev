/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DomainParticipantEvent_pkg_h
#define DomainParticipantEvent_pkg_h

extern void
DomainParticipantEvent_on_after_publisher_modified(
                            DDS_Publisher *const publisher,
                            const struct DDS_PublisherQos *const qos,
                            const struct DDS_PublisherListener *const listener);

extern void
DomainParticipantEvent_on_after_publisher_enabled(
                            DDS_Publisher *const publisher,
                            const struct DDS_PublisherQos *const qos);

extern void
DomainParticipantEvent_on_after_subscriber_modified(
                        DDS_Subscriber *const subscriber,
                        const struct DDS_SubscriberQos *const qos,
                        const struct DDS_SubscriberListener *const listener);

extern void
DomainParticipantEvent_on_after_subscriber_enabled(
                        DDS_Subscriber *const subscriber,
                        const struct DDS_SubscriberQos *const qos);

extern void
DomainParticipantEvent_on_after_datareader_created(DDS_DataReader *const dw,
                        const struct DDS_DataReaderQos *const qos);

extern void
DomainParticipantEvent_on_after_datareader_enabled(
                        DDS_DataReader *const reader,
                        const struct DDS_DataReaderQos *const qos);

extern void
DomainParticipantEvent_on_after_datareader_modified(DDS_DataReader *const dr,
                        const struct DDS_DataReaderQos *const qos,
                        const struct DDS_DataReaderListener *const listener);

extern void
DomainParticipantEvent_on_before_datareader_deleted(DDS_DataReader *const reader);

extern void
DomainParticipantEvent_on_after_datawriter_created(DDS_DataWriter *const dw,
                        const struct DDS_DataWriterQos *const qos);

extern void
DomainParticipantEvent_on_after_datawriter_enabled(
                        DDS_DataWriter *const writer,
                        const struct DDS_DataWriterQos *const qos);

extern void
DomainParticipantEvent_on_after_datawriter_modified(DDS_DataWriter *const dw,
                        const struct DDS_DataWriterQos *const qos,
                        const struct DDS_DataWriterListener *const listener);

extern void
DomainParticipantEvent_on_before_datawriter_deleted(DDS_DataWriter *const writer);

extern void
NDDS_DomainParticipant_on_offered_incompatible_qos(DDS_DomainParticipant * self,
                        DDS_DataWriter * writer,
                        const struct DDS_OfferedIncompatibleQosStatus *status);

extern void
NDDS_DomainParticipant_on_publication_matched(DDS_DomainParticipant * self,
                        DDS_DataWriter * writer,
                        const struct DDS_PublicationMatchedStatus *status);

extern void
NDDS_DomainParticipant_on_requested_incompatible_qos(
                        DDS_DomainParticipant *self,
                        DDS_DataReader * reader,
                        const struct DDS_RequestedIncompatibleQosStatus *status);

extern void
NDDS_DomainParticipant_on_subscription_matched(DDS_DomainParticipant *self,
                        DDS_DataReader *reader,
                        const struct DDS_SubscriptionMatchedStatus *status);

extern void
NDDS_DomainParticipant_on_data_available(DDS_DomainParticipant *self,
                                         DDS_Subscriber *subscriber,
                                         DDS_DataReader *reader);

extern void
NDDS_DomainParticipant_on_requested_deadline_missed(DDS_DomainParticipant *dp,
                        DDS_DataReader *reader,
                        const struct DDS_RequestedDeadlineMissedStatus *status);

extern void
NDDS_DomainParticipant_on_liveliness_changed(DDS_DomainParticipant *dp,
                            DDS_DataReader * reader,
                            const struct DDS_LivelinessChangedStatus *status);

extern void
NDDS_DomainParticipant_on_sample_lost(DDS_DomainParticipant * dp,
                           DDS_DataReader *reader,
                           const struct DDS_SampleLostStatus *status);

extern void
NDDS_DomainParticipant_on_sample_rejected(DDS_DomainParticipant *self,
                            DDS_DataReader *reader,
                            const struct DDS_SampleRejectedStatus *status);

extern void
NDDS_DomainParticipant_on_inconsistent_topic(DDS_DomainParticipant * self,
                              DDS_Topic * topic,
                              const struct DDS_InconsistentTopicStatus *status);

extern void
NDDS_DomainParticipant_on_liveliness_lost(DDS_DomainParticipant * self,
                        DDS_DataWriter *writer,
                        const struct DDS_LivelinessLostStatus *status);

extern void
NDDS_DomainParticipant_on_offered_deadline_missed(DDS_DomainParticipant * self,
                        DDS_DataWriter * writer,
                        const struct DDS_OfferedDeadlineMissedStatus *status);

#endif /* DomainParticipantEvent_pkg_h */
