/*
 * IbdZoneManager.h
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#ifndef IBDZONEMANAGER_H_
#define IBDZONEMANAGER_H_

#include <ZoneManager.h>

class IbdZoneManager: public ZoneManager
{
public:
    IbdZoneManager();
    virtual ~IbdZoneManager();

    /**
     * Config the hardware devices connected to this board.
     * @param controlledId
     * @param deviceType
     */
    virtual void configZoneManager(int controlledId, int deviceType);
};

#endif /* IBDZONEMANAGER_H_ */
