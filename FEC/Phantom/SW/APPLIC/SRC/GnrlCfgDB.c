/****************************************************************************
*
*           TITLE:   GnrlCfgDB
*
*			Service routines for the GENRAL DB
*
*           PROJECT: Dryer Gig (S10)
*
*           AUTHORS: Yossi Sarusi
*
*           Creation date: 21/8/12
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
#include "GnrlCfgDB.h"
#include "PWM.h"
#include "Opto_out.h"

#include "mb_cnfg.h"
#include "..\..\..\Common\MODBUS\modbus_exp.h"
#include "..\..\..\Common\MODBUS\RTU\rtu_master.h"
#include "rtc.h"
#ifdef CPP_FRAMEWORK
#include "logger.h"
#endif


#define APPLIC_VERSION 310  //3.10  first virsion of OPC integration

#define WALLS_ALARM_HIGH_TEMERATURE_TH 4400 // on 450 dgrees the safetry alarm will be activated which turn off the power

#define MIN_WALL_AVAILIABLE_TEMPERATURE 0
#define MAX_WALL_AVAILIABLE_TEMPERATURE 5000  //500 degrees

#define DEFAULT_CONSOL_LOG_TH 6

#define GNRL_NUMBER_OF_ELEMENTS 3

int DebugDisplayLevelMsgTh=DEFAULT_CONSOL_LOG_TH;
static STATUS GnrlCfg_UpdatePowerUpMntrDB();



/***********************************  DB CFG SET/GET HANDLERS*****************************************/

static STATUS Set_ID_GNRL_CFG_INT_SET_CONSOLE_LOG_MESSAGE_TH_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{

	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;

	DebugDisplayLevelMsgTh=Data; /* display messages into console with equal or higer level to DebugLevelMsgTh*/

	return OK;

}

/* specific set handle routine for GNRL_INT_ID_REMOTE_DISTANCE ID request */
static STATUS Set_ID_GNRL_CFG_INT_ETHER_IP_ADDRESS_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	char EthernetInternetAddr[100];
	unsigned long link_id;
	static int IpStackWasConfigured=FALSE;


	/* convert the IP address from int to string*/
	sprintf(EthernetInternetAddr,"%d.%d.%d.%d",(Data>>24)&0xff,(Data>>16) &0xff,(Data>>8) &0xff,Data & 0xff);

	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;
#if _LL_TO_DO_ 
	if (!IpStackWasConfigured)
	{
		IP_initIpStack(Data);/* temporary must be called only once (not free the memory)*/
		IpStackWasConfigured=TRUE;
	}


#if !WIN32
	M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE-1,"UTILS_SetBootEthernetInternetAddr EthernetInternetAddr=%s", EthernetInternetAddr, 0, 0, 0,0,0);
	if (UTILS_SetBootEthernetInternetAddr(EthernetInternetAddr)!=OK)
		return CFG_DB_ERROR_ID_HANDLE_RESULT;
	else
#endif

#endif          
		return OK;

}



CNTL_TYPE_PID ControlTypeHandle=CNTL_TYPE_PID_DEVICE;
static STATUS Set_ID_GNRL_CFG_INT_CONTROL_TYPE_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;

	ControlTypeHandle=Data;
	return OK;

}





/************************** CMND INT  *************************************/




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

static STATUS Set_ID_GNRL_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	
        
        return OK;

}

static STATUS Set_ID_GNRL_CMND_INT_EEPROM_DISPLAY_CONTENT_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;
#if _LL_TO_DO_ 
	GNRL_EEPROM_DisplayContent();
#endif
	/* turn off the ID_GNRL_CMND_INT_EEPROM_DISPLAY_CONTENT data*/
	Data=FALSE;
	if (CfgPutParam(NULL,ID_GNRL_CMND_INT_EEPROM_DISPLAY_CONTENT,&Data,CfgDB_SET_TO_RAM) != OK)
		return CFG_DB_ERROR_ID_HANDLE_RESULT;

	*SetFlag &=~CfgDB_SET_TO_RAM; /* to prevent setting the original TRUE value*/

	return OK;
}

static STATUS Set_ID_GNRL_CMND_INT_DEBUG_CTRL_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;
        
        M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_TRACE,"ID_GNRL_CMND_INT_DEBUG_VALUE was set:  CMND=%d, Data=%d",Data,CfgRetGnrlInt(ID_GNRL_CMND_INT_DEBUG_VALUE));
        
        switch (Data)
        {
          case (100):  //disable monitoring of modbus debug
            EnableUpdateModbusDeviceStatusEnabe(FALSE);
            break;
            case (101): //Enable monitoring of modbus debug
            EnableUpdateModbusDeviceStatusEnabe(TRUE);
            break;
            case (102):
            //LOG_PrintSysLog();
            break;
        }
}

static STATUS Set_ID_GNRL_CMND_INT_CLEAR_ALARM_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;

        //clear the alarm
        Data=0;
         return (CfgPutParam(NULL,ID_GNRL_MNTR_INT_CRITICAL_ALARM,&Data,CfgDB_SET_TO_DB));
}

static STATUS Set_ID_GNRL_CMND_INT_COLD_RESET_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{

	char DefaultFileName[50];
	char ModifiedFileName[50];
	CfgDB_ID Tempid;
	int value;

	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;



	if (Data)
	{
		
		/* restart the GNRL board*/
		CfgPutParam(NULL,ID_GNRL_CMND_INT_HW_RESET,&Data,CfgDB_SET_TO_DB);
	}


	return OK;
}

#if 0
static STATUS Set_ID_GNRL_CMND_INT_GNRL_HW_RESET_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	int startType;

	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;

	if (Data)
	{


		td("GNRL");/* activate the watch dog*/


#if 0
		FUNCPTR entry=(FUNCPTR)0xff800100;
		startType=BOOT_CLEAR | BOOT_QUICK_AUTOBOOT;

		SPI_TASK_CloseAccessToBackplane(); /* disable signals to the back plane during the reset*/
		SPI_TASK_CloseAccessFromBackplane(); /* disable signals from the back plane during the reset*/
		printf("Set_ID_GNRL_CMND_INT_GNRL_HW_RESET_handle - goto 0x%x StartType=%d",entry,startType);
		go (entry,startType); /* go to entry point - never to return */
		/*sysToMonitor(BOOT_CLEAR); *//* reset to boot flash*/
#endif
	}

#if 0
	/* turn off the ID_GNRL_CMND_INT_GNRL_HW_RESET data*/
	Data=FALSE;
	if (CfgPutParam(NULL,ID_GNRL_CMND_INT_GNRL_HW_RESET,&Data,CfgDB_SET_TO_RAM) != OK)
		return CFG_DB_ERROR_ID_HANDLE_RESULT;

	*SetFlag &=~CfgDB_SET_TO_RAM; /* to prevent setting the original TRUE value*/
#endif
	return OK;
}
#endif


static STATUS Set_ID_GNRL_CMND_INT_WD_ENABLE(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;

	M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"WD enable=%d",Data , 0, 0, 0,0,0);
#if _LL_TO_DO_            
	WdogChangeClock(Data==TRUE?0:1);
#endif         
	return OK;


}

/* Due the fact that enering to simulation mode disable the Set handler, 
exiting from this mode must be done only via SW reset*/
static STATUS Set_ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
	M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"Change Modbus master baudrate=%d",Data);
#ifdef PHANTOM_BOARD        
        ChangeUsartBoudrate(USART1,(uint32_t) Data) ;
#else          
        ChangeUsartBoudrate(USART3,(uint32_t) Data);
#endif        
	return OK;
}




static STATUS Set_ID_GNRL_CMND_INT_SET_OPERATION_MODE(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{
  switch ((OPERATION_MODE)Data)
  {
    case OPERATION_MODE_NORMAL:
      ActiveSimulationMode(FALSE); //turn off Simulation mode
      break;
    case OPERATION_MODE_SIMULATION:
      M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"Active Simulation mode");
        ActiveSimulationMode(TRUE); //turn on Simulation mode
      break;
    case OPERATION_MODE_ATP:
      break;
  }
   return OK;
}




static STATUS Set_ID_GNRL_CMND_INT_TIME_SS_HSS_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag){

	return( rtc_set_seconds_val(Data));
}

static STATUS Set_ID_GNRL_CMND_INT_TIME_HH_MM_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag){

	return( rtc_set_hours_minutes_vals(Data));
}

static STATUS Set_ID_GNRL_CMND_INT_DATE_MM_DD_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag){

	return( rtc_set_day_month_vals(Data));    
}

static STATUS Set_ID_GNRL_CMND_INT_DATE_YEAR_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag){

	return( rtc_set_year_val(Data));
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


static STATUS Set_ID_GNRL_MNTR_INT_ALARM_handle(int InstanceNum,int Index,CfgDB_INT_DATA Data,CfgDB_SET_FLAG *SetFlag)
{


	if (*SetFlag & CfgDB_SET_TEST_SANITY)
		return OK;
        
        
        
        //0 means ALARM CLEAR (default) and the output should be turned on. 1 means ALARM SET and the output signal should be turned off
	if (Data)
        {
                Safety_out_ctrl( DEACTIVE); //N.O - ALARM IS ON -> turn the output off
                M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"SAFETY ALARM IS ON");
					
        }
	else
        {
                Safety_out_ctrl( ACTIVE); //N.ON - ALARM IS OFF -> turn the output on
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"SAFETY ALARM IS OFF");
        }

	return OK;
}






/************************** VOLATILE STRING*************************************/

static STATUS Get_ID_GNRL_VOLATILE_STR_DATE_AND_TIME(int InstanceNum,int Index,char* Data)
{
#if _LL_TO_DO_   
	UTIL_GetDateAndTime(Data);
#endif    
	return OK;
}




static STATUS Set_ID_GNRL_VOLATILE_STR_DATE_AND_TIME(int InstanceNum,int Index,char* Data,CfgDB_SET_FLAG *SetFlag)
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
		EVENT_Set(NULL, ID_GNRL_ALARM_EVENT_SET_TIME_DATE, EventStr, NULL, ALARM_SET_FLAG_CHECK_ALL);
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
	Gnrl_sync_time();
#endif    

	/* to disable changing clock twice */

	return OK;

}

/************************** CFG STRING*************************************/

static STATUS Set_ID_GNRL_CFG_STR_TERM_SUPER_PASS_handle(int InstanceNum,int Index,char* Data,CfgDB_SET_FLAG *SetFlag)
{


#if _LL_TO_DO_
	if(!GnrlDbInInit())
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


/* The following structure contains the description data for configuration integer data type DB of GNRL*/
CfgDB_ID_IntDescriptor GnrlDbIntDescriptor[CfgDB_ID_id(ID_GNRL_CFG_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,     GetHandle,   SetHandle*/
	/*ID_GNRL_CFG_INT_SET_CONSOLE_LOG_MESSAGE_TH*/
	{"ID_GNRL_CFG_INT_SET_CONSOLE_LOG_MESSAGE_TH", INFORM_NOTHING, 4, M_LOGGER_LEVEL_TRACE+1,UNSIGNED_VALUE, 1,     DEFAULT_CONSOL_LOG_TH,               NULL,   Set_ID_GNRL_CFG_INT_SET_CONSOLE_LOG_MESSAGE_TH_handle},

	/* ID_GNRL_CFG_INT_ETHER_IP_ADDRESS */
	{"ID_GNRL_CFG_INT_ETHER_IP_ADDRESS", INFORM_NOTHING,0,0xffffffff,UNSIGNED_VALUE ,  1,/*"192.168.1.1"*/0xc0a80101,                   NULL,      Set_ID_GNRL_CFG_INT_ETHER_IP_ADDRESS_handle},

	/* ID_GNRL_CFG_INT_ETHER_IP_MASK */
	{"ID_GNRL_CFG_INT_ETHER_IP_MASK", INFORM_NOTHING,0,0xffffffff,UNSIGNED_VALUE ,  1,0xffffff00,                                    NULL,      NULL},

	/* ID_GNRL_CFG_INT_CONTROL_TYPE*/
	{"ID_GNRL_CFG_INT_ZONE_EXTERNAL_CONTROL", INFORM_NOTHING,CNTL_TYPE_PID_INTERNAL,CNTL_TYPE_PID_HOST,UNSIGNED_VALUE ,  1,CNTL_TYPE_PID_DEVICE, NULL,      Set_ID_GNRL_CFG_INT_CONTROL_TYPE_handle},


};


/* The following structure contains the description data for command integer data type DB of GNRL*/
CfgDB_ID_IntDescriptor GnrlDbCmndIntDescriptor[CfgDB_ID_id(ID_GNRL_CMND_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

	/*ID_GNRL_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES*/
	{"ID_GNRL_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES", INFORM_NOTHING ,FALSE,            TRUE,UNSIGNED_VALUE,   1,    FALSE,             NULL,       Set_ID_GNRL_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES_handle},
        
        /* ID_GNRL_CMND_INT_ZONE_ENABLE_TEMPERATURE_WALL_n*/
	/* ID_GNRL_CMND_INT_COLD_RESET */
	{"ID_GNRL_CMND_INT_COLD_RESET", INFORM_NOTHING,    FALSE,          TRUE,UNSIGNED_VALUE,         1,             FALSE,       NULL,    Set_ID_GNRL_CMND_INT_COLD_RESET_handle},

	/* ID_GNRL_CMND_INT_GNRL_SET_DEFAULT_CONFIGURATION */
	{"ID_GNRL_CMND_INT_GNRL_SET_DEFAULT_CONFIGURATION", INFORM_NOTHING,    FALSE,          TRUE,UNSIGNED_VALUE,         1,             FALSE,       NULL,    NULL /*(TBD)Set_ID_GNRL_CMND_INT_SET_DEFAULT_CONFIGURATION*/},

	/* ID_GNRL_CMND_INT_GNRL_HW_RESET */
	{"ID_GNRL_CMND_INT_GNRL_HW_RESET", INFORM_NOTHING,    FALSE,     TRUE,UNSIGNED_VALUE, 1,   FALSE,       NULL,    NULL},
	/*ID_GNRL_CMND_INT_BOOT_OPTION*/

	/*ID_GNRL_CMND_INT_WD_ENABLE*/
	{"ID_GNRL_CMND_INT_WD_ENABLE", INFORM_NOTHING,FALSE,TRUE,UNSIGNED_VALUE,1,TRUE,NULL,Set_ID_GNRL_CMND_INT_WD_ENABLE},

        /*ID_GNRL_CMND_INT_SET_OPERATION_MODE*/
        {"ID_GNRL_CMND_INT_SET_OPERATION_MODE", INFORM_NOTHING,OPERATION_MODE_NORMAL,OPERATION_MODE_ATP,UNSIGNED_VALUE,1,OPERATION_MODE_NORMAL,NULL,Set_ID_GNRL_CMND_INT_SET_OPERATION_MODE},
        
        /*ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD*/
        {"ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD", INFORM_NOTHING,9600,38400,UNSIGNED_VALUE,1,38400,NULL,Set_ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD},

	/*ID_GNRL_CMND_INT_EEPROM_DISPLAY_CONTENT*/
	{"ID_GNRL_CMND_INT_EEPROM_DISPLAY_CONTENT", INFORM_NOTHING ,FALSE,            TRUE,UNSIGNED_VALUE,   1,    FALSE,             NULL,       Set_ID_GNRL_CMND_INT_EEPROM_DISPLAY_CONTENT_handle},

	/* ID_GNRL_CMND_INT_TIME_SS_HSS */ /* Bits 15 - 9: Sec;   Bits 9 - 0: mSec    */
	{ "ID_GNRL_CMND_INT_TIME_SS_HSS", INFORM_NOTHING, 0, 0xffffffff, UNSIGNED_VALUE, 1, 0, NULL, Set_ID_GNRL_CMND_INT_TIME_SS_HSS_handle},

	/* ID_GNRL_CMND_INT_TIME_HH_MM */ /* Bits 15 - 8: Hours; Bits 7 - 0: Minutes */
	{ "ID_GNRL_CMND_INT_TIME_HH_MM",  INFORM_NOTHING, 0, 0xffffffff, UNSIGNED_VALUE, 1, 0, NULL, Set_ID_GNRL_CMND_INT_TIME_HH_MM_handle},

	/* ID_GNRL_CMND_INT_DATE_MM_DD */    /* Bits 15 - 8: Month; Bits 7 - 0: Day     */
	{ "ID_GNRL_CMND_INT_DATE_MM_DD",  INFORM_NOTHING, 0, 0xffffffff, UNSIGNED_VALUE, 1, 0, NULL, Set_ID_GNRL_CMND_INT_DATE_MM_DD_handle},

	/* ID_GNRL_CMND_INT_DATE_YEAR */     /* Bits 15 - 0: Year;                      */
	{ "ID_GNRL_CMND_INT_DATE_MM_DD",  INFORM_NOTHING, 0, 0xffffffff, UNSIGNED_VALUE, 1, 0, NULL, Set_ID_GNRL_CMND_INT_DATE_YEAR_handle},

	/*ID_GNRL_CMND_INT_CLEAR_ALARM*/
	{"ID_GNRL_CMND_INT_CLEAR_ALARM", INFORM_NOTHING ,FALSE,            TRUE,UNSIGNED_VALUE,   1,    FALSE,             NULL,       Set_ID_GNRL_CMND_INT_CLEAR_ALARM_handle},
        /*ID_GNRL_CMND_INT_DEBUG_CTRL*/
        {"ID_GNRL_CMND_INT_DEBUG_CTRL", INFORM_NOTHING ,0,  0xffffffff  ,UNSIGNED_VALUE,   1,    0,             NULL,       Set_ID_GNRL_CMND_INT_DEBUG_CTRL_handle},
         /*ID_GNRL_CMND_INT_DEBUG_VALUE*/
        {"ID_GNRL_CMND_INT_DEBUG_VALUE", INFORM_NOTHING ,0,  0xffffffff  ,UNSIGNED_VALUE,   1,    0,             NULL,       NULL},

};



/* The following structure contains the description data for monitor integer data type DB of GNRL*/
CfgDB_ID_IntDescriptor GnrlDbMntrIntDescriptor[CfgDB_ID_id(ID_GNRL_MNTR_INT_LAST)]=
{ /* default values */
	/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/


	/*ID_GNRL_MNTR_INT_CRITICAL_ALARM*/
	{"ID_GNRL_MNTR_INT_CRITICAL_ALARM", INFORM_NOTHING,    FALSE,           TRUE,UNSIGNED_VALUE,    1,             FALSE,	NULL, Set_ID_GNRL_MNTR_INT_ALARM_handle    },   // global alarm
        /*ID_GNRL_MNTR_INT_APPLIC_VERSION*/
        {"ID_GNRL_MNTR_INT_APPLIC_VERSION", INFORM_NOTHING,    0,           0xffffffff,UNSIGNED_VALUE,    1,             APPLIC_VERSION,	NULL, NULL    },   

};


/* this starcture contains the default configuration data of GNRL string type*/
CfgDB_ID_StrDescriptor GnrlDbStrDescriptor[CfgDB_ID_id(ID_GNRL_CFG_STR_LAST)]=
{ /* default values */
	/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

	/* ID_GNRL_CFG_STR_TERM_SUPER_PASS */
	{"ID_GNRL_CFG_STR_TERM_SUPER_PASS", INFORM_NOTHING,     MAX_LEN_PASSWORD-1,    1,          "Invalid"   ,NULL,     Set_ID_GNRL_CFG_STR_TERM_SUPER_PASS_handle},

	/*ID_GNRL_CFG_STR_ALARM_ENABLE (up to 52 GNRL alarms)*/ /* binary bit field - '1' means enable*/
	{"ID_GNRL_CFG_STR_ALARM_ENABLE", INFORM_NOTHING,     255,                         1,"FFFFFFFFFFFFF"     ,NULL,     NULL},
	/*ID_GNRL_CFG_STR_ALARM_SEVERITY (up to 52 GNRL alarms)*/ /* binary bit field- 4 bits per severity, '111' means igonored*/
	{"ID_GNRL_CFG_STR_ALARM_SEVERITY", INFORM_NOTHING,     255,                         1,"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"      ,NULL,     NULL},
};

/* this starcture contains the default configuration data of MUX string type*/
CfgDB_ID_StrDescriptor   GnrlDbVolatileStrDescriptor[CfgDB_ID_id(ID_GNRL_VOLATILE_STR_LAST)]=
{ /* default values */
	/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/
	
	/*ID_GNRL_VOLATILE_STR_GET_APPLIC_DESCRIPTION*/
	{"ID_GNRL_VOLATILE_STR_GET_APPLIC_DESCRIPTION", INFORM_NOTHING,     MAX_CFG_STR_SIZE,  1,          "Gnrl Applic descrip"    ,NULL,     NULL},

	/* ID_GNRL_VOLATILE_STR_DATE_AND_TIME */
	{"ID_GNRL_VOLATILE_STR_DATE_AND_TIME", INFORM_FORCE_USE_GET_HANDLE, 19,   1,"00-00-0000 00:00:00" ,Get_ID_GNRL_VOLATILE_STR_DATE_AND_TIME, Set_ID_GNRL_VOLATILE_STR_DATE_AND_TIME},


};


/* configuration integer element buffer pointers*/
static U32 * GnrlDbIntPtr[CfgDB_ID_id(ID_GNRL_CFG_INT_LAST)];
/* configuration string element buffer pointers*/
static char * GnrlDbStrPtr[CfgDB_ID_id(ID_GNRL_CFG_STR_LAST)];
/* configuration volatile string element buffer pointers*/
static char * GnrlDbVolatileStrPtr[CfgDB_ID_id(ID_GNRL_VOLATILE_STR_LAST)];
/* commands integer element buffer pointers*/
static U32 * GnrlDbCmndIntPtr[CfgDB_ID_id(ID_GNRL_CMND_INT_LAST)];
/* monitor integer element buffer pointers*/
static U32 * GnrlDbMntrIntPtr[CfgDB_ID_id(ID_GNRL_MNTR_INT_LAST)];


/*************************************************************************************************
Allocate and initialize the required memory in the RAM for the database elements
*************************************************************************************************/
static STATUS CreateRamDataBase()
{

	/* create the configuration Db of type integer*/
	if (CfgDB_CreateIntRamDB (&GnrlDbIntDescriptor[0],&GnrlDbIntPtr[0],
		CfgDB_ID_id(ID_GNRL_CFG_INT_LAST))==ERROR)
		return ERROR;

	/* create the configuration Db of type string*/
	if (CfgDB_CreateStrRamDB (&GnrlDbStrDescriptor[0],
		&GnrlDbStrPtr[0],CfgDB_ID_id(ID_GNRL_CFG_STR_LAST))==ERROR)
		return ERROR;

	/* create the configuration Db of type volatile string*/
	if (CfgDB_CreateStrRamDB (&GnrlDbVolatileStrDescriptor[0],
		&GnrlDbVolatileStrPtr[0],CfgDB_ID_id(ID_GNRL_VOLATILE_STR_LAST))==ERROR)
		return ERROR;

	/* create the command Db */
	if (CfgDB_CreateIntRamDB (&GnrlDbCmndIntDescriptor[0],&GnrlDbCmndIntPtr[0],
		CfgDB_ID_id(ID_GNRL_CMND_INT_LAST))==ERROR)
		return ERROR;

	/* create the monitor Db */
	if (CfgDB_CreateIntRamDB (&GnrlDbMntrIntDescriptor[0],&GnrlDbMntrIntPtr[0],
		CfgDB_ID_id(ID_GNRL_MNTR_INT_LAST))==ERROR)
		return ERROR;

	return OK;
}


static void RemoveRamDataBase()
{
	/* remove the configuration Db of type integer*/
	CfgDB_DeleteIntRamDB (&GnrlDbIntPtr[0],CfgDB_ID_id(ID_GNRL_CFG_INT_LAST));
	/* remove the configuration Db of type string*/
	CfgDB_DeleteStrRamDB (&GnrlDbStrPtr[0],CfgDB_ID_id(ID_GNRL_CFG_STR_LAST));
	/* remove the volatile Db of type string*/
	CfgDB_DeleteStrRamDB (&GnrlDbVolatileStrPtr[0],CfgDB_ID_id(ID_GNRL_VOLATILE_STR_LAST));
	/* remove the command Db of type integer*/
	CfgDB_DeleteIntRamDB (&GnrlDbCmndIntPtr[0],CfgDB_ID_id(ID_GNRL_CMND_INT_LAST));
	/* remove the monitor Db of type integer*/
	CfgDB_DeleteIntRamDB (&GnrlDbMntrIntPtr[0],CfgDB_ID_id(ID_GNRL_MNTR_INT_LAST));
}

#if WIN32
BOOL GnrlDbRdy=TRUE;
#else
BOOL GnrlDbRdy=FALSE;
#endif

#if WIN32
BOOL GnrlDbInInitState=FALSE;
#else
BOOL GnrlDbInInitState=TRUE;
#endif


STATUS GnrlCfgBuildDB(BOOL SkipDefaultFile)
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
			M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form CreateRamDataBase. ", 0, 0, 0, 0,0,0);
			return ERROR;
		}
	}

	id.Field.UnitType=CfgDB_UnitGNRL;
	id.Field.DataType=CfgDB_INT;
	id.Field.AccessType=CfgDB_NON_VOLATILE;
	/* Create and initialize the Gnrl int files  */
	if ((RetValue=CfgDB_FindFilesPathAndNames(id,DefaultFileName,ModifiedFileName,0))!=OK) /* find the exact name*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form CfgDB_FindFilesPathAndNames for integer type id=0x%x. ", id.DataAssign, 0, 0, 0,0,0);
		return RetValue;
	}

#if _LL_TO_DO_     

	CfgDB_TakeCfgSem(CARRIER_GNRL);
	/* create and init files with default data*/
	if ((RetValue=FileHandCreateAndInitCfgIntFileDB(DefaultFileName,ModifiedFileName,ID_GNRL_CFG_INT_FIRST, ID_GNRL_CFG_INT_LAST, &GnrlDbIntDescriptor[0], &GnrlDbIntPtr[0],SkipDefaultFile))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form FileHandCreateAndInitCfgIntFileDB. RetValue=%d", RetValue, 0, 0, 0,0,0);
	}

	CfgDB_GiveCfgSem(CARRIER_GNRL);
	id.Field.DataType=CfgDB_STR;

	/* Create and initialize the Gnrl int files  */
	if ((RetValue=CfgDB_FindFilesPathAndNames(id,DefaultFileName,ModifiedFileName,0))!=OK) /* find the exact name*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form CfgDB_FindFilesPathAndNames for string type id=0x%x. ", id, 0, 0, 0,0,0);
		return RetValue;	/* create and init files with default data*/
	}

	CfgDB_TakeCfgSem(CARRIER_GNRL);
	if ((RetValue=FileHandCreateAndInitCfgStrFileDB(DefaultFileName,ModifiedFileName,
		ID_GNRL_CFG_STR_FIRST, ID_GNRL_CFG_STR_LAST,
		&GnrlDbStrDescriptor[0], &GnrlDbStrPtr[0],SkipDefaultFile))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form FileHandCreateAndInitCfgStrFileDB. RetValue=%d", RetValue, 0, 0, 0,0,0);
	}
	CfgDB_GiveCfgSem(CARRIER_GNRL);
#endif
	return OK;
}

STATUS GnrlCfgUpdateDbFromSource( BOOL ConfigMismatchCheck,BOOL LoadConfigFromUnit)
{
	STATUS RetValue;
	/* update the integer DB from the content of the source - Alarm if mismatch*/
	if ((RetValue=CfgDB_UpdateIntDbFromSource(-1,&GnrlDbIntPtr[0],
		&GnrlDbIntDescriptor[0],ID_GNRL_CFG_INT_FIRST, ID_GNRL_CFG_INT_LAST,ConfigMismatchCheck,LoadConfigFromUnit))!=OK)
	{
		if (RetValue!=CFG_DB_ERROR_ID_CONFIG_MISMATCH)
			M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in GnrlCfgUpdateDbFromSource: CfgDB_UpdateIntDbFromSource RetValue=0x%x",RetValue,0,0,0,0,0);
	}

	return RetValue;
}



/*************************************************************************************************
Full initialization for the GNRL DB.
**************************************************************************************************/
STATUS GnrlCfgInit()
{
	//char DefaultFileName[100];
	//char ModifiedFileName[100];
	//CfgDB_ID id;
	STATUS RetValue;
	//int i;
	//U32 oem_enable;
	GnrlDbRdy=FALSE;
	GnrlDbInInitState=TRUE;


#if _LL_TO_DO_ 
	if ((RetValue=AlarmGNRL_Init())!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form AlarmGNRL_Init. ", 0, 0, 0, 0,0,0);
		return RetValue;
	}
#endif

	if ((RetValue=GnrlCfgBuildDB(FALSE))!=OK)
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in GnrlCfgBuildDB: RetValue=0x%x",RetValue,0,0,0,0,0);






#if _LL_TO_DO_ 
	/* the other task that need the GNRL DB in the following inform can now used the GNRL DB*/
	GnrlCfgUpdateEepromStruct();

#endif

	if ((RetValue=GnrlCfg_UpdatePowerUpMntrDB())!=OK) /* update monitor DB from GNRL HW peripherals*/
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form GnrlCfg_UpdatePowerUpMntrDB. RetValue=%d ", RetValue, 0, 0, 0,0,0);
	}



	/* call to all relevant set handle of each entry in the str DB (only those which has one)
	for completed updating */
	if ((RetValue=CfgDB_CallToAllStrSetHandlesAndInformClients(0,&GnrlDbStrPtr[0],
		&GnrlDbStrDescriptor[0],ID_GNRL_CFG_STR_FIRST, ID_GNRL_CFG_STR_LAST))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form CfgDB_CallToAllStrSetHandlesAndInformClients. RetValue=%d ", RetValue, 0, 0, 0,0,0);
	}

	/* call to all relevant set handle of each entry in the int DB (only those which has one)
	for completed updating */

	if ((RetValue=CfgDB_CallToAllIntSetHandlesAndInformClients(0,&GnrlDbIntPtr[0],
		&GnrlDbIntDescriptor[0],
		ID_GNRL_CFG_INT_FIRST, ID_GNRL_CFG_INT_LAST))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"GnrlCfgInit- Error form CfgDB_CallToAllIntSetHandlesAndInformClients. RetValue=%d", RetValue, 0, 0, 0,0,0);
	}

#if _LL_TO_DO_         
	/* update the last time configuration is modified - set to current time as default*/
	GnrlCfgDbUpdateLastConfigChangeTime();
#endif
	GnrlDbInInitState=FALSE;
	GnrlDbRdy=TRUE;

	return RetValue;

}

STATUS GnrlCfgDelete()
{
	if (!GnrlDbRdy)
		return ERROR;

	GnrlDbRdy=FALSE;
	RemoveRamDataBase();
	return OK;

}

int GnrlCfgDbReady()
{
#if WIN32
	return TRUE;
#else
	return GnrlDbRdy;
#endif
}


int GnrlDbInInit()
{
#if WIN32
	return FALSE;
#else
	return GnrlDbInInitState;
#endif

}


/*************************************************************************************************
Get element of any type (integer or tring) from the GNRL database
**************************************************************************************************/
STATUS  GnrlCfgDB_GetElement (CfgDB_ID Id, CfgDB_ID_data * buff,int DataLen,CfgDB_GET_FLAG GetFlag)
{

	switch (Id.Field.AccessType)
	{
	case CfgDB_NON_VOLATILE: /* get configuration data element*/
		{
			if (Id.Field.DataType==CfgDB_INT)
			{
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_CFG_INT_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_GetIntElement (0,Id, &GnrlDbIntPtr[0],
					&GnrlDbIntDescriptor[0],
					&buff->IntData,GetFlag));
			}
			else
			{
				if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_CFG_STR_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_GetStrElement (0,Id, &GnrlDbStrPtr[0],
					&GnrlDbStrDescriptor[0],
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
			if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_CMND_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetIntElement (0,Id, &GnrlDbCmndIntPtr[0],
				&GnrlDbCmndIntDescriptor[0],
				&buff->IntData,GetFlag));
		}
	case CfgDB_MONITOR:
		{
			/* check for leggal id*/
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
			/* check for available id */
			if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_MNTR_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetIntElement (0,Id, &GnrlDbMntrIntPtr[0],
				&GnrlDbMntrIntDescriptor[0],
				&buff->IntData,GetFlag));

		}

	case CfgDB_VOLATILE_STR:
		{
			if (Id.Field.DataType!=CfgDB_STR)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

			if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_VOLATILE_STR_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetStrElement (0,Id, &GnrlDbVolatileStrPtr[0],
				&GnrlDbVolatileStrDescriptor[0],
				buff->StrData,DataLen,GetFlag));

		}
	default:
		return CFG_DB_ERROR_ID_ACCESS_TYPE;
	}

}



/*************************************************************************************************
Set element of any type (integer or tring) into the GNRL database
**************************************************************************************************/
STATUS  GnrlCfgDB_SetElement (CfgDB_ID Id, CfgDB_ID_data * buff,CfgDB_SET_FLAG SetFlag)
{

	switch (Id.Field.AccessType)
	{
	case CfgDB_NON_VOLATILE: /* get configuration data element*/
		{
			if (Id.Field.DataType==CfgDB_INT)
			{ /* integer data type*/
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_CFG_INT_LAST))
					return CFG_DB_ERROR_ID_LIMIT;
				/* call the the generic routine*/
				return (CfgDB_SetIntElement (0,Id, &GnrlDbIntPtr[0],
					&GnrlDbIntDescriptor[0],
					buff->IntData,SetFlag));
			}
			else
			{ /* string data type*/
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_CFG_STR_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_SetStrElement (0,Id, &GnrlDbStrPtr[0],
					&GnrlDbStrDescriptor[0],
					buff->StrData,strlen(buff->StrData),SetFlag));
			}
		}
	case CfgDB_COMMAND:
		{
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be command*/
			/* check for leggal id*/
			if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_CMND_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetIntElement (0,Id, &GnrlDbCmndIntPtr[0],
				&GnrlDbCmndIntDescriptor[0],
				buff->IntData,SetFlag));
		}
	case CfgDB_MONITOR:
		{
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
			/* check for leggal id*/
			if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_MNTR_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetIntElement (0,Id, &GnrlDbMntrIntPtr[0],
				&GnrlDbMntrIntDescriptor[0],
				buff->IntData,SetFlag));

		}

	case CfgDB_VOLATILE_STR:
		{

			if (Id.Field.DataType!=CfgDB_STR)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

			if (Id.Field.Id>=CfgDB_ID_id(ID_GNRL_VOLATILE_STR_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetStrElement (0,Id, &GnrlDbVolatileStrPtr[0],
				&GnrlDbVolatileStrDescriptor[0],
				buff->StrData,strlen(buff->StrData),SetFlag));
		}
	default:
		return CFG_DB_ERROR_ID_ACCESS_TYPE;
	}

}




static STATUS GnrlCfg_UpdatePowerUpMntrDB()
{
	STATUS RetValue=OK;
#if _LL_TO_DO_      
	char DataStr[MAX_CFG_STR_SIZE];



	/* update the running version string and description of the Gnrl*/
	GNRL_EEPROM_GetGnrlRunningVersion(&BootOption);
#endif   

	return RetValue;
}



STATUS GnrlCfgShowDb(BOOL IntType)
{
	int id,index;
	CfgDB_INT_DATA* IntBufPtr;
	char * StrBufPtr;

	if (IntType)
	{

		for (id=0;id<CfgDB_ID_id(ID_GNRL_CFG_INT_LAST);id++)
		{
			IntBufPtr=GnrlDbIntPtr[id];
			for (index=0;index<GnrlDbIntDescriptor[id].MaxAllowedIndex;index++)
				M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"id=%d, id_index=%d, data=0x%x",id , index, IntBufPtr[index], 0,0,0);
		}

	}
	else
	{

		for (id=0;id<CfgDB_ID_id(ID_GNRL_CFG_STR_LAST);id++)
		{
			StrBufPtr=GnrlDbStrPtr[id];
			for (index=0;index<GnrlDbStrDescriptor[id].MaxAllowedIndex;index++)
				M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE,"id=%d, id_index=%d, data=%s",id , index, (char*)StrBufPtr+index*MAX_CFG_STR_SIZE);
		}
	}


	return OK;
}




BOOL GnrlDbIntIdIsNotDefault(CfgDB_INT_DATA id)
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
			DefaultValue=GnrlDbIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		case CfgDB_COMMAND:
			DefaultValue=GnrlDbCmndIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		case CfgDB_MONITOR:
			DefaultValue=GnrlDbMntrIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
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
STATUS GnrlCfgUpdateEepromStruct()
{
	STATUS RetValue=OK;
#if _LL_TO_DO_  
	EEPROM_HW_CONFIG_STRUCT HwConfigStruct;
	EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT ProductionBoardIdStruct;
	U32 data;
	int index;



#if !WIN32
	if ((RetValue=EEPROM_GetHwConfigurationValues(NULL,BOARD_GNRL,&HwConfigStruct))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in GnrlCfgUpdateEepromStruct: in EEPROM_GetHwConfigurationValues",0,0,0,0,0,0);
		return RetValue;
	}
	if ((RetValue=EEPROM_GetBoardIdValues(NULL,BOARD_GNRL,&ProductionBoardIdStruct))!=OK)
	{
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_ERROR,"ERROR in GnrlCfgUpdateEepromStruct: in EEPROM_GetBoardIdValues",0,0,0,0,0,0);
		return RetValue;
	}

#else
	HwConfigStruct.BoardType=EEPROM_BOARD_GNRL;
	HwConfigStruct.BoardSubType=0;
	HwConfigStruct.InterfaceType[0]=MUX_BOARD_TYPE_INTERFACE_OPTIC_SINGLE_MODE_SHORT_HAUL;
	HwConfigStruct.NumOfInterface[0]=1;
	HwConfigStruct.CRC=0;
	ProductionBoardIdStruct.NumOfWorkingHours=0;
#endif
	data=HwConfigStruct.BoardType;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_HW_BOARD_TYPE,&data,CfgDB_SET_TO_RAM);

	data=HwConfigStruct.Firmware;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_HW_FIRMWARE,&data,CfgDB_SET_TO_RAM);

	data=HwConfigStruct.Hardware;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_HW_HARDWARE,&data,CfgDB_SET_TO_RAM);

	data=HwConfigStruct.BoardSubType;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_HW_BOARD_SUB_TYPE,&data,CfgDB_SET_TO_RAM);

	for (index=0;index<EEPROM_MAX_NUM_OF_INTERFACES;index++)
	{
		data=HwConfigStruct.InterfaceType[index];
		CfgPutParam(NULL,CfgDB_ID_AddIndex(ID_GNRL_MNTR_INT_EEPROM_HW_INTERFACE_TYPE_n, index),&data,CfgDB_SET_TO_RAM);

		data=HwConfigStruct.NumOfInterface[index];
		CfgPutParam(NULL,CfgDB_ID_AddIndex(ID_GNRL_MNTR_INT_EEPROM_HW_NUM_OF_INTERFACES_n, index),&data,CfgDB_SET_TO_RAM);
	}
	data=HwConfigStruct.CRC;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_HW_CRC,&data,CfgDB_SET_TO_RAM);

	data=ProductionBoardIdStruct.NumOfWorkingHours;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_NUM_OF_WORKING_HOURS,&data,CfgDB_SET_TO_RAM);
	data=ProductionBoardIdStruct.DateCode;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_DATE_CODE,&data,CfgDB_SET_TO_RAM);
	data=ProductionBoardIdStruct.HASS_Num;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_HASS_NUM,&data,CfgDB_SET_TO_RAM);
	data=ProductionBoardIdStruct.Feature;
	CfgPutParam(NULL,ID_GNRL_MNTR_INT_EEPROM_FEATURE,&data,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_GNRL_VOLATILE_STR_EEPROM_SERIAL_NUM,ProductionBoardIdStruct.SerialNum,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_GNRL_VOLATILE_STR_EEPROM_PART_NUM,ProductionBoardIdStruct.PartNum,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_GNRL_VOLATILE_STR_EEPROM_DESCRIPTION_1,ProductionBoardIdStruct.Comment1,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_GNRL_VOLATILE_STR_EEPROM_DESCRIPTION_2,ProductionBoardIdStruct.Comment2,CfgDB_SET_TO_RAM);
	CfgPutParam(NULL,ID_GNRL_VOLATILE_STR_EEPROM_DESCRIPTION_3,ProductionBoardIdStruct.Comment3,CfgDB_SET_TO_RAM);

#endif  
	if (RetValue==OK)
		M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE-1,"GnrlCfgUpdateEepromStruct was update",0,0,0,0,0,0);
	return RetValue;


}





CfgDB_ID_IntDescriptor * GnrlCfg_DbIntDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements)
{
	switch (AccessType)
	{
	case CfgDB_NON_VOLATILE:
		*NumOfElements=CfgDB_ID_id(ID_GNRL_CFG_INT_LAST);
		return &GnrlDbIntDescriptor[0];
	case CfgDB_COMMAND:
		*NumOfElements=CfgDB_ID_id(ID_GNRL_CMND_INT_LAST);
		return &GnrlDbCmndIntDescriptor[0];
	case CfgDB_MONITOR:
		*NumOfElements=CfgDB_ID_id(ID_GNRL_MNTR_INT_LAST);
		return &GnrlDbMntrIntDescriptor[0];
	}
	return NULL;
}

CfgDB_ID_StrDescriptor * GnrlCfg_DbStrDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements)
{
	switch (AccessType)
	{
	case CfgDB_NON_VOLATILE:
		*NumOfElements=CfgDB_ID_id(ID_GNRL_CFG_STR_LAST);
		return &GnrlDbStrDescriptor[0];
	case CfgDB_VOLATILE_STR:
		*NumOfElements=CfgDB_ID_id(ID_GNRL_VOLATILE_STR_LAST);
		return &GnrlDbVolatileStrDescriptor[0];

	}

	return NULL;

}


