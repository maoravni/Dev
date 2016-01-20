/*
 e* ModbusInverterControl.h
 *
 *  Created on: 10 בספט 2013
 *      Author: maora
 */

#ifndef MODBUSINVERTERCONTROL_H_
#define MODBUSINVERTERCONTROL_H_

#include "ControlBase.h"
#include <Elements/ElementRepository.h>
#include <Peripherals/ModbusInverterPeripheralBase.h>

class ModbusInverterControl: public ControlBase
{
    ValidationElementFloat* m_requestedSetpoint;
    ElementBase* m_enableOutput; //!< This output enables power to the inverter
    ElementBase* m_setpoint;
    ElementBase* m_outputCurrent;
    ElementBase* m_outputFrequency;
    ElementBase* m_driveStatus;
//    T_ElementVector m_protectionElements;

    bool m_stopping;

public:
    ModbusInverterControl();
    virtual ~ModbusInverterControl();

    bool setSetpointSnActivationDelay(float value, uint32_t sn, uint32_t activationDelay);
    bool setSetpointActivationDelay(float value, uint32_t activationDelay);
    virtual bool setSetpoint(float setpoint, uint32_t sn);

    void setOutputEnableElement(ElementBase* element);
    void setOutputFrequencyElement(ElementBase* element);
    void setOutputCurrentElement(ElementBase* element);
    void setOutputSetpointElement(ElementBase* element);
    void setInverterPeripheral(ModbusInverterPeripheralBase* inverter);

    virtual ElementBase* getMainControlElement(){return m_setpoint;}

    virtual void execute();
//    virtual void executeProtectionCheck(ElementBase* element);
    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType);

    virtual bool sendNotification();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_ModbusInverter;
    }
//    virtual void addProtectionElement(ValidationElementBase* element);
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) {};
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType) {};
    virtual E_ActivationState getActivationState(){return (m_requestedSetpoint > 0) ? E_ActivationState_Active : E_ActivationState_Inactive;}

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    bool onMove2Standby();
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual void setBoardInReady(bool state);

private:
    void updateSetpoints();
};

#endif /* MODBUSINVERTERCONTROL_H_ */
