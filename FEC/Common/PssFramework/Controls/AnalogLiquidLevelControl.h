/*
 * AnalogLiquidLevelControl.h
 *
 *  Created on: 1 בדצמ 2013
 *      Author: maora
 */

#ifndef AnalogLiquidLevelControl_H_
#define AnalogLiquidLevelControl_H_

#include "ControlBase.h"

class AnalogLiquidLevelControl: public ControlBase
{
    float m_lowLevelValue;
    float m_midLevelValue;
    float m_highLevelValue;
    ElementBase* m_levelSensorElement;

    ElementBase* m_calculatedOutputLevel;

public:
    AnalogLiquidLevelControl();
    virtual ~AnalogLiquidLevelControl();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_HysteresisControl;
    }
    virtual ElementBase* getMainControlElement(){return m_calculatedOutputLevel;}

    virtual void execute();
    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();


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

    virtual void setPssId(uint16_t deviceId)
    {
        ControlBase::setPssId(deviceId);
        m_calculatedOutputLevel->setPssId(deviceId);
    }

    void setElementLevelSensor(ElementBase* element)
    {
        m_levelSensorElement = element;
        m_levelSensorElement->addObserver(this);
    }

    void setLevelThresholds(float low, float mid, float high);

    ElementBase* getElementCalculatedOutputLevel()
    {
        return m_calculatedOutputLevel;
    }

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}
};



#endif /* AnalogLiquidLevelControl_H_ */
