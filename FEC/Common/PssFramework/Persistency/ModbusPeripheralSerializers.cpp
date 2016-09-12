/*
 * ModbusPeripheralSerializers.cpp
 *
 *  Created on: 21 Apr 2016
 *      Author: maora
 */

#include "ModbusPeripheralSerializers.h"
#include <Peripherals/ModbusPeripheralBase.h>
#include <Peripherals/ModbusInverterSchneiderAtv32.h>
#include <Peripherals/ModbusInverterPeripheralBase.h>
#include <Persistency/ModbusPeripheralSerializers.h>
#include <peripherals/Modbus6RTDPeripheral.h>
#include <Peripherals/Modbus8TCPeripheral.h>
#include <Peripherals/ModbusDataCardPeripheral.h>
#include <Peripherals/ModbusPumaPeripheral.h>
#include <Peripherals/ModbusInverterCommanderSK.h>
#include <Peripherals/ModbusInverterUnidriveM200.h>

void Serializer<ModbusInverterPeripheralBase>::serialize(F_FILE* f, ModbusInverterPeripheralBase& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusPeripheralBase*>(&p)));

    Serializer<PeripheralBase> pbS;
    pbS.serialize(f,
            *(dynamic_cast<PeripheralBase*>(dynamic_cast<InputPeripheralBase*>(dynamic_cast<InputOutputPeripheralBase*>(dynamic_cast<ModbusInputOutputPeripheralBase*>(&p))))));

    uint16_t temp = p.m_outputFrequency->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.m_outputCurrent->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.m_frequencySetpoint->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.m_outputEnable->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    M_FWRITE_VARIABLE(p.m_inverterType, f);
    M_FWRITE_VARIABLE(p.m_currentMultiplier, f);
    M_FWRITE_VARIABLE(p.m_frequencyMultiplier, f);
    M_FWRITE_VARIABLE(p.m_setpointMultiplier, f);

    updateRecordSize(f);
}

void Serializer<ModbusInverterPeripheralBase>::deserialize(F_FILE* f, ModbusInverterPeripheralBase& p)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.deserialize(f, p);

    Serializer<PeripheralBase> pbS;
    pbS.deserialize(f,
            *(dynamic_cast<PeripheralBase*>(dynamic_cast<InputPeripheralBase*>(dynamic_cast<InputOutputPeripheralBase*>(dynamic_cast<ModbusInputOutputPeripheralBase*>(&p))))));
    uint16_t temp;

    // read the index of the output frequency.
    M_FREAD_VARIABLE(temp, f);
    p.m_outputFrequency = ElementRepository::getInstance().getElementByIndex(temp);

    M_FREAD_VARIABLE(temp, f);
    p.m_outputCurrent = ElementRepository::getInstance().getElementByIndex(temp);

    M_FREAD_VARIABLE(temp, f);
    p.m_frequencySetpoint = ElementRepository::getInstance().getElementByIndex(temp);
    p.m_frequencySetpoint->addObserver(&p);

    M_FREAD_VARIABLE(temp, f);
    p.m_outputEnable = ElementRepository::getInstance().getElementByIndex(temp);

    M_FREAD_VARIABLE(p.m_inverterType, f);
    M_FREAD_VARIABLE(p.m_currentMultiplier, f);
    M_FREAD_VARIABLE(p.m_frequencyMultiplier, f);
    M_FREAD_VARIABLE(p.m_setpointMultiplier, f);
}

void Serializer<ModbusPeripheralBase>::serialize(F_FILE* f, ModbusPeripheralBase& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    M_FWRITE_VARIABLE(p.m_slaveId, f);

    uint16_t temp = p.m_generalModbusSetupVector.size();
    M_FWRITE_VARIABLE(temp, f);

    for (int i = 0; i < temp; ++i)
        M_FWRITE_VARIABLE(p.m_generalModbusSetupVector[i], f);

    updateRecordSize(f);
}

void Serializer<ModbusPeripheralBase>::deserialize(F_FILE* f, ModbusPeripheralBase& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    M_FREAD_VARIABLE(p.m_slaveId, f);

    uint16_t numOfRegs;
    M_FREAD_VARIABLE(numOfRegs, f);

    T_ModbusRequestDataStruct data;
    for (int i = 0; i < numOfRegs; ++i)
    {
        M_FREAD_VARIABLE(data, f);
        p.m_generalModbusSetupVector.push_back(data);
    }

}

void Serializer<ModbusInverterSchneiderAtv32>::serialize(F_FILE* f, ModbusInverterSchneiderAtv32& p)
{

    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusInverterPeripheralBase*>(&p)));

    M_FWRITE_VARIABLE(p.m_maxSpeed, f);
    M_FWRITE_VARIABLE(p.m_minSpeed, f);
    M_FWRITE_VARIABLE(p.m_accelRate, f);
    M_FWRITE_VARIABLE(p.m_decelRate, f);
    M_FWRITE_VARIABLE(p.m_nominalFrequency, f);
    M_FWRITE_VARIABLE(p.m_nominalCurrent, f);
    M_FWRITE_VARIABLE(p.m_nominalRpm, f);

    M_FWRITE_ELEMENT_REFERENCE(p.m_driveStatus, f);

    updateRecordSize(f);
}

void Serializer<ModbusInverterSchneiderAtv32>::deserialize(F_FILE* f, ModbusInverterSchneiderAtv32& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.deserialize(f, p);

    M_FREAD_VARIABLE(p.m_maxSpeed, f);
    M_FREAD_VARIABLE(p.m_minSpeed, f);
    M_FREAD_VARIABLE(p.m_accelRate, f);
    M_FREAD_VARIABLE(p.m_decelRate, f);
    M_FREAD_VARIABLE(p.m_nominalFrequency, f);
    M_FREAD_VARIABLE(p.m_nominalCurrent, f);
    M_FREAD_VARIABLE(p.m_nominalRpm, f);

    M_FREAD_AND_REFERENCE_ELEMENT(p.m_driveStatus, f);

}

void Serializer<ModbusInverterCommanderSK>::serialize(F_FILE* f, ModbusInverterCommanderSK& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusInverterPeripheralBase*>(&p)));

    updateRecordSize(f);
}

void Serializer<ModbusInverterCommanderSK>::deserialize(F_FILE* f, ModbusInverterCommanderSK& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.deserialize(f, p);
}

void Serializer<ModbusInverterUnidriveM200>::serialize(F_FILE* f, ModbusInverterUnidriveM200& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusInverterPeripheralBase*>(&p)));

    M_FWRITE_ELEMENT_REFERENCE(p.m_driveStatus, f);

    updateRecordSize(f);
}

void Serializer<ModbusInverterUnidriveM200>::deserialize(F_FILE* f, ModbusInverterUnidriveM200& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.deserialize(f, p);

    M_FREAD_AND_REFERENCE_ELEMENT(p.m_driveStatus, f);
}

void Serializer<Modbus6RTDPeripheral>::serialize(F_FILE* f, Modbus6RTDPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusPeripheralBase*>(&p)));

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_temperatureElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    updateRecordSize(f);

}

void Serializer<Modbus6RTDPeripheral>::deserialize(F_FILE* f, Modbus6RTDPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_temperatureElementsArray[i] =
                dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(temp + i));
    }

}

void Serializer<ModbusPumaPeripheral>::serialize(F_FILE* f, ModbusPumaPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusPeripheralBase*>(&p)));

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_temperatureElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    updateRecordSize(f);
}

void Serializer<ModbusPumaPeripheral>::deserialize(F_FILE* f, ModbusPumaPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_temperatureElementsArray[i] =
                dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(temp + i));
    }

}

void Serializer<Modbus8TCPeripheral>::serialize(F_FILE* f, Modbus8TCPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusPeripheralBase*>(&p)));

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_temperatureElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    M_FWRITE_VARIABLE(p.m_aCoeff, f);
    M_FWRITE_VARIABLE(p.m_bCoeff, f);

    updateRecordSize(f);

}

void Serializer<Modbus8TCPeripheral>::deserialize(F_FILE* f, Modbus8TCPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_temperatureElementsArray[i] =
                dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(temp + i));
    }

    M_FREAD_VARIABLE(p.m_aCoeff, f);
    M_FREAD_VARIABLE(p.m_bCoeff, f);
}

void Serializer<ModbusDataCardPeripheral>::serialize(F_FILE* f, ModbusDataCardPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusPeripheralBase*>(&p)));

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_temperatureElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    M_FWRITE_VARIABLE(p.m_aCoeff, f);
    M_FWRITE_VARIABLE(p.m_bCoeff, f);

    updateRecordSize(f);

}

void Serializer<ModbusDataCardPeripheral>::deserialize(F_FILE* f, ModbusDataCardPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<ModbusPeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_temperatureElementsArray[i] =
                dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(temp + i));
    }

    M_FREAD_VARIABLE(p.m_aCoeff, f);
    M_FREAD_VARIABLE(p.m_bCoeff, f);
}
