/*
 * FanZone.cpp
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#include "FanZone.h"

FanZone::FanZone()
{
    m_fanActive = false;
}

FanZone::~FanZone()
{
}

void FanZone::addHeatExchangeZone(HeatExchangeZone* zone)
{
    m_heatExchangeVec.push_back(zone);
}

void FanZone::executeZone()
{
    if (m_state == E_ZoneState_Moving2Ready || m_state == E_ZoneState_Ready)
    {
        bool fanActive = true;
        for (int i = 0; i < m_heatExchangeVec.size(); ++i)
            fanActive &= m_heatExchangeVec[i]->isBlowerActive();

        if (fanActive != m_fanActive)
        {
            m_fanActive = fanActive;
            cfgPutParam(m_mwEnable, fanActive);
        }
    }
//    if (isMonitoringEnabled())
//    {
//        sendZoneStatus();
//    }
}

STATUS FanZone::enableZone(bool enable)
{
    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Ready;
    else
    {
        m_state = E_ZoneState_On;
        m_fanActive = false;
        cfgPutParam(m_mwEnable, 0);
    }
    return OK;
}

void FanZone::sendZoneStatus()
{
    sendMonitorReply(E_DataType_Status, getStatusStruct());
}
