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
  #include "stm32f2xx.h"
  #include <stdio.h>
  #include <string.h>
  #include "free_rtos_exp.h" 
  #include "os_cnfg.h"
  #include "ll_typedef.h"
  #include "..\..\..\Common\MODBUS\modbus_exp.h"
  #include "..\BSP\bsp_srvcs.h"
  #include "..\..\..\Common\HMI\hmi.h"
  #include "usart.h"
  #include "..\..\..\Common\AUX_TERM\aux_term.h"
  #include "..\CNFG\mb_cnfg.h"
  #include "..\BSP\ssr.h"
  #include "cfg_db_if.h"
 // #include "CmdHnd.h"
  #include "stm32f2x7_eth_bsp.h"
  #include "netconf.h"
  #include "GnrlCfgDB.h"
  #include "iwdg.h"
  #include "rtc.h"
  #include "CmbrCfgDB.h"
<<<<<<< .mine
 #include "Rtc.h"
=======
  #include "i2c_ee_cpal.h"
>>>>>>> .r471

/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
long SysTickCounter;
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
  static    int8_t msg_buf[255];
  uint8_t byte = 'a';
    MB_MASTER_NODE_PST  mb_node_1 = NULL;
    MB_SLAVE_NODE_PST   mb_node_2 = NULL;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

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
  static void cbf_write_single_reg(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t  token_val){
   uint16_t   reg_addr, reg_val, tmp_16;

    
    switch(func_status){
      case 1:
        func_status += 1;
        sprintf( (char*)msg_buf,"\r ERROR: Slave Responce CRC_16 FAULT!");
//        aux_term_put_msg(msg_buf);
        break;
      case 2:
        func_status += 2;
        sprintf( (char*)msg_buf,"\r ERROR: Slave didn't answer!");
//        aux_term_put_msg(msg_buf);
        break;
      case 3:
        func_status += 3;
        sprintf( (char*)msg_buf,"\r ERROR: Function Exeption: incorrect function params or Function unsupported by slave!");
//        aux_term_put_msg(msg_buf);
        break;        
      case 0:        
       memcpy( &tmp_16, &(resp_buf[2]), sizeof(uint16_t));
       reg_addr = SWAP_16(tmp_16);
       memcpy( &tmp_16, &(resp_buf[4]), sizeof(uint16_t));
       reg_val  = SWAP_16(tmp_16);
            sprintf( (char*)msg_buf,"\r SINGLE_REG_WRITE: slave_id = 0x%x, reg_addr = 0x%x, reg_val = %d - OK!", resp_buf[0], reg_addr, reg_val);
//            aux_term_put_msg(msg_buf);            
        break;
      default:
        sprintf( (char*)msg_buf,"\r UNKNOWN ERROR!");
//        aux_term_put_msg(msg_buf);
        break;
    }/* switch(func_status) */
    
 }/* static void cbf_write_single_reg(uint8_t func_status, uint8_t *resp_buf, uint8_t resp_sz) */
 
   static void cbf_read_holding_reg(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t  token_val, void *aux_ptr){
//   uint16_t   reg_addr, reg_val, tmp_16;
   uint16_t res_buf[15];
    
    switch(func_status){
      case 1:
        func_status += 1;
        sprintf( (char*)msg_buf,"\r ERROR: Slave Responce CRC_16 FAULT!");
//        aux_term_put_msg(msg_buf);
        break;
      case 2:
        func_status += 2;
        sprintf( (char*)msg_buf,"\r ERROR: Slave didn't answer!");
//        aux_term_put_msg(msg_buf);
        break;
      case 3:
        func_status += 3;
        sprintf( (char*)msg_buf,"\r ERROR: Function Exeption: incorrect function params or Function unsupported by slave!");
//        aux_term_put_msg(msg_buf);
        break;        
      case 0:
        
       memcpy( res_buf, resp_buf, resp_sz*2);

//            sprintf( (char*)msg_buf,"\r SINGLE_REG_WRITE: slave_id = 0x%x, reg_addr = 0x%x, reg_val = %d - OK!", resp_buf[0], reg_addr, reg_val);
//            aux_term_put_msg(msg_buf);            
        break;
      default:
        sprintf( (char*)msg_buf,"\r UNKNOWN ERROR!");
//        aux_term_put_msg(msg_buf);
        break;
    }/* switch(func_status) */
    
 }/* static void cbf_write_single_reg(uint8_t func_status, uint8_t *resp_buf, uint8_t resp_sz) */
 
/*******************************************************************************
  * @brief : The Modbus Master Test Task function.       
  * @param   
  * @retval  
*******************************************************************************/
  void mb_master_test( void* pvParameters ){
   static   uint16_t tempr_set_point = 10; //1 degree
   LL_STATUS res;    
    
    while (1){     
      if (tempr_set_point > 2000) 
        tempr_set_point = 10;     
          
 //     res = rtu_single_reg_write(mb_node_1,133,2, tempr_set_point, 0, 0, 0, cbf_write_single_reg);
//      vTaskDelay(1000);
//      res = rtu_hold_regs_read(mb_node_1,133, 2, 1, 0, 0,0, NULL); 
      res =  rtu_hold_regs_read( mb_node_1, 133, 0, 10, 0, 0, 0xA5/*token_val*/, cbf_read_holding_reg);
      tempr_set_point += 10; //+1 degree
      vTaskDelay(1000);
    }
  }/*************** void mb_master_test( void* pvParameters ) *****************/

/**
  * @brief  USART mirror test main task function  
  *
  * @param     
  *
  * @retval  
**/

  void uart_test(void * pvParameters){
   uint32_t res = 1;

    while(1){
      if(!(rcv_char(USART6, &byte))){        
//        mb_set_dir(MB_1, TX);       
         do{
           res = send_char(USART6, byte);
	       }while(res);
//      vTaskDelay(1000);         
      }
   }
   
  }/****************************    uart_test     *****************************/

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

    STATUS RetValue=xTaskCreate( Init_task, ( signed char * ) "Init", INIT_TASK_STACK_SZ, NULL, INIT_TASK_PRIORITY, NULL );
    if (RetValue !=pdPASS)
    {
          while(1)
          {
          };  
           
    }
 
   
	/******************************************************************************/    
  
     
//  res = xTaskCreate(uart_test, "uart_test", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, &x_uart_tst_th);
//  res = xTaskCreate( ( pdTASK_CODE ) mb_master_test, "MB_TST", STACK_SIZE, NULL, TASK_PRIORITY ,    NULL );
//#endif
    /* Start toogleLed4 task : Toggle LED4  every 250ms */
  //  xTaskCreate(alive_led_task, "LED_A", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
/******************************************************************************/    
      vTaskStartScheduler();    
/******************************************************************************/
      
  /* Should never get here as control is now taken by the scheduler */
    while(1){
    };
    //return 0;
    
  }/* int main(void */

void vApplicationIdleHook( void )
{
  if (TraceDisplayTraceLog())
  {
     LOG_PrintSysLog();
  }
}
/*----------------------------------------------------------------------------
  * @brief  Init_task task
  * @param  pvParameters not used
  * @retval None
 *----------------------------------------------------------------------------*/
void Init_task(void * pvParameters)
{
   /*  PWM task */
	PWM_init(PWM_TASK_PRIORITY, PWM_TASK_STACK_SZ);
//init the DB 
      GnrlCfgInit();
      while (!GnrlCfgDbReady())
      {
      }

      for (int Chamber=0;Chamber<MAX_NUM_OF_CHAMBER;Chamber++)
      {
        CmbrCfgInit(Chamber);
        while (!CmbrCfgDbRdy(Chamber))
        {};
      }
               
      
      cfg_db_if_init();     
      
      
        
        /* Initilaize the LwIP stack */
        LwIP_Init();
  
        tcp_mb_init();
  /* Initialize tcp echo server */
//  tcpecho_init();
        
        
        /*  Monitor task */
	Monitor_init(MONITOR_TASK_PRIORITY,MONITOR_TASK_STACK_SZ);
       
        //turn off  the safetry alarm
         U32 Data=0;
        CfgPutParam(CHAMBER_GNRL,ID_GNRL_MNTR_INT_CRITICAL_ALARM,&Data,CfgDB_SET_TO_DB);
        
        
        // test the sys log
        LOG_SYS_Init();
#if 0        
        LOG_AddSysLogMessageTask(1, "TSK1");
        LOG_AddSysLogMessageTask(2, "TSK2");
        LOG_AddSysLogMessageTask(3, "TSK3");
        LOG_AddSysLogMessageTask(4, "TSK4");
        LOG_AddSysLogMessageTask(5, "TSK5");  //last in the log buffer
        LOG_AddSysLogMessageTask(5, "TSK6");
        LOG_AddSysLogMessageTask(6, "TSK7");
        
        LOG_PrintSysLog();
        
        
#endif        
        //end test the sys log
        
        
        
        // Test the ALARM*/
        /*
        U32 Data=1;
        U32 RetValue=CfgPutParam(CHAMBER_1,ID_GNRL_MNTR_INT_ALARM,&Data,CfgDB_SET_TO_DB);
        
        Data=0;
        RetValue=CfgPutParam(CHAMBER_1,ID_GNRL_MNTR_INT_ALARM,&Data,CfgDB_SET_TO_DB);
       */
        rtc_init();        
    /*
        U32 Data=1;
       CfgPutParam(CHAMBER_1,ID_CMBR_CFG_INT_AIR_VALVE_IN_OPEN,&Data,CfgDB_SET_TO_DB);
       CfgPutParam(CHAMBER_1,ID_CMBR_CFG_INT_AIR_VALVE_OUT_OPEN,&Data,CfgDB_SET_TO_DB);
       
       CfgPutParam(CHAMBER_2,ID_CMBR_CFG_INT_AIR_VALVE_IN_OPEN,&Data,CfgDB_SET_TO_DB);
       CfgPutParam(CHAMBER_2,ID_CMBR_CFG_INT_AIR_VALVE_OUT_OPEN,&Data,CfgDB_SET_TO_DB);
        
      */
       
          
        iwdg_init();
        RCC_ClearFlag();/* Clear reset flags */   
        
<<<<<<< .mine
        //RTC Init
        rtc_init();
        
=======
        
        eeprom_init();
>>>>>>> .r471
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
