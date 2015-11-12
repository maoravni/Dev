/*
 * ZoneManager.h
 *
 *  Created on: 10 Dec 2012
 *      Author: maora
 */

#ifndef ZONEMANAGER_H_
#define ZONEMANAGER_H_

#include "BaseZone.h"
#include "TemperatureZone.h"
#include "HeaterZone.h"
#include "AirBlowerZone.h"
#include "AirBleedingZone.h"
#include "HeatExchangeZone.h"
#include "HeatExchangePumaZone.h"
#include "FanZone.h"
#include "HeaterInternalPid.h"

class ZoneNode
{
public:
    BaseZone* zone;
    ZoneNode* next;

    ZoneNode() :
    zone(NULL), next(NULL)
    {
    }
};

/*
 *
 */
class ZoneManager
{
protected:
    BaseZone** m_pZoneArray;
//    ZoneNode* m_pZoneListHead;
//    ZoneNode* m_pZoneListTail;
//    ZoneNode* m_currentZoneNode;
    //int m_numberOfZones;
    int m_numOfAssignedZones;
    int m_totalZones;

public:
    ZoneManager();
    virtual ~ZoneManager();

    /**
     * Add a zone to the zone manager linked list.
     * @param zone
     * @return
     */
    ZoneNode* addZone(BaseZone* zone);

    /**
     * Config the hardware devices connected to this board.
     * @param controlledId
     * @param deviceType
     */
    virtual void configZoneManager(int controlledId, int deviceType) = 0;

    /**
     * Go over all zones in the zone array and call its execute method.
     * @return true when all zones are in ready state.
     */
    virtual bool executeZones();

    /**
     * Get the head of the list.
     * @return
     */
//    ZoneNode* getHead()
//    {
//        return m_pZoneListHead;
//    }
    /**
     * Get the number of nodes in the linked list.
     * @return
     */
    int getNumberOfZones()
    {
        return m_totalZones;
    }

    int getTotalNumberOfZones()
    {
        return m_totalZones;
    }

    /**
     * Assign a zone ID to an existing unassigned zone in the linked list.
     * @param zoneId
     * @param zoneType
     * @return pointer to the zone if found an unassigned node. NULL otherwise.
     */
    BaseZone* assignIdToZoneType(unsigned char controllerId, unsigned char zoneId, char index, E_ZoneType zoneType);

    /**
     * Search through the zones for the zone with the assigned ID and return it.
     * @param zoneId
     * @return pointer to the requested zone. NULL if not found.
     */
    BaseZone* getZoneById(char zoneId);

    void reset();

    BaseZone *operator[](int index);

    virtual TemperatureZone* addTemperatureSensorZone(char controllerId, char deviceType, int instance, int index, U32 readValue);

    virtual HeaterZone* addHeaterZone(char controllerId, char deviceType, int instance, int index, U32 enable, U32 readValue,
            U32 setPoint, U32 mv,U32 PumaMv, U32 pidP, U32 pidI, U32 pidD, U32 powerLimit, U32 autoTune);

    virtual AirBlowerZone* addBlowerZone();

    //virtual AirBleedingZone* addBleedingZone(char controlledId, char deviceType, int instance, int index, U32 inValve, U32 outValve);

    virtual HeatExchangeZone* addHeatExchangeZone();

    virtual HeatExchangePumaZone* addHeatExchangePumaZone();

    virtual void configSampleTime(unsigned int interval);

};
#endif /* ZONEMANAGER_H_ */
