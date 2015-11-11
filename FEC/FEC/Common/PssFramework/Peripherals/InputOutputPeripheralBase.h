/*
 * InputOutputDeviceBase.h
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#ifndef INPUTOUTPUTDEVICEBASE_H_
#define INPUTOUTPUTDEVICEBASE_H_

#include "InputPeripheralBase.h"
#include "OutputPeripheralBase.h"

class InputOutputPeripheralBase: public InputPeripheralBase, public OutputPeripheralBase
{
public:
    InputOutputPeripheralBase();
    virtual ~InputOutputPeripheralBase();

    virtual void execute();

    virtual void setPssId(uint16_t deviceId) {InputPeripheralBase::setPssId(deviceId);}
    virtual uint16_t getPssId() const {return InputPeripheralBase::getPssId();}
    virtual void setUpdateInterval(uint16_t updateInterval) {InputPeripheralBase::setUpdateInterval(updateInterval);}

};

#endif /* INPUTOUTPUTDEVICEBASE_H_ */
