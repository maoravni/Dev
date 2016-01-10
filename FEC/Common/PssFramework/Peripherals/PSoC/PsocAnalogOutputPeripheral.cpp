/*
 * PsocAnalogOutputPeripheral.cpp
 *
 *  Created on: 12 αιπε 2014
 *      Author: maora
 */

#include "PsocAnalogOutputPeripheral.h"
#include "PsocHandler.h"

PsocAnalogOutputPeripheral::PsocAnalogOutputPeripheral()
{
    m_psocHandler = NULL;
    for (int i = 0; i < M_NUM_OF_PSOC_ANALOG_OUTPUTS; ++i)
    {
        // TODO: Get data type and range of the analog outputs.
        m_elementArray[i] = ElementRepository::getInstance().addValidationElementU8();
        m_elementArray[i]->addObserver(this);
        m_elementArray[i]->updateAllowedRange(0, 250, true, true);
    }

    m_firstElementIndex = m_elementArray[0]->getElementIndex();
}

PsocAnalogOutputPeripheral::~PsocAnalogOutputPeripheral()
{
    for (int i = 0; i < M_NUM_OF_PSOC_ANALOG_OUTPUTS; ++i)
    {
        m_psocHandler->addPendingAnalogOutputValue(i, 0);
    }
}

ElementBase* PsocAnalogOutputPeripheral::getElementByIndex(int index)
{
    return m_elementArray[index];
}

void PsocAnalogOutputPeripheral::enableElementByIndex(int index, bool enable)
{
}

void PsocAnalogOutputPeripheral::updateOutputs()
{
    for (int i = 0; i < M_NUM_OF_PSOC_ANALOG_OUTPUTS; ++i)
    {
        m_psocHandler->addPendingAnalogOutputValue(i, m_elementArray[i]->getValueU32());
    }
}

void PsocAnalogOutputPeripheral::updateNotification(ElementBase* element)
{
    int i = element->getElementIndex() - m_firstElementIndex;
    m_psocHandler->addPendingAnalogOutputValue(i, element->getValueU32());
}

ElementBase* PsocAnalogOutputPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_PSOC_ANALOG_OUTPUTS; ++i)
    {
        if (m_elementArray[i]->getPssId() == pssId)
            return m_elementArray[i];
    }
    return NULL;
}

void PsocAnalogOutputPeripheral::setPsocHandler(PsocHandler* psocHandler)
{
    m_psocHandler = psocHandler;
}

void PsocAnalogOutputPeripheral::downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore)
{
//    for (int index = 0; index < M_NUM_OF_PSOC_PWM_OUTPUTS; ++index)
//    {
//        m_psocHandler->configDigitalIO(completeSemaphore, index, m_pwmChannelType[index]);
//    }
}

void PsocAnalogOutputPeripheral::setPsocOutputsEnabled(bool enabled)
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "psoc %d Analog Outputs status is %d",
            m_psocHandler->getPsocIndex(), enabled);
    for (int i = 0; i < M_NUM_OF_PSOC_ANALOG_OUTPUTS; ++i)
    {
        if (m_elementArray[i]->isValid() != enabled)
            m_elementArray[i]->setValueValid(enabled);
    }
}

void PsocAnalogOutputPeripheral::raiseError(E_PsocErrorBits error, bool state)
{
    int i = 0;
    switch (error)
    {
    case E_PsocErrorBits_CableIdNotMatching:
        for (i = 0; i < M_NUM_OF_ANALOG_OUTPUTS; ++i)
        {
            m_elementArray[i]->updateErrorBits(E_PSSErrors_CableIdNotMatching, state);
        }
        break;
    }
}
