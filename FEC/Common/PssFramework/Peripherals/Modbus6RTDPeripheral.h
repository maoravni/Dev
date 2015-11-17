/*
 * Modbus6RTDPeripheral.h
 *
 *  Created on: 20 Jun 2013
 *      Author: maora
 */

#ifndef MODBUS6RTDPERIPHERAL_H_
#define MODBUS6RTDPERIPHERAL_H_

#include "ModbusPeripheralBase.h"
#include <Elements/ValidationElement.h>
#include <Elements/ElementRepository.h>

#define M_6RTD_NUM_OF_SENSORS 6
#define M_6RTD_REG_START_ADDRESS 1
#define M_6RTD_SAMPLE_INTERVAL 2000

class Modbus6RTDPeripheral : /*public InputPeripheralBase, public ModbusPeripheralBase*/ public ModbusInputPeripheralBase
{
//    uint16_t m_startAddress;

    ValidationElementFloat* m_temperatureElementsArray[M_6RTD_NUM_OF_SENSORS];
public:
    Modbus6RTDPeripheral(uint8_t slaveId);
    virtual ~Modbus6RTDPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_6RTD;}
    virtual int getElementCount() {return M_6RTD_NUM_OF_SENSORS;}

    virtual void readInputs();
//    virtual void updateOutputs();
//    virtual void updateNotification(ElementBase* element);

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);
};

#endif /* MODBUS6RTDPERIPHERAL_H_ */