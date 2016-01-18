/*
 * PidControl.h
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#ifndef PIDCONTROL_H_
#define PIDCONTROL_H_

#include "ControlBase.h"
#include <Elements/Element.h>
#include <PIDDynamicSampleTime.h>
#include <PID_AutoTune_v0.h>
#include <PscServer/PscMessageStructs.h>
#include <Controls/PidControl.h>
#include "ActivationWithFeedbackControl.h"

//typedef DynamicArray<ValidationElementBase*> T_ElementArray;

class PidControl: public ControlBase
{
    ValidationElementFloat* m_input;
    ElementBase* m_output;
    ValidationElementFloat* m_setpoint;
//    T_DeviceCheckerList m_dependentCheckers;
    //    T_ElementArray m_protectionElements;

    float m_maxTemperature;

    PIDDynamicSampleTime m_pidCalc;
    PID_ATune* m_pPidAutotune;
    bool m_isInAutotune;

public:
    PidControl();
    virtual ~PidControl();

    void setPowerLimit(float maxPower);
    void setOutputSmoothing(unsigned short outputSmoothing);
    void setTuningValues(float kp, float ki, float kd, float kTrapezRange);
    void setSetpointRange(float setpointRange);
    void sendTuningValues();
    E_AckStatus startAutoTune(float setpoint, float overshoot, float outputStart, float outputStep, u16_t lookback,
            bool reverseDirection, uint32_t sn);
    void sendPidValues();
    void cancelAutoTune();
    virtual void updateNotification(ElementBase* element);
    virtual void execute();
    bool executeLocalProtectionCheck(ElementBase* element);
    virtual bool executeDependencyCheck(ElementBase* element);
    virtual E_ControlType getControlType()
    {
        return E_ControlType_PidControl;
    }

    virtual ElementBase* getMainControlElement() {return m_setpoint;}

    float getKp();
    float getKi();
    float getKd();

    void setElementInput(ValidationElementFloat* input)
    {
        m_input = input;
        m_input->addObserver(this);
        m_input->setMissingDevicePriority(E_MissingDevicePriority_High);
//        setSecondaryPssId(m_input->getPssId());
    }

    void setElementOutput(ElementBase* output)
    {
        m_output = output;
        m_output->addObserver(this);
    }

    void setElementSetpoint(ValidationElementFloat* setpoint)
    {
        m_setpoint = setpoint;
        m_setpoint->addObserver(this);
    }

    ValidationElementFloat* getElementInput() const
    {
        return m_input;
    }

    ElementBase* getElementOutput() const
    {
        return m_output;
    }

    ValidationElementFloat* getElementSetpoint() const
    {
        return m_setpoint;
    }

#ifdef OLD_PROTECTION
    virtual void addProtectionElement(ValidationElementBase* element);
#endif
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) {};
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate,
//            bool lock, int checkType);

    virtual bool sendNotification();

    virtual bool setSetpoint(float setpoint, uint32_t sn) {return setSetpoint(setpoint, -5, +5, -10, +10, 0, 0, sn);}
    virtual bool setSetpoint(float sp, float loRange, float hiRange, float loWarn, float hiWarn, float feedForward, uint32_t delay, uint32_t sn);
    virtual bool setSetpoint(ValidationElementFloat* element);

    virtual bool onInitControl();
    virtual bool onMove2Standby();
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}
    virtual E_ActivationState getActivationState();

    virtual void setPssId(uint16_t deviceId);
};

#endif /* PIDCONTROL_H_ */
