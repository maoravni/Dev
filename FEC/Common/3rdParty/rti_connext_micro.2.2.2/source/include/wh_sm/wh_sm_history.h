/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef wh_sm_history_h
#define wh_sm_history_h



#ifndef wh_sm_dll_h
#include "wh_sm/wh_sm_dll.h"
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
#ifndef dds_c_discovery_h
#include "dds_c/dds_c_discovery.h"
#endif
#ifndef dds_c_type_h
#include "dds_c/dds_c_type.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct WHSM_History;

#define WHSM_HISTORY_VERSION_MAJOR 1
#define WHSM_HISTORY_VERSION_MINOR 0
#define WHSM_HISTORY_VERSION \
  ((WHSM_HISTORY_VERSION_MAJOR<<16) || (WHSM_HISTORY_VERSION_MINOR))

struct WHSM_HistoryProperty
{
    struct DDSHST_WriterProperty _parent;
    RTI_INT32 sample_sort_treshold;
};

#define WHSM_HistoryProperty_INITIALIZER {\
    DDSHST_WriterProperty_INITIALIZER, \
    16\
}

#define WHSM_HISTORY_INTERFACE_ID RT_MKINTERFACEID(\
            RT_COMPONENT_CLASS_WHISTORY,RT_COMPONENT_INSTANCE_WHISTORY_SM)

WHSMDllExport struct RT_ComponentFactoryI*
WHSM_HistoryFactory_get_interface(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#endif /* wh_sm_history_h */
