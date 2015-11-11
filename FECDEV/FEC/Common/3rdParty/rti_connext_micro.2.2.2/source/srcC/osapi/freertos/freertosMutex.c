/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "osapi/osapi_config.h"
#include "util/util_makeheader.h"

#if OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"

//#include "inc/hw_types.h"
//#include "driverlib/interrupt.h"

#include "osapi/osapi_types.h"
#include "osapi/osapi_heap.h"
#include "osapi/osapi_mutex.h"

struct OSAPI_Mutex
{
    xSemaphoreHandle mutex;
};
#endif

/*** SOURCE_BEGIN ***/
#if OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS

RTI_BOOL
OSAPI_Mutex_delete(struct OSAPI_Mutex *mutex)
{
    if (mutex != NULL)
    {
        if (mutex->mutex != NULL)
        {
            vSemaphoreDelete(mutex->mutex);
        }
        OSAPI_Heap_freeStructure(mutex);
    }

    return RTI_TRUE;
}

struct OSAPI_Mutex *
OSAPI_Mutex_new(void)
{
    struct OSAPI_Mutex *mutex = NULL;

//    if (OSAPI_Mutex_g_return_NULL)
//    {
//        return NULL;
//    }

    OSAPI_Heap_allocateStructure(&mutex, struct OSAPI_Mutex);
    if (mutex == NULL)
    {
        return NULL;
    }

    mutex->mutex = xSemaphoreCreateRecursiveMutex();
    if (mutex->mutex == NULL)
    {
        OSAPI_Heap_freeStructure(mutex);
        return NULL;
    }

    return mutex;
}

RTI_BOOL
OSAPI_Mutex_take(struct OSAPI_Mutex * mutex)
{
    if (mutex == NULL)
    {
        return RTI_FALSE;
    }
    if (pdTRUE != xSemaphoreTakeRecursive(mutex->mutex, portMAX_DELAY))
    {
        return RTI_FALSE;
    }
    return RTI_TRUE;
}


RTI_BOOL
OSAPI_Mutex_give(struct OSAPI_Mutex * mutex)
{
    if (mutex == NULL)
    {
        return RTI_FALSE;
    }
    if (pdTRUE != xSemaphoreGiveRecursive(mutex->mutex))
    {
        return RTI_FALSE;
    }
    return RTI_TRUE;
}
#endif
