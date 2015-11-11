/*
 * ModbusInverterSchneiderAtv32.h
 *
 *  Created on: 11 Jan 2015
 *      Author: maora
 */

#ifndef MODBUSINVERTERSCHNEIDERATV32_H_
#define MODBUSINVERTERSCHNEIDERATV32_H_

#include "ModbusInverterPeripheralBase.h"

class ModbusInverterSchneiderAtv32: public ModbusInverterPeripheralBase
{
private:
    int16_t m_maxSpeed;
    int16_t m_minSpeed;
    int16_t m_accelRate;
    int16_t m_decelRate;
    int16_t m_nominalFrequency;
    int16_t m_nominalCurrent;
    int16_t m_nominalRpm;

public:
    ModbusInverterSchneiderAtv32(uint8_t slaveId);
    virtual ~ModbusInverterSchneiderAtv32();

    virtual void readInputs();
    virtual void updateOutputs();
    virtual void setInverterType();

    virtual void setupInverterMinSpeed(float value);
    virtual void setupInverterMaxSpeed(float value);
    virtual void setupInverterAccelRate(float value);
    virtual void setupInverterDecelRate(float value);
    virtual void setupInverterMotorFrequency(float value);
    virtual void setupInverterMotorCurrent(float value);
    virtual void setupInverterMotorNominalRPM(float value);
    virtual void setupInverter();
    virtual void resetInverter();

private:
    void storeConfiguration();
    void performInverterAutotune();

};

#endif /* MODBUSINVERTERSCHNEIDERATV32_H_ */
