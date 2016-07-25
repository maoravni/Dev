/*
 * AnalogOutCurrentPeripheral.cpp
 *
 *  Created on: 6 баев 2013
 *      Author: maora
 */

#include "AnalogOutCurrentPeripheral.h"
#include <Elements/ElementRepository.h>
#ifndef WIN32
#include <DigTrimmer_ad5292bruz.h>
#endif
#include <assert.h>
#include <Persistency/PeripheralSerializers.h>

AnalogOutCurrentPeripheral::AnalogOutCurrentPeripheral()
{
    for (int i = 0; i < M_NUM_OF_MA_OUT; ++i)
    {
        m_elementArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_elementArray[i]->addObserver(this);
        m_elementArray[i]->updateAllowedRange(4, 20, true, true);
    }

    m_firstElementIndex = m_elementArray[0]->getElementIndex();
}

AnalogOutCurrentPeripheral::~AnalogOutCurrentPeripheral()
{
    for (int i = 0; i < M_NUM_OF_MA_OUT; ++i)
    {
        m_elementArray[i]->removeObserver(this);
    }
}

void AnalogOutCurrentPeripheral::updateOutputs()
{
//    for (int i = 0; i < M_NUM_OF_MA_OUT; ++i)
//        Set_AO_mA(i, m_elementArray[i]->getValue()*10);
}

void AnalogOutCurrentPeripheral::updateNotification(ElementBase* element)
{
    uint16_t elementIndex = element->getElementIndex()-m_firstElementIndex;

//    Set_AO_mA(elementIndex, element->getValueF()*10);
}

ElementBase* AnalogOutCurrentPeripheral::getElementByIndex(int index)
{
    if (index >= M_NUM_OF_MA_OUT)
        return NULL;

    return m_elementArray[index];
}

ElementBase* AnalogOutCurrentPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_MA_OUT; ++i)
    {
        if (m_elementArray[i]->getPssId() == pssId)
            return m_elementArray[i];
    }
    return NULL;
}

void AnalogOutCurrentPeripheral::enableElementByIndex(int index, bool enable)
{
}

AnalogOutCurrentPeripheral::AnalogOutCurrentPeripheral(F_FILE* f)
{
    Serializer<AnalogOutCurrentPeripheral> s;
    s.deserialize(f, *this);

    for (int i = 0; i < M_NUM_OF_MA_OUT; ++i)
    {
        m_elementArray[i]->addObserver(this);
    }
}

void AnalogOutCurrentPeripheral::serialize(F_FILE* f)
{
    Serializer<AnalogOutCurrentPeripheral> s;
    s.serialize(f, *this);
}
