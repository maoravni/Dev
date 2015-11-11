/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef dds_c_log_h
#define dds_c_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

/*** DDS_C ***/
/** discovery **/
/* DiscoveryLink.c */
#define LOG_FAIL_ASSERT_MATCHED_WRITER                               0x00000007
#define LOG_FAIL_REMOTE_PARTICIPANT_USE_DEFAULT_LOCATORS             0x00000008
#define LOG_ADD_READER_STATE                                         0x00000009 /* 1 int object id, 4 ints remote guid */
#define LOG_UNMATCH_READER_STATE                                     0x0000000a /* 1 int object id */
#define LOG_REMATCH_READER_STATE                                     0x0000000b /* 1 int object id */
#define LOG_FAIL_REMOVE_MATCHED_WRITER                               0x0000000c
#define LOG_DELETE_READER_STATE                                      0x0000000d
#define LOG_FAIL_ASSERT_MATCHED_READER                               0x0000000e
#define LOG_ADD_WRITER_STATE                                         0x0000000f /* 1 int object id, 4 ints remote guid */
#define LOG_UNMATCH_WRITER_STATE                                     0x00000010 /* 1 int object id */
#define LOG_FAIL_REMOVE_MATCHED_READER                               0x00000011
#define LOG_DELETE_WRITER_STATE                                      0x00000012
#define LOG_MATCH_LOCAL_READER_REMOTE_WRITER                         0x00000013
#define LOG_REMOVE_LOCAL_READER_MATCH_REMOTE_WRITER                  0x00000014
#define LOG_MATCH_LOCAL_WRITER_REMOTE_READER                         0x00000015
#define LOG_REMOVE_LOCAL_WRITER_MATCH_REMOTE_READER                  0x00000016
#define LOG_MATCH_REMOTE_READER_LOCAL_WRITERS                        0x00000017
#define LOG_REMOVE_REMOTE_READER_MATCH_LOCAL_WRITERS                 0x00000018
#define LOG_MATCH_REMOTE_WRITER_LOCAL_READERS                        0x00000019
#define LOG_REMOVE_REMOTE_WRITER_MATCH_LOCAL_READERS                 0x0000001a
/* DiscoveryMatch.c */
#define LOG_ATTEMPT_TO_MATCH_NULL_TOPIC                              0x0000001b
#define LOG_ATTEMPT_TO_MATCH_NULL_TYPE                               0x0000001c
#define LOG_TOPICS_SAME_BUT_TYPES_DIFFERENT                          0x0000001d
/* DiscoveryRx.c */
#define LOG_FAIL_ITERATE_READERS                                     0x0000001e /* 1 int db_retcode */
#define LOG_FAIL_LOOKUP_READER                                       0x0000001f /* 1 int db_retcode */
#define LOG_FAIL_LOOKUP_READER_STATE                                 0x00000020 /* 1 int db_retcode */
#define LOG_RECEIVE_THREAD_RUNNING                                   0x00000021 /* 1 int address, 1 short port */
#define LOG_FAIL_RECEIVE                                             0x00000022
#define LOG_MESSAGE_TRUNCATED_INCREASE_MEMORY_BUFFER                 0x00000023 /* 1 int recv_buffer length */
#define LOG_RECEIVE_NETIO_PING                                       0x00000024 /* 1 int address, 1 short port */
#define LOG_FAIL_INTERPRETER_PARSE                                   0x00000025 /* 1 int address, 1 short port */
#define LOG_RECEIVE_THREAD_EXITING                                   0x00000026
#define LOG_FAIL_START_THREAD                                        0x00000027 /* 1 int address, 1 short port */
#define LOG_START_THREAD                                             0x00000028 /* 1 int address, 1 short port */
#define LOG_FAIL_ADD_RECEIVE_PORT                                    0x00000029 /* 1 int retcode */
#define LOG_FAIL_ADD_RECEIVE_PORT_RECORD                             0x0000002a
#define LOG_FAIL_CREATE_THREAD                                       0x0000002b /* 1 int retcode */
#define LOG_FAIL_REMOVE_NONEXISTENT_RECEIVE_PORT                     0x0000002c /* 1 int interface, 1 short port */
/* DiscoveryTx.c */
#define LOG_FAIL_ADD_REMOTE_DESTINATION_DUE_TO_MAX_DESTINATION_PORTS 0x0000002d
/* RemoteEndpoint.c */
#define LOG_UNKNOWN_REMOTE_PARTICIPANT                               0x0000002e
#define LOG_REM_PARTICIPANT_KEY_NOTEQUALS_RECORD_KEY                 0x0000002f /* 1 string participant name */
#define LOG_ENDPOINT_NOT_CHILD_OF_PARTICIPANT                        0x00000030
#define LOG_INVALID_ENDPOINT_GUID                                    0x00000031
/* RemoteParticipant.c */
#define LOG_ASSERT_REMOTE_PARTICIPANT                                0x00000032
#define LOG_FAIL_UPDATE_LIVELINESS_REMOTE_PARTICIPANT                0x00000033
#define LOG_ENABLE_REMOTE_PARTICIPANT                                0x00000034
#define LOG_FAIL_POST_DEADLINE_REMOTE_PARTICIPANT                    0x00000035
#define LOG_REMOVE_REMOTE_PARTICIPANT                                0x00000036
/* RemotePublication.c */
#define LOG_ASSERT_REMOTE_PUBLICATION                                0x00000037
#define LOG_FAIL_INSERT_REMOTE_PUBLICATION_RECORD                    0x00000038
#define LOG_FAIL_LOOKUP_REMOTE_PUBLICATION_RECORD                    0x00000039
#define LOG_ENABLE_REMOTE_PUBLICATION                                0x0000003a
#define LOG_REMOVE_REMOTE_PUBLICATION                                0x0000003b
/* RemoteSubscription.c */
#define LOG_ASSERT_REMOTE_SUBSCRIPTION                               0x0000003c
#define LOG_FAIL_INSERT_REMOTE_SUBSCRIPTION_RECORD                   0x0000003d
#define LOG_FAIL_LOOKUP_REMOTE_SUBSCRIPTION_RECORD                   0x0000003e
#define LOG_ENABLE_REMOTE_SUBSCRIPTION                               0x0000003f
#define LOG_REMOVE_REMOTE_SUBSCRIPTION                               0x00000040
/** domain **/
/* DomainFactory.c */
#define LOG_BAD_FIRST_PARAMETER_IS_NULL                              0x00000041
#define LOG_BAD_SECOND_PARAMETER_IS_NULL                             0x00000042
#define LOG_BAD_THIRD_PARAMETER_IS_NULL                              0x00000043
#define LOG_BAD_STRING_PARAMETER_IS_TOO_LONG                         0x00000044
#define LOG_BAD_THIRD_PARAMETER_HAS_NULL_FIRST_FIELD                 0x00000045
#define LOG_BAD_THIRD_PARAMETER_HAS_NULL_SECOND_FIELD                0x00000046
#define LOG_BAD_THIRD_PARAMETER_EXISTING_VALUE_NOT_EQUAL             0x00000047
#define LOG_BAD_FOURTH_PARAMETER_EXISTING_VALUE_NOT_EQUAL            0x00000048
#define LOG_BAD_SECOND_PARAMETER_EXISTING_VALUE_NOT_EQUAL            0x00000049
#define LOG_FAIL_CREATE_PARTICIPANT_DUE_TO_MAX_PARTICIPANTS          0x0000004a
#define LOG_OUT_OF_OBJECT_IDS                                        0x0000004b
/* DomainParticipant.c */
#define LOG_FAIL_CREATE_DATABASE_TABLE                               0x0000004c
#define LOG_METATRAFFIC_UNICAST_PORT                                 0x0000004d /* 1 int port */
#define LOG_USERTRAFFIC_UNICAST_PORT                                 0x0000004e /* 1 int port */
#define LOG_INCONSISTENT_PARTICIPANT_QOS                             0x0000004f
#define LOG_INCONSISTENT_LISTENER_AND_STATUS_MASK                    0x00000050
#define LOG_NO_DISCOVERY_FACTORY_REGISTERED                          0x00000051
#define LOG_FAIL_CREATE_DISCOVERY_INSTANCE                           0x00000052
#define LOG_FAIL_CREATE_SYSTEM_CLOCK                                 0x00000053
#define LOG_FAIL_RETRIEVE_HOST_INTERFACE                             0x00000054
#define LOG_FOUND_INTERFACE_SETTING_HOSTID                           0x00000055 /* 1 int interface, 1 int host id */
#define LOG_SET_APP_ID                                               0x00000056 /* 1 int app id */
#define LOG_SET_INSTANCE_ID                                          0x00000057 /* 1 int instance id */
#define LOG_FAIL_CREATE_DOMAIN_SENDER                                0x00000058
#define LOG_FAIL_CREATE_DOMAIN_RECEIVER                              0x00000059
#define LOG_FAIL_CREATE_EVENT_GENERATOR                              0x0000005a
#define LOG_FAIL_CREATE_DATABASE                                     0x0000005b /* 1 int db_retcode */
#define LOG_FAIL_CREATE_PARTICIPANT_TABLE                            0x0000005c /* 1 int retcode */
#define LOG_FAIL_CREATE_PUBLISHER_TABLE                              0x0000005d /* 1 int retcode */
#define LOG_FAIL_CREATE_SUBSCRIBER_TABLE                             0x0000005e /* 1 int retcode */
#define LOG_FAIL_CREATE_DATAWRITER_TABLE                             0x0000005f /* 1 int retcode */
#define LOG_FAIL_CREATE_DATAREADER_TABLE                             0x00000060 /* 1 int retcode */
#define LOG_FAIL_CREATE_TOPIC_TABLE                                  0x00000061 /* 1 int retcode */
#define LOG_FAIL_CREATE_TYPE_TABLE                                   0x00000062 /* 1 int retcode */
#define LOG_FAIL_CREATE_STATE_TABLE                                  0x00000063 /* 1 int retcode */
#define LOG_FAIL_CREATE_RTPS_WRITER_SERVICE                          0x00000064
#define LOG_FAIL_CREATE_RTPS_READER_SERVICE                          0x00000065
#define LOG_FAIL_RESERVE_PORT_FOR_ANY_PARTICIPANT_INDEX              0x00000066
#define LOG_FAIL_INIT_BUILTIN_DATA                                   0x00000067
#define LOG_FAIL_INIT_DISCOVERY_PLUGIN                               0x00000068
#define LOG_FAIL_ENABLE_PARTICIPANT                                  0x00000069 /* 1 int retcode */
#define LOG_PARTICIPANT_OWNS_PUBLISHERS                              0x0000006a /* 1 int count */
#define LOG_PARTICIPANT_OWNS_SUBSCRIBERS                             0x0000006b /* 1 int count */
#define LOG_FAIL_DELETE_DATABASE                                     0x0000006c
#define LOG_INITIALIZE_UNICAST_LOCATORS                              0x0000006d /* 1 int port */
#define LOG_ADD_ADDRESS                                              0x0000006e /* 1 int address */
#define LOG_IGNORE_LOOPBACK_LOCATOR                                  0x0000006f
#define LOG_REMOVE_RECEIVE_PORT                                      0x00000070 /* 1 int interface, 1 short port */
#define LOG_FAIL_REMOVE_RECEIVE_PORT                                 0x00000071
#define LOG_FAIL_ADD_RECEIVE_PORT_DUE_TO_MAX_RECEIVE_PORTS           0x00000072 /* 1 int retcode */
#define LOG_ADD_RECEIVE_PORT                                         0x00000073 /* 1 int address, 1 int port */
#define LOG_FAIL_CALCULATE_METATRAFFIC_LOCATORS                      0x00000074
#define LOG_DOMAIN_ATTEMPT_PARTICIPANT_INDEX                         0x00000075 /* 1 int domain id, 1 int participant index */
#define LOG_FAIL_CALCULATE_USER_LOCATORS                             0x00000076
#define LOG_FAIL_RESERVE_PARTICIPANT_INDEX                           0x00000077
#define LOG_DOMAIN_USING_PARTICIPANT_INDEX                           0x00000078 /* 1 int domain id, 1 int participant index */
#define LOG_ENABLE_BUILTIN_PORTS                                     0x00000079
#define LOG_START_METATRAFFIC_THREAD                                 0x0000007a
#define LOG_START_USERTRAFFIC_THREAD                                 0x0000007b
#define LOG_START_MULTICAST_METATRAFFIC_THREAD                       0x0000007c
#define LOG_ENABLE_PARTICIPANT                                       0x0000007d /* 1 int object id */
#define LOG_ATTEMPT_TO_ENABLE_NONEXISTENT_PORTS                      0x0000007e
#define LOG_IMMUTABLE_PARTICIPANT_QOS                                0x0000007f
#define LOG_INCONSISTENT_TOPIC_LISTENER                              0x00000080
#define LOG_INCONSISTENT_PUBLISHER_LISTENER                          0x00000081
#define LOG_INCONSISTENT_SUBSCRIBER_LISTENER                         0x00000082
#define LOG_INCONSISTENT_PUBLISHER_QOS                               0x00000083
#define LOG_INCONSISTENT_SUBSCRIBER_QOS                              0x00000084
#define LOG_INCONSISTENT_TOPIC_QOS                                   0x00000085
#define LOG_BAD_FOURTH_PARAMETER_IS_NULL                             0x00000086
/* DomainParticipantQos.c */
#define LOG_INCONSISTENT_DISCOVERY_QOS                               0x00000087
#define LOG_INCONSISTENT_RESOURCE_LIMITS_QOS                         0x00000088
#define LOG_INCONSISTENT_EVENT_QOS                                   0x00000089
#define LOG_INCONSISTENT_WIRE_PROTOCOL_QOS                           0x0000008a
/* Topic.c */
#define LOG_FAIL_INVALIDATE_TOPIC_RECORD                             0x0000008b
#define LOG_FAIL_UNLOCK_TYPE_RECORD                                  0x0000008c
#define LOG_FAIL_DELETE_TOPIC_RECORD                                 0x0000008d
#define LOG_FAIL_CREATE_TOPIC_RECORD_DUE_TO_LOCAL_TOPIC_ALLOCATION   0x0000008e
#define LOG_FAIL_LOOKUP_TYPE_RECORD                                  0x0000008f /* 1 int db_retcode */
#define LOG_FAIL_INIT_TOPIC                                          0x00000090
#define LOG_FAIL_ENABLE_TOPIC                                        0x00000091 /* 1 int retcode */
#define LOG_FAIL_INSERT_TOPIC_RECORD                                 0x00000092 /* 1 int db_retcode */
#define LOG_FAIL_CREATE_TOPIC                                        0x00000093
/* Type.c */
#define LOG_FAIL_CREATE_TYPE_RECORD_DUE_TO_LOCAL_TYPE_ALLOCATION     0x00000094
#define LOG_FAIL_INSERT_TYPE_RECORD                                  0x00000095 /* 1 int db_retcode, 1 string type name */
#define LOG_GET_SERIALIZED_SAMPLE_MAX_SIZE_CALLBACK_IS_NULL          0x00000096
#define LOG_SERIALIZE_DATA_CALLBACK_IS_NULL                          0x00000097
#define LOG_DESERIALIZE_DATA_CALLBACK_IS_NULL                        0x00000098
#define LOG_CREATE_SAMPLE_CALLBACK_IS_NULL                           0x00000099
#define LOG_COPY_SAMPLE_CALLBACK_IS_NULL                             0x0000009a
#define LOG_PRINT_SAMPLE_CALLBACK_IS_NULL                            0x0000009b
#define LOG_DELETE_SAMPLE_CALLBACK_IS_NULL                           0x0000009c
#define LOG_GET_KEY_KIND_CALLBACK_IS_NULL                            0x0000009d
#define LOG_INSTANCE_TO_KEYHASH_CALLBACK_IS_NULL                     0x0000009e
#define LOG_FAIL_REMOVE_TYPE_RECORD                                  0x0000009f /* 1 int db_retcode */
#define LOG_FAIL_DELETE_TYPE_RECORD                                  0x000000a0 /* 1 int db_retcode */
/** infrastructure **/
/* Entity.c */
#define LOG_UNKNOWN_ENTITY                                           0x000000a1
/* GUID.c */
#define LOG_NOT_IMPLEMENTED                                          0x000000a2
/** publication **/
/* DataWriter.c */
#define LOG_FAIL_GET_MAX_SERIALIZED_LENGTH                           0x000000a3
#define LOG_FAIL_INVALIDATE_DATAWRITER_RECORD                        0x000000a4 /* 1 int db_retcode */
#define LOG_NO_WRITER_HISTORY_FACTORY                                0x000000a5
#define LOG_FAIL_UNLOCK_TOPIC_RECORD                                 0x000000a6 /* 1 int db_retcode */
#define LOG_FAIL_DELETE_DATAWRITER_RECORD                            0x000000a7 /* 1 int db_retcode */
#define LOG_INCONSISTENT_DATAWRITER_QOS                              0x000000a8
#define LOG_FAIL_CREATE_TOPIC_RECORD_DUE_TO_LOCAL_WRITER_ALLOCATION  0x000000a9
#define LOG_FAIL_SET_BYTE_SWAPPING_OF_STREAM                         0x000000aa
#define LOG_FAIL_LOCK_TOPIC_RECORD                                   0x000000ab /* 1 int db_retcode */
#define LOG_TOPIC_HAS_NULL_TYPE                                      0x000000ac
#define LOG_NOT_ALL_TYPE_PLUGIN_CALLBACKS_SET                        0x000000ad
#define LOG_FAIL_CREATE_WRITER_HISTORY                               0x000000ae
#define LOG_FAIL_INSERT_DATAWRITER_RECORD                            0x000000af /* 1 int db_retcode */
#define LOG_FAIL_ADD_DISCOVERY_LOCATORS                              0x000000b0
#define LOG_FAIL_ADD_PORT_FOR_LOCATOR                                0x000000b1 /* 1 int interface, 1 int port */
#define LOG_FAIL_ENABLE_DATAWRITER                                   0x000000b2 /* 1 int db_retcode */
#define LOG_FAIL_CREATE_DATAWRITER                                   0x000000b3
#define LOG_ADD_ANONYMOUS_LOCATOR                                    0x000000b4 /* 1 int address, 1 int port */
#define LOG_FAIL_ADD_ANONYMOUS_LOCATOR                               0x000000b5
#define LOG_FAIL_ADD_DISCOVERY_LOCATOR                               0x000000b6
#define LOG_DATAWRITER_ON_REQUEST_DATA_START                         0x000000b7 /* 2 ints sequence number */
#define LOG_DATAWRITER_ON_REQUEST_DATA_END                           0x000000b8 /* 2 ints sequence number */
#define LOG_FAIL_WH_LOAN_FINISHED                                    0x000000b9
#define LOG_ENABLE_DATAWRITER                                        0x000000ba /* 1 int object id */
#define LOG_FAIL_CREATE_RTPS_WRITER                                  0x000000bb /* 1 int object id */
#define LOG_FAIL_POST_DEADLINE_DATAWRITER                            0x000000bc /* 1 int object id */
#define LOG_FAIL_POST_LIVELINESS_DATAWRITER                          0x000000bd /* 1 int object id */
#define LOG_FAIL_UPDATE_LIVELINESS_DATAWRITER                        0x000000be /* 1 int object id */
#define LOG_WRITE                                                    0x000000bf /* 1 int object id */
#define LOG_FAIL_GET_ENTRY_DUE_TO_RESOURCE_LIMITS                    0x000000c0
#define LOG_OUT_OF_SAMPLES                                           0x000000c1
#define LOG_FAIL_SERIALIZE_DATA                                      0x000000c2
#define LOG_FAIL_SET_MIG_GENERATOR_SAMPLE                            0x000000c3
#define LOG_RTPS_FAIL_WRITER_SEND                                    0x000000c4
#define LOG_RTPS_FAIL_SEND_LIVELINESS_HB                             0x000000c5
#define LOG_INCONSISTENT_DATAWRITER_LISTENER                         0x000000c6
#define LOG_FAIL_UPDATE_DEADLINE_DATAWRITER                          0x000000c7
#define LOG_INCONSISTENT_DEADLINE_QOS                                0x000000c8
#define LOG_INCONSISTENT_LIVELINESS_QOS                              0x000000c9
#define LOG_INCONSISTENT_HISTORY_QOS                                 0x000000ca
#define LOG_INCONSISTENT_DATAWRITER_PROTOCOL_QOS                     0x000000cb
#define LOG_INCONSISTENT_TYPE_SUPPORT_QOS                            0x000000cc
#define LOG_INCONSISTENT_RELIABILITY_QOS                             0x000000cd
#define LOG_INCONSISTENT_OWNERSHIP_QOS                               0x000000ce
#define LOG_INCONSISTENT_OWNERSHIP_STRENGTH_QOS                      0x000000cf
#define LOG_INCONSISTENT_TRANSPORT_QOS                               0x000000d0
#define LOG_INCONSISTENT_HEARTBEATS_PER_MAX_SAMPLES_AND_MAX_SAMPLES  0x000000d1
/* Publisher.c */
#define LOG_PUBLISHER_OWNS_DATAWRITERS                               0x000000d2 /* 1 int count */
#define LOG_FAIL_INVALIDATE_PUBLISHER_RECORD                         0x000000d3 /* 1 int db_retcode */
#define LOG_FAIL_DELETE_PUBLISHER_RECORD                             0x000000d4 /* 1 int db_retcode */
#define LOG_FAIL_CREATE_PUB_RECORD_DUE_TO_LOCAL_PUB_ALLOCATION       0x000000d5
#define LOG_FAIL_ENABLE_PUBLISHER                                    0x000000d6 /* 1 int retcode */
#define LOG_FAIL_INSERT_PUBLISHER_RECORD                             0x000000d7 /* 1 int db_retcode */
#define LOG_ENABLE_PUBLISHER                                         0x000000d8 /* 1 int object id */
#define LOG_FAIL_SELECT_DATAWRITER_RECORD                            0x000000d9

/** subscription **/
/* DataReader.c */
#define LOG_FAIL_INVALIDATE_DATAREADER_RECORD                        0x000000da /* 1 int db_retcode */
#define LOG_NO_READER_HISTORY_FACTORY                                0x000000db
#define LOG_FAIL_DELETE_DATAREADER_RECORD                            0x000000dc /* 1 int db_retcode */
#define LOG_INCONSISTENT_DATAREADER_QOS                              0x000000dd
#define LOG_FAIL_CREATE_TOPIC_RECORD_DUE_TO_LOCAL_READER_ALLOCATION  0x000000de
#define LOG_FAIL_NARROW_TOPIC_DESCRIPTION_TO_TOPIC                   0x000000df
#define LOG_FAIL_CREATE_READER_HISTORY                               0x000000e0
#define LOG_FAIL_ENABLE_DATAREADER                                   0x000000e1 /* 1 int retcode */
#define LOG_FAIL_INSERT_DATAREADER_RECORD                            0x000000e2 /* 1 int db_retcode */
#define LOG_ENABLE_DATAREADER                                        0x000000e3 /* 1 int object id */
#define LOG_FAIL_CREATE_RTPS_READER                                  0x000000e4 /* 1 int object id */
#define LOG_FAIL_POST_DEADLINE_DATAREADER                            0x000000e5 /* 1 int object id */
#define LOG_FAIL_POST_LIVELINESS_DATAREADER                          0x000000e6 /* 1 int object id */
#define LOG_INCONSISTENT_DATAREADER_LISTENER                         0x000000e7
#define LOG_FAIL_COPY_DATA_TO_SAMPLE                                 0x000000e8
/* DataReaderEvent.c */
#define LOG_READER_LIVELINESS_CHECK                                  0x000000e9 /* 1 int object id */
/* DataReaderProtocol.c */
#define LOG_INCORRECT_CDR_ENCAPSULATION                              0x000000ea /* 1 int kind */
#define LOG_DATAREADER_STATE_LIVELINESS_DETECTED                     0x000000eb
#define LOG_DATAREADER_STATE_LIVELINESS_LOST                         0x000000ec
#define LOG_DATAREADER_STATE_COMMIT_DATA                             0x000000ed /* 2 ints sequence number */
#define LOG_READER_NOT_ENABLED                                       0x000000ee
#define LOG_FAIL_COMMIT_SAMPLE                                       0x000000ef
#define LOG_FOUND_KEY_HASH                                           0x000000f0
#define LOG_STATUS_INFO                                              0x000000f1 /* 1 int status info */
#define LOG_PAYLOAD_IS_DATA                                          0x000000f2 /* 2 ints sequence number, 2 ints timestamp */
#define LOG_PAYLOAD_IS_KEY                                           0x000000f3
#define LOG_CHANGED_OWNER                                            0x000000f4
#define LOG_NOT_OWNER                                                0x000000f5
#define LOG_FAIL_UPDATE_DEADLINE_DATAREADER                          0x000000f6 /* 1 int object id */
#define LOG_TIME_FILTERED_OUT_SAMPLE                                 0x000000f7 /* 2 ints sequence number */
#define LOG_FAIL_GET_CDR_SAMPLE                                      0x000000f8
#define LOG_FAIL_DESERIALIZE_DATA                                    0x000000f9
#define LOG_EXPECTED_KEY_NOT_FOUND_COMPUTING_KEY                     0x000000fa
#define LOG_FAIL_COMMIT_ENTRY                                        0x000000fb
/* DataReaderQos.c */
#define LOG_INCONSISTENT_TIME_BASED_FILTER_QOS                       0x000000fc
#define LOG_INCONSISTENT_DATAREADER_PROTOCOL_QOS                     0x000000fd
#define LOG_INCONSISTENT_DEADLINE_AND_TBF_MIN_SEPARATION             0x000000fe
/* Subscriber.c */
#define LOG_SUBSCRIBER_OWNS_DATAREADERS                              0x000000ff
#define LOG_FAIL_INVALIDATE_SUBSCRIBER_RECORD                        0x00000100 /* 1 int db_retcode */
#define LOG_FAIL_DELETE_SUBSCRIBER_RECORD                            0x00000101 /* 1 int db_retcode */
#define LOG_FAIL_CREATE_SUB_RECORD_DUE_TO_LOCAL_SUB_ALLOCATION       0x00000102
#define LOG_FAIL_ENABLE_SUBSCRIBER                                   0x00000103 /* 1 int retcode */
#define LOG_FAIL_INSERT_SUBSCRIBER_RECORD                            0x00000104 /* 1 int db_retcode */
#define LOG_ENABLE_SUBSCRIBER                                        0x00000105 /* 1 int object id */
#define LOG_FAIL_SELECT_DATAREADER_RECORD                            0x00000106

#define LOG_FAIL_SET_LISTENER                                        0x00000107
#define LOG_FAIL_ALLOCATE                                            0x00000108
#define LOG_FAIL_REGISTER_KEY                                        0x00000109
#define LOG_UNKNOWN_REMOTE_PARTICIPANT_BY_KEY                        0x0000010a

#define LOG_INCONSISTENT_DURABILITY_QOS                              0x0000010B

#define LOG_FAIL_INSTANCE_TO_KEYHASH                                 0x0000010c /* DataWriter.c */



#include "dds_c/dds_c_log_impl.h"

#endif /* dds_c_log_h */
