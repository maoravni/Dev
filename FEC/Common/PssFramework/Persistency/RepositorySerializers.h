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
    int serialize(F_FILE* f, ElementRepository &e);
    int deserialize(F_FILE* f, ElementRepository &e);

    int deserializeElement(F_FILE* f, ElementRepository &e);
};

#endif /* REPOSITORYSERIALIZERS_H_ */
