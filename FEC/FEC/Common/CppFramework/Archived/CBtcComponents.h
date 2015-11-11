/*
 * CBtcComponents.h
 *
 *  Created on: Mar 17, 2013
 *      Author: yossis
 */

#ifndef CBTCCOMPONENTS_H_
#define CBTCCOMPONENTS_H_

#include "CBtc.h"
#include <logger.h>
#include <opto_out.h>
class CBtc;

//// ERRORS

class CGlobalError: public MiddlewareAccessor
{
private:
    U32 m_Error;
    U32 error;
    U32 m_mwId;

public:

    CBtc * m_Btc;

    CGlobalError(U32 mwId = MW_BTC_ERROR_GLOBAL)
    {
        m_Btc=0;
        m_mwId = mwId;
        m_Error=0;
    }

    void setError(E_TmsErrors ErrorId, bool Set);

    U32 clearError();

    bool isError(E_TmsErrors ErrorId);


    U32 getError();
};

//  COMPONENTS

class CComponent: public BaseMiddlewareWrapperZone
{
protected:
    bool m_CompEnable;
    U32 m_CompValue;
    bool m_CmndEnable;
    U32 m_mwCompEnable;
    U32 m_mwCmndEnable;
public:
    virtual void executeZone()
    {
    }
    virtual void sendZoneStatus()
    {
    }

    virtual bool isCompEnable()
    {
        m_CompEnable = (cfgGetFromRam(m_mwCompEnable) != 0);
        return m_CompEnable;
    }

    virtual U32 getCompEnable()
    {
        m_CompValue = (cfgGetFromRam(m_mwCompEnable) != 0);
        return m_CompValue;
    }

    virtual bool isCmndEnable()
    {
        m_CmndEnable = (cfgGetFromRam(m_mwCmndEnable) != 0);
        return m_CmndEnable;
    }

    virtual void setCmndEnable(bool enable)
    {
        if (cfgPutParam(m_mwCmndEnable, enable) == OK)
            m_CompEnable = enable;

    }

    virtual void setCompEnable(bool enable)
    {
        if (cfgPutParam(m_mwCompEnable, enable) == OK)
            m_CompEnable = enable;
    }

    virtual void setCompEnable(U32 value)
    {
        if (cfgPutParam(m_mwCompEnable, value) == OK)
            m_CompValue = value;
    }
    virtual void configSampleInterval(unsigned int interval){}

};

class CTimeOutComponents: public CComponent
{

protected:
    int m_timeOut;
    U32 m_mwTimeOut;
    portTickType m_startTime;

public:

    virtual int getTimeOut()
    {
        m_timeOut = cfgGetFromRam(m_mwTimeOut);
        return (m_timeOut);
    }

    virtual void setTimeOut(int timeOut)
    {
        if (cfgPutParam(m_mwTimeOut, timeOut) == OK)
            m_timeOut = timeOut;
    }

};

class CBasePumpComponent: public CTimeOutComponents
{

protected:
    BTC_COMMANDS m_commandEnable;
    BTC_COMMANDS m_commandDisable;
public:
    bool m_InError;
    CBtc * m_Btc;

    virtual bool setCommand(bool enable, U32 Power);

    virtual void sendZoneStatus();

    virtual U32 GetPowerLimit()
    {
        return  (isCompEnable());
    }
    virtual bool IsStatusOk()
    {
        return m_InError;
    }

    //virtual U32 getPowerLimit();

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_Pump;
    }

//    CTankWaterDrainComponent(U32 Mw_CompEnable = MW_BTC_TANK_DRAIN_WATER_ENABLE, U32 Mw_CmndEnable =
//            MW_BTC_CMND_TANK_DRAIN_WATER_ENABLE)
    CBasePumpComponent(U32 Mw_CompEnable, U32 Mw_CmndEnable, BTC_COMMANDS enable, BTC_COMMANDS disable)
    {
        m_Btc=0;
        m_mwCompEnable = Mw_CompEnable;
        m_mwCmndEnable = Mw_CmndEnable;
        m_commandEnable = enable;
        m_commandDisable = disable;
        m_InError=false;
    }
};

class CTankWaterFillComponent: public CBasePumpComponent
{
    enum WATER_FILL_STATE
    {
        WATER_FILL_START = 0, WATER_FILL_STOP, WATER_FILL_ERROR, WATER_FILL_SUSPEND
    };

private:
    WATER_FILL_STATE m_WatereState;

public:

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_Pump;
    }

    virtual U32 GetPowerLimit()
    {
        return  (isCompEnable()); //0 or 1
    }

    virtual bool IsStatusOk()
    {
        return  m_InError; //0 or 1
    }


    CTankWaterFillComponent(U32 Mw_timeOut = MW_BTC_TANK_FILL_WATER_TIME_OUT) :
            CBasePumpComponent(MW_BTC_TANK_FILL_WATER_ENABLE, 0, BTC_COMMANDS_SERVICE_ENABLE_FRESH_WATER,
                    BTC_COMMANDS_SERVICE_DISABLE_FRESH_WATER)
    {
        m_mwTimeOut = Mw_timeOut;
        m_WatereState = WATER_FILL_SUSPEND;
    }
    ;

    void Start()
    {
        m_WatereState = WATER_FILL_STOP;
    }

    void Suspend(CGlobalError * pGlobalError)
    {
        m_WatereState = WATER_FILL_SUSPEND;
        setCompEnable(false); //stop filling water
        pGlobalError->setError(E_TmsErrors_WaterTankOverflow , false); //clear the error
        pGlobalError->setError(E_TmsErrors_WaterTankTimeOut , false); //clear the error
        m_InError=false;
    }
    //fill water periodic call
    bool StateHandle(bool WaterTankLowLevelState, bool WaterTankUpState, bool WaterTankOverflowState,
            BTC_STATE * localState, CGlobalError * pGlobalError)
    {
        switch (m_WatereState)
        {
        case WATER_FILL_STOP:
            if (!WaterTankOverflowState && !WaterTankLowLevelState && !WaterTankUpState)
            {
                setCompEnable(true); //turn on fill water
                m_startTime = xTaskGetTickCount();
                m_WatereState = WATER_FILL_START;
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "WATER FILL IN FILL START.m_startTime=%d ", m_startTime);
            }
            else if (WaterTankOverflowState)
            {
                setCompEnable(false); //stop filling water
                pGlobalError->setError(E_TmsErrors_WaterTankOverflow, true);
                m_InError=true;
                // *localState = BTC_STATE_ERROR;
                m_WatereState = WATER_FILL_ERROR;
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "WATER FILL IN OVEFLOW ERROR");
            }

            break;
        case WATER_FILL_START:
            if (WaterTankUpState)
            {
                setCompEnable(false); //stop filling water
                m_WatereState = WATER_FILL_STOP;
            }
            else if (WaterTankOverflowState)
            {
                setCompEnable(false); //stop filling water
                pGlobalError->setError(E_TmsErrors_WaterTankOverflow, true);
                m_InError=true;
                //*localState = BTC_STATE_ERROR;
                m_WatereState = WATER_FILL_ERROR;
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "WATER FILL IN OVEFLOW ERROR");
            }
            else
            {
                portTickType Time = xTaskGetTickCount();
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "WATER_FILL_START->Time=%d, StartTime=%d, ConfigTimeOut=%d ", Time,
                        m_startTime, getTimeOut() * 1000);
                if (Time > (m_startTime + getTimeOut() * 1000))
                {
                    setCompEnable(false); //stop filling water
                    pGlobalError->setError(E_TmsErrors_WaterTankTimeOut, true);
                    m_InError=true;;
                    //*localState = BTC_STATE_ERROR;
                    m_WatereState = WATER_FILL_ERROR;
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "WATER FILL IN TIMEOUT ERROR");
                }
            }
            break;
        case WATER_FILL_ERROR:
            m_WatereState = WATER_FILL_SUSPEND;
            return ll_error; //error

            break;
        case WATER_FILL_SUSPEND:
            if (WaterTankOverflowState)
                pGlobalError->setError(E_TmsErrors_WaterTankOverflow, true);
            else
                pGlobalError->setError(E_TmsErrors_WaterTankOverflow, false);

            break;
        default:
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "StateHandle state ERROR");
            break;
        }

        }; //switch

        return ll_ok;  //ok
    }
};

class CEngageComponent: public CTimeOutComponents
{

    typedef enum
    {
        ENGAGE_STOP, ENGAGE_START_ENGAGE, ENGAGE_START_DISENGAGE, ENGAGE_ERROR,
    } ENGAGE_STATE;

private:

    ENGAGE_STATE m_EngageState;

public:
    CBtc * m_Btc;

    bool setCommand(BTC_COMMANDS command);

    virtual void executeZone()
    {
//        sendZoneStatus();
    }
    virtual void sendZoneStatus();

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_BtcEngageSystem;
    }

    CEngageComponent(U32 Mw_CmndEnable = MW_BTC_CMND_ENGAGE_ENABLE, U32 Mw_CompEnable = MW_BTC_ENGAGE_ENABLE,
            U32 Mw_timeOut = MW_BTC_ENGAGE_TIME_OUT)
    {
        m_mwCmndEnable = Mw_CmndEnable;
        m_mwCompEnable = Mw_CompEnable;
        m_mwTimeOut = Mw_timeOut;
        m_EngageState = ENGAGE_STOP;
    }
    ;

    void restart(CGlobalError * pGlobalError)
    {
        pGlobalError->setError(E_TmsErrors_EngageTimeOut , false); //clear the error
        pGlobalError->setError(E_TmsErrors_Drawer, false); //clear the error
        m_EngageState = ENGAGE_STOP;
    }

    //Periodic call
    bool StateHandle(bool EngageState, bool DisengageState, bool DrawerState, BTC_STATE * localState,
            CGlobalError * pGlobalError)
    {

        switch (m_EngageState)
        {
        case ENGAGE_STOP:
            if (isCmndEnable())
            { //engage
                if (!EngageState)
                {
                    if (!DrawerState)
                    {
                        pGlobalError->setError(E_TmsErrors_Drawer, true);
                        //*localState = BTC_STATE_ERROR;
                        m_EngageState = ENGAGE_ERROR;
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ENGAGE IN, DRAWER ERROR");
                    }
                    else
                    {
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "ENGAGE_START_ENGAGE state");
                        setCompEnable(true); //engage
                        m_startTime = xTaskGetTickCount();
                        m_EngageState = ENGAGE_START_ENGAGE;
                    }
                }
            }
            else
            { //disengage
                if (!DisengageState)
                {
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "ENGAGE_START_DISENGAGE state");
                    setCompEnable(false); //disengage
                    m_startTime = xTaskGetTickCount();
                    m_EngageState = ENGAGE_START_DISENGAGE;
                }
            }

            break;
        case ENGAGE_START_ENGAGE:
            if (EngageState)
                m_EngageState = ENGAGE_STOP;
            else
            {
                portTickType Time = xTaskGetTickCount();
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "ENGAGE_START_ENGAGE->Time=%d, StartTime=%d, ConfigTimeOut=%d ",
                        Time, m_startTime, getTimeOut() * 1000);
                if (Time > (m_startTime + getTimeOut() * 1000))
                {
                    pGlobalError->setError(E_TmsErrors_EngageTimeOut , true);
                    //*localState = BTC_STATE_ERROR;
                    m_EngageState = ENGAGE_ERROR;
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ENGAGE IN TIMEOUT ERROR");
                }
            }
            break;
        case ENGAGE_START_DISENGAGE:
            if (DisengageState)
                m_EngageState = ENGAGE_STOP;
            else
            {
                portTickType Time = xTaskGetTickCount();
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "DISENGAGE_START_ENGAGE->Time=%d, StartTime=%d, ConfigTimeOut=%d ",
                        Time, m_startTime, getTimeOut() * 1000);
                if (Time > (m_startTime + getTimeOut() * 1000))
                {
                    pGlobalError->setError(E_TmsErrors_EngageTimeOut , true);
                    //*localState = BTC_STATE_ERROR;
                    m_EngageState = ENGAGE_ERROR;
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "DISENGAGE IN TIMEOUT ERROR");
                }
            }
            break;
        case ENGAGE_ERROR:
            return ll_error;
//            if (*localState == BTC_STATE_MOVE2RDY)
//            {
//                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Engage From Error 2 Move2Rdy -> Clear the errors");
//                pGlobalError->setError(BTC_GLOBAL_ERROR_ENGAGE_TIME_OUT, false); //clear the error
//                pGlobalError->setError(BTC_GLOBAL_ERROR_DRAWER, false); //clear the error
//
//                m_EngageState = ENGAGE_STOP;
//            }
            break;
        }

        return ll_ok;
    }
};

class CAirKnifePressureValveComponent: public CTimeOutComponents
{
    typedef enum
    {
        AIR_STOP, AIR_START_AIR, AIR_STOP_AIR, AIR_ERROR, AIR_SUSPEND,
    } AIR_STATE;
private:

    AIR_STATE m_AirState;
    int m_timeOut;

public:
    CBtc * m_Btc;

    virtual bool configValues(short value)
    {
        if (value != 0)
            setCommand(BTC_COMMANDS_SERVICE_ENABLE_AIR_KNIFE);
        else
            setCommand(BTC_COMMANDS_SERVICE_DISABLE_AIR_KNIFE);
    }

    bool setCommand(BTC_COMMANDS command);

    virtual void executeZone()
    {
    }
    virtual void sendZoneStatus();

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_Valve;
    }

    CAirKnifePressureValveComponent(U32 Mw_CmndEnable = MW_BTC_CMND_AIR_PRESSURE_KIFE_CMND_ENABLE, U32 Mw_Comp_enable =
            MW_BTC_AIR_PRESSURE_KIFE_ENABLE, U32 Mw_timeOut = MW_BTC_AIR_PRESSURE_KIFE_TIME_OUT)
    {
        m_mwCompEnable = Mw_Comp_enable;
        m_mwCmndEnable = Mw_CmndEnable;
        m_mwTimeOut = Mw_timeOut;
        m_AirState = AIR_SUSPEND;
    }

    void restart(CGlobalError * pGlobalError)
    {
        pGlobalError->setError(E_TmsErrors_AirKnifeTimeOut , false); //clear the error
        m_AirState = AIR_STOP;
    }
    //Air knife periodic call
    bool StateHandle(bool PressureState, BTC_STATE *localState, CGlobalError * pGlobalError)
    {

        switch (m_AirState)
        {
        case AIR_STOP:
            if (isCmndEnable())
            { //turn on
                //if (!PressureState)
                {
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "AIR PRESSURE START state");
                    setCompEnable(true); //turn on
                    m_startTime = xTaskGetTickCount();
                    m_AirState = AIR_START_AIR;
                }
            }
            else
            { //turn off
                //if (PressureState)
                {
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "AIR PRESSURE STOP state");
                    setCompEnable(false); //turn off
                    m_startTime = xTaskGetTickCount();
                    m_AirState = AIR_STOP_AIR;
                }
            }

            break;
        case AIR_START_AIR:
            if (PressureState)
                m_AirState = AIR_STOP;
            else
            {
                portTickType Time = xTaskGetTickCount();
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "AIR_START_AIR->Time=%d, StartTime=%d, ConfigTimeOut=%d ", Time,
                        m_startTime, getTimeOut() * 1000);
                if (Time > (m_startTime + getTimeOut() * 1000))
                {
                    pGlobalError->setError(E_TmsErrors_AirKnifeTimeOut , true);
                    //*localState = BTC_STATE_ERROR;
                    m_AirState = AIR_ERROR;
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "AIR_START TIMEOUT ERROR");
                }
            }
            break;
        case AIR_STOP_AIR:
            if (!PressureState)
                m_AirState = AIR_STOP;
            else
            {
                portTickType Time = xTaskGetTickCount();
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "AIR_STOP_AIR->Time=%d, StartTime=%d, ConfigTimeOut=%d ", Time,
                        m_startTime, getTimeOut() * 1000);
                if (Time > (m_startTime + getTimeOut() * 1000))
                {
                    pGlobalError->setError(E_TmsErrors_AirKnifeTimeOut , true);
                    //*localState = BTC_STATE_ERROR;
                    m_AirState = AIR_ERROR;
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "AIR_STOP IN TIMEOUT ERROR");
                }
            }
            break;
        case AIR_ERROR:
            m_AirState = AIR_SUSPEND;
            return ll_error; //error
            // setCompEnable(false); //turn off
            // setCmndEnable(false);
//            if (*localState == BTC_STATE_MOVE2RDY)
//            {
//                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Air From Error 2 Move2Rdy -> Clear the errors");
//                pGlobalError->setError(BTC_GLOBAL_ERROR_AIR_KNIFE_TIME_OUT, false); //clear the error
//                m_AirState = AIR_STOP;
//
//
//            }
            break;
        case AIR_SUSPEND:
            break;
        }

        return ll_ok;  //ok
    }
};

//class CDrainPumpComponent: public CComponent
//{
//public:
//
//    CBtc * m_Btc;
//
//    bool setCommand(BTC_COMMANDS command);
//
//    virtual E_ZoneType getZoneType()
//    {
//        return E_ZoneType_Pump;
//    }
//
//    CDrainPumpComponent(U32 Mw_CompEnable = MW_BTC_DRAIN_PUMP_ENABLE, U32 Mw_CmndEnable = MW_BTC_CMND_DRAIN_PUMP_ENABLE)
//    {
//        m_mwCompEnable = Mw_CompEnable;
//        m_mwCmndEnable = Mw_CmndEnable;
//    }
//};

class CDrawerInPlaceIndicationLedComponent: public CComponent
{

public:

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_Size; //not need to export - just for compilation pass
    }

    CDrawerInPlaceIndicationLedComponent(U32 Mw_CompEnable = MW_BTC_DRAWER_IN_PLACE_INDICATION_LED)
    {
        m_mwCompEnable = Mw_CompEnable;

    }
};



class CFillPumpComponent: public CBasePumpComponent
{
    typedef enum
    {
        FILL_PUMP_STOP, FILL_PUMP_START, FILL_PUMP_ERROR,
    } FILL_PUMP_STATE;

private:

    FILL_PUMP_STATE m_PumpState;

    U32 m_mwPowerLowLimit;
    U32 m_mwPowerHighLimit;
    U32 m_mwPower;

    U32 m_PowerLowLimit;
    U32 m_PowerHighLimit;
    U32 m_Power;
    bool m_InError;

    void SetPowerToHigh()
    {
        U32 HighLimitPower=getPowerHighLimit();
        if (getPower()!=HighLimitPower)
        {
            setPower(HighLimitPower);
            setCompEnable(HighLimitPower); //turn on
        }
    }

    void SetPowerToLow()
    {
        U32 LowLimitPower=getPowerLowLimit();
        if (getPower()!=LowLimitPower)
        {
            setPower(LowLimitPower);
            setCompEnable(LowLimitPower); //turn on
        }
    }

public:

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_Pump;
    }

    virtual U32 GetPowerLimit()
    {
        return  (getPower());
    }

    virtual bool IsStatusOk()
    {
        return  m_InError; //0 or 1
    }

    CFillPumpComponent(U32 Mw_PowerLowLimit = ID_BTC_CFG_INT_FILL_PUMP_LOW_POWER_LIMIT, U32 Mw_PowerHighLimit =
            ID_BTC_CFG_INT_FILL_PUMP_HIGH_POWER_LIMIT, U32 Mw_Power = ID_BTC_MNTR_INT_FILL_PUMP_POWER):
                CBasePumpComponent(MW_BTC_FILL_PUMP_POWER_SET, MW_BTC_CMND_FILL_PUMP_ENABLE,
                    BTC_COMMANDS_SERVICE_ENABLE_FILL_PUMP, BTC_COMMANDS_SERVICE_DISABLE_FILL_PUMP)
    {
        m_mwPowerLowLimit = Mw_PowerLowLimit;
        m_mwPowerHighLimit = Mw_PowerHighLimit;
        m_mwPower = Mw_Power;
        m_InError=false;

    }

    void setPowerLowLimit(U32 Power)
    {
        if (cfgPutParam(m_mwPowerLowLimit, Power) == OK)
            m_PowerLowLimit = Power;
    }

    U32 getPowerLowLimit()
    {
        m_PowerLowLimit = cfgGetFromRam(m_mwPowerLowLimit);
        return m_PowerLowLimit * 10;

    }
    void setPowerHighLimit(U32 Power)
    {
        if (cfgPutParam(m_mwPowerHighLimit, Power) == OK)
            m_PowerHighLimit = Power;
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "setPowerHighLimit. Power=%d",Power);

    }
    U32 getPowerHighLimit()
    {

        m_PowerHighLimit = cfgGetFromRam(m_mwPowerHighLimit);
        return m_PowerHighLimit * 10;
    }

    void setPower(U32 Power)
    {
        if (cfgPutParam(m_mwPower, Power) == OK)
            m_Power = Power;
        if (m_Power != 0)
            dig_out_ctrl(OPTO_OUT9, ACTIVE);
        else
            dig_out_ctrl(OPTO_OUT9, DEACTIVE);
    }
    U32 getPower()
    {
        m_Power = cfgGetFromRam(m_mwPower);
        return m_Power;
    }

    void restart(CGlobalError * pGlobalError)
    {
        setCompEnable(getPowerLowLimit()); //turn off - lowest power
        pGlobalError->setError(E_TmsErrors_TubOverflow , false); //clear the error
        pGlobalError->setError(E_TmsErrors_Drawer, false);
        pGlobalError->setError(E_TmsErrors_Engage , false);
        pGlobalError->setError(E_TmsErrors_EngageTimeOut , false);
        m_PumpState = FILL_PUMP_STOP;
        m_InError=false;
    }
//Periodic call
    bool StateHandle(bool TubOverflowState, bool EngageState, bool DrawerState, BTC_STATE * localState,
            CGlobalError * pGlobalError)
    {
        switch (m_PumpState)
        {
        case FILL_PUMP_STOP:
            if (isCmndEnable())
            { //turn on
                if (TubOverflowState /*|| !EngageState*/ || !DrawerState)
                { //sensors error
                    if (TubOverflowState)
                    {
                        pGlobalError->setError(E_TmsErrors_TubOverflow, true);
                        m_InError=true;
                    }
                    if (!DrawerState)
                    {
                        pGlobalError->setError(E_TmsErrors_Drawer, true);
                        m_InError=true;
                    }
//                    if (!EngageState)
//                    {
//                        pGlobalError->setError(E_TmsErrors_Engage , true);
//                        m_InError=true;
//                    }

                    //*localState = BTC_STATE_ERROR;
                    m_PumpState = FILL_PUMP_ERROR;
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "FILL PUMP ERROR");
                }
                else if (!TubOverflowState /*&& EngageState */&& DrawerState)
                { //sensors ok
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "FILL PUMP START state");
                    SetPowerToHigh();
                    m_PumpState = FILL_PUMP_START;
                }
            }
            else
            { //turn off
                SetPowerToLow();
            }

            break;
        case FILL_PUMP_START:
            if (!isCmndEnable())
            {
                SetPowerToLow();

                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "AIR PRESSURE STOP state");
                m_PumpState = FILL_PUMP_STOP;
            }
            else if (TubOverflowState /*|| !EngageState */|| !DrawerState)
            { //sensors error
                if (TubOverflowState)
                {
                    pGlobalError->setError(E_TmsErrors_TubOverflow , true);
                    m_InError=true;
                }
                if (!DrawerState)
                {
                    pGlobalError->setError(E_TmsErrors_Drawer , true);
                    m_InError=true;
                }
//                if (!EngageState)
//                {
//                    pGlobalError->setError(E_TmsErrors_Engage , true);
//                    m_InError=true;
//                }

                SetPowerToLow();  //turn off - lowest power

                //*localState = BTC_STATE_ERROR;
                m_PumpState = FILL_PUMP_ERROR;
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "FILL PUMP ERROR");
            }
            else
            {
                SetPowerToHigh();
            }
            break;
        case FILL_PUMP_ERROR:
            setPower(0);
            setCompEnable(false); //turn completed off
            setCmndEnable(false);
            m_InError=true;
            return ll_error;
//            if (*localState == BTC_STATE_MOVE2RDY)
//            {
//                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Air From Error 2 Move2Rdy -> Clear the errors");
//                pGlobalError->setError(BTC_GLOBAL_ERROR_TUB_OVERFLOW, false); //clear the error
//                pGlobalError->setError(BTC_GLOBAL_ERROR_DRAWER, false);
//                pGlobalError->setError(BTC_GLOBAL_ERROR_ENGAGE, false);
//                m_PumpState = FILL_PUMP_STOP;
//            }
            break;
        } //switch
        return ll_ok;
    }

};



#endif

