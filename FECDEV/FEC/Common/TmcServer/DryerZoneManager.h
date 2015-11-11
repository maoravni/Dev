/*
 * DryerZoneManager.h
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#ifndef DRYERZONEMANAGER_H_
#define DRYERZONEMANAGER_H_

#include <ZoneManager.h>

class DryerZoneManager: public ZoneManager
{
public:
    DryerZoneManager();
    virtual ~DryerZoneManager();

    /**
     * Config the hardware devices connected to this board.
     * @param controlledId
     * @param deviceType
     */
    virtual void configZoneManager(int controlledId, int deviceType);
};

#endif /* DRYERZONEMANAGER_H_ */
