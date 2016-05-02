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
    int serialize(F_FILE* f, ModbusPeripheralBase &p);
    int deserialize(F_FILE* f, ModbusPeripheralBase &p);
};

template<> class Serializer<ModbusInverterPeripheralBase> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusInverterPeripheralBase;}
    int serialize(F_FILE* f, ModbusInverterPeripheralBase &p);
    int deserialize(F_FILE* f, ModbusInverterPeripheralBase &p);
};

template<> class Serializer<ModbusSmcHrsChiller> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusSmcHrsChiller;}
    int serialize(F_FILE* f, ModbusSmcHrsChiller &p);
    int deserialize(F_FILE* f, ModbusSmcHrsChiller &p);
};

template<> class Serializer<Modbus6RTDPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_Modbus6RTDPeripheral;}
    int serialize(F_FILE* f, Modbus6RTDPeripheral &p);
    int deserialize(F_FILE* f, Modbus6RTDPeripheral &p);
};

template<> class Serializer<ModbusInverterCommanderSK> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusInverterCommanderSK;}
    int serialize(F_FILE* f, ModbusInverterCommanderSK &p);
    int deserialize(F_FILE* f, ModbusInverterCommanderSK &p);
};

template<> class Serializer<ModbusInverterSchneiderAtv32> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusInverterSchneiderAtv32;}
    int serialize(F_FILE* f, ModbusInverterSchneiderAtv32 &p);
    int deserialize(F_FILE* f, ModbusInverterSchneiderAtv32 &p);
};

template<> class Serializer<ModbusPumaPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusPumaPeripheral;}
    int serialize(F_FILE* f, ModbusPumaPeripheral &p);
    int deserialize(F_FILE* f, ModbusPumaPeripheral &p);
};

template<> class Serializer<Modbus8TCPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_Modbus8TCPeripheral;}
    int serialize(F_FILE* f, Modbus8TCPeripheral &p);
    int deserialize(F_FILE* f, Modbus8TCPeripheral &p);
};

template<> class Serializer<ModbusDataCardPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusDataCardPeripheral;}
    int serialize(F_FILE* f, ModbusDataCardPeripheral &p);
    int deserialize(F_FILE* f, ModbusDataCardPeripheral &p);
};

template<> class Serializer<ModbusInverterUnidriveM200> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_ModbusInverterUnidriveM200;}
    int serialize(F_FILE* f, ModbusInverterUnidriveM200 &p);
    int deserialize(F_FILE* f, ModbusInverterUnidriveM200 &p);
};

#endif /* MODBUSPERIPHERALSERIALIZERS_H_ */
