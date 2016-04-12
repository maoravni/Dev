/*
 * RangeCheckerSerializer.h
 *
 *  Created on: 10 Apr 2016
 *      Author: maora
 */

#ifndef RANGECHECKERSERIALIZER_H_
#define RANGECHECKERSERIALIZER_H_

#include "Serializer.h"
#include <Elements/RangeChecker.h>

template <class _type> class RangeChecker;

template<> class Serializer<RangeChecker<float> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    int serialize(F_FILE* f, RangeChecker<float> &r);
    int deserialize(F_FILE* f, RangeChecker<float> &r);
};

template<> class Serializer<RangeChecker<int8_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    int serialize(F_FILE* f, RangeChecker<int8_t> &r);
    int deserialize(F_FILE* f, RangeChecker<int8_t> &r);
};

template<> class Serializer<RangeChecker<int16_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    int serialize(F_FILE* f, RangeChecker<int16_t> &r);
    int deserialize(F_FILE* f, RangeChecker<int16_t> &r);
};

template<> class Serializer<RangeChecker<int32_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    int serialize(F_FILE* f, RangeChecker<int32_t> &r);
    int deserialize(F_FILE* f, RangeChecker<int32_t> &r);
};

template<> class Serializer<RangeChecker<uint8_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    int serialize(F_FILE* f, RangeChecker<uint8_t> &r);
    int deserialize(F_FILE* f, RangeChecker<uint8_t> &r);
};

template<> class Serializer<RangeChecker<uint16_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    int serialize(F_FILE* f, RangeChecker<uint16_t> &r);
    int deserialize(F_FILE* f, RangeChecker<uint16_t> &r);
};

template<> class Serializer<RangeChecker<uint32_t> >: public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    int serialize(F_FILE* f, RangeChecker<uint32_t> &r);
    int deserialize(F_FILE* f, RangeChecker<uint32_t> &r);
};

#endif /* RANGECHECKERSERIALIZER_H_ */
