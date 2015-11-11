/*
 * CBtcComponents.cpp
 *
 *  Created on: Mar 17, 2013
 *      Author: yossis
 */

#include "CBtc.h"
#include "CBtcComponents.h"
#include <MessageStructs.h>
#include <TmcMessageHandler.h>
#include <TmcMasterServer.h>

void CGlobalError::setError(E_TmsErrors ErrorId, bool Set)
{
    //ignored already handled
    if ((((ErrorId & m_Error) != 0) && Set) || (((ErrorId & m_Error) == 0) && !Set))
        return;

    if (Set)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "setError. start: ErrorId=%d, Set=%d ", ErrorId, Set);
    error = cfgGetFromRam(m_mwId);
    //M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "setError  Last Error=%d ", error);
    if (Set)
        error |= ErrorId;
    else
        error &= ~ErrorId;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "setError. Error=%x, State=%d ", error, Set);

    if (cfgPutParam(m_mwId, (U32) error) == OK) //update the MW
        m_Error = error;

    //report it to the host
    if (m_Btc)
        m_Btc->sendError((E_TmsErrors) error);

}

U32 CGlobalError::clearError()
{
    error = 0;
    if (cfgPutParam(m_mwId, (U32) error) == OK) //update the MW
        m_Error = error;
    return m_Error;
}

bool CGlobalError::isError(E_TmsErrors ErrorId)
{
    return ((m_Error & ErrorId) != 0);
}

U32 CGlobalError::getError()
{
    m_Error = cfgGetFromRam(m_mwId);
    return m_Error;
}

bool CEngageComponent::setCommand(BTC_COMMANDS command)
{
    return (m_Btc->setCommand(command));
}

bool CAirKnifePressureValveComponent::setCommand(BTC_COMMANDS command)
{
    return (m_Btc->setCommand(command));
}

void CAirKnifePressureValveComponent::sendZoneStatus()
{
    if (m_Btc->getZoneId() == M_BASE_ZONE_UNASSIGNED_ID)
        return;

    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;
    PSSValveStatusNotificationMsg &payload = replyMessage.payload.pSSValveStatusNotificationMsg;

    replyMessage.header.id.split.id = MSG_ValveStatusNotification;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(PSSValveStatusNotificationMsg);
    payload.cableId = m_controllerId;
    payload.deviceId = m_zoneId;

    payload.state = m_Btc->m_AirKnifePressureValveComponent.isCmndEnable() == 1 ? 100 : 0;
    payload.status = 0;

    masterServerInstance.sendMessage(replyMessage);

}

//bool CTankWaterDrainComponent::setCommand(BTC_COMMANDS command)
//{
//    return (m_Btc->setCommand(command));
//}

bool CBasePumpComponent::setCommand(bool enable, U32 Power)
{
    if (enable)
    {
        if (Power != 0 && Power != 10)
            m_Btc->m_FillPumpComponent.setPowerHighLimit(Power / 10);  //fill pump
        return (m_Btc->setCommand(m_commandEnable));
    }
    else
        return (m_Btc->setCommand(m_commandDisable));
}

void CBasePumpComponent::sendZoneStatus()
{
    if (m_Btc->getZoneId() == M_BASE_ZONE_UNASSIGNED_ID)
        return;

    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;
    PSSPumpStatusNotificationMsg &payload = replyMessage.payload.pSSPumpStatusNotificationMsg;

    replyMessage.header.id.split.id = MSG_PumpStatusNotification;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(PSSPumpStatusNotificationMsg);
    payload.cableId = m_controllerId;
    payload.deviceId = m_zoneId;

    payload.powerLimit = GetPowerLimit();
    payload.status = IsStatusOk();

    masterServerInstance.sendMessage(replyMessage);
}

void CEngageComponent::sendZoneStatus()
{
    if (m_Btc->getZoneId() == M_BASE_ZONE_UNASSIGNED_ID)
        return;

    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;
    PSSEngageSystemStatusNotificationMsg &payload = replyMessage.payload.pSSEngageSystemStatusNotificationMsg;

    replyMessage.header.id.split.id = MSG_EngageSystemStatusNotification;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(PSSEngageSystemStatusNotificationMsg);
    payload.cableId = m_controllerId;
    payload.deviceId = m_zoneId;

    payload.position = m_Btc->m_EngageState.isEngageState();   //0 disengage, 1 engage
    payload.valveState = m_Btc->m_EngageComponent.getCompEnable() ? 1 : 0;  //0 off, 1 on
    payload.sensor1State = m_Btc->m_EngageState.isState1() ? 1 : 0;
    payload.sensor2State = m_Btc->m_EngageState.isState2() ? 1 : 0;
    payload.sensor3State = m_Btc->m_EngageState.isState3() ? 1 : 0;
    payload.sensor4State = m_Btc->m_EngageState.isState4() ? 1 : 0;

    payload.status = m_Btc->m_GlobalError.isError(E_TmsErrors_EngageTimeOut) ? 1 : 0; //lsb error:  tbd while timeout

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "CEngageComponent::sendZoneStatus position=%d, valveState=%d,sensor1State=%d, sensor2State=%d,sensor3State=%d,sensor4State=%d,status=%d,",
            payload.position, payload.valveState, payload.sensor1State, payload.sensor2State, payload.sensor3State,
            payload.sensor4State, payload.status);

    masterServerInstance.sendMessage(replyMessage);
}

