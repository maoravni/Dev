/*
 * PID.h
 *
 *  Created on: 17 Apr 2013
 *      Author: maora
 */

#ifndef PID_H_
#define PID_H_

#include <FreeRTOS.h>

#define MANUAL 0
#define AUTOMATIC 1

#define DIRECT 0
#define REVERSE 1

class PID
{
    /*working variables*/
    portTickType m_lastTick;
    float m_input, m_output, m_setpoint, m_lastOutput;
    float m_error, m_dError;
    float m_lastError;
    float m_ITerm, m_lastInput;
    float m_kp, m_ki, m_kd;
    float m_kiErrorThreshold; //!< When temp error is above this threshold, we don't calculate the iTerm.
    float m_setpointSmoothingThreshold; //!< this is used to limit the temperature acceleration.
    float m_outputSmoothingCoeff;
    portTickType m_sampleTime; //1 sec
    float m_outMin, m_outMax;
    bool m_inAuto;
    bool m_enabled;

    int m_controllerDirection;

public:
    PID();
    virtual ~PID();

    /**
     * Compute the power output
     * @param input the current temperature.
     * @return the required output.
     */
    float Compute(float input);

    /**
     * Set the cooefficients for this control loop.
     * @param Kp
     * @param Ki
     * @param Kd
     */
    void SetTunings(float Kp, float Ki, float Kd);

    /**
     * Set the sample time that the control is running in.
     * @param NewSampleTime in ms.
     */
    void SetSampleTime(int NewSampleTime);

    /**
     * Set the power output limits.
     * @param Min
     * @param Max
     */
    void SetOutputLimits(float Min, float Max);

    /**
     * Set the mode of operation
     * @param Mode
     */
    void SetAutoMode(bool mode);

    /**
     * Set the controller direction - more power means hotter or colder.
     * @param Direction
     */
    void SetControllerDirection(bool isDirectOperation);

    /**
     *
     * @param setpoint
     */
    void SetSetPoint(float setpoint);

    bool isEnabled() const
    {
        return m_enabled;
    }

    void setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    float getKd() const
    {
        return m_kd;
    }

    float getKi() const
    {
        return m_ki;
    }

    float getKp() const
    {
        return m_kp;
    }

    float getKiErrorThreshold() const
    {
        return m_kiErrorThreshold;
    }

    void setKiErrorThreshold(float kiErrorThreshold)
    {
        m_kiErrorThreshold = kiErrorThreshold;
    }

    float getSetpointSmoothingThreshold() const
    {
        return m_setpointSmoothingThreshold;
    }

    void setSetpointSmoothingThreshold(float setpointSmoothingThreshold)
    {
        m_setpointSmoothingThreshold = setpointSmoothingThreshold;
    }

    int getOutputSmoothingWindow() const
    {
        return (int)(1/(m_outputSmoothingCoeff)-1);
    }

    void setOutputSmoothingWindow(int outputSmoothingWindow)
    {
        m_outputSmoothingCoeff = 1.0/((float)((float)outputSmoothingWindow+1.0));
    }

private:
    /**
     * Initialize the control loop.
     */
    void Initialize();


};

#endif /* PID_H_ */
