/*
 * ModbusInverterSchneiderAtv32Control.h
 *
 *  Created on: 15 Aug 2016
 *      Author: maora
 */

#ifndef MODBUSINVERTERSCHNEIDERATV32CONTROL_H_
#define MODBUSINVERTERSCHNEIDERATV32CONTROL_H_

#include <Controls/ModbusInverterControl.h>
#include <Peripherals/ModbusInverterSchneiderAtv32.h>

class ModbusInverterSchneiderAtv32Control: public ModbusInverterControl
{
private:
    ModbusInverterSchneiderAtv32 *m_inverterPeriph;

public:
    ModbusInverterSchneiderAtv32Control();
    virtual ~ModbusInverterSchneiderAtv32Control();
};

#endif /* MODBUSINVERTERSCHNEIDERATV32CONTROL_H_ */
