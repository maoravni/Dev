/*
 * DigitalOutputsDevice.cpp
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#include "DigitalOutputsPeripheral.h"

#include <Elements/ElementRepository.h>
#include <assert.h>
#include <opto_out.h>
#include <Persistency/PeripheralSerializers.h>


DigitalOutputsPeripheral::DigitalOutputsPeripheral()
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
//        ElementRepository::getInstance().addElement(&m_elementArray[i]);
        m_elementArray[i] = ElementRepository::getInstance().addElementU8();
        m_elementArray[i]->addObserver(this);
        m_enabledArray[i] = true;
    }

    m_firstElementIndex = m_elementArray[0]->getElementIndex();
}

DigitalOutputsPeripheral::~DigitalOutputsPeripheral()
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
//        if (m_enabledArray[i])
            dig_out_ctrl((OPTO_OUT_ID_ET) i, (OPTO_OUT_CTRL_VAL_ET) 0);
    }
}

void DigitalOutputsPeripheral::updateOutputs()
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
        if (m_enabledArray[i])
            dig_out_ctrl((OPTO_OUT_ID_ET) i, (OPTO_OUT_CTRL_VAL_ET) (m_elementArray[i]->getValue() != 0));
    }
}

void DigitalOutputsPeripheral::updateNotification(ElementBase* element)
{
    uint16_t elementIndex = element->getElementIndex()-m_firstElementIndex;

    if (m_enabledArray[elementIndex])
        dig_out_ctrl((OPTO_OUT_ID_ET) elementIndex, (OPTO_OUT_CTRL_VAL_ET) (m_elementArray[elementIndex]->getValue() != 0));

//    updateOutputs();
}

ElementBase* DigitalOutputsPeripheral::getElementByIndex(int index)
{
    assert(index < M_NUM_OF_DIGITAL_OUT);

    return m_elementArray[index];
}

ElementBase* DigitalOutputsPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
        if (m_elementArray[i]->getPssId() == pssId)
            return m_elementArray[i];
    }
    return NULL;
}

void DigitalOutputsPeripheral::enableElementByIndex(int index, bool enable)
{
    m_enabledArray[index] = enable;
}

DigitalOutputsPeripheral::DigitalOutputsPeripheral(F_FILE* f)
{
    Serializer<DigitalOutputsPeripheral> s;
    s.deserialize(f, *this);

    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
        m_elementArray[i]->addObserver(this);
    }
}

void DigitalOutputsPeripheral::serialize(F_FILE* f)
{
    Serializer<DigitalOutputsPeripheral> s;
    s.serialize(f, *this);

}
