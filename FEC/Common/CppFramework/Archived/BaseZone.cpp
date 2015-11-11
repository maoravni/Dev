/*
 * BaseZone.cpp
 *
 *  Created on: 6 Dec 2012
 *      Author: maora
 */

#include "BaseZone.h"
#include <TmcMasterServer.h>
#include <string.h>
#include "logger.h"

// TODO: assert when accessing a zone with value of 0xfe
BaseZone::BaseZone()
{
    m_enabled = false;
    m_monitorEnabled = false;
    m_zoneId = 0xfe;
    m_state = E_ZoneState_On;
    m_hasErrors = false;
    m_hasWarnings = false;
    m_zoneId = M_BASE_ZONE_UNASSIGNED_ID;
}

//BaseZone::BaseZone()
//{
//    m_enabled = false;
//    m_zoneType = E_ZoneType_Size;
//}

BaseZone::~BaseZone()
{
}

void BaseZone::sendMonitorReply(E_DataTypes dataType, U32 value)
{
    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_MonitorDataNotification;
    replyMessage.payload.pSSMonitoringDataNotificationMsg.cableId = m_controllerId;
    replyMessage.payload.pSSMonitoringDataNotificationMsg.zoneId = m_zoneId;
    replyMessage.payload.pSSMonitoringDataNotificationMsg.dataSpecification = dataType;
    replyMessage.payload.pSSMonitoringDataNotificationMsg.dataLength = 2;
    *((U32*) replyMessage.payload.pSSMonitoringDataNotificationMsg.data) = value;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSMonitoringDataNotificationMsg)
            - sizeof(replyMessage.payload.pSSMonitoringDataNotificationMsg.data) + 2;

    masterServerInstance.sendMessage(replyMessage);
}

STATUS BaseZone::enableZone(bool enable)
{
    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Standby;
    else
        m_state = E_ZoneState_On;
    return OK;
}

char BaseZone::getStatusStruct()
{
    T_ZoneStatus result;
    result.split.enabled = m_enabled;
    result.split.hasWarnings = m_hasWarnings;
    result.split.hasErrors = m_hasErrors;
    result.split.state = m_state;
    return result.full;
}

void BaseZone::sendMonitorReply(U32 dataType, char* data, int length)
{
    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_MonitorDataNotification;
    replyMessage.payload.pSSMonitoringDataNotificationMsg.cableId = m_controllerId;
    replyMessage.payload.pSSMonitoringDataNotificationMsg.zoneId = m_zoneId;
    replyMessage.payload.pSSMonitoringDataNotificationMsg.dataSpecification = dataType;
    replyMessage.payload.pSSMonitoringDataNotificationMsg.dataLength = length;
    memcpy(replyMessage.payload.pSSMonitoringDataNotificationMsg.data, data, length);
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSMonitoringDataNotificationMsg)
            - sizeof(replyMessage.payload.pSSMonitoringDataNotificationMsg.data) + length;

    masterServerInstance.sendMessage(replyMessage);
}

void BaseZone::reset()
{
    m_enabled = false;
    m_hasErrors = false;
    m_hasWarnings = false;
    m_monitorEnabled = false;
    m_state = E_ZoneState_On;
    m_zoneId = M_BASE_ZONE_UNASSIGNED_ID;
    enableZone(false);
}

void BaseZone::assignIds(unsigned char controllerId, unsigned char zoneId)
{
    m_zoneId = zoneId;
    m_controllerId = controllerId;
}

void BaseZone::sendSeqEnded(unsigned long messageId, unsigned long sn, char controllerId, char zoneId, int status)
{
    TmcMessageStruct reply;
    //TODO: Check if the board address should be hard-coded.
    reply.header.id.split.src = 0x5;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_SeqEnded;
    reply.header.length = sizeof(reply.header) + sizeof(reply.payload.pSSSeqEndedMsg);
    reply.payload.pSSSeqEndedMsg.originalRequestMsgId = 0x01050000 | messageId;
    reply.payload.pSSSeqEndedMsg.originalRequestSN = sn;
    reply.payload.pSSSeqEndedMsg.cableId = controllerId;
    reply.payload.pSSSeqEndedMsg.zoneId = zoneId;
    reply.payload.pSSSeqEndedMsg.status = status;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Seq Ended sent for message 0x%x", messageId);
    TmcMasterServer::getInstance().sendMessage(reply);
}

void BaseZone::sendError(E_TmsErrors error)
{
    TmcMessageStruct reply;
    PSSErrorNotificationMsg &payload = reply.payload.pSSErrorNotificationMsg;

    //TODO: Check if the board address should be hard-coded.
    reply.header.id.split.src = 0x5;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_ErrorNotification;
    reply.header.length = sizeof(reply.header) + sizeof(payload);
    payload.cableId = m_controllerId;
    payload.zoneId = m_zoneId;
    payload.errors = error;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "Error notification sent: controller=%d, zone=%d, error=%d", m_controllerId, m_zoneId, error);

    TmcMasterServer::getInstance().sendMessage(reply);
}

void BaseZone::sendWarning(E_TmsWarnings warning)
{
    TmcMessageStruct reply;
    PSSWarningNotificationMsg &payload = reply.payload.pSSWarningNotificationMsg;

    //TODO: Check if the board address should be hard-coded.
    reply.header.id.split.src = 0x5;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_WarningNotification;
    reply.header.length = sizeof(reply.header) + sizeof(payload);
    payload.cableId = m_controllerId;
    payload.zoneId = m_zoneId;
    payload.warnings = warning;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "Warning notification sent: controller=%d, zone=%d, warning=%d", m_controllerId, m_zoneId, warning);

    TmcMasterServer::getInstance().sendMessage(reply);
}

void BaseZone::raiseError(E_TmsErrors error, bool errorState)
{
    unsigned int lastError = m_currentErrorState;

    if (errorState)
    {
        m_hasErrors = true;
        m_currentErrorState |= error;
        if (m_currentErrorState != lastError)
            sendError(error);
    }
    else
    {
        m_currentErrorState &= ~error;
        if (m_currentErrorState == 0)
            m_hasErrors = false;
    }
}

void BaseZone::raiseWarning(E_TmsWarnings warning, bool warningState)
{
    unsigned int lastWarning = m_currentWarningState;

    if (warningState)
    {
        m_hasWarnings = true;
        m_currentWarningState |= warning;
        if (m_currentWarningState != lastWarning)
            sendWarning(warning);
    }
    else
    {
        m_currentWarningState &= ~warning;
        if (m_currentWarningState == 0)
            m_hasWarnings = false;
    }
}
