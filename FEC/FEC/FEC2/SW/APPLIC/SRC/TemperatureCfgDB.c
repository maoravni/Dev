/****************************************************************************
 *
 *           TITLE:   TemperatureCfgDB
 *
 *			Service routines for the TEMPERATURE DB
 *
 *           PROJECT: FEC board
 *
 *           AUTHORS: Yossi Sarusi
 *
 *           Creation date: 17/12/12
 *
 *
 **
 *
 *****************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
#define RTD_RES_RETURN

 //Temporary only for FEC's temperature validation/calibration test
//#define DISPLAY_SOURCE_DIGITAL_TEMPERATURE_VALUES

#include <string.h>

#include "free_rtos_exp.h" 
#include "os_cnfg.h"

//#include "pq_defs.h"
#include <stdlib.h>
#include <stdio.h>
//#include "syslib.h"
#include <ll_typedef.h>
#include "FEC.h"

#include "CfgDB.h"
#include "CfgDBUtil.h"
#include "GnrlCfgDB.h"
#include "TemperatureCfgDB.h"
#include "Calibration.h"
#include "PWM.h"
#include "Opto_out.h"

#include "mb_cnfg.h"
#include "..\..\..\Common\MODBUS\modbus_exp.h"
#include "..\..\..\Common\MODBUS\RTU\rtu_master.h"
#include "rtc.h"
#include "a2d.h"
#ifdef CPP_FRAMEWORK
#include "logger.h"
#endif
#include "leds.h"
#include "CbtcDef.h"
#include <math.h>

#define MOD_BUS_DEVICE_ADDR_PUMA_1	1
#define MOD_BUS_DEVICE_ADDR_PUMA_2	2
#define MOD_BUS_DEVICE_ADDR_PUMA_3	3
#define MOD_BUS_PUMA_REGISTER_START_CH_1	5000
#define MOD_BUS_PUMA_REGISTER_START_CH_2	5008
#define MOD_BUS_PUMA_REGISTER_START_CH_3	5016
#define MOD_BUS_PUMA_REGISTER_START_CH_4	5024
#define MOD_BUS_PUMA_NUM_OF_ELEMENTS            8

#define ALARM_HIGH_TEMERATURE_TH 5800 // on 580 degrees the safety alarm will be activated which turn off the power
#define TEMPERATURE_MAX_NUMBER_OF_ZONES 4
#define TEMPERATURE_MAX_NUMBER_OF_RO_DEVICES 4

#define MOD_BUS_DEVICE_ADDR_DATA_CARD_1   32
#define MOD_BUS_DEVICE_ADDR_DATA_CARD_2   33

#define MOD_BUS_DEVICE_ADDR_8TC_1	34
#define MOD_BUS_DEVICE_ADDR_8TC_2	35
#define MOD_BUS_DEVICE_ADDR_8TC_3	36
#define MOD_BUS_DEVICE_ADDR_8TC_4	37

#define MOD_BUS_DEVICE_ADDR_6RTD_1	39
#define MOD_BUS_DEVICE_ADDR_6RTD_2	40

#define MOD_BUS_DEVICE_ADDR_8TC_5	41
#define MOD_BUS_DEVICE_ADDR_8TC_6	42
#define MOD_BUS_DEVICE_ADDR_8TC_7	43

#define MOD_BUS_8TC_REGISTER_START	1
#define MOD_BUS_8AI_REGISTER_START	1
#define MOD_BUS_6RTD_REGISTER_START	1
#define MOD_BUS_DATA_CARD_REGISTER_START 0

#define MOD_BUS_8TC_NUM_OF_ELEMENTS   8

#define MOD_BUS_6RTD_NUM_OF_ELEMENTS   6

#define MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS   8

#define MIN_AVAILIABLE_TEMPERATURE 0
#define MAX_AVAILIABLE_TEMPERATURE 5000  //500 degrees
#define MIN_PID_P 0
#define MAX_PID_P 5000 //tbd
#define MIN_PID_I 0
#define MAX_PID_I 5000  //tbd
#define MIN_PID_D 0
#define MAX_PID_D 5000 //tbd
#define MIN_ZONE_AVAILIABLE_TEMPERATURE 0
#define MAX_ZONE_AVAILIABLE_TEMPERATURE 0xffff //  TBD5000  //500 degrees
#define DEFAULT_CONSOL_LOG_TH 6

typedef enum
{
    DEV_PUMA, DEV_8TC, DEV_6RTD, DEV_DATA_CARD, DEV_8AI,
} DEVICE_TYPE;

//addresses for set commands to the PUMA
typedef struct
{
    U16 Enable;
    U16 SP;
    U16 PID_P;
    U16 PID_I;
    U16 PID_D;
    U16 PowerLimit;
    U16 AutoTune;
} SET_PUMA_REGISTER_ADDRESS;

typedef struct
{
    U32 InstanceId;
    U32 DB_IdIndex;
    U32 DB_Id[8];  //up to 8 element each puma's modbus read
} DB_ID_TC;

typedef struct
{
    DEVICE_TYPE DeviceType;
    U16 DeviceId;
    U16 RegisterId;
    U8 NumberOfElements;
    DB_ID_TC DB_id;
    SET_PUMA_REGISTER_ADDRESS SetPumaRegisterAddress;
} CfgDB_ID_TC_PeripheralsModBusDescription;

//RO temperatute
typedef struct
{
    U32 InstanceId;
    U32 StartIdIndex;
    U32 DB_Id;
} DB_ID;

typedef struct
{
    DEVICE_TYPE DeviceType;
    U16 DeviceId;
    U16 RegisterId;
    U8 NumberOfElements;
    DB_ID DB_id;
} CfgDB_ID_PeripheralsModBusDescription;

static CfgDB_ID_PeripheralsModBusDescription PeripheralsModBusDescription[TEMPERATURE_MAX_NUMBER_OF_RO_DEVICES + 1];

const static CfgDB_ID_PeripheralsModBusDescription PeripheralsModBusDescriptionr[TEMPERATURE_MAX_NUMBER_OF_RO_DEVICES] =
{
//Device#1 6RTD
        { DEV_6RTD, MOD_BUS_DEVICE_ADDR_6RTD_1, MOD_BUS_6RTD_REGISTER_START, MOD_BUS_6RTD_NUM_OF_ELEMENTS,
        { NULL, 0, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n } },
        //Device#2 8TC_1
        { DEV_8TC, MOD_BUS_DEVICE_ADDR_8TC_1, MOD_BUS_8TC_REGISTER_START, MOD_BUS_8TC_NUM_OF_ELEMENTS,
        { NULL, 0, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n } },

        //DATA CARD 1
        { DEV_DATA_CARD, MOD_BUS_DEVICE_ADDR_DATA_CARD_1, MOD_BUS_DATA_CARD_REGISTER_START,
                MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS,
                { NULL, 0, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n } },

        //DATA CARD 1
        { DEV_DATA_CARD, MOD_BUS_DEVICE_ADDR_DATA_CARD_2, MOD_BUS_DATA_CARD_REGISTER_START,
                MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS,
                { NULL, 0, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n } },

};

static CfgDB_ID_TC_PeripheralsModBusDescription * PeripheralsModBusDescriptionPtrTC[TEMPERATURE_MAX_NUMBER_OF_ZONES + 1];

//0:SV x10, 1:SP x10, 2: MV x10, 4: On/off, 5: PID_P x 10, 6: PID_I x10, 7: PID_D x10 
static CfgDB_ID_TC_PeripheralsModBusDescription PeripheralsModBusDescriptionrTC[TEMPERATURE_MAX_NUMBER_OF_ZONES] =
{
//Device#1- zone_Z1
        { DEV_PUMA, MOD_BUS_DEVICE_ADDR_PUMA_1, MOD_BUS_PUMA_REGISTER_START_CH_1, MOD_BUS_PUMA_NUM_OF_ELEMENTS,
        { NULL, 0, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,
                ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n },
        { 16, 0, 1101, 1102, 1103, 1255, 1004 } },

        //Device#1- zone_Z2
        { DEV_PUMA, MOD_BUS_DEVICE_ADDR_PUMA_1, MOD_BUS_PUMA_REGISTER_START_CH_2, MOD_BUS_PUMA_NUM_OF_ELEMENTS,
        { NULL, 1, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,
                ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n },
        { 17, 1, 2101, 2102, 2103, 2255, 2004 } },

        //Device#1- zone_Z3
        { DEV_PUMA, MOD_BUS_DEVICE_ADDR_PUMA_1, MOD_BUS_PUMA_REGISTER_START_CH_3, MOD_BUS_PUMA_NUM_OF_ELEMENTS,
        { NULL, 2, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,
                ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n },
        { 18, 2, 3101, 3102, 3103, 3255, 3004 } },
        //Device#1- zone_Z4
        { DEV_PUMA, MOD_BUS_DEVICE_ADDR_PUMA_1, MOD_BUS_PUMA_REGISTER_START_CH_4, MOD_BUS_PUMA_NUM_OF_ELEMENTS,
        { NULL, 3, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,
                ID_TEMPERATURE_MNTR_INT_ZONE_MV_n, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
                ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n },
        { 19, 3, 4101, 4102, 4103, 4255, 4004 } },

};

//#define DRYER_NUMBER_OF_ZONES 2 
//#define DRYER_NUMBER_OF_PT100 3
//#define DRYER_NUMBER_OF_TC 4

static void InitPeripheralsModBusDescriptionr(SUBSYS_ID SubSysId)
{
    int Inx = 0;
    U32 ModbusBaud;
    switch (SubSysId)
    {
    case SUBSYS_DRYER:
        //one PUMA
        for (Inx = 0; Inx < DRYER_NUMBER_OF_ZONES; Inx++)
        {
            PeripheralsModBusDescriptionPtrTC[Inx] =
                    (CfgDB_ID_TC_PeripheralsModBusDescription *) &PeripheralsModBusDescriptionrTC[Inx];
        }
        Inx += 1;
        PeripheralsModBusDescriptionPtrTC[Inx] = 0; //end of list

        Inx = 0;

        //One 6RTD
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[Inx],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));
        PeripheralsModBusDescription[Inx].NumberOfElements = DRYER_NUMBER_OF_PT100;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = DRYER_NUMBER_OF_ZONES; //Id index start from the last one

        Inx += 1;

        //One 8TC
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[Inx],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));
        PeripheralsModBusDescription[Inx].NumberOfElements = DRYER_NUMBER_OF_TC;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = DRYER_NUMBER_OF_ZONES + DRYER_NUMBER_OF_PT100; //Id index start from the last one
        Inx += 1;

        PeripheralsModBusDescription[Inx].NumberOfElements = 0; //end of list
        break;
    case SUBSYS_IBD:
        //one PUMA
        for (Inx = 0; Inx < IBD_NUMBER_OF_ZONES; Inx++)
        {
            PeripheralsModBusDescriptionPtrTC[Inx] =
                    (CfgDB_ID_TC_PeripheralsModBusDescription *) &PeripheralsModBusDescriptionrTC[Inx];
        }
        Inx += 1;
        PeripheralsModBusDescriptionPtrTC[Inx] = 0; //end of list

        Inx = 0;

        //One 6RTD
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[Inx],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));

        PeripheralsModBusDescription[Inx].NumberOfElements = IBD_NUMBER_OF_PT100;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = IBD_NUMBER_OF_ZONES; //Id index start from the last one

        Inx += 1;

        //One 8TC
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[Inx],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));

        PeripheralsModBusDescription[Inx].NumberOfElements = IBD_NUMBER_OF_TC;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = IBD_NUMBER_OF_ZONES + IBD_NUMBER_OF_PT100; //Id index start from the last one
        Inx += 1;

        PeripheralsModBusDescription[Inx].NumberOfElements = 0; //end of list
        break;
    case SUBSYS_ITH:
        //one PUMA
        Inx = 0;
              for (Inx=0;Inx<ITH_NUMBER_OF_ZONES;Inx++)
              {
                  PeripheralsModBusDescriptionPtrTC[Inx]=(CfgDB_ID_TC_PeripheralsModBusDescription *)&PeripheralsModBusDescriptionrTC[Inx];
              }
              Inx+=1;
        PeripheralsModBusDescriptionPtrTC[Inx] = 0; //end of list

        Inx = 0;

        //One 6RTD
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[Inx],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));

        PeripheralsModBusDescription[Inx].NumberOfElements = ITH_NUMBER_OF_PT100;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = ITH_NUMBER_OF_ZONES; //Id index start from the last one

        Inx += 1;

        //One 8TC
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[Inx],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));

        PeripheralsModBusDescription[Inx].NumberOfElements = ITH_NUMBER_OF_TC;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = ITH_NUMBER_OF_ZONES + ITH_NUMBER_OF_PT100; //Id index start from the last one
        Inx += 1;

        PeripheralsModBusDescription[Inx].NumberOfElements = 0; //end of list

        break;
    case SUBSYS_BTC:
        //due to limitation of the Data card device the baud rate for the modbus devices are set to 19200 baud
        ModbusBaud = 19200;
        CfgPutParam(0, ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD, &ModbusBaud, CfgDB_SET_TO_DB);

        PeripheralsModBusDescriptionPtrTC[0] = 0; //no PUMA exists

        Inx = 0;
        //One 6RTD
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[0],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));
        PeripheralsModBusDescription[Inx].NumberOfElements = BTC_NUMBER_OF_PT100;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = 0; //Id index start from the last one

        Inx += 1;
        //Data card 1
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[2],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));
        PeripheralsModBusDescription[Inx].NumberOfElements = MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = BTC_NUMBER_OF_PT100; //Id index start from the last one

        Inx += 1;

        //Data card 2
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[3],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));
        PeripheralsModBusDescription[Inx].NumberOfElements = MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = BTC_NUMBER_OF_PT100 + MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS; //Id index start from the last one

        Inx += 1;

        PeripheralsModBusDescription[Inx].NumberOfElements = 0; //end of list

        break;
    case SUBSYS_CONDITIONER:
        //due to limitation of the Data card device the baud rate for the modbus devices are set to 19200 baud
        ModbusBaud = 19200;
        CfgPutParam(0, ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD, &ModbusBaud, CfgDB_SET_TO_DB);

        PeripheralsModBusDescriptionPtrTC[0] = 0; //no PUMA exists

        Inx = 0;
        //One 6RTD
        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[0],
                sizeof(CfgDB_ID_PeripheralsModBusDescription));
        PeripheralsModBusDescription[Inx].NumberOfElements = BTC_NUMBER_OF_PT100;
        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = 0; //Id index start from the last one

        Inx += 1;
//        //Data card 1
//        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[2],
//                sizeof(CfgDB_ID_PeripheralsModBusDescription));
//        PeripheralsModBusDescription[Inx].NumberOfElements = MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS;
//        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = BTC_NUMBER_OF_PT100; //Id index start from the last one
//
//        Inx += 1;
//
//        //Data card 2
//        memcpy(&PeripheralsModBusDescription[Inx], &PeripheralsModBusDescriptionr[3],
//                sizeof(CfgDB_ID_PeripheralsModBusDescription));
//        PeripheralsModBusDescription[Inx].NumberOfElements = MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS;
//        PeripheralsModBusDescription[Inx].DB_id.StartIdIndex = BTC_NUMBER_OF_PT100 + MOD_BUS_DATA_CARD_NUM_OF_ELEMENTS; //Id index start from the last one
//
//        Inx += 1;
//
        PeripheralsModBusDescription[Inx].NumberOfElements = 0; //end of list

        break;
    }
}

/***********************************  DB CFG SET/GET HANDLERS*****************************************/

typedef enum
{
    ZONE_SET_ENABLE,
    ZONE_SET_SP,
    ZONE_SET_PID_P,
    ZONE_SET_PID_I,
    ZONE_SET_PID_D,
    ZONE_SET_POWER_LIMIT,
    ZONE_SET_AUTO_TUNE
} ZONE_SET_TYPE;

static STATUS SetZoneTemperature(ZONE_SET_TYPE SetType, int DeviceInx, CfgDB_INT_DATA Data, CfgDB_SET_FLAG *SetFlag)
{
    if (IsInAtpMode())
    {
        //TBD internal handle
    }
    else
    {
        U16 DeviceId = PeripheralsModBusDescriptionPtrTC[DeviceInx]->DeviceId;
        U16 Register;
        switch (SetType)
        {
        case ZONE_SET_ENABLE:
#if 0              
            if ((GlobalSubSysId==SUBSYS_DRYER) || (GlobalSubSysId==SUBSYS_IBD))
            {
                Pwm_Enable(DeviceInx, (U8)Data);
                return OK;
            }
#endif                   
            Register = PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.Enable;
            Pwm_Enable(DeviceInx, (U8) Data);
            Data = Data == 0 ? 1 : 0; //fix it for PUMA
            break;
        case ZONE_SET_SP:
            Register = PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.SP;
            break;
        case ZONE_SET_PID_P:
            Register = PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.PID_P;
            break;
        case ZONE_SET_PID_I:
            Register = PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.PID_I;
            break;
        case ZONE_SET_PID_D:
            Register = PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.PID_D;
            break;
        case ZONE_SET_POWER_LIMIT:
            Register = PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.PowerLimit;
            break;
        case ZONE_SET_AUTO_TUNE:
            U16 IdIndex = PeripheralsModBusDescriptionPtrTC[DeviceInx]->DB_id.DB_IdIndex;
            U16 InstanceId = PeripheralsModBusDescriptionPtrTC[DeviceInx]->DB_id.InstanceId;

            //U32 data0=0;
            // CfgPutParam(InstanceId,CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,IdIndex),&data0,CfgDB_SET_TO_DB);
            //CfgPutParam(InstanceId,CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,IdIndex),&data0,CfgDB_SET_TO_DB);
            //CfgPutParam(InstanceId,CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,IdIndex),&data0,CfgDB_SET_TO_DB);

            Register = PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.AutoTune;
            break;

        }

        /*send the command to the PUMA*/
        return (rtu_single_reg_write(mb_node_1, DeviceId, Register, (U16) Data, 0, 0, 0, NULL));
    }

    return OK;
}

static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{
    return (SetZoneTemperature(ZONE_SET_PID_P, Index, Data, SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{
    return (SetZoneTemperature(ZONE_SET_PID_I, Index, Data, SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{
    return (SetZoneTemperature(ZONE_SET_PID_D, Index, Data, SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{
    return (SetZoneTemperature(ZONE_SET_POWER_LIMIT, Index, Data, SetFlag));
}

/************************** CMND INT  *************************************/

//static REQUEST_SEQUENCE RequestDeviceSequence=0;
static STATUS handleResponeError(uint8_t func_status)
{
    if (func_status != RTU_FUNC_ERR_OK)
    {
        switch (func_status)
        {
        case RTU_FUNC_ERR_CRC16:
            break;
        case RTU_FUNC_ERR_NO_SLAVE_RESP:
            break;
        case RTU_FUNC_ERR_EXEP:
            break;
        case RTU_FUNC_ERR_UNKNOWN:
            break;
        }
        return (func_status);
    }
}

uint32_t static StartSysTickCounter = 0;
static void MB_GetDataResponseHandlerTC(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz,
        uint32_t RequestDeviceSequence)
{

    U16 RxData[10];

    if (handleResponeError(func_status) != RTU_FUNC_ERR_OK)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "TEMPERATURE_UPDATE_DB_RESP ERROR: func_status=%d", func_status);
        return;
    }

    if (resp_sz != MOD_BUS_PUMA_NUM_OF_ELEMENTS)
        while (1)
        {
            ;
        }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "START ->TEMPERATURE_UPDATE_DB_RESP. total_ModbusRetries=%d",
            get_total_retries());

    for (int i = 0; i < resp_sz; i++)
    {

        U32 InstanceId = PeripheralsModBusDescriptionPtrTC[RequestDeviceSequence]->DB_id.InstanceId;
        U32 DB_Id = PeripheralsModBusDescriptionPtrTC[RequestDeviceSequence]->DB_id.DB_Id[i];
        U32 DB_IdIndex = PeripheralsModBusDescriptionPtrTC[RequestDeviceSequence]->DB_id.DB_IdIndex;

        if (DB_Id != 0)
        {
            U32 data = *(resp_buf + i);

            switch (PeripheralsModBusDescriptionPtrTC[RequestDeviceSequence]->DeviceType)
            {
            case DEV_PUMA:
            {
                // mark termocoupler not connected as 5000 value
                if (i == 0)  // PV value from PUMA
                {
                    if ((int) data < 0 || (data == 4200) || (data >= 8500))
                        data = SENSOR_NOT_CONNECTED_MARK;  //mark as thermocouple not connected
                    else if (data >= ALARM_HIGH_TEMERATURE_TH)
                    {
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "PUMA [%d] TEMPERATURE CRITICAL_ALARM data=%d",
                                RequestDeviceSequence, data);
                        //turn on  the safety alarm
                        U32 Data = 1;
                        CfgPutParam(0, ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM, &Data, CfgDB_SET_TO_DB);

                    }
                }
                else if (i == 2) //MV value
                {
                    // if ((GlobalSubSysId==SUBSYS_DRYER)  ||  (GlobalSubSysId==SUBSYS_IBD))
                    {
                        CfgPutParam(InstanceId, CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,DB_IdIndex),
                                &data, CfgDB_SET_TO_RAM); //just keep it for future use
                        continue;
                    }
                    //Pwm_SetDutyCycle(DB_IdIndex, (U8)(data/10)); //ITH
                }
                else
                {
                    // fix the set value of the on/off from the PUMA from  on=0, off=1 to on=1, off=0
                    if (i == 3) // Pumas on/off value
                    {
                        if ((GlobalSubSysId == SUBSYS_DRYER) || (GlobalSubSysId == SUBSYS_IBD))
                            continue;
                        data = data == 0 ? 1 : 0;  // set it as: 0n=1, off=0
                        data = (data == 1) & IsPwm_Enable(DB_IdIndex);
                    }
                }

            }
                break;

            } //switch

            char IdStr[60];
            if (CfgPutParam(InstanceId, CfgDB_ID_AddIndex(DB_Id,DB_IdIndex), &data, CfgDB_SET_TO_RAM) != OK)
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                        "********ERROR CfgPutPara:  TEMPERATURE_UPDATE_DB_RESP:  InstanceId=%d, DB_Id=%s, data=%d ",
                        InstanceId, Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_IdIndex), IdStr), data);

                //while (1) {};
            }
            if (i == 0) //PV
                M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "TEMPERATURE_UPDATE_DB_RESP: InstanceId=%d, DB_Id=%s, data=%d ",
                        InstanceId, Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_IdIndex), IdStr), data);
        } //if

    } //for

}

static void MB_GetDataResponseHandler(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz,
        uint32_t RequestDeviceSequence)
{

    if (handleResponeError(func_status) != RTU_FUNC_ERR_OK)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "TEMPERATURE_UPDATE_DB_RESP ERROR: func_status=%d", func_status);
        return;
    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "START ->TEMPERATURE_UPDATE_DB_RESP. total_ModbusRetries=%d",
            get_total_retries());

    U32 InstanceId = PeripheralsModBusDescription[RequestDeviceSequence].DB_id.InstanceId;
    U32 DB_Id = PeripheralsModBusDescription[RequestDeviceSequence].DB_id.DB_Id;
    U32 DB_StartIdIndex = PeripheralsModBusDescription[RequestDeviceSequence].DB_id.StartIdIndex;

    for (int i = 0; i < resp_sz; i++)
    {

        U32 data = *(resp_buf + i);

        switch (PeripheralsModBusDescription[RequestDeviceSequence].DeviceType)
        {

        case DEV_8TC:
        {
            if ((data == 32768) || (data > 63000))
                data = SENSOR_NOT_CONNECTED_MARK;  //mark as thermocouple not connected
            else if (data >= ALARM_HIGH_TEMERATURE_TH)
            {
                //turn on  the safety alarm
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "8TC [%d] TEMPERATURE CRITICAL_ALARM data=%d", i, data);
                U32 Data = 1;
                CfgPutParam(0, ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM, &Data, CfgDB_SET_TO_DB);
            }
        }
            break;
        case DEV_6RTD:
        {
            if ((data == 32768) || (data >= 8500))
                data = SENSOR_NOT_CONNECTED_MARK;  //mark as thermocouple not connected
            else if (data >= ALARM_HIGH_TEMERATURE_TH)
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "6RTD [%d] TEMPERATURE CRITICAL_ALARM data=%d", i, data);
                //turn on  the safety alarm
                U32 Data = 1;
                CfgPutParam(0, ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM, &Data, CfgDB_SET_TO_DB);
            }
        }
            break;

        case DEV_DATA_CARD:
        {
            /* if ((data==32768) || (data>=8500))
             data=SENSOR_NOT_CONNECTED_MARK;  //mark as thermocouple not connected
             else*/if (data >= ALARM_HIGH_TEMERATURE_TH)
            {
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "6RTD [%d] TEMPERATURE CRITICAL_ALARM data=%d", i, data);
                //turn on  the safety alarm
                U32 Data = 1;
                CfgPutParam(0, ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM, &Data, CfgDB_SET_TO_DB);
            }
        }
            break;
        } //switch

        char IdStr[60];
        if (CfgPutParam(InstanceId, CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i), &data, CfgDB_SET_TO_RAM) != OK)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                    "********ERROR CfgPutPara:  TEMPERATURE_UPDATE_DB_RESP:  InstanceId=%d, DB_Id=%s, data=%d ",
                    InstanceId, Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i), IdStr), data);

        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "TEMPERATURE_UPDATE_DB_RESP: InstanceId=%d, DB_Id=%s, data=%d ", InstanceId,
                Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i), IdStr), data);
    } //for

}

static STATUS Set_ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES_handle(int InstanceNum, int Index,
        CfgDB_INT_DATA Data, CfgDB_SET_FLAG *SetFlag)
{
    if (*SetFlag & CfgDB_SET_TEST_SANITY)
        return OK;

    U16 DeviceId;
    U16 RegisterId;
    U8 NumberOfElements;
    int ReqSequance;

    StartSysTickCounter = xTaskGetTickCountFromISR(); // uiTracePortGetTimeStamp();
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "START ->TEMPERATURE_UPDATE_DB_REQ");

    if (IsInAtpMode())
    {
        U32 TempData;
        for (int Index = 0; Index < NUMBER_OF_BOARD_TEMPERATURE_ZONES; Index++)
            CfgGetParam(NULL, CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,Index), &TempData, sizeof(TempData),
                    CfgDB_GET_DIRECTLY_FROM_SOURCE);
    }

    else
    {  // External zone handle
       // Handle TC
        ReqSequance = 0;
        while (PeripheralsModBusDescriptionPtrTC[ReqSequance])
        {
            DeviceId = PeripheralsModBusDescriptionPtrTC[ReqSequance]->DeviceId;
            RegisterId = PeripheralsModBusDescriptionPtrTC[ReqSequance]->RegisterId;
            NumberOfElements = PeripheralsModBusDescriptionPtrTC[ReqSequance]->NumberOfElements;
            LL_STATUS Status = rtu_hold_regs_read(mb_node_1, DeviceId, RegisterId, NumberOfElements, 0, 0, ReqSequance,
                    MB_GetDataResponseHandlerTC); //data from modbus device

            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                    "TEMPERATURE_UPDATE_DB_REQ_TC: ReqSequance=%d, DeviceId=%d, RegisterId=%d, NumberOfElements=%d ",
                    ReqSequance, DeviceId, RegisterId, NumberOfElements);
            ReqSequance += 1;
        }
        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "END ->TEMPERATURE_UPDATE_DB_REQ_TC. ReqSequance=%d", ReqSequance);

        // Handle RO
        ReqSequance = 0;
        while (PeripheralsModBusDescription[ReqSequance].NumberOfElements)
        {
            DeviceId = PeripheralsModBusDescription[ReqSequance].DeviceId;
            RegisterId = PeripheralsModBusDescription[ReqSequance].RegisterId;
            NumberOfElements = PeripheralsModBusDescription[ReqSequance].NumberOfElements;
            LL_STATUS Status = rtu_hold_regs_read(mb_node_1, DeviceId, RegisterId, NumberOfElements, 0, 0, ReqSequance,
                    MB_GetDataResponseHandler); //data from modbus device

            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                    "TEMPERATURE_UPDATE_DB_REQ: ReqSequance=%d, DeviceId=%d, RegisterId=%d, NumberOfElements=%d ",
                    ReqSequance, DeviceId, RegisterId, NumberOfElements);
            ReqSequance += 1;
        }
    }
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "END ->TEMPERATURE_UPDATE_DB_REQ. ReqSequance=%d", ReqSequance);

    return OK;
}

static STATUS Set_ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{

    return (SetZoneTemperature(ZONE_SET_ENABLE, Index, Data, SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CMND_INT_ZONE_SP_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{
    return (SetZoneTemperature(ZONE_SET_SP, Index, Data, SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{

    return (SetZoneTemperature(ZONE_SET_AUTO_TUNE, Index, Data, SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{
    if (IsInAtpMode())
        TemperatureCalibrationSetData(Index, Data);
    return OK;
}

/************************** MONITOR INT STRING*************************************/
//void (*cb_func)( uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val)
static void MB_GetResponse(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val)
{
    static uint16_t RxBuf[20];

    //total of 16bits elements
    resp_buf += 3; //point to the start of the data

    int TotalReceivedElement = (resp_sz - 5) >> 1; //TotalReceivedElement=(resp_sz-5)/2 where the 5 are composed of: 3 bytes of headers, and 2 byes of CRC
    for (int i = 0; i < TotalReceivedElement; i += 2)
    {
        RxBuf[i] = SWAP(*(uint16_t*)(resp_buf+i));

    }
}
/*******************************************************************************
 Returns a current value of temperature*10(for accurancy saving) into "Data" for
 channel "Index".
 *******************************************************************************/
STATUS GetTmpr_PT100_CelsiusDgr(int Index, int* Data)
{
    A2D_RES_ET res;
    uint16_t adc_val;
    uint32_t arrs_indx = 0; //, tmp_data_max, tmp_data_min;
    float j = 0;
    float k = 0;

    if (Index >= 12)
        return 0;

    static const uint32_t dig_val_arr[] =
    { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240,
            250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450,
            460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620, 630, 640, 650, 660,
            670, 680, 690, 700, 710, 720, 730, 740, 750, 760, 770, 780, 790, 800, 810, 820, 830, 840, 850, 860, 870,
            880, 890, 900, 910, 920, 930, 940, 950, 960, 970, 980, 990, 1000, 1010, 1020, 1030, 1040, 1050, 1060, 1070,
            1080, 1090, 1100, 1110, 1120, 1130, 1140, 1150, 1160, 1170, 1180, 1190, 1200, 1210, 1220, 1230, 1240, 1250,
            1260, 1270, 1280, 1290, 1300, 1310, 1320, 1330, 1340, 1350, 1360, 1370, 1380, 1390, 1400, 1410, 1420, 1430,
            1440, 1450, 1460, 1470, 1480, 1490, 1500, 1510, 1520, 1530, 1540, 1550, 1560, 1570, 1580, 1590, 1600, 1610,
            1620, 1630, 1640, 1650, 1660, 1670, 1680, 1690, 1700, 1710, 1720, 1730, 1740, 1750, 1760, 1770, 1780, 1790,
            1800, 1810, 1820, 1830, 1840, 1850, 1860, 1870, 1880, 1890, 1900, 1910, 1920, 1930, 1940, 1950, 1960, 1970,
            1980, 1990, 2000, 2010, 2020, 2030, 2040, 2050, 2060, 2070, 2080, 2090, 2100, 2110, 2120, 2130, 2140, 2150,
            2160, 2170, 2180, 2190, 2200, 2210, 2220, 2230, 2240, 2250, 2260, 2270, 2280, 2290, 2300, 2310, 2320, 2330,
            2340, 2350, 2360, 2370, 2380, 2390, 2400, 2410, 2420, 2430, 2440, 2450, 2460, 2470, 2480, 2490, 2500, 2510,
            2520, 2530, 2540, 2550, 2560, 2570, 2580, 2590, 2600, 2610, 2620, 2630, 2640, 2650, 2660, 2670, 2680, 2690,
            2700, 2710, 2720, 2730, 2740, 2750, 2760, 2770, 2780, 2790, 2800, 2810, 2820, 2830, 2840, 2850, 2860, 2870,
            2880, 2890, 2900, 2910, 2920, 2930, 2940, 2950, 2960, 2970, 2980, 2990, 3000, 3010, 3020, 3030, 3040, 3050,
            3060, 3070, 3080, 3090, 3100, 3110, 3120, 3130, 3140, 3150, 3160, 3170, 3180, 3190, 3200, 3210, 3220, 3230,
            3240, 3250, 3260, 3270, 3280, 3290, 3300, 3310, 3320, 3330, 3340, 3350, 3360, 3370, 3380, 3390, 3400, 3410,
            3420, 3430, 3440, 3450, 3460, 3470, 3480, 3490, 3500, 3510, 3520, 3530, 3540, 3550, 3560, 3570, 3580, 3590,
            3600, 3610, 3620, 3630, 3640, 3650, 3660, 3670, 3680, 3690, 3700, 3710, 3720, 3730, 3740, 3750, 3760, 3770,
            3780, 3790, 3800, 3810, 3820, 3830, 3840, 3850, 3860, 3870, 3880, 3890, 3900, 3910, 3920, 3930, 3940, 3950,
            3960, 3970, 3980, 3990, 4000, 4010, 4020, 4030, 4040, 4050, 4060, 4070, 4080, 4090, 4095 };
    static const uint32_t temper_val_arr_p10[] =
    { 0, 1, 2, 3, 12, 20, 29, 38, 46, 55, 63, 72, 80, 89, 98, 106, 115, 124, 132, 141, 150, 158, 167, 176, 185, 193,
            202, 211, 220, 228, 237, 246, 255, 263, 272, 281, 290, 299, 308, 317, 325, 334, 343, 352, 361, 370, 379,
            388, 397, 406, 415, 424, 433, 442, 451, 460, 469, 478, 487, 496, 505, 515, 524, 533, 542, 551, 560, 569,
            579, 588, 597, 606, 616, 625, 634, 643, 653, 662, 671, 681, 690, 699, 709, 718, 727, 737, 746, 756, 765,
            775, 784, 793, 803, 812, 822, 831, 841, 851, 860, 870, 879, 889, 899, 908, 918, 927, 937, 947, 957, 966,
            976, 986, 995, 1005, 1015, 1025, 1035, 1044, 1054, 1064, 1074, 1084, 1094, 1104, 1114, 1124, 1133, 1143,
            1153, 1163, 1173, 1183, 1194, 1204, 1214, 1224, 1234, 1244, 1254, 1264, 1274, 1285, 1295, 1305, 1315, 1325,
            1336, 1346, 1356, 1367, 1377, 1387, 1398, 1408, 1418, 1429, 1439, 1450, 1460, 1471, 1481, 1492, 1502, 1513,
            1523, 1534, 1544, 1555, 1566, 1576, 1587, 1598, 1608, 1619, 1630, 1641, 1651, 1662, 1673, 1684, 1695, 1706,
            1717, 1727, 1738, 1749, 1760, 1771, 1782, 1793, 1804, 1815, 1827, 1838, 1849, 1860, 1871, 1882, 1894, 1905,
            1916, 1927, 1939, 1950, 1961, 1973, 1984, 1996, 2007, 2018, 2030, 2041, 2053, 2064, 2076, 2088, 2099, 2111,
            2123, 2134, 2146, 2158, 2169, 2181, 2193, 2205, 2217, 2229, 2241, 2252, 2264, 2276, 2288, 2300, 2313, 2325,
            2337, 2349, 2361, 2373, 2386, 2398, 2410, 2422, 2435, 2447, 2459, 2472, 2484, 2497, 2509, 2522, 2534, 2547,
            2560, 2572, 2585, 2598, 2610, 2623, 2636, 2649, 2662, 2675, 2688, 2701, 2714, 2727, 2740, 2753, 2766, 2779,
            2792, 2806, 2819, 2832, 2845, 2859, 2872, 2886, 2899, 2913, 2926, 2940, 2954, 2967, 2981, 2995, 3008, 3022,
            3036, 3050, 3064, 3078, 3092, 3106, 3120, 3134, 3149, 3163, 3177, 3191, 3206, 3220, 3235, 3249, 3264, 3278,
            3293, 3308, 3322, 3337, 3352, 3367, 3382, 3397, 3412, 3427, 3442, 3457, 3473, 3488, 3503, 3519, 3534, 3550,
            3565, 3581, 3597, 3612, 3628, 3644, 3660, 3676, 3692, 3708, 3724, 3740, 3757, 3773, 3790, 3806, 3823, 3839,
            3856, 3873, 3890, 3907, 3923, 3941, 3958, 3975, 3992, 4010, 4027, 4045, 4062, 4080, 4098, 4115, 4133, 4151,
            4170, 4188, 4206, 4224, 4243, 4261, 4280, 4299, 4318, 4337, 4356, 4375, 4394, 4414, 4433, 4453, 4473, 4492,
            4512, 4532, 4553, 4573, 4593, 4614, 4635, 4656, 4677, 4698, 4719, 4740, 4762, 4784, 4806, 4828, 4850, 4872,
            4895, 4917, 4940, 4963, 4986, 5010, 5033, 5057, 5081, 5106, 5130, 5155, 5167 };

    if (!Data)
        return ERROR;

    res = a2d_read_dig((A2D_DATA_ET)(RtdId_2_Indx[Index]), &adc_val);
//   adc_val = 1560;
#ifdef DISPLAY_SOURCE_DIGITAL_TEMPERATURE_VALUES
    AUTO_DEBUG_MSG (UNMASK_ERROR_LEVEL,"Temperature = %d ", adc_val);
    *Data=adc_val;
    return OK;
#else

    if (res == WRONG_PARAM)
        return ERROR;
    else
    {
//        while (dig_val_arr[arrs_indx] <= adc_val)
//        {
//            arrs_indx++;
//            if (arrs_indx == ((sizeof(dig_val_arr)) / (sizeof(uint32_t))))
//            {
//                arrs_indx--;
//                return ERROR;
//            }
//        }/* while */
//
//        j = (float) (adc_val) / ((float) (dig_val_arr[arrs_indx]) + (float) (dig_val_arr[arrs_indx - 1]));
//        k = (float) (temper_val_arr_p10[arrs_indx]) - (float) (temper_val_arr_p10[arrs_indx - 1]);
//        j *= k;
//
//        *Data = (int) (j);
//
//        *Data += temper_val_arr_p10[arrs_indx - 1];

        float calcTemp;
        const float a = -6.0154E-6;
        const float b = 9.4526E-3;
        const float c = 2.0988E-2;
        calcTemp = (-b+sqrt((b*b-4*a*(c-(float)(adc_val)*3.3/4096))))/(2*a);
        *Data = calcTemp * 10;

        return OK;
    }/* else{ */
#endif
}/**************************************** GetTmpr_PT100_CelsiusDgr ****************************************/

static STATUS Get_ID_TEMPERATURE_MNTR_INT_ZONE_PV_n_handle(int InstanceNum, int Index, int* Data)
{

    return (GetTmpr_PT100_CelsiusDgr(Index, Data));

#if 0  
    A2D_RES_ET res;
    A2D_DATA_ET a2d_res_inx;
    uint16_t temper_dig_val;
    uint32_t arrs_indx = 0, tmp_data_max, tmp_data_min;

    res = a2d_read_dig( (A2D_DATA_ET) (RtdId_2_Indx[Index]), &temper_dig_val);

    *Data = temper_dig_val;

    if (res==WRONG_PARAM)
    return ERROR;
    else
    {
        if (Index==0)
        M_LOGGER_LOGF (M_LOGGER_LEVEL_TRACE,"DIG_TEMP=%d",temper_dig_val);
        while(dig_val_arr[arrs_indx] < temper_dig_val)
        {
            arrs_indx++;
            if(arrs_indx == ((sizeof(dig_val_arr))/(sizeof(uint32_t))) )
            {
                arrs_indx--;
                break;
            }
        }/* while */

        tmp_data_max = 10*temper_val_arr[arrs_indx]*temper_dig_val/dig_val_arr[arrs_indx];
        arrs_indx--;
        tmp_data_min = 10*temper_val_arr[arrs_indx]*temper_dig_val/dig_val_arr[arrs_indx];
        *Data = (tmp_data_max + tmp_data_min)/2;

        return OK;
    }/* else */
#endif   
}/* Get_ID_TEMPERATURE_MNTR_INT_ZONE_PV_n_handle */

static STATUS Set_ID_TEMPERATURE_MNTR_INT_ZONE_MV_n_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{
    //for BTC the fill pump PWM is implemented by HW
    if ((GlobalSubSysId == SUBSYS_BTC || GlobalSubSysId == SUBSYS_CONDITIONER)
            && ((Index == FILL_PUMP_DIGITAL_OUTPUT_1) || (Index == FILL_PUMP_DIGITAL_OUTPUT_2)))
    {
        if (Data < 3)  //0 for turn off
            Data = 3;  //0 not working
        hw_pwm_dutysicle_set(FILL_PUMP_DIGITAL_OUTPUT_1, Data);
        hw_pwm_dutysicle_set(FILL_PUMP_DIGITAL_OUTPUT_2, Data);
    }
    else
    {
        if (Data == 0) //in case value=0%, just turn the pwm off
            Pwm_Enable(Index, false);
        else
        {
            //in case not enabled, enable it
            if (!IsPwm_Enable(Index))
                Pwm_Enable(Index, true);

            Data /= 10;  //0-1000=> 0-100%
            // allow direct control the PWM if in PID_HOST control type
            //	if (CfgRetInt(NULL,ID_TEMPERATURE_CFG_INT_CONTROL_TYPE)==CNTL_TYPE_PID_HOST)  // TBD need to distiguse it also by the Set flag: internal pid (or device) or exteranl from the host (ignore if get from the host if in internal mode)
            {
                //the the value of the PWM channel (Pwm channels 0-4 are dedicated for the zones' zones)
                Pwm_SetDutyCycle(Index, (U8) Data);
            }
        }
    }
    return OK;

}

static STATUS Set_ID_TEMPERATURE_MNTR_INT_ALARM_handle(int InstanceNum, int Index, CfgDB_INT_DATA Data,
        CfgDB_SET_FLAG *SetFlag)
{

    if (*SetFlag & CfgDB_SET_TEST_SANITY)
        return OK;

    //0 means ALARM CLEAR (default) and the output should be turned on. 1 means ALARM SET and the output signal should be turned off
    if (Data)
    {
        Safety_out_ctrl(DEACTIVE); //N.O - ALARM IS ON -> turn the output off
        Turn_led_(LABL_SAFETY_LED9, (BOOL) 0);  //also turn off the safety led
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "SAFETY ALARM IS ON");

    }
    else
    {
        Safety_out_ctrl(ACTIVE); //N.ON - ALARM IS OFF -> turn the output on
        Turn_led_(LABL_SAFETY_LED9, (BOOL) 1);  //also turn on the safety led
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "SAFETY ALARM IS OFF");
    }

    return OK;
}

/************************** VOLATILE STRING*************************************/

static STATUS Get_ID_TEMPERATURE_VOLATILE_STR_DATE_AND_TIME(int InstanceNum, int Index, char* Data)
{
#if _LL_TO_DO_   
    UTIL_GetDateAndTime(Data);
#endif    
    return OK;
}

static STATUS Set_ID_TEMPERATURE_VOLATILE_STR_DATE_AND_TIME(int InstanceNum, int Index, char* Data,
        CfgDB_SET_FLAG *SetFlag)
{
    //STATUS RetValue;
    int day, month, year;
    int Hour, Min, Sec;
    unsigned int prev_time;
    char EventStr[60];
#ifdef _NTP
    PM_Time rec_time, curr_time;
    int diff;
    unsigned char curr_time_buf[10];
#endif

    if (strlen(Data) < 19)
        return ERROR;

    /* format validation check*/
    if ((Data[2] != '-') || (Data[5] != '-') || (Data[10] != ' ') || (Data[13] != ':') || (Data[16] != ':'))
        return ERROR;
    sscanf(Data, "%d-%d-%d %d:%d:%d", &day, &month, &year, &Hour, &Min, &Sec);

    if ((year < 2000) || (year > 2035) || (day < 1) || (day > 31) || (month < 1) || (month > 12))
        return ERROR;

    if ((Hour < 0) || (Hour > 23) || (Min < 0) || (Min > 59) || (Sec < 0) || (Sec > 59))
        return ERROR;

    if (*SetFlag & CfgDB_SET_TEST_SANITY)
        return OK;

#ifdef _NTP
    rec_time.second = (unsigned char) Sec;
    rec_time.minute = (unsigned char) Min;
    rec_time.hour = (unsigned char) Hour;
    rec_time.day = (unsigned char) day;
    rec_time.month = (unsigned char) month;
    rec_time.year = (unsigned short) year;

    UTIL_Time2Hex(curr_time_buf);

    curr_time.second = (unsigned char) curr_time_buf[6];
    curr_time.minute = (unsigned char) curr_time_buf[5];
    curr_time.hour = (unsigned char) curr_time_buf[4];
    curr_time.day = (unsigned char) curr_time_buf[3];
    curr_time.month = (unsigned char) curr_time_buf[2];
    curr_time.year = (unsigned short) curr_time_buf[0]*0x100+curr_time_buf[1];

    diff = ( PMDB_get_seconds_by_timestamp(rec_time) - PMDB_get_seconds_by_timestamp(curr_time) );

    if ( FirstNTPUpdateFlag == TRUE || (abs(diff)) > 30 )
    {
        FirstNTPUpdateFlag = FALSE;
#endif
    sprintf(EventStr, "Time & Date was set - %s", Data);
#if _LL_TO_DO_                  
    EVENT_Set(NULL, ID_TEMPERATURE_ALARM_EVENT_SET_TIME_DATE, EventStr, NULL, ALARM_SET_FLAG_CHECK_ALL);
#endif                
#ifdef _NTP
}
#endif

    /* Before setting the time wait that the previous event will be handled
     by the alarm task because we need that the time stamp in the log entry
     will be with the time before the modification*/
    {
#if _LL_TO_DO_            
        SEM_ID AlarmQEmptySem;

        AlarmQEmptySem=semBCreate(SEM_Q_FIFO,SEM_EMPTY);
        if ((RetValue=ALARM_SendMsg(ALARM_MSG_TYPE_GIVE_SEM,
                                NULL,(U32)AlarmQEmptySem,NULL,NULL,NULL,NULL))==OK)
        {

            if (semTake(AlarmQEmptySem,WAIT_FOREVER))
            {
                M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in ALARM_WaitForEmptyQueue in semTake AlarmQEmptySem");
            }
        }
        semDelete(AlarmQEmptySem);
#endif                
    }
    prev_time = time(0);
#if _LL_TO_DO_     
    UTILS_SetDate(day, month, year);
    UTILS_SetTime(Hour,Min,Sec);
    CARRIER_Time_Correct(prev_time);
    Temperature_sync_time();
#endif    

    /* to disable changing clock twice */

    return OK;

}

/************************** CFG STRING*************************************/

static STATUS Set_ID_TEMPERATURE_CFG_STR_TERM_SUPER_PASS_handle(int InstanceNum, int Index, char* Data,
        CfgDB_SET_FLAG *SetFlag)
{

#if _LL_TO_DO_
    if(!TemperatureDbInInit())
    {
        if (*SetFlag & CfgDB_SET_TEST_SANITY)
        {
            // to do		strcpy(SuperUserPassword,Data);
            return OK;
        }
        if(strcmp(Data,VerifySuperUserPassword)==0)
        return OK;
        else
        return ERROR;
    }
    else
#endif          
    return OK;
}

/**********************************/

/* The following structure contains the description data for configuration integer data type DB of TEMPERATURE*/
CfgDB_ID_IntDescriptor TemperatureDbIntDescriptor[CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST)] =
{ /* default values */
/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,     GetHandle,   SetHandle*/

/*ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n*/
{ "ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n", INFORM_NOTHING, 0, 5000, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE
        | IGNORED_SET_HANDLE_DURING_INIT, NUMBER_OF_BOARD_TEMPERATURE_ZONES, 0, NULL,
        Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n_handle },

/*ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n*/
{ "ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n", INFORM_NOTHING, 0, 5000, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE
        | IGNORED_SET_HANDLE_DURING_INIT, NUMBER_OF_BOARD_TEMPERATURE_ZONES, 0, NULL,
        Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n_handle },

/*ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n*/
{ "ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n", INFORM_NOTHING, 0, 5000, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE
        | IGNORED_SET_HANDLE_DURING_INIT, NUMBER_OF_BOARD_TEMPERATURE_ZONES, 0, NULL,
        Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n_handle },

/*ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n*/
{ "ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n", INFORM_NOTHING, 0, 5000, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE
        | IGNORED_SET_HANDLE_DURING_INIT, NUMBER_OF_BOARD_TEMPERATURE_ZONES, 10, NULL,
        Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n_handle },

/*ID_TEMPERATURE_CFG_INT_ZONE_PID_I0_n*/
{ "ID_TEMPERATURE_CFG_INT_ZONE_PID_I0_n", INFORM_NOTHING, 0, 5000, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE
        | IGNORED_SET_HANDLE_DURING_INIT, NUMBER_OF_BOARD_TEMPERATURE_ZONES, 0, NULL,
        Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n_handle },

/*ID_TEMPERATURE_CFG_INT_ZONE_PID_D0_n*/
{ "ID_TEMPERATURE_CFG_INT_ZONE_PID_D0_n", INFORM_NOTHING, 0, 5000, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE
        | IGNORED_SET_HANDLE_DURING_INIT, NUMBER_OF_BOARD_TEMPERATURE_ZONES, 0, NULL,
        Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n_handle },

/*ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n*/
{ "ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n", INFORM_NOTHING, 0, 1000, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE,
        NUMBER_OF_BOARD_TEMPERATURE_ZONES, 1000, NULL, Set_ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n_handle },

};

/* The following structure contains the description data for command integer data type DB of TEMPERATURE*/
CfgDB_ID_IntDescriptor TemperatureDbCmndIntDescriptor[CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST)] =
{ /* default values */
/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

/* ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES*/
{ "ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES", INFORM_NOTHING, FALSE, TRUE, UNSIGNED_VALUE
        | WRITE_RAM_BEFORE_SET_HANDLE, NUMBER_OF_BOARD_TEMPERATURE_ZONES, FALSE, NULL,
        Set_ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES_handle },

/* ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n*/
{ "ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n", INFORM_NOTHING, FALSE, TRUE, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE,
        NUMBER_OF_BOARD_TEMPERATURE_ZONES, FALSE, NULL, Set_ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n_handle },

/* ID_TEMPERATURE_CMND_INT_ZONE_SP_n*/
{ "ID_TEMPERATURE_CMND_INT_ZONE_SP_n", INFORM_NOTHING, MIN_ZONE_AVAILIABLE_TEMPERATURE, MAX_ZONE_AVAILIABLE_TEMPERATURE,
        UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE, NUMBER_OF_BOARD_TEMPERATURE_ZONES, 900, NULL,
        Set_ID_TEMPERATURE_CMND_INT_ZONE_SP_n_handle },

/* ID_TEMPERATURE_CMND_INT_ZONE_SP0_n*/
{ "ID_TEMPERATURE_CMND_INT_ZONE_SP0_n", INFORM_NOTHING, MIN_ZONE_AVAILIABLE_TEMPERATURE,
        MAX_ZONE_AVAILIABLE_TEMPERATURE, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE,
        NUMBER_OF_BOARD_TEMPERATURE_ZONES, 4900, NULL, NULL },

/*ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n*/
{ "ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE | WRITE_RAM_BEFORE_SET_HANDLE,
        NUMBER_OF_BOARD_TEMPERATURE_ZONES, 0, NULL, Set_ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n_handle },

/*ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n*/
{ "ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n", INFORM_NOTHING, -100, 100, SIGNED_VALUE, NUMBER_OF_BOARD_TEMPERATURE_ZONES,
        0, NULL, Set_ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n_handle },

};

/* The following structure contains the description data for monitor integer data type DB of TEMPERATURE*/
CfgDB_ID_IntDescriptor TemperatureDbMntrIntDescriptor[CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST)] =
{ /* default values */
/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

/* ID_TEMPERATURE_MNTR_INT_ZONE_PV_n */
{ "ID_TEMPERATURE_MNTR_INT_ZONE_PV_n", INFORM_NOTHING, MIN_ZONE_AVAILIABLE_TEMPERATURE, MAX_ZONE_AVAILIABLE_TEMPERATURE,
        UNSIGNED_VALUE, NUMBER_OF_TEMPERATURE_ZONES, 0, Get_ID_TEMPERATURE_MNTR_INT_ZONE_PV_n_handle, NULL },

/* ID_TEMPERATURE_MNTR_INT_ZONE_MV_n */
{ "ID_TEMPERATURE_MNTR_INT_ZONE_MV_n", INFORM_NOTHING, 0, 1000, UNSIGNED_VALUE, NUMBER_OF_BOARD_TEMPERATURE_ZONES, 0,
        NULL, Set_ID_TEMPERATURE_MNTR_INT_ZONE_MV_n_handle }, //0-100%

        /*ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n*/
        { "ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n", INFORM_NOTHING, 0, 1000, UNSIGNED_VALUE,
                NUMBER_OF_BOARD_TEMPERATURE_ZONES, 0, NULL, NULL }, //0-100%

        /*ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM*/
        { "ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM", INFORM_NOTHING, FALSE, TRUE, UNSIGNED_VALUE, 1, FALSE, NULL,
                Set_ID_TEMPERATURE_MNTR_INT_ALARM_handle },   // global alarm

        };

/* this starcture contains the default configuration data of TEMPERATURE string type*/
CfgDB_ID_StrDescriptor TemperatureDbStrDescriptor[CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST)] =
{ /* default values */
/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

/* ID_TEMPERATURE_CFG_STR_TBD */
{ "ID_TEMPERATURE_CFG_STR_TBD", INFORM_NOTHING, MAX_LEN_PASSWORD-1, 1, "Invalid", NULL, NULL }, };

/* this starcture contains the default configuration data of MUX string type*/
CfgDB_ID_StrDescriptor TemperatureDbVolatileStrDescriptor[CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST)] =
{ /* default values */
/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

/*ID_TEMPERATURE_VOLATILE_STR_GET_APPLIC_DESCRIPTION*/
{ "ID_TEMPERATURE_VOLATILE_STR_TBD", INFORM_NOTHING, MAX_CFG_STR_SIZE, 1, "", NULL, NULL },

};

/* configuration integer element buffer pointers*/
static U32 * TemperatureDbIntPtr[CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST)];
/* configuration string element buffer pointers*/
static char * TemperatureDbStrPtr[CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST)];
/* configuration volatile string element buffer pointers*/
static char * TemperatureDbVolatileStrPtr[CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST)];
/* commands integer element buffer pointers*/
static U32 * TemperatureDbCmndIntPtr[CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST)];
/* monitor integer element buffer pointers*/
static U32 * TemperatureDbMntrIntPtr[CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST)];

/*************************************************************************************************
 Allocate and initialize the required memory in the RAM for the database elements
 *************************************************************************************************/
static STATUS CreateRamDataBase()
{

    /* create the configuration Db of type integer*/
    if (CfgDB_CreateIntRamDB(&TemperatureDbIntDescriptor[0], &TemperatureDbIntPtr[0],
            CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST)) == ERROR)
        return ERROR;

    /* create the configuration Db of type string*/
    if (CfgDB_CreateStrRamDB(&TemperatureDbStrDescriptor[0], &TemperatureDbStrPtr[0],
            CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST)) == ERROR)
        return ERROR;

    /* create the configuration Db of type volatile string*/
    if (CfgDB_CreateStrRamDB(&TemperatureDbVolatileStrDescriptor[0], &TemperatureDbVolatileStrPtr[0],
            CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST)) == ERROR)
        return ERROR;

    /* create the command Db */
    if (CfgDB_CreateIntRamDB(&TemperatureDbCmndIntDescriptor[0], &TemperatureDbCmndIntPtr[0],
            CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST)) == ERROR)
        return ERROR;

    /* create the monitor Db */
    if (CfgDB_CreateIntRamDB(&TemperatureDbMntrIntDescriptor[0], &TemperatureDbMntrIntPtr[0],
            CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST)) == ERROR)
        return ERROR;

    return OK;
}

static void RemoveRamDataBase()
{
    /* remove the configuration Db of type integer*/
    CfgDB_DeleteIntRamDB(&TemperatureDbIntPtr[0], CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST));
    /* remove the configuration Db of type string*/
    CfgDB_DeleteStrRamDB(&TemperatureDbStrPtr[0], CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST));
    /* remove the volatile Db of type string*/
    CfgDB_DeleteStrRamDB(&TemperatureDbVolatileStrPtr[0], CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST));
    /* remove the command Db of type integer*/
    CfgDB_DeleteIntRamDB(&TemperatureDbCmndIntPtr[0], CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST));
    /* remove the monitor Db of type integer*/
    CfgDB_DeleteIntRamDB(&TemperatureDbMntrIntPtr[0], CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST));
}

#if WIN32
BOOL TemperatureDbRdy=TRUE;
#else
BOOL TemperatureDbRdy = FALSE;
#endif

#if WIN32
BOOL TemperatureDbInInitState=FALSE;
#else
BOOL TemperatureDbInInitState = TRUE;
#endif

STATUS TemperatureCfgBuildDB(BOOL SkipDefaultFile)
{
    char DefaultFileName[100];
    char ModifiedFileName[100];
    CfgDB_ID id;
    STATUS RetValue;

    if (!SkipDefaultFile)
    {
        /*allocates space in the RAM for the database*/
        if (CreateRamDataBase() == ERROR)
        {
            RemoveRamDataBase();
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "TemperatureCfgInit- Error form CreateRamDataBase. ");
            return ERROR;
        }
    }

    id.Field.UnitType = CfgDB_UnitTEMPERATURE;
    id.Field.DataType = CfgDB_INT;
    id.Field.AccessType = CfgDB_NON_VOLATILE;
    /* Create and initialize the Temperature int files  */
    if ((RetValue = CfgDB_FindFilesPathAndNames(id, DefaultFileName, ModifiedFileName, 0)) != OK) /* find the exact name*/
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                "TemperatureCfgInit- Error form CfgDB_FindFilesPathAndNames for integer type id=0x%x. ", id.DataAssign);
        return RetValue;
    }

#if _LL_TO_DO_     

    CfgDB_TakeCfgSem(CARRIER_TEMPERATURE);
    /* create and init files with default data*/
    if ((RetValue=FileHandCreateAndInitCfgIntFileDB(DefaultFileName,ModifiedFileName,ID_TEMPERATURE_CFG_INT_FIRST, ID_TEMPERATURE_CFG_INT_LAST, &TemperatureDbIntDescriptor[0], &TemperatureDbIntPtr[0],SkipDefaultFile))!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form FileHandCreateAndInitCfgIntFileDB. RetValue=%d", RetValue);
    }

    CfgDB_GiveCfgSem(CARRIER_TEMPERATURE);
    id.Field.DataType=CfgDB_STR;

    /* Create and initialize the Temperature int files  */
    if ((RetValue=CfgDB_FindFilesPathAndNames(id,DefaultFileName,ModifiedFileName,0))!=OK) /* find the exact name*/
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form CfgDB_FindFilesPathAndNames for string type id=0x%x. ", id);
        return RetValue; /* create and init files with default data*/
    }

    CfgDB_TakeCfgSem(CARRIER_TEMPERATURE);
    if ((RetValue=FileHandCreateAndInitCfgStrFileDB(DefaultFileName,ModifiedFileName,
                            ID_TEMPERATURE_CFG_STR_FIRST, ID_TEMPERATURE_CFG_STR_LAST,
                            &TemperatureDbStrDescriptor[0], &TemperatureDbStrPtr[0],SkipDefaultFile))!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form FileHandCreateAndInitCfgStrFileDB. RetValue=%d", RetValue);
    }
    CfgDB_GiveCfgSem(CARRIER_TEMPERATURE);
#endif
    return OK;
}

STATUS TemperatureCfgUpdateDbFromSource(BOOL ConfigMismatchCheck, BOOL LoadConfigFromUnit)
{
    STATUS RetValue;
    /* update the integer DB from the content of the source - Alarm if mismatch*/
    if ((RetValue = CfgDB_UpdateIntDbFromSource(-1, &TemperatureDbIntPtr[0], &TemperatureDbIntDescriptor[0],
    ID_TEMPERATURE_CFG_INT_FIRST, ID_TEMPERATURE_CFG_INT_LAST, ConfigMismatchCheck, LoadConfigFromUnit)) != OK)
    {
        if (RetValue != CFG_DB_ERROR_ID_CONFIG_MISMATCH)
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                    "ERROR in TemperatureCfgUpdateDbFromSource: CfgDB_UpdateIntDbFromSource RetValue=0x%x", RetValue);
    }

    return RetValue;
}

/*************************************************************************************************
 Full initialization for the TEMPERATURE DB.
 **************************************************************************************************/
STATUS TemperatureCfgInit()
{
    //char DefaultFileName[100];
    //char ModifiedFileName[100];
    //CfgDB_ID id;
    STATUS RetValue;
    //int i;
    //U32 oem_enable;
    TemperatureDbRdy = FALSE;
    TemperatureDbInInitState = TRUE;

    InitPeripheralsModBusDescriptionr(GlobalSubSysId);

#if _LL_TO_DO_ 
    if ((RetValue=AlarmTEMPERATURE_Init())!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form AlarmTEMPERATURE_Init. ");
        return RetValue;
    }
#endif

    if ((RetValue = TemperatureCfgBuildDB(FALSE)) != OK)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR in TemperatureCfgBuildDB: RetValue=0x%x", RetValue);

#if _LL_TO_DO_ 
    /* the other task that need the TEMPERATURE DB in the following inform can now used the TEMPERATURE DB*/
    TemperatureCfgUpdateEepromStruct();

#endif

    if ((RetValue = TemperatureCfg_UpdatePowerUpMntrDB()) != OK) /* update monitor DB from TEMPERATURE HW peripherals*/
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                "TemperatureCfgInit- Error form TemperatureCfg_UpdatePowerUpMntrDB. RetValue=%d ", RetValue);
    }

    /* call to all relevant set handle of each entry in the str DB (only those which has one)
     for completed updating */
    if ((RetValue = CfgDB_CallToAllStrSetHandlesAndInformClients(0, &TemperatureDbStrPtr[0],
            &TemperatureDbStrDescriptor[0], ID_TEMPERATURE_CFG_STR_FIRST, ID_TEMPERATURE_CFG_STR_LAST)) != OK)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                "TemperatureCfgInit- Error form CfgDB_CallToAllStrSetHandlesAndInformClients. RetValue=%d ", RetValue);
    }

    /* call to all relevant set handle of each entry in the int DB (only those which has one)
     for completed updating */
    /*
     if ((RetValue=CfgDB_CallToAllIntSetHandlesAndInformClients(0,&TemperatureDbIntPtr[0],
     &TemperatureDbIntDescriptor[0],
     ID_TEMPERATURE_CFG_INT_FIRST, ID_TEMPERATURE_CFG_INT_LAST))!=OK)
     {
     M_LOGGER_LOGF (M_LOGGER_LEVEL_TRACE,"TemperatureCfgInit- Error form CfgDB_CallToAllIntSetHandlesAndInformClients. RetValue=%d", RetValue, 0, 0, 0,0,0);
     }
     */

#if _LL_TO_DO_         
    /* update the last time configuration is modified - set to current time as default*/
    TemperatureCfgDbUpdateLastConfigChangeTime();
#endif
    TemperatureDbInInitState = FALSE;
    TemperatureDbRdy = TRUE;

    return RetValue;

}

STATUS TemperatureCfgDelete()
{
    if (!TemperatureDbRdy)
        return ERROR;

    TemperatureDbRdy = FALSE;
    RemoveRamDataBase();
    return OK;

}

int TemperatureCfgDbReady()
{
#if WIN32
    return TRUE;
#else
    return TemperatureDbRdy;
#endif
}

int TemperatureDbInInit()
{
#if WIN32
    return FALSE;
#else
    return TemperatureDbInInitState;
#endif

}

/*************************************************************************************************
 Get element of any type (integer or tring) from the TEMPERATURE database
 **************************************************************************************************/
STATUS TemperatureCfgDB_GetElement(CfgDB_ID Id, CfgDB_ID_data * buff, int DataLen, CfgDB_GET_FLAG GetFlag)
{

    switch (Id.Field.AccessType)
    {
    case CfgDB_NON_VOLATILE: /* get configuration data element*/
    {
        if (Id.Field.DataType == CfgDB_INT)
        {
            /* check for leggal id*/
            if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST))
                return CFG_DB_ERROR_ID_LIMIT;

            return (CfgDB_GetIntElement(0, Id, &TemperatureDbIntPtr[0], &TemperatureDbIntDescriptor[0], &buff->IntData,
                    GetFlag));
        }
        else
        {
            if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST))
                return CFG_DB_ERROR_ID_LIMIT;

            return (CfgDB_GetStrElement(0, Id, &TemperatureDbStrPtr[0], &TemperatureDbStrDescriptor[0], buff->StrData,
                    DataLen, GetFlag));
        }
        break;
    }
    case CfgDB_COMMAND:
    {
        /* check for leggal id*/
        if (Id.Field.DataType != CfgDB_INT)
            return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be command*/
        /* check for available id */
        if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST))
            return CFG_DB_ERROR_ID_LIMIT;

        return (CfgDB_GetIntElement(0, Id, &TemperatureDbCmndIntPtr[0], &TemperatureDbCmndIntDescriptor[0],
                &buff->IntData, GetFlag));
    }
    case CfgDB_MONITOR:
    {
        /* check for leggal id*/
        if (Id.Field.DataType != CfgDB_INT)
            return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
        /* check for available id */
        if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST))
            return CFG_DB_ERROR_ID_LIMIT;

        return (CfgDB_GetIntElement(0, Id, &TemperatureDbMntrIntPtr[0], &TemperatureDbMntrIntDescriptor[0],
                &buff->IntData, GetFlag));

    }

    case CfgDB_VOLATILE_STR:
    {
        if (Id.Field.DataType != CfgDB_STR)
            return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

        if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST))
            return CFG_DB_ERROR_ID_LIMIT;

        return (CfgDB_GetStrElement(0, Id, &TemperatureDbVolatileStrPtr[0], &TemperatureDbVolatileStrDescriptor[0],
                buff->StrData, DataLen, GetFlag));

    }
    default:
        return CFG_DB_ERROR_ID_ACCESS_TYPE;
    }

}

/*************************************************************************************************
 Set element of any type (integer or tring) into the TEMPERATURE database
 **************************************************************************************************/
STATUS TemperatureCfgDB_SetElement(CfgDB_ID Id, CfgDB_ID_data * buff, CfgDB_SET_FLAG SetFlag)
{

    switch (Id.Field.AccessType)
    {
    case CfgDB_NON_VOLATILE: /* get configuration data element*/
    {
        if (Id.Field.DataType == CfgDB_INT)
        { /* integer data type*/
            /* check for leggal id*/
            if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST))
                return CFG_DB_ERROR_ID_LIMIT;
            /* call the the generic routine*/
            return (CfgDB_SetIntElement(0, Id, &TemperatureDbIntPtr[0], &TemperatureDbIntDescriptor[0], buff->IntData,
                    SetFlag));
        }
        else
        { /* string data type*/
            /* check for leggal id*/
            if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST))
                return CFG_DB_ERROR_ID_LIMIT;

            return (CfgDB_SetStrElement(0, Id, &TemperatureDbStrPtr[0], &TemperatureDbStrDescriptor[0], buff->StrData,
                    strlen(buff->StrData), SetFlag));
        }
    }
    case CfgDB_COMMAND:
    {
        if (Id.Field.DataType != CfgDB_INT)
            return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be command*/
        /* check for leggal id*/
        if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST))
            return CFG_DB_ERROR_ID_LIMIT;

        return (CfgDB_SetIntElement(0, Id, &TemperatureDbCmndIntPtr[0], &TemperatureDbCmndIntDescriptor[0],
                buff->IntData, SetFlag));
    }
    case CfgDB_MONITOR:
    {
        if (Id.Field.DataType != CfgDB_INT)
            return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
        /* check for leggal id*/
        if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST))
            return CFG_DB_ERROR_ID_LIMIT;

        return (CfgDB_SetIntElement(0, Id, &TemperatureDbMntrIntPtr[0], &TemperatureDbMntrIntDescriptor[0],
                buff->IntData, SetFlag));

    }

    case CfgDB_VOLATILE_STR:
    {

        if (Id.Field.DataType != CfgDB_STR)
            return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

        if (Id.Field.Id >= CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST))
            return CFG_DB_ERROR_ID_LIMIT;

        return (CfgDB_SetStrElement(0, Id, &TemperatureDbVolatileStrPtr[0], &TemperatureDbVolatileStrDescriptor[0],
                buff->StrData, strlen(buff->StrData), SetFlag));
    }
    default:
        return CFG_DB_ERROR_ID_ACCESS_TYPE;
    }

}

static STATUS TemperatureCfg_UpdatePowerUpMntrDB()
{
    STATUS RetValue = OK;

    if ((RetValue = CfgDB_UpdateIntDbFromSource(0, &TemperatureDbMntrIntPtr[0], &TemperatureDbMntrIntDescriptor[0],
    ID_TEMPERATURE_MNTR_INT_FIRST, ID_TEMPERATURE_MNTR_INT_LAST, FALSE, FALSE)) != OK)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR in GnrlCfg_UpdatePowerUpMntrDB: CfgDB_UpdateIntDbFromSource ");
        return RetValue;
    }

    return RetValue;
}

STATUS TemperatureCfgShowDb(BOOL IntType)
{
    int id, index;
    CfgDB_INT_DATA* IntBufPtr;
    char * StrBufPtr;

    if (IntType)
    {

        for (id = 0; id < CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST); id++)
        {
            IntBufPtr = TemperatureDbIntPtr[id];
            for (index = 0; index < TemperatureDbIntDescriptor[id].MaxAllowedIndex; index++)
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "id=%d, id_index=%d, data=0x%x", id, index, IntBufPtr[index]);
        }

    }
    else
    {

        for (id = 0; id < CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST); id++)
        {
            StrBufPtr = TemperatureDbStrPtr[id];
            for (index = 0; index < TemperatureDbStrDescriptor[id].MaxAllowedIndex; index++)
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "id=%d, id_index=%d, data=%s", id, index,
                        (char*)StrBufPtr+index*MAX_CFG_STR_SIZE);
        }
    }

    return OK;
}

BOOL TemperatureDbIntIdIsNotDefault(CfgDB_INT_DATA id)
{
    U32 Data;
    U32 DefaultValue = 0;
    CfgDB_ID Id;

    if (CfgGetParam(NULL, id, &Data, sizeof(Data), CfgDB_GET_FROM_DB) == OK)
    {
        Id.DataAssign = id;
        switch (Id.Field.AccessType)
        {
        case CfgDB_NON_VOLATILE: /* get configuration data element*/
            DefaultValue = TemperatureDbIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
            break;
        case CfgDB_COMMAND:
            DefaultValue = TemperatureDbCmndIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
            break;
        case CfgDB_MONITOR:
            DefaultValue = TemperatureDbMntrIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
            break;
        }
        if (Data != DefaultValue)
            return TRUE; /* different from the default value*/
        else
            return FALSE; /* equal to the default value*/
    }
    else
        return FALSE;

}

/* update DB elements according to the status in the EEPROM*/
STATUS TemperatureCfgUpdateEepromStruct()
{
    STATUS RetValue = OK;
#if _LL_TO_DO_  
    EEPROM_HW_CONFIG_STRUCT HwConfigStruct;
    EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT ProductionBoardIdStruct;
    U32 data;
    int index;

#if !WIN32
    if ((RetValue=EEPROM_GetHwConfigurationValues(NULL,BOARD_TEMPERATURE,&HwConfigStruct))!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in TemperatureCfgUpdateEepromStruct: in EEPROM_GetHwConfigurationValues");
        return RetValue;
    }
    if ((RetValue=EEPROM_GetBoardIdValues(NULL,BOARD_TEMPERATURE,&ProductionBoardIdStruct))!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in TemperatureCfgUpdateEepromStruct: in EEPROM_GetBoardIdValues");
        return RetValue;
    }

#else
    HwConfigStruct.BoardType=EEPROM_BOARD_TEMPERATURE;
    HwConfigStruct.BoardSubType=0;
    HwConfigStruct.InterfaceType[0]=MUX_BOARD_TYPE_INTERFACE_OPTIC_SINGLE_MODE_SHORT_HAUL;
    HwConfigStruct.NumOfInterface[0]=1;
    HwConfigStruct.CRC=0;
    ProductionBoardIdStruct.NumOfWorkingHours=0;
#endif
    data=HwConfigStruct.BoardType;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_HW_BOARD_TYPE,&data,CfgDB_SET_TO_RAM);

    data=HwConfigStruct.Firmware;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_HW_FIRMWARE,&data,CfgDB_SET_TO_RAM);

    data=HwConfigStruct.Hardware;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_HW_HARDWARE,&data,CfgDB_SET_TO_RAM);

    data=HwConfigStruct.BoardSubType;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_HW_BOARD_SUB_TYPE,&data,CfgDB_SET_TO_RAM);

    for (index=0;index<EEPROM_MAX_NUM_OF_INTERFACES;index++)
    {
        data=HwConfigStruct.InterfaceType[index];
        CfgPutParam(NULL,CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_EEPROM_HW_INTERFACE_TYPE_n, index),&data,CfgDB_SET_TO_RAM);

        data=HwConfigStruct.NumOfInterface[index];
        CfgPutParam(NULL,CfgDB_ID_AddIndex(ID_TEMPERATURE_MNTR_INT_EEPROM_HW_NUM_OF_INTERFACES_n, index),&data,CfgDB_SET_TO_RAM);
    }
    data=HwConfigStruct.CRC;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_HW_CRC,&data,CfgDB_SET_TO_RAM);

    data=ProductionBoardIdStruct.NumOfWorkingHours;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_NUM_OF_WORKING_HOURS,&data,CfgDB_SET_TO_RAM);
    data=ProductionBoardIdStruct.DateCode;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_DATE_CODE,&data,CfgDB_SET_TO_RAM);
    data=ProductionBoardIdStruct.HASS_Num;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_HASS_NUM,&data,CfgDB_SET_TO_RAM);
    data=ProductionBoardIdStruct.Feature;
    CfgPutParam(NULL,ID_TEMPERATURE_MNTR_INT_EEPROM_FEATURE,&data,CfgDB_SET_TO_RAM);
    CfgPutParam(NULL,ID_TEMPERATURE_VOLATILE_STR_EEPROM_SERIAL_NUM,ProductionBoardIdStruct.SerialNum,CfgDB_SET_TO_RAM);
    CfgPutParam(NULL,ID_TEMPERATURE_VOLATILE_STR_EEPROM_PART_NUM,ProductionBoardIdStruct.PartNum,CfgDB_SET_TO_RAM);
    CfgPutParam(NULL,ID_TEMPERATURE_VOLATILE_STR_EEPROM_DESCRIPTION_1,ProductionBoardIdStruct.Comment1,CfgDB_SET_TO_RAM);
    CfgPutParam(NULL,ID_TEMPERATURE_VOLATILE_STR_EEPROM_DESCRIPTION_2,ProductionBoardIdStruct.Comment2,CfgDB_SET_TO_RAM);
    CfgPutParam(NULL,ID_TEMPERATURE_VOLATILE_STR_EEPROM_DESCRIPTION_3,ProductionBoardIdStruct.Comment3,CfgDB_SET_TO_RAM);

#endif  
    if (RetValue == OK)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "TemperatureCfgUpdateEepromStruct was update");
    return RetValue;

}

CfgDB_ID_IntDescriptor * TemperatureCfg_DbIntDescriptor(CfgDB_ACCESS_TYPE AccessType, int *NumOfElements)
{
    switch (AccessType)
    {
    case CfgDB_NON_VOLATILE:
        *NumOfElements = CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST);
        return &TemperatureDbIntDescriptor[0];
    case CfgDB_COMMAND:
        *NumOfElements = CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST);
        return &TemperatureDbCmndIntDescriptor[0];
    case CfgDB_MONITOR:
        *NumOfElements = CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST);
        return &TemperatureDbMntrIntDescriptor[0];
    }
    return NULL;
}

CfgDB_ID_StrDescriptor * TemperatureCfg_DbStrDescriptor(CfgDB_ACCESS_TYPE AccessType, int *NumOfElements)
{
    switch (AccessType)
    {
    case CfgDB_NON_VOLATILE:
        *NumOfElements = CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST);
        return &TemperatureDbStrDescriptor[0];
    case CfgDB_VOLATILE_STR:
        *NumOfElements = CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST);
        return &TemperatureDbVolatileStrDescriptor[0];

    }

    return NULL;

}

#ifdef __cplusplus
 }
#endif
