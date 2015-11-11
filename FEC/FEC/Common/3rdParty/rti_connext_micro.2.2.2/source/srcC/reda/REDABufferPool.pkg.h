/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef REDABufferPool_pkg_h
#define REDABufferPool_pkg_h

struct REDA_BufferPoolEntry
{
    struct REDA_BufferPoolEntry *_next;
};

struct REDA_BufferPool
{
    struct REDA_BufferPoolProperty property;
    struct REDA_BufferPoolEntry buffer_pool;
    REDA_BufferPool_finalizeFunc_T finalize_func;
    void *finalize_param;
    RTI_SIZE_T allocated_count;
    void *block_alloc;
};

#endif /* REDABufferPool_pkg_h */
