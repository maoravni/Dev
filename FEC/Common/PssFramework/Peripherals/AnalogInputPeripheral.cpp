/*
 * AnalogInputPeripheral.cpp
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#include "AnalogInputPeripheral.h"
#include <Elements/ElementRepository.h>
#include <Elements/ValidationElement.h>
#include <continousA2D.h>
#include <math.h>
#include <assert.h>
#include <logger.h>

AnalogInputPeripheral::AnalogInputPeripheral()
{
//    m_temperatureElementsArray = new ValidationElementFloat*[M_NUM_OF_ANALOG_INTERNAL_SENSORS];

    for (int i = 0; i < M_NUM_OF_ANALOG_INTERNAL_SENSORS; ++i) {
//        ElementRepository::getInstance().addElement(&m_temperatureElementsArray[i]);
        m_inputElementsArray[i] = ElementRepository::getInstance().addValidationElementFloat();
//        m_correctionA[i] = 1;
//        m_correctionB[i] = 0;
        m_scalingA[i] = 20.0/4096.0;
        m_scalingB[i] = 0;
    }

    setSensorDisconnectedValue(400);

    setUpdateInterval(50);
}

AnalogInputPeripheral::~AnalogInputPeripheral()
{
}

void AnalogInputPeripheral::readInputs()
{
    float maValue;
    convert2Digital(a2d_read_filtered_data(in_4_20mA_1), 0);
    convert2Digital(a2d_read_filtered_data(in_4_20mA_2), 1);
    convert2Digital(a2d_read_filtered_data(in_4_20mA_3), 2);
    convert2Digital(a2d_read_filtered_data(RTD_12), 3);
    // TODO: Add log messages
//    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Internal temp sensors: %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f", m_temperatureElementsArray[0]->getValue(),
//            m_temperatureElementsArray[1]->getValue(),
//            m_temperatureElementsArray[2]->getValue(),
//            m_temperatureElementsArray[3]->getValue(),
//            m_temperatureElementsArray[4]->getValue(),
//            m_temperatureElementsArray[5]->getValue(),
//            m_temperatureElementsArray[6]->getValue(),
//            m_temperatureElementsArray[7]->getValue(),
//            m_temperatureElementsArray[8]->getValue(),
//            m_temperatureElementsArray[9]->getValue(),
//            m_temperatureElementsArray[10]->getValue(),
//            m_temperatureElementsArray[11]->getValue());
//    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "Internal temp sensors valid: %d %d %d %d %d %d %d %d %d %d %d %d", m_temperatureElementsArray[0]->isValid(),
//            m_temperatureElementsArray[1]->isValid(),
//            m_temperatureElementsArray[2]->isValid(),
//            m_temperatureElementsArray[3]->isValid(),
//            m_temperatureElementsArray[4]->isValid(),
//            m_temperatureElementsArray[5]->isValid(),
//            m_temperatureElementsArray[6]->isValid(),
//            m_temperatureElementsArray[7]->isValid(),
//            m_temperatureElementsArray[8]->isValid(),
//            m_temperatureElementsArray[9]->isValid(),
//            m_temperatureElementsArray[10]->isValid(),
//            m_temperatureElementsArray[11]->isValid());
}

float AnalogInputPeripheral::convert2Digital(float input)
{
//    calcTemp = (-b+sqrt((b*b-4*a*(c-(float)(input)*3.3/4096))))/(2*a);
    float ma = (input*20/4096);
//    float ma = input;
    return ma;
}

void AnalogInputPeripheral::convert2Digital(float input, int index)
{
//    float maValue = convert2Digital(input);
    float maValue = input;
    m_inputElementsArray[index]->setValueValid((maValue >= m_sensorDisconnectedValue));
    maValue = (maValue*m_scalingA[index] + m_scalingB[index])/**m_correctionA[index] + m_correctionB[index]*/;
    *m_inputElementsArray[index] = maValue;
}

ElementBase* AnalogInputPeripheral::getElementByIndex(int index)
{
    if (index >= M_NUM_OF_ANALOG_INTERNAL_SENSORS)
        return NULL;

    return m_inputElementsArray[index];
}

void AnalogInputPeripheral::enableElementByIndex(int index, bool enable)
{
}

void AnalogInputPeripheral::setLpfWindow(unsigned long lpfWindow)
{
    a2d_config_lpf_window(lpfWindow);
}

void AnalogInputPeripheral::setCalibrationCoeff(int index, float aCoeff, float bCoeff)
{
//    m_correctionA[index] = aCoeff;
//    m_correctionB[index] = bCoeff;
    m_scalingA[index] *= aCoeff;
    m_scalingB[index] *= aCoeff;
    m_scalingB[index] += bCoeff;
}

ElementBase* AnalogInputPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_ANALOG_INTERNAL_SENSORS; ++i)
    {
        if (m_inputElementsArray[i]->getPssId() == pssId)
            return m_inputElementsArray[i];
    }
    return NULL;
}

void AnalogInputPeripheral::setScalingCoeff(int index, float aCoeff, float bCoeff)
{
    m_scalingA[index] *= aCoeff;
    m_scalingB[index] += bCoeff;
}
