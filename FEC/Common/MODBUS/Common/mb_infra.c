/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : Modbus.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS\Common\src  
*
*  PURPOSE     :  Modbus channel infrastructure related implementation.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.08     Igor Natachanny   1.0.1   Bug fixed: Incorrect parameter value in
*                                        crc16_calc function - handling added.
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
  #include "free_rtos_exp.h"
//  #include "..\..\..\..\..\Phantom\SW\BSP\bsp_srvcs.h"


/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "mb_cnfg.h"
  #include "mb_infra.h"
  #include "..\RTU\rtu_slave.h"
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

/* --- PRIVATE TYPES -------------------------------------------------------- */

  typedef enum _modbus_media_type_ {
    UART = 0,
    ETHRNT
/* next modbus media type should added here */
  }MB_MEDIA_TYPE_ET;

/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
  

  
/* --- PRIVATE MACROS ------------------------------------------------------- */

/* --- PRIVATE_DATA --------------------------------------------------------- */
  

//  USART_TypeDef *USARTx;
  
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  
//  static void mb_comm_task( void *pvParameters  );
  
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/

/*----------------------------------------------------------------------------*
  Calculate and return a CRC-16 value for "len" bytes started from "ptr". 
 *----------------------------------------------------------------------------*/
  uint16_t crc16_calc(uint8_t *ptr, int32_t len){
   uint32_t i = 0,k = 0;
   uint16_t crc16_val = 0xFFFF;
   
    if(( len < 0)||(len > MB_BUF_SZ)){
      return crc16_val;
    }
   
    for (k = 0; k < len; k++){
      
      crc16_val ^= *(ptr + k);
      
      for (i = 0; i < 8; ++i){
        if (crc16_val & 1)
            crc16_val = (crc16_val >> 1) ^ 0xA001;
        else
            crc16_val = (crc16_val >> 1);
      }
    }
   
    return crc16_val;
  }/* crc16_calc */

 
/*----------------------------------------------------------------------------*
   Sets the 'wrk_mode'('TX'|'RX') on the 'mb_node'('MB_1'|'MB_2'). 
 *----------------------------------------------------------------------------*/
  
  uint32_t mb_set_dir(MB_NODE_NAME_ET mb_node_name, uint32_t wrk_mode){

    switch(mb_node_name){
      case MB_2:
        if(wrk_mode == TX){
          DE2_PORT->BSRRL = DE2_PIN;  /* DE Hight */          
          RE2_PORT->BSRRL = RE2_PIN;  /* RE Hight */
        }else{
          DE2_PORT->BSRRH = DE2_PIN;  /* DE Low */          
          RE2_PORT->BSRRH = RE2_PIN;  /* RE Low */
      }
      break;
      case MB_1:
        if(wrk_mode == TX){
          DE_PORT->BSRRL = DE_PIN;  /* DE2 Hight */
          RE_PORT->BSRRL = RE_PIN;  /* RE2 Hight */
        }else{
          DE_PORT->BSRRH = DE_PIN;  /* DE2 Low */
          RE_PORT->BSRRH = RE_PIN;  /* RE2 Low */
        }
        break;
      default:
      return 1;
    }
    
    if(wrk_mode == RX){
      USART_ITConfig(mb_media_asign[mb_node_name], USART_IT_RXNE, ENABLE);
//      USART_ITConfig(mb_media_asign[mb_node_name], USART_IT_IDLE, ENABLE);
    }else{
       USART_ITConfig(mb_media_asign[mb_node_name], USART_IT_RXNE, DISABLE);
       USART_ITConfig(mb_media_asign[mb_node_name], USART_IT_IDLE, DISABLE);
     }
    
    return 0;
   
  }/******************************* mb_set_dir ********************************/
  
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/

/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
