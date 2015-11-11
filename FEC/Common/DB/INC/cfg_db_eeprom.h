/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PRM_DB_LIB 
*
*  FILE        : cfg_db_backup.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\DB\INC 
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
******************************************************************************/

#ifndef _CFG_DB_BACKUP_H_
#define _CFG_DB_BACKUP_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */
  #define   GENERAL_OFFSET  128
   
  #define   OFFSET_FILE_0   GENERAL_OFFSET
  #define   OFFSET_FILE_1   GENERAL_OFFSET+FILE_0_SZ
  #define   OFFSET_FILE_2   GENERAL_OFFSET+FILE_0_SZ+FILE_1_SZ
  #define   OFFSET_FILE_3   GENERAL_OFFSET+FILE_0_SZ+FILE_1_SZ+FILE_2_SZ
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  uint32_t get_int_val(uint32_t ee_addr, uint32_t *val_ptr);
  uint32_t set_int_val(uint32_t ee_addr, uint32_t *val_ptr);
  uint32_t get_str_val(uint32_t ee_addr, int8_t   *val_ptr);
  uint32_t set_str_val(uint32_t ee_addr, int8_t   *val_ptr);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _CFG_DB_BACKUP_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
