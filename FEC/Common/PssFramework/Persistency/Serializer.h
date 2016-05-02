/*
 * Serializer.h
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <api/fat_sl.h>
#include <stdint.h>
//#include <stdlib.h>
#include <vector>

#define M_RECORD_LENGTH_SIZE 2

struct EntityMapRecord
{
    uint16_t pssId;
    uint16_t filePos;
};

typedef std::vector<EntityMapRecord> T_EntityMapRecordVector;

#define M_FWRITE_VARIABLE(v, f) if (f_write(&v, sizeof(v), 1, f) != 1) return 0
#define M_FREAD_VARIABLE(v, f) if (f_read(&v, sizeof(v), 1, f) != 1) return 0

class SerializerBase
{
protected:
    int m_fileStartPos;
public:
    SerializerBase():m_fileStartPos(-1){}

    virtual uint8_t getSerializationVersion() = 0;
    virtual uint8_t getClassType() = 0;
    //virtual int serialize(F_FILE* f, T &t) = 0;
    //virtual int deserialize(F_FILE* f, T &t);

    int storeStartPosition(F_FILE* f);
    int updateRecordSize(F_FILE* f);
    int deserializeRecordSize(F_FILE* f, uint16_t &size);

    int serializeVersion(F_FILE* f);
    int deserializeVersion(F_FILE* f);

    int serializeClassType(F_FILE* f);
    int deserializeClassType(F_FILE* f, uint8_t &classType);
};

template<class T>
class Serializer : public SerializerBase
{
public:
//    Serializer(F_FILE* f) :
//            m_file(f)
//    {
//        m_startFilePos = f_tell(f);
//    }
//    virtual ~Serializer();

    int serialize(F_FILE* f, T &t);
    int deserialize(F_FILE* f, T &t);

};

template<class T>
inline int Serializer<T>::serialize(F_FILE* f, T& t)
{
    static_assert(sizeof(T) == 0, "Please implement template specialization for the specific type");
    return 0;
}

template<class T>
inline int Serializer<T>::deserialize(F_FILE* f, T& t)
{
    static_assert(sizeof(T) == 0, "Please implement template specialization for the specific type");
    return 0;
}

#endif /* SERIALIZER_H_ */
