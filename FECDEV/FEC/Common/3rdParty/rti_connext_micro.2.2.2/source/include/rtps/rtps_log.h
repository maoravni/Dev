/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rtps_log_h
#define rtps_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#define LOG_FAIL_ALLOCATE                                            0x0000018c
/* 1 int user data length */
#define LOG_INVALID_USER_DATA                                        0x0000018d
#define LOG_FAIL_QUERY_SENDER_CAPABILITY                             0x0000018e
 /* 1 int fail reason */
#define LOG_FAIL_MIG_GENERATOR_FLUSH                                 0x0000018f
 /* 1 int fail reason */
#define LOG_FAIL_ADD_HEARTBEAT                                       0x00000190
#define LOG_FAIL_CREATE_GENERATOR_CONTEXT                            0x00000191
 /* 1 int fail reason */
#define LOG_FAIL_ADD_GAP                                             0x00000192
#define LOG_FAIL_SEND_DATA                                           0x00000193
 /* 1 int submessage size */
#define LOG_SUBMESSAGE_SIZE_IS_CONSIDERED_JUMBOGRAM                  0x00000194
#define LOG_FAIL_END_INFO_DST_SUBMESSAGE                             0x00000195
#define LOG_NO_ADD_MULTICAST_REPLY_CURRENT_STATE_IDENTICAL           0x00000196
#define LOG_NO_ADD_MULTICAST_REPLY_UNSPECIFIED                       0x00000197
#define LOG_FAIL_END_INFO_REPLY_SUBMESSAGE                           0x00000198
#define LOG_NO_ADD_INFO_SOURCE_CURRENT_STATE_IDENTICAL               0x00000199
#define LOG_FAIL_END_INFO_SRC_SUBMESSAGE                             0x0000019a
#define LOG_FAIL_END_INFO_TS_SUBMESSAGE                              0x0000019b
#define LOG_FAIL_END_HB_SUBMESSAGE                                   0x0000019c
#define LOG_FAIL_END_DATA_SUBMESSAGE                                 0x0000019d
#define LOG_FAIL_END_ACK_SUBMESSAGE                                  0x0000019e
#define LOG_FAIL_END_GAP_SUBMESSAGE                                  0x0000019f
 /* 1 int protocol version, 1 int implementation version */
#define LOG_PROTOCOL_MAJOR_VERSION_GREATER_THAN_IMPLEMENTATION       0x000001a0
#define LOG_FAIL_POINT                                               0x000001a1
 /* 1 int buffer length, 1 int header size */
#define LOG_MESSAGE_BUFFER_LENGTH_NOT_ENOUGH_FOR_RTPS_HEADER_SIZE    0x000001a2
 /* 1 int bytes, 1 int valid header */
#define LOG_FIRST_BYTES_NOT_VALID_RTPS_HEADER                        0x000001a3
#define LOG_INCORRECT_PROTOCOL_VERSION                               0x000001a4
 /* 1 int submessage header size */
#define LOG_MESSAGE_NOT_ENOUGH_SPACE_FOR_SUBMESSAGE_HEADER           0x000001a5
 /* 1 int bytes left, 1 int data length */
#define LOG_MESSAGE_NOT_ENOUGH_SPACE_FOR_CONTEXT_DATA_LENGTH         0x000001a6
 /* 1 int submessage id, 3 ints guid prefix */
#define LOG_PARSE_SUBMESSAGE                                         0x000001a7
 /* 1 int object id, 4 ints guid */
#define LOG_LISTENER_EXISTS                                          0x000001a8
#define LOG_NOT_ENOUGH_SPACE_FOR_NEW_LISTENER                        0x000001a9
/* 1 int object id, 4 ints guid */
#define LOG_ADD_LISTENER                                             0x000001aa
 /* 4 ints guid, 2 ints sequence number */
#define LOG_RECEIVE_DATA                                             0x000001ab
#define LOG_FAIL_SET_PARAMS_BUFFER                                   0x000001ac
#define LOG_FAIL_SET_DATA_BUFFER                                     0x000001ad
 /* 1 int object id, 4 ints remote guid, 2 ints sample sequence number,
  * 2 ints lead sequence number
  * */
#define LOG_READER_DROPS_PREVIOUS_RECEIVED_SAMPLE                    0x000001ae
#define LOG_FAIL_LOST_DATA                                           0x000001af
#define LOG_FAIL_UPDATE_DATA_RECEIVED_BITMAP                         0x000001b0
 /* 2 ints sequence number */
#define LOG_NEW_DATA_NOT_ACCEPTED                                    0x000001b1
#define LOG_FAIL_REVERT_DATA_RECEIVED                                0x000001b2
#define LOG_FAIL_ADVANCE_DATA_RECEIVED                               0x000001b3
#define LOG_PROCESS_LIVELINESS_HEARTBEAT                             0x000001b4
#define LOG_EPOCH_LESS_THAN_LAST_EPOCH                               0x000001b5
 /* 4 ints remote guid, 2 ints first sn, 2 ints last sn, 1 int epoch */
#define LOG_RECEIVE_HEARTBEAT                                        0x000001b6
#define LOG_ALL_SAMPLES_RECEIVED                                     0x000001b7
 /* 1 int reader object id, 4 ints remote guid */
#define LOG_FAIL_SEND_ACKNACK                                        0x000001b8
 /* 4 ints remote guid, 2 ints first sn */
#define LOG_RECEIVE_GAP                                              0x000001b9
#define LOG_FAIL_ON_COMMIT_DATA                                      0x000001ba
 /* 1 int submessage id */
#define LOG_UNHANDLED_MESSAGE                                        0x000001bb
#define LOG_SERVICE_HAS_BEEN_DELETED                                 0x000001bc
#define LOG_FOUND_MATCHING_BUILTIN_PARTICIPANT_READER                0x000001bd
 /* 1 bool is_writer, 4 ints remote guid */
#define LOG_NO_ENDPOINT_MATCHING_REMOTE_ENDPOINT                     0x000001be
 /* 2 ints lead sequence number, 1 int bitcount, 1 int epoch */
#define LOG_SEND_ACKNACK                                             0x000001bf
#define LOG_SEND_UNKNOWN_SUBMESSAGE                                  0x000001c0
 /* 1 int db_retcode */
#define LOG_FAIL_SELECT_DESTINATION_RECORD                           0x000001c1
 /* 1 int retcode */
#define LOG_FAIL_FLUSH_DESTINATION                                   0x000001c2
 /* 1 int db_retcode */
#define LOG_FAIL_CREATE_DATAREADER_RECORD                            0x000001c3
 /* 1 int object id */
#define LOG_CREATE_DATAREADER_RECORD                                 0x000001c4
 /* 4 ints remote guid */
#define LOG_ASSERT_REMOTE_ENDPOINT                                   0x000001c5
#define LOG_REMOTE_WRITER_LIMIT_REACHED                              0x000001c6
#define LOG_FAIL_CREATE_RTPS_GENERATOR                               0x000001c7
#define LOG_FAIL_DELETE_DATAREADER_TABLE                             0x000001c8
 /* 1 int distance, 1 int bitcount */
#define LOG_SEQUENCE_NUMBER_OUT_OF_RANGE                             0x000001c9
 /* 1 int search bit */
#define LOG_FIRST_SEARCH_BIT_NOT_FOUND                               0x000001ca
#define LOG_FIRST_SEQUENCE_NUMBER_GREATER_THAN_LAST_SEQUENCE_NUMBER  0x000001cb
 /* 1 int bitcount */
#define LOG_BITCOUNT_OUT_OF_BOUNDS                                   0x000001cc
#define LOG_FAIL_SERIALIZE_HOST_ID                                   0x000001cd
#define LOG_FAIL_SERIALIZE_APP_ID                                    0x000001ce
#define LOG_FAIL_SERIALIZE_INSTANCE_ID                               0x000001cf
#define LOG_FAIL_SERIALIZE_OBJECT_ID                                 0x000001d0
#define LOG_STOP_PERIODIC_HEARTBEAT_EVENT_NO_RELIABLE_READERS        0x000001d1
#define LOG_FAIL_SEND_PERIODIC_HEARTBEAT                             0x000001d2
#define LOG_FAIL_ADD_DATA                                            0x000001d3
#define LOG_SEND_DATA                                                0x000001d4
#define LOG_SEND_HEARTBEAT                                           0x000001d5
#define LOG_SEND_GAP                                                 0x000001d6
 /* 1 int db_retcode */
#define LOG_FAIL_CREATE_DATAWRITER_RECORD                            0x000001d7
 /* 1 int object id */
#define LOG_CREATE_DATAWRITER_RECORD                                 0x000001d8
#define LOG_FAIL_DELETE_DATAWRITER_TABLE                             0x000001d9
 /* 2 ints lead sequence number, 1 int bitcount, 1 int epoch */
#define LOG_RECEIVE_ACK                                              0x000001da
 /* 2 ints lead sequence number, 1 int bitcount, 1 int epoch */
#define LOG_RECEIVE_ACKNACK                                          0x000001db
#define LOG_FAIL_GET_DESTINATIONS                                    0x000001dc
#define LOG_FAIL_BEGIN_MESSAGE                                       0x000001dd
 /* 2 ints sequence number */
#define LOG_FAIL_REQUEST_DATA                                        0x000001de
 /* 1 int submessage id, 2 ints sequence number */
#define LOG_RESEND                                                   0x000001df
#define LOG_FAIL_FINISH_MESSAGE                                      0x000001e0
 /* 1 int object id */
#define LOG_FAIL_POST_PERIODIC_HEARTBEAT                             0x000001e1
 /* 1 int submessage id, 3 ints guid prefix */
#define LOG_FAIL_INTERPRETER_PARSE_SUBMESSAGE                        0x000001e2
 /* 4 ints participant key */
#define LOG_UNKNOWN_REMOTE_PARTICIPANT_BY_KEY                        0x000001e3
 /* 1 int retcode */
#define LOG_FAIL_SET_LISTENER                                        0x000001e4
#define LOG_FAIL_BAD_PARAMETER                                       0x000001e5
#define LOG_FAIL_REGISTER_KEY                                        0x000001e6
#define LOG_RTPS_READER_UNSUPPORTED                                  0x00000300
#define LOG_RTPS_FAIL_ROUTE_PACKET                                   0x00000301
#define LOG_RTPS_OUT_OF_RESOURCES                                    0x00000302
#define LOG_RTPS_FAIL_ASSERT_PEER                                    0x00000303
#define LOG_RTPS_FAIL_ASSERT_TRANSPORT                               0x00000304
#define LOG_RTPS_ROUTE_NOT_EXIST                                     0x00000305
#define LOG_RTPS_BIND_NOT_EXIST                                      0x00000306
#define LOG_RTPS_RECEIVE_PURE_ACK                                    0x00000307
#define LOG_RTPS_INVALID_UPSTREAM_INTERFACE                          0x00000308
#define LOG_RTPS_READER_NO_HB_RESPONSE                               0x00000309
#define LOG_RTPS_DATA_OUT_OF_RANGE                                   0x0000030a
#define LOG_RTPS_DATA_ALREADY_RECEIVED                               0x0000030b
#define LOG_RTPS_UNSUPPORTED_INTERFACE                               0x0000030c
#define LOG_RTPS_INVALID_PACKET                                      0x0000030d
#define LOG_RTPS_UNKNOWN_SUBMESSAGE                                  0x0000030e
#define LOG_RTPS_FAIL_RECEIVE_ACKNACK                                0x0000030f
#define LOG_RTPS_FAIL_RECEIVE_DATA                                   0x00000310
#define LOG_RTPS_FAIL_RECEIVE_GAP                                    0x00000311
#define LOG_RTPS_FAIL_RECEIVE_HEARTBEAT                              0x00000312
#define LOG_RTPS_FAIL_RECEIVER_FORWARD_UPSTREAM                      0x00000313
#define LOG_RTPS_FAIL_SEND_LAST_SN_OF_WRITE                          0x00000314
#define LOG_RTPS_SEND_WINDOW_FULL                                    0x00000315
#define LOG_FAIL_GET_EXT_INTF                                        0x00000316



#include "rtps/rtps_log_impl.h"

#endif /* rtps_log_h */
