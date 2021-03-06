/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : BSP_LIB 
*
*  FILE        : mc24xx1025.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP
*
*  PURPOSE     : Microchip 24XX1025 EEPROM chip driver services.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.10     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
*
******************************************************************************/

#ifndef _MC24xx1025_DEF_
#define _MC24xx1025_DEF_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */
  #define MAX_EEPROM_ADDR     0x1FFFF /* 131071d */
  #define EEPROM_SIZE_BYTES   MAX_ADDR_BLOCK_1 + 1
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  void eeprom_rd_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr);   
  void eeprom_wr_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr);
  uint8_t read_eeprom(uint32_t mem_addr);
  void write_eeprom(uint32_t addr, uint8_t data);
  void i2c_init();
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/


#ifdef __cplusplus
}
#endif
#endif /* _MC24xx1025_DEF_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
