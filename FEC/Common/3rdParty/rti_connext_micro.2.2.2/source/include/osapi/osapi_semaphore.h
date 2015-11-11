/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_semaphore_h
#define osapi_semaphore_h


#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
    \brief Semaphore API.
*/

/*e \defgroup OSAPI_SemaphoreClass OSAPI Semaphore
    \ingroup OSAPIModule

    \brief Abstract Semaphore API.
 */

/*e
 * Infinite timeout
 */
#define OSAPI_SEMAPHORE_TIMEOUT_INFINITE -1

/*e \ingroup OSAPI_SemaphoreClass
 *
 * \brief Abstract semaphore object.
 */
struct OSAPI_Semaphore;
typedef struct OSAPI_Semaphore OSAPI_Semaphore_T;

/*e \ingroup OSAPI_SemaphoreClass
 *
 * \brief Create a semaphore.
 *
 * \return Pointer to a semaphore with a value of 1 on success, NULL on failure.
 *
 * \sa \ref OSAPIPortingClass, \ref OSAPI_Semaphore_delete
 *
 */
OSAPIDllExport OSAPI_Semaphore_T*
OSAPI_Semaphore_new(void);

/*e \ingroup OSAPI_SemaphoreClass
 *
 *  \brief Delete a semaphore.
 *
 *  \param [in] self Semaphore created with \ref OSAPI_Semaphore_new.
 *
 *  \sa \ref OSAPIPortingClass, \ref OSAPI_Semaphore_new
 */
OSAPIDllExport RTI_BOOL
OSAPI_Semaphore_delete(OSAPI_Semaphore_T *self);

/*e \ingroup OSAPI_SemaphoreClass
 *
 *  \brief Take a semaphore.
 *
 *  \param [in] self Semaphore previously created with \ref OSAPI_Semaphore_new.
 *
 *  \param [in] timeout Timeout in ms. The `take` call will wait at most
 *                       `timeout` ms before returning. If \ref
 *                       OSAPI_SEMAPHORE_TIMEOUT_INFINITE is specified, the
 *                       call will not return until the semaphore has a value of
 *                       1 or higher.
 *
 *  \param [out] fail_reason This parameter is not used, but present for
 *                          compatability reasons.
 *
 *  \return RTI_TRUE on success, RTI_FALSE on failure.
 *
 *  \sa \ref OSAPIPortingClass, \ref OSAPI_Semaphore_give
 */
OSAPIDllExport RTI_BOOL
OSAPI_Semaphore_take(OSAPI_Semaphore_T *self,RTI_INT32 timeout,
                    RTI_INT32 *fail_reason);

/*e \ingroup OSAPI_SemaphoreClass
 *
 *  \brief Give a semaphore.
 *
 *  \param [in] self Semaphore previously created with \ref OSAPI_Semaphore_new.
 *
 *  \return RTI_TRUE on success, RTI_FALSE on failure.
 *
 *  \sa \ref OSAPIPortingClass, \ref OSAPI_Semaphore_take
 *
 */
OSAPIDllExport RTI_BOOL
OSAPI_Semaphore_give(OSAPI_Semaphore_T *self);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* osapi_semaphore_h */
