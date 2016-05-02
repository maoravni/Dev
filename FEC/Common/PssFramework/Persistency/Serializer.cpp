/*
 * Serializer.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#include <Persistency/Serializer.h>

//template <class T> Serializer<T>::Serializer()
//{
//}
//
//template <class T> Serializer<T>::~Serializer()
//{
//}

//int serializer_serializeVersion(F_FILE* f, unsigned char version)
//{
//}

void SerializerBase::storeStartPosition(F_FILE* f)
{
    m_fileStartPos = f_tell(f);

    // advance to the start of the serialization location
    if (f_write(&m_fileStartPos, M_RECORD_LENGTH_SIZE, 1, f) != 1)
        throw "File operation Failed";
}

void SerializerBase::serializeVersion(F_FILE* f)
{
    uint8_t version = getSerializationVersion();
    // write the serialization version:
    int result = f_write(&version, sizeof(version), 1, f);
    if (result != sizeof(version))
        throw "File operation Failed";
}

uint16_t SerializerBase::deserializeRecordSize(F_FILE* f)
{
    uint16_t result;
    if (f_read(&result, M_RECORD_LENGTH_SIZE, 1, f) == 0)
        throw "File operation Failed";

    return result;
}

void SerializerBase::deserializeVersion(F_FILE* f)
{
    uint8_t version;
    if (f_read(&version, sizeof(version), 1, f) == 0)
        throw "File operation Failed";

    if (version != getSerializationVersion())
        throw "Wrong Version";
}

void SerializerBase::updateRecordSize(F_FILE* f)
{
    int size = f_tell(f) - m_fileStartPos;

    if (f_seek(f, -size, F_SEEK_CUR) != F_NO_ERROR)
        throw "File operation Failed";

    if (f_write(&size, M_RECORD_LENGTH_SIZE, 1, f) == 0)
        throw "File operation Failed";

    if (f_seek(f, size-2, F_SEEK_CUR) != F_NO_ERROR)
        throw "File operation Failed";
}

void SerializerBase::serializeClassType(F_FILE* f)
{
    uint8_t classType = getClassType();
    // write the serialization version:
    int result = f_write(&classType, sizeof(classType), 1, f);
    if (result != sizeof(classType))
        throw "File operation Failed";
}

uint8_t SerializerBase::deserializeClassType(F_FILE* f)
{
    uint8_t classType;
    if (f_read(&classType, sizeof(classType), 1, f) == 0)
        throw "File operation Failed";

    if (classType != getClassType())
        throw "Wrong Class Type";

    return classType;
}
