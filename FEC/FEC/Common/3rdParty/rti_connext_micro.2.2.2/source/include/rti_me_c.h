/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rti_me_c_h
#define rti_me_c_h

#ifdef __cplusplus
    extern "C" {
#endif



#ifdef RTIME_DLL_IMPORT
#define RTI_dds_c_DLL_VARIABLE
#define RTI_disc_dpde_DLL_VARIABLE
#define RTI_disc_dpse_DLL_VARIABLE
#define RTI_rh_sm_DLL_VARIABLE
#define RTI_wh_sm_DLL_VARIABLE
#define RTI_rtps_DLL_VARIABLE
#define RTI_netio_DLL_VARIABLE
#define RTI_cdr_DLL_VARIABLE
#define RTI_reda_DLL_VARIABLE
#define RTI_osapi_DLL_VARIABLE
#define RTI_rt_DLL_VARIABLE
#endif

#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_system_h
#include "osapi/osapi_system.h"
#endif
#ifndef osapi_heap_h
#include "osapi/osapi_heap.h"
#endif
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef osapi_stdio_h
#include "osapi/osapi_stdio.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef rtps_rtps_h
#include "rtps/rtps_rtps.h"
#endif
#ifndef netio_common_h
#include "netio/netio_common.h"
#endif
#ifndef reda_buffer_h
#include "reda/reda_buffer.h"
#endif
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif
#ifndef dds_c_discovery_h
#include "dds_c/dds_c_discovery.h"
#endif
#ifndef dds_c_domain_h
#include "dds_c/dds_c_domain.h"
#endif
#ifndef dds_c_publication_h
#include "dds_c/dds_c_publication.h"
#endif
#ifndef dds_c_subscription_h
#include "dds_c/dds_c_subscription.h"
#endif
#ifndef dds_c_sequence_h
#include "dds_c/dds_c_sequence.h"
#endif
#ifndef dds_c_type_h
#include "dds_c/dds_c_type.h"
#endif
#ifndef dds_c_topic_h
#include "dds_c/dds_c_topic.h"
#endif
#ifndef dds_c_string_h
#include "dds_c/dds_c_string.h"
#endif
#ifndef dds_c_data_h
#include "dds_c/dds_c_data.h"
#endif


#ifdef __cplusplus
    }	/* extern "C" */
#endif

#endif /* rti_me_c_h */
