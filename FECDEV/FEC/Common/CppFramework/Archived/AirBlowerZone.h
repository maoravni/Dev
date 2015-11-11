/*
 * AirBlowerZone.h
 *
 *  Created on: 17 Dec 2012
 *      Author: maora
 */

#ifndef AIRBLOWERZONE_H_
#define AIRBLOWERZONE_H_

#include "BaseZone.h"
#include "BaseMiddlewareWrapperZone.h"
#include <vector>
#include "HeatExchangeZone.h"

#define M_BLOWER_ZONE_NUMBER_OF_SET_POINTS 3

struct T_BlowerZoneMonitorReply
{
    unsigned short status;
    unsigned short power;
    unsigned short speed;
};

/*
 *
 */
class AirBlowerZone: public BaseMiddlewareWrapperZone
{
protected:

    short m_setPoints[M_BLOWER_ZONE_NUMBER_OF_SET_POINTS];
    short m_currentSetPoint;

    //! Id's for calling the middleware's API.
    U32 m_mwSetPoint;    //!< ID for reading the temperature
    U32 m_mwEnable;
    U32 m_mwFeedback;
    bool m_blowerActive;

    std::vector<HeatExchangeZone*> m_heatExchangeVec;

public:
    AirBlowerZone();
    virtual ~AirBlowerZone();

    virtual void setMiddlewareIds(int instance, int index, U32 setPoint);

    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType() {return E_ZoneType_Blower;}

    /**
     * read all zone values and send the reply to the OPC. Called from the periodic monitor task.
     * @return error value. OK/0 on success.
     */
    virtual void executeZone();
    virtual void sendZoneStatus();

    /**
     * setup a set point to a certain speed.
     * @param setPoint set point index
     * @param speed
     * @return true on success.
     */
    virtual bool configSetPoint(int setPoint, short speed);

    /**
     * Activate a preset set point.
     * @param setPoint index of the set point to activate.
     * @return true on success
     */
    virtual bool enableSetPoint(short setPoint);

    /**
     * Config the blower zone to a specific speed.
     * @param speed
     * @return true on success
     */
    virtual bool configValues(short speed);

    /**
     * Enable/Disable heating of the zone.
     * @param enable
     */
    virtual STATUS enableZone(bool enable);

    void setMwEnable(U32 mwEnable)
    {
        m_mwEnable = mwEnable;
    }

    void setMwFeedback(U32 mwFeedback)
    {
        m_mwFeedback = mwFeedback;
    }

    void setMwSetPoint(U32 mwSetPoint)
    {
        m_mwSetPoint = mwSetPoint;
    }

    void addHeatExchangeZone(HeatExchangeZone* zone);
    virtual void configSampleInterval(unsigned int interval){};

};

#endif /* AIRBLOWERZONE_H_ */
