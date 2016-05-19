/*
 * PsocPeripheralSerializer.h
 *
 *  Created on: 25 Apr 2016
 *      Author: maora
 */

#ifndef PSOCPERIPHERALSERIALIZER_H_
#define PSOCPERIPHERALSERIALIZER_H_

#include "Serializer.h"
#include "PeripheralSerializers.h"
#include <Peripherals/PSoC/PsocAnalogInputsPeripheral.h>
#include <Peripherals/PSoC/PsocHandler.h>

template<> class Serializer<PsocAnalogInputsPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_PsocAnalogInputsPeripheral;}
    void serialize(F_FILE* f, PsocAnalogInputsPeripheral &p);
    void deserialize(F_FILE* f, PsocAnalogInputsPeripheral &p);
};

template<> class Serializer<PsocPwmOutputPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_PsocPwmOutputPeripheral;}
    void serialize(F_FILE* f, PsocPwmOutputPeripheral &p);
    void deserialize(F_FILE* f, PsocPwmOutputPeripheral &p);
};

template<> class Serializer<PsocAnalogOutputPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_PsocAnalogOutputPeripheral;}
    void serialize(F_FILE* f, PsocAnalogOutputPeripheral &p);
    void deserialize(F_FILE* f, PsocAnalogOutputPeripheral &p);
};

template<> class Serializer<PsocDigitalOutputPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_PsocDigitalOutputPeripheral;}
    void serialize(F_FILE* f, PsocDigitalOutputPeripheral &p);
    void deserialize(F_FILE* f, PsocDigitalOutputPeripheral &p);
};

template<> class Serializer<PsocTemperaturePeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_PsocTemperaturePeripheral;}
    void serialize(F_FILE* f, PsocTemperaturePeripheral &p);
    void deserialize(F_FILE* f, PsocTemperaturePeripheral &p);
};

template<> class Serializer<PsocDigitalInputPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_PsocDigitalInputPeripheral;}
    void serialize(F_FILE* f, PsocDigitalInputPeripheral &p);
    void deserialize(F_FILE* f, PsocDigitalInputPeripheral &p);
};

#endif /* PSOCPERIPHERALSERIALIZER_H_ */
