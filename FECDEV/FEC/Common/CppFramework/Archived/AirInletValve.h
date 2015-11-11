/*
 * AirInletValve.h
 *
 *  Created on: 17 Feb 2013
 *      Author: maora
 */

#ifndef AIRINLETVALVE_H_
#define AIRINLETVALVE_H_

#include "BaseMiddlewareWrapperZone.h"

#define M_INLET_NUM_SETPOINTS 3

class AirInletValve: public BaseMiddlewareWrapperZone
{
protected:
    U32 m_mwValue;
    U32 m_setPoints[M_INLET_NUM_SETPOINTS];

public:
    AirInletValve();
    virtual ~AirInletValve();

    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType() {return E_ZoneType_AirInletValve;}

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

    virtual bool configValues(U32 value);

    virtual bool configSetPoint(int index, U32 value);

    /**
     * Activate a preset set point.
     * @param setPoint index of the set point to activate.
     * @return true on success
     */
    virtual bool enableSetPoint(int setPoint);

    void setMwValue(U32 mwValue)
    {
        m_mwValue = mwValue;
    }
    virtual void configSampleInterval(unsigned int interval){};

};

#endif /* AIRINLETVALVE_H_ */
