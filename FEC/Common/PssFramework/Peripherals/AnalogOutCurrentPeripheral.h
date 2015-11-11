/*
 * AnalogOutCurrentPeripheral.h
 *
 *  Created on: 6 баев 2013
 *      Author: maora
 */

#ifndef ANALOGOUTCURRENTPERIPHERAL_H_
#define ANALOGOUTCURRENTPERIPHERAL_H_

#include "OutputPeripheralBase.h"
#include <Elements/ValidationElement.h>

#define M_NUM_OF_MA_OUT 2

class AnalogOutCurrentPeripheral: public OutputPeripheralBase
{
    ValidationElementFloat* m_elementArray[M_NUM_OF_MA_OUT];

    uint16_t m_firstElementIndex;

public:
    AnalogOutCurrentPeripheral();
    virtual ~AnalogOutCurrentPeripheral();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_AO;}
    virtual int getElementCount() {return M_NUM_OF_MA_OUT;}

    virtual void updateOutputs();
    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

};

#endif /* ANALOGOUTCURRENTPERIPHERAL_H_ */
