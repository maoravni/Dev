/*
 * AirInletValve.cpp
 *
 *  Created on: 17 Feb 2013
 *      Author: maora
 */

#include "AirInletValve.h"

AirInletValve::AirInletValve()
{
}

AirInletValve::~AirInletValve()
{
}

STATUS AirInletValve::enableZone(bool enable)
{
    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Ready;
    else
    {
        m_state = E_ZoneState_On;
        cfgPutParam(m_mwValue, 0);
    }
    return OK;
}

void AirInletValve::executeZone()
{
//    bool AirInlatActive = true;
//
//    if (AirInlatActive != m_AirInlatActive)
//    {
//        m_AirInlatActive = AirInlatActive;
//        if (AirInlatActive)
//        {
//
//            cfgPutParam(m_mwValue, 3); //full open
//        }
//        else
//        {
//            cfgPutParam(m_mwValue, 0);
//        }
//    }
//
////    if (isMonitoringEnabled())
////        sendZoneStatus();
//
}

bool AirInletValve::configValues(U32 value)
{
    if (m_state == E_ZoneState_Ready)
    {
        if (value > 3)
            value = 3;
        cfgPutParam(m_mwValue, value);
    }

    return true;
}

bool AirInletValve::configSetPoint(int index, U32 value)
{
    if (index >= M_INLET_NUM_SETPOINTS)
        return false;

    m_setPoints[index] = value;

    return true;
}

void AirInletValve::sendZoneStatus()
{
    sendMonitorReply(E_DataType_Status, getStatusStruct());
    sendMonitorReply(E_DataType_SetPoint, cfgGetParam(m_mwValue));
}

bool AirInletValve::enableSetPoint(int setPoint)
{
    if (setPoint >= M_INLET_NUM_SETPOINTS)
        return false;

    configValues(m_setPoints[setPoint]);
    return true;
}
