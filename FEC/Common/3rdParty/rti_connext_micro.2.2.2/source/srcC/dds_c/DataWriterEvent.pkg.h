/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataWriterEvent_pkg_h
#define DataWriterEvent_pkg_h

extern void
DDS_DataWriterEvent_on_incompatible_qos(struct DDS_DataWriterImpl *dw,
                                        const DDS_BuiltinTopicKey_t *key);

extern void
DDS_DataWriterEvent_on_publication_matched(struct DDS_DataWriterImpl *dw,
                                           const DDS_BuiltinTopicKey_t *key,
                                           DDS_Boolean route_existed,
                                           DDS_Boolean matched);

extern OSAPITimeoutOp_t
DDS_DataWriterEvent_on_liveliness(struct OSAPITimeoutUserData *storage);

extern void
DDS_DataWriterEvent_on_sample_removed(void *config,
                                      DDS_InstanceHandle_t *key,
                                      struct DDSHST_WriterSample *sample);

extern void
DDS_DataWriterEvent_on_key_removed(void *config, DDS_InstanceHandle_t * key);

extern void
DDS_DataWriterEvent_on_deadline_missed(void *listener_data,
                                        DDS_InstanceHandle_t *key);

extern OSAPITimeoutOp_t
DDS_DataWriterEvent_on_deadline_expired(struct OSAPITimeoutUserData *storage);

#endif /* DataWriterEvent_pkg_h */
