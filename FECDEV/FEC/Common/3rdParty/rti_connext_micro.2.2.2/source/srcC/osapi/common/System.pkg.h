/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef System_pkg_h
#define System_pkg_h

struct OSAPI_System
{
    RTI_BOOL is_initialized;
    struct OSAPI_SystemProperty property;
};

extern RTI_UINT32 OSAPI_System_g_size;


#define OSAPI_System_INITIALIZER \
{ \
    RTI_FALSE, \
    OSAPI_SystemProperty_INITIALIZER \
}

struct OSAPI_System*
OSAPI_System_get_instance(void);

#endif /* System_pkg_h */
