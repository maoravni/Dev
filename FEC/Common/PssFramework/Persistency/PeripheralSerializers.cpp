/*
 * PeripheralSerializers.cpp
 *
 *  Created on: 20 Apr 2016
 *      Author: maora
 */

#include <Persistency/PeripheralSerializers.h>
#include <Peripherals/PeripheralBase.h>
#include <Peripherals/AnalogInputPeripheral.h>
#include <peripherals/AnalogOutCurrentPeripheral.h>
#include <Peripherals/DigitalInputsPeripheral.h>
#include <Peripherals/DigitalOutputsPeripheral.h>
#include <Peripherals/DryContactDigitalOutput.h>
#include <peripherals/InternalTemperatureSensors.h>
#include <Peripherals/SwPwmOutputPeripheral.h>
#include <Peripherals/VirtualPeripheral.h>

int Serializer<PeripheralBase>::serialize(F_FILE* f, PeripheralBase& p)
{
    storeStartPosition(f);

    serializeVersion(f);

    if (f_write(&p.m_peripheralRepIndex, sizeof(p.m_peripheralRepIndex), 1, f) == 0)
        return 0;

    if (f_write(&p.m_pssId, sizeof(p.m_pssId), 1, f) == 0)
        return 0;

    if (f_write(&p.m_updateInterval, sizeof(p.m_updateInterval), 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<PeripheralBase>::deserialize(F_FILE* f, PeripheralBase& p)
{
    deserializeRecordSize(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    //TODO: Compare the index assigned from the repository to the one read from the file.
    M_FREAD_VARIABLE(p.m_peripheralRepIndex, f);
    M_FREAD_VARIABLE(p.m_pssId, f);
    M_FREAD_VARIABLE(p.m_updateInterval, f);

    return 1;
}

int Serializer<AnalogInputPeripheral>::serialize(F_FILE* f, AnalogInputPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_inputElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    // store the calibration coefficients:
    if (f_write(p.m_scalingA, sizeof(p.m_scalingA), 1, f) == 0)
        return 0;
    if (f_write(p.m_scalingB, sizeof(p.m_scalingB), 1, f) == 0)
        return 0;

    updateRecordSize(f);

    return 1;
}

int Serializer<AnalogInputPeripheral>::deserialize(F_FILE* f, AnalogInputPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;
    M_FREAD_VARIABLE(temp, f);

    if (temp != p.getElementCount())
        return 0;

    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_inputElementsArray[i] = dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(temp+i));
    }

    M_FREAD_VARIABLE(p.m_scalingA, f);
    M_FREAD_VARIABLE(p.m_scalingB, f);

    return 1;
}

int Serializer<AnalogOutCurrentPeripheral>::serialize(F_FILE* f, AnalogOutCurrentPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_elementArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    updateRecordSize(f);

    return 1;
}

int Serializer<DigitalInputsPeripheral>::serialize(F_FILE* f, DigitalInputsPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_elementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    updateRecordSize(f);

    return 1;
}

int Serializer<DigitalOutputsPeripheral>::serialize(F_FILE* f, DigitalOutputsPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_elementArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    M_FWRITE_VARIABLE(p.m_enabledArray, f);

    updateRecordSize(f);

    return 1;
}

int Serializer<SwPwmOutputPeripheral>::serialize(F_FILE* f, SwPwmOutputPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_dutyCycleElementArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    M_FWRITE_VARIABLE(p.m_channelsEnabled, f);

    // TODO: make sure to reset the currentCounter and the dutyCycleLength.
    M_FWRITE_VARIABLE(p.m_channelStateArray, f);

    updateRecordSize(f);

    return 1;
}

int Serializer<DryContactDigitalOutput>::serialize(F_FILE* f, DryContactDigitalOutput& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_element->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    updateRecordSize(f);

    return 1;
}

int Serializer<InternalTemperatureSensors>::serialize(F_FILE* f, InternalTemperatureSensors& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_temperatureElementsArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    M_FWRITE_VARIABLE(p.m_aCoeff, f);
    M_FWRITE_VARIABLE(p.m_bCoeff, f);

    updateRecordSize(f);

    return 1;
}

int Serializer<VirtualPeripheral>::serialize(F_FILE* f, VirtualPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp = p.getPeripheralType();

    M_FWRITE_VARIABLE(temp, f);

    temp = p.getElementCount();

    // store the number of elements:
    M_FWRITE_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        temp = p.m_elementList[i]->getPssId();
        M_FWRITE_VARIABLE(temp, f);
    }

    updateRecordSize(f);

    return 1;
}
