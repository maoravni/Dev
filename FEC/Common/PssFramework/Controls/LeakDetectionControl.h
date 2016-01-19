/*
 * LeakDetectionControl.h
 *
 *  Created on: 28 Oct 2014
 *      Author: maora
 */

#ifndef LEAKDETECTIONCONTROL_H_
#define LEAKDETECTIONCONTROL_H_

#include "ControlBase.h"

enum E_LeakDetectionCalibrationStage
{
    E_LeakDetectionCalibrationStage_TankInitialLevel,
    E_LeakDetectionCalibrationStage_DeltaHTX,
    E_LeakDetectionCalibrationStage_TubStabilization,
    E_LeakDetectionCalibrationStage_DeltaTub,
    E_LeakDetectionCalibrationStage_EndCalibration,
    E_LeakDetectionCalibrationStage_Init
};

class LeakDetectionControl: public ControlBase
{
    ElementBase* m_tankLevel;
    ElementBase* m_tubLevel;
    int m_timeout;
    int m_window;
    int m_updateInterval;
    float m_filterCoeff;
    float m_tankLevelFiltered;
    float m_tubLevelFiltered;

    float m_deviation;
    E_LeakDetectionCalibrationStage m_currentStage;

    float m_tankInitialLevel;
    float m_deltaHtx;
    float m_deltaTub;

public:
    LeakDetectionControl();
    virtual ~LeakDetectionControl();

    virtual void updateNotification(ElementBase* element);

    virtual bool setSetpoint(float setpoint, uint32_t sn) {return true;}

    virtual void execute();
//    void executeProtectionCheck(ElementBase* element);
    virtual E_ControlType getControlType()
    {
        return E_ControlType_LeakageDetectionControl;
    }

    virtual ElementBase* getMainControlElement(){return NULL;}

    void setElementTankLevel(ElementBase* input);

    void setElementTubLevel(ElementBase* input)
    {
        m_tubLevel = input;
        m_tubLevel->addObserver(this);
    }

    ElementBase* getElementTankLevel() const
    {
        return m_tankLevel;
    }

    ElementBase* getElementTubLevel() const
    {
        return m_tubLevel;
    }

    void activateControl(unsigned short stage, int timeout, int window, float deviation, int sn);

#ifdef OLD_PROTECTION
    virtual void addProtectionElement(ValidationElementBase* element);
#endif
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate) {};
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock, int checkType) {};

    virtual bool sendNotification();

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool requestValueChange(ElementBase* element){return true;}
    virtual void timeoutExpired(uint16_t timeoutType){}

    virtual E_ActivationState getActivationState(){return E_ActivationState_Unknown;}

private:

    bool waitForStableLevel(float level);
    void calculateLpfCoeff();

};
#endif /* LEAKDETECTIONCONTROL_H_ */
