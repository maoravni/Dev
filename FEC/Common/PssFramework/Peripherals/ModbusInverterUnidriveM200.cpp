/*
 * ModbusInverter.cpp
 *
 *  Created on: 9
 *      Author: maora
 */

#include <Elements/ElementBase.h>
//#include <Peripherals/ModbusInverterCommanderSK.h>
#include <Peripherals/ModbusInverterUnidriveM200.h>
#include <Peripherals/ModbusPeripheralBase.h>
#include <logger.h>
#include "Persistency/ModbusPeripheralSerializers.h"

#define M_NUMBER_OF_RETRIES 10
#define M_INVERTER_SAMPLE_INTERVAL 250
#define M_INVERTER_FREQUENCY_SETPOINT_ADDRESS 120
#define M_INVERTER_OUTPUT_FREQUENCY_ADDRESS 500
#define M_INVERTER_OUTPUT_CURRENT_ADDRESS 400
#define M_MIN_SPEED_ADDRESS 106
#define M_MAX_SPEED_ADDRESS 105
#define M_ACCEL_RATE_ADDRESS 210
#define M_DECEL_RATE_ADDRESS 220
#define M_REMAP_ADDRESS_START 2255
#define M_REMAPPED_ADDRESS_START 55
#define M_INVERTER_REFERENCE_MODE_PRESET 3
#define M_INVERTER_CONTROL_MODE_PRESET 4
#define M_INVERTER_FORWARD_BIT 0x04
#define M_INVERTER_ENABLE_BIT 0x02

struct T_UnidriveRegisterSetup
{
    uint16_t regAddress;
    uint16_t value;
};

struct T_DriveReadReply
{
    int16_t setpoint;
    int16_t outputFrequency;
    int16_t outputCurrent;
    uint16_t controlMode;
    uint16_t ioWord;
    uint16_t driveStatus;
};

/**
 * During the Unidrive M200 setup we remap menu 0, so we can read multiple registers at the same time.
 */
static const T_UnidriveRegisterSetup UnidriveRegisterSetup[] =
{
{ M_REMAP_ADDRESS_START, 1021 }, //-< Remap Frequency Setpoint
        { M_REMAP_ADDRESS_START + 1, 5001 }, //-< Remap Frequency Output
        { M_REMAP_ADDRESS_START + 2, 4001 }, //-< Remap Output Current
        { M_REMAP_ADDRESS_START + 3, 11034 }, //-< Remap Current Control Mode (Should be preset(3))
        { M_REMAP_ADDRESS_START + 4, 8020 }, //-< Remap Digital IO Word
        { M_REMAP_ADDRESS_START + 5, 10104 }, //-< Remap Drive Status
        { 1133, M_INVERTER_CONTROL_MODE_PRESET }, //-< Setup drive configuration to be preset
        { 113, M_INVERTER_REFERENCE_MODE_PRESET }, //-< Setup reference selector to be preset
        { 114, 0 }, //-< Select preset 0
        { 0, 0 } };

ModbusInverterUnidriveM200::ModbusInverterUnidriveM200(uint8_t slaveId) :
        ModbusInverterPeripheralBase(slaveId)
{
    setInverterType();
    m_driveStatus = ElementRepository::getInstance().addValidationElementU16();
    m_driveStatus->setValue(0);
}

ModbusInverterUnidriveM200::~ModbusInverterUnidriveM200()
{
}

void ModbusInverterUnidriveM200::readInputs()
{
    T_DriveReadReply driveReadReply;
    uint32_t buffLength;

    E_ModbusError error;

    error = readHoldingRegs(getSlaveId(), M_REMAPPED_ADDRESS_START, 6, (uint8_t*) &driveReadReply, buffLength);
    if (error != E_ModbusError_Ok)
    {
        ++m_numOfFailedReads;
        if (m_numOfFailedReads > M_NUMBER_OF_RETRIES)
        {
            // TODO: Set the control at an error.
            m_outputCurrent->setValueValid(false);
            m_outputFrequency->setValueValid(false);
            m_outputFrequency->setValue(0);
            m_outputCurrent->setValue(0);
            InputPeripheralBase::setUpdateInterval(10000);
        }
        return;
    }

    // if we reach here, it means we successfully read registers from the inverter.
    // if we have a previous failure, we need to set the values to valid.
    // then we reset the number of failures.
    if (m_numOfFailedReads > 0)
    {
        // if an inverter is back online, need to set it up again and reset it.
        setupInverter();
        resetInverter();
        InputPeripheralBase::setUpdateInterval(M_INVERTER_SAMPLE_INTERVAL);
        m_outputCurrent->setValueValid(true);
        m_outputFrequency->setValueValid(true);
    }
    m_numOfFailedReads = 0;

    if (m_setpointUpdated)
    {
        error = writeSingleReg(getSlaveId(), M_INVERTER_FREQUENCY_SETPOINT_ADDRESS,
                (int16_t)(m_frequencySetpoint->getValueF() * m_setpointMultiplier));
        if (error != E_ModbusError_Ok)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Modbus Device %d error: setpoint update failed.", getSlaveId());
            ++m_numOfFailedReads;
            return;
        }
        m_setpointUpdated = false;
    }

    SWAP_AND_ASSIGN_16(driveReadReply.controlMode);
    SWAP_AND_ASSIGN_16(driveReadReply.driveStatus);
    SWAP_AND_ASSIGN_16(driveReadReply.ioWord);
    SWAP_AND_ASSIGN_16(driveReadReply.outputCurrent);
    SWAP_AND_ASSIGN_16(driveReadReply.outputFrequency);
    SWAP_AND_ASSIGN_16(driveReadReply.setpoint);
    m_outputFrequency->setValue((float) (driveReadReply.outputFrequency * m_frequencyMultiplier));
    m_outputCurrent->setValue((float) (driveReadReply.outputCurrent * m_currentMultiplier));
    // TODO: Compare the setpoint to the requested setpoint.

    // Drive control mode;
    // Check that the drive mode is still in Preset mode, and if not issue an error.
    // This is a latching error, and will be cleared only on drive reset.
    if (driveReadReply.controlMode != M_INVERTER_CONTROL_MODE_PRESET)
        m_driveStatus->updateErrorBits(E_PSSErrors_InverterInLocalMode, true);

    // Read the IO control word:
    // If the inverter's requested setpoint is not 0, and forward is not enabled,
    // issue an error.
    m_driveStatus->updateErrorBits(E_PSSErrors_InverterForwardNotEnabled,
            !(driveReadReply.ioWord & M_INVERTER_FORWARD_BIT));

    m_driveStatus->updateErrorBits(E_PSSErrors_InverterHwEnableDown, !(driveReadReply.ioWord & M_INVERTER_ENABLE_BIT));

}

void ModbusInverterUnidriveM200::updateOutputs()
{
    m_setpointUpdated = true;
//    writeSingleReg(getSlaveId(), M_INVERTER_FREQUENCY_SETPOINT_ADDRESS,
//            (int16_t)(m_frequencySetpoint->getValueF() * m_setpointMultiplier));
}

void ModbusInverterUnidriveM200::setInverterType()
{
    m_inverterType = E_ModbusInverterType_UnidriveM200;
    m_frequencyMultiplier = 0.01;
    m_setpointMultiplier = 100;
    m_currentMultiplier = 0.01;
}

void ModbusInverterUnidriveM200::setupInverterMinSpeed(float value)
{
    writeSingleReg(getSlaveId(), M_MIN_SPEED_ADDRESS, (int16_t) value * 100);
    vTaskDelay(10);
}

void ModbusInverterUnidriveM200::setupInverterMaxSpeed(float value)
{
    writeSingleReg(getSlaveId(), M_MAX_SPEED_ADDRESS, (int16_t) value * 100);
    vTaskDelay(10);
}

void ModbusInverterUnidriveM200::setupInverterAccelRate(float value)
{
    // select acceleration rate reference register 0
    writeSingleReg(getSlaveId(), 209, 0);
    vTaskDelay(10);
    // write the acceleration rate.
    writeSingleReg(getSlaveId(), M_ACCEL_RATE_ADDRESS, (int16_t) value * 10);
    vTaskDelay(10);
}

void ModbusInverterUnidriveM200::setupInverterDecelRate(float value)
{
    // select deceleration rate reference register 0
    writeSingleReg(getSlaveId(), 219, 0);
    vTaskDelay(10);
    //write the deceleration rate.
    writeSingleReg(getSlaveId(), M_DECEL_RATE_ADDRESS, (int16_t) value * 10);
    vTaskDelay(10);
}

void ModbusInverterUnidriveM200::setupInverter()
{
    for (int i = 0; i < sizeof(UnidriveRegisterSetup) && UnidriveRegisterSetup[i].regAddress != 0; ++i)
    {
        writeSingleReg(getSlaveId(), UnidriveRegisterSetup[i].regAddress, UnidriveRegisterSetup[i].value);
        vTaskDelay(10);
    }
}

ElementBase* ModbusInverterUnidriveM200::getElementByIndex(int index)
{
    if (index == 4)
        return m_driveStatus;
    return ModbusInverterPeripheralBase::getElementByIndex(index);
}

void ModbusInverterUnidriveM200::resetInverter()
{
    writeSingleReg(getSlaveId(), 1032, 1);
    vTaskDelay(30);
}

void ModbusInverterUnidriveM200::setupInverterMotorFrequency(float value)
{
}

void ModbusInverterUnidriveM200::setupInverterMotorCurrent(float value)
{
}

void ModbusInverterUnidriveM200::setupInverterMotorNominalRPM(float value)
{
}

ModbusInverterUnidriveM200::ModbusInverterUnidriveM200(F_FILE* f) : ModbusInverterPeripheralBase(f)
{
    Serializer<ModbusInverterUnidriveM200> s;
    s.deserialize(f, *this);
}

void ModbusInverterUnidriveM200::serialize(F_FILE* f)
{
    Serializer<ModbusInverterUnidriveM200> s;
    s.serialize(f, *this);
}
