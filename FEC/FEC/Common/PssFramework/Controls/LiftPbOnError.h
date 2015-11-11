/*
 * LiftPbOnError.h
 *
 *  Created on: 3 Jul 2014
 *      Author: maora
 */

#ifndef LIFTPBONERROR_H_
#define LIFTPBONERROR_H_

#include "ControlBase.h"

class LiftPbOnError: public ControlBase
{
    ElementBase* m_airPressure;
    ElementBase* m_tubEngage[4];
    ElementBase* m_squeegeeEngage[2];
    ElementBase* m_pbOk;

    int m_cableId;

public:
    LiftPbOnError();
    virtual ~LiftPbOnError();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_LiftPbOnError;
    }

    bool initInputElements(int cableId);
    virtual void execute();
    virtual bool sendNotification();
    virtual void updateNotification(ElementBase* element);
    virtual void timeoutExpired(uint16_t timeoutType)
    {
    }

    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}

    virtual ElementBase* getMainControlElement()
    {
        return NULL;
    }

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

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

    virtual bool onInitControl();
    virtual bool onMove2Standby();
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool requestValueChange(ElementBase* element)
    {
        return true;
    }
};

#endif /* LIFTPBONERROR_H_ */
