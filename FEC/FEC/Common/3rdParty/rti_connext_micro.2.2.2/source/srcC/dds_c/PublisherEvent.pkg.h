/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef PublisherEvent_pkg_h
#define PublisherEvent_pkg_h

extern DDS_Boolean
DDS_PublisherEvent_on_offered_incompatible_qos(DDS_Publisher *self,
                    DDS_DataWriter *writer,
                    const struct DDS_OfferedIncompatibleQosStatus *status);

extern DDS_Boolean
DDS_PublisherEvent_on_publication_matched(DDS_Publisher *self,
                     DDS_DataWriter *writer,
                     const struct DDS_PublicationMatchedStatus *status);

extern DDS_Boolean
DDS_PublisherEvent_on_liveliness_lost(DDS_Publisher *self,
                                  DDS_DataWriter *writer,
                                  const struct DDS_LivelinessLostStatus *status);

extern DDS_Boolean
DDS_PublisherEvent_on_offered_deadline_missed(DDS_Publisher *self,
                    DDS_DataWriter *writer,
                    const struct DDS_OfferedDeadlineMissedStatus *status);

#endif /* PublisherEvent_pkg_h */
