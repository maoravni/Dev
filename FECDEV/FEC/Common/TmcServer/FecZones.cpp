/*
 * FecZones.cpp
 *
 *  Created on: 10 Dec 2012
 *      Author: maora
 */

#include "FecZones.h"
#include "GnrlCfgDB.h"
#include "TemperatureCfgDB.h"
#include "OthersCfgDB.h"
#include "TemperatureZone.h"
#include "HeaterZone.h"
#include "AirBlowerZone.h"
#include "AirBleedingZone.h"

FecZones::FecZones()
{
}

FecZones::~FecZones()
{
}

void FecZones::configZoneManager(int controlledId, int deviceType)
{
    m_controllerId = controlledId;
    m_deviceType = deviceType;

    int i;
    // heater zones
    for (i = 0; i <= 5; ++i)
    {
        addHeaterZone(m_controllerId, m_deviceType, 0, i, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
                ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, ID_TEMPERATURE_CMND_INT_ZONE_SP_n, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,
                ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n);
    }

    // temperature zones
    for (i = 12; i <= 71; ++i)
    {
        addTemperatureSensorZone(m_controllerId, m_deviceType, 0, i, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n);
    }

    // blower zones
    AirBlowerZone* blowerZone;
    blowerZone = addBlowerZone(/*m_controllerId, m_deviceType, 0, 0, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n*/);
    blowerZone->setMwEnable(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 6));
    blowerZone->setMwFeedback(CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n, 0));
    blowerZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n, 0));

    blowerZone = addBlowerZone(/*m_controllerId, m_deviceType, 0, 0, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n*/);
    blowerZone->setMwEnable(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 7));
    blowerZone->setMwFeedback(CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n, 1));
    blowerZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n, 1));

}

