/*
 * HeaterZone.cpp
 *
 *  Created on: 13 Dec 2012
 *      Author: maora
 */

#include "HeaterZone.h"
#include "CfgDBUtil.h"
#include "CfgDB.h"
#include "TmcMasterServer.h"
#include "logger.h"

HeaterZone::HeaterZone()
{
    m_currentSetPoint.highRange = 0;
    m_currentSetPoint.lowRange = 0;
    m_currentSetPoint.highWarningRange = 0;
    m_currentSetPoint.lowWarningRange = 0;
    m_currentSetPoint.temperature = 0;

    m_enabled = false;

    m_spWasSet = false;
}

HeaterZone::~HeaterZone()
{
}

STATUS HeaterZone::enableZone(bool enable)
{
    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Standby;
    else
    {
        cfgPutParam(m_mwEnableId, 0);
        m_state = E_ZoneState_On;
    }

    // Maor {9 Jan 2013}: When enabling a zone, we should not enable it in the MW,
    // since we don't want it to start heating.
    //    U32 enableVal = enable;
    //    return cfgPutParam(m_mwEnableId, enableVal);=
    return OK;
}

bool HeaterZone::configValues(unsigned short temperature, short lowWarn, short lowRange, short highRange,
        short highWarn, U32 sn)
{
    m_currentSetPoint.temperature = temperature;
    m_currentSetPoint.lowWarningRange = lowWarn + temperature;
    m_currentSetPoint.lowRange = lowRange + temperature;
    m_currentSetPoint.highRange = highRange + temperature;
    m_currentSetPoint.highWarningRange = highWarn + temperature;

    STATUS status;
    U32 value = m_currentSetPoint.temperature;
    status = cfgPutParam(m_mwSetPoint, value);

    // if the zone is in the On state, it should not start
    if (m_state == E_ZoneState_On)
        return false;

    m_spWasSet = true;
    m_lastSn = sn;

    m_state = E_ZoneState_Moving2Ready;

    // enable the zone
    value = 1;
    status = cfgPutParam(m_mwEnableId, value);

    return true;
}

void HeaterZone::setMiddlewareIds(int instance, int index, U32 enable, U32 readValue, U32 setPoint, U32 mv, U32 PumaMv,
        U32 pidP, U32 pidI, U32 pidD, U32 powerLimit, U32 autoTune)
{
    m_mwEnableId = CfgDB_ID_AddIndex(enable, index);
    m_mwTempId = CfgDB_ID_AddIndex(readValue, index);
    m_mwSetPoint = CfgDB_ID_AddIndex(setPoint, index);
    m_mwMv = CfgDB_ID_AddIndex(mv, index);
    m_mwPidP = CfgDB_ID_AddIndex(pidP, index);
    m_mwPidI = CfgDB_ID_AddIndex(pidI, index);
    m_mwPidD = CfgDB_ID_AddIndex(pidD, index);
    m_mwPowerLimit = CfgDB_ID_AddIndex(powerLimit, index);
    m_mwAutoTune = CfgDB_ID_AddIndex(autoTune, index);
}

void HeaterZone::executeZone()
{
    U32 value;
    U32 status;
//    T_HeaterMonitorReplyStruct monitorReply;

    executeTemperatureReadout(m_mwTempId, value);

    // read status
//    if (isMonitoringEnabled())
//    {
//        sendZoneStatus();
//    }
}

STATUS HeaterZone::setPowerLimit(unsigned short powerLimit)
{
    U32 status;
    U32 value = powerLimit;

    return cfgPutParam(m_mwPowerLimit, value);
}

bool HeaterZone::configSetPoint(short setPoint, unsigned short temperature, short lowWarn, short lowRange,
        short highRange, short highWarn)
{
    if (setPoint >= M_TEMPERATURE_ZONE_NUMBER_OF_SET_POINTS)
        return false;

    m_setPoints[setPoint].temperature = temperature;
    m_setPoints[setPoint].lowWarningRange = lowWarn;
    m_setPoints[setPoint].lowRange = lowRange;
    m_setPoints[setPoint].highRange = highRange;
    m_setPoints[setPoint].highWarningRange = highWarn;

    return true;
}

bool HeaterZone::enableSetPoint(short setPoint)
{
    if (setPoint >= M_TEMPERATURE_ZONE_NUMBER_OF_SET_POINTS)
        return false;

    return configValues(m_setPoints[setPoint].temperature, m_setPoints[setPoint].lowWarningRange,
            m_setPoints[setPoint].lowRange, m_setPoints[setPoint].highRange, m_setPoints[setPoint].highWarningRange, 0);
}

U32 HeaterZone::executeTemperatureReadout(U32 mwid, U32& value)
{
    // TODO: implement a simulation mode for the readout of temperatures.
    STATUS status;
    short temperature;

    // read the temperature value from the database.
    status = cfgGetParam(mwid, value);
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
    switch (m_state)
    {
    case E_ZoneState_On:
    case E_ZoneState_Standby:
        break;
    case E_ZoneState_Moving2Ready:
        // TODO: add a timeout period for reaching a set point.
        // TODO: add a timeout for reaching a set point, before the state changes to ready.
        if (temperature >= m_currentSetPoint.lowRange && temperature <= m_currentSetPoint.highRange)
        {
            m_state = E_ZoneState_Ready;
            if (m_spWasSet && m_lastSn != 0)
                sendSeqEnded(0x13, m_lastSn, m_controllerId, m_zoneId, E_SeqEndedStatus_Success);
        }
        break;
    case E_ZoneState_Ready:
        if (temperature < m_currentSetPoint.lowWarningRange || temperature > m_currentSetPoint.highWarningRange)
        {
            if (temperature < m_currentSetPoint.lowWarningRange)
                raiseError(E_TmsErrors_TempExceedsLimit, true);
        }
        else
        {
            if (m_hasErrors)
                raiseError(E_TmsErrors_TempExceedsLimit, false);
            if (temperature < m_currentSetPoint.lowRange || temperature > m_currentSetPoint.highRange)
            {
                if (temperature < m_currentSetPoint.lowRange)
                    raiseWarning(E_TmsWarnings_TempExceedsLimit, true);
            }
            else
            {
                if (m_hasWarnings)
                    raiseWarning(E_TmsWarnings_TempExceedsLimit, false);
            }
        }
        break;
    }

    m_lastTemperatureRead = value;

    return status;
}

STATUS HeaterZone::setMaxTemperature(unsigned short maxTemperature)
{
    return OK;
}

void HeaterZone::sendZoneStatus()
{
    sendMonitorReply(E_DataType_Power, cfgGetParam(m_mwMv));
    sendMonitorReply(E_DataType_SetPoint, cfgGetParam(m_mwSetPoint));

    sendMonitorReply(E_DataType_Temperature, m_lastTemperatureRead);

    sendMonitorReply(E_DataType_Status, getStatusStruct());
    //sendMonitorReply(E_DataType_Status | E_DataType_Power | E_DataType_Temperature, (char*)&monitorReply, sizeof(T_HeaterMonitorReplyStruct));
}

void HeaterZone::configPidValues(float kp, float ki, float kd, short setpointSmoothing, float iThreshold, unsigned short outputSmoothing)
{
    cfgPutParam(m_mwPidP, kp);
    cfgPutParam(m_mwPidI, ki);
    cfgPutParam(m_mwPidD, kd);
}

E_AckStatus HeaterZone::startAutoTune(U16 setpoint, U16 overshoot, U16 powerStart, U16 powerStep, U16 lookback, bool reverseDirection, U32 sn)
{
    if (m_state == E_ZoneState_On)
        return E_AckStatus_InvalidState;
    cfgPutParam(m_mwAutoTune, 1);
    return E_AckStatus_Success;
}

void HeaterZone::sendPidValues()
{
    TmcMessageStruct reply;
    PSSPIDValuesNotificationMsg *payload = &reply.payload.pSSPIDValuesNotificationMsg;

    //TODO: Check if the board address should be hard-coded.
    reply.header.id.split.src = 0x5;
    reply.header.id.split.dst = 0x1;
    reply.header.id.split.id = MSG_PIDValuesNotification;
    reply.header.length = sizeof(reply.header) + sizeof(*payload);
    payload->cableId = m_controllerId;
    payload->zoneId = m_zoneId;
    payload->p = cfgGetParam(m_mwPidP);
    payload->i = cfgGetParam(m_mwPidI);
    payload->d = cfgGetParam(m_mwPidD);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "Pid Values Sent: controller=%d, zone=%d, PID=%f, %f, %f", m_controllerId, m_zoneId, payload->p, payload->i, payload->d);

    TmcMasterServer::getInstance().sendMessage(reply);
}
