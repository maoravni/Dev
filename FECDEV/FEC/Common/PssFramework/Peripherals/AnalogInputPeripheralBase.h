/*
 * AnalogInputPeripheralBase.h
 *
 *  Created on: 8 αιπε 2014
 *      Author: maora
 */

#ifndef ANALOGINPUTPERIPHERALBASE_H_
#define ANALOGINPUTPERIPHERALBASE_H_

#include "InputPeripheralBase.h"

//! This is a base class for a peripheral that supports value correction.
class AnalogInputPeripheralBase: public InputPeripheralBase
{
public:
    AnalogInputPeripheralBase();
    virtual ~AnalogInputPeripheralBase();

    virtual void setCalibrationCoeff(int index, float aCoeff, float bCoeff) = 0;
    virtual void setScalingCoeff(int index, float aCoeff, float bCoeff) = 0;
    virtual void setSensorType(int index, uint8_t type) = 0;

};

#endif /* ANALOGINPUTPERIPHERALBASE_H_ */
