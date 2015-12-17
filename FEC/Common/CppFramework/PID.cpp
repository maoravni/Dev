/*
 * PID.cpp
 *
 *  Created on: 17 Apr 2013
 *      Author: maora
 */

#include "PID.h"
#include <task.h>
#include <math.h>

PID::PID()
{
    m_input = 0;
    m_output = 0;
    m_lastOutput = 0;
    m_setpoint = 0;
    m_kiErrorThreshold = 0;
    m_setpointSmoothingThreshold = 0;
    m_outputSmoothingCoeff = 1;
//    m_kiErrorThreshold = 100;
//    m_setpointSmoothingThreshold = 50;
//    m_outputSmoothingWindow = 4;

    m_inAuto = false;
    m_enabled = false;

    SetOutputLimits(0, 100);
    SetSampleTime(1000);
    SetControllerDirection(true);
    SetTunings(1, 1, 1);
}

PID::~PID()
{
}

float PID::Compute(float input)
{
    if (!m_enabled)
        return 0;

    float calculatedSetpoint;

    m_input = input;

    if (!m_inAuto)
    {
        m_lastTick = xTaskGetTickCount();
        m_lastInput = input;
        return m_output;
    }
    unsigned long now = xTaskGetTickCount();

    if (m_setpointSmoothingThreshold != 0 && abs(m_setpoint - m_input) > m_setpointSmoothingThreshold)
    {
        if (m_setpoint > m_input)
            calculatedSetpoint = m_input + m_setpointSmoothingThreshold;
        else
            calculatedSetpoint = m_input - m_setpointSmoothingThreshold;
    }
    else
        calculatedSetpoint = m_setpoint;

    /*Compute all the working error variables*/
    m_error = calculatedSetpoint - m_input;
    if (m_kiErrorThreshold == 0 || abs(m_error) <= m_kiErrorThreshold)
    {
        m_ITerm += (m_ki * m_error);
        if (m_ITerm > m_outMax)
            m_ITerm = m_outMax;
        else if (m_ITerm < m_outMin)
            m_ITerm = m_outMin;
    }
    else
    {
        m_ITerm = 0;
//        m_dInput = 0;
    }
    m_dError = (m_error - m_lastError);

    /*Compute PID Output*/
    m_output = m_kp * m_error + m_ITerm + m_kd * m_dError;

    // compute output smoothing:
//    m_output = (m_outputSmoothingCoeff * m_lastOutput + m_output) / (m_outputSmoothingCoeff + 1);

    // truncate output value:
    if (m_output > m_outMax)
        m_output = m_outMax;
    else if (m_output < m_outMin)
        m_output = m_outMin;

    m_output = m_outputSmoothingCoeff*m_output + (1-m_outputSmoothingCoeff)*m_lastOutput;

    /*Remember some variables for next time*/
    m_lastTick = xTaskGetTickCount();
    m_lastInput = m_input;
    m_lastOutput = m_output;
    m_lastError = m_error;

    return m_output;
}

void PID::SetTunings(float Kp, float Ki, float Kd)
{
    if (Kp < 0 || Ki < 0 || Kd < 0)
        return;

    float SampleTimeInSec = ((float) m_sampleTime) / 1000;
    m_kp = Kp;
    m_ki = (Ki * SampleTimeInSec);
    m_kd = Kd / SampleTimeInSec;

//    setKiErrorThreshold(m_outMax/m_kp);

    if (m_controllerDirection == REVERSE)
    {
        m_kp = (0 - m_kp);
        m_ki = (0 - m_ki);
        m_kd = (0 - m_kd);
    }

}

void PID::SetSampleTime(int NewSampleTime)
{
    if (NewSampleTime > 0)
    {
        float ratio = (float) NewSampleTime / (float) m_sampleTime;
        m_ki *= ratio;
        m_kd /= ratio;
        m_sampleTime = (unsigned long) NewSampleTime;
    }
}

void PID::SetOutputLimits(float Min, float Max)
{
    if (Min > Max)
        return;
    m_outMin = Min;
    m_outMax = Max;

    if (m_output > m_outMax)
        m_output = m_outMax;
    else if (m_output < m_outMin)
        m_output = m_outMin;

    if (m_ITerm > m_outMax)
        m_ITerm = m_outMax;
    else if (m_ITerm < m_outMin)
        m_ITerm = m_outMin;

//    setKiErrorThreshold(Max/m_kp);
}

void PID::SetAutoMode(bool mode)
{
    if (mode == !m_inAuto)
    { /*we just went from manual to auto*/
        Initialize();
    }
    m_inAuto = mode;
}

void PID::SetSetPoint(float setpoint)
{
    m_setpoint = setpoint;
}

void PID::Initialize()
{
    //lastInput = Input;
    m_ITerm = m_output;
    if (m_ITerm > m_outMax)
        m_ITerm = m_outMax;
    else if (m_ITerm < m_outMin)
        m_ITerm = m_outMin;
    m_lastError = 0;
}

void PID::SetControllerDirection(bool isDirectOperation)
{
    m_controllerDirection = (isDirectOperation) ? DIRECT : REVERSE;
}
