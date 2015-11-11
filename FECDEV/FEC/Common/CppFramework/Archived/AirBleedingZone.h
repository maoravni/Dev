/*
 * AirBleeding.h
 *
 *  Created on: 17 Dec 2012
 *      Author: maora
 */

#ifndef AIRBLEEDING_H_
#define AIRBLEEDING_H_

#include "BaseZone.h"
#include "BaseMiddlewareWrapperZone.h"

struct T_AirBleedingZoneSetpoint
{
    short inValveState;
    short outValveState;
    T_AirBleedingZoneSetpoint():inValveState(0), outValveState(0) {}
};

struct T_AirBleedingZoneMonitorReply
{
    unsigned short status;
    unsigned short inValve;
    unsigned short outValve;
};

/*
 *
 */
class AirBleedingZone: public BaseMiddlewareWrapperZone
{
protected:
    T_AirBleedingZoneSetpoint m_setPoints[3];
    T_AirBleedingZoneSetpoint m_currentSetPoint;

    //! Id's for calling the middleware's API.
    U32 m_mwInValve;
    U32 m_mwOutValve;
public:
    AirBleedingZone();
    virtual ~AirBleedingZone();

    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType() {return E_ZoneType_AirBleeding;}

    /**
     * Enable/Disable heating of the zone.
     * @param enable
     */
    virtual STATUS enableZone(bool enable);

    //virtual void setMiddlewareIds(int instance, int index, U32 inValve, U32 outValve);

    /**
     * Read zone values and send the values to the OPC. Called from the periodic monitor task.
     * @return error value. OK/0 on success.
     */
    virtual void executeZone();
    virtual void sendZoneStatus();

    bool setValveState(int valve, int value);

    void setMwInValve(U32 mwInValve)
    {
        m_mwInValve = mwInValve;
    }

    void setMwOutValve(U32 mwOutValve)
    {
        m_mwOutValve = mwOutValve;
    }
};

#endif /* AIRBLEEDING_H_ */
