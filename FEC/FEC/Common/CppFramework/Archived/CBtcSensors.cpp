/*
 * CBtcSensors.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: yossis
 */
#include "CBtc.h"
#include "CBtcSensors.h"
#include <MessageStructs.h>
#include <TmcMessageHandler.h>
#include <TmcMasterServer.h>

void CTankLevelSens::sendZoneStatus()
{
    if (m_Btc->getZoneId() == M_BASE_ZONE_UNASSIGNED_ID)
        return;

    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;
    PSSTankLevelStatusNotificationMsg &payload = replyMessage.payload.pSSTankLevelStatusNotificationMsg;

    replyMessage.header.id.split.id = MSG_TankLevelStatusNotification;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(PSSTankLevelStatusNotificationMsg);
    payload.cableId = m_controllerId;
    payload.deviceId = m_zoneId;

    bool Lowlevel = m_Btc->m_WaterTankLowLevelState.GetState();
    bool Midlevel = m_Btc->m_WaterTankUpState.GetState();
    bool Overflowlevel = m_Btc->m_WaterTankOverflowState.GetState();

    int TankLevel=0;
    bool TankSensorsError=false;

    if ((!Lowlevel && !Midlevel && Overflowlevel) ||
            (!Lowlevel && Midlevel && !Overflowlevel) ||
            (!Lowlevel && Midlevel && Overflowlevel) ||
            (Lowlevel && !Midlevel && Overflowlevel))
        TankSensorsError=true;

    if (Overflowlevel)
        TankLevel = 3; //high
    else if (!Lowlevel && !Midlevel && !Overflowlevel)
        TankLevel = 0; //empty
    else if (Lowlevel && !Midlevel && !Overflowlevel)
        TankLevel = 1; //low
    else if (Lowlevel && Midlevel && !Overflowlevel)
            TankLevel = 2; //medium

    payload.tankLevel = TankLevel;
    payload.highLevelSensor = !Overflowlevel;  //mark red color for overflow
    payload.midLevelSensor = Midlevel;
    payload.lowLevelSensor = Lowlevel;

    if (TankSensorsError)
        m_Btc->m_GlobalError.setError(E_TmsErrors_SensorMalfunction ,true);

    payload.status = Overflowlevel;  //error report about overflow

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "CTankLevelSens::sendZoneStatus tankLevel=%d, highLevelSensor=%d,midLevelSensor=%d, lowLevelSensor=%d,status=%d, ",
            payload.tankLevel,payload.highLevelSensor,payload.midLevelSensor,payload.lowLevelSensor,payload.status);

    masterServerInstance.sendMessage(replyMessage);
}

void CSensState::sendZoneStatus()
{
    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;
    PSSDigitalSensorStatusNotificationMsg &payload = replyMessage.payload.pSSDigitalSensorStatusNotificationMsg;

    replyMessage.header.id.split.id = MSG_DigitalSensorStatusNotification;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(PSSDigitalSensorStatusNotificationMsg);
    payload.cableId = m_controllerId;
    payload.deviceId = m_zoneId;

    payload.state = GetOkState();
    payload.status = 0;  //lsb error:  tbd while timeout;

    masterServerInstance.sendMessage(replyMessage);
}

void CTankLevelSens::executeZone()
{
//    if (getZoneId() != M_BASE_ZONE_UNASSIGNED_ID || m_Btc->getZoneId() != M_BASE_ZONE_UNASSIGNED_ID)
//        sendZoneStatus();
}

