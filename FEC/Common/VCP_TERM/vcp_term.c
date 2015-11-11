/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : VCP_TERM_LIB 
*
*  FILE        : vcp_term.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\VCP_TERM 
*
*  PURPOSE     : All puporses Auxiliary Terminal Implementation   
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.10     Igor Natachanny   1.0.0   Initial version.
*
*******************************************************************************
      Copyright and Proprietary Statment.
******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/

/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
  #include <string.h>
  #include <stdint.h>
 #include <stdio.h>

//  #include "stdarg.h"
  #include "stm32f2xx.h"
  #include "free_rtos_exp.h"
  #include "leds_cnfg.h"
  #include "aux_trm_cnfg.h"
  #include "..\..\..\Common\HMI\hmi.h"
  #include "iwdg.h"

//  #include "usart.h"
#include "vcp_srvcs.h"

/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "..\AUX_TERM\aux_term.h"
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
  extern uint32_t aux_term_stack_wm;
  extern uint32_t pwm_stack_wm; 
  extern uint32_t mon_stack_wm; 
  extern uint32_t tcp_ip_stack_wm; 
  extern uint32_t eth_if_stack_wm; 
  extern uint32_t rtu_master_stack_wm;
  extern uint32_t tcp_mb_stack_wm;
 
  
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */

uint32_t	   vcp_term_stack_wm;

/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */


/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  void usb_term_task_rx(void *pvParameters);
  void usb_term_task_tx(void *pvParameters);  
/* --- PRIVATE_DATA --------------------------------------------------------- */
  xQueueHandle   que_hndl_rx = NULL, que_hndl_tx = NULL;

  
 
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/

/*----------------------------------------------------------------------------*
  Puts in the terminal queue.
 *----------------------------------------------------------------------------*/

  portBASE_TYPE usb_term_tx(int8_t *msg_txt_ptr, int8_t msg_len){
   AUX_TERM_QUE_ITM_ST new_msg;
   portBASE_TYPE res = ERROR;
   
    if(!que_hndl_tx)
      return res;    
    
    new_msg.len = msg_len;
    new_msg.ptr = pvPortMalloc(new_msg.len); //+1 for the 0 termination
    
    if(new_msg.ptr){
      memcpy( new_msg.ptr, msg_txt_ptr, new_msg.len ); 
      res = xQueueSend( que_hndl_tx, &new_msg, 0 );
      if (res!=pdPASS){
        vPortFree(new_msg.ptr);
        while (1){;}        
      }else 
        res=OK;
        
    } else{
        while (1){;}
      
      }
    
    return res;
}/* usb_term_tx */


/*----------------------------------------------------------------------------*
  Called from main at application starting after HW init.
  Creates the input message queue and the task of Auxiliary Terminal.
  Returns: 0-success;
 *----------------------------------------------------------------------------*/
  portBASE_TYPE usb_term_init(unsigned portBASE_TYPE queue_len, unsigned portSHORT stack_depth, unsigned portBASE_TYPE priority){
   portBASE_TYPE res_rx = OK, res_tx = ERROR;
    
    que_hndl_tx = xQueueCreate(queue_len, sizeof( AUX_TERM_QUE_ITM_ST ));
    que_hndl_rx = xQueueCreate(queue_len, sizeof( AUX_TERM_QUE_ITM_ST ));    
    
    res_tx = xTaskCreate( usb_term_task_tx, "usb_tx", stack_depth, NULL, priority, NULL );
    res_rx = xTaskCreate( usb_term_task_rx, "usb_rx", stack_depth, NULL, priority, NULL );
    
    if( (que_hndl_rx == NULL)||(que_hndl_tx == NULL)||(res_tx != pdTRUE)||(res_rx != pdTRUE)){
      while(1){};
    }
      
    return pdTRUE;  
  }/* usb_terminal_init*/

/*----------------------------------------------------------------------------*

 *----------------------------------------------------------------------------*/  
  void vcp_print_msg(char * msg, uint32_t msg_len)
  {
    SendResponse ((uint8_t *)msg, msg_len);
  }
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*----------------------------------------------------------------------------*
  Main function of Receive VCP terminal task.
*----------------------------------------------------------------------------*/
  U32 usb_rx_stack_wm;
  U32 usb_tx_stack_wm;
  void usb_term_task_rx(void *pvParameters){
   AUX_TERM_QUE_ITM_ST vcp_trm_msg;
   
    while(1){
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
		usb_rx_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif      
      xQueueReceive(que_hndl_rx, &vcp_trm_msg, portMAX_DELAY);     
      xQueueSend( que_hndl_tx, &vcp_trm_msg, 0 );//dummy
    }/* while(1) */
   
  }/* usb_term_task_rx */
         
/*----------------------------------------------------------------------------*
  Main function of Transmit VCP terminal task.
*----------------------------------------------------------------------------*/
  
  void usb_term_task_tx(void *pvParameters){

   AUX_TERM_QUE_ITM_ST vcp_trm_msg;
   
    while(1){
      
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
		usb_tx_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif      
      xQueueReceive(que_hndl_tx, &vcp_trm_msg, portMAX_DELAY);  
      vcp_print_msg((char*)vcp_trm_msg.ptr,vcp_trm_msg.len);
      vPortFree(vcp_trm_msg.ptr);      
    }/* while(1) */
   
  } /* usb_term_task_tx */
         
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
 //   portBASE_TYPE xStatus;
//static  size_t free_heap_size;

//      free_heap_size = xPortGetFreeHeapSize(); 