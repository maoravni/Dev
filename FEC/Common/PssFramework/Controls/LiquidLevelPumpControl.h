/*
 * LiquidLevelPumpControl.h
 *
 *  Created on: 1 בדצמ 2013
 *      Author: maora
 */

#ifndef LIQUIDLEVELPUMPCONTROL_H_
#define LIQUIDLEVELPUMPCONTROL_H_

#include "ControlBase.h"

class LiquidLevelPumpControl: public ControlBase
{
    ElementBase* m_tankLevelInput;

    // This control will control the speed of the drain pump.
    ElementBase* m_drainPump;

    // The fill pump is here just for safety - when we reach the highest level of the tank we need to stop the fill.
    ElementBase* m_fillPump;

    ElementBase* m_lowSetpoint;
    ElementBase* m_midSetpoint;
    ElementBase* m_highSetpoint;

    float m_currentSetpoint;
    float m_fillPumpValue;
    bool m_fillPumpProtectionActive;
    bool m_stopRequested;
    E_ControlState m_requestedState;

    int m_currentTankLevel;

public:
    LiquidLevelPumpControl();
    virtual ~LiquidLevelPumpControl();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_LiquidLevelPumpControl;
    }

    virtual ElementBase* getMainControlElement() {return NULL;}

    virtual void execute();

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}


    virtual void updateNotification(ElementBase* element);

    /**
     * This method asks the observer if a value change is permitted.
     * It's main use is when an output is required to change it's value
     * and this means that some other controls should be disabled,
     * or the output change is not permitted.
     * @param element
     */
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType) {}

    virtual bool sendNotification();
//    virtual void addProtectionElement(ValidationElementBase* element);
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) {};
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType) {};
    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

    virtual bool setSetpoint(float low, float mid, float high, uint32_t sn);

    void setElementDrainPump(ElementBase* drainPump)
    {
        m_drainPump = drainPump;
    }

    void setElementFillPump(ElementBase* fillPump)
    {
        m_fillPump = fillPump;
        m_fillPump->addObserver(this);
    }

//    void setElementHighSetpoint(ElementBase* highSetpoint)
//    {
//        m_highSetpoint = highSetpoint;
//    }
//
//    void setElementLowSetpoint(ElementBase* lowSetpoint)
//    {
//        m_lowSetpoint = lowSetpoint;
//    }
//
//    void seElementtMidSetpoint(ElementBase* midSetpoint)
//    {
//        m_midSetpoint = midSetpoint;
//    }

    void setElementTankLevelInput(ElementBase* tankLevelInput)
    {
        m_tankLevelInput = tankLevelInput;
        m_tankLevelInput->addObserver(this);
    }

    ElementBase* getElementHighSetpoint() const
    {
        return m_highSetpoint;
    }

    ElementBase* getElementLowSetpoint() const
    {
        return m_lowSetpoint;
    }

    ElementBase* getElementMidSetpoint() const
    {
        return m_midSetpoint;
    }

    virtual void serialize(F_FILE* f);

    template <class T> friend class Serializer;
};

#endif /* LIQUIDLEVELPUMPCONTROL_H_ */
