/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_bufferpool_h
#define reda_bufferpool_h


#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct REDA_BufferPoolProperty
{
    RTI_SIZE_T buffer_size;
    RTI_SIZE_T max_buffers;
    RTI_INT32 flags;
};

#define REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC (0x1)

#define REDA_BufferPoolProperty_INITIALIZER \
{ \
    0,\
    0,\
    0\
}

#define REDA_BUFFERPOOL_UNLIMITED (0xffffffff)

struct REDA_BufferPool;
typedef struct REDA_BufferPool *REDA_BufferPool_T;

typedef RTI_BOOL
(*REDA_BufferPool_initializeFunc_T)(void *initialize_param, void *buffer);

typedef RTI_BOOL
(*REDA_BufferPool_finalizeFunc_T)(void *finalize_param, void *buffer);

REDADllExport REDA_BufferPool_T
REDA_BufferPool_new(struct REDA_BufferPoolProperty *property,
                    REDA_BufferPool_initializeFunc_T initialize_func,
                    void *initialize_param,
                    REDA_BufferPool_finalizeFunc_T finalize_func,
                    void *finalize_param);

REDADllExport RTI_BOOL
REDA_BufferPool_delete(REDA_BufferPool_T pool);

REDADllExport void*
REDA_BufferPool_get_buffer(REDA_BufferPool_T pool);

REDADllExport void
REDA_BufferPool_return_buffer(REDA_BufferPool_T pool,void *buffer);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* reda_bufferpool_h */
