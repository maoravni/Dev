/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MODBUS_LIB 
*
*  FILE        : CmdHnd.c
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
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
  #include <stdio.h>
  #include <string.h>
  #include "stm32f2xx.h"
  #include "free_rtos_exp.h"
  #include "ll_typedef.h"
  #include "CmdHnd.h"
  #include "..\..\AUX_TERM\aux_term.h"
 
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
 
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */

/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
 
  
/* --- PRIVATE_DATA --------------------------------------------------------- */
  
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/

bool GlobalError;
static void vCmdhndTask( void *pvParameters );


static xQueueHandle CmdHndrQid;
xTaskHandle CmdHndrTaskId;

void DEBUG_WRITE(UINT8 Level, UINT8 ModuleId, UINT8 TypeId, INT16 SubTypeId)
{
}

#ifdef _DEBUG_VER_
static unsigned portBASE_TYPE MessageSize=sizeof(CMDHND_TASK_MsgQ_Struct);
#endif

LL_STATUS CMDHND_Init(int TaskPriority, int StackSize, int QueueSize)
{
	portBASE_TYPE RetValue;

#ifdef _DEBUG_VER_
	MessageSize=sizeof(CMDHND_TASK_MsgQ_Struct);
	CmdHndrQid=xQueueCreate( QueueSize,MessageSize);
#else
	CmdHndrQid=xQueueCreate( QueueSize, sizeof(CMDHND_TASK_MsgQ_Struct));
#endif
	if (CmdHndrQid==0)
	{
		DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,TASK_ID_CMDHDND,0,-100);
		return LL_ERROR;
	}
	
	
	RetValue=xTaskCreate( vCmdhndTask, ( signed char * ) "CmdHnd", StackSize, NULL, TaskPriority, &CmdHndrTaskId );
	if (RetValue !=pdPASS)
	{
		DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,TASK_ID_CMDHDND,0,-101);
		return LL_ERROR;

	}

	//memset(MAIN_COMMRcvBuff, '\0' ,UART_RX_BUFFER_SIZE);

	return LL_OK;
}

static LL_STATUS SendMessageQueue(UINT8* message,bool Urgent, bool FromInterrupt)
{
	 portBASE_TYPE RetValue;
	 signed portBASE_TYPE xHigherPriorityTaskWoken=pdFALSE;

	if (FromInterrupt)
	{
		if (Urgent)
			RetValue=xQueueSendToFrontFromISR( CmdHndrQid, message, &xHigherPriorityTaskWoken );
		else
			RetValue=xQueueSendToBackFromISR( CmdHndrQid, message, &xHigherPriorityTaskWoken );

#if 0 //remove for seving stack deep
		if( xHigherPriorityTaskWoken )
		{/* The task waiting has a higher priority is ready initiate	context	switch */
			taskYIELD ();
		}
#endif
	}
	else
	{
		if (Urgent)
			RetValue=xQueueSendToFront( CmdHndrQid, message, SEND_TICK_TO_WAIT );
		else
			RetValue=xQueueSendToBack( CmdHndrQid, message, SEND_TICK_TO_WAIT );
	}

    if (RetValue!=pdTRUE)
	{ //in case of failure we need to free the memory here
		//vPortFree(message.MsgBuf);
		DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,TASK_ID_CMDHDND,1,RetValue);
        return LL_ERROR;
    }

	return LL_OK;
}
//CMDHND_TASK_MsgQ_StructPtr   message;

//CMDHND_TASK_MsgQ_Struct   message;

static CMDHND_TASK_MsgQ_Struct   message;  //tbd should be replace with malloc and free

LL_STATUS CMDHND_SendMsg(CMDHND_TASK_MSG_TYPE MsgType, CMDHND_TASK_MSG_SUB_TYPE MsgSubType,
						void *MsgValue,UINT8 MsgLen, bool Urgent, bool FromInterrupt)
{

//	CMDHND_TASK_MsgQ_StructPtr   message=(void *)pvPortMalloc(sizeof(CMDHND_TASK_MsgQ_Struct)+CMDHAND_MAX_MESSEGE_LEN);

	//message=(void *)pvPortMalloc(sizeof(CMDHND_TASK_MsgQ_Struct)+CMDHAND_MAX_MESSEGE_LEN);

	
	//int size;

    message.MsgType=MsgType;
	message.MsgSubType=MsgSubType;

	assert_param (MsgLen<=CMDHAND_MAX_MESSEGE_LEN);

	if (MsgLen>CMDHAND_MAX_MESSEGE_LEN)
	{
			DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,TASK_ID_CMDHDND,1,-20);
			return LL_ERROR;

	}

	//if (MsgValue!=NULL)
	//{
		//message.MsgBuf=(void *)pvPortMalloc(MsgLen);
		
		//assert (message.MsgBuf != NULL);
		
		/*if (message.MsgBuf==NULL)
		{
			DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,TASK_ID_CMDHDND,1,-20);
			return LL_ERROR;

		}*/
		message.MsgLen=MsgLen;
		memcpy(message.MsgBuf,MsgValue,MsgLen);

	//}


	return (SendMessageQueue((UINT8*)&message, Urgent, FromInterrupt));
	
}


LL_STATUS CMDHND_SendMsgWithHeader(CMDHND_TASK_MSG_TYPE MsgType, CMDHND_TASK_MSG_SUB_TYPE MsgSubType,
						void *HeaderValue,UINT8 HeaderLen,void *MsgData,UINT8 MsgDataLen, bool Urgent, bool FromInterrupt)
{
	//CMDHND_TASK_MsgQ_Struct   message;
	UINT8 MsgLen=HeaderLen+MsgDataLen;

    message.MsgType=MsgType;
	message.MsgSubType=MsgSubType;

	assert_param (MsgLen<=CMDHAND_MAX_MESSEGE_LEN);

	if (MsgLen>CMDHAND_MAX_MESSEGE_LEN)
	{
			DEBUG_WRITE(M_LOGGER_LEVEL_TRACE,TASK_ID_CMDHDND,1,-20);
			return LL_ERROR;

	}
	
	message.MsgLen=MsgLen;
	memcpy(message.MsgBuf,HeaderValue,HeaderLen); //copy the header
	memcpy(message.MsgBuf+HeaderLen,MsgData,MsgDataLen); //copy the data

	return (SendMessageQueue((UINT8*)&message, Urgent, FromInterrupt));
	
}

/*static void temp(char * buf)
{
	vTaskDelay( 1000 );
				//strcpy(CLI_TXBuffer,"Yossi\n");
				//Send results
	puts ("Yossi\n");

}*/

//CMDHND_TASK_MsgQ_Struct   message;
static void vCmdhndTask( void *pvParameters ) 
{ 
   CMDHND_TASK_MsgQ_Struct   message_T;
   
   //tbd in case of no idle support in the UART-> CmdUartSyncTimerInit(); //for CMI sync message usege
  
   for(;;) 
   { 
#if 0
   		vTaskDelay( 1000 / portTICK_RATE_MS );
		//strcpy(CLI_TXBuffer,"Yossi\n");

        // Send results
        puts ("Yossi\n");
#endif


	   if (xQueueReceive( CmdHndrQid, &message_T, portMAX_DELAY ))
	   {
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
	   		int StackFree=uxTaskGetStackHighWaterMark(0);
#endif
			//LOG_ADD_SYS_LOG_MESSAGE(LOG_CMHND_MODULE,0,message_T.MsgType,0,NULL);
			switch ( message_T.MsgType )
			{

				case CMDHND_TYPE_AA: //Command Line Interface (Terminal)
					{
						message_T.MsgBuf[message_T.MsgLen]=0; //terminates the string
						//AA_Process((char*)message_T.MsgBuf);
						//vPortFree(message.MsgBuf); //free the buffer

					}
				break;
				case CMDHND_TYPE_BB: //Command Message Interface (WIN32 PC application)
					//BB_Process((char*)message_T.MsgBuf);
					//vPortFree(message.MsgBuf); //free the buffer
				break;
				case CMDHND_TYPE_CC:
						//CC_Process(message_T.MsgBuf); //sarve it
					      //vPortFree(message.MsgBuf); //free the buffer

				break;
				case CMDHND_TYPE_DD:
				{
					switch (message_T.MsgSubType)
					{
						case CMDHND_SUB_TYPE_TBD2: 
						{
							//if (DD_SendMessage(CMDHND_SUB_TYPE_TBD2,message_T.MsgLen,message_T.MsgBuf)==LL_ERROR)
							{
								GlobalError=true;
								//Treat other error handle if needed
								
							}
							
							break;
						}
						default:
							//DD_SendMessage(message_T.MsgSubType,message_T.MsgLen,message_T.MsgBuf);
							break;
					}

					break;
				}
			}
	   }

	  
      asm volatile("nop");      //processing event 
   } 
} 


//#endif
