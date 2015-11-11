/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef cdr_encapsulation_h
#define cdr_encapsulation_h



#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifndef cdr_serialize_h
#include "cdr/cdr_serialize.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* OMG CDR Big Endian */
#define RTI_CDR_ENCAPSULATION_ID_CDR_BE     0x0000
/* OMG CDR Little Endian */
#define RTI_CDR_ENCAPSULATION_ID_CDR_LE     0x0001
/* Parameter List OMG CDR Big Endian */
#define RTI_CDR_ENCAPSULATION_ID_PL_CDR_BE  0x0002
/* Parameter List OMG CDR Little Endian */
#define RTI_CDR_ENCAPSULATION_ID_PL_CDR_LE  0x0003

#define RTI_CDR_ENCAPSULATION_OPTIONS_NONE  0x0000

#define RTI_CDR_ENCAPSULATION_HEADER_SIZE   4

typedef RTI_UINT16 NDDSCDREncapsulationId;
typedef RTI_UINT16 NDDSCDREncapsulationOptions;

typedef struct NDDSCDREncapsulation
{
    NDDSCDREncapsulationId identifier;
    NDDSCDREncapsulationOptions options;
} NDDSCDREncapsulation;



#define CDREncapsulation_validCdrEncapsulation(_id, _opts, _sopts) \
((_id) == RTI_CDR_ENCAPSULATION_ID_CDR_BE ? RTI_TRUE : \
(_id) == RTI_CDR_ENCAPSULATION_ID_CDR_LE)

#define CDREncapsulation_validParameterCdrEncapsulation(_id, _opts, _sopts) \
((_id) == RTI_CDR_ENCAPSULATION_ID_PL_CDR_BE ? RTI_TRUE : \
(_id) == RTI_CDR_ENCAPSULATION_ID_PL_CDR_LE)

CDRDllExport void
CDR_Stream_serialize_CdrEncapsulation(struct CDR_Stream_t *me,
                                    RTI_UINT16 *kind,
                                    RTI_UINT16 *options);

CDRDllExport void
CDR_Stream_deserialize_CdrEncapsulation(struct CDR_Stream_t *me,
                                      RTI_UINT16 *kind,
                                      RTI_UINT16 *options);

#define CDR_Stream_getEncapsulationSize(_size)             \
(_size) += CDR_get_maxSizeSerialized_UnsignedShort(_size);  \
(_size) += CDR_get_maxSizeSerialized_UnsignedShort(_size);

CDRDllExport RTI_BOOL
CDR_Stream_deserialize_header(struct CDR_Stream_t *stream);

CDRDllExport RTI_BOOL
CDR_Stream_serialize_header(struct CDR_Stream_t * stream,RTI_BOOL is_guid);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* cdr_encapsulation_h */
