/*
 * HeaterPumaZone.cpp
 *
 *  Created on: Feb 18, 2013
 *      Author: yossis
 */

#include "HeaterPumaZone.h"

HeaterPumaZone::HeaterPumaZone()
{

}

HeaterPumaZone::~HeaterPumaZone()
{
}

void HeaterPumaZone::executeZone()
{
    U32 value;
    U32 status;

    cfgPutParam(m_mwMv, cfgGetParam(m_mwMvFromPuma));

    executeTemperatureReadout(m_mwTempId, value);

}

