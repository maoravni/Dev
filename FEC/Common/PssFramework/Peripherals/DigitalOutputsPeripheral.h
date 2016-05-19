/*
 * DigitalOutputsDevice.h
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#ifndef DIGITALOUTPUTSDEVICE_H_
#define DIGITALOUTPUTSDEVICE_H_

#include "OutputPeripheralBase.h"
#include <Elements/Element.h>

#ifdef FEC2_BOARD
#define M_NUM_OF_DIGITAL_OUT 8
#else
#define M_NUM_OF_DIGITAL_OUT 12
#endif

class DigitalOutputsPeripheral: public OutputPeripheralBase
{
    ElementU8* m_elementArray[M_NUM_OF_DIGITAL_OUT];
    bool m_enabledArray[M_NUM_OF_DIGITAL_OUT];

    uint16_t m_firstElementIndex;

public:
    DigitalOutputsPeripheral();
    DigitalOutputsPeripheral(F_FILE* f);
    virtual ~DigitalOutputsPeripheral();

    // TODO: need to be able to config a digital output back to a digital output?
    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_DO;}
    virtual int getElementCount() {return M_NUM_OF_DIGITAL_OUT;}

    virtual void updateOutputs();
    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;

};

#endif /* DIGITALOUTPUTSDEVICE_H_ */
