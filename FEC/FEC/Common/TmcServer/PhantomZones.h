/*
 * PhantomZones.h
 *
 *  Created on: 10 Dec 2012
 *      Author: maora
 */

#ifndef PHANTOMZONES_H_
#define PHANTOMZONES_H_

#include "ZoneManager.h"
/*
 *
 */
class PhantomZones: public ZoneManager
{
    int m_controllerId;
    int m_deviceType;
public:
    PhantomZones();
    virtual ~PhantomZones();

    void configZoneManager(int controlledId, int deviceType);

};

#endif /* PHANTOMZONES_H_ */
