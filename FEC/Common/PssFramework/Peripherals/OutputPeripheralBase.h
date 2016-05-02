/*
 * OutputDeviceBase.h
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#ifndef OUTPUTDEVICEBASE_H_
#define OUTPUTDEVICEBASE_H_

#include "PeripheralBase.h"
#include <Elements/IObserver.h>

class OutputPeripheralBase: public virtual PeripheralBase, public IObserver
{

public:
    OutputPeripheralBase();
    virtual ~OutputPeripheralBase();

    virtual void updateOutputs() = 0;
    virtual void execute();

};

#endif /* OUTPUTDEVICEBASE_H_ */
