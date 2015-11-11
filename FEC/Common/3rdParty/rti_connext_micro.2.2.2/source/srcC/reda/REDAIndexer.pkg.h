/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef REDAIndexer_pkg_h
#define REDAIndexer_pkg_h


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

#endif /* REDAIndexer_pkg_h */
