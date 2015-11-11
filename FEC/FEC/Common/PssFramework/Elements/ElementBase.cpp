/*
 * ElementBase.cpp
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#include "ElementBase.h"
#include <Tasks/UpdateSchedulerTask.h>
#include "ElementRepository.h"
#include <PscServer/PscMessageStructs.h>
#include <pscserver/PscMasterServer.h>
#include <PscSubsystem.h>

ElementBase::ElementBase()
{
    m_pssId = M_UNASSIGNED_PSS_ID;
    m_elementIndex = 0;
    m_monitoringEnabled = false;
    ElementRepository::getInstance().addElement(this);
    m_minInterval = 0;
    m_maxInterval = 0;
    m_lastNotificationTick = 0;
//    m_errorBits = 0;
//    m_warningBits = 0;
}

ElementBase::~ElementBase()
{
//    ElementRepository::getInstance().markElementAsDeleted(m_elementIndex);
}

void ElementBase::updateObservers()
{
    bool allowed = true;
    T_ObserverListIterator i;
//    for (i = m_observerList.begin(); i != m_observerList.end(); ++i)
//        allowed &= (*i)->requestValueChange(this);
    if (allowed)
    {
        for (i = m_observerList.begin(); i != m_observerList.end(); ++i)
            UpdateSchedulerTask::getInstance()->addUpdateEvent((*i), this);
    }
}

void ElementBase::addObserver(IObserver* observer)
{
    m_observerList.push_back(observer);
}

void ElementBase::removeObserver(IObserver* observer)
{
    T_ObserverListIterator it;

    if (m_observerList.size() == 0)
        return;

    for (it = m_observerList.begin(); it != m_observerList.end(); ++it)
    {
        if ((*it) == observer)
        {
            m_observerList.erase(it);
            return;
        }
    }
}

//TODO: Extract the send device status function to a single method.
void ElementBase::sendDeviceStatus(uint32_t value)
{
    // static instance for the master server.
    PscMessageStruct replyMessage;
    T_ValueTypeUnion val;
    val.u32 = value;

    replyMessage.header.id.split.id = MSG_DeviceStatusNotification;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.pssId = m_pssId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.value = val.u32;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.exceptions = hasExceptions();
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSDeviceStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void ElementBase::sendDeviceStatus(int32_t value)
{
    // static instance for the master server.
    PscMessageStruct replyMessage;
    T_ValueTypeUnion val;
    val.s32 = value;

    replyMessage.header.id.split.id = MSG_DeviceStatusNotification;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.pssId = m_pssId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.value = val.u32;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.exceptions = hasExceptions();
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSDeviceStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void ElementBase::sendDeviceStatus(uint8_t value)
{
    // static instance for the master server.
    PscMessageStruct replyMessage;
    T_ValueTypeUnion val;
    val.u32 = 0;
    val.u8 = value;

    replyMessage.header.id.split.id = MSG_DeviceStatusNotification;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.pssId = m_pssId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.value = val.u32;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.exceptions = hasExceptions();
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSDeviceStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void ElementBase::sendDeviceStatus(uint16_t value)
{
    // static instance for the master server.
    PscMessageStruct replyMessage;
    T_ValueTypeUnion val;
    val.u32 = 0;
    val.u16 = value;

    replyMessage.header.id.split.id = MSG_DeviceStatusNotification;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.pssId = m_pssId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.value = val.u32;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.exceptions = hasExceptions();
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSDeviceStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void ElementBase::sendDeviceStatus(int8_t value)
{
    // static instance for the master server.
    PscMessageStruct replyMessage;
    T_ValueTypeUnion val;
    val.u32 = 0;
    val.s8 = value;

    replyMessage.header.id.split.id = MSG_DeviceStatusNotification;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.pssId = m_pssId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.value = val.u32;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.exceptions = hasExceptions();
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSDeviceStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void ElementBase::sendDeviceStatus(int16_t value)
{
    // static instance for the master server.
    PscMessageStruct replyMessage;
    T_ValueTypeUnion val;
    val.u32 = 0;
    val.s16 = value;

    replyMessage.header.id.split.id = MSG_DeviceStatusNotification;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.pssId = m_pssId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.value = val.u32;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.exceptions = hasExceptions();
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSDeviceStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);
}

void ElementBase::sendDeviceStatus(float value)
{
    // static instance for the master server.
    PscMessageStruct replyMessage;
    T_ValueTypeUnion val;
    val.f = value;

    replyMessage.header.id.split.id = MSG_DeviceStatusNotification;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.pssId = m_pssId;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.value = val.u32;
    replyMessage.payload.pSSDeviceStatusNotificationMsg.exceptions = hasExceptions();
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSDeviceStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);
}

//void ElementBase::updateErrorsBits(E_PSSErrors error, bool state)
//{
//    uint32_t previousErrorBits = m_errorBits;
//
//    if (state)
//        m_errorBits |= error;
//    else
//        m_errorBits &= ~error;
//
//    if (m_errorBits != 0)
//        m_sta
//    if (m_errorBits != previousErrorBits && m_pssId != 0)
//        PscMasterServer::getInstance().sendError(Psc_ControllerId, m_pssId, m_errorBits);
//}
//
//void ElementBase::updateWarningBits(E_PSSErrors warning, bool state)
//{
//    uint32_t previousWarningBits = m_errorBits;
//
//    if (state)Rva
//        m_warningBits |= warning;
//    else
//        m_warningBits &= ~warning;
//
//    if (m_warningBits != previousWarningBits)
//        PscMasterServer::getInstance().sendWarning(Psc_ControllerId, m_pssId, m_warningBits);
//}

bool ElementBase::checkIfCanSendUpdate()
{
    portTickType currentTicks = xTaskGetTickCount();
    portTickType minInternal = getMinInterval();

    // if no minimum interval is set, we can update.
    if (minInternal == 0)
    {
        return true;
    }

    // if the minimum interval hasn't elapsed yet, return false.
    if ((currentTicks - m_lastNotificationTick) < minInternal)
        return false;

    // if we're here it means the minimum interval has elapsed, so update.
    m_lastNotificationTick = currentTicks;
    return true;
}

void ElementBase::sendError(uint16_t cableId, uint16_t pssId, uint32_t errors)
{
    PscMasterServer::getInstance().sendError(cableId, pssId, 0, errors);
}

void ElementBase::sendWarning(uint16_t cableId, uint16_t pssId, uint32_t warnings)
{
    PscMasterServer::getInstance().sendWarning(cableId, pssId, 0, warnings);
}
