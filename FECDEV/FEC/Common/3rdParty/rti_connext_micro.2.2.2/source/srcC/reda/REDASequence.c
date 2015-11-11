/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef util_makeheader_h
#include "util/util_makeheader.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef reda_sequence_h
#include "reda/reda_sequence.h"
#endif
#ifndef reda_log_h
#include "reda/reda_log.h"
#endif

struct REDA_Sequence
{
    void *_contiguous_buffer;
    RTI_INT32 _maximum;
    RTI_INT32 _length;
    RTI_INT32 _element_size;
    RTI_UINT8 _flags;
};

/*** SOURCE_BEGIN ***/

/* ------------------------------------------------------------------
 * Public Methods
 * ------------------------------------------------------------------ */
RTI_BOOL
REDA_Sequence_initialize(struct REDA_Sequence *self, RTI_INT32 element_size)
{
    LOG_PRECONDITION_P3(((self == NULL) || (element_size <= 0)),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_HIGH(self),OSAPI_LOG_PTR_LOW(self),
                        element_size)

    (self)->_contiguous_buffer = NULL;
    (self)->_maximum = 0;
    (self)->_length = 0;
    (self)->_element_size = element_size;
    (self)->_flags = 0;

    return RTI_TRUE;
}

RTI_BOOL
REDA_Sequence_finalize(struct REDA_Sequence *self)
{
    LOG_PRECONDITION((self == NULL || !REDA_Sequence_has_ownership(self)),
                     return RTI_FALSE)

    if (self->_contiguous_buffer != NULL)
    {
        OSAPI_Heap_freeBuffer((char *)self->_contiguous_buffer);
        self->_contiguous_buffer = NULL;
    }

    self->_element_size = 0;
    self->_length = 0;
    self->_maximum = 0;
    self->_flags = 0;

    return RTI_TRUE;
}

RTI_INT32
REDA_Sequence_get_maximum(const struct REDA_Sequence * self)
{
    LOG_PRECONDITION((self == NULL),return -1)

    return self->_maximum;
}

RTI_BOOL
REDA_Sequence_set_maximum(struct REDA_Sequence * self, RTI_INT32 new_max,
                          RTI_BOOL copy_content)
{
    void *new_buffer = NULL;
    void *old_buffer = NULL;
    RTI_INT32 new_length = 0;

    LOG_PRECONDITION_P3(((self == NULL) || (new_max < 0)),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_HIGH(self),OSAPI_LOG_PTR_LOW(self),
                        new_max)

    LOG_PRECONDITION_P1((!REDA_Sequence_has_ownership(self)),
                        return RTI_FALSE, self->_flags)

    if (new_max < 0)
    {
        return RTI_FALSE;
    }

    if ((RTI_INT32) new_max == self->_maximum)
    {
        return RTI_TRUE;
    }

    /* -- Allocate new contiguous buffer --- */
    if (new_max > 0)
    {
        OSAPI_Heap_allocateBufferInternal((char **)&new_buffer,
                                         new_max * self->_element_size,
                                         OSAPI_ALIGNMENT_DEFAULT);
        if (new_buffer == NULL)
        {
            REDA_Log_error(REDA_LOG_SEQUENCE_ALLOC_FAILED);
            return RTI_FALSE;
        }
    }

    new_length = (self->_length < (RTI_INT32) new_max)
        ? (RTI_INT32) self->_length : new_max;

    /* If the sequence element is contigous in memory, copy_content
     * is true and the entire sequence can be copied in a single copy
     * Otherwise a deep copy of each element must be performed using the
     * supplied T_copy method.
     */
    if ((new_length > 0) && 
        copy_content && 
        !OSAPI_Memory_copy(new_buffer,
                           self->_contiguous_buffer,
                           new_length * self->_element_size))
    {
        REDA_Log_error(REDA_LOG_SEQUENCE_COPY_FAILED);
        return RTI_FALSE;
    }

    /* --- Swap buffers --- */
    old_buffer = self->_contiguous_buffer;
    self->_contiguous_buffer = new_buffer;
    self->_maximum = new_max;
    self->_length = new_length;

    /* --- Free original buffer --- */
    /* If copy_content is false, it is up to the called to free the old
     * buffer
     */
    if ((old_buffer != NULL) && copy_content)
    {
        OSAPI_Heap_freeBuffer(old_buffer);
    }

    return RTI_TRUE;
}

RTI_INT32
REDA_Sequence_get_length(const struct REDA_Sequence * self)
{
    LOG_PRECONDITION((self == NULL),return -1)

    return self->_length;
}

RTI_BOOL
REDA_Sequence_set_length(struct REDA_Sequence * self, RTI_INT32 new_length)
{
    LOG_PRECONDITION_P2((self == NULL), return RTI_FALSE,
                        OSAPI_LOG_PTR_HIGH(self),OSAPI_LOG_PTR_LOW(self))

    if ((new_length < 0) || (new_length > self->_maximum))
    {
        return RTI_FALSE;
    }

    self->_length = new_length;

    return RTI_TRUE;
}

void*
REDA_Sequence_get_reference(const struct REDA_Sequence *self, RTI_INT32 i)
{
    LOG_PRECONDITION_P2((self == NULL), return NULL,
                        OSAPI_LOG_PTR_HIGH(self), OSAPI_LOG_PTR_LOW(self))

    if ((i < 0) || ((RTI_INT32) i >= self->_length))
    {
        return NULL;
    }

    if (self->_flags & REDA_SEQUENCE_FLAG_DISCONTIGUOUS)
    {
        /* return the i-th pointer in array of pointers */
        return (void *)( ((void **)self->_contiguous_buffer)[i] );
    }

    /* contiguous */
    return (void *)((char*)self->_contiguous_buffer + (i * self->_element_size));

}

struct REDA_Sequence*
REDA_Sequence_copy(struct REDA_Sequence *self, const struct REDA_Sequence *src,
                   RTI_BOOL copy_content)
{
    RTI_INT32 self_max = 0;
    RTI_INT32 src_max = 0;

    LOG_PRECONDITION_P6(((self == NULL) || (src == NULL) ||
                        ((self->_element_size != src->_element_size))),
                        return NULL,
                        OSAPI_LOG_PTR_HIGH(self),OSAPI_LOG_PTR_LOW(self),
                        self->_element_size,
                        OSAPI_LOG_PTR_HIGH(src),OSAPI_LOG_PTR_LOW(src),
                        src->_element_size)

    if (self->_element_size != src->_element_size)
    {
        return NULL;
    }

    if ((self->_contiguous_buffer == NULL) && (src->_contiguous_buffer == NULL))
    {
        return self;
    }

    self_max = REDA_Sequence_get_maximum(self);
    if (self_max < REDA_Sequence_get_length(src))
    {
        src_max = REDA_Sequence_get_maximum(src);
        if (!REDA_Sequence_set_maximum(self, src_max,copy_content))
        {
            REDA_Log_error_p3(REDA_LOG_SEQUENCE_SET_MAX_FAILED,
               OSAPI_LOG_PTR_HIGH(self),OSAPI_LOG_PTR_LOW(self),src_max);
            return NULL;
        }
        self->_maximum = src_max;
    }

    self->_length = src->_length;

    if (src->_length == 0)
    {
        return self;
    }

    if (copy_content)
    {
        if (!OSAPI_Memory_copy(self->_contiguous_buffer,
                src->_contiguous_buffer,
                REDA_Sequence_get_length(src) * src->_element_size))
        {
            REDA_Log_error_p6(REDA_LOG_SEQUENCE_SET_MAX_FAILED,
                    OSAPI_LOG_PTR_HIGH(self->_contiguous_buffer),
                    OSAPI_LOG_PTR_LOW(self->_contiguous_buffer),
                    OSAPI_LOG_PTR_HIGH(src->_contiguous_buffer),
                    OSAPI_LOG_PTR_LOW(src->_contiguous_buffer),
                    src->_length,src->_element_size);
            return NULL;
        }
    }

    return self;
}

RTI_BOOL
REDA_Sequence_is_equal(const struct REDA_Sequence *left,
                       const struct REDA_Sequence *right,
                       RTI_BOOL compare_content)
{
    LOG_PRECONDITION_P4(((left == NULL) || (right == NULL)),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_HIGH(left),OSAPI_LOG_PTR_LOW(left),
                        OSAPI_LOG_PTR_HIGH(right),OSAPI_LOG_PTR_LOW(right))

    /* compare owned sequences only */
    LOG_PRECONDITION_P2((!REDA_Sequence_has_ownership(left) || 
                         !REDA_Sequence_has_ownership(right)),
                        return RTI_FALSE, left->_flags, right->_flags)

    if ((left->_length != right->_length) ||
        (left->_element_size != right->_element_size))
    {
        REDA_Log_error_p4(REDA_LOG_SEQUENCE_INVALID_COMPARE,
                left->_length,right->_length,
                left->_element_size,right->_element_size);
        return RTI_FALSE;
    }

    if (left->_length == 0)
    {
        return RTI_TRUE;
    }

    if (compare_content)
    {
        return !OSAPI_Memory_compare(left->_contiguous_buffer,
                right->_contiguous_buffer,
                left->_length * left->_element_size);
    }
    else
    {
        return RTI_TRUE;
    }
}

RTI_BOOL
REDA_Sequence_loan_contiguous(struct REDA_Sequence *self,
                              void *buffer,
                              RTI_INT32 new_length,
                              RTI_INT32 new_max)
{
    /* TODO: make precondition */
    if (self == NULL || buffer == NULL || !REDA_Sequence_has_ownership(self))
    {
        return RTI_FALSE;
    }

    self->_contiguous_buffer = buffer;
    self->_length = new_length;
    self->_maximum = new_max;
        
    /* mark seq as using loaned buffer */
    self->_flags |= REDA_SEQUENCE_FLAG_LOAN;

    return RTI_TRUE;
}

RTI_BOOL
REDA_Sequence_loan_discontiguous(struct REDA_Sequence *self,
                              void *buffer,
                              RTI_INT32 new_length,
                              RTI_INT32 new_max)
{
    /* TODO: make precondition */
    if (self == NULL || buffer == NULL || !REDA_Sequence_has_ownership(self))
    {
        return RTI_FALSE;
    }

    self->_contiguous_buffer = buffer;
    self->_length = new_length;
    self->_maximum = new_max;
        
    /* flagged as both loaned and discontiguous */
    self->_flags |= (REDA_SEQUENCE_FLAG_LOAN|REDA_SEQUENCE_FLAG_DISCONTIGUOUS);

    return RTI_TRUE;
}

RTI_BOOL
REDA_Sequence_has_ownership(const struct REDA_Sequence *self)
{
    return ((self == NULL) || (self->_flags & REDA_SEQUENCE_FLAG_LOAN)) ?
        RTI_FALSE : RTI_TRUE;
}

RTI_BOOL
REDA_Sequence_set_buffer(struct REDA_Sequence *self,
                         void *buffer)
{
    if (self == NULL || !REDA_Sequence_has_ownership(self) || buffer == NULL)
    {
        return RTI_FALSE;
    }

    self->_contiguous_buffer = buffer;

    return RTI_TRUE;
}

void *
REDA_Sequence_get_buffer(const struct REDA_Sequence *self)
{
    return (self == NULL ? NULL : self->_contiguous_buffer);
}

RTI_BOOL
REDA_Sequence_unloan(struct REDA_Sequence *self)
{
    if (self == NULL || !(self->_flags & REDA_SEQUENCE_FLAG_LOAN))
    {
        return RTI_FALSE;
    }
    self->_contiguous_buffer = NULL;
    self->_length = 0;
    self->_maximum = 0;

    /* clear flags, as seq is unloaned, and has no buffer */
    self->_flags = 0;

    return RTI_TRUE;
}

RTI_BOOL
REDA_Sequence_has_discontiguous_buffer(const struct REDA_Sequence *self)
{
    return ((self == NULL) || 
            !(self->_flags & REDA_SEQUENCE_FLAG_DISCONTIGUOUS)) ? 
        RTI_FALSE : RTI_TRUE;
}


