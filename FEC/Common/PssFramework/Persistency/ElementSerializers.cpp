/*
 * ElementSerializers.cpp
 *
 *  Created on: 8 Apr 2016
 *      Author: maora
 */

#include <Persistency/ElementSerializers.h>
#include <api/fat_sl.h>
#include <Elements/Element.h>
#include "RangeCheckerSerializer.h"

struct ElementBaseInfo
{
    uint16_t elementIndex;
    uint16_t pssId;
    uint8_t minInterval;
    uint8_t maxInterval;
    uint8_t monitoringEnabled;
};

int Serializer<ElementBase>::serialize(F_FILE* f, ElementBase& e, E_SerializationElementType eType)
{
    int result;
    if (storeStartPosition(f) == 0)
        return 0;

    // write the element type before the version:
    if (f_write(&eType, 1, 1, f) != 1)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    ElementBaseInfo ebi;
    ebi.elementIndex = e.m_elementIndex;
    ebi.pssId = e.m_pssId;
    ebi.minInterval = e.m_minInterval;
    ebi.maxInterval = e.m_maxInterval;
    ebi.monitoringEnabled = e.m_monitoringEnabled;

    if (f_write(&ebi, sizeof(ebi), 1, f) == 0)
        return 0;

    updateRecordSize(f);
}

int Serializer<ElementBase>::deserialize(F_FILE* f, ElementBase& e)
{
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (deserializeVersion(f) == 0)
        return 0;

    ElementBaseInfo ebi;

    if (f_read(&ebi, sizeof(ebi), 1, f) == 0)
        return 0;

    //e.elementIndex = ebi.m_elementIndex;
    if (e.m_elementIndex != ebi.elementIndex)
        return 0;

    e.m_pssId = ebi.pssId;
    e.m_minInterval = ebi.minInterval;
    e.m_maxInterval = ebi.maxInterval;
    e.m_monitoringEnabled = ebi.monitoringEnabled;

    return 1;
}

int Serializer<Element<uint32_t> >::serialize(F_FILE* f, Element<uint32_t>& e)
{
    Serializer < ElementBase > s;
    return s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_U32);
}

int Serializer<Element<uint32_t> >::deserialize(F_FILE* f, Element<uint32_t>& e)
{
    Serializer < ElementBase > s;
    return s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

int Serializer<Element<int32_t> >::serialize(F_FILE* f, Element<int32_t>& e)
{
    Serializer < ElementBase > s;
    return s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_S32);
}

int Serializer<Element<int32_t> >::deserialize(F_FILE* f, Element<int32_t>& e)
{
    Serializer < ElementBase > s;
    return s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

int Serializer<Element<uint8_t> >::serialize(F_FILE* f, Element<uint8_t>& e)
{
    Serializer < ElementBase > s;
    return s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_U8);
}

int Serializer<Element<uint8_t> >::deserialize(F_FILE* f, Element<uint8_t>& e)
{
    Serializer < ElementBase > s;
    return s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

int Serializer<Element<int8_t> >::serialize(F_FILE* f, Element<int8_t>& e)
{
    Serializer < ElementBase > s;
    return s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_S8);
}

int Serializer<Element<int8_t> >::deserialize(F_FILE* f, Element<int8_t>& e)
{
    Serializer < ElementBase > s;
    return s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

int Serializer<Element<float> >::serialize(F_FILE* f, Element<float>& e)
{
    Serializer<ElementBase> s;
    return s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Float);
}

int Serializer<Element<float> >::deserialize(F_FILE* f, Element<float>& e)
{
    Serializer<ElementBase> s;
    return s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

int Serializer<Element<int16_t> >::serialize(F_FILE* f, Element<int16_t>& e)
{
    Serializer < ElementBase > s;
    return s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_S16);
}

int Serializer<Element<int16_t> >::deserialize(F_FILE* f, Element<int16_t>& e)
{
    Serializer < ElementBase > s;
    return s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

int Serializer<Element<uint16_t> >::serialize(F_FILE* f, Element<uint16_t>& e)
{
    Serializer < ElementBase > s;
    return s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_U16);
}

int Serializer<Element<uint16_t> >::deserialize(F_FILE* f, Element<uint16_t>& e)
{
    Serializer < ElementBase > s;
    return s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

int Serializer<ValidationElement<float> >::serialize(F_FILE* f, ValidationElement<float>& e)
{
    int result;
    if (storeStartPosition(f) == 0)
        return 0;

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_Float, f) == -1)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer<ElementBase> s;
    if (s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_Float) == 0)
        return 0;

    // write the missing device priority
    if (f_write(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer<RangeChecker<float> > rs;
    if (rs.serialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_warningRange) == 0)
        return 0;
}

int Serializer<ValidationElement<float> >::deserialize(F_FILE* f, ValidationElement<float>& e)
{
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (deserializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer<ElementBase> s;
    if (s.deserialize(f, *(dynamic_cast<ElementBase*>(&e))) == 0)
        return 0;

    // write the missing device priority
    if (f_read(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer<RangeChecker<float> > rs;
    if (rs.deserialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_warningRange) == 0)
        return 0;

    return 1;
}

int Serializer<ValidationElement<uint8_t> >::serialize(F_FILE* f, ValidationElement<uint8_t>& e)
{
    int result;
    if (storeStartPosition(f) == 0)
        return 0;

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_U8, f) == -1)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer < ElementBase > s;
    if (s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_U8) == 0)
        return 0;

    // write the missing device priority
    if (f_write(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer < RangeChecker<uint8_t> > rs;
    if (rs.serialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_warningRange) == 0)
        return 0;
}

int Serializer<ValidationElement<uint8_t> >::deserialize(F_FILE* f, ValidationElement<uint8_t>& e)
{
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (deserializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer<ElementBase> s;
    if (s.deserialize(f, *(dynamic_cast<ElementBase*>(&e))) == 0)
        return 0;

    // write the missing device priority
    if (f_read(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer<RangeChecker<uint8_t> > rs;
    if (rs.deserialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_warningRange) == 0)
        return 0;

    return 1;
}

int Serializer<ValidationElement<uint16_t> >::serialize(F_FILE* f, ValidationElement<uint16_t>& e)
{
    int result;
    if (storeStartPosition(f) == 0)
        return 0;

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_U16, f) == -1)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer < ElementBase > s;
    if (s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_U16) == 0)
        return 0;

    // write the missing device priority
    if (f_write(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer < RangeChecker<uint16_t> > rs;
    if (rs.serialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_warningRange) == 0)
        return 0;
}

int Serializer<ValidationElement<uint16_t> >::deserialize(F_FILE* f, ValidationElement<uint16_t>& e)
{
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (deserializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer<ElementBase> s;
    if (s.deserialize(f, *(dynamic_cast<ElementBase*>(&e))) == 0)
        return 0;

    // write the missing device priority
    if (f_read(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer<RangeChecker<uint16_t> > rs;
    if (rs.deserialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_warningRange) == 0)
        return 0;

    return 1;
}

int Serializer<ValidationElement<uint32_t> >::serialize(F_FILE* f, ValidationElement<uint32_t>& e)
{
    int result;
    if (storeStartPosition(f) == 0)
        return 0;

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_U32, f) == -1)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer < ElementBase > s;
    if (s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_U32) == 0)
        return 0;

    // write the missing device priority
    if (f_write(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer < RangeChecker<uint32_t> > rs;
    if (rs.serialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_warningRange) == 0)
        return 0;
}

int Serializer<ValidationElement<uint32_t> >::deserialize(F_FILE* f, ValidationElement<uint32_t>& e)
{
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (deserializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer<ElementBase> s;
    if (s.deserialize(f, *(dynamic_cast<ElementBase*>(&e))) == 0)
        return 0;

    // write the missing device priority
    if (f_read(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer<RangeChecker<uint32_t> > rs;
    if (rs.deserialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_warningRange) == 0)
        return 0;

    return 1;
}

int Serializer<ValidationElement<int8_t> >::serialize(F_FILE* f, ValidationElement<int8_t>& e)
{
    int result;
    if (storeStartPosition(f) == 0)
        return 0;

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_S8, f) == -1)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer < ElementBase > s;
    if (s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_S8) == 0)
        return 0;

    // write the missing device priority
    if (f_write(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer < RangeChecker<int8_t> > rs;
    if (rs.serialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_warningRange) == 0)
        return 0;
}

int Serializer<ValidationElement<int8_t> >::deserialize(F_FILE* f, ValidationElement<int8_t>& e)
{
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (deserializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer<ElementBase> s;
    if (s.deserialize(f, *(dynamic_cast<ElementBase*>(&e))) == 0)
        return 0;

    // write the missing device priority
    if (f_read(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer<RangeChecker<int8_t> > rs;
    if (rs.deserialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_warningRange) == 0)
        return 0;

    return 1;
}

int Serializer<ValidationElement<int16_t> >::serialize(F_FILE* f, ValidationElement<int16_t>& e)
{
    int result;
    if (storeStartPosition(f) == 0)
        return 0;

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_S16, f) == -1)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer < ElementBase > s;
    if (s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_S16) == 0)
        return 0;

    // write the missing device priority
    if (f_write(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer < RangeChecker<int16_t> > rs;
    if (rs.serialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_warningRange) == 0)
        return 0;
}

int Serializer<ValidationElement<int16_t> >::deserialize(F_FILE* f, ValidationElement<int16_t>& e)
{
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (deserializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer<ElementBase> s;
    if (s.deserialize(f, *(dynamic_cast<ElementBase*>(&e))) == 0)
        return 0;

    // write the missing device priority
    if (f_read(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer<RangeChecker<int16_t> > rs;
    if (rs.deserialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_warningRange) == 0)
        return 0;

    return 1;
}

int Serializer<ValidationElement<int32_t> >::serialize(F_FILE* f, ValidationElement<int32_t>& e)
{
    int result;
    if (storeStartPosition(f) == 0)
        return 0;

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_S32, f) == -1)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer < ElementBase > s;
    if (s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_S32) == 0)
        return 0;

    // write the missing device priority
    if (f_write(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer < RangeChecker<int32_t> > rs;
    if (rs.serialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.serialize(f, e.m_warningRange) == 0)
        return 0;
}

int Serializer<ValidationElement<int32_t> >::deserialize(F_FILE* f, ValidationElement<int32_t>& e)
{
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (deserializeVersion(f) == 0)
        return 0;

    // serialize the base class.
    Serializer<ElementBase> s;
    if (s.deserialize(f, *(dynamic_cast<ElementBase*>(&e))) == 0)
        return 0;

    // write the missing device priority
    if (f_read(&e.m_missingDevicePriority, 1, 1, f) == 0)
        return 0;

    Serializer<RangeChecker<int32_t> > rs;
    if (rs.deserialize(f, e.m_allowedRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_workingRange) == 0)
        return 0;

    if (rs.deserialize(f, e.m_warningRange) == 0)
        return 0;

    return 1;
}

