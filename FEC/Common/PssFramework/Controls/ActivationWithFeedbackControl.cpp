/*
 * ActivationWithFeedbackControl.cpp
 *
 *  Created on: 15 баеч 2013
 *      Author: maora
 */

#include "ActivationWithFeedbackControl.h"
#include <PscServer/PscMasterServer.h>
#include <PscSubsystem.h>
#include <logger.h>
#include <Tasks/UpdateSchedulerTask.h>

#define M_ACTIVATION_TIMEOUT_TYPE 0
#define M_IGNORE_PROTECTION_DELAY_TYPE 1

ActivationWithFeedbackControl::ActivationWithFeedbackControl()
{
    m_outputEnableDevice = NULL;
    m_outputDisableDevice = NULL;
//    m_activationTimeout = 0;
//    m_deactivationTimeout = 0;
    m_activationState = E_ActivationState_Unknown;
    m_previousActivationState = E_ActivationState_Unknown;
    m_behaviorOnInit = E_ActivationBehaviorOnInit_Unknown;
    m_lastProtectionState = false;
    m_ignoreProtections = false;
    m_activationOutputValue = 1;
    setIgnoreProtectionsDelay(0);
}

ActivationWithFeedbackControl::~ActivationWithFeedbackControl()
{
    T_DeviceCheckerListIterator dclIt;

    for (dclIt = m_feedbackCheckers.begin(); dclIt != m_feedbackCheckers.end(); ++dclIt)
    {
        (*dclIt).getElement()->removeObserver(this);
    }

//    for (dclIt = m_dependentCheckers.begin(); dclIt != m_dependentCheckers.end(); ++dclIt)
//    {
//        (*dclIt).getElement()->removeObserver(this);
//    }
}

void ActivationWithFeedbackControl::addFeedbackElement(ElementBase* element, float thresholdValue, bool greaterThan,
        bool deactivate)
{
    DeviceThresholdChecker checker;
    element->addObserver(this);
    checker.setElement(getPssId(), element);
    checker.m_thresholdValue = thresholdValue;
    checker.m_greaterThan = greaterThan;
    checker.m_deactivateOnChange = deactivate;
    checker.m_lockChange = false;
    m_feedbackCheckers.push_back(checker);
}

//void ActivationWithFeedbackControl::addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan,
//        bool deactivate, bool lock, int checkType)
//{
//    DeviceThresholdChecker checker;
//    element->addObserver(this);
//    checker.setElement(getPssId(), element);
//    checker.m_thresholdValue = thresholdValue;
//    checker.m_greaterThan = greaterThan;
//    checker.m_deactivateOnChange = deactivate;
//    checker.m_lockChange = lock;
//    checker.m_dependencyCheckType = (E_DependencyCheckType) checkType;
//    m_dependentCheckers.push_back(checker);
//}

void ActivationWithFeedbackControl::execute()
{
    bool feedbacksConsider, feedbacksIgnore;
    bool depsConsider, depsIgnore;
    bool outputsValid = checkValidOutputs();
    switch (m_controlState)
    {
    case E_ControlState_Move2Ready:
        checkAllFeedbacks(m_activationState, feedbacksConsider, feedbacksIgnore);
        checkAllDependencies(m_activationState, m_controlState, depsConsider, depsIgnore);
//        if (m_outputEnableDevice->getValueI32() == 1)
        if (m_activationState == E_ActivationState_Active)
        {
            if (feedbacksConsider)
                clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);
            // check all feedback devices. If they're all activated, move to ready:
            if (feedbacksConsider && depsConsider && outputsValid)
            {
                m_controlState = E_ControlState_Ready;

                // store the successfull activation state.
//                m_previousActivationState = m_activationState;
            }

            // if the dependency check failed:
            if (!depsConsider)
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deactivating control {[PSSID:%d]}", getPssId());
                // send errors to the OPC about the errors.
                logDependencyCheckFailures(m_activationState, m_controlState, E_PSSErrors_ActivationFailed);
                // issue a sequence ended error.
                if (m_lastSn != 0)
                {
                    PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateActivationWithFeedbackControl, m_lastSn,
                            Psc_ControllerId, getPssId(), E_SeqEndedStatus_Error);
                    m_lastSn = 0;
                }
                // TODO: Control should move to error when this scenario occurs.
//                raiseError(0, E_PSSErrors_ActivationFailed, true);
                // if also the dependencies, considering the "deactivate on change" has failed,
                // change the activation state.
                if (!depsConsider)
                    setActivationState(m_previousActivationState, 0);
            }
            else
                clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
        }
        else
        {
            if (!feedbacksConsider)
                clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);
            if (!depsConsider)
                clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
            if (!(feedbacksConsider && depsConsider))
                m_controlState = E_ControlState_Ready;
        }
        if (m_controlState == E_ControlState_Ready)
        {
            sendNotification();
            if (m_lastSn != 0)
            {
                PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateActivationWithFeedbackControl, m_lastSn,
                        Psc_ControllerId, getPssId(), E_SeqEndedStatus_Success);
                m_lastSn = 0;
            }
        }
        // TOOD: When timeout expires,send seq ended failure.
        break;
    case E_ControlState_Ready:
        checkAllFeedbacks(m_activationState, feedbacksConsider, feedbacksIgnore);
        checkAllDependencies(m_activationState, m_controlState, depsConsider, depsIgnore);
//        if (m_outputEnableDevice->getValueI32() == 1 && !(result && depResult))
        if (m_activationState == E_ActivationState_Active)
        {
//            if (!depsIgnore)
//                logDependencyCheckFailures(m_activationState, m_controlState, E_PSSErrors_ActivationFailed);
//            if (!feedbacksIgnore)
//                logFeedbackCheckFailures(m_activationState, E_PSSErrors_ActivationFailed);

            if (!(feedbacksConsider && depsConsider && outputsValid))
            {
                // TODO: Issue an error when the feedback devices change state while in ready.
                m_controlState = E_ControlState_Standby;
                logDependencyCheckFailures(m_activationState, m_controlState, E_PSSErrors_ActivationFailed);
                logFeedbackCheckFailures(m_activationState, E_PSSErrors_ActivationFailed);
//                raiseError(0, E_PSSErrors_ActivationFailed, true);
                setActivationState(m_previousActivationState, 0);
                sendNotification();
            }
        }
        break;
    default:
//        checkAllDependencies(m_activationState, m_controlState);
        break;
    }
}

bool ActivationWithFeedbackControl::activateControl(int outputValue, uint32_t timeout, uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return false;

    bool depsConsider, depsIgnore;

//    raiseError(0, E_PSSErrors_ActivationFailed, false);
//    clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);
    clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);

    // TODO: check this code.
//    for (T_DeviceCheckerListIterator i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//    {
//        raiseError((*i).getPssId(), E_PSSErrors_ActivationFailed, false);
//        (*i).resetPreviousCheckResult();
//    }

    m_controlState = E_ControlState_Move2Ready;
    m_activationOutputValue = outputValue;

    if (m_activationOutputValue != 0)
    {
        // check if all dependent devices are in the correct state.
        checkAllDependencies(E_ActivationState_Active, m_controlState, depsConsider, depsIgnore);
        // if we are already at the requested activation state, ignore the dependencies and start the activation.
        if ((m_previousActivationState == E_ActivationState_Active) || depsConsider)
        {
            raiseError(0, E_PSSErrors_ActivationFailed, false);
            clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);

            setActivationState(E_ActivationState_Active, sn);
            // make sure that if there's an existing timeout its removed.
            UpdateSchedulerTask::getInstance()->addTimeout(this, 0, M_ACTIVATION_TIMEOUT_TYPE);
            if (timeout > 0 && timeout != ((uint32_t) - 1))
                UpdateSchedulerTask::getInstance()->addTimeout(this, timeout, M_ACTIVATION_TIMEOUT_TYPE);
            execute();
//            if (m_outputEnableDevice != NULL)
//                *m_outputEnableDevice = 1;
//            if (m_outputDisableDevice != NULL)
//                *m_outputDisableDevice = 0;
        }
        else
        {
            // Issue an error that not all dependents are in they're required state.
            PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateActivationWithFeedbackControl, sn, Psc_ControllerId,
                    getPssId(), E_SeqEndedStatus_Error);
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Activation of {[PSSID:%d]} failed: not all dependencies are filled.",
                    getPssId());
            logDependencyCheckFailures(E_ActivationState_Active, m_controlState, E_PSSErrors_ActivationFailed);
        }
        return depsConsider;
    }
    else
    {
        // check if all dependent devices are in the correct state.
        checkAllDependencies(E_ActivationState_Inactive, m_controlState, depsConsider, depsIgnore);
        if ((m_previousActivationState == E_ActivationState_Inactive) || !depsConsider)
        {
            raiseError(0, E_PSSErrors_ActivationFailed, false);
            clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);

            setActivationState(E_ActivationState_Inactive, sn);
            // make sure that if there's an existing timeout its removed.
            UpdateSchedulerTask::getInstance()->addTimeout(this, 0, M_ACTIVATION_TIMEOUT_TYPE);
            if (timeout > 0 && timeout != ((uint32_t) - 1))
                UpdateSchedulerTask::getInstance()->addTimeout(this, timeout, M_ACTIVATION_TIMEOUT_TYPE);
            execute();
//        if (m_outputEnableDevice != NULL)
//            *m_outputEnableDevice = 0;
//        if (m_outputDisableDevice != NULL)
//            *m_outputDisableDevice = 1;
        }
        else
        {
            // Issue an error that not all dependents are in they're required state.
            PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateActivationWithFeedbackControl, sn, Psc_ControllerId,
                    getPssId(), E_SeqEndedStatus_Error);
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Deactivation of {[PSSID:%d]} failed: not all dependencies are filled.",
                    getPssId());
            logDependencyCheckFailures(E_ActivationState_Inactive, m_controlState, E_PSSErrors_ActivationFailed);
        }
        return !depsConsider;
    }

}

void ActivationWithFeedbackControl::executeBehaviorOnInit()
{
    bool resultConsider, resultIgnore;
    switch (m_behaviorOnInit)
    {
    case E_ActivationBehaviorOnInit_Unknown:
        setActivationState(E_ActivationState_Unknown, m_lastSn);
        break;
    case E_ActivationBehaviorOnInit_Deactivate:
        setActivationState(E_ActivationState_Inactive, m_lastSn);
        break;
    case E_ActivationBehaviorOnInit_Activate:
        setActivationState(E_ActivationState_Active, m_lastSn);
        break;
    case E_ActivationBehaviorOnInit_AccordingToFeedback:
        checkAllFeedbacks(E_ActivationState_Active, resultConsider, resultIgnore);
        if (resultConsider)
        {
            setActivationState(E_ActivationState_Active, m_lastSn);
            m_previousActivationState = E_ActivationState_Active;
        }
        else
        {
            checkAllFeedbacks(E_ActivationState_Inactive, resultConsider, resultIgnore);
            if (resultConsider == false)
            {
                setActivationState(E_ActivationState_Inactive, m_lastSn);
                m_previousActivationState = E_ActivationState_Inactive;
            }
            else
            {
                // TODO: Set the control in error state.
            }
        }

        break;
    }
    checkAllDependenciesForOppositeState();
}

bool ActivationWithFeedbackControl::onInitControl()
{
    executeBehaviorOnInit();
    ControlBase::endInitControl();
    return true;
}

bool ActivationWithFeedbackControl::onStopOnEmr()
{
    //    bool result = activateControl(false);
    //    setActivationState(E_ActivationState_Unknown, m_lastSn);
    m_controlState = E_ControlState_Standby;
    executeBehaviorOnInit();
    ControlBase::endStopOnEmr();
    return true;
}

bool ActivationWithFeedbackControl::onMove2Standby()
{
//    bool result = activateControl(false);
//    setActivationState(E_ActivationState_Unknown, m_lastSn);
    m_controlState = E_ControlState_Standby;
    executeBehaviorOnInit();
    ControlBase::endMove2Standby();
    return true;
}

bool ActivationWithFeedbackControl::onReset2On()
{
//    bool result = activateControl(false);
    setActivationState(E_ActivationState_Unknown, m_lastSn);
//    m_controlState = E_ControlState_On;
    ControlBase::endReset2On();
    return true;
}

bool ActivationWithFeedbackControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_activationState;
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_U32;
//    printf("control state: %d\n", m_controlState);
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

//void ActivationWithFeedbackControl::addProtectionElement(ValidationElementBase* element)
//{
//}
//
void ActivationWithFeedbackControl::updateNotification(ElementBase* element)
{
    if (m_controlState == E_ControlState_On)
        return;

    execute();

    checkAllDependenciesForOppositeState();

    if (m_controlState == E_ControlState_Move2Ready || m_controlState == E_ControlState_Ready)
        executeLocalProtectionCheck(element);
}

bool ActivationWithFeedbackControl::requestValueChange(ElementBase* element)
{
    /*
     T_DeviceCheckerListIterator i;

     // search for the element in the dependents list:
     for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
     {
     if (element == (*i).m_element)
     break;
     }
     if (element == (*i).m_element && m_outputEnableDevice->getValueF() != 0)
     return false;
     */
    return true;
}

void ActivationWithFeedbackControl::timeoutExpired(uint16_t timeoutType)
{
    switch (timeoutType)
    {
    case M_ACTIVATION_TIMEOUT_TYPE:
        if (m_controlState != E_ControlState_Move2Ready)
            return;
        // if the state is not "Ready" it means that not all feedbacks and dependencies have been filled.
        // so we return to the previous activation state.
        if (m_lastSn != 0)
        {
            PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateActivationWithFeedbackControl, m_lastSn,
                    Psc_ControllerId, getPssId(), E_SeqEndedStatus_Timeout);
            m_lastSn = 0;
        }
        setActivationState(m_previousActivationState, 0);
        raiseError(0, E_PSSErrors_ActivationFailed, true);
        logFeedbackCheckFailures(m_activationState, E_PSSErrors_ActivationFailed);
        logDependencyCheckFailures(m_activationState, m_controlState, E_PSSErrors_ActivationFailed);
        break;
    case M_IGNORE_PROTECTION_DELAY_TYPE:
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Control {[PSSID:%d]} ignore protections expired", getPssId());
        m_ignoreProtections = false;
        executeLocalProtectionCheck();
        break;
    }
}

/**
 * Check all feedbacks for the state of the control.
 * The "state" parameters controls whether to check if all feedbacks are true, or if all feedbacks are false.
 * If "state" is "Active", the function will return "true" if all feedbacks are true.
 * If "state" is "Inactive", the function will return "false" if all feedbacks are false.
 * @param state
 * @return
 */
void ActivationWithFeedbackControl::checkAllFeedbacks(E_ActivationState activationState,
        bool &resultConsiderDeactivateOnChange, bool &resultIgnoreDeactiveOnChange)
{
    T_DeviceCheckerListIterator i;
    bool tempConsider, tempIgnore;
    if (m_controlState == E_ControlState_Ready)
    {
        if (activationState == E_ActivationState_Active)
        {
            resultConsiderDeactivateOnChange = true;
            resultIgnoreDeactiveOnChange = true;
            for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
            {
                tempIgnore = (*i).checkDevice();
                tempConsider = tempIgnore || !(*i).m_deactivateOnChange;
                resultConsiderDeactivateOnChange &= tempConsider;
                resultIgnoreDeactiveOnChange &= tempIgnore;
            }
            return;
        }
        else
        {
            resultConsiderDeactivateOnChange = false;
            resultIgnoreDeactiveOnChange = false;
            for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
            {
                tempIgnore = !((*i).checkDevice());
                tempConsider = !((*i).checkDevice() && !(*i).m_deactivateOnChange);
                resultConsiderDeactivateOnChange |= tempConsider;
                resultIgnoreDeactiveOnChange |= tempIgnore;
            }
            return;
        }
    }
    else
    {
        if (activationState == E_ActivationState_Active)
        {
            resultConsiderDeactivateOnChange = true;
            for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
            {
                tempConsider = (*i).checkDevice();
                resultConsiderDeactivateOnChange &= tempConsider;
            }
            resultIgnoreDeactiveOnChange = resultConsiderDeactivateOnChange;
            return;
        }
        else
        {
            resultConsiderDeactivateOnChange = false;
            for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
            {
                tempConsider = (*i).checkDevice();
                resultConsiderDeactivateOnChange |= tempConsider;
            }
            resultIgnoreDeactiveOnChange = resultConsiderDeactivateOnChange;
            return;
        }
    }
}

void ActivationWithFeedbackControl::logFeedbackCheckFailures(E_ActivationState activationState, E_PSSErrors error)
{
    T_DeviceCheckerListIterator i;
    bool temp;
    if (m_controlState == E_ControlState_Ready)
    {
        if (activationState == E_ActivationState_Active)
        {
            for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
            {
                temp = (*i).checkDevice() || !(*i).m_deactivateOnChange;
                if (!(*i).compareToPreviousCheckResult(temp))
                {
                    if (temp)
                    {
                        if (raiseError((*i).getPssId(), error, false))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "ActivationWithFeedbackControl operation {[PSSID:%d]} feedback cleared: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
                    }
                    else
                    {
                        if (raiseError((*i).getPssId(), error, true))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "ActivationWithFeedbackControl operation {[PSSID:%d]} feedback failed: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
//                    (*i).m_previousCheckResult = temp;
                    }
                }
            }
        }
        else
        {
            for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
            {
                temp = !((*i).checkDevice() && !(*i).m_deactivateOnChange);
                if (!(*i).compareToPreviousCheckResult(temp))
                {
                    if (temp)
                    {
                        if (raiseError((*i).getPssId(), error, true))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "ActivationWithFeedbackControl operation {[PSSID:%d]} feedback failed: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
                    }
                    else
                    {
                        if (raiseError((*i).getPssId(), error, false))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "ActivationWithFeedbackControl operation {[PSSID:%d]} feedback cleared: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
//                    (*i).m_previousCheckResult = temp;
                    }
                }
            }
        }
    }
    else
    {
        if (activationState == E_ActivationState_Active)
        {
            for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
            {
                temp = (*i).checkDevice();
                if (!(*i).compareToPreviousCheckResult(temp))
                {
                    if (temp)
                    {
                        if (raiseError((*i).getPssId(), error, false))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "ActivationWithFeedbackControl operation {[PSSID:%d]} feedback cleared: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
                    }
                    else
                    {
                        if (raiseError((*i).getPssId(), error, true))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "ActivationWithFeedbackControl operation {[PSSID:%d]} feedback failed: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
//                    (*i).m_previousCheckResult = temp;
                    }
                }
            }
        }
        else
        {
            for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
            {
                temp = (*i).checkDevice();
                if (!(*i).compareToPreviousCheckResult(temp))
                {
                    if (temp)
                    {
                        if (raiseError((*i).getPssId(), error, true))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "ActivationWithFeedbackControl operation {[PSSID:%d]} feedback failed: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
                    }
                    else
                    {
                        if (raiseError((*i).getPssId(), error, false))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "ActivationWithFeedbackControl operation {[PSSID:%d]} feedback cleared: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
                        //                    (*i).m_previousCheckResult = temp;
                    }
                }
            }
        }
    }
}

/**
 * Check all feedbacks for the state of the control.
 * The "state" parameters controls whether to check if all feedbacks are true, or if all feedbacks are false.
 * If "state" is "Active", the function will return "true" if all feedbacks are true.
 * If "state" is "Inactive", the function will return "false" if all feedbacks are false.
 * @param state
 * @return
 */
//bool ActivationWithFeedbackControl::checkAllDependencies(E_ActivationState activationState, E_ControlState controlState)
//{
//    T_DeviceCheckerListIterator i;
//    bool result, temp;
//    if (controlState == E_ControlState_Ready)
//    {
//        if (activationState == E_ActivationState_Active)
//        {
//            result = true;
//            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//            {
//                temp = (*i).checkDevice(true) || !(*i).m_deactivateOnChange;
//                result &= temp;
//            }
////            raiseError(0, E_PSSErrors_ActivationNotReady, (result == false));
//            return (result == true);
//        }
//        else
//        {
//            result = false;
//            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//            {
//                temp = (!(*i).checkDevice(false) && !(*i).m_deactivateOnChange);
//                result |= temp;
//            }
////            raiseError(0, E_PSSErrors_ActivationNotReady, (result == true));
//            return result;
//        }
//    }
//    else
//    {
//        if (activationState == E_ActivationState_Active)
//        {
//            result = true;
//            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//            {
//                temp = (*i).checkDevice(true);
//                result &= temp;
//            }
////            raiseError(0, E_PSSErrors_ActivationNotReady, (result == false));
//            return (result == true);
//        }
//        else
//        {
//            result = false;
//            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//            {
//                temp = !(*i).checkDevice(false);
//                result |= temp;
//            }
////            raiseError(0, E_PSSErrors_ActivationNotReady, (result == true));
//            return result;
//        }
//    }
//}
//bool ActivationWithFeedbackControl::checkAllDependenciesForOppositeState()
//{
//    bool result;
//    switch (m_activationState)
//    {
//    case E_ActivationState_Active:
//        result = checkAllDependencies(E_ActivationState_Inactive, E_ControlState_Standby);
////        raiseError(0, E_PSSErrors_ActivationNotReady, (result == true));
//        if (result)
//            logDependencyCheckFailures(E_ActivationState_Inactive, E_ControlState_Standby,
//                    E_PSSErrors_ActivationNotReady);
//        else
//            clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
//        return result;
//    case E_ActivationState_Unknown:
//    case E_ActivationState_Inactive:
//        result = checkAllDependencies(E_ActivationState_Active, E_ControlState_Standby);
////        raiseError(0, E_PSSErrors_ActivationNotReady, (result == false));
//        if (result == false)
//            logDependencyCheckFailures(E_ActivationState_Active, E_ControlState_Standby,
//                    E_PSSErrors_ActivationNotReady);
//        else
//            clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
//        return result;
//    }
//    return false;
//}
//void ActivationWithFeedbackControl::logDependencyCheckFailures(E_ActivationState activationState,
//        E_ControlState controlState, E_PSSErrors error)
//{
//    T_DeviceCheckerListIterator i;
//    bool temp;
//    if (controlState == E_ControlState_Ready)
//    {
//        if (activationState == E_ActivationState_Active)
//        {
//            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//            {
//                temp = (*i).checkDevice(true) || !(*i).m_deactivateOnChange;
//                if (!(*i).compareToPreviousCheckResult(temp))
//                {
//                    if (temp)
//                    {
//                        raiseError((*i).getPssId(), error, false);
//                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                                "ActivationWithFeedbackControl operation {[PSSID:%d]} dependency cleared: Device {[PSSID:%d]}",
//                                getPssId(), (*i).getElement()->getPssId());
//                    }
//                    else
//                    {
//                        raiseError((*i).getPssId(), error, true);
//                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                                "ActivationWithFeedbackControl operation {[PSSID:%d]} dependency failed: Device {[PSSID:%d]}",
//                                getPssId(), (*i).getElement()->getPssId());
////                    (*i).m_previousCheckResult = temp;
//                    }
//                }
//            }
//        }
//        else
//        {
//            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//            {
//                temp = (!(*i).checkDevice(false) && !(*i).m_deactivateOnChange);
//                if (!(*i).compareToPreviousCheckResult(temp))
//                {
//                    if (temp)
//                    {
//                        raiseError((*i).getPssId(), error, true);
//                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                                "ActivationWithFeedbackControl operation {[PSSID:%d]} dependency failed: Device {[PSSID:%d]}",
//                                getPssId(), (*i).getElement()->getPssId());
//                    }
//                    else
//                    {
//                        raiseError((*i).getPssId(), error, false);
//                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                                "ActivationWithFeedbackControl operation {[PSSID:%d]} dependency cleared: Device {[PSSID:%d]}",
//                                getPssId(), (*i).getElement()->getPssId());
////                    (*i).m_previousCheckResult = temp;
//                    }
//                }
//            }
//        }
//    }
//    else
//    {
//        if (activationState == E_ActivationState_Active)
//        {
//            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//            {
//                temp = (*i).checkDevice(true);
//                if (!(*i).compareToPreviousCheckResult(temp))
//                {
//                    if (temp)
//                    {
//                        raiseError((*i).getPssId(), error, false);
//                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                                "ActivationWithFeedbackControl operation {[PSSID:%d]} dependency cleared: Device {[PSSID:%d]}",
//                                getPssId(), (*i).getElement()->getPssId());
//                    }
//                    else
//                    {
//                        raiseError((*i).getPssId(), error, true);
//                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                                "ActivationWithFeedbackControl operation {[PSSID:%d]} dependency failed: Device {[PSSID:%d]}",
//                                getPssId(), (*i).getElement()->getPssId());
////                    (*i).m_previousCheckResult = temp;
//                    }
//                }
//            }
//        }
//        else
//        {
//            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//            {
//                temp = !(*i).checkDevice(false);
//                if (!(*i).compareToPreviousCheckResult(temp))
//                {
//                    if (temp)
//                    {
//                        raiseError((*i).getPssId(), error, true);
//                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                                "ActivationWithFeedbackControl operation {[PSSID:%d]} dependency failed: Device {[PSSID:%d]}",
//                                getPssId(), (*i).getElement()->getPssId());
//                    }
//                    else
//                    {
//                        raiseError((*i).getPssId(), error, false);
//                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                                "ActivationWithFeedbackControl operation {[PSSID:%d]} dependency cleared: Device {[PSSID:%d]}",
//                                getPssId(), (*i).getElement()->getPssId());
////                    (*i).m_previousCheckResult = temp;
//                    }
//                }
//            }
//        }
//    }
//}
//bool ActivationWithFeedbackControl::checkAllDependencies()
//{
//    int i;
//    bool result = true;
//    for (i = 0; i < m_dependentCheckers.size(); ++i)
//        result &= m_dependentCheckers[i].checkDevice();
//
//    return result;
//}
//
void ActivationWithFeedbackControl::setOutputEnableDevice(ElementBase* element)
{
    m_outputEnableDevice = element;
    m_outputEnableDevice->addObserver(this);
//    if (getSecondaryPssId() == 0)
//        setSecondaryPssId(element->getPssId());
}

void ActivationWithFeedbackControl::setOutputDisableDevice(ElementBase* element)
{
    m_outputDisableDevice = element;
    m_outputDisableDevice->addObserver(this);
//    if (getSecondaryPssId() == 0)
//        setSecondaryPssId(element->getPssId());
}

void ActivationWithFeedbackControl::setActivationState(E_ActivationState activationState, uint32_t sn)
{
    if (m_previousActivationState != m_activationState)
        m_previousActivationState = m_activationState;
    m_activationState = activationState;

    m_lastSn = sn;

//    clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);
////    clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
    clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);

    if (m_activationState != E_ActivationState_Active)
        clearProtectionCheckErrors();

    bool depsConsider, depsIgnore;
    checkAllDependencies(m_activationState, m_controlState, depsConsider, depsIgnore);

    if (m_isProtectionActive)
        activationState = E_ActivationState_Unknown;

    switch (activationState)
    {
    case E_ActivationState_Unknown:
        if (m_outputEnableDevice != NULL)
            *m_outputEnableDevice = 0;
        if (m_outputDisableDevice != NULL)
            *m_outputDisableDevice = 0;
        raiseError(0, E_PSSErrors_ActivationFailed, false);
        for (T_DeviceCheckerListIterator i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
        {
            raiseError((*i).getPssId(), E_PSSErrors_ActivationFailed, false);
            (*i).resetPreviousCheckResult();
        }
        break;
    case E_ActivationState_Active:
        if (depsConsider == false)
            logDependencyCheckFailures(m_activationState, m_controlState, E_PSSErrors_ActivationFailed);
        else
        {
            clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);
//            clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);
        }
        if ((m_previousActivationState != activationState) && (depsConsider == false))
            break;
        if (m_outputEnableDevice != NULL)
            *m_outputEnableDevice = m_activationOutputValue;
        if (m_outputDisableDevice != NULL)
            *m_outputDisableDevice = 0;
        startIgnoringProtections();
        break;
    case E_ActivationState_Inactive:
        if (depsConsider == true)
            logDependencyCheckFailures(m_activationState, m_controlState, E_PSSErrors_ActivationFailed);
        else
        {
            clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);
//            clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);
        }
        if ((m_previousActivationState != activationState) && (depsConsider == true))
            break;
        if (m_outputEnableDevice != NULL)
            *m_outputEnableDevice = 0;
        if (m_outputDisableDevice != NULL)
            *m_outputDisableDevice = 1;
        break;
    }
}

bool ActivationWithFeedbackControl::onMove2Error()
{
//    bool result = activateControl(false);
    setActivationState(E_ActivationState_Unknown, m_lastSn);
    ControlBase::endMove2Error();
    return true;
}

bool ActivationWithFeedbackControl::onRecoverFromEmr()
{
    ControlBase::endRecoverFromEmr();
    return true;
}

bool ActivationWithFeedbackControl::executeLocalProtectionCheck(ElementBase* element)
{
    if (m_ignoreProtections)
    {
        return false;
    }

    // if the control should be inactive don't check for protections:
    if (m_activationState != E_ActivationState_Active && m_inProtectionActivationState != E_ActivationState_Active)
        return false;

    ControlBase::executeProtectionCheck(element/*, E_PSSErrors_DeviceExceedsSoftLimits*/);

    // if the previous protection state is different than the new
    if (m_lastProtectionState != m_isProtectionActive)
    {
        // if the previous protection state was inactive, store the activation state:
        if (m_lastProtectionState == false)
            m_inProtectionActivationState = m_activationState;

        if (m_isProtectionActive)
            setActivationState(E_ActivationState_Inactive, 0);
        else
            setActivationState(m_inProtectionActivationState, 0);
    }
    m_lastProtectionState = m_isProtectionActive;
    return m_isProtectionActive;
}

bool ActivationWithFeedbackControl::executeLocalProtectionCheck()
{
    if (m_ignoreProtections)
    {
        return false;
    }

    // if the control should be inactive don't check for protections:
    if (m_activationState != E_ActivationState_Active && m_inProtectionActivationState != E_ActivationState_Active)
        return false;

    ControlBase::executeProtectionCheck(/*, E_PSSErrors_DeviceExceedsSoftLimits*/);

    // if the previous protection state is different than the new
    if (m_lastProtectionState != m_isProtectionActive)
    {
        // if the previous protection state was inactive, store the activation state:
        if (m_lastProtectionState == false)
            m_inProtectionActivationState = m_activationState;

        if (m_isProtectionActive)
            setActivationState(E_ActivationState_Inactive, 0);
        else
            setActivationState(m_inProtectionActivationState, 0);
    }
    m_lastProtectionState = m_isProtectionActive;
    return m_isProtectionActive;
}

void ActivationWithFeedbackControl::startIgnoringProtections()
{
    if (m_ignoreProtections > 0)
    {
        UpdateSchedulerTask::getInstance()->addTimeout(this, m_ignoreProtectionsDelay, M_IGNORE_PROTECTION_DELAY_TYPE);
        m_ignoreProtections = true;
    }
}

void ActivationWithFeedbackControl::printDependencyErrors()
{
//    T_DeviceCheckerListIterator i;
//    for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//    {
//        printf("Primary %d secondary %d dependency errors %d\n", getPssId(), (*i).getSecondaryPssId(), (*i).getErrors());
//    }
}

void ActivationWithFeedbackControl::printFeedbackErrors()
{
//    T_DeviceCheckerListIterator i;
//    for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
//    {
//        printf("Primary %d secondary %d feedback errors %d\n", getPssId(), (*i).getSecondaryPssId(), (*i).getErrors());
//    }
}

void ActivationWithFeedbackControl::sendCurrentErrors()
{
    m_errorBitManager.sendCurrentErrors();
    T_DeviceCheckerListIterator i;
//    for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
//    {
//        (*i).sendCurrentErrors();
//    }
//    for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//    {
//        (*i).sendCurrentErrors();
//    }
}

bool ActivationWithFeedbackControl::checkValidOutputs()
{
    bool actValid = true;
    bool deactValid = true;
    if (m_outputEnableDevice != NULL)
        actValid = m_outputEnableDevice->isValid();
    if (m_outputDisableDevice != NULL)
        deactValid = m_outputDisableDevice->isValid();
    return (actValid && deactValid);
}

void DeviceThresholdChecker::resetPreviousCheckResult()
{
    m_previousCheckResult = E_DependencyCheckResult_Unchecked;
}

void ActivationWithFeedbackControl::clearFeedbackCheckFailures(E_PSSErrors error)
{
    T_DeviceCheckerListIterator i;
    for (i = m_feedbackCheckers.begin(); i != m_feedbackCheckers.end(); ++i)
    {
        raiseError((*i).getPssId(), error, false);
    }
}

//void ActivationWithFeedbackControl::clearDependencyCheckFailures(E_PSSErrors error)
//{
//    T_DeviceCheckerListIterator i;
//    for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
//    {
//        raiseError((*i).getPssId(), error, false);
//    }
//}

void ActivationWithFeedbackControl::startRecovery()
{
    clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);
    clearFeedbackCheckFailures(E_PSSErrors_ActivationNotReady);
    clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);
    clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
    //checkAllDependenciesForOppositeState();
}
