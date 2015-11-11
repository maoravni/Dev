/*
 * DryContactDigitalOutput.h
 *
 *  Created on: 24 αιεμ 2013
 *      Author: maora
 */

#ifndef DRYCONTACTDIGITALOUTPUT_H_
#define DRYCONTACTDIGITALOUTPUT_H_

#include "OutputPeripheralBase.h"
#include <Elements/Element.h>

class DryContactDigitalOutput: public OutputPeripheralBase
{
    ElementU8* m_element;

    uint16_t m_elementIndex;

public:
    DryContactDigitalOutput();
    virtual ~DryContactDigitalOutput();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_DryContact;}
    virtual int getElementCount() {return 1;}

    virtual void updateOutputs();
    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element) {return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

    virtual void setDryContactState(bool enable);

    virtual void startRecovery();

};

#endif /* DRYCONTACTDIGITALOUTPUT_H_ */
