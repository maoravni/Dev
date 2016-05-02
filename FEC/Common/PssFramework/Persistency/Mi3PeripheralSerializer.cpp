/*
 * Mi3PeripheralSerializer.cpp
 *
 *  Created on: 21 Apr 2016
 *      Author: maora
 */

#include <Persistency/Mi3PeripheralSerializer.h>
#include <Peripherals/Mi3I2CIrPeripheral.h>
#include <Peripherals/Mi3Sensor.h>

int Serializer<Mi3I2CIrPeripheral>::serialize(F_FILE* f, Mi3I2CIrPeripheral& p)
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

    return 1;
}

int Serializer<Mi3Sensor>::serialize(F_FILE* f, Mi3Sensor& s)
{
    int result;

    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    uint16_t temp = s.m_targTempElement->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);
    M_FWRITE_VARIABLE(s.m_address, f);

    // TODO: Serialize Mi3 sensor configuration.
    updateRecordSize(f);

    return 1;
}
