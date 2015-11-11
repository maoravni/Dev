/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : BSP_LIB 
*
*  FILE        : mc24xx1025.c
*   
*  PATH        :  C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP
*
*  PURPOSE     : Microchip 24XX1025 I2C EEPROM chip driver implementatuion.
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
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include <stm32f2xx.h>
  //#include "stm32f2xx_i2c.h"
  //#include <stm32f2xx_gpio.h>
  //#include <stm32f2xx_rcc.h>

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
/* I2C_1 channel related definitions */
  #define I2C_SPEED       100000//340000
  #define I2C_DUTYCYCLE   I2C_DutyCycle_2//I2C_DutyCycle_16_9//

/* Microchip 24XX1025 EEPROM definitions */
  #define MIN_ADDR_BLOCK_0 0x0000  /*      0d */
  #define MAX_ADDR_BLOCK_0 0xFFFF  /*  65535d */

  #define MIN_ADDR_BLOCK_1 0x10000 /*  65536d */
  #define MAX_ADDR_BLOCK_1 0x1FFFF /* 131071d */

  #define MAX_EEPROM_ADDR  0x1FFFF /* 131071d */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  void ee_rd_buf(uint32_t mem_addr,  uint32_t len, uint8_t *data_ptr);
  void ee_wr_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr, uint8_t ctrl_byte_val);
/* --- PRIVATE_DATA --------------------------------------------------------- */
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

#define I2C1_SLAVE_ADDR 0x33;
/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
  * @brief:   Reads "len" bytes from EEPROM to "data_ptr". A user is responsibile
  *           to provide a pointer to appropriate memory space.
  * @param:      
  *
  * @retval: 
**/
  void eeprom_rd_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr){
   uint32_t tmp_addr = mem_addr, step = 0;
   uint8_t *step_ptr = data_ptr;
   
    if((mem_addr + len) > MAX_ADDR_BLOCK_1)
      return;
    
    while(len){

      if(( (tmp_addr) < MAX_ADDR_BLOCK_0 )&&((tmp_addr + len) > MAX_ADDR_BLOCK_0)){
        step = MAX_ADDR_BLOCK_0 - tmp_addr + 1; 
      }else 
         step = len;
      
      ee_rd_buf(tmp_addr, step, step_ptr);
      
      step_ptr += (step);
      tmp_addr += (step);
      len      -= step;
          
    }/* while(len) */

  }/**************************** eeprom_rd_buf ********************************/ 

/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
  void write_eeprom(uint32_t mem_addr, uint8_t data){
   uint8_t addr_byte_low, addr_byte_hi, ctrl_byte_val = 0xA0;
      
    if( (mem_addr) > MAX_ADDR_BLOCK_0 ){
      mem_addr -= MIN_ADDR_BLOCK_1;
      ctrl_byte_val |= 0x08; /* Set bit#3 of a control byte for bloc_1 addressing */ 
    }
    
    addr_byte_low =  mem_addr & 0x000000FF; 
    addr_byte_hi =  (mem_addr & 0x0000FF00)>>8;
  
    I2C1->CR1 |= I2C_CR1_START;/* Send I2C1 START condition */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val, I2C_Direction_Transmitter);/* Send slave Address for write */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */  
 
    I2C_SendData(I2C1, addr_byte_hi);/* Send I2C1 slave internal address Hight byte*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)); /* Test on I2C1 EV8 and clear it */
   
    I2C_SendData(I2C1, addr_byte_low);/* Send I2C1 slave internal address */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));  /* Test on I2C1 EV8 and clear it */ 

    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);  /*enable NACK bit on next read and read final register*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);
  
    I2C_SendData(I2C1, data); /* write in register.*//* Write in I2C1 reg. test byte */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */
  
    I2C_AcknowledgeConfig(I2C1, ENABLE); 
  
    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */
  
    while(!I2C_CheckEvent(I2C1, 0x0));/* Test on I2C1 ??? and clear it */
    for(long i=0; i<100000; i++);
  }/***************************** write_eeprom ********************************/

/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
  uint8_t read_eeprom(uint32_t   mem_addr){
   uint8_t data, addr_byte_low, addr_byte_hi, ctrl_byte_val = 0xA0;
   
    
    if( (mem_addr) > MAX_ADDR_BLOCK_0 ){
      mem_addr -= MIN_ADDR_BLOCK_1;
      ctrl_byte_val |= 0x08; /* Set bit#3 of a control byte for bloc_1 addressing */ 
    }
    
    addr_byte_low =  mem_addr & 0x000000FF; 
    addr_byte_hi =  (mem_addr & 0x0000FF00)>>8;
  
    I2C_GenerateSTART(I2C1, ENABLE); /* Send I2C1 START condition */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val, I2C_Direction_Transmitter);/* Send slave Address for write in pointer reg. */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));  /* Test on I2C1 EV8 and clear it */
  
    I2C_SendData(I2C1,addr_byte_hi);/* Send address for read - Hight Byte*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)); /* Test on I2C1 EV8 and clear it */

    I2C_SendData(I2C1, addr_byte_low);/* Send address for read - Low Byte */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));  /* Test on I2C1 EV8 and clear it */
  
    I2C_GenerateSTART(I2C1, ENABLE);/* Send START condition a second time (Re-Start) */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);/* Enable NACK bit on next read and read final byte*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val, I2C_Direction_Receiver);/* Change the channel to receive from slave *//* Send address for read */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_RECEIVED));/* Test on I2C1 EV4 and clear it */

    data = I2C1->DR;/* Receive data from I2C1 register */

    I2C_AcknowledgeConfig(I2C1, ENABLE);

    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */
  
    while(!I2C_CheckEvent(I2C1, 0x00));/* Test on I2C1 ??? and clear it */
    while(!I2C_CheckEvent(I2C1, 0x0));
    
//    for(long i=0; i<500000; i++);
  
    return data;   
  }/******************************* read_eeprom *******************************/

/**
  * @brief: Inits I2C_1 to work with 24XX1025 EEPROM chip.  
  *
  * @param:      
  *
  * @retval: 
**/
  void i2c_init(void){
//   GPIO_InitTypeDef GPIO_InitStructure;
   I2C_InitTypeDef  I2C_InitStructure;
#if 0
    /* GPIO configuration */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  /* GPIOB Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);   /* I2C1 and Periph clock enable */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9; /* Configure I2C1 pins: SCL (pin PB8), SDA (pin PB9) */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;   /* Open Drain */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; /* I2C bus pulled high externally */
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
    /* I2C1 configuration */
    I2C_DeInit(I2C1);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);   /* I2C1 and Periph clock enable */
    I2C_StructInit(&I2C_InitStructure); /* reset all structure's fields to default values */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE;
    I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDR;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;//I2C_Ack_Enable;//
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
    /* Enable I2C1 */
    I2C_Cmd(I2C1, ENABLE);
    I2C_Init(I2C1, &I2C_InitStructure);
  }/****************************** i2c_init ***********************************/

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/

/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
  void ee_rd_buf(uint32_t mem_addr,  uint32_t len, uint8_t *data_ptr){
   uint8_t addr_byte_low, addr_byte_hi, ctrl_byte_val = 0xA0;//, count = 0;
      
    if( (mem_addr) > MAX_ADDR_BLOCK_0 ){
      mem_addr -= MIN_ADDR_BLOCK_1;
      ctrl_byte_val |= 0x08; /* Set bit#3 of a control byte for bloc_1 addressing */ 
    }
    
    addr_byte_low =  mem_addr & 0x000000FF; 
    addr_byte_hi =  (mem_addr & 0x0000FF00)>>8;
  
    I2C_GenerateSTART(I2C1, ENABLE); /* Send I2C1 START condition */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val, I2C_Direction_Transmitter);/* Send slave Address for write in pointer reg. */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));  /* Test on I2C1 EV8 and clear it */
  
    I2C_SendData(I2C1,addr_byte_hi);/* Send address for read - Hight Byte*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)); /* Test on I2C1 EV8 and clear it */

    I2C_SendData(I2C1, addr_byte_low);/* Send address for read - Low Byte */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));  /* Test on I2C1 EV8 and clear it */
  
    I2C_GenerateSTART(I2C1, ENABLE);/* Send START condition a second time (Re-Start) */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
      I2C_AcknowledgeConfig(I2C1, ENABLE);
 //   I2C_AcknowledgeConfig(I2C1, DISABLE);
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val, I2C_Direction_Receiver);/* Change the channel to receive from slave *//* Send address for read */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_RECEIVED));/* Test on I2C1 EV4 and clear it */

    for(uint32_t a = 0; a < len; a++){
      *data_ptr = I2C1->DR;/* Receive data from I2C1 register */
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
      data_ptr++;
//      count++;      
    }
    
    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);/* Enable NACK bit on next read and read final byte*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    
    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */
  
    while(!I2C_CheckEvent(I2C1, 0x00));/* Test on I2C1 ??? and clear it */
    while(!I2C_CheckEvent(I2C1, 0x0));
    
//    return count;       
  }/******************************* ee_rd_buf *******************************/ 
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
    void delay_ms(uint32_t ms){        
    volatile uint32_t nCount;        
    RCC_ClocksTypeDef RCC_Clocks;    
    RCC_GetClocksFreq (&RCC_Clocks);        
    nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;        
    for (; nCount!=0; nCount--);
  }/***************************************************************************/
#endif
#if 0
  /**
  * @brief:   Writes "len" bytes begin from "mem_addr" into EEPROM from "data_ptr". 
  *
  * @param:      
  *
  * @retval: 
**/
  void eeprom_wr_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr){

   uint8_t ctrl_byte_val = 0xA0;
   uint32_t page_bound = 0, tmp_addr = mem_addr, step = 0;
   uint8_t *step_ptr = data_ptr;
   
    if((mem_addr + len) > MAX_ADDR_BLOCK_1)
      return;
    
    while(len){
//#if 0      
      while( page_bound <= tmp_addr)
        page_bound += 127;

      if((tmp_addr + len) > page_bound){          
             step = page_bound - tmp_addr; 
      }else{
             step = len;
        }
      
      if( (tmp_addr) > MAX_ADDR_BLOCK_0 ){
        tmp_addr -= MIN_ADDR_BLOCK_1;
        ctrl_byte_val |= 0x08; /* Set bit#3 of a control byte for bloc_1 addressing */ 
      }
//#endif
//step = len;      
      ee_wr_buf(tmp_addr, step, step_ptr, ctrl_byte_val);
      
      step_ptr += step;
      tmp_addr += step;
      len      -= step;
          
    }/* while(len) */

  }/***************************** eeprom_wr_buf ********************************/

#endif
#if 0      
    if(( (mem_addr) < MAX_ADDR_BLOCK_0 )&&((mem_addr + len) > MAX_ADDR_BLOCK_0)){
      eeprom_rd_buf(mem_addr, (MAX_ADDR_BLOCK_0 - mem_addr), data_ptr);
      ctrl_byte_val |= 0x08; /* Set bit#3 of a control byte for bloc_1 addressing */ 
      eeprom_rd_buf(MIN_ADDR_BLOCK_1, (len - (MAX_ADDR_BLOCK_0 - mem_addr)), (data_ptr + (MAX_ADDR_BLOCK_0 - mem_addr) + 1));
      return;
    }else{ 
      if( mem_addr > MAX_ADDR_BLOCK_0 ){
        mem_addr -= MIN_ADDR_BLOCK_1;
        ctrl_byte_val |= 0x08; /* Set bit#3 of a control byte for bloc_1 addressing */ 
      }
      addr_byte_low =  mem_addr & 0x000000FF; 
      addr_byte_hi =  (mem_addr & 0x0000FF00)>>8;
      eeprom_rd_buf(mem_addr, len, data_ptr);
     }
#endif
#if 0    
   uint8_t ctrl_byte_val = 0xA0;
   uint32_t page_bound = 0;
   
    if((mem_addr + len) > MAX_ADDR_BLOCK_1)
      return;
    
    if(( (mem_addr) < MAX_ADDR_BLOCK_0 )&&((mem_addr + len) > MAX_ADDR_BLOCK_0)){
      ee_wr_buf(mem_addr, (MAX_ADDR_BLOCK_0 - mem_addr), data_ptr, 0xA0);//eeprom_wr_buf(mem_addr, (MAX_ADDR_BLOCK_0 - mem_addr), data_ptr);
      ctrl_byte_val |= 0x08; /* Set bit#3 of a control byte for bloc_1 addressing */ 
      ee_wr_buf(MIN_ADDR_BLOCK_1, (len - (MAX_ADDR_BLOCK_0 - mem_addr)), (data_ptr + (MAX_ADDR_BLOCK_0 - mem_addr) + 1),ctrl_byte_val);//eeprom_wr_buf(MIN_ADDR_BLOCK_1, (len - (MAX_ADDR_BLOCK_0 - mem_addr)), (data_ptr + (MAX_ADDR_BLOCK_0 - mem_addr) + 1));
      return;
    }else{ 
      if( mem_addr > MAX_ADDR_BLOCK_0 ){
        mem_addr -= MIN_ADDR_BLOCK_1;
        ctrl_byte_val |= 0x08; /* Set bit#3 of a control byte for bloc_1 addressing */ 
      }
     }
    
    while( page_bound < mem_addr)
      page_bound += 128;
    
    if((mem_addr + len) > page_bound){
      ee_wr_buf(mem_addr, (page_bound - mem_addr), data_ptr, ctrl_byte_val);
      ee_wr_buf(page_bound, (len - (page_bound - mem_addr)), (data_ptr + (page_bound - mem_addr)), ctrl_byte_val);
    }else
      ee_wr_buf(mem_addr, len, data_ptr, ctrl_byte_val);
#endif
#if 0
/**
  * @brief: Checks "eeprom_addr" and "data_size" validity by this criteria:
  *        "eeprom_addr" + "data_sz" < MAX_EEPROM_ADDR and builds a control byte
  *         value for a single 24XX1025 chip.
  *
  * @param:  "eeprom_addr" and "data_sz"
  *
  * @retval: The 24XX1025 control byte value(0xAX) - if success; 
  *          0xFF - if "eeprom_addr" and/or "data_sz" invalid 
**/ 
  uint8_t mc24xx1025_ctrl_byte_build(uint32_t addr, uint32_t sz){
    uint8_t ctrl_byte_val = 0xA0;

    if((addr + sz) > MAX_EEPROM_ADDR){
       return 0xFF;
    }
    if(((addr + sz) < MAX_ADDR_BLOCK_0)||((addr + sz) < MAX_ADDR_BLOCK_1)){
      if((addr + sz) < MAX_ADDR_BLOCK_0)
        return ctrl_byte_val;
      if((addr + sz) < MAX_ADDR_BLOCK_1)
        return (ctrl_byte_val | 0x08 ); /* Set bit#3 of a control byte for bloc_1 addressing */      
    }
    else return 0xFE;/* 2-block sequentially operation */
  }
#endif
#if 0
   do{
      I2C_GenerateSTART(I2C1, ENABLE);
      I2C_Send7bitAddress(I2C1, ctrl_byte_val, I2C_Direction_Transmitter);/* Send slave Address for write */
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */  
    }while(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0400);
  
    while(!I2C_CheckEvent(I2C1, 0x0));/* Test on I2C1 ??? and clear it */
    for(long i=0; i<100000; i++);
      
/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
  void ee_wr_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr, uint8_t ctrl_byte_val){
   uint8_t addr_byte_low, addr_byte_hi;
  
    addr_byte_low =  mem_addr & 0x000000FF; 
    addr_byte_hi =  (mem_addr & 0x0000FF00)>>8;
  
    I2C1->CR1 |= I2C_CR1_START;/* Send I2C1 START condition */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val, I2C_Direction_Transmitter);/* Send slave Address for write */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */  
 
    I2C_SendData(I2C1, addr_byte_hi);/* Send I2C1 slave internal address Hight byte*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)); /* Test on I2C1 EV8 and clear it */
   
    I2C_SendData(I2C1, addr_byte_low);/* Send I2C1 slave internal address */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));  /* Test on I2C1 EV8 and clear it */ 

    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);  /*enable NACK bit on next read and read final register*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    
    for(uint32_t a = 0; a <= len; a++){
      I2C_SendData(I2C1, *(data_ptr++)); /* write in register.*/
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */
    }
  
    I2C_AcknowledgeConfig(I2C1, ENABLE); 
  
    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */
  
    while(!I2C_CheckEvent(I2C1, 0x0));/* Test on I2C1 ??? and clear it */
    for(long i=0; i<100000; i++);
    
  }/******************************** ee_wr_buf ********************************/
#endif
#if 0
  /**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
  void ee_wr_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr, uint8_t ctrl_byte_val){
   uint8_t addr_byte_low, addr_byte_hi;
  
    addr_byte_low =  mem_addr & 0x000000FF; 
    addr_byte_hi =  (mem_addr & 0x0000FF00)>>8;
  
    I2C1->CR1 |= I2C_CR1_START;/* Send I2C1 START condition */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/* Test on I2C1 EV5 and clear it */
  
    I2C_Send7bitAddress(I2C1, ctrl_byte_val, I2C_Direction_Transmitter);/* Send slave Address for write */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */  
 
    I2C_SendData(I2C1, addr_byte_hi);/* Send I2C1 slave internal address Hight byte*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)); /* Test on I2C1 EV8 and clear it */
   
    I2C_SendData(I2C1, addr_byte_low);/* Send I2C1 slave internal address */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));  /* Test on I2C1 EV8 and clear it */ 

//    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);  /*enable NACK bit on next read and read final register*/
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    for(uint32_t a = 0; a <= len; a++){
      I2C_SendData(I2C1, *(data_ptr++)); /* write in register.*/
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));/* Test on I2C1 EV8 and clear it */
    }
  
    I2C_AcknowledgeConfig(I2C1, ENABLE); 
  
    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */
    
 delay_ms(10000);
  }/******************************** ee_wr_buf ********************************/
#endif
