/*
 * PressureSensorDevice.cpp
 *
 *  Created on: 11 Mar 2013
 *      Author: maora
 */

#include "PressureSensorDevice.h"
#include <TmcServer/TmcMasterServer.h>

PressureSensorDevice::PressureSensorDevice()
{

}

PressureSensorDevice::~PressureSensorDevice()
{
}

void PressureSensorDevice::sendZoneStatus()
{
    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct statusMsg;

    PSSPressureSensorStatusNotificationMsg &payload = statusMsg.payload.pSSPressureSensorStatusNotificationMsg;

    statusMsg.header.id.split.id = MSG_PressureSensorStatusNotification;
    payload.cableId = m_controllerId;
    payload.deviceId = m_zoneId;
    payload.pressure = cfgGetParam(m_mwInputId);
    payload.status = getStatusStruct();
    statusMsg.header.length = sizeof(statusMsg.header) + sizeof(PSSPressureSensorStatusNotificationMsg);

    masterServerInstance.sendMessage(statusMsg);
}
