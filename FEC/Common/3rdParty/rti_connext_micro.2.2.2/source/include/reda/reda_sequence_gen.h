/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_sequence_gen_h
#define reda_sequence_gen_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif

#define concatenate(A, B)   A ## B

#ifndef T
#define T RTI_UINT32
#endif

#if defined(T_copy) && !defined(T_initialize)
#error "T_copy defined without T_initialize"
#endif

#if defined(T_compare) && !defined(T_initialize)
#error "T_compare defined without T_initialize"
#endif

#if defined(T_initialize) || defined(T_finalize) || defined(T_copy)
#ifndef TSeq_map
#define _map(TSeq) concatenate(TSeq, _map)
#define TSeq_map _map(TSeq)
RTI_BOOL
TSeq_map(struct TSeq *seq,RTI_BOOL(*map)(T*),RTI_INT32 begin, RTI_INT32 end)
{
    RTI_INT32 i;

    for (i = begin; i <= end; i++)
    {
        if (!map(&seq->_contiguous_buffer[i]))
        {
            return RTI_FALSE;
        }
    }

    return RTI_TRUE;
}
#endif /* TSeq_map */

#endif

#ifndef TSeq_initialize
#define _initialize(TSeq) concatenate(TSeq, _initialize)
#define TSeq_initialize _initialize(TSeq)
RTI_BOOL
TSeq_initialize(struct TSeq *self)
{
#ifdef T_initialize
    RTI_BOOL result;
    result = REDA_Sequence_initialize((struct REDA_Sequence*)self, sizeof(T));
    if (!result)
    {
        return result;
    }
    return TSeq_map(self,T_initialize,0,self->_maximum-1);
#else
    return REDA_Sequence_initialize((struct REDA_Sequence*)self, sizeof(T));
#endif

#undef _initialize
#undef TSeq_initialize
}
#endif /* TSeq_initialize */

#ifndef TSeq_finalize
#define _finalize(TSeq) concatenate(TSeq, _finalize)
#define TSeq_finalize _finalize(TSeq)
RTI_BOOL
TSeq_finalize(struct TSeq *self)
{
#ifdef T_finalize
    RTI_BOOL result;
    result = TSeq_map(self,T_finalize,0,self->_maximum-1);
    if (!result)
    {
        return result;
    }
    return REDA_Sequence_finalize((struct REDA_Sequence*)self);
#else
    return REDA_Sequence_finalize((struct REDA_Sequence*)self);
#endif

#undef _finalize
#undef TSeq_finalize
}
#endif /* TSeq_finalize */

#ifndef TSeq_get_maximum
#define _get_maximum(TSeq) concatenate(TSeq, _get_maximum)
#define TSeq_get_maximum _get_maximum(TSeq)
RTI_INT32
TSeq_get_maximum(const struct TSeq *self)
{
    return REDA_Sequence_get_maximum((const struct REDA_Sequence *)self);

#undef _get_maximum
#undef TSeq_get_maximum
}
#endif /* TSeq_get_maximum */

#ifndef TSeq_set_maximum
#define _set_maximum(TSeq) concatenate(TSeq, _set_maximum)
#define TSeq_set_maximum _set_maximum(TSeq)
RTI_BOOL
TSeq_set_maximum(struct TSeq *self, RTI_INT32 new_max)
{
#ifdef T_initialize
    struct TSeq seq_copy;
    RTI_BOOL result;
    RTI_INT32 i;

    LOG_PRECONDITION(self == NULL,return RTI_FALSE)

    seq_copy = *self;

    result = REDA_Sequence_set_maximum((struct REDA_Sequence*)self,
                                        new_max,RTI_FALSE);
    if (!result)
    {
        return result;
    }

    result = TSeq_map(self,T_initialize,0,self->_maximum-1);
    if (!result)
    {
        return result;
    }

    if (self->_length > 0)
    {
        for (i = 0; i < seq_copy._length; ++i)
        {
            if (!T_copy(&self->_contiguous_buffer[i],
                        (const T*) &seq_copy._contiguous_buffer[i]))
            {
                return RTI_FALSE;
            }
        }
    }

    if (seq_copy._contiguous_buffer != NULL)
    {
        result = TSeq_map(&seq_copy,T_finalize,0,seq_copy._maximum-1);
        OSAPI_Heap_freeBuffer(seq_copy._contiguous_buffer);
    }
    return result;
#else
    return REDA_Sequence_set_maximum((struct REDA_Sequence*)self, new_max,RTI_TRUE);
#endif

#undef _set_maximum
#undef TSeq_set_maximum
}
#endif /* TSeq_set_maximum */

#ifndef TSeq_get_length
#define _get_length(TSeq) concatenate(TSeq, _get_length)
#define TSeq_get_length _get_length(TSeq)
RTI_INT32
TSeq_get_length(const struct TSeq *self)
{
    return REDA_Sequence_get_length((const struct REDA_Sequence *)self);

#undef _get_length
#undef TSeq_get_length
}
#endif /* TSeq_get_length */

#ifndef TSeq_set_length
#define _set_length(TSeq) concatenate(TSeq, _set_length)
#define TSeq_set_length _set_length(TSeq)
RTI_BOOL
TSeq_set_length(struct TSeq *self, RTI_INT32 new_length)
{
    return REDA_Sequence_set_length((struct REDA_Sequence *)self, new_length);

#undef _set_length
#undef TSeq_set_length
}
#endif /* TSeq_set_length */

#ifndef TSeq_get_reference
#define _get_reference(TSeq) concatenate(TSeq, _get_reference)
#define TSeq_get_reference _get_reference(TSeq)
T*
TSeq_get_reference(const struct TSeq *self, RTI_INT32 i)
{
    return (T*)REDA_Sequence_get_reference((const struct REDA_Sequence*)self,i);

#undef _get_reference
#undef TSeq_get_reference
}
#endif /* TSeq_get_reference */


#ifndef TSeq_copy
#define _copy(TSeq) concatenate(TSeq, _copy)
#define TSeq_copy _copy(TSeq)
struct TSeq*
TSeq_copy(struct TSeq *self, const struct TSeq *src)
{
#ifdef T_copy
    RTI_BOOL result;
    RTI_INT32 i;
    struct TSeq seq_copy;

    LOG_PRECONDITION(self == NULL,return NULL)

    seq_copy = *self;

    if (REDA_Sequence_copy((struct REDA_Sequence *)self,
                            (const struct REDA_Sequence *)src,
                            RTI_FALSE) == NULL)
    {
        return NULL;
    }

    if (self->_contiguous_buffer != seq_copy._contiguous_buffer)
    {
        result = TSeq_map(self,T_initialize,0,self->_maximum-1);
        if (!result)
        {
            return NULL;
        }
    }
    for (i = 0; i < src->_length; i++)
    {
        if (!T_copy(&self->_contiguous_buffer[i],
                    (const T*) &src->_contiguous_buffer[i]))
        {
            return NULL;
        }
    }

    if ((seq_copy._contiguous_buffer != NULL) &&
        (seq_copy._contiguous_buffer != self->_contiguous_buffer))
    {
        result = TSeq_map(&seq_copy,T_finalize,0,seq_copy._maximum-1);
        if (!result)
        {
            return NULL;
        }
        OSAPI_Heap_freeBuffer(seq_copy._contiguous_buffer);
    }

    return self;
#else
    return (struct TSeq*)REDA_Sequence_copy((struct REDA_Sequence *)self,
                                            (const struct REDA_Sequence *)src,
                                            RTI_TRUE);
#endif
#undef _copy
#undef TSeq_copy
}
#endif /* TSeq_copy */

#ifndef TSeq_is_equal
#define _is_equal(TSeq) concatenate(TSeq, _is_equal)
#define TSeq_is_equal _is_equal(TSeq)
RTI_BOOL
TSeq_is_equal(const struct TSeq *left, const struct TSeq *right)
{
#ifdef T_compare
    RTI_INT32 i;
    RTI_BOOL result;

    result = REDA_Sequence_is_equal((const struct REDA_Sequence*)left,
                                    (const struct REDA_Sequence*)right,RTI_FALSE);
    if (!result)
    {
        return result;
    }

    for (i = 0; i < left->_length; i++)
    {
        if (T_compare((const T*)&left->_contiguous_buffer[i],
                      (const T*)&right->_contiguous_buffer[i]))
        {
            return RTI_FALSE;
        }
    }

    return RTI_TRUE;
#else
    return REDA_Sequence_is_equal((const struct REDA_Sequence *)left,
                                  (const struct REDA_Sequence *)right,RTI_TRUE);
#endif
#undef _is_equal
#undef TSeq_is_equal
}
#endif /* TSeq_is_equal */

#ifndef TSeq_loan_contiguous
#define _loan_contiguous(TSeq) concatenate(TSeq, _loan_contiguous)
#define TSeq_loan_contiguous _loan_contiguous(TSeq)
RTI_BOOL
TSeq_loan_contiguous(struct TSeq *self, void *buffer, 
                     RTI_INT32 new_length, RTI_INT32 new_max)
{
    return REDA_Sequence_loan_contiguous(
       (struct REDA_Sequence *)self, buffer, new_length, new_max);
}
#undef _loan_contiguous
#undef TSeq_loan_contiguous
#endif /* TSeq_loan_contiguous */

#ifndef TSeq_loan_discontiguous
#define _loan_discontiguous(TSeq) concatenate(TSeq, _loan_discontiguous)
#define TSeq_loan_discontiguous _loan_discontiguous(TSeq)
RTI_BOOL
TSeq_loan_discontiguous(struct TSeq *self, void *buffer, 
                        RTI_INT32 new_length, RTI_INT32 new_max)
{
    return REDA_Sequence_loan_discontiguous(
       (struct REDA_Sequence *)self, buffer, new_length, new_max);
}
#undef _loan_discontiguous
#undef TSeq_loan_discontiguous
#endif /* TSeq_loan_discontiguous */


#ifndef TSeq_has_ownership
#define _has_ownership(TSeq) concatenate(TSeq, _has_ownership)
#define TSeq_has_ownership _has_ownership(TSeq)
RTI_BOOL
TSeq_has_ownership(const struct TSeq *self)
{
    return REDA_Sequence_has_ownership((const struct REDA_Sequence *)self);
}

#undef _has_ownership
#undef TSeq_has_ownership
#endif /* TSeq_has_ownership */


#ifndef TSeq_has_discontiguous_buffer
#define _has_discontiguous_buffer(TSeq) concatenate(TSeq, _discontiguous_buffer)
#define TSeq_has_discontiguous_buffer _has_discontiguous_buffer(TSeq)
RTI_BOOL
TSeq_has_discontiguous_buffer(const struct TSeq *self)
{
    return REDA_Sequence_has_discontiguous_buffer(
       (const struct REDA_Sequence *)self);
}

#undef _has_discontiguous_buffer
#undef TSeq_has_discontiguous_buffer
#endif /* TSeq_has_discontiguous_buffer */


#ifndef TSeq_get_buffer
#define _get_buffer(TSeq) concatenate(TSeq, _get_buffer)
#define TSeq_get_buffer _get_buffer(TSeq)
T*
TSeq_get_buffer(const struct TSeq *self)
{
    return (T*)REDA_Sequence_get_buffer((const struct REDA_Sequence *)self);
}

#undef _get_buffer
#undef TSeq_get_buffer
#endif /* TSeq_get_buffer */


#ifndef TSeq_set_buffer
#define _set_buffer(TSeq) concatenate(TSeq, _set_buffer)
#define TSeq_set_buffer _set_buffer(TSeq)
RTI_BOOL
TSeq_set_buffer(struct TSeq *self, T *buffer)
{
    return REDA_Sequence_set_buffer((struct REDA_Sequence *)self, 
                                    (void *)buffer);
}

#undef _set_buffer
#undef TSeq_set_buffer
#endif /* TSeq_set_buffer */


#ifndef TSeq_unloan
#define _unloan(TSeq) concatenate(TSeq, _unloan)
#define TSeq_unloan _unloan(TSeq)
RTI_BOOL
TSeq_unloan(struct TSeq *self)
{
    return REDA_Sequence_unloan((struct REDA_Sequence *)self);
}
#undef _unloan
#undef TSeq_unloan
#endif /* TSeq_unloan */


#undef _map
#undef TSeq_map
#undef _binary
#undef TSeq_binary
#undef concatenate


#endif /* reda_sequence_gen_h */
