/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_timer_h
#define osapi_timer_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_thread_h
#include "osapi/osapi_thread.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \defgroup OSAPI_TimerClass OSAPI Timer
 *  \ingroup OSAPIModule
*/

/*e \file
  \brief Timer API
*/

/*e \ingroup OSAPI_TimerClass
 *  \brief Create a periodic timer
 */
#define OSAPI_TIMER_PERIODIC (0x1)

/*e \ingroup OSAPI_TimerClass
 *  \brief Create a one-shot timer
 */
#define OSAPI_TIMER_ONE_SHOT (0x0)

/*e \ingroup OSAPI_TimerClass
 *  \brief Action taken by timer module when a timer callback returns.
 */
typedef enum
{
    OSAPI_TIMEOUT_OP_AUTOMATIC = 1,
    OSAPI_TIMEOUT_OP_MANUAL
} OSAPITimeoutOp_t;

/*e \ingroup OSAPI_TimerClass
 *  \brief User-data passed to timer-handler.
 */
struct OSAPITimeoutUserData
{
    RTI_UINT32 count[2];
    void *field[2];
};

struct OSAPI_TimerEntry;
struct OSAPITimeoutHandle
{
    /* In case an entry is re-used */
    struct OSAPI_TimerEntry *_entry;

    /* In case an entry is re-used */
    RTI_INT32 epoch;
};

typedef struct OSAPITimeoutHandle OSAPITimeoutHandle_t;

#define OSAPITimeoutHandle_t_INITIALIZER \
{\
    NULL,\
    -1\
}

struct OSAPI_TimerProperty
{
    RTI_INT32 max_entries;
    RTI_INT32 max_slots;
    struct OSAPI_ThreadProperties thread;
};

#define OSAPI_TimerProperty_INITIALIZER \
{\
    128,\
    32,\
    {  /* OSAPI_ThreadProperties */ \
        16*1024,\
        OSAPI_THREAD_PRIORITY_NORMAL,\
        OSAPI_THREAD_DEFAULT_OPTIONS,\
    },\
}

struct OSAPI_Timer;
typedef struct OSAPI_Timer *OSAPI_Timer_t;
typedef OSAPITimeoutOp_t(*OSAPITimeoutFunction)(
                                struct OSAPITimeoutUserData *user_data);

typedef void (*OSAPI_TimerTickHandlerFunction) (OSAPI_Timer_t timer);

/*e \ingroup OSAPI_TimerClass
    \brief Create a Timer.

    \details Create a new Timer. A Timer can manage multiple timeouts.

    Example:
    \code
    OSAPI_Timer_t my_timer;
    struct OSAPI_TimerProperty timer_property = OSAPI_TimerProperty_INITIALIZER;
    timer = OSAPI_Timer_new(&timer_property);
    if (timer == NULL) {
        return error;
    }
    \endcode

    The created Timer should be deleted with \ref OSAPI_Timer_delete.

    @param[in] property Timer property.

    @param[in] mutex Shared mutex.

    @mtsafety SAFE
*/
OSAPIDllExport OSAPI_Timer_t
OSAPI_Timer_new(struct OSAPI_TimerProperty *property, struct OSAPI_Mutex *mutex);

/*e \ingroup OSAPI_TimerClass
    \brief Delete a Timer.

    \details Delete a previously created Timer. All timeouts are cancelled.

    Example:
    \code
    OSAPI_Timer_t my_timer;

    ......

    OSAPI_Timer_delete(my_timer);
    if (timer == NULL) {
        return error;
    }
    \endcode

    The created Timer should be deleted with \ref OSAPI_Timer_delete.

    @param [in] timer Timer.

    @mtsafety SAFE
*/
OSAPIDllExport RTI_BOOL OSAPI_Timer_delete(OSAPI_Timer_t timer);

/*e \ingroup OSAPI_TimerClass
    \brief Schedule a timeout.

    \details This function schedules a timeout with the specified period. The
    timeout can either be rescheduled automatically or a new timeout must
    be created.

    Example:
    \code
    OSAPI_Timer_t my_timer;
    OSAPITimeoutHandle_t my_handle = OSAPITimeoutHandle_t_INITIALIZER;
    RTI_BOOL result;
    struct OSAPI_TimerEntryUserData user_data;

    result = OSAPI_Timer_create_timeout(my_timer,
                                    &my_handle,
                                    1000,
                                    OSAPI_TIMER_PERIODIC,
                                    &user_data);

    if (!result) {
        report error;
    }

    ......

    \endcode

    A timeout can cancelled  with \ref OSAPI_Timer_delete_timeout or the timeout
    can be modified with \ref OSAPI_Timer_update_timeout.

    @param[in]  timer Timer object. Cannot be NULL.
    @param[out] out_handle Reference to the timeout. Cannot be NULL.
    @param[in]  timeout Timeout in ms.
    @param[in]  flags Flags.
    @param[in]  timeout_handler Function to call at timeout. Cannot be NULL.
    @param[in]  user_data User data associated with the timeout. Can be NULL.

    @mtsafety SAFE
*/
OSAPIDllExport RTI_BOOL
OSAPI_Timer_create_timeout(OSAPI_Timer_t timer,
                          OSAPITimeoutHandle_t * out_handle,
                          RTI_INT32 timeout,
                          RTI_INT32 flags,
                          OSAPITimeoutFunction timeout_handler,
                          struct OSAPITimeoutUserData *user_data);

/*e \ingroup OSAPI_TimerClass
    \brief Reschedule a timeout.

    \details This function reschedules a previously scheduled timeout with
    the specified period.

    Example:
    \code
    OSAPI_Timer_t my_timer;
    OSAPITimeoutHandle_t my_handle = OSAPITimeoutHandle_t_INITIALIZER;
    RTI_BOOL result;
    struct OSAPI_TimerEntryUserData user_data;

    result = OSAPI_Timer_update_timeout(my_timer,
                                    &my_handle,
                                    2000);

    if (!result) {
        report error;
    }

    ......

    \endcode

    @param [in]  timer Timer object. Cannot be NULL.
    @param [out] out_handle Reference to the timeout. Cannot be NULL.
    @param [in]  timeout Timeout in ms.

    @mtsafety SAFE
*/
OSAPIDllExport RTI_BOOL
OSAPI_Timer_update_timeout(OSAPI_Timer_t timer,
                          OSAPITimeoutHandle_t *out_handle, RTI_INT32 timeout);

/*e \ingroup OSAPI_TimerClass
    \brief Stop a previously scheduled timeout

    \details This function stops a previously scheduled timeout.

    Example:
    \code
    OSAPI_Timer_t my_timer;
    OSAPITimeoutHandle_t my_handle = OSAPITimeoutHandle_t_INITIALIZER;
    RTI_BOOL result;

    ....

    result = OSAPI_Timer_stop_timer(my_timer,&my_handle);

    if (!result) {
        report error;
    }

    ......

    \endcode

    @param [in] timer Timer object. Cannot be NULL.
    @param [in] handle Reference to the timeout. Cannot be NULL.

    @mtsafety SAFE
*/
OSAPIDllExport RTI_BOOL
OSAPI_Timer_delete_timeout(OSAPI_Timer_t timer, OSAPITimeoutHandle_t *handle);

/*e \ingroup OSAPI_TimerClass
    \brief Get the user_data from a handle.

    Example:
    \code
    OSAPI_Timer_t my_timer;
    OSAPITimeoutHandle_t my_handle = OSAPITimeoutHandle_t_INITIALIZER;
    RTI_BOOL result;
    struct OSAPI_TimerEntryUserData user_data;

    ....

    result = OSAPITimeoutHandle_get_user_data(&user_data,&my_handle);

    if (!result) {
        report error;
    }

    ......

    \endcode

    @param [out] user_data User-data associated with handle. Cannot be NULL.
    @param [in]  handle Timer handle.

    @mtsafety SAFE
*/
OSAPIDllExport RTI_BOOL
OSAPITimeoutHandle_get_user_data(struct OSAPITimeoutUserData *user_data,
                                 OSAPITimeoutHandle_t *handle);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* osapi_timer_h */
