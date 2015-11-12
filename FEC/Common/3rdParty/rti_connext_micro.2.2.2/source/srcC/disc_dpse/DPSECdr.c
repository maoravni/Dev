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


/*e \ingroup DISCRtpsPidModule
  Used for field Participant::productVersion
 */
#define DISC_RTPS_PID_PRODUCT_VERSION                            (0x8000)

#define DISC_RTPS_PID_ENTITY_NAME   MIG_RTPS_PID_ENTITY_NAME



/*** SOURCE_BEGIN ***/

/* ----------------------------------------------------------------- */

/* Product Version */

RTI_BOOL
DPSE_Builtin_serializeProductVersion(struct CDR_Stream_t *stream,
                         const struct DDS_ProductVersion_t *productVersion,
                         void *param)
{

    /* TODO:  Add checking for size of buffer */
    CDR_Stream_serialize_1Byte(stream, &productVersion->major);

    CDR_Stream_serialize_1Byte(stream, &productVersion->minor);

    CDR_Stream_serialize_1Byte(stream, &productVersion->release);

    CDR_Stream_serialize_1Byte(stream, &productVersion->revision);

    return RTI_TRUE;
}

RTI_BOOL
DPSE_Builtin_deserializeProductVersion(struct CDR_Stream_t *stream,
                                struct DDS_ProductVersion_t *productVersion,
                                void * param)
{
    CDR_Stream_deserialize_1Byte(stream, &productVersion->major);

    CDR_Stream_deserialize_1Byte(stream, &productVersion->minor);

    CDR_Stream_deserialize_1Byte(stream, &productVersion->release);

    CDR_Stream_deserialize_1Byte(stream, &productVersion->revision);

    return RTI_TRUE;
}

RTI_UINT32
DPSE_Builtin_getProductVersionMaxSizeSerialized(RTI_UINT32 size)
{
    RTI_UINT32 origSize = size;

    size += CDR_get1ByteMaxSizeSerialized(size);
    size += CDR_get1ByteMaxSizeSerialized(size);
    size += CDR_get1ByteMaxSizeSerialized(size);
    size += CDR_get1ByteMaxSizeSerialized(size);

    return (size - origSize);
}


/* ----------------------------------------------------------------- */

/* Entity Name */

RTI_BOOL
DPSE_Builtin_serializeEntityNameQosPolicy(struct CDR_Stream_t *stream,
                            const struct DDS_EntityNameQosPolicy *entityName,
                            void *param)
{
    if (entityName->name == NULL)
    {
        return RTI_TRUE;
    }

    if (!CDR_Stream_serialize_String(stream, entityName->name,
                                     MIG_RTPS_PATHNAME_LEN_MAX + 1))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
DPSE_Builtin_deserializeEntityNameQosPolicy(struct CDR_Stream_t *stream,
                            struct DDS_EntityNameQosPolicy *entityName,
                            void * param)
{

    /* Entity name preallocated and a fixed size */
    if (!CDR_Stream_deserialize_String(stream, entityName->name,
                                       MIG_RTPS_PATHNAME_LEN_MAX))
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}


RTI_UINT32
DPSE_Builtin_getEntityNameQosPolicyMaxSizeSerialized(unsigned int size)
{
    RTI_UINT32 origSize = size;

    size += CDR_get_maxSizeSerialized_String(size, MIG_RTPS_PATHNAME_LEN_MAX + 1);

    return (size - origSize);
}
