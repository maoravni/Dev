/*
 * ProtectionAggregatorControl.h
 *
 *  Created on: 13 Jul 2016
 *      Author: maora
 */

#ifndef PROTECTIONAGGREGATORCONTROL_H_
#define PROTECTIONAGGREGATORCONTROL_H_

#include <Controls/ControlBase.h>

enum E_BitwiseOperation
{
    E_BitwiseOperation_And,
    E_BitwiseOperation_Or,
    E_BitwiseOperation_Xor,
};

class ProtectionAggregatorControl: public ControlBase
{
protected:
    ElementBase* m_calculatedOutput;
    bool m_negateResult;
    E_BitwiseOperation m_bitwiseOperation;

public:
    ProtectionAggregatorControl();
    ProtectionAggregatorControl(F_FILE* f);
    virtual ~ProtectionAggregatorControl();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_ProtectionAggregatorControl;
    }
    virtual ElementBase* getMainControlElement(){return m_calculatedOutput;}

    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual void execute();


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
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) {};

    virtual void setPssId(uint16_t deviceId)
    {
        ControlBase::setPssId(deviceId);
    }

    void setOutputElement(ElementBase* element)
    {
        m_calculatedOutput = element;
    }

    ElementBase* getOutputElement()
    {
        return m_calculatedOutput;
    }

    void setOperation(E_BitwiseOperation bitOp, bool negateResult);

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

    template <class T> friend class Serializer;

    void serialize(F_FILE* f);
};

#endif /* PROTECTIONAGGREGATORCONTROL_H_ */
