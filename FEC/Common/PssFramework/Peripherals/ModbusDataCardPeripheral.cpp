/*
 * ModBusDataCardPeripheral.cpp
 *
 *  Created on: 30 Jun 2013
 *      Author: maora
 */

#include "ModbusDataCardPeripheral.h"
#include <logger.h>

ModbusDataCardPeripheral::ModbusDataCardPeripheral(uint8_t slaveId) :
ModbusAnalogInputPeripheralBase(slaveId)
{
//    m_startAddress = 0;

    for (int i = 0; i < M_DataCard_NUM_OF_SENSORS; ++i)
    {
//        ElementRepository::getInstance().addElement(&m_temperatureElementsArray[i]);
        m_temperatureElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_aCoeff[i] = 1;
        m_bCoeff[i] = 0;
    }

    setUpdateInterval(M_DataCard_SAMPLE_INTERVAL);
}

ModbusDataCardPeripheral::~ModbusDataCardPeripheral()
{
}

void ModbusDataCardPeripheral::readInputs()
{
    // buffer should contain one additional value - the sensor state.
    static int16_t buffer[M_DataCard_NUM_OF_SENSORS];
    uint32_t bufferLength;
    int i;

    // read the temperature sensors
    E_ModbusError error = readHoldingRegs(getSlaveId(), M_DataCard_REG_START_ADDRESS, M_DataCard_NUM_OF_SENSORS, (uint8_t*) buffer, bufferLength);

    if (error != E_ModbusError_Ok)
    {
        if (isTimeoutExpired())
        {
            for (i = 0; i < M_DataCard_NUM_OF_SENSORS; ++i)
                m_temperatureElementsArray[i]->setValueValid(false);
        }
        return;
    }

//    uint16_t sensorConnectedMask = SWAP_16(buffer[M_DataCard_NUM_OF_SENSORS]);

    for (i = 0; i < M_DataCard_NUM_OF_SENSORS; ++i)
    {
        buffer[i] = SWAP_16(buffer[i]);
        m_temperatureElementsArray[i]->setValue(((float)(buffer[i] * 0.1))*m_aCoeff[i] + m_bCoeff[i]);
//        m_temperatureElementsArray[i]->setValueValid(((sensorConnectedMask&1) == 0));
//        sensorConnectedMask >>= 2;
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "DataCard sensors: %0.2f not connected: %d", m_temperatureElementsArray[i]->getValue(), m_temperatureElementsArray[i]->isValid());
    }

}

//void ModbusDataCardPeripheral::updateOutputs()
//{
//}

//void ModbusDataCardPeripheral::updateNotification(ElementBase* element)
//{
//}

ElementBase* ModbusDataCardPeripheral::getElementByIndex(int index)
{
    if (index >= M_DataCard_NUM_OF_SENSORS)
        return NULL;

    return m_temperatureElementsArray[index];
}

void ModbusDataCardPeripheral::enableElementByIndex(int index, bool enable)
{
}

void ModbusDataCardPeripheral::setCalibrationCoeff(int index, float aCoeff, float bCoeff)
{
    m_aCoeff[index] = aCoeff;
    m_bCoeff[index] = bCoeff;
}

ElementBase* ModbusDataCardPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_DataCard_NUM_OF_SENSORS; ++i)
    {
        if (m_temperatureElementsArray[i]->getPssId() == pssId)
            return m_temperatureElementsArray[i];
    }
    return NULL;
}

void ModbusDataCardPeripheral::setScalingCoeff(int index, float aCoeff, float bCoeff)
{
}
