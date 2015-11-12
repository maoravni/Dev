/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef PublicationBuiltinTopicDataPlugin_pkg_h
#define PublicationBuiltinTopicDataPlugin_pkg_h

extern RTI_BOOL
DPDE_PublicationBuiltinTopicDataTypePlugin_create_sample(
                                            struct NDDS_Type_Plugin *plugin,
                                            void **sample,
                                            void *param);

extern RTI_BOOL
DPDE_PublicationBuiltinTopicDataTypePlugin_delete_sample(
                                            struct NDDS_Type_Plugin *plugin,
                                            void *sample,
                                            void *param);

extern RTI_BOOL
DPDE_PublicationBuiltinTopicDataTypePlugin_copy_sample(
        struct NDDS_Type_Plugin *type,
        void *dst,
        const void *src,
        void *param);

struct NDDS_Type_Plugin*
DPDE_PublicationBuiltinTopicDataTypePlugin_get();

#endif /* PublicationBuiltinTopicDataPlugin_pkg_h */
