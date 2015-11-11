/****************************************************************************
*
*           TITLE:   LOGSYS
*
*			Service routines for the Trace system log
*
*           PROJECT: MIDDLEWARE
*
*           AUTHORS: Yossi Sarusi
*
*           Creation date: 28/9/12
*			Last Modified: 28/9/12
*
*****************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
#include "free_rtos_exp.h"
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif
#include "ll_typedef.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LogSys.h"
#include "io.h"
#ifdef CPP_FRAMEWORK
#include <logger.h>
#endif


#define min(a,b)  (((a) < (b)) ? (a) : (b))


 
LOG_SYS_LOG_DB_STRUCT_PTR SysLogPtr=NULL;
char KEY_WORD_FOR_THE_SYS_LOG[KEY_WORD_FOR_THE_SYS_LOG_LEN]="LANDA12";
//SEM_ID LogSysSem;
bool SysLogBusy=FALSE;
bool EnableLogOsSys;
static bool DisplayTraceLog=false;
int Entry;
xSemaphoreHandle xLogSysBinarySemaphore=NULL; 


//UINT8 SysLogbuf[LOG_SYS_SIZE_OF_RESERVED_MEM] __attribute__((section(".noinit")));
//static UINT8 SysLogbuf[LOG_SYS_SIZE_OF_RESERVED_MEM] @".noinit";

#if ( configUSE_TRACE_FACILITY == 1 )
//#define PC_SIZE_OF_RESERVED_MEM 300 //num of reserved bytes for the pc sys

//UINT8 PcLogbuf[PC_SIZE_OF_RESERVED_MEM] __attribute__((section(".noinit")));

#if 0
static UINT8 PcLogbuf[PC_SIZE_OF_RESERVED_MEM] @".noinit";
char pcWriteBuffer[100];

char * PcLogBufPtr()
{
	return PcLogbuf;
}


char * pcWriteBufferPtr()
{
	return pcWriteBuffer;
}



#endif 

#endif

static char TestLogBuffer[4096] @".noinit";
char * SysLogBufPtr()
{
        return (char*)TestLogBuffer;
	// tbd return (char*)(RCC_AHB1Periph_BKPSRAM);
}

#if 0
char * sysMemTop()
{
	return (char*)(0x4000-LOG_SYS_SIZE_OF_RESERVED_MEM); //last LOG_SYS_SIZE_OF_RESERVED_MEM bytes of the 8K available SRAM
}
#endif
//return true if during interrupt context, otherwise return false
bool intContext(U32 * IsrNum)
{
        *IsrNum=arm_get_ipsr () & 0x1ff; //bits 0-8
        return (*IsrNum!=0);
}

bool TraceDisplayTraceLog()
{
  if (DisplayTraceLog)
  {
    DisplayTraceLog=false;
   return true;
  }
  else return false;
}
/*
char * TaskNameSelf() 
{
	return ( tskTCB * )pxCurrentTCB->pcTaskName;
	//return prvGetTCBFromHandle(xTaskGetCurrentTaskHandle())->pcTaskName;
}*/

bool GetWatchdogReset(bool *WdReset)
{
	*WdReset=false;
	return LL_OK;
}


LL_STATUS LOG_SYS_Init()
{
/*    dwv_RST_SRC ResetSource;
    ResetSource=RstSrcGet();

*/

	SysLogBusy=false;
	EnableLogOsSys=false;

#ifdef _INCLUDE_OS_
	vSemaphoreCreateBinary( xLogSysBinarySemaphore ); 
#endif
    SysLogPtr=(LOG_SYS_LOG_DB_STRUCT_PTR)SysLogBufPtr(); /* Where the USER_RESERVED_MEM is start */

    //printf_P(PSTR("start Syslog init...SysLogPtr=0x%x\n"),(UINT16)SysLogPtr);

    if (strcmp(SysLogPtr->KeyForInitialization,KEY_WORD_FOR_THE_SYS_LOG) ||
        (SysLogPtr->LogHeader.LogEntryCounter>=LOG_MAX_NUM_OF_SYSLOG_ENTRIES)) 
    /*if (ResetSource==dwv_EXT_HARD_RST)*/ /* if power on reset*/
    {
        //printf_P(PSTR("SysLog start initialization SysLogPtr=0x%x!!!\n"),(UINT16)SysLogPtr);
        SysLogPtr->LogHeader.LogEntryCounter=0;
        SysLogPtr->LogHeader.NextWriteIndex=0;
//        SysLogPtr->LogUnitStruct.NumberOfEntries=0;
        /*SysLogPtr->LogUnitStruct.Mutex=NULL;*/
       // SysLogPtr->LogUnitStruct.LogDB=(UINT8 *)&SysLogPtr->DB;

		for (Entry=0;Entry<LOG_MAX_NUM_OF_SYSLOG_ENTRIES;Entry++)
		{
			SysLogPtr->LogData[Entry].TaskData.Time.Hour=0; /* default empty entry*/
			SysLogPtr->LogData[Entry].TaskData.Time.TimeStamp=0; /* default empty entry*/
		}

        strcpy(SysLogPtr->KeyForInitialization,KEY_WORD_FOR_THE_SYS_LOG); /* write the keyword*/
        //printf_P(PSTR("SysLog is initialized!!!\n"));
		/*init the false reset counter*/

		{ /* Count the abnormal events were the log is cleared in warm powered up (SW reset or WD reset)*/
			bool WdReset;
			if (GetWatchdogReset(&WdReset)==LL_OK)
			{
				if (WdReset)
				{
					int FalseResetCounter;
					if (LOG_GetFalseResetCounter(&FalseResetCounter)==LL_OK)
					{
						FalseResetCounter++;
						LOG_SetFalseResetCounter(FalseResetCounter);
					} else
                                        {
                                          AuxPrintStrMsg("ERROR in LOG_SYS_Init in LOG_GetFalseResetCounter\n");
                                         
                                       
                                        }
				}
				else /* cold reset*/
					LOG_SetFalseResetCounter(0); /* restart the counter*/

			} else 
                        {
                          AuxPrintStrMsg("ERROR in LOG_SYS_Init in IDC_EEPROM_GetIdcWatchdogReset\n");
                          
                        }
                        
		}

    }
   // else
     //   printf_P(PSTR("sys log keyword was found.  NumberOfEntries=%d, NextWriteEntryIndex=%d\n"),
       //     SysLogPtr->LogUnitStruct.NumberOfEntries,SysLogPtr->LogUnitStruct.NextWriteEntryIndex);

 	//EnableLogOsSys=true;
    return LL_OK;
}


void LOG_SYS_Terminated()
{
    EnableLogOsSys=FALSE;
    strcpy(SysLogPtr->KeyForInitialization,"DUMMY"); /* replace the keyworkd to force initialization*/
/*	semDelete(LogSysSem);*/
    LOG_SYS_Init();
	LOG_SetFalseLogInitCounter(0); /* restart the counter*/
}


void StartSysLog()
{
  EnableLogOsSys=true;
}

void StopSysLog()
{
  EnableLogOsSys=false;
 // DisplayTraceLog=true;
}
void PrintSyslogData()
{
  EnableLogOsSys=false;
  DisplayTraceLog=true;  //print the log entries into the terminal screen
}

/* the sysLog is not using the log task for improving the performance*/
LL_STATUS LOG_AddSysLogMessageTask(UINT8 TaskId, char *TaskName)
{
    LOG_SYS_DATA_ENTRY_STRUCT * DestEntryPtr;
    LOG_SYS_TASK_DATA LogEntry;
   // char text[250];
    int WriteIndex;
    int EntryCounter;
    UINT32 IsrNum;
    
    static unsigned portBASE_TYPE CurrentTaskId=0; 

        if (!EnableLogOsSys)
		return LL_OK;
#ifdef _INCLUDE_OS_
	xSemaphoreTake( xLogSysBinarySemaphore, portMAX_DELAY ); 
#endif

        
        
	if (SysLogPtr==NULL) /* syslog not initialized*/
	{
          
		//AuxPrintStrMsg(" ERROR in LOG_AddSysLogMessage, SysLog not initialized...\n");
		return LL_ERROR;
	}

       if (SysLogBusy)
		return LL_ERROR;
#if ( configUSE_TRACE_FACILITY == 1 )
         if (CurrentTaskId==TaskIdSelf())
               return LL_OK; //don't log if already log this task
              
        CurrentTaskId=TaskIdSelf(); //keep it
#endif        
	SysLogBusy=true;

        
        EntryCounter=SysLogPtr->LogHeader.LogEntryCounter;
        
	/*cyclic index increment*/
	WriteIndex=SysLogPtr->LogHeader.NextWriteIndex;
        DestEntryPtr=&SysLogPtr->LogData[WriteIndex];
        
        uint32_t TimeStampUsec=(xTaskGetTickCountFromISR()%1000)*1000+GetTimeStamp()/120; //convert it to usec
        
        RTC_TimeTypeDef  RTCT_Time;
        rtc_get_time(&RTCT_Time);
          
        DestEntryPtr->TaskData.Time.Hour=RTCT_Time.RTC_Hours;
        DestEntryPtr->TaskData.Time.Minute=RTCT_Time.RTC_Minutes;
        DestEntryPtr->TaskData.Time.Seconds=RTCT_Time.RTC_Seconds;
        DestEntryPtr->TaskData.Time.TimeStamp=TimeStampUsec;
        

	//DestEntryPtr=(LOG_SYS_ENTRY_STRUCT*)&SysLogPtr->LogUnitStruct.LogDB[WriteIndex*sizeof(LOG_SYS_ENTRY_STRUCT)];

	//tbd get from igor TIME_Get( &DestEntryPtr->TimeStamp.Minutes, &DestEntryPtr->TimeStamp.MiliSeconds); /* write the time*/

        if (TaskName!=NULL)
        {
          strncpy(DestEntryPtr->TaskData.TaskName,TaskName,MAX_TASK_NAME);
          DestEntryPtr->TaskData.TaskName[MAX_TASK_NAME-1]=0;
        }
        else
        /* put the task name or 'interrupt' */
        if (intContext(&IsrNum))
        {
                strncpy(DestEntryPtr->TaskData.TaskName,( const char * )pcTaskGetTaskName(0),( unsigned short )MAX_TASK_NAME);
                DestEntryPtr->TaskData.TaskName[MAX_TASK_NAME-1]=0;
                //strcpy(DestEntryPtr->TaskData.TaskName,"INTR");
                DestEntryPtr->TaskData.TaskId= (UINT8)IsrNum;
        }
        else
        {
                strncpy(DestEntryPtr->TaskData.TaskName,( const char * )pcTaskGetTaskName(0),( unsigned short )MAX_TASK_NAME);
                DestEntryPtr->TaskData.TaskName[MAX_TASK_NAME-1]=0;
                DestEntryPtr->TaskData.TaskId=TaskId;
        }
	
//put the data
	DestEntryPtr->TaskData.TaskId=TaskId;
	

    /* put the description*/
   // strncpy(DestEntryPtr->text,text,LOG_SYS_MAX_TEXT_LEN-1);
   // DestEntryPtr->text[strlen(DestEntryPtr->text)-1]=0;/*without the "\n"*/

        EntryCounter+=1;
        SysLogPtr->LogHeader.LogEntryCounter=EntryCounter;
        
        /*cyclic index increment*/
        WriteIndex+=1;
        if (WriteIndex==LOG_MAX_NUM_OF_SYSLOG_ENTRIES)
          WriteIndex=0;
        
	SysLogPtr->LogHeader.NextWriteIndex=WriteIndex;
  
#if 0        
        if (EntryCounter==100)
          PrintSyslogData();
       
       
#endif
 
	SysLogBusy=FALSE;
#ifdef _INCLUDE_OS_
	xSemaphoreGive( xLogSysBinarySemaphore );
#endif

    return LL_OK;

}


/* get entry num (starting from 0) from the last written entry*/
LL_STATUS LOG_GetLogEntry(U32 EntryIndex,LOG_SYS_DATA_ENTRY_STRUCT_PTR* LogEntryPtr)
{

    LOG_SYS_DATA_ENTRY_STRUCT * DestEntryPtr;
    LOG_SYS_TASK_DATA LogEntry;

     SysLogPtr->LogHeader.NextWriteIndex;
     
     
     // check if in the first cyclic
     if ( SysLogPtr->LogHeader.NextWriteIndex==SysLogPtr->LogHeader.LogEntryCounter)
     {//first cyclic
       if (EntryIndex> SysLogPtr->LogHeader.NextWriteIndex)
        return LL_ERROR;
       
        *LogEntryPtr=&SysLogPtr->LogData[SysLogPtr->LogHeader.NextWriteIndex-EntryIndex-1];
       
     }
     else
     {
       if (EntryIndex>LOG_MAX_NUM_OF_SYSLOG_ENTRIES)
        return LL_ERROR;
       
       int FindEntry;
       FindEntry=SysLogPtr->LogHeader.NextWriteIndex-EntryIndex-1;
       if (FindEntry>=0)
        *LogEntryPtr=&SysLogPtr->LogData[FindEntry];
       else
         *LogEntryPtr=&SysLogPtr->LogData[LOG_MAX_NUM_OF_SYSLOG_ENTRIES+FindEntry];
     }
     
   
    return LL_OK;
}

#if 0
/* the sysLog is not using the log task for improving the performance*/
LL_STATUS LOG_AddSysLogOSMessage(char * Msg)
{

	LOG_SYS_ENTRY_STRUCT * DestEntryPtr;
    int WriteIndex;

	if (!EnableLogOsSys)
		return LL_OK;

	if (SysLogBusy)
		return LL_ERROR;

	if (SysLogPtr==NULL) /* syslog not initialized*/
		return LL_ERROR;

	SysLogBusy=true;


    /*cyclic index increment*/
    WriteIndex=SysLogPtr->LogUnitStruct.NextWriteEntryIndex;

	DestEntryPtr=(LOG_SYS_ENTRY_STRUCT*)&SysLogPtr->LogUnitStruct.LogDB[WriteIndex*sizeof(LOG_SYS_ENTRY_STRUCT)];

	TIME_Get( &DestEntryPtr->TimeStamp.Minute, &DestEntryPtr->TimeStamp.Msec); /* write the time*/
	//strcpy(DestEntryPtr->DataStruct.TaskName,"OS HOOK");
	//strcpy(DestEntryPtr->text,Msg);

    SysLogPtr->LogUnitStruct.NextWriteEntryIndex=(WriteIndex+1) & LOG_MAX_NUM_OF_SYSLOG_ENTRIES;

    SysLogPtr->LogUnitStruct.NumberOfEntries++;

	SysLogBusy=FALSE;
    return OK;

}
#endif



LL_STATUS LOG_SetFalseResetCounter(int Value)
{
	SysLogPtr=(LOG_SYS_LOG_DB_STRUCT_PTR)SysLogBufPtr(); /* Where the USER_RESERVED_MEM is start */
	SysLogPtr->LogHeader.FalseResetCounter=Value;

	return LL_OK;
}


LL_STATUS LOG_GetFalseResetCounter(int * Value)
{
	SysLogPtr=(LOG_SYS_LOG_DB_STRUCT_PTR)SysLogBufPtr(); /* Where the USER_RESERVED_MEM is start */
	*Value=SysLogPtr->LogHeader.FalseResetCounter;
	return LL_OK;
}

int FalseResetCounter()
{
	int FalseResetCount;
	if (LOG_GetFalseResetCounter(&FalseResetCount)==LL_OK)
	{
		return FalseResetCount;
	}
	else
		return -10; /*not valid one*/
}




int LOG_GetLogMaxNumOfEntries()
{
  return LOG_MAX_NUM_OF_SYSLOG_ENTRIES;
}


char * LogTypeToStr(U8 /*LOG_TYPES*/ LogType)
{
  switch (LogType)
  {
    case LOG_TYPE_TASK_SWITCH:
      return "TASK_SWITCH";
      break;
    case LOG_TYPE_TASK_STACK:
      return "TASK_STACK";  //tbd
      break;
    case LOG_TYPE_MODULE:
      return "MODULE"; //tbd
      break;
    default:
      return  "TBD TBD";
      break;
  }
}


/**
* @brief  uDelay
*         This function provides delay time in micro sec
* @param  usec : Value of delay required in micro sec
* @retval None
*/
void uDelay (const uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}

char PrintMsg[100];
/* print the last sys log entries*/
void LOG_PrintSysLog()
{
	int Entry;
        static int LastTimeStampUsec=0;
       	LOG_SYS_DATA_ENTRY_STRUCT *EntryDataPtr;

	M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"\n+ TIME (HOUR:MIN:MSEC.STAMP (DIFF) +  DATA_ID  +  DATA       +");
#ifdef _INCLUDE_OS_
	xSemaphoreTake( xLogSysBinarySemaphore, portMAX_DELAY ); 
#endif
        
        
	for (Entry=0;Entry<LOG_GetLogMaxNumOfEntries();Entry++)
	{
	  if (LOG_GetLogEntry(Entry, &EntryDataPtr)==LL_OK)
	  {
            int DiffTimeStampUsec=LastTimeStampUsec-EntryDataPtr->TaskData.Time.TimeStamp;
                    
            
            int len=sprintf(PrintMsg,
                    "|    %u:%u:%u.%u (%u)  |    %8s (%d)    | %8s  |   %d\n\r",
				  (uint8_t)EntryDataPtr->TaskData.Time.Hour,
                                  (uint8_t)EntryDataPtr->TaskData.Time.Minute,
                                  (uint8_t)EntryDataPtr->TaskData.Time.Seconds,
                                  (uint32_t)EntryDataPtr->TaskData.Time.TimeStamp,
                                  DiffTimeStampUsec,
                                  LogTypeToStr(EntryDataPtr->TaskData.LogType),
                                  EntryDataPtr->TaskData.LogType,
                                  EntryDataPtr->TaskData.TaskName,
                                  Entry+1);
                
               

                    AuxPrintStrMsg(PrintMsg);  
             
            

	  }
          else
            break;
          LastTimeStampUsec=EntryDataPtr->TaskData.Time.TimeStamp;
          
	}

        
#ifdef _INCLUDE_OS_
	xSemaphoreGive( xLogSysBinarySemaphore );
#endif
       uDelay(10000); //10msec
}

#ifdef __cplusplus
 }
#endif
