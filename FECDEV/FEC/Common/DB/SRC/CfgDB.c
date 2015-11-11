/****************************************************************************
 *
 *           TITLE:   CfgDB
 *
 *			Service routines for the DB
 *
 *           PROJECT: MIDDLEWARE
 *
 *           AUTHORS: Yossi Sarusi
 *
 *           Creation date: 21/8/12
 *			Last Modified: 21/8/12
 *
 *****************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

 #include <string.h>
#include <stm32f2xx.h>
#include "ll_typedef.h"
#include <stdio.h>
#include <stdarg.h>
#include "free_rtos_exp.h"
#include "GnrlCfgDB.h"
#include "TemperatureCfgDB.h"
#include "OthersCfgDB.h"
#include "BtcCfgDB.h"
#include "CfgDB.h"
#include "rtc.h"
//#include "aux_term.h"
#include "vcp_term.h"

#include "CfgDB.h"
#include "Rtc.h"
#include "Eeprom_mc24xx1025.h"
#include "EepromDB_Def.h"
#ifdef CPP_FRAMEWORK
#include <logger.h>
#endif

STATUS CfgGetClientToInformFromIntDescriptor(int InstanceId, U32 id, CfgDB_INFORM_CLIENTS *ClientToInform);

static char PrintMsg[255];
static BOOL EnableLog = true; //false;//
static BOOL AutoDebug = true;
extern int DebugDisplayLevelMsgTh;

#if _OLD_LOGGER_   
void AUTO_DEBUG_MSG(int level,const char *format ,...)
{
    size_t str_m;
    va_list ap;
    int str_l_Tick, str_l;

    if (EnableLog)
    {

//		str_l_Tick=sprintf(PrintMsg,"%d: ",SysTickCounter); //add time         uint32_t rtc_show_time_date(char *tmp_buf, uint32_t prm)
        //  str_l_Tick=sprintf(PrintMsg,"%s.%d: ",rtc_show_time_date(tmp_buf, DATE_AND_TIME),SysTickCounter); //add time

        //str_l_Tick=sprintf(PrintMsg,"%d.%d: ",SysTickCounter,GetTimeStamp()*120); //add time
        uint32_t TimeStampMsec=xTaskGetTickCountFromISR();
        //uint32_t TimeStamp=GetTimeStamp();  //1/120 Usec resolution

        RTC_TimeTypeDef RTC_Time;
        rtc_get_time(&RTC_Time);
        str_l_Tick=sprintf(PrintMsg,"%d:%d:%d.%d ",
                RTC_Time.RTC_Hours,
                RTC_Time.RTC_Minutes,
                RTC_Time.RTC_Seconds,
                TimeStampMsec);//add time
        va_start(ap, format); /* measure the required size */
        str_l = vsnprintf(&PrintMsg[str_l_Tick],sizeof(PrintMsg)-str_l_Tick-1, format, ap); // add the message
        va_end(ap);

        str_l+=str_l_Tick;
        PrintMsg[str_l++]='\n';

        PrintMsg[str_l++]=0;
        if (level==M_LOGGER_LEVEL_TRACE)
        {

            AuxTermSendMsg(PrintMsg, str_l); //in FEC ATP print also to the AUX terminal

#ifndef PHANTOM_BOARD                  
            usb_term_tx( PrintMsg, str_l);

#endif                   

        }
        else
        if ((AutoDebug == TRUE) && (level>=DebugDisplayLevelMsgTh) /*&& (!IsPoolEmpty(1))*/)
        {
#ifndef PHANTOM_BOARD                  
            usb_term_tx( PrintMsg, str_l);
#else
            AuxTermSendMsg(PrintMsg, str_l);
#endif    
        }
    }

}
#endif

static xSemaphoreHandle DbCfgMutex[MAX_NUM_OF_INSTANCE + 1]; /* +1 for the GNRL*/

STATUS CfgInit()
{
    xSemaphoreHandle SemaphoreHand;

    for (int carrier = 0; carrier <= MAX_NUM_OF_INSTANCE; carrier++)
    {
        vSemaphoreCreateBinary(SemaphoreHand);
        DbCfgMutex[carrier] = SemaphoreHand;
    }
    return OK;
}

/*************************************************************************************************
 Main routine for setting data for any unit and any data type into the DB
 **************************************************************************************************/
STATUS CfgPutParam(int InstanceId, CfgDB_INT_DATA id, void * Data, CfgDB_SET_FLAG SetFlag)
{
    char DefaultFileName[100];
    char ModifiedFileName[100];
    CfgDB_ID ReqId;
    STATUS RetValue;
    char TmpStr[100];
    /*char TmpErrStr[50];*/

    ReqId.DataAssign = id; /* for convertion type only*/

    if ((RetValue = CfgDB_SetElement(InstanceId, ReqId, Data, SetFlag)) != OK)
    {
        /* in case were the CfgDB_SET_IGNORE_BOARD_EXISTENCE continue even if the board is not rdy*/
        if (!(((RetValue == CFG_DB_ERROR_ID_INSTANCE_NUM) || (RetValue == CFG_DB_ERROR_ID_UNIT_TYPE))
                && (SetFlag & CfgDB_SET_IGNORE_BOARD_EXISTENCE)))
        {
            if (!(SetFlag & CfgDB_SET_TEST_SANITY))
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                        "ERROR in CfgPutParam RetValue = %s, InstanceId=%d,Id = %s. Data=0x%x", Err2Str(RetValue),
                        InstanceId, Id2Str(id, TmpStr), *(int* )Data);
            return RetValue;
        }
    }

    if ((SetFlag & CfgDB_SET_TO_FILE) && !(SetFlag & CfgDB_SET_INFORM_REMOTE_ONLY_AND_IGNORED_REST_OF_THE_FLAG))
    {
#if 0
        if (ReqId.Field.AccessType==CfgDB_NON_VOLATILE)
        { /*update the file DB*/

            /* take the cfg sem*/
            if (ReqId.Field.UnitType==CfgDB_UnitGNRL)
            CfgDB_TakeCfgSem(INSTANCE_GNRL);
            else
            CfgDB_TakeCfgSem(InstanceId);

            /* Update the File DB*/
            if (ReqId.Field.DataType==CfgDB_INT)
            {
                uint32_t addr=EEPROM_DB_INT_ID_ADDR(InstanceId,ReqId.Field.UnitType,ReqId.Field.Id,ReqId.Field.Id_Index);
                set_int_val(addr, *(uint8_t*)Data);  //tbd need to be fixed ----> writeINT
            }
            else
            {
                uint32_t addr=EEPROM_DB_STR_ID_ADDR(InstanceId,ReqId.Field.UnitType,ReqId.Field.Id,ReqId.Field.Id_Index);
                set_str_val(addr, *(uint8_t*)Data);  //tbd need to be fixed --> writeSTR
            }

            /* give the cfg sem*/
            if (ReqId.Field.UnitType==CfgDB_UnitGNRL)
            CfgDB_GiveCfgSem(INSTANCE_GNRL,FALSE);
            else
            CfgDB_GiveCfgSem(InstanceId,FALSE);
        }
#endif                                  

        if (RetValue != OK)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR in CfgPutParam RetValue = %s, Id = %s. Data=0x%x",
                    Err2Str(RetValue), Id2Str(id, TmpStr), *(int* )Data);
            return RetValue;
        }

    }

    return OK;
}

#if 0

if ((SetFlag & CfgDB_SET_TO_FILE) && !(SetFlag & CfgDB_SET_INFORM_REMOTE_ONLY_AND_IGNORED_REST_OF_THE_FLAG))
{
    WdogServe(); // because writing to a file is a long process */
    if (ReqId.Field.AccessType==CfgDB_NON_VOLATILE)
    { /*update the modify file DB*/
        if ((RetValue=CfgDB_FindFilesPathAndNames(ReqId,DefaultFileName,ModifiedFileName,CarrierId))!=OK)
        {
            M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgPutParam RetValue = %s, Id = %s. Data=0x%x", Err2Str(RetValue), Id2Str(id, TmpStr), *(int*)Data);
            return RetValue;
        }

        /* take the cfg sem*/
        if ((ReqId.Field.UnitType==CfgDB_UnitIDC) || (ReqId.Field.UnitType==CfgDB_UnitAUX))
        CfgDB_TakeCfgSem(CARRIER_IDC);
        else
        CfgDB_TakeCfgSem(CarrierId);
        /* Update the File DB*/
        if (ReqId.Field.DataType==CfgDB_INT)
        RetValue=FileHandWriteIntParam(ModifiedFileName,ReqId, (CfgDB_INT_DATA*)Data);
        else
        RetValue=FileHandWriteStrParam(ModifiedFileName,ReqId, (char*)Data);

        /* if file not exists and we must write the element we need to create the file and then
         write the element*/
        if ((RetValue==FileHAND_ERROR_ID_FILE_NOT_EXIST) && (SetFlag & CfgDB_SET_IGNORE_BOARD_EXISTENCE))
        {
            FILE * fh;
            /* just create the file before update the element into it*/
            if( (fh = fopen( ModifiedFileName, "wb")) == NULL )
            { /* create it */
                return FileHAND_ERROR_ID_CREATE; /* can not create the file*/
            }
            fclose( fh );

            /* Update the File DB*/
            if (ReqId.Field.DataType==CfgDB_INT)
            RetValue=FileHandWriteIntParam(ModifiedFileName,ReqId, (CfgDB_INT_DATA*)Data);
            else
            RetValue=FileHandWriteStrParam(ModifiedFileName,ReqId, (char*)Data);
        }
        /* give the cfg sem*/
        if ((ReqId.Field.UnitType==CfgDB_UnitIDC) || (ReqId.Field.UnitType==CfgDB_UnitAUX))
        CfgDB_GiveCfgSem(CARRIER_IDC);
        else
        CfgDB_GiveCfgSem(CarrierId);
        /* update the last time configuration is modified*/
        IdcCfgDbUpdateLastConfigChangeTime();
    }

    if (RetValue!=OK)
    {
        M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgPutParam RetValue = %s, Id = %s. Data=0x%x", Err2Str(RetValue), Id2Str(id, TmpStr), *(int*)Data);
        return RetValue;
    }
#endif
//extern int IpTaskId;
/*************************************************************************************************
 Main routine for getting data for any unit and any data type from the DB
 **************************************************************************************************/
STATUS CfgGetParam(int InstanceId, CfgDB_INT_DATA id, void * Data, int DataLen, CfgDB_GET_FLAG GetFlag)
{
    char DefaultFileName[100];
    char ModifiedFileName[100];
    CfgDB_ID ReqId;
    STATUS RetValue;
    /*char TmpStr[100];*/
    /*char TmpErrStr[50];*/
    char IdText[80];

    ReqId.DataAssign = id;

    if (GetFlag & CfgDB_GET_DIRECTLY_FROM_RAM)
    { /* only if descriptor is ready*/

        if (CfgIdTextFromDescriptor(InstanceId, id, IdText, sizeof(IdText)) != OK)
        {
#if _LL_TO_DO_                   
            if ((taskIdSelf()!=IpTaskId) /*&& !CHAMBER_ChamberExist(InstanceId)*/)
            M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgGetParam in CfgIdTextFromDescriptor InstanceId=%d Id = 0x%x, GetFlag=0x%x t=0",
                    InstanceId,id,GetFlag);
#endif
        }
    }
    else
        sprintf(IdText, "0x%x", id);

    if ((GetFlag & CfgDB_GET_DIRECTLY_FROM_DEFAULT_FILE) || (GetFlag & CfgDB_GET_DIRECTLY_FROM_MODIFIED_FILE))
    {
        if (ReqId.Field.AccessType != CfgDB_NON_VOLATILE) /*only configuration has file */
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR in CfgGetParam InstanceId=%d Id = %s, GetFlag=0x%x t=1",
                    InstanceId, IdText, GetFlag);
            return CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG;
        }

        if (!(GetFlag & CfgDB_GET_IGNORE_BOARD_EXISTENCE))
        {
            /*I uses the following call to check for leggal access ( ID, Id_index , etc.*/
            if ((RetValue = CfgDB_GetElement(InstanceId, ReqId, Data, DataLen, CfgDB_SET_TEST_SANITY)) != OK)
            {
#if _LL_TO_DO_                            
                /* if the request id from the SNMP, and the DB is not ready don't display the error message*/
                if ((taskIdSelf()==IpTaskId) /*&& !CHAMBER_ChamberExist(InstanceId)*/)
                return RetValue;
                M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgGetParam InstanceId=%d Id = %s, GetFlag=0x%x t=2",
                        InstanceId,IdText,GetFlag);
#endif                                

                return RetValue;
            }
        }

        if ((RetValue = CfgDB_FindFilesPathAndNames(ReqId, DefaultFileName, ModifiedFileName, InstanceId)) != OK)
        {
#if _LL_TO_DO_                    
            /* if the request id from the SNMP, and the DB is not ready don't display the error message*/
            if ((taskIdSelf()==IpTaskId) /*&& !CHAMBER_ChamberExist(InstanceId)*/)
            return RetValue;

            M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgGetParam InstanceId=%d Id = %s, GetFlag=0x%x t=3",
                    InstanceId,IdText,GetFlag);
#endif                        

            return RetValue;
        }

        /* get the data from the file DB*/
        if (ReqId.Field.DataType == CfgDB_INT)
        {
#if _LL_TO_DO_                   
            if (GetFlag & CfgDB_GET_DIRECTLY_FROM_MODIFIED_FILE)
            if (FileHandReadIntParam(ModifiedFileName,ReqId, (CfgDB_INT_DATA*)Data)==OK)
            return OK;
            if (GetFlag & CfgDB_GET_DIRECTLY_FROM_DEFAULT_FILE)
            return (FileHandReadIntParam(DefaultFileName,ReqId, (CfgDB_INT_DATA*)Data));
            else
            return ERROR;

        }
        else
        {
            if (GetFlag & CfgDB_GET_DIRECTLY_FROM_MODIFIED_FILE)
            if (FileHandReadStrParam(ModifiedFileName,ReqId, (char*)Data,DataLen)==OK)
            return OK;
            if (GetFlag & CfgDB_GET_DIRECTLY_FROM_DEFAULT_FILE)
            return (FileHandReadStrParam(DefaultFileName,ReqId, (char*)Data,DataLen));
            else
            return ERROR;
#endif                         
        }
        return RetValue;

    }
    else if ((GetFlag & CfgDB_GET_DIRECTLY_FROM_RAM) || (GetFlag & CfgDB_GET_DIRECTLY_FROM_SOURCE)
            || (GetFlag & CfgDB_GET_WITH_CALLING_TO_HANDLE))
    {
        /*check for leggal access and return the data from the RAM DB or from the source*/
        if ((RetValue = CfgDB_GetElement(InstanceId, ReqId, Data, DataLen, GetFlag)) != OK)
        {
#if _LL_TO_DO_                           
            /* if the request id from the SNMP, and the DB is not ready don't display the error message*/
            if ((taskIdSelf()==IpTaskId) /*&& !CHAMBER_ChamberExist(InstanceId)*/)
            return RetValue;
            M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgGetParam InstanceId=%d Id = %s (0x%x), GetFlag=0x%x RetValue=%s t=4",
                    InstanceId,IdText,id,GetFlag,Err2Str(RetValue));
#endif                                

        }
        return RetValue;

    }
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR in CfgGetParam InstanceId=%d Id = %s, GetFlag=0x%x t=5", InstanceId,
                IdText, GetFlag);
        return CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG;
    }

}

/* check if data value is equal to parameter value at the database */
STATUS CfgCheckParam(int InstanceId, CfgDB_INT_DATA id, void * Data, CfgDB_SET_FLAG SetFlag)
{
    CfgDB_ID Id;

    Id.DataAssign = id;
    if (Id.Field.DataType == CfgDB_INT)
    {
        unsigned int LocalValue;
        unsigned int MateValue;

        if (CfgGetParam(InstanceId, id, &LocalValue, sizeof(unsigned int), CfgDB_GET_FROM_DB) != OK)
            return ERROR;
        MateValue = *(int *) Data;
        if (LocalValue != MateValue)
        {
            char IdText[80];
            char IdTextWithoutPrefix[70];
            char ExtraText[150];
            //int AlarmId;
            CfgDB_INFORM_CLIENTS ClientToInform;

            CfgIdTextFromDescriptor(InstanceId, id, IdText, sizeof(IdText));
            CfgExcludeIdPrefix(IdText, IdTextWithoutPrefix);
            CfgGetClientToInformFromIntDescriptor(InstanceId, id, &ClientToInform);
#if _LL_TO_DO_
            if ((ClientToInform & INFORM_NOT_SHOW_MISMATCH_VALUE) == 0)
            sprintf(ExtraText,"Mate Configuration Mismatch - (%s) M=0x%x, L=0x%x", IdTextWithoutPrefix,MateValue,LocalValue);
            else
            sprintf(ExtraText,"Mate Configuration Mismatch - (%s)", IdTextWithoutPrefix);

            if ( (Id.Field.UnitType == CfgDB_UnitGNRL) || (Id.Field.UnitType == CfgDB_UnitAUX) )
            {

                ALARM_Set(NULL, ID_GNRL_ALARM_MATE_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                AlarmId=ID_GNRL_ALARM_EVENT_MATE_CONFIGURATION_MISMATCH;

            }
            else
            {
                ALARM_Set(InstanceId, ID_CARR_ALARM_MATE_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                AlarmId=ID_CARR_ALARM_EVENT_MATE_CONFIGURATION_MISMATCH;
            }

            ALARM_ExpandSet(InstanceId, AlarmId, ExtraText,TRUE,ALARM_SET_FLAG_CHECK_ALL);
#endif                         
        }
    }
    else
    {
        char LocalValue[255];
        char *MateValue;
        if (CfgGetParam(InstanceId, id, LocalValue, sizeof(LocalValue), CfgDB_GET_FROM_DB) != OK)
            return ERROR;
        MateValue = (char *) Data;
        if (strcmp(LocalValue, MateValue) != 0)
        {
            char ExtraText[150];
            char IdText[80];
            char IdTextWithoutPrefix[70];
            int AlarmId;
            CfgDB_INFORM_CLIENTS ClientToInform;

            CfgIdTextFromDescriptor(InstanceId, id, IdText, sizeof(IdText));
            CfgExcludeIdPrefix(IdText, IdTextWithoutPrefix);
            CfgGetClientToInformFromIntDescriptor(InstanceId, id, &ClientToInform);
#if _LL_TO_DO_
            if ((ClientToInform & INFORM_NOT_SHOW_MISMATCH_VALUE) == 0)
            sprintf(ExtraText,"Mate Configuration Mismatch - (%s) M=%s, L=%s", IdTextWithoutPrefix,MateValue,LocalValue);
            else
            sprintf(ExtraText,"Mate Configuration Mismatch - (%s)", IdTextWithoutPrefix);

            if ( (Id.Field.UnitType == CfgDB_UnitGNRL) || (Id.Field.UnitType == CfgDB_UnitAUX))
            {
                ALARM_Set(NULL, ID_GNRL_ALARM_MATE_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                AlarmId=ID_GNRL_ALARM_EVENT_MATE_CONFIGURATION_MISMATCH;
            }
            else
            {
                ALARM_Set(InstanceId, ID_CARR_ALARM_MATE_CONFIGURATION_MISMATCH, TRUE,ALARM_SET_FLAG_CHECK_ALL |ALARM_SET_FLAG_TEST_BEFORE_SEND);
                AlarmId=ID_CARR_ALARM_EVENT_MATE_CONFIGURATION_MISMATCH;
            }

            ALARM_ExpandSet(InstanceId, AlarmId,ExtraText, TRUE,ALARM_SET_FLAG_CHECK_ALL);
#endif                        
        }
    }
    return OK;
}

int CfgRetInt(int InstanceId, CfgDB_INT_DATA id)
{
    int Data;
    if (CfgGetParam(InstanceId, id, &Data, sizeof(int), CfgDB_GET_FROM_DB) != OK)
    {
        Data = 0;
    }
    return (Data);
}

int CfgRetGnrlInt(CfgDB_INT_DATA id)
{
    int Data;
    if (CfgGetParam(NULL, id, &Data, sizeof(int), CfgDB_GET_FROM_DB) != OK)
    {
        return 0;
    }
    return (Data);
}

STATUS CfgTestInt(int InstanceId, CfgDB_INT_DATA id, int Data)
{

    return CfgPutParam(InstanceId, id, &Data, CfgDB_TEST_SANITY);
}

STATUS CfgGetGnrlInt(CfgDB_INT_DATA id, int * Data)
{
    return (CfgGetParam(0, id, Data, sizeof(int), CfgDB_GET_FROM_DB));
}

STATUS CfgShowFile(int InstanceId, CfgDB_UNIT_TYPE UnitType, BOOL IntType, BOOL DefaultFile)
{
#if _LL_TO_DO_   
    CfgDB_ID id;
    char DefaultFileName[100];
    char ModifiedFileName[100];
    STATUS RetValue;

    id.Field.UnitType=UnitType;
    id.Field.AccessType=CfgDB_NON_VOLATILE;
    id.Field.DataType=IntType==0?CfgDB_STR:CfgDB_INT;

    if ((RetValue=CfgDB_FindFilesPathAndNames(id,DefaultFileName,ModifiedFileName,InstanceId))!=OK)
    return RetValue;

    if (DefaultFile)
    return (FileHandShowParam(DefaultFileName,IntType));
    else
    return (FileHandShowParam(ModifiedFileName,IntType));
#endif
    return 0;

}

/* Copy all configuration DB from local DB to the mate:
 Internal protection - from InstanceId Db to the oher carrier DB.
 External protection - from this IDU DB to the mate IDU
 */

static CfgDB_ID_IntDescriptor * FindIntDescriptor(int InstanceId, CfgDB_UNIT_TYPE UnitType,
        CfgDB_ACCESS_TYPE AccessType, int * NumOfElements)
{

    switch (UnitType)
    {
    case CfgDB_UnitGNRL:
        return GnrlCfg_DbIntDescriptor(AccessType, NumOfElements);

    case CfgDB_UnitTEMPERATURE:
        return TemperatureCfg_DbIntDescriptor(AccessType, NumOfElements);
    case CfgDB_UnitOTHERS:
        return OthersCfg_DbIntDescriptor(AccessType, NumOfElements);
    case CfgDB_UnitBTC:
        return BtcCfg_DbIntDescriptor(AccessType, NumOfElements);

    default:
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR in FindIntDescriptor Illegal UnitType=%d, InstanceId=%d ", UnitType,
                InstanceId);
        return NULL;
    }
}

static CfgDB_ID_StrDescriptor * FindStrDescriptor(int InstanceId, CfgDB_UNIT_TYPE UnitType,
        CfgDB_ACCESS_TYPE AccessType, int *NumOfElements)
{

    switch (UnitType)
    {
    case CfgDB_UnitGNRL:
        return GnrlCfg_DbStrDescriptor(AccessType, NumOfElements);

    case CfgDB_UnitTEMPERATURE:
        return TemperatureCfg_DbStrDescriptor(AccessType, NumOfElements);
    case CfgDB_UnitOTHERS:
        return OthersCfg_DbStrDescriptor(AccessType, NumOfElements);
    case CfgDB_UnitBTC:
        return OthersCfg_DbStrDescriptor(AccessType, NumOfElements);

    default:
        return NULL;
    }
}

STATUS CfgGetMinMaxLimitValueFromIntDescriptor(int InstanceId, U32 id, int * MinLimitValue, int * MaxLimitValue)
{
    U32 NumOfElements;
    CfgDB_ID Id;
    CfgDB_ID_IntDescriptor * IntDescriptor;
    Id.DataAssign = id;

    IntDescriptor = FindIntDescriptor(InstanceId, Id.Field.UnitType, Id.Field.AccessType, &NumOfElements);
    if (IntDescriptor == NULL)
        return ERROR;

    if (Id.Field.Id >= NumOfElements)
        return ERROR;

    return (CfgDB_GetMinMaxLimitValueFromIntDescriptor(id, IntDescriptor, MinLimitValue, MaxLimitValue));
}

STATUS CfgGetDefaultValueFromIntDescriptor(int InstanceId, U32 id, int * DefaultValue)
{
    int NumOfElements;
    CfgDB_ID Id;
    CfgDB_ID_IntDescriptor * IntDescriptor;
    Id.DataAssign = id;

    IntDescriptor = FindIntDescriptor(InstanceId, Id.Field.UnitType, Id.Field.AccessType, &NumOfElements);
    if (IntDescriptor == NULL)
        return ERROR;

    if (Id.Field.Id >= NumOfElements)
        return ERROR;

    return (CfgDB_GetDefaultValueFromIntDescriptor(id, IntDescriptor, DefaultValue));
}

STATUS CfgGetClientToInformFromIntDescriptor(int InstanceId, U32 id, CfgDB_INFORM_CLIENTS *ClientToInform)
{
    CfgDB_ID Id;
    int NumOfElements;
    CfgDB_ID_IntDescriptor * IntDescriptor;
    CfgDB_ID_StrDescriptor * StrDescriptor;
    Id.DataAssign = id;

    if (Id.Field.DataType == CfgDB_INT)
    {
        IntDescriptor = FindIntDescriptor(InstanceId, Id.Field.UnitType, Id.Field.AccessType, &NumOfElements);
        if (IntDescriptor == NULL)
            return ERROR;

        *ClientToInform = IntDescriptor[Id.Field.Id].ClientWhichNeedToBeInform;
    }
    else
    {
        StrDescriptor = FindStrDescriptor(InstanceId, Id.Field.UnitType, Id.Field.AccessType, &NumOfElements);
        if (StrDescriptor == NULL)
            return ERROR;

        *ClientToInform = StrDescriptor[Id.Field.Id].ClientWhichNeedToBeInform;
    }

    return (OK);
}

STATUS CfgIdTextFromDescriptor(int InstanceId, U32 id, char *IdText, int MaxTextLen)
{
    CfgDB_ID Id;
    int NumOfElements;
    CfgDB_ID_IntDescriptor * IntDescriptor;
    CfgDB_ID_StrDescriptor * StrDescriptor;
    Id.DataAssign = id;

    if (Id.Field.DataType == CfgDB_INT)
    {
        IntDescriptor = FindIntDescriptor(InstanceId, Id.Field.UnitType, Id.Field.AccessType, &NumOfElements);
        if (IntDescriptor == NULL)
        {
#if _LL_TO_DO_                     
            if ((taskIdSelf()!=IpTaskId))
            M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgIdTextFromDescriptor in FindIntDescriptor. InstanceId=%d,Id = 0x%x.", InstanceId,Id.DataAssign);
#endif                        
            sprintf(IdText, "0x%x", id);
            return ERROR;
        }
        if (Id.Field.Id >= NumOfElements)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,
                    "ERROR in CfgIdTextFromDescriptor id greater than allowed. InstanceId=%d,Id = 0x%x, NumOfElements=%d",
                    InstanceId, Id.DataAssign, NumOfElements);
            sprintf(IdText, "0x%x", id);
            return ERROR;
        }

        strncpy(IdText, IntDescriptor[Id.Field.Id].IdText, MaxTextLen);

        if (Id.Field.Id_Index != 0)
        {
            char IndexText[10];
            sprintf(IndexText, "_%d", Id.Field.Id_Index);
            strcat(IdText, IndexText);
        }

    }
    else
    {
        StrDescriptor = FindStrDescriptor(InstanceId, Id.Field.UnitType, Id.Field.AccessType, &NumOfElements);
        if (StrDescriptor == NULL)
        {
#if _LL_TO_DO_                     
            if ((taskIdSelf()!=IpTaskId))
            M_LOGGER_LOGF (M_LOGGER_LEVEL_ERROR,"ERROR in CfgIdTextFromDescriptor in FindStrDescriptor. InstanceId=%d,Id = 0x%x.", InstanceId,Id.DataAssign);
#endif                        
            sprintf(IdText, "0x%x", id);
            return ERROR;
        }

        if (Id.Field.Id >= NumOfElements)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,
                    "ERROR in CfgIdTextFromDescriptor id greater than allowed. InstanceId=%d,Id = 0x%x, NumOfElements=%d",
                    InstanceId, Id.DataAssign, NumOfElements);
            sprintf(IdText, "0x%x", Id.DataAssign);
            return ERROR;
        }
        strncpy(IdText, StrDescriptor[Id.Field.Id].IdText, MaxTextLen);

        if (Id.Field.Id_Index != 0)
        {
            char IndexText[10];
            sprintf(IndexText, "_%d", Id.Field.Id_Index);
            strcat(IdText, IndexText);
        }
    }

    return (OK);
}

STATUS CfgDB_TakeCfgSem(int InstanceId)
{

    STATUS RetValue;
    xSemaphoreHandle Sem;

    if (InstanceId == INSTANCE_GNRL)
        Sem = DbCfgMutex[MAX_NUM_OF_INSTANCE];
    else
        Sem = DbCfgMutex[InstanceId];

    if ((RetValue = xSemaphoreTake(Sem,3000)) == pdTRUE) /* up to 3 second wait*/
        return OK;
    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "ERROR in CfgDB_WaitForCfgSem, in semTake,InstanceId=%d", InstanceId);
        CfgDB_GiveCfgSem(InstanceId, FALSE);
        return ERROR;
    }

    return OK;

}

STATUS CfgDB_GiveCfgSem(int InstanceId, bool FromIsr)
{

    xSemaphoreHandle Sem;

    if (InstanceId == INSTANCE_GNRL)
        Sem = DbCfgMutex[MAX_NUM_OF_INSTANCE];
    else
        Sem = DbCfgMutex[InstanceId];

    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (FromIsr)
    {
        xSemaphoreGiveFromISR(Sem, &xHigherPriorityTaskWoken);
    }
    else
    {
        xSemaphoreGive(Sem);
    }

    /* Switch tasks if necessary. */
    if (xHigherPriorityTaskWoken != pdFALSE)
    {
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }

    return OK;

}

char UnitStrCnv[CfgDB_UnitLast][12] =
{ "GNRL", "TEMPERATURE", "OTHERS" };

CfgDB_UNIT_TYPE Str2Unit(char * UnitStr)
{
    int Unit;
    //the part of unit in the ID string

    for (Unit = 0; Unit < CfgDB_UnitLast; Unit++)
    {
        if (!strcmp(UnitStr, UnitStrCnv[Unit]))
            //identical
            return Unit;
    }

    return ERROR;
}

char AccessStrCnv[4][10] =
{ "CFG", "CMND", "MNTR", "VOLATILE" };
CfgDB_ACCESS_TYPE Str2Access(char * AccessStr)
{
    int Access;
    //the part of unit in the ID string

    for (Access = 0; Access <= CfgDB_VOLATILE_STR; Access++)
    {
        if (!strcmp(AccessStr, AccessStrCnv[Access]))
            //identical
            return Access;
    }

    return ERROR;
}

STATUS IdStrElements(char* IdStr, CfgDB_UNIT_TYPE * UnitType, CfgDB_DATA_TYPE * DataType,
        CfgDB_ACCESS_TYPE * AccessType)
{
    char * Index1, *Index2;
    char UnitStr[15];
    char AccessStr[15];
    char DataTypeStr[7];
    int NumOfElement;

    //DecomposeString(IdStr,'_', &ElementStr, &NumOfElement);

    //start with the Unit type
    Index1 = strstr(IdStr, "_") + 1; //locate the first "_"
    Index2 = strstr(Index1, "_"); //locate the second "_"
    memcpy(UnitStr, Index1, Index2 - Index1);
    UnitStr[Index2 - Index1] = 0;
#if _LL_TO_DO_ 
    //in case of the MUX add the MUX type interface
    if (!strcmp(UnitStr,"MUX"))
    { //identical to "MUX"
        *UnitType=Str2Unit(UnitStr);//default unit MUX
        Index1=Index2+1;
        Index2=strstr(Index2+1,"_");//locate the next "_"
        memcpy(AccessStr,Index1,Index2-Index1);
        UnitStr[Index2-Index1]=0;

        *AccessType=Str2Access(AccessStr);//first make sure that it's not access part e.g: MUX_CFG_
        if (*AccessType==ERROR)
        { //it's not access. check for unit type e.g: MUX_REG_
            strcat(UnitStr,"_");
            strcat(UnitStr,AccessStr);
            *UnitType=Str2Unit(UnitStr);
            if (*UnitType==ERROR)
            { //if its also not part of the UNIT return ERROR
                return ERROR;
            }

            //now the access type
            Index1=strstr(Index2+1,"_");//locate the third "_"
            memcpy(AccessStr,Index2+1,Index1-Index2-1);
            AccessStr[Index1-Index2-1]=0;
            *AccessType=Str2Access(AccessStr);
            if (*AccessType==ERROR)
            return ERROR;
        }
        else
        Index1=Index2;
    }
    else
#endif
    { //it's not MUX
        *UnitType = Str2Unit(UnitStr);
        if (*UnitType == ERROR)
        { //if its not part of the UNIT it can
            return ERROR;
        }

        //now the access type
        Index1 = strstr(Index2 + 1, "_"); //locate the third "_"
        memcpy(AccessStr, Index2 + 1, Index1 - Index2 - 1);
        AccessStr[Index1 - Index2 - 1] = 0;
        *AccessType = Str2Access(AccessStr);
        if (*AccessType == ERROR)
            return ERROR;
    }

    //and finally the data type
    Index2 = strstr(Index1 + 1, "_"); //locate the forth "_"
    memcpy(DataTypeStr, Index1 + 1, Index2 - Index1 - 1);
    DataTypeStr[Index2 - Index1 - 1] = 0;
    if (!strcmp(DataTypeStr, "INT"))
        *DataType = CfgDB_INT;
    else if (!strcmp(DataTypeStr, "STR"))
        *DataType = CfgDB_STR;
    else if (!strcmp(DataTypeStr, "VOLATILE")) //Volatile STR - bug in the ODU
    {
        *DataType = CfgDB_STR;
        *AccessType = CfgDB_VOLATILE_STR;
    }
    else
        return ERROR;

    return OK;

}

int CfgDB_ID_Str2Id(int InstanceId, char *IdStr)
{
    CfgDB_UNIT_TYPE UnitType;
    CfgDB_DATA_TYPE DataType;
    CfgDB_ACCESS_TYPE AccessType;
    int NumOfElements, Id, RetId;
    CfgDB_ID_IntDescriptor * IntDescriptor;
    CfgDB_ID_StrDescriptor * StrDescriptor;

    if (IdStrElements(IdStr, &UnitType, &DataType, &AccessType) == ERROR)
        return ERROR;

    if (DataType == CfgDB_INT)
    {
        IntDescriptor = FindIntDescriptor(InstanceId, UnitType, AccessType, &NumOfElements);
        if (IntDescriptor == NULL)
            return ERROR;

        for (Id = 0; Id < NumOfElements; Id++)
        {
            if (!strcmp(IdStr, IntDescriptor->IdText))
            {
                //identical
                RetId = CfgDB_ID_FirstId(UnitType,AccessType,DataType);
                return RetId + Id;

            }
            IntDescriptor++;

        }
    }
    else
    {
        StrDescriptor = FindStrDescriptor(InstanceId, UnitType, AccessType, &NumOfElements);
        if (StrDescriptor == NULL)
            return ERROR;

        for (Id = 0; Id < NumOfElements; Id++)
        {
            if (!strcmp(IdStr, StrDescriptor->IdText))
            {
                //identical
                RetId = CfgDB_ID_FirstId(UnitType,AccessType,DataType);
                return RetId + Id;

            }
            StrDescriptor++;

        }
    }

    return ERROR;

}

/*The following routines enable to set and get DB element, by passing the ID as Text value*/
STATUS CfgPutParamText(int InstanceId, char * IdStr, void * Data, CfgDB_SET_FLAG SetFlag)
{
    return (CfgPutParam(InstanceId, CfgDB_ID_Str2Id(InstanceId, IdStr), Data, SetFlag));
}

STATUS CfgGetParamText(int InstanceId, char * IdStr, void * Data, int DataLen, CfgDB_GET_FLAG GetFlag)
{
    return (CfgGetParam(InstanceId, CfgDB_ID_Str2Id(InstanceId, IdStr), Data, DataLen, GetFlag));
}

int CfgRetIntText(int InstanceId, char * IdStr)
{
    int Data;
    if (CfgGetParam(InstanceId, CfgDB_ID_Str2Id(InstanceId, IdStr), &Data, sizeof(int), CfgDB_GET_FROM_DB) != OK)
    {
        Data = 0;
    }
    return (Data);
}

#ifdef __cplusplus
}
#endif
