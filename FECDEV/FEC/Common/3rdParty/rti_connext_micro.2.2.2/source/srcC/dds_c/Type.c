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
#ifndef dds_c_type_h
#include "dds_c/dds_c_type.h"
#endif

struct DDS_TypeImpl
{
    char name[MIG_RTPS_PATHNAME_LEN_MAX];
    struct NDDS_Type_Plugin *plugin;
    DDS_Long ref_count;
};

/*** SOURCE_BEGIN ***/

RTI_INT32
DDS_TypeImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
        const DB_Record_T op1, void *op2, void *op3)
{
    struct DDS_TypeImpl *record_left = (struct DDS_TypeImpl*)op1;
    const char *name_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        name_right = (const char*)op2;
    }
    else
    {
        name_right = ((struct DDS_TypeImpl*)op2)->name;
    }

    return strcmp(record_left->name,name_right);
}

DDS_Boolean
DDS_TypeImpl_initalize(struct DDS_TypeImpl *type,
                        const char *type_name,
                        struct NDDS_Type_Plugin *plugin)
{

    type->plugin = plugin;
    type->ref_count = 0;
    OSAPI_Memory_copy(type->name,type_name,OSAPI_String_length(type_name)+1);

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_TypeImpl_finalize(struct DDS_TypeImpl *type)
{

    type->plugin = NULL;
    type->ref_count = 0;

    return DDS_BOOLEAN_TRUE;
}

void
DDS_TypeImpl_ref_plus(struct DDS_TypeImpl *type)
{
    ++type->ref_count;
}

void
DDS_TypeImpl_ref_minus(struct DDS_TypeImpl *type)
{
    --type->ref_count;
}

DDS_Long
DDS_TypeImpl_ref_count(struct DDS_TypeImpl *type)
{
    return type->ref_count;
}

struct NDDS_Type_Plugin*
DDS_TypeImpl_get_plugin(DDS_Type *self)
{
    struct DDS_TypeImpl *type = (struct DDS_TypeImpl *)self;

    LOG_testPrecondition(type == NULL, return NULL);

    return type->plugin;
}

const char*
DDS_TypeImpl_get_type_name_reference(DDS_Type *self)
{
    struct DDS_TypeImpl *type = (struct DDS_TypeImpl *)self;

    LOG_testPrecondition(type == NULL, return NULL);

    return type->name;
}

DDS_Boolean
NDDS_TypePlugin_is_valid(struct NDDS_Type_Plugin *type_plugin)
{
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;

    if (type_plugin->get_serialized_sample_max_size == NULL)
    {
        DDSC_Log_error(LOG_GET_SERIALIZED_SAMPLE_MAX_SIZE_CALLBACK_IS_NULL);
        goto done;
    }

    if (type_plugin->serialize_data == NULL)
    {
        DDSC_Log_error(LOG_SERIALIZE_DATA_CALLBACK_IS_NULL);
        goto done;
    }

    if (type_plugin->deserialize_data == NULL)
    {
        DDSC_Log_error(LOG_DESERIALIZE_DATA_CALLBACK_IS_NULL);
        goto done;
    }

    if (type_plugin->create_sample == NULL)
    {
        DDSC_Log_error(LOG_CREATE_SAMPLE_CALLBACK_IS_NULL);
        goto done;
    }

    if (type_plugin->copy_sample == NULL)
    {
        DDSC_Log_error(LOG_COPY_SAMPLE_CALLBACK_IS_NULL);
        goto done;
    }

    if (type_plugin->delete_sample == NULL)
    {
        DDSC_Log_error(LOG_DELETE_SAMPLE_CALLBACK_IS_NULL);
        goto done;
    }

    if (type_plugin->get_key_kind == NULL)
    {
        DDSC_Log_error(LOG_GET_KEY_KIND_CALLBACK_IS_NULL);
        goto done;
    }

    if (type_plugin->instance_to_keyhash == NULL)
    {
        DDSC_Log_error(LOG_GET_KEY_KIND_CALLBACK_IS_NULL);
        goto done;
    }

    retval = DDS_BOOLEAN_TRUE;

done:

    return retval;
}


