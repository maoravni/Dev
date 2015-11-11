/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef cdr_stream_h
#define cdr_stream_h



#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifndef cdr_dll_h
#include "cdr/cdr_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define CDR_MAX_ALIGNMENT     8

/*e \ingroup RTICdrStreamModule
  Big endian in CDR.
*/
#define RTI_CDR_ENDIAN_BIG (0)

/*e \ingroup RTICdrStreamModule
  Little endian in CDR
*/
#define RTI_CDR_ENDIAN_LITTLE (1)


#define CDR_BYTESWAP_INVALID  -1

/* ----------------------------------------------------------------- */

/* TODO: this must be better defined - user needs to work with this struct */
struct CDR_Stream_t
{
    char *realbuff;             /* allocated buffer */
    char *buffer;               /* buffer aligned on 8-byte boundry */
    char *alignBase;            /* address used for alignment calculations */
    char *buffptr;              /* current location in buffer */
    RTI_UINT32 length;          /* length of the aligned buffer */
    RTI_BOOL needbyteswap;
    RTI_UINT8 endian;
};

/* CDR_Stream_Align requires cdrs to be a valid stream pointer */
#define CDR_Stream_Align(cdrs, align) \
    ((cdrs) ? (((cdrs)->buffptr = (cdrs)->alignBase + \
     ((((cdrs)->buffptr-(cdrs)->alignBase) + ((align) - 1)) & ~((align) - 1))), \
      RTI_TRUE) : RTI_FALSE)

/* CDR_Stream_reset_alignment requires cdrs to be a valid stream pointer */
#define CDR_Stream_reset_alignment(cdrs) \
    (cdrs)->alignBase = (cdrs)->buffptr

CDRDllExport void
CDR_Stream_Free(struct CDR_Stream_t *cdrs);

CDRDllExport struct CDR_Stream_t*
CDR_Stream_Alloc(RTI_UINT32 buffsize);

CDRDllExport void
CDR_Stream_Reset(struct CDR_Stream_t *cdrs);

CDRDllExport RTI_BOOL
CDR_Stream_ByteswapSet(struct CDR_Stream_t *cdrs, RTI_BOOL littleEndian);

CDRDllExport RTI_UINT32
CDR_Stream_GetCurrentPositionOffset(struct CDR_Stream_t *cdrs);

CDRDllExport RTI_BOOL
CDR_Stream_SetCurrentPositionOffset(struct CDR_Stream_t *cdrs,RTI_UINT32 num);

CDRDllExport RTI_BOOL
CDR_Stream_IncrementCurrentPosition(struct CDR_Stream_t *me,RTI_INT32 amount);

CDRDllExport RTI_BOOL
CDR_Stream_SetBuffer(struct CDR_Stream_t *me,char *buf, RTI_UINT32 length);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* cdr_stream_h */
