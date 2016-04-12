/*
 * Serializer.h
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <api/fat_sl.h>

#define M_RECORD_LENGTH_SIZE 2

class SerializerBase
{
protected:
    int m_fileStartPos;
public:
    SerializerBase():m_fileStartPos(-1){}

    virtual uint8_t getSerializationVersion() = 0;
    //virtual int serialize(F_FILE* f, T &t) = 0;
    //virtual int deserialize(F_FILE* f, T &t);

    int storeStartPosition(F_FILE* f);
    int updateRecordSize(F_FILE* f);
    int deserializeRecordSize(F_FILE* f, uint16_t &size);
    int serializeVersion(F_FILE* f);
    int deserializeVersion(F_FILE* f);
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
