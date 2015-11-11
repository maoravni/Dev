/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef disc_dpde_log_h
#define disc_dpde_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#define LOG_FAIL_SERIALIZE_ENTITY_NAME                               0x00000107
#define LOG_FAIL_SERIALIZE_TOPIC_NAME                                0x00000108
#define LOG_FAIL_SERIALIZE_TYPE_NAME                                 0x00000109
#define LOG_FAIL_SERIALIZE_GUID                                      0x0000010a
#define LOG_FAIL_SERIALIZE_DEFAULT_UNICAST_LOCATOR                   0x0000010b
#define LOG_FAIL_SERIALIZE_PROTOCOL_VERSION                          0x0000010c
#define LOG_FAIL_DESERIALIZE_PROTOCOL_VERSION                        0x0000010d
#define LOG_FAIL_SERIALIZE_VENDOR_ID                                 0x0000010e
#define LOG_FAIL_DESERIALIZE_VENDOR_ID                               0x0000010f
#define LOG_FAIL_SERIALIZE_PRODUCT_VERSION                           0x00000110
#define LOG_FAIL_SERIALIZE_PARTICIPANT_LEASE_DURATION                0x00000111
#define LOG_FAIL_SERIALIZE_BUILTIN_ENDPOINTS                         0x00000112
#define LOG_FAIL_DESERIALIZE_BUILTIN_ENDPOINTS                       0x00000113
 /* 1 int parameter id */
#define LOG_UNRECOGNIZED_PARTICIPANT_ANNOUNCEMENT_PARAMETER_ID       0x00000114
#define LOG_ANNOUNCE_SELF_TO_REMOTE_PARTICIPANT                      0x00000115
#define LOG_FAIL_ANNOUNCE_SELF_TO_REMOTE_PARTICIPANT                 0x00000116
#define LOG_SCHEDULE_FAST_ASSERTIONS                                 0x00000117
#define LOG_FAIL_SCHEDULE_FAST_ASSERTIONS                            0x00000118
#define LOG_FAIL_UPDATE_PARTICIPANT_ASSERTION                        0x00000119
#define LOG_INVALID_ADDRESS_IN_PEER_HOST_LIST                        0x0000011a
 /* 1 int address, 1 int port */
#define LOG_LOCATOR_ADDRESS_ADDED_TO_PROPERTIES                      0x0000011b
#define LOG_LOCAL_PARTICIPANT_HAS_INVALID_BUILTIN_DATA               0x0000011c
#define LOG_FAIL_REGISTER_PARTICIPANT_BUILTIN_TYPE                   0x0000011d
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_TOPIC                  0x0000011e
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_WRITER                 0x0000011f
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_READER                 0x00000120
#define LOG_FAIL_TAKE_PARTICIPANT_DISCOVERY_SAMPLE                   0x00000121
/* 1 int retcode */
#define LOG_FAIL_ASSERT_REMOTE_PARTICIPANT                           0x00000122
 /* 1 int retcode */
#define LOG_FAIL_ENABLE_REMOTE_PARTICIPANT                           0x00000123
#define LOG_FAIL_REFRESH_REMOTE_PARTICIPANT                          0x00000124
#define LOG_INVALID_PARTICIPANT_DISCOVERY_SAMPLE                     0x00000125
#define LOG_INVALID_MULTICAST_RECEIVE_ADDRESS                        0x00000126
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_PUBLISHER              0x00000127
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_SUBSCRIBER             0x00000128
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY                        0x00000129
#define LOG_FAIL_CREATE_PUBLICATION_DISCOVERY                        0x0000012a
#define LOG_FAIL_CREATE_SUBSCRIPTION_DISCOVERY                       0x0000012b
#define LOG_FAIL_INITIALIZE_DISCOVERY_PLUGIN                         0x0000012c
#define LOG_FAIL_DELETE_SUBSCRIPTION_BUILTIN_TOPIC                   0x0000012d
#define LOG_FAIL_DELETE_PUBLICATION_BUILTIN_TOPIC                    0x0000012e
#define LOG_FAIL_DELETE_PARTICIPANT_BUILTIN_TOPIC                    0x0000012f
#define LOG_FAIL_DISPOSE_PARTICIPANT                                 0x00000130
#define LOG_FAIL_CREATE_DPDE_DISCOVERY_PLUGIN                        0x00000131
 /* 1 int parameter id */
#define LOG_UNRECOGNIZED_PUBLICATION_ANNOUNCEMENT_PARAMETER_ID       0x00000132
#define LOG_FAIL_REGISTER_PUBLICATION_BUILTIN_TYPE                   0x00000133
#define LOG_FAIL_CREATE_PUBLICATION_DISCOVERY_TOPIC                  0x00000134
#define LOG_FAIL_CREATE_PUBLICATION_DISCOVERY_WRITER                 0x00000135
#define LOG_FAIL_CREATE_PUBLICATION_DISCOVERY_READER                 0x00000136
#define LOG_FAIL_DISPOSE_PUBLICATION                                 0x00000137
#define LOG_FAIL_ASSERT_REMOTE_PUBLICATION                           0x00000138
#define LOG_FAIL_ASSERT_REMOTE_SUBSCRIPTION                          0x00000139
#define LOG_FAIL_TAKE_PUBLICATION_DISCOVERY_SAMPLE                   0x0000013a
#define LOG_INVALID_PUBLICATION_DISCOVERY_SAMPLE                     0x0000013b
#define LOG_DELETE_NULL_PUBLICATION_BUILTIN_LISTENER                 0x0000013c
#define LOG_UNRECOGNIZED_SUBSCRIPTION_ANNOUNCEMENT_PARAMETER_ID      0x0000013d
#define LOG_FAIL_REGISTER_SUBSCRIPTION_BUILTIN_TYPE                  0x0000013e
#define LOG_FAIL_CREATE_SUBSCRIPTION_DISCOVERY_TOPIC                 0x0000013f
#define LOG_FAIL_CREATE_SUBSCRIPTION_DISCOVERY_WRITER                0x00000140
#define LOG_FAIL_CREATE_SUBSCRIPTION_DISCOVERY_READER                0x00000141
#define LOG_FAIL_DISPOSE_SUBSCRIPTION                                0x00000142
#define LOG_FAIL_TAKE_SUBSCRIPTION_DISCOVERY_SAMPLE                  0x00000143
#define LOG_INVALID_SUBSCRIPTION_DISCOVERY_SAMPLE                    0x00000144
#define LOG_DELETE_NULL_SUBSCRIPTION_BUILTIN_LISTENER                0x00000145



#include "disc_dpde/disc_dpde_log_impl.h"

#endif /* disc_dpde_log_h */
