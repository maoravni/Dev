/*
 * AirBleeding.cpp
 *
 *  Created on: 17 Dec 2012
 *      Author: maora
 */

#include "AirBleedingZone.h"
#include "CfgDBUtil.h"

AirBleedingZone::AirBleedingZone()
{
}

AirBleedingZone::~AirBleedingZone()
{
}

//void AirBleedingZone::setMiddlewareIds(int instance, int index, U32 inValve, U32 outValve)
//{
//    m_mwInstance = instance;
//    m_mwIndex = index;
//    m_mwInValve = CfgDB_ID_AddIndex(inValve, index);
//    m_mwOutValve = CfgDB_ID_AddIndex(outValve, index);
//}

void AirBleedingZone::executeZone()
{
//    T_AirBleedingZoneMonitorReply reply;
//    reply.status = getStatusStruct();
//    reply.inValve = m_currentSetPoint.inValveState;
//    reply.outValve = m_currentSetPoint.outValveState;
//    sendMonitorReply(E_DataType_InValveState | E_DataType_OutValveState | E_DataType_Status, (char*)&reply, sizeof(T_AirBleedingZoneMonitorReply));
//    if (isMonitoringEnabled())
//        sendZoneStatus();
}

STATUS AirBleedingZone::enableZone(bool enable)
{
    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Ready;
    else
    {
        m_state = E_ZoneState_On;
        setValveState(1, 0);
        setValveState(2, 0);
    }

    return OK;
}

void AirBleedingZone::sendZoneStatus()
{

    sendMonitorReply(E_DataType_InValveState, m_currentSetPoint.inValveState);
    sendMonitorReply(E_DataType_OutValveState, m_currentSetPoint.outValveState);
    sendMonitorReply(E_DataType_Status, getStatusStruct());

}

bool AirBleedingZone::setValveState(int valve, int value)
{
    switch (valve)
    {
    case 1:
        m_currentSetPoint.inValveState = value;
        cfgPutParam(m_mwInValve, (value != 0));
        return true;
    case 2:
        m_currentSetPoint.outValveState = value;
        cfgPutParam(m_mwOutValve, (value != 0));
        return true;
    default:
        return false;
    }
}
