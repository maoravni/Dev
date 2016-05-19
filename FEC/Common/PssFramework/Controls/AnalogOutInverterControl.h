/*
 * AnalogOutInverterControl.h
 *
 *  Created on: 6 баев 2013
 *      Author: maora
 */

#ifndef ANALOGOUTINVERTERCONTROL_H_
#define ANALOGOUTINVERTERCONTROL_H_

#include "ControlBase.h"
#include <Elements/ElementRepository.h>
#include <Persistency/ControlSerializers.h>

class AnalogOutInverterControl: public ControlBase
{
    ElementU8* m_enableOutput; //!< This output enables power to the inverter
    ValidationElementFloat* m_deviceSetpoint;
    ValidationElementFloat* m_requestedSetpointElement;

public:
    AnalogOutInverterControl();
    virtual ~AnalogOutInverterControl();

    bool setSetpoint(float value, uint32_t sn);

    virtual ElementBase* getMainControlElement(){return m_requestedSetpointElement;}

    void setOutputEnableElement(ElementU8* element);
    void setSetpointElement(ValidationElementFloat* element);

    virtual void execute();
    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual bool sendNotification();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_AnalogOutInverter;
    }
//    virtual void addProtectionElement(ValidationElementBase* element);
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) {};
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType) {};

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual E_ActivationState getActivationState(){return (m_requestedSetpointElement > 0) ? E_ActivationState_Active : E_ActivationState_Inactive;}

    virtual void serialize(F_FILE* f);


private:
    void setOutputValue(float value);

    template <class T> friend class Serializer;

};

#endif /* ANALOGOUTINVERTERCONTROL_H_ */
