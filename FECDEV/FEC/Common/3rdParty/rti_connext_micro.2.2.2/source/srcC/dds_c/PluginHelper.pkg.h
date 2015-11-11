/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef PluginHelper_pkg_h
#define PluginHelper_pkg_h

extern RTI_BOOL
PluginHelper_create_sample(struct NDDS_Type_Plugin *plugin,void **sample,void *param);

extern RTI_BOOL
PluginHelper_delete_sample(struct NDDS_Type_Plugin *plugin,void *sample,void *param);

extern RTI_BOOL
PluginHelper_copy_sample (struct NDDS_Type_Plugin *plugin,void *dst,const void *src,void *param);

extern RTI_BOOL
PluginHelper_get_sample(struct NDDS_Type_Plugin *plugin,void **sample,void *param);

extern RTI_BOOL
PluginHelper_return_sample(struct NDDS_Type_Plugin *plugin,void *sample,void *param);

extern RTI_BOOL
PluginHelper_get_buffer(struct NDDS_Type_Plugin *plugin,
                        struct REDA_Buffer **stream_buffer,
                        void *param);

extern RTI_BOOL
PluginHelper_return_buffer(struct NDDS_Type_Plugin *plugin,
                           struct REDA_Buffer *stream_buffer,
                           void *param);

extern RTI_BOOL
PluginHelper_on_participant_attached(struct NDDS_Type_Plugin *plugin,void *param);

extern RTI_BOOL
PluginHelper_on_participant_detached(struct NDDS_Type_Plugin *plugin,void *param);

extern RTI_BOOL
PluginHelper_on_endpoint_attached(struct NDDS_Type_Plugin *plugin,void *param);

extern RTI_BOOL
PluginHelper_on_endpoint_detached(struct NDDS_Type_Plugin *plugin,void *param);

extern RTI_BOOL
PluginHelper_instance_to_keyhash(struct NDDS_Type_Plugin *plugin,
                                 struct CDR_Stream_t *md5stream,
                                 DDS_KeyHash_t *keyHash,
                                 const void *instance,
                                 void *param);

extern NDDS_TypePluginKeyKind
PluginHelper_get_key_kind(struct NDDS_Type_Plugin *plugin,void *param);

#endif /* PluginHelper_pkg_h */
