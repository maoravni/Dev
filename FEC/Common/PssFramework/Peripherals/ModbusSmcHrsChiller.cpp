/*
 * ModbusSmcHrsChiller.cpp
 *
 *  Created on: 23 Jun 2015
 *      Author: maora
 */

#include <Peripherals/ModbusSmcHrsChiller.h>
#include "Persistency/ModbusPeripheralSerializers.h"

#define M_NUMBER_OF_RETRIES 10
#define M_SMC_CHILLERS_DEFAULT_UPDATE_INTERVAL 10000
#define M_SMC_REGISTER_START_ADDRESS 0
#define M_SMC_NUMBER_OF_REGISTERS 12
#define M_SMC_FLUID_TEMP_ADDRESS 0
#define M_SMC_FLUID_PRES_ADDRESS 2
#define M_SMC_STATUS_ADDRESS 4
#define M_SMC_SETPOINT_ADDRESS 0xb

ModbusSmcHrsChiller::ModbusSmcHrsChiller(uint8_t slaveId) :
        ModbusInputOutputPeripheralBase(slaveId)
{
    m_fluidTemperature = ElementRepository::getInstance().addValidationElementFloat();
    m_fluidPressure = ElementRepository::getInstance().addValidationElementFloat();
    m_setpoint = ElementRepository::getInstance().addValidationElementFloat();
    m_fluidTemperature->setValue((uint32_t)0);
    m_fluidPressure->setValue((uint32_t)0);
    m_setpoint->setValue((uint32_t)0);
    m_setpoint->addObserver(this);

    setUpdateInterval(M_SMC_CHILLERS_DEFAULT_UPDATE_INTERVAL);

    m_numOfFailedReads = 0;

}

ModbusSmcHrsChiller::~ModbusSmcHrsChiller()
{
    m_setpoint->removeObserver(this);
}

void ModbusSmcHrsChiller::readInputs()
{
    int16_t regBuffer[M_SMC_NUMBER_OF_REGISTERS];
    int16_t temp;
    uint32_t buffLength;

    E_ModbusError error;

    error = readHoldingRegs(getSlaveId(), M_SMC_REGISTER_START_ADDRESS, M_SMC_NUMBER_OF_REGISTERS, (uint8_t*) regBuffer,
            buffLength);
    if (error != E_ModbusError_Ok)
    {
        ++m_numOfFailedReads;
        if (m_numOfFailedReads > M_NUMBER_OF_RETRIES)
        {
            // TODO: Set the control at an error.
            m_fluidTemperature->setValueValid(false);
            m_fluidPressure->setValueValid(false);
            m_fluidTemperature->setValue((uint32_t)0);
            m_fluidPressure->setValue((uint32_t)0);
            setUpdateInterval(10000);
        }
        return;
    }

    if (m_numOfFailedReads > 0)
    {
        setUpdateInterval(M_SMC_CHILLERS_DEFAULT_UPDATE_INTERVAL);
        m_fluidTemperature->setValueValid(true);
        m_fluidPressure->setValueValid(true);
    }
    m_numOfFailedReads = 0;

    temp = SWAP_16(regBuffer[M_SMC_FLUID_TEMP_ADDRESS]);
    m_fluidTemperature->setValue((float)((float) temp * 0.1));

    temp = SWAP_16(regBuffer[M_SMC_FLUID_PRES_ADDRESS]);
    m_fluidPressure->setValue((float)((float) temp * 0.1));

    temp = SWAP_16(regBuffer[M_SMC_SETPOINT_ADDRESS]);
    float tempSetpoint = (float) temp * 0.1;
    if (tempSetpoint != m_setpoint->getValueF())
        m_setpoint->setValue(tempSetpoint);

    if (m_setpointUpdated)
    {
        error = writeSingleReg(getSlaveId(), M_SMC_SETPOINT_ADDRESS, (int16_t)(m_setpoint->getValueF() * 10));
        if (error != E_ModbusError_Ok)
        {
            ++m_numOfFailedReads;
            return;
        }
        m_setpointUpdated = false;
    }
}

void ModbusSmcHrsChiller::updateOutputs()
{
    m_setpointUpdated = true;
}

void ModbusSmcHrsChiller::updateNotification(ElementBase* element)
{
    updateOutputs();
}

ElementBase* ModbusSmcHrsChiller::getElementByIndex(int index)
{
    switch (index)
    {
    case 0:
        return m_fluidTemperature;
    case 1:
        return m_fluidPressure;
    case 2:
        return m_setpoint;
    }
    return NULL;
}

ElementBase* ModbusSmcHrsChiller::getElementByPssId(int pssId)
{
    if (m_fluidTemperature->getPssId() == pssId)
        return m_fluidTemperature;
    if (m_fluidPressure->getPssId() == pssId)
        return m_fluidPressure;
    if (m_setpoint->getPssId() == pssId)
        return m_setpoint;
    return NULL;
}

void ModbusSmcHrsChiller::enableElementByIndex(int index, bool enable)
{
}

int ModbusSmcHrsChiller::serialize(F_FILE* f)
{
    Serializer<ModbusSmcHrsChiller> s;
    return s.serialize(f, *this);
}
