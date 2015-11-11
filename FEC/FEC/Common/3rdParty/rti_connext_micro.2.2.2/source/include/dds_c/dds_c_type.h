/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_type_h
#define dds_c_type_h


#ifndef cdr_cdr_type_h
#include "cdr/cdr_cdr_type.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef cdr_encapsulation_h
#include "cdr/cdr_encapsulation.h"
#endif
#ifndef reda_buffer_h
#include "reda/reda_buffer.h"
#endif
#ifndef rtps_rtps_h
#include "rtps/rtps_rtps.h"
#endif
#ifndef dds_c_dll_h
#include "dds_c/dds_c_dll.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* ================================================================= */
/*                       Type Plugin Interface                       */
/* ================================================================= */
/*ce @ingroup DDSMicroTypesModule
 */
typedef struct DDS_TypeImpl DDS_Type;
struct NDDS_Type_Plugin;
typedef void * UserDataKeyHolder_t;

typedef struct NDDS_Type_PluginVersion {
    char majorRev;
    char minorRev;
} NDDS_Type_PluginVersion;

/*e \dref_TypePluginKeyKind
*/
typedef enum
{
    /*e \dref_TypePluginKeyKind_TYPEPLUGIN_NO_KEY
     */
    NDDS_TYPEPLUGIN_NO_KEY,

    /*i \dref_TypePluginKeyKind_TYPEPLUGIN_GUID_KEY
     */
    NDDS_TYPEPLUGIN_GUID_KEY,

    /*e \dref_TypePluginKeyKind_TYPEPLUGIN_USER_KEY
     */
    NDDS_TYPEPLUGIN_USER_KEY
} NDDS_TypePluginKeyKind;

/*ce \dref_NDDS_Type_GetSerializedSizeFunc
 */
typedef RTI_UINT32
(*NDDS_Type_GetSerializedSizeFunc)(
        struct NDDS_Type_Plugin *plugin,
        RTI_UINT32 current_alignment,
        void *param);

/*ce \dref_NDDS_Type_CreateSampleFunc
 */
typedef RTI_BOOL
(*NDDS_Type_CreateSampleFunc)(
        struct NDDS_Type_Plugin *plugin,
        void **sample,
        void *param);

/*ce \dref_NDDS_Type_DeleteSampleFunc
 */
typedef RTI_BOOL
(*NDDS_Type_DeleteSampleFunc) (
        struct NDDS_Type_Plugin *plugin,
        void *sample,
        void *param);

/*ce \dref_NDDS_Type_CopySampleFunc
 */
typedef RTI_BOOL
(*NDDS_Type_CopySampleFunc)(
        struct NDDS_Type_Plugin *plugin,
        void *dst,
        const void *src,
        void *param);

/*ce \dref_NDDS_Type_GetSampleFunc
 */
typedef RTI_BOOL
(*NDDS_Type_GetSampleFunc)(
        struct NDDS_Type_Plugin *plugin,
        void **sample,
        void *param);

/*ce \dref_NDDS_Type_ReturnSampleFunc
 */
typedef RTI_BOOL
(*NDDS_Type_ReturnSampleFunc) (
        struct NDDS_Type_Plugin *plugin,
        void *sample,
        void *param);

/*ce \dref_NDDS_Type_GetStreamBufferFunc
 */
typedef RTI_BOOL
(*NDDS_Type_GetStreamBufferFunc)(
        struct NDDS_Type_Plugin *plugin,
        struct REDA_Buffer **stream_buffer,
        void *param);

/*ce \dref_NDDS_Type_ReturnStreamBufferFunc
 */
typedef RTI_BOOL
(*NDDS_Type_ReturnStreamBufferFunc)(
        struct NDDS_Type_Plugin *plugin,
        struct REDA_Buffer *stream_buffer,
        void *param);


/*ce \dref_NDDS_PluginHelper_OnParticipantAttachedFunc
 */
typedef RTI_BOOL
(*NDDS_PluginHelper_OnParticipantAttachedFunc)(
        struct NDDS_Type_Plugin *plugin,
        void *param);

/*ce \dref_NDDS_PluginHelper_OnParticipantDetachedFunc
 */
typedef RTI_BOOL
(*NDDS_PluginHelper_OnParticipantDetachedFunc)(
        struct NDDS_Type_Plugin *plugin,
        void *param);

/*ce \dref_NDDS_PluginHelper_OnEndpointAttachedFunc
 */
typedef RTI_BOOL
(*NDDS_PluginHelper_OnEndpointAttachedFunc)(
        struct NDDS_Type_Plugin *plugin,
        void *param);

/*ce \dref_NDDS_PluginHelper_OnEndpointDetachedFunc
 */
typedef RTI_BOOL
(*NDDS_PluginHelper_OnEndpointDetachedFunc)(
        struct NDDS_Type_Plugin *plugin,
        void *param);

/*ce \dref_NDDS_Type_GetKeyKindFunc
 */
typedef NDDS_TypePluginKeyKind
(*NDDS_PluginHelper_GetKeyKindFunc)(
        struct NDDS_Type_Plugin *plugin,
        void *param);

/*ce \dref_NDDS_Type_InstanceToKeyHashFunc
 */
typedef RTI_BOOL
(*NDDS_PluginHelper_InstanceToKeyHashFunc)(
        struct NDDS_Type_Plugin *plugin,
        struct CDR_Stream_t *stream,
        DDS_KeyHash_t *keyHash,
        const void *instance,
        void *param);

/* **************************** end plugin function prototypes *********** */

/* anonymous declaration of TypeCode */

struct DDS_TypeCode;
typedef struct DDS_TypeCode DDS_TypeCode_t;

/*ce \dref_NDDS_Type_Plugin
 */
struct NDDS_Type_Plugin
{
    /* constants */

    /*ce \dref_NDDS_Type_Plugin_Version
     */
    NDDS_Type_PluginVersion version;

    /*ci \dref_NDDS_Type_Plugin_TypeCode
     */
    DDS_TypeCode_t* type_code;

    /*ci \dref_NDDS_Type_Plugin_encapsulationKind
     */
    NDDSCDREncapsulation* encapsulationKind;

    /*ce \dref_NDDS_Type_Plugin_KeyKind
     */
    NDDS_TypePluginKeyKind  key_kind;

    /* function pointers for type specifc functions */

    /*ce \dref_NDDS_Type_Plugin_serialize_data
     */
    CDR_Stream_SerializeFunction serialize_data;

    /*ce \dref_NDDS_Type_Plugin_deserialize_data
     */
    CDR_Stream_DeserializeFunction deserialize_data;

    /*ce \dref_NDDS_Type_Plugin_get_serialized_sample_max_size
     */
    NDDS_Type_GetSerializedSizeFunc get_serialized_sample_max_size;

    /*i \dref_NDDS_Type_Plugin_serialize_key
     */
    CDR_Stream_SerializeFunction serialize_key;

    /*i \dref_NDDS_Type_Plugin_deserialize_key
     */
    CDR_Stream_DeserializeFunction deserialize_key;

    /*ce \dref_NDDS_Type_Plugin_get_serializedKeyMaxSize
     */
    NDDS_Type_GetSerializedSizeFunc get_serialized_key_max_size;

    /*ce \dref_NDDS_Type_Plugin_create_sample
     */
    NDDS_Type_CreateSampleFunc create_sample;

    /*ce \dref_NDDS_Type_Plugin_delete_sample
     */
    NDDS_Type_DeleteSampleFunc delete_sample;

    /*ce \dref_NDDS_Type_Plugin_copy_sample
     */
    NDDS_Type_CopySampleFunc copy_sample;

    /*ce \dref_NDDS_Type_Plugin_get_sample
     */
    NDDS_Type_GetSampleFunc get_sample;

    /*ce \dref_NDDS_Type_Plugin_return_sample
     */
    NDDS_Type_ReturnSampleFunc return_sample;

    /*ce \dref_NDDS_Type_Plugin_get_buffer
     */
    NDDS_Type_GetStreamBufferFunc get_buffer;

    /*ce \dref_NDDS_Type_Plugin_return_buffer
     */
    NDDS_Type_ReturnStreamBufferFunc return_buffer;

    /* function pointers for common replaceable components */

    /*i \dref_NDDS_Type_Plugin_on_participant_attached
     */
    NDDS_PluginHelper_OnParticipantAttachedFunc on_participant_attached;

    /*i \dref_NDDS_Type_Plugin_on_participant_detached
     */
    NDDS_PluginHelper_OnParticipantDetachedFunc on_participant_detached;

    /*i \dref_NDDS_Type_Plugin_on_endpoint_attached
     */
    NDDS_PluginHelper_OnEndpointAttachedFunc on_endpoint_attached;

    /*i \dref_NDDS_Type_Plugin_on_endpoint_detached
     */
    NDDS_PluginHelper_OnEndpointDetachedFunc on_endpoint_detached;

    /*i \dref_NDDS_Type_Plugin_get_key_kind
     */
    NDDS_PluginHelper_GetKeyKindFunc get_key_kind;

    /*i \dref_NDDS_Type_Plugin_instance_to_keyhash
     */
    NDDS_PluginHelper_InstanceToKeyHashFunc instance_to_keyhash;
    
};

/* ================================================================= */
/*              Non-Type Specific Type Support Functions             */
/* ================================================================= */

/*ce \dref_PluginHelper_get_sample
 */
DDSCDllExport RTI_BOOL
PluginHelper_get_sample(struct NDDS_Type_Plugin *plugin,
                        void **sample,void *param);

/*ce \dref_PluginHelper_return_sample
 */
DDSCDllExport RTI_BOOL
PluginHelper_return_sample(struct NDDS_Type_Plugin *plugin,
                           void *sample,void *param);

/*ce \dref_PluginHelper_get_buffer
 */
DDSCDllExport RTI_BOOL
PluginHelper_get_buffer(struct NDDS_Type_Plugin *plugin,
                        struct REDA_Buffer **stream_buffer,
                        void *param);

/*ce \dref_PluginHelper_return_buffer
 */
DDSCDllExport RTI_BOOL
PluginHelper_return_buffer(struct NDDS_Type_Plugin *plugin,
                           struct REDA_Buffer *stream_buffer,
                           void *param);

/*ce \dref_PluginHelper_on_participant_attached
 */
DDSCDllExport RTI_BOOL
PluginHelper_on_participant_attached(struct NDDS_Type_Plugin *plugin,void *param);

/*ce \dref_PluginHelper_on_participant_detached
 */
DDSCDllExport RTI_BOOL
PluginHelper_on_participant_detached(struct NDDS_Type_Plugin *plugin,void *param);

/*ce \dref_PluginHelper_on_endpoint_attached
 */
DDSCDllExport RTI_BOOL
PluginHelper_on_endpoint_attached(struct NDDS_Type_Plugin *plugin,void *param);

/*ce \dref_PluginHelper_on_endpoint_detached
 */
DDSCDllExport RTI_BOOL
PluginHelper_on_endpoint_detached(struct NDDS_Type_Plugin *plugin,void *param);

/*ce \dref_PluginHelper_instance_to_keyhash
 */
DDSCDllExport RTI_BOOL
PluginHelper_instance_to_keyhash(struct NDDS_Type_Plugin *plugin,
                                 struct CDR_Stream_t *stream,
                                 DDS_KeyHash_t *keyHash,
                                 const void *instance,
                                 void *param);

/*ce \dref_PluginHelper_get_key_kind
 */
DDSCDllExport NDDS_TypePluginKeyKind
PluginHelper_get_key_kind(struct NDDS_Type_Plugin *plugin,void *param);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* dds_c_type_h */
