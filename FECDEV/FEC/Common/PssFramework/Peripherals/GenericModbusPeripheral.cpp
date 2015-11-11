/*
 * GenericModbusPeripheral.cpp
 *
 *  Created on: 20 Jun 2013
 *      Author: maora
 */

#include "GenericModbusPeripheral.h"

GenericModbusPeripheral::GenericModbusPeripheral(uint16_t startAddress, uint16_t numOfElements)
{
    m_startAddress = startAddress;
}

GenericModbusPeripheral::~GenericModbusPeripheral()
{
}
