/*
 * PsocPeripheralSerializer.cpp
 *
 *  Created on: 25 Apr 2016
 *      Author: maora
 */

#include <Persistency/PsocPeripheralSerializers.h>

void Serializer<PsocAnalogInputsPeripheral>::serialize(F_FILE* f, PsocAnalogInputsPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

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

    updateRecordSize(f);
}

void Serializer<PsocAnalogOutputPeripheral>::serialize(F_FILE* f, PsocAnalogOutputPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

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

    updateRecordSize(f);
}

void Serializer<PsocDigitalInputPeripheral>::serialize(F_FILE* f, PsocDigitalInputPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

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

    updateRecordSize(f);
}

void Serializer<PsocDigitalOutputPeripheral>::serialize(F_FILE* f, PsocDigitalOutputPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

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

    updateRecordSize(f);
}

void Serializer<PsocTemperaturePeripheral>::serialize(F_FILE* f, PsocTemperaturePeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

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

    updateRecordSize(f);
}

void Serializer<PsocPwmOutputPeripheral>::serialize(F_FILE* f, PsocPwmOutputPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

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

    updateRecordSize(f);
}

