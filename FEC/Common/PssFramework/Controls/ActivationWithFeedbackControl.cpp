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
#include <Persistency/ControlSerializers.h>

#define M_ACTIVATION_TIMEOUT_TYPE 0
#define M_IGNORE_PROTECTION_DELAY_TYPE 1

/**
 @startuml

 [*] --> On
 On --> Init : Init
 Init --> Unknown : Behavior On Init
 Init --> Active : Behavior On Init
 Init --> Inactive : Behavior On Init

 state Unknown {
 [*] --> UnknownActivationNotReady : Dep Not Filled
 [*] --> UnknownActivationReady : Dep Filled
 UnknownActivationNotReady -right-> UnknownActivationReady : Dep Filled
 UnknownActivationReady -left-> UnknownActivationNotReady : Dep Not Filled
 }

 UnknownActivationReady -right-> UnknownMoveToActivate : Activate
 UnknownMoveToActivate -right-> Active : FB and Dep Filled
 UnknownMoveToActivate -left-> UnknownActivationReady : Timeout on FB
 UnknownMoveToActivate -left-> UnknownActivationNotReady : Dep Not Filled

 state Active {
 [*] --> ActiveDeactivationNotReady : Dep Not Filled
 [*] --> ActiveDeactivationReady : Dep Filled
 ActiveDeactivationNotReady -right-> ActiveDeactivationReady : Dep Filled
 ActiveDeactivationReady -left-> ActiveDeactivationNotReady : Dep Not Filled
 }

 state Inactive {
 [*] --> InactiveActivationNotReady : Dep Not Filled
 [*] --> InactiveActivationReady : Dep Filled
 InactiveActivationNotReady -right-> InactiveActivationReady : Dep Filled
 InactiveActivationReady -left-> InactiveActivationNotReady : Dep Not Filled
 }

 InactiveActivationReady -left-> InactiveMoveToActivate
 InactiveMoveToActivate -left-> Active : FB and Dep Filled
 InactiveMoveToActivate -right-> InactiveActivationReady
 InactiveMoveToActivate -right-> InactiveActivationNotReady : Dep Not Filled

 @enduml
 */
ActivationWithFeedbackControl::ActivationWithFeedbackControl()
{
    m_outputEnableDevice = NULL;
    m_outputDisableDevice = NULL;
    m_feedbackEnabledDevice = NULL;
    m_feedbackDisabledDevice = NULL;
    m_pActivateControlDeviceList = NULL;
    m_pDeactivateControlDeviceList = NULL;

    m_activationState = E_ActivationState_Unknown;
    m_previousActivationState = E_ActivationState_Unknown;
    m_behaviorOnInit = E_ActivationBehaviorOnInit_Unknown;
    m_lastProtectionActiveState = false;
    m_lastFeedbackResult = false;
    m_lastDependencyResult = false;
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
    T_IODeviceCheckerListIterator iodIt;

    if (m_pActivateControlDeviceList != NULL)
    {
        for (iodIt = m_pActivateControlDeviceList->begin(); iodIt != m_pActivateControlDeviceList->end(); ++iodIt)
        {
            (iodIt)->getElement()->removeObserver(this);
        }
        delete m_pActivateControlDeviceList;
        m_pActivateControlDeviceList = NULL;
    }
    if (m_pDeactivateControlDeviceList != NULL)
    {
        for (iodIt = m_pDeactivateControlDeviceList->begin(); iodIt != m_pDeactivateControlDeviceList->end(); ++iodIt)
        {
            (iodIt)->getElement()->removeObserver(this);
        }
        delete m_pDeactivateControlDeviceList;
        m_pDeactivateControlDeviceList = NULL;
    }

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

void ActivationWithFeedbackControl::execute()
{
    bool /*feedbacksConsider, */feedbacksIgnore;
    bool /*depsConsider, */depsIgnore;
    bool outputsValid = checkValidOutputs();
    switch (m_controlState)
    {
    case E_ControlState_Move2Ready:
        checkAllFeedbacks(m_activationState, m_lastFeedbackResult, feedbacksIgnore);
        checkAllDependencies(m_activationState, m_controlState, m_lastDependencyResult, depsIgnore);
        if (m_activationState == E_ActivationState_Active)
        {
            if (m_lastFeedbackResult)
                clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);
            // check all feedback devices. If they're all activated, move to ready:
            if (m_lastFeedbackResult && m_lastDependencyResult && outputsValid)
            {
                m_controlState = E_ControlState_Ready;

                // store the successfull activation state.
//                m_previousActivationState = m_activationState;
            }

            // if the dependency check failed:
            if (!m_lastDependencyResult)
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
                if (!m_lastDependencyResult)
                    setActivationState(m_previousActivationState, 0);
            }
            else
                clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
        }
        else
        {
            if (!m_lastFeedbackResult)
                clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);
            if (!m_lastDependencyResult)
                clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
            if (!(m_lastFeedbackResult && m_lastDependencyResult))
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
        checkAllFeedbacks(m_activationState, m_lastFeedbackResult, feedbacksIgnore);
        checkAllDependencies(m_activationState, m_controlState, m_lastDependencyResult, depsIgnore);
        if (m_activationState == E_ActivationState_Active)
        {
            if (!(m_lastFeedbackResult && m_lastDependencyResult && outputsValid))
            {
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
        break;
    }
}

bool ActivationWithFeedbackControl::activateControl(int outputValue, uint32_t timeout, uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return false;

    bool /*depsConsider, */depsIgnore;

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

    if (outputValue != 0)
    {
        // check if all dependent devices are in the correct state.
        checkAllDependencies(E_ActivationState_Active, m_controlState, m_lastDependencyResult, depsIgnore);
        // if we are already at the requested activation state, ignore the dependencies and start the activation.
        if ((m_previousActivationState == E_ActivationState_Active) || m_lastDependencyResult)
        {
            raiseError(0, E_PSSErrors_ActivationFailed, false);
            clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);

            setActivationState(E_ActivationState_Active, sn);
            // make sure that if there's an existing timeout its removed.
            UpdateSchedulerTask::getInstance()->addTimeout(this, 0, M_ACTIVATION_TIMEOUT_TYPE);
            if (timeout > 0 && timeout != ((uint32_t) - 1))
                UpdateSchedulerTask::getInstance()->addTimeout(this, timeout, M_ACTIVATION_TIMEOUT_TYPE);
            execute();
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
        return m_lastDependencyResult;
    }
    else
    {
        //m_activationOutputValue = outputValue;
        // check if all dependent devices are in the correct state.
        checkAllDependencies(E_ActivationState_Inactive, m_controlState, m_lastDependencyResult, depsIgnore);
        if ((m_previousActivationState == E_ActivationState_Inactive) || !m_lastDependencyResult)
        {
            raiseError(0, E_PSSErrors_ActivationFailed, false);
            clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);

            setActivationState(E_ActivationState_Inactive, sn);
            // make sure that if there's an existing timeout its removed.
            UpdateSchedulerTask::getInstance()->addTimeout(this, 0, M_ACTIVATION_TIMEOUT_TYPE);
            if (timeout > 0 && timeout != ((uint32_t) - 1))
                UpdateSchedulerTask::getInstance()->addTimeout(this, timeout, M_ACTIVATION_TIMEOUT_TYPE);
            execute();
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
        return !m_lastDependencyResult;
    }

}

void ActivationWithFeedbackControl::executeBehaviorOnInit()
{
    bool resultIgnore;
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
        checkAllFeedbacks(E_ActivationState_Active, m_lastFeedbackResult, resultIgnore);
        if (m_lastFeedbackResult)
        {
            setActivationState(E_ActivationState_Active, m_lastSn);
            m_previousActivationState = E_ActivationState_Active;
        }
        else
        {
            checkAllFeedbacks(E_ActivationState_Inactive, m_lastFeedbackResult, resultIgnore);
            if (m_lastFeedbackResult == false)
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
    m_activateSignalHigh = -1;
    m_deactivateSignalHigh = -1;
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

bool ActivationWithFeedbackControl::onMove2Standby(uint32_t delay)
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

    checkCommandDevices();

    execute();

    checkAllDependenciesForOppositeState();

    if (m_controlState == E_ControlState_Move2Ready || m_controlState == E_ControlState_Ready)
        executeLocalProtectionCheck(element);

    calculateFeedbackOutputs();
}

bool ActivationWithFeedbackControl::requestValueChange(ElementBase* element)
{
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

void ActivationWithFeedbackControl::setFeedbackEnabledDevice(ElementBase* element)
{
    m_feedbackEnabledDevice = element;
}

void ActivationWithFeedbackControl::setFeedbackDisabledDevice(ElementBase* element)
{
    m_feedbackDisabledDevice = element;
}

void ActivationWithFeedbackControl::setOutputEnableDevice(ElementBase* element)
{
    m_outputEnableDevice = element;
//    m_outputEnableDevice->addObserver(this);
}

void ActivationWithFeedbackControl::setOutputDisableDevice(ElementBase* element)
{
    m_outputDisableDevice = element;
//    m_outputDisableDevice->addObserver(this);
}

void ActivationWithFeedbackControl::addActivateDevice(ElementBase* element, bool activeHigh)
{
    IODeviceChecker iod;
    iod.setElement(element);
    element->addObserver(this);
    iod.setActivateHigh(activeHigh);
    if (m_pActivateControlDeviceList == NULL)
        m_pActivateControlDeviceList = new T_IODeviceCheckerList();
    m_pActivateControlDeviceList->push_back(iod);
}

void ActivationWithFeedbackControl::addDeactivateDevice(ElementBase* element, bool activeHigh)
{
    IODeviceChecker iod;
    iod.setElement(element);
    element->addObserver(this);
    iod.setActivateHigh(activeHigh);
    if (m_pDeactivateControlDeviceList == NULL)
        m_pDeactivateControlDeviceList = new T_IODeviceCheckerList();
    m_pDeactivateControlDeviceList->push_back(iod);
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

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "setActivationState: %d", activationState);

    switch (activationState)
    {
    case E_ActivationState_Unknown:
        writeOutputs(E_ActivationState_Unknown);
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
        writeOutputs(E_ActivationState_Active);
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
        writeOutputs(E_ActivationState_Inactive);
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
    if (m_lastProtectionActiveState != m_isProtectionActive)
    {
        // if the previous protection state was inactive, store the activation state:
        if (m_lastProtectionActiveState == false)
            m_inProtectionActivationState = m_activationState;

        if (m_isProtectionActive)
            //setActivationState(E_ActivationState_Inactive, 0);
            writeOutputs(E_ActivationState_Unknown);
        else
            //setActivationState(m_inProtectionActivationState, 0);
            writeOutputs(m_activationState);
    }
    m_lastProtectionActiveState = m_isProtectionActive;
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
    if (m_lastProtectionActiveState != m_isProtectionActive)
    {
        // if the previous protection state was inactive, store the activation state:
        if (m_lastProtectionActiveState == false)
            m_inProtectionActivationState = m_activationState;

        if (m_isProtectionActive)
            setActivationState(E_ActivationState_Inactive, 0);
        else
            setActivationState(m_inProtectionActivationState, 0);
    }
    m_lastProtectionActiveState = m_isProtectionActive;
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
}

void ActivationWithFeedbackControl::checkCommandDevices()
{
    int activateSignalHigh = -1;
    int deactivateSignalHigh = -1;

    if (m_pActivateControlDeviceList == NULL && m_pDeactivateControlDeviceList == NULL)
        return;

    // set the signals high as default.
    T_IODeviceCheckerListIterator iodIt;
    // first calculate the state of the activate signals:
    if (m_pActivateControlDeviceList != NULL)
    {
        activateSignalHigh = 1;
        for (iodIt = m_pActivateControlDeviceList->begin(); iodIt != m_pActivateControlDeviceList->end(); ++iodIt)
        {
            activateSignalHigh &= (iodIt)->isActive();
        }
    }
    if (m_pDeactivateControlDeviceList != NULL)
    {
        deactivateSignalHigh = 1;
        for (iodIt = m_pDeactivateControlDeviceList->begin(); iodIt != m_pDeactivateControlDeviceList->end(); ++iodIt)
        {
            deactivateSignalHigh &= (iodIt)->isActive();
        }
    }

    // if both signals are -1 it means something went wrong, so set to unkown.
//    if ((activateSignalHigh == -1 && deactivateSignalHigh == -1) ||
//            (activateSignalHigh > 0 && deactivateSignalHigh > 0))
    if (m_activateSignalHigh == activateSignalHigh && m_deactivateSignalHigh == deactivateSignalHigh)
        return;

    m_activateSignalHigh = activateSignalHigh;
    m_deactivateSignalHigh = deactivateSignalHigh;
//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "activate %d deactivate %d activationState %d", activateSignalHigh, deactivateSignalHigh, m_activationState);
//    if (activateSignalHigh == deactivateSignalHigh && m_activationState != E_ActivationState_Unknown)
//    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Control {[PSSID:%d]} IO activation state: Unknown", getPssId());
//        setActivationState(E_ActivationState_Unknown, 0);
//    }
//    else
        if ((activateSignalHigh > 0 || (deactivateSignalHigh == 0 && activateSignalHigh == -1))
            && m_activationState != E_ActivationState_Active)
    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Control {[PSSID:%d]} IO activation state: Active", getPssId());
        m_controlState = E_ControlState_Move2Ready;
        setActivationState(E_ActivationState_Active, 0);
    }
    else if ((deactivateSignalHigh > 0 || (activateSignalHigh == 0 && deactivateSignalHigh == -1))
            && m_activationState != E_ActivationState_Inactive)
    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Control {[PSSID:%d]} IO activation state: Inactive", getPssId());
        m_controlState = E_ControlState_Move2Ready;
        setActivationState(E_ActivationState_Inactive, 0);
    }
}

void ActivationWithFeedbackControl::calculateFeedbackOutputs()
{
    if (m_feedbackEnabledDevice == NULL && m_feedbackDisabledDevice == NULL)
        return;

    m_feedbackEnabledDevice->setValue(
            (m_outputEnableDevice->getValueI32() != 0 && m_lastDependencyResult
                    && m_lastFeedbackResult && !m_lastProtectionActiveState));
//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "FB: {[PSSID:%d]} %d %d %d %d -> %d", getPssId(), m_outputEnableDevice->getValueI32(), m_lastDependencyResult, m_lastFeedbackResult, m_lastProtectionActiveState, m_feedbackEnabledDevice->getValueI32());
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

void ActivationWithFeedbackControl::startRecovery()
{
    clearFeedbackCheckFailures(E_PSSErrors_ActivationFailed);
    clearFeedbackCheckFailures(E_PSSErrors_ActivationNotReady);
    clearDependencyCheckFailures(E_PSSErrors_ActivationFailed);
    clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
    //checkAllDependenciesForOppositeState();
}

void ActivationWithFeedbackControl::writeOutputs(E_ActivationState activationState)
{
//    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "writeOutputs: %d", activationState);
    switch (activationState)
    {
    case E_ActivationBehaviorOnInit_Unknown:
        if (m_outputEnableDevice != NULL)
            m_outputEnableDevice->setValue((uint32_t) 0);
        if (m_outputDisableDevice != NULL)
            m_outputDisableDevice->setValue((uint32_t) 0);
        break;
    case E_ActivationState_Active:
        if (m_activationOutputValue == 0)
            m_activationOutputValue = 1;
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "writeOutputs m_activationOutputValue: %d", m_activationOutputValue);
        if (m_outputEnableDevice != NULL)
            m_outputEnableDevice->setValue(m_activationOutputValue);
        if (m_outputDisableDevice != NULL)
            m_outputDisableDevice->setValue((uint32_t) 0);
        break;
    case E_ActivationState_Inactive:
        if (m_outputEnableDevice != NULL)
            m_outputEnableDevice->setValue((uint32_t) 0);
        if (m_outputDisableDevice != NULL)
            m_outputDisableDevice->setValue((uint32_t) 1);
        break;
    }
}

void ActivationWithFeedbackControl::serialize(F_FILE* f)
{
    Serializer<ActivationWithFeedbackControl> s;
    s.serialize(f, *this);
}

ActivationWithFeedbackControl::ActivationWithFeedbackControl(F_FILE* f)
{
    Serializer<ActivationWithFeedbackControl> s;
    s.deserialize(f, *this);
}

