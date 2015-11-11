/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef disc_dpse_log_h
#define disc_dpse_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#define LOG_FAIL_SERIALIZE_METATRAFFIC_UNICAST_LOCATOR               0x00000146
#define LOG_FAIL_SERIALIZE_METATRAFFIC_MULTICAST_LOCATOR             0x00000147
#define LOG_FAIL_SERIALIZE_PARTICIPANT_NAME                          0x00000148
#define LOG_FAIL_DESERIALIZE_GUID                                    0x00000149
#define LOG_EXCEED_MAX_LOCATORS                                      0x0000014a
#define LOG_FAIL_DESERIALIZE_ENTITY_NAME                             0x0000014b
#define LOG_DISCOVER_MORE_THAN_MAX_REMOTE_PARTICIPANTS               0x0000014c
#define LOG_DELETE_NULL_PARTICIPANT_BUILTIN_LISTENER                 0x0000014d
#define LOG_BAD_THIRD_PARAMETER_HAS_NULL_THIRD_FIELD                 0x0000014e
#define LOG_BAD_THIRD_PARAMETER_HAS_NULL_FOURTH_FIELD                0x0000014f
#define LOG_FAIL_ANNOUNCE_LOCAL_PARTICIPANT_DELETION                 0x00000150
#define LOG_FAIL_DELETE_PARTICIPANT_DISCOVERY_DATA_WRITER            0x00000151
#define LOG_FAIL_DELETE_PARTICIPANT_DISCOVERY_PUBLISHER              0x00000152
#define LOG_FAIL_DELETE_PARTICIPANT_DISCOVERY_DATA_READER            0x00000153
#define LOG_FAIL_DELETE_PARTICIPANT_DISCOVERY_SUBSCRIBER             0x00000154
#define LOG_FAIL_CREATE_DPSE_DISCOVERY_PLUGIN                        0x00000155
#define LOG_FAIL_SERIALIZE_GUID                                      0x00000156
#define LOG_FAIL_SERIALIZE_BUILTIN_ENDPOINTS                         0x00000157
#define LOG_FAIL_SERIALIZE_PROTOCOL_VERSION                          0x00000158
#define LOG_FAIL_SERIALIZE_VENDOR_ID                                 0x00000159
#define LOG_FAIL_SERIALIZE_DEFAULT_UNICAST_LOCATOR                   0x0000015a
#define LOG_FAIL_SERIALIZE_PARTICIPANT_LEASE_DURATION                0x0000015b
#define LOG_FAIL_SERIALIZE_PRODUCT_VERSION                           0x0000015c
#define LOG_FAIL_DESERIALIZE_BUILTIN_ENDPOINTS                       0x0000015d
#define LOG_FAIL_DESERIALIZE_PROTOCOL_VERSION                        0x0000015e
#define LOG_FAIL_DESERIALIZE_VENDOR_ID                               0x0000015f
#define LOG_FAIL_ANNOUNCE_SELF_TO_REMOTE_PARTICIPANT                 0x00000160
#define LOG_FAIL_UPDATE_PARTICIPANT_ASSERTION                        0x00000161
#define LOG_FAIL_SCHEDULE_FAST_ASSERTIONS                            0x00000162
#define LOG_FAIL_TAKE_PARTICIPANT_DISCOVERY_SAMPLE                   0x00000163
#define LOG_FAIL_ASSERT_REMOTE_PARTICIPANT                           0x00000164
#define LOG_FAIL_REFRESH_REMOTE_PARTICIPANT                          0x00000165
#define LOG_INVALID_PARTICIPANT_DISCOVERY_SAMPLE                     0x00000166
#define LOG_LOCATOR_ADDRESS_ADDED_TO_PROPERTIES                      0x00000167
#define LOG_LOCAL_PARTICIPANT_HAS_INVALID_BUILTIN_DATA               0x00000168
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_PUBLISHER              0x00000169
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_SUBSCRIBER             0x0000016a
#define LOG_FAIL_REGISTER_PARTICIPANT_BUILTIN_TYPE                   0x0000016b
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_TOPIC                  0x0000016c
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_WRITER                 0x0000016d
#define LOG_FAIL_CREATE_PARTICIPANT_DISCOVERY_READER                 0x0000016e
#define LOG_FAIL_INITIALIZE_DISCOVERY_PLUGIN                         0x0000016f
#define LOG_FAIL_DISPOSE_PARTICIPANT                                 0x00000170


#include "disc_dpse/disc_dpse_log_impl.h"

#endif /* disc_dpse_log_h */
