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

int Serializer<PeripheralRepository>::serialize(F_FILE* f, PeripheralRepository& pr)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    // go over all elements and count the number of elements that have a PSSID:
    uint16_t numOfPeripherals = 0;

    numOfPeripherals = pr.m_periphVector.size();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing %d/%d Peripherals", numOfPeripherals, pr.m_periphVector.size());

    result = f_write(&numOfPeripherals, sizeof(numOfPeripherals), 1, f);
    if (result != 1)
        return 0;

    // write a placeholder for the map record:
    int mapPos = f_tell(f);
    EntityMapRecord entityMapRecord =
    { 0 };
    result = f_write(&entityMapRecord, sizeof(entityMapRecord), numOfPeripherals, f);
    if (result != numOfPeripherals)
        return 0;

    if (updateRecordSize(f) == 0)
        return 0;

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

        int recordLength = pr.m_periphVector[i]->serialize(f);
    }

    assert(entityMapVec.size() == numOfPeripherals);

    if (f_seek(f, mapPos, F_SEEK_SET) != F_NO_ERROR)
        return 0;
    for (int i = 0; i < numOfPeripherals; ++i)
    {
        if (f_write(&entityMapVec[i], sizeof(entityMapRecord), 1, f) == 0)
            return 0;
    }

    T_InternalPeripheralIndexes ipi;
    ipi.m_analogCurrentOutputsIndex = pr.m_analogCurrentOutputs->getPeripheralRepIndex();
    ipi.m_analogInputsIndex = pr.m_analogInputs->getPeripheralRepIndex();
    ipi.m_digitalInputsIndex = pr.m_digitalInputs->getPeripheralRepIndex();
    ipi.m_digitalOutputsIndex = pr.m_digitalOutputs->getPeripheralRepIndex();
    ipi.m_dryContactOutputIndex = pr.m_dryContactOutput->getPeripheralRepIndex();
    ipi.m_internalTemperatureSensorsIndex = pr.m_internalTemperatureSensors->getPeripheralRepIndex();
    ipi.m_swPwmOutputIndex = pr.m_swPwmOutput->getPeripheralRepIndex();

    if (f_write(&ipi, sizeof(ipi), 1, f) == 0)
        return 0;

    return 1;
}

int Serializer<PeripheralRepository>::deserialize(F_FILE* f, PeripheralRepository& pr)
{
    // read the record size:
    uint16_t recordSize;
    if (deserializeRecordSize(f, recordSize) == 0)
        return 0;

    // read the version:
    if (deserializeVersion(f) == 0)
        return 0;

    uint16_t numOfPeripherals;

    M_FREAD_VARIABLE(numOfPeripherals, f);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Reading %d peripherals", numOfPeripherals);

    EntityMapRecord mapRecord;
    T_EntityMapRecordVector mapVec;

    for (int i = 0; i < numOfPeripherals; ++i)
    {
        if (f_read(&mapRecord, sizeof(mapRecord), 1, f) == 0)
            return 0;

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping peripheral %d @ %d", mapRecord.pssId, mapRecord.filePos);

        mapVec.push_back(mapRecord);
    }

    for (int i = 0; i < numOfPeripherals; ++i)
    {
        if (deserializePeripheral(f, pr) == 0)
            return 0;
    }

    return 1;
}

int Serializer<PeripheralRepository>::deserializePeripheral(F_FILE* f, PeripheralRepository& pr)
{
    int elementStartPosition;
    uint16_t elementRecordSize;
    elementStartPosition = f_tell(f);

    if (deserializeRecordSize(f, elementRecordSize) == 0)
        return 0;

    uint8_t classType;
    if (deserializeClassType(f, classType) == 0)
        return 0;

    if (f_seek(f, elementStartPosition, F_SEEK_SET) != F_NO_ERROR)
        return 0;

    switch (classType)
    {
        case E_PeripheralSerializationType_AnalogInputPeripheral:
        {
            AnalogInputPeripheral* p = new AnalogInputPeripheral(f);
            break;
        }
        case E_PeripheralSerializationType_AnalogOutCurrentPeripheral:
        case E_PeripheralSerializationType_DigitalInputsPeripheral:
        case E_PeripheralSerializationType_DigitalOutputsPeripheral:
        case E_PeripheralSerializationType_Mi3I2CIrPeripheral:
        case E_PeripheralSerializationType_DryContactDigitalOutput:
        case E_PeripheralSerializationType_InternalTemperatureSensors:
        case E_PeripheralSerializationType_PsocAnalogInputsPeripheral:
        case E_PeripheralSerializationType_PsocPwmOutputPeripheral:
        case E_PeripheralSerializationType_PsocAnalogOutputPeripheral:
        case E_PeripheralSerializationType_PsocDigitalOutputPeripheral:
        case E_PeripheralSerializationType_PsocTemperaturePeripheral:
        case E_PeripheralSerializationType_PsocDigitalInputPeripheral:
        case E_PeripheralSerializationType_SwPwmOutputPeripheral:
        case E_PeripheralSerializationType_ModbusSmcHrsChiller:
        case E_PeripheralSerializationType_Modbus6RTDPeripheral:
        case E_PeripheralSerializationType_ModbusInverterCommanderSK:
        case E_PeripheralSerializationType_ModbusInverterSchneiderAtv32:
        case E_PeripheralSerializationType_ModbusPumaPeripheral:
        case E_PeripheralSerializationType_Modbus8TCPeripheral:
        case E_PeripheralSerializationType_VirtualPeripheral:
        case E_PeripheralSerializationType_ModbusDataCardPeripheral:
        case E_PeripheralSerializationType_ModbusInverterUnidriveM200:
        case E_PeripheralSerializationType_ModbusInverterPeripheralBase:
        case E_PeripheralSerializationType_Mi3Sensor:
        case E_PeripheralSerializationType_ModbusPeripheralBase:
    default:
        printf("bad parsing on the deserialization file\n");
    }
    return 1;
}

