/*
 * ZoneManager.cpp
 *
 *  Created on: 10 Dec 2012
 *      Author: maora
 */

#include "ZoneManager.h"

ZoneManager::ZoneManager()
{
    m_pZoneArray = NULL;
    m_numOfAssignedZones = 0;
    m_totalZones = 0;
}

ZoneManager::~ZoneManager()
{
    if (m_pZoneArray != NULL)
    {
        for (int i = 0; i < m_totalZones; ++i)
        {
            if (m_pZoneArray[i] != NULL)
                delete m_pZoneArray[i];
        }
        delete m_pZoneArray;
    }
}

ZoneNode * ZoneManager::addZone(BaseZone * zone)
{
    BaseZone** tempZoneArray = new BaseZone*[m_totalZones + 1];
    if (m_pZoneArray != NULL)
    {
        for (int i = 0; i < m_totalZones; ++i)
            tempZoneArray[i] = m_pZoneArray[i];
        delete m_pZoneArray;
    }
    m_pZoneArray = tempZoneArray;
    m_pZoneArray[m_totalZones] = zone;
    ++m_totalZones;

    return NULL;
}

BaseZone* ZoneManager::assignIdToZoneType(unsigned char controllerId, unsigned char zoneId, char index,
        E_ZoneType zoneType)
{
    char currentIndex = -1;

    for (int i = 0; i < m_totalZones; ++i)
    {
        if (m_pZoneArray[i]->getZoneType() == zoneType)
            ++currentIndex;

        if (index == currentIndex)
        {
            if (m_pZoneArray[i]->getZoneId() == M_BASE_ZONE_UNASSIGNED_ID)
            {
                m_pZoneArray[i]->assignIds(controllerId, zoneId);
                ++m_numOfAssignedZones;
                return m_pZoneArray[i];
            }
            else
                return NULL;
        }
//        if (m_pZoneArray[i]->getZoneType() == zoneType && m_pZoneArray[i]->getZoneId() == M_BASE_ZONE_UNASSIGNED_ID)
//        {
//            m_pZoneArray[i]->assignIds(deviceType, controllerId, zoneId);
//            ++m_numOfAssignedZones;
//            return m_pZoneArray[i];
//        }
    }
    return NULL;
}

BaseZone* ZoneManager::getZoneById(char zoneId)
{
    for (int i = 0; i < m_totalZones; ++i)
    {
        if (m_pZoneArray[i]->getZoneId() == zoneId)
        {
            return m_pZoneArray[i];
        }
    }
// if we're here it means we didn't find the node.
    return NULL;
}

void ZoneManager::reset()
{
    for (int i = 0; i < m_totalZones; ++i)
    {
        m_pZoneArray[i]->reset();
    }
    m_numOfAssignedZones = 0;
}

BaseZone *ZoneManager::operator [](int index)
{
    return m_pZoneArray[index];
}

TemperatureZone* ZoneManager::addTemperatureSensorZone(char controllerId, char deviceType, int instance, int index,
        U32 readValue)
{
    TemperatureZone *zone = new TemperatureZone();

    zone->setMiddlewareIds(instance, index, readValue);

    addZone(zone);

    return zone;
}

HeaterZone* ZoneManager::addHeaterZone(char controllerId, char deviceType, int instance, int index, U32 enable,
        U32 readValue, U32 setPoint, U32 mv, U32 PumaMv, U32 pidP, U32 pidI, U32 pidD, U32 powerLimit, U32 autoTune)
{
    HeaterZone* zone = new HeaterZone();

    zone->setMiddlewareIds(instance, index, enable, readValue, setPoint, mv, PumaMv, pidP, pidI, pidD, powerLimit,
            autoTune);

    addZone(zone);

    return zone;
}

AirBlowerZone* ZoneManager::addBlowerZone()
{
    AirBlowerZone* zone = new AirBlowerZone();

    addZone(zone);

    return zone;
}

//AirBleedingZone* ZoneManager::addBleedingZone(char controlledId, char deviceType, int instance, int index, U32 inValve,
//        U32 outValve)
//{
//    AirBleedingZone* zone = new AirBleedingZone();
//
//    //zone->setMiddlewareIds(instance, index, inValve, outValve);
//
//    addZone(zone);
//
//    return zone;
//}

HeatExchangeZone* ZoneManager::addHeatExchangeZone()
{
    HeatExchangeZone* zone = new HeatExchangeZone();

    addZone(zone);

    return zone;
}

bool ZoneManager::executeZones()
{
    int numOfZones = getNumberOfZones();

    bool allInReady = true;

    for (int i = 0; i < numOfZones; ++i)
    {
        BaseZone *zone = m_pZoneArray[i];

        zone->executeZone();

        if (zone->isMonitoringEnabled())
        {
            zone->sendZoneStatus();
        }

        // if a set point was activated, wait until all zones are in ready, and then
        // send a sequence ended message to the OPC;
        if (zone->getState() != E_ZoneState_Ready)
            allInReady = false;
    }

    return allInReady;
}

HeatExchangePumaZone* ZoneManager::addHeatExchangePumaZone()
{
    HeatExchangePumaZone* zone = new HeatExchangePumaZone();

    addZone(zone);

    return zone;
}

void ZoneManager::configSampleTime(unsigned int interval)
{
    for (int i = 0; i < getNumberOfZones(); ++i)
        m_pZoneArray[i]->configSampleInterval(interval);
}
