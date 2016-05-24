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

//enum E_DependencyCheckType
//{
//    E_DependencyCheckType_Ignore,
//    E_DependencyCheckType_Activate,
//    E_DependencyCheckType_Deactivate,
//    E_DependencyCheckType_DeactivateAndActivate
//};
//
//enum E_PreviousCheckResult
//{
//    E_DependencyCheckResult_False, E_DependencyCheckResult_True, E_DependencyCheckResult_Unchecked,
//};

//class DeviceThresholdChecker/*: public ErrorBitManager*/
//{
//private:
//    ElementBase* m_element;
//    E_PreviousCheckResult m_previousCheckResult;
//    uint16_t m_pssId;
//
//public:
//    float m_thresholdValue;
//    bool m_greaterThan;
//    E_DependencyCheckType m_dependencyCheckType;
//
//    /// This variable defines whether the observing device should
//    /// be deactivated if feedback device fails while active.
//    bool m_deactivateOnChange;
//
//    /// This variable should affect the observed device - if the observing device
//    /// is the observed device allowed to change state?
//    bool m_lockChange;
//
//    bool checkDevice(/*bool isControlActive*/)
//    {
////        if (isControlActive && !m_deactivateOnChange)
////            return true;
//        if (m_greaterThan)
//            return (m_element->getValueF() >= m_thresholdValue);
//        else
//            return (m_element->getValueF() < m_thresholdValue);
//    }
//
//    bool checkDevice(bool isControlActive)
//    {
//        switch (m_dependencyCheckType)
//        {
//        case E_DependencyCheckType_Ignore:
//            return true;
//        case E_DependencyCheckType_Activate:
//            if (!isControlActive)
//                return true;
//            break;
//        case E_DependencyCheckType_Deactivate:
//            if (isControlActive)
//                return true;
//            break;
//        default:
//            break;
//        }
//
//        return checkDevice();
//    }
//
//    DeviceThresholdChecker() :
//            m_element(NULL), m_thresholdValue(0), m_greaterThan(false), m_dependencyCheckType(
//                    E_DependencyCheckType_DeactivateAndActivate), m_previousCheckResult(
//                    E_DependencyCheckResult_Unchecked), m_deactivateOnChange(false), m_lockChange(false)
//    {
//
//    }
//
//    ElementBase* getElement()
//    {
//        return m_element;
//    }
//
//    void setElement(uint16_t primaryPssId, ElementBase* element)
//    {
//        setPssId(element->getPssId());
//        m_element = element;
////        setSecondaryPssId(m_element->getPssId());
//    }
//
//    // return true if the result is the same as the previous result.
//    bool compareToPreviousCheckResult(bool result)
//    {
//        if (m_previousCheckResult == E_DependencyCheckResult_Unchecked)
//        {
//            m_previousCheckResult = (result == true) ? E_DependencyCheckResult_True : E_DependencyCheckResult_False;
//            return false;
//        }
//        bool previousResult = (m_previousCheckResult == E_DependencyCheckResult_True);
//        m_previousCheckResult = (result) ? E_DependencyCheckResult_True : E_DependencyCheckResult_False;
//        return (previousResult == result);
//    }
//    void resetPreviousCheckResult();
//
//    uint16_t getPssId() const
//    {
//        return m_pssId;
//    }
//
//    void setPssId(uint16_t pssId)
//    {
//        m_pssId = pssId;
//    }
//};

//typedef DynamicArray<DeviceThresholdChecker> T_DeviceChecker;
//typedef std::vector<DeviceThresholdChecker> T_DeviceCheckerList;
//typedef std::vector<DeviceThresholdChecker>::iterator T_DeviceCheckerListIterator;

//enum E_ActivationState
//{
//    E_ActivationState_Unknown = 0, E_ActivationState_Active = 1, E_ActivationState_Inactive = 2,
//};

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

    bool m_lastProtectionState;

    void executeBehaviorOnInit();

public:
    ActivationWithFeedbackControl();
    ActivationWithFeedbackControl(F_FILE* f);
    virtual ~ActivationWithFeedbackControl();

    void setOutputEnableDevice(ElementBase* element);
    void setOutputDisableDevice(ElementBase* element);
//    void addFeedbackDevice(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate);
//    void addDependentDevice(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate, bool lock);

    virtual void execute();

    void writeOutputs(E_ActivationState activationState);

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
//    bool checkAllDependents(E_ActivationState state, E_ControlState controlState);
//    bool checkAllDependenciesForOppositeState();
    void setActivationState(E_ActivationState activationState, uint32_t sn);
    void logFeedbackCheckFailures(E_ActivationState state, E_PSSErrors error);
    void clearFeedbackCheckFailures(E_PSSErrors error);

    virtual E_ActivationState getActivationState(){return m_activationState;}

    //    void logDependencyCheckFailures(E_ActivationState state, E_ControlState controlState, E_PSSErrors error);
//    void clearDependencyCheckFailures(E_PSSErrors error);

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

    template <class T> friend class Serializer;
};

#endif /* ACTIVATIONWITHFEEDBACKCONTROL_H_ */
