/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "osapi/osapi_config.h"
#include "util/util_makeheader.h"
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef reda_string_h
#include "reda/reda_string.h"
#endif

#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef reda_circularlist_h
#include "reda/reda_circularlist.h"
#endif

#ifndef db_api_h
#include "db/db_api.h"
#endif
#ifndef db_log_h
#include "db/db_log.h"
#endif

#undef URTDB__DEBUG

#ifdef fail_with_retcode
#error "fail_with_retcode already defined"
#else
#define fail_with_retcode(__retcode) \
retcode = __retcode;\
goto done
#endif

/* NOTE: Must be divisible by 4 */
struct URTDB_RecordHeader
{
    REDA_CircularListNode_T node;
    RTI_UINT32 ref_count;
};

#define URTDB_RECORDER_HEADER_SIZE (sizeof(struct URTDB_RecordHeader))

struct DB_Cursor
{
    REDA_CircularListNode_T node;
    RTI_INT32 first_index;
    RTI_INT32 current_index;
    RTI_INT32 last_index;
    struct DB_Index *index;
};

struct URTDB_IndexEntry
{
    struct URTDB_RecordHeader *record;
};

RTI_PRIVATE const RTI_SIZE_T URTDB_IndexEntry_g_size = sizeof(struct URTDB_IndexEntry);

struct DB_Index
{
    REDA_CircularListNode_T node;
    DB_IndexCompare_T compare;
    DB_IndexHash_T hash;
    struct URTDB_IndexEntry *search_array;
    struct URTDB_IndexEntry *sorted_array;
    struct DB_IndexProperty property;
    RTI_SIZE_T record_count;
    RTI_SIZE_T last_changed_index;
};

struct DB_Table
{
    REDA_CircularListNode_T node;
    char name[URTDB_DATABASE_NAME_MAX_LENGTH];
    struct DB_TableProperty property;
    REDA_BufferPool_T records;
    REDA_BufferPool_T indices;
    REDA_BufferPool_T cursors;
    REDA_CircularList_T index_in_use;
    REDA_CircularList_T records_in_use;
    REDA_CircularList_T cursors_in_use;
    struct DB_Index default_index;
};

struct DB_Database
{
    char name[URTDB_DATABASE_NAME_MAX_LENGTH];
    REDA_BufferPool_T tables;
    REDA_CircularList_T tables_in_use;
    OSAPI_Mutex_T *lock;
    struct DB_DatabaseProperty property;
};

/*** SOURCE_BEGIN ***/

/*******************************************************************************
 *                                  Internal API
 *
 * - Internal APIs do not check inputs
 * - Internal APIs are never to be exported
 ******************************************************************************/
RTI_PRIVATE RTI_BOOL
URTDB_Table_initialize(void *initialize_param, void *buffer)
{
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
URTDB_Table_finalize(void *finalize_param, void *buffer)
{
    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
URTDB_Index_initialize(struct DB_Index *index,
                      DB_IndexHash_T hash_func,
                      DB_IndexCompare_T compare_func,
                      struct DB_IndexProperty *property)
{
    RTI_SIZE_T i;

    REDA_CircularListNode_init(&index->node);
    index->compare = compare_func;
    index->hash = hash_func;
    index->search_array = NULL;
    index->sorted_array = NULL;
    index->record_count = 0;
    index->property = *property;

    if (index->hash != NULL)
    {
        OSAPI_Heap_allocateArray(&index->search_array,
                property->max_entries,
                struct URTDB_IndexEntry);

        if (index->search_array == NULL)
        {
            return RTI_FALSE;
        }
    }

    OSAPI_Heap_allocateArray(&index->sorted_array,
                            property->max_entries,
                            struct URTDB_IndexEntry);
    if (index->sorted_array == NULL)
    {
        return RTI_FALSE;
    }

    for (i = 0; i < property->max_entries; ++i)
    {
        if (index->search_array)
        {
            index->search_array[i].record = NULL;
        }
        index->sorted_array[i].record = NULL;
    }

    return RTI_TRUE;
}

RTI_PRIVATE void
URTDB_Index_finalize(struct DB_Index *index)
{
    index->compare = NULL;
    index->hash = NULL;
    if (index->search_array)
    {
        OSAPI_Heap_freeArray(index->search_array);
        index->search_array = NULL;
    }
    if (index->sorted_array)
    {
        OSAPI_Heap_freeArray(index->sorted_array);
        index->sorted_array = NULL;
    }
}

RTI_PRIVATE DB_ReturnCode_T
URTDB_Table_binsearch(struct URTDB_IndexEntry *array,
                     RTI_INT32 lower,
                     RTI_INT32 upper,
                     void *op2,
                     void *op3,
                     RTI_INT32 opcode_flags,
                     RTI_INT32 *result,
                     RTI_BOOL find_upper,
                     DB_IndexCompare_T index_compare)
{
    RTI_INT32 mid,cmp_result,cand_index;
    DB_ReturnCode_T retcode = DB_RETCODE_NOT_EXISTS;

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
        cmp_result = index_compare(DB_SELECTOPCODE_EQUAL,
                                   opcode_flags,
                                   &array[mid].record[1],
                                   op2,
                                   NULL);
        if (!cmp_result)
        {
            cand_index = mid;
            retcode = DB_RETCODE_EXISTS;
            break;
        }
        else if (cmp_result < 0)
        {
            lower = mid+1;
            if ((find_upper) && (cand_index < mid))
            {
                cand_index = mid;
                retcode = DB_RETCODE_EXISTS;
            }
        }
        else
        {
            if (op3)
            {
                cmp_result = index_compare(DB_SELECTOPCODE_EQUAL,
                        opcode_flags,
                        &array[mid].record[1],
                        op3,
                        NULL);
                if ((cmp_result <= 0) && (cand_index > mid))
                {
                    cand_index = mid;
                    retcode = DB_RETCODE_EXISTS;
                }
            }
            upper = mid-1;
        }
    }

    if (result)
    {
        if ((lower > upper) && (retcode == DB_RETCODE_NOT_EXISTS))
        {
            *result = lower;
        }
        else
        {
            *result = cand_index;
        }
    }

    return retcode;
}

RTI_PRIVATE DB_ReturnCode_T
URTDB_Table_search_index(struct DB_Table *urtdb_table,
                        struct DB_Index *index,
                        void *op2,
                        void *op3,
                        RTI_INT32 opcode_flags,
                        struct URTDB_RecordHeader **retval,
                        RTI_UINT32 *lower_result,
                        RTI_UINT32 *upper_result)
{
    RTI_INT32 lower_index,upper_index;
    DB_ReturnCode_T retcode = DB_RETCODE_NOT_EXISTS;

    if (index->record_count == 0)
    {
        if (lower_result)
        {
            *lower_result = 0;
        }
        return DB_RETCODE_NOT_EXISTS;
    }

    if (!DB_SELECT_OP2_IS_KEY(opcode_flags))
    {
        op2 = &(((struct URTDB_RecordHeader *)op2)[1]);
    }

    if (op3 && !DB_SELECT_OP3_IS_KEY(opcode_flags))
    {
        op3 = &(((struct URTDB_RecordHeader *)op3)[1]);
    }

    if (retval)
    {
        *retval = NULL;
    }

    retcode = URTDB_Table_binsearch(index->sorted_array,
                                   0,
                                   index->record_count-1,
                                   op2,
                                   op3,
                                   opcode_flags,
                                   &lower_index,
                                   RTI_FALSE,
                                   index->compare);

    /*
     * If an upper bound was found, find the closest one. Limits are
     * [lower,upper], if upper == limit, short-cut sort
     */
    if (op3 && (retcode == DB_RETCODE_EXISTS) && upper_result)
    {
        if (index->compare(DB_SELECTOPCODE_EQUAL,opcode_flags,
                           &index->sorted_array[lower_index].record[1],op3,NULL))
        {
            retcode = DB_RETCODE_NOT_EXISTS;
            retcode = URTDB_Table_binsearch(index->sorted_array,
                                       lower_index,
                                       index->record_count-1,
                                       op3,
                                       NULL,
                                       opcode_flags,
                                       &upper_index,
                                       RTI_TRUE,
                                       index->compare);
        }
        else
        {
            retcode = DB_RETCODE_EXISTS;
            upper_index = lower_index;
        }
    }

    /* if the record does not exit, the lower index is the the index
     * where the record should have been. This is used with insertion
     */
    if (lower_result)
    {
        *lower_result = lower_index;
    }

    if (retcode == DB_RETCODE_EXISTS)
    {
        if (upper_result)
        {
            *upper_result = upper_index;
        }

        if (retval)
        {
            *retval = index->sorted_array[lower_index].record;
        }
    }

    return retcode;
}

RTI_PRIVATE void
URTDB_Table_update_index(struct DB_Table *urtdb_table,
                        struct DB_Index *index,
                        struct URTDB_RecordHeader *new_record,
                        RTI_BOOL remove_record)
{
    RTI_UINT32 insert_index,i;
    DB_ReturnCode_T retcode;
    RTI_SIZE_T record_count = 0;
    RTI_UINT32 lower_index;

    /* record_count == count _berfore_ new_record has been added/deleted
     */
    if ((index->record_count == 0) && !remove_record)
    {
        /*OSAPI_Stdio_printf("Add: new record @ %d\n",0);*/
        index->sorted_array[0].record = new_record;
        index->last_changed_index = 0;
        ++index->record_count;
        return;
    }

    if ((index->record_count == 1) && remove_record)
    {
#ifdef URTDB__DEBUG
        OSAPI_Stdio_printf("Delete: delete record @ %d\n",0);
#endif
        index->sorted_array[0].record = NULL;
        index->last_changed_index = 0;
        --index->record_count;
        return;
    }

    retcode = URTDB_Table_search_index(urtdb_table,index,
                                      new_record,NULL,
                                      0,
                                      NULL,
                                      &insert_index,
                                      NULL);

    record_count = index->record_count - insert_index;

    if (remove_record && (retcode == DB_RETCODE_EXISTS) &&
            (new_record == index->sorted_array[insert_index].record))
    {
        retcode = DB_RETCODE_NOT_EXISTS;
        if (index != &urtdb_table->default_index)
        {
            /* This is slow for large tables. An alternative is to keep
             * a list of alternative records. However, the justification
             * is that it is not anticipated that there will be a high-frequency
             * of deletion of records, unless during shutdown
             */
            for (i = 0; i < urtdb_table->default_index.record_count-1; ++i)
            {
                if (!index->compare(DB_SELECTOPCODE_EQUAL,0,
                        &urtdb_table->default_index.sorted_array[i].record[1],
                        &new_record[1],NULL))
                {
                    retcode = DB_RETCODE_EXISTS;
                    lower_index = i;
                    break;
                }
            }
        }
        if (retcode == DB_RETCODE_EXISTS)
        {
            /* If a record existed, simple substitute it for the old one
             */
#ifdef URTDB__DEBUG
            OSAPI_Stdio_printf("substitute\n");
#endif
            index->sorted_array[insert_index] = urtdb_table->default_index.sorted_array[lower_index];
            index->last_changed_index = insert_index;
        }
        else
        {
#ifdef URTDB__DEBUG
            OSAPI_Stdio_printf("Delete: remove from %d records %d to %d\n",
                    record_count-1,
                    insert_index+1,
                    insert_index);
#endif
            /* Otherwise remove the entry */
            OSAPI_Memory_move(&index->sorted_array[insert_index],
                    &index->sorted_array[insert_index+1],
                    URTDB_IndexEntry_g_size*(record_count-1));
            index->sorted_array[index->record_count-1].record = NULL;
            index->last_changed_index = insert_index;
            --index->record_count;
        }
    }
    else if (!remove_record && (retcode == DB_RETCODE_NOT_EXISTS))
    {
        /* urtdb_table->record_count _will be the _last_ index */
        if (insert_index != (index->record_count))
        {
#ifdef URTDB__DEBUG
            OSAPI_Stdio_printf("Add: copying %d from %d to %d\n",
                    record_count,
                    insert_index,
                    insert_index+1);
#endif
            OSAPI_Memory_move(&index->sorted_array[insert_index+1],
                    &index->sorted_array[insert_index],
                    URTDB_IndexEntry_g_size*(record_count));
        }
#ifdef URTDB__DEBUG
        OSAPI_Stdio_printf("Add: new record @ %d\n",insert_index);
#endif
        index->sorted_array[insert_index].record = new_record;
        index->last_changed_index = insert_index;
        ++index->record_count;
    }
}

RTI_PRIVATE void
URTDB_Table_update_indices(struct DB_Table *urtdb_table,
                          struct URTDB_RecordHeader *new_record,
                          RTI_BOOL remove_record)
{
    struct DB_Index *a_index;

    /* IMPORTANT: Always remove from the default record first to ensure that
     * the indices doesn't reuse that record for a substitute record.
     */
    URTDB_Table_update_index(urtdb_table,&urtdb_table->default_index,
                            new_record,remove_record);

    a_index = (struct DB_Index*)REDA_CircularList_get_first(
                                                &urtdb_table->index_in_use);
    while (!REDA_CircularList_node_at_head(&urtdb_table->index_in_use,
                                           &a_index->node))
    {
        URTDB_Table_update_index(urtdb_table,a_index,new_record,remove_record);
        a_index = (struct DB_Index*)REDA_CircularListNode_get_next(
                                                            &a_index->node);
    }

}

RTI_PRIVATE void
URTDB_Cursor_update(struct DB_Cursor *cursor,
                   RTI_BOOL index_removed)
{
    RTI_INT32 changed_index;

    if (cursor->index == NULL)
    {
        return;
    }

    changed_index = cursor->index->last_changed_index;

    if (changed_index > cursor->last_index)
    {
        return;
    }

    if (index_removed)
    {
        --cursor->last_index;
        if (changed_index < cursor->first_index)
        {
            --cursor->first_index;
        }
        if (changed_index <= cursor->current_index)
        {
            --cursor->current_index;
        }
    }
    else
    {
        if (changed_index < cursor->first_index)
        {
            /* If the inserted index is below the first record in the result set
             * it is guaranteed to not be part of the selection, so move cursors
             */
            ++cursor->first_index;
            ++cursor->current_index;
            ++cursor->last_index;
        }
        else if ((changed_index >= cursor->first_index) &&
                 (changed_index < cursor->current_index))
        {
            /* If an inserted record modifies part of the selection that
             * has already been iterated over, invalidate the cursor
             */
            cursor->index = NULL;
        }
        else if ((changed_index >= cursor->current_index) &&
                 (changed_index <= cursor->last_index))
        {
            ++cursor->last_index;
        }
    }
}

RTI_PRIVATE void
URTDB_Table_update_cursors(struct DB_Table *urtdb_table,
                          RTI_BOOL index_removed)
{
    struct DB_Cursor *a_cursor;

    a_cursor = (struct DB_Cursor*)REDA_CircularList_get_first(
                                                &urtdb_table->cursors_in_use);

    while (!REDA_CircularList_node_at_head(&urtdb_table->cursors_in_use,
                                           &a_cursor->node))
    {
        URTDB_Cursor_update(a_cursor,index_removed);
        a_cursor = (struct DB_Cursor*)REDA_CircularListNode_get_next(
                                                            &a_cursor->node);
    }
}

RTI_PRIVATE DB_ReturnCode_T
URTDB_Table_select(struct DB_Table *table,
                          DB_Index_T index,
                          DB_Cursor_T *eh,
                          DB_Select_T opcode,
                          void *arg0,void *arg1,void *arg2)
{
    struct DB_Cursor *cursor;
    DB_ReturnCode_T retcode;
    RTI_UINT32 lower_index;
    RTI_UINT32 upper_index;

    cursor = (struct DB_Cursor*)REDA_BufferPool_get_buffer(table->cursors);
    if (cursor == NULL)
    {
        return DB_RETCODE_OUT_OF_RESOURCES;
    }

    REDA_CircularListNode_init(&cursor->node);
    REDA_CircularList_link_node_after(&table->cursors_in_use,
                                     &cursor->node);
    *eh = cursor;
    cursor->index = index;
    if (opcode == DB_SELECTOPCODE_ALL)
    {
        cursor->first_index = 0;
        cursor->current_index = 0;
        cursor->last_index = index->record_count-1;
    }
    else if (opcode == DB_SELECTOPCODE_BETWEEN)
    {
        cursor->first_index = 0;
        cursor->current_index = 0;
        cursor->last_index = index->record_count-1;
        retcode = URTDB_Table_search_index(table,
                            index,
                            arg0,
                            arg1,
                            DB_SELECT_OP2_AS_KEY | DB_SELECT_OP3_AS_KEY,
                            NULL,
                            &lower_index,
                            &upper_index);

        if (retcode != DB_RETCODE_EXISTS)
        {
            REDA_CircularList_unlink_node(&cursor->node);
            REDA_BufferPool_return_buffer(table->cursors,cursor);
            fail_with_retcode(DB_RETCODE_NOT_EXISTS);
        }
        cursor->first_index = lower_index;
        cursor->current_index = lower_index;
        cursor->last_index = upper_index;
    }

    retcode = DB_RETCODE_OK;

done:
    return retcode;
}

/*******************************************************************************
 *                                  Public API
 ******************************************************************************/
DB_ReturnCode_T
DB_Database_create(DB_Database_T *db,
                  const char *name,
                  struct DB_DatabaseProperty *db_property,
                  struct OSAPI_Mutex *const shared_lock)
{
    DB_ReturnCode_T retcode = DB_RETCODE_OK;
    struct DB_Database *urtdb = NULL;
    struct REDA_BufferPoolProperty pool_property =
                                            REDA_BufferPoolProperty_INITIALIZER;

    LOG_PRECONDITION_P8(((db == NULL) || (*db != NULL) || (db_property == NULL) || (name == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
          ((db == NULL)?0:OSAPI_LOG_PTR_HIGH(db)),
          ((db == NULL)?0:OSAPI_LOG_PTR_LOW(db)),
          ((db == NULL)?0:OSAPI_LOG_PTR_HIGH(*db)),
          ((db == NULL)?0:OSAPI_LOG_PTR_LOW(*db)),
          ((name == NULL)?0:OSAPI_LOG_PTR_HIGH(name)),
		  ((name == NULL)?0:OSAPI_LOG_PTR_LOW(name)),
          ((db_property == NULL)?0:OSAPI_LOG_PTR_HIGH(db_property)),
		  ((db_property == NULL)?0:OSAPI_LOG_PTR_LOW(db_property)))

    *db = NULL;
    if ((REDA_String_length(name) > URTDB_DATABASE_NAME_MAX_LENGTH - 1))
    {
        *db = NULL;
        retcode = DB_RETCODE_BAD_PARAMETER;
        goto done;
    }

    if (db_property->max_tables == 0)
    {
        retcode = DB_RETCODE_BAD_PARAMETER;
        goto done;
    }

    if (((db_property->lock_mode == DB_LOCK_LEVEL_SHARED) &&
            shared_lock == NULL) ||
            ((db_property->lock_mode != DB_LOCK_LEVEL_SHARED) &&
                        shared_lock != NULL))
    {
        return DB_RETCODE_BAD_PARAMETER;
    }

    OSAPI_Heap_allocateStructure(&urtdb,struct DB_Database);

    if (!urtdb)
    {
        retcode = DB_RETCODE_OUT_OF_RESOURCES;
        goto done;
    }

    urtdb->property = *db_property;
    urtdb->tables = NULL;
    urtdb->lock = NULL;
    REDA_CircularList_init(&urtdb->tables_in_use);
    OSAPI_Memory_copy((void *)urtdb->name,
                     (const void *)name, REDA_String_length(name) + 1);

    if (db_property->lock_mode > DB_LOCK_LEVEL_SHARED)
    {
        urtdb->lock = OSAPI_Mutex_new();
        if (urtdb->lock == NULL)
        {
            retcode = DB_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }
    }
    else if (db_property->lock_mode == DB_LOCK_LEVEL_SHARED)
    {
        urtdb->lock = shared_lock;
    }

    pool_property.buffer_size = sizeof(struct DB_Table);
    pool_property.max_buffers = urtdb->property.max_tables;
    urtdb->tables = REDA_BufferPool_new(&pool_property,
                                       URTDB_Table_initialize,
                                       urtdb,
                                       URTDB_Table_finalize,
                                       urtdb);
    if (urtdb->tables == NULL)
    {
        goto done;
    }

    retcode = DB_RETCODE_OK;
    *db = urtdb;

done:

    if (retcode != DB_RETCODE_OK)
    {
        if (urtdb != NULL)
        {
            DB_Database_delete(urtdb);
        }
    }

    return retcode;
}

DB_ReturnCode_T
DB_Database_delete(DB_Database_T db)
{
    DB_ReturnCode_T retcode = DB_RETCODE_OK;
    struct DB_Table *table;
    struct DB_Table *table_next;

    LOG_PRECONDITION((db == NULL),return DB_RETCODE_BAD_PARAMETER)

    table = (struct DB_Table *)REDA_CircularList_get_first(
                                                        &db->tables_in_use);

    while (!REDA_CircularList_node_at_head(&db->tables_in_use,&table->node))
    {
        table_next = (struct DB_Table *)REDA_CircularListNode_get_next(
                                                                &table->node);
        retcode = DB_Database_delete_table(db,table);
        if (retcode != DB_RETCODE_OK)
        {
            return DB_RETCODE_EXISTS;
        }
        table = table_next;
    }

    if (!REDA_CircularList_is_empty(&db->tables_in_use))
    {
        return DB_RETCODE_EXISTS;
    }

    if (db->tables != NULL)
    {
        REDA_BufferPool_delete(db->tables);
    }

    if ((db->property.lock_mode != DB_LOCK_LEVEL_SHARED) &&
        (db->lock != NULL))
    {
        OSAPI_Mutex_delete(db->lock);
    }

    OSAPI_Heap_freeStructure(db);

    return retcode;
}

DB_ReturnCode_T
DB_Database_lock(DB_Database_T db)
{
    LOG_PRECONDITION((db == NULL),return DB_RETCODE_BAD_PARAMETER)

    if (db->lock && !OSAPI_Mutex_take(db->lock))
    {
        return DB_RETCODE_ERROR;
    }
    return DB_RETCODE_OK;
}

DB_ReturnCode_T
DB_Database_unlock(DB_Database_T db)
{
    LOG_PRECONDITION((db == NULL),return DB_RETCODE_BAD_PARAMETER)

    if (db->lock && !OSAPI_Mutex_give(db->lock))
    {
        return DB_RETCODE_ERROR;
    }

    return DB_RETCODE_OK;
}


DB_ReturnCode_T
DB_Database_create_table(DB_Table_T *table,
                        DB_Database_T db,
                        const char *name,
                        RTI_SIZE_T record_size,
                        DB_IndexHash_T hash_func,
                        DB_IndexCompare_T compare_func,
                        struct DB_TableProperty *property)
{
    DB_ReturnCode_T retcode = DB_RETCODE_OK;
    struct DB_Table *out_table;
    struct REDA_BufferPoolProperty pool_property =
                                            REDA_BufferPoolProperty_INITIALIZER;
    struct DB_IndexProperty index_property = DB_IndexProperty_INITIALIZER;

    LOG_PRECONDITION_P6(((table == NULL) || (*table != NULL) ||
                         (db == NULL) || (property == NULL) ||
                         (name == NULL) || (compare_func == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                      OSAPI_LOG_PTR_LOW(table),
                      ((table == NULL)?0:OSAPI_LOG_PTR_LOW(*table)),
                      OSAPI_LOG_PTR_LOW(db),
                      OSAPI_LOG_PTR_LOW(property),
                      OSAPI_LOG_PTR_LOW(name),
                      OSAPI_LOG_PTR_LOW(compare_func))

    if (REDA_String_length(name) > URTDB_TABLE_NAME_MAX_LENGTH - 1)
    {
        fail_with_retcode(DB_RETCODE_BAD_PARAMETER);
    }

    if (property->max_records == 0)
    {
        fail_with_retcode(DB_RETCODE_BAD_PARAMETER);
    }

    out_table = (struct DB_Table*)REDA_CircularList_get_first(&db->tables_in_use);


    while (!REDA_CircularList_node_at_head(&db->tables_in_use,&out_table->node))
    {
        if (!OSAPI_Memory_compare(out_table->name,name,REDA_String_length(name)))
        {
            break;
        }
        out_table = (struct DB_Table*)REDA_CircularListNode_get_next(&out_table->node);
    }

    if (out_table != (struct DB_Table*)&db->tables_in_use)
    {
        fail_with_retcode(DB_RETCODE_EXISTS);
    }

    out_table = (struct DB_Table*)REDA_BufferPool_get_buffer(db->tables);
    if (out_table == NULL)
    {
        fail_with_retcode(DB_RETCODE_OUT_OF_RESOURCES);
    }

    out_table->property = *property;
    OSAPI_Memory_copy((void *)out_table->name, (const void *)name,
                         REDA_String_length(name) + 1);


    pool_property.buffer_size = URTDB_RECORDER_HEADER_SIZE + record_size;
    pool_property.max_buffers = out_table->property.max_records;
    out_table->records = REDA_BufferPool_new(&pool_property,NULL,NULL,NULL,NULL);
    if (out_table->records == NULL)
    {
        fail_with_retcode(DB_RETCODE_OUT_OF_RESOURCES);
    }

    out_table->indices = NULL;
    if (out_table->property.max_indices > 0)
    {
        pool_property.buffer_size = sizeof(struct DB_Index);
        pool_property.max_buffers = out_table->property.max_indices;
        pool_property.flags = REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC;
        out_table->indices = REDA_BufferPool_new(&pool_property,
                                                NULL,NULL,NULL,NULL);
        if (out_table->indices == NULL)
        {
            fail_with_retcode(DB_RETCODE_OUT_OF_RESOURCES);
        }
    }

    out_table->cursors = NULL;
    if (out_table->property.max_cursors > 0)
    {
        pool_property.buffer_size = sizeof(struct DB_Cursor);
        pool_property.max_buffers = out_table->property.max_cursors;
        pool_property.flags = REDA_BUFFERPOOL_FLAG_ARRAY_ALLOC;
        out_table->cursors = REDA_BufferPool_new(&pool_property,
                                                NULL,NULL,NULL,NULL);
        if (out_table->cursors == NULL)
        {
            fail_with_retcode(DB_RETCODE_OUT_OF_RESOURCES);
        }
    }

    REDA_CircularList_init(&out_table->index_in_use);
    REDA_CircularList_init(&out_table->records_in_use);
    REDA_CircularList_init(&out_table->cursors_in_use);
    REDA_CircularListNode_init(&out_table->node);

    index_property.max_entries = out_table->property.max_records;
    index_property.unique = RTI_TRUE;
    URTDB_Index_initialize(&out_table->default_index,
                          hash_func,compare_func,
                          &index_property);
    REDA_CircularList_link_node_after(&db->tables_in_use,&out_table->node);

    *table = (DB_Table_T) out_table;
    retcode = DB_RETCODE_OK;

done:

    if (retcode != DB_RETCODE_OK)
    {
        if (*table != NULL)
        {
            DB_Database_delete_table(db,*table);
            *table = NULL;
        }
    }

    return retcode;
}

DBDllExport DB_ReturnCode_T
DB_Database_delete_table(DB_Database_T db,DB_Table_T table)
{
    DB_ReturnCode_T retcode = DB_RETCODE_NOT_EXISTS;

    LOG_PRECONDITION_P2(((table == NULL) || (db == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(db),
                        OSAPI_LOG_PTR_LOW(table))

    if (!REDA_CircularListNode_is_linked(&table->node))
    {
        return DB_RETCODE_BAD_PARAMETER;
    }

    if (!REDA_CircularList_is_empty(&table->records_in_use))
    {
        return DB_RETCODE_EXISTS;
    }

    if (!REDA_CircularList_is_empty(&table->cursors_in_use))
    {
        return DB_RETCODE_EXISTS;
    }

    if (!REDA_CircularList_is_empty(&table->index_in_use))
    {
        return DB_RETCODE_EXISTS;
    }

    if ((table->records) &&
        !REDA_BufferPool_delete(table->records))
    {
        fail_with_retcode(DB_RETCODE_EXISTS);
    }

    if ((table->indices) &&
        !REDA_BufferPool_delete(table->indices))
    {
        fail_with_retcode(DB_RETCODE_EXISTS);
    }

    if ((table->cursors) &&
        !REDA_BufferPool_delete(table->cursors))
    {
        fail_with_retcode(DB_RETCODE_EXISTS);
    }

    URTDB_Index_finalize(&table->default_index);
    REDA_CircularList_unlink_node(&table->node);
    REDA_BufferPool_return_buffer(db->tables,table);

    retcode = DB_RETCODE_OK;

done:

    return retcode;
}

/*************************** Table Functions **********************************/

DB_ReturnCode_T
DB_Table_create_record(DB_Table_T table,DB_Record_T *record)
{
    DB_ReturnCode_T retcode = DB_RETCODE_ERROR;
    struct URTDB_RecordHeader *internal_record;

    LOG_PRECONDITION_P3(((table == NULL) || (record == NULL) || (*record != NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(table),
                        OSAPI_LOG_PTR_LOW(record),
                        (record==NULL?0:OSAPI_LOG_PTR_LOW(*record)))

    internal_record = (struct URTDB_RecordHeader*)REDA_BufferPool_get_buffer(
                                                            table->records);
    if (internal_record == NULL)
    {
        fail_with_retcode(DB_RETCODE_OUT_OF_RESOURCES);
    }

    REDA_CircularListNode_init(&internal_record->node);
    internal_record->ref_count = 0;
    *record = (DB_Record_T)&internal_record[1];

    retcode = DB_RETCODE_OK;

done:
    return retcode;
}

DB_ReturnCode_T
DB_Table_insert_record(DB_Table_T table, DB_Record_T record)
{
    struct URTDB_RecordHeader *urdb_record = (struct URTDB_RecordHeader *)record;
    DB_ReturnCode_T retcode = DB_RETCODE_ERROR;
    RTI_UINT32 v_index;

    LOG_PRECONDITION_P2(((record == NULL) || (table == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(table),
                        OSAPI_LOG_PTR_LOW(record))

    --urdb_record;
    retcode = URTDB_Table_search_index(table,
                                      &table->default_index,
                                      urdb_record,
                                      NULL,
                                      0,
                                      NULL,
                                      &v_index,
                                      NULL);


    if (retcode != DB_RETCODE_NOT_EXISTS)
    {
        fail_with_retcode(retcode);
    }

    REDA_CircularList_link_node_after(&table->records_in_use,
                                     &urdb_record->node);
    URTDB_Table_update_indices(table,urdb_record,0);
    URTDB_Table_update_cursors(table,RTI_FALSE);

    retcode = DB_RETCODE_OK;

done:
    return retcode;
}

DB_ReturnCode_T
DB_Table_delete_record(DB_Table_T table,DB_Record_T record)
{
    DB_ReturnCode_T retcode = DB_RETCODE_ERROR;
    struct URTDB_RecordHeader *urdb_record = (struct URTDB_RecordHeader *)record;
    struct URTDB_RecordHeader *existing_record;
    RTI_UINT32 v_index;

    LOG_PRECONDITION_P2(((record == NULL) || (table == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(table),
                        OSAPI_LOG_PTR_LOW(record))

    --urdb_record;
    if (REDA_CircularListNode_is_linked(&urdb_record->node))
    {
        retcode = URTDB_Table_search_index(table,
                                          &table->default_index,
                                          urdb_record,
                                          NULL,
                                          0,
                                          &existing_record,
                                          &v_index,
                                          NULL);

        if (retcode != DB_RETCODE_EXISTS)
        {
            fail_with_retcode(DB_RETCODE_NOT_EXISTS);
        }

        URTDB_Table_update_indices(table,urdb_record,1);
        URTDB_Table_update_cursors(table,RTI_TRUE);
        REDA_CircularList_unlink_node(&urdb_record->node);
    }

    REDA_BufferPool_return_buffer(table->records,urdb_record);

    retcode = DB_RETCODE_OK;

done:
    return retcode;
}

DB_ReturnCode_T
DB_Table_remove_record(DB_Table_T table,DB_Record_T *record,DB_Key_T key)
{
    DB_ReturnCode_T retcode = DB_RETCODE_OK;
    struct URTDB_RecordHeader *existing_record;
    RTI_UINT32 v_index;

    LOG_PRECONDITION_P4(((record == NULL) || (table == NULL) || (key == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(table),
                        OSAPI_LOG_PTR_LOW(record),
                        (record==NULL?0:OSAPI_LOG_PTR_LOW(*record)),
                        OSAPI_LOG_PTR_LOW(key))

    retcode = URTDB_Table_search_index(table,
                                      &table->default_index,
                                      key,
                                      NULL,
                                      DB_SELECT_OP2_AS_KEY,
                                      &existing_record,
                                      &v_index,
                                      NULL);

    if (retcode != DB_RETCODE_EXISTS)
    {
        fail_with_retcode(DB_RETCODE_NOT_EXISTS);
    }

    URTDB_Table_update_indices(table,existing_record,1);
    URTDB_Table_update_cursors(table,RTI_TRUE);
    REDA_CircularList_unlink_node(&existing_record->node);
    *record = &existing_record[1];
    retcode = DB_RETCODE_OK;

done:
    return retcode;
}

DB_ReturnCode_T
DB_Table_create_index(DB_Table_T table,
                      DB_Index_T *index,
                      DB_IndexHash_T hash_func,
                      DB_IndexCompare_T compare_func,
                      const struct DB_IndexProperty *const property)
{
    struct DB_Index *a_index = NULL;
    RTI_UINT32 i;
    struct DB_IndexProperty real_property;

    LOG_PRECONDITION_P4(((index == NULL) || (property == NULL) ||
                        (compare_func == NULL) || (table == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(index),
                        OSAPI_LOG_PTR_LOW(property),
                        OSAPI_LOG_PTR_LOW(compare_func),
                        OSAPI_LOG_PTR_LOW(table))

    a_index = (struct DB_Index *)REDA_BufferPool_get_buffer(table->indices);
    if (a_index == NULL)
    {
        return DB_RETCODE_OUT_OF_RESOURCES;
    }

    real_property = *property;
    if (real_property.max_entries == 0)
    {
        real_property.max_entries = table->property.max_records;
    }

    if (!URTDB_Index_initialize(a_index,hash_func,compare_func,&real_property))
    {
        REDA_BufferPool_return_buffer(table->indices,a_index);
        return DB_RETCODE_OUT_OF_RESOURCES;
    }

    /* Add each new record to the new index
     */
    for (i = 0; i < table->default_index.record_count; i++)
    {
        URTDB_Table_update_index(table,a_index,
                                table->default_index.sorted_array[i].record,
                                RTI_FALSE);
    }

    REDA_CircularList_link_node_after(&table->index_in_use,&a_index->node);

    *index = a_index;

    return DB_RETCODE_OK;
}

DB_ReturnCode_T
DB_Table_delete_index(DB_Table_T table,DB_Index_T index)
{
    LOG_PRECONDITION_P2(((index == NULL) || (table == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(index),
                        OSAPI_LOG_PTR_LOW(table))

    REDA_CircularList_unlink_node(&index->node);
    URTDB_Index_finalize(index);
    REDA_BufferPool_return_buffer(table->indices,index);

    return DB_RETCODE_OK;
}

DB_ReturnCode_T
DB_Table_select_all(DB_Table_T table,DB_Index_T index,DB_Cursor_T *eh)
{
    LOG_PRECONDITION_P2(((table == NULL) || (eh == NULL)),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(table),
                        OSAPI_LOG_PTR_LOW(eh))

    if (index == DB_TABLE_DEFAULT_INDEX)
    {
        index = &table->default_index;
    }

    return URTDB_Table_select(table,index,eh,
                             DB_SELECTOPCODE_ALL,NULL,NULL,NULL);
}

DB_ReturnCode_T
DB_Table_select_match(DB_Table_T table,DB_Index_T index,
                      DB_Record_T *record,DB_Key_T key)
{
    struct URTDB_RecordHeader *urdb_record = NULL;
    DB_ReturnCode_T retcode = DB_RETCODE_ERROR;

    LOG_PRECONDITION_P3((table == NULL || record == NULL || key == NULL),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(table),
                        OSAPI_LOG_PTR_LOW(record),
                        OSAPI_LOG_PTR_LOW(key))

    if (index == DB_TABLE_DEFAULT_INDEX)
    {
        index = &table->default_index;
    }

    retcode = URTDB_Table_search_index(table,
                                      index,
                                      key,
                                      NULL,
                                      DB_SELECT_OP2_AS_KEY,
                                      &urdb_record,
                                      NULL,
                                      NULL);

    if (retcode != DB_RETCODE_EXISTS)
    {
        fail_with_retcode(retcode);
    }

    *record = &urdb_record[1];
    retcode = DB_RETCODE_OK;

done:
    return retcode;
}

DB_ReturnCode_T
DB_Table_select_range(DB_Table_T table,
                      DB_Index_T index,
                      DB_Cursor_T *eh,
                      DB_Key_T lower,
                      DB_Key_T upper)
{
    LOG_PRECONDITION_P4((table==NULL || eh==NULL || lower==NULL || upper==NULL),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(table),
                        OSAPI_LOG_PTR_LOW(eh),
                        OSAPI_LOG_PTR_LOW(lower),
                        OSAPI_LOG_PTR_LOW(upper))

    if (index == DB_TABLE_DEFAULT_INDEX)
    {
        index = &table->default_index;
    }

    return URTDB_Table_select(table,index,eh,
                             DB_SELECTOPCODE_BETWEEN,lower,upper,NULL);
}

RTI_INT32
DB_Cursor_get_count(DB_Cursor_T cursor)
{
    LOG_PRECONDITION(cursor==NULL,return -1);
    if (cursor->index == NULL)
    {
        return -1;
    }
    else
    {
        return (cursor->last_index - cursor->first_index  + 1);
    }
}

DB_ReturnCode_T
DB_Cursor_get_next(DB_Cursor_T cursor, DB_Record_T *record)
{
    LOG_PRECONDITION_P2((cursor==NULL || record==NULL),
                        return DB_RETCODE_BAD_PARAMETER,
                        OSAPI_LOG_PTR_LOW(cursor),
                        OSAPI_LOG_PTR_LOW(record))

    if (cursor->index == NULL)
    {
        return DB_RETCODE_BAD_PARAMETER;
    }

    if (cursor->current_index > cursor->last_index)
    {
        return DB_RETCODE_NO_DATA;
    }

    *record = &cursor->index->sorted_array[cursor->current_index].record[1];
    ++cursor->current_index;

    return DB_RETCODE_OK;
}

void
DB_Cursor_finish(DB_Table_T table,DB_Cursor_T cursor)
{
    LOG_PRECONDITION_P2((table==NULL || cursor==NULL),
                        return,
                        OSAPI_LOG_PTR_LOW(table),
                        OSAPI_LOG_PTR_LOW(cursor))

    cursor->index = NULL;
    if (REDA_CircularListNode_is_linked(&cursor->node))
    {
        REDA_CircularList_unlink_node(&cursor->node);
        REDA_BufferPool_return_buffer(table->cursors,cursor);
    }
}

#undef fail_with_retcode
