/*
 * PsocCurrentInputsPeripheral.cpp
 *
 *  Created on: 28 באפר 2014
 *      Author: maora
 */

#include "Peripherals/PSoC/PsocCurrentInputsPeripheral.h"
#include <Elements/ElementRepository.h>
#include "PsocHandler.h"

#define M_PSOC_COUNTS_TO_VOLTS      (float)(5.0/4096.0)
#define M_PSOC_ANALOG_IN_GAIN       (M_PSOC_COUNTS_TO_VOLTS*10)
#define M_PSOC_PEAK_CURRENT_GAIN    (M_PSOC_COUNTS_TO_VOLTS*80*0.707)
#define M_PSOC_ANALOG_IN_MIN_COUNTS 300

#define M_LowPassFilter(newVal, prevVal, coeff) coeff * newVal + (1 - coeff) * prevVal

PsocCurrentInputsPeripheral::PsocCurrentInputsPeripheral()
{
    for (int i = 0; i < M_NUM_OF_ANALOG_SENSORS; ++i)
    {
        m_analogElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_bCoeff[i] = 0;
    }

    m_aCoeff[0] = M_PSOC_PEAK_CURRENT_GAIN;
    m_aCoeff[1] = M_PSOC_PEAK_CURRENT_GAIN;
    m_aCoeff[2] = 1;
    m_aCoeff[3] = 1;

    m_peakCounts = 0;

    setUpdateInterval(1000);

    m_lpfCoeff = 1.0 / 5.0;

    m_psocHandler = NULL;

//    setUpdateInterval(50);
}

PsocCurrentInputsPeripheral::~PsocCurrentInputsPeripheral()
{
}

void PsocCurrentInputsPeripheral::readInputs()
{
    if (m_psocHandler->getPsocCommState() == E_PsocCommState_Enabled)
    {
        m_analogElementsArray[0]->setValueValid(true);
        m_analogElementsArray[1]->setValueValid(true);

        m_analogElementsArray[0]->setValue(
                (float) (((float) m_psocHandler->getLocalPeakCurrentValue(0)) * m_aCoeff[0] + m_bCoeff[0]));
        m_analogElementsArray[1]->setValue(
                (float) (((float) m_psocHandler->getLocalPeakCurrentValue(1)) * m_aCoeff[1] + m_bCoeff[1]));

        m_peakCounts = (uint16_t)(M_LowPassFilter((m_psocHandler->getLocalPwmControlPulseCounter()), m_peakCounts, m_lpfCoeff));
    }
}

void PsocCurrentInputsPeripheral::setPsocHandler(PsocHandler* psocHandler)
{
    m_psocHandler = psocHandler;
}

ElementBase* PsocCurrentInputsPeripheral::getElementByIndex(int index)
{
    return m_analogElementsArray[index];
}

void PsocCurrentInputsPeripheral::enableElementByIndex(int index, bool enable)
{
}

void PsocCurrentInputsPeripheral::setCalibrationCoeff(int index, float aCoeff, float bCoeff)
{
    m_aCoeff[index] *= aCoeff;
    m_bCoeff[index] *= aCoeff;
    m_bCoeff[index] += bCoeff;
}

void PsocCurrentInputsPeripheral::setScalingCoeff(int index, float aCoeff, float bCoeff)
{
    m_aCoeff[index] *= aCoeff;
    m_bCoeff[index] += bCoeff;
}

void PsocCurrentInputsPeripheral::downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore)
{
    // hardcoded for now.
    // TODO: Update the SPT and the ICD to have the limits configurable.
    if (m_analogElementsArray[0]->getPssId() != 0)
        m_psocHandler->configCurrentLoop(completeSemaphore, 0, 0, 4095);
    if (m_analogElementsArray[1]->getPssId() != 0)
        m_psocHandler->configCurrentLoop(completeSemaphore, 1, 0, 4095);
}
