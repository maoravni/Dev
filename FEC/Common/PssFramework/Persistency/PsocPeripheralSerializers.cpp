/*
 * PsocPeripheralSerializer.cpp
 *
 *  Created on: 25 Apr 2016
 *      Author: maora
 */

#include <Persistency/PsocPeripheralSerializers.h>

int Serializer<PsocAnalogInputsPeripheral>::serialize(F_FILE* f, PsocAnalogInputsPeripheral& p)
{
    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.m_psocHandler->getCableID();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_analogElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    // store the calibration coefficients:
    M_FWRITE_VARIABLE(p.m_aCoeff, f);
    M_FWRITE_VARIABLE(p.m_bCoeff, f);
    M_FWRITE_VARIABLE(p.m_filterSettleCounts, f);
    M_FWRITE_VARIABLE(p.m_lpfCoeff, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<PsocAnalogOutputPeripheral>::serialize(F_FILE* f, PsocAnalogOutputPeripheral& p)
{
    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.m_psocHandler->getCableID();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_elementArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<PsocDigitalInputPeripheral>::serialize(F_FILE* f, PsocDigitalInputPeripheral& p)
{
    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.m_psocHandler->getCableID();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_inputElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<PsocDigitalOutputPeripheral>::serialize(F_FILE* f, PsocDigitalOutputPeripheral& p)
{
    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.m_psocHandler->getCableID();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_outputElementArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<PsocTemperaturePeripheral>::serialize(F_FILE* f, PsocTemperaturePeripheral& p)
{
    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.m_psocHandler->getCableID();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_temperatureElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    M_FWRITE_VARIABLE(p.m_aCoeff, f);
    M_FWRITE_VARIABLE(p.m_bCoeff, f);
    M_FWRITE_VARIABLE(p.m_hardLimit, f);
    M_FWRITE_VARIABLE(p.m_sensorType, f);
    M_FWRITE_VARIABLE(p.m_lpfCoeff, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

int Serializer<PsocPwmOutputPeripheral>::serialize(F_FILE* f, PsocPwmOutputPeripheral& p)
{
    if (storeStartPosition(f) == 0)
        return 0;

    if (serializeClassType(f) == 0)
        return 0;

    if (serializeVersion(f) == 0)
        return 0;

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.m_psocHandler->getCableID();
    M_FWRITE_VARIABLE(temp, f);

    temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_dutyCycleElementArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    M_FWRITE_VARIABLE(p.m_pwmChannelType, f);
    M_FWRITE_VARIABLE(p.m_pwmChannelRampParameters, f);

    if (updateRecordSize(f) == 0)
        return 0;

    return 1;
}

