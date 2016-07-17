/*
 * ConcentrationCalculatorControl.h
 *
 *  Created on: 12 Jul 2016
 *      Author: maora
 */

#ifndef CONCENTRATIONCALCULATORCONTROL_H_
#define CONCENTRATIONCALCULATORCONTROL_H_

#include "ControlBase.h"

class ConcentrationCalculatorControl: public ControlBase
{
protected:
    ElementBase* m_viscosity;
    ElementBase* m_temperature;
    float m_concentration1, m_intercept1, m_slope1;
    float m_concentration2, m_intercept2, m_slope2;

    ElementBase* m_calculatedOutput;

public:
    ConcentrationCalculatorControl();
    ConcentrationCalculatorControl(F_FILE* f);
    virtual ~ConcentrationCalculatorControl();

    virtual E_ControlType getControlType()
    {
        return E_ControlType_ConcentrationCalculatorControl;
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
//    virtual void addProtectionElement(ValidationElementBase* element);
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) {};
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType) {};

    virtual void setPssId(uint16_t deviceId)
    {
        ControlBase::setPssId(deviceId);
//        m_calculatedOutput->setPssId(deviceId);
    }

    void setViscosityElement(ElementBase* element)
    {
        m_viscosity = element;
        m_viscosity->addObserver(this);
    }

    void setTemperatureElement(ElementBase* element)
    {
        m_temperature = element;
        m_temperature->addObserver(this);
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

    void setCoefficients(float c1, float i1, float s1, float c2, float i2, float s2);

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

    template <class T> friend class Serializer;

    void serialize(F_FILE* f);
};

#endif /* CONCENTRATIONCALCULATORCONTROL_H_ */
