/*
 * PeripheralRepositorySerializer.cpp
 *
 *  Created on: 20 Apr 2016
 *      Author: maora
 */

#include <Persistency/PeripheralRepositorySerializer.h>
#include <Persistency/PeripheralSerializers.h>
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

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing peripheral %d @ %d", entityMapRecord.pssId,
                entityMapRecord.filePos);

        pr.m_periphVector[i]->serialize(f);
    }

    assert(entityMapVec.size() == numOfPeripherals);

    if (f_seek(f, mapPos, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";
    for (int i = 0; i < numOfPeripherals; ++i)
    {
        M_FWRITE_VARIABLE(entityMapVec[i], f);
    }

    T_InternalPeripheralIndexes ipi;
    ipi.m_analogCurrentOutputsIndex = pr.m_analogCurrentOutputs->getPeripheralRepIndex();
    ipi.m_analogInputsIndex = pr.m_analogInputs->getPeripheralRepIndex();
    ipi.m_digitalInputsIndex = pr.m_digitalInputs->getPeripheralRepIndex();
    ipi.m_digitalOutputsIndex = pr.m_digitalOutputs->getPeripheralRepIndex();
    ipi.m_dryContactOutputIndex = pr.m_dryContactOutput->getPeripheralRepIndex();
    ipi.m_internalTemperatureSensorsIndex = pr.m_internalTemperatureSensors->getPeripheralRepIndex();
    ipi.m_swPwmOutputIndex = pr.m_swPwmOutput->getPeripheralRepIndex();

    M_FWRITE_VARIABLE(ipi, f);
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

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping peripheral %d @ %d", mapRecord.pssId, mapRecord.filePos);

        mapVec.push_back(mapRecord);
    }

    for (int i = 0; i < numOfPeripherals; ++i)
    {
        deserializePeripheral(f, pr);
    }
}

void Serializer<PeripheralRepository>::deserializePeripheral(F_FILE* f, PeripheralRepository& pr)
{
    int elementStartPosition;
    elementStartPosition = f_tell(f);

    deserializeRecordSize(f);

    uint8_t classType = deserializeClassType(f);

    if (f_seek(f, elementStartPosition, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";

    switch (classType)
    {
    case E_PeripheralSerializationType_AnalogInputPeripheral:
    {
        PeripheralBase* p = new AnalogInputPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_AnalogOutCurrentPeripheral:
    {
        PeripheralBase *p = new AnalogOutCurrentPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_DigitalInputsPeripheral:
    {
        PeripheralBase *p = new DigitalInputsPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_DigitalOutputsPeripheral:
    {
        PeripheralBase *p = new DigitalOutputsPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_Mi3I2CIrPeripheral:
    {
        PeripheralBase *p = new Mi3I2CIrPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_DryContactDigitalOutput:
    {
        PeripheralBase *p = new DryContactDigitalOutput(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_InternalTemperatureSensors:
    {
        PeripheralBase *p = new InternalTemperatureSensors(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_SwPwmOutputPeripheral:
    {
        PeripheralBase *p = new SwPwmOutputPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_Modbus6RTDPeripheral:
    {
        PeripheralBase *p = new Modbus6RTDPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusInverterCommanderSK:
    {
        InputPeripheralBase *p = new ModbusInverterCommanderSK(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusInverterSchneiderAtv32:
    {
        InputPeripheralBase *p = new ModbusInverterSchneiderAtv32(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusPumaPeripheral:
    {
        PeripheralBase *p = new ModbusPumaPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_Modbus8TCPeripheral:
    {
        PeripheralBase *p = new Modbus8TCPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_VirtualPeripheral:
    {
        PeripheralBase *p = new VirtualPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusDataCardPeripheral:
    {
        PeripheralBase *p = new ModbusDataCardPeripheral(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_ModbusInverterUnidriveM200:
    {
        InputPeripheralBase *p = new ModbusInverterUnidriveM200(f);
        PeripheralRepository::getInstance().addPeripheral(p);
        break;
    }
    case E_PeripheralSerializationType_PsocAnalogInputsPeripheral:
    case E_PeripheralSerializationType_PsocPwmOutputPeripheral:
    case E_PeripheralSerializationType_PsocAnalogOutputPeripheral:
    case E_PeripheralSerializationType_PsocDigitalOutputPeripheral:
    case E_PeripheralSerializationType_PsocTemperaturePeripheral:
    case E_PeripheralSerializationType_PsocDigitalInputPeripheral:
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

