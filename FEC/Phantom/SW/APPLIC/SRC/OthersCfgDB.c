/****************************************************************************
*
*           TITLE:   OthersCfgDB
*
*			Service routines for the OTHERS DB
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
#include "OthersCfgDB.h"
#include "PWM.h"
#include "Opto_out.h"

#include "mb_cnfg.h"
#include "..\..\..\Common\MODBUS\modbus_exp.h"
#include "..\..\..\Common\MODBUS\RTU\rtu_master.h"
#include "rtc.h"
#ifdef CPP_FRAMEWORK
#include "logger.h"
#endif

typedef struct {
		U32 InstanceId;
		U32 StartIdIndex;
		U32 DB_Id;  
	} DB_ID;


typedef enum {
        DEV_PUMA,
        DEV_8TC,
        DEV_6RTD,
        DEV_8AI
 }   DEVICE_TYPE;

typedef struct {
        DEVICE_TYPE  DeviceType;
	U16 DeviceId; 
	U16 RegisterId;
	U8  NumberOfElements;
	DB_ID DB_id;
}  CfgDB_ID_PeripheralsModBusDescription;


static CfgDB_ID_PeripheralsModBusDescription * PeripheralsModBusDescriptionPtr[OTHERS_NUMBER_OF_DEVICES+1];


const static CfgDB_ID_PeripheralsModBusDescription PeripheralsModBusDescriptionr[OTHERS_NUMBER_OF_DEVICES]=
{
        //Device#6- 8AII
  {DEV_8AI,MOD_BUS_DEVICE_ADDR_8AI,MOD_BUS_8AI_REGISTER_START,MOD_BUS_8AI_NUM_OF_ELEMENTS,{0,0,ID_OTHERS_MNTR_INT_ANALOG_INP_MA_DIG_n}}
};
                
 
static void InitPeripheralsModBusDescriptionr (BOARD_TYPE BoardType)
{
  int Inx;
  switch (BoardType)
  {
    case BOARD_TYPE_PHANTOM: //Phantom
    {
      for (Inx=0;Inx<OTHERS_NUMBER_OF_DEVICES;Inx++)
        PeripheralsModBusDescriptionPtr[Inx]=(CfgDB_ID_PeripheralsModBusDescription *)&PeripheralsModBusDescriptionr[Inx];
    }
    PeripheralsModBusDescriptionPtr[Inx]=0; //end of list
  }
}

/***********************************  DB CFG SET/GET HANDLERS*****************************************/


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
        
        return (RTU_FUNC_ERR_OK);
}

static int Convert12bitDataInto4_20ma(int DataDig)
{
  int DataAnalog;
   
  static int DebugDigData=0;
#if 0   //test
  DebugDigData+=1;
  if (DebugDigData==4095) DebugDigData=0;
  
  DataDig=DebugDigData;
#endif   
   DataAnalog= (400+((((DataDig*100)/4095)*16)));    //4+Data/4095 x (20-4)
   
   M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"Convert12bitDataInto4_20ma Dig data=%d, Analog data=%d",DataDig,DataAnalog);
   
   return DataAnalog;
}

uint32_t static StartSysTickCounter=0;
static void MB_GetDataResponseHandler(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz,uint32_t RequestDeviceSequence )
{

	//U16 RxData[10];

	if (handleResponeError(func_status)!=RTU_FUNC_ERR_OK)
	{
		M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_WARNING,"OTHERS_UPDATE_DB_RESP ERROR: func_status=%d",func_status);
		return;
	}


		M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"START ->OTHERS_UPDATE_DB_RESP. total_ModbusRetries=%d",get_total_retries());
		
        
                U32 InstanceId=PeripheralsModBusDescriptionPtr[RequestDeviceSequence]->DB_id.InstanceId;
                U32 DB_Id=PeripheralsModBusDescriptionPtr[RequestDeviceSequence]->DB_id.DB_Id;
                U32 DB_StartIdIndex=PeripheralsModBusDescriptionPtr[RequestDeviceSequence]->DB_id.StartIdIndex;
                
                for (int i=0;i<resp_sz;i++)
		{
                  
                    U32 data=*(resp_buf+i);
                                    
                      switch (PeripheralsModBusDescriptionPtr[RequestDeviceSequence]->DeviceType)
                      {
                        case DEV_8AI: //8AII 4-20ma devices
                          data=Convert12bitDataInto4_20ma(data); //convet it into 4-20ma value
                          break;
                      } //switch
                  
                  
                      char IdStr[60];
                      if (CfgPutParam(InstanceId,CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i),&data,CfgDB_SET_TO_RAM)!=OK)
                      {
                              M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_ERROR,"********ERROR CfgPutPara:  OTHERS_UPDATE_DB_RESP:  InstanceId=%d, DB_Id=%s, data=%d ",InstanceId,Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i), IdStr),data);

                              //while (1) {};
                      }
                      M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"OTHERS_UPDATE_DB_RESP: InstanceId=%d, DB_Id=%s, data=%d ",InstanceId,Id2Str(CfgDB_ID_AddIndex(DB_Id,DB_StartIdIndex+i), IdStr),data);
                } //for
 
}

static STATUS Set_ID_OTHERS_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{

	U16 DeviceId; 
	U16 RegisterId;
	U8  NumberOfElements;
        
        int ReqSequance;
	
	M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"*********************************************************************************");
	StartSysTickCounter=xTaskGetTickCountFromISR();// uiTracePortGetTimeStamp();
	M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"START ->OTHERS_UPDATE_DB_REQ");
	ReqSequance=0;
        while (PeripheralsModBusDescriptionPtr[ReqSequance])
	{
		DeviceId=PeripheralsModBusDescriptionPtr[ReqSequance]->DeviceId;
		RegisterId=PeripheralsModBusDescriptionPtr[ReqSequance]->RegisterId;
		NumberOfElements=PeripheralsModBusDescriptionPtr[ReqSequance]->NumberOfElements;
		LL_STATUS Status=rtu_hold_regs_read( mb_node_1, DeviceId, RegisterId , NumberOfElements,0, 0, ReqSequance,MB_GetDataResponseHandler); //data from modbus device

		M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"OTHERS_UPDATE_DB_REQ: ReqSequance=%d, DeviceId=%d, RegisterId=%d, NumberOfElements=%d ",ReqSequance,DeviceId,RegisterId,NumberOfElements);
                ReqSequance=ReqSequance+=1;
	}
	M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_INFO,"END ->OTHERS_UPDATE_DB_REQ. ReqSequance=%d",ReqSequance);

	return OK;
}




static STATUS Set_ID_OTHERS_CMND_INT_DIGITAL_OUT_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
    if (Index>NUMBER_OF_OPTO_OUTS)
      return ERROR;
        
    //TBD avoid direct access to SSR devices if not during ATP*/

#ifdef PHANTOM_BOARD
    dig_out_ctrl(Index, (int)Data==0?DEACTIVE:ACTIVE); //turn it on/off //data is converted in Phantom board
#else 
    dig_out_ctrl(Index, (int)Data==0?DEACTIVE:ACTIVE); //turn it on/off   
#endif
    
   
         
    return OK;
} 

static STATUS Set_ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
    return OK;
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






/************************** VOLATILE STRING*************************************/







/**********************************/


/* The following structure contains the description data for configuration integer data type DB of OTHERS*/
CfgDB_ID_IntDescriptor OthersDbIntDescriptor[CfgDB_ID_id(ID_OTHERS_CFG_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,     GetHandle,   SetHandle*/
	

	/*ID_OTHERS_CFG_INT_TBD*/
	{"ID_OTHERS_CFG_INT_TBD", INFORM_NOTHING,0,5000,UNSIGNED_VALUE ,  1,0, NULL,      NULL},

	
};


/* The following structure contains the description data for command integer data type DB of OTHERS*/
CfgDB_ID_IntDescriptor OthersDbCmndIntDescriptor[CfgDB_ID_id(ID_OTHERS_CMND_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/
    
        
        /* ID_OTHERS_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES*/
	{"ID_OTHERS_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES", INFORM_NOTHING,FALSE,TRUE,UNSIGNED_VALUE ,  1,FALSE, NULL,      Set_ID_OTHERS_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES_handle},
        /*ID_OTHERS_CMND_INT_DIGITAL_OUT_n*/
        {"ID_OTHERS_CMND_INT_DIGITAL_OUT_n", INFORM_NOTHING,FALSE,TRUE,UNSIGNED_VALUE ,  NUM_OF_DIGITAL_OUT,FALSE, NULL,      Set_ID_OTHERS_CMND_INT_DIGITAL_OUT_n_handle},
        /*ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n*/
        {"ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n", INFORM_NOTHING,FALSE,TRUE,UNSIGNED_VALUE ,  NUM_OF_ANALOG_OUT_MA,FALSE, NULL,      Set_ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n_handle},
        /*ID_OTHERS_CMND_INT_ANALOG_OUT_V*/
        {"ID_OTHERS_CMND_INT_ANALOG_OUT_V", INFORM_NOTHING,FALSE,TRUE,UNSIGNED_VALUE ,  1,FALSE, NULL,      NULL},
        
      
};



/* The following structure contains the description data for monitor integer data type DB of OTHERS*/
CfgDB_ID_IntDescriptor OthersDbMntrIntDescriptor[CfgDB_ID_id(ID_OTHERS_MNTR_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

        /* ID_OTHERS_MNTR_INT_DIGITAL_IN_n */
	{"ID_OTHERS_MNTR_INT_DIGITAL_INP_n", INFORM_NOTHING,    0,  1,UNSIGNED_VALUE,    NUM_OF_DIGITAL_INP,             0,  NULL,    NULL},
	/* ID_OTHERS_MNTR_INT_ANALOG_INP_MA_n */
	{"ID_OTHERS_MNTR_INT_ANALOG_INP_MA_n", INFORM_NOTHING,    MIN_AVAILIABLE_MA,  MAX_AVAILIABLE_MA,UNSIGNED_VALUE,    NUMBER_OF_ANALOG_INP_MA,             0,  NULL,    NULL},
        /* ID_OTHERS_MNTR_INT_ANALOG_INP_MA_DIG_n */
	{"ID_OTHERS_MNTR_INT_ANALOG_INP_MA_DIG_n", INFORM_NOTHING,    MIN_AVAILIABLE_MA_DIG,  MAX_AVAILIABLE_MA_DIG,UNSIGNED_VALUE,    NUMBER_OF_ANALOG_INP_MA,             0,  NULL,    NULL},
        /* ID_OTHERS_MNTR_INT_ANALOG_INP_HM_DIG_n */
	{"ID_OTHERS_MNTR_INT_ANALOG_INP_HM_DIG_n", INFORM_NOTHING,    MIN_AVAILIABLE_MA,  MAX_AVAILIABLE_MA,UNSIGNED_VALUE,    NUMBER_OF_ANALOG_INP_HM,             0,  NULL,    NULL},
        /* ID_OTHERS_MNTR_INT_ANALOG_INP_HM_MA_n */
	{"ID_OTHERS_MNTR_INT_ANALOG_INP_HM_MA_n", INFORM_NOTHING,    MIN_AVAILIABLE_MA,  MAX_AVAILIABLE_MA,UNSIGNED_VALUE,    NUMBER_OF_ANALOG_INP_HM,             0,  NULL,    NULL},
        /* ID_OTHERS_MNTR_INT_NOT_USED*/
        {"ID_OTHERS_MNTR_INT_NOT_USED", INFORM_NOTHING,    0,  65535,UNSIGNED_VALUE,    1,             0,  NULL,    NULL},
        

};


/* this starcture contains the default configuration data of OTHERS string type*/
CfgDB_ID_StrDescriptor OthersDbStrDescriptor[CfgDB_ID_id(ID_OTHERS_CFG_STR_LAST)]=
{ /* default values */
	/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

	/* ID_OTHERS_CFG_STR_TBD */
	{"ID_OTHERS_CFG_STR_TBD", INFORM_NOTHING,     MAX_LEN_PASSWORD-1,    1,          "Invalid"   ,NULL,     NULL},
};

/* this starcture contains the default configuration data of MUX string type*/
CfgDB_ID_StrDescriptor   OthersDbVolatileStrDescriptor[CfgDB_ID_id(ID_OTHERS_VOLATILE_STR_LAST)]=
{ /* default values */
	/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/
	
	/*ID_OTHERS_VOLATILE_STR_GET_APPLIC_DESCRIPTION*/
	{"ID_OTHERS_VOLATILE_STR_TBD", INFORM_NOTHING,     MAX_CFG_STR_SIZE,  1,          ""    ,NULL,     NULL},

};


/* configuration integer element buffer pointers*/
static U32 * OthersDbIntPtr[CfgDB_ID_id(ID_OTHERS_CFG_INT_LAST)];
/* configuration string element buffer pointers*/
static char * OthersDbStrPtr[CfgDB_ID_id(ID_OTHERS_CFG_STR_LAST)];
/* configuration volatile string element buffer pointers*/
static char * OthersDbVolatileStrPtr[CfgDB_ID_id(ID_OTHERS_VOLATILE_STR_LAST)];
/* commands integer element buffer pointers*/
static U32 * OthersDbCmndIntPtr[CfgDB_ID_id(ID_OTHERS_CMND_INT_LAST)];
/* monitor integer element buffer pointers*/
static U32 * OthersDbMntrIntPtr[CfgDB_ID_id(ID_OTHERS_MNTR_INT_LAST)];


/*************************************************************************************************
Allocate and initialize the required memory in the RAM for the database elements
*************************************************************************************************/
static STATUS CreateRamDataBase()
{

	/* create the configuration Db of type integer*/
	if (CfgDB_CreateIntRamDB (&OthersDbIntDescriptor[0],&OthersDbIntPtr[0],
		CfgDB_ID_id(ID_OTHERS_CFG_INT_LAST))==ERROR)
		return ERROR;

	/* create the configuration Db of type string*/
	if (CfgDB_CreateStrRamDB (&OthersDbStrDescriptor[0],
		&OthersDbStrPtr[0],CfgDB_ID_id(ID_OTHERS_CFG_STR_LAST))==ERROR)
		return ERROR;

	/* create the configuration Db of type volatile string*/
	if (CfgDB_CreateStrRamDB (&OthersDbVolatileStrDescriptor[0],
		&OthersDbVolatileStrPtr[0],CfgDB_ID_id(ID_OTHERS_VOLATILE_STR_LAST))==ERROR)
		return ERROR;

	/* create the command Db */
	if (CfgDB_CreateIntRamDB (&OthersDbCmndIntDescriptor[0],&OthersDbCmndIntPtr[0],
		CfgDB_ID_id(ID_OTHERS_CMND_INT_LAST))==ERROR)
		return ERROR;

	/* create the monitor Db */
	if (CfgDB_CreateIntRamDB (&OthersDbMntrIntDescriptor[0],&OthersDbMntrIntPtr[0],
		CfgDB_ID_id(ID_OTHERS_MNTR_INT_LAST))==ERROR)
		return ERROR;

	return OK;
}


static void RemoveRamDataBase()
{
	/* remove the configuration Db of type integer*/
	CfgDB_DeleteIntRamDB (&OthersDbIntPtr[0],CfgDB_ID_id(ID_OTHERS_CFG_INT_LAST));
	/* remove the configuration Db of type string*/
	CfgDB_DeleteStrRamDB (&OthersDbStrPtr[0],CfgDB_ID_id(ID_OTHERS_CFG_STR_LAST));
	/* remove the volatile Db of type string*/
	CfgDB_DeleteStrRamDB (&OthersDbVolatileStrPtr[0],CfgDB_ID_id(ID_OTHERS_VOLATILE_STR_LAST));
	/* remove the command Db of type integer*/
	CfgDB_DeleteIntRamDB (&OthersDbCmndIntPtr[0],CfgDB_ID_id(ID_OTHERS_CMND_INT_LAST));
	/* remove the monitor Db of type integer*/
	CfgDB_DeleteIntRamDB (&OthersDbMntrIntPtr[0],CfgDB_ID_id(ID_OTHERS_MNTR_INT_LAST));
}

#if WIN32
BOOL OthersDbRdy=TRUE;
#else
BOOL OthersDbRdy=FALSE;
#endif

#if WIN32
BOOL OthersDbInInitState=FALSE;
#else
BOOL OthersDbInInitState=TRUE;
#endif


STATUS OthersCfgBuildDB(BOOL SkipDefaultFile)
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
			M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form CreateRamDataBase. ", 0, 0, 0, 0,0,0);
			return ERROR;
		}
	}

	id.Field.UnitType=CfgDB_UnitOTHERS;
	id.Field.DataType=CfgDB_INT;
	id.Field.AccessType=CfgDB_NON_VOLATILE;
	/* Create and initialize the Others int files  */
	if ((RetValue=CfgDB_FindFilesPathAndNames(id,DefaultFileName,ModifiedFileName,0))!=OK) /* find the exact name*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form CfgDB_FindFilesPathAndNames for integer type id=0x%x. ", id.DataAssign, 0, 0, 0,0,0);
		return RetValue;
	}

#if _LL_TO_DO_     

	CfgDB_TakeCfgSem(CARRIER_OTHERS);
	/* create and init files with default data*/
	if ((RetValue=FileHandCreateAndInitCfgIntFileDB(DefaultFileName,ModifiedFileName,ID_OTHERS_CFG_INT_FIRST, ID_OTHERS_CFG_INT_LAST, &OthersDbIntDescriptor[0], &OthersDbIntPtr[0],SkipDefaultFile))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form FileHandCreateAndInitCfgIntFileDB. RetValue=%d", RetValue, 0, 0, 0,0,0);
	}

	CfgDB_GiveCfgSem(CARRIER_OTHERS);
	id.Field.DataType=CfgDB_STR;

	/* Create and initialize the Others int files  */
	if ((RetValue=CfgDB_FindFilesPathAndNames(id,DefaultFileName,ModifiedFileName,0))!=OK) /* find the exact name*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form CfgDB_FindFilesPathAndNames for string type id=0x%x. ", id, 0, 0, 0,0,0);
		return RetValue;	/* create and init files with default data*/
	}

	CfgDB_TakeCfgSem(CARRIER_OTHERS);
	if ((RetValue=FileHandCreateAndInitCfgStrFileDB(DefaultFileName,ModifiedFileName,
		ID_OTHERS_CFG_STR_FIRST, ID_OTHERS_CFG_STR_LAST,
		&OthersDbStrDescriptor[0], &OthersDbStrPtr[0],SkipDefaultFile))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form FileHandCreateAndInitCfgStrFileDB. RetValue=%d", RetValue, 0, 0, 0,0,0);
	}
	CfgDB_GiveCfgSem(CARRIER_OTHERS);
#endif
	return OK;
}

STATUS OthersCfgUpdateDbFromSource( BOOL ConfigMismatchCheck,BOOL LoadConfigFromUnit)
{
	STATUS RetValue;
	/* update the integer DB from the content of the source - Alarm if mismatch*/
	if ((RetValue=CfgDB_UpdateIntDbFromSource(-1,&OthersDbIntPtr[0],
		&OthersDbIntDescriptor[0],ID_OTHERS_CFG_INT_FIRST, ID_OTHERS_CFG_INT_LAST,ConfigMismatchCheck,LoadConfigFromUnit))!=OK)
	{
		if (RetValue!=CFG_DB_ERROR_ID_CONFIG_MISMATCH)
			M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in OthersCfgUpdateDbFromSource: CfgDB_UpdateIntDbFromSource RetValue=0x%x",RetValue,0,0,0,0,0);
	}

	return RetValue;
}



/*************************************************************************************************
Full initialization for the OTHERS DB.
**************************************************************************************************/
STATUS OthersCfgInit()
{
	//char DefaultFileName[100];
	//char ModifiedFileName[100];
	//CfgDB_ID id;
	STATUS RetValue;
	//int i;
	//U32 oem_enable;
	OthersDbRdy=FALSE;
	OthersDbInInitState=TRUE;

        InitPeripheralsModBusDescriptionr (BOARD_TYPE_PHANTOM);   //TBD should be replace according to the board type    

#if _LL_TO_DO_ 
	if ((RetValue=AlarmOTHERS_Init())!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form AlarmOTHERS_Init. ", 0, 0, 0, 0,0,0);
		return RetValue;
	}
#endif

	if ((RetValue=OthersCfgBuildDB(FALSE))!=OK)
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in OthersCfgBuildDB: RetValue=0x%x",RetValue,0,0,0,0,0);






#if _LL_TO_DO_ 
	/* the other task that need the OTHERS DB in the following inform can now used the OTHERS DB*/
	OthersCfgUpdateEepromStruct();

#endif

	if ((RetValue=OthersCfg_UpdatePowerUpMntrDB())!=OK) /* update monitor DB from OTHERS HW peripherals*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form OthersCfg_UpdatePowerUpMntrDB. RetValue=%d ", RetValue, 0, 0, 0,0,0);
	}



	/* call to all relevant set handle of each entry in the str DB (only those which has one)
	for completed updating */
	if ((RetValue=CfgDB_CallToAllStrSetHandlesAndInformClients(0,&OthersDbStrPtr[0],
		&OthersDbStrDescriptor[0],ID_OTHERS_CFG_STR_FIRST, ID_OTHERS_CFG_STR_LAST))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form CfgDB_CallToAllStrSetHandlesAndInformClients. RetValue=%d ", RetValue, 0, 0, 0,0,0);
	}

	/* call to all relevant set handle of each entry in the int DB (only those which has one)
	for completed updating */

	if ((RetValue=CfgDB_CallToAllIntSetHandlesAndInformClients(0,&OthersDbIntPtr[0],
		&OthersDbIntDescriptor[0],
		ID_OTHERS_CFG_INT_FIRST, ID_OTHERS_CFG_INT_LAST))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"OthersCfgInit- Error form CfgDB_CallToAllIntSetHandlesAndInformClients. RetValue=%d", RetValue, 0, 0, 0,0,0);
        }
	
         

#if _LL_TO_DO_         
	/* update the last time configuration is modified - set to current time as default*/
	OthersCfgDbUpdateLastConfigChangeTime();
#endif
	OthersDbInInitState=FALSE;
	OthersDbRdy=TRUE;

	return RetValue;

}

STATUS OthersCfgDelete()
{
	if (!OthersDbRdy)
		return ERROR;

	OthersDbRdy=FALSE;
	RemoveRamDataBase();
	return OK;

}

int OthersCfgDbReady()
{
#if WIN32
	return TRUE;
#else
	return OthersDbRdy;
#endif
}


int OthersDbInInit()
{
#if WIN32
	return FALSE;
#else
	return OthersDbInInitState;
#endif

}


/*************************************************************************************************
Get element of any type (integer or tring) from the OTHERS database
**************************************************************************************************/
STATUS  OthersCfgDB_GetElement (CfgDB_ID Id, CfgDB_ID_data * buff,int DataLen,CfgDB_GET_FLAG GetFlag)
{

	switch (Id.Field.AccessType)
	{
	case CfgDB_NON_VOLATILE: /* get configuration data element*/
		{
			if (Id.Field.DataType==CfgDB_INT)
			{
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_CFG_INT_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_GetIntElement (0,Id, &OthersDbIntPtr[0],
					&OthersDbIntDescriptor[0],
					&buff->IntData,GetFlag));
			}
			else
			{
				if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_CFG_STR_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_GetStrElement (0,Id, &OthersDbStrPtr[0],
					&OthersDbStrDescriptor[0],
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
			if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_CMND_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetIntElement (0,Id, &OthersDbCmndIntPtr[0],
				&OthersDbCmndIntDescriptor[0],
				&buff->IntData,GetFlag));
		}
	case CfgDB_MONITOR:
		{
			/* check for leggal id*/
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
			/* check for available id */
			if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_MNTR_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetIntElement (0,Id, &OthersDbMntrIntPtr[0],
				&OthersDbMntrIntDescriptor[0],
				&buff->IntData,GetFlag));

		}

	case CfgDB_VOLATILE_STR:
		{
			if (Id.Field.DataType!=CfgDB_STR)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

			if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_VOLATILE_STR_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetStrElement (0,Id, &OthersDbVolatileStrPtr[0],
				&OthersDbVolatileStrDescriptor[0],
				buff->StrData,DataLen,GetFlag));

		}
	default:
		return CFG_DB_ERROR_ID_ACCESS_TYPE;
	}

}



/*************************************************************************************************
Set element of any type (integer or tring) into the OTHERS database
**************************************************************************************************/
STATUS  OthersCfgDB_SetElement (CfgDB_ID Id, CfgDB_ID_data * buff,CfgDB_SET_FLAG SetFlag)
{

	switch (Id.Field.AccessType)
	{
	case CfgDB_NON_VOLATILE: /* get configuration data element*/
		{
			if (Id.Field.DataType==CfgDB_INT)
			{ /* integer data type*/
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_CFG_INT_LAST))
					return CFG_DB_ERROR_ID_LIMIT;
				/* call the the generic routine*/
				return (CfgDB_SetIntElement (0,Id, &OthersDbIntPtr[0],
					&OthersDbIntDescriptor[0],
					buff->IntData,SetFlag));
			}
			else
			{ /* string data type*/
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_CFG_STR_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_SetStrElement (0,Id, &OthersDbStrPtr[0],
					&OthersDbStrDescriptor[0],
					buff->StrData,strlen(buff->StrData),SetFlag));
			}
		}
	case CfgDB_COMMAND:
		{
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be command*/
			/* check for leggal id*/
			if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_CMND_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetIntElement (0,Id, &OthersDbCmndIntPtr[0],
				&OthersDbCmndIntDescriptor[0],
				buff->IntData,SetFlag));
		}
	case CfgDB_MONITOR:
		{
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
			/* check for leggal id*/
			if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_MNTR_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetIntElement (0,Id, &OthersDbMntrIntPtr[0],
				&OthersDbMntrIntDescriptor[0],
				buff->IntData,SetFlag));

		}

	case CfgDB_VOLATILE_STR:
		{

			if (Id.Field.DataType!=CfgDB_STR)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

			if (Id.Field.Id>=CfgDB_ID_id(ID_OTHERS_VOLATILE_STR_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetStrElement (0,Id, &OthersDbVolatileStrPtr[0],
				&OthersDbVolatileStrDescriptor[0],
				buff->StrData,strlen(buff->StrData),SetFlag));
		}
	default:
		return CFG_DB_ERROR_ID_ACCESS_TYPE;
	}

}




static STATUS OthersCfg_UpdatePowerUpMntrDB()
{
	STATUS RetValue=OK;
#if _LL_TO_DO_      
	char DataStr[MAX_CFG_STR_SIZE];



	/* update the running version string and description of the Others*/
	OTHERS_EEPROM_GetOthersRunningVersion(&BootOption);
#endif   

	return RetValue;
}



STATUS OthersCfgShowDb(BOOL IntType)
{
	int id,index;
	CfgDB_INT_DATA* IntBufPtr;
	char * StrBufPtr;

	if (IntType)
	{

		for (id=0;id<CfgDB_ID_id(ID_OTHERS_CFG_INT_LAST);id++)
		{
			IntBufPtr=OthersDbIntPtr[id];
			for (index=0;index<OthersDbIntDescriptor[id].MaxAllowedIndex;index++)
				M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"id=%d, id_index=%d, data=0x%x",id , index, IntBufPtr[index], 0,0,0);
		}

	}
	else
	{

		for (id=0;id<CfgDB_ID_id(ID_OTHERS_CFG_STR_LAST);id++)
		{
			StrBufPtr=OthersDbStrPtr[id];
			for (index=0;index<OthersDbStrDescriptor[id].MaxAllowedIndex;index++)
				M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"id=%d, id_index=%d, data=%s",id , index, (char*)StrBufPtr+index*MAX_CFG_STR_SIZE);
		}
	}


	return OK;
}




BOOL OthersDbIntIdIsNotDefault(CfgDB_INT_DATA id)
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
			DefaultValue=OthersDbIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		case CfgDB_COMMAND:
			DefaultValue=OthersDbCmndIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		case CfgDB_MONITOR:
			DefaultValue=OthersDbMntrIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
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
STATUS OthersCfgUpdateEepromStruct()
{
	STATUS RetValue=OK;
#if _LL_TO_DO_  
	EEPROM_HW_CONFIG_STRUCT HwConfigStruct;
	EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT ProductionBoardIdStruct;
	U32 data;
	int index;



#if !WIN32
	if ((RetValue=EEPROM_GetHwConfigurationValues(NULL,BOARD_OTHERS,&HwConfigStruct))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in OthersCfgUpdateEepromStruct: in EEPROM_GetHwConfigurationValues",0,0,0,0,0,0);
		return RetValue;
	}
	if ((RetValue=EEPROM_GetBoardIdValues(NULL,BOARD_OTHERS,&ProductionBoardIdStruct))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in OthersCfgUpdateEepromStruct: in EEPROM_GetBoardIdValues",0,0,0,0,0,0);
		return RetValue;
	}

#else
	HwConfigStruct.BoardType=EEPROM_BOARD_OTHERS;
	HwConfigStruct.BoardSubType=0;
	HwConfigStruct.InterfaceType[0]=MUX_BOARD_TYPE_INTERFACE_OPTIC_SINGLE_MODE_SHORT_HAUL;
	HwConfigStruct.NumOfInterface[0]=1;
	HwConfigStruct.CRC=0;
	ProductionBoardIdStruct.NumOfWorkingHours=0;
#endif
	data=HwConfigStruct.BoardType;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_HW_BOARD_TYPE,&data,CfgDB_SET_TO_RAM);

	data=HwConfigStruct.Firmware;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_HW_FIRMWARE,&data,CfgDB_SET_TO_RAM);

	data=HwConfigStruct.Hardware;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_HW_HARDWARE,&data,CfgDB_SET_TO_RAM);

	data=HwConfigStruct.BoardSubType;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_HW_BOARD_SUB_TYPE,&data,CfgDB_SET_TO_RAM);

	for (index=0;index<EEPROM_MAX_NUM_OF_INTERFACES;index++)
	{
		data=HwConfigStruct.InterfaceType[index];
		CfgPutParam(NULL,CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_EEPROM_HW_INTERFACE_TYPE_n, index),&data,CfgDB_SET_TO_RAM);

		data=HwConfigStruct.NumOfInterface[index];
		CfgPutParam(NULL,CfgDB_ID_AddIndex(ID_OTHERS_MNTR_INT_EEPROM_HW_NUM_OF_INTERFACES_n, index),&data,CfgDB_SET_TO_RAM);
	}
	data=HwConfigStruct.CRC;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_HW_CRC,&data,CfgDB_SET_TO_RAM);

	data=ProductionBoardIdStruct.NumOfWorkingHours;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_NUM_OF_WORKING_HOURS,&data,CfgDB_SET_TO_RAM);
	data=ProductionBoardIdStruct.DateCode;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_DATE_CODE,&data,CfgDB_SET_TO_RAM);
	data=ProductionBoardIdStruct.HASS_Num;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_HASS_NUM,&data,CfgDB_SET_TO_RAM);
	data=ProductionBoardIdStruct.Feature;
	CfgPutParam(NULL,ID_OTHERS_MNTR_INT_EEPROM_FEATURE,&data,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_OTHERS_VOLATILE_STR_EEPROM_SERIAL_NUM,ProductionBoardIdStruct.SerialNum,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_OTHERS_VOLATILE_STR_EEPROM_PART_NUM,ProductionBoardIdStruct.PartNum,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_OTHERS_VOLATILE_STR_EEPROM_DESCRIPTION_1,ProductionBoardIdStruct.Comment1,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_OTHERS_VOLATILE_STR_EEPROM_DESCRIPTION_2,ProductionBoardIdStruct.Comment2,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_OTHERS_VOLATILE_STR_EEPROM_DESCRIPTION_3,ProductionBoardIdStruct.Comment3,CfgDB_SET_TO_RAM);

#endif  
	if (RetValue==OK)
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE-1,"OthersCfgUpdateEepromStruct was update",0,0,0,0,0,0);
	return RetValue;


}





CfgDB_ID_IntDescriptor * OthersCfg_DbIntDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements)
{
	switch (AccessType)
	{
	case CfgDB_NON_VOLATILE:
		*NumOfElements=CfgDB_ID_id(ID_OTHERS_CFG_INT_LAST);
		return &OthersDbIntDescriptor[0];
	case CfgDB_COMMAND:
		*NumOfElements=CfgDB_ID_id(ID_OTHERS_CMND_INT_LAST);
		return &OthersDbCmndIntDescriptor[0];
	case CfgDB_MONITOR:
		*NumOfElements=CfgDB_ID_id(ID_OTHERS_MNTR_INT_LAST);
		return &OthersDbMntrIntDescriptor[0];
	}
	return NULL;
}

CfgDB_ID_StrDescriptor * OthersCfg_DbStrDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements)
{
	switch (AccessType)
	{
	case CfgDB_NON_VOLATILE:
		*NumOfElements=CfgDB_ID_id(ID_OTHERS_CFG_STR_LAST);
		return &OthersDbStrDescriptor[0];
	case CfgDB_VOLATILE_STR:
		*NumOfElements=CfgDB_ID_id(ID_OTHERS_VOLATILE_STR_LAST);
		return &OthersDbVolatileStrDescriptor[0];

	}

	return NULL;

}


