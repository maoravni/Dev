/*
 * AnalogInputZone.cpp
 *
 *  Created on: 17 Feb 2013
 *      Author: maora
 */

#include "AnalogInputZone.h"

AnalogInputZone::AnalogInputZone()
{
}

AnalogInputZone::~AnalogInputZone()
{
}

void AnalogInputZone::executeZone()
{
//    if (isMonitoringEnabled()) {
//        sendZoneStatus();
//    }
}

void AnalogInputZone::sendZoneStatus()
{
    sendMonitorReply(E_DataType_Status, getStatusStruct());
    sendMonitorReply(E_DataType_Temperature, cfgGetParam(m_mwInputId));
}

STATUS AnalogInputZone::enableZone(bool enable)
{
    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Ready;
    else
        m_state = E_ZoneState_On;
    return OK;
}


