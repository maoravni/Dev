/*
 * PeripheralSerializers.h
 *
 *  Created on: 20 Apr 2016
 *      Author: maora
 */

#ifndef PERIPHERALSERIALIZERS_H_
#define PERIPHERALSERIALIZERS_H_

#include "Serializer.h"
//#include <api/fat_sl.h>
//#include <Peripherals/PeripheralBase.h>

class PeripheralBase;
class AnalogInputPeripheral;
class AnalogOutCurrentPeripheral;
class DigitalInputsPeripheral;
class DigitalOutputsPeripheral;
class DryContactDigitalOutput;
class InternalTemperatureSensors;
class PsocAnalogInputsPeripheral;
class PsocPwmOutputPeripheral;
class PsocAnalogOutputPeripheral;
class PsocDigitalOutputPeripheral;
class PsocTemperaturePeripheral;
class PsocDigitalInputPeripheral;
class SwPwmOutputPeripheral;
class VirtualPeripheral;

enum E_PeripheralSerializationType
{
    E_PeripheralSerializationType_AnalogInputPeripheral,
    E_PeripheralSerializationType_AnalogOutCurrentPeripheral,
    E_PeripheralSerializationType_DigitalInputsPeripheral,
    E_PeripheralSerializationType_DigitalOutputsPeripheral,
    E_PeripheralSerializationType_Mi3I2CIrPeripheral,
    E_PeripheralSerializationType_DryContactDigitalOutput,
    E_PeripheralSerializationType_InternalTemperatureSensors,
    E_PeripheralSerializationType_PsocAnalogInputsPeripheral,
    E_PeripheralSerializationType_PsocPwmOutputPeripheral,
    E_PeripheralSerializationType_PsocAnalogOutputPeripheral,
    E_PeripheralSerializationType_PsocDigitalOutputPeripheral,
    E_PeripheralSerializationType_PsocTemperaturePeripheral,
    E_PeripheralSerializationType_PsocDigitalInputPeripheral,
    E_PeripheralSerializationType_SwPwmOutputPeripheral,
    E_PeripheralSerializationType_ModbusSmcHrsChiller,
    E_PeripheralSerializationType_Modbus6RTDPeripheral,
    E_PeripheralSerializationType_ModbusInverterCommanderSK,
    E_PeripheralSerializationType_ModbusInverterSchneiderAtv32,
    E_PeripheralSerializationType_ModbusPumaPeripheral,
    E_PeripheralSerializationType_Modbus8TCPeripheral,
    E_PeripheralSerializationType_VirtualPeripheral,
    E_PeripheralSerializationType_ModbusDataCardPeripheral,
    E_PeripheralSerializationType_ModbusInverterUnidriveM200,
    E_PeripheralSerializationType_ModbusInverterPeripheralBase,
    E_PeripheralSerializationType_Mi3Sensor,
    E_PeripheralSerializationType_ModbusPeripheralBase,
};

template<> class Serializer<PeripheralBase> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return -1;}
    void serialize(F_FILE* f, PeripheralBase &p);
    void deserialize(F_FILE* f, PeripheralBase &p);
};

template<> class Serializer<AnalogInputPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_AnalogInputPeripheral;}
    void serialize(F_FILE* f, AnalogInputPeripheral &p);
    void deserialize(F_FILE* f, AnalogInputPeripheral &p);
};

template<> class Serializer<AnalogOutCurrentPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_AnalogOutCurrentPeripheral;}
    void serialize(F_FILE* f, AnalogOutCurrentPeripheral &p);
    void deserialize(F_FILE* f, AnalogOutCurrentPeripheral &p);
};

template<> class Serializer<DigitalInputsPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_DigitalInputsPeripheral;}
    void serialize(F_FILE* f, DigitalInputsPeripheral &p);
    void deserialize(F_FILE* f, DigitalInputsPeripheral &p);
};

template<> class Serializer<DigitalOutputsPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_DigitalOutputsPeripheral;}
    void serialize(F_FILE* f, DigitalOutputsPeripheral &p);
    void deserialize(F_FILE* f, DigitalOutputsPeripheral &p);
};

template<> class Serializer<DryContactDigitalOutput> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_DryContactDigitalOutput;}
    void serialize(F_FILE* f, DryContactDigitalOutput &p);
    void deserialize(F_FILE* f, DryContactDigitalOutput &p);
};

template<> class Serializer<InternalTemperatureSensors> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_InternalTemperatureSensors;}
    void serialize(F_FILE* f, InternalTemperatureSensors &p);
    void deserialize(F_FILE* f, InternalTemperatureSensors &p);
};

template<> class Serializer<SwPwmOutputPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_SwPwmOutputPeripheral;}
    void serialize(F_FILE* f, SwPwmOutputPeripheral &p);
    void deserialize(F_FILE* f, SwPwmOutputPeripheral &p);
};

template<> class Serializer<VirtualPeripheral> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_PeripheralSerializationType_VirtualPeripheral;}
    void serialize(F_FILE* f, VirtualPeripheral &p);
    void deserialize(F_FILE* f, VirtualPeripheral &p);
};





    #endif /* PERIPHERALSERIALIZERS_H_ */
