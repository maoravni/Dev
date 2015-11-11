/*
 * FecZones.h
 *
 *  Created on: 10 Jan 2013
 *      Author: maora
 */

#ifndef FECZONES_H_
#define FECZONES_H_

#include "ZoneManager.h"

class FecZones : public ZoneManager
{
    int m_controllerId;
    int m_deviceType;
public:
    FecZones();
    virtual ~FecZones();

    void configZoneManager(int controlledId, int deviceType);

};

#endif /* FECZONES_H_ */
