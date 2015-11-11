/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataReaderDiscovery_pkg_h
#define DataReaderDiscovery_pkg_h

extern DDS_Boolean
DDS_DataReader_writer_is_compatible(DDS_DataReader *datareader,
                                    const struct DDS_DataWriterQos *dw_qos);

extern DDS_Boolean
DDS_DataReader_add_route(DDS_DataReader *self,
                         const DDS_BuiltinTopicKey_t *key,
                         const struct DDS_DataWriterQos *const qos,
                         const struct DDS_LocatorSeq *locator_seq,
                         RTI_BOOL *route_existed);

extern DDS_Boolean
DDS_DataReader_delete_route(DDS_DataReader *self,
                            const DDS_BuiltinTopicKey_t *key,
                            const struct DDS_LocatorSeq *locator_seq,
                            RTI_BOOL *route_existed);

extern DDS_Boolean
DDS_DataReader_match_writer(DDS_DataReader *self,
                            const struct DDS_BuiltinTopicKey_t *key,
                            const struct DDS_DataWriterQos *const qos,
                            const struct DDS_LocatorSeq *locator_seq);


extern DDS_Boolean
DDS_DataReader_unmatch_writer(DDS_DataReader *datareader,
                              const struct DDS_BuiltinTopicKey_t *key,
                              const struct DDS_DataWriterQos *const qos,
                              const struct DDS_LocatorSeq *locator_seq);

extern DDS_Boolean
DDS_DataReader_match_local_writer(DDS_DataReader *self,
                                  struct DDS_BuiltinTopicKey_t *key,
                                  const char *topic_name,
                                  const char *type_name,
                                  const struct DDS_DataWriterQos *const qos);

extern DDS_Boolean
DDS_DataReader_match_remote_writer(DDS_DataReader *self,
        const struct DDS_ParticipantBuiltinTopicData *const parent_data,
        const struct DDS_PublicationBuiltinTopicData *const data);

extern DDS_Boolean
DDS_DataReader_unmatch_local_writer(DDS_DataReader *self,
                                    struct DDS_BuiltinTopicKey_t *key,
                                    struct DDS_DataWriterQos *const qos);

extern DDS_Boolean
DDS_DataReader_unmatch_remote_writer(DDS_DataReader *self,
                    const struct DDS_ParticipantBuiltinTopicData *const parent_data,
                    const struct DDS_PublicationBuiltinTopicData *const data);

#endif /* DataReaderDiscovery_pkg_h */
