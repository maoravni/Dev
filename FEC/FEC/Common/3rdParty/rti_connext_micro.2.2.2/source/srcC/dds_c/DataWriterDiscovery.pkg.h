/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataWriterDiscovery_pkg_h
#define DataWriterDiscovery_pkg_h

extern DDS_Boolean
DDS_DataWriter_reader_is_compatible(DDS_DataWriter *datawriter,
                                    const struct DDS_DataReaderQos *dr_qos);

extern DDS_Boolean
DDS_DataWriter_add_route(DDS_DataWriter *datawriter,
                         const DDS_BuiltinTopicKey_t *key,
                         const struct DDS_DataReaderQos *const qos,
                         const struct DDS_LocatorSeq *locator_seq,
                         RTI_BOOL *route_existed);

extern DDS_Boolean
DDS_DataWriter_delete_route(DDS_DataWriter *datawriter,
                            const DDS_BuiltinTopicKey_t *key,
                            const struct DDS_LocatorSeq *locator_seq,
                            RTI_BOOL *route_existed);

extern DDS_Boolean
DDS_DataWriter_match_reader(DDS_DataWriter *datawriter,
                            const struct DDS_BuiltinTopicKey_t *key,
                            const struct DDS_DataReaderQos *const qos,
                            const struct DDS_LocatorSeq *locator_seq);

extern DDS_Boolean
DDS_DataWriter_unmatch_reader(DDS_DataWriter *datawriter,
                            const struct DDS_BuiltinTopicKey_t *key,
                            const struct DDS_DataReaderQos *const qos,
                            const struct DDS_LocatorSeq *locator_seq);

extern DDS_Boolean
DDS_DataWriter_match_local_reader(DDS_DataWriter *datawriter,
                                  struct DDS_BuiltinTopicKey_t *key,
                                  const char *topic_name,
                                  const char *type_name,
                                  const struct DDS_DataReaderQos *const qos);

extern DDS_Boolean
DDS_DataWriter_match_remote_reader(DDS_DataWriter *self,
        const struct DDS_ParticipantBuiltinTopicData *const parent_data,
        const struct DDS_SubscriptionBuiltinTopicData *const data);

extern DDS_Boolean
DDS_DataWriter_unmatch_local_reader(DDS_DataWriter *self,
                                    struct DDS_BuiltinTopicKey_t *key,
                                    struct DDS_DataReaderQos *const qos);

extern DDS_Boolean
DDS_DataWriter_unmatch_remote_reader(DDS_DataWriter *self,
        const struct DDS_ParticipantBuiltinTopicData *const parent_data,
        const struct DDS_SubscriptionBuiltinTopicData *const data);

#endif /* DataWriterDiscovery_pkg_h */
