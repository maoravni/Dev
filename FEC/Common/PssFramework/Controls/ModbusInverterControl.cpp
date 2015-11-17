/*
 * ModbusInverterControl.cpp
 *
 *  Created on: 10 ���� 2013
 *      Author: maora
 */

#include "ModbusInverterControl.h"
#include <PscServer/PscMasterServer.h>
#include <PscSubsystem.h>
#include <Peripherals/PeripheralRepository.h>
#include "logger.h"

ModbusInverterControl::ModbusInverterControl()
{
    m_enableOutput = NULL;
    m_outputCurrent = NULL;
    m_outputFrequency = NULL;
    m_setpoint = NULL;
    m_driveStatus = NULL;
    m_stopping = false;
    m_requestedSetpoint = ElementRepository::getInstance().addValidationElementFloat();
    m_lastSn = 0;
    m_isProtectionActive = false;
//    m_setpoint = ElementRepository::getInstance().addValidationElementFloat();
//    m_setpoint->addObserver(this);

}

ModbusInverterControl::~ModbusInverterControl()
{
//    *m_enableOutput = 0;
//    *m_setpoint = 0;
//    vTaskDelay(100);
    m_setpoint->removeObserver(this);
    m_outputFrequency->removeObserver(this);
    m_outputFrequency->removeObserver(this);
}

bool ModbusInverterControl::setSetpoint(float value)
{
    if (m_stopping || m_controlState == E_ControlState_On)
        return false;

    // if the old requested setpoint equals the new requested setpoint don't do anything.
//    if (m_requestedSetpoint == value && m_controlState == E_ControlState_Standby)
//        return true;

// change the state
// TODO: Implement a state machine, so that seq ended sending will be encapsulated.
    if (!m_stopping)
        m_controlState = E_ControlState_Move2Ready;

    // set the ranges to the input:
    if (m_setpoint->getValueF() != value)
    {
        m_setpoint->setValue(value);
    }
    m_requestedSetpoint->setValue(value);
    m_requestedSetpoint->updateWorkingRange(value - 1, value + 1, true, true);
    m_requestedSetpoint->updateWarningRange(value - 2, value + 2, true, true);

    *m_enableOutput = (value != 0 && !m_isProtectionActive) ? 1 : 0;

    sendNotification();

    return true;
}

bool ModbusInverterControl::setSetpoint(float value, uint32_t sn)
{
    m_lastSn = sn;
    return setSetpoint(value);
}

void ModbusInverterControl::setOutputEnableElement(ElementBase* element)
{
    m_enableOutput = element;
}

void ModbusInverterControl::setInverterPeripheral(ModbusInverterPeripheralBase* inverter)
{
//    m_setpoint = inverter->getFrequencySetpointElement();
    m_outputCurrent = inverter->getOutputCurrentElement();
    m_outputFrequency = inverter->getOutputFrequencyElement();
    m_setpoint->addObserver(this);
//    m_outputCurrent->addObserver(this);
    m_outputFrequency->addObserver(this);
}

void ModbusInverterControl::setOutputFrequencyElement(ElementBase* element)
{
    m_outputFrequency = element;
    m_outputFrequency->addObserver(this);
}

void ModbusInverterControl::setOutputCurrentElement(ElementBase* element)
{
    m_outputCurrent = element;
//    m_outputCurrent->addObserver(this);
}

void ModbusInverterControl::setOutputSetpointElement(ElementBase* element)
{
    m_setpoint = element;
    m_setpoint->addObserver(this);
}

void ModbusInverterControl::execute()
{
    switch (m_controlState)
    {
    case E_ControlState_On:
    case E_ControlState_Standby:
        // when we're on standby, we want the blower to stop, so we set the setpoint to "0".
        // if the blower is stopped, we need to lower the output so the motor would not get fried.
        if (m_outputFrequency->getValueF() != 0)
        {
            *m_setpoint = 0;
            *m_requestedSetpoint = 0;
        }
        else
        {
            if (m_enableOutput->getValueU32())
                *m_enableOutput = 0;
        }
        if (m_stopping && m_outputFrequency->getValueF() == 0)
        {
            m_stopping = false;
//            endMove2Standby();
        }
        break;
    case E_ControlState_Move2Ready:
        if (((int) (m_outputFrequency->getValueF() * 10)) == ((int) (m_setpoint->getValueF() * 10)))
        {
            m_controlState = E_ControlState_Ready;
            sendNotification();
            if (m_lastSn != 0)
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Inverter {[PSSID:%d]} changed state to Ready", getPssId());
                PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateInverterControl, m_lastSn, Psc_ControllerId,
                        getPssId(), E_SeqEndedStatus_Success);
                m_lastSn = 0;
            }
        }
        break;
    case E_ControlState_Ready:
        // TODO: Check output current and issue an error when required.
        // if there's already some errors or warnings, this might remove them, so we remove the exceeded limit
        // errors and warnings:
        float inputValue = m_outputFrequency->getValueF();

        if (m_requestedSetpoint->isInWorkingRange(inputValue))
        {
            if (getControlExceptions() != 0)
            {
                raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
                raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
            }
        }
        else
        {
//            if (m_requestedSetpoint->isInWarningRange(inputValue))
//            {
//                raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
//                raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, true);
//            }
//            else
//            {
//                raiseError(0, E_PSSErrors_ControlExceedsLimits, true);
//                raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
//            }
        }
        break;
    }
}

void ModbusInverterControl::updateNotification(ElementBase* element)
{
    if (!m_outputFrequency->isValid() || !m_outputCurrent->isValid()
            || (m_outputFrequency->getErrors() & E_PSSErrors_InverterInLocalMode))
    {
//        m_controlState = E_ControlState_Error;
//        sendNotification();
        move2Error(MSG_ActivateInverterControl, m_lastSn);
    }
    else
    {
        if (m_controlState == E_ControlState_Error)
        {
            // todo: call Reset2On.
//            m_controlState = E_ControlState_On;
//            sendNotification();
            reset2On(0, 0);
        }
    }

    if (element == m_outputFrequency)
    {
        execute();
    }
    else if (element == m_setpoint)
    {
        setSetpoint(m_setpoint->getValueF());
    }
    else if (element == m_driveStatus)
    {
    }
    else
    {
        // if it's not the input and not the setpoint, then it must be the protection.
        //      so instead of checking which one is it, we simply check the element
        //      we don't observe the value of the output element, so we can't receive events on it.
        if (executeProtectionCheck(element/*, E_PSSErrors_DeviceExceedsSoftLimits*/))
        {
//            raiseError(E_PSSErrors_DeviceExceedsSoftLimits, true);
            // we should also stop the inverter
            *m_enableOutput = 0;
            m_enableOutput->sendDeviceStatus();
        }
        else
        {
            *m_enableOutput = (m_requestedSetpoint != 0) ? 1 : 0;
            m_enableOutput->sendDeviceStatus();
//            raiseError(E_PSSErrors_DeviceExceedsSoftLimits, false);
        }
    }
}

bool ModbusInverterControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_setpoint->getValueF();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

//void ModbusInverterControl::addProtectionElement(ValidationElementBase* element)
//{
//    m_protectionElements.push_back(element);
//    element->addObserver(this);
//}

bool ModbusInverterControl::onInitControl()
{
//    *m_enableOutput = 1;
    execute();
    endInitControl();
    return true;
}

bool ModbusInverterControl::onMove2Standby()
{
    *m_enableOutput = 0;
    if (m_setpoint->getValueF() != 0)
    {
        *m_setpoint = 0;
        *m_requestedSetpoint = 0;
    }
    m_controlState = E_ControlState_Standby;
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
    m_stopping = true;
    endMove2Standby();
    execute();
    return true;
}

bool ModbusInverterControl::onReset2On()
{
    *m_enableOutput = 0;
    if (m_setpoint->getValueF() != 0)
    {
        *m_setpoint = 0;
        *m_requestedSetpoint = 0;
    }
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
    m_stopping = true;
    endReset2On();
    execute();
    return true;
}

bool ModbusInverterControl::onMove2Error()
{
    *m_enableOutput = 0;
    if (m_setpoint->getValueF() != 0)
    {
        *m_setpoint = 0;
        *m_requestedSetpoint = 0;
    }
    m_stopping = true;
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
    endMove2Error();
    execute();
    return true;
}

bool ModbusInverterControl::onStopOnEmr()
{
    *m_enableOutput = 0;
    if (m_setpoint->getValueF() != 0)
    {
        *m_setpoint = 0;
        *m_requestedSetpoint = 0;
    }
    m_controlState = E_ControlState_Standby;
    m_stopping = true;
    raiseError(0, E_PSSErrors_ControlExceedsLimits, false);
    raiseWarning(0, E_PSSWarnings_ControlExceedsLimits, false);
    endStopOnEmr();
    execute();
    return true;
}

bool ModbusInverterControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    execute();
    return true;
}

void ModbusInverterControl::setBoardInReady(bool state)
{
    ControlBase::setBoardInReady(state);
//    if (state)
//    {
//        m_driveStatus = PeripheralRepository::getInstance().getPeripheralContainingElementPssId(
//                m_outputFrequency->getPssId())->getElementByIndex(4);
//        if (m_driveStatus != NULL)
//        {
//            m_driveStatus->addObserver(this);
//        }
//    }
}
//void ModbusInverterControl::executeProtectionCheck(ElementBase* element)
//{
//    // Protection elements should stop the inverter is above max working, and continue when below min working.
//
//    // if the protection is not active, we can simply check the updated element if it should
//    // turn on the protection:
//    if (!m_isProtectionActive)
//    {
//        // if the temp is above protection limit, turn on the protection.
//        if (element->isOutsideSoftLimit())
//        {
//            m_isProtectionActive = true;
//            raiseError(E_PSSErrors_TemperatureExceedsLimits, true);
//            // we should also stop the inverter
//            *m_enableOutput = 0;
//            m_enableOutput->sendDeviceStatus();
//        }
//    }
//    // the protection is active, so we should check all protection elements to see if they are below limit:
//    else
//    {
//        int i;
//        bool safe = true;
//        for (i = 0; i < m_protectionElements.size(); ++i)
//        {
//            safe = (safe && m_protectionElements[i]->isInsideLimit());
//        }
//        if (safe)
//        {
//            m_isProtectionActive = false;
//            *m_enableOutput = (m_requestedSetpoint != 0) ? 1 : 0;
//            m_enableOutput->sendDeviceStatus();
//            raiseError(E_PSSErrors_TemperatureExceedsLimits, false);
//        }
//    }
//}
