/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef util_makeheader_h
#include "util/util_makeheader.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_alignment_h
#include "osapi/osapi_alignment.h"
#endif
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef osapi_process_h
#include "osapi/osapi_process.h"
#endif
#ifndef osapi_thread_h
#include "osapi/osapi_thread.h"
#endif
#ifndef osapi_stdio_h
#include "osapi/osapi_stdio.h"
#endif

struct OSAPI_LogMessageSeq
{
    OSAPI_LogMessage_T *_contiguous_buffer;
    RTI_INT32 _maximum;
    RTI_INT32 _length;
    RTI_INT32 _element_size;
};

typedef struct OSAPI_LogImpl
{
    char *write_pointer;
    char *buffer_start;
    RTI_INT32 buffer_size;
    RTI_INT32 log_entry_size;
    struct OSAPI_Mutex *lock;
    RTI_BOOL store_debug_logs;
    RTI_BOOL has_wrapped;
    OSAPI_LogVerbosity_T verbosity;
} OSAPI_LogImpl;

/*i @ingroup OSAPI
 * 
 * @brief   The singleton log buffer instance.
 */
struct OSAPI_LogImpl OSAPI_LogImpl_g_singleton =
{
    NULL,
    NULL,
    0,
    sizeof(OSAPI_LogMessage_T),
    NULL,
    RTI_FALSE,
    RTI_FALSE,
    OSAPI_LOG_VERBOSITY_ERROR
};

/*i @ingroup OSAPI
 * 
 * @brief   The singleton log handler instance.
 */
OSAPI_Log_log_handlerFUNC OSAPI_Log_g_user_handler = NULL;

void *OSAPI_Log_g_user_handler_param = NULL;

OSAPI_LogMessage_T OSAPI_Log_g_message;

/*** SOURCE_BEGIN ***/

RTI_BOOL
OSAPI_LogMessageSeq_initialize(struct OSAPI_LogMessageSeq *self)
{
    self->_contiguous_buffer = NULL;
    self->_element_size = sizeof(struct OSAPI_LogMessage);
    self->_length = 0;
    self->_maximum = 0;

    return RTI_TRUE;
}

RTI_BOOL
OSAPI_LogMessageSeq_finalize(struct OSAPI_LogMessageSeq *self)
{
    if (self->_contiguous_buffer)
    {
        OSAPI_Heap_freeBuffer(self->_contiguous_buffer);
        self->_contiguous_buffer = NULL;
    }

    return RTI_TRUE;
}

RTI_INT32
OSAPI_LogMessageSeq_get_maximum(const struct OSAPI_LogMessageSeq *self)
{
    if (self == NULL)
    {
        return -1;
    }

    return self->_maximum;
}

RTI_BOOL
OSAPI_LogMessageSeq_set_maximum(struct OSAPI_LogMessageSeq *self,
                                RTI_INT32 new_max)
{
    void *new_buffer = NULL;
    void *old_buffer = NULL;
    RTI_INT32 new_length = 0;

    if ((self == NULL) || (new_max < 0))
    {
        return RTI_FALSE;
    }

    if (new_max == self->_maximum)
    {
        return RTI_TRUE;
    }

    if (new_max > 0)
    {
        OSAPI_Heap_allocateBuffer((char **)&new_buffer,
                                         new_max * self->_element_size,
                                         OSAPI_ALIGNMENT_DEFAULT);
        if (new_buffer == NULL)
        {
            return RTI_FALSE;
        }
    }

    new_length = (self->_length < new_max) ? self->_length : new_max;

    if (!OSAPI_Memory_copy(new_buffer,
                          self->_contiguous_buffer,
                          new_length * self->_element_size))
    {
        return RTI_FALSE;
    }

    old_buffer = self->_contiguous_buffer;
    self->_contiguous_buffer = new_buffer;
    self->_maximum = new_max;
    self->_length = new_length;

    if (old_buffer != NULL)
    {
        OSAPI_Heap_freeBuffer(old_buffer);
    }

    return RTI_TRUE;
}

OSAPI_LogMessage_T*
OSAPI_LogMessageSeq_get_reference(const struct OSAPI_LogMessageSeq *self,
                                  RTI_INT32 i)
{
    if ((self == NULL) || (i < 0) || (i >= self->_length))
    {
        return NULL;
    }

    return &self->_contiguous_buffer[i];
}

RTI_INT32
OSAPI_LogMessageSeq_get_length(const struct OSAPI_LogMessageSeq *self)
{
    if (self == NULL)
    {
        return -1;
    }
    return self->_length;
}

RTI_BOOL
OSAPI_LogMessageSeq_set_length(struct OSAPI_LogMessageSeq *self,
                               RTI_INT32 new_length)
{
    if ((self == NULL) || (new_length < 0) ||
        (new_length > self->_maximum))
    {
        return RTI_FALSE;
    }

    self->_length = new_length;

    return RTI_TRUE;
}

RTI_BOOL
OSAPI_Log_set_log_handler(OSAPI_Log_log_handlerFUNC handler, void *param)
{
    if (handler == NULL)
    {
        return RTI_FALSE;
    }
    OSAPI_Log_g_user_handler = handler;
    OSAPI_Log_g_user_handler_param = param;

    return RTI_TRUE;
}

RTI_BOOL
OSAPI_Log_initialize(RTI_UINT32 bytes, RTI_BOOL store_debug_logs)
{
    /* round down to bytes that can fit a whole number of LogMessages */
    RTI_INT32 bytes_actual =
            (bytes / sizeof(OSAPI_LogMessage_T)) * sizeof(OSAPI_LogMessage_T);

    if (bytes_actual < 1)
    {
        return RTI_FALSE;
    }

    OSAPI_Heap_allocateBuffer((char **)&OSAPI_LogImpl_g_singleton.buffer_start,
                             bytes_actual,OSAPI_ALIGNMENT_DEFAULT);

    if (OSAPI_LogImpl_g_singleton.buffer_start == NULL)
    {
        return RTI_FALSE;
    }

    OSAPI_LogImpl_g_singleton.lock = OSAPI_Mutex_new();
    if (OSAPI_LogImpl_g_singleton.lock == NULL)
    {
        return RTI_FALSE;
    }

    OSAPI_Memory_zero(OSAPI_LogImpl_g_singleton.buffer_start, bytes_actual);
    OSAPI_LogImpl_g_singleton.write_pointer =
                                        OSAPI_LogImpl_g_singleton.buffer_start;
    OSAPI_LogImpl_g_singleton.buffer_size = bytes_actual;
    OSAPI_LogImpl_g_singleton.log_entry_size = sizeof(OSAPI_LogMessage_T);
    OSAPI_LogImpl_g_singleton.store_debug_logs = store_debug_logs;
    OSAPI_LogImpl_g_singleton.has_wrapped = RTI_FALSE;
    OSAPI_LogImpl_g_singleton.verbosity = OSAPI_LOG_VERBOSITY_ERROR;

    return RTI_TRUE;
}

RTI_BOOL
OSAPI_Log_finalize(void)
{
    OSAPI_Heap_freeBuffer((char *)OSAPI_LogImpl_g_singleton.buffer_start);

    if (!OSAPI_Mutex_delete(OSAPI_LogImpl_g_singleton.lock))
    {
        return RTI_FALSE;
    }

    OSAPI_LogImpl_g_singleton.buffer_start = NULL;
    OSAPI_LogImpl_g_singleton.write_pointer = NULL;
    OSAPI_LogImpl_g_singleton.lock = NULL;
    OSAPI_LogImpl_g_singleton.store_debug_logs = RTI_FALSE;
    OSAPI_LogImpl_g_singleton.has_wrapped = RTI_FALSE;

    return RTI_TRUE;
}

void
OSAPI_Log_set_verbosity(OSAPI_LogVerbosity_T verbosity)
{
    OSAPI_LogImpl_g_singleton.verbosity = verbosity;
}

void
OSAPI_Log_write_internal(OSAPI_LogEntryHeader_T error_header,
                         OSAPI_LogEntryPayload_T *error_payload,
                         RTI_UINT32 line_num)
{
    OSAPI_LogMessage_T *log_message = NULL;
    OSAPI_LogEntryInfo_T *entry_info = NULL;
    RTI_INT32 length = error_header >> 28;
    RTI_INT32 i;
    RTI_BOOL using_log_buffer = ((OSAPI_LogImpl_g_singleton.buffer_start != NULL)
                            && (((error_header & LOG_DEBUG) != LOG_DEBUG)
                                || OSAPI_LogImpl_g_singleton.store_debug_logs));

#ifdef NDEBUG
    if ((error_header & LOG_DEBUG) == LOG_DEBUG)
    {
        return;
    }
#endif

    if (!using_log_buffer)
    {
        if ((error_header & LOG_DEBUG) == LOG_DEBUG)
        {
            if (OSAPI_LogImpl_g_singleton.verbosity >= OSAPI_LOG_VERBOSITY_DEBUG)
            {
                OSAPI_Stdio_printf("DEBUG: line %u, header 0x%08x\n",
                                  line_num,
                                  error_header);
                if (error_payload != NULL)
                {
                    for (i = 0; i < length; i++)
                    {
                        OSAPI_Stdio_printf("\targument[%d] = 0x%x\n",
                                          i,
                                          error_payload->value[i]);
                    }
                }
            }
        }
        else if ((error_header & LOG_WARNING) == LOG_WARNING)
        {
            if (OSAPI_LogImpl_g_singleton.verbosity >=
                                          OSAPI_LOG_VERBOSITY_WARNING)
            {
                OSAPI_Stdio_printf("WARNING: line %u, header 0x%08x\n",
                                  line_num,
                                  error_header);
                if (error_payload != NULL)
                {
                    for (i = 0; i < length; i++)
                    {
                        OSAPI_Stdio_printf("\targument[%d] = 0x%x\n",
                                          i,
                                          error_payload->value[i]);
                    }
                }
            }
        }
        else if (((error_header & LOG_RTI_ERROR) == LOG_RTI_ERROR)
                 || ((error_header & LOG_USER_ERROR) == LOG_USER_ERROR))
        {
            if (OSAPI_LogImpl_g_singleton.verbosity >= OSAPI_LOG_VERBOSITY_ERROR)
            {
                OSAPI_Stdio_printf("ERROR: line %u, header 0x%08x\n",
                                   line_num,
                                   error_header);
                if (error_payload != NULL)
                {
                    for (i = 0; i < length; i++)
                    {
                        OSAPI_Stdio_printf("\targument[%d] = 0x%x\n",
                                          i,
                                          error_payload->value[i]);
                    }
                }
            }
        }
    }

    if (using_log_buffer || (OSAPI_Log_g_user_handler != NULL))
    {
        /* construct the log message */

        if (using_log_buffer)
        {
            if (!OSAPI_Mutex_take(OSAPI_LogImpl_g_singleton.lock))
            {
                return;
            }
            log_message =
                (OSAPI_LogMessage_T *) OSAPI_LogImpl_g_singleton.write_pointer;
        }
        else
        {
            log_message = &OSAPI_Log_g_message;
        }
        entry_info = &(log_message->info);

        entry_info->thread_id = OSAPI_Thread_self();

        OSAPI_System_get_time(&entry_info->timestamp);

        entry_info->line_number = line_num;
        entry_info->header = error_header;

        /* copy the log message parameters */
        OSAPI_Memory_copy(&(log_message->payload), error_payload,
                         length * sizeof(OSAPI_LogEntryPayloadElement_T));

        if (using_log_buffer)
        {
            /* update the write pointer */
            OSAPI_LogImpl_g_singleton.write_pointer =
                ((OSAPI_LogImpl_g_singleton.write_pointer -
                        OSAPI_LogImpl_g_singleton.buffer_start +
                        OSAPI_LogImpl_g_singleton.log_entry_size) %
                        OSAPI_LogImpl_g_singleton.buffer_size) +
                        OSAPI_LogImpl_g_singleton.buffer_start;
            if (!OSAPI_LogImpl_g_singleton.has_wrapped
                && (OSAPI_LogImpl_g_singleton.write_pointer ==
                        OSAPI_LogImpl_g_singleton.buffer_start))
            {
                OSAPI_LogImpl_g_singleton.has_wrapped = RTI_TRUE;
            }
        }
    }

    if (using_log_buffer)
    {
        OSAPI_Mutex_give(OSAPI_LogImpl_g_singleton.lock);
    }
    if (OSAPI_Log_g_user_handler != NULL)
    {
        OSAPI_Log_g_user_handler(OSAPI_Log_g_user_handler_param, log_message);
    }
}

RTI_BOOL
OSAPI_Log_is_storing_debug(void)
{
    return OSAPI_LogImpl_g_singleton.store_debug_logs;
}

RTI_BOOL
OSAPI_Log_get_last_entries(struct OSAPI_LogMessageSeq *messages,
                          RTI_UINT32 max_length_in,
                          OSAPI_ThreadId thread_id,
                          RTI_UINT32 * length_out)
{
    RTI_INT32 valid_messages_in_buffer, messages_read = 0, message_count = 0;
    RTI_INT32 current_maximum;
    char *read_pointer;
    OSAPI_LogMessage_T *log_message_read_pointer;
    RTI_BOOL ok = RTI_FALSE;

    if ((OSAPI_LogImpl_g_singleton.buffer_start == NULL) || (messages == NULL)
        || (length_out == NULL))
    {
        return ok;
    }

    *length_out = 0;

    if (!OSAPI_Mutex_take(OSAPI_LogImpl_g_singleton.lock))
    {
        return ok;
    }

    read_pointer =  ((OSAPI_LogImpl_g_singleton.write_pointer -
            OSAPI_LogImpl_g_singleton.buffer_start -
            OSAPI_LogImpl_g_singleton.log_entry_size +
            OSAPI_LogImpl_g_singleton.buffer_size) %
            OSAPI_LogImpl_g_singleton.buffer_size) +
                    OSAPI_LogImpl_g_singleton.buffer_start;

    if (OSAPI_LogImpl_g_singleton.has_wrapped)
    {
        valid_messages_in_buffer =
                OSAPI_LogImpl_g_singleton.buffer_size /
                OSAPI_LogImpl_g_singleton.log_entry_size;
    }
    else
    {
        valid_messages_in_buffer =
            (OSAPI_LogImpl_g_singleton.write_pointer -
                    OSAPI_LogImpl_g_singleton.buffer_start) /
                    OSAPI_LogImpl_g_singleton.log_entry_size;
    }

    current_maximum = OSAPI_LogMessageSeq_get_maximum(messages);
    if (!OSAPI_LogMessageSeq_set_length(messages, message_count))
    {
        goto done;
    }

    while (messages_read < valid_messages_in_buffer)
    {
        log_message_read_pointer = (OSAPI_LogMessage_T *) read_pointer;
        if ((thread_id == OSAPI_THREAD_ANY)
            || (log_message_read_pointer->info.thread_id == thread_id))
        {
            message_count++;
            if ((current_maximum < message_count)
                || ((max_length_in != LOG_LENGTH_UNLIMITED)
                    && ((*length_out + OSAPI_LogImpl_g_singleton.log_entry_size)
                        > max_length_in)))
            {
                /* can't fit any more */
                ok = RTI_TRUE;
                goto done;
            }

            *length_out += OSAPI_LogImpl_g_singleton.log_entry_size;

            if (!OSAPI_LogMessageSeq_set_length(messages, message_count))
            {
                goto done;
            }

            OSAPI_Memory_copy(OSAPI_LogMessageSeq_get_reference
                             (messages, message_count - 1),
                             log_message_read_pointer,
                             OSAPI_LogImpl_g_singleton.log_entry_size);
        }

        read_pointer =
            ((read_pointer - OSAPI_LogImpl_g_singleton.buffer_start -
                    OSAPI_LogImpl_g_singleton.log_entry_size +
                    OSAPI_LogImpl_g_singleton.buffer_size) %
                    OSAPI_LogImpl_g_singleton.buffer_size) +
                    OSAPI_LogImpl_g_singleton.buffer_start;
        messages_read++;
    }

    ok = RTI_TRUE;

done:
    OSAPI_Mutex_give(OSAPI_LogImpl_g_singleton.lock);

    return ok;
}

void
OSAPI_Log_msg_pN(RTI_INT32 code, RTI_INT32 line,RTI_INT32 pc,RTI_INT32 p1,
                 RTI_INT32 p2,RTI_INT32 p3,RTI_INT32 p4,RTI_INT32 p5,
                 RTI_INT32 p6,RTI_INT32 p7,RTI_INT32 p8,RTI_INT32 p9)
{
    struct OSAPI_LogEntryPayload payload;
    payload.value[0]=p1;
    payload.value[1]=p2;
    payload.value[2]=p3;
    payload.value[3]=p4;
    payload.value[4]=p5;
    payload.value[5]=p6;
    payload.value[6]=p7;
    payload.value[7]=p8;
    payload.value[8]=p9;
    OSAPI_Log_write_internal((pc << 28) | code, &payload,line);
}

