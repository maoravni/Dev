/*
 * ModbusInverter.h
 *
 *  Created on: 9 בספט 2013
 *      Author: maora
 */

#ifndef ModbusInverterCommanderSK_H_
#define ModbusInverterCommanderSK_H_

#include "ModbusInverterPeripheralBase.h"
#include "InputPeripheralBase.h"
#include "OutputPeripheralBase.h"
#include <Elements/ElementRepository.h>

class ModbusInverterCommanderSK: /*public OutputPeripheralBase, public InputPeripheralBase, */public ModbusInverterPeripheralBase
{
public:
    ModbusInverterCommanderSK(uint8_t slaveId);
    virtual ~ModbusInverterCommanderSK();

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

    virtual int serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);
};

#endif /* MODBUSINVERTER_H_ */
