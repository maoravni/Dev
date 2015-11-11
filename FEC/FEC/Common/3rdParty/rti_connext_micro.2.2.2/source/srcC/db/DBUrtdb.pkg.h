/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DBUrtdb_pkg_h
#define DBUrtdb_pkg_h


#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef reda_circularlist_h
#include "reda/reda_circularlist.h"
#endif

struct URTDB_RecordHeader
{
    REDA_CircularListNode_T node;
    RTI_UINT32 ref_count;
};

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

#endif /* DBUrtdb_pkg_h */
