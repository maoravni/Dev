/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataReaderEvent_pkg_h
#define DataReaderEvent_pkg_h

extern void
DDS_DataReaderEvent_on_incompatible_qos(struct DDS_DataReaderImpl *dr,
                                        const DDS_BuiltinTopicKey_t *key);

extern void
DDS_DataReaderEvent_on_subscription_matched(struct DDS_DataReaderImpl *dr,
                                           const DDS_BuiltinTopicKey_t *key,
                                           DDS_Boolean route_existed,
                                           DDS_Boolean matched);

extern void
DDS_DataReaderEvent_on_data_available(struct DDSHST_Reader *rh,
                                      void *listener_data,
                                      DDS_InstanceHandle_t * key,
                                      DDSHST_ReaderSample_T * sample);

extern void
DDS_DataReaderEvent_on_sample_removed(struct DDSHST_Reader *rh,
                                        void *listener_data,
                                        DDS_InstanceHandle_t *key,
                                        DDSHST_ReaderSample_T *sample);

extern void
DDS_DataReaderEvent_on_hst_sample_rejected(
                        struct DDSHST_Reader *rh,
                        void *listener_data,
                        DDS_InstanceHandle_t *key,
                        DDS_SampleRejectedStatusKind reason);

extern void
DDS_DataReaderEvent_sample_lost(struct DDS_DataReaderImpl *dr);

extern void
DDS_DataReaderEvent_on_hst_sample_lost(
        struct DDSHST_Reader *rh,
        void *listener_data);

extern void
DDS_DataReaderEvent_on_sample_lost(struct DDS_DataReaderImpl *dr,
                                   struct NETIO_Address *source,
                                   struct REDA_SequenceNumber *first_sn,
                                   RTI_UINT32 count);

extern RTI_BOOL
DDS_DataReaderEvent_on_commit_data(struct DDS_DataReaderImpl *datareader,
                                   struct NETIO_Address *source,
                                   struct REDA_SequenceNumber *first_sn,
                                   RTI_UINT32 count);

extern void
DDS_DataReaderEvent_on_liveliness_changed(struct DDS_DataReaderImpl *datareader,
                                          DDS_InstanceHandle_t *instance);

extern void
DDS_DataReaderEvent_on_liveliness_detected(struct DDS_DataReaderImpl *datareader,
                                           struct NETIO_Address *source);

extern void
DDS_DataReaderEvent_on_liveliness_lost(struct DDS_DataReaderImpl *datareader,
                                       struct NETIO_Address *source);

extern void
DDS_DataReaderEvent_deadline_missed(struct DDS_DataReaderImpl *datareader,
                                    DDS_InstanceHandle_t *ih);

extern void
DDS_DataReaderEvent_on_deadline_missed(struct DDSHST_Reader *rh,
                                        void *listener_data,
                                        DDS_InstanceHandle_t *key);

extern OSAPITimeoutOp_t
DDS_DataReaderEvent_on_deadline_timeout(struct OSAPITimeoutUserData *storage);

#endif /* DataReaderEvent_pkg_h */
