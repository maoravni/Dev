/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_common_h
#define netio_common_h


#include <errno.h>
#include <string.h>
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef reda_buffer_h
#include "reda/reda_buffer.h"
#endif
#ifndef netio_dll_h
#include "netio/netio_dll.h"
#endif
#ifndef netio_socket_h
#include "netio/netio_socket.h"
#endif

#ifdef RTI_VXWORKS
#include <vxWorks.h>
#include <net/if.h>
#include <netinet/in.h>
#include <ioctl.h>
#include <ioLib.h>
#include <sockLib.h>
#ifdef RTI_RTP
  #include <strings.h>
#endif
#elif defined(RTI_UNIX) || defined(RTI_DARWIN)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define NETIO_ntohs ntohs
#define NETIO_htons htons

#define NETIO_ntohl ntohl
#define NETIO_htonl htonl

#define NETIO_PING_MSG "RTIPING"
#define NETIO_PING_MSG_SIZE (7)

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* netio_common_h */
