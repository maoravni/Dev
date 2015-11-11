/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef Type_pkg_h
#define Type_pkg_h

struct DDS_TypeImpl
{
    char name[MIG_RTPS_PATHNAME_LEN_MAX];
    struct NDDS_Type_Plugin *plugin;
    DDS_Long ref_count;
};

extern RTI_INT32
DDS_TypeImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
        const DB_Record_T op1, void *op2, void *op3);

extern DDS_Boolean
DDS_TypeImpl_initalize(struct DDS_TypeImpl *type,
                        const char *type_name,
                        struct NDDS_Type_Plugin *plugin);

extern DDS_Boolean
DDS_TypeImpl_finalize(struct DDS_TypeImpl *type);

extern void
DDS_TypeImpl_ref_plus(struct DDS_TypeImpl *type);

extern void
DDS_TypeImpl_ref_minus(struct DDS_TypeImpl *type);

extern DDS_Long
DDS_TypeImpl_ref_count(struct DDS_TypeImpl *type);

struct NDDS_Type_Plugin*
DDS_TypeImpl_get_plugin(DDS_Type *self);

extern const char*
DDS_TypeImpl_get_type_name_reference(DDS_Type *self);

extern DDS_Boolean
NDDS_TypePlugin_is_valid(struct NDDS_Type_Plugin *type_plugin);

#endif /* Type_pkg_h */
