/*
 * HeatExchangeZone.cpp
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#include "HeatExchangeZone.h"
#include <TemperatureCfgDB.h>

//heaters protection values
#define M_MAX_HEATERS_TEMP 5000 //!< The temperature the heaters should get to before turning them off.
#define M_MIN_HEATERS_TEMP 4900 //!< The temperature the heaters should get down to before turning them on.
HeatExchangeZone::HeatExchangeZone()
{
    m_heatersActive = false;
    m_blowerActive = false;
}

HeatExchangeZone::~HeatExchangeZone()
{
}

void HeatExchangeZone::addMwHeaterId(U32 mwHeaterId)
{
    // add the ID to the vector
    m_mwHeaterIdVec.push_back(mwHeaterId);

    // add a temp value to the vector.
    m_tempVec.push_back(0);
}

bool HeatExchangeZone::configValues(unsigned short temperature, short lowWarn, short lowRange, short highRange,
        short highWarn, U32 sn)
{
    bool status = HeaterZone::configValues(temperature, lowWarn, lowRange, highRange, highWarn, sn);

    if (status)
        cfgPutParam(m_mwEnableId, 1);

    m_blowerActive = false;
    return status;
}

void HeatExchangeZone::activateHeaters(U32 Mv)
{
//    U32 powerLimit;
//    U32 status = cfgGetParam(m_mwPowerLimit, powerLimit); //get the power limit setting

    if (Mv != 0)
    {
        cfgPutParam(m_mwMv, Mv); //set the MV (heater power) upon Power limit value
        m_heatersActive = true;
    }
    else
    {
        cfgPutParam(m_mwMv, 0);  //turn it off
        m_heatersActive = false;
    }
}

STATUS HeatExchangeZone::enableZone(bool enable)
{
    m_enabled = enable;

    if (enable)
    {
        m_state = E_ZoneState_Standby;
    }
    else
    {  //disable zone -> switch to state on and disable the zone
        m_state = E_ZoneState_On;
        cfgPutParam(m_mwEnableId, enable);
        activateHeaters(0);
    }

    return OK;
}

void HeatExchangeZone::executeHeaters(U32 htxTemp, U32 spHigh, U32 spLow)
{
    if (htxTemp >= spHigh)
    {
        m_blowerActive = true;
        activateHeaters(0);
    }
    else if (htxTemp < spLow)
    {
        activateHeaters(cfgGetParam(m_mwPowerLimit));
    }
}

void HeatExchangeZone::executeZone()
{
    U32 avgTemp, htxTemp, spHtx, mv, PumaMv, pidPHtx;
    U32 spHeatersHigh, spHeatersLow, spHeaters, pidPHeaters;
    U32 spDiv, i, vecSize;
    bool enabled;
    //   U32 sum, avgCount;

    STATUS status;
    enabled = cfgGetParam(m_mwEnableId);

    vecSize = m_mwHeaterIdVec.size();

    if (enabled || isMonitoringEnabled())
    {
        for (i = 0; i < vecSize; ++i)
            m_tempVec[i] = cfgGetParam(m_mwHeaterIdVec[i]);
        executeTemperatureReadout(m_mwHeatExchangeId, htxTemp);
        spHtx = cfgGetParam(m_mwSetPoint);
        mv = cfgGetParam(m_mwMv);
        pidPHtx = cfgGetParam(m_mwPidPHtx);
    }

    if (enabled)
    {

        if (m_mwSetPointHeaters == 0)
        {
            spHeatersHigh = M_MAX_HEATERS_TEMP;
            spHeatersLow = M_MIN_HEATERS_TEMP;
        }
        else
        {
            pidPHeaters = cfgGetParam(m_mwPidP);
            spHeaters = cfgGetParam(m_mwSetPointHeaters);
            spDiv = (spHeaters * pidPHeaters) / 1000;
            spHeatersHigh = spHeaters + spDiv;
            spHeatersLow = spHeaters - spDiv;
        }

        spDiv = (spHtx * pidPHtx) / 1000;
        U32 spHigh = spHtx + spDiv;
        U32 spLow = spHtx - spDiv;

        if ((int) spLow < 0) //protect from negative
            spLow = 0;  //min value

        // check the set-point stored in the middleware, and if its different than the current re-apply:
        if (spHtx != m_currentSetPoint.temperature)
        {
            //configValues(spHtx, spHtx - 10, spHtx - 5, spHtx + 5, spHtx + 10, 0);
            m_currentSetPoint.temperature = spHtx;
            m_currentSetPoint.highRange = spHtx + 5;
            m_currentSetPoint.highWarningRange = spHtx + 10;
            m_currentSetPoint.lowWarningRange = spHtx - 10;
            m_currentSetPoint.lowRange = spHtx - 5;
        }

        // if a temperature returns SENSOR_NOT_CONNECTED_MARK it means its not connected, so we should
        // not use it in the calculations.
        for (i = 0; i < vecSize; ++i)
        {
            if (m_tempVec[i] >= SENSOR_NOT_CONNECTED_MARK)
                m_tempVec[i] = 0;
        }

        // check for heater protection : in case one of the heater exceed temperature, turn off the heaters and turn on the safety mode
        bool heaterInSafety = false;
        for (i = 0; i < vecSize; ++i)
            heaterInSafety |= (m_tempVec[i] >= spHeatersHigh);

        if (heaterInSafety)
            activateHeaters(0);

        //if all the heaters below temperature, release the safety mode
        bool heaterUnderSafety = true;
        for (i = 0; i < vecSize; ++i)
            heaterUnderSafety &= (m_tempVec[i] < spHeatersLow);

        //only during none safey mode control the temperature of the HTX
        if (heaterUnderSafety)
        {
            executeHeaters(htxTemp, spHigh, spLow);
        }

        // if there's a blower for each IBD zone, the blower ID will be != 0. Then we will
        // turn on the blower.
        // if we have several IBDs for each blower, then the blower ID will be 0, so we don't do anything here.
//        if (m_mwBlowerId != 0 && m_blowerActive)
//            cfgPutParam(m_mwBlowerId, 1);
    }
    else
    { //IBD is turned off
        if (m_heatersActive)
        {
            activateHeaters(0);
        }
    }

//    if (isMonitoringEnabled())
//    {
//        sendZoneStatus();
//    }

}

void HeatExchangeZone::sendZoneStatus()
{
    sendMonitorReply(E_DataType_Status, getStatusStruct());
    sendMonitorReply(E_DataType_SetPoint, cfgGetParam(m_mwSetPoint));
    sendMonitorReply(E_DataType_Temperature, m_lastTemperatureRead);
    sendMonitorReply(E_DataType_Power, cfgGetParam(m_mwMv));
}

STATUS HeatExchangeZone::setMaxTemperature(unsigned short maxTemperature)
{
    return cfgPutParam(m_mwSetPointHeaters, maxTemperature);
}
