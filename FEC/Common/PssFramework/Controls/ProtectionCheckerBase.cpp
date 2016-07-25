/*
 * ProtectionCheckerBase.cpp
 *
 *  Created on: 3 Sep 2014
 *      Author: maora
 */

#include "Controls/ProtectionCheckerBase.h"
#include <Tasks/UpdateSchedulerTask.h>

ProtectionCheckerBase::ProtectionCheckerBase()
{
    m_observedElement = NULL;
    m_protectionStatus = ElementRepository::getInstance().addElementU8();
    m_protectionStatus->setValue((uint32_t)0);
    m_debounceTimeout = 0;
    m_debounceTimeoutStarted = false;
}

ProtectionCheckerBase::ProtectionCheckerBase(F_FILE* f)
{
    m_observedElement = NULL;
    m_debounceTimeout = 0;
    m_debounceTimeoutStarted = false;
}

ProtectionCheckerBase::~ProtectionCheckerBase()
{
    m_observedElement->removeObserver(this);
}

void ProtectionCheckerBase::addObserver(IObserver* observer)
{
    m_protectionStatus->addObserver(observer);
}

void ProtectionCheckerBase::setElement(ElementBase* element)
{
    m_observedElement = element;
    m_protectionStatus->setPssId(element->getPssId());
//    setSecondaryPssId(element->getPssId());
    m_observedElement->addObserver(this);
}

uint16_t ProtectionCheckerBase::getPssId()
{
    uint16_t result;
    // TODO: Why was this code necessary?
    if ((result = m_protectionStatus->getPssId()) != m_observedElement->getPssId())
    {
        m_protectionStatus->setPssId(m_observedElement->getPssId());
//        setSecondaryPssId(m_observedElement->getPssId());
    }

    return result;
}

void ProtectionCheckerBase::setNewProtectionState(E_DeviceProtectionState state)
{
    if (state != m_protectionStatus->getValueU32())
    {
        if (m_debounceTimeout == 0)
        {
            m_protectionStatus->setValue(state);
        }
        else
        {
            if (m_debounceTimeoutStarted == false)
            {
                if (state == E_DeviceProtectionState_InRange)
                {
                    m_protectionStatus->setValue(state);
                }
                else
                {
                    m_debounceTimeoutStarted = true;
                    UpdateSchedulerTask::getInstance()->addProtectionDebounceTimeout(this, m_debounceTimeout*1000);
                }
            }
        }
    }
}

void ProtectionCheckerBase::timeoutExpired(uint16_t timeoutType)
{
    E_DeviceProtectionState state;

    m_debounceTimeoutStarted = false;

    state = calcProtectionState(m_observedElement);

    if (state != m_protectionStatus->getValueU32())
        m_protectionStatus->setValue(state);
}

void ProtectionCheckerBase::setProtectionIndex(uint16_t index)
{
    m_protectionIndex = index;
}

