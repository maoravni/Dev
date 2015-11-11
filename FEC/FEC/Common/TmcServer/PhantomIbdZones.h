/*
 * PhantomIbdZones.h
 *
 *  Created on: 10 Dec 2012
 *      Author: maora
 */

#ifndef PhantomIbdZones_H_
#define PhantomIbdZones_H_

#include "ZoneManager.h"
/*
 *
 */
class PhantomIbdZones: public ZoneManager
{
    int m_controllerId;
    int m_deviceType;
public:
    PhantomIbdZones();
    virtual ~PhantomIbdZones();

    void configZoneManager(int controlledId, int deviceType);

};

#endif /* PhantomIbdZones_H_ */
