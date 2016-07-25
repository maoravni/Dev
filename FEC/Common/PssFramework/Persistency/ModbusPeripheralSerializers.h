/*
 * ModbusPeripheralSerializers.h
 *
 *  Created on: 21 Apr 2016
 *      Author: maora
 */

#ifndef MODBUSPERIPHERALSERIALIZERS_H_
#define MODBUSPERIPHERALSERIALIZERS_H_

#include "PeripheralSerializers.h"
#include <Peripherals/ModbusInverterPeripheralBase.h>
#include "Serializer.h"


class ModbusSmcHrsChiller;
class Modbus6RTDPeripheral;
class ModbusInverterCommanderSK;
class ModbusInverterSchneiderAtv32;
class ModbusPumaPeripheral;
class Modbus8TCPeripheral;
class ModbusDataCardPeripheral;
class ModbusInverterUnidriveM200;
//class ModbusInverterPeripheralBase;

template<> class Serializer<ModbusPeripheralBase> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusPeripheralBase;}
    void serialize(F_FILE* f, ModbusPeripheralBase &p);
    void deserialize(F_FILE* f, ModbusPeripheralBase &p);
};

template<> class Serializer<ModbusInverterPeripheralBase> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusInverterPeripheralBase;}
    void serialize(F_FILE* f, ModbusInverterPeripheralBase &p);
    void deserialize(F_FILE* f, ModbusInverterPeripheralBase &p);
};

template<> class Serializer<ModbusSmcHrsChiller> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusSmcHrsChiller;}
    void serialize(F_FILE* f, ModbusSmcHrsChiller &p);
    void deserialize(F_FILE* f, ModbusSmcHrsChiller &p);
};

template<> class Serializer<Modbus6RTDPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_Modbus6RTDPeripheral;}
    void serialize(F_FILE* f, Modbus6RTDPeripheral &p);
    void deserialize(F_FILE* f, Modbus6RTDPeripheral &p);
};

template<> class Serializer<ModbusInverterCommanderSK> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusInverterCommanderSK;}
    void serialize(F_FILE* f, ModbusInverterCommanderSK &p);
    void deserialize(F_FILE* f, ModbusInverterCommanderSK &p);
};

template<> class Serializer<ModbusInverterSchneiderAtv32> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusInverterSchneiderAtv32;}
    void serialize(F_FILE* f, ModbusInverterSchneiderAtv32 &p);
    void deserialize(F_FILE* f, ModbusInverterSchneiderAtv32 &p);
};

template<> class Serializer<ModbusPumaPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusPumaPeripheral;}
    void serialize(F_FILE* f, ModbusPumaPeripheral &p);
    void deserialize(F_FILE* f, ModbusPumaPeripheral &p);
};

template<> class Serializer<Modbus8TCPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_Modbus8TCPeripheral;}
    void serialize(F_FILE* f, Modbus8TCPeripheral &p);
    void deserialize(F_FILE* f, Modbus8TCPeripheral &p);
};

template<> class Serializer<ModbusDataCardPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusDataCardPeripheral;}
    void serialize(F_FILE* f, ModbusDataCardPeripheral &p);
    void deserialize(F_FILE* f, ModbusDataCardPeripheral &p);
};

template<> class Serializer<ModbusInverterUnidriveM200> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusInverterUnidriveM200;}
    void serialize(F_FILE* f, ModbusInverterUnidriveM200 &p);
    void deserialize(F_FILE* f, ModbusInverterUnidriveM200 &p);
};

#endif /* MODBUSPERIPHERALSERIALIZERS_H_ */
