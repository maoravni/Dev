/*
 * InputDeviceBase.h
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#ifndef INPUTDEVICEBASE_H_
#define INPUTDEVICEBASE_H_

#include "PeripheralBase.h"

class InputPeripheralBase: public PeripheralBase
{
public:
    InputPeripheralBase();
    virtual ~InputPeripheralBase();

    virtual void readInputs() = 0;
    virtual void execute();

};

#endif /* INPUTDEVICEBASE_H_ */
