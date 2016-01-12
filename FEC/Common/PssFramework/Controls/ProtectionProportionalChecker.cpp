/*
 * ProtectionProportionalChecker.cpp
 *
 *  Created on: 3 Sep 2014
 *      Author: maora
 */

#include "Controls/ProtectionProportionalChecker.h"

ProtectionProportionalChecker::ProtectionProportionalChecker()
{
    m_referenceElement = NULL;
    m_gain = 1;
    m_offset = 0;
}

ProtectionProportionalChecker::~ProtectionProportionalChecker()
{
    m_referenceElement->removeObserver(this);
}

void ProtectionProportionalChecker::setReferenceElement(ElementBase* element)
{
    m_referenceElement = element;
    m_referenceElement->addObserver(this);
}

void ProtectionProportionalChecker::updateParameters(float gain, float offset, float low, float high)
{
    // since we divide the input by the reference, we get the percentage.
    // we then check if the ratio is in the limits.
    m_allowedRange.updateRange(low, high, true, true);
    m_gain = gain;
    m_offset = offset;
}

void ProtectionProportionalChecker::updateNotification(ElementBase* element)
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

E_DeviceProtectionState ProtectionProportionalChecker::calcProtectionState(ElementBase* element)
{
    E_DeviceProtectionState state;
    state = E_DeviceProtectionState_InRange;

    float reference = m_referenceElement->getValueF();
    float observed = m_observedElement->getValueF();
    float ratio;
    if (!m_referenceElement->isValid() || !m_observedElement->isValid())
        state = E_DeviceProtectionState_SoftLimitExceeded;

    if (reference == 0)
        state = E_DeviceProtectionState_InRange;
    else
    {
        ratio = (100 * (observed * m_gain + m_offset)) / reference;

        if (m_allowedRange.inRange(ratio))
            state = E_DeviceProtectionState_InRange;
        else
            state = E_DeviceProtectionState_SoftLimitExceeded;
    }
    return state;
}
