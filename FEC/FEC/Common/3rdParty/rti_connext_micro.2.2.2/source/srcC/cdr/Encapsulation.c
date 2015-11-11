/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef cdr_stream_h
#include "cdr/cdr_stream.h"
#endif
#ifndef cdr_serialize_h
#include "cdr/cdr_serialize.h"
#endif
#ifndef cdr_encapsulation_h
#include "cdr/cdr_encapsulation.h"
#endif

/*** SOURCE_BEGIN ***/

void
CDR_Stream_serialize_CdrEncapsulation(struct CDR_Stream_t *me,
                                    RTI_UINT16 *kind,
                                    RTI_UINT16 *options)
{
    CDR_Stream_serialize_UnsignedShortToBigEndian(me, kind);
    CDR_Stream_serialize_UnsignedShortToBigEndian(me, options);
}


void
CDR_Stream_deserialize_CdrEncapsulation(struct CDR_Stream_t *me,
                                      RTI_UINT16 *kind,
                                      RTI_UINT16 *options)
{
    CDR_Stream_deserialize_UnsignedShortFromBigEndian(me, kind);
    CDR_Stream_deserialize_UnsignedShortFromBigEndian(me, options);
}

RTI_BOOL
CDR_Stream_deserialize_header(struct CDR_Stream_t *stream)
{
    RTI_UINT16 kind;
    RTI_UINT16 options;
    RTI_BOOL retval = RTI_TRUE;

    CDR_Stream_deserialize_CdrEncapsulation(stream, &kind, &options);

    switch (kind)
    {
        case RTI_CDR_ENCAPSULATION_ID_PL_CDR_BE:
        case RTI_CDR_ENCAPSULATION_ID_CDR_BE:
            if (stream->endian != RTI_CDR_ENDIAN_BIG)
            {
                stream->endian = RTI_CDR_ENDIAN_BIG;
                stream->needbyteswap =
                    (stream->needbyteswap == RTI_TRUE) ? RTI_FALSE : RTI_TRUE;
            }
            break;
        case RTI_CDR_ENCAPSULATION_ID_PL_CDR_LE:
        case RTI_CDR_ENCAPSULATION_ID_CDR_LE:
            if (stream->endian != RTI_CDR_ENDIAN_LITTLE)
            {
                stream->endian = RTI_CDR_ENDIAN_LITTLE;
                stream->needbyteswap =
                    (stream->needbyteswap == RTI_TRUE) ? RTI_FALSE : RTI_TRUE;
            }
            break;
        default:
            retval = RTI_FALSE;
            break;
    }

    return retval;
}

RTI_BOOL
CDR_Stream_serialize_header(struct CDR_Stream_t * stream,RTI_BOOL is_guid)
{
    RTI_UINT16 kind;
    RTI_UINT16 options;

    if (is_guid)
    {
        kind = ((stream->endian == RTI_CDR_ENDIAN_BIG) ?
                RTI_CDR_ENCAPSULATION_ID_PL_CDR_BE :
                RTI_CDR_ENCAPSULATION_ID_PL_CDR_LE);
    }
    else
    {
        kind = ((stream->endian == RTI_CDR_ENDIAN_BIG) ?
                RTI_CDR_ENCAPSULATION_ID_CDR_BE :
                RTI_CDR_ENCAPSULATION_ID_CDR_LE);
    }

    options = RTI_CDR_ENCAPSULATION_OPTIONS_NONE;

    CDR_Stream_serialize_CdrEncapsulation(stream, &kind, &options);

    return RTI_TRUE;
}
