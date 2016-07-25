/*
 * AnalogOutInverterControl.cpp
 *
 *  Created on: 6 баев 2013
 *      Author: maora
 */

#include "AnalogOutInverterControl.h"
#include <PscSubsystem.h>
#include <PscServer/PscMasterServer.h>
#include <Persistency/ControlSerializers.h>

AnalogOutInverterControl::AnalogOutInverterControl()
{
    m_enableOutput = NULL;
    m_deviceSetpoint = NULL;
    m_requestedSetpointElement = NULL;
}

AnalogOutInverterControl::~AnalogOutInverterControl()
{
}

bool AnalogOutInverterControl::setSetpoint(float value, uint32_t sn)
{
    if (m_controlState == E_ControlState_On)
        return false;

    m_controlState = E_ControlState_Ready;
    sendNotification();

    m_requestedSetpointElement->setValue(value);
    setOutputValue(value);

    m_lastSn = sn;

    if (m_lastSn != 0)
        PscMasterServer::getInstance().sendSeqEnded(MSG_ActivateInverterControl, m_lastSn, Psc_ControllerId, getPssId(),
                E_SeqEndedStatus_Success);

    return true;
}

void AnalogOutInverterControl::setOutputValue(float value)
{
    if (m_isProtectionActive)
    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "{[PSSID:%d]} Protection Active. Setpoint %f", getPssId(), value);
        value = 0;
    }

    if (m_deviceSetpoint->getValueF() != value)
    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "{[PSSID:%d]} Setting output device to setpoint %f", getPssId(), value);
        m_deviceSetpoint->setValue(value);
    }
    if (m_enableOutput != NULL)
    {
        if (value == 0)
            m_enableOutput->setValue((uint32_t) 0);
        else
            m_enableOutput->setValue((uint32_t) 1);
    }
}

void AnalogOutInverterControl::updateNotification(ElementBase* element)
{
    if (element == m_requestedSetpointElement)
    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "{[PSSID:%d]} Setting output value to %f", getPssId(), element->getValueF());
        setOutputValue(element->getValueF());
    }
    else
    {
        executeProtectionCheck(element/*, E_PSSErrors_DeviceExceedsSoftLimits*/);
        if (m_isProtectionActive)
        {
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "{[PSSID:%d]} Protection Active. Setpoint %f", getPssId(), m_requestedSetpointElement->getValueF());
            setOutputValue(0);
        }
        else
        {
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "{[PSSID:%d]} Protection Inactive. Setpoint %f", getPssId(), m_requestedSetpointElement->getValueF());
            setOutputValue(m_requestedSetpointElement->getValueF());
        }
    }
}

void AnalogOutInverterControl::setOutputEnableElement(ElementU8* element)
{
    m_enableOutput = element;
//    m_enableOutput->addObserver(this);
}

void AnalogOutInverterControl::setSetpointElement(ValidationElementFloat* element)
{
    m_deviceSetpoint = element;
//    m_deviceSetpoint->addObserver(this);
    if (m_requestedSetpointElement == NULL)
    {
        m_requestedSetpointElement = ElementRepository::getInstance().addValidationElementFloat();
    }
    m_requestedSetpointElement->addObserver(this);
    m_requestedSetpointElement->setPssId(getPssId());
//    m_setpoint->setPssId(getPssId());
}

void AnalogOutInverterControl::execute()
{
}

bool AnalogOutInverterControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_Float;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_requestedSetpointElement->getValue();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

//void AnalogOutInverterControl::addProtectionElement(ValidationElementBase* element)
//{
//}
//
bool AnalogOutInverterControl::onInitControl()
{
//    *m_enableOutput = 1;
    executeProtectionCheck(/*E_PSSErrors_DeviceExceedsSoftLimits*/);
    endInitControl();
    return true;
}

bool AnalogOutInverterControl::onMove2Standby(uint32_t delay)
{
    setOutputValue(0);
    endMove2Standby();
    return true;
}

bool AnalogOutInverterControl::onReset2On()
{
    setOutputValue(0);
    endReset2On();
    return true;
}

bool AnalogOutInverterControl::onMove2Error()
{
    setOutputValue(0);
    endMove2Error();
    return true;
}

bool AnalogOutInverterControl::onStopOnEmr()
{
    setOutputValue(0);
    endStopOnEmr();
    return true;
}

bool AnalogOutInverterControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

void AnalogOutInverterControl::serialize(F_FILE* f)
{
    Serializer<AnalogOutInverterControl> s;
    s.serialize(f, *this);
}

AnalogOutInverterControl::AnalogOutInverterControl(F_FILE* f)
{
    Serializer<AnalogOutInverterControl> s;
    s.deserialize(f, *this);
    setSetpointElement(m_deviceSetpoint);
}

