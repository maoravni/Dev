/*
 m/ * IthZoneManager.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: yossis
 */

#include "IthZoneManager.h"
#include <TemperatureCfgDB.h>
#include <OthersCfgDB.h>
#include <AnalogInputZone.h>
#include <AirInletValve.h>
#include <HeaterPumaZone.h>
#include <PressureSensorDevice.h>
#include <HeaterOutputSetpoint.h>

IthZoneManager::IthZoneManager()
{

}

IthZoneManager::~IthZoneManager()
{
}

void IthZoneManager::configZoneManager(int controlledId, int deviceType)
{
    HeaterPumaZone* heaterZone;

    /*
     addHeaterZone(m_controllerId, m_deviceType, 0, i, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
     ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, ID_TEMPERATURE_CFG_INT_ZONE_SP_n, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,
     ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
     ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,
     ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n);
     */

//    for (int i = 0; i < ITH_NUMBER_OF_ZONES; ++i)
//    {
//        heaterZone = new HeaterPumaZone();
//        heaterZone->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, i));
//        heaterZone->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, i));
//        heaterZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, i));
//        heaterZone->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, i));
//        heaterZone->setMwMvFromPuma(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n, i));
//        heaterZone->setMwPidP(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, i));
//        heaterZone->setMwPidI(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n, i));
//        heaterZone->setMwPidD(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, i));
//        heaterZone->setMwAutoTune(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n, i));
//        heaterZone->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, i));
//        addZone(heaterZone);
//    }
    HeaterInternalPid* pidHeater;

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 0));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 0));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 0));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 0));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 0));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(pidHeater);

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 1));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 1));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 1));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 1));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 1));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(pidHeater);

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 2));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 2));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 2));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 2));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 2));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(pidHeater);

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 3));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 3));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 3));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 3));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 3));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(pidHeater);

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 4));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 12));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 4));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 4));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 4));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(pidHeater);

    pidHeater = new HeaterInternalPid();
    pidHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 5));
    pidHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 13));
    pidHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 5));
    pidHeater->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 5));
    pidHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 5));
    pidHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(pidHeater);

    HeaterOutputSetpoint* setpointHeater = new HeaterOutputSetpoint();
//    setpointHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 6));
    setpointHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 8));
    setpointHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 6));
    setpointHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 6));
    setpointHeater->setOutputDevice(static_cast<HeaterInternalPid*>((*this)[1]));
    setpointHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(setpointHeater);

    setpointHeater = new HeaterOutputSetpoint();
//    setpointHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 7));
    setpointHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 9));
    setpointHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 7));
    setpointHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 7));
    setpointHeater->setOutputDevice(static_cast<HeaterInternalPid*>((*this)[3]));
    setpointHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(setpointHeater);

    setpointHeater = new HeaterOutputSetpoint();
//    setpointHeater->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 8));
    setpointHeater->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 10));
    setpointHeater->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 8));
    setpointHeater->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 8));
    setpointHeater->setOutputDevice(static_cast<HeaterInternalPid*>((*this)[2]));
    setpointHeater->configPidValues(1, 0, 0, 0, 0, 0);
    addZone(setpointHeater);

    for (int i = ITH_NUMBER_OF_ZONES; i < (ITH_NUMBER_OF_ZONES + ITH_NUMBER_OF_PT100 + ITH_NUMBER_OF_TC); ++i)
    {
        addTemperatureSensorZone(controlledId, deviceType, 0, i, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n);
    }

    PressureSensorDevice *pressureSensor = new PressureSensorDevice();
    pressureSensor->setMwInputId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 9));
    addZone(pressureSensor);

    AirBlowerZone* blowerZone = addBlowerZone();
    blowerZone->setMwEnable(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n, 6));
    blowerZone->setMwFeedback(CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n, 0));
    blowerZone->setMwSetPoint(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n, 0));

    AirInletValve* inletZone = new AirInletValve();
    inletZone->setMwValue(CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_ITH_VALVES_STATE, 0));
    addZone(inletZone);
}
