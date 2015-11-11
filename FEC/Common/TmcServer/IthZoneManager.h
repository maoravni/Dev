/*
 * IthZoneManager.h
 *
 *  Created on: Feb 13, 2013
 *      Author: yossis
 */

#ifndef ITHZONEMANAGER_H_
#define ITHZONEMANAGER_H_

#include <ZoneManager.h>

class IthZoneManager: public ZoneManager
{
public:
    IthZoneManager();
    virtual ~IthZoneManager();

    /**
     * Config the hardware devices connected to this board.
     * @param controlledId
     * @param deviceType
     */
    virtual void configZoneManager(int controlledId, int deviceType);

};

#endif /* ITHZONEMANAGER_H_ */
