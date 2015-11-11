/*
 * FanZone.h
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#ifndef FANZONE_H_
#define FANZONE_H_

#include "BaseMiddlewareWrapperZone.h"
#include "HeatExchangeZone.h"

class FanZone: public BaseMiddlewareWrapperZone
{
protected:
    U32 m_mwEnable;

    std::vector<HeatExchangeZone*> m_heatExchangeVec;
    int m_numOfZones;
    bool m_fanActive;

public:
    FanZone();
    virtual ~FanZone();

    void addHeatExchangeZone(HeatExchangeZone* zone);

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_Fan;
    }

    virtual void executeZone();

    /**
     * Enable/Disable heating of the zone.
     * @param enable
     */
    virtual STATUS enableZone(bool enable);

    void setMwEnable(U32 mwEnable)
    {
        m_mwEnable = mwEnable;
    }

    /**
     * send the zone status to the SUM.
     */
    virtual void sendZoneStatus();

    virtual void configSampleInterval(unsigned int interval){};

};

#endif /* FANZONE_H_ */
