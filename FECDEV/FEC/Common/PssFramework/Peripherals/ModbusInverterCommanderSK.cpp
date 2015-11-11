/*
 * ModbusInverter.cpp
 *
 *  Created on: 9 בספט 2013
 *      Author: maora
 */

#include "ModbusInverterCommanderSK.h"

#define M_NUMBER_OF_RETRIES 10
#define M_INVERTER_SAMPLE_INTERVAL 250
#define M_INVERTER_FREQUENCY_SETPOINT_ADDRESS 120
#define M_INVERTER_OUTPUT_FREQUENCY_ADDRESS 500
#define M_INVERTER_OUTPUT_CURRENT_ADDRESS 400
#define M_MIN_SPEED_ADDRESS 106
#define M_MAX_SPEED_ADDRESS 105
#define M_ACCEL_RATE_ADDRESS 210
#define M_DECEL_RATE_ADDRESS 220

ModbusInverterCommanderSK::ModbusInverterCommanderSK(uint8_t slaveId) :
        ModbusInverterPeripheralBase(slaveId)
{
    setInverterType();
    setupInverter();
    resetInverter();
}

ModbusInverterCommanderSK::~ModbusInverterCommanderSK()
{
    m_frequencySetpoint->removeObserver(this);
}

void ModbusInverterCommanderSK::readInputs()
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
            InputPeripheralBase::setUpdateInterval(10000);
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
        InputPeripheralBase::setUpdateInterval(M_INVERTER_SAMPLE_INTERVAL);
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

void ModbusInverterCommanderSK::updateOutputs()
{
    m_setpointUpdated = true;
//    writeSingleReg(getSlaveId(), M_INVERTER_FREQUENCY_SETPOINT_ADDRESS,
//            (int16_t)(m_frequencySetpoint->getValueF() * m_setpointMultiplier));
}

void ModbusInverterCommanderSK::setInverterType()
{
    m_inverterType = E_ModbusInverterType_CommanderSK;
    m_frequencyMultiplier = 0.1;
    m_setpointMultiplier = 10;
    m_currentMultiplier = 0.01;
}

void ModbusInverterCommanderSK::setupInverterMinSpeed(float value)
{
    writeSingleReg(getSlaveId(), M_MIN_SPEED_ADDRESS, (int16_t) value * 10);
    vTaskDelay(30);
}

void ModbusInverterCommanderSK::setupInverterMaxSpeed(float value)
{
    writeSingleReg(getSlaveId(), M_MAX_SPEED_ADDRESS, (int16_t) value * 10);
    vTaskDelay(30);
}

void ModbusInverterCommanderSK::setupInverterAccelRate(float value)
{
    // select acceleration rate reference register 0
    writeSingleReg(getSlaveId(), 209, 0);
    vTaskDelay(30);
    writeSingleReg(getSlaveId(), M_ACCEL_RATE_ADDRESS, (int16_t) value * 10);
    vTaskDelay(30);
}

void ModbusInverterCommanderSK::setupInverterDecelRate(float value)
{
    // select deceleration rate reference register 0
    writeSingleReg(getSlaveId(), 219, 0);
    vTaskDelay(30);
    writeSingleReg(getSlaveId(), M_DECEL_RATE_ADDRESS, (int16_t) value * 10);
    vTaskDelay(30);
}

void ModbusInverterCommanderSK::setupInverter()
{
    // write the drive configuration to be preset.
    writeSingleReg(getSlaveId(), 1126, 3);
    vTaskDelay(30);
    // set reference selector to be preset
    writeSingleReg(getSlaveId(), 113, 3);
    vTaskDelay(30);
    // set the selected preset to be 0.
    writeSingleReg(getSlaveId(), 114, 0);
    vTaskDelay(30);
}

void ModbusInverterCommanderSK::resetInverter()
{
    writeSingleReg(getSlaveId(), 1032, 1);
    vTaskDelay(30);
}

void ModbusInverterCommanderSK::setupInverterMotorFrequency(float value)
{
}

void ModbusInverterCommanderSK::setupInverterMotorCurrent(float value)
{
}

void ModbusInverterCommanderSK::setupInverterMotorNominalRPM(float value)
{
}
