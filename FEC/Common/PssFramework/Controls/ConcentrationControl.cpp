/*
 * ConcentrationControl.cpp
 *
 *  Created on: 23 Mar 2016
 *      Author: maora
 */

#include <Controls/ConcentrationControl.h>
#include <PscServer/PscMasterServer.h>
#include <Tasks/UpdateSchedulerTask.h>

ConcentrationControl::ConcentrationControl()
{
    m_concentration = NULL;
    m_tankLevel = NULL;
    m_conditionerValve = NULL;
    m_waterValve = NULL;

    m_concentrationHighSetpoint = 0;
    m_liquidLevelHighSetpoint = 0;

    m_concentrationLowSetpoint = ElementRepository::getInstance().addValidationElementFloat();
    m_liquidLevelLowSetpoint = ElementRepository::getInstance().addValidationElementFloat();

    m_fillState = false;
    m_timeoutExpired = false;
}

ConcentrationControl::~ConcentrationControl()
{
    m_concentration->removeObserver(this);
    m_tankLevel->removeObserver(this);
}

void ConcentrationControl::updateNotification(ElementBase* element)
{
    if (element == m_concentration || element == m_tankLevel)
    {
        raiseError(element->getPssId(), E_PSSErrors_SensorMalfunction, !element->isValid());
        if (!element->isValid())
            move2Error(MSG_activateconcentrationcontrol, m_lastSn);
    }

    if (m_controlState == E_ControlState_Error && m_concentration->isValid() && m_tankLevel->isValid())
    {
        reset2On(0, 0);
    }

    if (m_controlState > E_ControlState_Standby && m_controlState <= E_ControlState_Ready)
    {
        execute();
    }
}

void ConcentrationControl::executeLevelControl()
{
    if (m_fillState)
    {
        if (m_tankLevel->getValueF() > m_liquidLevelHighSetpoint)
        {
            m_fillState = false;
            m_conditionerValve->setValue(false);
            m_waterValve->setValue(false);
        }
    }
    else
    {
        if (m_tankLevel->getValueF() < m_liquidLevelLowSetpoint->getValueF())
        {
            m_fillState = true;
            if (m_concentration->getValueF() < m_concentrationLowSetpoint->getValueF())
            {
                m_conditionerValve->setValue(true);
            }
            else
            {
                m_waterValve->setValue(true);
            }
        }
    }
}

void ConcentrationControl::executeLimitsCheck(ValidationElementFloat* checkingElement, float inputValue)
{
    int pssId = checkingElement->getPssId();

    if (checkingElement->isInWorkingRange(inputValue))
    {
        if (getControlExceptions() != 0)
        {
            raiseError(pssId, E_PSSErrors_ControlExceedsLimits, false);
            raiseWarning(pssId, E_PSSWarnings_ControlExceedsLimits, false);
        }
    }
    else
    {
        if (checkingElement->isInWarningRange(inputValue))
        {
            raiseError(pssId, E_PSSErrors_ControlExceedsLimits, false);
            raiseWarning(pssId, E_PSSWarnings_ControlExceedsLimits, true);
        }
        else
        {
            raiseError(pssId, E_PSSErrors_ControlExceedsLimits, true);
            raiseWarning(pssId, E_PSSWarnings_ControlExceedsLimits, false);
        }
    }
}

void ConcentrationControl::execute()
{
    float tankLevel = m_tankLevel->getValueF();
    float concentration = m_concentration->getValueF();

    if (m_timeoutExpired = false)
        return;

    executeLevelControl();
    switch (m_controlState)
    {
    case E_ControlState_Move2Ready:
        if (tankLevel > m_liquidLevelLowSetpoint->getValueF() && tankLevel < m_liquidLevelHighSetpoint
                && concentration > m_concentrationLowSetpoint->getValueF()
                && concentration > m_concentrationHighSetpoint)
        {
            m_controlState = E_ControlState_Ready;
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "ConcentrationControl {[PSSID:%d]} changed state to Ready", getPssId());
            sendNotification();
            if (m_lastSn != 0)
                PscMasterServer::getInstance().sendSeqEnded(MSG_activateconcentrationcontrol, m_lastSn,
                        Psc_ControllerId, getPssId(), E_SeqEndedStatus_Success);
        }
        break;
    case E_ControlState_Ready:
        executeLimitsCheck(m_liquidLevelLowSetpoint, tankLevel);
        executeLimitsCheck(m_concentrationLowSetpoint, concentration);
        break;
    }
}

bool ConcentrationControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_Float;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_concentrationLowSetpoint->getValue();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

bool ConcentrationControl::setSetpoint(float llLow, float llHigh, float llLoRange, float llHighRange, float llLoWarn,
        float llHighWarn, float cndLow, float cndHigh, float cndLoRange, float cndHighRange, float cndLoWarn,
        float cndHighWarn, uint32_t delay, uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return false;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "Updating ConcentrationControl Setpoint liquid=(%f)-(%f) ranges=(%f)-(%f)-(%f)-(%f) cond=(%f)-(%f) ranges=(%f)-(%f)-(%f)-(%f) delay=%d sn=%d",
            llLow, llHigh, llLoWarn, llLoRange, llHighRange, llHighWarn, cndLow, cndHigh, cndLoWarn, cndLoRange,
            cndHighRange, cndHighWarn, delay, sn);

    m_concentrationLowSetpoint->setValue(cndLow);
    m_concentrationHighSetpoint = cndHigh;
    m_concentrationLowSetpoint->updateWorkingRange(cndLow+cndLoRange, cndHigh+cndHighRange, true, true);
    m_concentrationLowSetpoint->updateWarningRange(cndLow+cndLoWarn, cndHigh+cndHighWarn, true, true);

    m_liquidLevelLowSetpoint->setValue(llLow);
    m_liquidLevelHighSetpoint = llHigh;
    m_liquidLevelLowSetpoint->updateWorkingRange(llLow+llLoRange, llHigh+llHighRange, true, true);
    m_liquidLevelLowSetpoint->updateWarningRange(llLow+llLoWarn, llHigh+llHighWarn, true, true);

    // remove current timeouts if they exist:
    UpdateSchedulerTask::getInstance()->addTimeout(this, 0);

    resetControl();

    if (delay == 0)
    {
        m_timeoutExpired = true;
    }
    else
    {
        m_timeoutExpired = false;
        UpdateSchedulerTask::getInstance()->addTimeout(this, delay);
    }

    m_controlState = E_ControlState_Move2Ready;

    sendNotification();

    m_lastSn = sn;

    return true;
}

void ConcentrationControl::resetControl()
{
    m_fillState = false;
    m_conditionerValve->setValue(false);
    m_waterValve->setValue(false);

    raiseError(m_concentration->getPssId(), E_PSSErrors_ControlExceedsLimits, false);
    raiseError(m_tankLevel->getPssId(), E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(m_concentration->getPssId(), E_PSSWarnings_ControlExceedsLimits, false);
    raiseWarning(m_tankLevel->getPssId(), E_PSSWarnings_ControlExceedsLimits, false);
}

bool ConcentrationControl::onInitControl()
{
    resetControl();

    endInitControl();
    return true;
}

bool ConcentrationControl::onMove2Error()
{
    resetControl();

    endMove2Error();
//    m_input->updateObservers();
    return true;
}

bool ConcentrationControl::onReset2On()
{
    resetControl();

    endReset2On();
    return true;
}

bool ConcentrationControl::onMove2Standby(uint32_t delay)
{
    resetControl();

    endMove2Standby();
//    m_input->updateObservers();
    return true;
}

bool ConcentrationControl::onStopOnEmr()
{
    resetControl();

    endStopOnEmr();
    return true;
}

bool ConcentrationControl::onRecoverFromEmr()
{
    resetControl();

    endRecoverFromEmr();
    return true;
}

void ConcentrationControl::timeoutExpired(uint16_t timeoutType)
{
    m_timeoutExpired = true;
}

E_ActivationState ConcentrationControl::getActivationState()
{
    if (m_controlState == E_ControlState_Move2Ready || m_controlState == E_ControlState_Ready)
        return E_ActivationState_Active;
    else
        return E_ActivationState_Inactive;
}

