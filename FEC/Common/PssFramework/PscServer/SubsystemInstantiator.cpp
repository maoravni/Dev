/*
 * SubsystemInstantiator.cpp
 *
 *  Created on: 7 Jul 2013
 *      Author: maora
 */

#include "SubsystemInstantiator.h"
#include <Peripherals/PeripheralRepository.h>
#include <Peripherals/ModbusPumaPeripheral.h>
#include <Peripherals/Modbus8TCPeripheral.h>
#include <Peripherals/SwPwmOutputPeripheral.h>
#include <Controls/PidControl.h>

SubsystemInstantiator::SubsystemInstantiator()
{

}

SubsystemInstantiator::~SubsystemInstantiator()
{
}

void SubsystemInstantiator::ith()
{
    int i;


    // add a PUMA:
    ModbusPumaPeripheral* puma = new ModbusPumaPeripheral(1);
    puma->setPssId(1000);
    puma->initPuma();

    for (i = 0; i < M_PUMA_NUM_OF_SENSORS; ++i)
        m_temperatureSensorArray.push_back(static_cast<ValidationElementFloat*>(puma->getElementByIndex(i)));

    // add 8tc
    Modbus8TCPeripheral* mb8tc = new Modbus8TCPeripheral(34);
    mb8tc->setPssId(1001);
    for (i = 0; i < M_8TC_NUM_OF_SENSORS; ++i)
        m_temperatureSensorArray.push_back(static_cast<ValidationElementFloat*>(mb8tc->getElementByIndex(i)));

//    return;
    // config the sw pwm channels:
    PeripheralRepository::getInstance().initInternalPeripherals();
    SwPwmOutputPeripheral* swpwm = PeripheralRepository::getInstance().getSwPwmOutputPeripheral();
    swpwm->setPssId(1002);

    swpwm->configPwmChannel(0, 50, 0);
    swpwm->configPwmChannel(1, 50, 0);
    swpwm->configPwmChannel(2, 50, 0);
    swpwm->configPwmChannel(3, 50, 0);
    swpwm->configPwmChannel(4, 100, 1);
    swpwm->configPwmChannel(5, 100, 1);

    PidControl* pidControl;

//    pidControl = new PidControl();
//    pidControl->setElementInput(static_cast<ValidationElementFloat*>(puma->getElementByIndex(0)));
//    pidControl->setElementOutput(static_cast<ValidationElementFloat*>(swpwm->getElementByIndex(0)));
//    pidControl->setElementSetpoint(ElementRepository::getInstance().addValidationElementFloat());
//    m_pidControlArray.push_back(pidControl);
//
//    pidControl = new PidControl();
//    pidControl->setElementInput(static_cast<ValidationElementFloat*>(puma->getElementByIndex(1)));
//    pidControl->setElementOutput(static_cast<ValidationElementFloat*>(swpwm->getElementByIndex(1)));
//    pidControl->setElementSetpoint(ElementRepository::getInstance().addValidationElementFloat());
//    m_pidControlArray.push_back(pidControl);
//
//    pidControl = new PidControl();
//    pidControl->setElementInput(static_cast<ValidationElementFloat*>(puma->getElementByIndex(2)));
//    pidControl->setElementOutput(static_cast<ValidationElementFloat*>(swpwm->getElementByIndex(2)));
//    pidControl->setElementSetpoint(ElementRepository::getInstance().addValidationElementFloat());
//    m_pidControlArray.push_back(pidControl);
//
//    pidControl = new PidControl();
//    pidControl->setElementInput(static_cast<ValidationElementFloat*>(puma->getElementByIndex(3)));
//    pidControl->setElementOutput(static_cast<ValidationElementFloat*>(swpwm->getElementByIndex(3)));
//    pidControl->setElementSetpoint(ElementRepository::getInstance().addValidationElementFloat());
//    m_pidControlArray.push_back(pidControl);
//
//    pidControl = new PidControl();
//    pidControl->setElementInput(static_cast<ValidationElementFloat*>(mb8tc->getElementByIndex(5)));
//    pidControl->setElementOutput(static_cast<ValidationElementFloat*>(swpwm->getElementByIndex(4)));
//    pidControl->setElementSetpoint(ElementRepository::getInstance().addValidationElementFloat());
//    m_pidControlArray.push_back(pidControl);
//
//    pidControl = new PidControl();
//    pidControl->setElementInput(static_cast<ValidationElementFloat*>(mb8tc->getElementByIndex(6)));
//    pidControl->setElementOutput(static_cast<ValidationElementFloat*>(swpwm->getElementByIndex(5)));
//    pidControl->setElementSetpoint(ElementRepository::getInstance().addValidationElementFloat());
//    m_pidControlArray.push_back(pidControl);

//    AirBleedingControl *valve = new AirBleedingControl();
//    valve->setOutputElements(static_cast<ElementU8*>(PeripheralRepository::getInstance().getDigitalOutputsPeripheral()->getElementByIndex(7)),
//            static_cast<ElementU8*>(PeripheralRepository::getInstance().getDigitalOutputsPeripheral()->getElementByIndex(8)));
//    m_airBleedingControlArray.push_back(valve);
//
//    valve = new AirBleedingControl();
//    valve->setOutputElements(static_cast<ElementU8*>(PeripheralRepository::getInstance().getDigitalOutputsPeripheral()->getElementByIndex(9)),
//            static_cast<ElementU8*>(PeripheralRepository::getInstance().getDigitalOutputsPeripheral()->getElementByIndex(10)));
//    m_airBleedingControlArray.push_back(valve);

}

void SubsystemInstantiator::ibt()
{
}

void SubsystemInstantiator::dryer()
{
}

void SubsystemInstantiator::btc()
{
}

PidControl* SubsystemInstantiator::getPid(int index)
{
    if (index >= m_pidControlArray.size())
        return NULL;

    return m_pidControlArray[index];
}

ValidationElementFloat* SubsystemInstantiator::getTemperatureSensor(int index)
{
    if (index >= m_temperatureSensorArray.size())
        return NULL;

    return m_temperatureSensorArray[index];
}

AirBleedingControl* SubsystemInstantiator::getAirBleedingControl(int index)
{
    if (index >= m_airBleedingControlArray.size())
        return NULL;

    return m_airBleedingControlArray[index];
}
