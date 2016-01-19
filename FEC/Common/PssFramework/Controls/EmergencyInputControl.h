/*
 * EmergencyInputControl.h
 *
 *  Created on: 29 באפר 2014
 *      Author: maora
 */

#ifndef EMERGENCYINPUTCONTROL_H_
#define EMERGENCYINPUTCONTROL_H_

#include <Controls/ControlBase.h>
#include <Elements/ElementRepository.h>

class EmergencyInputControl: public ControlBase
{
    ElementBase* m_emergencyInputElement;
    bool m_emergencyActive;
public:
    EmergencyInputControl();
    virtual ~EmergencyInputControl();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_EmergencyInputControl;
    }

    virtual void execute();
    virtual bool sendNotification();
    virtual void updateNotification(ElementBase* element);
    virtual void timeoutExpired(uint16_t timeoutType)
    {
    }

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

    virtual ElementBase* getMainControlElement()
    {
        return m_emergencyInputElement;
    }

    void setEmergencyInputElement(ElementBase* emergencyInputElement);

//    virtual void addProtectionElement(ValidationElementBase* element)
//    {
//    }
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate)
    {
    }
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate,
//            bool lock, int checkType)
//    {
//    }

    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}

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

    bool isEmergencyActive();

};

#endif /* EMERGENCYINPUTCONTROL_H_ */
