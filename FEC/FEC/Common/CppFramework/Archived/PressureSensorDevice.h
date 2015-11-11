/*
 * PressureSensorDevice.h
 *
 *  Created on: 11 Mar 2013
 *      Author: maora
 */

#ifndef PRESSURESENSORDEVICE_H_
#define PRESSURESENSORDEVICE_H_

#include "AnalogInputZone.h"

class PressureSensorDevice: public AnalogInputZone
{
public:
    PressureSensorDevice();
    virtual ~PressureSensorDevice();

    virtual void sendZoneStatus();
    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType() {return E_ZoneType_PressureSensor;}
    virtual void configSampleInterval(unsigned int interval){};

};

#endif /* PRESSURESENSORDEVICE_H_ */
