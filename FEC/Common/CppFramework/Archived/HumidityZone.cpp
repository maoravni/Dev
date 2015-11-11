/*
 * HumidityZone.cpp
 *
 *  Created on: 17 Dec 2012
 *      Author: maora
 */

#include "HumidityZone.h"
#include "logger.h"
#include "CfgDB.h"

HumidityZone::HumidityZone()
{
}

HumidityZone::~HumidityZone()
{
}

void HumidityZone::setMiddlewareIds(int instance, int index, U32 readValue)
{
    m_mwReadValueId = readValue;
}

U32 HumidityZone::readZone(U32& value)
{
    // TODO: implement a simulation mode for the readout of humidity.
    STATUS status;

    // read the temperature value from the database.
    status = cfgGetParam(m_mwReadValueId, value);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
            "Humidity Zone controller=%d zone=%d status=%d value=%d", m_controllerId, m_zoneId, status, value);

    if (status != OK)
        return status;

    return status;
}
