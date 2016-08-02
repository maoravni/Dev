/*
 * RepositorySerializers.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

//#include <Elements/ElementRepository.h>
#include <Persistency/ElementRepositorySerializer.h>
#include <Elements/ElementBase.h>
#include <vector>
#include <logger.h>

void Serializer<ElementRepository>::serialize(F_FILE* f, ElementRepository& e)
{
    int result;

    storeStartPosition(f);

    serializeVersion(f);

    // go over all elements and count the number of elements that have a PSSID:
    uint16_t numOfElements = e.m_elementList.size();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing %d/%d elements with PSSID", numOfElements, e.m_elementList.size());

    M_FWRITE_VARIABLE(numOfElements, f);

    // write a placeholder for the map record:
    int mapPos = f_tell(f);
    EntityMapRecord elementMapRecord =
    { 0 };
    for (int i = 0; i < e.m_elementList.size(); ++i)
        M_FWRITE_VARIABLE(elementMapRecord, f);

    updateRecordSize(f);

    T_EntityMapRecordVector elementMapVec;

    // serialize each of the elements:
    for (int i = 0; i < e.m_elementList.size(); ++i)
    {
        // store the position of the elements in the map record.
        elementMapRecord.pssId = e.m_elementList[i]->getPssId();
        elementMapRecord.filePos = f_tell(f);
        elementMapVec.push_back(elementMapRecord);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing element {[PSSID:%d]} @ %d", elementMapRecord.pssId,
                elementMapRecord.filePos);

        e.m_elementList[i]->serialize(f);
    }

    assert(elementMapVec.size() == numOfElements);

    if (f_seek(f, mapPos, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";
    for (int i = 0; i < numOfElements; ++i)
    {
        M_FWRITE_VARIABLE(elementMapVec[i], f);
    }
}

void Serializer<ElementRepository>::deserialize(F_FILE* f, ElementRepository& e)
{
    // read the record size:
    //uint16_t recordSize;
    deserializeRecordSize(f);

    // read the version:
    deserializeVersion(f);

    uint16_t numOfElements;

    M_FREAD_VARIABLE(numOfElements, f);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Reading %d elements", numOfElements);

    EntityMapRecord mapRecord;
    T_EntityMapRecordVector mapVec;

    for (int i = 0; i < numOfElements; ++i)
    {
        M_FREAD_VARIABLE(mapRecord, f);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping element {[PSSID:%d]} @ %d", mapRecord.pssId, mapRecord.filePos);

        mapVec.push_back(mapRecord);
    }

    for (int i = 0; i < numOfElements; ++i)
    {
        deserializeNextElement(f, e);
    }

}

void Serializer<ElementRepository>::serializeElement(F_FILE* f, ElementBase* e)
{
    // read the record size:
    //uint16_t recordSize;
    deserializeRecordSize(f);

    // read the version:
    deserializeVersion(f);

    uint16_t numOfElements;

    M_FREAD_VARIABLE(numOfElements, f);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Reading %d elements", numOfElements);

    EntityMapRecord mapRecord;
    T_EntityMapRecordVector mapVec;

    for (int i = 0; i < numOfElements; ++i)
    {
        M_FREAD_VARIABLE(mapRecord, f);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping element {[PSSID:%d]} @ %d", mapRecord.pssId, mapRecord.filePos);

        mapVec.push_back(mapRecord);
    }
    // after reading the map record, we can go directly to the position of the peripheral and serialize it:
    int elementIndex = e->getElementIndex();
    int elementPosition = mapVec[elementIndex].filePos;

    if (f_seek(f, elementPosition, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";

    e->serialize(f);
}

void Serializer<ElementRepository>::deserializeNextElement(F_FILE* f, ElementRepository& e)
{
    int elementStartPosition;
    uint16_t elementRecordSize;
    elementStartPosition = f_tell(f);

    deserializeRecordSize(f);

    uint8_t dataType;
    M_FREAD_VARIABLE(dataType, f);

    if (f_seek(f, elementStartPosition, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";

    switch (dataType)
    {
    case E_SerializationElementType_S8:
    {
        Serializer<Element<int8_t> > s;
        ElementI8* e = ElementRepository::getInstance().addElementI8();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_S16:
    {
        Serializer<Element<int16_t> > s;
        ElementI16* e = ElementRepository::getInstance().addElementI16();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_S32:
    {
        Serializer<Element<int32_t> > s;
        ElementI32* e = ElementRepository::getInstance().addElementI32();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Bool:
    case E_SerializationElementType_U8:
    {
        Serializer<Element<uint8_t> > s;
        ElementU8* e = ElementRepository::getInstance().addElementU8();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_U16:
    {
        Serializer<Element<uint16_t> > s;
        ElementU16* e = ElementRepository::getInstance().addElementU16();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_U32:
    {
        Serializer<Element<uint32_t> > s;
        ElementU32* e = ElementRepository::getInstance().addElementU32();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Float:
    {
        Serializer<Element<float> > s;
        ElementFloat* e = ElementRepository::getInstance().addElementFloat();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Validation_S8:
    {
        Serializer<ValidationElement<int8_t> > s;
        ValidationElementI8* e = ElementRepository::getInstance().addValidationElementI8();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Validation_S16:
    {
        Serializer<ValidationElement<int16_t> > s;
        ValidationElementI16* e = ElementRepository::getInstance().addValidationElementI16();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Validation_S32:
    {
        Serializer<ValidationElement<int32_t> > s;
        ValidationElementI32* e = ElementRepository::getInstance().addValidationElementI32();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Validation_Bool:
    case E_SerializationElementType_Validation_U8:
    {
        Serializer<ValidationElement<uint8_t> > s;
        ValidationElementU8* e = ElementRepository::getInstance().addValidationElementU8();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Validation_U16:
    {
        Serializer<ValidationElement<uint16_t> > s;
        ValidationElementU16* e = ElementRepository::getInstance().addValidationElementU16();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Validation_U32:
    {
        Serializer<ValidationElement<uint32_t> > s;
        ValidationElementU32* e = ElementRepository::getInstance().addValidationElementU32();
        s.deserialize(f, *e);
        break;
    }
    case E_SerializationElementType_Validation_Float:
    {
        Serializer<ValidationElement<float> > s;
        ValidationElementFloat* e = ElementRepository::getInstance().addValidationElementFloat();
        s.deserialize(f, *e);
        break;
    }
    default:
        printf("bad parsing on the deserialization file\n");
    }
}

