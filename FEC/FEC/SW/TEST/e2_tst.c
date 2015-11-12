/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : FEC 
*
*  FILE        : e2_tst.c
*   
*  PATH        :  C:\Landalabs\Spitfire\Embedded\Projects\FEC\SW\TEST  
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
*  2012.01     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
* 
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
#define E2_TST_IMPL
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
  #include "stm32f2xx.h"
  #include "ll_typedef.h"
  #include "cfg_db_eeprom.h"
  #include "e2_tst.h"
  #include "GnrlCfgDB.h"
  #include "CfgDB.h"
#include "logger.h"

/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
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
/* --- PRIVATE_DATA --------------------------------------------------------- */
  uint8_t str_rx[255];
  static uint32_t state = 2;
  static uint32_t EepromTestResult = TEST_FAIL;
  static uint32_t ee_addr;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
 void EEPROM_TestInit()
 {
   state = 2;
   ee_addr = EEPROM_TST_START_ADDR;
 }/*************************** void EEPROM_TestInit() *************************/
 
/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/ 
 void eeprom_tst()
  {
   uint32_t eeprom_res = ERROR;
   static uint32_t ee_val = 1, i;
   uint32_t ee_val_rx = 0x5A5A;
//   uint32_t tst_size = sizeof(str_ptrn) - 1;
   
     switch(state){
          case 0:/* INT_SET */
            eeprom_res = set_int_val(ee_addr, &ee_val);
         state = 1;
            if(!eeprom_res)
            M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"SET_INT_VAL - OK!: addr = %d, val = %d", ee_addr, ee_val);
            else
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"SET_INT_VAL - ERROR: addr = %d", ee_addr);
          break;
          case 1:/* INT_GET */
            eeprom_res = get_int_val(ee_addr, &ee_val_rx);
            if((!eeprom_res)&&(ee_val_rx == ee_val))
              M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"GET_INT_VAL - OK!: addr = %d, val = %d", ee_addr, ee_val);
            else
              M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"GET_INT_VAL - ERROR: addr = %d", ee_addr);
         state = 2;
          break;
          case 2:/************************ STR_SET ****************************/
            eeprom_res = set_str_val(ee_addr, (int8_t *)str_ptrn);
         if((!eeprom_res)){
              M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"SET_STRING_VAL - OK!: addr = %d", ee_addr);
           state = 3;
         }
         else{
           M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"SET_INT_VAL - ERROR: addr = %d", ee_addr);
           EepromTestResult = TEST_FAIL;
           CfgPutParam(NULL, ID_GNRL_MNTR_INT_EEPROM_TEST_RESULT, &EepromTestResult, CfgDB_SET_TO_DB);
           state = 2;
         }
          break;
          case 3:/************************ STR_GET ****************************/
            eeprom_res = get_str_val(ee_addr, (int8_t *)str_rx );
         for(i = 0; i < EEPROM_TST_PTRN_SZ; i++){
              if(str_rx[i] != str_ptrn[i]){
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"GET_STR_VAL - ERROR: pos = %d", i);
                EepromTestResult = TEST_FAIL;
                break;
              }
              if(i == 254){
                M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"GET_STRING_VAL - OK!: start_addr = %d ", ee_addr);
                EepromTestResult = TEST_PASS;
              }
            }              
//         ee_addr++;
//         ee_val++;
         state = 2;
         CfgPutParam(NULL, ID_GNRL_MNTR_INT_EEPROM_TEST_RESULT, &EepromTestResult, CfgDB_SET_TO_DB);
          break;          
     }/* switch(state) */

  }/*************************** void eeprom_tst() *****************************/
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
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
