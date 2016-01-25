/*
 * ControlBase.cpp
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#include "ControlBase.h"
#include "ControlRepository.h"
#include <PscServer/PscMasterServer.h>
#include <PscSubsystem.h>
#include <Tasks/UpdateSchedulerTask.h>
#include "logger.h"
#include "DeviceThresholdChecker.h"

ControlBase::ControlBase()
{
    m_controlIndex = 0;
    m_pssId = M_UNASSIGNED_PSS_ID;
    m_controlState = E_ControlState_On;
    m_controlExceptions.full = 0;
    m_lastSn = 0;
    m_monitoringEnabled = false;
    ControlRepository::getInstance().addControl(this);
    m_isProtectionActive = false;
    m_isProtectionWarningActive = false;
    m_isEnabled = false;
    m_stopOnEmrBehavior = true;
    m_stopOnDisconnection = true;
}

ControlBase::~ControlBase()
{
//    delete m_pDependentCheckers;
    T_DeviceCheckerListIterator dclIt;
    for (dclIt = m_dependentCheckers.begin(); dclIt != m_dependentCheckers.end(); ++dclIt)
    {
        (*dclIt).getElement()->removeObserver(this);
    }
}

bool ControlBase::initControl(uint32_t msgId, uint32_t sn)
{
    if (m_controlState == E_ControlState_Emergency)
    {
        PscMasterServer::getInstance().sendAck(msgId, sn, Psc_ControllerId, getPssId(), E_AckStatus_InvalidState);
        return false;
    }
    if (m_controlState == E_ControlState_On)
    {
        m_lastSn = sn;
        m_lastStateChangeMessageId = msgId;
        PscMasterServer::getInstance().sendAck(msgId, m_lastSn, Psc_ControllerId, getPssId(), E_AckStatus_Success);
        return onInitControl();
    }
    PscMasterServer::getInstance().sendAck(msgId, sn, Psc_ControllerId, getPssId(), E_AckStatus_InvalidState);
    return false;
}

bool ControlBase::move2Standby(uint32_t msgId, uint32_t delay, uint32_t sn)
{
    if (m_controlState == E_ControlState_Emergency || m_controlState == E_ControlState_On)
    {
        PscMasterServer::getInstance().sendAck(msgId, sn, Psc_ControllerId, getPssId(), E_AckStatus_InvalidState);
        return false;
    }
    if (m_controlState >= E_ControlState_Standby && m_controlState <= E_ControlState_Ready)
    {
        m_lastSn = sn;
        m_lastStateChangeMessageId = msgId;
        PscMasterServer::getInstance().sendAck(msgId, m_lastSn, Psc_ControllerId, getPssId(), E_AckStatus_Success);
        return onMove2Standby(delay);
    }
    PscMasterServer::getInstance().sendAck(msgId, sn, Psc_ControllerId, getPssId(), E_AckStatus_InvalidState);
    return false;
}

bool ControlBase::reset2On(uint32_t msgId, uint32_t sn)
{
    if (m_controlState == E_ControlState_Emergency)
    {
        PscMasterServer::getInstance().sendAck(msgId, sn, Psc_ControllerId, getPssId(), E_AckStatus_InvalidState);
        return false;
    }
    if (m_controlState != E_ControlState_Emergency)
    {
        m_lastSn = sn;
        m_lastStateChangeMessageId = msgId;
        PscMasterServer::getInstance().sendAck(msgId, m_lastSn, Psc_ControllerId, getPssId(), E_AckStatus_Success);
        return onReset2On();
    }
    PscMasterServer::getInstance().sendAck(msgId, sn, Psc_ControllerId, getPssId(), E_AckStatus_Success);
    return true;
}

void ControlBase::endInitControl()
{
    m_controlState = E_ControlState_Standby;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Control {[PSSID:%d]} changed state to Standby", m_pssId);
    sendNotification();
    PscMasterServer::getInstance().sendSeqEnded(m_lastStateChangeMessageId, m_lastSn, Psc_ControllerId, getPssId(),
            E_SeqEndedStatus_Success);
    m_lastSn = 0;
}

void ControlBase::endMove2Standby()
{
    m_controlState = E_ControlState_Standby;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Control {[PSSID:%d]} changed state to Standby", m_pssId);
    sendNotification();
    PscMasterServer::getInstance().sendSeqEnded(m_lastStateChangeMessageId, m_lastSn, Psc_ControllerId, getPssId(),
            E_SeqEndedStatus_Success);
    m_lastSn = 0;
}

void ControlBase::endReset2On()
{
    m_controlState = E_ControlState_On;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Control {[PSSID:%d]} changed state to On", m_pssId);
    sendNotification();
    PscMasterServer::getInstance().sendSeqEnded(m_lastStateChangeMessageId, m_lastSn, Psc_ControllerId, getPssId(),
            E_SeqEndedStatus_Success);
    m_lastSn = 0;
}

void ControlBase::setMonitoringEnabled(bool monitoringEnabled)
{
    m_monitoringEnabled = monitoringEnabled;
    if (m_monitoringEnabled/* && UpdateSchedulerTask::getInstance().isBoardInReady()*/)
        sendNotification();
}

//void ControlBase::raiseError(E_PSSErrors error, bool errorState)
//{
//    uint32_t lastError = m_errorBits;
//
//    if (errorState)
//    {
//        m_errorBits |= error;
//        if (m_errorBits != lastError)
//            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), m_errorBits);
//        setControlExceptions(E_ExceptionState_Set, E_ExceptionState_Unchanged);
//    }
//    else
//    {
//        m_errorBits &= ~error;
//        if (m_errorBits != lastError)
//            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), m_errorBits);
//        if (m_errorBits == 0)
//            setControlExceptions(E_ExceptionState_Reset, E_ExceptionState_Unchanged);
//    }
//}
//
//bool ControlBase::isErrorRaised(E_PSSErrors error)
//{
//    return ((m_errorBits & error) != 0);
//}
//
//void ControlBase::raiseWarning(E_PSSWarnings warning, bool warningState)
//{
//    uint32_t lastWarning = m_warningBits;
//
//    if (warningState)
//    {
//        m_warningBits |= warning;
//        if (m_warningBits != lastWarning)
//            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), m_warningBits);
//        setControlExceptions(E_ExceptionState_Unchanged, E_ExceptionState_Set);
//    }
//    else
//    {
//        m_warningBits &= ~warning;
//        if (m_warningBits != lastWarning)
//            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), m_warningBits);
//        if (m_warningBits == 0)
//            setControlExceptions(E_ExceptionState_Unchanged, E_ExceptionState_Reset);
//    }
//}

//void ControlBase::sendCurrentErrors()
//{
//    // TODO: send all current errors from the protection checkers
//    sendErrors()
//}
//
//void ControlBase::sendCurrentWarnings()
//{
//    // TODO: send all current errors from the protection checkers
//    PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), M_PSS_ID_ALL, m_warningBits);
//}

void ControlBase::setControlExceptions(E_ExceptionState errors, E_ExceptionState warnings)
{
    switch (errors)
    {
    case E_ExceptionState_Set:
        m_controlExceptions.split.hasErrors = 1;
        break;
    case E_ExceptionState_Reset:
        m_controlExceptions.split.hasErrors = 0;
        break;
    }
    switch (warnings)
    {
    case E_ExceptionState_Set:
        m_controlExceptions.split.hasWarnings = 1;
        break;
    case E_ExceptionState_Reset:
        m_controlExceptions.split.hasWarnings = 0;
        break;
    }
    if (m_controlExceptions.full != m_previousControlExceptions.full)
        sendNotification();

    m_previousControlExceptions.full = m_controlExceptions.full;
}

bool ControlBase::move2Error(uint32_t msgId, uint32_t sn)
{
    if (m_controlState == E_ControlState_Emergency)
    {
        PscMasterServer::getInstance().sendAck(msgId, m_lastSn, Psc_ControllerId, getPssId(), E_AckStatus_InvalidState);
        return false;
    }
    if (m_controlState != E_ControlState_Error)
    {
        if (m_lastSn != 0 && sn != 0)
        {
            PscMasterServer::getInstance().sendSeqEnded(msgId, m_lastSn, Psc_ControllerId, getPssId(), E_SeqEndedStatus_Error);
            m_lastSn = 0;
        }
//        m_lastSn = sn;
//        m_lastStateChangeMessageId = msgId;
//        PscMasterServer::getInstance().sendAck(msgId, m_lastSn, Psc_ControllerId, getPssId(), E_AckStatus_Success);
        return onMove2Error();
    }
//    PscMasterServer::getInstance().sendAck(msgId, m_lastSn, Psc_ControllerId, getPssId(), E_AckStatus_InvalidState);
    return false;
}

bool ControlBase::stopOnEmr()
{
    if (m_stopOnEmrBehavior)
        return onStopOnEmr();
    else
        return true;
}

bool ControlBase::recoverFromEmr()
{
    if (m_controlState == E_ControlState_Emergency)
        return onRecoverFromEmr();
    return true;
}

void ControlBase::clearProtectionCheckErrors()
{
    T_ProtectionCheckerListIterator i;

    for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
    {
        raiseError((*i)->getPssId(), (*i)->getErrorType(), false);
    }

    m_isProtectionActive = false;
    m_isProtectionWarningActive = false;

}

//! This function returns "true" if protection is active
bool ControlBase::executeProtectionCheck(ElementBase* element/*, E_PSSErrors error*/)
{
    if (m_protectionCheckers.size() == 0)
        return false;

// if the protection is not active, we can simply check the updated element if it should
// turn on the protection:
    if (!m_isProtectionActive && !m_isProtectionWarningActive)
    {
        // if the temp is above protection limit, turn on the protection.
        if (element->getValueI32() >= E_DeviceProtectionState_SoftLimitExceededWarning)
        {
            return executeProtectionCheck(/*error*/);
        }
    }
    else
        return executeProtectionCheck(/*error*/);

    return false;
}

bool ControlBase::executeProtectionCheck(/*E_PSSErrors error*/)
{
    if (m_protectionCheckers.size() == 0)
        return false;

    T_ProtectionCheckerListIterator i;
    // Protection elements should stop heating if temp is above max working, and continue when below min working.

    // if the protection is not active, we can simply check the updated element if it should
    // turn on the protection:
    if (!m_isProtectionActive && !m_isProtectionWarningActive)
    {
        // if the temp is above protection limit, turn on the protection.
        for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
        {
//            if ((*i)->getProtectionState() == E_DeviceProtectionState_InRange)
//            {
//                m_errorBitManager.raiseError((*i)->getPssId(), (*i)->getErrorType(), false);
//                m_errorBitManager.raiseWarning((*i)->getPssId(), (*i)->getWarningType(), false);
//            }
            if ((*i)->getProtectionState() == E_DeviceProtectionState_SoftLimitExceededWarning)
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_WARNING, "Device {[PSSID:%d]} exceeds warning limits",
                        (*i)->getProtectionElement()->getPssId());
                m_isProtectionWarningActive = true;
                m_errorBitManager.raiseWarning((*i)->getPssId(), (*i)->getWarningType(), true);
                m_errorBitManager.raiseError((*i)->getPssId(), (*i)->getErrorType(), false);
            }
            if ((*i)->getProtectionState() >= E_DeviceProtectionState_SoftLimitExceeded)
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Device {[PSSID:%d]} exceeds limits",
                        (*i)->getProtectionElement()->getPssId());
                m_isProtectionActive = true;
                m_errorBitManager.raiseError((*i)->getPssId(), (*i)->getErrorType(), true);
                m_errorBitManager.raiseWarning((*i)->getPssId(), (*i)->getWarningType(), false);
//                return true;
            }
        }
    }
    // the protection is active, so we should check all protection elements to see if they are below limit:
    else
    {
        bool safe = true;
        bool currentInRange;
        for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
        {
            currentInRange = ((*i)->getProtectionState() == E_DeviceProtectionState_InRange);
            if (currentInRange)
            {
                m_errorBitManager.raiseError((*i)->getPssId(), (*i)->getErrorType(), false);
                m_errorBitManager.raiseWarning((*i)->getPssId(), (*i)->getWarningType(), false);
            }
//                (*i)->raiseError(/*m_secondaryPssId, */error, false, 0);
//            if (currentInRange == false)
//                M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Device {[PSSID:%d]} exceeds limits");
            safe = (safe && currentInRange);
        }
        if (safe)
        {
            m_isProtectionActive = false;
            m_isProtectionWarningActive = false;
        }
    }
    return m_isProtectionActive;
}

#ifdef OLD_PROTECTION
void ControlBase::addProtectionElement(ElementBase* element)
{
    m_protectionElements.push_back(element);
    element->addObserver(this);
}
#endif

void ControlBase::addProtectionChecker(ProtectionCheckerBase* protectionChecker)
{
    protectionChecker->addObserver(this);
//    protectionChecker->setPssId(getPssId());
    m_protectionCheckers.push_back(protectionChecker);
}

void ControlBase::endMove2Error()
{
    m_controlState = E_ControlState_Error;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Control {[PSSID:%d]} changed state to Error", m_pssId);
    //PscMasterServer::getInstance().sendSeqEnded(m_lastStateChangeMessageId, m_lastSn, Psc_ControllerId, getPssId(), E_SeqEndedStatus_Success);
    m_lastSn = 0;
    sendNotification();
}

void ControlBase::endStopOnEmr()
{
    // TODO: Restore moving to emergency when we can handle errors correctly.
    m_controlState = E_ControlState_Standby;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Control {[PSSID:%d]} changed state to Emr", m_pssId);
    m_lastSn = 0;
    sendNotification();
}

// return true when the error bits have changed.
bool ControlBase::raiseError(uint16_t secondaryPssId, E_PSSErrors error, bool errorState)
{
    uint32_t previousErrorBits, newErrorBits;
    previousErrorBits = m_errorBitManager.getErrors(secondaryPssId);
    newErrorBits = m_errorBitManager.raiseError(secondaryPssId, error, errorState);
    if (previousErrorBits != newErrorBits)
    {
        if (newErrorBits == 0)
        {
            setControlExceptions(E_ExceptionState_Reset, E_ExceptionState_Unchanged);
        }
        else
        {
            setControlExceptions(E_ExceptionState_Set, E_ExceptionState_Unchanged);
        }
        return true;
    }
    return false;
}

// return true when the error bits have changed.
bool ControlBase::raiseWarning(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState)
{
    uint32_t previousWarningBits, newWarningBits;
    previousWarningBits = m_errorBitManager.getWarnings(secondaryPssId);
    newWarningBits = m_errorBitManager.raiseWarning(secondaryPssId, warning, warningState);
    if (previousWarningBits != newWarningBits)
    {
        if (newWarningBits == 0)
        {
            setControlExceptions(E_ExceptionState_Unchanged, E_ExceptionState_Reset);
        }
        else
        {
            setControlExceptions(E_ExceptionState_Unchanged, E_ExceptionState_Set);
        }
        return true;
    }
    return false;
}

/**
 * Check all dependencies for the state of the control.
 * The "state" parameters controls whether to check if all feedbacks are true, or if all feedbacks are false.
 * If "state" is "Active", the function will return "true" if all feedbacks are true.
 * If "state" is "Inactive", the function will return "false" if all feedbacks are false.
 * resultConsiderDeactivateOnChange is used to check if we should deactivate a control which is currently active.
 * resultIgnoreDeactivateOnChange is used to issue errors in case an active control has a problem with dependencies.
 * The two parameters must be coexist because in case a control cannot be deactivate once active for protection considerations,
 * but an error must be issues for the OPC to start a proper deactivation sequence.
 * @param state
 * @return
 */
void ControlBase::checkAllDependencies(E_ActivationState activationState, E_ControlState controlState,
        bool &resultConsiderDeactivateOnChange, bool &resultIgnoreDeactiveOnChange)
{
    T_DeviceCheckerListIterator i;
    bool tempConsider, tempIgnore;
    if (controlState == E_ControlState_Ready)
    {
        if (activationState == E_ActivationState_Active)
        {
            resultConsiderDeactivateOnChange = true;
            resultIgnoreDeactiveOnChange = true;
            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
            {
                tempConsider = (*i).checkDevice(true) || !(*i).m_deactivateOnChange;
                tempIgnore = (*i).checkDevice(true);
                resultConsiderDeactivateOnChange &= tempConsider;
                resultIgnoreDeactiveOnChange &= tempIgnore;
            }
//            raiseError(0, E_PSSErrors_ActivationNotReady, (result == false));
            return;
        }
        else
        {
            resultConsiderDeactivateOnChange = false;
            resultIgnoreDeactiveOnChange = false;
            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
            {
                tempConsider = (!(*i).checkDevice(false) && !(*i).m_deactivateOnChange);
                tempIgnore = !(*i).checkDevice(false);
                resultConsiderDeactivateOnChange |= tempConsider;
                resultIgnoreDeactiveOnChange |= tempIgnore;
            }
//            raiseError(0, E_PSSErrors_ActivationNotReady, (result == true));
            return;
        }
    }
    else
    {
        if (activationState == E_ActivationState_Active)
        {
            resultConsiderDeactivateOnChange = true;
            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
            {
                tempConsider = (*i).checkDevice(true);
                resultConsiderDeactivateOnChange &= tempConsider;
            }
            resultIgnoreDeactiveOnChange = resultConsiderDeactivateOnChange;
//            raiseError(0, E_PSSErrors_ActivationNotReady, (result == false));
            return;
        }
        else
        {
            resultConsiderDeactivateOnChange = false;
            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
            {
                tempConsider = !(*i).checkDevice(false);
                resultConsiderDeactivateOnChange |= tempConsider;
            }
//            raiseError(0, E_PSSErrors_ActivationNotReady, (result == true));
            resultIgnoreDeactiveOnChange = resultConsiderDeactivateOnChange;
            return;
        }
    }
}

void ControlBase::endRecoverFromEmr()
{
//    m_controlState = E_ControlState_On;
    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Control {[PSSID:%d]} changed state to On", m_pssId);
    m_lastSn = 0;
    sendNotification();
}

void ControlBase::setBoardInReady(bool state)
{
}

void ControlBase::sendCurrentErrors()
{
    m_errorBitManager.sendCurrentErrors();

//    T_ProtectionCheckerListIterator i;
//    for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
//    {
//        (*i)->sendCurrentErrors();
//    }
}

void ControlBase::sendCurrentWarnings()
{
    m_errorBitManager.sendCurrentWarnings();
//    T_ProtectionCheckerListIterator i;
//    for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
//    {
//        (*i)->sendCurrentWarnings();
//    }
}

void ControlBase::addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan, bool deactivate,
        bool lock, int checkType)
{
    DeviceThresholdChecker checker;

    element->addObserver(this);
    checker.setElement(getPssId(), element);
    checker.m_thresholdValue = thresholdValue;
    checker.m_greaterThan = greaterThan;
    checker.m_deactivateOnChange = deactivate;
    checker.m_lockChange = lock;
    checker.m_dependencyCheckType = (E_DependencyCheckType) checkType;
    m_dependentCheckers.push_back(checker);
}

void ControlBase::logDependencyCheckFailures(E_ActivationState activationState, E_ControlState controlState,
        E_PSSErrors error)
{
    T_DeviceCheckerListIterator i;
    bool temp;
    if (controlState == E_ControlState_Ready)
    {
        if (activationState == E_ActivationState_Active)
        {
            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
            {
                temp = (*i).checkDevice(true) || !(*i).m_deactivateOnChange;
                if (!(*i).compareToPreviousCheckResult(temp))
                {
                    if (temp)
                    {
                        if (raiseError((*i).getPssId(), error, false))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "Activation operation {[PSSID:%d]} dependency cleared: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
                    }
                    else
                    {
                        if (raiseError((*i).getPssId(), error, true))
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                    "Activation operation {[PSSID:%d]} dependency failed: Device {[PSSID:%d]}",
                                    getPssId(), (*i).getElement()->getPssId());
//                    (*i).m_previousCheckResult = temp;
                    }
                }
            }
        }
        else
        {
            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
            {
                temp = (!(*i).checkDevice(false) && !(*i).m_deactivateOnChange);
                if (!(*i).compareToPreviousCheckResult(temp))
                {
                    if (temp)
                    {
                        if (raiseError((*i).getPssId(), error, true))
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                "Activation operation {[PSSID:%d]} dependency failed: Device {[PSSID:%d]}", getPssId(),
                                (*i).getElement()->getPssId());
                    }
                    else
                    {
                        if (raiseError((*i).getPssId(), error, false))
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                "Activation operation {[PSSID:%d]} dependency cleared: Device {[PSSID:%d]}", getPssId(),
                                (*i).getElement()->getPssId());
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
            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
            {
                temp = (*i).checkDevice(true);
                if (!(*i).compareToPreviousCheckResult(temp))
                {
                    if (temp)
                    {
                        if (raiseError((*i).getPssId(), error, false))
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                "Activation operation {[PSSID:%d]} dependency cleared: Device {[PSSID:%d]}", getPssId(),
                                (*i).getElement()->getPssId());
                    }
                    else
                    {
                        if (raiseError((*i).getPssId(), error, true))
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                "Activation operation {[PSSID:%d]} dependency failed: Device {[PSSID:%d]}", getPssId(),
                                (*i).getElement()->getPssId());
//                    (*i).m_previousCheckResult = temp;
                    }
                }
            }
        }
        else
        {
            for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
            {
                temp = !(*i).checkDevice(false);
                if (!(*i).compareToPreviousCheckResult(temp))
                {
                    if (temp)
                    {
                        if (raiseError((*i).getPssId(), error, true))
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                "Activation operation {[PSSID:%d]} dependency failed: Device {[PSSID:%d]}", getPssId(),
                                (*i).getElement()->getPssId());
                    }
                    else
                    {
                        if (raiseError((*i).getPssId(), error, false))
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                                "Activation operation {[PSSID:%d]} dependency cleared: Device {[PSSID:%d]}", getPssId(),
                                (*i).getElement()->getPssId());
//                    (*i).m_previousCheckResult = temp;
                    }
                }
            }
        }
    }
}

void ControlBase::clearDependencyCheckFailures(E_PSSErrors error)
{
    T_DeviceCheckerListIterator i;
    for (i = m_dependentCheckers.begin(); i != m_dependentCheckers.end(); ++i)
    {
        raiseError((*i).getPssId(), error, false);
        // TODO: Check if the resetPreviousCheckResult call is required.
        (*i).resetPreviousCheckResult();
    }
}

bool ControlBase::checkAllDependenciesForOppositeState()
{
    bool resultConsider, resultIgnore;
    switch (getActivationState())
    {
    case E_ActivationState_Active:
        checkAllDependencies(E_ActivationState_Inactive, E_ControlState_Standby, resultConsider, resultIgnore);
//        raiseError(0, E_PSSErrors_ActivationNotReady, (result == true));
        if (resultIgnore)
            logDependencyCheckFailures(E_ActivationState_Inactive, E_ControlState_Standby,
                    E_PSSErrors_ActivationNotReady);
        else
            clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
        return resultIgnore;
    case E_ActivationState_Unknown:
    case E_ActivationState_Inactive:
        checkAllDependencies(E_ActivationState_Active, E_ControlState_Standby, resultConsider, resultIgnore);
//        raiseError(0, E_PSSErrors_ActivationNotReady, (result == false));
        if (resultIgnore == false)
            logDependencyCheckFailures(E_ActivationState_Active, E_ControlState_Standby,
                    E_PSSErrors_ActivationNotReady);
        else
            clearDependencyCheckFailures(E_PSSErrors_ActivationNotReady);
        return resultIgnore;
    }
    return false;
}

