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

#include "osapi/osapi_heap.h"
#include "osapi/osapi_mutex.h"
#include "osapi/osapi_semaphore.h"

struct OSAPI_Semaphore
{
    xSemaphoreHandle semaphore;
};
#endif

/*** SOURCE_BEGIN ***/

#if OSAPI_PLATFORM == OSAPI_PLATFORM_FREERTOS
RTI_BOOL
OSAPI_Semaphore_delete(struct OSAPI_Semaphore *self)
{
    if (self != NULL)
    {
        if (self->semaphore != NULL)
        {
            vSemaphoreDelete(self->semaphore);
        }
        OSAPI_Heap_freeStructure(self);
    }
    return RTI_TRUE;
}

struct OSAPI_Semaphore *
OSAPI_Semaphore_new(void)
{
    struct OSAPI_Semaphore *semaphore = NULL;

//    if (OSAPI_Semaphore_g_return_NULL)
//    {
//        return NULL;
//    }

    OSAPI_Heap_allocateStructure(&semaphore, struct OSAPI_Semaphore);

    if (semaphore == NULL)
    {
        return NULL;
    }

    vSemaphoreCreateBinary(semaphore->semaphore);
    if (semaphore->semaphore == NULL)
    {
        OSAPI_Heap_freeStructure(semaphore);
        return NULL;
    }

    return semaphore;
}

RTI_BOOL
OSAPI_Semaphore_take(struct OSAPI_Semaphore *self, int timeoutMs, int *failReason)
{
    if (self == NULL)
    {
        return RTI_FALSE;
    }
    if (timeoutMs != OSAPI_SEMAPHORE_TIMEOUT_INFINITE)
    {
        timeoutMs /= portTICK_RATE_MS;
    }
    do
    {
        if (pdTRUE ==
            xSemaphoreTake(self->semaphore, timeoutMs))
        {
            return RTI_TRUE;
        }
    }
    while (timeoutMs == OSAPI_SEMAPHORE_TIMEOUT_INFINITE);


    return RTI_TRUE;
}

RTI_BOOL
OSAPI_Semaphore_give(struct OSAPI_Semaphore *self)
{
    if (self == NULL)
    {
        return RTI_FALSE;
    }
    if (pdTRUE != xSemaphoreGive(self->semaphore))
    {
        return RTI_TRUE;
    }
    return RTI_TRUE;
}

#endif
