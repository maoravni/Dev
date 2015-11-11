/*
 * ModbusSmcHrsChiller.h
 *
 *  Created on: 23 Jun 2015
 *      Author: maora
 */

#ifndef MODBUSSMCHRSCHILLER_H_
#define MODBUSSMCHRSCHILLER_H_

#include <Peripherals/ModbusInverterPeripheralBase.h>

class ModbusSmcHrsChiller: public ModbusInputOutputPeripheralBase
{
    ValidationElementFloat* m_fluidTemperature;
    ValidationElementFloat* m_fluidPressure;
    ValidationElementFloat* m_setpoint;

    bool m_setpointUpdated;
    int m_numOfFailedReads;

public:
    ModbusSmcHrsChiller(uint8_t slaveId);
    virtual ~ModbusSmcHrsChiller();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_SmcHrsChiller;}
    virtual int getElementCount(){ return 3;}

    virtual void readInputs();
    virtual void updateOutputs();
    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);

    virtual void enableElementByIndex(int index, bool enable);

};

#endif /* MODBUSSMCHRSCHILLER_H_ */
