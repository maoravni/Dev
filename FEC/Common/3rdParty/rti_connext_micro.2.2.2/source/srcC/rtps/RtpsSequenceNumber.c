/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#include "osapi/osapi_types.h"
#include "rtps/rtps_rtps.h"

/*** SOURCE_BEGIN ***/

int
MIGRtpsSequenceNumber_getDistance(const struct REDA_SequenceNumber *s1,
                                  const struct REDA_SequenceNumber *s2)
{
    struct REDA_SequenceNumber s;
    /*  MIGLOG_preconditionOnly(
     * LOG_methodNameDeclaration("MIGRtpsSequenceNumber_getDistance"))
     * MIGLOG_testPrecondition(s1 == NULL || s2 == NULL,
     * return -1); */

    if (REDA_SequenceNumber_compare(s1, s2) >= 0)
    {
        REDA_SequenceNumber_subtract(&s, s1, s2);
    }
    else
    {
        REDA_SequenceNumber_subtract(&s, s2, s1);
    }
    return (s.high > 0) ||
        (s.low > MIG_RTPS_BITMAP_DISTANCE_MAX) ?
        MIG_RTPS_BITMAP_DISTANCE_MAX : s.low;
}
