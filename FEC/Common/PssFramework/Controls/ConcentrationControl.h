/*
 * ConcentrationControl.h
 *
 *  Created on: 23 Mar 2016
 *      Author: maora
 */

#ifndef CONCENTRATIONCONTROL_H_
#define CONCENTRATIONCONTROL_H_

#include <Controls/ControlBase.h>

class ConcentrationControl: public ControlBase
{
    ElementBase* m_concentration;
    ElementBase* m_tankLevel;
    ElementBase* m_conditionerValve;
    uint8_t m_conditionerValveActivationValue;
    ElementBase* m_waterValve;
    uint8_t m_waterValveActivationValue;

    ValidationElementFloat* m_concentrationLowSetpoint;
    ValidationElementFloat* m_liquidLevelLowSetpoint;
    float m_concentrationHighSetpoint;
    float m_liquidLevelHighSetpoint;

    bool m_fillState;
    bool m_timeoutExpired;


public:
    ConcentrationControl();
    virtual ~ConcentrationControl();

    virtual void updateNotification(ElementBase* element);
    virtual void execute();
//    bool executeLocalProtectionCheck(ElementBase* element);
//    virtual bool executeDependencyCheck(ElementBase* element);
    virtual E_ControlType getControlType()
    {
        return E_ControlType_ConcentrationControl;
    }

    virtual ElementBase* getMainControlElement()
    {
        return m_concentration;
    }

    void setElementConcentrationInput(ElementBase* input)
    {
        m_concentration = input;
        m_concentration->addObserver(this);
        m_concentration->setMissingDevicePriority(E_MissingDevicePriority_High);
//        setSecondaryPssId(m_input->getPssId());
    }

    void setElementTankLevelInput(ElementBase* input)
    {
        m_tankLevel = input;
        m_tankLevel->addObserver(this);
        m_tankLevel->setMissingDevicePriority(E_MissingDevicePriority_High);
        //        setSecondaryPssId(m_input->getPssId());
    }

    void setElementConditionerValve(ElementBase* output, uint8_t activationValue)
    {
        m_conditionerValve = output;
        m_conditionerValve->addObserver(this);
        if (activationValue == 0)
            activationValue = 1;
        m_conditionerValveActivationValue = activationValue;
    }

    void setElementWaterValve(ElementBase* output, uint8_t activationValue)
    {
        m_waterValve = output;
        m_waterValve->addObserver(this);
        if (activationValue == 0)
            activationValue = 1;
        m_waterValveActivationValue = activationValue;
    }

    virtual bool sendNotification();

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool requestValueChange(ElementBase* element)
    {
        return true;
    }
    virtual void timeoutExpired(uint16_t timeoutType);
    virtual E_ActivationState getActivationState();

    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate)
    {
    }
    ;
    virtual bool setSetpoint(float setpoint, uint32_t sn)
    {
        return false;
    }
    bool setSetpoint(float llLow, float llHigh, float llLoRange, float llHighRange, float llLoWarn, float llHighWarn,
            float cndLow, float cndHigh, float cndLoRange, float cndHighRange, float cndLoWarn, float cndHighWarn,
            uint32_t delay, uint32_t sn);


    virtual void serialize(F_FILE* f);
private:
    void resetOutput();
    void executeLevelControl();
    void executeLimitsCheck(ValidationElementFloat* checkingElement, float inputValue);
    void resetControl();

    template <class T> friend class Serializer;
};

#endif /* CONCENTRATIONCONTROL_H_ */
