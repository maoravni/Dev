/*
 * PeripheralRepositorySerializer.h
 *
 *  Created on: 20 Apr 2016
 *      Author: maora
 */

#ifndef PERIPHERALREPOSITORYSERIALIZER_H_
#define PERIPHERALREPOSITORYSERIALIZER_H_

#include "Serializer.h"
#include <Peripherals/PeripheralRepository.h>

template<> class Serializer<PeripheralRepository> : public SerializerBase
{

public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return -1;}
    int serialize(F_FILE* f, PeripheralRepository &e);
    int deserialize(F_FILE* f, PeripheralRepository &e);

    int deserializePeripheral(F_FILE* f, PeripheralRepository &e);
};

#endif /* PERIPHERALREPOSITORYSERIALIZER_H_ */
