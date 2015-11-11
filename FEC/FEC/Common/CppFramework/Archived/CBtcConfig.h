/*
 * CBtcConfig.h
 *
 *  Created on: Mar 17, 2013
 *      Author: yossis
 */

#ifndef CBTCCONFIG_H_
#define CBTCCONFIG_H_

//  CONFIGURATION

class CConfig: public MiddlewareAccessor
{
private:
    U32 m_CfgValue;
protected:
    U32 m_mwValue;

public:
    bool Value()
    {
        return m_CfgValue;
    }

    U32 getValue()
    {
        m_CfgValue = (cfgGetFromSource(m_mwValue) == 1);
        return m_CfgValue;
    }
};

class CWaterTankHighTemperatureLimitCfg: public CConfig
{
public:

    CWaterTankHighTemperatureLimitCfg(U32 MwValue = ID_BTC_CFG_INT_WATER_TANK_HIGH_TEMPERATURE_LIMIT)
    {
        m_mwValue = MwValue;
    }
};

class CWaterLineHighTemperatureLimitCfg: public CConfig
{

public:
    CWaterLineHighTemperatureLimitCfg(U32 MwValue = ID_BTC_CFG_INT_WATER_LINE_HIGH_TEMPERATURE_LIMIT)
    {
        m_mwValue = MwValue;
    }

};

#endif
