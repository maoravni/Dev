/*
 * HeatExchangeZone.h
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#ifndef HEATEXCHANGEZONE_H_
#define HEATEXCHANGEZONE_H_

#include "HeaterZone.h"
#include <vector>

class HeatExchangeZone: public HeaterZone
{
protected:
    U32 m_mwHeatExchangeId;
    U32 m_mwSetPointHeaters;
    U32 m_mwPidPHtx;
    std::vector<U32> m_mwHeaterIdVec;
    std::vector<U32> m_tempVec;

    bool m_heatersActive;
    bool m_blowerActive;

public:
    HeatExchangeZone();
    virtual ~HeatExchangeZone();

    /**
     * read all zone values and send the reply to the OPC. Called from the periodic monitor task.
     * @return error value. OK/0 on success.
     */
    virtual void executeZone();
    virtual void sendZoneStatus();

    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_Heater;
    }

    bool isBlowerActive()
    {
        return m_blowerActive;
    }

    virtual void activateHeaters(U32 Mv);

    /**
     * Config the temperature zone to a specific set of
     * @param temperature
     * @param lowWarn
     * @param lowRange
     * @param highRange
     * @param highWarn
     * @param sn
     * @return true on success
     */
    virtual bool configValues(unsigned short temperature, short lowWarn, short lowRange, short highRange,
            short highWarn, U32 sn);

    /**
     * Enable/Disable heating of the zone.
     * @param enable
     */
    virtual STATUS enableZone(bool enable);

    virtual void executeHeaters(U32 htxTemp, U32 spHigh, U32 spLow);

    void setMwHeatExchangeId(U32 mwHeatExchangeId)
    {
        m_mwHeatExchangeId = mwHeatExchangeId;
    }

    void setMwPidPHtx(U32 mwPidPHtx)
    {
        m_mwPidPHtx = mwPidPHtx;
    }

    void setMwPidPHeaters(U32 mwPidP)
    {
        m_mwPidP = mwPidP;
    }

    void setMwSetPointHeaters(U32 mwSetPointHeaters)
    {
        m_mwSetPointHeaters = mwSetPointHeaters;
    }

    void addMwHeaterId(U32 mwHeaterId);

    virtual STATUS setMaxTemperature(unsigned short maxTemperature);

};

#endif /* HEATEXCHANGEZONE_H_ */
