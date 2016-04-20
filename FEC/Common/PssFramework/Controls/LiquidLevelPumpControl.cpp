/*
 * LiquidLevelPumpControl.cpp
 *
 *  Created on: 1 בדצמ 2013
 *      Author: maora
 */

#include "LiquidLevelPumpControl.h"
#include <PscServer/PscMasterServer.h>
#include <PscSubsystem.h>
#include <Elements/ElementRepository.h>
#include "logger.h"

LiquidLevelPumpControl::LiquidLevelPumpControl()
{
    m_fillPumpValue = 0;
    m_fillPumpProtectionActive = false;
    m_tankLevelInput = NULL;
    m_drainPump = NULL;
    m_fillPump = NULL;
    m_stopRequested = false;
    m_currentSetpoint = 0;

    // TODO: Need to add validation values for these variables.
    m_lowSetpoint = ElementRepository::getInstance().addValidationElementFloat();
    m_midSetpoint = ElementRepository::getInstance().addValidationElementFloat();
    m_highSetpoint = ElementRepository::getInstance().addValidationElementFloat();

    m_currentTankLevel = 0;
    m_requestedState = E_ControlState_Off;
}

LiquidLevelPumpControl::~LiquidLevelPumpControl()
{
    m_tankLevelInput->removeObserver(this);
    m_fillPump->removeObserver(this);
}

void LiquidLevelPumpControl::execute()
{
    bool activateProtection = false;

    switch (m_tankLevelInput->getValueI32())
    {
    case 0:
        m_currentSetpoint = 0;
        if (m_controlState == E_ControlState_Move2Ready)
        {
            sendNotification();
            m_controlState = E_ControlState_Ready;
            if (m_lastSn != 0)
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "LiquidLevelPumpControl {[PSSID:%d]} changed state to Ready", getPssId());
                PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateWaterTankLevelControl, m_lastSn, Psc_ControllerId, getPssId(),
                        E_SeqEndedStatus_Success);
            }
        }
        break;
    case 1:
        m_currentSetpoint = m_lowSetpoint->getValueF();
        break;
    case 2:
        m_currentSetpoint = m_midSetpoint->getValueF();
        break;
    case 3:
        m_currentSetpoint = m_highSetpoint->getValueF();
        activateProtection = true;
        break;
    default:
        // TODO: Issue an error about an illegal state of the liquid level tank.
        break;
    }

    // TODO: check the state of the inverter controls, to see if they're not in some error.

    m_drainPump->setValue(m_currentSetpoint);
    // if we need to turn on the protection, stop the fill pump.
    if (activateProtection && !m_fillPumpProtectionActive && m_fillPump->getValueI32() != 0)
    {
        m_fillPumpProtectionActive = true;
        m_fillPump->setValue((uint32_t)0);
    }

    // if we need to turn off the protection
    if (m_fillPumpProtectionActive && !activateProtection)
    {
        // put the fill pump into its previous speed:
        m_fillPumpProtectionActive = false;
        m_fillPump->setValue(m_fillPumpValue);
    }

    // if we need to stop the control wait for the liquid level to drop to 0 and then stop the pumps.
    if (m_stopRequested)
    {
        if (m_tankLevelInput->getValueI32() == 0)
        {
            m_stopRequested = false;
            m_currentSetpoint = 0;
            switch (m_requestedState)
            {
            case E_ControlState_Standby:
                endMove2Standby();
                break;
            case E_ControlState_On:
                endReset2On();
                break;
            case E_ControlState_Error:
                endMove2Error();
                break;
            }
//            if (m_requestedState == E_ControlState_Standby)
//                endMove2Standby();
//            else
//                endReset2On();
            if (m_fillPump != NULL && m_fillPump->getValueI32() != 0)
                m_fillPump->setValue((uint32_t)0);
        }
    }
}

void LiquidLevelPumpControl::updateNotification(ElementBase* element)
{
    if (element == m_fillPump && !m_fillPumpProtectionActive)
        m_fillPumpValue = m_fillPump->getValueF();

//    if (m_controlState == E_ControlState_Move2Ready)
//    {
//        sendNotification();
//        m_controlState = E_ControlState_Ready;
//    }

    if (m_controlState > E_ControlState_Standby && m_controlState <= E_ControlState_Ready)
        execute();
}

bool LiquidLevelPumpControl::onInitControl()
{
    endInitControl();
    return true;
}

bool LiquidLevelPumpControl::onMove2Standby(uint32_t delay)
{
    m_requestedState = E_ControlState_Standby;
    m_stopRequested = true;
    execute();
    return true;
}

bool LiquidLevelPumpControl::onReset2On()
{
    m_requestedState = E_ControlState_On;
    m_stopRequested = true;
    m_currentSetpoint = 0;
    execute();
    return true;
}

bool LiquidLevelPumpControl::onMove2Error()
{
    m_requestedState = E_ControlState_Error;
    m_stopRequested = true;
    m_currentSetpoint = 0;
    execute();
    return true;

}

bool LiquidLevelPumpControl::onStopOnEmr()
{
    m_requestedState = E_ControlState_Standby;
    m_stopRequested = true;
    execute();
    return true;
}

bool LiquidLevelPumpControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

bool LiquidLevelPumpControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_Float;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_currentSetpoint;
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

//void LiquidLevelPumpControl::addProtectionElement(ValidationElementBase* element)
//{
//}

bool LiquidLevelPumpControl::setSetpoint(float low, float mid, float high, uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return false;

    // change the state
    // TODO: Implement a state machine, so that seq ended sending will be encapsulated.
    m_controlState = E_ControlState_Move2Ready;

    // set the ranges to the input:
    m_lowSetpoint->setValue(low);
    m_midSetpoint->setValue(mid);
    m_highSetpoint->setValue(high);

    execute();
    sendNotification();

//    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "LiquidLevelPumpControl %d changed state to Ready", getPssId());
//    PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateWaterTankLevelControl, sn, Psc_ControllerId, getPssId(),
//            E_SeqEndedStatus_Success);

    m_lastSn = sn;

    return true;
}
