/*
 * RepositorySerializers.h
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#ifndef REPOSITORYSERIALIZERS_H_
#define REPOSITORYSERIALIZERS_H_

#include "Serializer.h"
#include <Elements/ElementRepository.h>

template<> class Serializer<ElementRepository> : public SerializerBase
{

public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return 0;}
    void serialize(F_FILE* f, ElementRepository &e);
    void deserialize(F_FILE* f, ElementRepository &e);

    void deserializeElement(F_FILE* f, ElementRepository &e);
};

#endif /* REPOSITORYSERIALIZERS_H_ */
