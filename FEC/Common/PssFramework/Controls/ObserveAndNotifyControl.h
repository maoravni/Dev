/*
 * ObserveAndNotifyControl.h
 *
 *  Created on: 30 Jul 2015
 *      Author: vitalyp
 */

#ifndef OBSERVE_AND_NOTIFY_CONTROL_H_
#define OBSERVE_AND_NOTIFY_CONTROL_H_

#include <Controls/ControlBase.h>

class ObserveAndNotifyControl: public ControlBase
{
    ValidationElementFloat * m_input;
    ValidationElementFloat * m_setpoint;

public:
    ObserveAndNotifyControl();
    virtual ~ObserveAndNotifyControl();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_ObserveAndNotifyControl;
    }

//    bool initInputElements(int cableId);
    virtual void execute();
    virtual bool sendNotification();
    virtual void updateNotification(ElementBase* element);
    virtual void timeoutExpired(uint16_t timeoutType)
    {
    }

    virtual bool setSetpoint(float setpoint, uint32_t sn);
    bool setSetpoint(float sp, float loRange, float hiRange, float loWarn, float hiWarn, uint32_t sn);
    bool setSetpoint(ValidationElementFloat* element);
    void setElementInput(ValidationElementFloat* input);
//    void setElementSetpoint(ValidationElementFloat* setpoint);

    virtual ElementBase* getMainControlElement()
    {
        return NULL;
    }

    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate)
    {
    }

    virtual E_ActivationState getActivationState()
    {
        return E_ActivationState_Active;
    }

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
};

#endif /* OBSERVE_AND_NOTIFY_CONTROL_H_ */
