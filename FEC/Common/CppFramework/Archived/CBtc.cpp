/*
 * CBtc.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: yossis
 */

#include "Logger.h"
#include "opto_out_cnfg.h"
#include "CBtc.h"
#include "HW_pwm.h"
#include "pwm.h"
#include "TmcMessageHandler.h"
#include <TmcMasterServer.h>

U32 cBtc_stack_wm;

bool CBtc::HandleNotificationsAndAlarms()
{

    //handle drawer position led notification
    if (m_DrawerPositionState.isState())
        m_DrawerInPlaceIndicationLedComponent.setCompEnable(true);
    else
        m_DrawerInPlaceIndicationLedComponent.setCompEnable(false);

    if (m_BlanketStopState.isState())
    {
        m_GlobalError.setError(E_TmsErrors_Emergency, true);
        setCommand(BTC_COMMANDS_DISABLE_COOLING); //stop cooling
    }
    else
    {
        m_GlobalError.setError(E_TmsErrors_Emergency, false);
    }

#if 0
    //water tank exceed temperature handling
    if (m_WaterTankTemperature.getValue()> m_WaterTankHighTemperatureLimitCfg.getValue())
    m_WaterTankExceedTemperatureError.setError(true);
    else
    m_WaterTankExceedTemperatureError.setError(false);
    //M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"m_WaterTankExceedTemperatureError=%d",m_WaterTankExceedTemperatureError.isError());

    //water line exceed temperature
    if (m_WaterLineTemperature.getValue()> m_WaterLineHighTemperatureLimitCfg.getValue())
    m_WaterLineExceedTemperatureError.setError(true);
    else
    m_WaterTankExceedTemperatureError.setError(false);
    //M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"m_WaterTankExceedTemperatureError=%d",m_WaterTankExceedTemperatureError.isError());
#endif

    return false;
}

bool CBtc::FillWaterTankHandeling()
{
    bool RetError;

    RetError = m_TankWaterFillComponent.StateHandle(m_WaterTankLowLevelState.isState(), m_WaterTankUpState.isState(),
            m_WaterTankOverflowState.isState(), &localState, &m_GlobalError); //handle water fill states

    return RetError;

}

bool CBtc::EngageHandeling()
{
    bool RetError;

    RetError = m_EngageComponent.StateHandle(m_EngageState.isEngage(), m_EngageState.isDisengage(),
            m_DrawerPositionState.isState(), &localState, &m_GlobalError);
    return RetError;
}

bool CBtc::AirKnifePressureHandeling()
{
    bool RetError;

    RetError = m_AirKnifePressureValveComponent.StateHandle(m_AirKnifePressureNormalState.isState(), &localState,
            &m_GlobalError);
    return RetError;
}

bool CBtc::DrainWaterTankHandeling()
{
    if (m_TankWaterDrainComponent.isCmndEnable())
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "DrainWaterTankHandeling ON");
        m_TankWaterDrainComponent.setCompEnable(true);
    }
    else
        m_TankWaterDrainComponent.setCompEnable(false);

    return true;
}

bool CBtc::FillPumpHandeling()
{
    bool RetError;

    RetError = m_FillPumpComponent.StateHandle(m_TubOverflowState.isState(), m_EngageState.isEngage(),
            m_DrawerPositionState.isState(), &localState, &m_GlobalError);

    return RetError;
}

bool CBtc::DrainPumpHandeling()
{
    if (m_DrainPumpComponent.isCmndEnable())
    {
        m_DrainPumpComponent.setCompEnable(true);
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "DrainPumpHandeling ON");
    }
    else
    {
        m_DrainPumpComponent.setCompEnable(false);

    }
    return ll_ok;
}

bool CBtc::StartHandeling()
{
    static portTickType EndTime;

    switch (m_startCoolingHandelingState)
    {
    case START_PRESTART:
        if (!m_DrawerPositionState.isState())
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "start cooling->ERROR- Drawer position");
            m_GlobalError.setError(E_TmsErrors_Drawer, true);
            return ll_error;
        }
        else
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "start cooling->START_FILL_WATER");
            m_startCoolingHandelingState = START_FILL_WATER;
            m_TankWaterFillComponent.Start(); //start fill water process
            m_GlobalError.setError(E_TmsErrors_Drawer, false);
        }
        break;
    case START_FILL_WATER:

        if (FillWaterTankHandeling() == ll_error)
        { //error in filling tank process
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "start cooling->Error from FillWaterTankHandeling");
            return ll_error;
        }
        else
        {
            if (m_WaterTankLowLevelState.isState())  //wait until above low level
            {
                m_startCoolingHandelingState = START_KNIFE_AIR;
                M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "start cooling->START_KNIFE_AIR");
            }
        }
        break;
    case START_KNIFE_AIR:
        FillWaterTankHandeling();
        m_AirKnifePressureValveComponent.restart(&m_GlobalError);
        m_AirKnifePressureValveComponent.setCmndEnable(true);
        if (AirKnifePressureHandeling() == ll_error)
        { //error in filling tank process
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "start cooling->Error from FillWaterTankHandeling");
            return ll_error;
        }
        else if (m_AirKnifePressureNormalState.isState())
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "start cooling->START_ENGAGE");
            m_startCoolingHandelingState = START_ENGAGE;
        }
        break;
    case START_ENGAGE:
        FillWaterTankHandeling();
        AirKnifePressureHandeling();
        m_EngageComponent.restart(&m_GlobalError);
        m_EngageComponent.setCmndEnable(true);
        if (EngageHandeling() == ll_error)
        { //error in engage tank process
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "start cooling->Error from EngageHandeling");
            return ll_error;
        }
        else if (m_EngageState.isEngage())
        {
            m_startCoolingHandelingState = START_DRAIN_PUMP;
            EndTime = xTaskGetTickCount() + 5000; //before activate fill pump -> tbd (get it from global param) wait 5 seconds
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "start cooling->START_DRAIN_PUMP EndTime=%d",EndTime);
        }
        break;
    case START_DRAIN_PUMP:
        FillWaterTankHandeling();
        AirKnifePressureHandeling();
        EngageHandeling();
        m_DrainPumpComponent.setCmndEnable(true);
        DrainPumpHandeling();
        if (xTaskGetTickCount() > EndTime)
        {
            EndTime = xTaskGetTickCount() + 5000; //before activate blanket (mark completed) -> tbd (get it from global param) wait 5 seconds
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "start cooling->START_FILL_PUMP. Time=%d, EndTime=%d",xTaskGetTickCount(),EndTime);
            m_startCoolingHandelingState = START_FILL_PUMP;

        }
        break;
    case START_FILL_PUMP:
        FillWaterTankHandeling();
        AirKnifePressureHandeling();
        EngageHandeling();
        m_FillPumpComponent.setCmndEnable(true);
        if (FillPumpHandeling() == ll_error)
        { //error in engage tank process
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "start cooling->Error from FillPumpHandeling");
            return ll_error;
        }
        else if (xTaskGetTickCount() > EndTime)
        {
            EndTime = xTaskGetTickCount() + 5000; //tbd (get it from global param) wait 5 seconds before mark ready
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "start cooling->START_COOLING_COMPLETED. Time=%d, EndTime=%d",xTaskGetTickCount(),EndTime);
            m_startCoolingHandelingState = START_COOLING_COMPLETED;
        }
        break;
//    case START_FILL_PUMP_DELAY:
//        FillWaterTankHandeling();
//        FillPumpHandeling();
//        if (xTaskGetTickCount() > EndTime)
//        {
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "start cooling->Tank water is ready ---> Completed");
//            m_startCoolingHandelingState = START_COOLING_COMPLETED;
//        }
//        break;
    case START_COOLING_COMPLETED:
        FillWaterTankHandeling();
        AirKnifePressureHandeling();
        EngageHandeling();
        FillPumpHandeling();
        break;
    }
    return ll_ok;
}

bool CBtc::StopHandeling()
{

    static portTickType EndTime;

    switch (m_stopCoolingHandelingState)
    {
    case STOP_PRESTART:
        //clear all errors
        m_GlobalError.clearError();
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "stop cooling->Water fill suspended");
        m_TankWaterFillComponent.setCompEnable(false); //stop fill water
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "stop cooling->Fill pump suspended");
        m_FillPumpComponent.setCompEnable(false); //stop fill pump

        EndTime = xTaskGetTickCount() + 5000; //before stoping drain pump -> tbd (get it from global param) wait 5 seconds
        m_stopCoolingHandelingState = STOP_DRAIN_PUMP;
        break;

    case STOP_DRAIN_PUMP:
//         if (FillPumpHandeling()==ll_error)
//        { //error in engage tank process
//           M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "stop cooling->Error from FillPumpHandeling");
//           return ll_error;
//        }
        if (xTaskGetTickCount() > EndTime)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "stop cooling->STOP_DRAIN_PUMP");
            m_DrainPumpComponent.setCompEnable(false); //stop drain pump
            m_stopCoolingHandelingState = STOP_ENGAGE;
            m_EngageComponent.restart(&m_GlobalError);
            m_EngageComponent.setCmndEnable(false);
        }
        break;
    case STOP_ENGAGE:
        if (EngageHandeling() == ll_error)
        { //error in engage tank process
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Stop cooling->Error from EngageHandeling");
            return ll_error;
        }
        else if (m_EngageState.isDisengage())
        {
            m_stopCoolingHandelingState = STOP_AIR_KNIFE;
            m_AirKnifePressureValveComponent.setCompEnable(false);
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "Stop cooling->STOP_AIR_KNIFE");
        }
        break;
    case STOP_AIR_KNIFE:
        m_stopCoolingHandelingState = STOP_COOLING_COMPLETED;
        break;

    case STOP_COOLING_COMPLETED:
        break;
    }
    return ll_ok;
}

void CBtc::clearAllEnables()
{
    //m_EngageComponent.restart(&m_GlobalError);
    m_EngageComponent.setCmndEnable(false);
    m_TankWaterFillComponent.Suspend(&m_GlobalError);
    m_TankWaterFillComponent.setCmndEnable(false);
    m_AirKnifePressureValveComponent.setCmndEnable(false);
    m_AirKnifePressureValveComponent.restart(&m_GlobalError);
    m_DrawerInPlaceIndicationLedComponent.setCmndEnable(false);
    m_TankWaterDrainComponent.setCmndEnable(false);
    m_FillPumpComponent.setCmndEnable(false);
    m_DrainPumpComponent.setCmndEnable(false);
    m_GlobalError.clearError();
}

bool CBtc::handleHostCommand(BTC_COMMANDS command)
{
    if (command == hostCommand)
        return true; //already served

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "handleHostCommand command=%d. localState=%d", command,localState);

    switch (command)
    {
    case BTC_COMMANDS_NONE:
        break; //do nothing
    case BTC_COMMANDS_ENABLE_COOLING:
        if ((localState == BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_ENABLE_COOLING  - Not allowed in Service mode. localState=%d", localState);
            return false; //NACK if in service mode or not in error state
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_ENABLE_COOLING");
        m_startCoolingHandelingState = START_PRESTART;

        m_EngageComponent.reset();
        m_AirKnifePressureValveComponent.restart(&m_GlobalError);
        m_FillPumpComponent.restart(&m_GlobalError);
        localState = BTC_STATE_MOVE2RDY;  //start cooling system
        break;

    case BTC_COMMANDS_DISABLE_COOLING:
        /*  if ((localState == BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
         {
         M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_DISABLE_COOLING  - Not allowed in Service mode. localState=%d",localState);
         return false; //NACK if in service mode or not in error state
         }*/
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_DISABLE_COOLING");
        clearAllEnables();
        m_stopCoolingHandelingState = STOP_PRESTART;
        localState = BTC_STATE_ON;  //stop cooling system
        break;

    case BTC_COMMANDS_ENABLE_SERVICE_MODE:
        if (localState != BTC_STATE_ON)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_ENABLE_SERVICE_MODE  - Must be in ON state. localState=%d",localState);
            return false; //NACK must be in halt
        }
        clearAllEnables();
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_ENABLE_SERVICE_MODE");

        sendSeqEnded(MSG_Enable, m_lastSn, m_controllerId, m_zoneId, E_SeqEndedStatus_Success);

        localState = BTC_STATE_SERVICE;
        break;

    case BTC_COMMANDS_DISABLE_SERVICE_MODE:
        if (localState != BTC_STATE_SERVICE)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_DISABLE_SERVICE_MODE  - must be in Service mode. localState=%d",localState);
            return false; //NACK
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_DISABLE_SERVICE_MODE");
        localState = BTC_STATE_MOVE2ON;
        break;

    case BTC_COMMANDS_SERVICE_ENABLE_FRESH_WATER:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_ENABLE_FRESH_WATER  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_ENABLE_FRESH_WATER");
        m_TankWaterFillComponent.Start();

        break;

    case BTC_COMMANDS_SERVICE_DISABLE_FRESH_WATER:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_DISABLE_FRESH_WATER  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_DISABLE_FRESH_WATER");
        m_TankWaterFillComponent.Suspend(&m_GlobalError);
        break;

    case BTC_COMMANDS_SERVICE_ENABLE_ENGAGE:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_ENABLE_ENGAGE  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }

        m_EngageComponent.setCmndEnable(true);
        m_EngageComponent.restart(&m_GlobalError);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_ENABLE_ENGAGE");
        break;

    case BTC_COMMANDS_SERVICE_DISABLE_ENGAGE:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_DISABLE_ENGAGE  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_DISABLE_ENGAGE");
        m_EngageComponent.setCmndEnable(false);
        m_EngageComponent.restart(&m_GlobalError);

        break;
    case BTC_COMMANDS_SERVICE_ENABLE_AIR_KNIFE:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_ENABLE_AIR_KNIFE  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_ENABLE_AIR_KNIFE");
        m_AirKnifePressureValveComponent.setCmndEnable(true);
        m_AirKnifePressureValveComponent.restart(&m_GlobalError);

        break;
    case BTC_COMMANDS_SERVICE_DISABLE_AIR_KNIFE:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_DISABLE_AIR_KNIFE  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }

        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_DISABLE_AIR_KNIFE");
        m_AirKnifePressureValveComponent.setCmndEnable(false);
        m_AirKnifePressureValveComponent.restart(&m_GlobalError);

        break;

    case BTC_COMMANDS_SERVICE_ENABLE_DRAIN_TANK:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_ENABLE_DRAIN_TANK  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_ENABLE_DRAIN_TANK");
        m_TankWaterDrainComponent.setCmndEnable(true);
        break;
    case BTC_COMMANDS_SERVICE_DISABLE_DRAIN_TANK:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_DISABLE_DRAIN_TANK  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_DISABLE_DRAIN_TANK");
        m_TankWaterDrainComponent.setCmndEnable(false);
        break;
    case BTC_COMMANDS_SERVICE_ENABLE_FILL_PUMP:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_ENABLE_FILL_PUMP  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }

        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_ENABLE_FILL_PUMP");
        m_FillPumpComponent.setCmndEnable(true);
        m_FillPumpComponent.restart(&m_GlobalError);
        break;

    case BTC_COMMANDS_SERVICE_DISABLE_FILL_PUMP:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_DISABLE_FILL_PUMP  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }

        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_DISABLE_FILL_PUMP");
        m_FillPumpComponent.setCmndEnable(false);
        m_FillPumpComponent.restart(&m_GlobalError);

        break;
    case BTC_COMMANDS_SERVICE_ENABLE_DRAIN_PUMP:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_ENABLE_DRAIN_PUMP  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }

        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_ENABLE_DRAIN_PUMP");
        m_DrainPumpComponent.setCmndEnable(true);
        break;
    case BTC_COMMANDS_SERVICE_DISABLE_DRAIN_PUMP:
        if ((localState != BTC_STATE_SERVICE) && (localState != BTC_STATE_ERROR))
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR BTC_COMMANDS_SERVICE_DISABLE_DRAIN_PUMP  - Not in Service mode localState=%d",localState);
            return false; //NACK
        }

        M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "BTC_COMMANDS_SERVICE_DISABLE_DRAIN_PUMP");
        m_DrainPumpComponent.setCmndEnable(false);
        break;
    default:
        return false;
    }

    hostCommand = command; //keep it

    return true; //ACK

}

void CBtc::StateMachineHandeling()
{
    switch (localState)
    {
    case BTC_STATE_ON:
        FillWaterTankHandeling();
        EngageHandeling();
        AirKnifePressureHandeling();
        DrainWaterTankHandeling();
        DrainPumpHandeling();
        FillPumpHandeling();
        HandleNotificationsAndAlarms();
        break;
    case BTC_STATE_STANDBY: //off do nothing just wait for commands
        HandleNotificationsAndAlarms();
        break;
    case BTC_STATE_SERVICE:
        // tbd check and add ack when completed on each of them according to the latest hostCommand and hostCommandEnable
        FillWaterTankHandeling();
        EngageHandeling();
        AirKnifePressureHandeling();
        DrainWaterTankHandeling();
        DrainPumpHandeling();
        FillPumpHandeling();

        HandleNotificationsAndAlarms();

        break;
    case BTC_STATE_MOVE2RDY:

        if (StartHandeling() == ll_error)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "StateMachineHandeling->Error from StartHandeling. move to BTC_STATE_ERROR");
            m_HostBtcStateCommand.setState(BTC_STATE_ERROR);
            localState = BTC_STATE_ERROR;
        }

        else if (m_startCoolingHandelingState == START_COOLING_COMPLETED)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "StateMachineHandeling->START_COOLING_COMPLETED move to BTC_STATE_RDY");
            localState = BTC_STATE_RDY;  //start cooling completed successfully go to ready state
            setCommand(BTC_COMMANDS_NONE);
        }
        break;
    case BTC_STATE_MOVE2ON:
        if (StopHandeling() == ll_error)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "StateMachineHandeling->Error from StopHandeling. move to BTC_STATE_ERROR");
            m_HostBtcStateCommand.setState(BTC_STATE_ERROR);
            localState = BTC_STATE_ERROR;
            sendSeqEnded(MSG_Stop, m_lastSn, m_controllerId, m_zoneId, E_SeqEndedStatus_Timeout);  //stop
        }
        else if (m_stopCoolingHandelingState == STOP_COOLING_COMPLETED)
        {
            localState = BTC_STATE_RDY;  //start cooling completed successfully go to ready state
            M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "StateMachineHandeling->STOP_COOLING_COMPLETED move to BTC_STATE_RDY");
            sendSeqEnded(MSG_Stop, m_lastSn, m_controllerId, m_zoneId, E_SeqEndedStatus_Success);  //stop
            setCommand(BTC_COMMANDS_NONE);
        }

        break;
    case BTC_STATE_RDY:
        if (FillWaterTankHandeling() == ll_error)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "StateMachineHandeling->Error from FillWaterTankHandeling. move to BTC_STATE_ERROR");
            localState = BTC_STATE_ERROR;
        }
        if (EngageHandeling() == ll_error)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "StateMachineHandeling->Error from EngageHandeling. move to BTC_STATE_ERROR");
            localState = BTC_STATE_ERROR;
        }
        if (AirKnifePressureHandeling() == ll_error)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "StateMachineHandeling->Error from AirKnifePressureHandeling. move to BTC_STATE_ERROR");
            localState = BTC_STATE_ERROR;
        }
        DrainWaterTankHandeling();
        DrainPumpHandeling();
        if (FillPumpHandeling() == ll_error)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "StateMachineHandeling->Error from FillPumpHandeling. move to BTC_STATE_ERROR");
            localState = BTC_STATE_ERROR;
        }

        HandleNotificationsAndAlarms();
        break;
    case BTC_STATE_ERROR:
        HandleNotificationsAndAlarms();

        setCommand(BTC_COMMANDS_NONE);
        //m_HostBtcStateCommand.setState(BTC_STATE_ERROR);
        //stay here until suitable command from host
        break;
    }
}

bool CBtc::setCommand(BTC_COMMANDS command)
{
    m_HostBtcStateCommand.setCommand(command);
}

BTC_STATE CBtc::getBtcState()
{
    return m_HostBtcStateCommand.getState();
}

CBtc::CBtc(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority) :
        m_DrawerPositionState(MW_BTC_DRAWER_POSITION_STATE), m_WaterTankLowLevelState(MW_BTC_WATER_TANK_LOW_LEVEL_STATE,
                true), m_TubOverflowState(MW_BTC_WATER_TUB_OVERFLOW_STATE, true,true), m_AirKnifePressureNormalState(
                MW_BTC_AIR_KNIFE_PRESSURE_NORMAL_STATE), m_BlanketStopState(MW_BTC_BLANKET_STOP_STATE), m_WaterTankOverflowState(
                MW_BTC_WATER_TANK_OVERFLOW_STATE, true), m_WaterTankUpState(MW_BTC_WATER_TANK_UP_STATE, true), m_WaterTankTemperature(
                MW_BTC_WATER_TANK_TEMPERATURE), m_WaterLineTemperature(MW_BTC_WATER_LINE_IN_TEMPERATURE), m_WaterContamination(
                MW_BTC_WATER_CONTAMINATION), m_TankWaterDrainComponent(MW_BTC_TANK_DRAIN_WATER_ENABLE,
                MW_BTC_CMND_TANK_DRAIN_WATER_ENABLE, BTC_COMMANDS_SERVICE_ENABLE_DRAIN_TANK,
                BTC_COMMANDS_SERVICE_DISABLE_DRAIN_TANK), m_DrainPumpComponent(MW_BTC_DRAIN_PUMP_ENABLE,
                MW_BTC_CMND_DRAIN_PUMP_ENABLE, BTC_COMMANDS_SERVICE_ENABLE_DRAIN_PUMP,
                BTC_COMMANDS_SERVICE_DISABLE_DRAIN_PUMP)
{
    //create the task
    create(pcName, usStackDepth, uxPriority);

}

CBtc::~CBtc()
{
}

portBASE_TYPE CBtc::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    //create and init the BTC MW DB
    BtcCfgInit();
    localState = BTC_STATE_ON; //default
    hostCommand = BTC_COMMANDS_NONE;
    m_HostBtcStateCommand.setCommand(hostCommand);
    clearAllEnables(); //clear all internal commands

    // create the message queue:
    if (m_BtcMessageQueue.create((unsigned portBASE_TYPE) M_BTC_QUEUE_SIZE,
            (unsigned portBASE_TYPE) sizeof(T_BtcQueueItem*)) == NULL)
        return false;

    //PWM init
    //hw_pwm_channel_init(FILL_PUMP_DIGITAL_OUTPUT);
    //hw_pwm_init(0,0,1000); //1 Khz = 1msec



   // Pwm_DualPower(BTC_DUAL_PWM_POWER_ID, true); //dual power for this output

    //export this object to its members
    m_EngageComponent.m_Btc = this;
    m_TankWaterFillComponent.m_Btc = this;
    m_AirKnifePressureValveComponent.m_Btc = this;
    m_TankWaterDrainComponent.m_Btc = this;
    m_FillPumpComponent.m_Btc = this;
    m_DrainPumpComponent.m_Btc = this;

    m_TankLevelSens.m_Btc = this;

    m_GlobalError.m_Btc = this;

    return true;
}

void CBtc::run()
{

    T_BtcQueueItem *pBtcQueueItem;

    for (;;)
    {
        // reset the watchdog:
        IWDG_ReloadCounter();

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
        cBtc_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif

        if (m_BtcMessageQueue.receive(&pBtcQueueItem, 500) == pdPASS)
        {
            //tbd handle message from queue
        }
        else

        { //time out --> periodic BTC handling

            //BTC commands handler
            if (!handleHostCommand(m_HostBtcStateCommand.getCommand())) //handle command from host
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "NACK from setHostCommand. Command=%d",m_HostBtcStateCommand.getCommand());

            //main states handling
            StateMachineHandeling();

        }

    }
}

STATUS CBtc::enableZone(bool enable)
{
    if (enable)
    {
        setCommand(BTC_COMMANDS_ENABLE_SERVICE_MODE);

    }
    else
        setCommand(BTC_COMMANDS_DISABLE_COOLING);

    return OK;
}

void CBtc::executeZone()
{
}

void CBtc::sendZoneStatus()
{
    if (getZoneId() == M_BASE_ZONE_UNASSIGNED_ID)
        return;

    // static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;
    PSSBTCControlStatusNotificationMsg &payload = replyMessage.payload.pSSBTCControlStatusNotificationMsg;

    replyMessage.header.id.split.id = MSG_BtcControlStatusNotification;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(PSSBTCControlStatusNotificationMsg);
    payload.cableId = m_controllerId;
    payload.deviceId = m_zoneId;

    payload.powerLimit = 0;
    payload.temperature = 0;
    T_ZoneStatus zoneStatus;
    zoneStatus.split.enabled = 1;
    zoneStatus.split.hasErrors = m_GlobalError.getError() != 0;
    zoneStatus.split.hasWarnings = 0;

    switch (localState)
    {
    case BTC_STATE_ON:
    case BTC_STATE_ERROR:
        zoneStatus.split.state = E_ZoneState_On;
        break;
    case BTC_STATE_STANDBY:
    case BTC_STATE_SERVICE:
        zoneStatus.split.state = E_ZoneState_Standby;
        break;
    case BTC_STATE_MOVE2RDY:
    case BTC_STATE_MOVE2ON:
        zoneStatus.split.state = E_ZoneState_Moving2Ready;
        break;
    case BTC_STATE_RDY:
        zoneStatus.split.state = E_ZoneState_Ready;
        break;
    }

    payload.status = zoneStatus.full;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "sendZoneStatus. localState=%d, state=%d",localState,zoneStatus.split.state);

//    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO,
//            "CEngageComponent::sendZoneStatus position=%d, valveState=%d,sensor1State=%d, sensor2State=%d,sensor3State=%d,sensor4State=%d,status=%d, ",
//            payload.position, payload.valveState, payload.sensor1State, payload.sensor2State, payload.sensor3State,
//            payload.sensor4State, payload.status);

    masterServerInstance.sendMessage(replyMessage);
}

BaseZone* CBtc::getEngageComponent()
{
    return &m_EngageComponent;
}

BaseZone* CBtc::getTankWaterFillComponent()
{
    return &m_TankWaterFillComponent;
}

BaseZone* CBtc::getAirKnifePressureValveComponent()
{
    return &m_AirKnifePressureValveComponent;
}

BaseZone* CBtc::getDrawerInPlaceIndicationLedComponent()
{
    return &m_DrawerInPlaceIndicationLedComponent;
}

BaseZone* CBtc::getTankWaterDrainComponent()
{
    return &m_TankWaterDrainComponent;
}

BaseZone* CBtc::getFillPumpComponent()
{
    return &m_FillPumpComponent;
}

BaseZone* CBtc::getDrainPumpComponent()
{
    return &m_DrainPumpComponent;
}

BaseZone* CBtc::getAirKnifePressureNormalState()
{
    return &m_AirKnifePressureNormalState;
}

BaseZone* CBtc::getDrawerPositionState()
{
    return &m_DrawerPositionState;
}

BaseZone* CBtc::getWaterTankLowLevelState()
{
    return &m_WaterTankLowLevelState;
}

BaseZone* CBtc::getWaterTankUpState()
{
    return &m_WaterTankUpState;
}

BaseZone* CBtc::getWaterTankOverflowState()
{
    return &m_WaterTankOverflowState;
}

BaseZone* CBtc::getBlanketStopState()
{
    return &m_BlanketStopState;
}

BaseZone* CBtc::getEngageState()
{
    return &m_EngageState;
}

BaseZone* CBtc::getTubOverflowState()
{
    return &m_TubOverflowState;
}

BaseZone* CBtc::getTankLevelSens()
{
    return &m_TankLevelSens;
}

BaseZone* CBtc::getWaterContamination()
{
    return &m_WaterContamination;
}

BaseZone* CBtc::getWaterTankTemperature()
{
    return &m_WaterTankTemperature;
}

void CBtc::reset()
{
    clearAllEnables();
    BaseZone::reset();
}

BaseZone* CBtc::getWaterLineTemperature()
{
    return &m_WaterLineTemperature;
}
