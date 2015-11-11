/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : AUX_TERM_LIB 
*
*  FILE        : aux_term.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\AUX_TERM 
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
*  2012.07     Igor Natachanny   1.1.0   LED_UART_TX blinking added.
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
#include <stm32f2xx.h>
  #include "free_rtos_exp.h"
  #include "leds_cnfg.h"
  #include "aux_trm_cnfg.h"
  #include "..\..\..\Common\HMI\hmi.h"
  #include "iwdg.h"

  #include "usart.h"

#ifdef CPP_FRAMEWORK
#include "logger.h"
#endif


/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "aux_term.h"
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
  extern uint32_t aux_term_stack_wm;
  extern uint32_t pwm_stack_wm; 
  extern uint32_t mon_stack_wm; 
  extern uint32_t tcp_ip_stack_wm; 
  extern uint32_t eth_if_stack_wm; 
  extern uint32_t rtu_master_stack_wm;
  extern uint32_t tcp_mb_stack_wm;
  extern uint32_t usb_rx_stack_wm;
  extern uint32_t usb_tx_stack_wm;
 
  
  extern uint32_t clogger_stack_wm;
  extern uint32_t ctcpconnector_stack_wm; 
  extern uint32_t periodicmonitor_stack_wm;
  extern uint32_t tmcmasterserver_stack_wm;  
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */

uint32_t	   aux_term_stack_wm;

/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
//  typedef struct _aux_term_queue_item_ {
//    int8_t *ptr;
//    uint8_t len;
//  }AUX_TERM_QUE_ITM_ST, *AUX_TERM_QUE_ITM_PST;

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  void aux_term_task(void *pvParameters);
  
/* --- PRIVATE_DATA --------------------------------------------------------- */
//  int8_t  ok_msg_txt[] = "\r\naux_term started!\r\n";
  xQueueHandle   queue_handle = NULL;
  xTaskHandle    task_handle;
  
 
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*----------------------------------------------------------------------------*
  Outputs a Water Mark of a tassk stack.
 *----------------------------------------------------------------------------*/
  static char pcWriteBuffer[500];
  void  disp_stacks_wm(){

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"tcp_ip_stack_wm = %d; eth_if_stack_wm=%d; tcp_mb_stack_wm = %d ",
                   tcp_ip_stack_wm,
                   eth_if_stack_wm,
                   tcp_mb_stack_wm);
                                  
  
    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"rtu_master_stack_wm = %d; aux_term_stack_wm = %d;pwm_stack_wm=%d,mon_stack_wm=%d,usb_rx_stack_wm=%d,usb_tx_stack_wm=%d",
                   rtu_master_stack_wm,
                   aux_term_stack_wm,
                   pwm_stack_wm,
                   mon_stack_wm,
                   usb_rx_stack_wm,
                   usb_tx_stack_wm);
    
    
    
    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"clogger_stack_wm = %d; ctcpconnector_stack_wm = %d; periodicmonitor_stack_wm = %d; tmcmasterserver_stack_wm = %d;",
                   clogger_stack_wm,
                   ctcpconnector_stack_wm,
                   periodicmonitor_stack_wm,
                   tmcmasterserver_stack_wm,
                   tmcmasterserver_stack_wm
                     );    
    
  }/* disp_stacks_wm */
/*----------------------------------------------------------------------------*
  Outputs a size of currently free part of a heap memory.
 *----------------------------------------------------------------------------*/
  void  disp_free_heap_sz(){
   size_t free_heap_size;
   
    //free_heap_size = xPortGetFreeHeapSize();
  }/* disp_free_heap_sz */
/*----------------------------------------------------------------------------*
  Puts in the terminal queue.
 *----------------------------------------------------------------------------*/

portBASE_TYPE AuxTermSendMsg(int8_t *msg_txt_ptr, int8_t MsgLen)
{
   AUX_TERM_QUE_ITM_ST new_msg;
   portBASE_TYPE res = ERROR;
   
    if(!queue_handle)
      return res;    
    
    new_msg.len=MsgLen;
    new_msg.ptr = pvPortMalloc(new_msg.len); //+1 for the 0 termination
    
    if(new_msg.ptr){
      memcpy( new_msg.ptr, msg_txt_ptr, new_msg.len ); 
      res = xQueueSend( queue_handle, &new_msg, 0 );
      if (res!=pdPASS)
      {
        vPortFree(new_msg.ptr);
        while (1)
        {
          ;
        }
        
      } else res=OK;
        
    }
    else
    {
      while (1)
      {
        ;
      }
      
    }
    
    return res;
}    //AuxTermSendMsg


/*----------------------------------------------------------------------------*
  Called from main at application starting after HW init.
  Creates the input message queue and the task of Auxiliary Terminal.
  Returns: 0-seccess;
 *----------------------------------------------------------------------------*/
  portBASE_TYPE aux_term_init(unsigned portBASE_TYPE queue_len, unsigned portSHORT stack_depth, unsigned portBASE_TYPE priority){
   portBASE_TYPE res = ERROR;
    
    queue_handle = xQueueCreate(queue_len, sizeof( AUX_TERM_QUE_ITM_ST ));    
    if( queue_handle == NULL )
     return ERROR;
    
    /* Create a task only if a queue was successfully created */
    res = xTaskCreate( aux_term_task, "aux_term", stack_depth, NULL, priority, &task_handle );
    
    if(res == pdTRUE)
      M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO,"Aux_term started!");  

    return res;
    
  }/* aux_terminal_init*/
  
  
  void AuxPrintMsg(char * Msg, uint8_t MsgLen)
  {
    send_buf((uint8_t *)Msg, MsgLen, AUX_TRM_USART); 
  }
  
  void AuxPrintStrMsg(char * Msg)
  {
    send_buf((uint8_t *)Msg, strlen(Msg), AUX_TRM_USART); 
  }
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*----------------------------------------------------------------------------*
  Main function of auxiliary terminal task.
*----------------------------------------------------------------------------*/
  void aux_term_task(void *pvParameters){
//   portBASE_TYPE xStatus;
static  size_t free_heap_size;
   AUX_TERM_QUE_ITM_ST aux_trm_msg;
   
    while(1){
set_task_alive_bit(AUX_TERM_ALV_E);   
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
  aux_term_stack_wm = uxTaskGetStackHighWaterMark(task_handle);
#endif
      free_heap_size = xPortGetFreeHeapSize(); 
      xQueueReceive(queue_handle, &aux_trm_msg, portMAX_DELAY);  
      AuxPrintMsg((char*)aux_trm_msg.ptr,aux_trm_msg.len);
      vPortFree(aux_trm_msg.ptr);
     
     // led_toggle(LED_UART_TX);
      
    }/* while(1) */
   
  } /* aux_term_task */
         
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
