/*
 * RangeCheckerSerializer.cpp
 *
 *  Created on: 10 Apr 2016
 *      Author: maora
 */

#include <Persistency/RangeCheckerSerializer.h>

void Serializer<RangeChecker<float> >::serialize(F_FILE* f, RangeChecker<float>& r)
{
    storeStartPosition(f);

    M_FWRITE_VARIABLE(r.m_minValue, f);
    M_FWRITE_VARIABLE(r.m_maxValue, f);
    M_FWRITE_VARIABLE(r.m_checkMin, f);
    M_FWRITE_VARIABLE(r.m_checkMax, f);

    updateRecordSize(f);
}

void Serializer<RangeChecker<int8_t> >::serialize(F_FILE* f, RangeChecker<int8_t>& r)
{
    storeStartPosition(f);

    M_FWRITE_VARIABLE(r.m_minValue, f);
    M_FWRITE_VARIABLE(r.m_maxValue, f);
    M_FWRITE_VARIABLE(r.m_checkMin, f);
    M_FWRITE_VARIABLE(r.m_checkMax, f);

    updateRecordSize(f);
}

void Serializer<RangeChecker<int16_t> >::serialize(F_FILE* f, RangeChecker<int16_t>& r)
{
    storeStartPosition(f);

    M_FWRITE_VARIABLE(r.m_minValue, f);
    M_FWRITE_VARIABLE(r.m_maxValue, f);
    M_FWRITE_VARIABLE(r.m_checkMin, f);
    M_FWRITE_VARIABLE(r.m_checkMax, f);

    updateRecordSize(f);
}

void Serializer<RangeChecker<int32_t> >::serialize(F_FILE* f, RangeChecker<int32_t>& r)
{
    storeStartPosition(f);

    M_FWRITE_VARIABLE(r.m_minValue, f);
    M_FWRITE_VARIABLE(r.m_maxValue, f);
    M_FWRITE_VARIABLE(r.m_checkMin, f);
    M_FWRITE_VARIABLE(r.m_checkMax, f);

    updateRecordSize(f);
}

void Serializer<RangeChecker<uint8_t> >::serialize(F_FILE* f, RangeChecker<uint8_t>& r)
{
    storeStartPosition(f);

    M_FWRITE_VARIABLE(r.m_minValue, f);
    M_FWRITE_VARIABLE(r.m_maxValue, f);
    M_FWRITE_VARIABLE(r.m_checkMin, f);
    M_FWRITE_VARIABLE(r.m_checkMax, f);

    updateRecordSize(f);
}

void Serializer<RangeChecker<uint16_t> >::serialize(F_FILE* f, RangeChecker<uint16_t>& r)
{
    storeStartPosition(f);

    M_FWRITE_VARIABLE(r.m_minValue, f);
    M_FWRITE_VARIABLE(r.m_maxValue, f);
    M_FWRITE_VARIABLE(r.m_checkMin, f);
    M_FWRITE_VARIABLE(r.m_checkMax, f);

    updateRecordSize(f);
}

void Serializer<RangeChecker<uint32_t> >::serialize(F_FILE* f, RangeChecker<uint32_t>& r)
{
    storeStartPosition(f);

    M_FWRITE_VARIABLE(r.m_minValue, f);
    M_FWRITE_VARIABLE(r.m_maxValue, f);
    M_FWRITE_VARIABLE(r.m_checkMin, f);
    M_FWRITE_VARIABLE(r.m_checkMax, f);

    updateRecordSize(f);
}

void Serializer<RangeChecker<float> >::deserialize(F_FILE* f, RangeChecker<float>& r)
{
    deserializeRecordSize(f);

    M_FREAD_VARIABLE(r.m_minValue, f);
    M_FREAD_VARIABLE(r.m_maxValue, f);
    M_FREAD_VARIABLE(r.m_checkMin, f);
    M_FREAD_VARIABLE(r.m_checkMax, f);
}

void Serializer<RangeChecker<uint8_t> >::deserialize(F_FILE* f, RangeChecker<uint8_t>& r)
{
    deserializeRecordSize(f);

    M_FREAD_VARIABLE(r.m_minValue, f);
    M_FREAD_VARIABLE(r.m_maxValue, f);
    M_FREAD_VARIABLE(r.m_checkMin, f);
    M_FREAD_VARIABLE(r.m_checkMax, f);
}

void Serializer<RangeChecker<uint16_t> >::deserialize(F_FILE* f, RangeChecker<uint16_t>& r)
{
    deserializeRecordSize(f);

    M_FREAD_VARIABLE(r.m_minValue, f);
    M_FREAD_VARIABLE(r.m_maxValue, f);
    M_FREAD_VARIABLE(r.m_checkMin, f);
    M_FREAD_VARIABLE(r.m_checkMax, f);
}

void Serializer<RangeChecker<uint32_t> >::deserialize(F_FILE* f, RangeChecker<uint32_t>& r)
{
    deserializeRecordSize(f);

    M_FREAD_VARIABLE(r.m_minValue, f);
    M_FREAD_VARIABLE(r.m_maxValue, f);
    M_FREAD_VARIABLE(r.m_checkMin, f);
    M_FREAD_VARIABLE(r.m_checkMax, f);
}

void Serializer<RangeChecker<int8_t> >::deserialize(F_FILE* f, RangeChecker<int8_t>& r)
{
    deserializeRecordSize(f);

    M_FREAD_VARIABLE(r.m_minValue, f);
    M_FREAD_VARIABLE(r.m_maxValue, f);
    M_FREAD_VARIABLE(r.m_checkMin, f);
    M_FREAD_VARIABLE(r.m_checkMax, f);
}

void Serializer<RangeChecker<int16_t> >::deserialize(F_FILE* f, RangeChecker<int16_t>& r)
{
    deserializeRecordSize(f);

    M_FREAD_VARIABLE(r.m_minValue, f);
    M_FREAD_VARIABLE(r.m_maxValue, f);
    M_FREAD_VARIABLE(r.m_checkMin, f);
    M_FREAD_VARIABLE(r.m_checkMax, f);
}

void Serializer<RangeChecker<int32_t> >::deserialize(F_FILE* f, RangeChecker<int32_t>& r)
{
    deserializeRecordSize(f);

    M_FREAD_VARIABLE(r.m_minValue, f);
    M_FREAD_VARIABLE(r.m_maxValue, f);
    M_FREAD_VARIABLE(r.m_checkMin, f);
    M_FREAD_VARIABLE(r.m_checkMax, f);
}

