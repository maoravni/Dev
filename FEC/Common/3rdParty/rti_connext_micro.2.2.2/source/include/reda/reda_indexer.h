/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_indexer_h
#define reda_indexer_h


#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
    \brief Defines the REDA_Indexer interface
*/

struct REDA_Indexer;
typedef struct REDA_Indexer REDA_Indexer_T;

struct REDA_IndexerProperty
{
    RTI_INT32 max_entries;
};

#define REDA_IndexerProperty_INITIALIZER \
{\
    1\
}

typedef RTI_INT32
(*REDA_Indexer_compare_T)(const void *const record, RTI_BOOL key_is_record,
                          const void *const key);

typedef RTI_UINT32
(*REDA_Indexer_hash_T)(void* a_record,void *a_key,RTI_INT32 max_buckets);

REDADllExport REDA_Indexer_T*
REDA_Indexer_new(REDA_Indexer_compare_T compare,
                 REDA_Indexer_hash_T hash,
                 struct REDA_IndexerProperty *property);

REDADllExport RTI_BOOL
REDA_Indexer_delete(REDA_Indexer_T *indexer);

REDADllExport RTI_BOOL
REDA_Indexer_add_entry(REDA_Indexer_T *indexer,void *entry);

REDADllExport void*
REDA_Indexer_remove_entry(REDA_Indexer_T *indexer,const void *const key);

REDADllExport void*
REDA_Indexer_find_entry(REDA_Indexer_T *indexer,const void *const key);

REDADllExport RTI_INT32
REDA_Indexer_get_count(REDA_Indexer_T *indexer);

REDADllExport void*
REDA_Indexer_get_entry(REDA_Indexer_T *indexer,RTI_INT32 index);

REDADllExport void*
REDA_Indexer_get_first_entry(REDA_Indexer_T *indexer);

REDADllExport void*
REDA_Indexer_get_last_entry(REDA_Indexer_T *indexer);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#include "reda/reda_buffer_impl.h"

#endif /* reda_indexer_h */
