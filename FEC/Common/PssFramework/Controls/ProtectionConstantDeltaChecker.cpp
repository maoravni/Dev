/*
 * ProtectionConstantDeltaChecker.cpp
 *
 *  Created on: 3 Sep 2014
 *      Author: maora
 */

#include "Controls/ProtectionConstantDeltaChecker.h"
#include <Persistency/ControlSerializers.h>

ProtectionConstantDeltaChecker::ProtectionConstantDeltaChecker()
{
    m_referenceElement = NULL;
}

ProtectionConstantDeltaChecker::~ProtectionConstantDeltaChecker()
{
    m_referenceElement->removeObserver(this);
}

void ProtectionConstantDeltaChecker::setReferenceElement(ElementBase* element)
{
    m_referenceElement = element;
    m_referenceElement->addObserver(this);
}

void ProtectionConstantDeltaChecker::setAllowedDelta(float low, float high)
{
    // since we subtract the reference from the observed, we negate the low limit,
    // and check the difference against the range.
    m_allowedRange.updateRange(-low, high, true, true);
}

void ProtectionConstantDeltaChecker::updateNotification(ElementBase* element)
{
    E_DeviceProtectionState state;

    state = E_DeviceProtectionState_InRange;

    if (!element->isValid())
    {
        if (m_observedElement->getMissingDevicePriority() == E_MissingDevicePriority_High)
            state = E_DeviceProtectionState_SoftLimitExceeded;
        else
            state = E_DeviceProtectionState_InRange;
    }
    else
    {
        state = calcProtectionState(element);
    }

    setNewProtectionState(state);
}

E_DeviceProtectionState ProtectionConstantDeltaChecker::calcProtectionState(ElementBase* element)
{
    E_DeviceProtectionState state;

    state = E_DeviceProtectionState_InRange;

    float reference = m_referenceElement->getValueF();
    float observed = m_observedElement->getValueF();
    float delta = observed - reference;

    if (!m_referenceElement->isValid() || !m_observedElement->isValid())
    {
        if (m_observedElement->getMissingDevicePriority() == E_MissingDevicePriority_High)
            state = E_DeviceProtectionState_SoftLimitExceeded;
        else
            state = E_DeviceProtectionState_InRange;
    }
    else if (m_allowedRange.inRange(delta))
        state = E_DeviceProtectionState_InRange;
    else
        state = E_DeviceProtectionState_SoftLimitExceeded;

    return state;
}

void ProtectionConstantDeltaChecker::serialize(F_FILE* f)
{
    Serializer<ProtectionConstantDeltaChecker> s;
    s.serialize(f, *this);
}

ProtectionConstantDeltaChecker::ProtectionConstantDeltaChecker(F_FILE* f):ProtectionCheckerBase(f)
{
    Serializer<ProtectionConstantDeltaChecker> s;
    s.deserialize(f, *this);
    setReferenceElement(m_referenceElement);
}
