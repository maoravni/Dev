/*
 * AirBlowerZone.cpp
 *
 *  Created on: 17 Dec 2012
 *      Author: maora
 */

#include "AirBlowerZone.h"
#include "CfgDBUtil.h"

AirBlowerZone::AirBlowerZone()
{
    m_blowerActive = false;

}

AirBlowerZone::~AirBlowerZone()
{
}

void AirBlowerZone::setMiddlewareIds(int instance, int index, U32 setPoint)
{
    m_mwSetPoint = CfgDB_ID_AddIndex(setPoint, index);
}

void AirBlowerZone::executeZone()
{
    bool blowerActive = true;
    for (int i = 0; i < m_heatExchangeVec.size(); ++i)
        blowerActive &= m_heatExchangeVec[i]->isBlowerActive();

    if (cfgGetParam(m_mwEnable))
    {
        if (blowerActive != m_blowerActive)
        {
            m_blowerActive = blowerActive;
            if (blowerActive)
            {
                //enableZone(true);
                cfgPutParam(m_mwSetPoint, m_currentSetPoint);
            }
            else
            {
                cfgPutParam(m_mwSetPoint, 0);
            }
        }
    }

//    if (isMonitoringEnabled())
//    {
//        sendZoneStatus();
//    }
}

bool AirBlowerZone::configSetPoint(int setPoint, short speed)
{
    if (setPoint >= M_BLOWER_ZONE_NUMBER_OF_SET_POINTS)
        return false;

    m_setPoints[setPoint] = speed;

    return true;
}

bool AirBlowerZone::enableSetPoint(short setPoint)
{
    if (setPoint >= M_BLOWER_ZONE_NUMBER_OF_SET_POINTS)
        return false;

    return configValues(m_setPoints[setPoint]);
}

bool AirBlowerZone::configValues(short speed)
{
    m_currentSetPoint = speed;

    cfgPutParam(m_mwSetPoint, m_currentSetPoint);

    m_state = E_ZoneState_Ready;

    return true;
}

STATUS AirBlowerZone::enableZone(bool enable)
{
    cfgPutParam(m_mwEnable, enable);
    cfgPutParam(m_mwSetPoint, 0);

    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Ready;
    else
        m_state = E_ZoneState_On;
    return OK;
}

void AirBlowerZone::sendZoneStatus()
{
    sendMonitorReply(E_DataType_BlowerSpeed, m_currentSetPoint);
    sendMonitorReply(E_DataType_Power, m_currentSetPoint*0.1);
    sendMonitorReply(E_DataType_Status, getStatusStruct());
}

void AirBlowerZone::addHeatExchangeZone(HeatExchangeZone* zone)
{
    m_heatExchangeVec.push_back(zone);
}
