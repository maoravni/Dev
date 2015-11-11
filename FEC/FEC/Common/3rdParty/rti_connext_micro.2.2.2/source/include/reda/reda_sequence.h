/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_sequence_h
#define reda_sequence_h


#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define REDA_DEFINE_SEQUENCE_STRUCT(_TSeq,_T) \
struct _TSeq {\
_T* _contiguous_buffer;\
RTI_INT32 _maximum;\
RTI_INT32 _length;\
RTI_INT32 _element_size;\
RTI_UINT8 _flags;\
};

#define REDA_DEFINE_SEQUENCE_IN_C(TSeq, T) \
REDADllExport                                        \
RTI_BOOL TSeq ## _initialize(struct TSeq* self);                    \
                                                                       \
REDADllExport                                        \
RTI_BOOL TSeq ## _finalize(struct TSeq* self);                      \
                                                                       \
REDADllExport                                        \
RTI_INT32 TSeq ## _get_maximum(const struct TSeq* self);                \
                                                                       \
REDADllExport                                        \
RTI_BOOL TSeq ## _set_maximum(struct TSeq* self, RTI_INT32 new_max); \
                                                                       \
REDADllExport                                        \
RTI_INT32 TSeq ## _get_length(const struct TSeq *self);                 \
                                                                       \
REDADllExport                                        \
RTI_BOOL TSeq ## _set_length(struct TSeq *self, RTI_INT32 new_length);\
                                                                       \
REDADllExport                                        \
T* TSeq ## _get_reference(const struct TSeq* self, RTI_INT32 i);        \
                                                                       \
REDADllExport                                        \
struct TSeq* TSeq ## _copy(struct TSeq* self, const struct TSeq* src); \
                                                                       \
REDADllExport                                        \
RTI_BOOL TSeq ## _is_equal(const struct TSeq* self, const struct TSeq* src); \
                                                                       \
REDADllExport                                         \
RTI_BOOL TSeq ## _loan_contiguous(struct TSeq* self, void *buffer,         \
                                  RTI_INT32 new_length, RTI_INT32 new_max); \
                                                                           \
REDADllExport                                         \
RTI_BOOL TSeq ## _loan_discontiguous(struct TSeq* self, void *buffer,         \
                                  RTI_INT32 new_length, RTI_INT32 new_max); \
                                                                           \
REDADllExport \
RTI_BOOL TSeq ## _unloan(struct TSeq *self); \
\
REDADllExport                                             \
RTI_BOOL TSeq ## _has_ownership(const struct TSeq* self); \
                                                          \
REDADllExport                                             \
T* TSeq ## _get_buffer(const struct TSeq* self); \
\
REDADllExport                                             \
RTI_BOOL TSeq ## _set_buffer(struct TSeq* self, T *buffer); \
\
REDADllExport \
RTI_BOOL TSeq ## _has_discontiguous_buffer(const struct TSeq* self);


#define REDA_DEFINE_SEQUENCE(_TSeq,_T) \
REDA_DEFINE_SEQUENCE_STRUCT(_TSeq,_T) \
REDA_DEFINE_SEQUENCE_IN_C(_TSeq,_T)

#define REDA_DEFINE_SEQUENCE_INITIALIZER(t_) \
{\
NULL,0,0,sizeof(t_),0 \
}

struct REDA_Sequence;

/* Set when sequence is using a loaned buffer */
#define REDA_SEQUENCE_FLAG_LOAN  0x01

/* Set when buffer contains pointers to elements */
#define REDA_SEQUENCE_FLAG_DISCONTIGUOUS   0x02

REDADllExport RTI_BOOL
REDA_Sequence_initialize(struct REDA_Sequence *self, RTI_INT32 element_size);

REDADllExport RTI_BOOL
REDA_Sequence_finalize(struct REDA_Sequence *self);

REDADllExport RTI_INT32
REDA_Sequence_get_maximum(const struct REDA_Sequence *self);

REDADllExport RTI_BOOL
REDA_Sequence_set_maximum(struct REDA_Sequence *self, RTI_INT32 new_max,
                          RTI_BOOL copy_content);

REDADllExport RTI_INT32
REDA_Sequence_get_length(const struct REDA_Sequence *self);

REDADllExport RTI_BOOL
REDA_Sequence_set_length(struct REDA_Sequence *self, RTI_INT32 new_length);

REDADllExport void *
REDA_Sequence_get_buffer(const struct REDA_Sequence *self);

REDADllExport RTI_BOOL
REDA_Sequence_set_buffer(struct REDA_Sequence *self, void *buffer);

REDADllExport void *
REDA_Sequence_get_reference(const struct REDA_Sequence *self,RTI_INT32 i);

REDADllExport struct REDA_Sequence*
REDA_Sequence_copy(struct REDA_Sequence *self,
                   const struct REDA_Sequence *src,
                   RTI_BOOL copy_content);

REDADllExport RTI_BOOL
REDA_Sequence_is_equal(const struct REDA_Sequence *left,
                       const struct REDA_Sequence *right,
                       RTI_BOOL compare_content);

REDADllExport RTI_BOOL
REDA_Sequence_loan_contiguous(struct REDA_Sequence *self, void *buffer,
                              RTI_INT32 new_length, RTI_INT32 new_max);

REDADllExport RTI_BOOL
REDA_Sequence_loan_discontiguous(struct REDA_Sequence *self, void *buffer,
                                 RTI_INT32 new_length, RTI_INT32 new_max);

REDADllExport RTI_BOOL
REDA_Sequence_unloan(struct REDA_Sequence *self);

REDADllExport RTI_BOOL
REDA_Sequence_has_ownership(const struct REDA_Sequence *self);

REDADllExport RTI_BOOL
REDA_Sequence_has_discontiguous_buffer(const struct REDA_Sequence *self);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* reda_sequence_h */
