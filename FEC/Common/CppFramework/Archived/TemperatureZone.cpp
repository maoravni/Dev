/*
 * TemperatureZone.cpp
 *
 *  Created on: 5 Dec 2012
 *      Author: maora
 */

#include "TemperatureZone.h"
#include "CfgDB.h"
#include "logger.h"
#include "TmcMasterServer.h"

// TODO: fix the data type and the data length.
// `
TemperatureZone::TemperatureZone()
{
//    m_currentSetPoint.highRange = 0;
//    m_currentSetPoint.lowRange = 0;
//    m_currentSetPoint.highWarningRange = 0;
//    m_currentSetPoint.lowWarningRange = 0;
//    m_currentSetPoint.temperature = 0;
    m_enabled = false;

}

TemperatureZone::~TemperatureZone()
{
}

U32 TemperatureZone::readZone(U32 &value)
{
    // TODO: implement a simulation mode for the readout of temperatures.
    STATUS status;
    short temperature;

    // read the temperature value from the database.
    status = cfgGetParam(m_mwReadValueId, value);
    temperature = value;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "Temperature Zone controller=%d zone=%d status=%d state=%d value=%d", m_controllerId, m_zoneId, status, m_state, value);

    if (status != OK)
        return status;

    m_hasWarnings = false;
    m_hasErrors = false;
    // if we haven't reached the set point yet, we don't check for errors/warning.
    // when we have reached the working zone, we set the var to TRUE, and then we check for
    // errors/warning.
    return status;
}

STATUS TemperatureZone::enableZone(bool enable)
{
    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Ready;
    else
        m_state = E_ZoneState_On;

    return OK;
}

void TemperatureZone::executeZone()
{
    U32 value;
    U32 status;

//    if (isMonitoringEnabled())
//        sendZoneStatus();

}

void TemperatureZone::setMiddlewareIds(int instance, int index, U32 readValue)
{
    m_mwReadValueId = CfgDB_ID_AddIndex(readValue, index);
}

void TemperatureZone::sendZoneStatus()
{
    T_TemperatureZoneMonitorReply monitorReply;
    U32 value;
    readZone(value);
    monitorReply.status = getStatusStruct();
    monitorReply.temperature = value;
    sendMonitorReply(E_DataType_Status | E_DataType_Temperature, (char*) &monitorReply,
            sizeof(T_TemperatureZoneMonitorReply));
}
