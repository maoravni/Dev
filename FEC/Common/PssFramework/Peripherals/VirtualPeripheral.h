/*
 * VirtualPeripheral.h
 *
 *  Created on: 16 Sep 2015
 *      Author: maora
 */

#ifndef VIRTUALPERIPHERAL_H_
#define VIRTUALPERIPHERAL_H_

#include <Peripherals/PeripheralBase.h>
#include <Elements/ElementBase.h>

class VirtualPeripheral: public PeripheralBase
{
    E_PeripheralType m_peripheralType;
    T_ElementList m_elementList;

public:
    VirtualPeripheral(E_PeripheralType peripheralType);
    virtual ~VirtualPeripheral();

    virtual void execute();

    virtual E_PeripheralType getPeripheralType();
    virtual int getElementCount();

    virtual ElementBase* getElementByIndex(int index);
    virtual ElementBase* getElementByPssId(int pssId);

    /**
     * In some peripherals we want to have the option for the peripheral to ignore some of the elements.
     * Or in other cases, like the digital outputs/sw pwm/hw pwm, where they share outputs,
     * we want to have only one peripheral use the output.
     * @param index
     * @return
     */
    virtual void enableElementByIndex(int index, bool enable);
};

#endif /* VIRTUALPERIPHERAL_H_ */
