/*
 * ActivationWithFeedbackControl.h
 *
 *  Created on: 15 баеч 2013
 *      Author: maora
 */

#ifndef ACTIVATIONWITHFEEDBACKCONTROL_H_
#define ACTIVATIONWITHFEEDBACKCONTROL_H_

#include "ControlBase.h"
#include <Elements/ElementRepository.h>
/*
#include <list>
#include <vector>
*/
#include "DeviceThresholdChecker.h"

class IODeviceChecker
{
private:
    ElementBase* m_element;
    bool m_activateHigh;

public:
    IODeviceChecker():m_element(0), m_activateHigh(true){}
    IODeviceChecker(F_FILE* f);

    bool isActive()
    {
        return (m_activateHigh == (m_element->getValueU32() != 0));
    }

    bool isActivateHigh() const
    {
        return m_activateHigh;
    }

    void setActivateHigh(bool activateHigh)
    {
        m_activateHigh = activateHigh;
    }

    ElementBase* getElement() const
    {
        return m_element;
    }

    void setElement( ElementBase* element)
    {
        m_element = element;
    }

    void serialize(F_FILE* f);

    template <class T> friend class Serializer;

};

typedef std::vector<IODeviceChecker> T_IODeviceCheckerList;
typedef std::vector<IODeviceChecker>::iterator T_IODeviceCheckerListIterator;

enum E_ActivationBehaviorOnInit
{
    E_ActivationBehaviorOnInit_Unknown,
    E_ActivationBehaviorOnInit_Deactivate,
    E_ActivationBehaviorOnInit_Activate,
    E_ActivationBehaviorOnInit_AccordingToFeedback,
};

class ActivationWithFeedbackControl: public ControlBase
{
    ElementBase* m_outputEnableDevice;
    ElementBase* m_outputDisableDevice;
    ElementBase* m_feedbackEnabledDevice;
    ElementBase* m_feedbackDisabledDevice;

    T_IODeviceCheckerList* m_pActivateControlDeviceList;
    T_IODeviceCheckerList* m_pDeactivateControlDeviceList;

    uint32_t m_activationOutputValue;
    E_ActivationState m_activationState;
    E_ActivationState m_previousActivationState;
    E_ActivationState m_inProtectionActivationState;

    T_DeviceCheckerList m_feedbackCheckers;
//    T_DeviceCheckerList m_dependentCheckers;

//    portTickType m_activationTimeout;
//    portTickType m_deactivationTimeout;
    portTickType m_ignoreProtectionsDelay;
    bool m_ignoreProtections;

    E_ActivationBehaviorOnInit m_behaviorOnInit;

    bool m_lastProtectionActiveState;
    bool m_lastFeedbackResult;
    bool m_lastDependencyResult;
    int8_t m_activateSignalHigh;
    int8_t m_deactivateSignalHigh;

public:
    ActivationWithFeedbackControl();
    ActivationWithFeedbackControl(F_FILE* f);
    virtual ~ActivationWithFeedbackControl();

    virtual void setOutputEnableDevice(ElementBase* element);
    virtual void setOutputDisableDevice(ElementBase* element);
    virtual void setFeedbackEnabledDevice(ElementBase* element);
    virtual void setFeedbackDisabledDevice(ElementBase* element);
    virtual void addActivateDevice(ElementBase* element, bool activeHigh);
    virtual void addDeactivateDevice(ElementBase* element, bool activeHigh);

    virtual void execute();

    virtual void writeOutputs(E_ActivationState activationState);

    virtual E_ControlType getControlType()
    {
        return E_ControlType_ActivationWithFeedback;
    }

    virtual ElementBase* getMainControlElement()
    {
        return m_outputEnableDevice;
    }

    virtual bool activateControl(int outputValue, uint32_t timeout, uint32_t sn);
    virtual bool setSetpoint(float setpoint, uint32_t sn)
    {
        return activateControl((int)(setpoint), portMAX_DELAY, sn);
    }

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
    virtual bool onMove2Error();
    virtual bool onStopOnEmr();
    virtual bool onRecoverFromEmr();

    virtual bool sendNotification();
//    virtual void addProtectionElement(ValidationElementBase* element);
    virtual void addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate);
//    virtual void addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate,
//            bool lock, int checkType);

    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element);
    virtual void timeoutExpired(uint16_t timeoutType);

    void checkAllFeedbacks(E_ActivationState state, bool &resultConsiderDeactivateOnChange, bool &resultIgnoreDeactiveOnChange);
    void setActivationState(E_ActivationState activationState, uint32_t sn);
    void logFeedbackCheckFailures(E_ActivationState state, E_PSSErrors error);
    void clearFeedbackCheckFailures(E_PSSErrors error);
    void checkCommandDevices();
    void calculateFeedbackOutputs();

    virtual E_ActivationState getActivationState(){return m_activationState;}

    virtual void sendCurrentErrors();

    void setBehaviorOnInit(uint8_t behaviorOnInit)
    {
        m_behaviorOnInit = (E_ActivationBehaviorOnInit) behaviorOnInit;
    }

    portTickType getIgnoreProtectionsDelay() const
    {
        return m_ignoreProtectionsDelay;
    }

    void setIgnoreProtectionsDelay(portTickType ignoreProtectionsDelay)
    {
        m_ignoreProtectionsDelay = ignoreProtectionsDelay;
    }

    bool executeLocalProtectionCheck(ElementBase* element);
    bool executeLocalProtectionCheck();

    virtual void startRecovery();

    void printDependencyErrors();
    void printFeedbackErrors();

    virtual void serialize(F_FILE* f);

private:
    void startIgnoringProtections();
    bool checkValidOutputs();
    void executeBehaviorOnInit();

    template <class T> friend class Serializer;
};

#endif /* ACTIVATIONWITHFEEDBACKCONTROL_H_ */
