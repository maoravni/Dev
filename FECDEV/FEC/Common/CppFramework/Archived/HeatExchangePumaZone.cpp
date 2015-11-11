/*
 * HeatExchangePumaZone.cpp
 *
 *  Created on: 12 Feb 2013
 *      Author: maora
 */

#include "HeatExchangePumaZone.h"

HeatExchangePumaZone::HeatExchangePumaZone()
{

}

HeatExchangePumaZone::~HeatExchangePumaZone()
{
}

void HeatExchangePumaZone::executeHeaters(U32 htxTemp, U32 spHigh, U32 spLow)
{
    U32 PumaMv = cfgGetParam(m_mwMvFromPuma);
    activateHeaters(PumaMv);
    m_blowerActive = true;
}
