/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : MOSQUITO 
*
*  FILE        : main_test.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\MOSQUITO\SW\TEST 
*
*  PURPOSE     : Main test task for Modbus testing under Free RTOS.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : MOSQUITO 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.05     Igor Natachanny   1.0.0   Initial version
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
  #include "CmdHnd.h"


/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
  extern   void mb_master_test( void* pvParameters );
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
 xSemaphoreHandle xSmphrRx;
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
  #define TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
  #define STACK_SIZE    ( configMINIMAL_STACK_SIZE + 350 )
/*--------------- Tasks Priority -------------*/
     
#define LED_TASK_PRIO    ( tskIDLE_PRIORITY + 1 )

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
//    void mb_master_test( void* pvParameters );
  void toggle_led(void * pvParameters);
  void uart_test(void * pvParameters);
/* --- PRIVATE_DATA --------------------------------------------------------- */
//  xSemaphoreHandle xSmphrTx = NULL; // Semaphore between acquisition and com tasks
//  xSemaphoreHandle xSmphrRx = NULL; // Semaphore between acquisition and com tasks

  LED_ID_ET led = LED_ALIVE;

LED_ID_ET tp = MCU_TP_1;
uint8_t byte = 'a'; 
static    int8_t msg_buf[30];

   MB_SLAVE_NODE_PST  mb_node_2 = NULL;
   MB_MASTER_NODE_PST mb_node_1 = NULL;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
  int main(void){
  /*!< At this stage the microcontroller clock setting is already configured to 
       120 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */
   xTaskHandle xHandle;//, x_uart_tst_th;
   signed portBASE_TYPE res;
   
   
    prvSetupHardware(); /* Mosquito Board low level hardware setup. */
    
    
    res = aux_term_init(AUX_TERM_QUEUE_SZ, configMINIMAL_STACK_SIZE, TERMINAL_TASK_PRIORITY);/* Keep it first! */

    
    mb_node_1 = mb_master_node_create(MB_1);
    res = mb_master_node_init("mb_1_master", MODBUS_TASK_STACK_SZ, MODBUS_TASK_PRIORITY, MODBUS_QUEUE_LEN, mb_node_1);
    if(!res){
      while(1){
      };
    }    
    
    mb_node_2 = mb_slave_node_create(MB_2);
    res = mb_slave_node_init("mb_2_slave", MODBUS_TASK_STACK_SZ, MODBUS_TASK_PRIORITY, MODBUS_QUEUE_LEN, mb_node_2);
    if(!res){
      while(1){
      };
    }    


    
    // initialized the Temperature controllers outputs for the SSRs 
//    SSR_Init();
    
    CMDHND_Init(CMDHND_TASK_PRIORITY, CMDHND_TASK_STACK_SZ, CMDHND_QUEUE_LENGTH);
    
  /* Start toogleLed5 task : Toggle LED  every 250ms */
    res = xTaskCreate(alive_led_task, "toggle_alive_led", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, &xHandle);

//    res = xTaskCreate(uart_test, "uart_test", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, &x_uart_tst_th);
    res = xTaskCreate( ( pdTASK_CODE ) mb_master_test, "MB_TST", STACK_SIZE, NULL, TASK_PRIORITY ,    NULL );
    
      vTaskStartScheduler();    

  /* Should never get here as control is now taken by the scheduler */
    while(1){
    };
    //return 0;
    
  }/* int main(void */
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/**
  * @brief : The Modbus Master Test Task function.       
  * @param   
  * @retval  
**/
  void mb_master_test( void* pvParameters ){
   static   uint16_t tempr_set_point = 10; //1 degrees
   LL_STATUS res;    
    
    while (1){     
      if (tempr_set_point > 2000) 
        tempr_set_point = 10;     
          
      res = rtu_single_reg_write(mb_node_1,133,2, tempr_set_point, 0, 0, NULL);
//      vTaskDelay(1000);
//      res = rtu_hold_regs_read(mb_node_1,133, 2, 1, 0, 0, NULL); 

      tempr_set_point += 10; //+1 degrees
      vTaskDelay(1000);
    }
  }/*************** void mb_master_test( void* pvParameters ) *****************/

/*----------------------------------------------------------------------------
  USART mirror test main task function
 *----------------------------------------------------------------------------*/
  void uart_test(void * pvParameters){
   uint32_t res = 1;

    mb_set_dir(MB_1, RX);
    while(1){
      if(!(rcv_char(USART3, &byte))){        
        mb_set_dir(MB_1, TX);       
         do{
           res = send_char(USART3, byte);
	       }while(res);
//      vTaskDelay(1000);
       mb_set_dir(MB_1, RX);          
      }
   }
   
  }/*uart_test*/
  



//------------------------------------------------------------------------------
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