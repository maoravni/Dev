/*
 * HeaterBase.cpp
 *
 *  Created on: 21 Apr 2013
 *      Author: maora
 */

#include "HeaterBase.h"

HeaterBase::HeaterBase()
{
}

HeaterBase::~HeaterBase()
{
}

bool HeaterBase::configValues(unsigned short temperature, short lowWarn, short lowRange, short highRange,
        short highWarn, U32 sn)
{
    return true;
}

STATUS HeaterBase::setPowerLimit(unsigned short powerLimit)
{
    return 0;
}



