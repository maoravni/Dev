/*
 * PhantomZones.cpp
 *
 *  Created on: 10 Dec 2012
 *      Author: maora
 */

#include "PhantomZones.h"
#include "GnrlCfgDB.h"
#include "TemperatureCfgDB.h"
#include "OthersCfgDB.h"
#include "TemperatureZone.h"
#include "HeaterZone.h"
#include "AirBlowerZone.h"
#include "AirBleedingZone.h"

PhantomZones::PhantomZones()
{
}

PhantomZones::~PhantomZones()
{
}

void PhantomZones::configZoneManager(int controlledId, int deviceType)
{
    m_controllerId = controlledId;
    m_deviceType = deviceType;


    int i;
    // heater zones
    for (i = 0; i <= 11; ++i)
    {
        addHeaterZone(m_controllerId, m_deviceType, 0, i, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
                ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, ID_TEMPERATURE_CFG_INT_ZONE_SP_n, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,
                ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n);
    }

    // temperature zones
    for (i = 12; i <= 71; ++i)
    {
        addTemperatureZone(m_controllerId, m_deviceType, 0, i, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n);
    }

    // blower zones
    addBlowerZone(m_controllerId, m_deviceType, 0, 0, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n);
    addBlowerZone(m_controllerId, m_deviceType, 0, 1, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n);
    addBlowerZone(m_controllerId, m_deviceType, 0, 2, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n);

    // Bleeding zones
    addBleedingZone(m_controllerId, m_deviceType, 0, 0, CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 12),
            CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 13));
    addBleedingZone(m_controllerId, m_deviceType, 0, 0, CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 14),
            CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 15));

}

