/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef db_api_h
#define db_api_h


#ifndef urtdb_dll_h
#include "db/db_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct DB_Database;
typedef struct DB_Database* DB_Database_T;

struct DB_Table;
typedef struct DB_Table *DB_Table_T;

struct DB_Record;
typedef void *DB_Record_T;

typedef void* DB_Key_T;

struct DB_Cursor;
typedef struct DB_Cursor *DB_Cursor_T;

struct DB_Index;
typedef struct DB_Index *DB_Index_T;

/****************/
#if 0
typedef void* DB_Database_T;
typedef void* DB_Table_T;
typedef void* DB_Record_T;
typedef void* DB_Key_T;
typedef void* DB_Cursor_T;
typedef void* DB_Index_T;
#endif

#define URTDB_DATABASE_NAME_MAX_LENGTH 16
#define URTDB_TABLE_NAME_MAX_LENGTH    16

typedef enum
{
    DB_RETCODE_OK = 0,
    DB_RETCODE_BAD_PARAMETER,
    DB_RETCODE_EXISTS,
    DB_RETCODE_NOT_EXISTS,
    DB_RETCODE_NO_DATA,
    DB_RETCODE_OUT_OF_RESOURCES,
    DB_RETCODE_ERROR
} DB_ReturnCode_T;

typedef enum
{
    DB_LOCK_LEVEL_NONE   = 0,
    DB_LOCK_LEVEL_SHARED = 1,
    DB_LOCK_LEVEL_GLOBAL = 2
} DB_LockLevel_T;

typedef enum
{
    DB_SELECTOPCODE_EQUAL   = 1,
    DB_SELECTOPCODE_BETWEEN = 2,
    DB_SELECTOPCODE_ALL     = 3
} DB_Select_T;

struct DB_DatabaseProperty
{
    RTI_SIZE_T max_tables;
    DB_LockLevel_T lock_mode;
};

#define DB_DatabaseProperty_INITIALIZER \
{\
    1,\
    DB_LOCK_LEVEL_NONE\
}

#define DB_SELECT_OP2_AS_KEY (0x1)
#define DB_SELECT_OP3_AS_KEY (0x2)
#define DB_SELECT_OP2_IS_KEY(flags) (flags & DB_SELECT_OP2_AS_KEY)
#define DB_SELECT_OP3_IS_KEY(flags) (flags & DB_SELECT_OP3_AS_KEY)

/* Invariant:
 * op1 - Always DB_Record_T
 * op2 - DB_Record_T or DB_Key_T (passed transparently from the user)
 * op3 - NULL, DB_Record_T or DB_Key_T (passed transparently from the user)
 */
typedef RTI_INT32
(*DB_IndexCompare_T)(DB_Select_T opcode,RTI_INT32 flags,
                  const DB_Record_T op1, void *op2, void *op3);

typedef RTI_UINT32
(*DB_IndexHash_T)(const DB_Record_T a_record,DB_Key_T a_key,RTI_INT32 max_buckets);

struct DB_TableProperty
{
    RTI_SIZE_T max_records;
    RTI_SIZE_T max_indices;
    RTI_SIZE_T max_cursors;
};

#define DB_TableProperty_INITIALIZER \
{\
    0,\
    0,\
    1\
}

struct DB_IndexProperty
{
    RTI_SIZE_T max_entries;
    RTI_BOOL unique;
};

#define DB_IndexProperty_INITIALIZER \
{\
    0,\
    RTI_TRUE\
}

/**************************** Database Functions ******************************/

DBDllExport DB_ReturnCode_T
DB_Database_create(DB_Database_T *db,
                  const char *name,
                  struct DB_DatabaseProperty *property,
                  OSAPI_Mutex_T *const shared_lock);

DBDllExport DB_ReturnCode_T
DB_Database_delete(DB_Database_T db);

DBDllExport DB_ReturnCode_T
DB_Database_lock(DB_Database_T db);

DBDllExport DB_ReturnCode_T
DB_Database_unlock(DB_Database_T db);

DBDllExport DB_ReturnCode_T
DB_Database_create_table(DB_Table_T *table,
                         DB_Database_T db,
                         const char *name,
                         RTI_SIZE_T record_size,
                         DB_IndexHash_T hash_func,
                         DB_IndexCompare_T compare_func,
                         struct DB_TableProperty *property);

#define DB_Database_create_table_type(t_,db_,name_,TYPE,prop_) \
        DB_Database_create_table(t_,db_,name_,sizeof(TYPE),prop_)

DBDllExport DB_ReturnCode_T
DB_Database_delete_table(DB_Database_T db,DB_Table_T table);

/*************************** Table Functions **********************************/

DBDllExport DB_ReturnCode_T
DB_Table_create_record(DB_Table_T table,DB_Record_T *record);

DBDllExport DB_ReturnCode_T
DB_Table_insert_record(DB_Table_T table, DB_Record_T record);

DB_ReturnCode_T
DB_Table_remove_record(DB_Table_T table,DB_Record_T *record,DB_Key_T key);

DBDllExport DB_ReturnCode_T
DB_Table_delete_record(DB_Table_T table,DB_Record_T record);

#define DB_TABLE_DEFAULT_INDEX 0

DBDllExport DB_ReturnCode_T
DB_Table_create_index(DB_Table_T table,
                      DB_Index_T *index,
                      DB_IndexHash_T hash_func,
                      DB_IndexCompare_T compare_func,
                      const struct DB_IndexProperty *const property);

DBDllExport DB_ReturnCode_T
DB_Table_delete_index(DB_Table_T table,DB_Index_T index);

DBDllExport DB_ReturnCode_T
DB_Table_select_all(DB_Table_T table,DB_Index_T index,DB_Cursor_T *eh);

#define DB_Table_select_all_default(table_,eh_) \
        DB_Table_select_all(table_,DB_TABLE_DEFAULT_INDEX,eh_)

DBDllExport DB_ReturnCode_T
DB_Table_select_match(DB_Table_T table,DB_Index_T index,
                     DB_Record_T *record,DB_Key_T key);

DBDllExport DB_ReturnCode_T
DB_Table_select_range(DB_Table_T table,DB_Index_T index,
                     DB_Cursor_T *eh,
                     DB_Key_T lower,DB_Key_T upper);

DBDllExport RTI_INT32
DB_Cursor_get_count(DB_Cursor_T handle);

DBDllExport DB_ReturnCode_T
DB_Cursor_get_next(DB_Cursor_T handle, DB_Record_T *record);

DBDllExport void
DB_Cursor_finish(DB_Table_T table,DB_Cursor_T handle);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* db_api_h */
