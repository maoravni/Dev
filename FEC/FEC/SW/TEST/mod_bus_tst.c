/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : FEC  
*
*  FILE        : mod_bus_tst.c 
*   
*  PATH        :  
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.40.1.3812
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2013.01     Igor Natachanny   1.0.0   Initial version
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
  #include <string.h>
  #include "stm32f2xx.h"
  #include "free_rtos_exp.h"
  #include "ll_typedef.h"
  #include "GnrlCfgDB.h"
  #include "CfgDB.h"
#ifdef CPP_FRAMEWORK
#include "logger.h"
#endif
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include"rtu_master.h"
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
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
static void cbf_write_single_reg_test(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t  token_val);
/* --- PRIVATE_DATA --------------------------------------------------------- */
 static STATUS     TestResult = 0xAAAA5555; /* test not finished yet */
 static uint32_t   TestFinishFlag = 0;
 static u32 state = 0, tx_flag = 0;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*******************************************************************************
  * @brief: Modbus Test Initialization. 
  *
  * @param:      
  *
  * @retval: 
*******************************************************************************/
  void Modbus_TestInit(){
  
    state   = 0;
    tx_flag = 0;
  }
 
/*******************************************************************************
  * @brief: Modbus Test Monitor. 
  *
  * @param:      
  *
  * @retval: 
*******************************************************************************/  
  void rtu_master_tst_mntr(uint8_t slave_id, uint16_t reg_addr)//, uint16_t reg_val)
  {
    LL_STATUS res;
    static uint16_t reg_val = 10;
    
    switch(state){
      case 0:
        res = rtu_single_reg_write(mb_node_1, slave_id, reg_addr, reg_val, 0, 0, 0, cbf_write_single_reg_test);
        if(!res)  state = 1;        
        else state = 0;
      break;
      case 1:
        if(TestFinishFlag == 1){
          TestFinishFlag = 0;
          reg_val += 10; //+1 degree
          if(reg_val > 2000)
            reg_val = 10; 
          state = 0;
        }
//        else TestResult = 0xAAAA5555; /* test not finished yet */
      break;
    }
    
  }/************************** rtu_master_tst_mntr ****************************/
  
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
  static void cbf_write_single_reg_test(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t  token_val){
   uint16_t   reg_addr, reg_val, tmp_16;
   uint32_t   TestResult;

    TestFinishFlag = 1;
    switch(func_status){
      case 1:
        /* ERROR: Slave Responce CRC_16 FAULT */
        TestResult = TEST_FAIL;
        break;
      case 2:
        /* ERROR: Slave didn't answer! */
        TestResult = TEST_FAIL;
        break;
      case 3:
        /* ERROR: Function Exeption: incorrect function params or Function unsupported by slave! */
        TestResult = TEST_FAIL;
        break;        
      case 0:
       TestResult = 1;        
       memcpy( &tmp_16, &(resp_buf[2]), sizeof(uint16_t));
 //      reg_addr = SWAP_16(tmp_16);
       memcpy( &tmp_16, &(resp_buf[4]), sizeof(uint16_t));
//       reg_val  = SWAP_16(tmp_16);
//            sprintf( (char*)msg_buf,"\r SINGLE_REG_WRITE: slave_id = 0x%x, reg_addr = 0x%x, reg_val = %d - OK!", resp_buf[0], reg_addr, reg_val);
//            aux_term_put_msg(msg_buf);            
        break;
      default:
        /* ERROR: UNKNOWN ERROR! */
        break;
    }/* switch(func_status) */
    
    CfgPutParam(NULL, ID_GNRL_MNTR_INT_MODBUS_TEST_RESULT, &TestResult ,CfgDB_SET_TO_DB);
    
    vTaskDelay(1000);

 }/* static void cbf_write_single_reg_test(uint8_t func_status, uint8_t *resp_buf, uint8_t resp_sz) */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
 /*******************************************************************************
  * @brief : The Modbus Master Test Task function.       
  * @param   
  * @retval  
*******************************************************************************/
  void mb_master_test( void* pvParameters ){
   static   uint16_t tempr_set_point = 10; //1 degree
   LL_STATUS res;

   
    while (1){     
      if (tempr_set_point > 2000) 
        tempr_set_point = 10;     
          
      res = rtu_single_reg_write(mb_node_1,2,2, tempr_set_point, 0, 0, 0, cbf_write_single_reg_test);
//      vTaskDelay(1000);
//      res = rtu_hold_regs_read(mb_node_1, 2, 2, 1, 0, 0,0, NULL); 
//      res =  rtu_hold_regs_read( mb_node_1, 133, 0, 10, 0, 0, 0xA5/*token_val*/, NULL);
      tempr_set_point += 10; //+1 degree        

      vTaskDelay(1000);
    }
  }/*************** void mb_master_test( void* pvParameters ) *****************/
/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/