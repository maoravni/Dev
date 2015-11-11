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
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef reda_log_h
#include "reda/reda_log.h"
#endif
#ifndef reda_index_h
#include "reda/reda_indexer.h"
#endif


#ifndef reda_index_h
#include "reda/reda_indexer.h"
#endif


struct REDA_Indexer
{
    struct REDA_IndexerProperty property;
    void **elements;
    REDA_Indexer_compare_T compare;
    REDA_Indexer_hash_T hash;
    RTI_INT32 high_index;
};

#define REDA_IndexerImpl_get_count(indexer_) \
((indexer_)->high_index + 1)

/*** SOURCE_BEGIN ***/

RTI_PRIVATE RTI_BOOL
REDA_Indexer_binsearch(struct REDA_Indexer *const indexer,
                       RTI_BOOL key_is_record,
                       const void *const key_lower,
                       const void *const key_upper,
                       RTI_INT32 *const result,
                       RTI_BOOL find_upper)
{
    RTI_INT32 mid,cmp_result,cand_index;
    RTI_BOOL exists = RTI_FALSE;
    RTI_INT32 lower = 0;
    RTI_INT32 upper = indexer->high_index;

    if (find_upper)
    {
        cand_index = -1;
    }
    else
    {
        cand_index = 0x7fffffffL;
    }

    while (lower <= upper)
    {
        mid = lower + (upper-lower)/2;
        cmp_result = indexer->compare(indexer->elements[mid],key_is_record,key_lower);
        if (!cmp_result)
        {
            cand_index = mid;
            exists = RTI_TRUE;
            break;
        }
        else if (cmp_result < 0)
        {
            lower = mid+1;
            if ((find_upper) && (cand_index < mid))
            {
                cand_index = mid;
                exists = RTI_TRUE;
            }
        }
        else
        {
            if (key_upper)
            {
                cmp_result = indexer->compare(indexer->elements[mid],key_is_record,key_upper);
                if ((cmp_result <= 0) && (cand_index > mid))
                {
                    cand_index = mid;
                    exists = RTI_TRUE;
                }
            }
            upper = mid-1;
        }
    }

    if (result)
    {
        if ((lower > upper) && (!exists))
        {
            *result = lower;
        }
        else
        {
            *result = cand_index;
        }
    }

    return exists;
}

REDA_Indexer_T*
REDA_Indexer_new(REDA_Indexer_compare_T compare,
                 REDA_Indexer_hash_T hash,
                 struct REDA_IndexerProperty *property)
{
    struct REDA_Indexer *retval = NULL;
    struct REDA_Indexer *indexer = NULL;
    RTI_INT32 i;

    OSAPI_Heap_allocateStructure(&indexer,struct REDA_Indexer);
    if (indexer == NULL)
    {
        goto done;
    }

    indexer->property = *property;
    indexer->compare = compare;
    indexer->hash = hash;
    indexer->high_index = -1;

     OSAPI_Heap_allocateArray(&indexer->elements,indexer->property.max_entries,void*);
     if (indexer->elements == NULL)
     {
         OSAPI_Heap_freeStructure(indexer);
         goto done;
     }

     for (i = 0; i < indexer->property.max_entries; ++i)
     {
         indexer->elements[i] = NULL;
     }

     retval = indexer;

done:
    return retval;
}

RTI_BOOL
REDA_Indexer_delete(REDA_Indexer_T *indexer)
{
    if (indexer->elements != NULL)
    {
        OSAPI_Heap_freeArray(indexer->elements);
        indexer->elements = NULL;
    }

    OSAPI_Heap_freeStructure(indexer);

    return RTI_TRUE;
}

RTI_BOOL
REDA_Indexer_add_entry(REDA_Indexer_T *indexer,void *entry)
{
    RTI_BOOL exists;
    RTI_INT32 new_index;

    if (indexer->high_index == -1)
    {
        indexer->elements[0] = entry;
        indexer->high_index = 0;
        return RTI_TRUE;
    }

    if (REDA_IndexerImpl_get_count(indexer) == indexer->property.max_entries)
    {
        return RTI_FALSE;
    }

    exists = REDA_Indexer_binsearch(indexer,RTI_TRUE,entry,NULL,&new_index,RTI_FALSE);
    if (exists)
    {
        return RTI_FALSE;
    }

    if (new_index <= indexer->high_index)
    {
        OSAPI_Memory_move(&indexer->elements[new_index+1],
                &indexer->elements[new_index],
                sizeof(void*) * (indexer->high_index - new_index + 1));
    }

    ++indexer->high_index;

    indexer->elements[new_index] = entry;

    return RTI_TRUE;
}

void*
REDA_Indexer_remove_entry(REDA_Indexer_T *indexer,const void *const key)
{
    RTI_BOOL exists;
    RTI_INT32 entry_index;
    void *entry;

    if (indexer->high_index == -1)
    {
        return NULL;
    }

    if (indexer->high_index == 0)
    {
        entry = indexer->elements[0];
        indexer->elements[0] = NULL;
        indexer->high_index = -1;
        return entry;
    }

    exists = REDA_Indexer_binsearch(indexer,RTI_FALSE,key,NULL,&entry_index,RTI_FALSE);
    if (!exists)
    {
        return NULL;
    }

    entry = indexer->elements[entry_index];
    if (entry_index == indexer->high_index)
    {
        indexer->elements[entry_index] = NULL;
    }
    else
    {
        OSAPI_Memory_move(&indexer->elements[entry_index],
                &indexer->elements[entry_index+1],
                sizeof(void*) * (indexer->high_index - entry_index));
    }

    --indexer->high_index;

    return entry;
}

void*
REDA_Indexer_find_entry(REDA_Indexer_T *indexer,const void *const key)
{
    RTI_BOOL exists;
    RTI_INT32 entry_index;

    exists = REDA_Indexer_binsearch(indexer,RTI_FALSE,key,NULL,&entry_index,RTI_FALSE);
    if (!exists)
    {
        return NULL;
    }

    return indexer->elements[entry_index];
}

RTI_INT32
REDA_Indexer_get_count(REDA_Indexer_T *indexer)
{
    if (indexer->high_index == -1)
    {
        return 0;
    }

    return REDA_IndexerImpl_get_count(indexer);
}

void*
REDA_Indexer_get_entry(REDA_Indexer_T *indexer,RTI_INT32 index)
{
    return indexer->elements[index];
}

void*
REDA_Indexer_get_first_entry(REDA_Indexer_T *indexer)
{
    if (indexer->high_index == -1)
    {
        return NULL;
    }

    return indexer->elements[0];
}

void*
REDA_Indexer_get_last_entry(REDA_Indexer_T *indexer)
{
    if (indexer->high_index == -1)
    {
        return NULL;
    }

    return indexer->elements[indexer->high_index];
}
