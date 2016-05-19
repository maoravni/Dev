/*
 * ModBus8TCPeripheral.cpp
 *
 *  Created on: 30 Jun 2013
 *      Author: maora
 */

#include "Modbus8TCPeripheral.h"
#include <logger.h>
#include "Persistency/ModbusPeripheralSerializers.h"


Modbus8TCPeripheral::Modbus8TCPeripheral(uint8_t slaveId) :
        ModbusAnalogInputPeripheralBase(slaveId)
{
//    m_startAddress = 0;

    for (int i = 0; i < M_8TC_NUM_OF_SENSORS; ++i)
    {
//        ElementRepository::getInstance().addElement(&m_temperatureElementsArray[i]);
        m_temperatureElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_aCoeff[i] = 1;
        m_bCoeff[i] = 0;
    }

    setUpdateInterval(M_8TC_SAMPLE_INTERVAL);
}

Modbus8TCPeripheral::~Modbus8TCPeripheral()
{
}

void Modbus8TCPeripheral::readInputs()
{
    // buffer should contain one additional value - the sensor state.
    static int16_t buffer[M_8TC_NUM_OF_SENSORS + 2];
    uint32_t bufferLength;
    int i;

    // read the temperature sensors
    E_ModbusError error = readHoldingRegs(getSlaveId(), M_8TC_REG_START_ADDRESS, M_8TC_NUM_OF_SENSORS + 2,
            (uint8_t*) buffer, bufferLength);

    if (error != E_ModbusError_Ok)
    {
        if (isTimeoutExpired())
        {
            for (i = 0; i < M_8TC_NUM_OF_SENSORS; ++i)
                m_temperatureElementsArray[i]->setValueValid(false);
        }
        return;
    }

    uint16_t sensorConnectedMask = SWAP_16(buffer[M_8TC_NUM_OF_SENSORS+1]);

    for (i = 0; i < M_8TC_NUM_OF_SENSORS; ++i)
    {
        buffer[i] = SWAP_16(buffer[i]);
        m_temperatureElementsArray[i]->setValue(((float) (buffer[i] * 0.1)) * m_aCoeff[i] + m_bCoeff[i]);
        m_temperatureElementsArray[i]->setValueValid(((sensorConnectedMask & 1) == 0));
        sensorConnectedMask >>= 2;
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "8TC sensors: %0.2f not connected: %d", m_temperatureElementsArray[i]->getValue(), m_temperatureElementsArray[i]->isValid());
    }

}

//void Modbus8TCPeripheral::updateOutputs()
//{
//}

//void Modbus8TCPeripheral::updateNotification(ElementBase* element)
//{
//}

ElementBase* Modbus8TCPeripheral::getElementByIndex(int index)
{
    if (index >= M_8TC_NUM_OF_SENSORS)
        return NULL;

    return m_temperatureElementsArray[index];
}

void Modbus8TCPeripheral::enableElementByIndex(int index, bool enable)
{
}

void Modbus8TCPeripheral::setCalibrationCoeff(int index, float aCoeff, float bCoeff)
{
    m_aCoeff[index] = aCoeff;
    m_bCoeff[index] = bCoeff;
}

ElementBase* Modbus8TCPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_8TC_NUM_OF_SENSORS; ++i)
    {
        if (m_temperatureElementsArray[i]->getPssId() == pssId)
            return m_temperatureElementsArray[i];
    }
    return NULL;
}

void Modbus8TCPeripheral::setScalingCoeff(int index, float aCoeff, float bCoeff)
{
}

Modbus8TCPeripheral::Modbus8TCPeripheral(F_FILE* f) : ModbusAnalogInputPeripheralBase(0)
{
    Serializer<Modbus8TCPeripheral> s;
    s.deserialize(f, *this);
}

void Modbus8TCPeripheral::serialize(F_FILE* f)
{
    Serializer<Modbus8TCPeripheral> s;
    s.serialize(f, *this);
}
