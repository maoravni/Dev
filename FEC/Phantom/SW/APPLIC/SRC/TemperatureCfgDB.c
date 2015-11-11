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

#include <string.h>

#include "free_rtos_exp.h" 
#include "os_cnfg.h"


//#include "pq_defs.h"
#include <stdlib.h>
#include <stdio.h>
//#include "syslib.h"
#include "ll_typedef.h"
#include "Phantom.h"

#include "CfgDB.h"
#include "CfgDBUtil.h"
#include "TemperatureCfgDB.h"
#include "PWM.h"
#include "Opto_out.h"

#include "mb_cnfg.h"
#include "..\..\..\Common\MODBUS\modbus_exp.h"
#include "..\..\..\Common\MODBUS\RTU\rtu_master.h"
#include "rtc.h"
#ifdef CPP_FRAMEWORK
#include "logger.h"
#endif



#define MOD_BUS_DEVICE_ADDR_PUMA_1	1
#define MOD_BUS_DEVICE_ADDR_PUMA_2	2
#define MOD_BUS_DEVICE_ADDR_PUMA_3	3
#define MOD_BUS_PUMA_REGISTER_START_CH_1	5000
#define MOD_BUS_PUMA_REGISTER_START_CH_2	5008
#define MOD_BUS_PUMA_REGISTER_START_CH_3	5016
#define MOD_BUS_PUMA_REGISTER_START_CH_4	5024
#define MOD_BUS_PUMA_NUM_OF_ELEMENTS            8

#define HTX_ALARM_HIGH_TEMERATURE_TH 5800 // on 580 dgrees the safetry alarm will be activated which turn off the power
#define ZONE_ALARM_HIGH_TEMERATURE_TH 4500 // on 450 dgrees the safetry alarm will be activated which turn off the power


#define TEMPERATURE_NUMBER_OF_DEVICES_TC 12  

#define TEMPERATURE_NUMBER_OF_DEVICES  9  // 4 dryer TC, 5 IBD



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


#define MOD_BUS_8TC_NUM_OF_ELEMENTS   8

#define MOD_BUS_6RTD_NUM_OF_ELEMENTS   6

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


typedef enum {
        DEV_PUMA,
        DEV_8TC,
        DEV_6RTD,
        DEV_8AI
 }   DEVICE_TYPE;

//addresses for set commands to the PUMA
typedef struct {
		U16 Enable;
		U16 SP;   
		U16 PID_P;
		U16 PID_I;
		U16 PID_D;
		U16 PowerLimit;
		U16 AutoTune;
} SET_PUMA_REGISTER_ADDRESS;

typedef struct {
		U32 InstanceId;
		U32 DB_IdIndex;
		U32 DB_Id[8];  //up to 8 element each puma's modbus read
	} DB_ID_TC;

typedef struct {
        DEVICE_TYPE  DeviceType;
	U16 DeviceId; 
	U16 RegisterId;
	U8  NumberOfElements;
	DB_ID_TC DB_id;
	SET_PUMA_REGISTER_ADDRESS  SetPumaRegisterAddress;
}  CfgDB_ID_TC_PeripheralsModBusDescription;


//RO temperatute
typedef struct {
		U32 InstanceId;
		U32 StartIdIndex;
		U32 DB_Id;  
	} DB_ID;

typedef struct {
        DEVICE_TYPE  DeviceType;
	U16 DeviceId; 
	U16 RegisterId;
	U8  NumberOfElements;
	DB_ID DB_id;
}  CfgDB_ID_PeripheralsModBusDescription;


static CfgDB_ID_PeripheralsModBusDescription * PeripheralsModBusDescriptionPtr[TEMPERATURE_NUMBER_OF_DEVICES+1];


const static CfgDB_ID_PeripheralsModBusDescription PeripheralsModBusDescriptionr[TEMPERATURE_NUMBER_OF_DEVICES]=
{
  //Device#1 8TC_1
  {DEV_8TC,MOD_BUS_DEVICE_ADDR_8TC_1,MOD_BUS_8TC_REGISTER_START,MOD_BUS_8TC_NUM_OF_ELEMENTS,{NULL,12,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}},  //Dryer 12-19
  {DEV_8TC,MOD_BUS_DEVICE_ADDR_8TC_2,MOD_BUS_8TC_REGISTER_START,MOD_BUS_8TC_NUM_OF_ELEMENTS-1,{NULL,20,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}},  //Dryer 20-26 
  {DEV_8TC,MOD_BUS_DEVICE_ADDR_8TC_3,MOD_BUS_8TC_REGISTER_START,MOD_BUS_8TC_NUM_OF_ELEMENTS,{NULL,27,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}},  //Dryer 27-34 
  {DEV_8TC,MOD_BUS_DEVICE_ADDR_8TC_4,MOD_BUS_8TC_REGISTER_START,MOD_BUS_8TC_NUM_OF_ELEMENTS-1,{NULL,35,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}},  //Dryer 35-41 

  {DEV_6RTD,MOD_BUS_DEVICE_ADDR_6RTD_1,MOD_BUS_6RTD_REGISTER_START,MOD_BUS_6RTD_NUM_OF_ELEMENTS,{NULL,42,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}},  //IBD pt100  42-47
  {DEV_6RTD,MOD_BUS_DEVICE_ADDR_6RTD_2,MOD_BUS_6RTD_REGISTER_START,MOD_BUS_6RTD_NUM_OF_ELEMENTS,{NULL,48,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}}, //IBD pt100 48-53
        
  {DEV_8TC,MOD_BUS_DEVICE_ADDR_8TC_5,MOD_BUS_8TC_REGISTER_START,MOD_BUS_8TC_NUM_OF_ELEMENTS,{NULL,54,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}},   //IBD K  55-61
  {DEV_8TC,MOD_BUS_DEVICE_ADDR_8TC_6,MOD_BUS_8TC_REGISTER_START,MOD_BUS_8TC_NUM_OF_ELEMENTS,{NULL,62,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}},  //IBD K 62-69
  {DEV_8TC,MOD_BUS_DEVICE_ADDR_8TC_7,MOD_BUS_8TC_REGISTER_START,2,{NULL,70,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n}},  //IBD K 70-71
 
 };


static CfgDB_ID_TC_PeripheralsModBusDescription * PeripheralsModBusDescriptionPtrTC[TEMPERATURE_NUMBER_OF_DEVICES_TC+1];

//0:SV x10, 1:SP x10, 2: MV x10, 4: On/off, 5: PID_P x 10, 6: PID_I x10, 7: PID_D x10 
static CfgDB_ID_TC_PeripheralsModBusDescription PeripheralsModBusDescriptionrTC[TEMPERATURE_NUMBER_OF_DEVICES_TC]=
{
  	//Device#1- zone_Z1
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_1,MOD_BUS_PUMA_REGISTER_START_CH_1,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,0,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{16,0,1101,1102,1103,1255,1004}},  
      
        //Device#1- zone_Z2
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_1,MOD_BUS_PUMA_REGISTER_START_CH_2,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,1,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{17,1,2101,2102,2103,2255,2004}}, 
       
 	//Device#1- zone_Z3
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_1,MOD_BUS_PUMA_REGISTER_START_CH_3,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,2,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{18,2,3101,3102,3103,3255,3004}},  
	//Device#1- zone_Z4
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_1,MOD_BUS_PUMA_REGISTER_START_CH_4,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,3,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{19,3,4101,4102,4103,4255,4004}},  
        
        //Device#2- zone_Z1
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_2,MOD_BUS_PUMA_REGISTER_START_CH_1,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,4,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{16,0,1101,1102,1103,1255,1004}},  
        //Device#2- zone_Z2
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_2,MOD_BUS_PUMA_REGISTER_START_CH_2,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,5,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{17,1,2101,2102,2103,2255,2004}}, 
 	//Device#2- zone_Z3
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_2,MOD_BUS_PUMA_REGISTER_START_CH_3,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,6,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{18,2,3101,3102,3103,3255,3004}},  
	//Device#2- zone_Z4
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_2,MOD_BUS_PUMA_REGISTER_START_CH_4,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,7,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{19,3,4101,4102,4103,4255,4004}},  
        
         //Device#3- zone_Z1
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_3,MOD_BUS_PUMA_REGISTER_START_CH_1,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,8,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{16,0,1101,1102,1103,1255,1004}},  
        //Device#3- zone_Z2
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_3,MOD_BUS_PUMA_REGISTER_START_CH_2,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,9,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{17,1,2101,2102,2103,2255,2004}}, 
 	//Device#3- zone_Z3
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_3,MOD_BUS_PUMA_REGISTER_START_CH_3,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,10,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{18,2,3101,3102,3103,3255,3004}},  
	//Device#3- zone_Z4
	{DEV_PUMA,MOD_BUS_DEVICE_ADDR_PUMA_3,MOD_BUS_PUMA_REGISTER_START_CH_4,MOD_BUS_PUMA_NUM_OF_ELEMENTS,{NULL,11,ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,ID_TEMPERATURE_CFG_INT_ZONE_SP_n,ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n},
	{19,3,4101,4102,4103,4255,4004}},  
      
        
};




static void InitPeripheralsModBusDescriptionr (BOARD_TYPE BoardType)
{
  int Inx;
  switch (BoardType)
  {
    case BOARD_TYPE_PHANTOM: //Phantom
    {
      for (Inx=0;Inx<TEMPERATURE_NUMBER_OF_DEVICES_TC;Inx++)
        PeripheralsModBusDescriptionPtrTC[Inx]=(CfgDB_ID_TC_PeripheralsModBusDescription *)&PeripheralsModBusDescriptionrTC[Inx];
      
       PeripheralsModBusDescriptionPtrTC[Inx]=0; //end of list
       
       for (Inx=0;Inx<TEMPERATURE_NUMBER_OF_DEVICES;Inx++)
        PeripheralsModBusDescriptionPtr[Inx]=(CfgDB_ID_PeripheralsModBusDescription *)&PeripheralsModBusDescriptionr[Inx];
      
       PeripheralsModBusDescriptionPtr[Inx]=0; //end of list
    }
   
  }
}

/***********************************  DB CFG SET/GET HANDLERS*****************************************/

typedef  enum {
  ZONE_SET_ENABLE,
  ZONE_SET_SP,
  ZONE_SET_PID_P,
  ZONE_SET_PID_I,
  ZONE_SET_PID_D,
  ZONE_SET_POWER_LIMIT,
  ZONE_SET_AUTO_TUNE
} ZONE_SET_TYPE;

static STATUS SetZoneTemperature(ZONE_SET_TYPE SetType,int DeviceInx,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
        U16 DeviceId=PeripheralsModBusDescriptionPtrTC[DeviceInx]->DeviceId;
        U16 Register;
        switch (SetType)
        {
          case ZONE_SET_ENABLE:
#ifdef _NEW_IBD_CONTROL_            
            if (DeviceInx>=6) //devices 6-11 are IBDs
            {
              Pwm_Enable(DeviceInx, (U8)Data);
              return OK;
            }
#endif              
            Register=PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.Enable;
            Pwm_Enable(DeviceInx, (U8)Data);
            Data=Data==0?1:0; //fix it for PUMA
            break;
           case ZONE_SET_SP:
            Register=PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.SP;
            break;
          case ZONE_SET_PID_P:
            Register=PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.PID_P;
            break;
          case ZONE_SET_PID_I:
            Register=PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.PID_I;
            break;
          case ZONE_SET_PID_D:
            Register=PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.PID_D;
            break;
          case ZONE_SET_POWER_LIMIT:
            Register=PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.PowerLimit;
            break;
          case ZONE_SET_AUTO_TUNE:
             U16 IdIndex=PeripheralsModBusDescriptionPtrTC[DeviceInx]->DB_id.DB_IdIndex;
             U16 InstanceId=PeripheralsModBusDescriptionPtrTC[DeviceInx]->DB_id.InstanceId;
             
            //U32 data0=0;
           // CfgPutParam(InstanceId,CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,IdIndex),&data0,CfgDB_SET_TO_DB);
            //CfgPutParam(InstanceId,CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,IdIndex),&data0,CfgDB_SET_TO_DB);
            //CfgPutParam(InstanceId,CfgDB_ID_AddIndex(ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,IdIndex),&data0,CfgDB_SET_TO_DB);
              
            Register=PeripheralsModBusDescriptionPtrTC[DeviceInx]->SetPumaRegisterAddress.AutoTune;
          break;
           
        }

	/*send the command to the PUMA*/
	return (rtu_single_reg_write(mb_node_1,DeviceId,Register, (U16)Data, 0, 0,0, NULL));
}


static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_SP_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
        return (SetZoneTemperature(ZONE_SET_SP,Index,Data,SetFlag));
}


static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
        return (SetZoneTemperature(ZONE_SET_PID_P,Index,Data,SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
         return (SetZoneTemperature(ZONE_SET_PID_I,Index,Data,SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
         return (SetZoneTemperature(ZONE_SET_PID_D,Index,Data,SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
    return (SetZoneTemperature(ZONE_SET_POWER_LIMIT,Index,Data,SetFlag));
}


/************************** CMND INT  *************************************/


//static REQUEST_SEQUENCE RequestDeviceSequence=0;

static STATUS handleResponeError(uint8_t func_status)
{
	if (func_status!=RTU_FUNC_ERR_OK)
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

uint32_t static StartSysTickCounter=0;
static void MB_GetDataResponseHandlerTC(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz,uint32_t RequestDeviceSequence )
{

	U16 RxData[10];

	if (handleResponeError(func_status)!=RTU_FUNC_ERR_OK)
	{
		M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_WARNING,"TEMPERATURE_UPDATE_DB_RESP ERROR: func_status=%d",func_status);
		return;
	}

	if (resp_sz!=MOD_BUS_PUMA_NUM_OF_ELEMENTS)
		while (1)
		{
			;
		}

		M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"START ->TEMPERATURE_UPDATE_DB_RESP. total_ModbusRetries=%d",get_total_retries());
              
                for (int i=0;i<resp_sz;i++)
		{
                  
                  U32 InstanceId=PeripheralsModBusDescriptionPtrTC[RequestDeviceSequence]->DB_id.InstanceId;
                  U32 DB_Id=PeripheralsModBusDescriptionPtrTC[RequestDeviceSequence]->DB_id.DB_Id[i];
                  U32 DB_IdIndex=PeripheralsModBusDescriptionPtrTC[RequestDeviceSequence]->DB_id.DB_IdIndex;
                
                  if (DB_Id!=0)
                  {
                    U32 data=*(resp_buf+i);
                                    
                      switch (PeripheralsModBusDescriptionPtrTC[RequestDeviceSequence]->DeviceType)
                      {
                        case DEV_PUMA:
                        {
       				// mark termocoupler not connected as 5000 value
				if (i==0)  // PV value from PUMA
				{
					if ((int)data<0 || (data==4200) || (data==63536))
						data=SENSOR_NOT_CONNECTED_MARK;  //mark as termocoupler not connected
                                        else if (data>=ZONE_ALARM_HIGH_TEMERATURE_TH)
                                        {
                                          //turn on  the safetry alarm
                                             U32 Data=1;
                                            CfgPutParam(0,ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM,&Data,CfgDB_SET_TO_DB);
                                        }
				}
				else
				if (i==2) //MV value
                                {
#ifdef _NEW_IBD_CONTROL_            
                                if (DB_IdIndex>=6) //devices 6-11 are IBDs
                                  continue;
#endif                                   
                                       Pwm_SetDutyCycle(DB_IdIndex, (U8)(data/10)); 
                                }
                                else
                                {
                                  // fix the set value of the on/off from the PUMA from  on=0, off=1 to on=1, off=0
                                  if (i==3) // Pumas on/off value
                                  {
#ifdef _NEW_IBD_CONTROL_            
                                if (DB_IdIndex>=6) //devices 6-11 are IBDs
                                  continue;
#endif                                     
                                          data=data==0?1:0;  // set it as: 0n=1, off=0
                                          data=(data==1) & IsPwm_Enable(DB_IdIndex);
                                  }
                                }
			
                         }
                          break;
                       
                        
                      } //switch
                  
                  
                      char IdStr[60];
                      if (CfgPutParam(InstanceId,CfgDB_ID_AddIndex(DB_Id,DB_IdIndex),&data,CfgDB_SET_TO_RAM)!=OK)
                      {
                              M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_ERROR,"********ERROR CfgPutPara:  TEMPERATURE_UPDATE_DB_RESP:  InstanceId=%d, DB_Id=%s, data=%d ",InstanceId,Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_IdIndex), IdStr),data);

                              //while (1) {};
                      }
                      M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"TEMPERATURE_UPDATE_DB_RESP: InstanceId=%d, DB_Id=%s, data=%d ",InstanceId,Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_IdIndex), IdStr),data);
                  } //if
                  
                  
                } //for
                

}




static void MB_GetDataResponseHandler(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz,uint32_t RequestDeviceSequence )
{


	if (handleResponeError(func_status)!=RTU_FUNC_ERR_OK)
	{
		M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_WARNING,"TEMPERATURE_UPDATE_DB_RESP ERROR: func_status=%d",func_status);
		return;
	}


        M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"START ->TEMPERATURE_UPDATE_DB_RESP. total_ModbusRetries=%d",get_total_retries());
        

        U32 InstanceId=PeripheralsModBusDescriptionPtr[RequestDeviceSequence]->DB_id.InstanceId;
        U32 DB_Id=PeripheralsModBusDescriptionPtr[RequestDeviceSequence]->DB_id.DB_Id;
        U32 DB_StartIdIndex=PeripheralsModBusDescriptionPtr[RequestDeviceSequence]->DB_id.StartIdIndex;
        
        for (int i=0;i<resp_sz;i++)
        {
          
            U32 data=*(resp_buf+i);
                            
              switch (PeripheralsModBusDescriptionPtr[RequestDeviceSequence]->DeviceType)
              {
                   
                  case DEV_8TC:
                  {
                    if ((data==32768) || (data>65000))
                      data=SENSOR_NOT_CONNECTED_MARK;  //mark as termocoupler not connected
                     else if (data>=HTX_ALARM_HIGH_TEMERATURE_TH)
                      {
                        //turn on  the safetry alarm
                           U32 Data=1;
                          CfgPutParam(0,ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM,&Data,CfgDB_SET_TO_DB);
                      }
                  }
                  break;
                  case DEV_6RTD:
                  {
                    if (data==32768)
                      data=SENSOR_NOT_CONNECTED_MARK;  //mark as termocoupler not connected
                    else if (data>=ZONE_ALARM_HIGH_TEMERATURE_TH)
                    {
                      //turn on  the safetry alarm
                         U32 Data=1;
                        CfgPutParam(0,ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM,&Data,CfgDB_SET_TO_DB);
                    }
                  }
                  break;
              } //switch
          
          
              char IdStr[60];
              if (CfgPutParam(InstanceId,CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i),&data,CfgDB_SET_TO_RAM)!=OK)
              {
                      M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_ERROR,"********ERROR CfgPutPara:  TEMPERATURE_UPDATE_DB_RESP:  InstanceId=%d, DB_Id=%s, data=%d ",InstanceId,Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i), IdStr),data);
                     
              }
              M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"TEMPERATURE_UPDATE_DB_RESP: InstanceId=%d, DB_Id=%s, data=%d ",InstanceId,Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i), IdStr),data);
        } //for
 
}


static STATUS Set_ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;

	U16 DeviceId; 
	U16 RegisterId;
	U8  NumberOfElements;
	int  ReqSequance;
      
	M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"*********************************************************************************");
	StartSysTickCounter=xTaskGetTickCountFromISR();// uiTracePortGetTimeStamp();
	M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"START ->TEMPERATURE_UPDATE_DB_REQ");
        
        // Handle TC
        ReqSequance=0;
        while (PeripheralsModBusDescriptionPtrTC[ReqSequance])
	{
		DeviceId=PeripheralsModBusDescriptionPtrTC[ReqSequance]->DeviceId;
		RegisterId=PeripheralsModBusDescriptionPtrTC[ReqSequance]->RegisterId;
		NumberOfElements=PeripheralsModBusDescriptionPtrTC[ReqSequance]->NumberOfElements;
		LL_STATUS Status=rtu_hold_regs_read( mb_node_1, DeviceId, RegisterId , NumberOfElements,0, 0, ReqSequance,MB_GetDataResponseHandlerTC); //data from modbus device

		M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"TEMPERATURE_UPDATE_DB_REQ_TC: ReqSequance=%d, DeviceId=%d, RegisterId=%d, NumberOfElements=%d ",ReqSequance,DeviceId,RegisterId,NumberOfElements);
                ReqSequance+=1;
	}
	M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"END ->TEMPERATURE_UPDATE_DB_REQ_TC. ReqSequance=%d",ReqSequance);
        
        
        // Handle RO
        ReqSequance=0;
        while (PeripheralsModBusDescriptionPtr[ReqSequance])
	{
		DeviceId=PeripheralsModBusDescriptionPtr[ReqSequance]->DeviceId;
		RegisterId=PeripheralsModBusDescriptionPtr[ReqSequance]->RegisterId;
		NumberOfElements=PeripheralsModBusDescriptionPtr[ReqSequance]->NumberOfElements;
		LL_STATUS Status=rtu_hold_regs_read( mb_node_1, DeviceId, RegisterId , NumberOfElements,0, 0, ReqSequance,MB_GetDataResponseHandler); //data from modbus device

		M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"TEMPERATURE_UPDATE_DB_REQ: ReqSequance=%d, DeviceId=%d, RegisterId=%d, NumberOfElements=%d ",ReqSequance,DeviceId,RegisterId,NumberOfElements);
                ReqSequance+=1;
	}
	M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"END ->TEMPERATURE_UPDATE_DB_REQ. ReqSequance=%d",ReqSequance);

	return OK;
}

static STATUS Set_ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
        
        return (SetZoneTemperature(ZONE_SET_ENABLE,Index,Data,SetFlag));
}

static STATUS Set_ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{

        return (SetZoneTemperature(ZONE_SET_AUTO_TUNE,Index,Data,SetFlag));
}

/************************** MONITOR INT STRING*************************************/
//void (*cb_func)( uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val)


static void MB_GetResponse(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz,uint32_t token_val)
{
	static uint16_t RxBuf[20];

	//total of 16bits elements
	resp_buf+=3; //point to the start of the data

	int TotalReceivedElement=(resp_sz-5)>>1;  //TotalReceivedElement=(resp_sz-5)/2 where the 5 are composed of: 3 bytes of headers, and 2 byes of CRC
	for (int i=0;i<TotalReceivedElement;i+=2)
	{
		RxBuf[i]=SWAP(*(uint16_t*)(resp_buf+i));

	}
}

static STATUS Get_ID_TEMPERATURE_MNTR_INT_ZONE_PV_n_handle(int InstanceNum,int Index,int* Data)
{

	//tbd - just for testing
	LL_STATUS Status=rtu_hold_regs_read( mb_node_1, 38, 0 , 1,0, 0, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,MB_GetResponse);  
	*Data=0;
	return OK;
}



static STATUS Set_ID_TEMPERATURE_MNTR_INT_ZONE_MV_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	Data/=10;  //0-1000=> 0-100%
	// allow direct control the PWM if in PID_HOST control type
	//	if (CfgRetInt(NULL,ID_TEMPERATURE_CFG_INT_CONTROL_TYPE)==CNTL_TYPE_PID_HOST)  // TBD need to distiguse it also by the Set flag: internal pid (or device) or exteranl from the host (ignore if get from the host if in internal mode)
	{
		//the the value of the PWM channel (Pwm channels 0-4 are dedicated for the zones' zones)
		Pwm_SetDutyCycle(Index, (U8)Data);
	}
        return OK;

}

static STATUS Set_ID_TEMPERATURE_MNTR_INT_ALARM_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{


	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;
        
        
        
        //0 means ALARM CLEAR (default) and the output should be turned on. 1 means ALARM SET and the output signal should be turned off
	if (Data)
        {
                Safety_out_ctrl(DEACTIVE); //N.O - ALARM IS ON -> turn the output off
                M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"SAFETY ALARM IS ON");
					
        }
	else
        {
                Safety_out_ctrl(ACTIVE); //N.ON - ALARM IS OFF -> turn the output on
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"SAFETY ALARM IS OFF");
        }

	return OK;
}






/************************** VOLATILE STRING*************************************/

static STATUS Get_ID_TEMPERATURE_VOLATILE_STR_DATE_AND_TIME(int InstanceNum,int Index,char* Data)
{
#if _LL_TO_DO_   
	UTIL_GetDateAndTime(Data);
#endif    
	return OK;
}




static STATUS Set_ID_TEMPERATURE_VOLATILE_STR_DATE_AND_TIME(int InstanceNum,int Index,char* Data,CfgDB_SET_FLAG *SetFlag)
{
	//STATUS RetValue;
	int day,month,year;
	int Hour,Min,Sec;
	unsigned int prev_time;
	char EventStr[60];
#ifdef _NTP
	PM_Time rec_time, curr_time;
	int diff;
	unsigned char curr_time_buf[10];
#endif

	if (strlen(Data)<19)
		return ERROR;

	/* format validation check*/
	if ((Data[2]!='-') || (Data[5]!='-') || (Data[10]!=' ') || (Data[13]!=':') || (Data[16]!=':'))
		return ERROR;
	sscanf(Data,"%d-%d-%d %d:%d:%d",&day,&month,&year,&Hour,&Min,&Sec);

	if ((year<2000) || (year>2035)|| (day<1) || (day>31) || (month<1) || (month>12))
		return ERROR;


	if ((Hour<0) || (Hour>23) || (Min<0) || (Min>59) || (Sec<0)|| (Sec>59))
		return ERROR;

	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;

#ifdef _NTP
	rec_time.second =  (unsigned char) Sec;
	rec_time.minute =  (unsigned char) Min;
	rec_time.hour 	=  (unsigned char) Hour;
	rec_time.day 	=  (unsigned char) day;
	rec_time.month 	=  (unsigned char) month;
	rec_time.year 	=  (unsigned short) year;

	UTIL_Time2Hex(curr_time_buf);

	curr_time.second =  (unsigned char) curr_time_buf[6];
	curr_time.minute =  (unsigned char) curr_time_buf[5];
	curr_time.hour 	 =  (unsigned char) curr_time_buf[4];
	curr_time.day 	 =  (unsigned char) curr_time_buf[3];
	curr_time.month  =  (unsigned char) curr_time_buf[2];
	curr_time.year 	 =  (unsigned short) curr_time_buf[0]*0x100+curr_time_buf[1];

	diff = ( PMDB_get_seconds_by_timestamp(rec_time) - PMDB_get_seconds_by_timestamp(curr_time) );

	if ( FirstNTPUpdateFlag == TRUE || (abs(diff)) > 30 )
	{
		FirstNTPUpdateFlag = FALSE;
#endif
		sprintf (EventStr, "Time & Date was set - %s", Data);
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
				M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in ALARM_WaitForEmptyQueue in semTake AlarmQEmptySem",
					0, 0,0,0,0,0);
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

static STATUS Set_ID_TEMPERATURE_CFG_STR_TERM_SUPER_PASS_handle(int InstanceNum,int Index,char* Data,CfgDB_SET_FLAG *SetFlag)
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
CfgDB_ID_IntDescriptor TemperatureDbIntDescriptor[CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,     GetHandle,   SetHandle*/
	

	/*ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n*/
	{"ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n", INFORM_NOTHING,0,5000,UNSIGNED_VALUE |WRITE_RAM_BEFORE_SET_HANDLE|IGNORED_SET_HANDLE_DURING_INIT,  NUMBER_OF_TEMPERATURE_ZONES,0, NULL,      Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n_handle},

	/*ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n*/
	{"ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n", INFORM_NOTHING,0,5000,UNSIGNED_VALUE|WRITE_RAM_BEFORE_SET_HANDLE |IGNORED_SET_HANDLE_DURING_INIT,  NUMBER_OF_TEMPERATURE_ZONES,0, NULL,      Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n_handle},

	/*ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n*/
	{"ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n", INFORM_NOTHING,0,5000,UNSIGNED_VALUE|WRITE_RAM_BEFORE_SET_HANDLE|IGNORED_SET_HANDLE_DURING_INIT ,  NUMBER_OF_TEMPERATURE_ZONES,0, NULL,      Set_ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n_handle},

	/* ID_TEMPERATURE_CFG_INT_ZONE_SP_n*/
	{"ID_TEMPERATURE_CFG_INT_ZONE_SP_n", INFORM_NOTHING,MIN_ZONE_AVAILIABLE_TEMPERATURE,MAX_ZONE_AVAILIABLE_TEMPERATURE,UNSIGNED_VALUE|WRITE_RAM_BEFORE_SET_HANDLE ,  NUMBER_OF_TEMPERATURE_ZONES,900, NULL,      Set_ID_TEMPERATURE_CFG_INT_ZONE_SP_n_handle},

	/*ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n*/
	{"ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n", INFORM_NOTHING,0,1000,UNSIGNED_VALUE|WRITE_RAM_BEFORE_SET_HANDLE ,  NUMBER_OF_TEMPERATURE_ZONES,1000, NULL,      Set_ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n_handle},

};


/* The following structure contains the description data for command integer data type DB of TEMPERATURE*/
CfgDB_ID_IntDescriptor TemperatureDbCmndIntDescriptor[CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/
    
        
        /* ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES*/
	{"ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES", INFORM_NOTHING,FALSE,TRUE,UNSIGNED_VALUE |WRITE_RAM_BEFORE_SET_HANDLE ,  NUMBER_OF_TEMPERATURE_ZONES,FALSE, NULL,      Set_ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES_handle},
          
        /* ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n*/
	{"ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n", INFORM_NOTHING,FALSE,TRUE,UNSIGNED_VALUE |WRITE_RAM_BEFORE_SET_HANDLE ,  NUMBER_OF_TEMPERATURE_ZONES,FALSE, NULL,      Set_ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n_handle},

	/*ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n*/
	{"ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n", INFORM_NOTHING,0,1,UNSIGNED_VALUE |WRITE_RAM_BEFORE_SET_HANDLE,  NUMBER_OF_TEMPERATURE_ZONES,0, NULL,      Set_ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n_handle},
};



/* The following structure contains the description data for monitor integer data type DB of TEMPERATURE*/
CfgDB_ID_IntDescriptor TemperatureDbMntrIntDescriptor[CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

	/* ID_TEMPERATURE_MNTR_INT_ZONE_PV_n */
	{"ID_TEMPERATURE_MNTR_INT_ZONE_PV_n", INFORM_NOTHING,    MIN_ZONE_AVAILIABLE_TEMPERATURE,  MAX_ZONE_AVAILIABLE_TEMPERATURE,UNSIGNED_VALUE,    73,             0,  Get_ID_TEMPERATURE_MNTR_INT_ZONE_PV_n_handle,    NULL},

	/* ID_TEMPERATURE_MNTR_INT_ZONE_MV_n */
	{"ID_TEMPERATURE_MNTR_INT_ZONE_MV_n", INFORM_NOTHING,    0,           1000,UNSIGNED_VALUE,    NUMBER_OF_TEMPERATURE_ZONES,             0,	NULL, Set_ID_TEMPERATURE_MNTR_INT_ZONE_MV_n_handle    }, //0-100%

	/*ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM*/
	{"ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM", INFORM_NOTHING,    FALSE,           TRUE,UNSIGNED_VALUE,    1,             FALSE,	NULL, Set_ID_TEMPERATURE_MNTR_INT_ALARM_handle    },   // global alarm
 
};


/* this starcture contains the default configuration data of TEMPERATURE string type*/
CfgDB_ID_StrDescriptor TemperatureDbStrDescriptor[CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST)]=
{ /* default values */
	/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

	/* ID_TEMPERATURE_CFG_STR_TBD */
	{"ID_TEMPERATURE_CFG_STR_TBD", INFORM_NOTHING,     MAX_LEN_PASSWORD-1,    1,          "Invalid"   ,NULL,     NULL},
};

/* this starcture contains the default configuration data of MUX string type*/
CfgDB_ID_StrDescriptor   TemperatureDbVolatileStrDescriptor[CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST)]=
{ /* default values */
	/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/
	
	/*ID_TEMPERATURE_VOLATILE_STR_GET_APPLIC_DESCRIPTION*/
	{"ID_TEMPERATURE_VOLATILE_STR_TBD", INFORM_NOTHING,     MAX_CFG_STR_SIZE,  1,          ""    ,NULL,     NULL},

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
	if (CfgDB_CreateIntRamDB (&TemperatureDbIntDescriptor[0],&TemperatureDbIntPtr[0],
		CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST))==ERROR)
		return ERROR;

	/* create the configuration Db of type string*/
	if (CfgDB_CreateStrRamDB (&TemperatureDbStrDescriptor[0],
		&TemperatureDbStrPtr[0],CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST))==ERROR)
		return ERROR;

	/* create the configuration Db of type volatile string*/
	if (CfgDB_CreateStrRamDB (&TemperatureDbVolatileStrDescriptor[0],
		&TemperatureDbVolatileStrPtr[0],CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST))==ERROR)
		return ERROR;

	/* create the command Db */
	if (CfgDB_CreateIntRamDB (&TemperatureDbCmndIntDescriptor[0],&TemperatureDbCmndIntPtr[0],
		CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST))==ERROR)
		return ERROR;

	/* create the monitor Db */
	if (CfgDB_CreateIntRamDB (&TemperatureDbMntrIntDescriptor[0],&TemperatureDbMntrIntPtr[0],
		CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST))==ERROR)
		return ERROR;

	return OK;
}


static void RemoveRamDataBase()
{
	/* remove the configuration Db of type integer*/
	CfgDB_DeleteIntRamDB (&TemperatureDbIntPtr[0],CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST));
	/* remove the configuration Db of type string*/
	CfgDB_DeleteStrRamDB (&TemperatureDbStrPtr[0],CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST));
	/* remove the volatile Db of type string*/
	CfgDB_DeleteStrRamDB (&TemperatureDbVolatileStrPtr[0],CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST));
	/* remove the command Db of type integer*/
	CfgDB_DeleteIntRamDB (&TemperatureDbCmndIntPtr[0],CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST));
	/* remove the monitor Db of type integer*/
	CfgDB_DeleteIntRamDB (&TemperatureDbMntrIntPtr[0],CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST));
}

#if WIN32
BOOL TemperatureDbRdy=TRUE;
#else
BOOL TemperatureDbRdy=FALSE;
#endif

#if WIN32
BOOL TemperatureDbInInitState=FALSE;
#else
BOOL TemperatureDbInInitState=TRUE;
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
		if (CreateRamDataBase()==ERROR)
		{
			RemoveRamDataBase();
			M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form CreateRamDataBase. ", 0, 0, 0, 0,0,0);
			return ERROR;
		}
	}

	id.Field.UnitType=CfgDB_UnitTEMPERATURE;
	id.Field.DataType=CfgDB_INT;
	id.Field.AccessType=CfgDB_NON_VOLATILE;
	/* Create and initialize the Temperature int files  */
	if ((RetValue=CfgDB_FindFilesPathAndNames(id,DefaultFileName,ModifiedFileName,0))!=OK) /* find the exact name*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form CfgDB_FindFilesPathAndNames for integer type id=0x%x. ", id.DataAssign, 0, 0, 0,0,0);
		return RetValue;
	}

#if _LL_TO_DO_     

	CfgDB_TakeCfgSem(CARRIER_TEMPERATURE);
	/* create and init files with default data*/
	if ((RetValue=FileHandCreateAndInitCfgIntFileDB(DefaultFileName,ModifiedFileName,ID_TEMPERATURE_CFG_INT_FIRST, ID_TEMPERATURE_CFG_INT_LAST, &TemperatureDbIntDescriptor[0], &TemperatureDbIntPtr[0],SkipDefaultFile))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form FileHandCreateAndInitCfgIntFileDB. RetValue=%d", RetValue, 0, 0, 0,0,0);
	}

	CfgDB_GiveCfgSem(CARRIER_TEMPERATURE);
	id.Field.DataType=CfgDB_STR;

	/* Create and initialize the Temperature int files  */
	if ((RetValue=CfgDB_FindFilesPathAndNames(id,DefaultFileName,ModifiedFileName,0))!=OK) /* find the exact name*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form CfgDB_FindFilesPathAndNames for string type id=0x%x. ", id, 0, 0, 0,0,0);
		return RetValue;	/* create and init files with default data*/
	}

	CfgDB_TakeCfgSem(CARRIER_TEMPERATURE);
	if ((RetValue=FileHandCreateAndInitCfgStrFileDB(DefaultFileName,ModifiedFileName,
		ID_TEMPERATURE_CFG_STR_FIRST, ID_TEMPERATURE_CFG_STR_LAST,
		&TemperatureDbStrDescriptor[0], &TemperatureDbStrPtr[0],SkipDefaultFile))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form FileHandCreateAndInitCfgStrFileDB. RetValue=%d", RetValue, 0, 0, 0,0,0);
	}
	CfgDB_GiveCfgSem(CARRIER_TEMPERATURE);
#endif
	return OK;
}

STATUS TemperatureCfgUpdateDbFromSource( BOOL ConfigMismatchCheck,BOOL LoadConfigFromUnit)
{
	STATUS RetValue;
	/* update the integer DB from the content of the source - Alarm if mismatch*/
	if ((RetValue=CfgDB_UpdateIntDbFromSource(-1,&TemperatureDbIntPtr[0],
		&TemperatureDbIntDescriptor[0],ID_TEMPERATURE_CFG_INT_FIRST, ID_TEMPERATURE_CFG_INT_LAST,ConfigMismatchCheck,LoadConfigFromUnit))!=OK)
	{
		if (RetValue!=CFG_DB_ERROR_ID_CONFIG_MISMATCH)
			M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in TemperatureCfgUpdateDbFromSource: CfgDB_UpdateIntDbFromSource RetValue=0x%x",RetValue,0,0,0,0,0);
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
	TemperatureDbRdy=FALSE;
	TemperatureDbInInitState=TRUE;

         InitPeripheralsModBusDescriptionr (BOARD_TYPE_PHANTOM);   //TBD should be replace according to the board type   

#if _LL_TO_DO_ 
	if ((RetValue=AlarmTEMPERATURE_Init())!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form AlarmTEMPERATURE_Init. ", 0, 0, 0, 0,0,0);
		return RetValue;
	}
#endif

	if ((RetValue=TemperatureCfgBuildDB(FALSE))!=OK)
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in TemperatureCfgBuildDB: RetValue=0x%x",RetValue,0,0,0,0,0);






#if _LL_TO_DO_ 
	/* the other task that need the TEMPERATURE DB in the following inform can now used the TEMPERATURE DB*/
	TemperatureCfgUpdateEepromStruct();

#endif

	if ((RetValue=TemperatureCfg_UpdatePowerUpMntrDB())!=OK) /* update monitor DB from TEMPERATURE HW peripherals*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form TemperatureCfg_UpdatePowerUpMntrDB. RetValue=%d ", RetValue, 0, 0, 0,0,0);
	}



	/* call to all relevant set handle of each entry in the str DB (only those which has one)
	for completed updating */
	if ((RetValue=CfgDB_CallToAllStrSetHandlesAndInformClients(0,&TemperatureDbStrPtr[0],
		&TemperatureDbStrDescriptor[0],ID_TEMPERATURE_CFG_STR_FIRST, ID_TEMPERATURE_CFG_STR_LAST))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form CfgDB_CallToAllStrSetHandlesAndInformClients. RetValue=%d ", RetValue, 0, 0, 0,0,0);
	}

	/* call to all relevant set handle of each entry in the int DB (only those which has one)
	for completed updating */
/*
	if ((RetValue=CfgDB_CallToAllIntSetHandlesAndInformClients(0,&TemperatureDbIntPtr[0],
		&TemperatureDbIntDescriptor[0],
		ID_TEMPERATURE_CFG_INT_FIRST, ID_TEMPERATURE_CFG_INT_LAST))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"TemperatureCfgInit- Error form CfgDB_CallToAllIntSetHandlesAndInformClients. RetValue=%d", RetValue, 0, 0, 0,0,0);
	}
*/
         
#if _LL_TO_DO_         
	/* update the last time configuration is modified - set to current time as default*/
	TemperatureCfgDbUpdateLastConfigChangeTime();
#endif
	TemperatureDbInInitState=FALSE;
	TemperatureDbRdy=TRUE;

	return RetValue;

}

STATUS TemperatureCfgDelete()
{
	if (!TemperatureDbRdy)
		return ERROR;

	TemperatureDbRdy=FALSE;
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
STATUS  TemperatureCfgDB_GetElement (CfgDB_ID Id, CfgDB_ID_data * buff,int DataLen,CfgDB_GET_FLAG GetFlag)
{

	switch (Id.Field.AccessType)
	{
	case CfgDB_NON_VOLATILE: /* get configuration data element*/
		{
			if (Id.Field.DataType==CfgDB_INT)
			{
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_GetIntElement (0,Id, &TemperatureDbIntPtr[0],
					&TemperatureDbIntDescriptor[0],
					&buff->IntData,GetFlag));
			}
			else
			{
				if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_GetStrElement (0,Id, &TemperatureDbStrPtr[0],
					&TemperatureDbStrDescriptor[0],
					buff->StrData,DataLen,GetFlag));
			}
			break;
		}
	case CfgDB_COMMAND:
		{
			/* check for leggal id*/
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be command*/
			/* check for available id */
			if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetIntElement (0,Id, &TemperatureDbCmndIntPtr[0],
				&TemperatureDbCmndIntDescriptor[0],
				&buff->IntData,GetFlag));
		}
	case CfgDB_MONITOR:
		{
			/* check for leggal id*/
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
			/* check for available id */
			if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetIntElement (0,Id, &TemperatureDbMntrIntPtr[0],
				&TemperatureDbMntrIntDescriptor[0],
				&buff->IntData,GetFlag));

		}

	case CfgDB_VOLATILE_STR:
		{
			if (Id.Field.DataType!=CfgDB_STR)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

			if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetStrElement (0,Id, &TemperatureDbVolatileStrPtr[0],
				&TemperatureDbVolatileStrDescriptor[0],
				buff->StrData,DataLen,GetFlag));

		}
	default:
		return CFG_DB_ERROR_ID_ACCESS_TYPE;
	}

}



/*************************************************************************************************
Set element of any type (integer or tring) into the TEMPERATURE database
**************************************************************************************************/
STATUS  TemperatureCfgDB_SetElement (CfgDB_ID Id, CfgDB_ID_data * buff,CfgDB_SET_FLAG SetFlag)
{

	switch (Id.Field.AccessType)
	{
	case CfgDB_NON_VOLATILE: /* get configuration data element*/
		{
			if (Id.Field.DataType==CfgDB_INT)
			{ /* integer data type*/
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST))
					return CFG_DB_ERROR_ID_LIMIT;
				/* call the the generic routine*/
				return (CfgDB_SetIntElement (0,Id, &TemperatureDbIntPtr[0],
					&TemperatureDbIntDescriptor[0],
					buff->IntData,SetFlag));
			}
			else
			{ /* string data type*/
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_SetStrElement (0,Id, &TemperatureDbStrPtr[0],
					&TemperatureDbStrDescriptor[0],
					buff->StrData,strlen(buff->StrData),SetFlag));
			}
		}
	case CfgDB_COMMAND:
		{
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be command*/
			/* check for leggal id*/
			if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetIntElement (0,Id, &TemperatureDbCmndIntPtr[0],
				&TemperatureDbCmndIntDescriptor[0],
				buff->IntData,SetFlag));
		}
	case CfgDB_MONITOR:
		{
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
			/* check for leggal id*/
			if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetIntElement (0,Id, &TemperatureDbMntrIntPtr[0],
				&TemperatureDbMntrIntDescriptor[0],
				buff->IntData,SetFlag));

		}

	case CfgDB_VOLATILE_STR:
		{

			if (Id.Field.DataType!=CfgDB_STR)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

			if (Id.Field.Id>=CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetStrElement (0,Id, &TemperatureDbVolatileStrPtr[0],
				&TemperatureDbVolatileStrDescriptor[0],
				buff->StrData,strlen(buff->StrData),SetFlag));
		}
	default:
		return CFG_DB_ERROR_ID_ACCESS_TYPE;
	}

}




static STATUS TemperatureCfg_UpdatePowerUpMntrDB()
{
	STATUS RetValue=OK;
#if _LL_TO_DO_      
	char DataStr[MAX_CFG_STR_SIZE];



	/* update the running version string and description of the Temperature*/
	TEMPERATURE_EEPROM_GetTemperatureRunningVersion(&BootOption);
#endif   

	return RetValue;
}



STATUS TemperatureCfgShowDb(BOOL IntType)
{
	int id,index;
	CfgDB_INT_DATA* IntBufPtr;
	char * StrBufPtr;

	if (IntType)
	{

		for (id=0;id<CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST);id++)
		{
			IntBufPtr=TemperatureDbIntPtr[id];
			for (index=0;index<TemperatureDbIntDescriptor[id].MaxAllowedIndex;index++)
				M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"id=%d, id_index=%d, data=0x%x",id , index, IntBufPtr[index], 0,0,0);
		}

	}
	else
	{

		for (id=0;id<CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST);id++)
		{
			StrBufPtr=TemperatureDbStrPtr[id];
			for (index=0;index<TemperatureDbStrDescriptor[id].MaxAllowedIndex;index++)
				M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"id=%d, id_index=%d, data=%s",id , index, (char*)StrBufPtr+index*MAX_CFG_STR_SIZE);
		}
	}


	return OK;
}




BOOL TemperatureDbIntIdIsNotDefault(CfgDB_INT_DATA id)
{
	U32 Data;
	U32 DefaultValue = 0;
	CfgDB_ID Id;

	if (CfgGetParam(NULL, id,&Data,sizeof(Data), CfgDB_GET_FROM_DB)==OK)
	{
		Id.DataAssign=id;
		switch (Id.Field.AccessType)
		{
		case CfgDB_NON_VOLATILE: /* get configuration data element*/
			DefaultValue=TemperatureDbIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		case CfgDB_COMMAND:
			DefaultValue=TemperatureDbCmndIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		case CfgDB_MONITOR:
			DefaultValue=TemperatureDbMntrIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		}
		if (Data!=DefaultValue)
			return TRUE; /* different from the default value*/
		else
			return FALSE; /* equal to the default value*/
	}
	else return FALSE;

}






/* update DB elements according to the status in the EEPROM*/
STATUS TemperatureCfgUpdateEepromStruct()
{
	STATUS RetValue=OK;
#if _LL_TO_DO_  
	EEPROM_HW_CONFIG_STRUCT HwConfigStruct;
	EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT ProductionBoardIdStruct;
	U32 data;
	int index;



#if !WIN32
	if ((RetValue=EEPROM_GetHwConfigurationValues(NULL,BOARD_TEMPERATURE,&HwConfigStruct))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in TemperatureCfgUpdateEepromStruct: in EEPROM_GetHwConfigurationValues",0,0,0,0,0,0);
		return RetValue;
	}
	if ((RetValue=EEPROM_GetBoardIdValues(NULL,BOARD_TEMPERATURE,&ProductionBoardIdStruct))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in TemperatureCfgUpdateEepromStruct: in EEPROM_GetBoardIdValues",0,0,0,0,0,0);
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
	if (RetValue==OK)
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE-1,"TemperatureCfgUpdateEepromStruct was update",0,0,0,0,0,0);
	return RetValue;


}





CfgDB_ID_IntDescriptor * TemperatureCfg_DbIntDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements)
{
	switch (AccessType)
	{
	case CfgDB_NON_VOLATILE:
		*NumOfElements=CfgDB_ID_id(ID_TEMPERATURE_CFG_INT_LAST);
		return &TemperatureDbIntDescriptor[0];
	case CfgDB_COMMAND:
		*NumOfElements=CfgDB_ID_id(ID_TEMPERATURE_CMND_INT_LAST);
		return &TemperatureDbCmndIntDescriptor[0];
	case CfgDB_MONITOR:
		*NumOfElements=CfgDB_ID_id(ID_TEMPERATURE_MNTR_INT_LAST);
		return &TemperatureDbMntrIntDescriptor[0];
	}
	return NULL;
}

CfgDB_ID_StrDescriptor * TemperatureCfg_DbStrDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements)
{
	switch (AccessType)
	{
	case CfgDB_NON_VOLATILE:
		*NumOfElements=CfgDB_ID_id(ID_TEMPERATURE_CFG_STR_LAST);
		return &TemperatureDbStrDescriptor[0];
	case CfgDB_VOLATILE_STR:
		*NumOfElements=CfgDB_ID_id(ID_TEMPERATURE_VOLATILE_STR_LAST);
		return &TemperatureDbVolatileStrDescriptor[0];

	}

	return NULL;

}


