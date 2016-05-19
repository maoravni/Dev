/*
 * DeviceProtectionChecker.cpp
 *
 *  Created on: 1 αιεπ 2014
 *      Author: maora
 */

#include "DeviceProtectionChecker.h"
#include <Tasks/UpdateSchedulerTask.h>
#include <Persistency/ControlSerializers.h>

DeviceProtectionChecker::DeviceProtectionChecker()
{
//    m_observedElement = NULL;
//    m_protectionStatus = ElementRepository::getInstance().addElementU8();
//    *m_protectionStatus = 0;
}

DeviceProtectionChecker::~DeviceProtectionChecker()
{
}

//void DeviceProtectionChecker::setElement(ElementBase* element)
//{
////    m_observedElement = element;
////    m_observedElement->addObserver(this);
//}

void DeviceProtectionChecker::updateSoftProtectionRange(float low, float high, bool useLow, bool useHigh)
{
    m_softProtectionRange.updateRange(low, high, useLow, useHigh);
}

void DeviceProtectionChecker::updateHardProtectionRange(float low, float high, bool useLow, bool useHigh)
{
    m_hardProtectionRange.updateRange(low, high, useLow, useHigh);
}

//void DeviceProtectionChecker::addObserver(IObserver* observer)
//{
//    m_protectionStatus->addObserver(observer);
//}

void DeviceProtectionChecker::updateNotification(ElementBase* element)
{
    E_DeviceProtectionState state;

    state = E_DeviceProtectionState_InRange;
    // TODO: When the observed element is invalid, the protection state should also be changed to invalid.
    // This, combined with the missing device priority (which the two elements should have the same settings)
    // will enable a control to decide itself what to do when a protection/malfunction occurs.
    if (m_observedElement->isValid())
    {
        state = calcProtectionState(element);
    }

    setNewProtectionState(state);
}

E_DeviceProtectionState DeviceProtectionChecker::calcProtectionState(ElementBase* element)
{
    E_DeviceProtectionState state;
    if (isOutsideHardLimit())
        state = E_DeviceProtectionState_HardLimitExceeded;
    else if (isOutsideSoftLimit())
        state = E_DeviceProtectionState_SoftLimitExceeded;
    else
        state = E_DeviceProtectionState_InRange;
    return state;
}

bool DeviceProtectionChecker::requestValueChange(ElementBase* element)
{
    return true;
}

void DeviceProtectionChecker::serialize(F_FILE* f)
{
    Serializer<DeviceProtectionChecker> s;
    s.serialize(f, *this);
}
