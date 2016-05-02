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

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

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

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<Mi3Sensor>::serialize(F_FILE* f, Mi3Sensor& s)
{
    int result;

    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    uint16_t temp = s.m_targTempElement->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);
    M_FWRITE_VARIABLE(s.m_address, f);

    // TODO: Serialize Mi3 sensor configuration.
    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}
