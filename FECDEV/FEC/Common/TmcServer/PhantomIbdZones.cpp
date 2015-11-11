/*
 * PhantomIbdZones.cpp
 *
 *  Created on: 10 Dec 2012
 *      Author: maora
 */

#include "PhantomIbdZones.h"
#include "GnrlCfgDB.h"
#include "TemperatureCfgDB.h"
#include "OthersCfgDB.h"
#include "TemperatureZone.h"
#include "HeaterZone.h"
#include "AirBlowerZone.h"
#include "AirBleedingZone.h"
#include "IbdZone.h"

PhantomIbdZones::PhantomIbdZones()
{
}

PhantomIbdZones::~PhantomIbdZones()
{
}

void PhantomIbdZones::configZoneManager(int controlledId, int deviceType)
{
    m_controllerId = controlledId;
    m_deviceType = deviceType;

    int i;
    // heater zones
    for (i = 0; i <= 5; ++i)
    {
        addHeaterZone(m_controllerId, m_deviceType, 0, i, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
                ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, ID_TEMPERATURE_CFG_INT_ZONE_SP_n, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,
                ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n);
    }

//    IbdBlowerZone *fanZone = addIbdBlowerZone(m_controllerId, m_deviceType, CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 16));
//    fanZone->m_zoneId = 0xf0;

//    fanZone->addIbdZone(
            addIbdZone(m_controllerId, m_deviceType, CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 6),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_SP_n, 6), // heat exchange set point
            0,                                                      // default sp for heater fingers
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 54),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 55),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 56),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 6),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 6),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 6),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 6),
            0,                                                          // default values for P for the heater fingers.
            0);
//            );

//    fanZone->addIbdZone(
            addIbdZone(m_controllerId, m_deviceType, CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 7),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_SP_n, 7),
            0,
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 57),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 58),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 59),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 7),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 7),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 7),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 7),
            0,
            0);
//            );

//    fanZone->addIbdZone(
            addIbdZone(m_controllerId, m_deviceType, CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 8),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_SP_n, 8),
            0,
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 60),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 61),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 62),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 8),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 8),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 8),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 8),
            0,
            0);
//            );

//    fanZone = addIbdBlowerZone(m_controllerId, m_deviceType, CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 17));
//    fanZone->m_zoneId = 0xf1;

//    fanZone->addIbdZone(
            addIbdZone(m_controllerId, m_deviceType, CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 9),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_SP_n, 9),
            0,
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 63),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 64),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 65),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 9),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 9),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 9),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 9),
            0,
            0);
//            );

//    fanZone->addIbdZone(
            addIbdZone(m_controllerId, m_deviceType, CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 10),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_SP_n, 10),
            0,
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 66),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 67),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 68),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 10),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 10),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 10),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 10),
            0,
            0);
//            );

//    fanZone->addIbdZone(
            addIbdZone(m_controllerId, m_deviceType, CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 11),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_SP_n, 11),
            0,
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 69),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 70),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 71),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 11),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 11),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 11),
            CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 11),
            0,
            0);
//            );

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
//    addBleedingZone(m_controllerId, m_deviceType, 0, 0, CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 12),
//            CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 13));
//    addBleedingZone(m_controllerId, m_deviceType, 0, 0, CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 14),
//            CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 15));

}

