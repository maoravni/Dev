/*
 * PsocDigitalOutputPeripheral.cpp
 *
 *  Created on: 12 αιπε 2014
 *      Author: maora
 */

#include "PsocDigitalOutputPeripheral.h"
#include "PsocHandler.h"
#include <Peripherals/PeripheralRepository.h>
#include "Persistency/PsocPeripheralSerializers.h"

#define M_OUTPUT_VALUE 1

PsocDigitalOutputPeripheral::PsocDigitalOutputPeripheral()
{
    m_psocHandler = NULL;

    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        m_outputElementArray[i] = ElementRepository::getInstance().addValidationElementU8();
        m_outputElementArray[i]->addObserver(this);
    }

    m_firstElementIndex = m_outputElementArray[0]->getElementIndex();
}

PsocDigitalOutputPeripheral::~PsocDigitalOutputPeripheral()
{
    int outValue;
    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        m_psocHandler->addPendingPwmOutputValue(i, 0);
    }
}

ElementBase* PsocDigitalOutputPeripheral::getElementByIndex(int index)
{
    return m_outputElementArray[index];
}

void PsocDigitalOutputPeripheral::enableElementByIndex(int index, bool enable)
{
}

void PsocDigitalOutputPeripheral::updateOutputs()
{
    int outValue;
    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        outValue = (m_outputElementArray[i]->getValueU32() != 0) ? M_OUTPUT_VALUE : 0;

        m_psocHandler->addPendingPwmOutputValue(i, outValue);
    }
}

void PsocDigitalOutputPeripheral::updateNotification(ElementBase* element)
{
    int i = element->getElementIndex() - m_firstElementIndex;
    int outValue = (m_outputElementArray[i]->getValueU32() != 0) ? M_OUTPUT_VALUE : 0;
    m_psocHandler->addPendingPwmOutputValue(i, outValue);
}

ElementBase* PsocDigitalOutputPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        if (m_outputElementArray[i]->getPssId() == pssId)
            return m_outputElementArray[i];
    }
    return NULL;
}

void PsocDigitalOutputPeripheral::setPsocHandler(PsocHandler* psocHandler)
{
    m_psocHandler = psocHandler;
}

void PsocDigitalOutputPeripheral::setPsocOutputsEnabled(bool enabled)
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "psoc %d Digital Outputs status is %d", m_psocHandler->getPsocIndex(), enabled);
    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        if (m_outputElementArray[i]->isValid() != enabled)
            m_outputElementArray[i]->setValueValid(enabled);
    }
}

void PsocDigitalOutputPeripheral::raiseError(E_PsocErrorBits error, bool state)
{
    int i = 0;
    switch (error)
    {
    case E_PsocErrorBits_CableIdNotMatching:
        for (i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
        {
            m_outputElementArray[i]->updateErrorBits(E_PSSErrors_CableIdNotMatching, state);
        }
        break;
    case E_PsocErrorBits_SsrOverheating:
        m_outputElementArray[0]->updateErrorBits(E_PSSErrors_SsrOverheating, state);
        break;
    case E_PsocErrorBits_SsrAlwaysOn:
        m_outputElementArray[0]->updateErrorBits(E_PSSErrors_SsrAlwaysOn, state);
        PeripheralRepository::getInstance().getDryContactOutput()->setDryContactState(false);
        break;
    }
}

void PsocDigitalOutputPeripheral::serialize(F_FILE* f)
{
    Serializer<PsocDigitalOutputPeripheral> s;
    s.serialize(f, *this);
}

PsocDigitalOutputPeripheral::PsocDigitalOutputPeripheral(F_FILE* f)
{
    Serializer<PsocDigitalOutputPeripheral> s;
    s.deserialize(f, *this);
}

