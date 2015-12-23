/*
 * CalculateOnTwoDevicesControl.h
 *
 *  Created on: 29 Sep 2015
 *      Author: maora
 */

#ifndef CALCULATEONTWODEVICESCONTROL_H_
#define CALCULATEONTWODEVICESCONTROL_H_

#include "ControlBase.h"

class CalculateOnTwoDevicesControl: public ControlBase
{
protected:
    ElementBase* m_input1;
    ElementBase* m_input2;
    float m_previousInput1, m_previousInput2;

    ElementBase* m_calculatedOutput;

public:
    CalculateOnTwoDevicesControl();
    virtual ~CalculateOnTwoDevicesControl();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_CalculateOnTwoDevices;
    }
    virtual ElementBase* getMainControlElement(){return m_calculatedOutput;}

    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}

    virtual bool onInitControl();
    virtual bool onMove2Standby();
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
//        m_calculatedOutput->setPssId(deviceId);
    }

    void setInput1Element(ElementBase* element)
    {
        m_input1 = element;
        m_input1->addObserver(this);
    }

    void setInput2Element(ElementBase* element)
    {
        m_input2 = element;
        m_input2->addObserver(this);
    }

    void setOutputElement(ElementBase* element)
    {
        m_calculatedOutput = element;
        //m_input2->addObserver(this);
    }

    ElementBase* getOutputElement()
    {
        return m_calculatedOutput;
    }

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}
};

class SubtractTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    SubtractTwoDevicesControl();
    virtual ~SubtractTwoDevicesControl();

    virtual void execute();
};

class AddTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    AddTwoDevicesControl();
    virtual ~AddTwoDevicesControl();

    virtual void execute();
};

class MultiplyTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    MultiplyTwoDevicesControl();
    virtual ~MultiplyTwoDevicesControl();

    virtual void execute();
};

class DivideTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    DivideTwoDevicesControl();
    virtual ~DivideTwoDevicesControl();

    virtual void execute();
};

class MaxTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    MaxTwoDevicesControl();
    virtual ~MaxTwoDevicesControl();

    virtual void execute();
};

class MinTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    MinTwoDevicesControl();
    virtual ~MinTwoDevicesControl();

    virtual void execute();
};

#endif /* CALCULATEONTWODEVICESCONTROL_H_ */
