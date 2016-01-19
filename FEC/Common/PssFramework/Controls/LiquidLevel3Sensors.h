/*
 * LiquidLevel3Sensors.h
 *
 *  Created on: 1 בדצמ 2013
 *      Author: maora
 */

#ifndef LIQUIDLEVEL3SENSORS_H_
#define LIQUIDLEVEL3SENSORS_H_

#include "ControlBase.h"

class LiquidLevel3Sensors: public ControlBase
{
    ElementBase* m_lowSensor;
    ElementBase* m_midSensor;
    ElementBase* m_highSensor;
    ElementBase* m_calculatedOutputLevel;

public:
    LiquidLevel3Sensors();
    virtual ~LiquidLevel3Sensors();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_HysteresisControl;
    }
    virtual ElementBase* getMainControlElement(){return m_calculatedOutputLevel;}

    virtual void execute();

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual void updateNotification(ElementBase* element);
    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}

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
    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

    virtual void setPssId(uint16_t deviceId)
    {
        ControlBase::setPssId(deviceId);
        m_calculatedOutputLevel->setPssId(deviceId);
    }

    void setElementHighSensor(ElementBase* highSensor)
    {
        m_highSensor = highSensor;
        m_highSensor->addObserver(this);
    }

    void setElementLowSensor(ElementBase* lowSensor)
    {
        m_lowSensor = lowSensor;
        m_lowSensor->addObserver(this);
    }

    void setElementMidSensor(ElementBase* midSensor)
    {
        m_midSensor = midSensor;
        m_midSensor->addObserver(this);
    }

    ElementBase* getElementCalculatedOutputLevel()
    {
        return m_calculatedOutputLevel;
    }
};

#endif /* LIQUIDLEVEL3SENSORS_H_ */
