/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MODBUS_LIB 
*
*  FILE        : rtu_slave.h
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

#ifndef _RTU_SLAVE_H_
#define _RTU_SLAVE_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */
  typedef struct _modbus_slave_queue_item_ {
    uint8_t *req_ptr;
    int16_t  req_size;
    uint8_t *rsp_ptr;
    int16_t  rsp_size;    
    void* (*call_back_fun)();
  }MB_SLAVE_IN_QUE_ITM_ST, *MB_SLAVE_IN_QUE_ITM_PST;
   
  typedef struct _mb_slave_node_descriptor_ {
    MB_NODE_NAME_ET             name;
    USART_TypeDef              *media_ptr;
    xQueueHandle                queue;        /* input message queue */
    xSemaphoreHandle            resp_tx_end_sem;
    xTaskHandle                *mb_task_handle;
    void                       *mb_task_params;   
  }MB_SLAVE_NODE_ST, *MB_SLAVE_NODE_PST;
  
/* --- EXPORTED CONSTANTS -------------------------------------------------- */
  extern uint8_t   slave_addr;
  extern MB_SLAVE_NODE_PST mb_node_2;
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  signed portBASE_TYPE  mb_slave_node_init(const signed char *const mb_task_name, unsigned short stack_depth, unsigned portBASE_TYPE priority, unsigned portBASE_TYPE que_len, MB_SLAVE_NODE_PST node_ptr);
  MB_SLAVE_NODE_PST mb_slave_node_create(MB_NODE_NAME_ET mb_node_name);
  uint8_t set_slave_addr();
  uint8_t get_slave_addr();
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _RTU_SLAVE_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
