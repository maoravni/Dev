/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef SubscriberEvent_pkg_h
#define SubscriberEvent_pkg_h

extern DDS_Boolean
DDS_SubscriberEvent_on_requested_incompatible_qos(DDS_Subscriber *subscriber,
                   DDS_DataReader *reader,
                   const struct DDS_RequestedIncompatibleQosStatus*status);

extern DDS_Boolean
DDS_SubscriberEvent_on_subscription_matched(DDS_Subscriber *subscriber,
                            DDS_DataReader *reader,
                            const struct DDS_SubscriptionMatchedStatus *status);

extern DDS_Boolean
DDS_SubscriberEvent_on_data_available(DDS_Subscriber *subscriber,
                                      DDS_DataReader *reader);

extern  DDS_Boolean
DDS_SubscriberEvent_on_sample_rejected(DDS_Subscriber * subscriber,
                           DDS_DataReader *reader,
                           const struct DDS_SampleRejectedStatus *status);

extern DDS_Boolean
DDS_SubscriberEvent_on_requested_deadline_missed(DDS_Subscriber *subscriber,
                     DDS_DataReader *reader,
                     const struct DDS_RequestedDeadlineMissedStatus *status);

extern DDS_Boolean
DDS_SubscriberEvent_on_liveliness_changed(DDS_Subscriber * subscriber,
                   DDS_DataReader * reader,
                   const struct DDS_LivelinessChangedStatus *status);

extern DDS_Boolean
DDS_SubscriberEvent_on_sample_lost(DDS_Subscriber * subscriber,
                               DDS_DataReader * reader,
                               const struct DDS_SampleLostStatus *status);

#endif /* SubscriberEvent_pkg_h */
