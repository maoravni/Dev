/*
 * HeaterOutputSetpoint.h
 *
 *  Created on: 7 May 2013
 *      Author: maora
 */

#ifndef HEATEROUTPUTSETPOINT_H_
#define HEATEROUTPUTSETPOINT_H_

#include "HeaterInternalPid.h"

class HeaterOutputSetpoint : public HeaterInternalPid
{
    HeaterInternalPid* m_outputDevice;
    int m_lastPower;

public:
    HeaterOutputSetpoint();
    virtual ~HeaterOutputSetpoint();

    void setOutputDevice(HeaterInternalPid* outputDevice)
    {
        m_outputDevice = outputDevice;
    }

    virtual void sendZoneStatus();
protected:
    virtual void setOutputPower(int power);
};

#endif /* HEATEROUTPUTSETPOINT_H_ */
