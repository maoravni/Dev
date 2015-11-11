/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef ParticipantBuiltinTopicDataPlugin_pkg_h
#define ParticipantBuiltinTopicDataPlugin_pkg_h

extern RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_create_sample(
                                                struct NDDS_Type_Plugin* plugin,
                                                void **sample,
                                                void *param);

extern RTI_BOOL
DPDE_ParticipantBuiltinTopicDataTypePlugin_delete_sample(
                                            struct NDDS_Type_Plugin *plugin,
                                            void *sample,
                                            void *param);

struct NDDS_Type_Plugin*
DPDE_ParticipantBuiltinTopicDataTypePlugin_get();

#endif /* ParticipantBuiltinTopicDataPlugin_pkg_h */
