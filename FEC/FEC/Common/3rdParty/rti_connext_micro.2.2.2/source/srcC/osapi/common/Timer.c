/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#include "osapi/osapi_config.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_mutex.h"
#include "osapi/osapi_system.h"
#include "osapi/osapi_timer.h"
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

RTI_PRIVATE const RTI_INT32 OSAPI_TIMER_DEFAULT_MAX_ENTRIES = 32;
RTI_PRIVATE const RTI_INT32 OSAPI_TIMER_NONRECURING_PERIOD = -1;
RTI_PRIVATE const RTI_INT32 OSAPI_TIMER_EPOCH_INVALID = -1;

RTI_PRIVATE void OSAPI_Timer_tick(OSAPI_Timer_t timer);

/* NOTE: We don't use the REDA data-types as REDA is yet not built. However,
 * as most of the functionality is simple macros, not much is lost
 */
struct OSAPI_TimerCircularListNode
{
    struct OSAPI_TimerCircularListNode *_prev;
    struct OSAPI_TimerCircularListNode *_next;
};

typedef struct OSAPI_TimerCircularListNode OSAPI_TimerCircularList_T;

#define OSAPI_TimerCircularList_init(c_list_) \
{\
    (c_list_)->_next = (c_list_);\
    (c_list_)->_prev = (c_list_);\
}

#define OSAPI_TimerCircularListNode_init(node_) \
{\
  (node_)->_next = NULL;\
  (node_)->_prev = NULL;\
}

#define OSAPI_TimerCircularList_link_node_after(prev_node_,node_) \
{\
  struct OSAPI_TimerCircularListNode *t_;\
  t_ = (prev_node_);\
  (node_)->_next = (t_)->_next; \
  (t_)->_next = (node_);\
  (node_)->_next->_prev = (node_); \
  (node_)->_prev = (t_); \
}

#define OSAPI_TimerCircularList_unlink_node(node_) \
{\
    if (((node_)->_next != NULL) && ((node_)->_prev != NULL)) \
    {\
        (node_)->_prev->_next = (node_)->_next; \
        (node_)->_next->_prev = (node_)->_prev; \
        (node_)->_next = NULL; \
        (node_)->_prev = NULL; \
    }\
}

#define OSAPI_TimerCircularList_is_empty(c_list_) \
  ((c_list_)->_next == (c_list_) ? RTI_TRUE : RTI_FALSE)

#define OSAPI_TimerCircularList_get_first(c_list_) ((c_list_)->_next)

#define OSAPI_TimerCircularListNode_get_next(c_node_) ((c_node_)->_next)

#define OSAPI_TIMER_HANDLE_IS_VALID(h_) \
(((h_)->epoch != OSAPI_TIMER_EPOCH_INVALID) && ((h_)->_entry != NULL) && \
 ((h_)->_entry->epoch != OSAPI_TIMER_EPOCH_INVALID) && ((h_)->_entry->epoch == (h_)->epoch))

struct OSAPI_TimerEntry
{
    struct OSAPI_TimerCircularListNode _node;

    /* -1 = one-shot */
    RTI_INT32 period;

    /* times aound the wheel */
    RTI_INT32 wheel_count;

    /* User-data stored with the time-out */
    struct OSAPITimeoutUserData user_data;

    /* In case an antry is re-used */
    RTI_INT32 epoch;

    /* user callback */
    OSAPITimeoutFunction timeout;

    RTI_INT32 flags;
};

struct OSAPI_Timer
{
    OSAPI_TimerCircularList_T *wheel;
    OSAPI_TimerCircularList_T free_head;
    struct OSAPI_TimerProperty property;
    RTI_INT32 current_slot;
    RTI_INT32 epoch;
    struct OSAPI_Mutex *mutex;
    RTI_INT32 resolution;
    RTI_BOOL mutex_is_shared;
};

/*** SOURCE_BEGIN ***/

OSAPI_Timer_t
OSAPI_Timer_new(struct OSAPI_TimerProperty *property, struct OSAPI_Mutex *mutex)
{
    struct OSAPI_Timer *timer;
    RTI_INT32 i, max;
    struct OSAPI_TimerEntry *entry;

    LOG_PRECONDITION((property == NULL), return NULL)

    OSAPI_Heap_allocateStructure(&timer, struct OSAPI_Timer);

    if (timer == NULL)
    {
        OSAPI_Log_error(OSAPI_LOG_TIMER_NEW_FAILED);
        return NULL;
    }

    timer->property = *property;
    OSAPI_Heap_allocateArray(&timer->wheel,
                            property->max_slots, OSAPI_TimerCircularList_T);
    timer->current_slot = 0;
    timer->epoch = 0;
    timer->resolution = OSAPI_System_get_timer_resolution();


    for (i = 0; i < timer->property.max_slots; ++i)
    {
        OSAPI_TimerCircularList_init(&timer->wheel[i]);
    }

    if (timer->property.max_entries > 0)
    {
        max = timer->property.max_entries;
    }
    else
    {
        max = OSAPI_TIMER_DEFAULT_MAX_ENTRIES;
    }

    OSAPI_TimerCircularList_init(&timer->free_head);

    for (i = 0; i < max; ++i)
    {
        OSAPI_Heap_allocateStructure(&entry, struct OSAPI_TimerEntry);
        OSAPI_TimerCircularListNode_init(&entry->_node);
        entry->epoch = 0;
        entry->period = -1;
        entry->wheel_count = 0;
        OSAPI_TimerCircularList_link_node_after(&timer->free_head,
                                               &entry->_node);
    }

    if (mutex == NULL)
    {
        timer->mutex = OSAPI_Mutex_new();
        if (timer->mutex == NULL)
        {
            OSAPI_Log_error(OSAPI_LOG_TIMER_NEW_FAILED);
            (void)OSAPI_Timer_delete(timer);
            return NULL;
        }
        timer->mutex_is_shared = RTI_FALSE;
    }
    else
    {
        timer->mutex = mutex;
        timer->mutex_is_shared = RTI_TRUE;
    }

    if (!OSAPI_System_start_timer(timer, OSAPI_Timer_tick))
    {
        OSAPI_Log_error_p4(OSAPI_LOG_TIMER_NEW_FAILED,
           OSAPI_LOG_PTR_HIGH(timer),OSAPI_LOG_PTR_LOW(timer),
           OSAPI_LOG_PTR_HIGH(OSAPI_Timer_tick),
           OSAPI_LOG_PTR_LOW(OSAPI_Timer_tick));
        (void)OSAPI_Timer_delete(timer);
        return NULL;
    }

    return timer;
}

/* TODO: return type void */
RTI_BOOL
OSAPI_Timer_delete(OSAPI_Timer_t timer)
{
    struct OSAPI_TimerEntry *entry, *entry_next;
    RTI_INT32 slot;

    LOG_PRECONDITION((timer == NULL), return RTI_FALSE)

    if (!OSAPI_System_stop_timer(timer))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_TIMER_DELETE_FAILED,
                           OSAPI_LOG_PTR_HIGH(timer),OSAPI_LOG_PTR_LOW(timer));
        return RTI_FALSE;
    }

    for (slot = 0; slot < timer->property.max_slots; ++slot)
    {
        entry = (struct OSAPI_TimerEntry *)
                     OSAPI_TimerCircularList_get_first(&timer->wheel[slot]);
        while (entry != (struct OSAPI_TimerEntry *)&timer->wheel[slot])
        {
            entry_next = (struct OSAPI_TimerEntry*)
                            OSAPI_TimerCircularListNode_get_next(&entry->_node);
            OSAPI_Heap_freeStructure(entry);
            entry = entry_next;
        }
    }

    entry = (struct OSAPI_TimerEntry *)
                    OSAPI_TimerCircularList_get_first(&timer->free_head);
    while (entry != (struct OSAPI_TimerEntry *)&timer->free_head)
    {
        entry_next = (struct OSAPI_TimerEntry *)
                            OSAPI_TimerCircularListNode_get_next(&entry->_node);
        OSAPI_Heap_freeStructure(entry);
        entry = entry_next;
    }

    if (!timer->mutex_is_shared)
    {
        if (!OSAPI_Mutex_delete(timer->mutex))
        {
            OSAPI_Log_error_p2(OSAPI_LOG_TIMER_DELETE_FAILED,
                   OSAPI_LOG_PTR_HIGH(timer->mutex),
                   OSAPI_LOG_PTR_LOW(timer->mutex));
            return RTI_FALSE;
        }
    }

    OSAPI_Heap_freeArray(timer->wheel);
    OSAPI_Heap_freeStructure(timer);

    return RTI_TRUE;
}

RTI_PRIVATE void
OSAPI_Timer_schedule_entry(OSAPI_Timer_t timer,
                          struct OSAPI_TimerEntry *entry,
                          RTI_INT32 timeout)
{
    OSAPI_TimerCircularList_T *slot_entry;

    entry->wheel_count = timeout / (timer->resolution *
                                    timer->property.max_slots);
    slot_entry = &timer->wheel[(timer->current_slot +
                                (timeout / timer->resolution)) %
                               timer->property.max_slots];
    OSAPI_TimerCircularList_link_node_after(slot_entry->_prev, &entry->_node);
}

RTI_PRIVATE void
OSAPI_Timer_tick(OSAPI_Timer_t timer)
{
    OSAPI_TimerCircularList_T *entry_list;
    struct OSAPI_TimerEntry *entry, *entry_next;
    OSAPITimeoutOp_t timeout_op;

    if (!OSAPI_Mutex_take(timer->mutex))
    {
        OSAPI_Log_error(OSAPI_LOG_TIMER_TICK_FAILED);
        return;
    }

    timer->current_slot = (timer->current_slot + 1) % timer->property.max_slots;
    entry_list = &timer->wheel[timer->current_slot];

    entry = (struct OSAPI_TimerEntry *)
                                 OSAPI_TimerCircularList_get_first(entry_list);
    while (entry != (struct OSAPI_TimerEntry *)entry_list)
    {
        entry_next = (struct OSAPI_TimerEntry*)
                            OSAPI_TimerCircularListNode_get_next(&entry->_node);
        if (entry->wheel_count == 0)
        {
            OSAPI_TimerCircularList_unlink_node(&entry->_node);
            timeout_op = entry->timeout(&entry->user_data);
            if (timeout_op == OSAPI_TIMEOUT_OP_MANUAL)
            {
                entry = entry_next;
                continue;
            }
            if ((entry->period > 0) && (entry->epoch > 0))
            {
                OSAPI_Timer_schedule_entry(timer, entry, entry->period);
            }
        }
        else
        {
            --entry->wheel_count;
        }
        entry = entry_next;
    }

    if (!OSAPI_Mutex_give(timer->mutex))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_TIMER_TICK_FAILED,
             OSAPI_LOG_PTR_HIGH(timer->mutex),OSAPI_LOG_PTR_LOW(timer->mutex));
    }
}

RTI_BOOL
OSAPI_Timer_create_timeout(OSAPI_Timer_t timer,
                          OSAPITimeoutHandle_t *out_handle,
                          RTI_INT32 timeout,
                          RTI_INT32 flags,
                          OSAPITimeoutFunction timeout_handler,
                          struct OSAPITimeoutUserData *user_data)
{
    struct OSAPI_TimerEntry *entry;

    LOG_PRECONDITION_P7((timer == NULL) || (out_handle == NULL) ||
                        (timeout < 0) || (timeout_handler == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_HIGH(timer),
                        OSAPI_LOG_PTR_LOW(timer),
                        OSAPI_LOG_PTR_HIGH(out_handle),
                        OSAPI_LOG_PTR_LOW(out_handle),
                        timeout,
                        OSAPI_LOG_PTR_HIGH(timeout_handler),
                        OSAPI_LOG_PTR_LOW(timeout_handler))

    if (!OSAPI_Mutex_take(timer->mutex))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_TIMER_CREATE_TIMEOUT_FAILED,
             OSAPI_LOG_PTR_HIGH(timer->mutex),OSAPI_LOG_PTR_LOW(timer->mutex));
        return RTI_FALSE;
    }

    if (OSAPI_TimerCircularList_is_empty(&timer->free_head))
    {
        if (!OSAPI_Mutex_give(timer->mutex))
        {
            OSAPI_Log_error_p2(OSAPI_LOG_TIMER_CREATE_TIMEOUT_FAILED,
                 OSAPI_LOG_PTR_HIGH(timer->mutex),OSAPI_LOG_PTR_LOW(timer->mutex));
        }
        return RTI_FALSE;
    }

    entry = (struct OSAPI_TimerEntry *)
                        OSAPI_TimerCircularList_get_first(&timer->free_head);

    OSAPI_TimerCircularList_unlink_node(&entry->_node);

    ++timer->epoch;
    entry->epoch = timer->epoch;
    if (user_data != NULL)
    {
        entry->user_data = *user_data;
    }
    entry->timeout = timeout_handler;
    entry->flags = flags;

    if (entry->flags & OSAPI_TIMER_PERIODIC)
    {
        entry->period = timeout;
    }
    else
    {
        entry->period = OSAPI_TIMER_NONRECURING_PERIOD;
    }

    OSAPI_Timer_schedule_entry(timer, entry, timeout);

    out_handle->epoch = entry->epoch;
    out_handle->_entry = entry;

    if (!OSAPI_Mutex_give(timer->mutex))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_TIMER_CREATE_TIMEOUT_FAILED,
             OSAPI_LOG_PTR_HIGH(timer->mutex),OSAPI_LOG_PTR_LOW(timer->mutex));
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
OSAPI_Timer_update_timeout(OSAPI_Timer_t timer,
                          OSAPITimeoutHandle_t *handle,
                          RTI_INT32 timeout)
{
    RTI_BOOL retval = RTI_FALSE;

    LOG_PRECONDITION_P5((timer == NULL) || (handle == NULL) ||
                        (timeout < 0),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_HIGH(timer),
                        OSAPI_LOG_PTR_LOW(timer),
                        OSAPI_LOG_PTR_HIGH(handle),
                        OSAPI_LOG_PTR_LOW(handle),
                        timeout)

    if (!OSAPI_Mutex_take(timer->mutex))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_TIMER_UPDATE_TIMEOUT_FAILED,
             OSAPI_LOG_PTR_HIGH(timer->mutex),OSAPI_LOG_PTR_LOW(timer->mutex));
        return RTI_FALSE;
    }

    if (OSAPI_TIMER_HANDLE_IS_VALID(handle))
    {
        OSAPI_TimerCircularList_unlink_node(&handle->_entry->_node);

        if (handle->_entry->flags & OSAPI_TIMER_PERIODIC)
        {
            handle->_entry->period = timeout;
        }

        OSAPI_Timer_schedule_entry(timer,handle->_entry,timeout);

        retval = RTI_TRUE;
    }

    if (!OSAPI_Mutex_give(timer->mutex))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_TIMER_UPDATE_TIMEOUT_FAILED,
             OSAPI_LOG_PTR_HIGH(timer->mutex),OSAPI_LOG_PTR_LOW(timer->mutex));
        return RTI_FALSE;
    }

    return retval;
}

RTI_BOOL
OSAPI_Timer_delete_timeout(OSAPI_Timer_t timer, OSAPITimeoutHandle_t *handle)
{
    RTI_BOOL retval = RTI_FALSE;

    LOG_PRECONDITION_P4((timer == NULL) || (handle == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_HIGH(timer),
                        OSAPI_LOG_PTR_LOW(timer),
                        OSAPI_LOG_PTR_HIGH(handle),
                        OSAPI_LOG_PTR_LOW(handle))

    if (!OSAPI_Mutex_take(timer->mutex))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_TIMER_DELETE_TIMEOUT_FAILED,
             OSAPI_LOG_PTR_HIGH(timer->mutex),OSAPI_LOG_PTR_LOW(timer->mutex));
        return RTI_FALSE;
    }

    if (OSAPI_TIMER_HANDLE_IS_VALID(handle))
    {
        OSAPI_TimerCircularList_unlink_node(&handle->_entry->_node);
        OSAPI_TimerCircularList_link_node_after(timer->free_head._prev,
                                               &handle->_entry->_node);
        handle->epoch = OSAPI_TIMER_EPOCH_INVALID;
        handle->_entry->epoch = OSAPI_TIMER_EPOCH_INVALID;

        retval = RTI_TRUE;
    }

    if (!OSAPI_Mutex_give(timer->mutex))
    {
        OSAPI_Log_error_p2(OSAPI_LOG_TIMER_DELETE_TIMEOUT_FAILED,
             OSAPI_LOG_PTR_HIGH(timer->mutex),OSAPI_LOG_PTR_LOW(timer->mutex));
        return RTI_FALSE;
    }

    return retval;
}

RTI_BOOL
OSAPITimeoutHandle_get_user_data(struct OSAPITimeoutUserData *out,
                                 OSAPITimeoutHandle_t *handle)
{
    LOG_PRECONDITION_P4((out == NULL) || (handle == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_HIGH(out),
                        OSAPI_LOG_PTR_LOW(out),
                        OSAPI_LOG_PTR_HIGH(handle),
                        OSAPI_LOG_PTR_LOW(handle))

    if ((handle->_entry != NULL) && (handle->epoch == handle->_entry->epoch))
    {
        *out = handle->_entry->user_data;
        return RTI_TRUE;
    }

    OSAPI_Log_error_p4(OSAPI_LOG_TIMER_GET_USER_DATA_FAILED,
            OSAPI_LOG_PTR_HIGH(handle->_entry),OSAPI_LOG_PTR_LOW(handle->_entry),
            handle->epoch,handle->_entry->epoch);

    return RTI_FALSE;
}
