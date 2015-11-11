/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#include "osapi/osapi_config.h"

#ifdef HAVE_SOCKET_API

#include <stdlib.h>
#include <errno.h>
#ifdef RTI_VXWORKS
#include <netinet/in.h>
#elif defined(RTI_UNIX)
#include <unistd.h>
#ifdef RTI_SOL2
#include <sys/sockio.h>
#elif defined(RTI_LINUX)
#include <linux/sockios.h>
#include <sys/ioctl.h>
#elif defined(RTI_DARWIN)
#include <sys/ioctl.h>
#endif
#elif defined(RTI_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <netioapi.h>
#elif (defined(LWIP_SYS) && defined(HAVE_SOCKET_API))
#include "lwip/sockets.h"
#endif

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#include "osapi/osapi_string.h"
#ifndef db_api_h
#include "db/db_api.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif
#include "netio/netio_config.h"
#include "netio/netio_common.h"
#include "netio/netio_socket.h"
#include "reda/reda_sequence.h"
#ifndef netio_log_h
#include "netio/netio_log.h"
#endif
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_interface_h
#include "netio/netio_interface.h"
#endif

typedef enum {
    state_begin,
    state_find_prefix,
    state_find_address,
    state_find_port,
    state_find_at,
    state_find_slash,
    state_find_slashslash,
    state_find_string,
    state_find_delimiter,
    state_find_done
} NETIO_ParseStateKind_T;

typedef enum {
    token_none,
    token_digit,
    token_string,
    token_at,
    token_slash,
    token_slashslash,
    token_null,
    token_delimiter,
    token_colon,
    token_unknown
} NETIO_AddressTokenKind_T;

typedef union
{
    char string_token[1024];
    RTI_INT32 digit_token;
} NETO_TokenValue_T;

typedef struct
{
    NETIO_AddressTokenKind_T kind;
    NETO_TokenValue_T value;
} NETO_Token_T;

/*** SOURCE_BEGIN ***/

#define T struct NETIO_Address
#define TSeq NETIO_AddressSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

#define T struct NETIO_Netmask
#define TSeq NETIO_NetmaskSeq
#undef reda_sequence_gen_h
#include "reda/reda_sequence_gen.h"
#undef T
#undef TSeq

/*
 * String to address conversions.
 */
#define INADDRSZ         4

RTI_PRIVATE RTI_INT32
NETIO_Address_pton4(const char *src, unsigned char *dst)
{
    static const char digits[] = "0123456789";
    int saw_digit, octets, ch;
    unsigned char tmp[INADDRSZ], *tp;

    saw_digit = 0;
    octets = 0;
    *(tp = tmp) = 0;
    while ((ch = *src++) != '\0')
    {
        const char *pch;

        if ((pch = strrchr(digits, ch)) != NULL)
        {
            unsigned int new = *tp * 10 + (pch - digits);

            if (new > 255)
            {
                return (0);
            }
            *tp = new;
            if (!saw_digit)
            {
                if (++octets > 4)
                {
                    return (0);
                }
                saw_digit = 1;
            }
        }
        else if (ch == '.' && saw_digit)
        {
            if (octets == 4)
            {
                return (0);
            }
            *++tp = 0;
            saw_digit = 0;
        }
        else
        {
            return (0);
        }
    }
    if (octets < 4)
    {
        return (0);
    }

    OSAPI_Memory_copy(dst, tmp, INADDRSZ);

    return (1);
}

RTI_BOOL
NETIO_Address_ipv4_string_to_address(struct NETIO_Address *address_out,
                                     const char *address_in)
{
    struct sockaddr_in sockAddr;
    unsigned char addr[4];

    OSAPI_Memory_zero(address_out, sizeof(struct NETIO_Address));

    /* Check to see if it's in IPv4 or IPv6 notation */
    if (NETIO_Address_pton4(address_in, addr))
    {
        address_out->kind = NETIO_ADDRESS_KIND_UDPv4;
        address_out->value.ipv4.address = 0;
        address_out->value.ipv4.address |= (addr[0] << 24);
        address_out->value.ipv4.address |= (addr[1] << 16);
        address_out->value.ipv4.address |= (addr[2] << 8);
        address_out->value.ipv4.address |= (addr[3]);
        return RTI_TRUE;
    }

    if (OSAPISocket_getHostByName(&sockAddr.sin_addr, address_in))
    {
        NETIO_Log_error(LOG_FAIL_GET_HOST_BY_NAME);
        return RTI_FALSE;
    }

    address_out->kind = NETIO_ADDRESS_KIND_UDPv4;
    address_out->value.ipv4.address = sockAddr.sin_addr.s_addr;
    address_out->value.ipv4.address = NETIO_ntohl(address_out->value.ipv4.address);

    return RTI_TRUE;
}

RTI_PRIVATE RTI_BOOL
NETIO_Address_get_token(const char **c_ptr,
                        NETO_Token_T *token,
                        RTI_BOOL allow_address)
{
    RTI_INT32 string_index=0;
    RTI_BOOL done = RTI_FALSE;
    RTI_BOOL rval = RTI_FALSE;
    const char *s_ptr = *c_ptr;

    token->kind = token_none;

    while (**c_ptr && !done)
    {
        switch(token->kind)
        {
            case token_none:
                if ((**c_ptr >= '0') && (**c_ptr <= '9'))
                {
                    token->kind = token_digit;
                    string_index = 1;
                    token->value.digit_token = 0;
                    token->value.digit_token += **c_ptr - '0';
                    (*c_ptr)++;
                }
                else if (((**c_ptr >= 'a') && (**c_ptr <= 'z')) ||
                         ((**c_ptr >= 'A') && (**c_ptr <= 'Z')) ||
                         (**c_ptr == '_') ||
                         (allow_address &&
                                 ((**c_ptr == '.') ||
                                  (**c_ptr == ':') ||
                                  (**c_ptr == '#') ||
                                  ((**c_ptr >= '0') && (**c_ptr <= '9')))))
                {
                    token->kind = token_string;
                    string_index = 0;
                    token->value.string_token[string_index] = **c_ptr;
                    ++string_index;
                    token->value.string_token[string_index] = 0;
                    (*c_ptr)++;
                }
                else if (**c_ptr == '@')
                {
                    token->kind = token_at;
                    (*c_ptr)++;
                    done = RTI_TRUE;
                    rval = RTI_TRUE;
                }
                else if (**c_ptr == '/')
                {
                    token->kind = token_slash;
                    (*c_ptr)++;
                }
                else if (**c_ptr == ':')
                {
                    token->kind = token_colon;
                    (*c_ptr)++;
                }
                break;
            case token_digit:
                if ((**c_ptr >= '0') && (**c_ptr <= '9'))
                {
                    ++string_index;
                    token->value.digit_token *= 10;
                    token->value.digit_token += **c_ptr - '0';
                    (*c_ptr)++;
                }
                else if (allow_address && ((**c_ptr == '.') ||
                        (**c_ptr == ':') ||
                        (**c_ptr == '#') ||
                        ((**c_ptr >= 'a') && (**c_ptr <= 'f')) ||
                        ((**c_ptr >= 'A') && (**c_ptr <= 'F'))))
                {
                    RTI_INT32 i;
                    token->kind = token_string;
                    for (i = 0; i < string_index; i++)
                    {
                        token->value.string_token[i] = *s_ptr;
                        s_ptr++;
                    }
                    token->value.string_token[i] = 0;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_TRUE;
                }
                break;
            case token_slash:
                if (**c_ptr == '/')
                {
                    token->kind = token_slashslash;
                    (*c_ptr)++;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_TRUE;
                }
                break;
            case token_colon:
                if (**c_ptr == '/')
                {
                    token->kind = token_delimiter;
                    (*c_ptr)++;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_TRUE;
                }
                break;
            case token_delimiter:
                if (**c_ptr == '/')
                {
                    (*c_ptr)++;
                }
                else
                {
                    token->kind = token_unknown;
                }
                done = RTI_TRUE;
                rval = RTI_TRUE;
                break;
            case token_string:
                if (((**c_ptr >= 'a') && (**c_ptr <= 'z')) ||
                     ((**c_ptr >= 'A') && (**c_ptr <= 'Z')) ||
                     ((**c_ptr >= '0') && (**c_ptr <= '9')) ||
                     (**c_ptr == '_') ||
                         (allow_address &&
                                 ((**c_ptr == '.') ||
                                  (**c_ptr == ':') ||
                                  (**c_ptr == '#'))))
                {
                    token->value.string_token[string_index] = **c_ptr;
                    ++string_index;
                    token->value.string_token[string_index] = 0;
                    (*c_ptr)++;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_TRUE;
                }
                break;
            default:
                token->kind = token_unknown;
                done = RTI_TRUE;
                rval = RTI_FALSE;
                break;
        }
    }

    if ((token->kind == token_none) && (**c_ptr == 0))
    {
        token->kind = token_null;
    }

    if (**c_ptr == 0)
    {
        rval = RTI_TRUE;
    }

#if NETIO_DBEUG
    if (token->kind == token_digit)
    {
        printf("%d\n",token->value.digit_token);
    }
    else if (token->kind == token_string)
    {
        printf("[%s]\n",token->value.string_token);
    }
    else if (token->kind == token_at)
    {
        printf("@\n");
    }
    else if (token->kind == token_slash)
    {
        printf("/\n");
    }
    else if (token->kind == token_delimiter)
    {
        printf("://\n");
    }
#endif

    return rval;
}

RTI_BOOL
NETIO_Address_parse(const char *name,
                    RTI_UINT32 *base_port,
                    RTI_INT32 *index,
                    RT_ComponentFactoryId_T *out_id,
                    char *address_string)
{
    RTI_BOOL rval = RTI_FALSE;
    const char *c_ptr;
    NETIO_ParseStateKind_T parse_state;
    NETO_Token_T token;
    RTI_BOOL allow_address = RTI_FALSE;
    RTI_BOOL done = RTI_FALSE;

    LOG_PRECONDITION_P3(((name == NULL) || (out_id == NULL) ||
                         (address_string == NULL)),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(name),
                        OSAPI_LOG_PTR_LOW(out_id),
                        OSAPI_LOG_PTR_LOW(address_string))

    /*
     * Algorithm:
     *
     * An address is on the form
     * [index @]<interface>://[<address>[/port]]
     *
     * - break address string into
     *
     * - A sequence of index ranges (0 if none is given)
     * - interface_name (must be given)
     * - address_string
     * - a port number (0) if none is given
     *
     * - Lookup intf for address 0 (default for any address) based on ifname
     *
     * For each {index,address,port} do:
     *  port = resolve_port(param,port_base,index)
     *  call intf->resolve(port,address_string,resolved);
     * done
     *
     * The result of this is
     *  - the interface
     *  - A sequence of addresses (kind,port,address)
     *
     *  NOTE: This is just address resolution and does not do anything other
     *  then resolve address.
     */

    *index = 0;
    *base_port = 0;
    c_ptr = name;
    RT_ComponentFactoryId_clear(out_id);

    parse_state = state_begin;
    address_string[0] = 0;

    while (NETIO_Address_get_token(&c_ptr,&token,allow_address) && !done)
    {
        switch (parse_state)
        {
            case state_begin:
                if (token.kind == token_digit)
                {
                    /* Assume index, look for @ */
                    *index = token.value.digit_token;
                    parse_state = state_find_at;
                }
                else if (token.kind == token_string)
                {
                    /* Assume prefix */
                    OSAPI_Memory_copy(&out_id->_name,token.value.string_token,OSAPI_String_length(token.value.string_token)+1);
                    parse_state = state_find_delimiter;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_FALSE;
                }
                allow_address = RTI_FALSE;
                break;
            case state_find_at:
                if (token.kind == token_at)
                {
                    /* path = index@ */
                    parse_state = state_find_prefix;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_FALSE;
                }
                allow_address = RTI_FALSE;
                break;
            case state_find_prefix:
                if (token.kind == token_string)
                {
                    /* path = index@prefix */
                    OSAPI_Memory_copy(&out_id->_name,token.value.string_token,OSAPI_String_length(token.value.string_token)+1);
                    parse_state = state_find_delimiter;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_FALSE;
                }
                allow_address = RTI_FALSE;
                break;
            case state_find_delimiter:
                parse_state = state_find_address;
                allow_address = RTI_TRUE;
                break;
            case state_find_address:
                if (token.kind == token_string)
                {
                    OSAPI_Memory_copy(address_string,
                            token.value.string_token,
                            OSAPI_String_length(token.value.string_token)+1);
                    parse_state = state_find_slash;
                }
                else if (token.kind == token_null)
                {
                    rval = RTI_TRUE;
                    done = RTI_TRUE;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_FALSE;
                }
                allow_address = RTI_FALSE;
                break;
            case state_find_slash:
                if (token.kind == token_slash)
                {
                    parse_state = state_find_port;
                }
                else if (token.kind == token_null)
                {
                    rval = RTI_TRUE;
                    done = RTI_TRUE;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_FALSE;
                }
                allow_address = RTI_FALSE;
                break;
            case state_find_port:
                if (token.kind == token_digit)
                {
                    *base_port = token.value.digit_token;
                    parse_state = state_find_done;
                }
                else
                {
                    done = RTI_TRUE;
                    rval = RTI_FALSE;
                }
                allow_address = RTI_FALSE;
                break;
            case state_find_done:
                if (token.kind == token_null)
                {
                    rval = RTI_TRUE;
                }
                else
                {
                    rval = RTI_FALSE;
                }
                done = RTI_TRUE;
                allow_address = RTI_FALSE;
                break;
            default:
                break;
        }
    }

    return rval;
}

#endif
