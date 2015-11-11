/*
 * PsocAnalogInputsPeripheral.cpp
 *
 *  Created on: 28 באפר 2014
 *      Author: maora
 */

#include "Peripherals/PSoC/PsocAnalogInputsPeripheral.h"
#include <Elements/ElementRepository.h>
#include "PsocHandler.h"

#define M_PSOC_COUNTS_TO_VOLTS      (float)(5.0/4096.0)
#define M_PSOC_ANALOG_IN_GAIN       (M_PSOC_COUNTS_TO_VOLTS)
#define M_PSOC_PEAK_CURRENT_GAIN    (M_PSOC_COUNTS_TO_VOLTS*80*0.707)
#define M_PSOC_ANALOG_IN_MIN_COUNTS 2500

#define M_LowPassFilter(newVal, prevVal, coeff) coeff * newVal + (1 - coeff) * prevVal

PsocAnalogInputsPeripheral::PsocAnalogInputsPeripheral()
{
    for (int i = 0; i < M_NUM_OF_ANALOG_SENSORS; ++i)
    {
        m_analogElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_analogElementsArray[i]->setValue((float) 0.0);
        m_bCoeff[i] = 0;
    }

    m_aCoeff[0] = M_PSOC_ANALOG_IN_GAIN;
    m_aCoeff[1] = M_PSOC_ANALOG_IN_GAIN;
    m_aCoeff[2] = M_PSOC_PEAK_CURRENT_GAIN;
    m_aCoeff[3] = M_PSOC_PEAK_CURRENT_GAIN;
    m_aCoeff[4] = M_PSOC_PEAK_CURRENT_GAIN * 4 * 0.01;
    m_aCoeff[5] = M_PSOC_PEAK_CURRENT_GAIN * 4 * 0.01;

    //    m_lastUpdateTick = 0;
    m_peakCounts = 0;

    setUpdateInterval(250);

    m_lpfCoeff = 1.0 / 10.0;
    m_filterSettleCounts = 20;//2*window. window = 10

    m_lastEnergyCalculationTickCount = 0;
    m_psocHandler = NULL;
    //    setUpdateInterval(50);
}

PsocAnalogInputsPeripheral::~PsocAnalogInputsPeripheral()
{
}

void PsocAnalogInputsPeripheral::setFilterSettleCounts(uint16_t LowPassFilterWindow)
{
    if(LowPassFilterWindow == 0)
    {
        LowPassFilterWindow=1;
    }
    m_lpfCoeff = 1.0 / LowPassFilterWindow;
    m_filterSettleCounts = 2 * LowPassFilterWindow;
}


void PsocAnalogInputsPeripheral::setUpdateInterval(uint16_t updateInterval)
{
	if (updateInterval == 0)
		return;

    if(updateInterval > MAX_UPDATE_INTERVAL)
    {
        updateInterval = 1000;
    }
    PeripheralBase::setUpdateInterval(updateInterval);
}

void PsocAnalogInputsPeripheral::readInputs()
{
    float analogNew0, analogNew1, analogPrev0, analogPrev1;

    if (m_psocHandler->getPsocCommState() == E_PsocCommState_Enabled)
    {
        int i;
        m_analogElementsArray[0]->setValueValid(
                (m_psocHandler->getLocalAnalogInValue(0) >= M_PSOC_ANALOG_IN_MIN_COUNTS));
        m_analogElementsArray[1]->setValueValid(
                (m_psocHandler->getLocalAnalogInValue(1) >= M_PSOC_ANALOG_IN_MIN_COUNTS));
        m_analogElementsArray[2]->setValueValid(true);
        m_analogElementsArray[3]->setValueValid(true);
        m_analogElementsArray[4]->setValueValid(true);
        m_analogElementsArray[5]->setValueValid(true);

        // get the analog input value.
        //        m_analogElementsArray[0]->setValue(
        //                (float) (((float) m_psocHandler->getLocalAnalogInValue(0)) * m_aCoeff[0] + m_bCoeff[0]));
        //        m_analogElementsArray[1]->setValue(
        //                (float) (((float) m_psocHandler->getLocalAnalogInValue(1)) * m_aCoeff[1] + m_bCoeff[1]));
        if (m_filterSettleCounts > 0)
        {
            analogNew0 = (float) (((float) m_psocHandler->getLocalAnalogInValue(0)) * m_aCoeff[0] + m_bCoeff[0]);
            analogNew1 = (float) (((float) m_psocHandler->getLocalAnalogInValue(1)) * m_aCoeff[1] + m_bCoeff[1]);
            m_analogElementsArray[0]->setValue(analogNew0);
            m_analogElementsArray[1]->setValue(analogNew1);
            --m_filterSettleCounts;
        }
        else
        {
            analogNew0 = (float) (((float) m_psocHandler->getLocalAnalogInValue(0)) * m_aCoeff[0] + m_bCoeff[0]);
            analogNew1 = (float) (((float) m_psocHandler->getLocalAnalogInValue(1)) * m_aCoeff[1] + m_bCoeff[1]);
            analogPrev0 = m_analogElementsArray[0]->getValueF();
            analogPrev1 = m_analogElementsArray[1]->getValueF();
            analogNew0 = M_LowPassFilter(analogNew0, analogPrev0, m_lpfCoeff);
            analogNew1 = M_LowPassFilter(analogNew1, analogPrev1, m_lpfCoeff);
            m_analogElementsArray[0]->setValue(analogNew0);
            m_analogElementsArray[1]->setValue(analogNew1);
        }
        // get the number of cycles open by during the interval.
        //        m_peakCounts = M_LowPassFilter((m_psocHandler->getLocalPwmControlPulseCounter()), m_peakCounts, m_lpfCoeff);
        portTickType currentTickCount = xTaskGetTickCount();

        if ((currentTickCount - m_lastEnergyCalculationTickCount) >= 1000)
        {
            m_peakCounts = m_psocHandler->getLocalPwmControlPulseCounter();
            float currentPeak1 = m_analogElementsArray[2]->getValueF();
            float currentPeak2 = m_analogElementsArray[3]->getValueF();

            m_analogElementsArray[2]->setValue(m_psocHandler->getLocalPeakCurrentValue(0) * m_aCoeff[2] + m_bCoeff[2]);
            m_analogElementsArray[3]->setValue(m_psocHandler->getLocalPeakCurrentValue(1) * m_aCoeff[3] + m_bCoeff[3]);

            // calculate the actual duty cycle that the output was open in this interval,
            // as a percentage.
            //            float actualDutyCycle = m_peakCounts * (0.01 * (1000 / (float) m_updateInterval));

            // calculate actual energy.
            m_analogElementsArray[4]->setValue(m_psocHandler->getLocalEnergyCounter(0) * m_aCoeff[4] + m_bCoeff[4]);
            m_analogElementsArray[5]->setValue(m_psocHandler->getLocalEnergyCounter(1) * m_aCoeff[5] + m_bCoeff[5]);

            m_lastEnergyCalculationTickCount = currentTickCount;
        }
    }
}

void PsocAnalogInputsPeripheral::setPsocHandler(PsocHandler* psocHandler)
{
    m_psocHandler = psocHandler;
}

ElementBase* PsocAnalogInputsPeripheral::getElementByIndex(int index)
{
    return m_analogElementsArray[index];
}

void PsocAnalogInputsPeripheral::enableElementByIndex(int index, bool enable)
{
}

void PsocAnalogInputsPeripheral::setCalibrationCoeff(int index, float aCoeff, float bCoeff)
{
    m_aCoeff[index] *= aCoeff;
    m_bCoeff[index] *= aCoeff;
    m_bCoeff[index] += bCoeff;
}

void PsocAnalogInputsPeripheral::setScalingCoeff(int index, float aCoeff, float bCoeff)
{
    m_aCoeff[index] *= aCoeff;
    m_bCoeff[index] += bCoeff;
}

ElementBase* PsocAnalogInputsPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_ANALOG_SENSORS; ++i)
    {
        if (m_analogElementsArray[i]->getPssId() == pssId)
            return m_analogElementsArray[i];
    }
    return NULL;
}

void PsocAnalogInputsPeripheral::downloadConfigurationToPsoc(CBinarySemaphore* completeSemaphore)
{
    // hardcoded for now.
    // TODO: Update the SPT and the ICD to have the limits configurable.
    if (m_analogElementsArray[2]->getPssId() != 0 || m_analogElementsArray[3]->getPssId() != 0)
    {
        m_psocHandler->configCurrentLoop(completeSemaphore, 0, 0, 4095);
        m_psocHandler->configCurrentLoop(completeSemaphore, 1, 0, 4095);
    }

    float analogNew0 = (float) (((float) m_psocHandler->getLocalAnalogInValue(0)) * m_aCoeff[0] + m_bCoeff[0]);
    float analogNew1 = (float) (((float) m_psocHandler->getLocalAnalogInValue(1)) * m_aCoeff[1] + m_bCoeff[1]);
    m_analogElementsArray[0]->setValue(analogNew0);
    m_analogElementsArray[1]->setValue(analogNew1);
}

void PsocAnalogInputsPeripheral::raiseError(E_PsocErrorBits error, bool state)
{
    int i = 0;
    switch (error)
    {
    case E_PsocErrorBits_MissingCurrentSensor:
        for (i = 2; i < 3; ++i)
        {
            m_analogElementsArray[i]->updateErrorBits(E_PSSErrors_SensorMalfunction, state);
        }
        break;
    }
}

