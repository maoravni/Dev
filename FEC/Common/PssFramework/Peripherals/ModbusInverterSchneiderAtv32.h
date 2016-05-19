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
    ModbusInverterSchneiderAtv32(F_FILE* f);
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

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);
private:
    void storeConfiguration();
    void performInverterAutotune();

    template <class T> friend class Serializer;

};

#endif /* MODBUSINVERTERSCHNEIDERATV32_H_ */
