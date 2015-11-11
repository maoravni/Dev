/*
 * BtcZoneManager.h
 *
 *  Created on: Feb 13, 2013
 *      Author: yossis
 */

#ifndef BTCZONEMANAGER_H_
#define BTCZONEMANAGER_H_

#include <ZoneManager.h>

class BtcZoneManager: public ZoneManager
{
public:
    BtcZoneManager();
    virtual ~BtcZoneManager();

    /**
     * Config the hardware devices connected to this board.
     * @param controlledId
     * @param deviceType
     */
    virtual void configZoneManager(int controlledId, int deviceType);

};

#endif /* BTCZONEMANAGER_H_ */
