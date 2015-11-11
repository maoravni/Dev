#ifndef _LOG_SYS_H
#define _LOG_SYS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define LOG_UART_MODULE		1
#define LOG_MONITOR_MODULE	2
#define LOG_TCP_MODULE	        3
#define LOG_TCP_MSG_MODULE	4
#define LOG_PWM_MODULE    	5
#define LOG_CAN_MODULE	        6



//#ifdef _USE_LOG_SYS_
#define LOG_ADD_SYS_LOG_MESSAGE(Id,Val1,Val2,Val3,Text)	LOG_AddSysLogMessage(LOG_TYPE_SYS,Id,Val1,Val2,Val3,Text)
//#else 
//#define LOG_ADD_SYS_LOG_MESSAGE(LogType,Id,Val1,Val2,Val3,Text) ;
//#endif

#define KEY_WORD_FOR_THE_SYS_LOG_LEN 8

#define LOG_MAX_NUM_OF_SYSLOG_ENTRIES 100  // should be 255 /* 4KB -> 1024 32bits-> 256 128bits-> -1 for header */

typedef enum
{
  LOG_TYPE_TASK_SWITCH,
  LOG_TYPE_TASK_STACK,
  LOG_TYPE_MODULE,
  LOG_LAST_TYPE,
}  LOG_TYPES;




#define LOG_SYS_DEBUG LOG_LAST_TYPE

typedef struct
{
  U32 TimeStamp;
  U8  Hour;
  U8 Minute;
  U8 Seconds;
} LOG_SYS_TIME_DATA;

typedef struct
{
  U8  Day;
  U8  Month;
  U8  Year;
 } LOG_SYS_DATE_DATA;

#define MAX_TASK_NAME 6  //including the 0 termination

//mast be 128 bits size structure 
typedef struct
{
    LOG_SYS_TIME_DATA Time;  //64 bits alignment
    U8 /*LOG_TYPES*/ LogType; // 8 bits
    U8 TaskId;
    char TaskName[MAX_TASK_NAME];
    
} LOG_SYS_TASK_DATA, *LOG_SYS_TASK_DATA_PTR;


//mast be 128 bits size structure 
typedef struct
{
    U32 Spare[4];
} LOG_SYS_USER_DATA, *LOG_SYS_USER_DATA_PTR;

//256 bits size structure 
typedef struct
{
  U8 Version;  
  LOG_SYS_DATE_DATA DateOfStartLog;  //24 bits
  LOG_SYS_DATE_DATA DateOfLatestPowerUp;  //32 bits
  U32 LogEntryCounter;
  U32 NextWriteIndex;
  U32 FalseResetCounter; //count the WD resets 
  
  UINT8 * LogDB; /* pointer to RAM DB*/
  
  U32 Spare[3];
  
} LOG_SYS_TASK_HEADER;

//128 bits size structure
typedef union
{
    LOG_SYS_TASK_DATA TaskData;
    LOG_SYS_USER_DATA UserData;
} LOG_SYS_DATA_ENTRY_STRUCT,*LOG_SYS_DATA_ENTRY_STRUCT_PTR;


typedef struct
{
    char KeyForInitialization[KEY_WORD_FOR_THE_SYS_LOG_LEN]; /* if contain the key dont initialize the DB*/
    LOG_SYS_TASK_HEADER LogHeader;
    LOG_SYS_DATA_ENTRY_STRUCT LogData[LOG_MAX_NUM_OF_SYSLOG_ENTRIES];
	
} LOG_SYS_LOG_DB_STRUCT, *LOG_SYS_LOG_DB_STRUCT_PTR;




LL_STATUS LOG_SYS_Init();
void LOG_SYS_Terminated();

LL_STATUS LOG_AddSysLogMessageTask(UINT8 TaskId, char *TaskName); //for task context switch recording

LL_STATUS LOG_SetFalseResetCounter(int Value);
LL_STATUS LOG_GetFalseResetCounter(int * Value);
int FalseResetCounter();
void LOG_PrintSysLog();
void StartSysLog();

void StopSysLog();

void PrintSyslogData();
bool TraceDisplayTraceLog();
char * pcWriteBufferPtr();


#ifdef __cplusplus
}
#endif

#endif 
