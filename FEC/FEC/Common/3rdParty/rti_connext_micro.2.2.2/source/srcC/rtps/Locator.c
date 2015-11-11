/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
/*i @file
 * @ingroup DDSDomainModule
 */

#ifndef util_makeheader_h
#include "util/util_makeheader.h"
#endif
#ifndef mig_rtps_h
#include "rtps/rtps_rtps.h"
#endif
#if 0
#ifndef dds_c_infrastructure_h
  #include "dds_c/dds_c_infrastructure.h"
#endif
#endif
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif

const RTI_INT32 RTPS_LOCATOR_KIND_INVALID = -1;
const RTI_UINT32 RTPS_LOCATOR_PORT_INVALID = 0;
const unsigned char
    RTPS_LOCATOR_ADDRESS_INVALID[RTPS_LOCATOR_ADDRESS_LENGTH_MAX] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const struct RTPS_Locator_t RTPS_LOCATOR_INVALID = {
    -1,
    0,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const RTI_INT32 RTPS_LOCATOR_KIND_RESERVED = 0;
const RTI_INT32 RTPS_LOCATOR_KIND_UDPv4 = 1;
const RTI_INT32 RTPS_LOCATOR_KIND_UDPv6 = 2;
const RTI_INT32 RTPS_LOCATOR_KIND_SHMEM = 3;

/*** SOURCE_BEGIN ***/

RTPS_ReturnCode_t
RTPS_Locator_copy(struct RTPS_Locator_t *out, const struct RTPS_Locator_t *in)
{
    RTPS_ReturnCode_t result = RTPS_RETCODE_OK;

#define fail_with(__code) \
    result = (__code);\
    goto done;

    if ((out == NULL) || (in == NULL))
    {
        fail_with(RTPS_RETCODE_BAD_PARAMETER);
    }

    *out = *in;
    done:
    return result;

#undef fail_with
}

RTPS_ReturnCode_t
RTPS_Locator_initialize(struct RTPS_Locator_t * self)
{
RTPS_ReturnCode_t result = RTPS_RETCODE_OK;
#define fail_with(__code) \
result = (__code);\
goto done;

    if (self == NULL)
    {
        fail_with(RTPS_RETCODE_BAD_PARAMETER);
    }

    *self = RTPS_LOCATOR_INVALID;

    done:
    return result;

#undef fail_with
}

RTPS_ReturnCode_t
RTPS_Locator_finalize(struct RTPS_Locator_t * self)
{
RTPS_ReturnCode_t result = RTPS_RETCODE_OK;

#define fail_with(__code) \
result = (__code);\
goto done;

    if (self == NULL)
    {
        fail_with(RTPS_RETCODE_BAD_PARAMETER);
    }

    done:
    return result;
#undef fail_with
}


RTI_BOOL
RTPS_Locator_is_equal(const struct RTPS_Locator_t * left,
                      const struct RTPS_Locator_t * right)
{
    if ((left == NULL) || (right == NULL))
    {
        return RTI_FALSE;
    }

    return !OSAPI_Memory_compare(left, right, sizeof(struct RTPS_Locator_t));
}

RTI_BOOL
RTPS_Locator_is_consistent(const struct RTPS_Locator_t *self)
{
    if (self == NULL)
    {
        return RTI_FALSE;
    }

    if ((self->kind == RTPS_LOCATOR_KIND_UDPv4) &&
        ((self->port > 0) && (self->port < 0xffff)))
    {
        return RTI_TRUE;
    }

    return RTI_FALSE;
}
