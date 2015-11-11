/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_log_h
#define osapi_log_h



#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif /*  */
#ifndef osapi_thread_h
#include "osapi/osapi_thread.h"
#endif /*  */
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif /*  */
#ifndef osapi_system_h
#include "osapi/osapi_system.h"
#endif /*  */
#ifndef osapi_stdio_h
#include "osapi/osapi_stdio.h"
#endif /*  */

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
  \brief Log API
*/
/*e \defgroup OSAPILogClass OSAPI Log
    \ingroup OSAPIModule

    \details

  The Log API provides functionality for logging events.

  The log buffer is a fixed-size ring buffer of fixed-size log entries.
  Each log entry will have a fixed-length header and a fixed-length payload.

   \verbatim
   +-----+------+--------+------------+
   | len | type | module |X|error-code|
   +-----+------+--------+------------+
   \endverbatim

 *
 * <ul>
 *  <li> 4 bits for length of payload, in 32-bit ints
 *  <li> 4 bits for message type (error, warning, etc)
 *  <li> 8 bits for module
 *  <l>  1 bit for extended format (not in use)
 *  <li> 15 bits for error code
 *  <li>
 * </ul>
 *
 * Example Log Header: 0x1203005d
 * <ul>
 *  <li> length: 0x1           one parameter
 *  <li> type: 0x2             RTI Error
 *  <li> module: 0x03          DDS_C module
 *  <li> X=0:                  Not extended format
 *  <li> error code: 0x005d    LOG_FAIL_CREATE_PUBLISHER_TABLE
 * </ul>
 *
 * Please refer to include/osapi/osapi_log.h for all the error codes.
 */

/******************************************************************************
 *                                Log types
 ******************************************************************************/
#define LOG_USER_ERROR              0x01000000
#define LOG_RTI_ERROR               0x02000000
#define LOG_ERROR                   0x02000000
#define LOG_WARNING                 0x03000000
#define LOG_DEBUG                   0x04000000
#define LOG_PRECONDITION_ERROR      0x05000000

/******************************************************************************
 *                            Modules
 ******************************************************************************/
#define LOG_CDR                     0x00010000
#define LOG_CLOCK                   0x00020000
#define LOG_DDS_C                   0x00030000
#define LOG_DISC_DPDE               0x00040000
#define LOG_DISC_DPSE               0x00050000
#define LOG_EVENT                   0x00060000
#define LOG_NETIO                   0x00070000
#define LOG_OSAPI                   0x00080000
#define LOG_REDA                    0x00090000
#define LOG_RH_SM                   0x000a0000
#define LOG_RTPS                    0x000b0000
#define LOG_URTDB                   0x000c0000
#define LOG_UTIL                    0x000d0000
#define LOG_WH_SM                   0x000e0000
#define LOG_DB                      0x000f0000
#define LOG_RT                      0x00100000
#define LOG_COMMON                  0x00110000

/******************************************************************************
 *                            OSAPI Error codes
 ******************************************************************************/

#define LOG_FAIL_CREATE_MUTEX                                       0x00000002
#define LOG_FAIL_GET_CLOCK_RESOLUTION                               0x00000003
#define LOG_FAIL_GET_CLOCK_TIME                                     0x00000004
#define LOG_FAIL_USE_SEMAPHORE_FOR_ROLLOVER                         0x00000005
#define LOG_FAIL_ROLLOVER_CHECK                                     0x00000006
#define LOG_FAIL_WAKEUP_EVENT_THREAD                                0x00000156
#define LOG_FAIL_TAKE_MUTEX_FOR_EVENT_LIST                          0x00000157
/* 2 ints wakeup time, 2 ints now time */
#define LOG_WAKEUP_TIME_ALREADY_SURPASSED                           0x00000158
#define LOG_FAIL_GIVE_MUTEX_FOR_EVENT_LIST                          0x00000159
#define LOG_FAIL_SLEEP                                              0x0000015a
#define LOG_EVENT_THREAD_EXITING                                    0x0000015b
#define LOG_FAIL_SPAWN_EVENT_THREAD                                 0x0000015c
#define LOG_FAIL_START_EVENT_THREAD                                 0x0000015d
#define LOG_FAIL_DESTROY_EVENT_THREAD                               0x0000015e
#define LOG_DESTROY_EVENT_THREAD                                    0x0000015f
#define LOG_FAIL_CREATE_EVENT_TIMER                                 0x00000160
#define LOG_FAIL_STOP_EVENT_THREAD                                  0x00000161
#define LOG_WAKEUP_EARLY_FOR_EVENT                                  0x00000162
#define LOG_OUT_OF_EVENTS_DUE_TO_EVENT_QOS_INITIAL_COUNT            0x00000163
/* 1 int num events, 1 int slot */
#define LOG_INSERT_EVENT                                            0x00000164
#define LOG_NOT_UPDATING_ANY_FIELDS_BECAUSE_THEY_ARE_ALL_NULL       0x00000165
#define LOG_EVENT_HANDLE_INVALID                                    0x00000166
/* 1 int epoch, 1 int vx */
#define LOG_FAIL_CREATE_SEMAPHORE                                   0x00000167
#define OSAPI_LOG_SYSTEM_GET_NEXT_OBJECT_ID_FAILED                  0x00000001
#define OSAPI_LOG_SYSTEM_SET_PROPERTY_FAILED                        0x00000002
#define OSAPI_LOG_THREAD_NEW_FAILED                                 0x00000003
#define OSAPI_LOG_THREAD_EXEC_FAILED                                0x00000004
#define OSAPI_LOG_THREAD_START_FAILED                               0x00000005
#define OSAPI_LOG_THREAD_DESTROY_FAILED                             0x00000006
#define OSAPI_LOG_TIMER_NEW_FAILED                                  0x00000007
#define OSAPI_LOG_TIMER_DELETE_FAILED                               0x00000008
#define OSAPI_LOG_TIMER_TICK_FAILED                                 0x00000009
#define OSAPI_LOG_TIMER_CREATE_TIMEOUT_FAILED                       0x0000000a
#define OSAPI_LOG_TIMER_UPDATE_TIMEOUT_FAILED                       0x0000000b
#define OSAPI_LOG_TIMER_DELETE_TIMEOUT_FAILED                       0x0000000c
#define OSAPI_LOG_TIMER_GET_USER_DATA_FAILED                        0x0000000d
#define OSAPI_LOG_HEAP_INTERNAL_ALLOCATED_FAILED                    0x0000000e
#define OSAPI_LOG_MUTEX_DELETE_FAILED                               0x0000000f
#define OSAPI_LOG_MUTEX_NEW_FAILED                                  0x00000010
#define OSAPI_LOG_MUTEX_TAKE_FAILED                                 0x00000011
#define OSAPI_LOG_MUTEX_GIVE_FAILED                                 0x00000012
#define OSAPI_LOG_SEMAPHORE_DELETE_FAILED                           0x00000013
#define OSAPI_LOG_SEMAPHORE_NEW_FAILED                              0x00000014
#define OSAPI_LOG_SEMAPHORE_TAKE_FAILED                             0x00000015
#define OSAPI_LOG_SEMAPHORE_GIVE_FAILED                             0x00000016
#define OSAPI_LOG_SYSTEM_START_TIMER_FAILED                         0x00000017
#define OSAPI_LOG_SYSTEM_THREAD_PRIORITY_MAP_FAILED                 0x00000018
#define OSAPI_LOG_SYSTEM_THREAD_GET_POLICY_FAILED                   0x00000019
#define OSAPI_LOG_SYSTEM_THREAD_CREATE                              0x0000001a

typedef RTI_UINT32 OSAPI_LogEntryHeader_T;
typedef RTI_UINT32 OSAPI_LogEntryPayloadElement_T;

#define LOG_ENTRY_PAYLOAD_MAX_ELEMENTS      9
#define OSAPI_THREAD_ANY                    0
#define LOG_LENGTH_UNLIMITED                0

/*ce
    @brief Interface used for log message.
    
    These log messages are stored internally as entries in a ring buffer.
*/
/*
 * The log buffer will be a fixed-size ring buffer of fixed-size log entries.
 * Each log entry will have a fixed-length header and a fixed-length payload.
 *
 *  %-------------------------------------------------------------------
 *  %| len   | type   | module         |      error code               |
 *  %-------------------------------------------------------------------
 *
 *  4 bits for length of payload, in 32-bit ints
 *  4 bits for message type (error, warning, etc)
 *  8 bits for module
 *  16 bits for error code
 *
 *  Example Log Header: 0x1203005d
 *  length ......: 0x1      // one parameter
 *  type ........: 0x2      // RTI Error
 *  module ......: 0x03     // DDS_C module
 *  error code ..: 0x005d   // LOG_FAIL_CREATE_PUBLISHER_TABLE
 *
 */
typedef struct OSAPI_LogEntryInfo
{

    /* e \dref_LogMessage_thread_id thread id of the thread that logged this
     * message */
    OSAPI_ThreadId thread_id;

    /* e \dref_LogMessage_timestamp time this message was logged */
    OSAPI_NtpTime timestamp;

    /* e \dref_LogMessage_line_number line number of the file where this
     * message is printed */
    RTI_UINT32 line_number;

    /* e \dref_LogMessage_header header info for this message, including
     * length of the message payload */
    OSAPI_LogEntryHeader_T header;

    /* store the payload here? pointer or struct? */
} OSAPI_LogEntryInfo_T;

typedef struct OSAPI_LogEntryPayload
{
    OSAPI_LogEntryPayloadElement_T value[LOG_ENTRY_PAYLOAD_MAX_ELEMENTS];

    /* For strings, we store as many characters as we can at the end. */
} OSAPI_LogEntryPayload_T;

typedef struct OSAPI_LogMessage
{
    OSAPI_LogEntryInfo_T info;

    /* store the payload struct here */
    OSAPI_LogEntryPayload_T payload;
} OSAPI_LogMessage_T;

struct OSAPI_LogMessageSeq;

/* verbosity levels */
/*e
 */
typedef enum
{
    /* e \dref_LogVerbosity_OSAPI_LOG_VERBOSITY_SILENT */
    OSAPI_LOG_VERBOSITY_SILENT  = 0,
    /* e \dref_LogVerbosity_OSAPI_LOG_VERBOSITY_ERROR */
    OSAPI_LOG_VERBOSITY_ERROR   = 1,
    /* e \dref_LogVerbosity_OSAPI_LOG_VERBOSITY_WARNING */
    OSAPI_LOG_VERBOSITY_WARNING = 2,
    /* e \dref_LogVerbosity_OSAPI_LOG_VERBOSITY_DEBUG */
    OSAPI_LOG_VERBOSITY_DEBUG   = 3
} OSAPI_LogVerbosity_T;

OSAPIDllExport RTI_BOOL
OSAPI_LogMessageSeq_initialize(struct OSAPI_LogMessageSeq *self);

OSAPIDllExport RTI_BOOL
OSAPI_LogMessageSeq_finalize(struct OSAPI_LogMessageSeq *self);

OSAPIDllExport RTI_INT32
OSAPI_LogMessageSeq_get_maximum(const struct OSAPI_LogMessageSeq *self);

OSAPIDllExport RTI_BOOL
OSAPI_LogMessageSeq_set_maximum(struct OSAPI_LogMessageSeq *self,
                               RTI_INT32 new_max);

OSAPIDllExport RTI_INT32
OSAPI_LogMessageSeq_get_length(const struct OSAPI_LogMessageSeq *self);

OSAPIDllExport RTI_BOOL
OSAPI_LogMessageSeq_set_length(struct OSAPI_LogMessageSeq *self,
                              RTI_INT32 length);

OSAPIDllExport OSAPI_LogMessage_T*
OSAPI_LogMessageSeq_get_reference(const struct OSAPI_LogMessageSeq *self,
                                 RTI_INT32 i);

/* callback for user handling of log messages */
typedef void (*OSAPI_Log_log_handlerFUNC)(void *param,
                                         struct OSAPI_LogMessage *msg);

/*ce
    @brief Set the log handler

    The log handler is a function that the user can implement to handle log
    messages.
*/
OSAPIDllExport RTI_BOOL
OSAPI_Log_set_log_handler(OSAPI_Log_log_handlerFUNC handler,
                         void *param);

/*ce
    @brief Initialize the log buffer

    Dynamically allocates bytes for the log ring buffer. Also prints out endianness.
    If store_debug_logs is false, debug-level logs are printed to the console.
    Otherwise, the logs are stored in the log buffer.
*/
OSAPIDllExport RTI_BOOL
OSAPI_Log_initialize(RTI_UINT32 bytes,RTI_BOOL store_debug_logs);

/*ce
    @brief Finalize the log buffer.

    Frees the memory for the log ring buffer.
*/
OSAPIDllExport RTI_BOOL
OSAPI_Log_finalize(void);

/*ce
    @brief Get the last log messages.

    Get the most recent max_length bytes of log messages for thread_id and
    output them in messages. max_length = LOG_LENGTH_UNLIMITED means get all
    messages. messages is user-allocated, so there will be copying. The 
	length of the messages sequence is also updated. However, the user is
    responsible for initializing, allocating, and finalizing the sequence.
*/
OSAPIDllExport RTI_BOOL
OSAPI_Log_get_last_entries(struct OSAPI_LogMessageSeq *messages,
                           RTI_UINT32 max_length_in,
                           OSAPI_ThreadId thread_id,
                           RTI_UINT32 *length_out);

/*ce
    @brief Set the verbosity of logging printfs.
*/
OSAPIDllExport void
OSAPI_Log_set_verbosity(OSAPI_LogVerbosity_T verbosity);


/*ce
    @brief Add a log message to the buffer.
*/
#define OSAPI_Log_write(header, payload) \
  OSAPI_Log_write_internal(header, payload, __LINE__)

OSAPIDllExport void
OSAPI_Log_write_internal(OSAPI_LogEntryHeader_T error_header,
                         OSAPI_LogEntryPayload_T *error_payload,
                         RTI_UINT32 line_num);

/*ci
    @brief Whether or not we are storing debug logs in the log buffer.
*/
OSAPIDllExport RTI_BOOL
OSAPI_Log_is_storing_debug(void);

OSAPIDllExport void
OSAPI_Log_msg_pN(RTI_INT32 code, RTI_INT32 line,RTI_INT32 pc,RTI_INT32 p1,
                 RTI_INT32 p2,RTI_INT32 p3,RTI_INT32 p4,RTI_INT32 p5,
                 RTI_INT32 p6,RTI_INT32 p7,RTI_INT32 p8,RTI_INT32 p9);

/* 9/29/11
 * fixed-size records with array of N ints as payload. User will decode the
 * payload depending on the trailer. For strings, we can log the char *,
 * not the actual chars.
 */
#ifdef __cplusplus
}                               /* extern "C" */
#endif

#include "osapi/osapi_log_impl.h"

#endif /* osapi_log_h */
