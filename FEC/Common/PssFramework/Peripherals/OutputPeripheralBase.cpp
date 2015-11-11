/*
 * OutputDeviceBase.cpp
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#include "OutputPeripheralBase.h"

OutputPeripheralBase::OutputPeripheralBase()
{
}

OutputPeripheralBase::~OutputPeripheralBase()
{
}

void OutputPeripheralBase::execute()
{
    updateOutputs();
}

