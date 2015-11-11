/*
 * ModbusInverter.cpp
 *
 *  Created on: 9 בספט 2013
 *      Author: maora
 */

#include "ModbusInverterPeripheralBase.h"

#define M_NUMBER_OF_RETRIES 10
#define M_INVERTER_SAMPLE_INTERVAL 250
#define M_INVERTER_FREQUENCY_SETPOINT_ADDRESS 120
#define M_INVERTER_OUTPUT_FREQUENCY_ADDRESS 500
#define M_INVERTER_OUTPUT_CURRENT_ADDRESS 400

ModbusInverterPeripheralBase::ModbusInverterPeripheralBase(uint8_t slaveId) :
        ModbusInputOutputPeripheralBase(slaveId)
{
    m_outputFrequency = ElementRepository::getInstance().addValidationElementFloat();
    m_outputCurrent = ElementRepository::getInstance().addValidationElementFloat();
    m_frequencySetpoint = ElementRepository::getInstance().addValidationElementFloat();
    *m_outputFrequency = 0;
    *m_outputCurrent = 0;
    *m_frequencySetpoint = 0;
    m_setpointUpdated = true;
    m_frequencySetpoint->addObserver(this);

    setUpdateInterval(M_INVERTER_SAMPLE_INTERVAL);

    m_numOfFailedReads = 0;

//    setInverterType();
//    setupInverter();

//    m_setpointUpdated = false;
}

ModbusInverterPeripheralBase::~ModbusInverterPeripheralBase()
{
    m_frequencySetpoint->removeObserver(this);
}

void ModbusInverterPeripheralBase::readInputs()
{
    int16_t outFreq;
    int16_t outCurrent;
    uint32_t buffLength;

    E_ModbusError error;

    error = readHoldingRegs(getSlaveId(), M_INVERTER_OUTPUT_FREQUENCY_ADDRESS, 1, (uint8_t*) &outFreq, buffLength);
    if (error != E_ModbusError_Ok)
    {
        ++m_numOfFailedReads;
        if (m_numOfFailedReads > M_NUMBER_OF_RETRIES)
        {
            // TODO: Set the control at an error.
            m_outputCurrent->setValueValid(false);
            m_outputFrequency->setValueValid(false);
            *m_outputFrequency = 0;
            *m_outputCurrent = 0;
            setUpdateInterval(10000);
        }
        return;
    }

    error = readHoldingRegs(getSlaveId(), M_INVERTER_OUTPUT_CURRENT_ADDRESS, 1, (uint8_t*) &outCurrent, buffLength);
    if (error != E_ModbusError_Ok)
    {
        ++m_numOfFailedReads;
        return;
    }

    if (m_numOfFailedReads > 0)
    {
        setUpdateInterval(M_INVERTER_SAMPLE_INTERVAL);
        m_outputCurrent->setValueValid(true);
        m_outputFrequency->setValueValid(true);
    }
    m_numOfFailedReads = 0;

    outFreq = SWAP_16(outFreq);
    outCurrent = SWAP_16(outCurrent);
    *m_outputFrequency = (float) (outFreq * m_frequencyMultiplier);
    *m_outputCurrent = (float) (outCurrent * m_currentMultiplier);

    if (m_setpointUpdated)
    {
        error = writeSingleReg(getSlaveId(), M_INVERTER_FREQUENCY_SETPOINT_ADDRESS,
                (int16_t)(m_frequencySetpoint->getValueF() * m_setpointMultiplier));
        if (error != E_ModbusError_Ok)
        {
            ++m_numOfFailedReads;
            return;
        }
        m_setpointUpdated = false;
    }
}

ElementBase* ModbusInverterPeripheralBase::getElementByIndex(int index)
{
    switch (index)
    {
    case 0:
        return m_outputFrequency;
    case 1:
        return m_outputCurrent;
    case 2:
        return m_frequencySetpoint;
    }
    return NULL;
}

void ModbusInverterPeripheralBase::updateOutputs()
{
    m_setpointUpdated = true;
//    writeSingleReg(getSlaveId(), M_INVERTER_FREQUENCY_SETPOINT_ADDRESS,
//            (int16_t)(m_frequencySetpoint->getValueF() * m_setpointMultiplier));
}

void ModbusInverterPeripheralBase::updateNotification(ElementBase* element)
{
    updateOutputs();
}

ElementBase* ModbusInverterPeripheralBase::getElementByPssId(int pssId)
{
    if (m_outputFrequency->getPssId() == pssId)
        return m_outputFrequency;
    if (m_outputCurrent->getPssId() == pssId)
        return m_outputCurrent;
    if (m_frequencySetpoint->getPssId() == pssId)
        return m_frequencySetpoint;
    return NULL;
}

void ModbusInverterPeripheralBase::enableElementByIndex(int index, bool enable)
{
}

void ModbusInverterPeripheralBase::setBoardInReady(bool state)
{
    OutputPeripheralBase::setBoardInReady(state);
    if (state)
    {
        setupInverter();
        resetInverter();
    }
}

void ModbusInverterPeripheralBase::startRecovery()
{
    m_numOfFailedReads = 0;
    setUpdateInterval(M_INVERTER_SAMPLE_INTERVAL);
    m_outputCurrent->setValueValid(true);
    m_outputFrequency->setValueValid(true);
    m_setpointUpdated = true;
    setupInverter();
    resetInverter();
}
//void ModbusInverterPeripheralBase::setInverterType(E_ModbusInverterType inverterType)
//{
//    m_inverterType = inverterType;
//    switch (m_inverterType)
//    {
//    case E_ModbusInverterType_CommanderSK:
//        m_frequencyMultiplier = 0.1;
//        m_setpointMultiplier = 10;
//        m_currentMultiplier = 0.01;
//        break;
//    case E_ModbusInverterType_UnidriveM200:
//        m_frequencyMultiplier = 0.01;
//        m_setpointMultiplier = 100;
//        m_currentMultiplier = 0.01;
//        break;
//    }
//}
//
