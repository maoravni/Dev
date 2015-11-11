/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : tcp_mb.c 
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\APL
*
*  PURPOSE     : Information Interchange between TCP and Modbus ports implementation.
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
  #include <stdint.h>
  #include "lwip/opt.h"

#if LWIP_NETCONN

  #include "lwip/sys.h"
  #include "lwip/api.h"
#include "..\..\..\Common\MODBUS\modbus_exp.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
  extern MB_NODE_PT mb_node_1;
  extern xSemaphoreHandle xSmphrRx;
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
  #define TCP_MB_THREAD_PRIO  ( tskIDLE_PRIORITY + 2 )
/* --- PRIVATE MACROS ------------------------------------------------------- */

  #define word(HI,LW) ((uint16_t) (((uint16_t)HI<<8)| LW)) 
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  static void tcp_mb_thread(void *arg);
/* --- PRIVATE_DATA --------------------------------------------------------- */
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
void tcp_mb_init(void){

  sys_thread_new("tcp_mb_thread", tcp_mb_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCP_MB_THREAD_PRIO);
}/* void tcp_mb_init(void) */
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
  static void tcp_mb_thread(void *arg){
   struct netconn *conn, *newconn;
   err_t err;

    LWIP_UNUSED_ARG(arg);

    /* Create a new connection identifier. */
    conn = netconn_new(NETCONN_TCP);
  
    if (conn!=NULL){
      
      err = netconn_bind(conn, NULL, 502);/* Bind connection to well known port # 502. */   
      
      if (err == ERR_OK){      
        
        netconn_listen(conn);/* Tell connection to go into listening mode. */
    
        while (1){
        
          newconn = netconn_accept(conn);/* Grab new connection. */
    
          /* Process the new connection. */
          if (newconn){
            struct netbuf *buf;
            void *data;
            u16_t len;
            unsigned char slave_adr,op_code;
            u16_t val, reg_num;
            u8_t hi_byte, low_byte;
            u16_t i;
            
            while ((buf = netconn_recv(newconn)) != NULL){

                  netbuf_data(buf, &data, &len);
                  
                  for(i = 0; i < 4; i++)
                    mb_last_resp[i] = *((u8_t*)data + i);

                  slave_adr =  *((unsigned char*)data + 6);
                  op_code   = *((unsigned char*)data + 7);
//                  netconn_write(newconn, data, len, NETCONN_COPY);
                  /* Igor 18/04/2012 */
                  switch(op_code){        /* ModbusConfig Command Name */   
                    case COILS_READ:      /* Read Output Status  1    */
                      break;                     
                    case HOLD_REGS_READ:  /* Read Output Registers  3 */
                      hi_byte = *((u8_t*)data + 8);
                      low_byte = *((u8_t*)data + 9);
                      reg_num = word(hi_byte,low_byte); 
                      hi_byte = *((u8_t*)data + 10);
                      low_byte = *((u8_t*)data + 11);
                      val = word(hi_byte,low_byte);
                      ModbusbMaster_HoldRegsRead( mb_node_1, slave_adr, reg_num, val);
                      
                      break;
                    case INPUT_REGS_READ: /* Read Input Registers  4  */
                      break;
                    case SINGLE_COIL_READ: /* ???  = 5, */
                      break;                       
                    case SINGLE_REG_WRITE: /* Write single Register 6 */
                      hi_byte = *((u8_t*)data + 8);
                      low_byte = *((u8_t*)data + 9);
                      reg_num = word(hi_byte,low_byte); 
                      hi_byte = *((u8_t*)data + 10);
                      low_byte = *((u8_t*)data + 11);
                      val = word(hi_byte,low_byte);
                      ModbusbMaster_SingleRegWrite( mb_node_1, slave_adr, reg_num, val);
                      break;
                    default:
                      break;
                  }/* switch(op_code) */
                  
                  while (xSemaphoreTake(xSmphrRx, portMAX_DELAY) != pdPASS){
                  }
      
                  mb_last_resp[4] = (u8_t)(mb_last_resp_len>>8);
                  mb_last_resp[5] = (u8_t)(mb_last_resp_len & 0x00FF);                      
                  netconn_write(newconn, mb_last_resp, mb_last_resp_len+6, NETCONN_COPY);
                   
              netbuf_delete(buf);
              
            }/* while ((buf = netconn_recv(newconn)) != NULL) */
        
            /* Close connection and discard connection identifier. */
            netconn_close(newconn);
            netconn_delete(newconn);
          }/* if (newconn) */
        }/* while (1) */
      }/* if (err == ERR_OK) */
      else
        printf(" can not bind TCP netconn");
    }/* if (conn!=NULL) */
    else
      printf("can not create TCP netconn");
  }/* static void tcp_mb_thread(void *arg) */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/*----------------------------------------------------------------------------
  synop
 *----------------------------------------------------------------------------*/








/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
