/*
 * DigitalInputDevice.cpp
 *
 *  Created on: 23 Apr 2013
 *      Author: maora
 */

#include "DigitalInputDevice.h"

DigitalInputDevice::DigitalInputDevice()
{
}

DigitalInputDevice::~DigitalInputDevice()
{
}

void DigitalInputDevice::executeZone()
{
    if (cfgGetParam(m_mwInput) == 0)
        m_state = E_ZoneState_On;
    else
        m_state = E_ZoneState_Ready;
}

void DigitalInputDevice::sendZoneStatus()
{
    sendMonitorReply(E_DataType_Status, getStatusStruct());
}
