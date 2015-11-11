/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_data_h
#define dds_c_data_h


  #ifndef dds_c_common_h
    #include "dds_c/dds_c_common.h"
  #endif
  #ifndef dds_c_infrastructure_h
    #include "dds_c/dds_c_infrastructure.h"
  #endif
  #ifndef dds_c_topic_h
    #include "dds_c/dds_c_topic.h"
  #endif
  #ifndef dds_c_publication_h
    #include "dds_c/dds_c_publication.h"
  #endif
  #ifndef dds_c_subscription_h
    #include "dds_c/dds_c_subscription.h"
  #endif
  #ifndef dds_c_domain_h
    #include "dds_c/dds_c_domain.h"
  #endif

#ifdef __cplusplus
    extern "C" {
#endif


#ifndef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif


#define DDS_DATAWRITER_C(TDataWriter, TData)                              \
    typedef struct TDataWriter TDataWriter;                               \
    NDDSUSERDllExport DDSCDllExport TDataWriter* TDataWriter ## _narrow(  \
        DDS_DataWriter* writer);                                          \
    NDDSUSERDllExport DDSCDllExport DDS_DataWriter* TDataWriter ## _as_datawriter( \
        TDataWriter* writer);                                             \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_InstanceHandle_t TDataWriter ## _register_instance(               \
        TDataWriter* self, const TData* instance_data);                   \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_InstanceHandle_t TDataWriter ## _register_instance_w_timestamp(   \
        TDataWriter* self, const TData* instance_data,                    \
        const struct DDS_Time_t* source_timestamp);                       \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_ReturnCode_t TDataWriter ## _unregister_instance(                 \
        TDataWriter* self, const TData* instance_data,                    \
        const DDS_InstanceHandle_t* handle);                              \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_ReturnCode_t TDataWriter ## _unregister_instance_w_timestamp(     \
        TDataWriter* self, const TData* instance_data,                    \
        const DDS_InstanceHandle_t* handle,                               \
        const struct DDS_Time_t* source_timestamp);                       \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_ReturnCode_t TDataWriter ## _write(                               \
        TDataWriter* self, const TData* instance_data,                    \
        const DDS_InstanceHandle_t* handle);                              \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_ReturnCode_t TDataWriter ## _write_w_timestamp(                   \
        TDataWriter* self, const TData* instance_data,                    \
        const DDS_InstanceHandle_t* handle,                               \
        const struct DDS_Time_t* source_timestamp);                       \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_ReturnCode_t TDataWriter ## _write_w_params(                      \
        TDataWriter* self, const TData* instance_data,                    \
        struct DDS_WriteParams_t* params);                          \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_ReturnCode_t TDataWriter ## _dispose(                             \
        TDataWriter* self, const TData* instance_data,                    \
        const DDS_InstanceHandle_t* instance_handle);                     \
    NDDSUSERDllExport DDSCDllExport                                       \
    DDS_ReturnCode_t TDataWriter ## _dispose_w_timestamp(                 \
        TDataWriter* self, const TData* instance_data,                    \
        const DDS_InstanceHandle_t* instance_handle,                      \
        const struct DDS_Time_t* source_timestamp)                       
        


#define DDS_DATAREADER_C(TDataReader, TDataSeq, TData)                       \
    typedef struct TDataReader TDataReader;                                  \
                                                                             \
    NDDSUSERDllExport DDSCDllExport TDataReader* TDataReader ## _narrow(     \
        DDS_DataReader* reader);                                             \
    NDDSUSERDllExport DDSCDllExport DDS_DataReader* TDataReader ## _as_datareader(   \
        TDataReader* reader);                                                \
    NDDSUSERDllExport DDSCDllExport                                          \
    DDS_ReturnCode_t TDataReader ## _read(TDataReader* self,                 \
        struct TDataSeq* received_data, struct DDS_SampleInfoSeq* info_seq,  \
        DDS_Long max_samples,                                                    \
        DDS_SampleStateMask sample_states, DDS_ViewStateMask view_states,    \
        DDS_InstanceStateMask instance_states);                              \
                                                                             \
    NDDSUSERDllExport DDSCDllExport                                          \
    DDS_ReturnCode_t TDataReader ## _take(TDataReader* self,                 \
        struct TDataSeq* received_data, struct DDS_SampleInfoSeq* info_seq,  \
        DDS_Long max_samples,                                                    \
        DDS_SampleStateMask sample_mask, DDS_ViewStateMask view_mask,        \
        DDS_InstanceStateMask instance_mask);                                \
                                                                             \
    NDDSUSERDllExport DDSCDllExport                                          \
    DDS_ReturnCode_t TDataReader ## _read_next_sample(                       \
        TDataReader* self, TData* received_data,                             \
        struct DDS_SampleInfo* sample_info);                                 \
                                                                             \
    NDDSUSERDllExport DDSCDllExport                                          \
    DDS_ReturnCode_t TDataReader ## _take_next_sample(                       \
        TDataReader* self, TData* received_data,                             \
        struct DDS_SampleInfo* sample_info);                                 \
                                                                             \
    NDDSUSERDllExport DDSCDllExport                                          \
    DDS_ReturnCode_t TDataReader ## _read_instance(                          \
        TDataReader* self,                                                   \
        struct TDataSeq* received_data, struct DDS_SampleInfoSeq* info_seq,  \
        DDS_Long max_samples,                                                    \
        const DDS_InstanceHandle_t* a_handle,                                \
        DDS_SampleStateMask sample_states, DDS_ViewStateMask view_states,    \
        DDS_InstanceStateMask instance_states);                              \
                                                                             \
    NDDSUSERDllExport DDSCDllExport                                          \
    DDS_ReturnCode_t TDataReader ## _take_instance(                          \
        TDataReader* self,                                                   \
        struct TDataSeq* received_data, struct DDS_SampleInfoSeq* info_seq,  \
        DDS_Long max_samples,                                                    \
        const DDS_InstanceHandle_t* a_handle,                                \
        DDS_SampleStateMask sample_states, DDS_ViewStateMask view_states,    \
        DDS_InstanceStateMask instance_states);                              \
                                                                             \
    NDDSUSERDllExport DDSCDllExport                                          \
    DDS_ReturnCode_t TDataReader ## _return_loan(                            \
        TDataReader* self,                                                   \
        struct TDataSeq* received_data, struct DDS_SampleInfoSeq* info_seq); \
                                                                                \
    NDDSUSERDllExport DDSCDllExport                                   \
    DDS_InstanceHandle_t TDataReader ## _lookup_instance(                        \
        TDataReader* self,                                                   \
        const TData* key_holder)
                                                                             
        


#ifdef __cplusplus
    }	/* extern "C" */
#endif

#endif /* dds_c_data_h */
