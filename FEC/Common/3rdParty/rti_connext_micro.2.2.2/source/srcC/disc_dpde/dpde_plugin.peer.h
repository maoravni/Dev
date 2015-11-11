/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dpde_plugin_peer_h
#define dpde_plugin_peer_h


/*e \ingroup DISCRtpsPidModule
  Used for field Participant::productVersion
 */
#define DISC_RTPS_PID_PRODUCT_VERSION                            (0x8000)

#define DISC_RTPS_PID_ENTITY_NAME   MIG_RTPS_PID_ENTITY_NAME


extern RTI_BOOL
DDS_Cdr_serializeFourByteParameter(struct CDR_Stream_t *stream,
                                   const void *in,
                                   DDS_UnsignedShort parameterId);

extern DDS_UnsignedLong
DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(DDS_UnsignedLong size);

extern DDS_UnsignedLong
DDS_CdrQosPolicy_get_header_max_size_serialized(DDS_UnsignedLong size);

typedef RTI_BOOL(*DDS_Cdr_DeserializeParameterValueFunction)
    (RTI_BOOL * ok, void *parameters, struct CDR_Stream_t * stream,
     unsigned short parameterId, unsigned short parameterLength);

typedef void (*DDS_Cdr_SetDefaultParameterValuesFunction)
    (void *parameter);

extern RTI_BOOL
DDS_Cdr_deserializeParameterSequence(void *parameter,
                                     struct CDR_Stream_t *stream,
                                     DDS_Cdr_SetDefaultParameterValuesFunction
                                     setDefaultParameterValuesFnc,
                                     DDS_Cdr_DeserializeParameterValueFunction
                                     deserializeParameterValueFnc);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeEntityNameQosPolicy(
    struct CDR_Stream_t *stream,
    const struct DDS_EntityNameQosPolicy *entityName,
    void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeEntityNameQosPolicy(
    struct CDR_Stream_t *stream,
    struct DDS_EntityNameQosPolicy *entityName,
    void * param);

extern unsigned int
DDS_CdrQosPolicy_getEntityNameQosPolicyMaxSizeSerialized(unsigned int size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeTopicName(struct CDR_Stream_t *stream,
                                    const char *topic_name,
                                    void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeTopicName(struct CDR_Stream_t *stream,
                                      char *topic_name,
                                      void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getTopicNameMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeTypeName(struct CDR_Stream_t *stream,
                                   const char *type_name,
                                   void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeTypeName(struct CDR_Stream_t *stream,
                                     char *type_name,
                                     void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getTypeNameMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeDeadline(struct CDR_Stream_t *stream,
                                   const struct DDS_DeadlineQosPolicy *deadline,
                                   void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeDeadline(struct CDR_Stream_t *stream,
                                     struct DDS_DeadlineQosPolicy *deadline,
                                     void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getDeadlineMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeOwnership(struct CDR_Stream_t *stream,
                                    const struct DDS_OwnershipQosPolicy
                                    *ownership,
                                    void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeOwnership(struct CDR_Stream_t *stream,
                                      struct DDS_OwnershipQosPolicy *ownership,
                                      void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getOwnershipMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeOwnershipStrength(struct CDR_Stream_t *stream,
                                            const struct
                                            DDS_OwnershipStrengthQosPolicy
                                            *ownership_strength,
                                            void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeOwnershipStrength(struct CDR_Stream_t *stream,
                                              struct
                                              DDS_OwnershipStrengthQosPolicy
                                              *ownership_strength,
                                              void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getOwnershipStrengthMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeReliability(struct CDR_Stream_t *stream,
                                      const struct DDS_ReliabilityQosPolicy
                                      *reliability,
                                      void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeReliability(struct CDR_Stream_t *stream,
                                        struct DDS_ReliabilityQosPolicy
                                        *reliability,
                                        void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getReliabilityMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeLiveliness(struct CDR_Stream_t *stream,
                            const struct DDS_LivelinessQosPolicy *liveliness,
                            void *param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeLiveliness(struct CDR_Stream_t *stream,
                                       struct DDS_LivelinessQosPolicy
                                       *liveliness,
                                       void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getLivelinessMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeKey(struct CDR_Stream_t *stream,
                              const struct DDS_BuiltinTopicKey_t *key,
                              DDS_UnsignedShort id);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeKey(struct CDR_Stream_t *stream,
                                struct DDS_BuiltinTopicKey_t *key,
                                void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getKeyMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeLocator(struct CDR_Stream_t *stream,
                                  const struct DDS_LocatorSeq *locator,
                                  DDS_UnsignedShort id);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeLocator(struct CDR_Stream_t *stream,
                                    struct DDS_LocatorSeq *locator,
                                    void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getLocatorMaxSerializedSize(struct DDS_LocatorSeq *locator,
                                             RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeProtocolVersion(struct CDR_Stream_t *stream,
                                          const struct DDS_ProtocolVersion
                                          *protocol,
                                          void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeProtocolVersion(struct CDR_Stream_t *stream,
                                            struct DDS_ProtocolVersion
                                            *protocol,
                                            void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getProtocolVersionMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeVendorId(struct CDR_Stream_t *stream,
                                   const struct DDS_VendorId *vendor,
                                   void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeVendorId(struct CDR_Stream_t *stream,
                                     struct DDS_VendorId *vendor,
                                     void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getVendorIdMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeDurability(struct CDR_Stream_t *stream,
                           const struct DDS_DurabilityQosPolicy *durability,
                           void *param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeDurability(struct CDR_Stream_t *stream,
                                struct DDS_DurabilityQosPolicy *durability,
                                void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getDurabilityMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeProductVersion(struct CDR_Stream_t *stream,
                                         const struct DDS_ProductVersion
                                         *product,
                                         void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeProductVersion(struct CDR_Stream_t *stream,
                                           struct DDS_ProductVersion *product,
                                           void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getProductVersionMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeLeaseDuration(struct CDR_Stream_t *stream,
                                        const struct DDS_Duration_t
                                        *lease_duration,
                                        void * param);

extern RTI_BOOL
DDS_CdrQosPolicy_deserializeLeaseDuration(struct CDR_Stream_t *stream,
                                          struct DDS_Duration_t *lease_duration,
                                          void * param);

extern RTI_UINT32
DDS_CdrQosPolicy_getLeaseDurationMaxSerializedSize(RTI_UINT32 size);

extern RTI_BOOL
DDS_CdrQosPolicy_serializeSendQueueSize(struct CDR_Stream_t *stream,
                                        DDS_Long send_queue_size,
                                        void * param);

#endif /* dpde_plugin_peer_h */
