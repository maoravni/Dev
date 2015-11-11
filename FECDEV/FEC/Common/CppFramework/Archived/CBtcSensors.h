/*
 * CBtcSensors.h
 *
 *  Created on: Mar 17, 2013
 *      Author: yossis
 */

#ifndef CBTCSENSORS_H_
#define CBTCSENSORS_H_

#include "CBtc.h"
#include <MessageStructs.h>

class CSensState: public BaseMiddlewareWrapperZone
{
private:
    bool m_state;
    bool m_OkState;
protected:
    U32 m_mwState;
    bool m_OpositeInput;
    bool m_OpositeForOk;

public:

    CSensState(U32 Mw_State,bool OpositeInput=false,bool OpositeForOk=false)
    {
        m_mwState = Mw_State;
        m_OpositeInput=OpositeInput;
        m_OpositeForOk=OpositeForOk;
    }

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_DigitalInput;
    }
    virtual void executeZone()
    {
    }
    virtual void sendZoneStatus();

    bool GetState()
    {
        return m_state;
    }

    bool GetOkState()
    {
        return m_OkState;
    }

    bool isState()
    {
        if (m_OpositeInput)
            m_state = (cfgGetFromSource(m_mwState) == 0);
        else
            m_state = (cfgGetFromSource(m_mwState) == 1);


        if (m_OpositeForOk)
            m_OkState=!m_state;
        else
            m_OkState=m_state;

        return m_state;
    }

    virtual void configSampleInterval(unsigned int interval){}

};

//class CDrawerPositionState: public CSensState
//{
//
//public:
//    CDrawerPositionState(U32 Mw_State = MW_BTC_DRAWER_POSITION_STATE)
//    {
//        m_mwState = Mw_State;
//
//    }
//
//};
//
//class CWaterTankLowLevelState: public CSensState
//{
//
//public:
//    CWaterTankLowLevelState(U32 Mw_State = MW_BTC_WATER_TANK_LOW_LEVEL_STATE)
//    {
//        m_mwState = Mw_State;
//    }
//};

//class CWaterTankUpState: public CSensState
//{
//
//public:
//
//    CWaterTankUpState(U32 Mw_State = MW_BTC_WATER_TANK_UP_STATE)
//    {
//        m_mwState = Mw_State;
//    }
//};

//class CWaterTankOverflowState: public CSensState
//{
//public:
//
//    CWaterTankOverflowState(U32 Mw_State = MW_BTC_WATER_TANK_OVERFLOW_STATE)
//    {
//        m_mwState = Mw_State;
//    }
//};

//class CBlanketStopState: public CSensState
//{
//public:
//
//    CBlanketStopState(U32 Mw_State = MW_BTC_BLANKET_STOP_STATE)
//    {
//        m_mwState = Mw_State;
//    }
//};

class CTankLevelSens: public BaseMiddlewareWrapperZone
{

public:

    CBtc * m_Btc;

    virtual void executeZone();

    virtual void sendZoneStatus();

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_TankLevelSens;
    }

    virtual void configSampleInterval(unsigned int interval){};

};

class CEngageState: public BaseMiddlewareWrapperZone
{
private:
    bool m_EngageState;
    bool m_DisEngageState;
    U32 m_mwState;

    bool m_state1;
    bool m_state2;
    bool m_state3;
    bool m_state4;
    U32 m_mwState1;
    U32 m_mwState2;
    U32 m_mwState3;
    U32 m_mwState4;

public:

    virtual void executeZone()
    {

    }
    virtual void sendZoneStatus()
    {

    }
    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_DigitalInput;
    }

    CEngageState(U32 Mw_State = ID_BTC_MNTR_INT_ENGAGE, U32 Mw_State1 = MW_BTC_ENGAGE_STATE_1, U32 Mw_State2 =
            MW_BTC_ENGAGE_STATE_2, U32 Mw_State3 = MW_BTC_ENGAGE_STATE_3, U32 Mw_State4 = MW_BTC_ENGAGE_STATE_4);

    virtual bool isEngage();
    virtual bool isDisengage();

    bool isDisEngageState() const
    {
        return m_DisEngageState;
    }

    bool isEngageState() const
    {
        return m_EngageState;
    }

    bool isState1() const
    {
        return m_state1;
    }

    bool isState2() const
    {
        return m_state2;
    }

    bool isState3() const
    {
        return m_state3;
    }

    bool isState4() const
    {
        return m_state4;
    }
    virtual void configSampleInterval(unsigned int interval){};

};

//class CAirKnifePressureNormalState: public CSensState
//{
//
//public:
//
//    CAirKnifePressureNormalState(U32 Mw_State = MW_BTC_AIR_KNIFE_PRESSURE_NORMAL_STATE)
//    {
//        m_mwState = Mw_State;
//    }
//
//};

//class CTubOverflowState: public CSensState
//{
//public:
//
//    CTubOverflowState(U32 Mw_State = MW_BTC_WATER_TUB_OVERFLOW_STATE)
//    {
//        m_mwState = Mw_State;
//    }
//
//};

class CValueState: public BaseMiddlewareWrapperZone
{
private:
    U32 m_value;
protected:
    U32 m_mwValue;

public:
    CValueState(U32 mwValue) :
            m_mwValue(mwValue)
    {
        m_state = E_ZoneState_Ready;
    }

    virtual void executeZone()
    {
    }
    virtual void sendZoneStatus()
    {
        TmcMessageStruct message;
        PSSWaterContaminationStatusNotificationMsg *payload = &message.payload.pSSWaterContaminationStatusNotificationMsg;

        payload->cableId = m_controllerId;
        payload->deviceId = m_zoneId;
        payload->contamination = getValue();
        payload->status = 0;
    }

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_AnalogInput;
    }

    bool Value()
    {
        return m_value;
    }

    U32 getValue()
    {
        m_value = (cfgGetFromSource(m_mwValue) == 1);
        return m_value;
    }

    U32 getRamValue()
        {
            m_value = (cfgGetFromRam(m_mwValue) == 1);
            return m_value;
        }

    virtual void configSampleInterval(unsigned int interval){}

};

class CTemperatureState: public CValueState
{
public:
    U32 m_value;

    CTemperatureState(U32 mwValue) : CValueState(mwValue)
    {
    }

    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_TemperatureSensor;
    }


    virtual void sendZoneStatus()
    {
        T_TemperatureZoneMonitorReply monitorReply;
        monitorReply.status = getStatusStruct();
        monitorReply.temperature = getRamValue();
        sendMonitorReply(E_DataType_Status | E_DataType_Temperature, (char*) &monitorReply,
                sizeof(T_TemperatureZoneMonitorReply));
    }
};

//class CWaterTankTemperature: public CValueState
//{
//
//public:
//
//    CWaterTankTemperature(U32 mwValue = MW_BTC_WATER_TANK_TEMPERATURE)
//    {
//        m_mwValue = mwValue;
//    }
//
//};
//
//class CWaterLineTemperature: public CValueState
//{
//public:
//
//    CWaterLineTemperature(U32 mwValue = MW_BTC_WATER_LINE_IN_TEMPERATURE)
//    {
//        m_mwValue = mwValue;
//    }
//};
//
//class CBlanketTemperature: public CValueState
//{
//public:
//    virtual E_ZoneType getZoneType()
//    {
//        return E_ZoneType_BtcBlanketTemperature;
//    }
//
//    CBlanketTemperature(U32 mwValue = MW_BTC_BLANKET_TEMPERATURE)
//    {
//        m_mwValue = mwValue;
//    }
//
//};
//
//class CWaterContamination: public CValueState
//{
//public:
//    virtual E_ZoneType getZoneType()
//    {
//        return E_ZoneType_AnalogInput;
//    }
//
//    CWaterContamination(U32 mwValue = MW_BTC_WATER_CONTAMINATION)
//    {
//        m_mwValue = mwValue;
//    }
//
//};
//
inline bool CEngageState::isDisengage()
{
    m_state1 = (cfgGetFromSource(m_mwState1) == 1);
    m_state2 = (cfgGetFromSource(m_mwState2) == 1);
    m_state3 = (cfgGetFromSource(m_mwState3) == 1);
    m_state4 = (cfgGetFromSource(m_mwState4) == 1);
    bool m_DisengageState = !(m_state1 | m_state2 | m_state3 | m_state4); //all 4 sensors are reset
    return m_DisengageState;
}

inline bool CEngageState::isEngage()
{
    m_state1 = (cfgGetFromSource(m_mwState1) == 1);
    m_state2 = (cfgGetFromSource(m_mwState2) == 1);
    m_state3 = (cfgGetFromSource(m_mwState3) == 1);
    m_state4 = (cfgGetFromSource(m_mwState4) == 1);
    bool EngageState = m_state1 & m_state2 & m_state3 & m_state4; //all 4 sensors are set
    if (cfgPutParam(m_mwState, EngageState) == OK)
        m_EngageState = EngageState;

    return m_EngageState;
}

inline CEngageState::CEngageState(U32 Mw_State, U32 Mw_State1, U32 Mw_State2, U32 Mw_State3, U32 Mw_State4)
{
    m_mwState = Mw_State;
    m_mwState1 = Mw_State1;
    m_mwState2 = Mw_State2;
    m_mwState3 = Mw_State3;
    m_mwState4 = Mw_State4;
}

#endif
