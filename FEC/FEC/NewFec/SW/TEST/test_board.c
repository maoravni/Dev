/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : FEC  
*
*  FILE        : test_board.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\FEC\SW\TEST 
*
*  PURPOSE     : The FEC board HW test support. 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  FEC at its own board.
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.10     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
* 
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
  #include "ll_typedef.h"

#include <stm32f2xx.h>
  #include "free_rtos_exp.h"
  #include "cfg_db_eeprom.h"
  #include "eeprom_mc24xx1025.h"
  #include "rtu_master.h"
  #include "opto_out.h"
  #include "modbus_exp.h"
  #include "netconf.h"
  #include "..\BSP\dg_in_ctrl.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "test_board.h"
  #include "e2_tst.h"
#include "a2d_tst.h"
#include "can_tst.h"
#include "dig_in_tst.h"
  #include "bsp_srvcs.h"
#include "mod_bus_tst.h"
#include "os_cnfg.h"
#include "Monitor.h"
#include "Board_Temperature_tmp275.h"
#include "DigTrimmer_ad5292bruz.h"
#include <logger.h>
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
  #define STACK_SIZE    ( configMINIMAL_STACK_SIZE )
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */ 
  void e2_test( void* pvParameters );
  void dac_test( void* pvParameters );  
  void opto_out_test( void* pvParameters );    
  void test_init_task(void * pvParameters);
  void a2d_test( void* pvParameters );
/* --- PRIVATE_DATA --------------------------------------------------------- */
xSemaphoreHandle dig_in_out_sem = NULL;
//static OPTO_OUT_CTRL_VAL_ET val = ACTIVE;
//static OPTO_OUT_ID_ET id;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
//#define  GPOUT_CTRL_IMPL_HERE
/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/


void test_init_task(void * pvParameters){
  
   LwIP_Init();  
   tcp_mb_init();
   Monitor_init(MONITOR_TASK_PRIORITY, MONITOR_TASK_STACK_SZ);
//  xTaskCreate( ( pdTASK_CODE ) a2d_test,        "A2D_TST"   ,STACK_SIZE, NULL, TASK_PRIORITY , NULL );  
//  xTaskCreate( ( pdTASK_CODE ) mb_master_test, "MB_TST",   STACK_SIZE, NULL, TASK_PRIORITY , NULL );
//   xTaskCreate( ( pdTASK_CODE ) opto_out_test,  "OPTO_TST", STACK_SIZE, NULL, TASK_PRIORITY , NULL );
//   vSemaphoreCreateBinary(dig_in_out_sem);
//   if(dig_in_out_sem)
//     xSemaphoreTake(dig_in_out_sem, portMAX_DELAY);
//  xTaskCreate( ( pdTASK_CODE ) dig_input_test, "DG_IN_TST",STACK_SIZE, NULL, TASK_PRIORITY , NULL );
//  xTaskCreate( ( pdTASK_CODE ) e2_test,        "E2_TST"   ,STACK_SIZE, NULL, TASK_PRIORITY , NULL );
//   xTaskCreate( ( pdTASK_CODE ) an_out_test,        "an_out"   ,STACK_SIZE, NULL, TASK_PRIORITY , NULL );
//   CAN_TestInit();
//   xTaskCreate( ( pdTASK_CODE ) can_test, "can_test"   ,STACK_SIZE, NULL, TASK_PRIORITY , NULL );
//   xTaskCreate( ( pdTASK_CODE ) dac_test, "dac_test"   ,STACK_SIZE, NULL, TASK_PRIORITY , NULL );
   vTaskDelete( NULL );//delete this task
}
  

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*******************************************************************************
  * @brief : On-chip DAC test Task function.       
  * @param   
  * @retval  
*******************************************************************************/
  void dac_test( void* pvParameters ){
   u16 res;
   static u16 val = 0;
   
    while (1){
      setDAC1(val); 
      M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"DAC_0_10V  = %d", val);
      val += 1000;
      if(val > 10000)
        val = 0;      
     vTaskDelay(3000);      
    }/* while (1) */
    
  }/************** void an_out_test( void* pvParameters ) *********************/
/*******************************************************************************
  * @brief : Analog output test Task function.       
  * @param   
  * @retval  
*******************************************************************************/
  void an_out_test( void* pvParameters ){
   u16 res;
   static u16 val = 4000;
    while (1){ 
      Set_AO_mA(TRIMMER1,val);        
      M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"TRIMMER_1  = %d", val);
      val += 1000;
      if(val > 20000) val = 4000;
      vTaskDelay(5000);
      
//      res = Get_AO_mA(TRIMMER1);
//     vTaskDelay(1000);      
    }    
  }/************** void an_out_test( void* pvParameters ) *********************/
/*******************************************************************************
  * @brief : EEPROM test Task function.       
  * @param   
  * @retval  
*******************************************************************************/
  void e2_test( void* pvParameters ){
    while (1){ 

       eeprom_tst();        
//SendResponse ((uint8_t*)&(ADC_resalt_arr[ADC1_in10]),2);
      vTaskDelay(1000);
    }    
  }/*********************** e2_test ************************************/
 


  OPTO_OUT_CTRL_VAL_ET invers_val(OPTO_OUT_CTRL_VAL_ET val){
    
    if(val == DEACTIVE) val = ACTIVE;
    else val = DEACTIVE;
    return val;
  }
/*******************************************************************************
  * @brief : The OPTO OUTPUTS Test Task function.       
  * @param   
  * @retval  
*******************************************************************************/
  void opto_out_test( void* pvParameters ){
   static OPTO_OUT_CTRL_VAL_ET val = ACTIVE;
   OPTO_OUT_ID_ET id;
   signed portBASE_TYPE      res;
   uint8_t new_val;
    
    while (1){
      
      for(id = OPTO_OUT1; id < NUMBER_OF_OPTO_OUTS; id++){      
        dig_out_ctrl(id, val);
        vTaskDelay(250);        
      }/* for */
      
      for(id = OPTO_OUT1; id < NUMBER_OF_OPTO_OUTS; id++){
        val = invers_val(val);
        dig_out_ctrl(id, val);
 //       res = xSemaphoreGive(dig_in_out_sem);
        new_val = get_dig_out_val( id );
//M_LOGGER_LOGF (M_LOGGER_LEVEL_TRACE, "DIG_OUT_%d = %d",id, new_val,0,0,0,0,0);        
        vTaskDelay(250);
        val = invers_val(val);
        dig_out_ctrl(id, val);     
      }/* for */
      
//        dig_out_ctrl(SAFETY, val);
      
       val = invers_val(val);
      
      vTaskDelay(1000);
      
    }/* while (1) */
  }/*************** void opto_out__test( void* pvParameters ) *****************/



/**
  * @brief  USART mirror test main task function  
  *
  * @param     
  *
  * @retval  
**/
#if 0
  void uart_test(void * pvParameters){
   uint32_t res = 1;
   uint8_t byte;
   
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
#endif
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
#if 0
/*   **
  * @brief  Turns selected 'gp_out' On. 
  * @param  'gp_out' going turn On.
  * @retval None
  */
  void gp_out_hight(GPIO_ID_ET gp_out){

    PortOf_[gp_out]->BSRRH = PinOf_[gp_out];
    
  }/* led_light */
/*----------------------------------------------------------------------------
  * @brief  test_init_task task
  * @param  pvParameters not used
  * @retval None
 *----------------------------------------------------------------------------*/

/**
  * @brief  Turns selected 'gp_out' Off. 
  * @param  'gp_out' going turn Off.
  * @retval None
  */
  void gp_out_low(GPIO_ID_ET gp_out){

    PortOf_[gp_out]->BSRRL = PinOf_[gp_out];
    
  }/* led_dark */


/**
  * @brief  Toggles the selected 'gp_out'. 
  * @param  'gp_out' going toggle.
  * @retval None
  */
  void gp_out_toggle(GPIO_ID_ET gp_out){
    
  PortOf_[gp_out]->ODR ^= PinOf_[gp_out];
  
  }/* led_toggle */
#endif
