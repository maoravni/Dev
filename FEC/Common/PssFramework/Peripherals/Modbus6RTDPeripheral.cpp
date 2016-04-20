/*
 * Modbus6RTDPeripheral.cpp
 *
 *  Created on: 20 Jun 2013
 *      Author: maora
 */

#include "Modbus6RTDPeripheral.h"
#include <stdio.h>
#include <logger.h>

Modbus6RTDPeripheral::Modbus6RTDPeripheral(uint8_t slaveId) :
        ModbusInputPeripheralBase(slaveId)
{
//    m_startAddress = 0;

    for (int i = 0; i < M_6RTD_NUM_OF_SENSORS; ++i)
    {
//        ElementRepository::getInstance().addElement(&m_temperatureElementsArray[i]);
        m_temperatureElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
    }

    setUpdateInterval(M_6RTD_SAMPLE_INTERVAL);
}

Modbus6RTDPeripheral::~Modbus6RTDPeripheral()
{
}

void Modbus6RTDPeripheral::readInputs()
{
    // buffer should contain one additional value - the sensor state.
    static uint16_t buffer[M_6RTD_NUM_OF_SENSORS + 1];
    uint32_t bufferLength;
    int i;

    // read the temperature sensors
    E_ModbusError error = readHoldingRegs(getSlaveId(), M_6RTD_REG_START_ADDRESS, M_6RTD_NUM_OF_SENSORS + 1,
            (uint8_t*) buffer, bufferLength);

    if (error != E_ModbusError_Ok)
    {
        if (isTimeoutExpired())
        {
            for (i = 0; i < M_6RTD_NUM_OF_SENSORS; ++i)
                m_temperatureElementsArray[i]->setValueValid(false);
        }
        return;
    }

    uint16_t sensorConnectedMask = SWAP_16(buffer[M_6RTD_NUM_OF_SENSORS]);

    for (i = 0; i < M_6RTD_NUM_OF_SENSORS; ++i)
    {
        buffer[i] = SWAP_16(buffer[i]);
        m_temperatureElementsArray[i]->setValue((float) (buffer[i] * 0.1));
        m_temperatureElementsArray[i]->setValueValid(((sensorConnectedMask & 1) == 0));
        sensorConnectedMask >>= 2;
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "6RTD sensors: %0.2f not connected: %d",
//                m_temperatureElementsArray[i]->getValue(), m_temperatureElementsArray[i]->isValid());
    }

}

//void Modbus6RTDPeripheral::updateOutputs()
//{
//}

//void Modbus6RTDPeripheral::updateNotification(ElementBase* element)
//{
//}

ElementBase* Modbus6RTDPeripheral::getElementByIndex(int index)
{
    if (index >= M_6RTD_NUM_OF_SENSORS)
        return NULL;

    return m_temperatureElementsArray[index];
}

ElementBase* Modbus6RTDPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_6RTD_NUM_OF_SENSORS; ++i)
    {
        if (m_temperatureElementsArray[i]->getPssId())
            return m_temperatureElementsArray[i];
    }
    return NULL;
}

void Modbus6RTDPeripheral::enableElementByIndex(int index, bool enable)
{
}
