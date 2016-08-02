/*
 * ControlRepositorySerializer.h
 *
 *  Created on: 9 May 2016
 *      Author: maora
 */

#ifndef CONTROLREPOSITORYSERIALIZER_H_
#define CONTROLREPOSITORYSERIALIZER_H_

#include "Serializer.h"
#include <Controls/ControlRepository.h>

template<> class Serializer<ControlRepository> : public SerializerBase
{

public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return 0;}
    void serialize(F_FILE* f, ControlRepository &e);
    void deserialize(F_FILE* f, ControlRepository &e);

    void serializeControl(F_FILE* f, ControlBase* p);
    ControlBase* deserializeNextControl(F_FILE* f, ControlRepository &e);
};

#endif /* CONTROLREPOSITORYSERIALIZER_H_ */
