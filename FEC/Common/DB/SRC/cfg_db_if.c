/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : cfg_db_if_init.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\DB\SRC 
*
*  PURPOSE     : Configuration Data Base Interface init procedures.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : Phantom board. 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.08     Igor Natachanny   1.0.0   Initial version
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
  #include <stdint.h>
  #include <string.h>
  #include "ll_typedef.h"
  #include "free_rtos_exp.h"
  #include "rtu_master.h"
  #include "modbus_exp.h"
  #include "..\..\..\Common\MODBUS\MbTcp\tcp_mb.h"
//#if LWIP_NETCONN
  #include "lwip/sys.h"
  #include "lwip/api.h"
//#endif /* LWIP_NETCONN */
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "GnrlCfgDB.h"
  #include "TemperatureCfgDB.h"
  #include "OthersCfgDB.h"
  #include "BtcCfgDB.h"
  #include "CfgDB.h"
#include "cfg_db_if.h"
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
  LL_STATUS db_req_hndr(uint8_t func_code, REQ_DATA_UT data, uint8_t *mb_tcp_resp, struct netconn *newconn);
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  uint8_t set_mb_exep_code(STATUS db_err_code);
  CFG_DB_IF_ENTRY_PS db_entry_search(uint16_t  mb_reg);
/* --- PRIVATE_DATA --------------------------------------------------------- */
  #define NR  NULL
  #define CFG_DB_IF_CNFG
  #include "cfg_db_if_cnfg.h"
  CFG_DB_IF_ENTRY_PS   cfg_db_if_data_ptr = NULL;
  // Maor {27 Dec 2012}: removed the static in order to be able to use this in the TMC Server.
  uint32_t num_of_entries = 0;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
  * @brief: Handles the Data Base requests. 
  *
  * @param:
  *
  * @retval: 
**/
  LL_STATUS db_req_hndr(uint8_t func_code, REQ_DATA_UT data, uint8_t *mb_tcp_resp, struct netconn *newconn){
   LL_STATUS  res;
   STATUS db_res;
   CFG_DB_IF_ENTRY_PS  req_entry_ptr;
   uint16_t data_val, i, resp_sz, start_addr,regs_quant, data_16;
   MBAP_HEADER_PST mbap_hdr_ptr = (MBAP_HEADER_PST) mb_tcp_resp;
   uint32_t data_32;
   uint8_t exept_code;
   
   mbap_hdr_ptr->unit_id = THIS_UNIT;
      
    switch(func_code){
      case HOLD_REGS_READ:                  
        start_addr = SWAP_16(data.hold_regs_read_req.start_addr);/*** Unpack command data ***/
        regs_quant = SWAP_16(data.hold_regs_read_req.regs_quant);
        for(i = 0; i < regs_quant; i++ ){
          req_entry_ptr = db_entry_search(start_addr + i);
          if(!req_entry_ptr){ db_res = 1008; break;}    
          db_res = CfgGetParam(req_entry_ptr->instance, CfgDB_ID_AddIndex(req_entry_ptr->db_id,req_entry_ptr->db_id_inx), &data_32, sizeof(U32), CfgDB_GET_FROM_DB);  
           if(!db_res){
            data_val = data_16 = data_32;
            data_val = SWAP_16(data_16); 
            memcpy( (mb_tcp_resp + 9 + i*2), &(data_val), sizeof(uint16_t));
          }else break;
        }
        resp_sz = sizeof(MBAP_HEADER_ST) - 1 ;
        if(!db_res){/* build success responce */
          memcpy( (mb_tcp_resp + resp_sz), &(func_code), sizeof(uint8_t));
          i = regs_quant * 2;
          memcpy( (mb_tcp_resp + resp_sz + 1 ), &(i), sizeof(uint8_t));
          resp_sz += i;
        }else{/* build fault exeption */
           resp_sz = sizeof(MBAP_HEADER_ST) - 1 ;
           func_code |= 0x80;
           memcpy((mb_tcp_resp + resp_sz), &(func_code), sizeof(uint8_t));
           resp_sz++;
           exept_code = set_mb_exep_code(db_res);
           memcpy( (mb_tcp_resp + resp_sz), &(exept_code), sizeof(uint8_t));
           resp_sz = 3;
           res = LL_ERROR;
        }
        break;
      case INPUT_REGS_READ:
//        db_res = CfgGetParam(int InstanceId,CfgDB_INT_DATA id,void * Data, int DataLen,CfgDB_GET_FLAG GetFlag);
        break;
      case SINGLE_REG_WRITE:
        start_addr = SWAP_16(data.single_reg_write_req.reg_addr);
        data_val = SWAP_16(data.single_reg_write_req.reg_val);
        data_32 = data_val;
        req_entry_ptr = db_entry_search(start_addr);
        if(req_entry_ptr != NULL)        
          db_res = CfgPutParam(req_entry_ptr->instance, CfgDB_ID_AddIndex(req_entry_ptr->db_id,req_entry_ptr->db_id_inx), &data_32, CfgDB_SET_TO_DB);
        else 
          db_res = 1008;/* should be replaced with appropriate one */
        
        resp_sz = sizeof(MBAP_HEADER_ST) - 1 ;
        if(!db_res){/* build success responce */
          memcpy( (mb_tcp_resp + resp_sz), &(func_code), sizeof(uint8_t));
          memcpy( (mb_tcp_resp + resp_sz + 1 ), &(data.single_reg_write_req.reg_addr), sizeof(uint16_t));
          memcpy( (mb_tcp_resp + resp_sz + 3 ), &(data.single_reg_write_req.reg_val) , sizeof(uint16_t));
          resp_sz = 6;
          res = 0;
        }else{/* build fault exeption */
           resp_sz = sizeof(MBAP_HEADER_ST) - 1 ;
           func_code |= 0x80;
           memcpy((mb_tcp_resp + resp_sz), &(func_code), sizeof(uint8_t));
           resp_sz++;
           exept_code = set_mb_exep_code(db_res);
           memcpy( (mb_tcp_resp + resp_sz), &(exept_code), sizeof(uint8_t));
           resp_sz = 3;
           res = LL_ERROR;
          }
        break;
    }
   
    mbap_hdr_ptr->len = SWAP_16((resp_sz - 4));
    if (newconn != NULL) {
        netconn_write(newconn, mb_tcp_resp, resp_sz + 2 , NETCONN_COPY);
        return res;
    }
    else {
        return resp_sz+2;
    }

    
  }/******* LL_STATUS db_req_hndr(uint8_t func_code, REQ_DATA_UT data) ********/
  
/**
  * @brief: Moves to RAM and sorts the Configuration Data Base Interface Data.   
  *
  * @param: none.     
  *
  * @retval: none.
**/
  STATUS_ET 
    cfg_db_if_init(){
   uint32_t i,k;
   uint32_t cnfg_data_size = sizeof(cfg_db_if_data)/sizeof(CFG_DB_IF_ENTRY_ST);
   uint32_t tmp;
   uint32_t max_mb_reg;
   uint8_t *mark_ptr = NULL;

#define FREE  0x5A
#define TAKEN 0x00
   
    cfg_db_if_data_ptr = (CFG_DB_IF_ENTRY_PS)pvPortMalloc((sizeof(CFG_DB_IF_ENTRY_ST) * cnfg_data_size));
    if(cfg_db_if_data_ptr == NULL)
          return NO_ROOM_E;
    
    mark_ptr = (uint8_t *)pvPortMalloc((sizeof(uint8_t ) * cnfg_data_size));
    if(mark_ptr == NULL){
      vPortFree(cfg_db_if_data_ptr);
      return NO_ROOM_E;
    }else
       memset(mark_ptr, FREE, /*size_t*/ cnfg_data_size);
   
    for(i = 0; i < cnfg_data_size; i++){
      k = 0;
      max_mb_reg = 0;
      
      for(k = 0; k < cnfg_data_size; k++){
        if((cfg_db_if_data[k].mb_reg > max_mb_reg)&&(*(mark_ptr + k) == FREE)){
          tmp = k;
          max_mb_reg = cfg_db_if_data[k].mb_reg;
        }
      }/* while */
            
      memcpy( (cfg_db_if_data_ptr + i), &(cfg_db_if_data[tmp]), sizeof(CFG_DB_IF_ENTRY_ST) );
      *(mark_ptr + tmp) = TAKEN;
    }/* for */
    
    vPortFree(mark_ptr);
    num_of_entries = cnfg_data_size;
    
    return OK_E;
    
  }/**************************** cfg_db_if_init *******************************/
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
    /**
  * @brief:  Sets Modbus Exeption Code according to received DB Error Code.   
  *
  * @param:  DB Error Code.  
  *
  * @retval:  Modbus Exeption Code.
**/
  uint8_t set_mb_exep_code(STATUS db_err_code){
   uint8_t exep_code;
    
    switch(db_err_code){
      case CFG_DB_ERROR_ID_LIMIT:                /* 1000 TBD replace the 1000 with global value*//* illegal id requeset */
      case CFG_DB_ERROR_ID_INDEX_LIMIT:          /* 1001 id index requeset is rejected by its suitable id descriptor    */
      case CFG_DB_ERROR_ID_DATA_TYPE:            /* 1002 illegal requesed data type                                     */
      case CFG_DB_ERROR_ID_ACCESS_TYPE:          /* 1003 illegal requesed access type                                   */
      case CFG_DB_ERROR_ID_UNIT_TYPE:            /* 1004 illegal requesed unit type                                     */
      case CFG_DB_ERROR_ID_INSTANCE_NUM:         /* 1005 illegal requesed instance number                               */
      case CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG:     /* 1006 set flag is not suitable to the operation                      */
      case CFG_DB_ERROR_ID_ILLEGAL_DATA_ELEMENT: /* 1007 data element is rejected by its descriptor                     */
        exep_code = 3;
        break;
      case CFG_DB_ERROR_ID_ILLEGAL_STR_LEN:      /* 1008 string element is rejected because it's too long               */
      case CFG_DB_ERROR_ID_HANDLE_RESULT:        /* 1009 if handle routine not return OK                                */
      case CFG_DB_ERROR_ID_HANDLE_BUSY:          /* 1010 if handle routine not completed the previous call              */
      case CFG_DB_ERROR_ID_INFORM_BUSY:          /* 1011 if queue is full                                               */
      case CFG_DB_ERROR_ID_CONFIG_MISMATCH:      /* 1012 */
        exep_code = 2;
        break;
    }
    
    return exep_code;
  }/************************** set_mb_exep_code *******************************/

  /**
  * @brief: Seeks into the DB for required entry by received Modbus Register address.   
  *
  * @param: Modbus Register Address.  
  *
  * @retval: Pointer to required DB entry.
**/
  CFG_DB_IF_ENTRY_PS 
    db_entry_search(uint16_t  mb_reg){
   CFG_DB_IF_ENTRY_PS req_db_entry_ptr = cfg_db_if_data_ptr;
   int32_t curr_db_offset, max_offset = num_of_entries, min_offset = 0;
   
    NEXT_ITER:  
    curr_db_offset = (max_offset + min_offset)/2;  
    
    req_db_entry_ptr = cfg_db_if_data_ptr + curr_db_offset;
   
    if(req_db_entry_ptr->mb_reg == mb_reg){
      return req_db_entry_ptr;/* Stop the seach - Required entry is found; */
    }else{
       if((max_offset - 1) == min_offset )
         return  NULL;/* Stop the seach - Required entry is not found; */      
       if(req_db_entry_ptr->mb_reg > mb_reg){       
         min_offset = curr_db_offset;/* continue to search in upper half of DB*/
       }else{
          if(req_db_entry_ptr->mb_reg < mb_reg){
            max_offset = curr_db_offset;/* continue to search in lower half of DB*/
          }
      }
      goto NEXT_ITER;
     }

#if 0
   CFG_DB_IF_ENTRY_PS req_db_entry_ptr = cfg_db_if_data_ptr;
   int32_t curr_db_offset = 0;

   while((req_db_entry_ptr->mb_reg != mb_reg)&&(curr_db_offset < num_of_entries)){
     curr_db_offset++;
     req_db_entry_ptr = cfg_db_if_data_ptr +curr_db_offset;
   }
   if(req_db_entry_ptr->mb_reg == mb_reg)
     return req_db_entry_ptr;
   else return NULL;

#endif  
  }/************************* db_entry_search *********************************/
    
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
//  typedef union{
//  }DB_ID_UT, *DB_ID__PU;
#if 0
    
      case 0:

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
        *(mb_tcp_resp + 7) = func_code;
        memcpy((mb_tcp_resp + 8),&data_val, sizeof(uint16_t));
        mbap_hdr_ptr->len = SWAP_16(i+2);
        break;

    }
#endif
