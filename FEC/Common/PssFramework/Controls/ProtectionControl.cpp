/*
 * ProtectionControl.cpp
 *
 *  Created on: 24 αιεμ 2013
 *      Author: maora
 */

#include "ProtectionControl.h"
#include <Peripherals/PeripheralRepository.h>

ProtectionControl::ProtectionControl()
{
    PeripheralRepository::getInstance().initDryContactOutput(0);
    m_dryContactElement = PeripheralRepository::getInstance().getDryContactOutput()->getElementByIndex(0);

    *m_dryContactElement = 1;
}

ProtectionControl::~ProtectionControl()
{
    T_ProtectionCheckerListIterator i;
    for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
    {
        delete (*i);
    }

    m_protectionCheckers.clear();
}

void ProtectionControl::execute()
{
}

bool ProtectionControl::sendNotification()
{
	return true;
}

void ProtectionControl::updateNotification(ElementBase* element)
{
#ifdef OLD_PROTECTION
    ValidationElementBase *validationElement = static_cast<ValidationElementBase*>(element);

    // if the value is in the error range:
    if (validationElement->isOutsideHardLimit())
    *m_dryContactElement = 0;
#else
    if (element->getValueU32() == E_DeviceProtectionState_HardLimitExceeded)
        *m_dryContactElement = 0;
#endif
}

//void ProtectionControl::addProtectionElement(ValidationElementBase* element)
//{
//    m_observedElementArray.push_back(element);
//    element->addObserver(this);
//}

DeviceProtectionChecker* ProtectionControl::createDeviceProtectionChecker()
{
    DeviceProtectionChecker* deviceChecker = new DeviceProtectionChecker();
    addProtectionChecker(deviceChecker);
    return deviceChecker;
}

bool ProtectionControl::getElementUpperHardLimit(ElementBase* element, float& limit)
{
    bool result;
    DeviceProtectionChecker* deviceChecker = getElementInProtection(E_ProtectionCheckerType_AbsoluteValue, element);
    if (deviceChecker == NULL)
        return false;
    limit = deviceChecker->getUpperHardLimit();
    return true;
}

bool ProtectionControl::getElementLowerHardLimit(ElementBase* element, float& limit)
{
    bool result;
    DeviceProtectionChecker* deviceChecker = getElementInProtection(E_ProtectionCheckerType_AbsoluteValue, element);
    if (deviceChecker == NULL)
        return false;
    limit = deviceChecker->getLowerHardLimit();
    return true;
}

void ProtectionControl::addProtectionCheckerButDontSubsribe(ProtectionCheckerBase* protection)
{
    m_protectionCheckers.push_back(protection);
}

DeviceProtectionChecker* ProtectionControl::getElementInProtection(E_ProtectionCheckerType checkerType, ElementBase* element)
{
    T_ProtectionCheckerListIterator i;
    ElementBase* foundElement;
    for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
    {
        if ((*i)->getProtectionType() != checkerType)
            continue;
        foundElement = (*i)->getProtectionElement();
        int a = element->getPssId();
        int b = foundElement->getPssId();
        if (element == foundElement)
            return static_cast<DeviceProtectionChecker*>(*i);
    }
    return NULL;
}

bool ProtectionControl::onStopOnEmr()
{
    endStopOnEmr();
    return true;
}

bool ProtectionControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

void ProtectionControl::startRecovery()
{
    *m_dryContactElement = 1;
}
