/*
 * ObserveAndNotifyControl.cpp
 *
 *  Created on: 30 Jul 2015
 *      Author: vitalyp
 */

#include "Controls/LiftPbOnErrorCcsGen2.h"
#include <Peripherals/PeripheralRepository.h>
#include <Peripherals/PSoC/PsocManager.h>
#include <PscServer/PscMessageHandler.h>
#include "ObserveAndNotifyControl.h"
#include <PscServer/PscMasterServer.h>

ObserveAndNotifyControl::ObserveAndNotifyControl() :
        ControlBase()
{
    setPssId(M_PSS_ID_ALL);
    m_input = NULL;
    m_setpoint = ElementRepository::getInstance().addValidationElementFloat();
    m_setpoint->addObserver(this);
}

ObserveAndNotifyControl::~ObserveAndNotifyControl()
{
    m_input->removeObserver(this);
    m_setpoint->removeObserver(this);
}

void ObserveAndNotifyControl::setElementInput(ValidationElementFloat* input)
{
    m_input = input;
    m_input->addObserver(this);
    m_input->setMissingDevicePriority(E_MissingDevicePriority_High);
}

//void ObserveAndNotifyControl::setElementSetpoint(ValidationElementFloat* setpoint)
//{
//    m_setpoint = setpoint;
//    m_setpoint->addObserver(this);
//}
//
bool ObserveAndNotifyControl::setSetpoint(ValidationElementFloat* element)
{
    if (element->getValue() != m_setpoint->getValue())
    {
        return setSetpoint(element->getValue(), element->getMinWorking(), element->getMaxWorking(),
                element->getMinWarning(), element->getMaxWarning(), 0);
    }
    return true;
}

/*
 * set the setPoint from other controler
 *
 * sn- serial number, need to answer to the opc with the same number.
 */
bool ObserveAndNotifyControl::setSetpoint(float setpoint, uint32_t sn)
{
    return setSetpoint(setpoint, -5, +5, -10, +10, sn);
}

/*
 * this function sets the setPoint from outside and not from other conrol
 *
 */
bool ObserveAndNotifyControl::setSetpoint(float sp, float loRange, float hiRange, float loWarn, float hiWarn,
        uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return false;

    if (sp == 0)
    {
        return true;
    }

    // change the state
    // TODO: Implement a state machine, so that seq ended sending will be encapsulated.
    m_controlState = E_ControlState_Move2Ready;

    // set the ranges to the input:
    m_setpoint->updateWorkingRange(sp + loRange, sp + hiRange, true, true);
    m_setpoint->updateWarningRange(sp + loWarn, sp + hiWarn, true, true);
    *m_setpoint = sp;

    // lower the warnings/errors, in case they were raised previously.
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);

    sendNotification();

    m_lastSn = sn;

    return true;
}

void ObserveAndNotifyControl::execute()
{
    switch (m_controlState)
    {
    case E_ControlState_Move2Ready:
        // TODO: add a timeout for reaching the set point
        if (m_input->isValid() && m_setpoint->isInWorkingRange(m_input->getValue()))
        {
            m_controlState = E_ControlState_Ready;
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "ObserveAndNotifyControl {[PSSID:%d]} changed state to Ready", getPssId());
            sendNotification();
            if (m_lastSn != 0)
                PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateObserveAndNotifyControlMsg, m_lastSn, Psc_ControllerId,
                        getPssId(), E_SeqEndedStatus_Success);
        }
        break;
    case E_ControlState_Ready:
        // TODO: Check working range and warning range, and issue an error
        // TODO: Check that errors are communicated properly to the SUM.
        float inputValue = m_input->getValue();
        if (m_input->isValid())
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
                }
            }
        }
        break;
    }
}

bool ObserveAndNotifyControl::sendNotification()
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

void ObserveAndNotifyControl::updateNotification(ElementBase* element)
{
    if (element == m_input)
    {
        if (element->isValid())
            execute();
        else
            move2Error(MSG_ActivateObserveAndNotifyControlMsg, m_lastSn);
    }
}

bool ObserveAndNotifyControl::onInitControl()
{
    endInitControl();
    return true;
}

bool ObserveAndNotifyControl::onMove2Standby(uint32_t delay)
{
    endMove2Standby();
    return true;
}

bool ObserveAndNotifyControl::onReset2On()
{
    endReset2On();
    return true;
}

bool ObserveAndNotifyControl::onMove2Error()
{
    endMove2Error();
    return true;
}

bool ObserveAndNotifyControl::onStopOnEmr()
{
    endStopOnEmr();
    return true;
}

//bool ObserveAndNotifyControl::initInputElements(int cableId)
//{
//    return true;
//}

bool ObserveAndNotifyControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

