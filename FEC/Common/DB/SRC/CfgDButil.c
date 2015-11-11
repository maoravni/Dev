/* CfgDBUtil.c - DataBase engine and utilities */

/****************************************************************************
*
*           Title:   CfgDBUtil
*
*			Utitilies routine for the configuration DB
*
*           Project: MIDDLEWARE
*
*           Authors: Yossi Sarusi
*
*           Creation date: 19/8/12
*			Last Modified: 19/8/12
*
**
*
*****************************************************************************/
/*******************************************
DESCRIPTION:
*DataBase Engine
*/
#ifdef __cplusplus
 extern "C" {
#endif


#include <string.h>
#include "free_rtos_exp.h"
#include <stm32f2xx.h>
#include "ll_typedef.h"
#include <stdlib.h>
#include <stdio.h>

#include "CfgDButil.h"
#include "CfgDB.h"
#include "GnrlCfgDB.h"
#include "TemperatureCfgDB.h"
#include "OthersCfgDB.h"
#ifdef CPP_FRAMEWORK
#include <logger.h>
#endif

#define cfgDbUtil_DisplayLevel 5

#if _LL_TO_DO_ 
extern SEM_ID DbStrMutex;
#endif

#define CFG_FILE_PATH LocalFileDiskName

/* long file name handled by dos2 only */
#define GNRL_FILE_NAME     "Gnrl"

#define CFG_FILE_DEFAULT  "_Def"
#define CFG_FILE_MODIFIED "_Mdf"

#define CFG_FILE_INT      "_int"
#define CFG_FILE_STR      "_Str"

#define CFG_FILE_EXTENSION ".cfg"

typedef struct timeout_db
{
    INSTANCE_ID Instance;
    CfgDB_INT_DATA Id;
    int data;
    int timeout_count;
    struct timeout_db *next;
    int expired;
} TIMEOUT_DB;

static TIMEOUT_DB *timeout_header = NULL;

#if _LL_TO_DO_ 
static SEM_ID timeout_sem;
#endif
/**************************************************************************************************
 update the client about the change of the data
 **************************************************************************************************/
static STATUS InformClients(CfgDB_INFORM_CLIENTS ClientWhichNeedToBeInform, int InstanceNum, CfgDB_INT_DATA Id,
        void * Data, CfgDB_SET_FLAG SetFlag)
{
    STATUS TmpRetValue;
    CfgDB_ID TmpId;
    STATUS RetValue = OK;

    TmpId.DataAssign = Id;

#if _LL_TO_DO_ 

    if (ClientWhichNeedToBeInform & INFORM_TEMPERATURE)
    {
        if (TmpId.Field.UnitType==CfgDB_UnitGNRL)
        {
            for (InstanceNum=0;InstanceNum<MAX_NUM_OF_INSTANCE;InstanceNum++)
            {
            }
        }
        else
        {
            if ((TmpRetValue=CHAMBER_SendMsg(InstanceNum,NULL,CHAMBER_SET_DATA,Id,Data,FALSE))!=OK)
            RetValue=TmpRetValue;
        }
#if WIN32
        Temperature_SetData(InstanceNum,Id,Data);
#endif
    }

    if (ClientWhichNeedToBeInform & INFORM_GNRL)
    {
#if WIN32
        Gnrl_SetData(Id,Data);
#endif
        if ((TmpRetValue=GNRL_SendMsg(GNRL_SET_DATA,Id,Data))!=OK)
        RetValue=TmpRetValue;
    }

#endif        
    if (RetValue != OK)
        return CFG_DB_ERROR_ID_INFORM_BUSY;
    return RetValue;
}

/*
 This routine return the full path and files name (default and modified)
 according to the request id of the DB.
 */
STATUS CfgDB_FindFilesPathAndNames(CfgDB_ID id, char * RetDefaultFileName, char * RetModifiedFileName, int InstanceNum)
{
#if _LL_TO_DO_ 
    /*char Tmp[5];*/
    char DefaultFileName[50]="";
    char ModifiedFileName[50]="";
    /*	DIRECTORY_FILE_TYPE DirFileType;*/
    STATUS statusDef;
    STATUS statusMod;
    DIRECTORY_FILE_TYPE DirFileType;
    /* only CfgDB_NON_VOLATILE access type has file */
    if (id.Field.AccessType!=CfgDB_NON_VOLATILE)
    return CFG_DB_ERROR_ID_ACCESS_TYPE;
#if 0
    /* find the file path and name according the ID*/
    sprintf(DefaultFileName,"%s%d/",CFG_FILE_PATH,0); /* disk0*/
    sprintf(ModifiedFileName,"%s%d/",CFG_FILE_PATH,0);/* disk0*/
#endif

    statusDef = CfgDB_FindFilesPathAndNamesNew(id.Field.UnitType, id.Field.DataType,TRUE,
            InstanceNum,DefaultFileName,RetDefaultFileName);

    statusMod = CfgDB_FindFilesPathAndNamesNew(id.Field.UnitType, id.Field.DataType,FALSE,
            InstanceNum, ModifiedFileName,RetModifiedFileName);
    if ((statusDef == OK) && (statusMod == OK))
    return OK;
    else
    return ERROR;

#else
    return OK;
#endif
}

/*************************************************************************************************
 Generic routine for getting elements (string or integer) of any unit type from
 the DB.
 **************************************************************************************************/
STATUS CfgDB_GetElement(int InstanceNum, CfgDB_ID Id, CfgDB_ID_data * buff, int DataLen, CfgDB_GET_FLAG GetFlag)
{

    if (InstanceNum >= MAX_NUM_OF_INSTANCE)
        return CFG_DB_ERROR_ID_INSTANCE_NUM;

    switch (Id.Field.UnitType)
    {
    case CfgDB_UnitGNRL:
        return (GnrlCfgDB_GetElement(Id, buff, DataLen, GetFlag));
        break;

    case CfgDB_UnitTEMPERATURE:
        return (TemperatureCfgDB_GetElement(Id, buff, DataLen, GetFlag));
        break;
    case CfgDB_UnitOTHERS:
        return (OthersCfgDB_GetElement(Id, buff, DataLen, GetFlag));
        break;
    case CfgDB_UnitBTC:
        return (BtcCfgDB_GetElement(Id, buff, DataLen, GetFlag));
        break;

    default:
        return CFG_DB_ERROR_ID_UNIT_TYPE;
    }

    return CFG_DB_ERROR_ID_UNIT_TYPE;
}

/*************************************************************************************************
 Generic routine for setting elements (string or integer) of any unit type into
 the DB.
 **************************************************************************************************/
STATUS CfgDB_SetElement(int InstanceNum, CfgDB_ID Id, CfgDB_ID_data * buff, CfgDB_SET_FLAG SetFlag)
{
    /*U32* Ptr;*/

    if (InstanceNum >= MAX_NUM_OF_INSTANCE)
        return CFG_DB_ERROR_ID_INSTANCE_NUM;

    switch (Id.Field.UnitType)
    {
    case CfgDB_UnitGNRL:
        return (GnrlCfgDB_SetElement(Id, buff, SetFlag));
        break;

    case CfgDB_UnitTEMPERATURE:
        return (TemperatureCfgDB_SetElement(Id, buff, SetFlag));
        break;
    case CfgDB_UnitOTHERS:
        return (OthersCfgDB_SetElement(Id, buff, SetFlag));
        break;
    case CfgDB_UnitBTC:
        return (BtcCfgDB_SetElement(Id, buff, SetFlag));
        break;

    default:
        return CFG_DB_ERROR_ID_UNIT_TYPE;
    }

    return CFG_DB_ERROR_ID_UNIT_TYPE;
}

/*************************************************************
 *CfgDB_CheckDB - Check DB validity
 *
 *Check the validity of a specific DB id
 */

STATUS CfgDB_CheckDB(int InstanceNum, int IdData)
{

    CfgDB_ID *Id;

    Id = (CfgDB_ID*) &IdData;

    if (InstanceNum >= MAX_NUM_OF_INSTANCE)
        return ERROR;

    switch (Id->Field.UnitType)
    {
    case CfgDB_UnitGNRL:
        if (!GnrlDbReady())
            return ERROR;
        break;

    case CfgDB_UnitTEMPERATURE:
        if (!TemperatureCfgDbRdy(InstanceNum))
            return ERROR;
    case CfgDB_UnitOTHERS:
        if (!OthersCfgDbRdy(InstanceNum))
            return ERROR;
        break;
    case CfgDB_UnitBTC:
        if (!BtcCfgDbRdy(InstanceNum))
            return ERROR;
        break;
    default:
        return ERROR;
    }
    return OK;

}

/*************************************************************************************************
 this routine create and initialize the DB of type integer in default values
 located on the descriptor buffer
 *************************************************************************************************/
STATUS CfgDB_CreateIntRamDB(CfgDB_ID_IntDescriptor IntDescriptor[], CfgDB_INT_DATA* IntBufPtr[], int MaxElements)

{
    int id, IdIndex;
    int MaxIndexForId = 0;
    BOOL DefaultDataAsPointer = NULL;
    CfgDB_INT_DATA DefaultData = 0x12345678;
    int *DefaultDataPtr = NULL;

    /* for the configuration integers*/
    for (id = 0; id < MaxElements; id++)
    {
        U32* Ptr;
        if (IntDescriptor != NULL)
        { /* if exist extract the following parameters from it*/
            MaxIndexForId = IntDescriptor[id].MaxAllowedIndex;
            DefaultDataAsPointer = IntDescriptor[id].SignedLimitCheck & DEFAULT_DATA_AS_POINTER;
            if (DefaultDataAsPointer == 0)
                DefaultData = IntDescriptor[id].DefaultValue;
            else
                DefaultDataPtr = (int*) IntDescriptor[id].DefaultValue;

        }

        if ((Ptr = IntBufPtr[id] = pvPortMalloc(sizeof(CfgDB_INT_DATA) * MaxIndexForId)) == NULL)
            return ERROR;
        /* now fill with default values*/
        for (IdIndex = 0; IdIndex < MaxIndexForId; IdIndex++)
        {
            if (DefaultDataAsPointer)
                Ptr[IdIndex] = DefaultDataPtr[IdIndex];
            else
                Ptr[IdIndex] = DefaultData; /* the same default data to all*/
        }
    }
    return OK;
}

void CfgDB_DeleteIntRamDB(CfgDB_INT_DATA* IntBufPtr[], int MaxElements)
{
    int id;
    for (id = 0; id < MaxElements; id++)
    {
        if (IntBufPtr[id] != NULL)
        {
            vPortFree(IntBufPtr[id]);
            IntBufPtr[id] = NULL;
        }
    }
}

/* *************************************************************************************************
 this routine create and initialize the DB of type string in default values
 located on the descriptor buffer
 **************************************************************************************************/
STATUS CfgDB_CreateStrRamDB(CfgDB_ID_StrDescriptor StrDescriptor[], char * StrBufPtr[], int MaxElements)

{
    int id, IdIndex;
    int MaxIndexForId = 1; /* default one entry per id */
    char DefaultData[] = "Default String";
    char * DefaultDataPtr = (char*) &DefaultData;

    /* for the configuration integers*/
    for (id = 0; id < MaxElements; id++)
    {
        /*char tmp[10];*/
        char *Ptr;

        if (StrDescriptor != NULL)
        { /* if exist extract the following parameters from it*/
            MaxIndexForId = StrDescriptor[id].MaxAllowedIndex;
            DefaultDataPtr = (char*) &StrDescriptor[id].DefaultValue;
        }
        if ((Ptr = StrBufPtr[id] = pvPortMalloc(MAX_CFG_STR_SIZE * MaxIndexForId)) == NULL)
            return ERROR;
        /* now fill with default values*/
        for (IdIndex = 0; IdIndex < MaxIndexForId; IdIndex++)
        {
            strcpy((Ptr + IdIndex * MAX_CFG_STR_SIZE), DefaultDataPtr);
        }
    }
    return OK;
}

void CfgDB_DeleteStrRamDB(char * StrBufPtr[], int MaxElements)
{
    int id;
    for (id = 0; id < MaxElements; id++)
        if (StrBufPtr[id] != NULL)
        {
            vPortFree(StrBufPtr[id]);
            StrBufPtr[id] = NULL;
        }
}
/**************************************************************************************************
 Generic routine which check the validity of request value of type integer
 **************************************************************************************************/
STATUS CfgDB_MinMax(CfgDB_INT_DATA ReqValue, CfgDB_INT_DATA MinValue, CfgDB_INT_DATA MaxValue, BOOL SignedCheck)
{
    if (SignedCheck)
    {
        if (((int) ReqValue < (int) MinValue) || ((int) ReqValue > (int) MaxValue))
            return CFG_DB_ERROR_ID_ILLEGAL_DATA_ELEMENT;
    }
    else
    { /*unsigned check*/
        if ((ReqValue < MinValue) || (ReqValue > MaxValue))
            return CFG_DB_ERROR_ID_ILLEGAL_DATA_ELEMENT;
    }
    return OK;
}

/* Modify the MaxIndex value of the ID in the integer type descriptor */
void CfgDB_SetMaxIndexInIntDescriptor(U32 id, CfgDB_ID_IntDescriptor IntDescriptor[], int MaxIndex)
{
    CfgDB_ID_IntDescriptor * TempIntDescriptor;

    TempIntDescriptor = &IntDescriptor[CfgDB_ID_id(id)];
    if (TempIntDescriptor)
        if (!(TempIntDescriptor->ClientWhichNeedToBeInform & INFORM_CONSTANT_MAX_INDEX))
            TempIntDescriptor->MaxAllowedIndex = MaxIndex;
}

/* Modify the MaxIndex value of the ID in the string type descriptor */
void CfgDB_SetMaxIndexInStrDescriptor(U32 id, CfgDB_ID_StrDescriptor StrDescriptor[], int MaxIndex)
{
    CfgDB_ID_StrDescriptor * TempStrDescriptor;

    TempStrDescriptor = &StrDescriptor[CfgDB_ID_id(id)];
    if (TempStrDescriptor)
        if (!(TempStrDescriptor->ClientWhichNeedToBeInform & INFORM_CONSTANT_MAX_INDEX))
            TempStrDescriptor->MaxAllowedIndex = MaxIndex;
}

#ifdef _SUMMIT_
/* return the MaxIndex of the ID in the integer type descriptor */
STATUS CfgDB_GetMaxIndexFromIntDescriptor(U32 id,CfgDB_ID_IntDescriptor IntDescriptor[],int * MaxIndex)
{
    CfgDB_ID_IntDescriptor * TempIntDescriptor;

    TempIntDescriptor=&IntDescriptor[CfgDB_ID_id(id)];
    if (TempIntDescriptor)
    {
        *MaxIndex=TempIntDescriptor->MaxAllowedIndex;
        return OK;
    }
    return ERROR;
}

/* return the MaxIndex of the ID in the string type descriptor */
STATUS CfgDB_GetMaxIndexFromStrDescriptor(U32 id,CfgDB_ID_StrDescriptor StrDescriptor[],int * MaxIndex)
{
    CfgDB_ID_StrDescriptor * TempStrDescriptor;

    TempStrDescriptor=&StrDescriptor[CfgDB_ID_id(id)];
    if (TempStrDescriptor)
    {
        *MaxIndex=TempStrDescriptor->MaxAllowedIndex;
        return OK;
    }
    return ERROR;
}
#endif

/* Modify the MaxLimitValue value of the ID in the integer type descriptor */
void CfgDB_SetMaxLimitValueInIntDescriptor(U32 id, CfgDB_ID_IntDescriptor IntDescriptor[], int MaxLimitValue)
{
    CfgDB_ID_IntDescriptor * TempIntDescriptor;

    TempIntDescriptor = &IntDescriptor[CfgDB_ID_id(id)];
    if (TempIntDescriptor)
        TempIntDescriptor->MaxLimitValue = MaxLimitValue;
}

/* Modify the MinLimitValue value of the ID in the integer type descriptor */
void CfgDB_SetMinLimitValueInIntDescriptor(U32 id, CfgDB_ID_IntDescriptor IntDescriptor[], int MinLimitValue)
{
    CfgDB_ID_IntDescriptor * TempIntDescriptor;

    TempIntDescriptor = &IntDescriptor[CfgDB_ID_id(id)];
    if (TempIntDescriptor)
        TempIntDescriptor->MinLimitValue = MinLimitValue;
}

/* return the MinLimitValue and the MaxLimitValue values of the ID in the integer type descriptor */
STATUS CfgDB_GetMinMaxLimitValueFromIntDescriptor(U32 id, CfgDB_ID_IntDescriptor IntDescriptor[], int * MinLimitValue,
        int * MaxLimitValue)
{
    CfgDB_ID_IntDescriptor * TempIntDescriptor;

    TempIntDescriptor = &IntDescriptor[CfgDB_ID_id(id)];
    if (TempIntDescriptor)
    {
        *MinLimitValue = TempIntDescriptor->MinLimitValue;
        *MaxLimitValue = TempIntDescriptor->MaxLimitValue;
        return OK;
    }
    return ERROR;
}

/* return the Default Value of the ID in the integer type descriptor */
STATUS CfgDB_GetDefaultValueFromIntDescriptor(U32 id, CfgDB_ID_IntDescriptor IntDescriptor[], int * DefaultValue)
{
    CfgDB_ID_IntDescriptor * TempIntDescriptor;

    TempIntDescriptor = &IntDescriptor[CfgDB_ID_id(id)];
    if (TempIntDescriptor)
    {
        *DefaultValue = TempIntDescriptor->DefaultValue;
        return OK;
    }
    return ERROR;
}

/**************************************************************************************************
 Generic routine which handle getting element from DB buffer of type integer.
 **************************************************************************************************/
STATUS CfgDB_GetIntElement(int InstanceNum, CfgDB_ID Id, CfgDB_INT_DATA* IntBufPtr[],
        CfgDB_ID_IntDescriptor IntDescriptor[], CfgDB_INT_DATA * Data, CfgDB_GET_FLAG GetFlag)
{
    STATUS RetValue;

    if (IntDescriptor != NULL)
    {
        if (Id.Field.Id_Index >= IntDescriptor[Id.Field.Id].MaxAllowedIndex)
            return CFG_DB_ERROR_ID_INDEX_LIMIT;

        /* modify the get flag if the inform element conatin CfgDB_GET_WITH_CALLING_TO_HANDLE*/
        if (IntDescriptor[Id.Field.Id].ClientWhichNeedToBeInform & INFORM_FORCE_USE_GET_HANDLE)
            GetFlag |= CfgDB_GET_WITH_CALLING_TO_HANDLE;

        if (GetFlag & CfgDB_SET_TEST_SANITY) /* the test was previously done*/
            return OK;

        /* if request data is from the source and not from the DB */
        if (GetFlag & CfgDB_GET_DIRECTLY_FROM_SOURCE)
        {/* check for suitable get handle routine*/
            if (IntDescriptor[Id.Field.Id].GetHandle != NULL)
            {
                /* call to handle routine and get the data element from there*/
                if ((RetValue = IntDescriptor[Id.Field.Id].GetHandle(InstanceNum, Id.Field.Id_Index, Data)) == OK)
                {
                    IntBufPtr[Id.Field.Id][Id.Field.Id_Index] = *Data; /*also update the DB*/
                    return OK;
                }
                else
                    return RetValue;
            }
            else if (!(GetFlag & CfgDB_GET_DIRECTLY_FROM_RAM))
                return CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG; /* can not found get handle routine*/
        }

        if (GetFlag & CfgDB_GET_WITH_CALLING_TO_HANDLE)
        {
            if (IntDescriptor[Id.Field.Id].GetHandle != NULL)
            {

                /* call to handle routine and get the data element from there*/
                if ((RetValue = IntDescriptor[Id.Field.Id].GetHandle(InstanceNum, Id.Field.Id_Index, Data)) == OK)
                {
                    IntBufPtr[Id.Field.Id][Id.Field.Id_Index] = *Data; /*also update the DB*/
                    return OK;
                }
                else
                    return RetValue;
            }

        }

    }

    if (GetFlag & CfgDB_GET_DIRECTLY_FROM_RAM)
    {
        if (IntBufPtr[Id.Field.Id])
        {
            *Data = IntBufPtr[Id.Field.Id][Id.Field.Id_Index];
            return OK;
        }
        else
            return CFG_DB_ERROR_ID_INSTANCE_NUM;
    }
    else
        return CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG;

}

/**************************************************************************************************
 Generic routine which handle setting element into DB buffer of type integer.
 **************************************************************************************************/
STATUS CfgDB_SetIntElement(int InstanceNum, CfgDB_ID Id, CfgDB_INT_DATA* IntBufPtr[],
        CfgDB_ID_IntDescriptor IntDescriptor[], CfgDB_INT_DATA Data, CfgDB_SET_FLAG SetFlag)
{
    U32* Ptr;
    CfgDB_INFORM_CLIENTS ClientWhichNeedToBeInform;
    STATUS RetValue = OK;
    CfgDB_ID_IntDescriptor * TempIntDescriptor;

    Ptr = IntBufPtr[Id.Field.Id];

    if (IntDescriptor != NULL)
    {
        BOOL SetToRamBeforeSetHandle;
        TempIntDescriptor = &IntDescriptor[Id.Field.Id];

        /* check for leggal id index*/
        if (Id.Field.Id_Index >= TempIntDescriptor->MaxAllowedIndex)
            return CFG_DB_ERROR_ID_INDEX_LIMIT;

        /* check if value in leggal borders*/
        if ((RetValue = CfgDB_MinMax(Data, TempIntDescriptor->MinLimitValue, TempIntDescriptor->MaxLimitValue,
                (TempIntDescriptor->SignedLimitCheck & SIGNED_VALUE))) != OK)
            return RetValue;

        ClientWhichNeedToBeInform = TempIntDescriptor->ClientWhichNeedToBeInform;

        SetToRamBeforeSetHandle = (TempIntDescriptor->SignedLimitCheck & WRITE_RAM_BEFORE_SET_HANDLE)
                == WRITE_RAM_BEFORE_SET_HANDLE;

        if ((SetToRamBeforeSetHandle) && (SetFlag & CfgDB_SET_WITH_CALLING_TO_HANDLE))
        {
            /* update the RAM DB*/
            Ptr[Id.Field.Id_Index] = Data;
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                    "********CfgDB_SetIntElement:  WRITE_RAM_BEFORE_SET_HANDLE: ID=%d, data=%d ", Id.Field, Data);
        }

        if ((SetFlag & CfgDB_SET_WITH_CALLING_TO_HANDLE) && (TempIntDescriptor->SetHandle != NULL)
                && !InSimulationMode())
        /* if set handle exist execute the handle routine*/
        {
            if ((RetValue = TempIntDescriptor->SetHandle(InstanceNum, Id.Field.Id_Index, Data, &SetFlag)) != OK)
                return CFG_DB_ERROR_ID_HANDLE_RESULT;
        }

        if (SetFlag & CfgDB_SET_TO_RAM)
            /* update the RAM DB*/
            Ptr[Id.Field.Id_Index] = Data;

        if ((SetFlag & CfgDB_SET_WITH_INFORM_CLIENTS) && (ClientWhichNeedToBeInform != INFORM_NOTHING)
                && !(SetFlag & CfgDB_SET_TEST_SANITY))
        /* inform the clients task*/
        {
            if ((RetValue = InformClients(ClientWhichNeedToBeInform, InstanceNum, Id.DataAssign, &Data, SetFlag)) != OK)
                return RetValue;
        }

    }

    return RetValue;

}

/* protect the string update by a special mutex*/
static STATUS UpdateStrDB(char * dest, char * source, int DataLen)
{
#if _LL_TO_DO_  
    /* mutex when handling the string*/
    if (semTake(DbStrMutex,sysClkRateGet() * 1)==OK) /* up to 1 second wait*/
    {
        strncpy(dest,source,DataLen);
        dest[DataLen-1]=0;
        semGive(DbStrMutex);
        return OK;
    }
    else
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in UpdateStrDB: semTake failed on DbStrMutex");
        return ERROR;
    }
#else
    return OK;
#endif                
}

/* *************************************************************************************************
 Generic routine which handle getting element from DB buffer of type string.
 **************************************************************************************************/
STATUS CfgDB_GetStrElement(int InstanceNum, CfgDB_ID Id, char* StrBufPtr[], CfgDB_ID_StrDescriptor StrDescriptor[],
        char * Data, int DataLen, CfgDB_GET_FLAG GetFlag)
{
    char *Ptr;
    STATUS RetValue = OK;

    if (StrDescriptor != NULL)
    {
        if (Id.Field.Id_Index >= StrDescriptor[Id.Field.Id].MaxAllowedIndex)
            return CFG_DB_ERROR_ID_INDEX_LIMIT;

        /* modify the get flag if the inform element conatin CfgDB_GET_WITH_CALLING_TO_HANDLE*/
        if (StrDescriptor[Id.Field.Id].ClientWhichNeedToBeInform & INFORM_FORCE_USE_GET_HANDLE)
            GetFlag |= CfgDB_GET_WITH_CALLING_TO_HANDLE;

        if (GetFlag & CfgDB_GET_DIRECTLY_FROM_SOURCE)
        {/* check for suitable get handle routine*/
            if (StrDescriptor[Id.Field.Id].GetHandle != NULL)
            {
                /* call to handle routine and get the data element from there*/
                if ((RetValue = StrDescriptor[Id.Field.Id].GetHandle(InstanceNum, Id.Field.Id_Index, Data)) == OK)
                {
                    /*update the DB*/
                    Ptr = StrBufPtr[Id.Field.Id]; /*points to relevant entry*/
                    return (UpdateStrDB((Ptr + Id.Field.Id_Index * MAX_CFG_STR_SIZE), Data, DataLen));
                }
                else
                    return RetValue;
            }
            else if (!(GetFlag & CfgDB_GET_DIRECTLY_FROM_RAM))
                return CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG; /* can not found get handle routine*/
        }

        if (GetFlag & CfgDB_GET_WITH_CALLING_TO_HANDLE)
        {
            if (StrDescriptor[Id.Field.Id].GetHandle != NULL)
            {
                /* call to handle routine and get the data element from there*/
                if ((RetValue = StrDescriptor[Id.Field.Id].GetHandle(InstanceNum, Id.Field.Id_Index, Data)) == OK)
                {
                    /*update the DB*/
                    Ptr = StrBufPtr[Id.Field.Id]; /*points to relevant entry*/
                    return (UpdateStrDB((Ptr + Id.Field.Id_Index * MAX_CFG_STR_SIZE), Data, DataLen));
                }
                else
                    return RetValue;
            }

        }
    }

    if (GetFlag & CfgDB_GET_DIRECTLY_FROM_RAM)
    {
        if (StrBufPtr[Id.Field.Id])
            Ptr = StrBufPtr[Id.Field.Id]; /*points to relevant entry*/
        else
            return CFG_DB_ERROR_ID_INSTANCE_NUM;

        return (UpdateStrDB(Data, (Ptr + Id.Field.Id_Index * MAX_CFG_STR_SIZE), DataLen));
#if 0
        /* mutex when handling the string*/
        if (semTake(DbStrMutex,sysClkRateGet() * 1)==OK) /* up to 1 second wait*/
        {
            strcpy(Data,(Ptr+Id.Field.Id_Index*MAX_CFG_STR_SIZE));
            semGive(DbStrMutex);
            return OK;
        }
        else
        {
            M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgDB_GetStrElement: semTake failed on DbStrMutex");
            return ERROR;
        }
#endif
    }
    else
        return CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG;

}

/**************************************************************************************************
 Generic routine which handle setting element into DB buffer of type string.
 **************************************************************************************************/
STATUS CfgDB_SetStrElement(int InstanceNum, CfgDB_ID Id, char* StrBufPtr[], CfgDB_ID_StrDescriptor StrDescriptor[],
        char * Data, int DataLen, CfgDB_SET_FLAG SetFlag)
{
    char *Ptr;
    CfgDB_ID_StrDescriptor * TempStrDescriptor;
    CfgDB_INFORM_CLIENTS ClientWhichNeedToBeInform;
    STATUS RetValue = OK;

    if (StrDescriptor != NULL)
    {
        Ptr = StrBufPtr[Id.Field.Id];/*points to relevant entry*/
        TempStrDescriptor = &StrDescriptor[Id.Field.Id];/*points to relevant entry*/

        /* check for leggal id index*/
        if (Id.Field.Id_Index >= TempStrDescriptor->MaxAllowedIndex)
            return CFG_DB_ERROR_ID_INDEX_LIMIT;

        if (DataLen > TempStrDescriptor->MaxAllowedStrLen)
            return CFG_DB_ERROR_ID_ILLEGAL_STR_LEN;

        ClientWhichNeedToBeInform = TempStrDescriptor->ClientWhichNeedToBeInform; /* default*/
        if (SetFlag & CfgDB_SET_INFORM_REMOTE_ONLY_AND_IGNORED_REST_OF_THE_FLAG)
        {
            /* leave only the CfgDB_SET_WITH_INFORM_CLIENTS and ignore rest of the flag */
            SetFlag &= (CfgDB_SET_WITH_INFORM_CLIENTS | CfgDB_SET_TEST_SANITY);

        }

        if ((SetFlag & CfgDB_SET_WITH_CALLING_TO_HANDLE) && (TempStrDescriptor->SetHandle != NULL)
                && !InSimulationMode())
        /* if set handle exist execute the handle routine*/
        {
            if ((RetValue = TempStrDescriptor->SetHandle(InstanceNum, Id.Field.Id_Index, Data, &SetFlag)) != OK)
                return CFG_DB_ERROR_ID_HANDLE_RESULT;
        }

        if (SetFlag & CfgDB_SET_TO_RAM)
        {
            /* finally update the DB*/
            Ptr += Id.Field.Id_Index * MAX_CFG_STR_SIZE;
            /* mutex when handling the string*/
#if _LL_TO_DO_                         
            if (semTake(DbStrMutex,sysClkRateGet() * 1)==OK) /* up to 1 second wait*/
            {
                strcpy(Ptr,Data); /* update the RAM DB*/
                semGive(DbStrMutex);
            }
            else
            {
                M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgDB_SetStrElement: semTake failed on DbStrMutex");
                return ERROR;
            }
#endif                        
        }

        if ((SetFlag & CfgDB_SET_WITH_INFORM_CLIENTS) && (ClientWhichNeedToBeInform != INFORM_NOTHING)
                && !(SetFlag & CfgDB_SET_TEST_SANITY))
        /* inform the clients task*/
        {
            if ((RetValue = InformClients(ClientWhichNeedToBeInform, InstanceNum, Id.DataAssign, Data, SetFlag)) != OK)
                return RetValue;
        }

    }

    return OK;

}

/**************************************************************************************************
 This routine is called at the intialization state. it passes all the relevant
 entries OF TYPE INTEGER (set handle and inform clents) on the dscriptor of each id and update
 the clients about the relevant data on the database
 **************************************************************************************************/
STATUS CfgDB_CallToAllIntSetHandlesAndInformClients(int InstanceNum, CfgDB_INT_DATA* IntBufPtr[],
        CfgDB_ID_IntDescriptor IntDescriptor[], int FirstId, int LastId)
{

    int id, OriginalId, IdIndex;
    int MaxIndexForId = 1; /*default 1 element per id*/
    STATUS RetValue;
    CfgDB_SET_FLAG SetFlag = CfgDB_SET_TO_DB;

    if (InSimulationMode())
        return OK;

    /* for each id in and id_index call to its sutiable SetHandle routine if it exist*/
    for (OriginalId = FirstId; OriginalId < LastId; OriginalId++)
    {
        /*U32* Ptr;*/
        id = CfgDB_ID_id(OriginalId); /* leave only the id offset element*/

        if (IntDescriptor != NULL)
        { /* if exist extract the max allowed index for this id*/
            MaxIndexForId = IntDescriptor[id].MaxAllowedIndex;
        }

        /* call to SetHandle routine*/
        if ((IntDescriptor[id].SetHandle != NULL)
                && !(IntDescriptor[id].SignedLimitCheck & IGNORED_SET_HANDLE_DURING_INIT))
        {
            /*CfgDB_ID  TmpId;
             TmpId.DataAssign=OriginalId;*/

            for (IdIndex = 0; IdIndex < MaxIndexForId; IdIndex++)
            {/* if set handle exist execute the handle routine*/
                RetValue = IntDescriptor[id].SetHandle(InstanceNum, IdIndex, IntBufPtr[id][IdIndex], &SetFlag);
                if (RetValue != OK)
                {
                    char IdText[80];
                    CfgIdTextFromDescriptor(InstanceNum, OriginalId, IdText, sizeof(IdText));
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                            "ERROR in CfgDB_CallToAllIntSetHandlesAndInformClients in SetHandle: InstanceNum=%d,  id=%s, IdIndex=%d, RetValue=%d",
                            InstanceNum, IdText, IdIndex, RetValue);
                    /*return RetValue;*/
                }
            }
        }

        /* inform the clients task*/
        if (IntDescriptor[id].ClientWhichNeedToBeInform != INFORM_NOTHING)
        {
            for (IdIndex = 0; IdIndex < MaxIndexForId; IdIndex++)
            {
                if ((RetValue = InformClients(IntDescriptor[id].ClientWhichNeedToBeInform, InstanceNum,
                        CfgDB_ID_AddIndex(OriginalId,IdIndex), &IntBufPtr[id][IdIndex], SetFlag)) != OK)
                {
                    char IdText[80];
                    CfgIdTextFromDescriptor(InstanceNum, OriginalId, IdText, sizeof(IdText));
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                            "ERROR in CfgDB_CallToAllIntSetHandlesAndInformClients in InformClients: InstanceNum=%d,  id=%s, IdIndex=%d, RetValue=%d",
                            InstanceNum, IdText, IdIndex, RetValue);

                    /*return RetValue;*/
                }
            }
        }

    }

    return OK;

}

/**************************************************************************************************
 This routine is called at the intialization state. it passes all the relevant
 entries OF TYPE STRING (set handle and inform clents) on the dscriptor of each id and update
 the clients about the relevant data on the database
 **************************************************************************************************/
STATUS CfgDB_CallToAllStrSetHandlesAndInformClients(int InstanceNum, char* StrBufPtr[],
        CfgDB_ID_StrDescriptor StrDescriptor[], int FirstId, int LastId)
{
    int id, OriginalId, IdIndex;
    int MaxIndexForId = 1; /* default one entry per id*/
    STATUS RetValue;
    CfgDB_SET_FLAG SetFlag = CfgDB_SET_TO_DB;

    if (InSimulationMode())
        return OK;

    /* for the configuration integers*/
    for (OriginalId = FirstId; OriginalId < LastId; OriginalId++)
    {
        /*char tmp[10];*/
        char *Ptr;

        id = CfgDB_ID_id(OriginalId); /* leave only the id offset element*/

        if (StrDescriptor != NULL)
        { /* if exist extract the following MaxIndexForId from it*/
            MaxIndexForId = StrDescriptor[id].MaxAllowedIndex;
        }

        /* if set handle exist execute the handle routine*/
        if (StrDescriptor[id].SetHandle != NULL)
        {
            /*CfgDB_ID  TmpId;
             TmpId.DataAssign=OriginalId;*/

            Ptr = StrBufPtr[id]; /* points to the first string releted to the id*/
            for (IdIndex = 0; IdIndex < MaxIndexForId; IdIndex++)
            {
                if ((RetValue = StrDescriptor[id].SetHandle(InstanceNum, IdIndex, Ptr + IdIndex * MAX_CFG_STR_SIZE,
                        &SetFlag)) != OK)
                {
                    char IdText[80];
                    CfgIdTextFromDescriptor(InstanceNum, OriginalId, IdText, sizeof(IdText));
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                            "ERROR in CfgDB_CallToAllIntSetHandlesAndInformClients in SetHandle: InstanceNum=%d,  id=%s, IdIndex=%d, Data=%s, RetValue=%d",
                            InstanceNum, IdText, IdIndex, Ptr + IdIndex * MAX_CFG_STR_SIZE, RetValue);
                    /*return RetValue;*/
                }

            }
        }

        /* inform the clients task*/
        if (StrDescriptor[id].ClientWhichNeedToBeInform != INFORM_NOTHING)
        {
            Ptr = StrBufPtr[id]; /* points to the first string releted to the id*/
            for (IdIndex = 0; IdIndex < MaxIndexForId; IdIndex++)
            {
                if ((RetValue = InformClients(StrDescriptor[id].ClientWhichNeedToBeInform, InstanceNum,
                        CfgDB_ID_AddIndex(OriginalId,IdIndex), Ptr + IdIndex * MAX_CFG_STR_SIZE, SetFlag)) != OK)
                {
                    char IdText[80];
                    CfgIdTextFromDescriptor(InstanceNum, OriginalId, IdText, sizeof(IdText));
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                            "ERROR in CfgDB_CallToAllIntSetHandlesAndInformClients in InformClients: InstanceNum=%d,  id=%s, IdIndex=%d, RetValue=%d",
                            InstanceNum, IdText, IdIndex, RetValue);
                    /*return RetValue;*/
                }

            }
        }

    }

    return OK;
}

/**************************************************************************************************
 This routine is called upon user request (for protection purpose). it copys all configuration of type
 string (that their inform element in  the descriptor allowed it) from local DB to the mate DB.

 **************************************************************************************************/
STATUS CfgDB_CopyStrDbToMate(int InstanceNum, char* StrBufPtr[], CfgDB_ID_StrDescriptor StrDescriptor[], int FirstId,
        int LastId, int ConfigMismatchCheck)
{
    int id, OriginalId, IdIndex;
    int MaxIndexForId;
    STATUS RetValue;
    CfgDB_SET_FLAG SetFlag;
    CfgDB_ID Id;
    int idx;

    SetFlag = CfgDB_SET_TO_FILE | CfgDB_SET_IGNORE_PROTECTED_MATE | CfgDB_SET_IGNORE_BOARD_EXISTENCE;
    if (ConfigMismatchCheck)
        SetFlag |= CfgDB_SET_PROTECTED_MATE_MISMATCH_CHECK;

    idx = 0;
    RetValue = OK;
    /* for the configuration integers*/
    for (OriginalId = FirstId; OriginalId < LastId; OriginalId++)
    {
        /*char tmp[10];*/
        char *Ptr;
        Id.DataAssign = OriginalId;
        id = CfgDB_ID_id(OriginalId); /* leave only the id offset element*/

        if ((StrDescriptor != NULL) && (StrBufPtr[id] != NULL))
        {
            /* if exist extract the following MaxIndexForId from it*/
            MaxIndexForId = StrDescriptor[id].MaxAllowedIndex;
            Ptr = StrBufPtr[id]; /* points to the first string releted to the id*/

        }
        else
            RetValue |= ERROR; /* database not exist */

        /* send copy to mate in blocks, and wait 500ms in between */
        if (idx < 10)
            idx++;
        else
        {
            vTaskDelay(100); //taskDelay(100); /*add delay of 100msec*/
            idx = 0;
        }

    }

    return RetValue;
}

/**************************************************************************************************
 This routine is called upon user request (for protection purpose). it copys all configuration of type
 integer (that their inform element in  the descriptor allowed it) from local DB to the mate DB.

 **************************************************************************************************/
STATUS CfgDB_CopyIntDbToMate(int InstanceNum, CfgDB_INT_DATA* IntBufPtr[], CfgDB_ID_IntDescriptor IntDescriptor[],
        int FirstId, int LastId, int ConfigMismatchCheck)
{
    int id, OriginalId, IdIndex;
    int MaxIndexForId; /*default 1 element per id*/
    STATUS RetValue;
    CfgDB_SET_FLAG SetFlag;
    CfgDB_ID Id;
    int idx;

    SetFlag = CfgDB_SET_TO_FILE | CfgDB_SET_IGNORE_PROTECTED_MATE | CfgDB_SET_IGNORE_BOARD_EXISTENCE;
    if (ConfigMismatchCheck)
        SetFlag |= CfgDB_SET_PROTECTED_MATE_MISMATCH_CHECK;

    idx = 0;
    RetValue = OK;

    /* for each id in and id_index call to its sutiable SetHandle routine if it exist*/
    for (OriginalId = FirstId; OriginalId < LastId; OriginalId++)
    {
        /*U32* Ptr;*/
        Id.DataAssign = OriginalId;
        id = CfgDB_ID_id(OriginalId); /* leave only the id offset element*/

        if ((IntDescriptor != NULL) && (IntBufPtr[id] != NULL))
        {
            /* if exist extract the max allowed index for this id*/
            MaxIndexForId = IntDescriptor[id].MaxAllowedIndex;

        }
        else
            RetValue |= ERROR; /* database not exist */

        /* send copy to mate in blocks, and wait 200ms in between */
        if (idx < 10)
            idx++;
        else
        {
            vTaskDelay(200); //taskDelay(200); /*add delay of 200msec*/
            idx = 0;
        }

    }

    return RetValue;
}

/**************************************************************************************************
 This routine is called at the intialization state (if the source is hot).
 it passes all the relevant entries OF TYPE INTEGER (get handle) on the dscriptor
 of each id and update the DB ram about the relevant data from the source itself.
 **************************************************************************************************/
STATUS CfgDB_UpdateIntDbFromSource(int InstanceNum, CfgDB_INT_DATA* IntBufPtr[], CfgDB_ID_IntDescriptor IntDescriptor[],
        int FirstId, int LastId, BOOL ConfigMismatchCheck, BOOL LoadConfigFromUnit)
{
    int id, OriginalId, IdIndex;
    int MaxIndexForId = 1; /*default 1 element per id*/
    STATUS RetValue, RetValue1;
    int DataFromSource;
    /*CfgDB_ID  TmpId;*/
    char RetIdStr[150];
    /*char UnitTypeStr[20];*/

    RetValue1 = OK;

    //WdogServe();

    /* for each id in and id_index call to its sutiable SetHandle routine if it exist*/
    for (OriginalId = FirstId; OriginalId < LastId; OriginalId++)
    {
        /*U32* Ptr;*/
        id = CfgDB_ID_id(OriginalId); /* leave only the id offset element*/

        if (IntDescriptor != NULL)
        { /* if exist extract the max allowed index for this id*/
            MaxIndexForId = IntDescriptor[id].MaxAllowedIndex;
        }

        /* call to SetHandle routine*/
        if (IntDescriptor[id].GetHandle != NULL)
        {
            char IdText[80];
            char IdTextWithoutPrefix[70];
            int DataInDb;
            BOOL ForceCopy = FALSE;

            for (IdIndex = 0; IdIndex < MaxIndexForId; IdIndex++)
            {/* if set handle exist execute the handle routine*/
                DataFromSource = IntBufPtr[id][IdIndex]; /* by default put the DB value*/
                RetValue = IntDescriptor[id].GetHandle(InstanceNum, IdIndex, &DataFromSource);

                if (RetValue != OK)
                {
                    CfgIdTextFromDescriptor(InstanceNum, CfgDB_ID_AddIndex(OriginalId,IdIndex), IdText, sizeof(IdText));

                    M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                            "ERROR in CfgDB_UpdateIntDbFromSource in GetHandle: InstanceNum=%d, id=%s, IdIndex=%d, RetValue=0x%x",
                            InstanceNum, IdText, IdIndex, RetValue);
                    continue;
                    /*return RetValue;*/
                }

                if (LoadConfigFromUnit)
                {
                    /* get the value in the configuration file*/
                    if (CfgGetParam(InstanceNum, CfgDB_ID_AddIndex(OriginalId,IdIndex), &DataInDb, sizeof(DataInDb),
                            CfgDB_GET_DIRECTLY_FROM_DEFAULT_FILE | CfgDB_GET_DIRECTLY_FROM_MODIFIED_FILE) != OK)
                        ForceCopy = TRUE;
                }
                else
                    DataInDb = IntBufPtr[id][IdIndex];

                /* if mismatch was found, get the source value and initiates alarm */
                if (DataFromSource != DataInDb || ForceCopy)
                {
                    ForceCopy = FALSE;
                    if (ConfigMismatchCheck)
                    { /* alarm if mismatch occured*/
                        char ExtraText[150];
                        int AlarmId;
                        int tempInstanceNum;
#if _LL_TO_DO_

                        CfgIdTextFromDescriptor(InstanceNum, CfgDB_ID_AddIndex(OriginalId,IdIndex), IdText, sizeof(IdText));
                        CfgExcludeIdPrefix(IdText, IdTextWithoutPrefix);

                        if ((IntDescriptor[id].ClientWhichNeedToBeInform & INFORM_NOT_SHOW_MISMATCH_VALUE) == 0)
                        sprintf(ExtraText,"User Configuration Mismatch - (%s) S=0x%x, D=0x%x if=%d",
                                IdTextWithoutPrefix,DataFromSource,DataInDb,IdIndex + 1);
                        else
                        sprintf(ExtraText,"User Configuration Mismatch - (%s) if=%d",
                                IdTextWithoutPrefix,IdIndex + 1);

                        /*TmpId.DataAssign=OriginalId;
                         TmpId.Field.Id_Index=IdIndex;*/
                        M_LOGGER_LOGF(M_LOGGER_LEVEL_WARNING,"CfgDB_UpdateIntDbFromSource : configuration mismatch- Instance=%d, unitType=%s,OriginalId=%s, . DataFromSource=0x%x, Data in DB =0x%x",
                                InstanceNum, CfgUnit2Str(OriginalId),Id2Str(CfgDB_ID_AddIndex(OriginalId,IdIndex),RetIdStr), DataFromSource,IntBufPtr[id][IdIndex]);

                        if (InstanceNum==-1) /*Gnrl*/
                        {
                            CfgDB_ID Tmp_id;
                            Tmp_id.DataAssign = OriginalId;
                            if(Tmp_id.Field.UnitType == CfgDB_UnitAUX)/*Haim add aux user configuration mismatch*/
                            ALARM_Set(NULL, ID_GNRL_ALARM_AUX_USER_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                            else
                            ALARM_Set(NULL, ID_GNRL_ALARM_USER_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                            AlarmId=ID_GNRL_ALARM_EVENT_USER_CONFIGURATION_MISMATCH;
                            tempInstanceNum=NULL;
                        }
                        else
                        {
                            tempInstanceNum=InstanceNum;
                            ALARM_Set(tempInstanceNum, ID_TEMPERATURE_ALARM_USER_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                            AlarmId=ID_CMBR_ALARM_EVENT_USER_CONFIGURATION_MISMATCH;
                        }

                        ALARM_ExpandSet(tempInstanceNum, AlarmId, ExtraText,TRUE,ALARM_SET_FLAG_CHECK_ALL);
                        RetValue1=CFG_DB_ERROR_ID_CONFIG_MISMATCH;
#endif                                                
                    }
                    IntBufPtr[id][IdIndex] = DataFromSource; /* update the DB (RAM only) */
                    if (LoadConfigFromUnit)
                    {
                        /* Update the DB (file only)*/
                        CfgPutParam(InstanceNum, CfgDB_ID_AddIndex(OriginalId,IdIndex), &DataFromSource,
                                CfgDB_SET_TO_FILE | CfgDB_SET_IGNORE_PROTECTED_MATE);
                    }
                }
            }
        }
    }

    return RetValue1;

}

/**************************************************************************************************
 This routine is called at the intialization state. it passes all the relevant
 entries OF TYPE STRING (set handle and inform clents) on the dscriptor of each id and update
 the clients about the relevant data on the database
 **************************************************************************************************/
STATUS CfgDB_UpdateStrDbFromSource(int InstanceNum, char* StrBufPtr[], CfgDB_ID_StrDescriptor StrDescriptor[],
        int FirstId, int LastId, BOOL ConfigMismatchCheck, BOOL LoadConfigFromUnit)
{
    int id, OriginalId, IdIndex;
    int MaxIndexForId = 1; /* default one entry per id*/
    STATUS RetValue, RetValue1;
    char DataFromSource[MAX_CFG_STR_SIZE];
    char DataInDb[MAX_CFG_STR_SIZE];
    CfgDB_ID TmpId;

    RetValue1 = OK;
#if _LL_TO_DO_ 

    WdogServe();

    /* for the configuration integers*/
    for (OriginalId=FirstId;OriginalId<LastId;OriginalId++)
    {
        /*char tmp[10];*/
        char *Ptr;
        id=CfgDB_ID_id(OriginalId); /* leave only the id offset element*/

        if (StrDescriptor!=NULL)
        { /* if exist extract the following MaxIndexForId from it*/
            MaxIndexForId=StrDescriptor[id].MaxAllowedIndex;
        }

        /* if set handle exist execute the handle routine*/
        if (StrDescriptor[id].GetHandle!=NULL)
        {
            BOOL ForceCopy = FALSE;
            Ptr=StrBufPtr[id]; /* points to the first string releted to the id*/
            for (IdIndex=0;IdIndex<MaxIndexForId;IdIndex++)
            {
                char * IndexPtr;
                IndexPtr=Ptr+IdIndex*MAX_CFG_STR_SIZE; /* points to the relevant index data*/

                strcpy(DataFromSource,IndexPtr); /*by default put the value in the DB*/
                if ((RetValue=StrDescriptor[id].GetHandle(InstanceNum,IdIndex,DataFromSource))!=OK)
                {
                    M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgDB_UpdateStrDbFromSource in GetHandle: InstanceNum=%d, id=%d, IdIndex=%d, RetValue=0x%x", InstanceNum, id, IdIndex,RetValue);
                    continue;
                    /*return RetValue;*/
                }

                if (LoadConfigFromUnit)
                {
                    /* get the value in the configuration file*/
                    if(CfgGetParam(InstanceNum, OriginalId,&DataInDb,MAX_CFG_STR_SIZE, CfgDB_GET_DIRECTLY_FROM_DEFAULT_FILE |CfgDB_GET_DIRECTLY_FROM_MODIFIED_FILE)!=OK)
                    ForceCopy = TRUE;
                }
                else
                strcpy(DataInDb,IndexPtr);

                /* mutex when handling the string*/
                if (semTake(DbStrMutex,sysClkRateGet() * 1)==OK) /* up to 1 second wait*/
                {
                    /* if mismatch was found, get the value from the source and initiates alarm */
                    if (strcmp(DataFromSource,DataInDb) || ForceCopy)
                    { /* mismatch*/
                        ForceCopy = FALSE;
                        if (ConfigMismatchCheck)
                        { /* alarm if mismatch occured*/
                            char ExtraText[150];
                            /*char RetIdStr[100];*/
                            char IdText[80];
                            char IdTextWithoutPrefix[70];
                            int AlarmId;

                            CfgIdTextFromDescriptor(InstanceNum, OriginalId, IdText, sizeof(IdText));
                            CfgExcludeIdPrefix(IdText, IdTextWithoutPrefix);
                            TmpId.DataAssign=OriginalId;
#ifndef _FALCON
                            if ((StrDescriptor[id].ClientWhichNeedToBeInform & INFORM_NOT_SHOW_MISMATCH_VALUE) == 0)
                            sprintf(ExtraText,"User Configuration Mismatch - (%s) S=%s, D=%s",
                                    IdTextWithoutPrefix,DataFromSource,IndexPtr);
                            else
                            sprintf(ExtraText,"User Configuration Mismatch - (%s)",
                                    IdTextWithoutPrefix);
#else
                            if ((StrDescriptor[id].ClientWhichNeedToBeInform & INFORM_NOT_SHOW_MISMATCH_VALUE) == 0)
                            sprintf(ExtraText,"User Configuration Mismatch - (%s) S=%s, D=%s if=%d",
                                    IdTextWithoutPrefix,DataFromSource,IndexPtr,IdIndex + 1);
                            else
                            sprintf(ExtraText,"User Configuration Mismatch - (%s) if=%d",
                                    IdTextWithoutPrefix,IdIndex + 1);
#endif
                            M_LOGGER_LOGF (M_LOGGER_LEVEL_WARNING,"CfgDB_UpdateStrDbFromSource : configuration mismatch- Instance=%d, UnitType=%d,Id=0x%x, IdIndex=%d, DataFromSource=%s, DataInDb=%s",
                                    InstanceNum, TmpId.Field.UnitType, id,IdIndex, DataFromSource,IndexPtr);

                            if (InstanceNum==-1) /*Gnrl*/
                            {
                                ALARM_Set(NULL, ID_GNRL_ALARM_USER_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                                AlarmId=ID_GNRL_ALARM_EVENT_USER_CONFIGURATION_MISMATCH;
                                InstanceNum=NULL;
                            }
                            else
                            {
                                ALARM_Set(InstanceNum, ID_TEMPERATURE_ALARM_USER_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                                AlarmId=ID_TEMPERATURE_ALARM_EVENT_USER_CONFIGURATION_MISMATCH;
                            }

                            ALARM_ExpandSet(InstanceNum, AlarmId,ExtraText, TRUE,ALARM_SET_FLAG_CHECK_ALL);
                            RetValue1=CFG_DB_ERROR_ID_CONFIG_MISMATCH;
                        }
                        strcpy(IndexPtr,DataFromSource); /* update the DB (RAM only) */
                        if (LoadConfigFromUnit)
                        {
                            /* Update the DB (file only)*/
                            CfgPutParam(InstanceNum, OriginalId,&DataFromSource,CfgDB_SET_TO_FILE);
                        }

                    }
                    semGive(DbStrMutex);
                }
                else
                {
                    M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgDB_UpdateStrDbFromSource: semTake failed on DbStrMutex");
                    return ERROR;
                }
            }
        }
    }
#endif
    return RetValue1;
}

#if _LL_TO_DO_ 
STATUS UpdatePostResetHeaderVersionAndDescriptionStrDb(INSTANCE_ID InstanceNum,DOWNLOAD_FileType FileType,
        int FileSubType,int Firmware,int FirmwareIndex, int SelectVer)
{

    char LocalFileName[60];
    char FullLocalFileName[200];
    char HeaderVersionStr[MAX_CFG_STR_SIZE];
    char HeaderDescriptionStr[MAX_CFG_STR_SIZE];
    STATUS RetValue;
    CfgDB_INT_DATA CfgVersionStrId = 0,CfgDescriptionStrId = 0;
    BOOL DefaultDisk;
    DIRECTORY_FILE_TYPE DirType;

    DefaultDisk=TRUE; /* /tffs0/ */
    DirType=DIRECTORY_APPLIC;

    switch (FileType)
    {
        case DOWNLOAD_FileType_BOOT:
        CfgVersionStrId=ID_GNRL_VOLATILE_STR_BOOT_SOFT_VERSION;
        CfgDescriptionStrId=ID_GNRL_VOLATILE_STR_BOOT_SOFT_DESCRIPTION;
        DirType=DIRECTORY_ROOT;
        break;
        case DOWNLOAD_FileType_GNRL:
        CfgVersionStrId=ID_GNRL_VOLATILE_STR_GET_APPLIC_VER;
        CfgDescriptionStrId=ID_GNRL_VOLATILE_STR_GET_APPLIC_DESCRIPTION;
        break;
    }

    /* in case where the SelectVer=ERROR clear the string data in the DB*/
    if (SelectVer!=ERROR)
    {
        /* find the full file name*/
        strcpy(LocalFileName,"");
        UTILS_GenerateNameOfApplicFile(FileType,FileSubType,Firmware,FirmwareIndex,SelectVer,LocalFileName);
        UTILS_AddDirctoryToFileName(DirType,LocalFileName,NULL,FullLocalFileName,DefaultDisk);
        strcpy(HeaderVersionStr,"");
        strcpy(HeaderDescriptionStr,"");
        /* read the version and description strings from the file header*/
        if (UTIL_FileExist(FullLocalFileName))
        {
            if ( GetHeaderVersionAndDescriptionStr(FullLocalFileName, HeaderVersionStr, HeaderDescriptionStr, MAX_CFG_STR_SIZE)!=OK)
            {
                M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"UpdatePostResetHeaderVersionAndDescriptionStrDb- Error form GetHeaderVersionAndDescriptionStr FileName=%s not exists",FullLocalFileName);
            }
        }

    }
    else
    {
        strcpy(HeaderVersionStr,"");
        strcpy(HeaderDescriptionStr,"");
    }
    /* update current application version and description*/
    if ((RetValue=CfgPutParam(InstanceNum,CfgVersionStrId,HeaderVersionStr,CfgDB_SET_TO_RAM))!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"UpdatePostResetHeaderVersionAndDescriptionStrDb- Error form CfgPutParam Id=0x%x.  Error=%d",CfgVersionStrId,RetValue);
    }

    if ((RetValue=CfgPutParam(InstanceNum,CfgDescriptionStrId,HeaderDescriptionStr,CfgDB_SET_TO_RAM))!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"UpdatePostResetHeaderVersionAndDescriptionStrDb- Error form CfgPutParam Id=0x%x.  Error=%d",CfgDescriptionStrId,RetValue);
    }

    return OK;
}
#endif

char * CfgUnit2Str(int id)
{
    CfgDB_ID Id;
    Id.DataAssign = id;
    switch (Id.Field.UnitType)
    {
    case CfgDB_UnitGNRL:
        return "GNRL";
        break;

    case CfgDB_UnitTEMPERATURE:
        return "CBR";
        break;

    default:
        return "ERR_Unit";
        break;
    }
}

char * Access2Str(int id)
{
    CfgDB_ID Id;
    Id.DataAssign = id;

    switch (Id.Field.AccessType)
    {
    case CfgDB_NON_VOLATILE:
        return "CFG";
        break;
    case CfgDB_COMMAND:
        return "CMND";
        break;
    case CfgDB_MONITOR:
        return "MNTR";
        break;
    case CfgDB_VOLATILE_STR:
        return "VOLATILE STR";
        break;
    default:
        return "DEF_ACCSES";
        break;
    }
}

char * DataType2Str(int id)
{
    CfgDB_ID Id;
    Id.DataAssign = id;
    switch (Id.Field.DataType)
    {
    case CfgDB_INT:
        return "INT";
        break;
    case CfgDB_STR:
        return "STR";
        break;
    default:
        return "Def";
        break;

    }
}
char * Id2Str(int id, char * RetStr)
{
    char TempStr[80];
    int Instance = INSTANCE_1;
    /*char TempStr1[80];*/
    CfgDB_ID Id;
    Id.DataAssign = id;

#if _LL_TO_DO_
    if (!MdmCfgDbRdy(CHAMBER_LEFT))
    Instance=CHAMBER_RIGHT; /* fix abug when left drawer DB not exists*/
#endif
    CfgIdTextFromDescriptor(Instance, id, TempStr, sizeof(TempStr));
    sprintf(RetStr, "%s", TempStr);

    return RetStr;
}

char * Err2Str(CfgDB_ERROR_ID ErrId)
{
    static char RetStr[50];
    switch (ErrId)
    {
    case CFG_DB_ERROR_ID_LIMIT:
        return "ID_LIMIT";
        break;
    case CFG_DB_ERROR_ID_INDEX_LIMIT:
        return "INDEX_LIMIT";
        break;
    case CFG_DB_ERROR_ID_DATA_TYPE:
        return "DATA_TYPE";
        break;
    case CFG_DB_ERROR_ID_ACCESS_TYPE:
        return "ACCESS_TYPE";
        break;
    case CFG_DB_ERROR_ID_UNIT_TYPE:
        return "UNIT_TYPE";
        break;
    case CFG_DB_ERROR_ID_INSTANCE_NUM:
        return "CHAMBER_NUM";
        break;
    case CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG:
        return "ILLEGAL_SET_FLAG";
        break;
    case CFG_DB_ERROR_ID_ILLEGAL_DATA_ELEMENT:
        return "ILLEGAL_DATA_ELEMENT";
        break;
    case CFG_DB_ERROR_ID_ILLEGAL_STR_LEN:
        return "ILLEGAL_STR_LEN";
        break;
    case CFG_DB_ERROR_ID_HANDLE_RESULT:
        return "HANDLE_RESULT";
        break;
    case CFG_DB_ERROR_ID_HANDLE_BUSY:
        return "HANDLE_BUSY";
        break;
    case CFG_DB_ERROR_ID_INFORM_BUSY:
        return "INFORM_BUSY";
        break;

    case CFG_DB_ERROR_ID_CONFIG_MISMATCH:
        return "CONFIG_MISMATCH";
        break;
#if _LL_TO_DO_ 
        /* file error*/
        case FileHAND_ERROR_ID_CREATE:
        return "FILE CREATE";
        break;
        case FileHAND_ERROR_ID_WRITE:
        return "FILE WRITE";
        break;
        case FileHAND_ERROR_ID_READ:
        return "FILE READ";
        break;
        case FileHAND_ERROR_ID_SEEK:
        return "FILE SEEK";
        break;
        case FileHAND_ERROR_ID_IVALID_ID:
        return "FILE IVALID_ID";
        break;
        case FileHAND_ERROR_ID_IVALID_ID_INDEX:
        return "FILE IVALID_ID_INDEX";
        break;
        case FileHAND_ERROR_ID_IVALID_VALUE:
        return "FILE IVALID_VALUE";
        break;
        case FileHAND_ERROR_ID_ELEMENT_NOT_FOUND:
        return "FILE ELEMENT_NOT_FOUND";
        break;
        case FileHAND_ERROR_ID_FILE_NOT_EXIST:
        return "FILE NOT_EXIST";
        break;
#endif                        
    default:
        sprintf(RetStr, "DEFAULT =%d", ErrId);
        return RetStr;
        break;
    }
}

/*************************simulation handeling*************************************/

static BOOL SimulationMode = false;

BOOL InSimulationMode()
{
    return (SimulationMode);
}

void ActiveSimulationMode(BOOL Acitive)
{
    SimulationMode = Acitive;
}

/* return true if there is a value in the modified file, which means
 that the configuration value was modified by the user*/
BOOL CfgIdWasModified(int InstanceNum, CfgDB_INT_DATA Id)
{
    U32 Data;
    if (CfgGetParam(InstanceNum, Id, &Data, sizeof(Data), CfgDB_GET_DIRECTLY_FROM_MODIFIED_FILE) == OK)
        return TRUE;
    else
        return FALSE;
}

/*SEC ***************** TimeOut Routines *********************************/

static void TimeOutDelete(TIMEOUT_DB *timeout, TIMEOUT_DB *prev)
{
    if (timeout == NULL)
        return;
    if (prev == NULL)
    {
        timeout_header = timeout->next;
        vPortFree(timeout);
        return;
    }
    prev->next = timeout->next;
    vPortFree(timeout);
}

static void TimeOutDeleteRecord(INSTANCE_ID Instance, CfgDB_INT_DATA Id,
        int (*cmp_func)(TIMEOUT_DB *, INSTANCE_ID, CfgDB_INT_DATA))
{
#if _LL_TO_DO_     
    TIMEOUT_DB *timeout;
    TIMEOUT_DB *prev = NULL;
    timeout = timeout_header;
    semTake(timeout_sem,WAIT_FOREVER);
    while(timeout !=NULL)
    {
        if (cmp_func(timeout,Instance,Id))
        {
            TimeOutDelete(timeout, prev);
            break;
        }
        else
        {
            prev = timeout;
            timeout = timeout->next;
        }
    }
    semGive(timeout_sem);
#endif    
}

static int SameIdAndInstance(TIMEOUT_DB *timeout, INSTANCE_ID Instance, CfgDB_INT_DATA Id)
{
    if ((timeout->Id == Id) && (timeout->Instance == Instance))
        return (1);
    else
        return (0);
}

static int SameInstance(TIMEOUT_DB *timeout, INSTANCE_ID Instance, CfgDB_INT_DATA Id)
{
    CfgDB_ID ID;
    ID.DataAssign = timeout->Id;
    switch (ID.Field.UnitType)
    {
    case CfgDB_UnitTEMPERATURE:

        if (timeout->Instance == Instance)
            return (1);
        else
            return (0);
    };
    return (0);
}

void CfgTimeOutXCDelete(int Instance)
{
#if _LL_TO_DO_     
    semTake(timeout_sem,WAIT_FOREVER);
    TimeOutDeleteRecord(Instance,0,SameXC);
    semGive(timeout_sem);
#endif      
}

void CfgTimeOutInit()
{
#if _LL_TO_DO_     
    timeout_sem = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
    timeout_header = NULL;
#endif  
}

/*
 *  FUNCTION: CfgTimeOutCreate
 *
 *  PARAMETERS:Instance - Instance number
 *             Id - DataBase Id
 *             data - parameter
 *             count - timeout (in second)
 *
 *  DESCRIPTION: Generate a CfgPutParam with Instance and Id after timeout
 *
 *  RETURNS: none
 *
 */

void CfgTimeOutCreate(int Instance, CfgDB_INT_DATA Id, int data, int count)
{
#if _LL_TO_DO_     
    TIMEOUT_DB *timeout;
    if (count < 0) return;
    semTake(timeout_sem,WAIT_FOREVER);
    timeout = (TIMEOUT_DB *)pvPortMalloc(1);
    timeout->Instance = Instance;
    timeout->Id = Id;
    timeout->data = data;
    timeout->timeout_count = count;
    timeout->next = timeout_header;
    timeout->expired = 0;
    timeout_header = timeout;
    semGive(timeout_sem);
#endif    
}

/*
 *  FUNCTION: CfgTimeOutDelete
 *
 *  PARAMETERS:Instance - Instance number
 *             Id - DataBase Id
 *
 *  DESCRIPTION: Delete Timeout entry with specified parameters
 *
 *  RETURNS: none
 *
 */

void CfgTimeOutDelete(int InstanceId, CfgDB_INT_DATA Id)
{
#if _LL_TO_DO_     
    semTake(timeout_sem,WAIT_FOREVER);
    TimeOutDeleteRecord(Instance,Id,SameIdAndInstance);
    semGive(timeout_sem);
#endif    
}

void CfgTimeOutReplace(int InstanceId, CfgDB_INT_DATA Id, int count)
{
#if _LL_TO_DO_     
    TIMEOUT_DB *timeout;
    CfgDB_ID TimeoutId;
    timeout = timeout_header;

    semTake(timeout_sem,WAIT_FOREVER);
    while (timeout != NULL)
    {
        TimeoutId.DataAssign=timeout->Id;
        TimeoutId.Field.Id_Index=0; /* handle all timeout Id's interfaces of the request ID*/
        if ((timeout->Instance == Instance) && (TimeoutId.DataAssign == Id))
        {
            timeout->timeout_count = count;
        }
        timeout = timeout->next;
    }
    semGive(timeout_sem);
#endif    
}

/*
 *  FUNCTION: CfgTimeOutInstanceDelete
 *
 *  PARAMETERS:Instance - Instance number
 *
 *  DESCRIPTION: Delete Timeout entries assosiated with the given Instance
 *
 *  RETURNS: none
 *
 */

void CfgTimeOutInstanceDelete(int InstanceId)
{
#if _LL_TO_DO_     
    semTake(timeout_sem,WAIT_FOREVER);
    TimeOutDeleteRecord(Instance,0,SameInstance);
    semGive(timeout_sem);
#endif    
}

void CfgTimeOutScan()
{
#if _LL_TO_DO_   
    TIMEOUT_DB *timeout;
    TIMEOUT_DB *prev = NULL;
    TIMEOUT_DB *next;
    semTake(timeout_sem,WAIT_FOREVER);
    timeout = timeout_header;
    while (timeout != NULL)
    {
        next = timeout->next;
        if (timeout->timeout_count > 0)
        {
            timeout->timeout_count --;
            if ( timeout->timeout_count == 0)
            {
                timeout->expired = 1; /* mark timeout for delete if not done in put param */
                CfgPutParam(timeout->Instance,timeout->Id,&timeout->data,CfgDB_SET_TO_DB);
            }
        }
        timeout = next;
    }
    timeout = timeout_header;
    while (timeout != NULL)
    {
        next = timeout->next;
        if (timeout->expired == 1)
        TimeOutDelete(timeout,prev);
        else
        prev = timeout;
        timeout = next;
    }
    semGive(timeout_sem);
#endif    
}

/*
 *  FUNCTION: showCfgTimers
 *
 *  PARAMETERS: none
 *  DESCRIPTION: Show Active Cfg Timers
 *
 *  RETURNS: none
 *
 */

void showCfgTimers()
{
#if _LL_TO_DO_      
    TIMEOUT_DB *timeout;
    semTake(timeout_sem,WAIT_FOREVER);
    timeout = timeout_header;
    while (timeout != NULL)
    {
        DISPL_printf("car=%d,id=0x%x,exp=%d\r\n",timeout->Instance,timeout->Id,timeout->timeout_count);
        timeout = timeout->next;
    }
    semGive(timeout_sem);
#endif    
}

void CfgSetFactoryDefault(int InstanceNum, BOOL EntireDisk)
{
#if _LL_TO_DO_    
    char DirectoryName[40];
    /*int InstanceNum;*/
    FILE * fh;
    int PrevOutputFd;

    SetConsolePrintToTempFile(TRUE, &PrevOutputFd, &fh);
#if 0
    char FullLocalFileName[100];

    UTILS_AddDirctoryToFileName(DIRECTORY_ROOT,"tempFile",NULL,FullLocalFileName,TRUE);

    /* open the local disk file*/
    if( (fh = fopen( FullLocalFileName, "wb")) == NULL )
    { /*create it */
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgSetFactoryDefault in fopen file name=%s", FullLocalFileName);
        fclose( fh );
        return;
    }

    PrevOutputFd=ioGlobalStdGet(1);

    ioGlobalStdSet (
            1, /* std input (0), output (1), or error (2) */
            fileno( fh) ); /* new underlying file descriptor */
#endif
    if (EntireDisk)
    {
        UTILS_FindDirName(DIRECTORY_GNRL_CONFIG,NULL,TRUE,DirectoryName); /* Gnrl config dir*/
        strcat(DirectoryName,"/*_Mdf*.*"); /* only modified files*/
        WdogServe();
        xdelete(DirectoryName);

        for (InstanceNum=0;InstanceNum<MAX_NUM_OF_INSTANCE;InstanceNum++)
        {
            UTILS_FindDirName(DIRECTORY_TEMPERATURE_CONFIG,InstanceNum,TRUE,DirectoryName); /* Instance config dir*/
            strcat(DirectoryName,"/*_Mdf*.*"); /* only modified files*/
            WdogServe();
            xdelete(DirectoryName);
            UTILS_FindDirName(DIRECTORY_TEMPERATURE_CONFIG,InstanceNum,TRUE,DirectoryName); /* Instance config dir*/
            strcat(DirectoryName,"/*_*_Mdf*.*"); /* only modified files- */
            WdogServe();
            xdelete(DirectoryName);
        }
    }
    else
    {
        UTILS_FindDirName(DIRECTORY_TEMPERATURE_CONFIG,InstanceNum,TRUE,DirectoryName); /* Instance config dir*/
        strcat(DirectoryName,"/*_Mdf*.*"); /* only modified files*/
        WdogServe();
        xdelete(DirectoryName);
        UTILS_FindDirName(DIRECTORY_TEMPERATURE_CONFIG,InstanceNum,TRUE,DirectoryName); /* Instance config dir*/
        strcat(DirectoryName,"/*_*_Mdf*.*"); /* only modified files- */
        WdogServe();
        xdelete(DirectoryName);
    }

    SetConsolePrintToTempFile(FALSE, &PrevOutputFd, &fh);
    /*	ioGlobalStdSet (1, PrevOutputFd);
     fclose( fh );
     remove(FullLocalFileName);*/
#endif
}

STATUS CfgExcludeIdPrefix(char * IdText, char * IdWithoutPrefix)
{
    char * FindStr;
    if ((FindStr = strstr(IdText, "_INT_")) != NULL)
        strcpy(IdWithoutPrefix, &FindStr[5]); /* without the "_INT_"*/
    else if ((FindStr = strstr(IdText, "_STR_")) != NULL)
        strcpy(IdWithoutPrefix, &FindStr[5]); /* without the "_STR_"*/
    else
        return ERROR;

    return OK;

}
BOOL CfgTcpLock()
{
#if _OMEGA
    if (OMEGA_Enabled(CHAMBER_LEFT) || OMEGA_Enabled(CHAMBER_RIGHT))
    return(TRUE);
#endif
    return (FALSE);
}

STATUS UpdateRetValue(STATUS CurrentRetValue, STATUS NewRetValue)
{
    if (CurrentRetValue != OK)
        return CurrentRetValue;
    else
        return NewRetValue;

}

STATUS CfgDB_FindFilesPathAndNamesNew(int unitType, BOOL StrFile, BOOL DefFile, int InstanceNum, char *FileName,
        char *retFileName)
{
#if _LL_TO_DO_    
    int retValue;
    DIRECTORY_FILE_TYPE DirFileType;
    char Tmp[5];
    FileName[0] = '\0';
    retFileName[0] = '\0';

    if((retValue = CfgDB_FindFileNameAndDirectoryFileType(unitType, FileName, &DirFileType))!=OK)
    return retValue;

    if(DefFile)
    strcat(FileName,CFG_FILE_DEFAULT);/* add '_def' */
    else
    strcat(FileName,CFG_FILE_MODIFIED);/* add '_mdf' */

    if (StrFile)
    strcat(FileName,CFG_FILE_STR); /* add '_str' */
    else
    strcat(FileName,CFG_FILE_INT); /* add '_int' */

    strcat(FileName,(const char*)itoa(InstanceNum, Tmp,10));

    /* add the file extension */
    strcat(FileName,CFG_FILE_EXTENSION);
    /* add full path to file name*/

    UTILS_AddDirctoryToFileName(DirFileType,FileName, InstanceNum,retFileName,TRUE);
#endif    
    return OK;
}

/************** UNITEST SECTION ********************/
void test_timers()
{
    CfgTimeOutCreate(0, 100, 5, 10);
    CfgTimeOutCreate(0, 110, 5, 10);
    CfgTimeOutDelete(0, 100);
    CfgTimeOutDelete(0, 110);
    CfgTimeOutCreate(0, 100, 5, 10);
    CfgTimeOutCreate(0, 110, 5, 10);
    CfgTimeOutCreate(0, 100, 5, 10);
    CfgTimeOutCreate(0, 100, 5, 9);
}

#if _LL_TO_DO_  
STATUS CfgDB_FindFileNameAndDirectoryFileType(int unitType,char *FileName,DIRECTORY_FILE_TYPE *DirFileType)
{

    char Tmp[5];
    FileName[0] = '\0';

    switch (unitType)
    {
        case CfgDB_UnitGNRL:
        strcat(FileName,GNRL_FILE_NAME);
        *DirFileType=DIRECTORY_GNRL_CONFIG;
        break;

        case CfgDB_UnitTEMPERATURE:
        strcat(FileName,CHAMBER_FILE_NAME);
        *DirFileType=DIRECTORY_TEMPERATURE_CONFIG;
        break;

    }
    return OK;
}

#endif
#ifdef __cplusplus
 }
#endif
