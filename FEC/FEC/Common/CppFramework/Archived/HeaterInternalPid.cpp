/*
 * HeaterInternalPid.cpp
 *
 *  Created on: 21 Apr 2013
 *      Author: maora
 */

#include "HeaterInternalPid.h"
#include "CfgDBUtil.h"
#include "CfgDB.h"
#include "logger.h"
#include <TmcServer/TmcMessageHandler.h>
#include <TmcServer/TmcMasterServer.h>
#include <TemperatureCfgDB.h>

HeaterInternalPid::HeaterInternalPid()
{
    m_currentSetPoint.highRange = 0;
    m_currentSetPoint.lowRange = 0;
    m_currentSetPoint.highWarningRange = 0;
    m_currentSetPoint.lowWarningRange = 0;
    m_currentSetPoint.temperature = 0;
    setMaxTemperature(10000);

    m_enabled = false;

    m_spWasSet = false;
    m_safetyActivated = false;

    m_pPidAutoTune = NULL;
}

HeaterInternalPid::~HeaterInternalPid()
{
}

STATUS HeaterInternalPid::enableZone(bool enable)
{
    m_enabled = enable;
    if (enable)
        m_state = E_ZoneState_Standby;
    else
    {
        cfgPutParam(m_mwEnableId, 0);
        setOutputPower(0);
        m_pidCalculator.setEnabled(false);
        m_pidCalculator.SetAutoMode(false);
        if (m_isInAutoTune)
        {
            cancelAutoTune();
        }
        m_state = E_ZoneState_On;
    }

    // Maor {9 Jan 2013}: When enabling a zone, we should not enable it in the MW,
    // since we don't want it to start heating.
    //    U32 enableVal = enable;
    //    return cfgPutParam(m_mwEnableId, enableVal);=
    return OK;
}

bool HeaterInternalPid::configValues(unsigned short temperature, short lowWarn, short lowRange, short highRange,
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
    if (m_mwEnableId != 0)
        status = cfgPutParam(m_mwEnableId, value);
    m_pidCalculator.setEnabled(true);

    m_pidCalculator.SetAutoMode(true);
    m_pidCalculator.SetSetPoint(temperature * 0.1);

    return true;
}

//void HeaterInternalPid::setMiddlewareIds(int instance, int index, U32 enable, U32 readValue, U32 setPoint, U32 mv,
//        U32 PumaMv, U32 pidP, U32 pidI, U32 pidD, U32 powerLimit)
//{
//    m_mwEnableId = CfgDB_ID_AddIndex(enable, index);
//    m_mwTempId = CfgDB_ID_AddIndex(readValue, index);
//    m_mwSetPoint = CfgDB_ID_AddIndex(setPoint, index);
//    m_mwMv = CfgDB_ID_AddIndex(mv, index);
//    m_mwPidP = CfgDB_ID_AddIndex(pidP, index);
//    m_mwPidI = CfgDB_ID_AddIndex(pidI, index);
//    m_mwPidD = CfgDB_ID_AddIndex(pidD, index);
//    m_mwPowerLimit = CfgDB_ID_AddIndex(powerLimit, index);
//    //m_mwAutoTune = CfgDB_ID_AddIndex(autoTune, index);
//}

void HeaterInternalPid::executeZone()
{
    U32 value;
    U32 status;

    executeTemperatureReadout(m_mwTempId, value);

    U32 output;
    if (m_isInAutoTune)
    {
        output = m_pPidAutoTune->Compute(m_lastTemperatureRead * 0.1) * 10;
        if (!m_pPidAutoTune->isRunning())
        {
            m_isInAutoTune = false;
            m_pidCalculator.SetTunings(m_pPidAutoTune->GetKp(), m_pPidAutoTune->GetKi(), m_pPidAutoTune->GetKd());
//            printf("p: %f, i: %f, d: %f\n", m_pidAutoTune.GetKp(), m_pidAutoTune.GetKi(), m_pidAutoTune.GetKd());
            sendSeqEnded(MSG_AutoTune, m_autoTuneSn, m_controllerId, m_zoneId, E_AckStatus_Success);
            sendPidValues();
            delete m_pPidAutoTune;
            m_pPidAutoTune = NULL;
        }
    }
    else
        output = m_pidCalculator.Compute(m_lastTemperatureRead * 0.1) * 10;

    // check for max temperature on safety zones:
    int vecSize = m_mwSafetyTempIdVec.size();
    int i;

    // TODO: Extract method.
    if (m_state >= E_ZoneState_Moving2Ready)
    {
        bool maxLimitReached = false;
        bool underMinLimit = true;
        for (i = 0; i < vecSize; ++i)
        {
            m_safetyTempVec[i] = cfgGetParam(m_mwSafetyTempIdVec[i]);
            if (m_safetyTempVec[i] >= SENSOR_NOT_CONNECTED_MARK)
                m_safetyTempVec[i] = 0;
            maxLimitReached |= (m_safetyTempVec[i] >= m_safetyTempUpperLimit);
            underMinLimit &= (m_safetyTempVec[i] < m_safetyTempLowerLimit);
        }

        if (maxLimitReached == true)
        {
//        if (m_safetyActivated == false)
//        {
//            m_hasErrors = true;
//            sendError(E_TmsErrors_TempExceedsLimit);
//        }
            raiseError(E_TmsErrors_HeaterTempExceedsLimit, true);
            m_safetyActivated = true;
        }
        if (underMinLimit == true)
        {
//        m_hasErrors = false;
            raiseError(E_TmsErrors_HeaterTempExceedsLimit, false);
            m_safetyActivated = false;
        }

        if (m_safetyActivated)
            setOutputPower(0);
        else
            setOutputPower(output);
    }
}

STATUS HeaterInternalPid::setPowerLimit(unsigned short powerLimit)
{
    U32 status;
    U32 value = powerLimit;

    m_pidCalculator.SetOutputLimits(0, powerLimit * 0.1);

    return cfgPutParam(m_mwPowerLimit, value);
}

bool HeaterInternalPid::configSetPoint(short setPoint, unsigned short temperature, short lowWarn, short lowRange,
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

bool HeaterInternalPid::enableSetPoint(short setPoint)
{
    if (setPoint >= M_TEMPERATURE_ZONE_NUMBER_OF_SET_POINTS)
        return false;

    return configValues(m_setPoints[setPoint].temperature, m_setPoints[setPoint].lowWarningRange,
            m_setPoints[setPoint].lowRange, m_setPoints[setPoint].highRange, m_setPoints[setPoint].highWarningRange, 0);
}

U32 HeaterInternalPid::executeTemperatureReadout(U32 mwid, U32& value)
{
    // TODO: implement a simulation mode for the readout of temperatures.
    STATUS status;
    short temperature;

    // read the temperature value from the database.
    if (mwid <= 12)
        value = cfgGetFromSource(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, mwid));
    else
        value = cfgGetParam(mwid);

    temperature = value;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Temperature Zone controller=%d zone=%d status=%d state=%d value=%d",
            m_controllerId, m_zoneId, status, m_state, value);

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
            raiseError(E_TmsErrors_TempExceedsLimit, true);
        }
        else
        {
            if (m_hasErrors)
                raiseError(E_TmsErrors_TempExceedsLimit, false);
            if (temperature < m_currentSetPoint.lowRange || temperature > m_currentSetPoint.highRange)
            {
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

STATUS HeaterInternalPid::setMaxTemperature(unsigned short maxTemperature)
{
    m_safetyTempUpperLimit = maxTemperature;
    m_safetyTempLowerLimit = maxTemperature * 0.9;
}

void HeaterInternalPid::sendZoneStatus()
{
    sendMonitorReply(E_DataType_Power, cfgGetParam(m_mwMv));
    sendMonitorReply(E_DataType_SetPoint, cfgGetParam(m_mwSetPoint));

    sendMonitorReply(E_DataType_Temperature, m_lastTemperatureRead);

    sendMonitorReply(E_DataType_Status, getStatusStruct());
    //sendMonitorReply(E_DataType_Status | E_DataType_Power | E_DataType_Temperature, (char*)&monitorReply, sizeof(T_HeaterMonitorReplyStruct));
}

//void HeaterInternalPid::setPidValues(float kp, float ki, float kd)
//{
//    m_pidCalculator.SetTunings(kp, ki, kd);
//}

E_AckStatus HeaterInternalPid::startAutoTune(U16 setpoint, U16 overshoot, U16 powerStart, U16 powerStep, U16 lookback,
        bool reverseDirection, U32 sn)
{
    if (m_state == E_ZoneState_On)
        return E_AckStatus_InvalidState;

    if (m_isInAutoTune)
        cancelAutoTune();

    m_pPidAutoTune = new PID_ATune(E_PID_ControlType_PID, lookback, setpoint * 0.1, overshoot * 0.1, powerStart * 0.1,
            powerStep * 0.1);
    if (m_pPidAutoTune == NULL)
    {
        cancelAutoTune();
        return E_AckStatus_InvalidDevice;
    }

    m_autoTuneSn = sn;
    m_isInAutoTune = true;
    configValues(setpoint, -10, -5, +5, +10, 0);
    return E_AckStatus_Success;
}

float HeaterInternalPid::getKp()
{
    return m_pidCalculator.getKp();
}

float HeaterInternalPid::getKi()
{
    return 1 / m_pidCalculator.getKi();
}

float HeaterInternalPid::getKd()
{
    return m_pidCalculator.getKd();
}

void HeaterInternalPid::sendPidValues()
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
    payload->p = getKp();
    payload->i = getKi();
    payload->d = getKd();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Pid Values Sent: controller=%d, zone=%d, PID=%f, %f, %f", m_controllerId,
            m_zoneId, payload->p, payload->i, payload->d);

    TmcMasterServer::getInstance().sendMessage(reply);
}

void HeaterInternalPid::configPidValues(float kp, float ki, float kd, short setpointSmoothing, float iThreshold, unsigned short outputSmoothing)
{
    m_pidCalculator.SetTunings(kp, 1 / ki, kd);
    m_pidCalculator.setSetpointSmoothingThreshold(setpointSmoothing*0.1);
    m_pidCalculator.setKiErrorThreshold(iThreshold);
    m_pidCalculator.setOutputSmoothingWindow(outputSmoothing);
}

void HeaterInternalPid::cancelAutoTune()
{
    if (m_pPidAutoTune != NULL)
    {
        m_pPidAutoTune->Cancel();
        delete m_pPidAutoTune;
        m_pPidAutoTune = NULL;
    }

    if (m_isInAutoTune)
    {
        m_isInAutoTune = false;
        sendSeqEnded(MSG_AutoTune, m_autoTuneSn, m_controllerId, m_zoneId, E_AckStatus_Canceled);
    }
}

void HeaterInternalPid::configSampleInterval(unsigned int interval)
{
    m_pidCalculator.SetSampleTime(interval);
}

void HeaterInternalPid::setOutputPower(int power)
{
    cfgPutParam(m_mwMv, power);
}

void HeaterInternalPid::addMwHeaterId(U32 mwHeaterId)
{
    // add the ID to the vector
    m_mwSafetyTempIdVec.push_back(mwHeaterId);

    // add a temp value to the vector.
    m_safetyTempVec.push_back(0);
}
