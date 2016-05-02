/*
 * RangeCheckerSerializer.cpp
 *
 *  Created on: 10 Apr 2016
 *      Author: maora
 */

#include <Persistency/RangeCheckerSerializer.h>

int Serializer<RangeChecker<float> >::serialize(F_FILE* f, RangeChecker<float>& r)
{
    storeStartPosition(f);

    if (f_write(&r.m_minValue, sizeof(float), 1, f) == 0)
        return 0;

    if (f_write(&r.m_maxValue, sizeof(float), 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<RangeChecker<int8_t> >::serialize(F_FILE* f, RangeChecker<int8_t>& r)
{
    storeStartPosition(f);

    if (f_write(&r.m_minValue, sizeof(int8_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_maxValue, sizeof(int8_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<RangeChecker<int16_t> >::serialize(F_FILE* f, RangeChecker<int16_t>& r)
{
    storeStartPosition(f);

    if (f_write(&r.m_minValue, sizeof(int16_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_maxValue, sizeof(int16_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<RangeChecker<int32_t> >::serialize(F_FILE* f, RangeChecker<int32_t>& r)
{
    storeStartPosition(f);

    if (f_write(&r.m_minValue, sizeof(int32_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_maxValue, sizeof(int32_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<RangeChecker<uint8_t> >::serialize(F_FILE* f, RangeChecker<uint8_t>& r)
{
    storeStartPosition(f);

    if (f_write(&r.m_minValue, sizeof(uint8_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_maxValue, sizeof(uint8_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<RangeChecker<uint16_t> >::serialize(F_FILE* f, RangeChecker<uint16_t>& r)
{
    storeStartPosition(f);

    if (f_write(&r.m_minValue, sizeof(uint16_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_maxValue, sizeof(uint16_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<RangeChecker<uint32_t> >::serialize(F_FILE* f, RangeChecker<uint32_t>& r)
{
    storeStartPosition(f);

    if (f_write(&r.m_minValue, sizeof(uint32_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_maxValue, sizeof(uint32_t), 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_write(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<RangeChecker<float> >::deserialize(F_FILE* f, RangeChecker<float>& r)
{
    deserializeRecordSize(f);

    if (f_read(&r.m_minValue, sizeof(float), 1, f) == 0)
        return 0;

    if (f_read(&r.m_maxValue, sizeof(float), 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    return 1;
}

int Serializer<RangeChecker<uint8_t> >::deserialize(F_FILE* f, RangeChecker<uint8_t>& r)
{
    deserializeRecordSize(f);

    if (f_read(&r.m_minValue, sizeof(uint8_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_maxValue, sizeof(uint8_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    return 1;
}

int Serializer<RangeChecker<uint16_t> >::deserialize(F_FILE* f, RangeChecker<uint16_t>& r)
{
    deserializeRecordSize(f);

    if (f_read(&r.m_minValue, sizeof(uint16_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_maxValue, sizeof(uint16_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    return 1;
}

int Serializer<RangeChecker<uint32_t> >::deserialize(F_FILE* f, RangeChecker<uint32_t>& r)
{
    deserializeRecordSize(f);

    if (f_read(&r.m_minValue, sizeof(uint32_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_maxValue, sizeof(uint32_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    return 1;
}

int Serializer<RangeChecker<int8_t> >::deserialize(F_FILE* f, RangeChecker<int8_t>& r)
{
    deserializeRecordSize(f);

    if (f_read(&r.m_minValue, sizeof(int8_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_maxValue, sizeof(int8_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    return 1;
}

int Serializer<RangeChecker<int16_t> >::deserialize(F_FILE* f, RangeChecker<int16_t>& r)
{
    deserializeRecordSize(f);

    if (f_read(&r.m_minValue, sizeof(int16_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_maxValue, sizeof(int16_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    return 1;
}

int Serializer<RangeChecker<int32_t> >::deserialize(F_FILE* f, RangeChecker<int32_t>& r)
{
    deserializeRecordSize(f);

    if (f_read(&r.m_minValue, sizeof(int32_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_maxValue, sizeof(int32_t), 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMin, 1, 1, f) == 0)
        return 0;

    if (f_read(&r.m_checkMax, 1, 1, f) == 0)
        return 0;

    return 1;
}

