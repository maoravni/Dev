/*
 * HeatExchangePumaZone.h
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#ifndef HEATEXCHANGEPUMAZONE_H_
#define HEATEXCHANGEPUMAZONE_H_

#include "HeatExchangeZone.h"

class HeatExchangePumaZone: public HeatExchangeZone
{
protected:
    U32 m_mwMvFromPuma;

public:
    HeatExchangePumaZone();
    virtual ~HeatExchangePumaZone();

    virtual void executeHeaters(U32 htxTemp, U32 spHigh, U32 spLow);

    void setMwMvFromPuma(U32 mwMvFromPuma)
    {
        m_mwMvFromPuma = mwMvFromPuma;
    }
};

#endif /* HEATEXCHANGEPUMAZONE_H_ */
