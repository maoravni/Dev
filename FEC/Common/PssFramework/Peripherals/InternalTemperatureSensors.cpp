/*
 * InternalTemperatureSensors.cpp
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#include "InternalTemperatureSensors.h"
#include <Elements/ElementRepository.h>
#include <Elements/ValidationElement.h>
#include <continousA2D.h>
#include <math.h>
#include <assert.h>
#include <logger.h>

InternalTemperatureSensors::InternalTemperatureSensors()
{
//    m_temperatureElementsArray = new ValidationElementFloat*[M_NUM_OF_INTERNAL_SENSORS];

    for (int i = 0; i < M_NUM_OF_INTERNAL_SENSORS; ++i) {
//        ElementRepository::getInstance().addElement(&m_temperatureElementsArray[i]);
        m_temperatureElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
        m_aCoeff[i] = 1;
        m_bCoeff[i] = 0;
    }

    setSensorDisconnectedValue(510);
}

InternalTemperatureSensors::~InternalTemperatureSensors()
{
}

void InternalTemperatureSensors::readInputs()
{
    float tempValue;
//    m_temperatureElementsArray[0] = convertDigital2Temperature(a2d_read_filtered_data(RTD_1));
//    m_temperatureElementsArray[1] = convertDigital2Temperature(a2d_read_filtered_data(RTD_2));
//    m_temperatureElementsArray[2] = convertDigital2Temperature(a2d_read_filtered_data(RTD_3));
//    m_temperatureElementsArray[3] = convertDigital2Temperature(a2d_read_filtered_data(RTD_4));
//    m_temperatureElementsArray[4] = convertDigital2Temperature(a2d_read_filtered_data(RTD_5));
//    m_temperatureElementsArray[5] = convertDigital2Temperature(a2d_read_filtered_data(RTD_6));
//    m_temperatureElementsArray[6] = convertDigital2Temperature(a2d_read_filtered_data(RTD_7));
//    m_temperatureElementsArray[7] = convertDigital2Temperature(a2d_read_filtered_data(RTD_8));
//    m_temperatureElementsArray[8] = convertDigital2Temperature(a2d_read_filtered_data(RTD_9));
//    m_temperatureElementsArray[9] = convertDigital2Temperature(a2d_read_filtered_data(RTD_10));
//    m_temperatureElementsArray[10] = convertDigital2Temperature(a2d_read_filtered_data(RTD_11));
//    m_temperatureElementsArray[11] = convertDigital2Temperature(a2d_read_filtered_data(RTD_12));
    convertDigital2Temperature(a2d_read_filtered_data(RTD_1), 0);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_2), 1);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_3), 2);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_4), 3);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_5), 4);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_6), 5);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_7), 6);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_8), 7);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_9), 8);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_10), 9);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_11), 10);
    convertDigital2Temperature(a2d_read_filtered_data(RTD_12), 11);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Internal temp sensors: %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f", m_temperatureElementsArray[0]->getValue(),
            m_temperatureElementsArray[1]->getValue(),
            m_temperatureElementsArray[2]->getValue(),
            m_temperatureElementsArray[3]->getValue(),
            m_temperatureElementsArray[4]->getValue(),
            m_temperatureElementsArray[5]->getValue(),
            m_temperatureElementsArray[6]->getValue(),
            m_temperatureElementsArray[7]->getValue(),
            m_temperatureElementsArray[8]->getValue(),
            m_temperatureElementsArray[9]->getValue(),
            m_temperatureElementsArray[10]->getValue(),
            m_temperatureElementsArray[11]->getValue());
    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Internal temp sensors valid: %d %d %d %d %d %d %d %d %d %d %d %d", m_temperatureElementsArray[0]->isValid(),
            m_temperatureElementsArray[1]->isValid(),
            m_temperatureElementsArray[2]->isValid(),
            m_temperatureElementsArray[3]->isValid(),
            m_temperatureElementsArray[4]->isValid(),
            m_temperatureElementsArray[5]->isValid(),
            m_temperatureElementsArray[6]->isValid(),
            m_temperatureElementsArray[7]->isValid(),
            m_temperatureElementsArray[8]->isValid(),
            m_temperatureElementsArray[9]->isValid(),
            m_temperatureElementsArray[10]->isValid(),
            m_temperatureElementsArray[11]->isValid());
}

float InternalTemperatureSensors::convertDigital2Temperature(float input, float aCoeff, float bCoeff)
{
    float calcTemp;
    const float a = -6.0154E-6;
    const float b = 9.4526E-3;
    const float c = 2.0988E-2;
    calcTemp = (-b+sqrt((b*b-4*a*(c-(float)(input)*3.3/4096))))/(2*a);
    calcTemp = aCoeff*calcTemp + bCoeff;
    //calcTemp = round(calcTemp*10)*0.1;
    return calcTemp;
}

void InternalTemperatureSensors::convertDigital2Temperature(float input, int index)
{
    float tempValue = convertDigital2Temperature(input, m_aCoeff[index], m_bCoeff[index]);
    m_temperatureElementsArray[index]->setValueValid((tempValue < m_sensorDisconnectedValue));
    m_temperatureElementsArray[index]->setValue(tempValue);
}

ElementBase* InternalTemperatureSensors::getElementByIndex(int index)
{
    if (index >= M_NUM_OF_INTERNAL_SENSORS)
        return NULL;

    return m_temperatureElementsArray[index];
}

void InternalTemperatureSensors::enableElementByIndex(int index, bool enable)
{
}

void InternalTemperatureSensors::setLpfWindow(unsigned long lpfWindow)
{
    a2d_config_lpf_window(lpfWindow);
}

void InternalTemperatureSensors::setCalibrationCoeff(int index, float aCoeff, float bCoeff)
{
    m_aCoeff[index] = aCoeff;
    m_bCoeff[index] = bCoeff;
}

ElementBase* InternalTemperatureSensors::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_INTERNAL_SENSORS; ++i)
    {
        if (m_temperatureElementsArray[i]->getPssId() == pssId)
            return m_temperatureElementsArray[i];
    }
    return NULL;
}

void InternalTemperatureSensors::setScalingCoeff(int index, float aCoeff, float bCoeff)
{
}
