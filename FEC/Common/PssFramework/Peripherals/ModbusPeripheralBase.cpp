/*
 * ModbusPeripheralBase.cpp
 *
 *  Created on: 19 Jun 2013
 *      Author: maora
 */

#include "ModbusPeripheralBase.h"
#include <usart.h>
#include <logger.h>
#include <crc16.h>
#include <string.h>
#include <Tasks/UpdateSchedulerTask.h>

extern "C" xSemaphoreHandle usartResponseCompleteSemaphore;
extern "C" T_UsartRespBuffer usartResponseBuffer;

uint8_t m_minimumDelayBetweenRequests = 10;
uint16_t m_responseTimeout = 250;
portTickType ModbusPeripheralBase::m_lastGoodResponseTickCount = 0; //!< last good response tick count. Used for timeout when the device stops responding.

int lastTransactionDelay;

ModbusPeripheralBase::ModbusPeripheralBase(uint8_t slaveId)
{
//    m_transmissionCompleteSemaphore.attach(usartResponseCompleteSemaphore);
    m_numOfRetries = 3;
    m_slaveId = slaveId;
    m_lastGoodResponseTickCount = 0;
}

ModbusPeripheralBase::~ModbusPeripheralBase()
{
}

E_ModbusError ModbusPeripheralBase::readHoldingRegs(uint8_t slaveId, uint16_t startAddress, uint16_t numOfRegs,
        uint8_t *buffer, uint32_t &bufferLength)
{
    return generalReadRegs(E_ModbusFunctionCode_ReadHoldingRegs, slaveId, startAddress, numOfRegs, buffer, bufferLength);
}

E_ModbusError ModbusPeripheralBase::readInputRegs(uint8_t slaveId, uint16_t startAddress, uint16_t numOfRegs,
        uint8_t* buffer, uint32_t& bufferLength)
{
    return generalReadRegs(E_ModbusFunctionCode_ReadInputRegs, slaveId, startAddress, numOfRegs, buffer, bufferLength);
}

E_ModbusError ModbusPeripheralBase::generalReadRegs(E_ModbusFunctionCode funcCode, uint8_t slaveId,
        uint16_t startAddress, uint16_t numOfRegs, uint8_t* buffer, uint32_t& bufferLength)
{
    T_ModbusRtuRequestFrame requestFrame;
    uint16_t responseCrc, expectedResponseCrc;


    E_ModbusError result = E_ModbusError_NoSlaveResponse;

    requestFrame.split.slave_id = slaveId;
    requestFrame.split.funcCode = funcCode;
    // swap the address and value, since the transmission is big-endian:
    requestFrame.split.address = SWAP_16(startAddress);
    requestFrame.split.value = SWAP_16(numOfRegs);

    // calculate the CRC of the frame:
    requestFrame.split.crc16_val = crc16_calc(requestFrame.buffer, 6);

    int i;
    for (i = 0; i < m_numOfRetries; ++i)
    {
        if (i > 0)
            M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "Modbus Device %d response error %d retry %d startAddress %d, lastTransactionDelay %d", m_slaveId, result, i, startAddress, lastTransactionDelay);

        // send the buffer to the device:
        usartResponseBuffer.resp_len = 0;
        usart3_set_dir(true);

        transactionDelay();

        send_buf(requestFrame.buffer, 8, M_ModbusUart );

        // take the semaphore to wait for response
//        puts("usart take");
        if (xSemaphoreTake(usartResponseCompleteSemaphore, m_responseTimeout) == pdTRUE && usartResponseBuffer.resp_len != 0)
        {
            // check that the response id is the same as the request:
            if (usartResponseBuffer.resp_buf[0] != slaveId) {
                result = E_ModbusError_UnexpectedError;
                continue;
            }

            // check that the response function is the same as the request:
            if (usartResponseBuffer.resp_buf[1] != (uint8_t)funcCode) {
                result = E_ModbusError_UnexpectedError;
                continue;
            }

            // check that the number of bytes to read is as expected:
            if (usartResponseBuffer.resp_buf[2] != numOfRegs*2) {
                result = E_ModbusError_UnexpectedError;
                continue;
            }

            // check the CRC of the response:
            expectedResponseCrc = crc16_calc(usartResponseBuffer.resp_buf, numOfRegs*2+3);
            responseCrc = *(uint16_t*)(usartResponseBuffer.resp_buf+(numOfRegs*2+3));
            if (responseCrc != expectedResponseCrc) {
                result = E_ModbusError_CrcError;
                continue;
            }

            // process the response and return the data.
            // we decrement 5 because of the 3 bytes in the header and the 2 CRC bytes at the end.
            bufferLength = numOfRegs*2;
            memcpy(buffer, usartResponseBuffer.resp_buf+3, bufferLength);
            usartResponseBuffer.resp_len = 0;
            clearTimeoutFlag();
            result = E_ModbusError_Ok;
            break;
        }
    }

    return result;
}

E_ModbusError ModbusPeripheralBase::writeSingleReg(uint8_t slaveId, uint16_t address, uint16_t value)
{
    T_ModbusRtuRequestFrame requestFrame;
    uint16_t responseCrc, expectedResponseCrc;

    E_ModbusError result = E_ModbusError_NoSlaveResponse;

    requestFrame.split.slave_id = slaveId;
    requestFrame.split.funcCode = E_ModbusFunctionCode_WriteSingleReg;

    // swap the address and value, since the transmission is big-endian:
    requestFrame.split.address = SWAP_16(address);
    requestFrame.split.value = SWAP_16(value);

    // calculate the CRC of the frame:
    requestFrame.split.crc16_val = crc16_calc(requestFrame.buffer, 6);

    int i;
    for (i = 0; i < m_numOfRetries; ++i)
    {
        // send the buffer to the device:
        usartResponseBuffer.resp_len = 0;
        usart3_set_dir(true);

        transactionDelay();

        send_buf(requestFrame.buffer, 8, M_ModbusUart );

        // take the semaphore to wait for response
        if (xSemaphoreTake(usartResponseCompleteSemaphore, m_responseTimeout) == pdTRUE && usartResponseBuffer.resp_len != 0)
        {
            // check that the response id is the same as the request:
            if (usartResponseBuffer.resp_buf[0] != slaveId)
            {
                result = E_ModbusError_UnexpectedError;
                continue;
            }


            // check that the response function is the same as the request:
            if (usartResponseBuffer.resp_buf[1] != (uint8_t)E_ModbusFunctionCode_WriteSingleReg)
            {
                result = E_ModbusError_UnexpectedError;
                continue;
            }

            // check the CRC of the response:
            expectedResponseCrc = crc16_calc(usartResponseBuffer.resp_buf, 6);
            responseCrc = *(uint16_t*)(usartResponseBuffer.resp_buf+6);
            if (responseCrc != expectedResponseCrc)
            {
                result = E_ModbusError_CrcError;
                continue;
            }

            // process the response and return the data.
            // we decrement 5 because of the 3 bytes in the header and the 2 CRC bytes at the end.
            usartResponseBuffer.resp_len = 0;
            if ((requestFrame.split.address == *(uint16_t*)(usartResponseBuffer.resp_buf+2)) && (requestFrame.split.value == *(uint16_t*)(usartResponseBuffer.resp_buf+4)))
            {
                result = E_ModbusError_Ok;
                break;
            }
            else
            {
                result = E_ModbusError_UnexpectedError;
                continue;
            }
        }

    }

    clearTimeoutFlag();
    return result;
}

uint16_t ModbusPeripheralBase::crc16_calc(uint8_t* ptr, int32_t len)
{
    uint32_t i = 0, k = 0;
    uint16_t crc16_val = 0xFFFF;
    if ((len < 0) || (len > SERRXBUFSIZE))
    {
        return crc16_val;
    }
    for (k = 0; k < len; k++)
    {
        crc16_val ^= *(ptr + k);
        for (i = 0; i < 8; ++i)
        {
            if (crc16_val & 1)
                crc16_val = (crc16_val >> 1) ^ 0xA001;
            else
                crc16_val = (crc16_val >> 1);
        }
    }
    return crc16_val;
}

UpdateSchedulerTaskBase* ModbusPeripheralBase::getSchedulerTask()
{
    return ModbusSchedulerTask::getInstance();
}

void ModbusPeripheralBase::transactionDelay()
{
    int elapsedTicks = xTaskGetTickCount() - m_lastGoodResponseTickCount;
    elapsedTicks = (elapsedTicks < 0) ? 0 : elapsedTicks;

    if (elapsedTicks < m_minimumDelayBetweenRequests)
    {
//        printf("transaction delay: %d\n", m_minimumDelayBetweenRequests - elapsedTicks);
        lastTransactionDelay = m_minimumDelayBetweenRequests - elapsedTicks;
        vTaskDelay(m_minimumDelayBetweenRequests - elapsedTicks);
    }
}

void ModbusPeripheralBase::setResponseTimeout(uint16_t responseTimeout)
{
    m_responseTimeout = responseTimeout;
}

uint16_t ModbusPeripheralBase::getResponseTimeout() const
{
    return m_responseTimeout;
}

ModbusInputPeripheralBase::ModbusInputPeripheralBase(uint8_t slaveId) : ModbusPeripheralBase(slaveId)
{
}

ModbusAnalogInputPeripheralBase::ModbusAnalogInputPeripheralBase(uint8_t slaveId) : ModbusPeripheralBase(slaveId)
{
}

ModbusInputOutputPeripheralBase::ModbusInputOutputPeripheralBase(uint8_t slaveId) : ModbusPeripheralBase(slaveId)
{
}

UpdateSchedulerTaskBase* ModbusInputPeripheralBase::getSchedulerTask()
{
    return ModbusSchedulerTask::getInstance();
}

UpdateSchedulerTaskBase* ModbusAnalogInputPeripheralBase::getSchedulerTask()
{
    return ModbusSchedulerTask::getInstance();
}

UpdateSchedulerTaskBase* ModbusInputOutputPeripheralBase::getSchedulerTask()
{
    return ModbusSchedulerTask::getInstance();
}

void ModbusPeripheralBase::addGeneralSetupRegister(uint16_t address, uint16_t value)
{
    T_ModbusRequestDataStruct data;
    data.address = address;
    data.value = value;
    m_generalModbusSetupVector.push_back(data);
}

void ModbusPeripheralBase::writeGeneralSetupVector()
{
    for (int i = 0; i < m_generalModbusSetupVector.size(); ++i)
    {
        writeSingleReg(getSlaveId(), m_generalModbusSetupVector[i].address, m_generalModbusSetupVector[i].value);
    }
}
