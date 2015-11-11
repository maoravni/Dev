/*
 * DigitalInputDevice.h
 *
 *  Created on: 23 Apr 2013
 *      Author: maora
 */

#ifndef DIGITALINPUTDEVICE_H_
#define DIGITALINPUTDEVICE_H_

#include "BaseMiddlewareWrapperZone.h"

class DigitalInputDevice: public BaseMiddlewareWrapperZone
{
    U32 m_mwInput;
public:
    DigitalInputDevice();
    virtual ~DigitalInputDevice();

    virtual E_ZoneType getZoneType() { return E_ZoneType_DigitalInput; }

    void setMwInput(U32 mwInput)
    {
        m_mwInput = mwInput;
    }

    /**
     * Read zone values and send the values to the OPC. Called from the periodic monitor task.
     */
    virtual void executeZone();

    /**
     * send the zone status to the SUM.
     */
    virtual void sendZoneStatus();
};

#endif /* DIGITALINPUTDEVICE_H_ */
