/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : main_test.c
*   
*  PATH        :  
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.08     Igor Natachanny   1.0.0   Initial version
*  24/10/12    Yossi Sarusi      1.01    Replace the Main_test to main.c ( cleanup)
*
*******************************************************************************
      Copyright and Proprietary Statment.
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include <stm32f2xx.h>
  #include <stdio.h>
  #include <string.h>
  #include "free_rtos_exp.h" 
  #include "os_cnfg.h"
  #include "ll_typedef.h"
  #include "..\..\..\Common\MODBUS\modbus_exp.h"
  #include "bsp_srvcs.h"
  #include "..\..\..\Common\HMI\hmi.h"
  #include "usart.h"
  #include "..\..\..\Common\AUX_TERM\aux_term.h"
  #include "..\CNFG\mb_cnfg.h"
  #include "..\BSP\ssr.h"
  #include "cfg_db_if.h"
  #include "netconf.h"
  #include "GnrlCfgDB.h"
  #include "iwdg.h"
  #include "CmbrCfgDB.h"
 #include "Rtc.h"

#include "usbd_cdc_vcp.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "vcp_srvcs.h"
#include "..\..\..\Common\VCP_TERM\vcp_term.h"
#include "test_board.h"

/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
long SysTickCounter;
extern USB_OTG_CORE_HANDLE  USB_OTG_dev;//__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;
extern MB_MASTER_NODE_PST  mb_node_1;
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
  extern void tcpecho_init(void);
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
  #define TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
  #define STACK_SIZE    ( configMINIMAL_STACK_SIZE + 350 )
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
void Init_task(void * pvParameters);
/* --- PRIVATE_DATA --------------------------------------------------------- */
//  static    int8_t msg_buf[255];
  uint8_t byte = 'a';
 //   MB_MASTER_NODE_PST  mb_node_1 = NULL;
//    MB_SLAVE_NODE_PST   mb_node_2 = NULL;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
#define TEST_FEC
/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
#if ( configUSE_TRACE_FACILITY == 1 )
#define PC_SIZE_OF_RESERVED_MEM 400 //num of reserved bytes for the pc sys

static UINT8 PcLogbuf[PC_SIZE_OF_RESERVED_MEM] @ ".noinit";
char pcWriteBuffer[100];
char * PcLogBufPtr()
{
	return PcLogbuf;
}


char * pcWriteBufferPtr()
{
	return pcWriteBuffer;
}




void PrintVtaskList()
{
	char * pcWriteBuffer =pcWriteBufferPtr();
	vTaskList( pcWriteBuffer );
	puts((void *)pcWriteBuffer);
}

void StartTrace()
{
	vTaskStartTrace( PcLogBufPtr(), PC_SIZE_OF_RESERVED_MEM );
}
#endif
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName)
{
    /* This function will get called if a task overflows its stack.   If the
     parameters are corrupt then inspect pxCurrentTCB to find which was the
     offending task. */

    (void) pxTask;
    (void) pcTaskName;

    for (;;)
        ;
}

void vApplicationIdleHook( void )
{
  if (TraceDisplayTraceLog())
  {
     LOG_PrintSysLog();
  }
}

static void ChamberInit(int ChamberId)
{
        CmbrCfgInit(ChamberId);  //DB init
        while (!CmbrCfgDbRdy(ChamberId)) {}; //wait for ready
        
        /*Due the fact that we need to start with Zone disable state and the PUMA devices keep the latest enable setting 
          we need to force the PUMA into disable state*/
        U32 Data=FALSE;
        CfgPutParam(ChamberId,ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_FRONT,&Data,CfgDB_SET_TO_DB);
        CfgPutParam(ChamberId,ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_MEDIUM,&Data,CfgDB_SET_TO_DB);
        CfgPutParam(ChamberId,ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_REAR,&Data,CfgDB_SET_TO_DB);
     
}
 
static void GenrlAndWallsInit()
{
      GnrlCfgInit();  //DB init
      while (!GnrlCfgDbReady()) {};
      
       /*Due the fact that we need to start with Zone disable state and the PUMA devices keep the latest enable setting 
          we need to force the PUMA into disable state*/
       U32 Data=FALSE;
       for (int Index=0;Index<NUMBER_OFF_WALLS;Index++)
         CfgPutParam(CHAMBER_GNRL,CfgDB_ID_AddIndex(ID_GNRL_CMND_INT_ZONE_ENABLE_TEMPERATURE_WALL_n,Index),&Data,CfgDB_SET_TO_DB);
     
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
  int main(void){
  /*!< At this stage the microcontroller clock setting is already configured to 
       120 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */
   xTaskHandle xHandle, x_uart_tst_th;
   signed portBASE_TYPE res;
   
   
    prvSetupHardware(); /*  */
    ETH_BSP_Config();/* configure ethernet (GPIOs, clocks, MAC, DMA) */      
    
//i2c_init();
    rtc_init();      /* Real Time Clock Init */ 
    a2d_init_hw();
   
    
     
//      adc_init();
    
    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);    
    usb_term_init(AUX_TERM_QUEUE_SZ, USB_TASK_STACK_SZ, USB_TASK_PRIORITY);
         
    a2d_start(A2D_TASK_STACK_SZ, A2D_TASK_PRIORITY);
    res = aux_term_init(AUX_TERM_QUEUE_SZ, TERMINAL_TASK_STACK_SZ, TERMINAL_TASK_PRIORITY);/* Keep it first! */
	  if(!res){
		  while(1){
		  };
	  }

    mb_node_1 = mb_master_node_create(MB_1);
    res = mb_master_node_init("mb_1_master", MODBUS_TASK_STACK_SZ, MODBUS_TASK_PRIORITY, MODBUS_QUEUE_LEN, mb_node_1);
    if(!res){
      while(1){
      };
    }
#ifdef TEST_FEC
    STATUS RetValue=xTaskCreate( test_init_task, ( signed char * ) "main_test", INIT_TASK_STACK_SZ, NULL, INIT_TASK_PRIORITY, NULL );
    if (RetValue !=pdPASS)
    {
          while(1)
          {
          };  
           
    }
#else    
    STATUS RetValue=xTaskCreate( Init_task, ( signed char * ) "Init", INIT_TASK_STACK_SZ, NULL, INIT_TASK_PRIORITY, NULL );
    if (RetValue !=pdPASS)
    {
          while(1)
          {
          };  
           
    }
#endif
/******************************************************************************/    
      vTaskStartScheduler();    
/******************************************************************************/
      
  /* Should never get here as control is now taken by the scheduler */
    while(1){
    };
    //return 0;
    
  }/************************ int main(void ************************************/


#if 0
void vApplicationIdleHook( void )
{
  if (TraceDisplayTraceLog())
  {
     LOG_PrintSysLog();
  }
}
#endif
/*----------------------------------------------------------------------------
  * @brief  Init_task task
  * @param  pvParameters not used
  * @retval None
 *----------------------------------------------------------------------------*/
void Init_task(void * pvParameters)
{
   
   /*  PWM task */
  PWM_init(PWM_TASK_PRIORITY, PWM_TASK_STACK_SZ);
    
  
   
   
  
   GenrlAndWallsInit();

 
   for (int Chamber=0;Chamber<MAX_NUM_OF_CHAMBER;Chamber++)
     ChamberInit(Chamber);
   cfg_db_if_init();
   
 
  
   
    /* Initilaize the LwIP stack */
   LwIP_Init();  
   tcp_mb_init();
       
   /*  Monitor task */
  Monitor_init(MONITOR_TASK_PRIORITY,MONITOR_TASK_STACK_SZ);
  
   //turn off  the safetry alarm
   U32 Data=1;  //NC
   CfgPutParam(CHAMBER_GNRL,ID_GNRL_MNTR_INT_CRITICAL_ALARM,&Data,CfgDB_SET_TO_DB);
 
        
   // test the sys log
   LOG_SYS_Init();
       
   iwdg_init();
   RCC_ClearFlag();/* Clear reset flags */   
        
   //Real Time Clock Init
   rtc_init();
        
   //delete the init task
   vTaskDelete( NULL );
   
 }/* Init_task */


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/**
  * @brief   
  *
  * @param     
  *
  * @retval  
**/
