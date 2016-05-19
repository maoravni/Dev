/*
 * ElementSerializers.cpp
 *
 *  Created on: 8 Apr 2016
 *      Author: maora
 */

#include <Persistency/ElementSerializers.h>
//#include <api/fat_sl.h>
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

void Serializer<ElementBase>::serialize(F_FILE* f, ElementBase& e, E_SerializationElementType eType)
{
    storeStartPosition(f);

    // write the element type before the version:
    M_FWRITE_VARIABLE(eType, f);

    serializeVersion(f);

    ElementBaseInfo ebi;
    ebi.elementIndex = e.m_elementIndex;
    ebi.pssId = e.m_pssId;
    ebi.minInterval = e.m_minInterval;
    ebi.maxInterval = e.m_maxInterval;
    ebi.monitoringEnabled = e.m_monitoringEnabled;

    M_FWRITE_VARIABLE(ebi, f);

    updateRecordSize(f);
}

void Serializer<ElementBase>::deserialize(F_FILE* f, ElementBase& e)
{
    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    deserializeVersion(f);

    ElementBaseInfo ebi;

    M_FREAD_VARIABLE(ebi, f);

    //e.elementIndex = ebi.m_elementIndex;
    if (e.m_elementIndex != ebi.elementIndex)
        throw "Bad Serialization";

    e.m_pssId = ebi.pssId;
    e.m_minInterval = ebi.minInterval;
    e.m_maxInterval = ebi.maxInterval;
    e.m_monitoringEnabled = ebi.monitoringEnabled;
}

void Serializer<Element<uint32_t> >::serialize(F_FILE* f, Element<uint32_t>& e)
{
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_U32);
}

void Serializer<Element<uint32_t> >::deserialize(F_FILE* f, Element<uint32_t>& e)
{
    Serializer < ElementBase > s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

void Serializer<Element<int32_t> >::serialize(F_FILE* f, Element<int32_t>& e)
{
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_S32);
}

void Serializer<Element<int32_t> >::deserialize(F_FILE* f, Element<int32_t>& e)
{
    Serializer < ElementBase > s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

void Serializer<Element<uint8_t> >::serialize(F_FILE* f, Element<uint8_t>& e)
{
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_U8);
}

void Serializer<Element<uint8_t> >::deserialize(F_FILE* f, Element<uint8_t>& e)
{
    Serializer < ElementBase > s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

void Serializer<Element<int8_t> >::serialize(F_FILE* f, Element<int8_t>& e)
{
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_S8);
}

void Serializer<Element<int8_t> >::deserialize(F_FILE* f, Element<int8_t>& e)
{
    Serializer < ElementBase > s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

void Serializer<Element<float> >::serialize(F_FILE* f, Element<float>& e)
{
    Serializer<ElementBase> s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Float);
}

void Serializer<Element<float> >::deserialize(F_FILE* f, Element<float>& e)
{
    Serializer<ElementBase> s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

void Serializer<Element<int16_t> >::serialize(F_FILE* f, Element<int16_t>& e)
{
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_S16);
}

void Serializer<Element<int16_t> >::deserialize(F_FILE* f, Element<int16_t>& e)
{
    Serializer < ElementBase > s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

void Serializer<Element<uint16_t> >::serialize(F_FILE* f, Element<uint16_t>& e)
{
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_U16);
}

void Serializer<Element<uint16_t> >::deserialize(F_FILE* f, Element<uint16_t>& e)
{
    Serializer < ElementBase > s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));
}

void Serializer<ValidationElement<float> >::serialize(F_FILE* f, ValidationElement<float>& e)
{
    storeStartPosition(f);

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_Float, f) == -1)
        throw "File operation Failed";

    serializeVersion(f);

    // serialize the base class.
    Serializer<ElementBase> s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_Float);

    // write the missing device priority
    M_FWRITE_VARIABLE(e.m_missingDevicePriority, f);

    Serializer<RangeChecker<float> > rs;
    rs.serialize(f, e.m_allowedRange);
    rs.serialize(f, e.m_workingRange);
    rs.serialize(f, e.m_warningRange);

    updateRecordSize(f);
}

void Serializer<ValidationElement<float> >::deserialize(F_FILE* f, ValidationElement<float>& e)
{
    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    deserializeVersion(f);

    // serialize the base class.
    Serializer<ElementBase> s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));

    // write the missing device priority
    M_FREAD_VARIABLE(e.m_missingDevicePriority, f);

    Serializer<RangeChecker<float> > rs;
    rs.deserialize(f, e.m_allowedRange);
    rs.deserialize(f, e.m_workingRange);
    rs.deserialize(f, e.m_warningRange);
}

void Serializer<ValidationElement<uint8_t> >::serialize(F_FILE* f, ValidationElement<uint8_t>& e)
{
    storeStartPosition(f);

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_U8, f) == -1)
        throw "File operation Failed";

    serializeVersion(f);

    // serialize the base class.
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_U8);

    // write the missing device priority
    M_FWRITE_VARIABLE(e.m_missingDevicePriority, f);

    Serializer < RangeChecker<uint8_t> > rs;
    rs.serialize(f, e.m_allowedRange);
    rs.serialize(f, e.m_workingRange);
    rs.serialize(f, e.m_warningRange);

    updateRecordSize(f);

}

void Serializer<ValidationElement<uint8_t> >::deserialize(F_FILE* f, ValidationElement<uint8_t>& e)
{
    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    deserializeVersion(f);

    // serialize the base class.
    Serializer<ElementBase> s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));

    // write the missing device priority
    M_FREAD_VARIABLE(e.m_missingDevicePriority, f);

    Serializer<RangeChecker<uint8_t> > rs;
    rs.deserialize(f, e.m_allowedRange);
    rs.deserialize(f, e.m_workingRange);
    rs.deserialize(f, e.m_warningRange);
}

void Serializer<ValidationElement<uint16_t> >::serialize(F_FILE* f, ValidationElement<uint16_t>& e)
{
    storeStartPosition(f);

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_U16, f) == -1)
        throw "File operation Failed";

    serializeVersion(f);

    // serialize the base class.
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_U16);

    // write the missing device priority
    M_FWRITE_VARIABLE(e.m_missingDevicePriority, f);

    Serializer < RangeChecker<uint16_t> > rs;
    rs.serialize(f, e.m_allowedRange);
    rs.serialize(f, e.m_workingRange);
    rs.serialize(f, e.m_warningRange);

    updateRecordSize(f);
}

void Serializer<ValidationElement<uint16_t> >::deserialize(F_FILE* f, ValidationElement<uint16_t>& e)
{
    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    deserializeVersion(f);

    // serialize the base class.
    Serializer<ElementBase> s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));

    // write the missing device priority
    M_FREAD_VARIABLE(e.m_missingDevicePriority, f);

    Serializer<RangeChecker<uint16_t> > rs;
    rs.deserialize(f, e.m_allowedRange);
    rs.deserialize(f, e.m_workingRange);
    rs.deserialize(f, e.m_warningRange);
}

void Serializer<ValidationElement<uint32_t> >::serialize(F_FILE* f, ValidationElement<uint32_t>& e)
{
    storeStartPosition(f);

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_U32, f) == -1)
        throw "File operation Failed";

    serializeVersion(f);

    // serialize the base class.
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_U32);

    // write the missing device priority
    M_FWRITE_VARIABLE(e.m_missingDevicePriority, f);

    Serializer < RangeChecker<uint32_t> > rs;
    rs.serialize(f, e.m_allowedRange);
    rs.serialize(f, e.m_workingRange);
    rs.serialize(f, e.m_warningRange);

    updateRecordSize(f);

}

void Serializer<ValidationElement<uint32_t> >::deserialize(F_FILE* f, ValidationElement<uint32_t>& e)
{
    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    deserializeVersion(f);

    // serialize the base class.
    Serializer<ElementBase> s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));

    // write the missing device priority
    M_FREAD_VARIABLE(e.m_missingDevicePriority, f);

    Serializer<RangeChecker<uint32_t> > rs;
    rs.deserialize(f, e.m_allowedRange);
    rs.deserialize(f, e.m_workingRange);
    rs.deserialize(f, e.m_warningRange);
}

void Serializer<ValidationElement<int8_t> >::serialize(F_FILE* f, ValidationElement<int8_t>& e)
{
    storeStartPosition(f);

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_S8, f) == -1)
        throw "File operation Failed";

    serializeVersion(f);

    // serialize the base class.
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_S8);

    // write the missing device priority
    M_FWRITE_VARIABLE(e.m_missingDevicePriority, f);

    Serializer < RangeChecker<int8_t> > rs;
    rs.serialize(f, e.m_allowedRange);
    rs.serialize(f, e.m_workingRange);
    rs.serialize(f, e.m_warningRange);

    updateRecordSize(f);
}

void Serializer<ValidationElement<int8_t> >::deserialize(F_FILE* f, ValidationElement<int8_t>& e)
{
    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    deserializeVersion(f);

    // serialize the base class.
    Serializer<ElementBase> s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));

    // write the missing device priority
    M_FREAD_VARIABLE(e.m_missingDevicePriority, f);

    Serializer<RangeChecker<int8_t> > rs;
    rs.deserialize(f, e.m_allowedRange);
    rs.deserialize(f, e.m_workingRange);
    rs.deserialize(f, e.m_warningRange);
}

void Serializer<ValidationElement<int16_t> >::serialize(F_FILE* f, ValidationElement<int16_t>& e)
{
    storeStartPosition(f);

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_S16, f) == -1)
        throw "File operation Failed";

    serializeVersion(f);

    // serialize the base class.
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_S16);

    // write the missing device priority
    M_FWRITE_VARIABLE(e.m_missingDevicePriority, f);

    Serializer < RangeChecker<int16_t> > rs;
    rs.serialize(f, e.m_allowedRange);
    rs.serialize(f, e.m_workingRange);
    rs.serialize(f, e.m_warningRange);

    updateRecordSize(f);
}

void Serializer<ValidationElement<int16_t> >::deserialize(F_FILE* f, ValidationElement<int16_t>& e)
{
    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    deserializeVersion(f);

    // serialize the base class.
    Serializer<ElementBase> s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));

    // write the missing device priority
    M_FREAD_VARIABLE(e.m_missingDevicePriority, f);

    Serializer<RangeChecker<int16_t> > rs;
    rs.deserialize(f, e.m_allowedRange);
    rs.deserialize(f, e.m_workingRange);
    rs.deserialize(f, e.m_warningRange);
}

void Serializer<ValidationElement<int32_t> >::serialize(F_FILE* f, ValidationElement<int32_t>& e)
{
    storeStartPosition(f);

    // write the element type before the version:
    if (f_putc(E_SerializationElementType_Validation_S32, f) == -1)
        throw "File operation Failed";

    serializeVersion(f);

    // serialize the base class.
    Serializer < ElementBase > s;
    s.serialize(f, *(dynamic_cast<ElementBase*>(&e)), E_SerializationElementType_Validation_S32);

    // write the missing device priority
    M_FWRITE_VARIABLE(e.m_missingDevicePriority, f);

    Serializer < RangeChecker<int32_t> > rs;
    rs.serialize(f, e.m_allowedRange);
    rs.serialize(f, e.m_workingRange);
    rs.serialize(f, e.m_warningRange);

    updateRecordSize(f);
}

void Serializer<ValidationElement<int32_t> >::deserialize(F_FILE* f, ValidationElement<int32_t>& e)
{
    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    deserializeVersion(f);

    // serialize the base class.
    Serializer<ElementBase> s;
    s.deserialize(f, *(dynamic_cast<ElementBase*>(&e)));

    // write the missing device priority
    M_FREAD_VARIABLE(e.m_missingDevicePriority, f);

    Serializer<RangeChecker<int32_t> > rs;
    rs.deserialize(f, e.m_allowedRange);
    rs.deserialize(f, e.m_workingRange);
    rs.deserialize(f, e.m_warningRange);
}

