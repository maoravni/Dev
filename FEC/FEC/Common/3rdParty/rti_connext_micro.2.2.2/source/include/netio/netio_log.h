/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_log_h
#define netio_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

 /* 1 int errno */
#define LOG_GETNAME_FAILED                                           0x00000167

#define LOG_FAIL_CREATE_SOCKET                                       0x00000168
 /* 1 int errno */
#define LOG_FAIL_IOCTL_SIOCGIFCONF                                   0x00000169
/* 1 int errno */
#define LOG_FAIL_IOCTL_SIOCGIFFLAGS                                  0x0000016a
 /* 1 int errno */
#define LOG_FAIL_CLOSE_SOCKET                                        0x0000016b
#define LOG_FAIL_GET_HOST_BY_NAME                                    0x0000016c
 /* 1 int max entry ports */
#define LOG_EXCEED_MAX_ENTRY_PORTS                                   0x0000016d
 /* 1 int errno */
#define LOG_FAIL_FCNTL_FD_CLOEXEC                                    0x0000016e
#define LOG_FAIL_REUSE_MULTICAST_PORT                                0x0000016f
/* 1 int interface, 1 int port, 1 int errno */
#define LOG_FAIL_SOCKET_BINDING                                      0x00000170
/* 1 int receive buffer size, 1 int errno */
#define LOG_FAIL_SOCKET_OPTIONING_SO_RCVBUF                          0x00000171
/* 1 int address */
#define LOG_FAIL_JOIN_MULTICAST_GROUP                                0x00000172
 /* 1 int address */
#define LOG_JOIN_MULTICAST_GROUP                                     0x00000173
/* 1 int address */
#define LOG_DID_NOT_JOIN_MULTICAST_GROUP                             0x00000174
 /* 1 int interface, 1 int port, 1 int multicast address */
#define LOG_MULTICAST_MEMBERSHIP_ALREADY_EXISTS                      0x00000175
 /* 1 int interface, 1 int port, 1 int multicast address, 1 int errno */
#define LOG_FAIL_SOCKET_OPTIONING_IP_ADD_MEMBERSHIP                  0x00000176
 /* 1 int interface, 1 int port, 1 int length */
#define LOG_START_BLOCKING_RECEIVE                                   0x00000177
 /* 1 int interface, 1 int port, 1 int errno */
#define LOG_FAIL_RECVFROM                                            0x00000178
 /* 1 int address, 1 int port */
#define LOG_SEND_UNBLOCK                                             0x00000179
 /* 1 int address, 1 int port, 1 int result, 1 int errno */
#define LOG_FAIL_SEND_UNBLOCK                                        0x0000017a
#define LOG_ENTRY_PORT_NOT_EVEN_INITIALIZED_NOTHING_TO_REMOVE        0x0000017b
/* 1 int bytes, 1 int addr, 1 int port, 1 int errno */
#define LOG_FAIL_SEND_REMOTE_HOST_DOWN                               0x0000017c
 /* 1 int bytes, 1 int addr, 1 int port, 1 int errno */
#define LOG_FAIL_SEND_LOCAL_NETWORK_DOWN                             0x0000017d
 /* 1 int bytes wanted, 1 int bytes sent, 1 int errno */
#define LOG_FAIL_SOCKET_SEND                                         0x0000017e
 /* 1 int address, 1 int port */
#define LOG_FAIL_ADD_NEW_ENTRY_DESTINATION_TABLE_ALREADY_FULL        0x0000017f
 /* 1 int address, 1 int port, 1 int errno */
#define LOG_FAIL_SEND_PING                                           0x00000180
 /* 1 int addr, 1 int port, 1 int id, 1 int count, 1 int left */
#define LOG_COMPLETELY_REMOVE_DESTINATION_TABLE_ENTRY                0x00000181
 /* 1 int send buffer size, 1 int errno */
#define LOG_FAIL_SOCKET_OPTIONING_SO_SNDBUF                          0x00000182
 /* 1 int option, 1 int errno */
#define LOG_FAIL_SOCKET_OPTIONING_IP_MULTICAST_LOOP                  0x00000183
 /* 1 int option, 1 int errno */
#define LOG_FAIL_SOCKET_OPTIONING_IP_MULTICAST_TTL                   0x00000184
 /* 1 int addr, 1 int errno */
#define LOG_FAIL_SOCKET_OPTIONING_IP_MULTICAST_IF                    0x00000185
#define LOG_INTERFACE_FAIL_INITIALIZE                                0x00000200
#define LOG_INTERFACE_FAIL_BIND                                      0x00000201
#define LOG_INTERFACE_FAIL_BIND_EXTERNAL                             0x00000202
#define LOG_INTERFACE_FAIL_UNBIND                                    0x00000203
#define LOG_INTERFACE_FAIL_UNBIND_EXTERNAL                           0x00000204
#define LOG_INTERFACE_FAIL_SEND                                      0x00000205
#define LOG_INTERFACE_FAIL_RECEIVE                                   0x00000206
#define LOG_INTERFACE_FAIL_REQUEST                                   0x00000207
#define LOG_INTERFACE_FAIL_RETURN_LOAN                               0x00000208
#define LOG_INTERFACE_FAIL_ACK                                       0x00000209
#define LOG_INTERFACE_NOT_ENABLED                                    0x0000020a
#define LOG_INTERFACE_MISMATCH                                       0x0000020b
#define LOG_INTERFACE_UNSUPPORTED                                    0x0000020c

#define NETIO_LOG_ROUTETABLE_NEW_ALLOC_FAILED                        0x0000020d
#define NETIO_LOG_ROUTETABLE_TABLE_CREATE_FAILED                     0x0000020e
#define NETIO_LOG_ROUTETABLE_TABLE_DELETE_FAILED                     0x0000020f
#define NETIO_LOG_ROUTETABLE_ROUTE_CREATE_FAILED                     0x00000210
#define NETIO_LOG_ROUTETABLE_ROUTE_INSERT_FAILED                     0x00000211
#define NETIO_LOG_ROUTETABLE_ROUTE_DELETE_FAILED                     0x00000212



#include "netio/netio_log_impl.h"

#endif /* netio_log_h */
