/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#include "rtps/rtps_rtps.h"
#ifndef rtps_log_h
#include "rtps/rtps_log.h"
#endif
#ifndef cdr_serialize_h
#include "cdr/cdr_serialize.h"
#endif

#define MIG_RTPS_BITMAP_32_BITS_MSB (0x80000000)

#define MIG_RTPS_BITMAP_32_ONES (0xFFFFFFFF)
#define MIG_RTPS_BITMAP_24_ONES (0xFFFFFF)
#define MIG_RTPS_BITMAP_16_ONES (0xFFFF)
#define MIG_RTPS_BITMAP_8_ONES  (0xFF)

#define modBy32(x) ((x) & 0x1F)
#define divideBy32(x) ((x) >> 5)
#define getArraySize(length) divideBy32((length) + 31)
#define MIGRtpsBitmap_getArraySize(me) getArraySize((me)->_bitCount)

/*** SOURCE_BEGIN ***/

#ifdef RTI_PRECONDITION_TEST
void
MIGRtpsBitmap_reset(struct MIGRtpsBitmap *me,
                    const struct REDA_SequenceNumber *seqNum,
                    RTI_INT32 bitCount)
{
    MIGRtpsBitmap_resetMacro(me, seqNum, bitCount);
}
#endif /* RTI_PRECONDITION_TEST */

RTI_BOOL
MIGRtpsBitmap_setBit(struct MIGRtpsBitmap *me,
                     const struct REDA_SequenceNumber *id, RTI_BOOL on)
{
    RTI_INT32 distance, byteOffset;
    RTI_UINT32 mask;

    distance = MIGRtpsSequenceNumber_getDistance(&me->_lead, id);

    /* Check for out of range. */
    if (REDA_SequenceNumber_compare(id, &me->_lead) < 0
        || distance >= me->_bitCount)
    {
        RTPS_LOG_WARNING_P2(LOG_SEQUENCE_NUMBER_OUT_OF_RANGE, distance,
                          me->_bitCount);
        return RTI_FALSE;
    }

    byteOffset = divideBy32(distance);
    mask = 1 << (31 - modBy32(distance));

    if (on)
    {
        me->_32bits[byteOffset] |= mask;
    }
    else
    {
        me->_32bits[byteOffset] &= ~mask;
    }

    return RTI_TRUE;
}


RTI_BOOL
MIGRtpsBitmap_getBit(const struct MIGRtpsBitmap * me,
                     RTI_BOOL * bit, const struct REDA_SequenceNumber * id)
{
    RTI_INT32 byteOffset, bitOffset, distance;

    distance = MIGRtpsSequenceNumber_getDistance(&me->_lead, id);

    /* Check for out of range. */
    if (REDA_SequenceNumber_compare(id, &me->_lead) < 0 ||
        distance >= me->_bitCount)
    {
        RTPS_LOG_WARNING_P2(LOG_SEQUENCE_NUMBER_OUT_OF_RANGE, distance,
                          me->_bitCount);
        return RTI_FALSE;
    }

    byteOffset = divideBy32(distance);
    bitOffset = modBy32(distance);
    *bit = 0x01 & (me->_32bits[byteOffset] >> (31 - bitOffset));

    return RTI_TRUE;
}


RTI_BOOL
MIGRtpsBitmap_getFirstBit(const struct MIGRtpsBitmap * me,
                          struct REDA_SequenceNumber * position,
                          RTI_BOOL searchBit)
{
    RTI_BOOL found = RTI_FALSE;
    RTI_UINT32 value;
    RTI_INT32 bitArraySize, extraBits, validBits;
    RTI_INT32 i, j = 0;

    bitArraySize = MIGRtpsBitmap_getArraySize(me);
    extraBits = modBy32(me->_bitCount);
    for (i = 0; i < bitArraySize && !found; ++i)
    {
        /* If a full 32 bits, first check all bits at once. */
        if ((extraBits == 0) || (i != (bitArraySize - 1)))
        {
            if ((searchBit && me->_32bits[i] == 0) ||
                (!searchBit && me->_32bits[i] == MIG_RTPS_BITMAP_32_ONES))
            {
                continue;
            }
            validBits = 32;
        }
        else
        {
            /* The last integer may not have a full 32 valid bits. */
            validBits = extraBits;
        }

        /* Search bit by bit. */
        for (j = 0, value = me->_32bits[i];
             j < validBits && !found; ++j, value <<= 1)
        {
            if ((searchBit &&
                 ((value & MIG_RTPS_BITMAP_32_BITS_MSB) ==
                  MIG_RTPS_BITMAP_32_BITS_MSB)) ||
                (!searchBit && ((value & MIG_RTPS_BITMAP_32_BITS_MSB) == 0)))
            {
                found = RTI_TRUE;
            }
        }
    }

    if (position != NULL)
    {
        position->high = 0;
        if (found)
        {
            position->low = 32 * (i - 1) + (j - 1);
        }
        else
        {
            position->low = me->_bitCount;
        }
        REDA_SequenceNumber_increment(position, &me->_lead);
    }

    if (!found)
    {
        RTPS_LOG_DEBUG_P1(LOG_FIRST_SEARCH_BIT_NOT_FOUND, searchBit)
    }

    return found;
}


RTI_BOOL
MIGRtpsBitmap_fill(struct MIGRtpsBitmap * me,
                   const struct REDA_SequenceNumber * firstSeqNum,
                   const struct REDA_SequenceNumber * lastSeqNum, RTI_BOOL bit)
{
    RTI_BOOL ok = RTI_FALSE;
    RTI_INT32 firstBit, lastBit, first32Bits, last32Bits, startBit, stopBit;
    RTI_UINT32 mask;
    RTI_INT32 i, j;

    /* Guarantee first <= last ordering, otherwise fail. */
    if (REDA_SequenceNumber_compare(firstSeqNum, lastSeqNum) > 0)
    {
        RTPS_Log_error(
            LOG_FIRST_SEQUENCE_NUMBER_GREATER_THAN_LAST_SEQUENCE_NUMBER);
        goto finally;
    }
    /* Given ordering, if range falls outside of bitmap, noop success. */
    if ((REDA_SequenceNumber_compare(lastSeqNum, &me->_lead) < 0) ||
        ((REDA_SequenceNumber_compare(firstSeqNum, &me->_lead) >= 0) &&
         (MIGRtpsSequenceNumber_getDistance(firstSeqNum, &me->_lead) >=
          me->_bitCount)) || (me->_bitCount == 0))
    {
        goto success;
    }

    /* Convert sequences to offsets and truncate to bitmap's boundaries. */
    firstBit = (REDA_SequenceNumber_compare(firstSeqNum, &me->_lead) <= 0) ?
        0 : MIGRtpsSequenceNumber_getDistance(firstSeqNum, &me->_lead);
    lastBit = MIGRtpsSequenceNumber_getDistance(lastSeqNum, &me->_lead);
    if (lastBit >= me->_bitCount)
    {
        lastBit = me->_bitCount - 1;
    }

    /* Iterate through the bitmap and set bits as needed. */
    first32Bits = divideBy32(firstBit);
    last32Bits = divideBy32(lastBit);

    for (i = first32Bits; i <= last32Bits; ++i)
    {
        if (i == first32Bits)
        {
            startBit = modBy32(firstBit);
            mask = 1 << (31 - startBit);
        }
        else
        {
            startBit = 0;
            mask = MIG_RTPS_BITMAP_32_BITS_MSB;
        }
        stopBit = (i == last32Bits) ? modBy32(lastBit) : 31;

        /* Try to set all 32 bits at once if possible. */
        if ((startBit == 0) && (stopBit == 31))
        {
            me->_32bits[i] = bit ? MIG_RTPS_BITMAP_32_ONES : 0;
            continue;
        }

        for (j = startBit; j <= stopBit; ++j, mask >>= 1)
        {
            if (bit)
            {
                me->_32bits[i] |= mask;
            }
            else
            {
                me->_32bits[i] &= ~mask;
            }
        }
    }

    success:ok = RTI_TRUE;
    finally:return ok;
}


void
MIGRtpsBitmap_copy(struct MIGRtpsBitmap *me,
                   const struct MIGRtpsBitmap *source, unsigned int length)
{
    RTI_INT32 i = 0;
    RTI_UINT32 mask;
    RTI_INT32 bitArraySize, full32Bits, extraBits;

    me->_lead = source->_lead;
    me->_bitCount = length;

    bitArraySize = getArraySize(length);
    full32Bits = divideBy32(source->_bitCount);
    extraBits = modBy32(source->_bitCount);
    for (i = 0; i < bitArraySize; ++i)
    {
        if (i < full32Bits)
        {
            /* copy 32 bits at once */
            me->_32bits[i] = source->_32bits[i];
        }
        else if ((i == full32Bits) && (extraBits != 0))
        {
            /* Mask out invalid bits that are being copied from the source. */
            mask = MIG_RTPS_BITMAP_32_ONES << (32 - extraBits); /* 0 filled */
            me->_32bits[i] = source->_32bits[i] & mask;
        }
        else
        {
            me->_32bits[i] = 0;
        }
    }
}


void
MIGRtpsBitmap_merge(struct MIGRtpsBitmap *me,
                    const struct MIGRtpsBitmap *source)
{

    struct REDA_SequenceNumber firstSeqNum, lastSeqNum;
    RTI_INT32 firstBit, lastBit, first32Bits, last32Bits, startBit,
        stopBit, distance, i, j;
    RTI_UINT32 mask;

    firstSeqNum = source->_lead;

    /* last = first + source->bitCount - 1 */
    lastSeqNum.high = 0;
    lastSeqNum.low = source->_bitCount;
    REDA_SequenceNumber_increment(&lastSeqNum, &firstSeqNum);
    REDA_SequenceNumber_minusminus(&lastSeqNum);

    /* If source falls outside of me, noop. */
    if (REDA_SequenceNumber_compare(&lastSeqNum, &me->_lead) < 0 ||
        (REDA_SequenceNumber_compare(&firstSeqNum, &me->_lead) >= 0 &&
         MIGRtpsSequenceNumber_getDistance(&firstSeqNum,
                                           &me->_lead) >= me->_bitCount)
        || me->_bitCount == 0 || source->_bitCount == 0)
    {
        return;
    }

    /* Truncate source sequence numbers to me's boundaries. */
    if (REDA_SequenceNumber_compare(&firstSeqNum, &me->_lead) < 0)
    {
        firstSeqNum = me->_lead;
    }
    if (MIGRtpsSequenceNumber_getDistance(&lastSeqNum, &me->_lead) >=
        me->_bitCount)
    {
        lastSeqNum.high = 0;
        lastSeqNum.low = me->_bitCount;
        REDA_SequenceNumber_increment(&lastSeqNum, &me->_lead);
        REDA_SequenceNumber_minusminus(&lastSeqNum);
    }

    /* Get the bit indexes of the sequences into the source bitmap. */
    firstBit = MIGRtpsSequenceNumber_getDistance(&firstSeqNum, &source->_lead);
    lastBit = MIGRtpsSequenceNumber_getDistance(&lastSeqNum, &source->_lead);

    /* Iterate through source and set corresponding bits in dest as needed. */
    distance = MIGRtpsSequenceNumber_getDistance(&firstSeqNum, &me->_lead);
    first32Bits = divideBy32(firstBit);
    last32Bits = divideBy32(lastBit);
    for (i = first32Bits; i <= last32Bits; ++i)
    {
        if (i == first32Bits)
        {
            startBit = modBy32(firstBit);
            mask = 1 << (31 - startBit);
        }
        else
        {
            startBit = 0;
            mask = MIG_RTPS_BITMAP_32_BITS_MSB;
        }
        if (i == last32Bits)
        {
            stopBit = modBy32(lastBit);
        }
        else
        {
            stopBit = 31;
        }

        /* If possible, check 32 bits at once to see if there are any 1's. */
        if ((startBit == 0 && stopBit == 31) && /* addressing all 32 bits */
            source->_32bits[i] == 0)
        {
            /* nothing in source */
            distance += 32;     /* skip to next 32 bits */
            continue;
        }

        /* Else we need to find the 1 bits in the source bitmap. */
        /* TODO: optimize to remove for() loop */
        for (j = startBit; j <= stopBit; ++j, mask >>= 1, ++distance)
        {
            if ((source->_32bits[i] & mask) == mask)
            {
                /* Need to set the bit in the me bitmap. */
                me->_32bits[divideBy32(distance)] |= (1 <<
                                                      (31 - modBy32(distance)));
            }
        }
    }
}


RTI_BOOL
MIGRtpsBitmap_shift(struct MIGRtpsBitmap *me,
                    const struct REDA_SequenceNumber *sequenceNumber)
{
    RTI_BOOL ok = RTI_FALSE;
    RTI_INT32 i, distance, bitArraySize, full32Bits, extraBits, validBits;
    RTI_UINT32 mask, value;

    i = REDA_SequenceNumber_compare(sequenceNumber, &me->_lead);

    if (i < 0)
    {
        RTPS_LOG_WARNING(LOG_SEQUENCE_NUMBER_OUT_OF_RANGE);
        goto finally;
    }
    if (i == 0)
    {
        /* noop */
        goto success;
    }

    distance = MIGRtpsSequenceNumber_getDistance(sequenceNumber, &me->_lead);
    if (distance >= me->_bitCount)
    {
        /* 0 fill */
        MIGRtpsBitmap_reset(me, sequenceNumber, me->_bitCount);
        goto success;
    }

    me->_lead = *sequenceNumber;
    bitArraySize = MIGRtpsBitmap_getArraySize(me);
    full32Bits = divideBy32(distance);
    if (full32Bits > 0)
    {
        /* have at least 1 full 32 bit in distance */
        validBits = modBy32(me->_bitCount);
        /* Shift bits an integer (32 bits) at a time. */
        for (i = 0; (i + full32Bits) < bitArraySize; ++i)
        {
            /* The last integer may not have a full 32 valid bits to shift. */
            if (((i + full32Bits) == (bitArraySize - 1)) && (validBits != 0))
            {
                mask = MIG_RTPS_BITMAP_32_ONES << (32 - validBits);
                me->_32bits[i] = me->_32bits[i + full32Bits] & mask;
            }
            else
            {
                /* copy entire later 32 bits to earlier 32 bits */
                me->_32bits[i] = me->_32bits[i + full32Bits];
            }
        }
        /* The rest: fill with zeroes if nowhere to shift from. */
        for (; i < bitArraySize; ++i)
        {
            me->_32bits[i] = 0;
        }
    }

    extraBits = modBy32(distance);
    if (extraBits > 0)
    {
        validBits = modBy32(me->_bitCount);
        /* Shift each integer in the integer array that was fully shifted
         * in the previous step by an additional extraBits bits sucking in
         * bits from the next integer as necessary but don't handle the
         * last integer yet as it has nowhere to suck in bits if needed. */
        for (i = 0; (i + full32Bits) < (bitArraySize - 1); ++i)
        {
            me->_32bits[i] <<= extraBits;
            /* Suck in bits from next integer as needed. */
            mask = MIG_RTPS_BITMAP_32_ONES << (32 - extraBits);
            /* If next to last, we may not have enough bits to suck in from. */
            if (((i + 1 + full32Bits) == (bitArraySize - 1)) &&
                (extraBits > validBits) && (validBits != 0))
            {
                mask <<= (extraBits - validBits);
            }
            value = me->_32bits[i + 1] & mask;
            value >>= (32 - extraBits);
            me->_32bits[i] |= value;
        }
        /* Now handle the last integer of the bitmap that was fully shifted. */
        me->_32bits[i] <<= extraBits;
        /* Make sure newly shifted in bits of the last integer are zeroed. */
        mask = MIG_RTPS_BITMAP_32_ONES;
        if (validBits != 0)
        {
            mask <<= (32 - validBits);
        }
        mask <<= extraBits;
        me->_32bits[i] &= mask;
    }

    success:ok = RTI_TRUE;
    finally:return ok;
}

void
MIGRtpsBitmap_truncate(struct MIGRtpsBitmap *me,
                       const struct REDA_SequenceNumber *sequenceNumber)
{
    RTI_INT32 distance;

    if (REDA_SequenceNumber_compare(sequenceNumber, &me->_lead) < 0)
    {
        me->_bitCount = 0;
        return;
    }

    distance = MIGRtpsSequenceNumber_getDistance(&me->_lead, sequenceNumber);
    if (distance < me->_bitCount)
    {
        me->_bitCount = distance + 1;
    }
}

void
MIGRtpsBitmap_invert(struct MIGRtpsBitmap *me)
{
    RTI_INT32 bitArraySize, i;

    bitArraySize = MIGRtpsBitmap_getArraySize(me);
    for (i = 0; i < bitArraySize; ++i)
    {
        me->_32bits[i] = ~(me->_32bits[i]);
    }
}


void
MIGRtpsBitmap_serialize(const struct MIGRtpsBitmap *me,
                        char **streamPtr, RTI_BOOL needByteSwap)
{
    char *stream;
    RTI_INT32 i, int32Count = (me->_bitCount + 31) / 32;

    stream = *streamPtr;

    CDR_serialize_REDASequenceNumber(&stream, &(me->_lead), needByteSwap);
    CDR_serialize_Long(&stream, &me->_bitCount, needByteSwap);

    for (i = 0; i < int32Count; ++i)
    {
        CDR_serialize_UnsignedLong(&stream, &me->_32bits[i], needByteSwap);
    }

    *streamPtr = stream;
}

RTI_BOOL
MIGRtpsBitmap_deserialize(struct MIGRtpsBitmap *me,
                          const char **streamPtr, RTI_BOOL needByteSwap)
{
    RTI_BOOL ok = RTI_FALSE;
    RTI_INT32 i, bitArraySize;
    char *stream = (char *)(*streamPtr);

    CDR_deserialize_REDASequenceNumber(&stream, &(me->_lead), needByteSwap);
    CDR_deserialize_Long(&stream, &me->_bitCount, needByteSwap);

    /* Make sure that _bitCount does not get set to a screwy value.
     * Assume that the maximum _bitCount is MIG_RTPS_BITMAP_SIZE_MAX. */
    if ((me->_bitCount > MIG_RTPS_BITMAP_SIZE_MAX) || (me->_bitCount < 0))
    {
        /* Set _bitCount to 0 so that we don't interpret the bitmap array. */
        RTPS_Log_error_p1(LOG_BITCOUNT_OUT_OF_BOUNDS, me->_bitCount);
        me->_bitCount = 0;
        goto finally;
    }

    /* Calculate only once for efficiency */
    bitArraySize = MIGRtpsBitmap_getArraySize(me);
    for (i = 0; i < bitArraySize; ++i)
    {
        CDR_deserialize_UnsignedLong(&stream, &me->_32bits[i], needByteSwap);
    }

    /* Atomically update the stream position */
    *streamPtr = stream;

    ok = RTI_TRUE;
    finally:return ok;
}

