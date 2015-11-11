/*
 * ModbusPumaPeripheral.h
 *
 *  Created on: 7 Jul 2013
 *      Author: maora
 */

#ifndef MODBUSPUMAPERIPHERAL_H_
#define MODBUSPUMAPERIPHERAL_H_

#include "ModbusPeripheralBase.h"
#include <Elements/ValidationElement.h>

#define M_PUMA_NUM_OF_SENSORS 4
#define M_PUMA_SAMPLE_INTERVAL 200

class ModbusPumaPeripheral: public ModbusInputPeripheralBase
{
    ValidationElementFloat* m_temperatureElementsArray[M_PUMA_NUM_OF_SENSORS];
    bool m_userRegistersInitialized;
public:
    ModbusPumaPeripheral(uint8_t slaveId);
    virtual ~ModbusPumaPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_Puma;}
    virtual int getElementCount(){ return M_PUMA_NUM_OF_SENSORS;}

    virtual void readInputs();
    //    virtual void updateOutputs();
    //    virtual void updateNotification(ElementBase* element);

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);
    virtual bool initPuma();
};

#endif /* MODBUSPUMAPERIPHERAL_H_ */
