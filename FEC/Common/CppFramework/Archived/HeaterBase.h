/*
 * HeaterBase.h
 *
 *  Created on: 21 Apr 2013
 *      Author: maora
 */

#ifndef HEATERBASE_H_
#define HEATERBASE_H_

#include "BaseMiddlewareWrapperZone.h"

class HeaterBase: public BaseMiddlewareWrapperZone
{
protected:
    U32 m_autoTuneSn;

public:
    HeaterBase();
    virtual ~HeaterBase();

    virtual bool configValues(unsigned short temperature, short lowWarn, short lowRange, short highRange, short highWarn, U32 sn);
    virtual STATUS setPowerLimit(unsigned short powerLimit);
    virtual STATUS setMaxTemperature(unsigned short maxTemperature) = 0;
    virtual E_AckStatus startAutoTune(U16 setpoint, U16 overshoot, U16 powerStart, U16 powerStep, U16 lookback, bool reverseDirection, U32 sn) = 0;
    virtual void sendPidValues() = 0;
    virtual void configPidValues(float kp, float ki, float kd, short setpointSmoothing, float iThreshold, unsigned short outputSmoothing) = 0;

};

#endif /* HEATERBASE_H_ */
