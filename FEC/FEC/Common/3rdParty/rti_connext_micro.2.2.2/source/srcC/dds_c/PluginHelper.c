/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef cdr_cdr_type_h
#include "cdr/cdr_cdr_type.h"
#endif
#ifndef cdr_serialize_h
#include "cdr/cdr_serialize.h"
#endif
#ifndef cdr_md5_h
#include "cdr/cdr_md5.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif 
#ifndef dds_c_type_h
#include "dds_c/dds_c_type.h"
#endif

/*** SOURCE_BEGIN ***/

#if 0
RTI_BOOL
PluginHelper_create_sample(struct NDDS_Type_Plugin *plugin,void **sample,void *param)
{
};

RTI_BOOL
PluginHelper_delete_sample(struct NDDS_Type_Plugin *plugin,void *sample,void *param)
{
};

RTI_BOOL
PluginHelper_copy_sample (struct NDDS_Type_Plugin *plugin,void *dst,const void *src,void *param)
{
};
#endif
RTI_BOOL
PluginHelper_get_sample(struct NDDS_Type_Plugin *plugin,void **sample,void *param)
{
    return RTI_TRUE;
};

RTI_BOOL
PluginHelper_return_sample(struct NDDS_Type_Plugin *plugin,void *sample,void *param)
{
    return RTI_TRUE;
};

RTI_BOOL
PluginHelper_get_buffer(struct NDDS_Type_Plugin *plugin,
                        struct REDA_Buffer **stream_buffer,
                        void *param)
{
    return RTI_TRUE;
};

RTI_BOOL
PluginHelper_return_buffer(struct NDDS_Type_Plugin *plugin,
                           struct REDA_Buffer *stream_buffer,
                           void *param)
{
    return RTI_TRUE;
};

RTI_BOOL
PluginHelper_on_participant_attached(struct NDDS_Type_Plugin *plugin,void *param)
{
    return RTI_TRUE;
};

RTI_BOOL
PluginHelper_on_participant_detached(struct NDDS_Type_Plugin *plugin,void *param)
{
    return RTI_TRUE;
};

RTI_BOOL
PluginHelper_on_endpoint_attached(struct NDDS_Type_Plugin *plugin,void *param)
{
    return RTI_TRUE;
};

RTI_BOOL
PluginHelper_on_endpoint_detached(struct NDDS_Type_Plugin *plugin,void *param)
{
    return RTI_TRUE;
};

RTI_BOOL
PluginHelper_instance_to_keyhash(struct NDDS_Type_Plugin *plugin,
                                 struct CDR_Stream_t *md5stream,
                                 DDS_KeyHash_t *keyHash,
                                 const void *instance,
                                 void *param)
{
    RTI_UINT32 serializied_key_max_size;
#ifdef RTI_ENDIAN_LITTLE
    RTI_BOOL stream_needbyteswap;
#endif

    /* verify key support exists for this plugin */
    if ((plugin->key_kind == NDDS_TYPEPLUGIN_NO_KEY) ||
        (plugin->serialize_key == NULL) ||
        (plugin->get_serialized_key_max_size == NULL)) 
    {
        return RTI_FALSE;
    }
    /* keyhash includes potential padding, so insure key stream is zero */
    serializied_key_max_size =
        plugin->get_serialized_key_max_size(plugin,0,NULL);
    /* insure sufficient room in stream for key */
    if (serializied_key_max_size > md5stream->length)
    {
        return RTI_FALSE;
    }
    OSAPI_Memory_zero(md5stream->buffer, serializied_key_max_size);
    /* keyhash is always calculated from Big Endian serialization,
       so if we are little endian we need to set byte swap */
#ifdef RTI_ENDIAN_LITTLE
    stream_needbyteswap = md5stream->needbyteswap;
    md5stream->needbyteswap = RTI_TRUE;
#endif
    CDR_Stream_Reset(md5stream);
    if (!plugin->serialize_key(md5stream, instance, param)) 
    {
#ifdef RTI_ENDIAN_LITTLE
        md5stream->needbyteswap = stream_needbyteswap;
#endif
        return RTI_FALSE;
    }
    if (serializied_key_max_size > (unsigned int)(MIG_RTPS_KEY_HASH_MAX_LENGTH)) 
    {
        NDDSCDR_Stream_computeMD5(md5stream, keyHash->value);
    } 
    else 
    {
        /* sanity check to insure serialized key hasn't exceeded max size */
        if (CDR_Stream_GetCurrentPositionOffset(md5stream) >
            (unsigned int)(MIG_RTPS_KEY_HASH_MAX_LENGTH)) 
        {
            return RTI_FALSE;
        }
        OSAPI_Memory_zero(keyHash->value,MIG_RTPS_KEY_HASH_MAX_LENGTH);
        OSAPI_Memory_copy(keyHash->value, md5stream->buffer,
                         CDR_Stream_GetCurrentPositionOffset(md5stream));
    }
    keyHash->length = MIG_RTPS_KEY_HASH_MAX_LENGTH;
#ifdef RTI_ENDIAN_LITTLE
        md5stream->needbyteswap = stream_needbyteswap;
#endif
    return RTI_TRUE;
};


NDDS_TypePluginKeyKind
PluginHelper_get_key_kind(struct NDDS_Type_Plugin *plugin,void *param)
{
    return plugin->key_kind;
};


