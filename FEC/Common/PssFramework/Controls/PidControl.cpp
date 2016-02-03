/*
 * PidControl.cpp
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#include "PidControl.h"
#include "ControlRepository.h"
#include <PscSubsystem.h>
#include <logger.h>
#include <PscServer/PscMasterServer.h>
#include <Tasks/UpdateSchedulerTask.h>

PidControl::PidControl()
{
    m_input = NULL;
    m_output = NULL;
    m_setpoint = NULL;
    m_isInAutotune = false;
    setPowerLimit(100);
    setTuningValues(1, 0, 0, 0);
    setOutputSmoothing(0);
    m_isProtectionActive = false;
    m_pPidAutotune = NULL;
    m_pidCalc.Initialize();
    m_feedForward = 0;
    m_timeoutExpired = true;
    m_outputIsControl = false;
//    m_fromMove2Standby = false;
}

PidControl::~PidControl()
{
    m_input->removeObserver(this);
    m_output->removeObserver(this);
    m_setpoint->removeObserver(this);
}

void PidControl::setPowerLimit(float maxPower)
{
    m_pidCalc.SetOutputLimits(0, maxPower);
}

E_AckStatus PidControl::startAutoTune(float setpoint, float overshoot, float outputStart, float outputStep,
        u16_t lookback, bool reverseDirection, uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return E_AckStatus_InvalidState;

    if (m_isInAutotune)
        cancelAutoTune();

    m_pPidAutotune = new PID_ATune(E_PID_ControlType_PID, lookback, setpoint, overshoot, outputStart, outputStep);

    if (m_pPidAutotune == NULL)
    {
        cancelAutoTune();
        return E_AckStatus_InvalidDevice;
    }

//    m_lastSn = sn;
    m_isInAutotune = true;

    setSetpoint(setpoint, -5, +5, -10, +10, 0, 0, sn);

    return E_AckStatus_Success;
}

void PidControl::cancelAutoTune()
{
    if (m_pPidAutotune != NULL)
    {
        m_pPidAutotune->Cancel();
        delete m_pPidAutotune;
        m_pPidAutotune = NULL;
    }

    if (m_isInAutotune)
    {
        m_isInAutotune = false;
        PscMasterServer::getInstance().sendSeqEnded(MSG_AutoTune, m_lastSn, Psc_ControllerId, getPssId(),
                E_SeqEndedStatus_Stopped);
    }
}

void PidControl::updateNotification(ElementBase* element)
{
    if (element == m_output)
    {
        raiseError(element->getPssId(), E_PSSErrors_SensorMalfunction, !m_output->isValid());
        if (!m_output->isValid())
            move2Error(MSG_ActivatePIDControl, m_lastSn);
        return;
    }

    if (m_controlState == E_ControlState_On)
    {
        if (element == m_input && !m_input->isValid())
            move2Error(MSG_ActivatePIDControl, m_lastSn);
        executeLocalProtectionCheck(element);
        return;
    }

    if (m_controlState == E_ControlState_Error && m_input->isValid())
    {
        // todo: call Reset2On.
//        m_controlState = E_ControlState_On;
//        sendNotification();
        reset2On(0, 0);
    }

    if (element == m_input)
    {
        if (m_controlState > E_ControlState_Standby && m_controlState <= E_ControlState_Ready)
        {
//            execute();
            if (m_input->isValid())
                execute();
            else
                move2Error(MSG_ActivatePIDControl, m_lastSn);
        }
        return;
    }
    if (element == m_setpoint)
    {
        setSetpoint(m_setpoint);
        return;
    }
    // if it's not the input and not the setpoint, then it must be the protection.
    //      so instead of checking which one is it, we simply check the element
    //      we don't observe the value of the output element, so we can't receive events on it.
    executeLocalProtectionCheck(element);
}

bool PidControl::executeLocalProtectionCheck(ElementBase* element)
{
    bool result = ControlBase::executeProtectionCheck(element/*, E_PSSErrors_DeviceExceedsSoftLimits*/);
    if (result)
    {
        // TODO: Change error to "Protection Activated"
//        raiseError(E_PSSErrors_DeviceExceedsSoftLimits, true);
        // we should also stop the heating:
//        *m_output = 0;
//        m_output->sendDeviceStatus();
        resetOutput();
    }

    return result;
}

void PidControl::execute()
{
    // calculate the PID output value.
    float pidResult;

    if (m_isInAutotune)
    {
        pidResult = m_pPidAutotune->Compute(m_input->getValue());
        if (!m_pPidAutotune->isRunning())
        {
            m_isInAutotune = false;
            m_pidCalc.SetTunings(m_pPidAutotune->GetKp(), m_pPidAutotune->GetKi(), m_pPidAutotune->GetKd(), 0);
            //            printf("p: %f, i: %f, d: %f\n", m_pidAutoTune.GetKp(), m_pidAutoTune.GetKi(), m_pidAutoTune.GetKd());
            PscMasterServer::getInstance().sendSeqEnded(MSG_AutoTune, m_lastSn, Psc_ControllerId, getPssId(),
                    E_SeqEndedStatus_Success);
            sendPidValues();
            delete m_pPidAutotune;
            m_pPidAutotune = NULL;
        }
//        {
//            m_isInAutotune = false;
//            m_pidCalc.SetTunings(getKp()+1, getKi()+1, getKd()+1);
//            //            printf("p: %f, i: %f, d: %f\n", m_pidAutoTune.GetKp(), m_pidAutoTune.GetKi(), m_pidAutoTune.GetKd());
//            PscMasterServer::getInstance().sendSeqEnded(MSG_AutoTune, m_lastSn, Psc_ControllerId, getPssId(),
//                    E_SeqEndedStatus_Success);
//            sendPidValues();
//            delete m_pPidAutotune;
//            m_pPidAutotune = NULL;
//        }
    }
    else
        pidResult = m_pidCalc.Compute(m_input->getValue());

//    if (!m_input->isValid())
//        move2Error(0, 0);
//
    // if protection is not active, set the output result to the output element:
    if (!m_isProtectionActive)
        *m_output = pidResult;
//    else
//        *m_output = 0;

    switch (m_controlState)
    {
    case E_ControlState_Move2Ready:
        if (!m_timeoutExpired)
            break;
        // TODO: add a timeout for reaching the set point
        if (m_input->isValid() && m_setpoint->isInWorkingRange(m_input->getValue()))
        {
            m_controlState = E_ControlState_Ready;
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "PidControl {[PSSID:%d]} changed state to Ready", getPssId());
            sendNotification();
            if (m_lastSn != 0)
                PscMasterServer::getInstance().sendSeqEnded(MSG_ActivatePIDControl, m_lastSn, Psc_ControllerId,
                        getPssId(), E_SeqEndedStatus_Success);
        }
        break;
    case E_ControlState_Ready:
        // TODO: Check working range and warning range, and issue an error
        // TODO: Check that errors are communicated properly to the SUM.
        float inputValue = m_input->getValue();
        if (!m_input->isValid())
        {
//            raiseError()
        }
        else
        {
            // if there's already some errors or warnings, this might remove them, so we remove the exceeded limit
            // errors and warnings:
            if (m_setpoint->isInWorkingRange(inputValue))
            {
                if (getControlExceptions() != 0)
                {
                    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
                    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
                }
            }
            else
            {
                if (m_setpoint->isInWarningRange(inputValue))
                {
                    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
                    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, true);
                }
                else
                {
                    raiseError(0, E_PSSErrors_ControlExceedsLimits, true);
                    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
                }
            }
        }
        break;
    }
}

float PidControl::getKp()
{
    return m_pidCalc.getKp();
}

float PidControl::getKi()
{
    return 1 / m_pidCalc.getKi();
}

float PidControl::getKd()
{
    return m_pidCalc.getKd();
}

bool PidControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_Float;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_setpoint->getValue();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

bool PidControl::setSetpoint(float sp, float loRange, float hiRange, float loWarn, float hiWarn, float feedForward,
        uint32_t delay, uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return false;

//    if (sp == 0)
//    {
//        m_pidCalc.setEnabled(false);
//        return true;
//    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Updating PidControl Setpoint sp=%f (%f)-(%f)-(%f)-(%f) FF=%f delay=%d sn=%d",
            sp, loWarn, loRange, hiRange, hiWarn, feedForward, delay, sn);

    taskENTER_CRITICAL();

    // remove current timeouts if they exist:
    UpdateSchedulerTask::getInstance()->addTimeout(this, 0);

    // set the ranges to the input:
    m_setpoint->updateWorkingRange(sp + loRange, sp + hiRange, true, true);
    m_setpoint->updateWarningRange(sp + loWarn, sp + hiWarn, true, true);
    *m_setpoint = sp;
    m_feedForward = feedForward;

    // lower the warnings/errors, in case they were raised previously.
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);

    if (delay == 0)
    {
        m_timeoutExpired = true;
        m_pidCalc.setAutoMode(true);
        m_pidCalc.setEnabled(true);
        m_pidCalc.setSetPoint(sp, m_feedForward);
    }
    else
    {
        m_timeoutExpired = false;
        m_pidCalc.setAutoMode(false);
        m_pidCalc.setEnabled(true);
        m_pidCalc.setSetPoint(sp, m_feedForward);
        UpdateSchedulerTask::getInstance()->addTimeout(this, delay);
    }

    // change the state
    // TODO: Implement a state machine, so that seq ended sending will be encapsulated.
    m_controlState = E_ControlState_Move2Ready;

    taskEXIT_CRITICAL();

    sendNotification();

    m_lastSn = sn;

    return true;
}

void PidControl::sendTuningValues()
{
    // TODO implement sendTuningValues;
}

bool PidControl::setSetpoint(ValidationElementFloat* element)
{
    if (!m_pidCalc.isEnabled() && element->getValue() != m_pidCalc.getSetPoint())
    {
        return setSetpoint(element->getValue(), element->getMinWorking(), element->getMaxWorking(),
                element->getMinWarning(), element->getMaxWarning(), m_feedForward, 0, 0);
    }
    return true;
}

void PidControl::setOutputSmoothing(unsigned short outputSmoothing)
{
    m_pidCalc.setOutputSmoothingWindow(outputSmoothing);
}

void PidControl::setTuningValues(float kp, float ki, float kd, float kTrapezRange)
{
    if (ki == 0)
        m_pidCalc.SetTunings(kp, 0, kd, kTrapezRange);
    else
        m_pidCalc.SetTunings(kp, 1 / ki, kd, kTrapezRange);
}

void PidControl::sendPidValues()
{
    PscMessageStruct reply;
    PSSGetPIDReplyMsg *payload = &reply.payload.pSSGetPIDReplyMsg;

    //TODO: Check if the board address should be hard-coded.
    reply.header.id.split.id = MSG_GetPIDReply;
    reply.header.length = sizeof(reply.header) + sizeof(*payload);
    payload->cableId = Psc_ControllerId;
    payload->pssId = getPssId();
    payload->p = getKp();
    payload->i = getKi();
    payload->d = getKd();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Pid Values Sent: controller=%d, zone={[PSSID:%d]}, PID=%f, %f, %f",
            Psc_ControllerId, payload->pssId, payload->p, payload->i, payload->d);

    PscMasterServer::getInstance().sendMessage(reply);
}

bool PidControl::onMove2Standby(uint32_t delay)
{
    m_pidCalc.setEnabled(false);
    if (m_output->getValueF() != 0)
    {
        resetOutput();
//        *m_output = 0;
//    m_output->sendDeviceStatus();
    }
    m_pidCalc.setEnabled(false);
    m_pidCalc.Initialize();

    // lower the warnings/errors, in case they were raised previously.
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);

    endMove2Standby();
//    m_input->updateObservers();
    return true;
}

bool PidControl::onMove2Error()
{
    m_pidCalc.setEnabled(false);
    if (m_output->getValueF() != 0)
    {
        resetOutput();
//        *m_output = 0;
//    m_output->sendDeviceStatus();=
    }
    m_pidCalc.reset();
    cancelAutoTune();
    endMove2Error();
//    m_input->updateObservers();
    return true;
}

bool PidControl::onReset2On()
{
    m_pidCalc.setEnabled(false);
    if (m_output->getValueF() != 0)
    {
        resetOutput();
//        *m_output = 0;
//    m_output->sendDeviceStatus();
    }
    m_pidCalc.reset();
    cancelAutoTune();

    // lower the warnings/errors, in case they were raised previously.
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);

    endReset2On();
//    m_input->updateObservers();
    return true;
}

bool PidControl::onInitControl()
{
//    m_input->updateObservers();

    // lower the warnings/errors, in case they were raised previously.
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);

    endInitControl();
    return true;
}

#ifdef OLD_PROTECTION
void PidControl::addProtectionElement(ValidationElementBase* element)
{
    if (element == m_input)
    {
        m_inputIsInProtection = true;
    }
    ControlBase::addProtectionElement(element);
}
#endif

bool PidControl::onStopOnEmr()
{
    m_pidCalc.setEnabled(false);
    {
        resetOutput();
//        *m_output = 0;
//    m_output->sendDeviceStatus();
    }
    m_pidCalc.setEnabled(false);
    m_pidCalc.Initialize();

    // lower the warnings/errors, in case they were raised previously.
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);

    endStopOnEmr();
//    m_input->updateObservers();
    return true;
}

bool PidControl::onRecoverFromEmr()
{
    // lower the warnings/errors, in case they were raised previously.
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);

    endRecoverFromEmr();
    return true;
}

//void PidControl::addDependentElement(ElementBase* element, float thresholdValue, bool greaterThan,
//        bool deactivate, bool lock, int checkType)
//{
//    DeviceThresholdChecker checker;
//    element->addObserver(this);
////    checker.m_element = element;
//    checker.setElement(getPssId(), element);
//    checker.m_thresholdValue = thresholdValue;
//    checker.m_greaterThan = greaterThan;
//    checker.m_deactivateOnChange = deactivate;
//    checker.m_lockChange = lock;
//    checker.m_dependencyCheckType = (E_DependencyCheckType) checkType;
//    m_dependentCheckers.push_back(checker);
//}

bool PidControl::executeDependencyCheck(ElementBase* element)
{
    return true;
}

void PidControl::timeoutExpired(uint16_t timeoutType)
{
    m_timeoutExpired = true;
    m_pidCalc.setAutoMode(true);
    m_pidCalc.setEnabled(true);
    m_pidCalc.setSetPoint(m_setpoint->getValueF(), m_feedForward);
}

E_ActivationState PidControl::getActivationState()
{
    if (m_controlState == E_ControlState_Move2Ready || m_controlState == E_ControlState_Ready)
        return E_ActivationState_Active;
    else
        return E_ActivationState_Inactive;
}

void PidControl::setPssId(uint16_t deviceId)
{
    ControlBase::setPssId(deviceId);
    m_pidCalc.setPssId(getPssId());
}

void PidControl::setSetpointRange(float setpointRange)
{
    m_pidCalc.setSetPointRange(setpointRange);
}

void PidControl::resetOutput()
{
    *m_output = 0;
//    if (m_outputIsControl)
//    {
//        ControlRepository::getInstance().getControlByPssId(m_output->getPssId())->sendNotification();
//    }
//    else
        m_output->sendDeviceStatus();
}
