/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "osapi/osapi_config.h"

#ifdef HAVE_SOCKET_API
#include <stdlib.h>
#include <errno.h>

#include "util/util_makeheader.h"
#include "netio/netio_socket.h"
#include "osapi/osapi_types.h"
#include "osapi/osapi_string.h"
#include "osapi/osapi_log.h"

#if defined(RTI_VXWORKS)
#include <ioLib.h>
#include <ioctl.h>
#include <hostLib.h>
#include <netinet/tcp.h>
#elif defined(RTI_UNIX)
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netdb.h>
#if defined(RTI_LINUX)
#include <sys/ioctl.h>
#endif
#endif

#define RTI_OSAPI_SOCKET_UDPV4_ADDRESS_NAME_LENGTH_MAX 64

#endif

/*** SOURCE_BEGIN ***/

#ifdef HAVE_SOCKET_API

RTI_BOOL
OSAPISocketModule_init()
{
#if defined(RTI_WIN32) || defined(RTI_WINCE)
WSADATA wd;
int rtCode;
#endif
    static RTI_BOOL alreadyCalled = RTI_FALSE;

    if (alreadyCalled)
    {
        return RTI_TRUE;
    }

#if defined(RTI_WIN32) || defined(RTI_WINCE)

    if (!(rtCode = WSAStartup(0x0202, &wd)) ||  /* 2.2 success */
        !(rtCode = WSAStartup(0x0002, &wd)))
    {                           /* 2.0 success */
    }
    else
    {
        LOG_printError("OS error: %s failed with error: %d", "WSAStartup",
                       rtCode);

        return RTI_FALSE;
    }

    /* Check version returned */
    if (HIBYTE(wd.wVersion) != 2)
    {                           /* must not be older than 2.0 */
        LOG_printError("Error: Incompatible WinSock version, make sure you"
                       "are linking against ws2_32.lib.");
        return RTI_FALSE;
    }
#endif

    alreadyCalled = RTI_TRUE;
    return RTI_TRUE;
}

/* Called when converting string representation of IP address
*/ 
RTI_INT32
OSAPISocket_getHostByName(struct in_addr *address_out, const char *hostName_in)
{
    RTI_INT32 result = -1;

    char hostname[RTI_OSAPI_SOCKET_UDPV4_ADDRESS_NAME_LENGTH_MAX];

#if (defined(RTI_WIN32) || defined(RTI_UNIX) || (LWIP_DNS && LWIP_SOCKET))
    struct hostent *hostEntry = NULL;
#endif

    LOG_testPrecondition(address_out == NULL, goto done);

    if (!OSAPISocketModule_init())
    {
        LOG_printError("OS error: %s failed with error: %d",
                       "OSAPISocketModule_init", OSAPISocket_getError());
        goto done;
    }

    if (hostName_in == NULL)    
    {
    	/* by default, lwIP does not support gethostname. Needs to be ported
    	 * for below to pass.
    	 */
        if (netiosock_gethostname(
            hostname, RTI_OSAPI_SOCKET_UDPV4_ADDRESS_NAME_LENGTH_MAX - 1) != 0)
        {
            LOG_printError("OS error: %s failed with error: %d",
                           "gethostname", OSAPISocket_getError());
            goto done;
        }

        hostName_in = hostname;
    }

#if (defined(RTI_WIN32) || defined(RTI_UNIX) || (LWIP_DNS && LWIP_SOCKET))

    hostEntry = (struct hostent *)netiosock_gethostbyname((char *)hostName_in);
    if (!hostEntry)
    {
        LOG_printError("OS error: %s failed with error: %d",
                       "gethostbyname", OSAPISocket_getError());
        goto done;
    }
    OSAPI_Memory_copy(address_out, hostEntry->h_addr_list[0],
                     hostEntry->h_length);

#elif defined(RTI_VXWORKS)
#ifdef RTI_CERT
    /* cert API subset does not support hostGetByName() */
    goto done;
#else
    address_out->s_addr = hostGetByName((char *)hostName_in);

    if (address_out->s_addr == (unsigned int)ERROR)
    {
        LOG_printError("OS error: %s failed with error: %d",
                       "hostGetByName", OSAPISocket_getError());

        goto done;
    }
#endif
#else
/* Not supported for FreeRTOS with lwIP. 
 * Needs equivalent of gethostbyname.
 */
#error "Compiler Error: this method need to be ported."

#endif

    result = 0;
    done:
    return result;
}


#endif
