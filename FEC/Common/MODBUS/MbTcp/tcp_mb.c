/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MODBUS_LIB 
*
*  FILE        : tcp_mb.c 
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS\MbTcp
*
*  PURPOSE     : Information Interchange between TCP and Modbus ports implementation.
*
*  TOOLCHAIN:  : IAR ARM Embedded Workbench 6.303 Evaluation
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
  #include <stdint.h>
  #include <string.h>
  #include "lwip/opt.h"

#if LWIP_NETCONN

  #include "lwip/sys.h"
  #include "lwip/api.h"
  #include "..\..\..\Common\MODBUS\modbus_exp.h"
  #include "cfg_db_if.h"
  #include "ll_typedef.h"
  #include "iwdg.h"
 #include "os_cnfg.h"

/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
//  extern MB_NODE_PT mb_node_1;
//  extern xSemaphoreHandle xSmphrRx;
extern uint8_t xSmphrRx;
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
  
  #define MB_TCP_RESP_SZ   300
/* --- PRIVATE MACROS ------------------------------------------------------- */

/* --- PRIVATE TYPES -------------------------------------------------------- */
/*
  typedef struct mbap_hdr_ {
    uint16_t   transaction_id;
    uint16_t   protocol_id;
    uint16_t   len;
    uint8_t    unit_id;
  }MBAP_HEADER_ST, *MBAP_HEADER_PST;
*/
/*
  typedef struct mb_tcp_adu {
    uint16_t   transaction_id;
    uint16_t   protocol_id;
    uint16_t   len;
    uint8_t    unit_id;
    uint8_t    func_code;
    REQ_DATA_UT data;    
  }MB_TCP_ADU_ST, *MB_TCP_ADU_PST;
*/
  
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  static void tcp_mb_thread(void *arg);
/* --- PRIVATE_DATA --------------------------------------------------------- */
  struct netconn   *newconn;
  static u8_t   mb_tcp_resp[MB_TCP_RESP_SZ];
  MBAP_HEADER_PST mbap_hdr_ptr = (MBAP_HEADER_PST) &(mb_tcp_resp[0]);
  MB_TCP_ADU_PST   adu_ptr;
  uint32_t  token_val = 0xFFFFFFFF;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
  * @brief   MbTcp Task Creation function.    
  * @param   : none;
  * @retval  res     : none.
***/
void tcp_mb_init(void){

  sys_thread_new("tcp_mb_thread", tcp_mb_thread, NULL, TCP_MB_TASK_STACK_SZ, TCP_MB_TASK_PRIORITY);
}/* void tcp_mb_init(void) */
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
static void mb_tcp_cbf(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val){

      switch(func_status){
      case 2:
        resp_buf[0] = adu_ptr->func_code | 0x80;       
        break;        
      default:        
        memcpy(&(mb_tcp_resp[6]),resp_buf, resp_sz-2);
        mbap_hdr_ptr->len = SWAP_16(resp_sz-2);
        netconn_write(newconn, mb_tcp_resp, (resp_sz-2) + 6 , NETCONN_COPY);        
        break;

    }/* switch(func_status) */
}
/******************************************************************************/
/**
  * @brief   MbTcp Task  function.    
  * @param   : none;
  * @retval  res     : none.
***/
uint32_t tcp_mb_stack_wm;
  static void tcp_mb_thread(void *arg){
   struct netconn *conn;
   err_t err;
   LL_STATUS  res;
   
   LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!=NULL){
    err = netconn_bind(conn, NULL, 502);/* Bind connection to well known port # 502. */
    
    if (err == ERR_OK){
      netconn_listen(conn);/* Tell connection to go into listening mode. */
    
      while (1){ 
        
        set_task_alive_bit(TCP_MB_ALV_E);
        
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
  tcp_mb_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif         
        newconn = netconn_accept(conn);/* Grab new connection. */
    
        if (newconn){ /* Process the new connection. */
          struct netbuf *buf;
          void *data;
          u16_t len;
      
          while ((buf = netconn_recv(newconn)) != NULL){ 

            do{ 
             
                netbuf_data(buf, &data, &len);
                adu_ptr = data;

                memcpy(mb_tcp_resp,data,4);
                
                if(adu_ptr->unit_id == THIS_UNIT){
                  res = db_req_hndr(adu_ptr->func_code, adu_ptr->data,mb_tcp_resp, newconn);
                }else{/* external slave */ 
                   switch(adu_ptr->func_code){                                
                     case HOLD_REGS_READ:
                       do{
                         res =  rtu_hold_regs_read( mb_node_1, adu_ptr->unit_id, SWAP_16(adu_ptr->data.single_reg_write_req.reg_addr), SWAP_16(adu_ptr->data.single_reg_write_req.reg_val), 0, 0, token_val, mb_tcp_cbf);
                       }while(res != LL_OK);
                       break;
                     case INPUT_REGS_READ:
                       do{
                         res =  rtu_input_regs_read(mb_node_1, adu_ptr->unit_id, SWAP_16(adu_ptr->data.single_reg_write_req.reg_addr), SWAP_16(adu_ptr->data.single_reg_write_req.reg_val), 0, 0, token_val, mb_tcp_cbf);                        
                       }while(res != LL_OK);
                       break;                       
                     case SINGLE_REG_WRITE:
                       do{
                         res = rtu_single_reg_write(mb_node_1, adu_ptr->unit_id, SWAP_16(adu_ptr->data.single_reg_write_req.reg_addr), SWAP_16(adu_ptr->data.single_reg_write_req.reg_val), 0, 0, token_val, mb_tcp_cbf);
                       }while(res != LL_OK);
                       break;
                     default:
                       res = 10;
                       break;
                   }/* switch(adu_ptr->func_code) */
                }/* external slave */ 
            }while (netbuf_next(buf) >= 0);
            
            netbuf_delete(buf);
          }
        
          /* Close connection and discard connection identifier. */
          netconn_close(newconn);
          netconn_delete(newconn);
        }
      }
    } else {
      printf(" can not bind TCP netconn");
    }
  }else{
         printf("can not create TCP netconn");
   }
}
#endif /* LWIP_NETCONN */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/



