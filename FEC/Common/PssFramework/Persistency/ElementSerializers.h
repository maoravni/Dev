/*
 * ElementSerializers.h
 *
 *  Created on: 8 Apr 2016
 *      Author: maora
 */

#ifndef ELEMENTSERIALIZERS_H_
#define ELEMENTSERIALIZERS_H_

//#include <Elements/Element.h>
#include <elements/ElementBase.h>
#include "Serializer.h"

template <class _type> class Element;
template <class _type> class ValidationElement;

template<> class Serializer<ElementBase> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return -1;}
    int serialize(F_FILE* f, ElementBase &e, E_SerializationElementType eType);
    int deserialize(F_FILE* f, ElementBase &e);
};

template<> class Serializer<Element<uint32_t> >
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_U32;}
    int serialize(F_FILE* f, Element<uint32_t> &e);
    int deserialize(F_FILE* f, Element<uint32_t> &e);
};

template<> class Serializer<Element<int32_t> >
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_S32;}
    int serialize(F_FILE* f, Element<int32_t> &e);
    int deserialize(F_FILE* f, Element<int32_t> &e);
};

template<> class Serializer<Element<uint16_t> >
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_U16;}
    int serialize(F_FILE* f, Element<uint16_t> &e);
    int deserialize(F_FILE* f, Element<uint16_t> &e);
};

template<> class Serializer<Element<int16_t> >
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_S16;}
    int serialize(F_FILE* f, Element<int16_t> &e);
    int deserialize(F_FILE* f, Element<int16_t> &e);
};

template<> class Serializer<Element<uint8_t> >
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_U8;}
    int serialize(F_FILE* f, Element<uint8_t> &e);
    int deserialize(F_FILE* f, Element<uint8_t> &e);
};

template<> class Serializer<Element<int8_t> >
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_S8;}
    int serialize(F_FILE* f, Element<int8_t> &e);
    int deserialize(F_FILE* f, Element<int8_t> &e);
};

template<> class Serializer<Element<float> >
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_Float;}
    int serialize(F_FILE* f, Element<float> &e);
    int deserialize(F_FILE* f, Element<float> &e);
};

template<> class Serializer<ValidationElement<float> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_Validation_Float;}
    int serialize(F_FILE* f, ValidationElement<float> &e);
    int deserialize(F_FILE* f, ValidationElement<float> &e);
};

template<> class Serializer<ValidationElement<uint8_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_Validation_U8;}
    int serialize(F_FILE* f, ValidationElement<uint8_t> &e);
    int deserialize(F_FILE* f, ValidationElement<uint8_t> &e);
};

template<> class Serializer<ValidationElement<uint16_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_Validation_U16;}
    int serialize(F_FILE* f, ValidationElement<uint16_t> &e);
    int deserialize(F_FILE* f, ValidationElement<uint16_t> &e);
};

template<> class Serializer<ValidationElement<uint32_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_Validation_U32;}
    int serialize(F_FILE* f, ValidationElement<uint32_t> &e);
    int deserialize(F_FILE* f, ValidationElement<uint32_t> &e);
};

template<> class Serializer<ValidationElement<int8_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_Validation_S8;}
    int serialize(F_FILE* f, ValidationElement<int8_t> &e);
    int deserialize(F_FILE* f, ValidationElement<int8_t> &e);
};

template<> class Serializer<ValidationElement<int16_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_Validation_S16;}
    int serialize(F_FILE* f, ValidationElement<int16_t> &e);
    int deserialize(F_FILE* f, ValidationElement<int16_t> &e);
};

template<> class Serializer<ValidationElement<int32_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_SerializationElementType_Validation_S32;}
    int serialize(F_FILE* f, ValidationElement<int32_t> &e);
    int deserialize(F_FILE* f, ValidationElement<int32_t> &e);
};


#endif /* ELEMENTSERIALIZERS_H_ */
