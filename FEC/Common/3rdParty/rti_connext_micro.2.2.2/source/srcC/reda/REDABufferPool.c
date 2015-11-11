/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#ifndef osapi_config_h
#include "osapi/osapi_config.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef reda_log_h
#include "reda/reda_log.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif

struct REDA_BufferPoolEntry
{
    struct REDA_BufferPoolEntry *_next;
};

#define REDA_BUFFERPOOL_HEADER_SIZE (sizeof(struct REDA_BufferPoolEntry))
#define REDA_BUFFERPOOL_ALIGNUP(size_) \
((((size_) + REDA_BUFFERPOOL_HEADER_SIZE) & (sizeof(void*)-1)) ? \
 (((size_) + REDA_BUFFERPOOL_HEADER_SIZE + sizeof(void*)) & ~(sizeof(void*)-1))\
  : (size_) + REDA_BUFFERPOOL_HEADER_SIZE)

struct REDA_BufferPool
{
    struct REDA_BufferPoolProperty property;
    struct REDA_BufferPoolEntry buffer_pool;
    REDA_BufferPool_finalizeFunc_T finalize_func;
    void *finalize_param;
    RTI_SIZE_T allocated_count;
    void *block_alloc;
};

/*** SOURCE_BEGIN ***/

REDA_BufferPool_T
REDA_BufferPool_new(struct REDA_BufferPoolProperty *property,
                   REDA_BufferPool_initializeFunc_T initialize_func,
                   void *initialize_param,
                   REDA_BufferPool_finalizeFunc_T finalize_func,
                   void *finalize_param)
{
    struct REDA_BufferPool *pool = NULL;
    RTI_SIZE_T real_size,alloc_size;
    struct REDA_BufferPoolEntry *pool_entry;
    RTI_SIZE_T i;

    LOG_PRECONDITION((property == NULL) || (property->buffer_size == 0) ||
                     (property->max_buffers == 0) ||
                     (property->buffer_size == REDA_BUFFERPOOL_UNLIMITED) ||
                     (property->max_buffers == REDA_BUFFERPOOL_UNLIMITED),
                     goto done)

    OSAPI_Heap_allocateStructure(&pool,struct REDA_BufferPool);
    if (pool == NULL)
    {
        REDA_Log_error(REDA_LOG_BUFFERPOOL_OUT_OF_RESOURCES);
        goto done;
    }

    pool->buffer_pool._next = NULL;
    pool->property = *property;
    pool->finalize_func = finalize_func;
    pool->finalize_param = finalize_param;
    pool->allocated_count = 0;
    pool->block_alloc = NULL;

    real_size = REDA_BUFFERPOOL_ALIGNUP(property->buffer_size);
    alloc_size = real_size;
    if (pool->property.flags & REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC)
    {
        alloc_size *= pool->property.max_buffers;
    }

    OSAPI_Heap_allocateBuffer((char**)&pool->buffer_pool._next,
                             alloc_size,
                             OSAPI_ALIGNMENT_DEFAULT);
    if (pool->buffer_pool._next == NULL)
    {
        REDA_Log_error(REDA_LOG_BUFFERPOOL_OUT_OF_RESOURCES);
        goto failure;
    }
    pool->block_alloc = pool->buffer_pool._next;
    if (initialize_func &&
        !initialize_func(initialize_param,&pool->buffer_pool._next[1]))
    {
        REDA_Log_error_p2(REDA_LOG_BUFFERPOOL_BUFFER_INITIALIZATION_FAILED,
                          OSAPI_LOG_PTR_HIGH(initialize_func),
                          OSAPI_LOG_PTR_LOW(initialize_func));
        goto failure;
    }

    pool_entry = pool->buffer_pool._next;
    for (i = 1; i < pool->property.max_buffers; ++i)
    {
        if (pool->property.flags & REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC)
        {
            pool_entry->_next = (struct REDA_BufferPoolEntry *)
                                              ((char*)pool_entry + real_size);
        }
        else
        {
            OSAPI_Heap_allocateBuffer((char**)&pool_entry->_next,
                                     real_size,
                                     OSAPI_ALIGNMENT_DEFAULT);
            if (pool_entry->_next == NULL)
            {
                goto failure;
            }
        }
        if (initialize_func &&
            !initialize_func(initialize_param,&pool_entry->_next[1]))
        {
            REDA_Log_error_p2(REDA_LOG_BUFFERPOOL_BUFFER_INITIALIZATION_FAILED,
                              OSAPI_LOG_PTR_HIGH(initialize_func),
                              OSAPI_LOG_PTR_LOW(initialize_func));
            goto failure;
        }
        pool_entry = pool_entry->_next;
    }

    pool_entry->_next = NULL;

done:
    return pool;

failure:
    (void)REDA_BufferPool_delete(pool);
    pool = NULL;

    return NULL;
}

RTI_BOOL
REDA_BufferPool_delete(REDA_BufferPool_T pool)
{
    struct REDA_BufferPoolEntry *pool_entry,*pool_entry_next;

    LOG_PRECONDITION(pool == NULL,return RTI_FALSE)

    if (pool->allocated_count > 0)
    {
        return RTI_FALSE;
    }

    pool_entry = pool->buffer_pool._next;
    while (pool_entry != NULL)
    {
        pool_entry_next = pool_entry->_next;
        if (pool->finalize_func)
        {
            pool->finalize_func(pool->finalize_param,(void*)&pool_entry[1]);
        }
        if (!(pool->property.flags & REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC))
        {
            OSAPI_Heap_freeBuffer(pool_entry);
        }
        pool_entry = pool_entry_next;
    }

    if (pool->property.flags & REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC)
    {
        OSAPI_Heap_freeBuffer(pool->block_alloc);
    }

    OSAPI_Heap_freeStructure(pool);

    return RTI_TRUE;
}

void*
REDA_BufferPool_get_buffer(REDA_BufferPool_T pool)
{
    struct REDA_BufferPoolEntry *retval = NULL;

    LOG_PRECONDITION(pool == NULL,return NULL)

    retval = (void*)pool->buffer_pool._next;
    if (retval == NULL)
    {
        return NULL;
    }

    pool->buffer_pool._next = retval->_next;
    ++pool->allocated_count;

    return &retval[1];
}

void
REDA_BufferPool_return_buffer(REDA_BufferPool_T pool,void *buffer)
{
    struct REDA_BufferPoolEntry *retval = (struct REDA_BufferPoolEntry *)buffer;

    LOG_PRECONDITION_P4(((pool == NULL) || (buffer == NULL)),return,
            OSAPI_LOG_PTR_HIGH(pool),OSAPI_LOG_PTR_LOW(pool),
            OSAPI_LOG_PTR_HIGH(buffer),OSAPI_LOG_PTR_LOW(buffer))

    --retval;
    retval->_next = pool->buffer_pool._next;
    pool->buffer_pool._next = retval;
    --pool->allocated_count;
}
