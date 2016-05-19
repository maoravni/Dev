/*
 * Mi3PeripheralSerializer.h
 *
 *  Created on: 21 Apr 2016
 *      Author: maora
 */

#ifndef MI3PERIPHERALSERIALIZER_H_
#define MI3PERIPHERALSERIALIZER_H_

#include "PeripheralSerializers.h"

class Mi3I2CIrPeripheral;
class Mi3Sensor;

template<> class Serializer<Mi3I2CIrPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_Mi3I2CIrPeripheral;}
    void serialize(F_FILE* f, Mi3I2CIrPeripheral &p);
    void deserialize(F_FILE* f, Mi3I2CIrPeripheral &p);
};

template<> class Serializer<Mi3Sensor> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_Mi3Sensor;}
    void serialize(F_FILE* f, Mi3Sensor &p);
    void deserialize(F_FILE* f, Mi3Sensor &p);
};

#endif /* MI3PERIPHERALSERIALIZER_H_ */
