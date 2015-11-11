/*
 * HysteresisControl.h
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#ifndef HysteresisControl_H_
#define HysteresisControl_H_

#include "ControlBase.h"
#include <Elements/Element.h>
#include <PIDDynamicSampleTime.h>
#include <PID_AutoTune_v0.h>
#include <PscServer/PscMessageStructs.h>

//typedef DynamicArray<ValidationElementBase*> T_ElementArray;

class HysteresisControl: public ControlBase
{
    ValidationElementFloat* m_input;
    ElementBase* m_output;
    // TODO: Rename the names of the variables, to make them less confiusing, especially since we can now control cooling.
    // TODO: Probably name them upper and lower.
    // TODO: Problem: after refactoring, need to add parameter that describes whether we're cooling or heating.
    ValidationElementFloat* m_activateSetpointElement;
    ValidationElementFloat* m_deactivateSetpointElement;
    float m_activateSetpoint;
    float m_deactivateSetpoint;
    float m_outputValue;
//    T_ElementArray m_protectionElements;

    float m_maxTemperature;
    bool m_coolingMode;

    // TODO: Check if protection is required in the hysteresis control?
//    bool m_isProtectionActive;

public:
    HysteresisControl();
    virtual ~HysteresisControl();

    virtual void updateNotification(ElementBase* element);
    virtual void execute();
//    void executeProtectionCheck(ElementBase* element);
    virtual E_ControlType getControlType()
    {
        return E_ControlType_HysteresisControl;
    }

    virtual ElementBase* getMainControlElement(){return m_deactivateSetpointElement;}

    float getKp();
    float getKi();
    float getKd();

    void setElementInput(ValidationElementFloat* input)
    {
        m_input = input;
        m_input->addObserver(this);
//        setSecondaryPssId(m_input->getPssId());
    }

    void setElementOutput(ElementBase* output)
    {
        m_output = output;
//        m_output->addObserver(this);
    }

    ValidationElementFloat* getElementInput() const
    {
        return m_input;
    }

    ElementBase* getElementOutput() const
    {
        return m_output;
    }

    ValidationElementFloat* getElementActivateSetpoint() const
    {
        return m_activateSetpointElement;
    }

    ValidationElementFloat* getElementDeactivateSetpoint() const
    {
        return m_deactivateSetpointElement;
    }

#ifdef OLD_PROTECTION
    virtual void addProtectionElement(ValidationElementBase* element);
#endif
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) {};
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType) {};

    virtual bool sendNotification();

    virtual bool setSetpoint(float actSp, float deactSp, float loRange, float hiRange, float loWarn, float hiWarn, uint32_t sn);
    virtual bool setSetpoint(ValidationElementFloat* activateElement, ValidationElementFloat* deactivateElement);
    virtual bool setSetpoint(float setpoint, uint32_t sn);


    virtual bool onInitControl();
    virtual bool onMove2Standby();
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual E_ActivationState getActivationState();

    float getOutputValue() const
    {
        return m_outputValue;
    }

    void setOutputValue(float outputValue)
    {
        m_outputValue = outputValue;
    }

    void executeControl(float inputValue);

private:
    void setElementActivateSetpoint(ValidationElementFloat* setpoint)
    {
        m_activateSetpointElement = setpoint;
        m_activateSetpointElement->addObserver(this);
    }

    void setElementDeactivateSetpoint(ValidationElementFloat* setpoint)
    {
        m_deactivateSetpointElement = setpoint;
        m_deactivateSetpointElement->addObserver(this);
    }

};

#endif /* HysteresisControl_H_ */
