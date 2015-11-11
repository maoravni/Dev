/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MOSQUITO
*
*  FILE        : CmdHnd.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.07     Yossi Sarusi   1.0.0   Initial version
*
*******************************************************************************
*/

#ifndef CMDHND_H
#define CMDHND_H


#define CMDHND_QUEUE_LENGTH 10
#define SEND_TICK_TO_WAIT 20
#define SEND_HIGHER_PRI_TASK_WOKEN TRUE
#define CMDHAND_MAX_MESSEGE_LEN 252 //due to limitation of RTOS qeueu size cannot exceed the 255-3  //was 80 

// Task/module ids for debug log message
#define TASK_ID_CMDHDND 1

typedef enum {
	CMDHND_TYPE_AA,
	CMDHND_TYPE_BB,
	CMDHND_TYPE_CC,
	CMDHND_TYPE_DD
} CMDHND_TASK_MSG_TYPE;

typedef enum {
	CMDHND_SUB_TYPE_NULL,
	CMDHND_SUB_TYPE_TBD2,
	CMDHND_SUB_TYPE_TBD3
} CMDHND_TASK_MSG_SUB_TYPE;

typedef struct {
	CMDHND_TASK_MSG_TYPE		MsgType;
	CMDHND_TASK_MSG_SUB_TYPE	MsgSubType;
	UINT8                       MsgLen;
	UINT8 						MsgBuf[CMDHAND_MAX_MESSEGE_LEN];
} CMDHND_TASK_MsgQ_Struct,*CMDHND_TASK_MsgQ_StructPtr;



LL_STATUS CMDHND_Init(int TaskPriority, int StackSize, int QueueSize);
LL_STATUS CMDHND_SendMsg(CMDHND_TASK_MSG_TYPE MsgType, CMDHND_TASK_MSG_SUB_TYPE MsgSubType,
						void *MsgValue,UINT8 MsgLen, bool Urgent, bool FromInterrupt);
LL_STATUS CMDHND_SendMsgWithHeader(CMDHND_TASK_MSG_TYPE MsgType, CMDHND_TASK_MSG_SUB_TYPE MsgSubType,
						void *HeaderValue,UINT8 HeaderLen,void *MsgData,UINT8 MsgDataLen, bool Urgent, bool FromInterrupt);
#endif
