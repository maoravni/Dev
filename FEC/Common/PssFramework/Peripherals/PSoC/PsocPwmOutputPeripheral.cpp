/*
 * PsocPwmOutputPeripheral.cpp
 *
 *  Created on: 12 αιπε 2014
 *      Author: maora
 */

#include "PsocPwmOutputPeripheral.h"
#include "PsocHandler.h"
#include <Peripherals/PeripheralRepository.h>

PsocPwmOutputPeripheral::PsocPwmOutputPeripheral()
{
    m_psocHandler = NULL;
    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        m_dutyCycleElementArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_dutyCycleElementArray[i]->addObserver(this);
        m_dutyCycleElementArray[i]->updateAllowedRange(0, 100, true, true);
        m_pwmChannelType[i] = E_DigitalIOType_DigitalOut;
    }

    m_firstElementIndex = m_dutyCycleElementArray[0]->getElementIndex();

    memset(m_pwmChannelRampParameters, 0, sizeof(m_pwmChannelRampParameters));
}

PsocPwmOutputPeripheral::~PsocPwmOutputPeripheral()
{
    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        m_psocHandler->addPendingPwmOutputValue(i, 0);
        m_dutyCycleElementArray[i]->removeObserver(this);
    }
}

ElementBase* PsocPwmOutputPeripheral::getElementByIndex(int index)
{
    return m_dutyCycleElementArray[index];
}

void PsocPwmOutputPeripheral::enableElementByIndex(int index, bool enable)
{
}

void PsocPwmOutputPeripheral::updateOutputs()
{
    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        m_psocHandler->addPendingPwmOutputValue(i, m_dutyCycleElementArray[i]->getValueU32());
    }
}

void PsocPwmOutputPeripheral::updateNotification(ElementBase* element)
{
    int i = element->getElementIndex() - m_firstElementIndex;
    m_psocHandler->addPendingPwmOutputValue(i, element->getValueU32());
}

ElementBase* PsocPwmOutputPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
    {
        if (m_dutyCycleElementArray[i]->getPssId() == pssId)
            return m_dutyCycleElementArray[i];
    }
    return NULL;
}

void PsocPwmOutputPeripheral::setPsocHandler(PsocHandler* psocHandler)
{
    m_psocHandler = psocHandler;
}

void PsocPwmOutputPeripheral::configPwmChannel(uint8_t channelIndex, uint16_t cycleLength, uint8_t riseStep,
        uint8_t fallStep, uint16_t riseTime, uint16_t fallTime)
{
    switch (channelIndex)
    {
    case 0:
    {
        switch (cycleLength)
        {
        case 0:
            m_pwmChannelType[channelIndex] = E_DigitalIOType_RandomPWM;
            break;
        case 1:
            m_pwmChannelType[channelIndex] = E_DigitalIOType_DistributedPWM;
            break;
//        case 2:
//            m_pwmChannelType[channelIndex] = E_DigitalIOType_FastPWM;
//            break;
        default:
            m_pwmChannelType[channelIndex] = E_DigitalIOType_PWM;
            break;
        }
        break;
    }
    case 1:
        m_pwmChannelType[channelIndex] = E_DigitalIOType_RandomPWM;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        m_pwmChannelType[channelIndex] = E_DigitalIOType_PWM;
        break;
    }

    m_pwmChannelRampParameters[channelIndex].riseStep = riseStep;
    m_pwmChannelRampParameters[channelIndex].fallStep = fallStep;
    m_pwmChannelRampParameters[channelIndex].riseTime = riseTime / 100;
    m_pwmChannelRampParameters[channelIndex].fallTime = fallTime / 100;
}

void PsocPwmOutputPeripheral::downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore)
{
    for (int index = 0; index < M_NUM_OF_PSOC_PWM_OUTPUTS; ++index)
    {
        m_psocHandler->configDigitalIO(completeSemaphore, index, m_pwmChannelType[index],
                m_pwmChannelRampParameters[index].riseStep, m_pwmChannelRampParameters[index].fallStep,
                m_pwmChannelRampParameters[index].riseTime, m_pwmChannelRampParameters[index].fallTime);
    }
}

void PsocPwmOutputPeripheral::setPsocOutputsEnabled(bool enabled)
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "psoc %d PWM Outputs status is %d", m_psocHandler->getPsocIndex(), enabled);
    for (int i = 0; i < M_NUM_OF_PWM_OUTPUTS; ++i)
    {
        if (m_dutyCycleElementArray[i]->isValid() != enabled)
            m_dutyCycleElementArray[i]->setValueValid(enabled);
    }
}

void PsocPwmOutputPeripheral::raiseError(E_PsocErrorBits error, bool state)
{
    int i = 0;
    switch (error)
    {
    case E_PsocErrorBits_CableIdNotMatching:
        for (i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
        {
            //m_dutyCycleElementArray[i]->setValueValid(!state);
            m_dutyCycleElementArray[i]->updateErrorBits(E_PSSErrors_CableIdNotMatching, state);
        }
        break;
    case E_PsocErrorBits_SsrOverheating:
//        for (i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
//         {
        //m_dutyCycleElementArray[i]->setValueValid(!state);
        m_dutyCycleElementArray[0]->updateErrorBits(E_PSSErrors_SsrOverheating, state);
//         }
        break;
    case E_PsocErrorBits_SsrAlwaysOn:
//       for (i = 0; i < M_NUM_OF_PSOC_PWM_OUTPUTS; ++i)
//        {
        //m_dutyCycleElementArray[i]->setValueValid(!state);
        m_dutyCycleElementArray[0]->updateErrorBits(E_PSSErrors_SsrAlwaysOn, state);
        PeripheralRepository::getInstance().getDryContactOutput()->setDryContactState(false);
//        }
        break;
    }
}

