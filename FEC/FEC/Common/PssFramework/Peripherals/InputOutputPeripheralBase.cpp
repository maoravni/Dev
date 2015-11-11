/*
 * InputOutputDeviceBase.cpp
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#include "InputOutputPeripheralBase.h"

InputOutputPeripheralBase::InputOutputPeripheralBase()
{

}

InputOutputPeripheralBase::~InputOutputPeripheralBase()
{
}

void InputOutputPeripheralBase::execute()
{
    readInputs();
    updateOutputs();
}
