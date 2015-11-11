/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DPSEParticipantBuiltinTopicData_pkg_h
#define DPSEParticipantBuiltinTopicData_pkg_h

typedef RTI_BOOL
(*DDS_ParticipantBuiltinTopicData_TypePluginDeserializeParameterValueFunction)(
        RTI_BOOL * ok, void *parameters, struct CDR_Stream_t * stream,
        DDS_UnsignedShort parameterId, DDS_UnsignedShort parameterLength);

typedef void
(*DDS_ParticipantBuiltinTopicData_TypePluginSetDefaultParameterValuesFunction)(void *parameter);

extern RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_copy_sample(
        struct NDDS_Type_Plugin *type,
        void *dst,
        const void *src,
        void *param);

extern RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_deserialize(
                                                struct CDR_Stream_t *stream,
                                                void *sample,
                                                void *param);

extern RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_serializeKey(
                                     struct CDR_Stream_t *stream,
                                     const void *sample_key,
                                     void *param);

extern RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_deserializeKey(
                                    struct CDR_Stream_t *stream,
                                    const UserDataKeyHolder_t sample_key,
                                    void *param);

extern RTI_BOOL
DPSE_ParticipantBuiltinTopicDataTypePlugin_instanceToKeyHash(
                                    struct NDDS_Type_Plugin *plugin,
                                    struct CDR_Stream_t *stream,
                                    DDS_KeyHash_t *keyHash,
                                    const void *instance,
                                    void *param);

struct NDDS_Type_Plugin*
DPSE_ParticipantBuiltinTopicDataTypePlugin_get();

#endif /* DPSEParticipantBuiltinTopicData_pkg_h */
