/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "osapi/osapi_types.h"
#include "util/util_makeheader.h"
#include "rtps/rtps_rtps.h"

/*** SOURCE_BEGIN ***/

RTI_BOOL
MIGRtpsParameterSequence_skip(const char **stream, RTI_BOOL needByteSwap)
{
    RTI_UINT16 parameterLength = 0;
    RTI_INT16 parameterId;

    /* read parameterId */
    if (needByteSwap)
    {
        *((char *)(&parameterId) + 1) = *((*stream)++);
        *((char *)(&parameterId)) = *((*stream)++);
    }
    else
    {
        parameterId = *((RTI_UINT16 *) (*stream));
        *stream += 2;
    }

    /* read parameter length */
    if (needByteSwap)
    {
        *((char *)(&parameterLength) + 1) = *((*stream)++);
        *((char *)(&parameterLength)) = *((*stream)++);
    }
    else
    {
        parameterLength = *((RTI_UINT16 *) (*stream));
        *stream += 2;
    }

    while (parameterId != (int)MIG_RTPS_PID_SENTINEL)
    {
        /* skip length */
        *stream += parameterLength;
        /* read parameter id */
        if (needByteSwap)
        {
            *((char *)(&parameterId) + 1) = *((*stream)++);
            *((char *)(&parameterId)) = *((*stream)++);
        }
        else
        {
            parameterId = *((RTI_UINT16 *) (*stream));
            *stream += 2;
        }
        /* read parameter length */
        if (needByteSwap)
        {
            *((char *)(&parameterLength) + 1) = *((*stream)++);
            *((char *)(&parameterLength)) = *((*stream)++);
        }
        else
        {
            parameterLength = *((RTI_UINT16 *) (*stream));
            *stream += 2;
        }
    }
    return RTI_TRUE;
}
