/*
 * GenericModbusPeripheral.h
 *
 *  Created on: 20 Jun 2013
 *      Author: maora
 */

#ifndef GENERICMODBUSPERIPHERAL_H_
#define GENERICMODBUSPERIPHERAL_H_

#include "ModbusPeripheralBase.h"

class GenericModbusPeripheral//: public ModbusPeripheralBase
{
    uint16_t m_startAddress;
public:
    GenericModbusPeripheral(uint16_t startAddress, uint16_t numOfElements);
    virtual ~GenericModbusPeripheral();
};

#endif /* GENERICMODBUSPERIPHERAL_H_ */
