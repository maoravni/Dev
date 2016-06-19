/*
 * PsocPeripheralSerializer.cpp
 *
 *  Created on: 25 Apr 2016
 *      Author: maora
 */

#include <Persistency/PsocPeripheralSerializers.h>
#include <PscServer/PscMessageHandler.h>

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

void Serializer<PsocAnalogInputsPeripheral>::deserialize(F_FILE* f, PsocAnalogInputsPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t cableId;
    M_FREAD_VARIABLE(cableId, f);

    PsocHandler* psocHandler = PscMessageHandler::getInstance()->getPsocManager()->getPsocHandlerByCableId(cableId);
    if (psocHandler == NULL)
        throw "CableId Not Found";

    p.m_psocHandler = psocHandler;
    psocHandler->setAnalogInPeripheral(&p);

    uint16_t elementCount;
    M_FREAD_VARIABLE(elementCount, f);

    uint16_t firstElementIndex;
    M_FREAD_VARIABLE(firstElementIndex, f);

    for (int i = 0; i < elementCount; ++i)
    {
        p.m_analogElementsArray[i] =
                dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(
                        firstElementIndex + i));
    }

    M_FREAD_VARIABLE(p.m_aCoeff, f);
    M_FREAD_VARIABLE(p.m_bCoeff, f);
    M_FREAD_VARIABLE(p.m_filterSettleCounts, f);
    M_FREAD_VARIABLE(p.m_lpfCoeff, f);
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

void Serializer<PsocAnalogOutputPeripheral>::deserialize(F_FILE* f, PsocAnalogOutputPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t cableId;
    M_FREAD_VARIABLE(cableId, f);

    PsocHandler* psocHandler = PscMessageHandler::getInstance()->getPsocManager()->getPsocHandlerByCableId(cableId);
    if (psocHandler == NULL)
        throw "CableId Not Found";

    p.m_psocHandler = psocHandler;
    psocHandler->setAnalogOutPeripheral(&p);

    uint16_t elementCount;
    M_FREAD_VARIABLE(elementCount, f);

    uint16_t firstElementIndex;
    M_FREAD_VARIABLE(firstElementIndex, f);

    for (int i = 0; i < elementCount; ++i)
    {
        p.m_elementArray[i] = dynamic_cast<ValidationElementU8*>(ElementRepository::getInstance().getElementByIndex(
                firstElementIndex + i));
    }
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

void Serializer<PsocDigitalInputPeripheral>::deserialize(F_FILE* f, PsocDigitalInputPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t cableId;
    M_FREAD_VARIABLE(cableId, f);

    PsocHandler* psocHandler = PscMessageHandler::getInstance()->getPsocManager()->getPsocHandlerByCableId(cableId);
    if (psocHandler == NULL)
        throw "CableId Not Found";

    p.m_psocHandler = psocHandler;
    psocHandler->setDigitalInPeripheral(&p);

    uint16_t elementCount;
    M_FREAD_VARIABLE(elementCount, f);

    uint16_t firstElementIndex;
    M_FREAD_VARIABLE(firstElementIndex, f);

    for (int i = 0; i < elementCount; ++i)
    {
        p.m_inputElementsArray[i] = dynamic_cast<ElementU8*>(ElementRepository::getInstance().getElementByIndex(
                firstElementIndex + i));
    }
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

void Serializer<PsocDigitalOutputPeripheral>::deserialize(F_FILE* f, PsocDigitalOutputPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t cableId;
    M_FREAD_VARIABLE(cableId, f);

    PsocHandler* psocHandler = PscMessageHandler::getInstance()->getPsocManager()->getPsocHandlerByCableId(cableId);
    if (psocHandler == NULL)
        throw "CableId Not Found";

    p.m_psocHandler = psocHandler;
    psocHandler->setDigitalOutPeripheral(&p);

    uint16_t elementCount;
    M_FREAD_VARIABLE(elementCount, f);

    uint16_t firstElementIndex;
    M_FREAD_VARIABLE(firstElementIndex, f);

    for (int i = 0; i < elementCount; ++i)
    {
        p.m_outputElementArray[i] =
                dynamic_cast<ValidationElementU8*>(ElementRepository::getInstance().getElementByIndex(
                        firstElementIndex + i));
    }
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

void Serializer<PsocTemperaturePeripheral>::deserialize(F_FILE* f, PsocTemperaturePeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t cableId;
    M_FREAD_VARIABLE(cableId, f);

    PsocHandler* psocHandler = PscMessageHandler::getInstance()->getPsocManager()->getPsocHandlerByCableId(cableId);
    if (psocHandler == NULL)
        throw "CableId Not Found";

    p.m_psocHandler = psocHandler;
    psocHandler->setTemperaturePeripheral(&p);

    uint16_t elementCount;
    M_FREAD_VARIABLE(elementCount, f);

    uint16_t firstElementIndex;
    M_FREAD_VARIABLE(firstElementIndex, f);

    for (int i = 0; i < elementCount; ++i)
    {
        p.m_temperatureElementsArray[i] =
                dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(
                        firstElementIndex + i));
    }

    M_FREAD_VARIABLE(p.m_aCoeff, f);
    M_FREAD_VARIABLE(p.m_bCoeff, f);
    M_FREAD_VARIABLE(p.m_hardLimit, f);
    M_FREAD_VARIABLE(p.m_sensorType, f);
    M_FREAD_VARIABLE(p.m_lpfCoeff, f);
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

void Serializer<PsocPwmOutputPeripheral>::deserialize(F_FILE* f, PsocPwmOutputPeripheral& p)
{
    deserializeRecordSize(f);

    deserializeClassType(f);

    deserializeVersion(f);

    Serializer<PeripheralBase> baseS;
    baseS.deserialize(f, p);

    uint16_t cableId;
    M_FREAD_VARIABLE(cableId, f);

    PsocHandler* psocHandler = PscMessageHandler::getInstance()->getPsocManager()->getPsocHandlerByCableId(cableId);
    if (psocHandler == NULL)
        throw "CableId Not Found";

    p.m_psocHandler = psocHandler;
    psocHandler->setPwmPeripheral(&p);

    uint16_t elementCount;
    M_FREAD_VARIABLE(elementCount, f);

    uint16_t firstElementIndex;
    M_FREAD_VARIABLE(firstElementIndex, f);

    for (int i = 0; i < elementCount; ++i)
    {
        p.m_dutyCycleElementArray[i] = dynamic_cast<ValidationElementFloat*>(ElementRepository::getInstance().getElementByIndex(
                firstElementIndex + i));
    }

    M_FREAD_VARIABLE(p.m_pwmChannelType, f);
    M_FREAD_VARIABLE(p.m_pwmChannelRampParameters, f);

}
