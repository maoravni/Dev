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

int SerializerBase::storeStartPosition(F_FILE* f)
{
    m_fileStartPos = f_tell(f);

    // advance to the start of the serialization location
    if (f_write(&m_fileStartPos, M_RECORD_LENGTH_SIZE, 1, f) != 1)
        return 0;
    return M_RECORD_LENGTH_SIZE;
}

int SerializerBase::serializeVersion(F_FILE* f)
{
    uint8_t version = getSerializationVersion();
    // write the serialization version:
    int result = f_write(&version, sizeof(version), 1, f);
    if (result != sizeof(version))
        return 0;

    return sizeof(version);
}

int SerializerBase::deserializeRecordSize(F_FILE* f, uint16_t &size)
{
    if (f_read(&size, M_RECORD_LENGTH_SIZE, 1, f) == 0)
        return 0;

    return 1;
}

int SerializerBase::deserializeVersion(F_FILE* f)
{
    uint8_t version;
    if (f_read(&version, sizeof(version), 1, f) == 0)
        return 0;

    if (version != getSerializationVersion())
        return 0;

    return 1;
}

int SerializerBase::updateRecordSize(F_FILE* f)
{
    int size = f_tell(f) - m_fileStartPos;

    if (f_seek(f, -size, F_SEEK_CUR) != F_NO_ERROR)
        return 0;

    if (f_write(&size, M_RECORD_LENGTH_SIZE, 1, f) == 0)
        return 0;

    if (f_seek(f, size-2, F_SEEK_CUR) != F_NO_ERROR)
        return 0;

    return 1;
}

int SerializerBase::serializeClassType(F_FILE* f)
{
    uint8_t classType = getClassType();
    // write the serialization version:
    int result = f_write(&classType, sizeof(classType), 1, f);
    if (result != sizeof(classType))
        return 0;

    return sizeof(classType);
}

int SerializerBase::deserializeClassType(F_FILE* f, uint8_t &classType)
{
    if (f_read(&classType, sizeof(classType), 1, f) == 0)
        return 0;

    if (classType != getClassType())
        return 0;

    return 1;
}
