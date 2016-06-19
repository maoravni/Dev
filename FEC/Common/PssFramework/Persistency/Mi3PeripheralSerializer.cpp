/*
 * Mi3PeripheralSerializer.cpp
 *
 *  Created on: 21 Apr 2016
 *      Author: maora
 */

#include <Persistency/Mi3PeripheralSerializer.h>
#include <Peripherals/Mi3I2CIrPeripheral.h>
#include <Peripherals/Mi3Sensor.h>

void Serializer<Mi3I2CIrPeripheral>::serialize(F_FILE* f, Mi3I2CIrPeripheral& p)
{
    int result;

    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp;
    temp = p.m_resetCount->getElementIndex();

    M_FWRITE_VARIABLE(temp, f);

    temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    Serializer<Mi3Sensor> sensorSerializer;

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        sensorSerializer.serialize(f, *p.m_sensorList[i]);
    }

    updateRecordSize(f);
}

void Serializer<Mi3I2CIrPeripheral>::deserialize(F_FILE* f, Mi3I2CIrPeripheral& p)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t numberOfSensors;

    // read the number of elements:
    M_FREAD_VARIABLE(numberOfSensors, f);

    for (int i = 0; i < numberOfSensors; ++i)
    {
        Mi3Sensor* mi3Sensor = new Mi3Sensor(f);
        p.m_sensorList.push_back(mi3Sensor);
    }
}

void Serializer<Mi3Sensor>::serialize(F_FILE* f, Mi3Sensor& s)
{
    int result;

    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    uint16_t temp = s.m_targTempElement->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);
    M_FWRITE_VARIABLE(s.m_address, f);

    M_FWRITE_VARIABLE(s.m_sensorConfiguration, f);

    // TODO: Serialize Mi3 sensor configuration.
    updateRecordSize(f);
}

void Serializer<Mi3Sensor>::deserialize(F_FILE* f, Mi3Sensor& s)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(s.m_targTempElement, ValidationElementFloat, f);
    M_FREAD_VARIABLE(s.m_address, f);
    M_FREAD_VARIABLE(s.m_sensorConfiguration, f);
}
