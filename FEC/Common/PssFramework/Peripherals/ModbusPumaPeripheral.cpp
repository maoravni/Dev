/*
 * ModbusPumaPeripheral.cpp
 *
 *  Created on: 7 Jul 2013
 *      Author: maora
 */

#include "ModbusPumaPeripheral.h"
#include <Elements/ElementRepository.h>
#include <logger.h>
#include <task.h>
#include "Persistency/ModbusPeripheralSerializers.h"


#define M_PUMA_CHANNEL_START_ADDRESS 1001
#define M_PUMA_ERROR_START_ADDRESS 1008
#define M_PUMA_ERROR_OFFSET 1000
#define M_PUMA_USER_REGISTER_CONFIG_START 300
#define M_PUMA_USER_REGISTER_READ_START 5000
#define M_PUMA_RESET_ADDRESS 0x64

ModbusPumaPeripheral::ModbusPumaPeripheral(uint8_t slaveId) :
ModbusInputPeripheralBase(slaveId)

{
    for (int i = 0; i < M_PUMA_NUM_OF_SENSORS; ++i)
    {
        //ElementRepository::getInstance().addElement(&m_temperatureElementsArray[i]);
        m_temperatureElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
    }

    setUpdateInterval(M_PUMA_SAMPLE_INTERVAL);
    m_userRegistersInitialized = false;
}

ModbusPumaPeripheral::~ModbusPumaPeripheral()
{
}

void ModbusPumaPeripheral::readInputs()
{
    // buffer should contain one additional value - the sensor state.
    static uint16_t buffer[M_PUMA_NUM_OF_SENSORS * 2];
    uint16_t sensorConnectedStatus;
    uint32_t bufferLength;
    E_ModbusError error;
    int i;

    if (!m_userRegistersInitialized)
    {
        m_userRegistersInitialized = initPuma();
        return;
    }
    // read the temperature sensors
//    E_ModbusError error = readInputRegs(getSlaveId(), M_PUMA_CHANNEL_START_ADDRESS, M_PUMA_NUM_OF_SENSORS,
//            (uint8_t*) buffer, bufferLength);
//
//    if (error != E_ModbusError_Ok)
//        return;
//
//    vTaskDelay(50);
    error = readHoldingRegs(getSlaveId(), M_PUMA_USER_REGISTER_READ_START, M_PUMA_NUM_OF_SENSORS * 2, (uint8_t*) buffer,
            bufferLength);

    if (error != E_ModbusError_Ok)
    {
        if (isTimeoutExpired())
        {
            for (i = 0; i < M_PUMA_NUM_OF_SENSORS; ++i)
                m_temperatureElementsArray[i]->setValueValid(false);
        }
        return;
    }

    for (i = 0; i < M_PUMA_NUM_OF_SENSORS; ++i)
    {
        buffer[i] = SWAP_16(buffer[i]);
        buffer[i + M_PUMA_NUM_OF_SENSORS] = SWAP_16(buffer[i+M_PUMA_NUM_OF_SENSORS]);
        m_temperatureElementsArray[i]->setValue((float) (buffer[i] * 0.1));
        m_temperatureElementsArray[i]->setValueValid((buffer[i + M_PUMA_NUM_OF_SENSORS] == 0));
        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "PUMA sensors: %0.2f not connected: %d",
                m_temperatureElementsArray[i]->getValue(), m_temperatureElementsArray[i]->isValid());
    }
}

ElementBase* ModbusPumaPeripheral::getElementByIndex(int index)
{
    if (index >= M_PUMA_NUM_OF_SENSORS)
        return NULL;

    return m_temperatureElementsArray[index];
}

void ModbusPumaPeripheral::enableElementByIndex(int index, bool enable)
{
}

ElementBase* ModbusPumaPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_PUMA_NUM_OF_SENSORS; ++i)
    {
        if (m_temperatureElementsArray[i]->getPssId() == pssId)
            return m_temperatureElementsArray[i];
    }
    return NULL;
}

bool ModbusPumaPeripheral::initPuma()
{
    E_ModbusError error;
    uint16_t buffer[M_PUMA_NUM_OF_SENSORS];
    uint32_t bufferLength;
    uint16_t regValue;
    bool registerWasWritten = false;

#define M_DELAY 200

// TODO: Insert maximum retry count here!!!
    int i;

    // read the setup registers:
    error = readHoldingRegs(getSlaveId(), M_PUMA_USER_REGISTER_CONFIG_START, M_PUMA_NUM_OF_SENSORS, (uint8_t*) buffer,
            bufferLength);

    if (error != E_ModbusError_Ok)
        return false;

    for (i = 0; i < M_PUMA_NUM_OF_SENSORS; ++i)
    {
        buffer[i] = SWAP_16(buffer[i]);
        regValue = M_PUMA_CHANNEL_START_ADDRESS + (M_PUMA_ERROR_OFFSET * i) + M_MODBUS_INPUT_START_ADDRESS;

        if (buffer[i] != regValue)
        {
            vTaskDelay(M_DELAY);
            while (writeSingleReg(getSlaveId(), M_PUMA_USER_REGISTER_CONFIG_START + i, regValue) != E_ModbusError_Ok)
                vTaskDelay(M_DELAY);
            registerWasWritten = true;
        }
    }

    error = readHoldingRegs(getSlaveId(), M_PUMA_USER_REGISTER_CONFIG_START + M_PUMA_NUM_OF_SENSORS,
            M_PUMA_NUM_OF_SENSORS, (uint8_t*) buffer, bufferLength);

    if (error != E_ModbusError_Ok)
        return false;

    for (i = 0; i < M_PUMA_NUM_OF_SENSORS; ++i)
    {
        buffer[i] = SWAP_16(buffer[i]);
        regValue = M_PUMA_ERROR_START_ADDRESS + (M_PUMA_ERROR_OFFSET * i) + M_MODBUS_INPUT_START_ADDRESS;

        if (buffer[i] != regValue)
        {
            vTaskDelay(M_DELAY);
            while (writeSingleReg(getSlaveId(), M_PUMA_USER_REGISTER_CONFIG_START + i + M_PUMA_NUM_OF_SENSORS,
                    M_PUMA_ERROR_START_ADDRESS + (M_PUMA_ERROR_OFFSET * i) + M_MODBUS_INPUT_START_ADDRESS)
                    != E_ModbusError_Ok)
                vTaskDelay(M_DELAY);
            registerWasWritten = true;
        }
    }

    // reset the PUMA:
    if (registerWasWritten)
    {
        vTaskDelay(M_DELAY);
        while (writeSingleReg(getSlaveId(), M_PUMA_RESET_ADDRESS, 1) != E_ModbusError_Ok)
            vTaskDelay(M_DELAY);
        return false;
    }

    return true;
}

ModbusPumaPeripheral::ModbusPumaPeripheral(F_FILE* f) : ModbusInputPeripheralBase(0)
{
    Serializer<ModbusPumaPeripheral> s;
    s.deserialize(f, *this);
}

void ModbusPumaPeripheral::serialize(F_FILE* f)
{
    Serializer<ModbusPumaPeripheral> s;
    s.serialize(f, *this);
}
