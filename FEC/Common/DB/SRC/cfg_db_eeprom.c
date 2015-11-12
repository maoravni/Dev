/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PRM_DB_LIB 
*
*  FILE        : cfg_db_backup.c
*   
*  PATH        :  C:\Landalabs\Spitfire\Embedded\Projects\Common\DB\SRC
*
*  PURPOSE     :  Configuration parameters Data Base saving in Backup SRAM.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.09     Igor Natachanny   1.0.0   Initial version
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
  #include <stdint.h>
  #include <string.h>
  #include "ll_typedef.h"
  #include "cfg_db_eeprom.h"
  #include "eeprom_cnfg.h"
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
  #define SIZE_OF_INT_DB_VAL     8  //4 bytes (U32) for ID, U32 for data
  #define SIZE_OF_STR_DB_VAL   255  //4 bytes (U32) , 255 bytes for data

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
  
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
/* --- PRIVATE_DATA --------------------------------------------------------- */

/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/**
  * @brief: Retrives a integer(32 bits size) value from a CNFG_DB_BACKUP.  
  *
  * @param:IN: "ee_addr" - the address into EEPROM of a parameter value 1st byte.
  *       OUT: "val_ptr" - a user provided pointer to a retrived parameter value.
  *
  * @retval: OK - if success, ERROR - otherwise.
**/
  uint32_t get_int_val(uint32_t ee_addr, uint32_t *val_ptr){
    uint32_t res = ERROR;
    
    if((ee_addr < (MAX_EEPROM_ADDR - SIZE_OF_INT_DB_VAL)) && val_ptr){
      eeprom_rd_buf(ee_addr, SIZE_OF_INT_DB_VAL, (uint8_t*) val_ptr);
      res = OK;
    }
        
    return res;
  }/******************************** get_int_val ******************************/

/**
  * @brief: Saves a integer(32 bits size) value into a CNFG_DB_BACKUP.  
  *
  * @param:IN: "ee_addr" - the address into EEPROM of a parameter value 1st byte.
  *       OUT: "val_ptr" - a user provided pointer to a parameter value.
  *
  * @retval: OK - if success, ERROR - otherwise.
**/
  uint32_t set_int_val(uint32_t ee_addr, uint32_t *val_ptr){
   uint32_t res = ERROR;
    
    if((ee_addr < (MAX_EEPROM_ADDR - SIZE_OF_INT_DB_VAL)) && val_ptr){
  
      for(int32_t i = 0; i < SIZE_OF_INT_DB_VAL; i++)
        write_eeprom(ee_addr + i, *((uint8_t*)val_ptr + i) );
      
      res = OK;
    }
    
    return res;
  }/******************************** set_int_val ******************************/

/**
  * @brief: Retrives a fix size string(255 Bytes) value from a CNFG_DB_BACKUP.  
  *
  * @param:IN: "ee_addr" - the address into EEPROM of a parameter value 1st byte.
  *       OUT: "val_ptr" - a user provided pointer to a parameter value.
  *
  * @retval: OK - if success, ERROR - otherwise.
**/
  uint32_t get_str_val(uint32_t ee_addr, int8_t *val_ptr){
   uint32_t res = ERROR;
   
    if((ee_addr < (MAX_EEPROM_ADDR - SIZE_OF_STR_DB_VAL)) && val_ptr){
      eeprom_rd_buf(ee_addr, SIZE_OF_STR_DB_VAL, (uint8_t*) val_ptr);
      res = OK;
    }
    return res;
  }/******************************** get_str_val ******************************/

/**
  * @brief: Saves a integer(32 bits size) value into a CNFG_DB_BACKUP.  
  *
  * @param:IN: "ee_addr" - the address into EEPROM of a parameter value 1st byte.
  *       OUT: "val_ptr" - a user provided pointer to a parameter value.
  *
  * @retval: OK - if success, ERROR - otherwise.
**/
  uint32_t set_str_val(uint32_t ee_addr, int8_t *val_ptr){
    uint32_t res = ERROR;
    
    if((ee_addr < (MAX_EEPROM_ADDR - SIZE_OF_STR_DB_VAL)) && val_ptr){
       for(int32_t i = 0; i < SIZE_OF_STR_DB_VAL; i++)
        write_eeprom(ee_addr + i, *((uint8_t*)val_ptr + i) );
      res = OK;
    } 
    return res;
  }/******************************** set_str_val ******************************/


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
#if 0
  typedef struct _cfg_params_db_img_dscrptr_ {
   uint16_t db_img_size; 
   uint16_t db_img_crc16;
  }CFG_PARAMS_DB_IMG_DSCRPTR_ST, *CFG_PARAMS_DB_IMG_DSCRPTR_PST;
  #pragma location = 0x40024000     /* begin of Backup SRAM Area in stm32f207 */
  __no_init volatile CFG_PARAMS_DB_IMG_DSCRPTR_ST db_bkup_descr;
#endif