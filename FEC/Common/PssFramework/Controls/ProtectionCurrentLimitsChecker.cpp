/*
 * ProtectionCurrentLimitsChecker.cpp
 *
 *  Created on: 6 Aug 2015
 *      Author: maora
 */

#include <Controls/ProtectionCurrentLimitsChecker.h>
#include <Persistency/ControlSerializers.h>

ProtectionCurrentLimitsChecker::ProtectionCurrentLimitsChecker()
{
//    m_observedElement = NULL;
//    m_protectionStatus = ElementRepository::getInstance().addElementU8();
//    *m_protectionStatus = 0;
}

ProtectionCurrentLimitsChecker::~ProtectionCurrentLimitsChecker()
{
}

//void ProtectionCurrentLimitsChecker::setElement(ElementBase* element)
//{
////    m_observedElement = element;
////    m_observedElement->addObserver(this);
//}

void ProtectionCurrentLimitsChecker::setLimits(float lowWarning, float lowError, float high)
{
    m_allowedRangeError.updateRange(lowError, high, true, true);
    m_allowedRangeWarning.updateRange(lowWarning, high, true, true);
}

void ProtectionCurrentLimitsChecker::updateNotification(ElementBase* element)
{
    E_DeviceProtectionState state;

    state = E_DeviceProtectionState_InRange;
    if (m_observedElement->isValid())
    {
        state = calcProtectionState(element);
    }
    else
    {
        if (m_observedElement->getMissingDevicePriority() == E_MissingDevicePriority_High)
            state = E_DeviceProtectionState_SoftLimitExceeded;
        else
            state = E_DeviceProtectionState_InRange;
    }

    setNewProtectionState(state);
}

E_DeviceProtectionState ProtectionCurrentLimitsChecker::calcProtectionState(ElementBase* element)
{
    E_DeviceProtectionState state;
    // If the current is "0", it means that either the output is closed, which will raise the error "PsocOutputsNotOpen",
    // or the output is intentionally closed, which is fine.
    // We use "getValueI32" to force a cast.
    if (element->getValueI32() == 0)
        state = E_DeviceProtectionState_InRange;
    else if (m_allowedRangeError.aboveMaxRange(element->getValueF()))
        state = E_DeviceProtectionState_HardLimitExceeded;
    else if (m_allowedRangeWarning.belowMinRange(element->getValueF()))
        state = E_DeviceProtectionState_SoftLimitExceededWarning;
    else if (m_allowedRangeError.belowMinRange(element->getValueF()))
        state = E_DeviceProtectionState_SoftLimitExceeded;
    else
        state = E_DeviceProtectionState_InRange;
    return state;
}

void ProtectionCurrentLimitsChecker::serialize(F_FILE* f)
{
    Serializer<ProtectionCurrentLimitsChecker> s;
    s.serialize(f, *this);
}

ProtectionCurrentLimitsChecker::ProtectionCurrentLimitsChecker(F_FILE* f):ProtectionCheckerBase(f)
{
    Serializer<ProtectionCurrentLimitsChecker> s;
    s.deserialize(f, *this);
}
