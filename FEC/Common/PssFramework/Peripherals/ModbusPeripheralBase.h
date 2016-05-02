/*
 * ModbusPeripheralBase.h
 *
 *  Created on: 19 Jun 2013
 *      Author: maora
 */

#ifndef MODBUSPERIPHERALBASE_H_
#define MODBUSPERIPHERALBASE_H_

#include "InputOutputPeripheralBase.h"
#include "AnalogInputPeripheralBase.h"
#include <CBinarySemaphore.h>
#include <task.h>
#include <vector>
#ifdef WIN32
#include <win32/PortAllocations.h>
#endif

#if defined(FEC_BOARD) || defined(FEC2_BOARD)
#define M_ModbusUart USART3
#endif

#define SWAP_16(value16) (((uint16_t)value16<<8) | ((uint16_t)value16>>8))
#define SWAP_AND_ASSIGN_16(x) x = SWAP_16(x)

#define M_MODBUS_INPUT_START_ADDRESS 30000
#define M_MODBUS_HOLDING_START_ADDRESS 40000

enum E_ModbusError
{
    E_ModbusError_Ok = 0,
    E_ModbusError_CrcError = 1,
    E_ModbusError_NoSlaveResponse = 2,
    E_ModbusError_UnexpectedError = 3,
    E_ModbusError_UnknownError = 0xFF
};

enum E_ModbusFunctionCode
{
    E_ModbusFunctionCode_ReadCoils = 1,
    E_ModbusFunctionCode_ReadHoldingRegs = 3,
    E_ModbusFunctionCode_ReadInputRegs = 4,
    E_ModbusFunctionCode_ReadSingleCoil = 5,
    E_ModbusFunctionCode_WriteSingleReg = 6,
    E_ModbusFunctionCode_Size
};

struct T_ModbusRequestDataStruct
{
    uint16_t address;
    uint16_t value;
};

union T_ModbusRequestData
{
    T_ModbusRequestDataStruct single_coil_write_req;
    uint8_t buffer[4];
};

struct T_ModbusRtuRequestFrameStruct
{
    uint8_t slave_id;
    uint8_t funcCode;
    uint16_t address;
    uint16_t value;
    uint16_t crc16_val;
};

union T_ModbusRtuRequestFrame
{
    T_ModbusRtuRequestFrameStruct split;
    uint8_t buffer[8];
};

typedef std::vector<T_ModbusRequestDataStruct> T_GeneralModbusSetupVector;

#define M_NO_RESPONSE_TIMEOUT 10000

class UpdateSchedulerTaskBase;

class ModbusPeripheralBase
{
private:
//    CBinarySemaphore m_transmissionCompleteSemaphore;
    uint8_t m_numOfRetries;
//    uint8_t m_responseTimeout;
//    uint8_t m_minimumDelayBetweenRequests;

    uint8_t m_slaveId;

    static portTickType m_lastGoodResponseTickCount; //!< last good response tick count. Used for timeout when the device stops responding.

    T_GeneralModbusSetupVector m_generalModbusSetupVector;

public:
    ModbusPeripheralBase(uint8_t slaveId);
    virtual ~ModbusPeripheralBase();

    uint8_t getNumOfRetries() const
    {
        return m_numOfRetries;
    }

    void setNumOfRetries(uint8_t numOfRetries)
    {
        m_numOfRetries = numOfRetries;
    }

    uint16_t getResponseTimeout() const;

    void setResponseTimeout(uint16_t responseTimeout);

    uint8_t getSlaveId() const
    {
        return m_slaveId;
    }

    void setSlaveId(uint8_t slaveId)
    {
        m_slaveId = slaveId;
    }

    virtual UpdateSchedulerTaskBase* getSchedulerTask();
    virtual void addGeneralSetupRegister(uint16_t address, uint16_t value);

protected:
    E_ModbusError readHoldingRegs(uint8_t slaveId, uint16_t startAddress, uint16_t numOfRegs,
            uint8_t *buffer, uint32_t &bufferLength);
    E_ModbusError readInputRegs(uint8_t slaveId, uint16_t startAddress, uint16_t numOfRegs,
            uint8_t *buffer, uint32_t &bufferLength);

    E_ModbusError writeSingleReg(uint8_t slaveId, uint16_t address, uint16_t value);

    void transactionDelay();

    void clearTimeoutFlag()
    {
        m_lastGoodResponseTickCount = xTaskGetTickCount();
    }

    bool isTimeoutExpired()
    {
        return ((xTaskGetTickCount() - m_lastGoodResponseTickCount) >= M_NO_RESPONSE_TIMEOUT);
    }

    virtual void writeGeneralSetupVector();

private:
    E_ModbusError generalReadRegs(E_ModbusFunctionCode funcCode, uint8_t slaveId, uint16_t startAddress, uint16_t numOfRegs,
            uint8_t *buffer, uint32_t &bufferLength);
    uint16_t crc16_calc(uint8_t* ptr, int32_t len);

    template <class T> friend class Serializer;

};

class ModbusInputPeripheralBase : public ModbusPeripheralBase, public InputPeripheralBase
{
public:
    ModbusInputPeripheralBase(uint8_t slaveId);
    virtual UpdateSchedulerTaskBase* getSchedulerTask();
};

class ModbusAnalogInputPeripheralBase : public ModbusPeripheralBase, public AnalogInputPeripheralBase
{
public:
    ModbusAnalogInputPeripheralBase(uint8_t slaveId);
    virtual UpdateSchedulerTaskBase* getSchedulerTask();
};

class ModbusInputOutputPeripheralBase : public ModbusPeripheralBase, public InputOutputPeripheralBase
{
public:
    ModbusInputOutputPeripheralBase(uint8_t slaveId);
    virtual UpdateSchedulerTaskBase* getSchedulerTask();
};

#endif /* MODBUSPERIPHERALBASE_H_ */
