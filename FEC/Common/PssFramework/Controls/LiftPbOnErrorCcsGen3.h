/*
 * LiftPbOnErrorCcsGen3.h
 *
 *  Created on: 28 Jul 2015
 *      Author: maora
 */

#ifndef LIFTPBONERRORCCSGen3_H_
#define LIFTPBONERRORCCSGen3_H_

#include <Controls/ControlBase.h>

class LiftPbOnErrorCcsGen3: public ControlBase
{
    ElementBase* m_airPressureUp;
    ElementBase* m_airPressureDown;
    ElementBase* m_blanketMoving;
    ElementBase* m_airPressureOk;
    ElementBase* m_pbOk;
//    bool m_prevPbOk;

    int m_cableId;

public:
    LiftPbOnErrorCcsGen3();
    LiftPbOnErrorCcsGen3(F_FILE* f);
    virtual ~LiftPbOnErrorCcsGen3();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_LiftPbOnErrorCcsGen3;
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

    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate)
    {
    }

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

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

    virtual void serialize(F_FILE* f);

    template <class T> friend class Serializer;
};

#endif /* LIFTPBONERRORCCSGen3_H_ */
