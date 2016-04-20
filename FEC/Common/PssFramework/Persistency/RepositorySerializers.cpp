/*
 * RepositorySerializers.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

//#include <Elements/ElementRepository.h>
#include <Persistency/RepositorySerializers.h>
#include <Elements/ElementBase.h>
#include <vector>
#include <logger.h>

struct ElementMapRecord
{
    uint16_t pssId;
    uint16_t filePos;
};

typedef std::vector<ElementMapRecord> T_ElementMapRecordVector;

int Serializer<ElementRepository>::serialize(F_FILE* f, ElementRepository& e)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // go over all elements and count the number of elements that have a PSSID:
    uint16_t numOfElements = 0;

    numOfElements = e.m_elementList.size();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing %d/%d elements with PSSID", numOfElements, e.m_elementList.size());

    result = f_write(&numOfElements, sizeof(numOfElements), 1, f);
    if (result != 1)
        return 0;

    // write a placeholder for the map record:
    int mapPos = f_tell(f);
    ElementMapRecord elementMapRecord =
    { 0 };
    result = f_write(&elementMapRecord, sizeof(elementMapRecord), numOfElements, f);
    if (result != numOfElements)
        return 0;

    if (updateRecordSize(f) == 0)
        return 0;

    T_ElementMapRecordVector elementMapVec;

    // serialize each of the elements:
    for (int i = 0; i < e.m_elementList.size(); ++i)
    {
        // store the position of the elements in the map record.
        elementMapRecord.pssId = e.m_elementList[i]->getPssId();
        elementMapRecord.filePos = f_tell(f);
        elementMapVec.push_back(elementMapRecord);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing element %d @ %d", elementMapRecord.pssId,
                elementMapRecord.filePos);

        int recordLength = e.m_elementList[i]->serialize(f);
    }

    assert(elementMapVec.size() == numOfElements);

    if (f_seek(f, mapPos, F_SEEK_SET) != F_NO_ERROR)
        return 0;
    for (int i = 0; i < numOfElements; ++i)
    {
        if (f_write(&elementMapVec[i], sizeof(ElementMapRecord), 1, f) == 0)
            return 0;
    }
}

int Serializer<ElementRepository>::deserialize(F_FILE* f, ElementRepository& e)
{

    // read the record size:
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    // read the version:
    if (deserializeVersion(f) == 0)
        return 0;

    uint16_t numOfElements;

    if (f_read(&numOfElements, sizeof(numOfElements), 1, f) == 0)
        return 0;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Reading %d elements", numOfElements);

    ElementMapRecord mapRecord;
    T_ElementMapRecordVector mapVec;

    for (int i = 0; i < numOfElements; ++i)
    {
        if (f_read(&mapRecord, sizeof(mapRecord), 1, f) == 0)
            return 0;

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping element %d @ %d", mapRecord.pssId, mapRecord.filePos);

        mapVec.push_back(mapRecord);
    }

    for (int i = 0; i < numOfElements; ++i)
    {
        if (deserializeElement(f, e) == 0)
            return 0;
    }

    return 1;
}

int Serializer<ElementRepository>::deserializeElement(F_FILE* f, ElementRepository& e)
{
    int elementStartPosition;
    uint16_t elementRecordSize;
    elementStartPosition = f_tell(f);

    if (deserializeRecordSize(f, elementRecordSize) == 0)
        return 0;

    uint8_t dataType;
    if (f_read(&dataType, 1, 1, f) == 0)
        return 0;

    if (f_seek(f, elementStartPosition, F_SEEK_SET) != F_NO_ERROR)
        return 0;

    switch (dataType)
    {
    case E_SerializationElementType_S8:
    {
        Serializer<Element<int8_t> > s;
        ElementI8* e = ElementRepository::getInstance().addElementI8();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_S16:
    {
        Serializer<Element<int16_t> > s;
        ElementI16* e = ElementRepository::getInstance().addElementI16();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_S32:
    {
        Serializer<Element<int32_t> > s;
        ElementI32* e = ElementRepository::getInstance().addElementI32();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Bool:
    case E_SerializationElementType_U8:
    {
        Serializer<Element<uint8_t> > s;
        ElementU8* e = ElementRepository::getInstance().addElementU8();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_U16:
    {
        Serializer<Element<uint16_t> > s;
        ElementU16* e = ElementRepository::getInstance().addElementU16();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_U32:
    {
        Serializer<Element<uint32_t> > s;
        ElementU32* e = ElementRepository::getInstance().addElementU32();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Float:
    {
        Serializer<Element<float> > s;
        ElementFloat* e = ElementRepository::getInstance().addElementFloat();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Validation_S8:
    {
        Serializer<ValidationElement<int8_t> > s;
        ValidationElementI8* e = ElementRepository::getInstance().addValidationElementI8();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Validation_S16:
    {
        Serializer<ValidationElement<int16_t> > s;
        ValidationElementI16* e = ElementRepository::getInstance().addValidationElementI16();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Validation_S32:
    {
        Serializer<ValidationElement<int32_t> > s;
        ValidationElementI32* e = ElementRepository::getInstance().addValidationElementI32();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Validation_Bool:
    case E_SerializationElementType_Validation_U8:
    {
        Serializer<ValidationElement<uint8_t> > s;
        ValidationElementU8* e = ElementRepository::getInstance().addValidationElementU8();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Validation_U16:
    {
        Serializer<ValidationElement<uint16_t> > s;
        ValidationElementU16* e = ElementRepository::getInstance().addValidationElementU16();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Validation_U32:
    {
        Serializer<ValidationElement<uint32_t> > s;
        ValidationElementU32* e = ElementRepository::getInstance().addValidationElementU32();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    case E_SerializationElementType_Validation_Float:
    {
        Serializer<ValidationElement<float> > s;
        ValidationElementFloat* e = ElementRepository::getInstance().addValidationElementFloat();
        if (s.deserialize(f, *e) == 0)
            return 0;
        break;
    }
    default:
        printf("bad parsing on the deserialization file\n");
    }
    return 1;
}

