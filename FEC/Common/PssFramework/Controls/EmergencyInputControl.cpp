/*
 * EmergencyInputControl.cpp
 *
 *  Created on: 29 באפר 2014
 *      Author: maora
 */

#include "Controls/EmergencyInputControl.h"
#include <Elements/ElementRepository.h>
#include <Peripherals/PeripheralRepository.h>
#include <Controls/ControlRepository.h>
#include <dg_input_cnfg.h>
#include <PscServer/PscMessageHandler.h>
#include <Persistency/ControlSerializers.h>

#define M_EMERGENCY_INPUT_INDEX 5

EmergencyInputControl::EmergencyInputControl()
{
    DigitalInputsPeripheral *digitalInputs = PeripheralRepository::getInstance().getDigitalInputsPeripheral();
    if (digitalInputs == NULL)
    {
        PeripheralRepository::getInstance().initDigitalInputs(0, M_EMERGENCY_INPUT_INDEX);
        digitalInputs = PeripheralRepository::getInstance().getDigitalInputsPeripheral();
    }
    assert(digitalInputs != NULL);

    setEmergencyInputElement(digitalInputs->getElementByIndex(M_EMERGENCY_INPUT_INDEX));
    m_emergencyActive = false;
}

EmergencyInputControl::~EmergencyInputControl()
{
    m_emergencyInputElement->removeObserver(this);
}

void EmergencyInputControl::setEmergencyInputElement(ElementBase* emergencyInputElement)
{
    m_emergencyInputElement = emergencyInputElement;
    m_emergencyInputElement->addObserver(this);
}

bool EmergencyInputControl::onInitControl()
{
    endInitControl();
    return true;
}

bool EmergencyInputControl::onMove2Standby(uint32_t delay)
{
    endMove2Standby();
    return true;
}

bool EmergencyInputControl::onReset2On()
{
    endReset2On();
    return true;
}

bool EmergencyInputControl::onMove2Error()
{
    endMove2Error();
    return true;
}

void EmergencyInputControl::execute()
{
}

bool EmergencyInputControl::sendNotification()
{
    return true;
}

void EmergencyInputControl::updateNotification(ElementBase* element)
{
    // emr input is normally closed.
    if (m_emergencyActive && m_emergencyInputElement->getValueI32() == 1)
    {
        m_emergencyActive = false;
//        PscMessageHandler::getInstance()->recoverFromEmrError();
    }
    if (!m_emergencyActive && m_emergencyInputElement->getValueI32() == 0)
    {
        m_emergencyActive = true;
        PscMessageHandler::getInstance()->stopOnEmr();
    }
//        ControlRepository::getInstance().resetAllControlsToOn();
}

bool EmergencyInputControl::onStopOnEmr()
{
    endStopOnEmr();
    return true;
}

bool EmergencyInputControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

bool EmergencyInputControl::isEmergencyActive()
{
    return (m_emergencyActive);
}

void EmergencyInputControl::serialize(F_FILE* f)
{
    Serializer<EmergencyInputControl> s;
    s.serialize(f, *this);
}
