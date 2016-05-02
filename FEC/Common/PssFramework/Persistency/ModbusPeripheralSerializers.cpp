/*
 * ModbusPeripheralSerializers.cpp
 *
 *  Created on: 21 Apr 2016
 *      Author: maora
 */

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

int Serializer<ModbusInverterPeripheralBase>::serialize(F_FILE* f, ModbusInverterPeripheralBase& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<ModbusPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusPeripheralBase*>(&p)));

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

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<ModbusPeripheralBase>::serialize(F_FILE* f, ModbusPeripheralBase& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    M_FWRITE_VARIABLE(p.m_slaveId, f);

    uint16_t temp = p.m_generalModbusSetupVector.size();
    M_FWRITE_VARIABLE(temp, f);

    for (int i = 0; i < temp; ++i)
        M_FWRITE_VARIABLE(p.m_generalModbusSetupVector[i], f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}


int Serializer<ModbusInverterSchneiderAtv32>::serialize(F_FILE* f, ModbusInverterSchneiderAtv32& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusInverterPeripheralBase*>(&p)));

    M_FWRITE_VARIABLE(p.m_maxSpeed, f);
    M_FWRITE_VARIABLE(p.m_minSpeed, f);
    M_FWRITE_VARIABLE(p.m_accelRate, f);
    M_FWRITE_VARIABLE(p.m_decelRate, f);
    M_FWRITE_VARIABLE(p.m_nominalFrequency, f);
    M_FWRITE_VARIABLE(p.m_nominalCurrent, f);
    M_FWRITE_VARIABLE(p.m_nominalRpm, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<ModbusInverterCommanderSK>::serialize(F_FILE* f, ModbusInverterCommanderSK& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusInverterPeripheralBase*>(&p)));

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<ModbusInverterUnidriveM200>::serialize(F_FILE* f, ModbusInverterUnidriveM200& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<ModbusInverterPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusInverterPeripheralBase*>(&p)));

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<Modbus6RTDPeripheral>::serialize(F_FILE* f, Modbus6RTDPeripheral& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<ModbusPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusPeripheralBase*>(&p)));

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_temperatureElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<ModbusPumaPeripheral>::serialize(F_FILE* f, ModbusPumaPeripheral& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<ModbusPeripheralBase> baseS;
    baseS.serialize(f, *(dynamic_cast<ModbusPeripheralBase*>(&p)));

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_temperatureElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<Modbus8TCPeripheral>::serialize(F_FILE* f, Modbus8TCPeripheral& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

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

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<ModbusDataCardPeripheral>::serialize(F_FILE* f, ModbusDataCardPeripheral& p)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

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

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

