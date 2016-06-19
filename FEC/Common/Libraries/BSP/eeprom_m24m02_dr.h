/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : BSP_LIB
 *
 *  FILE        : eeprom_m24_m02_dr.h
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

#ifndef _M24M02DR_DEF_H_
#define _M24M02DR_DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */
#define MAX_EEPROM_ADDR     0x3FFFF /* 262143d  - 256KBytes */
#define EEPROM_SIZE_BYTES   MAX_ADDR_BLOCK_1 + 1
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
void eeprom_wr_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr);
void eeprom_rd_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr);
uint32_t eeprom_read_buffer_sequential(uint16_t mem_addr, uint16_t len, uint8_t *data_ptr);
uint8_t read_eeprom(uint32_t mem_addr);
void write_eeprom(uint32_t addr, uint8_t data);
void i2c_init();

void I2C1_ByteWrite(uint8_t* pBuffer, uint16_t WriteAddr);
void I2C1_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
void I2C1_BufferWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
void I2C1_BufferRead(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
void I2C1_WaitEepromStandbyState(void);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/
#ifdef __cplusplus
}
#endif
#endif /* _M24M02DR_DEF_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
