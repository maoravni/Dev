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

public:
    ModbusInverterUnidriveM200(uint8_t slaveId);
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

};

#endif /* MODBUSINVERTER_H_ */
