/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_socket_h
#define netio_socket_h


#ifndef RTI_WINCE
#include <errno.h>
#endif

  /* Architectures that require u_char (standard POSIX header) */
#if (defined(RTI_UNIX) || defined(RTI_VXWORKS))
#include <sys/types.h>
#endif

#if defined(RTI_VXWORKS) 
#include <netinet/in.h>
#elif defined(RTI_UNIX)
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#elif defined(RTI_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#ifndef EADDRINUSE
#define EADDRINUSE WSAEADDRINUSE
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#elif defined(LWIP_SYS)
#include "lwip/arch.h"
#if defined(HAVE_SOCKET_API)
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#endif
#endif

#if defined(RTI_VXWORKS)
#include <ioLib.h>
#include <sockLib.h>
#endif

#ifndef netio_dll_h
#include "netio/netio_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(RTI_WIN32)
/*e \ingroup OSAPISocketClass
  
  Get the last error code set.  To make sure you are not reading an
  error code corresponding to a previous call, set the error code to
  RTI_OSAPI_SOCKET_NOERROR because making a netio call. This call is not
  thread-safe on some OSes at the moment.  
  
  @see RtiNetioErrnoSet RTI_OSAPI_SOCKET_NOERROR 
*/

#define OSAPISocket_getError WSAGetLastError
/*e \ingroup OSAPISocketClass
  
  Set the system level error code.
  
  @param ErrorNumber Error code.
  
  @see RtiNetioErrnoGet
*/
#define OSAPISocket_setError WSASetLastError

#define RTI_OSAPI_SOCKET_MULTICAST_LOOPBACK_ON  TRUE
#define RTI_OSAPI_SOCKET_MULTICAST_LOOPBACK_OFF FALSE
#define RTI_OSAPI_SOCKET_BROADCAST_ON  TRUE
#define RTI_OSAPI_SOCKET_BROADCAST_OFF FALSE
#else
#ifdef RTI_VXWORKS
#ifndef errno
#define errnoGet() errno
#endif
#endif
#define OSAPISocket_getError() errno
#define OSAPISocket_setError(x) errno = (x)
#define RTI_OSAPI_SOCKET_BROADCAST_ON  1
#define RTI_OSAPI_SOCKET_BROADCAST_OFF 0
#define RTI_OSAPI_SOCKET_MULTICAST_LOOPBACK_ON  1
#define RTI_OSAPI_SOCKET_MULTICAST_LOOPBACK_OFF 0
#endif

/* socket API */
#if (defined(LWIP_SYS) && defined(HAVE_SOCKET_API))
#define netiosock_accept(a,b,c)         lwip_accept(a,b,c)
#define netiosock_bind(a,b,c)           lwip_bind(a,b,c)
#define netiosock_shutdown(a,b)         lwip_shutdown(a,b)
#define netiosock_closesocket(s)        lwip_close(s)
#define netiosock_connect(a,b,c)        lwip_connect(a,b,c)
#define netiosock_getsockname(a,b,c)    lwip_getsockname(a,b,c)
#define netiosock_getpeername(a,b,c)    lwip_getpeername(a,b,c)
#define netiosock_setsockopt(a,b,c,d,e) lwip_setsockopt(a,b,c,d,e)
#define netiosock_getsockopt(a,b,c,d,e) lwip_getsockopt(a,b,c,d,e)
#define netiosock_listen(a,b)           lwip_listen(a,b)
#define netiosock_recv(a,b,c,d)         lwip_recv(a,b,c,d)
#define netiosock_recvfrom(a,b,c,d,e,f) lwip_recvfrom(a,b,c,d,e,f)
#define netiosock_send(a,b,c,d)         lwip_send(a,b,c,d)
#define netiosock_sendto(a,b,c,d,e,f)   lwip_sendto(a,b,c,d,e,f)
#define netiosock_socket(a,b,c)         lwip_socket(a,b,c)
#define netiosock_select(a,b,c,d,e)     lwip_select(a,b,c,d,e)
#define netiosock_ioctlsocket(a,b,c)    lwip_ioctl(a,b,c)
#define netiosock_read(a,b,c)           lwip_read(a,b,c)
#define netiosock_write(a,b,c)          lwip_write(a,b,c)
#define netiosock_close(s)              lwip_close(s)
#define netiosock_sendmsg(a,b,c)        -1 /* unsupported */
#define netiosock_gethostname(a,b)      -1 /* unsupported */
#if LWIP_DNS
#define netiosock_gethostbyname(n)		lwip_gethostbyname(n)
#else
#define netiosock_gethostbyname(n)		-1 /* unsupported */
#endif
#else
#define netiosock_accept(a,b,c)         accept(a,b,c)
#define netiosock_bind(a,b,c)           bind(a,b,c)
#define netiosock_shutdown(a,b)         shutdown(a,b)
#define netiosock_connect(a,b,c)        connect(a,b,c)
#define netiosock_getsockname(a,b,c)    getsockname(a,b,c)
#define netiosock_getpeername(a,b,c)    getpeername(a,b,c)
#define netiosock_setsockopt(a,b,c,d,e) setsockopt(a,b,c,d,e)
#define netiosock_getsockopt(a,b,c,d,e) getsockopt(a,b,c,d,e)
#define netiosock_listen(a,b)           listen(a,b)
#define netiosock_recv(a,b,c,d)         recv(a,b,c,d)
#define netiosock_recvfrom(a,b,c,d,e,f) recvfrom(a,b,c,d,e,f)
#define netiosock_send(a,b,c,d)         send(a,b,c,d)
#define netiosock_sendto(a,b,c,d,e,f)   sendto(a,b,c,d,e,f)
#define netiosock_sendmsg(a,b,c)        sendmsg(a,b,c)
#define netiosock_socket(a,b,c)         socket(a,b,c)
#define netiosock_select(a,b,c,d,e)     select(a,b,c,d,e)
#define netiosock_ioctlsocket(a,b,c)    ioctl(a,b,c)
#define netiosock_read(a,b,c)           read(a,b,c)
#define netiosock_write(a,b,c)          write(a,b,c)
#if defined(RTI_WIN32)
#define netiosock_close(s)              closesocket(s)
#else
#define netiosock_close(s)              close(s)
#endif
#define netiosock_gethostname(a,b)      gethostname(a,b)
#define netiosock_gethostbyname(n)		gethostbyname(n)
#endif

/*e \ingroup OSAPISocketClass
  @brief Initialize and query avaliability of Netio transport

  This function can be used to query the availability of a netio transport
  (currently only NETIO_IP), and initialize that implementation.

  The function will return RTI_TRUE if successful, RTI_FALSE if the requested
  transport is not available, or the initialization was unsucessful.

  This function must be called at least onece before any other netio
  functions; it is idempotent so that it can be called multiple times
  without side effects.
*/
NETIODllExport RTI_BOOL
OSAPISocketModule_init(void);

/* \ingroup OSAPISocketClass
  @brief Converts a host name into an internet address.

  If the input host name is NULL this function will return 
  the address of the host whose name was found using 
  gethostname.

  @return 0 if successful, -1 if error
*/
NETIODllExport RTI_INT32
OSAPISocket_getHostByName(struct in_addr *address_out, const char *hostName_in);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* netio_socket_h */
