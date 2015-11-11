/*
 * AnalogInputZone.h
 *
 *  Created on: 17 Feb 2013
 *      Author: maora
 */

#ifndef ANALOGINPUTZONE_H_
#define ANALOGINPUTZONE_H_

#include "BaseMiddlewareWrapperZone.h"

class AnalogInputZone: public BaseMiddlewareWrapperZone
{
protected:
    U32 m_mwInputId;

public:
    AnalogInputZone();
    virtual ~AnalogInputZone();

    /**
     * read all zone values and send the reply to the OPC. Called from the periodic monitor task.
     * @return error value. OK/0 on success.
     */
    virtual void executeZone();
    virtual void sendZoneStatus();

    /**
     * Set the zone enabled status.
     * @param enable
     * @return OK/0 if the enable succeeded.
     */
    virtual STATUS enableZone(bool enable);

    void setMwInputId(U32 mwInputId)
    {
        m_mwInputId = mwInputId;
    }

    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType() {return E_ZoneType_AnalogInput;}

};

#endif /* ANALOGINPUTZONE_H_ */
