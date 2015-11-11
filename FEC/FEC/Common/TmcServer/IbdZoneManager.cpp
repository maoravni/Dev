/*
 * IbdZoneManager.cpp
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#include "IbdZoneManager.h"
#include <OthersCfgDB.h>
#include <TemperatureCfgDB.h>
#include <HeaterInternalPid.h>
#include <math.h>

IbdZoneManager::IbdZoneManager()
{
}

IbdZoneManager::~IbdZoneManager()
{
}

void IbdZoneManager::configZoneManager(int controlledId, int deviceType)
{
    HeatExchangePumaZone *htxPumaZone;

//    htxPumaZone = addHeatExchangePumaZone();
//    htxPumaZone->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 0));
//    htxPumaZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 0));
//    htxPumaZone->setMwSetPointHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP0_n, 0));
//    htxPumaZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 6));
//    htxPumaZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 8));
//#ifndef WITH_DAT9k_PROTECTION
//    htxPumaZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 10));
//#endif
//    htxPumaZone->setMwHeatExchangeId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 0));
//    htxPumaZone->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 0));
//    htxPumaZone->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 0));
//    htxPumaZone->setMwMvFromPuma(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n, 0));
//    htxPumaZone->setMwPidPHtx(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 0));
//    htxPumaZone->setMwPidPHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n, 0));
//
//    htxPumaZone = addHeatExchangePumaZone();
//    htxPumaZone->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 1));
//    htxPumaZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 1));
//    htxPumaZone->setMwSetPointHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP0_n, 1));
//    htxPumaZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 7));
//    htxPumaZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 9));
//#ifndef WITH_DAT9k_PROTECTION
//    htxPumaZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 11));
//#endif
//    htxPumaZone->setMwHeatExchangeId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 1));
//    htxPumaZone->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 1));
//    htxPumaZone->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 1));
//    htxPumaZone->setMwMvFromPuma(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n, 1));
//    htxPumaZone->setMwPidPHtx(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 1));
//    htxPumaZone->setMwPidPHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n, 1));
//
//    HeatExchangeZone *htxZone;
//
//    htxZone = addHeatExchangeZone();
//    htxZone->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 0));
//    htxZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 0));
//    htxZone->setMwSetPointHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP0_n, 0));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 6));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 8));
//#ifndef WITH_DAT9k_PROTECTION
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 10));
//#endif
//    htxZone->setMwHeatExchangeId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 0));
//    htxZone->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 0));
//    htxZone->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 0));
//    htxZone->setMwPidPHtx(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 0));
//    htxZone->setMwPidPHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n, 0));
//
//    htxZone = addHeatExchangeZone();
//    htxZone->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 1));
//    htxZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 1));
//    htxZone->setMwSetPointHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP0_n, 1));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 7));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 9));
//#ifndef WITH_DAT9k_PROTECTION
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 11));
//#endif
//    htxZone->setMwHeatExchangeId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 1));
//    htxZone->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 1));
//    htxZone->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 1));
//    htxZone->setMwPidPHtx(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 1));
//    htxZone->setMwPidPHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n, 1));

    HeaterInternalPid *pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 0));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 0));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 0));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 0));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 0));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 6));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 8));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 10));
    addZone(pidHeater);

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 1));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 1));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 1));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 1));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 1));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 7));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 9));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 11));
    addZone(pidHeater);

    AirBlowerZone* blowerZone = addBlowerZone();
    blowerZone->setMwEnable(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 6));
    blowerZone->setMwFeedback(CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n, 0));
    blowerZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n, 0));
    //blowerZone->assignIds(0, 0, 0xf0);

    blowerZone = addBlowerZone();
    blowerZone->setMwEnable(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 7));
    blowerZone->setMwFeedback(CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n, 1));
    blowerZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n, 1));
//    blowerZone->assignIds(0, 0, 0xf1);


    for (int i = IBD_NUMBER_OF_ZONES; i < (IBD_NUMBER_OF_ZONES+IBD_NUMBER_OF_PT100+IBD_NUMBER_OF_TC); ++i)
    {
        addTemperatureSensorZone(controlledId, deviceType, 0, i, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n);
    }

}
