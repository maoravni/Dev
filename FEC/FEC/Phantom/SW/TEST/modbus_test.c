/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : modbus_test.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\TEST 
*
*  PURPOSE     : Main test task for Modbus testing under Free RTOS.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.01     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
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
  #include "..\..\..\Common\MODBUS\modbus_exp.h"



//#include "serial_debug.h"
  #include ".\BSP\bsp_srvcs.h"
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
  #define TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
  #define STACK_SIZE    ( configMINIMAL_STACK_SIZE + 350 )
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  void vMB_Test( void* pvParameters );
  void vMB_RxTx( void* pvParameters );
/* --- PRIVATE_DATA --------------------------------------------------------- */
  xSemaphoreHandle xSmphrTx = NULL; // Semaphore between acquisition and com tasks
  xSemaphoreHandle xSmphrRx = NULL; // Semaphore between acquisition and com tasks
  MB_NODE_PT mb_node_1 = NULL;
  
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

    
    phantom_board_setup(); /* Phantom Board low level hardware setup. */
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    vSemaphoreCreateBinary(xSmphrTx);
    if(xSmphrTx)
      xSemaphoreTake(xSmphrTx, portMAX_DELAY);
    
//    mb_node_1 = MB_node_create(MB_1,MB_MASTER);
mb_node_1 = MB_node_create(MB_1, MB_MASTER, "MB_COMM_1", MODBUS_TASK_STACK_SZ, MODBUS_TASK_PRIORITY, MODBUS_QUEUE_LEN);    
    
    
    xTaskCreate( ( pdTASK_CODE ) vMB_Test, "MB_TST", STACK_SIZE, NULL, TASK_PRIORITY ,    NULL );    
//    xTaskCreate( ( pdTASK_CODE ) vMB_RxTx, "Com",    STACK_SIZE, NULL, TASK_PRIORITY + 2, NULL );

    vTaskStartScheduler();

    /* Should never get here! */
    return 0;
    
  }/* int main(void */
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*----------------------------------------------------------------------------
  The Main Test Task for Modbus Communication.
*----------------------------------------------------------------------------*/
  void vMB_Test( void* pvParameters ){
   static   uint16_t Sp=10; //1 degrees
//   portTickType xLastWakeTime;
//   const portTickType xFrequency = 3;  // = 0.3 ms (tick = 0.1 ms)
    
    while (1){
      
//      ModbusMaster_writeSingleRegister(mb_node_1,2,2,Sp); //device address 2, register 2
      ModbusbMaster_SingleRegWrite( mb_node_1, 2, 2, Sp);
          
      Sp+=10; //+1 degrees
      if (Sp>=2000) 
        Sp=10;     
          
//      vTaskDelayUntil( &xLastWakeTime, xFrequency ); /* wait 500 mSec */
      vTaskDelay(1000);
    }

  }/* void vMB_Test( void* pvParameters ) */
/*----------------------------------------------------------------------------
  The Main Modbus Communication Task. Responsible for transmit and receive data via a 
  Modbus channel.
 *----------------------------------------------------------------------------*/  
  void vMB_RxTx( void* pvParameters ){
    uint8_t test_byte;
//    int32_t res = 0;
//    uint8_t test_arr[] = {'t','e','s','t',' ','#','1',0x0D,0x0A};
//      uint8_t test_arr[9]; 

    mb_half_dplx_ctrl(mb_node_1, RX);
        
    while(1){
 //     rcv_buf(test_arr, sizeof(test_arr), USART3);
 //     send_buf(test_arr, sizeof(test_arr), USART3);
           
      if(!(rcv_char(USART3, &test_byte))){
        
        mb_half_dplx_ctrl(mb_node_1, TX);
        send_char(USART3, test_byte);       
        while( xSemaphoreTake( xSmphrTx, portMAX_DELAY ) != pdTRUE ){}
        //xSemaphoreTake(xSmphrTx, portMAX_DELAY);
        mb_half_dplx_ctrl(mb_node_1, RX);

      }
         
    }/* while(1) */
         
  }/* void vMB_RxTx( void* pvParameters ) */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/*----------------------------------------------------------------------------
  synop
 *----------------------------------------------------------------------------*/
  //        do{
//            res = get_rx_idle_flag(USART3);
//        }while(res == 0);
  