/****************************************************************************
*
*           TITLE:   BtcCfgDB
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

#ifdef __cplusplus
 extern "C" {
#endif

 #include <string.h>

#include "free_rtos_exp.h" 
#include "os_cnfg.h"


//#include "pq_defs.h"
#include <stdlib.h>
#include <stdio.h>
//#include "syslib.h"
#include "ll_typedef.h"
#include "Fec.h"


#include "CfgDB.h"
#include "CfgDBUtil.h"
#include "BtcCfgDB.h"
#include <logger.h>



/***********************************  DB CFG SET/GET HANDLERS*****************************************/




/************************** CMND INT  *************************************/





/************************** MONITOR INT STRING*************************************/



/************************** VOLATILE STRING*************************************/



/************************** CFG STRING*************************************/








/**********************************/



/* The following structure contains the description data for configuration integer data type DB of GNRL*/
CfgDB_ID_IntDescriptor BtcDbIntDescriptor[CfgDB_ID_id(ID_BTC_CFG_INT_LAST)]=
{ /* default values */
        /* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,     GetHandle,   SetHandle*/
        /*ID_BTC_CFG_INT_BLANKET_SP*/
        {"ID_BTC_CFG_INT_BLANKET_SP", INFORM_NOTHING, 0, 2000,UNSIGNED_VALUE, 1,     700,               NULL,   NULL},
        /*ID_BTC_CFG_INT_BLANKET_PID_P*/
        {"ID_BTC_CFG_INT_BLANKET_PID_P", INFORM_NOTHING, 0, 1000,UNSIGNED_VALUE, 1,     50,               NULL,   NULL},
        /*ID_BTC_CFG_INT_BLANKET_PID_I*/
        {"ID_BTC_CFG_INT_BLANKET_PID_I", INFORM_NOTHING, 0, 1000,UNSIGNED_VALUE, 1,     0,               NULL,   NULL},
        /*ID_BTC_CFG_INT_BLANKET_PID_D*/
        {"ID_BTC_CFG_INT_BLANKET_PID_D", INFORM_NOTHING, 0, 1000,UNSIGNED_VALUE, 1,     0,               NULL,   NULL},

        /*ID_BTC_CFG_INT_FILL_PUMP_HIGH_POWER_LIMIT*/
        {"ID_BTC_CFG_INT_FILL_PUMP_HIGH_POWER_LIMIT", INFORM_NOTHING, 0, 1000,UNSIGNED_VALUE, 1,     1000,               NULL,   NULL},
        /*ID_BTC_CFG_INT_FILL_PUMP_LOW_POWER_LIMIT*/
        {"ID_BTC_CFG_INT_FILL_PUMP_LOW_POWER_LIMIT", INFORM_NOTHING, 0, 1000,UNSIGNED_VALUE, 1,     0,               NULL,   NULL},
        /* ID_BTC_CFG_INT_DRAIN_PUMP_POWER_LIMIT,*/
        {"ID_BTC_CFG_INT_DRAIN_PUMP_POWER_LIMIT", INFORM_NOTHING, 0, 1000,UNSIGNED_VALUE, 1,     1000,               NULL,   NULL},
        /*ID_BTC_CFG_INT_DRAIN_PUMP_DELAY*/
        {"ID_BTC_CFG_INT_WATER_TANK_FILL_TIME_OUT", INFORM_NOTHING, 0, 600,UNSIGNED_VALUE, 1,     120,               NULL,   NULL},
        /*ID_BTC_CFG_INT_AIR_PRESSURE_TIME_OUT*/
        {"ID_BTC_CFG_INT_AIR_PRESSURE_TIME_OUT", INFORM_NOTHING, 0, 60,UNSIGNED_VALUE, 1,     3,               NULL,   NULL},
        /*ID_BTC_CFG_INT_ENGAGE_TIME_OUT*/
        {"ID_BTC_CFG_INT_ENGAGE_TIME_OUT", INFORM_NOTHING, 0, 60,UNSIGNED_VALUE, 1,     10,               NULL,   NULL},
        /*ID_BTC_CFG_INT_WATER_TANK_HIGH_TEMPERATURE_LIMIT*/
        {"ID_BTC_CFG_INT_WATER_TANK_HIGH_TEMPERATURE_LIMIT", INFORM_NOTHING, 0, 100,UNSIGNED_VALUE, 1,     50,               NULL,   NULL},
        /*ID_BTC_CFG_INT_WATER_LINE_HIGH_TEMPERATURE_LIMIT*/
        {"ID_BTC_CFG_INT_WATER_LINE_HIGH_TEMPERATURE_LIMIT", INFORM_NOTHING, 0, 100,UNSIGNED_VALUE, 1,     50,               NULL,   NULL},
        /*ID_BTC_CFG_INT_WATER_CONTAMINATION_HIGH_LIMIT*/
        {"ID_BTC_CFG_INT_WATER_CONTAMINATION_HIGH_LIMIT", INFORM_NOTHING, 0, 100,UNSIGNED_VALUE, 1,     100,               NULL,   NULL},

};


/* The following structure contains the description data for command integer data type DB of GNRL*/
CfgDB_ID_IntDescriptor BtcDbCmndIntDescriptor[CfgDB_ID_id(ID_BTC_CMND_INT_LAST)] =
{ /* default values */
/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

    /*ID_BTC_CMND_INT_COMMAND*/
    { "ID_BTC_CMND_INT_STATE", INFORM_NOTHING, BTC_COMMANDS_NONE, BTC_COMMANDS_SERVICE_DISABLE_DRAIN_PUMP, UNSIGNED_VALUE, 1, BTC_COMMANDS_DISABLE_COOLING, NULL, NULL },
    /*ID_BTC_CMND_INT_FILL_PUMP_ENABLE*/
    { "ID_BTC_CMND_INT_FILL_PUMP_ENABLE", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_CMND_INT_DRAIN_PUMP_ENABLE*/
    { "ID_BTC_CMND_INT_DRAIN_PUMP_ENABLE", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_CMND_INT_ENGAGE_ENABLE*/
    { "ID_BTC_CMND_INT_ENGAGE_ENABLE", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_CMND_AIR_PRESSURE_KIFE_CMND_ENABLE*/
    { "ID_BTC_CMND_AIR_PRESSURE_KIFE_CMND_ENABLE", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_CMND_TANK_DRAIN_WATER_ENABLE*/
    { "ID_BTC_CMND_TANK_DRAIN_WATER_ENABLE", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
};

/* The following structure contains the description data for monitor integer data type DB of GNRL*/
CfgDB_ID_IntDescriptor BtcDbMntrIntDescriptor[CfgDB_ID_id(ID_BTC_MNTR_INT_LAST)] =
{ /* default values */
/* InformClients,MinLimitValue,MaxLimitValue,SignedLimitCheck,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

    /*ID_BTC_MNTR_INT_STATE*/
    { "ID_BTC_MNTR_INT_STATE", INFORM_NOTHING, BTC_STATE_STANDBY, BTC_STATE_ERROR, UNSIGNED_VALUE, 1, BTC_STATE_STANDBY, NULL, NULL },
    /*ID_BTC_MNTR_INT_BLANKET_TEMPERATURE*/
    { "ID_BTC_MNTR_INT_BLANKET_TEMPERATURE", INFORM_NOTHING, 0, 5000, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_FILL_PUMP_POWER*/
    { "ID_BTC_MNTR_INT_FILL_PUMP_POWER", INFORM_NOTHING, 0, 1000, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_WATER_CONTAMINATION*/
    { "ID_BTC_MNTR_INT_WATER_CONTAMINATION", INFORM_NOTHING, 0, 100, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ENGAGE*/
    { "ID_BTC_MNTR_INT_ENGAGE", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_GLOBAL*/
    { "ID_BTC_MNTR_INT_ALARM_GLOBAL", INFORM_NOTHING, 0, 0xffffffff, UNSIGNED_VALUE, 1, 0, NULL, NULL },
#if 0
    /*ID_BTC_MNTR_INT_ALARM_ENGAGE_TIME_OUT*/
    { "ID_BTC_MNTR_INT_ALARM_ENGAGE_TIME_OUT", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_FILL_WATER_TIME_OUT*/
    { "ID_BTC_MNTR_INT_ALARM_FILL_WATER_TIME_OUT", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_AIR_KNIFE_TIME_OUT*/
    { "ID_BTC_MNTR_INT_ALARM_AIR_KNIFE_TIME_OUT", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_WATER_TANK_EXCEED_TEMPREATURE*/
    { "ID_BTC_MNTR_INT_ALARM_WATER_TANK_EXCEED_TEMPREATURE", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_WATER_TANK_EXCEED_TEMPREATURE*/
    { "ID_BTC_MNTR_INT_ALARM_WATER_LINE_EXCEED_TEMPREATURE", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_WATER_CONTAMINATION*/
    { "ID_BTC_MNTR_INT_ALARM_WATER_CONTAMINATION", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_WATER_TANK_OVERFLOW*/
    { "ID_BTC_MNTR_INT_ALARM_WATER_TANK_OVERFLOW", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
     /*ID_BTC_MNTR_INT_ALARM_TUB_OVERFLOW*/
    { "ID_BTC_MNTR_INT_ALARM_TUB_OVERFLOW", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_DRAWER*/
    { "ID_BTC_MNTR_INT_ALARM_DRAWER", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
    /*ID_BTC_MNTR_INT_ALARM_BLANKET*/
    { "ID_BTC_MNTR_INT_ALARM_BLANKET", INFORM_NOTHING, 0, 1, UNSIGNED_VALUE, 1, 0, NULL, NULL },
#endif
};


/* this starcture contains the default configuration data of GNRL string type*/
CfgDB_ID_StrDescriptor BtcDbStrDescriptor[CfgDB_ID_id(ID_BTC_CFG_STR_LAST)]=
{ /* default values */
	/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

	/* ID_BTC_CFG_STR_NOT_USED */
        {"ID_BTC_CFG_STR_NOT_USED", INFORM_NOTHING, MAX_CFG_STR_SIZE, 1, "FFFFFFFFFFFFF",               NULL,   NULL},

};

/* this starcture contains the default configuration data of MUX string type*/
CfgDB_ID_StrDescriptor   BtcDbVolatileStrDescriptor[CfgDB_ID_id(ID_BTC_VOLATILE_STR_LAST)]=
{ /* default values */
	/* InformClients,MaxAllowedStrLen ,MaxAllowedIndex,DefaultValue,GetHandle,SetHandle*/

	/*ID_BTC_VOLATILE_STR_NOT_USED*/
	{"ID_BTC_VOLATILE_STR_NOT_USED", INFORM_NOTHING,     MAX_CFG_STR_SIZE,  1,          "Not Used"    ,NULL,     NULL},



};


/* configuration integer element buffer pointers*/
static U32 * BtcDbIntPtr[CfgDB_ID_id(ID_BTC_CFG_INT_LAST)];
/* configuration string element buffer pointers*/
static char * BtcDbStrPtr[CfgDB_ID_id(ID_BTC_CFG_STR_LAST)];
/* configuration volatile string element buffer pointers*/
static char * BtcDbVolatileStrPtr[CfgDB_ID_id(ID_BTC_VOLATILE_STR_LAST)];
/* commands integer element buffer pointers*/
static U32 * BtcDbCmndIntPtr[CfgDB_ID_id(ID_BTC_CMND_INT_LAST)];
/* monitor integer element buffer pointers*/
static U32 * BtcDbMntrIntPtr[CfgDB_ID_id(ID_BTC_MNTR_INT_LAST)];


/*************************************************************************************************
Allocate and initialize the required memory in the RAM for the database elements
*************************************************************************************************/
static STATUS CreateRamDataBase()
{

	/* create the configuration Db of type integer*/
	if (CfgDB_CreateIntRamDB (&BtcDbIntDescriptor[0],&BtcDbIntPtr[0],
		CfgDB_ID_id(ID_BTC_CFG_INT_LAST))==ERROR)
		return ERROR;

	/* create the configuration Db of type string*/
	if (CfgDB_CreateStrRamDB (&BtcDbStrDescriptor[0],
		&BtcDbStrPtr[0],CfgDB_ID_id(ID_BTC_CFG_STR_LAST))==ERROR)
		return ERROR;

	/* create the configuration Db of type volatile string*/
	if (CfgDB_CreateStrRamDB (&BtcDbVolatileStrDescriptor[0],
		&BtcDbVolatileStrPtr[0],CfgDB_ID_id(ID_BTC_VOLATILE_STR_LAST))==ERROR)
		return ERROR;

	/* create the command Db */
	if (CfgDB_CreateIntRamDB (&BtcDbCmndIntDescriptor[0],&BtcDbCmndIntPtr[0],
		CfgDB_ID_id(ID_BTC_CMND_INT_LAST))==ERROR)
		return ERROR;

	/* create the monitor Db */
	if (CfgDB_CreateIntRamDB (&BtcDbMntrIntDescriptor[0],&BtcDbMntrIntPtr[0],
		CfgDB_ID_id(ID_BTC_MNTR_INT_LAST))==ERROR)
		return ERROR;

	return OK;
}


static void RemoveRamDataBase()
{
	/* remove the configuration Db of type integer*/
	CfgDB_DeleteIntRamDB (&BtcDbIntPtr[0],CfgDB_ID_id(ID_BTC_CFG_INT_LAST));
	/* remove the configuration Db of type string*/
	CfgDB_DeleteStrRamDB (&BtcDbStrPtr[0],CfgDB_ID_id(ID_BTC_CFG_STR_LAST));
	/* remove the volatile Db of type string*/
	CfgDB_DeleteStrRamDB (&BtcDbVolatileStrPtr[0],CfgDB_ID_id(ID_BTC_VOLATILE_STR_LAST));
	/* remove the command Db of type integer*/
	CfgDB_DeleteIntRamDB (&BtcDbCmndIntPtr[0],CfgDB_ID_id(ID_BTC_CMND_INT_LAST));
	/* remove the monitor Db of type integer*/
	CfgDB_DeleteIntRamDB (&BtcDbMntrIntPtr[0],CfgDB_ID_id(ID_BTC_MNTR_INT_LAST));
}

#if WIN32
BOOL BtcDbRdy=TRUE;
#else
BOOL BtcDbRdy=FALSE;
#endif

#if WIN32
BOOL BtcDbInInitState=FALSE;
#else
BOOL BtcDbInInitState=TRUE;
#endif


STATUS BtcCfgBuildDB(BOOL SkipDefaultFile)
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
			M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"BtcCfgInit- Error form CreateRamDataBase. ");
			return ERROR;
		}
	}


	return OK;
}

STATUS BtcCfgUpdateDbFromSource( BOOL ConfigMismatchCheck,BOOL LoadConfigFromUnit)
{
	STATUS RetValue;
	/* update the integer DB from the content of the source - Alarm if mismatch*/
	if ((RetValue=CfgDB_UpdateIntDbFromSource(-1,&BtcDbIntPtr[0],
		&BtcDbIntDescriptor[0],ID_BTC_CFG_INT_FIRST, ID_BTC_CFG_INT_LAST,ConfigMismatchCheck,LoadConfigFromUnit))!=OK)
	{
		if (RetValue!=CFG_DB_ERROR_ID_CONFIG_MISMATCH)
			M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in BtcCfgUpdateDbFromSource: CfgDB_UpdateIntDbFromSource RetValue=0x%x",RetValue);
	}

	return RetValue;
}



/*************************************************************************************************
Full initialization for the GNRL DB.
**************************************************************************************************/
STATUS BtcCfgInit()
{
	//char DefaultFileName[100];
	//char ModifiedFileName[100];
	//CfgDB_ID id;
	STATUS RetValue;
	//int i;
	//U32 oem_enable;
	BtcDbRdy=FALSE;
	BtcDbInInitState=TRUE;

	if ((RetValue=BtcCfgBuildDB(FALSE))!=OK)
		M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in BtcCfgBuildDB: RetValue=0x%x",RetValue);

       

	if ((RetValue=BtcCfg_UpdatePowerUpMntrDB())!=OK) /* update monitor DB from GNRL HW peripherals*/
	{
		M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"BtcCfgInit- Error form BtcCfg_UpdatePowerUpMntrDB. RetValue=%d ", RetValue);
	}

#if 0
        //update pheriperal data during power up
        UpdatePowerUpData();
#endif
	/* call to all relevant set handle of each entry in the str DB (only those which has one)
	for completed updating */
	if ((RetValue=CfgDB_CallToAllStrSetHandlesAndInformClients(0,&BtcDbStrPtr[0],
		&BtcDbStrDescriptor[0],ID_BTC_CFG_STR_FIRST, ID_BTC_CFG_STR_LAST))!=OK)
	{
		M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"BtcCfgInit- Error form CfgDB_CallToAllStrSetHandlesAndInformClients. RetValue=%d ", RetValue);
	}

	/* call to all relevant set handle of each entry in the int DB (only those which has one)
	for completed updating */

	if ((RetValue=CfgDB_CallToAllIntSetHandlesAndInformClients(0,&BtcDbIntPtr[0],
		&BtcDbIntDescriptor[0],
		ID_BTC_CFG_INT_FIRST, ID_BTC_CFG_INT_LAST))!=OK)
	{
		M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"BtcCfgInit- Error form CfgDB_CallToAllIntSetHandlesAndInformClients. RetValue=%d", RetValue);
	}

	BtcDbInInitState=FALSE;
	BtcDbRdy=TRUE;

	return RetValue;

}

STATUS BtcCfgDelete()
{
	if (!BtcDbRdy)
		return ERROR;

	BtcDbRdy=FALSE;
	RemoveRamDataBase();
	return OK;

}

int BtcCfgDbReady()
{
#if WIN32
	return TRUE;
#else
	return BtcDbRdy;
#endif
}


int BtcDbInInit()
{
#if WIN32
	return FALSE;
#else
	return BtcDbInInitState;
#endif

}


/*************************************************************************************************
Get element of any type (integer or tring) from the GNRL database
**************************************************************************************************/
STATUS  BtcCfgDB_GetElement (CfgDB_ID Id, CfgDB_ID_data * buff,int DataLen,CfgDB_GET_FLAG GetFlag)
{

	switch (Id.Field.AccessType)
	{
	case CfgDB_NON_VOLATILE: /* get configuration data element*/
		{
			if (Id.Field.DataType==CfgDB_INT)
			{
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_CFG_INT_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_GetIntElement (0,Id, &BtcDbIntPtr[0],
					&BtcDbIntDescriptor[0],
					&buff->IntData,GetFlag));
			}
			else
			{
				if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_CFG_STR_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_GetStrElement (0,Id, &BtcDbStrPtr[0],
					&BtcDbStrDescriptor[0],
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
			if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_CMND_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetIntElement (0,Id, &BtcDbCmndIntPtr[0],
				&BtcDbCmndIntDescriptor[0],
				&buff->IntData,GetFlag));
		}
	case CfgDB_MONITOR:
		{
			/* check for leggal id*/
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
			/* check for available id */
			if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_MNTR_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetIntElement (0,Id, &BtcDbMntrIntPtr[0],
				&BtcDbMntrIntDescriptor[0],
				&buff->IntData,GetFlag));

		}

	case CfgDB_VOLATILE_STR:
		{
			if (Id.Field.DataType!=CfgDB_STR)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

			if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_VOLATILE_STR_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_GetStrElement (0,Id, &BtcDbVolatileStrPtr[0],
				&BtcDbVolatileStrDescriptor[0],
				buff->StrData,DataLen,GetFlag));

		}
	default:
		return CFG_DB_ERROR_ID_ACCESS_TYPE;
	}

}



/*************************************************************************************************
Set element of any type (integer or tring) into the GNRL database
**************************************************************************************************/
STATUS  BtcCfgDB_SetElement (CfgDB_ID Id, CfgDB_ID_data * buff,CfgDB_SET_FLAG SetFlag)
{

	switch (Id.Field.AccessType)
	{
	case CfgDB_NON_VOLATILE: /* get configuration data element*/
		{
			if (Id.Field.DataType==CfgDB_INT)
			{ /* integer data type*/
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_CFG_INT_LAST))
					return CFG_DB_ERROR_ID_LIMIT;
				/* call the the generic routine*/
				return (CfgDB_SetIntElement (0,Id, &BtcDbIntPtr[0],
					&BtcDbIntDescriptor[0],
					buff->IntData,SetFlag));
			}
			else
			{ /* string data type*/
				/* check for leggal id*/
				if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_CFG_STR_LAST))
					return CFG_DB_ERROR_ID_LIMIT;

				return (CfgDB_SetStrElement (0,Id, &BtcDbStrPtr[0],
					&BtcDbStrDescriptor[0],
					buff->StrData,strlen(buff->StrData),SetFlag));
			}
		}
	case CfgDB_COMMAND:
		{
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be command*/
			/* check for leggal id*/
			if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_CMND_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetIntElement (0,Id, &BtcDbCmndIntPtr[0],
				&BtcDbCmndIntDescriptor[0],
				buff->IntData,SetFlag));
		}
	case CfgDB_MONITOR:
		{
			if (Id.Field.DataType!=CfgDB_INT)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*only integers data type can be monitors*/
			/* check for leggal id*/
			if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_MNTR_INT_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetIntElement (0,Id, &BtcDbMntrIntPtr[0],
				&BtcDbMntrIntDescriptor[0],
				buff->IntData,SetFlag));

		}

	case CfgDB_VOLATILE_STR:
		{

			if (Id.Field.DataType!=CfgDB_STR)
				return CFG_DB_ERROR_ID_DATA_TYPE; /*test for string type*/

			if (Id.Field.Id>=CfgDB_ID_id(ID_BTC_VOLATILE_STR_LAST))
				return CFG_DB_ERROR_ID_LIMIT;

			return (CfgDB_SetStrElement (0,Id, &BtcDbVolatileStrPtr[0],
				&BtcDbVolatileStrDescriptor[0],
				buff->StrData,strlen(buff->StrData),SetFlag));
		}
	default:
		return CFG_DB_ERROR_ID_ACCESS_TYPE;
	}

}




static STATUS BtcCfg_UpdatePowerUpMntrDB()
{
     STATUS RetValue=OK;
        
     if ((RetValue=CfgDB_UpdateIntDbFromSource(0,&BtcDbMntrIntPtr[0],
							&BtcDbMntrIntDescriptor[0],ID_BTC_MNTR_INT_FIRST,
							ID_BTC_MNTR_INT_LAST,FALSE,FALSE))!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in BtcCfg_UpdatePowerUpMntrDB: CfgDB_UpdateIntDbFromSource ");
		return RetValue;
    }
 
    return RetValue;
}



STATUS BtcCfgShowDb(BOOL IntType)
{
	int id,index;
	CfgDB_INT_DATA* IntBufPtr;
	char * StrBufPtr;

	if (IntType)
	{

		for (id=0;id<CfgDB_ID_id(ID_BTC_CFG_INT_LAST);id++)
		{
			IntBufPtr=BtcDbIntPtr[id];
			for (index=0;index<BtcDbIntDescriptor[id].MaxAllowedIndex;index++)
				M_LOGGER_LOGF (M_LOGGER_LEVEL_TRACE,"id=%d, id_index=%d, data=0x%x",id , index, IntBufPtr[index]);
		}

	}
	else
	{

		for (id=0;id<CfgDB_ID_id(ID_BTC_CFG_STR_LAST);id++)
		{
			StrBufPtr=BtcDbStrPtr[id];
			for (index=0;index<BtcDbStrDescriptor[id].MaxAllowedIndex;index++)
				M_LOGGER_LOGF (M_LOGGER_LEVEL_TRACE,"id=%d, id_index=%d, data=%s",id , index, (char*)StrBufPtr+index*MAX_CFG_STR_SIZE);
		}
	}


	return OK;
}




BOOL BtcDbIntIdIsNotDefault(CfgDB_INT_DATA id)
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
			DefaultValue=BtcDbIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		case CfgDB_COMMAND:
			DefaultValue=BtcDbCmndIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		case CfgDB_MONITOR:
			DefaultValue=BtcDbMntrIntDescriptor[CfgDB_ID_id(id)].DefaultValue;
			break;
		}
		if (Data!=DefaultValue)
			return TRUE; /* different from the default value*/
		else
			return FALSE; /* equal to the default value*/
	}
	else return FALSE;

}



CfgDB_ID_IntDescriptor * BtcCfg_DbIntDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements)
{
	switch (AccessType)
	{
	case CfgDB_NON_VOLATILE:
		*NumOfElements=CfgDB_ID_id(ID_BTC_CFG_INT_LAST);
		return &BtcDbIntDescriptor[0];
	case CfgDB_COMMAND:
		*NumOfElements=CfgDB_ID_id(ID_BTC_CMND_INT_LAST);
		return &BtcDbCmndIntDescriptor[0];
	case CfgDB_MONITOR:
		*NumOfElements=CfgDB_ID_id(ID_BTC_MNTR_INT_LAST);
		return &BtcDbMntrIntDescriptor[0];
	}
	return NULL;
}

CfgDB_ID_StrDescriptor * BtcCfg_DbStrDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements)
{
	switch (AccessType)
	{
	case CfgDB_NON_VOLATILE:
		*NumOfElements=CfgDB_ID_id(ID_BTC_CFG_STR_LAST);
		return &BtcDbStrDescriptor[0];
	case CfgDB_VOLATILE_STR:
		*NumOfElements=CfgDB_ID_id(ID_BTC_VOLATILE_STR_LAST);
		return &BtcDbVolatileStrDescriptor[0];

	}

	return NULL;

}


#ifdef __cplusplus
 }
#endif
