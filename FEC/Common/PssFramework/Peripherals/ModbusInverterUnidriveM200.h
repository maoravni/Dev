/*
 * ModbusInverter.h
 *
 *  Created on: 9 בספט 2013
 *      Author: maora
 */

#ifndef ModbusInverterUnidriveM200_H_
#define ModbusInverterUnidriveM200_H_

//#include <Elements/ElementRepository.h>
#include <Peripherals/ModbusInverterPeripheralBase.h>
//#include "InputPeripheralBase.h"
//#include "OutputPeripheralBase.h"

class ModbusInverterUnidriveM200: /*public OutputPeripheralBase, public InputPeripheralBase, */public ModbusInverterPeripheralBase
{
private:
    ElementBase* m_driveStatus;
    int16_t m_maxSpeed;
    int16_t m_minSpeed;
    int16_t m_accelRate;
    int16_t m_decelRate;
    int16_t m_nominalFrequency;
    int16_t m_nominalCurrent;
    int16_t m_nominalRpm;

public:
    ModbusInverterUnidriveM200(uint8_t slaveId);
    ModbusInverterUnidriveM200(F_FILE* f);
    virtual ~ModbusInverterUnidriveM200();

    virtual void readInputs();
    virtual void updateOutputs();
    virtual void setInverterType();

    virtual int getElementCount(){ return 4;}
    virtual ElementBase* getElementByIndex(int index);

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
    virtual void sendInverterMinSpeed();
    virtual void sendInverterMaxSpeed();
    virtual void sendInverterAccelRate();
    virtual void sendInverterDecelRate();

    template <class T> friend class Serializer;
};

#endif /* MODBUSINVERTER_H_ */
