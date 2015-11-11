/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef util_makeheader_h
#include "util/util_makeheader.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif

/*** SOURCE_BEGIN ***/

void
RT_Component_initialize(RT_Component_T *c,
                       struct RT_ComponentI *intf,
                       RTI_UINT32 id,
                       const struct RT_ComponentProperty *const property,
                       const struct RT_ComponentListener *const listener)
{
    c->id = id;
    c->_intf = intf;
    c->_flags = 0;
    if (property)
    {
        c->_property = *property;
    }
    if (listener)
    {
        c->_listener = *listener;
    }
}

void
RT_Component_finalize(RT_Component_T *c)
{
}

void
RT_Component_run(RT_Component_T * self)
{
}
