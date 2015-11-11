/*
 * btc_def.h
 *
 *  Created on: Mar 17, 2013
 *      Author: yossis
 */

#ifndef BTC_DEF_H_
#define BTC_DEF_H_

//#if _NEW_BTC_APPROCH_
//commands

#define MW_BTC_CMND_ENGAGE_ENABLE                   ID_BTC_CMND_INT_ENGAGE_ENABLE
#define MW_BTC_CMND_AIR_PRESSURE_KIFE_CMND_ENABLE   ID_BTC_CMND_AIR_PRESSURE_KIFE_CMND_ENABLE
#define MW_BTC_CMND_TANK_DRAIN_WATER_ENABLE         ID_BTC_CMND_TANK_DRAIN_WATER_ENABLE
#define MW_BTC_CMND_FILL_PUMP_ENABLE                ID_BTC_CMND_INT_FILL_PUMP_ENABLE
#define MW_BTC_CMND_DRAIN_PUMP_ENABLE               ID_BTC_CMND_INT_DRAIN_PUMP_ENABLE

//components active

#define MW_BTC_AIR_PRESSURE_KIFE_ENABLE         (CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n,1)) //DO2
#define MW_BTC_ENGAGE_ENABLE                    (CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n,2)) //DO3
#define MW_BTC_DRAWER_IN_PLACE_INDICATION_LED   (CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n,4)) //DO5
#define MW_BTC_TANK_FILL_WATER_ENABLE           (CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n,6)) //DO7
#define MW_BTC_TANK_DRAIN_WATER_ENABLE          (CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n,7)) //DO8
#define MW_BTC_DRAIN_PUMP_ENABLE                (CfgDB_ID_AddIndex(ID_OTHERS_CMND_INT_DIGITAL_OUT_n,8)) //DO9

#define MW_BTC_FILL_PUMP_POWER_SET              (CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,10)) //PWM DO 11-12 (dual power)
#define BTC_DUAL_PWM_POWER_ID                   (((MW_BTC_FILL_PUMP_POWER_SET-ID_TEMPERATURE_MNTR_INT_ZONE_MV_n)>>CfgDB_ID_Id_BitFieldSize))

#define FILL_PUMP_DIGITAL_OUTPUT_1              OPTO_OUT11
#define FILL_PUMP_DIGITAL_OUTPUT_2              OPTO_OUT12

//configuration
#define MW_BTC_BLANKET_SP                        ID_BTC_CFG_INT_BLANKET_SP
#define MW_BTC_BLANKET_PID_P                     ID_BTC_CFG_INT_BLANKET_PID_P
#define MW_BTC_BLANKET_PID_I                     ID_BTC_CFG_INT_BLANKET_PID_I
#define MW_BTC_BLANKET_PID_D                     ID_BTC_CFG_INT_BLANKET_PID_P

#define MW_BTC_FILL_PUMP_POWER_LOW_LIMIT         ID_BTC_CFG_INT_FILL_PUMP_LOW_POWER_LIMIT
#define MW_BTC_FILL_PUMP_POWER_HIGH_LIMIT        ID_BTC_CFG_INT_FILL_PUMP_HIGH_POWER_LIMIT
#define MW_BTC_FILL_PUMP_POWER                   ID_BTC_MNTR_INT_FILL_PUMP_POWER

#define MW_BTC_DRAIN_PUMP_POWER                  ID_BTC_CFG_INT_DRAIN_PUMP_POWER
#define MW_BTC_TANK_FILL_WATER_TIME_OUT          ID_BTC_CFG_INT_WATER_TANK_FILL_TIME_OUT
#define MW_BTC_AIR_PRESSURE_KIFE_TIME_OUT        ID_BTC_CFG_INT_AIR_KNIFE_PRESSURE_TIME_OUT
#define MW_BTC_ENGAGE_TIME_OUT                   ID_BTC_CFG_INT_ENGAGE_TIME_OUT
#define MW_BTC_WATER_TANK_HIGH_TEMPERATURE_LIMIT ID_BTC_CFG_INT_WATER_TANK_HIGH_TEMPERATURE_LIMIT
#define MW_BTC_WATER_LINE_HIGH_TEMPERATURE_LIMIT ID_BTC_CFG_INT_WATER_LINE_HIGH_TEMPERATURE_LIMIT
#define MW_BTC_WATER_CONTAMINATION_HIGH_LIMIT    ID_BTC_CFG_INT_WATER_CONTAMINATION_HIGH_LIMIT

//btc state
#define MW_BTC_CURRENT_STATE                     ID_BTC_MNTR_INT_STATE
#define MW_BTC_SET_STATE                         ID_BTC_CMND_INT_STATE

//sensors states
#define MW_BTC_DRAWER_POSITION_STATE             (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,4)) //DI5
#define MW_BTC_WATER_TUB_OVERFLOW_STATE          (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,5)) //DI6
#define MW_BTC_WATER_TANK_LOW_LEVEL_STATE        (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,6)) //DI7
#define MW_BTC_WATER_TANK_UP_STATE               (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,7)) //DI8
#define MW_BTC_WATER_TANK_OVERFLOW_STATE         (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,8)) //D9
#define MW_BTC_AIR_KNIFE_PRESSURE_NORMAL_STATE   (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,9)) //DI10
#define MW_BTC_BLANKET_STOP_STATE                (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,11)) //DI12
#define MW_BTC_ENGAGE_STATE                      ID_BTC_MNTR_INT_ENGAGE

#define MW_BTC_WATER_TANK_TEMPERATURE            (CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,0)) //pv1
#define MW_BTC_WATER_LINE_IN_TEMPERATURE         (CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,1)) //pv2
#define MW_BTC_WATER_CONTAMINATION               ID_BTC_MNTR_INT_WATER_CONTAMINATION
#define MW_BTC_BLANKET_TEMPERATURE               ID_BTC_MNTR_INT_BLANKET_TEMPERATURE
#define MW_BTC_IRT_n(n)                          (CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,n+2)) //pv3-pv18
#define MW_BTC_ENGAGE_STATE_1                    (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,0)) //DI1
#define MW_BTC_ENGAGE_STATE_2                    (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,1)) //DI2
#define MW_BTC_ENGAGE_STATE_3                    (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,2)) //DI3
#define MW_BTC_ENGAGE_STATE_4                    (CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_DIGITAL_INP_n,3)) //DI4
//configuration

#define MW_BTC_WATER_TANK_TEMPERATURE_TH         ID_BTC_CFG_INT_WATER_TANK_HIGH_TEMPERATURE_LIMIT

#define MW_BTC_WATER_LINE_IN_TEMPERATURE_TH      ID_BTC_CFG_INT_WATER_LINE_HIGH_TEMPERATURE_LIMIT

#define MW_BTC_WATER_CONTAMINATION_TH            ID_BTC_CFG_INT_WATER_CONTAMINATION_HIGH_LIMIT

//alarms/error
#define MW_BTC_ERROR_GLOBAL                          ID_BTC_MNTR_INT_ALARM_GLOBAL

#if 0
#define MW_BTC_ERROR_ENGAGE_TIME_OUT                 ID_BTC_MNTR_INT_ALARM_ENGAGE_TIME_OUT
#define MW_BTC_ERROR_FILL_WATER_TIME_OUT             ID_BTC_MNTR_INT_ALARM_FILL_WATER_TIME_OUT
#define MW_BTC_ERROR_AIR_KNIFE_TIME_OUT              ID_BTC_MNTR_INT_ALARM_AIR_KNIFE_TIME_OUT
#define MW_BTC_ERRORWATER_TANK_EXCEED_TEMPREATURE    ID_BTC_MNTR_INT_ALARM_WATER_TANK_EXCEED_TEMPREATURE
#define MW_BTC_ERROR_WATER_LINE_EXCEED_TEMPREATURE   ID_BTC_MNTR_INT_ALARM_WATER_LINE_EXCEED_TEMPREATURE
#define MW_BTC_ERROR_WATER_CONTAMINATION             ID_BTC_MNTR_INT_ALARM_WATER_CONTAMINATION
#define MW_BTC_ERROR_WATER_TANK_OVERFLOW             ID_BTC_MNTR_INT_ALARM_WATER_TANK_OVERFLOW
#define MW_BTC_ERROR_TUB_OVERFLOW                    ID_BTC_MNTR_INT_ALARM_TUB_OVERFLOW
#define MW_BTC_ERROR_DRAWER                          ID_BTC_MNTR_INT_ALARM_DRAWER
#define MW_BTC_ERROR_BLANKET                         ID_BTC_MNTR_INT_ALARM_BLANKET
#endif


//STATES

#define MW_BTC_CURRENT_STATE                     ID_BTC_MNTR_INT_STATE
#define MW_BTC_SET_COMMAND                       ID_BTC_CMND_INT_COMMAND


#endif /* BTC_DEF_H_ */
