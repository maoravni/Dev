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
#ifndef infrastructure_peer_h
#include "infrastructure.peer.h"
#endif

/* exported in infrastructure.ifc */
const struct DDS_BuiltinTopicKey_t DDS_BUILTINTOPICKEY_UNKNOWN =
    { {0, 0, 0, 0} };
const struct DDS_BuiltinTopicKey_t DDS_BUILTINTOPICKEY_AUTO = { {0, 0, 0, 0} };
const struct DDS_BuiltinTopicKey_t DDS_BUILTINTOPICKEY_PREFIX_UNKNOWN = { {0, 0, 0} };
const struct DDS_BuiltinTopicKey_t DDS_BUILTINTOPICKEY_PREFIX_AUTO = { {0, 0, 0,} };

/*** SOURCE_BEGIN ***/

DDS_Long
DDS_BuiltinTopicKey_compare(const DDS_BuiltinTopicKey_t *left,
                            const DDS_BuiltinTopicKey_t *right)
{
    DDS_Long i;

    for (i = 0; i < DDS_BUILTIN_TOPIC_KEY_TYPE_NATIVE_LENGTH; ++i)
    {
        if (left->value[i] > right->value[i])
        {
            return 1;
        }
        if (left->value[i] < right->value[i])
        {
            return -1;
        }
    }
    return 0;
}

DDS_Boolean
DDS_BuiltinTopicKey_equals(const DDS_BuiltinTopicKey_t *a,
                           const DDS_BuiltinTopicKey_t *b)
{
    LOG_testPrecondition(a == NULL || b == NULL, return DDS_BOOLEAN_FALSE);

    return MIGRtpsGuid_equals((struct MIGRtpsGuid*)a, (struct MIGRtpsGuid*)b);
}

DDS_Boolean
DDS_BuiltinTopicKey_prefix_equals(const DDS_BuiltinTopicKey_t * a,
                                  const DDS_BuiltinTopicKey_t * b)
{
    LOG_testPrecondition(a == NULL || b == NULL, return DDS_BOOLEAN_FALSE);

    return MIGRtpsGuid_prefix_equals((struct MIGRtpsGuid *)a,(struct MIGRtpsGuid *)b);
}

DDS_Boolean
DDS_BuiltinTopicKey_suffix_equals(const DDS_BuiltinTopicKey_t * a,
                                  const DDS_BuiltinTopicKey_t * b)
{
    LOG_testPrecondition(a == NULL || b == NULL, return DDS_BOOLEAN_FALSE);

    return MIGRtpsGuid_suffix_equals((struct MIGRtpsGuid *)a,(struct MIGRtpsGuid *)b);
}

void
DDS_BuiltinTopicKey_copy_prefix(DDS_BuiltinTopicKey_t * a,
                                const DDS_BuiltinTopicKey_t * b)
{
    LOG_testPrecondition(a == NULL || b == NULL, return);

    OSAPI_Memory_copy(a->value,b->value, sizeof(DDS_BUILTINTOPICKEY_PREFIX_UNKNOWN));
}

void
DDS_BuiltinTopicKey_copy_suffix(DDS_BuiltinTopicKey_t * a,
                                const DDS_BuiltinTopicKey_t * b)
{
    LOG_testPrecondition(a == NULL || b == NULL, return);

    a->value[3] = b->value[3];
}

void
DDS_BuiltinTopicKey_from_guid(DDS_BuiltinTopicKey_t *out,
                              const DDS_InstanceHandle_t *in)
{
    OSAPI_Memory_copy(out->value,in->octet, 16);
#ifdef RTI_ENDIAN_LITTLE
    out->value[0] = NETIO_ntohl(out->value[0]);
    out->value[1] = NETIO_ntohl(out->value[1]);
    out->value[2] = NETIO_ntohl(out->value[2]);
    out->value[3] = NETIO_ntohl(out->value[3]);
#endif
}

void
DDS_BuiltinTopicKey_to_guid(DDS_InstanceHandle_t *out,
                            const DDS_BuiltinTopicKey_t *in)
{
    DDS_UnsignedLong *ptr;
    OSAPI_Memory_copy(out->octet,in->value,16);
#ifdef RTI_ENDIAN_LITTLE
    ptr = (DDS_UnsignedLong*)out->octet;
    *ptr = NETIO_ntohl(*ptr);
    *(ptr+1) = NETIO_ntohl(*(ptr+1));
    *(ptr+2) = NETIO_ntohl(*(ptr+2));
    *(ptr+3) = NETIO_ntohl(*(ptr+3));
#endif
}

