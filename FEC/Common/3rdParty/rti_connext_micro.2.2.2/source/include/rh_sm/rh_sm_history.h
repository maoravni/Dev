/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rh_sm_history_h
#define rh_sm_history_h



#ifndef rh_sm_dll_h
#include "rh_sm/rh_sm_dll.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_mutex_h
#include "osapi/osapi_mutex.h"
#endif
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef osapi_system_h
#include "osapi/osapi_system.h"
#endif
#ifndef reda_buffer_h
#include "reda/reda_buffer.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct RHSM_History;

#define RHSM_HISTORY_VERSION_MAJOR 1
#define RHSM_HISTORY_VERSION_MINOR 0
#define RHSM_HISTORY_VERSION \
  ((RHSM_HISTORY_VERSION_MAJOR<<16) || (RHSM_HISTORY_VERSION_MINOR))

struct RHSM_HistoryProperty
{
    struct DDSHST_ReaderProperty _parent;
    RTI_INT32 _dummy;
};

#define RHSM_HistoryProperty_INITIALIZER {\
    DDSHST_ReaderProperty_INITIALIZER,\
    0,\
}

#define RHSM_HISTORY_INTERFACE_ID \
                            RT_MKINTERFACEID(RT_COMPONENT_CLASS_RHISTORY,\
                            RT_COMPONENT_INSTANCE_RHISTORY_SM)

RHSMDllExport struct RT_ComponentFactoryI*
RHSM_HistoryFactory_get_interface(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* rh_sm_history_h */
