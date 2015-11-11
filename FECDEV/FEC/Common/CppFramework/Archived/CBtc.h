/*
 * CBtc.h
 *
 *  Created on: Feb 20, 2013
 *      Author: yossis
 */

#ifndef CBTC_H_
#define CBTC_H_

#include <AManagedTask.h>
#include "CQueue.h"

#include "CfgDB.h"
#include "BtcCfgDB.h"
#include "OthersCfgDB.h"
#include "TemperatureCfgDB.h"
#include "ll_typedef.h"
#include "BaseZone.h"
#include "BaseMiddlewareWrapperZone.h"

#include "CBtcDef.h"

#include "CBtcComponents.h"
#include "CBtcSensors.h"
#include "CBtcConfig.h"

#include <Logger.h>

#define M_BTC_QUEUE_SIZE 30

struct T_BtcQueueItem
{
    int MessageType;
    int MessageData;
};

//class CBtc;

//class CMw
//{
//public:
//    U32 cfgPutParam(U32 id, U32 value)
//    {
//        U32 status = OK;
//        status = CfgPutParam(0, id, &value, (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);
//        return status;
//    }
//
//    U32 cfgGetFromRam(U32 id)
//    {
//        U32 value;
//        CfgGetParam(0, id, &value, sizeof(value), CfgDB_GET_DIRECTLY_FROM_RAM);
//        return value;
//    }
//
//    U32 cfgGetFromSource(U32 id)
//    {
//        U32 value;
//        CfgGetParam(0, id, &value, sizeof(value), CfgDB_GET_DIRECTLY_FROM_SOURCE);
//        return value;
//    }
//};

class CBtcStateCommand: public MiddlewareAccessor
{
private:
    BTC_STATE m_state;
    BTC_COMMANDS m_command;
    U32 m_mwSetCommand;
    U32 m_mwCurrentState;
    MiddlewareAccessor m_MiddlewareAccessor;
public:
    void setState(BTC_STATE state)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "setState State=%d ", state);
        if (m_MiddlewareAccessor.cfgPutParam(m_mwCurrentState, state) == OK)
        {
            m_state = (BTC_STATE) state;

        }
    }

    BTC_STATE getState()
    {
        m_state = (BTC_STATE) m_MiddlewareAccessor.cfgGetFromRam(m_mwCurrentState);
        return m_state;
    }

    BTC_COMMANDS getCommand()
    {
        m_command = (BTC_COMMANDS) m_MiddlewareAccessor.cfgGetFromRam(m_mwSetCommand);
        return m_command;
    }

    void setCommand(BTC_COMMANDS command)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "setCommand command=%d ", command);
        if (m_MiddlewareAccessor.cfgPutParam(m_mwSetCommand, command) == OK)
        {
            m_command = (BTC_COMMANDS) command;

        }
    }

    void setMiddlewareAccessor(MiddlewareAccessor middlewareAccessor)
    {
        m_MiddlewareAccessor = middlewareAccessor;
    }

    CBtcStateCommand(U32 Mw_SetCommand = MW_BTC_SET_COMMAND, U32 Mw_CurrentState = MW_BTC_CURRENT_STATE)
    {

        m_mwSetCommand = Mw_SetCommand;
        m_mwCurrentState = Mw_CurrentState;

    }

};

#if 0 //handled by global error member
class CEngageError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CEngageError (U32 mwId=ID_BTC_MNTR_INT_ALARM_ENGAGE_TIME_OUT)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CWaterTankFillError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CWaterTankFillError (U32 mwId=MW_BTC_ERROR_FILL_WATER_TIME_OUT)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CAirKnifeError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CAirKnifeError (U32 mwId=MW_BTC_ERROR_AIR_KNIFE_TIME_OUT)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CWaterTankExceedTemperatureError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CWaterTankExceedTemperatureError (U32 mwId=ID_BTC_MNTR_INT_ALARM_WATER_TANK_EXCEED_TEMPREATURE)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CWaterLineExceedTemperatureError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CWaterLineExceedTemperatureError (U32 mwId=ID_BTC_MNTR_INT_ALARM_WATER_LINE_EXCEED_TEMPREATURE)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CWaterContminationError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CWaterContminationError (U32 mwId=MW_BTC_ERROR_WATER_CONTAMINATION)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CWaterTankOverflowError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CWaterTankOverflowError (U32 mwId=MW_BTC_ERROR_WATER_TANK_OVERFLOW)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CWaterTubOverflowError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CWaterTubOverflowError (U32 mwId=MW_BTC_ERROR_TUB_OVERFLOW)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CDrawerError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CDrawerError (U32 mwId=MW_BTC_ERROR_DRAWER)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

class CBlanketError: public MiddlewareAccessor
{
private:
    bool m_Error;
    U32 m_mwId;

public:

    CBlanketError (U32 mwId=MW_BTC_ERROR_BLANKET)
    {
        m_mwId=mwId;
    }

    void setError(bool error)
    {
        if (cfgPutParam(m_mwId,(U32)error)==OK) //update the MW
        m_Error = error;
    }

    bool isError()
    {
        m_Error=cfgGetFromRam(m_mwId);
        return m_Error;
    }
};

#endif

//CBTC CLASS

class CBtc: public AManagedTask, public BaseZone
{

    CQueue m_BtcMessageQueue;

private:

    enum START_STATE
    {
        START_PRESTART,
        START_FILL_WATER,
        START_KNIFE_AIR,
        START_ENGAGE,
        START_DRAIN_PUMP,
        START_FILL_PUMP,
        START_FILL_PUMP_DELAY,
        START_COOLING_COMPLETED
    };

    START_STATE m_startCoolingHandelingState;

    enum STOP_COOLING_STATE
    {
        STOP_PRESTART, STOP_DRAIN_PUMP, STOP_ENGAGE, STOP_AIR_KNIFE, STOP_COOLING_COMPLETED
    };

    STOP_COOLING_STATE m_stopCoolingHandelingState;

    //configuration
    CWaterTankHighTemperatureLimitCfg m_WaterTankHighTemperatureLimitCfg;
    CWaterLineHighTemperatureLimitCfg m_WaterLineHighTemperatureLimitCfg;

    BTC_COMMANDS hostCommand;
    BTC_STATE localState;



#if 0
    CEngageError m_EngageError;
    CWaterTankFillError m_TankWaterFillError;
    CAirKnifeError m_AirKnifeError;
    CWaterTankExceedTemperatureError m_WaterTankExceedTemperatureError;
    CWaterLineExceedTemperatureError m_WaterLineExceedTemperatureError;
    CWaterContminationError m_WaterContminationError;
    CWaterTankOverflowError m_WaterTankOverflowError;
    CWaterTubOverflowError m_TubOverflowError;
    CDrawerError m_DrawerError;
    CBlanketError m_BlanketError;
#endif

    bool StartHandeling();
    bool StopHandeling();

    bool HandleNotificationsAndAlarms();
    bool FillWaterTankHandeling();
    bool EngageHandeling();
    bool AirKnifePressureHandeling();
    bool DrainWaterTankHandeling();

    bool FillPumpHandeling();
    bool DrainPumpHandeling();

    void StateMachineHandeling();

    void clearAllEnables();
    bool handleHostCommand(BTC_COMMANDS command); //commands handling from host
    CBtcStateCommand m_HostBtcStateCommand;  //for host (SUM) interface
public:

    //Errors
     CGlobalError m_GlobalError;  //bit field error status format of the following

    //states
    CSensState m_DrawerPositionState;
    CSensState m_WaterTankLowLevelState;
    CSensState m_WaterTankUpState;
    CSensState m_WaterTankOverflowState;
    CSensState m_BlanketStopState;
    CEngageState m_EngageState;
    CSensState m_AirKnifePressureNormalState;
    CSensState m_TubOverflowState;
    CTemperatureState m_WaterTankTemperature;
    CTemperatureState m_WaterLineTemperature;
    CTankLevelSens m_TankLevelSens;
    CValueState m_WaterContamination;

    //Components
    CEngageComponent m_EngageComponent;
    CTankWaterFillComponent m_TankWaterFillComponent;  //activation
    CAirKnifePressureValveComponent m_AirKnifePressureValveComponent;
    CDrawerInPlaceIndicationLedComponent m_DrawerInPlaceIndicationLedComponent;
    CBasePumpComponent m_TankWaterDrainComponent;
    CFillPumpComponent m_FillPumpComponent;
    CBasePumpComponent m_DrainPumpComponent;

//interface methods for the BTC
    bool setCommand(BTC_COMMANDS command);
    BTC_STATE getBtcState();

    CBtc(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);
    virtual ~CBtc();

    void run();

    portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
            unsigned portBASE_TYPE uxPriority);

    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType()
    {
        return E_ZoneType_Cooling;
    }

    /**
     * Read zone values and send the values to the OPC. Called from the periodic monitor task.
     * @return error value. OK/0 on success.
     */
    virtual void executeZone();

    /**
     * send the zone status to the SUM.
     */
    virtual void sendZoneStatus();

    virtual void reset();

    virtual STATUS enableZone(bool enable);

    virtual BaseZone* getEngageComponent();
    virtual BaseZone* getTankWaterFillComponent();
    virtual BaseZone* getAirKnifePressureValveComponent();
    virtual BaseZone* getDrawerInPlaceIndicationLedComponent();
    virtual BaseZone* getTankWaterDrainComponent();
    virtual BaseZone* getFillPumpComponent();
    virtual BaseZone* getDrainPumpComponent();

    virtual BaseZone* getAirKnifePressureNormalState();
    virtual BaseZone* getDrawerPositionState();
    virtual BaseZone* getWaterTankLowLevelState();
    virtual BaseZone* getWaterTankUpState();
    virtual BaseZone* getWaterTankOverflowState();
    virtual BaseZone* getBlanketStopState();
    virtual BaseZone* getEngageState();
    virtual BaseZone* getTubOverflowState();
    virtual BaseZone* getTankLevelSens();
    virtual BaseZone* getWaterContamination();

    virtual BaseZone* getWaterTankTemperature();
    virtual BaseZone* getWaterLineTemperature();

    virtual void configSampleInterval(unsigned int interval){}

};

#endif /* CBTC_H_ */
