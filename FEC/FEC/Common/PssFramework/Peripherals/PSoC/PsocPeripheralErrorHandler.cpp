/*
 * PsocPeripheralErrorHandler.cpp
 *
 *  Created on: 15 May 2015
 *      Author: maora
 */

#include <Peripherals/PSoC/PsocPeripheralErrorHandler.h>

PsocPeripheralErrorHandler::PsocPeripheralErrorHandler()
{
    m_currentErrors = 0;
    m_currentWarnings = 0;
}

PsocPeripheralErrorHandler::~PsocPeripheralErrorHandler()
{
    // TODO Auto-generated destructor stub
}

