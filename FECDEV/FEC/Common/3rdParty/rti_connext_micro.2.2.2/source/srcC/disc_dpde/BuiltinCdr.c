/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef disc_dpde_log_h
#include "disc_dpde/disc_dpde_log.h"
#endif


/*e \ingroup DISCRtpsPidModule
  Used for field Participant::productVersion
 */
#define DISC_RTPS_PID_PRODUCT_VERSION                            (0x8000)

#define DISC_RTPS_PID_ENTITY_NAME   MIG_RTPS_PID_ENTITY_NAME



#define CDR_PARAMETER_ALIGNMENT 4

/*** SOURCE_BEGIN ***/

RTI_PRIVATE DDS_Boolean
DDS_Cdr_insertParameterLength(struct CDR_Stream_t *stream,
                              DDS_Long parameterBeginPosition,
                              DDS_UnsignedShort parameterLength,
                              RTI_BOOL parameterSuccess)
{
    DDS_Long currentPosition;
    RTI_BOOL ok = RTI_TRUE;

    LOG_testPrecondition(stream == NULL ||
                         (stream->buffer + parameterBeginPosition) 
                          < stream->alignBase,
                         return RTI_FALSE);

    if (parameterSuccess)
    {
        currentPosition = CDR_Stream_GetCurrentPositionOffset(stream);

        ok = CDR_Stream_SetCurrentPositionOffset(stream,
                                                   parameterBeginPosition +
                                                   CDR_SHORT_SIZE)
            && CDR_Stream_serialize_UnsignedShort(stream, &parameterLength);

        if (ok)
        {
            CDR_Stream_SetCurrentPositionOffset(stream, currentPosition);
            return RTI_TRUE;
        }
    }

    /* undo this parameter */
    CDR_Stream_SetCurrentPositionOffset(stream, parameterBeginPosition);
    return RTI_FALSE;
}

RTI_BOOL
DDS_Cdr_serializeFourByteParameter(struct CDR_Stream_t *stream,
                                   const void *in,
                                   DDS_UnsignedShort parameterId)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength;
    DDS_Long parameterBeginPosition, valueBeginPosition, valueEndPosition;
    DDS_Long parameterSize;

    LOG_testPrecondition(stream == NULL || in == NULL, return RTI_FALSE);

    parameterBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter Id */
    ok = ok && CDR_Stream_serialize_UnsignedShort(stream, &parameterId);

    /* skip parameter length */
    ok = ok
        && CDR_Stream_IncrementCurrentPosition(stream, CDR_SHORT_SIZE);

    /* parameter value parameter has to be 4-byte aligned */
    valueBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);
    parameterSize = 4U;

#ifdef TODO
    ok = ok && CDR_Stream_CheckSize(stream, parameterSize);
#endif
    if (ok)
    {
        CDR_Stream_serialize_Long(stream, (RTI_INT32 *) in);
    }

    CDR_Stream_Align(stream, CDR_PARAMETER_ALIGNMENT);
    valueEndPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter length */
    parameterLength = valueEndPosition - valueBeginPosition;

    return DDS_Cdr_insertParameterLength(stream, parameterBeginPosition,
                                         parameterLength, ok);
}

RTI_BOOL
DDS_Cdr_serializeFourOctets(struct CDR_Stream_t * stream,
                            const char *in,
                            DDS_UnsignedShort parameterId)
{
RTI_BOOL ok = RTI_TRUE;

    CDR_Stream_serialize_1Byte(stream, &in[0]);
    CDR_Stream_serialize_1Byte(stream, &in[1]);
    CDR_Stream_serialize_1Byte(stream, &in[2]);
    CDR_Stream_serialize_1Byte(stream, &in[3]);

    return ok;
}

RTI_BOOL
DDS_Cdr_serializeNonPrimitiveSequenceParameter(struct CDR_Stream_t * stream,
                                               const void *in,
                                               DDS_UnsignedLong length,
                                               DDS_UnsignedLong maximumLength,
                                               DDS_UnsignedLong elementSize,
                                               CDR_Stream_SerializeFunction
                                               serializeFunction,
                                               DDS_UnsignedShort parameterId,
                                               RTI_BOOL serializeEncapsulation,
                                               RTI_BOOL serializeSample)
{
    RTI_BOOL ok = RTI_TRUE;
    struct REDA_Buffer state;
    DDS_UnsignedShort parameterLength;
    DDS_UnsignedLong parameterBeginPosition, valueBeginPosition,
        valueEndPosition;
    struct CDR_OctetSeq octetSeq;

    LOG_testPrecondition(stream == NULL || in == NULL ||
                         serializeFunction == NULL, return RTI_FALSE);

    /* repackage non-primitive sequence components into REDA sequence form */
    octetSeq._contiguous_buffer = (RTI_UINT8*) in;
    octetSeq._maximum = maximumLength;
    octetSeq._length = length;
    octetSeq._element_size = elementSize;

    parameterBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter Id */
    ok = ok && CDR_Stream_serialize_UnsignedShort(stream, &parameterId);

    /* skip parameter length */
    ok = ok
        && CDR_Stream_IncrementCurrentPosition(stream, CDR_SHORT_SIZE);

    /* parameter value parameter has to be 4-byte aligned */
    valueBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);
    ok = ok && CDR_Stream_serialize_NonPrimitiveSequence(stream,
                                    (const struct REDA_Sequence*) &octetSeq,
                                    serializeFunction,
                                    NULL);
    CDR_Stream_Align(stream, CDR_PARAMETER_ALIGNMENT);
    valueEndPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter length */
    parameterLength = valueEndPosition - valueBeginPosition;

    return DDS_Cdr_insertParameterLength(stream, parameterBeginPosition,
                                         parameterLength, ok);
}

RTI_BOOL
DDS_CdrQosPolicy_serializeNonPrimitiveParameter(struct CDR_Stream_t * stream,
                                                const void *in,
                                                CDR_Stream_SerializeFunction
                                                serializeFunction,
                                                DDS_UnsignedShort parameterId,
                                                RTI_BOOL serializeEncapsulation,
                                                RTI_BOOL serializeSample)
{
RTI_BOOL ok = RTI_TRUE;
struct REDA_Buffer state;
DDS_UnsignedShort parameterLength;
DDS_Long parameterBeginPosition, valueBeginPosition, valueEndPosition;

    LOG_testPrecondition(stream == NULL ||
                         ((in == NULL)
                          && (parameterId != MIG_RTPS_PID_TYPE_NAME)
                          && (parameterId != MIG_RTPS_PID_TOPIC_NAME))
                         || serializeFunction == NULL, return RTI_FALSE);

    CDR_Stream_Align(stream, CDR_PARAMETER_ALIGNMENT);
    parameterBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter Id */
    ok = ok && CDR_Stream_serialize_UnsignedShort(stream, &parameterId);

    /* skip parameter length */
    ok = ok
        && CDR_Stream_IncrementCurrentPosition(stream, CDR_SHORT_SIZE);

    /* parameter value parameter has to be 4-byte aligned */
    valueBeginPosition = CDR_Stream_GetCurrentPositionOffset(stream);
    ok = ok && serializeFunction(stream, in, NULL);
    CDR_Stream_Align(stream, CDR_PARAMETER_ALIGNMENT);
    valueEndPosition = CDR_Stream_GetCurrentPositionOffset(stream);

    /* parameter length */
    parameterLength = valueEndPosition - valueBeginPosition;

    return DDS_Cdr_insertParameterLength(stream, parameterBeginPosition,
                                         parameterLength, ok);
}

DDS_UnsignedLong
DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(DDS_UnsignedLong size)
{
DDS_UnsignedLong origSize = size;

    /* align to 4 bytes boundary */
    size = CDR_alignUpwards(size, CDR_PARAMETER_ALIGNMENT);

    /* paramterId */
    size += CDR_get_maxSizeSerialized_UnsignedShort(size);

    /* paramterLength */
    size += CDR_get_maxSizeSerialized_UnsignedShort(size);

    return (size - origSize);
}

DDS_UnsignedLong
DDS_CdrQosPolicy_get_header_max_size_serialized(DDS_UnsignedLong size)
{
DDS_Long origSize = size;

    /* align to 4 bytes boundary */
    size = CDR_alignUpwards(size, CDR_PARAMETER_ALIGNMENT);

    CDR_Stream_getEncapsulationSize(size);

    return size - origSize;

}

void
DDS_Cdr_ipv6_from_ipv4(struct DDS_Locator_t *ipv6Locator,
                       struct DDS_LocatorUdpv4_t *ipv4Locator)
{
    RTI_UINT32 tmp = ipv4Locator->address;

    OSAPI_Memory_zero(ipv6Locator->address, 12);

    /* This does not need to be byte swapped because we always store the locator
     * in network order.
     */
    ipv6Locator->address[12] = (unsigned char)(tmp >> 24);
    ipv6Locator->address[13] = (unsigned char)((tmp & 0x00FF0000) >> 16);
    ipv6Locator->address[14] = (unsigned char)((tmp & 0x0000FF00) >> 8);
    ipv6Locator->address[15] = (unsigned char)((tmp & 0x000000FF));
}

RTI_BOOL
DDS_Cdr_serializeString(struct CDR_Stream_t *stream,
                        const char *in,
                        RTI_BOOL serializeEncapsulation, RTI_BOOL serializeData)
{
    if (in == NULL)
    {
        return CDR_Stream_serialize_String(stream, in,
                                         MIG_RTPS_PATHNAME_LEN_MAX + 1);
    }
    else
    {
        return CDR_Stream_serialize_String(stream, in,
                                         MIG_RTPS_PATHNAME_LEN_MAX + 1);
    }
}

/*i \ingroup DDS_PublicationBuiltinTopicData_TypePluginModule
  Prototype for deserializing parameter value of the given parameterId.

  @param ok \b Out. Indicate if parameter value is correctly deserialized.
  @param parameter \b Out. Pointer to the parameter to which value should be
  deserialized.
  @param stream \b InOut. Stream for deserialization.
  @param parameterId \b In. Parameter ID of the value to be deserialized.
  @param parameterLength \b In. Length of parameter value.
  @param deserializeOption \b In. deserialize option obtained from
  reader property.

  @return Return RTI_TRUE if there is a match in parameterId, and ok is set to
  RTI_TRUE if deserialization of the parameter value is successful.
  Otherwise, return RTI_FALSE.

  Returning RTI_TRUE does not mean that the parameter value is
  correctly deserialized.
*/
typedef RTI_BOOL(*DDS_Cdr_DeserializeParameterValueFunction)
    (RTI_BOOL * ok, void *parameters, struct CDR_Stream_t * stream,
     unsigned short parameterId, unsigned short parameterLength);

/*i \ingroup DDS_PublicationBuiltinTopicData_TypePluginModule
  Prototype for setting parameter to default value.

  @param parameter \b Out. Setup parameter with default values.
*/
typedef void (*DDS_Cdr_SetDefaultParameterValuesFunction)
    (void *parameter);

RTI_BOOL
DDS_Cdr_deserializeParameterSequence(void *parameter,
                                     struct CDR_Stream_t *stream,
                                     DDS_Cdr_SetDefaultParameterValuesFunction
                                     setDefaultParameterValuesFnc,
                                     DDS_Cdr_DeserializeParameterValueFunction
                                     deserializeParameterValueFnc)
{
    unsigned short parameterId, parameterLength;
    RTI_BOOL ok = RTI_TRUE;
    struct REDA_Buffer state;
    DDS_Long origPosition;

    LOG_testPrecondition((parameter == NULL) || (stream == NULL) ||
                         (setDefaultParameterValuesFnc == NULL) ||
                         (deserializeParameterValueFnc == NULL),
                         return RTI_FALSE);

    /* set parameter to default value */
    setDefaultParameterValuesFnc(parameter);

    /* parameter id and length */
    ok = ok && CDR_Stream_deserialize_UnsignedShort(stream, &parameterId);
    ok = ok && CDR_Stream_deserialize_UnsignedShort(stream, &parameterLength);

    while (ok && parameterId != MIG_RTPS_PID_SENTINEL)
    {
        origPosition = CDR_Stream_GetCurrentPositionOffset(stream);

        /* logical reset of stream per parameter value */
        deserializeParameterValueFnc(&ok, parameter,
                                     stream, parameterId, parameterLength);

        /* skip to next parameter */
        ok = ok && CDR_Stream_SetCurrentPositionOffset(stream,
                                                       origPosition +
                                                       parameterLength);

        /* parameter id and length */
        ok = ok && CDR_Stream_deserialize_UnsignedShort(stream, &parameterId);
        ok = ok && CDR_Stream_deserialize_UnsignedShort(stream, &parameterLength);
    }

    return ok;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeEntityNameQosPolicy(
    struct CDR_Stream_t *stream,
    const struct DDS_EntityNameQosPolicy *entityName,
    void * param)
{
    if (entityName->name == NULL)
    {
        return RTI_TRUE;
    }

    if (entityName != NULL)
    {
        if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter
            (stream, entityName->name,
             (CDR_Stream_SerializeFunction) DDS_Cdr_serializeString,
             DISC_RTPS_PID_ENTITY_NAME, RTI_FALSE, RTI_TRUE))
        {
            /*LOG_printError("serialization of entity name failed\n"); */
            DPDE_Log_error(LOG_FAIL_SERIALIZE_ENTITY_NAME);
            return RTI_FALSE;
        }
    }

    return RTI_TRUE;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeEntityNameQosPolicy(
    struct CDR_Stream_t *stream,
    struct DDS_EntityNameQosPolicy *entityName,
    void * param)
{
    if (!CDR_Stream_deserialize_String(stream, entityName->name,
                                       MIG_RTPS_PATHNAME_LEN_MAX))
    {
        return RTI_FALSE;
    }
    return RTI_TRUE;
}


unsigned int
DDS_CdrQosPolicy_getEntityNameQosPolicyMaxSizeSerialized(unsigned int size)
{
    unsigned int origSize = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);

    size += CDR_get_maxSizeSerialized_String(size, MIG_RTPS_PATHNAME_LEN_MAX + 1);

    return (size - origSize);
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeTopicName(struct CDR_Stream_t *stream,
                                    const char *topic_name,
                                    void * param)
{
    RTI_BOOL retval = RTI_FALSE;

    if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter(
            stream, topic_name,
            (CDR_Stream_SerializeFunction) DDS_Cdr_serializeString,
            MIG_RTPS_PID_TOPIC_NAME, RTI_FALSE, RTI_TRUE))
    {
        /*LOG_printError("serialization of topic_name failed\n"); */
        DPDE_Log_error(LOG_FAIL_SERIALIZE_TOPIC_NAME);
        goto done;
    }

    retval = RTI_TRUE;

    done:
    return retval;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeTopicName(struct CDR_Stream_t *stream,
                                      char *topic_name,
                                      void * param)
{
    return CDR_Stream_deserialize_String(stream, topic_name,
                                         MIG_RTPS_PATHNAME_LEN_MAX);
}

RTI_UINT32
DDS_CdrQosPolicy_getTopicNameMaxSerializedSize(RTI_UINT32 size)
{
RTI_UINT32 orig_size = size;

    size += CDR_get_maxSizeSerialized_String(size, MIG_RTPS_PATHNAME_LEN_MAX + 1);

    return size - orig_size;
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeTypeName(struct CDR_Stream_t *stream,
                                   const char *type_name,
                                   void * param)
{
    RTI_BOOL retval = RTI_FALSE;

    if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter
        (stream, type_name,
         (CDR_Stream_SerializeFunction) DDS_Cdr_serializeString,
         MIG_RTPS_PID_TYPE_NAME, RTI_FALSE, RTI_TRUE))
    {
        /*LOG_printError("serialization of type_name failed\n"); */
        DPDE_Log_error(LOG_FAIL_SERIALIZE_TYPE_NAME);
        goto done;
    }

    retval = RTI_TRUE;

    done:
    return retval;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeTypeName(struct CDR_Stream_t *stream,
                                     char *type_name,
                                     void * param)
{
    return CDR_Stream_deserialize_String(stream, type_name,
                                         MIG_RTPS_PATHNAME_LEN_MAX);
}

RTI_UINT32
DDS_CdrQosPolicy_getTypeNameMaxSerializedSize(RTI_UINT32 size)
{
RTI_UINT32 orig_size = size;

    size += CDR_get_maxSizeSerialized_String(size, MIG_RTPS_PATHNAME_LEN_MAX + 1);

    return size - orig_size;
}

#define DDS_CdrQosPolicy_serializeParameter(id_,length_) \
    parameterId = id_;\
    parameterLength = length_;\
    ok = ok && CDR_Stream_serialize_UnsignedShort(stream, &parameterId);\
    ok = ok && CDR_Stream_serialize_UnsignedShort(stream, &parameterLength);\

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeDeadline(struct CDR_Stream_t *stream,
                                   const struct DDS_DeadlineQosPolicy *deadline,
                                   void * param)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength, parameterId;

    DDS_CdrQosPolicy_serializeParameter(MIG_RTPS_PID_DEADLINE,
                                        2 * CDR_get_maxSizeSerialized_Long(0));

    ok = ok
        && CDR_Stream_serialize_UnsignedLong(stream,
                                           (RTI_UINT32 *) & deadline->period.
                                           sec);
    ok = ok
        && CDR_Stream_serialize_UnsignedLong(stream,
                                           (RTI_UINT32 *) & deadline->period.
                                           nanosec);

    return ok;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeDeadline(struct CDR_Stream_t *stream,
                                     struct DDS_DeadlineQosPolicy *deadline,
                                     void * param)
{
    RTI_BOOL ok = RTI_TRUE;

    ok = ok && CDR_Stream_deserialize_UnsignedLong(stream,
                                                 (RTI_UINT32 *) &
                                                 deadline->period.sec);
    ok = ok
        && CDR_Stream_deserialize_UnsignedLong(stream,
                                             (RTI_UINT32 *) & deadline->period.
                                             nanosec);

    return ok;
}

RTI_UINT32
DDS_CdrQosPolicy_getDeadlineMaxSerializedSize(RTI_UINT32 size)
{
RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += CDR_get_maxSizeSerialized_Long(0);
    size += CDR_get_maxSizeSerialized_Long(0);

    return size - orig_size;
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeOwnership(struct CDR_Stream_t *stream,
                                    const struct DDS_OwnershipQosPolicy
                                    *ownership,
                                    void * param)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength, parameterId;

    DDS_CdrQosPolicy_serializeParameter(MIG_RTPS_PID_OWNERSHIP,
                                        CDR_get_maxSizeSerialized_Long(0));

    ok = ok && CDR_Stream_serialize_UnsignedLong(stream,
                                               (RTI_UINT32 *) &
                                               ownership->kind);
    return ok;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeOwnership(struct CDR_Stream_t *stream,
                                      struct DDS_OwnershipQosPolicy *ownership,
                                      void * param)
{
    return CDR_Stream_deserialize_UnsignedLong(stream,
                                             (RTI_UINT32 *) & ownership->kind);
}

RTI_UINT32
DDS_CdrQosPolicy_getOwnershipMaxSerializedSize(RTI_UINT32 size)
{
RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += CDR_get_maxSizeSerialized_Long(0);

    return size - orig_size;
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeOwnershipStrength(struct CDR_Stream_t *stream,
                                            const struct
                                            DDS_OwnershipStrengthQosPolicy
                                            *ownership_strength,
                                            void * param)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength, parameterId;

    DDS_CdrQosPolicy_serializeParameter(MIG_RTPS_PID_OWNERSHIP_STRENGTH,
                                        CDR_get_maxSizeSerialized_Long(0));

    ok = ok && CDR_Stream_serialize_UnsignedLong(stream,
                                               (RTI_UINT32 *) &
                                               ownership_strength->value);
    return ok;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeOwnershipStrength(struct CDR_Stream_t *stream,
                                              struct
                                              DDS_OwnershipStrengthQosPolicy
                                              *ownership_strength,
                                              void * param)
{
    return CDR_Stream_deserialize_UnsignedLong(stream,
                                             (RTI_UINT32 *) &
                                             ownership_strength->value);
}

RTI_UINT32
DDS_CdrQosPolicy_getOwnershipStrengthMaxSerializedSize(RTI_UINT32 size)
{
RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += CDR_get_maxSizeSerialized_Long(0);

    return size - orig_size;
}

/*----------------------------------------------------------------------------*/


RTI_BOOL
DDS_CdrQosPolicy_serializeReliability(struct CDR_Stream_t *stream,
                                      const struct DDS_ReliabilityQosPolicy
                                      *reliability,
                                      void * param)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength, parameterId;

    DDS_CdrQosPolicy_serializeParameter(MIG_RTPS_PID_RELIABILITY,
                                        3 * CDR_get_maxSizeSerialized_Long(0));

    ok = ok && CDR_Stream_serialize_UnsignedLong(stream,
                                               (RTI_UINT32 *) &
                                               reliability->kind);

    ok = ok && CDR_Stream_serialize_UnsignedLong(stream,
                                               (RTI_UINT32 *) &
                                               reliability->
                                               max_blocking_time.sec);

    ok = ok && CDR_Stream_serialize_UnsignedLong(stream,
                                               (RTI_UINT32 *) &
                                               reliability->
                                               max_blocking_time.nanosec);

    return ok;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeReliability(struct CDR_Stream_t *stream,
                                        struct DDS_ReliabilityQosPolicy
                                        *reliability,
                                        void * param)
{
    RTI_BOOL ok = RTI_TRUE;

    ok = ok && CDR_Stream_deserialize_UnsignedLong(stream,
                                                 (RTI_UINT32 *) &
                                                 reliability->kind);

    ok = ok && CDR_Stream_deserialize_UnsignedLong(stream,
                                                 (RTI_UINT32 *) &
                                                 reliability->
                                                 max_blocking_time.sec);

    ok = ok && CDR_Stream_deserialize_UnsignedLong(stream,
                                                 (RTI_UINT32 *) &
                                                 reliability->
                                                 max_blocking_time.nanosec);

    return ok;
}

RTI_UINT32
DDS_CdrQosPolicy_getReliabilityMaxSerializedSize(RTI_UINT32 size)
{
RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += 3 * CDR_get_maxSizeSerialized_Long(0);

    return size - orig_size;
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeLiveliness(struct CDR_Stream_t *stream,
                            const struct DDS_LivelinessQosPolicy *liveliness,
                            void *param)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength, parameterId;

    DDS_CdrQosPolicy_serializeParameter(MIG_RTPS_PID_LIVELINESS,
                                        3 * CDR_get_maxSizeSerialized_Long(0));

    ok = ok
        && CDR_Stream_serialize_UnsignedLong(stream,
                        (RTI_UINT32 *)&liveliness->kind);

    ok = ok && CDR_Stream_serialize_UnsignedLong(stream,
                        (RTI_UINT32 *)&liveliness->lease_duration.sec);

    ok = ok && CDR_Stream_serialize_UnsignedLong(stream,
                        (RTI_UINT32 *)&liveliness->lease_duration.nanosec);

    return ok;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeLiveliness(struct CDR_Stream_t *stream,
                                       struct DDS_LivelinessQosPolicy
                                       *liveliness,
                                       void * param)
{
    RTI_BOOL ok = RTI_TRUE;

    ok = ok
        && CDR_Stream_deserialize_UnsignedLong(stream,
                                (RTI_UINT32 *)&liveliness->kind);

    ok = ok
        && CDR_Stream_deserialize_UnsignedLong(stream,
                            (RTI_UINT32 *)&liveliness->lease_duration.sec);

    ok = ok && CDR_Stream_deserialize_UnsignedLong(stream,
                            (RTI_UINT32 *)&liveliness->lease_duration.nanosec);

    return ok;
}

RTI_UINT32
DDS_CdrQosPolicy_getLivelinessMaxSerializedSize(RTI_UINT32 size)
{
    RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += 3 * CDR_get_maxSizeSerialized_Long(0);

    return (size - orig_size);
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeKey(struct CDR_Stream_t *stream,
                              const struct DDS_BuiltinTopicKey_t *key,
                              DDS_UnsignedShort id)
{
    struct MIGRtpsGuid guid;

    guid.prefix.hostId = key->value[0];
    guid.prefix.appId = key->value[1];
    guid.prefix.instanceId = key->value[2];
    guid.objectId = key->value[3];

    if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter
        (stream, &guid, (CDR_Stream_SerializeFunction) MIGRtpsGuid_serialize,
         id, RTI_FALSE, RTI_TRUE))
    {
        /*LOG_printError("Serialization of GUID failed\n"); */
        DPDE_Log_error(LOG_FAIL_SERIALIZE_GUID);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeKey(struct CDR_Stream_t *stream,
                                struct DDS_BuiltinTopicKey_t *key,
                                void * param)
{
    struct MIGRtpsGuid guid;

    if (!MIGRtpsGuid_deserialize(stream, &guid, NULL))
    {
        return RTI_FALSE;
    }

    key->value[0] = guid.prefix.hostId;
    key->value[1] = guid.prefix.appId;
    key->value[2] = guid.prefix.instanceId;
    key->value[3] = guid.objectId;

    return RTI_TRUE;
}

RTI_UINT32
DDS_CdrQosPolicy_getKeyMaxSerializedSize(RTI_UINT32 size)
{
    RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += sizeof(struct DDS_BuiltinTopicKey_t);

    return size - orig_size;
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeLocator(struct CDR_Stream_t *stream,
                                  const struct DDS_LocatorSeq *locator,
                                  DDS_UnsignedShort id)
{
    struct DDS_Locator loc;
    RTI_UINT32 i, l;

    l = DDS_LocatorSeq_get_length(locator);

    for (i = 0; i < l; ++i)
    {

        loc = *DDS_LocatorSeq_get_reference(locator, i);

        if (loc.kind == RTPS_LOCATOR_KIND_UDPv4)
        {

    struct DDS_LocatorUdpv4_t *udp4_loc = (struct DDS_LocatorUdpv4_t *)&loc;

            DDS_Cdr_ipv6_from_ipv4(&loc, udp4_loc);
        }

        if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter(stream, &loc,
                                                             (CDR_Stream_SerializeFunction)
                                                             MIGRtps_serializeIpv6Locator,
                                                             id, RTI_FALSE,
                                                             RTI_TRUE))
        {
            /*LOG_printError("Serialization of default unicast loc failed\n"); */
            DPDE_Log_error(LOG_FAIL_SERIALIZE_DEFAULT_UNICAST_LOCATOR);
            return RTI_FALSE;
        }
    }

    return RTI_TRUE;
}


RTI_BOOL
DDS_CdrQosPolicy_deserializeLocator(struct CDR_Stream_t *stream,
                                    struct DDS_LocatorSeq *locator,
                                    void * param)
{
    struct DDS_Locator *loc;
    RTI_UINT32 l, ml;

    l = DDS_LocatorSeq_get_length(locator);
    ml = DDS_LocatorSeq_get_maximum(locator);

    if (l == ml)
    {
        return RTI_FALSE;
    }

    DDS_LocatorSeq_set_length(locator, l + 1);

    loc = DDS_LocatorSeq_get_reference(locator, l);
    if (!MIGRtps_deserializeIpv6Locator(stream, loc, NULL))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_UINT32
DDS_CdrQosPolicy_getLocatorMaxSerializedSize(struct DDS_LocatorSeq *locator,
                                             RTI_UINT32 size)
{
    RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += CDR_get_maxSizeSerialized_UnsignedLong(size);
    size += DDS_LocatorSeq_get_maximum(locator)
        * MIGRtps_getIpv6LocatorMaxSizeSerialized(size);

    return (size - orig_size);
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeProtocolVersion(struct CDR_Stream_t *stream,
                                          const struct DDS_ProtocolVersion
                                          *protocol,
                                          void * param)
{
    RTI_UINT16 protocolVersion;

    protocolVersion = (protocol->major << 8) | protocol->minor;

    if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter
        (stream, &protocolVersion,
         (CDR_Stream_SerializeFunction) MIGRtps_serialize2Octets,
         MIG_RTPS_PID_PROTOCOL_VERSION, RTI_FALSE, RTI_TRUE))
    {
        /*LOG_printError("Serialization of protocol version failed"); */
        DPDE_Log_error(LOG_FAIL_SERIALIZE_PROTOCOL_VERSION);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeProtocolVersion(struct CDR_Stream_t *stream,
                                            struct DDS_ProtocolVersion
                                            *protocol,
                                            void * param)
{
    RTI_UINT16 protocolVersion;

    if (!MIGRtps_deserialize2Octets(stream, &protocolVersion, NULL))
    {
        /*LOG_printDebug("Error deserializing protocol version\n"); */
        DPDE_Log_error(LOG_FAIL_DESERIALIZE_PROTOCOL_VERSION);
        return RTI_FALSE;
    }

    protocol->major = protocolVersion >> 8;
    protocol->minor = protocolVersion & 0x00ff;

    return RTI_TRUE;
}

RTI_UINT32
DDS_CdrQosPolicy_getProtocolVersionMaxSerializedSize(RTI_UINT32 size)
{
RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += MIGRtps_get2OctetsMaxSizeSerialized(size);
    size += MIGRtps_get2OctetsMaxSizeSerialized(size);

    return (size - orig_size);
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeVendorId(struct CDR_Stream_t *stream,
                                   const struct DDS_VendorId *vendor,
                                   void * param)
{
    RTI_UINT16 vendorId;

    vendorId = (vendor->vendorId[0] << 8) | vendor->vendorId[1];

    if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter
        (stream, &vendorId,
         (CDR_Stream_SerializeFunction) MIGRtps_serialize2Octets,
         MIG_RTPS_PID_VENDOR_ID, RTI_FALSE, RTI_TRUE))
    {
        /*LOG_printError("Serialization of vendor id failed\n"); */
        DPDE_Log_error(LOG_FAIL_SERIALIZE_VENDOR_ID);

        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeVendorId(struct CDR_Stream_t *stream,
                                     struct DDS_VendorId *vendor,
                                     void * param)
{
    MIGRtpsVendorId vendorId;

    if (!MIGRtps_deserialize2Octets(stream, &vendorId, NULL))
    {
        /*LOG_printDebug("Error deserializing vendor ID\n"); */
        DPDE_Log_error(LOG_FAIL_DESERIALIZE_VENDOR_ID);
        return RTI_FALSE;
    }

    vendor->vendorId[0] = MIGRtpsVendorId_getMajor(&vendorId);
    vendor->vendorId[1] = MIGRtpsVendorId_getMinor(&vendorId);

    return RTI_TRUE;
}

RTI_UINT32
DDS_CdrQosPolicy_getVendorIdMaxSerializedSize(RTI_UINT32 size)
{
    RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += MIGRtps_get2OctetsMaxSizeSerialized(size);

    return (size - orig_size);
}
/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeDurability(struct CDR_Stream_t *stream,
                           const struct DDS_DurabilityQosPolicy *durability,
                           void *param)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength, parameterId;

    DDS_CdrQosPolicy_serializeParameter(MIG_RTPS_PID_DURABILITY,
                                        CDR_get_maxSizeSerialized_Long(0));

    ok = ok
        && CDR_Stream_serialize_UnsignedLong(stream,
                                           (RTI_UINT32 *)&durability->kind);

    return RTI_TRUE;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeDurability(struct CDR_Stream_t *stream,
                                struct DDS_DurabilityQosPolicy *durability,
                                void * param)
{
    RTI_BOOL ok = RTI_TRUE;

    ok = ok
        && CDR_Stream_deserialize_UnsignedLong(stream,
                                             (RTI_UINT32 *) & durability->kind);

    return ok;
}

RTI_UINT32
DDS_CdrQosPolicy_getDurabilityMaxSerializedSize(RTI_UINT32 size)
{
    RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += CDR_get_maxSizeSerialized_Long(0);

    return (size - orig_size);
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeProductVersion(struct CDR_Stream_t *stream,
                                         const struct DDS_ProductVersion
                                         *product,
                                         void * param)
{
    /* Product version */
    if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter
        (stream, product,
         (CDR_Stream_SerializeFunction) DDS_Cdr_serializeFourOctets,
         DISC_RTPS_PID_PRODUCT_VERSION, RTI_FALSE, RTI_TRUE))
    {
        /*LOG_printError("Serialization of product version failed\n"); */
        DPDE_Log_error(LOG_FAIL_SERIALIZE_PRODUCT_VERSION);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeProductVersion(struct CDR_Stream_t *stream,
                                           struct DDS_ProductVersion *product,
                                           void * param)
{
    CDR_Stream_deserialize_1Byte(stream, &product->major);
    CDR_Stream_deserialize_1Byte(stream, &product->minor);
    CDR_Stream_deserialize_1Byte(stream, &product->release);
    CDR_Stream_deserialize_1Byte(stream, &product->revision);

    return RTI_TRUE;
}

RTI_UINT32
DDS_CdrQosPolicy_getProductVersionMaxSerializedSize(RTI_UINT32 size)
{
    RTI_UINT32 origSize = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += CDR_get1ByteMaxSizeSerialized(size);
    size += CDR_get1ByteMaxSizeSerialized(size);
    size += CDR_get1ByteMaxSizeSerialized(size);
    size += CDR_get1ByteMaxSizeSerialized(size);

    return (size - origSize);
}

/*----------------------------------------------------------------------------*/

RTI_BOOL
DDS_CdrQosPolicy_serializeLeaseDuration(struct CDR_Stream_t *stream,
                                        const struct DDS_Duration_t
                                        *lease_duration,
                                        void * param)
{
    struct OSAPI_NtpTime serializeTime;

    OSAPI_NtpTime_packFromNanosec(&serializeTime,
                                 lease_duration->sec, lease_duration->nanosec);

    if (!DDS_CdrQosPolicy_serializeNonPrimitiveParameter
        (stream, &serializeTime,
         (CDR_Stream_SerializeFunction) MIGRtps_serializeRtiNtpTime,
         MIG_RTPS_PID_LEASE_DURATION, RTI_FALSE, RTI_TRUE))
    {
        /*LOG_printError("Serialization of participant lease duration failed\n"); */
        DPDE_Log_error(LOG_FAIL_SERIALIZE_PARTICIPANT_LEASE_DURATION);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
DDS_CdrQosPolicy_deserializeLeaseDuration(struct CDR_Stream_t *stream,
                                          struct DDS_Duration_t *lease_duration,
                                          void * param)
{
    struct OSAPI_NtpTime time;

    if (!MIGRtps_deserializeRtiNtpTime(stream, &time, NULL))
    {
        return RTI_FALSE;
    }

    OSAPI_NtpTime_unpackToNanosec(&lease_duration->sec,
                                 &lease_duration->nanosec, &time);

    return RTI_TRUE;
}

RTI_UINT32
DDS_CdrQosPolicy_getLeaseDurationMaxSerializedSize(RTI_UINT32 size)
{
    RTI_UINT32 orig_size = size;

    size += DDS_CdrQosPolicy_get_parameter_header_max_size_serialized(size);
    size += 3 * CDR_get_maxSizeSerialized_Long(0);

    return (size - orig_size);
}

RTI_BOOL
DDS_CdrQosPolicy_serializeSendQueueSize(struct CDR_Stream_t *stream,
                                        DDS_Long send_queue_size,
                                        void * param)
{
    RTI_BOOL ok = RTI_TRUE;
    DDS_UnsignedShort parameterLength, parameterId;

    DDS_CdrQosPolicy_serializeParameter(MIG_RTPS_PID_SEND_QUEUE_SIZE_DEPRECATED,
                                        CDR_get_maxSizeSerialized_Long(0));

    ok = ok && CDR_Stream_serialize_Long(stream, (RTI_INT32 *) & send_queue_size);

    return ok;
}
