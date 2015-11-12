/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef db_log_h
#define db_log_h


#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#define LOG_DB_FAIL_CREATE_TABLE                            0x00000400
#define LOG_DB_FAIL_SELECT_ALL                              0x00000401
#define LOG_DB_FAIL_SELECT_MATCH                            0x00000402
#define LOG_DB_FAIL_SELECT_RANGE                            0x00000403
#define LOG_DB_FAIL_CREATE_RECORD                           0x00000404
#define LOG_DB_FAIL_INSERT_RECORD                           0x00000405
#define LOG_DB_FAIL_REMOVE_RECORD                           0x00000406
#define LOG_DB_FAIL_DELETE_RECORD                           0x00000407
#define LOG_DB_FAIL_CREATE_INDEX                            0x00000408



#include "db/db_log_impl.h"

#endif /* db_log_h */
