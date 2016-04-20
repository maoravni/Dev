/*
 * ProtectionControl.h
 *
 *  Created on: 24 αιεμ 2013
 *      Author: maora
 */

#ifndef PROTECTIONCONTROL_H_
#define PROTECTIONCONTROL_H_

#include "ControlBase.h"
#include <DynamicArray.h>
#include <Elements/ValidationElementBase.h>
#include <elements/Element.h>

//typedef DynamicArray<ValidationElementBase*> T_ElementArray;

class ProtectionControl: public ControlBase
{
//    T_ElementArray m_observedElementArray;
    ElementBase* m_dryContactElement;

public:
    ProtectionControl();
    virtual ~ProtectionControl();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_Protection;
    }

    virtual void execute();
    virtual bool sendNotification();
    virtual void updateNotification(ElementBase* element);
    virtual void timeoutExpired(uint16_t timeoutType)
    {
    }

    virtual ElementBase* getMainControlElement()
    {
        return m_dryContactElement;
    }

    void setDryContactElement(ElementU8* dryContactElement)
    {
        m_dryContactElement = dryContactElement;

        // turn on the contactor when assigning it:
        m_dryContactElement->setValue((uint32_t)1);
    }

//    virtual void addProtectionElement(ValidationElementBase* element);
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate)
    {
    }
    ;
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType) {};

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool setSetpoint(float setpoint, uint32_t sn)
    {
        return true;
    }
    virtual bool requestValueChange(ElementBase* element)
    {
        return true;
    }
    virtual DeviceProtectionChecker* createDeviceProtectionChecker();
    virtual void addProtectionCheckerButDontSubsribe(ProtectionCheckerBase* protection);

    bool getElementUpperHardLimit(ElementBase* element, float& limit);
    bool getElementLowerHardLimit(ElementBase* element, float& limit);

    virtual E_ActivationState getActivationState()
    {
        return E_ActivationState_Unknown;
    }

    virtual void startRecovery();

private:
    DeviceProtectionChecker* getElementInProtection(E_ProtectionCheckerType checkerType, ElementBase* element);
};

inline bool ProtectionControl::onInitControl()
{
    endInitControl();
    return true;
}

inline bool ProtectionControl::onMove2Standby(uint32_t delay)
{
    endMove2Standby();
    return true;
}

inline bool ProtectionControl::onReset2On()
{
    endReset2On();
    return true;
}

inline bool ProtectionControl::onMove2Error()
{
    endMove2Error();
    return true;
}

#endif /* PROTECTIONCONTROL_H_ */
