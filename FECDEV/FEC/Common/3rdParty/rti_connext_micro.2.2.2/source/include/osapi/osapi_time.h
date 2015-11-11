/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_time_h
#define osapi_time_h


#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \file
  \brief Time API
*/

/*e \defgroup OSAPITimeClass OSAPI Time
 *  \ingroup OSAPIModule
*/

/*e \ingroup OSAPITimeClass

    \brief NtpTime API.
*/

/* ================================================================= */
/*                           Definition: Time                        */
/* ----------------------------------------------------------------- */

/*e \ingroup rtiTime

  @brief NTP Time representation.

  Expresses time in NTP format. The second field is simply an integer
  expressing seconds. The fraction field expresses 1/2^32 of a second.
  We strongly urge customers to use our provided macros to convert this
  format to and from human readable form.

  OSAPI_NtpTime_init must be called before OSAPI_NtpTime_get. Creating a domain
  has the side-effect of calling OSAPI_NtpTime_init.

  \b Example:

  The following is a simple example on how to prepare a struct OSAPI_NtpTime structure
  to be 1.5 seconds.

  \verbatim
     struct OSAPI_NtpTime ntpTime;

     OSAPI_NtpTime_packFromMillisec(ntpTime, 1, 500);
  \endverbatim

  @see OSAPI_NtpTime_packFromMillisec OSAPI_NtpTime_packFromMicrosec
  @see OSAPI_NtpTime_packFromNanosec
  @see OSAPI_NtpTime_unpackFromMillisec OSAPI_NtpTime_unpackFromMicrosec
  @see OSAPI_NtpTime_unpackFromNanosec
 */
typedef struct OSAPI_NtpTime
{
    /* e Seconds.* */
    RTI_INT32 sec;
    /* e fraction of a second in 1/2^32 form. */
    RTI_UINT32 frac;
} OSAPI_NtpTime;

/*e \ingroup rtiTime
   The maximum number of seconds that can be represented using NTP time.
 */
#define OSAPI_NTP_TIME_SEC_MAX ((RTI_INT32)0x7fffffff)

/*e \ingroup rtiTime
   The largest possible value of the fraction field in NTP time.
 */
#define OSAPI_NTP_TIME_FRAC_MAX ((RTI_UINT32)0xffffffff)

/*e \ingroup rtiTime
   @brief Set struct OSAPI_NtpTime time to zero.

   @param time (struct OSAPI_NtpTime*) Pointer to struct OSAPI_NtpTime struct that needs to be
   set to zero.
 */
#define OSAPI_NtpTime_setZero(time) \
{ \
    (time)->sec = 0; \
    (time)->frac = 0; \
}

/*e \ingroup rtiTime
   @brief Set struct OSAPI_NtpTime time to maximum value.

   @param time (struct OSAPI_NtpTime*) Pointer to struct OSAPI_NtpTime struct that needs to be
   set to its maximum possible value.

   The maximum possible value equals:

   (time).sec = OSAPI_NTP_TIME_SEC_MAX
   (time).frac = OSAPI_NTP_TIME_FRAC_MAX

 */
#define OSAPI_NtpTime_setMax(time) \
{ \
    (time)->sec = OSAPI_NTP_TIME_SEC_MAX; \
    (time)->frac = OSAPI_NTP_TIME_FRAC_MAX; \
}

#define OSAPI_NtpTime_toDouble(t) \
   (((double)((t)->frac)/4294967296.0f) + (double)(t)->sec)

/*e \ingroup rtiTime

  @brief Macro to convert from seconds and milliseconds to struct OSAPI_NtpTime format.

  @param time (struct OSAPI_NtpTime) contains the answer.
  @param s  (integer) Seconds to convert.
  @param msec (millisecond) Fraction portion (less than 1000).

  \verbatim
    struct OSAPI_NtpTime time;
    long sec, msec;
    sec = 10;
    msec = 577;

    OSAPI_NtpTime_packFromMillisec(time, sec, msec);
  \endverbatim

  After the above call is made, the variable time will contain the equivalent
  timestamp in struct OSAPI_NtpTime representation.

  This macro assumes that msec<1000. It is the caller's responsibility
  to ensure this. This is done for performance reasons (the extra check slows
  the execution by a factor of 5). If msec may be greater than 1000, you can invoke
  the macro as follows:
  \verbatim
    OSAPI_NtpTime_packFromMillisec(time, sec + msec/1000, msec%1000);
  \endverbatim

  This macro only evaluates its arguments once, so it is safe to invoke it
  with an argument that has side effects; that is, if you write:
  \verbatim
    RTI_UINT32 count = 10;
    OSAPI_NtpTime_packFromMillisec(time, sec, count++)
  \endverbatim
  After the macro count is guaranteed to be 11.

  The accuracy of this conversion over the whole range of millisecond values
  is 0.013 milliseconds.

  This is a fairly efficient macro. On a 400MHz Pentium-II Linux box
  the execution time is about 0.06 usec.

  \verbatim
     2^22*time.frac = ms + ms/2^6 + ms/2^7 + ms/2^11 + ms/2^14 + 1.3e-5
                    = ms + ms*393/2^14;
          time.frac = (ms<<22) + (ms<<16) + (ms<<15) + (ms<<11) + (ms<<8)
                    = (ms<<22) + ((ms*393)<<8)
  \endverbatim

  @see struct OSAPI_NtpTime OSAPI_NtpTime_unpackToMillisec
*/
#define OSAPI_NtpTime_packFromMillisec(time, s, msec) \
{ \
    register RTI_UINT32 ms = msec; \
    (time)->sec  = s; \
    (time)->frac = (ms<<22) + ((ms*393)<<8); \
}

/*e \ingroup rtiTime

  @brief Macro to convert from struct OSAPI_NtpTime to seconds and milliseconds.

  @param s (integer) Holds the seconds answer.
  @param msec (integer) Holds the millisecond answer.
  @param time (struct OSAPI_NtpTime) The time to convert to seconds and milliseconds.

  This macro does not check for overflow, so for a near-infinite time value,
  the conversion result may end up being negative. It is the responsibility
  of the user to avoid passing these large time values.

  Use as indicated by the following code:
  \verbatim}
    struct OSAPI_NtpTime time;
    long sec, msec;

    time.sec  = 10;
    time.frac = 577;

    OSAPI_NtpTime_unpackToMillisec(sec, msec, time);
  \endverbatim

  After the above call is made, the variables "sec" and "msec" will
  contain the equivalent timestamp.

  The accuracy of this conversion over the whole range of struct OSAPI_NtpTime values
  is 0.5 milliseconds, except for ntp values above {0x7fffffff, 0x7fffffff},
  accuracy is up to nearly 1 millisecond.

  This is a fairly efficient macro. On a 400MHz Pentium-II Linux box
  the execution time is about 0.08 usec.

  \verbatim
     ms/2^22 = frac*1000/1024 = frac*(1 - 24/1024) = frac*(1 - 3/2^7) =
               frac*(1 - 1/2^6 - 1/2^7)
  \endverbatim

  @see struct OSAPI_NtpTime OSAPI_NtpTime_packFromMillisec
*/
#define OSAPI_NtpTime_unpackToMillisec(s, msec, time) \
{ \
    *(s)    = (time)->sec; \
    *(msec) = ((time)->frac - ((time)->frac>>6) - ((time)->frac>>7) + (1<<21))>>22; \
    if( ((*(msec))>=1000)&&((*(s))!=0x7FFFFFFF) ) { (*(msec)) -= 1000; (*(s))++; } \
}

/*e \ingroup rtiTime

  @brief Macro to convert from seconds and microseconds to struct OSAPI_NtpTime format.

  @param time (struct OSAPI_NtpTime) to contain the answer.
  @param s (integer) seconds of the time to covert.
  @param usec (integer) microseconds fraction to convert from.

  This macro does not check for overflow, so for a near-infinite time value,
  the conversion result may end up being negative. It is the caller's 
  responsibility to avoid passing these large time values.

  Use as indicated by the following code:
  \verbatim
    struct OSAPI_NtpTime ntp;
    long sec, usec;
    sec  = 10;
    usec = 577;

    OSAPI_NtpTime_packFromMicrosec(ntp, sec, usec);
  \endverbatim

  After the above call is made, the variable ntp will contain the equivalent
  time stamp in struct OSAPI_NtpTime representation.

  This macro assumes that msec<1000000. It is the caller's responsibility
  to ensure this. This is done for performance reasons, the extra check
  slows the execution a factor of 5! If msec may be greater than 1000000,
  you can invoke the macro as follows:
  \verbatim
    OSAPI_NtpTime_packFromMicrosec(ntp, sec + usec/1000000, usec%1000000);
  \endverbatim

  This macro only evaluates its arguments once, so it is safe to invoke it
  with an argument that has side effects; that is, if you write:
  \verbatim
    RTI_UINT32 count = 10;
    OSAPI_NtpTime_packFromMicrosec(ntp, sec, count++)
  \endverbatim

  After the macro, count is guaranteed to be 11.

  The accuracy of this conversion over the whole range of microsecond values
  is 0.0028 microseconds.

  This is a fairly efficient macro. On a 400MHz Pentium-II Linux box
  the execution time is about 0.08 usec.

  \verbatim
    2^12 * ntp.frac = us + us/2^5  + us/2^6  + us/2^10 + us/2^11
                         + us/2^13 + us/2^14 + us/2^15 + us/2^16
                         + us/2^18 + us/2^19 + us/2^20 + us/2^21
                         + us/2^23 + 3e-9
                    = us + us*99/2^11 + us*15/2^16 + us*61/2^23
           ntp.frac = (us<<12) + (us<<7) + (us<<6) + (us<<2) + (us<<1)
                               + (us>>1) + (us>>2) + (us>>3) + (us>>4)
                               + (us>>6) + (us>>7) + (us>>8) + (us>>9) + (us>>11)
                    = (us<<12) + ((us*99)<<1)+ ((us*15)>>4)+ ((us*61)>>11)
  \endverbatim
*/
#define OSAPI_NtpTime_packFromMicrosec(time, s, usec) \
{ \
    register RTI_UINT32 us = usec; \
    (time)->sec  = s; \
    (time)->frac = (us<<12)+ ((us*99)<<1)+ ((us*15 + ((us*61)>>7))>>4); \
}

/*e \ingroup rtiTime

  @brief Macro to convert from struct OSAPI_NtpTime to seconds and microseconds.

  @param s (integer) Holds the second portion.
  @param usec (integer) holds the microsecond fraction.
  @param time (struct OSAPI_NtpTime) to be converted.

  This macro does not check for overflow, so for a near-infinite time value,
  the conversion result may end up being negative. It is the caller's 
  responsibility to avoid passing these large time values.

  Use as indicated by the following code:
  \verbatim
    struct OSAPI_NtpTime ntp;
    long sec, usec;

    ntp.sec  = 10;
    ntp.frac = 577;

    OSAPI_NtpTime_unpackToMicrosec(sec, usec, ntp);
  \endverbatim

  After the above call is made, the variables "sec" and "usec" will
  contain the equivalent timestamp.

  The accuracy of this conversion over the whole range of struct OSAPI_NtpTime values
  is 0.5 microseconds, except for ntp value above {0x7fffffff, 0x7fffffff},
  accuracy is up to nearly 1 millisecond.

  This is a fairly efficient macro. On a 400MHz Pentium-II Linux box
  the execution time is about 0.12 usec.

  \verbatim
     us = frac*1000000/2^20 = frac*( 1 - 48576/2^20 )
        = frac*(1 - 47/2^10 - 7/2^14)
        = frac - ((47*frac)>>10) - ((7*frac)>>14)
        = frac - (frac>>5)-(frac>>7)-(frac>>8)-(frac>>9)-(frac>>10)
             - (frac>>12)-(frac>>13)-(frac>>14)
  \endverbatim
*/
#define OSAPI_NtpTime_unpackToMicrosec(s, usec, time) \
{ \
    register RTI_UINT32 rtiNtpTemp = (time)->frac; \
    *(s)    = (time)->sec; \
    *(usec) = ((time)->frac - (rtiNtpTemp>>5)-(rtiNtpTemp>>7)-(rtiNtpTemp>>8)- \
            (rtiNtpTemp>>9)-(rtiNtpTemp>>10) - (rtiNtpTemp>>12) - \
            (rtiNtpTemp>>13)-(rtiNtpTemp>>14) + (1<<11)) >> 12; \
    if( ((*(usec)) >= 1000000) && ((*(s))!=0x7FFFFFFF) ) { \
        (*(usec)) -= 1000000; \
        (*(s))++; \
    } \
}

/*e \ingroup rtiTime

  @brief Macro to convert from seconds and nanoseconds to struct OSAPI_NtpTime format.

  @param time (struct OSAPI_NtpTime) Holds the answer.
  @param s (integer) Seconds to convert from.
  @param nsec (integer) Nanosecond fraction to convert from.

  Use as indicated by the following code:
  \verbatim
    struct OSAPI_NtpTime time;
    long sec, nsec;
    sec = 10;
    nsec = 577;

    OSAPI_NtpTime_packFromNanosec(time, sec, msec);
  \endverbatim

  After the above call is made, the variable time will contain the equivalent
  timestamp in struct OSAPI_NtpTime representation.

  This macro assumes that nsec<1000000000. It is the caller's responsibility
  to ensure this. This is done for performance reasons (the extra check
  slows the execution a factor of 5). If msec may be greater than 1000000, you
  can invoke the macro as follows:
  \verbatim}
    OSAPI_NtpTime_packFromNanosec(time, sec + nsec/1000000000, nsec%1000000000);
  \endverbatim

  This macro only evaluates its arguments once, so it is safe to invoke it
  with an argument that has side effects; that is, if you write:
  \verbatim
    RTI_UINT32 count = 10;
    OSAPI_NtpTime_packFromNanosec(time, sec, count++)
  \endverbatim
  After the macro, count is guaranteed to be 11.

  The accuracy of this conversion over the whole range of nanosecond values
  is 0.5 nanoseconds, except for ntp value above {0x7fffffff, 0x7fffffff},
  accuracy is up to nearly 1 millisecond.

  This is a fairly efficient macro. On a 400MHz Pentium-II Linux box 
  the execution time is about 0.10 usec.

  \verbatim
     2^32/10^9 ns = 4*ns + ns/2^2  + ns/2^5  + ns/2^7 + ns/2^8  + ns/2^9   +
                           ns/2^15 + ns/2^17 + ns/2^18 + ns/2^19 + ns/2^20 +
                           ns/2^21 + ns/2^23 + ns/2^29 + ns/2^32

     2^32/10^9 ns = 4*ns + (ns     + ns/2^3  + ns/2^5 + ns/2^6  + ns/2^7   +
                           ns/2^13 + ns/2^15 + ns/2^16 + ns/2^17 + ns/2^18 +
                           ns/2^19 + ns/2^21 + ns/2^27 + ns/2^30)*(1/2^2)
  \endverbatim
*/
#define OSAPI_NtpTime_packFromNanosec(time, s, nsec) \
{ \
    register RTI_UINT32 ns = nsec; \
    (time)->sec  = s; \
    (time)->frac = (ns<<2)  + ((ns  + (ns>>3)  + (ns>>5)  + (ns>>6)  + \
                   (ns>>7)  + (ns>>13) + (ns>>15) + (ns>>16) + (ns>>17) + \
                   (ns>>18) + (ns>>19) + (ns>>21) + (ns>>27) + (ns>>30))>>2);  \
}

/*e \ingroup rtiTime

  @brief Macro to convert from struct OSAPI_NtpTime to seconds and nanoseconds.

  @param s (integer) Second of the time to covert.
  @param nsec (integer) Nanosecond fraction to convert.
  @param time (struct OSAPI_NtpTime) to convert from.

  Use as indicated by the following code:
  \verbatim
    struct OSAPI_NtpTime time;
    long sec, nsec;
    time.sec  = 10;
    time.frac = 577;

    OSAPI_NtpTime_unpackToNanosec(sec, nsec, time);
  \endverbatim
  After the above call is made, the variables "sec" and "nsec" will
  contain the equivalent timestamp.

  The accuracy of this conversion over the whole range of struct OSAPI_NtpTime values
  is 0.5 nsec. 
  
  This is a fairly efficient macro. On a 400MHz Pentium-II Linux
  box the execution time is about 0.09 usec.

  Given that 294967296 = 0x1194d800 and:
  \verbatim
      4*ns = frac*4000000000/2^32 = frac*(1 - 294967296/2^32)
           = frac*(1 - 1/2^4 - 1/2^8 - 9/2^12 - 4/2^16 - 13/2^20 - 8/2^24)
           = frac*(1 - 1>>4 - 1>>8 - 1>>9 - 1>>12 - 1>>14 - 1>>17 - 1>>18 -
               1>>20 - 1>>21)
  \endverbatim
*/
#define OSAPI_NtpTime_unpackToNanosec(s, nsec, time) \
{ \
    register RTI_UINT32 rtiNtpTemp = (time)->frac>>1; \
    register RTI_UINT32 rtiNtpDelta; \
    *(s)     = (time)->sec; \
    rtiNtpDelta = (rtiNtpTemp) + (rtiNtpTemp>>4) + (rtiNtpTemp>>5) + \
            (rtiNtpTemp>>8) + (rtiNtpTemp>>10) + (rtiNtpTemp>>13) + \
            (rtiNtpTemp>>14)+ (rtiNtpTemp>>16) + (rtiNtpTemp>>17); \
    *(nsec) = ((time)->frac - ((rtiNtpDelta+4)>>3) + 1) >> 2; \
    if( ((*(nsec))>=1000000000) && ((*(s))!=0x7FFFFFFF) ) { \
        (*(nsec)) -= 1000000000; \
        (*(s))++; \
    } \
}


/*e \ingroup rtiTime

  @brief Precision used in bit manipulation for struct OSAPI_NtpTime fraction
         conversions.

  The larger the precision, the more bits you pick up in the long
  division to get the fraction. If the precision is too large, so that the
  denominator shifted by the precision bits exceeds the 2^32 limit, the
  result will be incorrect. For example, if the denominator is 60 (such as for 
  the VxWorks sysClkRate), precision_bits larger than 27 exceed the limit and
  produce an incorrect result. In general:

  maximum precision_bits = 32 - (int)log2(denominator_per_sec),

  where log2(x) can be calculated by:

  \verbatim
    int log2answer = 0;
    while (x >>= 1) {1
        log2answer++;
    }
   \endverbatim

   For 60 (sysClkRateGet()), this would be 27.
   For gettimeofday() with microsec resolution, this would be 12.

   But in practice, that kind of precision is not necessary nor desirable
   since the conversion would be slower. Instead, the precision is
   only important relative to the denominator. If your clock frequency
   is low (e.g., 60 Hz), you only have 16.6 ms resolution anyway. If you are
   within a tenth of your clock resolution, that is probably good enough.

   If we generalized too much and tried to handle nanosec resolution "ticks,"
   we could not guarantee adequate accuracy for all ranges. So internally, we
   selected a reasonable number that gives us adequate accuracy over large
   ranges. We wanted a precision that will work well up to 1e6 tick rate
   (thinking about UNIX gettimeofday()); 7 passed the test.  
*/
#define OSAPI_NTP_TIME_PRECISION_BITS_DEFAULT 7

/*e \ingroup rtiTime

  @brief Macro to convert numerator/denom_per_sec into NTP time.

  @param time struct OSAPI_NtpTime to contain the result. The existing
  content of time does not affect the result.

  @param numerator May be larger than denominator. Since this is a macro
  and the numerator is evaluated multiple times, you should not specify things
  like (i++).

  @param denominator_per_sec Denominator.
  Physically, this means ticks per sec. Since this is a macro and
  the denominator is evaluated multiple times, you should not specify things
  like (i++).
*/
#define OSAPI_NtpTime_packFromFraction(time, numerator, denominator_per_sec) \
    OSAPI_NtpTime_packFromFractionBits((time), (numerator), (denominator_per_sec), \
                                OSAPI_NTP_TIME_PRECISION_BITS_DEFAULT)

/*e \ingroup rtiTime

  @brief Transforms from struct OSAPI_NtpTime to any integral fraction of a 
		second without using floating point operations.

  @param numerator RTI_UINT32 where to put the result of the conversion.

  @param denominator_per_sec The number of units of the fraction contained in
  a second. For example, this should be 1000 to convert to milliseconds, or 
  60 if you are using the default VxWorks clock rate.

  @param time struct OSAPI_NtpTime to unpack

  The algoritm is based on the observation that the time.frac looked at
  as a sequence of bits: b[1], b[2], b[3], ... represents the
  following expansion:

   time_in_sec = b[1]*1/2 + b[1]*1/4 +...+ b[2]*1/8
               = Sum{i=1..32} b[i]*1/2^i

  Since we want to obtain the time in units of which there are 
  denominator_per_sec units in a second, the arithmetic is:

   time_in_units = denominator_per_sec*time_in_sec
                 = denominator_per_sec*time.sec+denominator_per_sec*time.frac

  And denominator_per_sec*time.frac =
       Sum{i=1..32} b[i]*(denominator_per_sec/2^i)

  The key here is that b[i] is easily computed as (1<<31)>>(i-1)
  and (denominator_per_sec/2^i) is easily computed as denominator_per_sec>>i.

  Note that each time we compute (denominator_per_sec/2^i) to add it, we may
  be off by one unit. Therefore, at the end of the computation we may
  be off by log2(denominator_per_sec) units, which means we should in fact
  perform each step with an accuracy of log2(2*denominator_per_sec)
  greater. In other words, we should use units a factor
  log2(2*denominator_per_sec) greater than the ones we require at the end
  of the conversion.

  Another key observation is that the computation can end as soon
  as (denominator_per_sec<<OSAPI_NTP_TIME_PRECISION_BITS_DEFAULT)>>i becomes
  zero because that means what is left has a significance of
  less than one unit>>OSAPI_NTP_TIME_PRECISION_BITS_DEFAULT. This will give a
  conversion that is strictly smaller than the NTP timestamp. For example, 
  for millisecond conversion log2(1000) = 10, we will do 10 operations so we need a
  precision of 1/20 or 5 bits (4 bits gives 1/16, 5 gives 1/32).

  The final operation:
      (precision_units + (1<<OSAPI_NTP_TIME_PRECISION_BITS_DEFAULT-1))>>
       OSAPI_NTP_TIME_PRECISION_BITS_DEFAULT;
  makes sure that the integer division by
  2^(OSAPI_NTP_TIME_PRECISION_BITS_DEFAULT) is
  rounded off to the closest number.
*/
#define OSAPI_NtpTime_unpackToFraction(numerator, denominator_per_sec, time) \
    OSAPI_NtpTime_unpackToFractionBits((numerator), (denominator_per_sec), (time), \
                                OSAPI_NTP_TIME_PRECISION_BITS_DEFAULT)

#define OSAPI_NtpTime_packFromFractionBits(time, numerator, denominator_per_sec, \
                                    precisionBits) \
{ \
    register RTI_UINT32 current_bit = 0x80000000; \
    register RTI_UINT32 current_units = (denominator_per_sec)<<precisionBits; \
    register RTI_UINT32 precision_numerator = \
        ((numerator)%(denominator_per_sec))<< precisionBits; \
    (time)->sec  = (numerator)/(denominator_per_sec); \
    (time)->frac = 0; \
    while (current_units >>= 1) { \
        (time)->frac += \
            (((precision_numerator) >= current_units) ? \
             (((precision_numerator) -= current_units), current_bit) : 0); \
        current_bit >>= 1; \
    } \
}

#define OSAPI_NtpTime_unpackToFractionBits(numerator, denominator_per_sec, time, \
                                    precisionBits) \
{ \
    register RTI_UINT32 current_bit = 0x80000000; \
    register RTI_UINT32 current_units = (*(denominator_per_sec))<<(precisionBits);\
    register RTI_UINT32 precision_numerator = 0; \
    numerator = (*(denominator_per_sec))*(time)->sec; \
    while ((current_units >>= 1)) { \
        precision_numerator += ((time)->frac & current_bit) ? current_units:0;\
        current_bit >>= 1; \
    } \
    *numerator += (precision_numerator + \
                  (1<<((precisionBits)-1)))>>(precisionBits);\
}

/*e \ingroup rtiTime

  @brief Compare two OSAPI_NtpTimes.

  @param time1 struct OSAPI_NtpTime, NOT NULL.
  @param time2 struct OSAPI_NtpTime, NOT NULL.

  @return If time1 > time2, return 1. If time1 < time2, return -1.
  If time1 == time2, return 0.
*/
#define OSAPI_NtpTime_compare(time1, time2) \
((((time1)->sec) > ((time2)->sec)) ? 1 : \
 ((((time1)->sec) < ((time2)->sec)) ? -1 : \
  ((((time1)->frac) > ((time2)->frac)) ? 1 : \
   ((((time1)->frac) < ((time2)->frac)) ? -1 : 0))))

/*e \ingroup rtiTime

  A NULL struct OSAPI_NtpTime pointer is considered infinity. This is consistent
  with the concept of infinite time on UNIX systems.

  In addition, if the seconds field equals OSAPI_NTP_TIME_SEC_MAX, the
  time value is also considered infinite.

  @param time Pointer to RTITime.
  @return RTI_TRUE if time is infinite, RTI_FALSE otherwise.

*/
#define OSAPI_NtpTime_isInfinite(time) ((!(time) || \
                                      ((time)->sec == OSAPI_NTP_TIME_SEC_MAX))? \
                                      RTI_TRUE : RTI_FALSE)

/*e \ingroup rtiTime

  @brief answer = t1 + t2.

  This macro does not check for overflow.

  @param answer struct OSAPI_NtpTime
  @param t1 struct OSAPI_NtpTime
  @param t2 struct OSAPI_NtpTime
*/
#define OSAPI_NtpTime_add(answer, t1, t2) \
{\
    (answer)->sec =  (t1)->sec + (t2)->sec; \
    (answer)->frac = (t1)->frac + (t2)->frac; \
    if (((answer)->frac < (t1)->frac) || ((answer)->frac < (t2)->frac)) { \
        (answer)->sec++; \
    } \
}

/*e \ingroup rtiTime

  @brief answer = t1 - t2.

  @param answer struct OSAPI_NtpTime
  @param t1 struct OSAPI_NtpTime`
  @param t2 struct OSAPI_NtpTime
 */
/* TODO: this can be removed... is checking t2 inifinite really necessary? */
#define OSAPI_NtpTime_subtract(answer, t1, t2) \
{\
   if (OSAPI_NtpTime_isInfinite(t1)) { \
    *(answer) = *(t1); \
   } else if (OSAPI_NtpTime_isInfinite(t2)) { \
    (answer)->sec = 0; \
    (answer)->frac = 0; \
   } else { \
    (answer)->sec  = (t1)->sec - (t2)->sec; \
    (answer)->frac = (t1)->frac - (t2)->frac; \
    if ((answer)->frac > (t1)->frac) { \
        (answer)->sec--; \
     } \
   } \
}

/*e \ingroup rtiTime

  @brief answer = time << shift.

  To use this macro for multiplying by num, shift can be calculated
  as log10(num)/log10(2).

  @param answer struct OSAPI_NtpTime
  @param time   struct OSAPI_NtpTime
  @param shift  int
*/
#define OSAPI_NtpTime_leftShift(answer, time, shift) \
{\
  register RTI_UINT32 mask    = 0xffffffff; \
      if (shift) { \
       (answer)->sec = ((time)->sec)<<shift; \
       mask = mask<<(32 - shift); \
       mask &= (time)->frac; \
       mask = mask>>(32 - shift); \
       (answer)->sec |= mask; \
       (answer)->frac = ((time)->frac)<<shift; \
    } \
     else { \
       answer = time; \
   } \
}

/*e \ingroup rtiTime

  @brief answer = time >> shift.

  To use this macro for dividing by num, shift can be calculated
  as log10(num)/log10(2).

  @param answer struct OSAPI_NtpTime
  @param time   struct OSAPI_NtpTime
  @param shift  int
*/
#define OSAPI_NtpTime_rightShift(answer, time, shift) \
{\
   register RTI_UINT32 mask    = 0xffffffff; \
    if (shift) { \
      (answer)->frac = ((time)->frac)>>shift; \
      mask = mask>>(32 - shift); \
      mask &= (time)->sec; \
      mask = mask<<(32 - shift); \
      (answer)->frac |= mask; \
      (answer)->sec = ((time)->sec)>>shift; \
   } \
   else { \
       answer = time; \
   } \
}

/*e \ingroup rtiTime

  @brief answer += time.

  This macro does not check for overflow.

  @param answer struct OSAPI_NtpTime
  @param time struct OSAPI_NtpTime
*/
#define OSAPI_NtpTime_increment(answer, time) \
{ \
    register RTI_UINT32 currentFrac = (answer)->frac; \
    (answer)->sec  += (time)->sec; \
    (answer)->frac += (time)->frac; \
    if (((answer)->frac < (time)->frac) || ((answer)->frac < currentFrac)) { \
        (answer)->sec++; \
    } \
}

/*e \ingroup rtiTime

  @brief Decrement one struct OSAPI_NtpTime value by another struct OSAPI_NtpTime value.

  Precondition:
  Postcondition: answer -= time.

  @param answer struct OSAPI_NtpTime
  @param time struct OSAPI_NtpTime

*/
#define OSAPI_NtpTime_decrement(answer, time) \
{ \
    register RTI_UINT32 currentFrac = (answer)->frac; \
    (answer)->sec  -= (time)->sec; \
    (answer)->frac -= (time)->frac; \
    if (((answer)->frac > currentFrac)) { (answer)->sec--; } \
}

/*e \ingroup rtiTime

  @brief time = time + 1 fraction (~200 picoseconds).

  This macro does not check for overflow, so time cannot be infinite.

  @param time
  @return NO RETURN VALUE => do NOT evaluate this expression; simply call it.
*/
#define OSAPI_NtpTime_incrementInfinitesimally(time) \
    ((++((time)->frac)) == 0) ? ++(time)->sec : 0

/*e \ingroup rtiTime

  Checks if the time structure represents zero time. Zero time may be useful
  when getting the sign of the time quantity.
  @param time Pointer to struct OSAPI_NtpTime. Must not be NULL.
  @return RTI_TRUE if time is zero, RTI_FALSE otherwise.
*/
#define OSAPI_NtpTime_isZero(time) (((time) && (time)->sec==0 && \
                                           (time)->frac==0) ? \
                                           RTI_TRUE : RTI_FALSE)

/*e \ingroup rtiTime

  @brief Zero time.

  This global variable is for convenience. It allows you to see if a 
  RTITime variable is negative or positive by comparing against this.

  @see RtiTimeCompare
*/
#define OSAPI_NTP_TIME_ZERO {0,0}

/*e \ingroup rtiTime

  Represents the maximum timevalue that can be represented using the
  NTP time format. For all practical purposes, it can be considered
  equivalent to infinity.

*/
#define OSAPI_NTP_TIME_MAX {OSAPI_NTP_TIME_SEC_MAX,OSAPI_NTP_TIME_FRAC_MAX}

/*e \ingroup rtiTime
   The number of nanoseconds per second. 1e9.
   @see RtiTimePack RtiTimeUnpack
*/
#define OSAPI_NTP_TIME_NSEC_PER_SEC  (1000000000)
/*e \ingroup rtiTime
   The number of microseconds per second. 1e6.
   @see RtiTimePack RtiTimeUnpack
*/
#define OSAPI_NTP_TIME_USEC_PER_SEC  (1000000)
/*e \ingroup rtiTime
   The number of milliseconds per second. 1e3.
   @see RtiTimePack RtiTimeUnpack
 */
#define OSAPI_NTP_TIME_MSEC_PER_SEC  (1000)
/*e \ingroup rtiTime
  The number of microseconds per milliseconds. 1e3.
   @see RtiTimePack RtiTimeUnpack
 */
#define OSAPI_NTP_TIME_NSEC_PER_USEC (1000)
/*e \ingroup rtiTime
   The number of microseconds per milliseconds. 1e3.
   @see RtiTimePack RtiTimeUnpack
*/
#define OSAPI_NTP_TIME_USEC_PER_MSEC (1000)
/*e \ingroup rtiTime
   The number of seconds per second. 1.
   @see RtiTimePack RtiTimeUnpack
 */
#define OSAPI_NTP_TIME_SEC_PER_SEC   (1)

/*e \ingroup rtiTime
   The number of nano seconds per milli second. 1e6.
   @see RtiTimePack RtiTimeUnpack
 */
#define OSAPI_NTP_TIME_NSEC_PER_MSEC   (1000000)

#ifdef __cplusplus
}                  /* extern "C" */
#endif

#endif /* osapi_time_h */
