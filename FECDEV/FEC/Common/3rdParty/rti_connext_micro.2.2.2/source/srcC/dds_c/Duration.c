/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

/*i @file
 * @ingroup DDSTimeSupportModule
 */

#include "util/util_makeheader.h"
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_common_h
#include "dds_c/dds_c_common.h"
#endif
#ifndef dds_c_infrastructure_h
#include "dds_c/dds_c_infrastructure.h"
#endif

/* ------------------------------------------------------------------
 * Public Constants
 * ------------------------------------------------------------------ */

/* exported in infrastructure.ifc */
const DDS_Long DDS_DURATION_INFINITE_SEC = 0x7fffffff;
const DDS_UnsignedLong DDS_DURATION_INFINITE_NSEC = 0xffffffff;
const struct DDS_Duration_t DDS_DURATION_INFINITE = { 0x7fffffff, 0xffffffff };

/* exported in infrastructure.ifc */
const DDS_Long DDS_DURATION_ZERO_SEC = 0;
const DDS_UnsignedLong DDS_DURATION_ZERO_NSEC = 0;
const struct DDS_Duration_t DDS_DURATION_ZERO = { 0, 0 };


/* ------------------------------------------------------------------
 * Peer Function Prototypes
 * ------------------------------------------------------------------ */

/* TODO: Add NTP time support */
/* #ifdef MICRODDS_NTP_SUPPORT */
void DDS_Duration_to_ntp_time(const struct DDS_Duration_t *self,
                                   struct OSAPI_NtpTime *dst);

void DDS_Duration_from_ntp_time(struct DDS_Duration_t *self,
                                     const struct OSAPI_NtpTime *src);
/* #endif */

/*** SOURCE_BEGIN ***/

/* ------------------------------------------------------------------
 * Peer Functions
 * ------------------------------------------------------------------ */
/* TODO: Add NTP time support */
/* #ifdef MICRODDS_NTP_SUPPORT  */

/*i @ingroup DDSBasicTypesModule
 * @pre Both arguments must be non-NULL.
 */
void
DDS_Duration_to_ntp_time(const struct DDS_Duration_t *self,
                         struct OSAPI_NtpTime *dst)
{
    DDS_Long origSec = 0;
    DDS_Long sec = 0;
    DDS_Long nsec = 0;

    if (DDS_Duration_is_infinite(self))
    {
        dst->sec = OSAPI_NTP_TIME_SEC_MAX;
        dst->frac = OSAPI_NTP_TIME_FRAC_MAX;
    }
    else
    {
        /* OSAPI_NtpTime_packFromNanosec assumes that a.) # seconds and
         * #nanoseconds are both signed and b.) # nanoseconds is less
         * than one billion. We need to make sure of that here.
         */
        origSec = self->sec;
        sec = self->sec + (DDS_Long) (self->nanosec / 1000000000);
        if (sec < origSec || sec > OSAPI_NTP_TIME_SEC_MAX)
        {
            /*overflow! */
            sec = OSAPI_NTP_TIME_SEC_MAX;
        }
        nsec = (DDS_Long) (self->nanosec % 1000000000);
        OSAPI_NtpTime_packFromNanosec(dst, sec, nsec);
    }
}


/*i @ingroup DDSBasicTypesModule
 * 
 * @param   self    Cannot be NULL.
 * @param   src     NULL indicates an infinite period of time.
 */
void
DDS_Duration_from_ntp_time(struct DDS_Duration_t *self,
                           const struct OSAPI_NtpTime *src)
{

    if (OSAPI_NtpTime_isInfinite(src))
    {
        self->sec = DDS_DURATION_INFINITE_SEC;
        self->nanosec = DDS_DURATION_INFINITE_NSEC;
    }
    else
    {
        OSAPI_NtpTime_unpackToNanosec(&self->sec, &self->nanosec, src);
    }
}

/* #endif */

/*i @ingroup DDSBasicTypesModule
   If left > right, then return 1. If left < right, then return -1.
   If left == right, then return 0
*/
int
DDS_Duration_compare(const struct DDS_Duration_t *left,
                     const struct DDS_Duration_t *right)
{
    if (DDS_Duration_is_infinite(left))
    {
        if (DDS_Duration_is_infinite(right))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    if (DDS_Duration_is_infinite(right))
    {
        return -1;
    }

    if (left->sec > right->sec)
    {
        return 1;
    }
    else if (left->sec < right->sec)
    {
        return -1;
    }
    else
    {
        if (left->nanosec > right->nanosec)
        {
            return 1;
        }
        else if (left->nanosec < right->nanosec)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
}

/* End $Id: Duration.c,v 1.11 2007/07/17 20:43:40 elaine Exp $ */
