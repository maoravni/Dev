/*
 * DryerZoneManager.cpp
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#include "DryerZoneManager.h"
#include <TemperatureCfgDB.h>
#include <OthersCfgDB.h>
#include <HeaterOutputSetpoint.h>

DryerZoneManager::DryerZoneManager()
{
}

DryerZoneManager::~DryerZoneManager()
{
}

void DryerZoneManager::configZoneManager(int controlledId, int deviceType)
{

//    HeatExchangePumaZone *htxZone;
//
//    htxZone = addHeatExchangePumaZone();
//    htxZone->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 0));
//    htxZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 0));
//    htxZone->setMwSetPointHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP0_n, 0));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 5));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 7));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 9));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 11));
//    htxZone->setMwHeatExchangeId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 0));
//    htxZone->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 0));
//    htxZone->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 0));
//    htxZone->setMwMvFromPuma(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n, 0));
//    htxZone->setMwPidPHtx(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 0));
//    htxZone->setMwPidPHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n, 0));
//
//    htxZone = addHeatExchangePumaZone();
//    htxZone->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 1));
//    htxZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 1));
//    htxZone->setMwSetPointHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP0_n, 1));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 6));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 8));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 10));
//    htxZone->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 12));
//    htxZone->setMwHeatExchangeId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 1));
//    htxZone->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 1));
//    htxZone->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 1));
//    htxZone->setMwMvFromPuma(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n, 1));
//    htxZone->setMwPidPHtx(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 1));
//    htxZone->setMwPidPHeaters(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n, 1));

    HeaterInternalPid* pidHeater;

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 0));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 0));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 0));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 0));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 0));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 5));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 7));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 9));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 11));
    addZone(pidHeater);

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 1));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 1));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 1));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 1));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 1));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 6));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 8));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 10));
    pidHeater->addMwHeaterId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 12));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(pidHeater);

    HeaterOutputSetpoint* setpointHeater = new HeaterOutputSetpoint();
//    setpointHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 7));
    setpointHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 3));
    setpointHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 2));
    setpointHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 2));
    setpointHeater->setOutputDevice(static_cast<HeaterInternalPid*>((*this)[0]));
    setpointHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(setpointHeater);

    setpointHeater = new HeaterOutputSetpoint();
//    setpointHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 7));
    setpointHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 4));
    setpointHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 3));
    setpointHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 3));
    setpointHeater->setOutputDevice(static_cast<HeaterInternalPid*>((*this)[1]));
    setpointHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(setpointHeater);

    AirBlowerZone* blowerZone = addBlowerZone();
    blowerZone->setMwEnable(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 6));
    blowerZone->setMwFeedback(CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n, 0));
    blowerZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n, 0));
    //blowerZone->assignIds(0, 0, 0xf0);

    FanZone* fanZone = new FanZone();
    fanZone->setMwEnable(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 8));
    addZone(fanZone);

    for (int i = DRYER_NUMBER_OF_ZONES; i < (DRYER_NUMBER_OF_ZONES + DRYER_NUMBER_OF_PT100 + DRYER_NUMBER_OF_TC); ++i)
    {
        addTemperatureSensorZone(controlledId, deviceType, 0, i, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n);
    }

}

