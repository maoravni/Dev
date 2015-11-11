/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_system_h
#define osapi_system_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef osapi_timer_h
#include "osapi/osapi_timer.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
  \brief System API
*/

/*e \defgroup OSAPI_SystemClass OSAPI System
    \ingroup OSAPIModule

    \brief Abstract System API.

    \details The System is defined as the physical or virtual
    execution environment and implements functions that are not necessarily
    provided by the operating system. For example, a custom embedded board may
    include a special real-time clock which is synchronized via GPS, but is
    not available via a regular OS system call.
*/

/*e \ingroup OSAPI_SystemClass
 *
 *  System properties
 */
struct OSAPI_SystemProperty
{
    struct OSAPI_TimerProperty timer_property;
};

#define OSAPI_SystemProperty_INITIALIZER \
{\
    OSAPI_TimerProperty_INITIALIZER \
}

/*e \ingroup OSAPI_SystemClass
 *
 *  Abstract UUID object.  128-bit value.
 */
struct OSAPI_SystemUUID
{
    RTI_UINT32 value[4];
};

/*e \ingroup OSAPI_SystemClass
  \brief Generate a unique universal identifier (UUID)
*/
OSAPIDllExport void
OSAPI_System_generate_uuid(struct OSAPI_SystemUUID *uuid_out);

/*e \ingroup OSAPI_SystemClass
  \brief Get the current system time.

  \details In general, the system time is used by components to correlate both
  internal and external events, such as data reception and ordering. Thus,
  it is recommended that this function returns the real time. However, it is not
  strictly required.

  Notes:
   - It is assumed that the time returned by get_time is monotonically
     increasing. It it up to the implementation of this function to ensure this
     holds true.
   - It is ok to return the same time as the last call.

  @param[out] now Time in NtpTime format.

  @pre Initialized system time.

  @exception None.

  @mtsafety SAFE
*/
OSAPIDllExport RTI_BOOL
OSAPI_System_get_time(OSAPI_NtpTime *now);

/*e \ingroup OSAPI_SystemClass
    \brief Get the resolution of the clock driving the timer
    \fn OSAPI_System_get_timer_resolution
*/
OSAPIDllExport RTI_INT32
OSAPI_System_get_timer_resolution(void);

/*e \ingroup OSAPI_SystemClass

    \brief Start the timer.

    @param [in]  self Timer object.

    @param [in]  tick_handler Timer handle.
*/
OSAPIDllExport RTI_BOOL
OSAPI_System_start_timer(OSAPI_Timer_t self,
                        OSAPI_TimerTickHandlerFunction tick_handler);

/*e \ingroup OSAPI_SystemClass
    \brief Stop the timer.

    @param [in] self Timer
*/
OSAPIDllExport RTI_BOOL
OSAPI_System_stop_timer(OSAPI_Timer_t self);

/*e \ingroup OSAPI_SystemClass
    \brief Stop the timer.

    @param [in] self Timer
*/
OSAPIDllExport RTI_BOOL
OSAPI_System_initialize(void);

/*e \ingroup OSAPI_SystemClass
    \brief Stop the timer.

    @param [in] self Timer
*/
OSAPIDllExport RTI_BOOL
OSAPI_System_finalize(void);

/*e \ingroup OSAPI_SystemClass
    \brief Get the system properties

    @param [out] property
*/
OSAPIDllExport RTI_BOOL
OSAPI_System_get_property(struct OSAPI_SystemProperty *property);

/*e \ingroup OSAPI_SystemClass
    \brief Set the system properties

    @param [in] property
*/
OSAPIDllExport RTI_BOOL
OSAPI_System_set_property(struct OSAPI_SystemProperty *property);

/*e \ingroup OSAPI_SystemClass
    \brief Return next ID
*/
OSAPIDllExport RTI_INT32
OSAPI_System_get_next_object_id(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* osapi_system_h */
