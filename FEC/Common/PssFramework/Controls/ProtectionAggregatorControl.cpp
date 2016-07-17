/*
 * ProtectionAggregatorControl.cpp
 *
 *  Created on: 13 Jul 2016
 *      Author: maora
 */

#include <Controls/ProtectionAggregatorControl.h>
#include <PscServer/PscMasterServer.h>
#include <Persistency/ControlSerializers.h>

ProtectionAggregatorControl::ProtectionAggregatorControl()
{
    m_calculatedOutput = NULL;
    m_bitwiseOperation = E_BitwiseOperation_And;
    m_negateResult = false;
}

ProtectionAggregatorControl::~ProtectionAggregatorControl()
{
}

bool ProtectionAggregatorControl::onInitControl()
{
    endInitControl();
    return true;
}

bool ProtectionAggregatorControl::onMove2Standby(uint32_t delay)
{
    endMove2Standby();
    return true;
}

bool ProtectionAggregatorControl::onReset2On()
{
    endReset2On();
    return true;
}

bool ProtectionAggregatorControl::onMove2Error()
{
    endMove2Error();
    return true;
}

bool ProtectionAggregatorControl::onStopOnEmr()
{
    endStopOnEmr();
    return true;
}

bool ProtectionAggregatorControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

void ProtectionAggregatorControl::updateNotification(ElementBase* element)
{
    T_ProtectionCheckerListIterator i;

    // calculate the bitwise operation of all protections.
    switch (m_bitwiseOperation)
    {
    case E_BitwiseOperation_And:
        m_isProtectionActive = true;

        for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
        {
            m_isProtectionActive &= ((*i)->getProtectionState() >= E_DeviceProtectionState_SoftLimitExceeded);
        }
        break;
    case E_BitwiseOperation_Or:
        m_isProtectionActive = false;

        for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
        {
            m_isProtectionActive |= ((*i)->getProtectionState() >= E_DeviceProtectionState_SoftLimitExceeded);
        }
        break;
    case E_BitwiseOperation_Xor:
        m_isProtectionActive = false;

        for (i = m_protectionCheckers.begin(); i != m_protectionCheckers.end(); ++i)
        {
            m_isProtectionActive ^= ((*i)->getProtectionState() >= E_DeviceProtectionState_SoftLimitExceeded);
        }
        break;
    }

    // negate the result if needed.
    m_isProtectionActive = (m_negateResult) ? !m_isProtectionActive : m_isProtectionActive;

    m_calculatedOutput->setValue((uint32_t)m_isProtectionActive);
}

void ProtectionAggregatorControl::execute()
{
}

bool ProtectionAggregatorControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_U32;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_calculatedOutput->getValueU32();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

void ProtectionAggregatorControl::setOperation(E_BitwiseOperation bitOp, bool negateResult)
{
    m_bitwiseOperation = bitOp;
    m_negateResult = negateResult;
}

void ProtectionAggregatorControl::serialize(F_FILE* f)
{
    Serializer<ProtectionAggregatorControl> s;
    s.serialize(f, *this);
}

ProtectionAggregatorControl::ProtectionAggregatorControl(F_FILE* f)
{
    Serializer<ProtectionAggregatorControl> s;
    s.deserialize(f, *this);
}
