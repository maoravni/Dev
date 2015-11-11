/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DPSECdr_pkg_h
#define DPSECdr_pkg_h


/*e \ingroup DISCRtpsPidModule
  Used for field Participant::productVersion
 */
#define DISC_RTPS_PID_PRODUCT_VERSION                            (0x8000)

#define DISC_RTPS_PID_ENTITY_NAME   MIG_RTPS_PID_ENTITY_NAME


extern RTI_BOOL
DPSE_Builtin_serializeProductVersion(struct CDR_Stream_t *stream,
                         const struct DDS_ProductVersion_t *productVersion,
                         void *param);

extern RTI_BOOL
DPSE_Builtin_deserializeProductVersion(struct CDR_Stream_t *stream,
                                struct DDS_ProductVersion_t *productVersion,
                                void * param);

extern RTI_UINT32
DPSE_Builtin_getProductVersionMaxSizeSerialized(RTI_UINT32 size);

extern RTI_BOOL
DPSE_Builtin_serializeEntityNameQosPolicy(struct CDR_Stream_t *stream,
                            const struct DDS_EntityNameQosPolicy *entityName,
                            void *param);

extern RTI_BOOL
DPSE_Builtin_deserializeEntityNameQosPolicy(struct CDR_Stream_t *stream,
                            struct DDS_EntityNameQosPolicy *entityName,
                            void * param);

extern RTI_UINT32
DPSE_Builtin_getEntityNameQosPolicyMaxSizeSerialized(unsigned int size);

#endif /* DPSECdr_pkg_h */
