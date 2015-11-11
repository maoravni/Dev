/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MODBUS_LIB 
*
*  FILE        : rtu_slave.c
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
*  2012.06     Igor Natachanny   1.0.1   Incorrect crc16 handling - bug fixed.
*
*******************************************************************************
      Copyright and Proprietary Statment.
*******************************************************************************
See MODBUS Application Protocol Specification V1.1b  
 on http://www.Modbus-IDA.org for a reference.
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
#define  GET_NODE_ID_IMPL_HERE
#define  MB_INIT
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
  #include <stdio.h>
  #include <string.h>
#include <stm32f2xx.h>
  #include "free_rtos_exp.h"
  #include "..\..\AUX_TERM\aux_term.h"
  #include "usart.h"
  #include "leds_cnfg.h"
  #include "..\..\..\Common\HMI\hmi.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "rtu_msgs.h"
  #include "mb_cnfg.h"
  #include "..\Common\mb_infra.h"
  #include "rtu_slave.h"
#include <logger.h>

/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
uint16_t crc16_update(uint16_t crc, uint8_t a);
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
MB_SLAVE_NODE_PST mb_node_2;
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
//  #define SWAP_16(value16) (((uint16_t)value16<<8) | ((uint16_t)value16>>8))
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  typedef uint32_t (*mb_func_hndl)(void*);
  void rtu_slave_comm_task(MB_SLAVE_NODE_PST mb_node_ptr);
  uint16_t mb_pack_u16(uint16_t data);
  
/* --- PRIVATE_DATA --------------------------------------------------------- */
  uint8_t   slave_addr = 0;                 /* TBD should be moved to unit data base*/
  
  /* Debugging puporses local data */
  int8_t    txt_buf[256];
  uint16_t tst_regs_array[255];/* Debug purposes only temporary data storage */
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
 
/**
  * @brief    Creates a Modbus Slave Node at a selected media. 
  * @param    mb_node_name: Modbus node identificator.
  * @retval   node_ptr:     pointer to created node descriptor(NULL - if fault)
  */  
  MB_SLAVE_NODE_PST mb_slave_node_create(MB_NODE_NAME_ET mb_node_name){
   MB_SLAVE_NODE_PST node_ptr = NULL;
   
    node_ptr = pvPortMalloc(sizeof(MB_SLAVE_NODE_ST));
    
    if(node_ptr){
      node_ptr->name = mb_node_name;
      slave_addr = set_slave_addr();
      /* assigment to real connectivites */
      node_ptr->media_ptr = mb_media_asign[mb_node_name]; 
    }
    else
    {
      M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"mb_slave_node_create: error in pvPortMalloc");
      while (1)
      {
        ;
      }
          
    }
    
    return   node_ptr; 
  }/* mb_slave_node_create */
  
/**
  * @brief    Creates a communication handler task for a previously created Modbus Slave Node.
  * @brief    Once created a Slave should switched in RX mode.
  * @param    mb_task_name : just string of characters for debugging purposes.
  * @param    stack_depth  : the stack size of the task to be created.
  * @param    priority     : the priority of the task to be created.
  * @param    que_len      : the input queue size of the task to be created.
  * @param    node_ptr     : pointer to Modbus node descriptor.
  * @retval   res          : (ERROR - if fault)
  */  
    signed portBASE_TYPE
  mb_slave_node_init(
    const signed char       *const mb_task_name, 
    unsigned short           stack_depth, 
    unsigned portBASE_TYPE   priority, 
    unsigned portBASE_TYPE   que_len, 
    MB_SLAVE_NODE_PST        node_ptr)
  {   
   signed portBASE_TYPE res = ERROR;   
   
   if(node_ptr){
     
     vSemaphoreCreateBinary(node_ptr->resp_tx_end_sem);
     if(node_ptr->resp_tx_end_sem)
      xSemaphoreTake(node_ptr->resp_tx_end_sem, portMAX_DELAY);
     
     node_ptr->queue = xQueueCreate( que_len, sizeof(MB_SLAVE_IN_QUE_ITM_ST));   
     if(node_ptr->queue){ 
       res = xTaskCreate( (pdTASK_CODE)rtu_slave_comm_task, mb_task_name, stack_depth, (void *)node_ptr, priority, node_ptr->mb_task_handle );
       if(res)
         mb_set_dir(node_ptr->name, RX);
       else{
         vQueueDelete(node_ptr->queue); /* Delete the node queue and release   */
         vPortFree(node_ptr);           /* taken by a node descriptor memory   */   
                                        /* if a node communication task isn't created */
       }
     }                        
     
     
   }/* if(node_ptr) */
   
   return res;
  }/*** mb_slave_node_create ***/
  
/**
  * @brief    A main function of Modbus Slave node communication Task. 
  * @param    mb_node_ptr: a pointer to Modbus node descriptor.
  * @retval   None.
  */  
  void rtu_slave_comm_task(MB_SLAVE_NODE_PST mb_node_ptr){
   uint16_t                  crc16_val;
   MB_RTU_REQ_ADU_PUT        req_adu_ptr;
   MB_RTU_RESP_ADU_PUT       resp_ptr;
   uint16_t                  reg_addr, reg_val,start_addr,regs_quant,k;
   uint8_t                   byte_count;
   uint32_t                  resp_size;
   MB_SLAVE_IN_QUE_ITM_ST    cur_req;
   uint8_t                   i = 1;
   
    while(1){
    
      xQueueReceive(mb_node_ptr->queue, (MB_SLAVE_IN_QUE_ITM_PST)(&cur_req), portMAX_DELAY);
      
      led_toggle(MODBUS_LED);
      
      req_adu_ptr  = (MB_RTU_REQ_ADU_PUT)cur_req.req_ptr;      
      crc16_val = crc16_calc(cur_req.req_ptr, cur_req.req_size);   
      
      if(req_adu_ptr->adu.crc16_val != crc16_val)
        continue;/* do nothing if crc16 fault according to MODBUS Application Protocol Specification V1.1b*/
      
      resp_ptr = (MB_RTU_RESP_ADU_PUT)cur_req.rsp_ptr;   
      

      switch(req_adu_ptr->adu.func_code){
        case COILS_READ:
          break;
        case HOLD_REGS_READ:
//          break;
        case INPUT_REGS_READ:
          /*** Unpack command data ***/
          start_addr = SWAP_16(req_adu_ptr->adu.req_data.input_regs_read_req.start_addr);
          regs_quant = SWAP_16(req_adu_ptr->adu.req_data.input_regs_read_req.regs_quant);
          
/*** Sanity check and exeption pdu assembling        ***/
          if((start_addr < 1)||(start_addr > 254)){
            resp_ptr->adu.pdu.exep.error_code = 0x84;
            resp_ptr->adu.pdu.exep.exept_code = ILG_REG_ADDR;
            resp_size = 4;
          }/* if reg_addr is WRONG */
          else{
            if((regs_quant < 1)||((start_addr+regs_quant) > 255)){
              resp_ptr->adu.pdu.exep.error_code = 0x84;
              resp_ptr->adu.pdu.exep.exept_code = ILG_REG_VAL;
              resp_size = 4;
            }/* if regs_quant is WRONG */
            else{
/*** Function processing and responce pdu assembling        ***/
              i = 1;              
              memcpy( &(resp_ptr->buf[i]),&(req_adu_ptr->adu.func_code), sizeof(uint8_t));
              i += sizeof(uint8_t);
              byte_count =regs_quant * 2;;
              memcpy( &(resp_ptr->buf[i]),&byte_count, sizeof(uint8_t));
              i += sizeof(uint8_t);
              for(k = start_addr; k < (start_addr + regs_quant); k++){
                reg_val = SWAP_16(tst_regs_array[k]);
                // For continiously read test : tst_regs_array[k]++;
                memcpy( &(resp_ptr->buf[i]),&(reg_val), ( sizeof(uint16_t)));
                i += (sizeof(uint16_t));
              }
              resp_size = byte_count + 5;
            }
          }/* if reg_addr is WRONG */
          break;
          
        case SINGLE_COIL_READ:
          break;
          
        case SINGLE_REG_WRITE:         
/*** Unpack command data ***/
          reg_addr = SWAP_16(req_adu_ptr->adu.req_data.single_reg_write_req.reg_addr);
          reg_val = SWAP_16(req_adu_ptr->adu.req_data.single_reg_write_req.reg_val);
          
/*** Sanity check and exeption pdu assembling        ***/
          if((reg_addr < 1)&&(reg_addr > 254)){
            resp_ptr->adu.pdu.exep.error_code = 0x86;
            resp_ptr->adu.pdu.exep.exept_code = ILG_REG_ADDR;
            resp_size = 4;
          }/* if reg_addr is WRONG */
          else{
            if((reg_val < 10)&&(reg_val > 2000)){
              resp_ptr->adu.pdu.exep.error_code = 0x86;
              resp_ptr->adu.pdu.exep.exept_code = ILG_REG_VAL;
              resp_size = 4;
            }/* if reg_val is WRONG */
            else{
/*** Function processing and responce pdu assembling        ***/
              tst_regs_array[reg_addr] = reg_val;
              i = 1;              
              memcpy( &(resp_ptr->buf[i]),&(req_adu_ptr->adu.func_code), sizeof(uint8_t));
              i += sizeof(uint8_t);
              memcpy( &(resp_ptr->buf[i]),&(req_adu_ptr->adu.req_data.single_reg_write_req.reg_addr), sizeof(uint16_t));
              i += sizeof(uint16_t);
              memcpy( &(resp_ptr->buf[i]),&(req_adu_ptr->adu.req_data.single_reg_write_req.reg_val), sizeof(uint16_t));
              i += sizeof(uint16_t);
//              i--;
              resp_size = 8;
            }
          }/* if reg_addr is WRONG */
          break;
        default:
          break;          
      }/* switch */
      
/*** Function processing and responce pdu assembling        ***/
      resp_ptr->adu.dev_adr = slave_addr;
      
      crc16_val = crc16_calc(cur_req.rsp_ptr, resp_size - 2);      

      memcpy( &(resp_ptr->buf[i]),&(crc16_val), sizeof(uint16_t));
           
      mb_set_dir(mb_node_ptr->name, TX);
      
      send_buf((uint8_t*)resp_ptr, resp_size, mb_node_ptr->media_ptr);
      
      xSemaphoreTake(mb_node_2->resp_tx_end_sem, portMAX_DELAY);
      
      led_toggle(MODBUS_LED);

      mb_set_dir(mb_node_ptr->name, RX);
      
//      sprintf((char*)txt_buf,"\r\nreg_wr: reg_add = %d; reg_val =%d",reg_addr,reg_val);      
//      aux_term_put_msg((int8_t*)txt_buf);      

    }/* while(1) */
    
  }/*************************** rtu_slave_comm_task ***************************/  

  /**
  * @brief    Assemble and return a Modbus Slave Node Address from a most X bits of a prede- 
  * @brief    fined value MB_SLAVE_ADDR_TEMPLATE and leasts Y bits from on-board switch. 
  * @param    None.
  * @retval   slave_addr: assembled node address.
  */
  uint8_t set_slave_addr(){
   uint8_t node_id_bit =  NODE_ID0;
   uint8_t slave_addr  =  0x00;
    
    for(node_id_bit =  NODE_ID0; node_id_bit < NUMBER_OF_NODEIDs; node_id_bit++){    
      if(GPIO_ReadInputDataBit(PortOf[node_id_bit], PinOf[node_id_bit]))
        slave_addr |= (1<<node_id_bit); 
    }/*for*/
    
    slave_addr |= MB_SLAVE_ADDR_TEMPLATE;
    
    return slave_addr;

  }/* set_slave_adr */
 
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
  /**
  * @brief    Prepares a 16-bit data to be sended on MModbus. 
  * @param    16-bit data word to be swapped.
  * @retval   swapped 16-bit data.
  */
  uint16_t mb_pack_u16(uint16_t data){
    return SWAP_16(data);    
  }/* uint16_t mb_pack_u16(uint16_t data) */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
  
