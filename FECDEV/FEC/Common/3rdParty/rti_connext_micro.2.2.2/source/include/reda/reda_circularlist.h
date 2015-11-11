/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_circularlist_h
#define reda_circularlist_h


#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct REDA_CircularListNode
{
    struct REDA_CircularListNode *_prev;
    struct REDA_CircularListNode *_next;
};

struct REDA_CircularListMixInNode
{
    struct REDA_CircularListNode *_prev;
    struct REDA_CircularListNode *_next;
    void *_data;
};

typedef struct REDA_CircularListNode REDA_CircularList_T;
typedef struct REDA_CircularListNode REDA_CircularListNode_T;

#define REDA_CircularListMixInNode_get_data(node_) (node_)->_data

REDADllExport void
REDA_CircularList_init(REDA_CircularList_T *list);

REDADllExport void
REDA_CircularListNode_init(struct REDA_CircularListNode *node);

REDADllExport void
REDA_CircularListMixInNode_init(struct REDA_CircularListMixInNode *node,void *data);

REDADllExport void
REDA_CircularList_link_node_after(struct REDA_CircularListNode *after,
                                  struct REDA_CircularListNode *node);

REDADllExport void
REDA_CircularList_unlink_node(struct REDA_CircularListNode *node);

#if 0
#define REDA_CircularList_init(c_list_) \
{\
    (c_list_)->_next = (c_list_);\
    (c_list_)->_prev = (c_list_);\
}

#define REDA_CircularListNode_init(node_) \
{\
  (node_)->_next = NULL;\
  (node_)->_prev = NULL;\
}

#define REDA_CircularListMixInNode_init(node_,data_) \
{\
  (node_)->_next = NULL;\
  (node_)->_prev = NULL;\
  (node_)->_data = data_;\
}

#define REDA_CircularList_link_node_after(after_,node_) \
{\
  struct REDA_CircularListNode *t_;\
  t_ = (after_);\
  (node_)->_next = (t_)->_next; \
  (t_)->_next = (node_);\
  (node_)->_next->_prev = (node_); \
  (node_)->_prev = (t_); \
}

#define REDA_CircularList_unlink_node(node_) \
(node_)->_prev->_next = (node_)->_next; \
(node_)->_next->_prev = (node_)->_prev; \
(node_)->_next = NULL; \
(node_)->_prev = NULL; \

#endif

#define REDA_CircularList_append(list_,node_) \
        REDA_CircularList_link_node_after((list_)->_prev,node_)

#define REDA_CircularList_prepend(list_,node_) \
        REDA_CircularList_link_node_after((list_),node_)


#define REDA_CircularList_is_empty(c_list_) \
  ((c_list_)->_next == (c_list_) ? 1 : 0)

#define REDA_CircularList_node_at_head(c_list_,c_node_) \
((c_list_) == (c_node_))

#define REDA_CircularList_get_first(c_list_) ((c_list_)->_next)
#define REDA_CircularList_get_last(c_list_) ((c_list_)->_prev)
#define REDA_CircularListNode_get_next(c_node_) ((c_node_)->_next)
#define REDA_CircularListNode_get_prev(c_node_) ((c_node_)->_prev)

#define REDA_CircularListNode_is_linked(c_node_) \
        (((c_node_)->_next || (c_node_)->_prev) ? 1 : 0)

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* reda_circularlist_h */
