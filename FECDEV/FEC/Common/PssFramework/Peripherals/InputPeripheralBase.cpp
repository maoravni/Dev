/*
 * InputDeviceBase.cpp
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#include "InputPeripheralBase.h"

InputPeripheralBase::InputPeripheralBase():PeripheralBase()
{
}

InputPeripheralBase::~InputPeripheralBase()
{
}

void InputPeripheralBase::execute()
{
    readInputs();
}
