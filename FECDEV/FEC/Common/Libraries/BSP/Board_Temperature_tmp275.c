/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  
*
*  FILE        :   tmp275.c 
*   
*  PATH        :  $PROJ_DIR$..\..\..\Common\Libraries\BSP  
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
*  2012.01     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
* 
*******************************************************************************/
// talk to tmp75/175/275 family of i2c temperature sensors
// eric volpe (epvuc@limpoc.com) 3/18/2009
// constants for the config register are included in tmp75.h
// for example, 
// tmp75_write_config(TMP75_12BITS);
//
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#ifdef __cplusplus
 extern "C" {
#endif

#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif
  #include "ll_typedef.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "Board_Temperature_tmp275.h"
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
  typedef union _tmp275_select_val_ {
    uint8_t whole;
    struct{
    uint8_t rd_wr        :1; /* bit 0    */
    uint8_t addr_bits    :3; /* bits 1-3 */
    uint8_t dev_type     :4; /* bits 4-7 */
    }field;
  }TMP275_SLCT_VAL_UT, *TMP275_SLCT_VAL_PUT;
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  static void delay_ms(uint32_t ms);
/* --- PRIVATE_DATA --------------------------------------------------------- */
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
 /**
  * @brief:  read current temperature in degC * 16.
  *
  * @param:  no.    
  *
  * @retval: current board temperature value.
**/ 
  int16_t tmp75_read_tempr(void)
  {
   TMP275_SLCT_VAL_UT ctrl_byte_val; 
   uint8_t   tmpr_hi = 0;
   uint8_t   tmpr_lo = 0;
   int16_t   tempr   = 0;
  
    ctrl_byte_val.whole = 0;
  
    ctrl_byte_val.field.dev_type  = 9;
    ctrl_byte_val.field.addr_bits = 1;
    ctrl_byte_val.field.rd_wr     = 0;
  
    I2C1->CR1 |= I2C_CR1_START;/* Send I2C1 START condition */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
   
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);/* Send slave Address for write */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */
  
    I2C_SendData(I2C1, 0);/* Write the Pointer Register for select the Temperature register */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)); /* Test on I2C1 EV8 and clear it */
  
    I2C_GenerateSTART(I2C1, ENABLE);/* Send START condition a second time (Re-Start) */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
    //I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);/* Enable NACK bit on next read and read final byte*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    
    ctrl_byte_val.field.rd_wr     = 1;
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Receiver);/* Change the channel to receive from slave *//* Send address for read */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_RECEIVED));/* Test on I2C1 EV4 and clear it */

    tmpr_hi = I2C1->DR;/* Receive data from I2C1 register */
    
    I2C_GenerateSTART(I2C1, ENABLE);/* Send START condition a second time (Re-Start) */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);/* Enable NACK bit on next read and read final byte*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Receiver);/* Change the channel to receive from slave *//* Send address for read */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_RECEIVED));/* Test on I2C1 EV4 and clear it */

    tmpr_lo = I2C1->DR;/* Receive data from I2C1 register */
  
    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */
    while(!I2C_CheckEvent(I2C1, 0x0));/* Test on I2C1 ??? and clear it */
    for(long i=0; i<100000; i++);  
    delay_ms(10);
  
	  tempr = tmpr_hi;//((tmphi<<8)|tmplo)>>4;
    tempr = tempr << 4;
    tempr += (tmpr_lo>>4);
  
    tempr = tempr >> 4;
  

	  return(tempr);
  }/******************** int16_t tmp75_read_tempr(void) ***********************/ 

/**
  * @brief:  write to tmp75 config register
  *
  * @param:  new configuration register value.    
  *
  * @retval: no.
**/
void tmp75_write_config(uint8_t conf)
{
 //DEV_SELECT_VAL_UT  ctrl_byte_val;
 
//	i2c_start_wait(TMP75_ADDR+I2C_WRITE);
  I2C1->CR1 |= I2C_CR1_START;/* Send I2C1 START condition */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
//  I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);/* Send slave Address for write */
//    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */
  
//	i2c_write(0x01); // address config register
//	i2c_write(conf);
  
//	i2c_stop();
   I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */
  
    while(!I2C_CheckEvent(I2C1, 0x0));/* Test on I2C1 ??? and clear it */
    for(long i=0; i<100000; i++);
    
    delay_ms(10);
}
#if 0
// read tmp75 config register. see tmp75.h for bit meanings
uint8_t tmp75_read_config(void)
{ 
	uint8_t conf;
	i2c_start_wait(TMP75_ADDR+I2C_WRITE);
	i2c_write(0x01); // address config register
	i2c_rep_start(TMP75_ADDR + I2C_READ);
	conf = i2c_readNak();
	i2c_stop();
	return(conf);
}

// write to tmp75 config register
void tmp75_write_config(uint8_t conf)
{ 
	i2c_start_wait(TMP75_ADDR+I2C_WRITE);
	i2c_write(0x01); // address config register
	i2c_write(conf);
	i2c_stop();
}

// read current temperature in degC * 16. 
int16_t tmp75_read_temp(void)
{ 
	uint8_t tmphi, tmplo;
	int16_t temp;
	i2c_start_wait(TMP75_ADDR+I2C_WRITE);
	i2c_write(0x00); // address Temperature register
	i2c_rep_start(TMP75_ADDR + I2C_READ);
	tmphi = i2c_readAck();
	tmplo = i2c_readNak();
	i2c_stop();
	temp = ((tmphi<<8)|tmplo)>>4;
	return(temp);
} 

// read low temp alarm threshold
int16_t tmp75_read_tlow(void)
{ 
	uint8_t tmphi, tmplo;
	uint16_t temp;
	i2c_start_wait(TMP75_ADDR+I2C_WRITE);
	i2c_write(0x02); // address TempLow register
	i2c_rep_start(TMP75_ADDR + I2C_READ);
	tmphi = i2c_readAck();
	tmplo = i2c_readNak();
	i2c_stop();
	temp = ((tmphi<<8)|tmplo)>>4;
	return(temp);
} 

// read high temp alarm threshold
int16_t tmp75_read_thi(void)
{ 
	uint8_t tmphi, tmplo;
	uint16_t temp;
	i2c_start_wait(TMP75_ADDR+I2C_WRITE);
	i2c_write(0x03); // address TempHigh register
	i2c_rep_start(TMP75_ADDR + I2C_READ);
	tmphi = i2c_readAck();
	tmplo = i2c_readNak();
	i2c_stop();
	temp = ((tmphi<<8)|tmplo)>>4;
	return(temp);
} 

// set temp alarm low threshold. (12 bit number)
void tmp75_write_tlow(int16_t tlow)
{ 
	tlow <<= 4;
	i2c_start_wait(TMP75_ADDR+I2C_WRITE);
	i2c_write(0x02); // address TempLow register
	i2c_write(tlow>>8);	// write high byte
	i2c_write(tlow&0x00ff); // write low byte
	i2c_stop();
} 

// set temp alarm high threshold. (12 bit number)
void tmp75_write_thi(int16_t thi)
{ 
	thi <<= 4;
	i2c_start_wait(TMP75_ADDR+I2C_WRITE);
	i2c_write(0x03); // address TempHigh register
	i2c_write(thi>>8);	// write high byte
	i2c_write(thi&0x00ff); // write low byte
	i2c_stop();
}
#endif
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
 static void delay_ms(uint32_t ms){        
    volatile uint32_t nCount;        
    RCC_ClocksTypeDef RCC_Clocks;    
    RCC_GetClocksFreq (&RCC_Clocks);        
    nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;        
    for (; nCount!=0; nCount--);
  }/***************************************************************************/
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
#ifdef __cplusplus
 }
#endif
