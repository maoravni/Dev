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

void Serializer<PeripheralBase>::serialize(F_FILE* f, PeripheralBase& p)
{
    storeStartPosition(f);

    serializeVersion(f);

    M_FWRITE_VARIABLE(p.m_peripheralRepIndex, f);
    M_FWRITE_VARIABLE(p.m_pssId, f);
    M_FWRITE_VARIABLE(p.m_updateInterval, f);

    updateRecordSize(f);
}

void Serializer<PeripheralBase>::deserialize(F_FILE* f, PeripheralBase& p)
{
    deserializeRecordSize(f);

    deserializeVersion(f);

//    Serializer<PeripheralBase> baseS;
//    baseS.deserialize(f, p);

    //TODO: Compare the index assigned from the repository to the one read from the file.
    M_FREAD_VARIABLE(p.m_peripheralRepIndex, f);
    M_FREAD_VARIABLE(p.m_pssId, f);
    M_FREAD_VARIABLE(p.m_updateInterval, f);
}

void Serializer<AnalogInputPeripheral>::serialize(F_FILE* f, AnalogInputPeripheral& p)
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
    M_FWRITE_VARIABLE(p.m_scalingA, f);
    M_FWRITE_VARIABLE(p.m_scalingB, f);

    updateRecordSize(f);
}

void Serializer<AnalogInputPeripheral>::deserialize(F_FILE* f, AnalogInputPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;
    M_FREAD_VARIABLE(temp, f);

    if (temp != p.getElementCount())
        throw "Bad Deserialization";

    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_inputElementsArray[i] =
                dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(temp + i));
    }

    M_FREAD_VARIABLE(p.m_scalingA, f);
    M_FREAD_VARIABLE(p.m_scalingB, f);
}

void Serializer<AnalogOutCurrentPeripheral>::serialize(F_FILE* f, AnalogOutCurrentPeripheral& p)
{
    storeStartPosition(f);

    serializeClassType(f);

    serializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.serialize(f, p);

    uint16_t temp;

    // store the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // store the first index of the first element:
    temp = p.m_elementArray[0]->getElementIndex();
    M_FWRITE_VARIABLE(temp, f);

    updateRecordSize(f);
}

void Serializer<AnalogOutCurrentPeripheral>::deserialize(F_FILE* f, AnalogOutCurrentPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_elementArray[i] = dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(
                temp + i));
    }
}

void Serializer<DigitalInputsPeripheral>::serialize(F_FILE* f, DigitalInputsPeripheral& p)
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
}

void Serializer<DigitalInputsPeripheral>::deserialize(F_FILE* f, DigitalInputsPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_elementsArray[i] = dynamic_cast<ElementU8*>(ElementRepository::getInstance().getElementByIndex(temp + i));
    }
}

void Serializer<DigitalOutputsPeripheral>::serialize(F_FILE* f, DigitalOutputsPeripheral& p)
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
}

void Serializer<DigitalOutputsPeripheral>::deserialize(F_FILE* f, DigitalOutputsPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_elementArray[i] = dynamic_cast<ElementU8*>(ElementRepository::getInstance().getElementByIndex(temp + i));
    }

    M_FREAD_VARIABLE(p.m_enabledArray, f);
}

void Serializer<SwPwmOutputPeripheral>::serialize(F_FILE* f, SwPwmOutputPeripheral& p)
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
}

void Serializer<DryContactDigitalOutput>::serialize(F_FILE* f, DryContactDigitalOutput& p)
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
}

void Serializer<DryContactDigitalOutput>::deserialize(F_FILE* f, DryContactDigitalOutput& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    p.m_element = dynamic_cast<ElementU8*>(ElementRepository::getInstance().getElementByIndex(temp));
}

void Serializer<InternalTemperatureSensors>::serialize(F_FILE* f, InternalTemperatureSensors& p)
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
}

void Serializer<InternalTemperatureSensors>::deserialize(F_FILE* f, InternalTemperatureSensors& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_temperatureElementsArray[i] =
                dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(temp + i));
    }

    M_FREAD_VARIABLE(p.m_aCoeff, f);
    M_FREAD_VARIABLE(p.m_bCoeff, f);
}

void Serializer<VirtualPeripheral>::serialize(F_FILE* f, VirtualPeripheral& p)
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
        temp = p.m_elementList[i]->getElementIndex();
        M_FWRITE_VARIABLE(temp, f);
    }

    updateRecordSize(f);
}

void Serializer<VirtualPeripheral>::deserialize(F_FILE* f, VirtualPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t temp;

    // read the peripheral type:
    M_FREAD_VARIABLE(temp, f);

    p.m_peripheralType = (E_PeripheralType)temp;

    // read the number of elements:
    M_FREAD_VARIABLE(temp, f);

    // TODO: check that the read number of elements is the same as the peripheral.

    // read first element index:
    M_FREAD_VARIABLE(temp, f);

    for (int i = 0; i < p.getElementCount(); ++i)
    {
        p.m_elementList[i] = ElementRepository::getInstance().getElementByIndex(temp + i);
    }
}
