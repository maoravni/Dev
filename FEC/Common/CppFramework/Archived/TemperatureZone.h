/*
 * TemperatureZone.h
 *
 *  Created on: 5 Dec 2012
 *      Author: maora
 */

#ifndef TEMPERATUREZONE_H_
#define TEMPERATUREZONE_H_

#include "ll_typedef.h"
#include "CfgDBUtil.h"
#include "BaseZone.h"
#include "BaseMiddlewareWrapperZone.h"

/**
 * Class for abstraction of a temperature read-only zone.
 */
class TemperatureZone : public BaseMiddlewareWrapperZone
{
protected:
    //! Id's for calling the middleware's API.
    U32 m_mwReadValueId;    //!< ID for reading the temperature

public:
    TemperatureZone();
    virtual ~TemperatureZone();

    /**
     * get the current value of the temperature zone
     */
    virtual U32 readZone(U32 &value);

    /**
     * Enable/Disable heating of the zone.
     * @param enable
     */
    virtual STATUS enableZone(bool enable);

    /**
     * Config the values for calling the middleware's API
     * @param instance
     * @param index
     * @param readValue
     */
    virtual void setMiddlewareIds(int instance, int index, U32 readValue);

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
    virtual E_ZoneType getZoneType() {return E_ZoneType_TemperatureSensor;}

    void setMwReadValueId(U32 mwReadValueId)
    {
        m_mwReadValueId = mwReadValueId;
    }
    virtual void configSampleInterval(unsigned int interval){};

};

#endif /* TEMPERATUREZONE_H_ */
