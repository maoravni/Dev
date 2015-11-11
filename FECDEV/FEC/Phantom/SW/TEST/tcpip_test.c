/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f2x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tcpip.h"
//#include "serial_debug.h"
#include "CNFG\mb_cnfg.h"
#include ".\BSP\bsp_srvcs.h"
#include "hmi.h"
//C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\TEST
//C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS
#include "..\..\..\Common\MODBUS\modbus_exp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
  #define TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
  #define STACK_SIZE    ( configMINIMAL_STACK_SIZE + 350 )
/*--------------- LCD Messages ---------------*/
#define MESSAGE1   "     STM32F2x7      "
#define MESSAGE2   "  STM32F-2 Series   "
#define MESSAGE3   " UDP/TCP EchoServer "
#define MESSAGE4   "                    "

/*--------------- Tasks Priority -------------*/
#define DHCP_TASK_PRIO   ( tskIDLE_PRIORITY + 2 )      
#define LED_TASK_PRIO    ( tskIDLE_PRIORITY + 1 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern struct netif xnetif;
__IO uint32_t test;
 /* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  void vMB_Test( void* pvParameters );
  void vMB_RxTx( void* pvParameters );
/* --- PRIVATE_DATA --------------------------------------------------------- */
//  xSemaphoreHandle xSmphrTx = NULL; // Semaphore between acquisition and com tasks
  xSemaphoreHandle xSmphrRx = NULL; // Semaphore between acquisition and com tasks
  MB_NODE_PT mb_node_1 = NULL;
  
/* Private function prototypes -----------------------------------------------*/
extern void tcp_mb_init(void);
extern void tcpecho_init(void);
extern void udpecho_init(void);

/* Private functions ---------------------------------------------------------*/
/*----------------------------------------------------------------------------
  The Main Test Task for Modbus Communication.
*----------------------------------------------------------------------------*/
  void vMB_Test( void* pvParameters ){
   static   uint16_t Sp=10; //1 degrees
//   portTickType xLastWakeTime;
//   const portTickType xFrequency = 3;  // = 0.3 ms (tick = 0.1 ms)
    
    while (1){
      
//      ModbusMaster_writeSingleRegister(mb_node_1,2,2,Sp); //device address 2, register 2
//Now by tcp command      
      ModbusbMaster_SingleRegWrite( mb_node_1, 133, 2, Sp);
          
      Sp+=10; //+1 degrees
      if (Sp>=2000) 
        Sp=10;     
          
//      vTaskDelayUntil( &xLastWakeTime, xFrequency ); /* wait 500 mSec */
      vTaskDelay(1000);
    }

  }/* void vMB_Test( void* pvParameters ) */
  
/**
  * @brief  Toggle Led4 task
  * @param  pvParameters not used
  * @retval None
  */
void ToggleLed_A(void * pvParameters)
{
  while(1)
  {
            /* toggle LED4 each 250ms */
        led_toggle(LED_A);
        vTaskDelay(250);
#if 0    
    test = xnetif.ip_addr.addr;
    /*check if IP address assigned*/
    if (test !=0)
    {      
      for( ;; )
      {
        /* toggle LED4 each 250ms */
        led_toggle(LED_A);
        vTaskDelay(250);
      }
    }
#endif
  }
  
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured to 
       120 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */

    phantom_board_setup(); /* Phantom Board low level hardware setup. */
    
    
    led_dark(LED_A);
    led_dark(LED_B);
    led_dark(LED_C);
    led_dark(LED_D);
    
  
  /* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
//2012/06/25  ETH_BSP_Config();
    
  /* Initilaize the LwIP stack */
//2012/06/25  LwIP_Init();
  
  /* Initialize tcp echo server */
//  tcpecho_init();
  
  /* Initialize tcp_mb information exchange */
//2012/06/25  tcp_mb_init();
  
      USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    
    vSemaphoreCreateBinary(xSmphrRx);
    if(xSmphrRx)
      xSemaphoreTake(xSmphrRx, portMAX_DELAY);
    
    mb_node_1 = MB_node_create(MB_1, MB_MASTER, "MB_COMM_1", MODBUS_TASK_STACK_SZ, MODBUS_TASK_PRIORITY, MODBUS_QUEUE_LEN);
    
  /* Start toogleLed4 task : Toggle LED4  every 250ms */
  xTaskCreate(ToggleLed_A, "LED_A", configMINIMAL_STACK_SIZE, NULL, LED_TASK_PRIO, NULL);
  
//  xTaskCreate( ( pdTASK_CODE ) vMB_Test, "MB_TST", STACK_SIZE, NULL, TASK_PRIORITY ,    NULL );
    xTaskCreate( ( pdTASK_CODE ) vMB_Test, "MB_TST", STACK_SIZE, NULL, TASK_PRIORITY ,    NULL );
    
  /* Start scheduler */
  vTaskStartScheduler();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}



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


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
