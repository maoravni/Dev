/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MODBUS_LIB 
*
*  FILE        : rtu_master.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS\Rtu 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.06     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.
 
******************************************************************************/

#ifndef _RTU_MASTER_H_
#define _RTU_MASTER_H_


/* --- INCLUDES ------------------------------------------------------------ */
  #include"mb_cnfg.h"
  #include"rtu_msgs.h"
/* --- EXPORTED TYPES ------------------------------------------------------ */
  typedef enum rtu_func_err {
    RTU_FUNC_ERR_OK = 0,
    RTU_FUNC_ERR_CRC16,
    RTU_FUNC_ERR_NO_SLAVE_RESP,
    RTU_FUNC_ERR_EXEP = 3,
    RTU_FUNC_ERR_UNKNOWN = 0xFF
  }RTU_FUNC_ERR_ET;
  
  typedef struct _mb_master_node_descriptor_ {
    MB_NODE_NAME_ET       name;
    USART_TypeDef        *media_ptr;
    xQueueHandle          queue;        /* input message queue */
    xSemaphoreHandle      resp_rx_end_sem;
    uint8_t               resp_buf[255];//MB_RTU_RESP_ADU_UT    resp;
    int32_t               resp_len;
    xTaskHandle          *mb_task_handle;
    void                 *mb_task_params;
    uint8_t               retry_count;
  }MB_MASTER_NODE_ST, *MB_MASTER_NODE_PST;
  
  typedef struct _modbus_master_queue_item_ {
    uint8_t  slave_id;    
    MB_FUNC_CODE_ET  func_code;
    // Maor {1 Jan 2013}: Static allocation fix
    //REQ_DATA_UT *data_ptr;
    REQ_DATA_UT data_arr;
    uint32_t  data_size;
    uint32_t  token_val;    
    void (*call_back_fun)( uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val);
  }MB_MASTER_IN_QUE_ITM_ST, *MB_MASTER_IN_QUE_ITM_PST;
  
/* --- EXPORTED CONSTANTS -------------------------------------------------- */
  #define RETRIES_NUM  3
  #define THIS_UNIT   30
  extern MB_MASTER_NODE_PST mb_node_1;
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif

  uint32_t get_total_no_resp();
  uint32_t get_total_retries();
   LL_STATUS rtu_input_regs_read(MB_MASTER_NODE_PST node_ptr, uint8_t slave_id, uint16_t start_addr, uint16_t regs_quant, bool urgent, bool from_interrupt, uint32_t token_val, void (*cb_func)( uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val));
    LL_STATUS rtu_hold_regs_read(MB_MASTER_NODE_PST node_ptr, uint8_t slave_id, uint16_t start_addr, uint16_t regs_quant, bool urgent, bool from_interrupt, uint32_t token_val, void (*cb_func)( uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val));  
  LL_STATUS rtu_single_reg_write(MB_MASTER_NODE_PST node_ptr, uint8_t slave_id, uint16_t reg_addr,   uint16_t reg_val,    bool urgent, bool from_interrupt, uint32_t token_val, void (*cb_func)( uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val));
  signed portBASE_TYPE  mb_master_node_init(const signed char *const mb_task_name, unsigned short stack_depth, unsigned portBASE_TYPE priority, unsigned portBASE_TYPE que_len, MB_MASTER_NODE_PST node_ptr);
  MB_MASTER_NODE_PST mb_master_node_create(MB_NODE_NAME_ET mb_node_name);

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/
  #define RTU_MASTER_SEND_QUEU_TICK_TO_WAIT 200  //50 msec
  #define RTU_MASTER_SEM_RECV_TICK_TO_WAIT 100   //100msec
#ifdef __cplusplus
}
#endif
#endif /* _RTU_MASTER_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
