/*
 * DryContactDigitalOutput.cpp
 *
 *  Created on: 24 αιεμ 2013
 *      Author: maora
 */

#include "DryContactDigitalOutput.h"
#include <Elements/ElementRepository.h>
#include <opto_out.h>
#include <leds.h>
#include <Persistency/PeripheralSerializers.h>

DryContactDigitalOutput::DryContactDigitalOutput()
{
    m_element = ElementRepository::getInstance().addElementU8();
    m_element->addObserver(this);
    m_elementIndex = m_element->getElementIndex();
}

DryContactDigitalOutput::~DryContactDigitalOutput()
{
    Safety_out_ctrl((OPTO_OUT_CTRL_VAL_ET) 0);
    Turn_led_(LABL_SAFETY_LED9, 0);
}

void DryContactDigitalOutput::updateOutputs()
{
    Safety_out_ctrl((OPTO_OUT_CTRL_VAL_ET) (m_element->getValue() != 0));
    Turn_led_(LABL_SAFETY_LED9, (m_element->getValue() != 0));
}

void DryContactDigitalOutput::updateNotification(ElementBase* element)
{
    if (element != m_element)
        return;

    updateOutputs();
}

ElementBase* DryContactDigitalOutput::getElementByIndex(int index)
{
    if (index != 0)
        return NULL;

    return m_element;
}

ElementBase* DryContactDigitalOutput::getElementByPssId(int pssId)
{
    if (pssId == m_element->getPssId())
        return m_element;
    return NULL;
}

void DryContactDigitalOutput::enableElementByIndex(int index, bool enable)
{

}

/**
 * Enable: true means the dry contact is closed, and everything is ok.
 *         false means the dry contact is open, and there's a safety event.
 */
void DryContactDigitalOutput::setDryContactState(bool enable)
{
    m_element->setValue(enable);
}

void DryContactDigitalOutput::startRecovery()
{
    setDryContactState(true);
}

DryContactDigitalOutput::DryContactDigitalOutput(F_FILE* f)
{
    Serializer<DryContactDigitalOutput> s;
    s.deserialize(f, *this);
    m_element->addObserver(this);
}

void DryContactDigitalOutput::serialize(F_FILE* f)
{
    Serializer<DryContactDigitalOutput> s;
    s.serialize(f, *this);
}
