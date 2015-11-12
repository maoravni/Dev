/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef mig_rtps_h
#include "rtps/rtps_rtps.h"
#endif

#ifdef USE_OLD_IDS

const char *MIG_RTPS_SUBMESSAGE_ID_NAME[MIG_RTPS_SUBMESSAGE_ID_COUNT_MAX] = {
    "INVALID",                  /* 0x00 */
    "PAD",                      /* 0x01 */
    "DATA",                     /* 0x02 */
    "NOKEY_DATA",               /* 0x03 */
    "UNKNOWN",                  /* 0x04 */
    "UNKNOWN",                  /* 0x05 */
    "ACKNACK",                  /* 0x06 */
    "HEARTBEAT",                /* 0x07 */
    "GAP",                      /* 0x08 */
    "INFO_TS",                  /* 0x09 */
    "UNKNOWN",                  /* 0x0a */
    "UNKNOWN",                  /* 0x0b */
    "INFO_SRC",                 /* 0x0c */
    "INFO_REPLY_IP4",           /* 0x0d */
    "INFO_DST",                 /* 0x0e */
    "INFO_REPLY",               /* 0x0f */
    "DATA_FRAG",                /* 0x10 */
    "NOKEY_DATA_FRAG",          /* 0x11 */
    "NACK_FRAG",                /* 0x12 */
    "HEARTBEAT_FRAG"            /* 0x13 */
};

#else

const char *MIG_RTPS_SUBMESSAGE_ID_NAME[MIG_RTPS_SUBMESSAGE_ID_COUNT_MAX] = {
    "INVALID",                  /* 0x00 */
    "PAD",                      /* 0x01 */
    "VAR",                      /* 0x02 */
    "ISSUE",                    /* 0x03 */
    "UNKNOWN",                  /* 0x04 */
    "UNKNOWN",                  /* 0x05 */
    "ACK",                      /* 0x06 */
    "HEARTBEAT",                /* 0x07 */
    "GAP",                      /* 0x08 */
    "INFO_TS",                  /* 0x09 */
    "UNKNOWN",                  /* 0x0a */
    "UNKNOWN",                  /* 0x0b */
    "INFO_SRC",                 /* 0x0c */
    "INFO_REPLY_IP4",           /* 0x0d */
    "INFO_DST",                 /* 0x0e */
    "INFO_REPLY",               /* 0x0f */
    "DATA_FRAG",                /* 0x10 */
    "NOKEY_DATA_FRAG",          /* 0x11 */
    "NACK_FRAG",                /* 0x12 */
    "HEARTBEAT_FRAG",           /* 0x13 */
    "UNKNOWN",                  /* 0X14 */
    "DATA",                     /* 0X15 */
    "DATA_FRAG_X",              /* 0X16 */
    "ACK_BATCH",                /* 0X17 */
    "DATA_BATCH",               /* 0X18 */
    "HEARTBEAT_BATCH"           /* 0X19 */
};

#endif
