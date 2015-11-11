/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef cdr_md5_h
#define cdr_md5_h



#ifndef cdr_dll_h
#include "cdr/cdr_dll.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

CDRDllExport void
NDDSCDR_Stream_computeMD5(struct CDR_Stream_t *me,RTI_UINT8 * out);

#ifdef __cplusplus
}                               /* end extern "C" */
#endif


#endif /* cdr_md5_h */
