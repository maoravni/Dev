/*
 * SwPwmOutputDevice.cpp
 *
 *  Created on: 13 Jun 2013
 *      Author: maora
 */

#include "SwPwmOutputPeripheral.h"
#include <Elements/ElementRepository.h>
#include "PeripheralRepository.h"
#include <opto_out.h>
#include "GeneratedGratedPwm.h"
#include <stdlib.h>

#define M_PWM_TASK_TICK_LENGTH 10

SwPwmOutputPeripheral::SwPwmOutputPeripheral()
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
//        ElementRepository::getInstance().addElement(&m_dutyCycleElementArray[i]);
        m_dutyCycleElementArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_dutyCycleElementArray[i]->addObserver(this);
        m_dutyCycleElementArray[i]->updateAllowedRange(0, 100, true, true);
        m_channelStateArray[i].enabled = false;
        m_channelStateArray[i].mode = E_SoftwarePwmMode_Normal;
        m_channelStateArray[i].groupId = 0;
        m_channelStateArray[i].totalCycleLength = 0;
        m_channelStateArray[i].dutyCycleLength = 0;
    }

    m_firstElementIndex = m_dutyCycleElementArray[0]->getElementIndex();

    m_boardInReady = false;
    m_channelsEnabled = false;
}

SwPwmOutputPeripheral::~SwPwmOutputPeripheral()
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
//        ElementRepository::getInstance().addElement(&m_dutyCycleElementArray[i]);
        m_dutyCycleElementArray[i]->removeObserver(this);
    }
}

#ifdef WIN32
#define Var_GetBit_BB(VarAddr, BitNumber) 0
#else
#define Var_GetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (SRAM_BB_BASE | (((uint32_t)VarAddr - SRAM_BASE) << 5) | ((BitNumber) << 2)))
#endif

void SwPwmOutputPeripheral::run()
{
    portTickType currentTickCount = getTickCount();
    uint32_t randVal;

    while (1)
    {
        if (m_boardInReady && m_channelsEnabled)
        {
            for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
            {
                // if the channel is disabled ignore it.
                if (!m_channelStateArray[i].enabled)
                    continue;

                ++m_channelStateArray[i].currentCounter;
                switch (m_channelStateArray[i].mode)
                {
                case E_SoftwarePwmMode_Random:
                    randVal = rand();

                    dig_out_ctrl((OPTO_OUT_ID_ET) i,
                            (_opto_out_ctrl_val_) (randVal <= m_channelStateArray[i].dutyCycleLength));
                    break;
                case E_SoftwarePwmMode_Distributed:
                    if (m_channelStateArray[i].currentCounter >= 100)
                    {
                        m_channelStateArray[i].currentCounter = 0;
                    }

                    dig_out_ctrl((OPTO_OUT_ID_ET) i,
                            (_opto_out_ctrl_val_) Var_GetBit_BB((gratedPwm[m_channelStateArray[i].dutyCycleLength]),
                                    m_channelStateArray[i].currentCounter));
                    break;
                default:
                    // if we're past the duty cycle, turn off the output
                    if (m_channelStateArray[i].currentCounter >= m_channelStateArray[i].dutyCycleLength)
                    {
                        dig_out_ctrl((OPTO_OUT_ID_ET) i, DEACTIVE);
                    }
                    if (m_channelStateArray[i].currentCounter >= m_channelStateArray[i].totalCycleLength)
                    {
                        m_channelStateArray[i].currentCounter = 0;
                        if (m_channelStateArray[i].dutyCycleLength > 0)
                            dig_out_ctrl((OPTO_OUT_ID_ET) i, ACTIVE);
                    }
                    break;
                }
            }
            delayUntil(&currentTickCount, M_PWM_TASK_TICK_LENGTH);
        }
        else
            delay(1000);
    }
}

void SwPwmOutputPeripheral::configPwmChannel(uint8_t channelIndex, uint16_t cycleLength, uint8_t groupId)
{
    switch (cycleLength)
    {
    case 0:
        m_channelStateArray[channelIndex].mode = E_SoftwarePwmMode_Random;
        m_channelStateArray[channelIndex].totalCycleLength = RAND_MAX;
        break;
    case 1:
        m_channelStateArray[channelIndex].mode = E_SoftwarePwmMode_Distributed;
        m_channelStateArray[channelIndex].totalCycleLength = 100;
        m_channelStateArray[channelIndex].groupId = groupId;
        break;
    default:
        m_channelStateArray[channelIndex].mode = E_SoftwarePwmMode_Normal;
        m_channelStateArray[channelIndex].totalCycleLength = cycleLength / 10;
        m_channelStateArray[channelIndex].groupId = groupId;
        break;
    }
}

void SwPwmOutputPeripheral::enableElementByIndex(int channelIndex, bool enable)
{
    m_channelStateArray[channelIndex].enabled = enable;

    if (enable)
        m_channelsEnabled = true;

    // when we enable a PWM channel, we need to disable the channel in the digital out device, so that
    // the output value can't be overridden:
    DigitalOutputsPeripheral * digitalOutPeriph = PeripheralRepository::getInstance().getDigitalOutputsPeripheral();
    if (digitalOutPeriph == NULL)
    {
        PeripheralRepository::getInstance().initDigitalOutputs(0, M_NUM_OF_DIGITAL_OUT);
        digitalOutPeriph = PeripheralRepository::getInstance().getDigitalOutputsPeripheral();
    }
    if (enable)
        digitalOutPeriph->enableElementByIndex(channelIndex, false);
    else
        digitalOutPeriph->enableElementByIndex(channelIndex, true);
    reassignCounterOffset();
}

bool SwPwmOutputPeripheral::createTask()
{
    return create("SwPwm", 100, configMAX_PRIORITIES - 1);
}

ElementBase* SwPwmOutputPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
        if (m_dutyCycleElementArray[i]->getPssId() == pssId)
            return m_dutyCycleElementArray[i];
    }
    return NULL;
}

void SwPwmOutputPeripheral::setDutyCycle(int i)
{
    if (m_channelStateArray[i].enabled)
        m_channelStateArray[i].dutyCycleLength = (uint32_t)(
                m_dutyCycleElementArray[i]->getValue() / 100 * m_channelStateArray[i].totalCycleLength);
}

void SwPwmOutputPeripheral::updateOutputs()
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
        setDutyCycle(i);
    }
}

void SwPwmOutputPeripheral::updateNotification(ElementBase* element)
{
    int i = element->getElementIndex() - m_firstElementIndex;

    setDutyCycle(i);
}

ElementBase* SwPwmOutputPeripheral::getElementByIndex(int index)
{
    return m_dutyCycleElementArray[index];
}

void SwPwmOutputPeripheral::reassignCounterOffset()
{
    uint8_t groupArray[M_NUM_OF_DIGITAL_OUT];
    uint8_t channelCountArray[M_NUM_OF_DIGITAL_OUT];
    uint8_t groupCycleLength[M_NUM_OF_DIGITAL_OUT];
    uint8_t tempCounterArray[M_NUM_OF_DIGITAL_OUT];
    int i, j;

    // clear the group array
    for (i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
        groupArray[i] = 0xff;
        channelCountArray[i] = 0;
    }

    // go over all channels, and count channels in the same group:
    for (i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
        // if the channel is disabled ignore it.
        if (!m_channelStateArray[i].enabled)
            continue;

        // find if the group is used:
        for (j = 0; j < M_NUM_OF_DIGITAL_OUT; ++j)
        {
            if (groupArray[j] == 0xff || groupArray[j] == m_channelStateArray[i].groupId)
                break;
        }

        groupArray[j] = m_channelStateArray[i].groupId;
        // store the cycle length in this group.
        // assumes that all channels in the same have the same cycle length
        groupCycleLength[j] = m_channelStateArray[i].totalCycleLength;
        // count the number of channels in this group
        ++channelCountArray[j];
    }

    for (i = 0; i < M_NUM_OF_DIGITAL_OUT; ++i)
    {
        if (groupArray[i] == -1)
            continue;
        if (channelCountArray[i] == 0)
            continue;

        // calculate the counter offset:
        uint8_t offset = groupCycleLength[i] / channelCountArray[i];
        uint8_t count = 0;
        for (j = 0; j < M_NUM_OF_DIGITAL_OUT; ++j)
        {
            // reassign the channel counter and advance the offset.
            if (m_channelStateArray[j].enabled && m_channelStateArray[j].groupId == groupArray[i])
            {
                m_channelStateArray[j].currentCounter = count;
                count += offset;
            }
        }
    }
}

