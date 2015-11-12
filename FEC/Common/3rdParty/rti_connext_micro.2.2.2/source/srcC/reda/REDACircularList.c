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
#ifndef reda_circular_h
#include "reda/reda_circularlist.h"
#endif


#ifndef reda_circular_h
#include "reda/reda_circularlist.h"
#endif


void
REDA_CircularList_init(REDA_CircularList_T *list)
{
    list->_next = list;
    list->_prev = list;
}

void
REDA_CircularListNode_init(struct REDA_CircularListNode *node)
{
  node->_next = NULL;
  node->_prev = NULL;
}

void
REDA_CircularListMixInNode_init(struct REDA_CircularListMixInNode *node,void *data)
{
  node->_next = NULL;
  node->_prev = NULL;
  node->_data = data;
}

void
REDA_CircularList_link_node_after(struct REDA_CircularListNode *after,
                                  struct REDA_CircularListNode *node)
{
    node->_next = after->_next;
    after->_next = node;
    node->_next->_prev = node;
    node->_prev = after;
}

void
REDA_CircularList_unlink_node(struct REDA_CircularListNode *node)
{
    if ((node->_prev == NULL) && (node->_next == NULL))
    {
        return;
    }
    node->_prev->_next = node->_next;
    node->_next->_prev = node->_prev;
    node->_next = NULL;
    node->_prev = NULL;
}

