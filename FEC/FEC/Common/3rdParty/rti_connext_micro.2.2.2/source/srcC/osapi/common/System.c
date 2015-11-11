/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#include "osapi/osapi_thread.h"
#include "osapi/osapi_types.h"
#include "osapi/osapi_system.h"
#include "osapi/osapi_log.h"

struct OSAPI_System
{
    RTI_BOOL is_initialized;
    struct OSAPI_SystemProperty property;
};

RTI_UINT32 OSAPI_System_g_size;


#define OSAPI_System_INITIALIZER \
{ \
    RTI_FALSE, \
    OSAPI_SystemProperty_INITIALIZER \
}


/* This is a portable function, but not to be used outside of the
 * OSAPI library
 */
struct OSAPI_System*
OSAPI_System_get_instance(void);

RTI_PRIVATE RTI_INT32 OSAPI_System_g_object_id = 0;

/*** SOURCE_BEGIN ***/

RTI_INT32
OSAPI_System_get_next_object_id(void)
{
    ++OSAPI_System_g_object_id;

    if (OSAPI_System_g_object_id == 0xffffff)
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_GET_NEXT_OBJECT_ID_FAILED);
        return -1;
    }

    return OSAPI_System_g_object_id;
}

RTI_BOOL
OSAPI_System_get_property(struct OSAPI_SystemProperty *property)
{
    struct OSAPI_System *system = OSAPI_System_get_instance();

    LOG_PRECONDITION(property == NULL,return RTI_FALSE;)

    *property = system->property;

    return RTI_TRUE;
}

RTI_BOOL
OSAPI_System_set_property(struct OSAPI_SystemProperty *property)
{
    struct OSAPI_System *system = OSAPI_System_get_instance();

    LOG_PRECONDITION(property == NULL,return RTI_FALSE;)

    if (system->is_initialized)
    {
        OSAPI_Log_error(OSAPI_LOG_SYSTEM_SET_PROPERTY_FAILED);
        return RTI_FALSE;
    }

    system->property = *property;

    return RTI_TRUE;
}

/* END-OF_FILE: System.c */
