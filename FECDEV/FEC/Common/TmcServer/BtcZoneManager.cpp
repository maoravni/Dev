/*
 * BtcZoneManager.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: yossis
 */

#include "BtcZoneManager.h"
#include "CBtc.h"
#include <os_cnfg.h>

BtcZoneManager::BtcZoneManager()
{
}

BtcZoneManager::~BtcZoneManager()
{
}

void BtcZoneManager::configZoneManager(int controlledId, int deviceType)
{

    CBtc* zone = new CBtc("BTC", BTC_TASK_STACK_SZ, BTC_TASK_PRIORITY);

    addZone(zone);

    HeaterZone* coolingControl = new HeaterZone();
    coolingControl->setMwEnableId(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n, 0));
    coolingControl->setMwTempId(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, 0));
    coolingControl->setMwSetPoint(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_SP_n, 0));
    coolingControl->setMwMv(CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, 0));
    coolingControl->setMwPidP(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, 0));
    coolingControl->setMwPidI(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n, 0));
    coolingControl->setMwPidD(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, 0));
    coolingControl->setMwPowerLimit(CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n, 0));
    coolingControl->setMwAutoTune(CfgDB_ID_AddIndex(ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n, 0));
    addZone(coolingControl);

    //pumps
    addZone(zone->getTankWaterFillComponent());
    addZone(zone->getTankWaterDrainComponent());
    addZone(zone->getFillPumpComponent());
    addZone(zone->getDrainPumpComponent());

    // Engage system
    addZone(zone->getEngageComponent());

    // valve
    addZone(zone->getAirKnifePressureValveComponent());

    // Digital Inputs
    addZone(zone->getBlanketStopState());
    addZone(zone->getDrawerPositionState());
    addZone(zone->getTubOverflowState());
    addZone(zone->getAirKnifePressureNormalState());
    //addZone(zone->getEngageState());

    // Water Contamination
    addZone(zone->getWaterContamination());

    // Water Tank level state
    addZone(zone->getTankLevelSens());

    // RO temperatures
    for (int i = BTC_NUMBER_OF_ZONES; i < (BTC_NUMBER_OF_ZONES + BTC_NUMBER_OF_PT100 + BTC_NUMBER_OF_IRT); ++i)
    {
        addTemperatureSensorZone(controlledId, deviceType, 0, i, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n);
    }
}
