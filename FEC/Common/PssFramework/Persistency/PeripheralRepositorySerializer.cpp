/*
 * PeripheralRepositorySerializer.cpp
 *
 *  Created on: 20 Apr 2016
 *      Author: maora
 */

#include <Persistency/PeripheralRepositorySerializer.h>
#include <Persistency/PeripheralSerializers.h>
#include "PsocPeripheralSerializers.h"
#include <logger.h>

struct T_InternalPeripheralIndexes
{
    uint16_t m_internalTemperatureSensorsIndex;
    uint16_t m_digitalOutputsIndex;
    uint16_t m_swPwmOutputIndex;
    uint16_t m_dryContactOutputIndex;
    uint16_t m_digitalInputsIndex;
    uint16_t m_analogInputsIndex;
    uint16_t m_analogCurrentOutputsIndex;
    uint16_t m_mi3i2cIrSensorIndex;
};

void Serializer<PeripheralRepository>::serialize(F_FILE* f, PeripheralRepository& pr)
{
    int result;

    storeStartPosition(f);

    serializeVersion(f);

    // go over all elements and count the number of elements that have a PSSID:
    uint16_t numOfPeripherals = 0;

    numOfPeripherals = pr.m_periphVector.size();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing %d/%d Peripherals", numOfPeripherals, pr.m_periphVector.size());

    M_FWRITE_VARIABLE(numOfPeripherals, f);

    // write a placeholder for the map record:
    int mapPos = f_tell(f);
    EntityMapRecord entityMapRecord =
    { 0 };
    for (int i = 0; i < pr.m_periphVector.size(); ++i)
        M_FWRITE_VARIABLE(entityMapRecord, f);

    updateRecordSize(f);

    T_EntityMapRecordVector entityMapVec;

    // serialize each of the peripherals:
    for (int i = 0; i < pr.m_periphVector.size(); ++i)
    {
        // store the position of the elements in the map record.
        entityMapRecord.pssId = pr.m_periphVector[i]->getPssId();
        entityMapRecord.filePos = f_tell(f);
        entityMapVec.push_back(entityMapRecord);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing peripheral {[PSSID:%d]} @ %d", entityMapRecord.pssId,
                entityMapRecord.filePos);

        pr.m_periphVector[i]->serialize(f);
    }

    T_InternalPeripheralIndexes ipi;
    ipi.m_analogCurrentOutputsIndex = pr.m_analogCurrentOutputs->getPeripheralRepIndex();
    ipi.m_analogInputsIndex = pr.m_analogInputs->getPeripheralRepIndex();
    ipi.m_digitalInputsIndex = pr.m_digitalInputs->getPeripheralRepIndex();
    ipi.m_digitalOutputsIndex = pr.m_digitalOutputs->getPeripheralRepIndex();
    ipi.m_dryContactOutputIndex = pr.m_dryContactOutput->getPeripheralRepIndex();
    ipi.m_internalTemperatureSensorsIndex = pr.m_internalTemperatureSensors->getPeripheralRepIndex();
    ipi.m_swPwmOutputIndex = pr.m_swPwmOutput->getPeripheralRepIndex();
    ipi.m_mi3i2cIrSensorIndex = pr.m_mi3i2cIrSensorPeripheral->getPeripheralRepIndex();

    M_FWRITE_VARIABLE(ipi, f);

    assert(entityMapVec.size() == numOfPeripherals);

    if (f_seek(f, mapPos, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";
    for (int i = 0; i < numOfPeripherals; ++i)
    {
        M_FWRITE_VARIABLE(entityMapVec[i], f);
    }

}

void Serializer<PeripheralRepository>::deserialize(F_FILE* f, PeripheralRepository& pr)
{
    // read the record size:
    deserializeRecordSize(f);

    // read the version:
    deserializeVersion(f);

    uint16_t numOfPeripherals;

    M_FREAD_VARIABLE(numOfPeripherals, f);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Reading %d peripherals", numOfPeripherals);

    EntityMapRecord mapRecord;
    T_EntityMapRecordVector mapVec;

    for (int i = 0; i < numOfPeripherals; ++i)
    {
        M_FREAD_VARIABLE(mapRecord, f);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping peripheral {[PSSID:%d]} @ %d", mapRecord.pssId, mapRecord.filePos);

        mapVec.push_back(mapRecord);
    }

    for (int i = 0; i < numOfPeripherals; ++i)
    {
        deserializeNextPeripheral(f, pr);
    }

    T_InternalPeripheralIndexes ipi;

    M_FREAD_VARIABLE(ipi, f);
    pr.m_analogCurrentOutputs = dynamic_cast<AnalogOutCurrentPeripheral*>(pr.getPeripheralByIndex(ipi.m_analogCurrentOutputsIndex));
    pr.m_analogInputs = dynamic_cast<AnalogInputPeripheral*>(pr.getPeripheralByIndex(ipi.m_analogInputsIndex));
    pr.m_digitalInputs = dynamic_cast<DigitalInputsPeripheral*>(pr.getPeripheralByIndex(ipi.m_digitalInputsIndex));
    pr.m_digitalOutputs = dynamic_cast<DigitalOutputsPeripheral*>(pr.getPeripheralByIndex(ipi.m_digitalOutputsIndex));
    pr.m_dryContactOutput = dynamic_cast<DryContactDigitalOutput*>(pr.getPeripheralByIndex(ipi.m_dryContactOutputIndex));
    pr.m_internalTemperatureSensors = dynamic_cast<InternalTemperatureSensors*>(pr.getPeripheralByIndex(ipi.m_internalTemperatureSensorsIndex));
    pr.m_swPwmOutput = dynamic_cast<SwPwmOutputPeripheral*>(pr.getPeripheralByIndex(ipi.m_swPwmOutputIndex));
    pr.m_mi3i2cIrSensorPeripheral = dynamic_cast<Mi3I2CIrPeripheral*>(pr.getPeripheralByIndex(ipi.m_mi3i2cIrSensorIndex));
}

void Serializer<PeripheralRepository>::serializePeripheral(F_FILE* f, PeripheralBase* p)
{
    // read the record size:
    deserializeRecordSize(f);

    // read the version:
    deserializeVersion(f);

    uint16_t numOfPeripherals;

    M_FREAD_VARIABLE(numOfPeripherals, f);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Reading %d peripherals", numOfPeripherals);

    EntityMapRecord mapRecord;
    T_EntityMapRecordVector mapVec;

    for (int i = 0; i < numOfPeripherals; ++i)
    {
        M_FREAD_VARIABLE(mapRecord, f);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping peripheral {[PSSID:%d]} @ %d", mapRecord.pssId, mapRecord.filePos);

        mapVec.push_back(mapRecord);
    }

    // after reading the map record, we can go directly to the position of the peripheral and serialize it:
    int periphIndex = p->getPeripheralRepIndex();
    int periphPosition = mapVec[periphIndex].filePos;

    if (f_seek(f, periphPosition, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";

    p->serialize(f);
}

void Serializer<PeripheralRepository>::deserializeNextPeripheral(F_FILE* f, PeripheralRepository& pr)
{
    int elementStartPosition;
    int currentRecordSize;
    elementStartPosition = f_tell(f);

    currentRecordSize = deserializeRecordSize(f);

    uint8_t classType;
    M_FREAD_VARIABLE(classType, f);

    if (f_seek(f, elementStartPosition, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";

    switch (classType)
    {
    case E_PeripheralSerializationType_AnalogInputPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing AnalogInputPeripheral");
        PeripheralBase* p = new AnalogInputPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_AnalogOutCurrentPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing AnalogOutCurrentPeripheral");
        PeripheralBase *p = new AnalogOutCurrentPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_DigitalInputsPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing DigitalInputsPeripheral");
        PeripheralBase *p = new DigitalInputsPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_DigitalOutputsPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing DigitalOutputsPeripheral");
        PeripheralBase *p = new DigitalOutputsPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_Mi3I2CIrPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing Mi3I2CIrPeripheral");
        PeripheralBase *p = new Mi3I2CIrPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_DryContactDigitalOutput:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing DryContactDigitalOutput");
        PeripheralBase *p = new DryContactDigitalOutput(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_InternalTemperatureSensors:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing InternalTemperatureSensors");
        PeripheralBase *p = new InternalTemperatureSensors(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_SwPwmOutputPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing SwPwmOutputPeripheral");
        PeripheralBase *p = new SwPwmOutputPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_Modbus6RTDPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing Modbus6RTDPeripheral");
        PeripheralBase *p = new Modbus6RTDPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusInverterCommanderSK:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing ModbusInverterCommanderSK");
        InputPeripheralBase *p = new ModbusInverterCommanderSK(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusInverterSchneiderAtv32:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing ModbusInverterSchneiderAtv32");
        ModbusInverterSchneiderAtv32 *p = new ModbusInverterSchneiderAtv32(f);
        PeripheralRepository::getInstance().addPeripheral((InputPeripheralBase*)p);
        break;
    }
    case E_PeripheralSerializationType_ModbusPumaPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing ModbusPumaPeripheral");
        PeripheralBase *p = new ModbusPumaPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_Modbus8TCPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing Modbus8TCPeripheral");
        PeripheralBase *p = new Modbus8TCPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_VirtualPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing VirtualPeripheral");
        PeripheralBase *p = new VirtualPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusDataCardPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing ModbusDataCardPeripheral");
        PeripheralBase *p = new ModbusDataCardPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusInverterUnidriveM200:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing ModbusInverterUnidriveM200");
        InputPeripheralBase *p = new ModbusInverterUnidriveM200(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_PsocAnalogInputsPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing PsocAnalogInputsPeripheral");
        PeripheralBase *p = new PsocAnalogInputsPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_PsocPwmOutputPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing PsocPwmOutputPeripheral");
        PeripheralBase *p = new PsocPwmOutputPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_PsocAnalogOutputPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing PsocAnalogOutputPeripheral");
        PeripheralBase *p = new PsocAnalogOutputPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_PsocDigitalOutputPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing PsocDigitalOutputPeripheral");
        PeripheralBase *p = new PsocDigitalOutputPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_PsocTemperaturePeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing PsocTemperaturePeripheral");
        PeripheralBase *p = new PsocTemperaturePeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_PsocDigitalInputPeripheral:
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing PsocDigitalInputPeripheral");
        PeripheralBase *p = new PsocDigitalInputPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusSmcHrsChiller:
        throw "Bad Serialization";
        break;
    case E_PeripheralSerializationType_ModbusInverterPeripheralBase:
        throw "Bad Serialization";
        break;
    case E_PeripheralSerializationType_Mi3Sensor:
        throw "Bad Serialization";
        break;
    case E_PeripheralSerializationType_ModbusPeripheralBase:
        throw "Bad Serialization";
        break;
    default:
        throw "Bad Serialization";
        break;
    }
}

