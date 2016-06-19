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

#define M_FWRITE_VARIABLE(v, f) if (f_write(&v, sizeof(v), 1, f) != 1) throw __FILE__, __LINE__,"File operation Failed"
#define M_FREAD_VARIABLE(v, f) if (f_read(&v, sizeof(v), 1, f) != 1) throw __FILE__, __LINE__,"File operation Failed"
#define M_FREAD_AND_REFERENCE_ELEMENT(element, file) \
    { \
        uint16_t temp; \
        M_FREAD_VARIABLE(temp, file); \
        element = ElementRepository::getInstance().getElementByIndex(temp); \
        if (element == NULL) \
            throw __FILE__, __LINE__,"Element not found"; \
    }
#define M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(element, castTo, file) \
    { \
        uint16_t temp; \
        M_FREAD_VARIABLE(temp, file); \
        element = dynamic_cast<castTo*>(ElementRepository::getInstance().getElementByIndex(temp)); \
        if (element == NULL) \
            throw __FILE__, __LINE__,"Element not found"; \
    }



class SerializerBase
{
protected:
    int m_fileStartPos;
public:
    SerializerBase():m_fileStartPos(-1){}
    virtual ~SerializerBase() {}

    virtual uint8_t getSerializationVersion() = 0;
    virtual uint8_t getClassType() = 0;
    //virtual void serialize(F_FILE* f, T &t) = 0;
    //virtual void deserialize(F_FILE* f, T &t);

    void storeStartPosition(F_FILE* f);
    void updateRecordSize(F_FILE* f);
    uint16_t deserializeRecordSize(F_FILE* f);

    void serializeVersion(F_FILE* f);
    void deserializeVersion(F_FILE* f);

    void serializeClassType(F_FILE* f);
    uint8_t deserializeClassType(F_FILE* f);


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

    void serialize(F_FILE* f, T &t);
    void deserialize(F_FILE* f, T &t);

};

template<class T>
inline void Serializer<T>::serialize(F_FILE* f, T& t)
{
    static_assert(sizeof(T) == 0, "Please implement template specialization for the specific type");
    throw "Not Implemented";
}

template<class T>
inline void Serializer<T>::deserialize(F_FILE* f, T& t)
{
    static_assert(sizeof(T) == 0, "Please implement template specialization for the specific type");
    throw "Not Implemented";
}

#endif /* SERIALIZER_H_ */
