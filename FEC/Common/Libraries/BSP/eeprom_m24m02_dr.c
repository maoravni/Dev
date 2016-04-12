/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : BSP_LIB
 *
 *  FILE        : eeprom_m24m02_dr.c
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
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif

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
#define I2C_SPEED       100000 //340000
#define I2C_DUTYCYCLE   I2C_DutyCycle_2 //I2C_DutyCycle_16_9//
/* Microchip 24XX1025 EEPROM definitions */
#define MIN_ADDR_BLOCK_0 0x0000  /*      0d */
#define MAX_ADDR_BLOCK_0 0xFFFF  /*  65535d */

#define MIN_ADDR_BLOCK_1 0x10000 /*  65536d */
#define MAX_ADDR_BLOCK_1 0x1FFFF /* 131071d */

//  #define MAX_EEPROM_ADDR  0x1FFFF /* 131071d */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */

typedef union _dev_select_val_
{
    uint8_t whole;
    struct
    {
        uint8_t rd_wr :1; /* bit 0    */
        uint8_t addr_ms_bits :2; /* bits 1-2 */
        uint8_t e2 :1; /* bit 3    */
        uint8_t dev_type :4; /* bits 4-7 */
    } field;
} DEV_SELECT_VAL_UT, *DEV_SELECT_VAL_PUT;

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
void delay_ms(uint32_t ms);
void ee_rd_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr);
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
 * @brief: Writes 1 byte to EEPROM.
 *
 * @param:
 *
 * @retval:
 **/
void write_eeprom(uint32_t mem_addr, uint8_t data)
{
    DEV_SELECT_VAL_UT ctrl_byte_val;
    uint8_t addr_byte_low, addr_byte_hi;

    ctrl_byte_val.whole = 0;

    ctrl_byte_val.field.dev_type = 0x0A;
    ctrl_byte_val.field.addr_ms_bits = (mem_addr & 0x00030000) >> 16;
    ctrl_byte_val.field.e2 = 0;
    ctrl_byte_val.field.rd_wr = 0;

    addr_byte_low = mem_addr & 0x000000FF;
    addr_byte_hi = (mem_addr & 0x0000FF00) >> 8;

    I2C1->CR1 |= I2C_CR1_START;/* Send I2C1 START condition */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;/* Test on I2C1 EV5 and clear it */

    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);/* Send slave Address for write */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ;/* Test on I2C1 EV8 and clear it */

    I2C_SendData(I2C1, addr_byte_hi);/* Send I2C1 slave internal address Hight byte*/
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ; /* Test on I2C1 EV8 and clear it */

    I2C_SendData(I2C1, addr_byte_low);/* Send I2C1 slave internal address */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ; /* Test on I2C1 EV8 and clear it */

    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current); /*enable NACK bit on next read and read final register*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);

    I2C_SendData(I2C1, data); /* write in register.*//* Write in I2C1 reg. test byte */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ;/* Test on I2C1 EV8 and clear it */

    I2C_AcknowledgeConfig(I2C1, ENABLE);

    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */

    while (!I2C_CheckEvent(I2C1, 0x0))
        ;/* Test on I2C1 ??? and clear it */
//    for(long i=0; i<100000; i++);

    vTaskDelay(10);
//    delay_ms(10);
}/***************************** write_eeprom ********************************/

/**
 * @brief:   Return one byte readed from "mem_addr" location into EEPROM.
 *
 * @param:
 *
 * @retval:
 **/
uint8_t read_eeprom(uint32_t mem_addr)
{
    DEV_SELECT_VAL_UT ctrl_byte_val;
    uint8_t data, addr_byte_low, addr_byte_hi;

    ctrl_byte_val.whole = 0;

    ctrl_byte_val.field.dev_type = 0x0A;
    ctrl_byte_val.field.addr_ms_bits = (mem_addr & 0x00030000) >> 16;
    ctrl_byte_val.field.e2 = 0;
    ctrl_byte_val.field.rd_wr = 1;

    addr_byte_low = mem_addr & 0x000000FF;
    addr_byte_hi = (mem_addr & 0x0000FF00) >> 8;

    I2C_GenerateSTART(I2C1, ENABLE); /* Send I2C1 START condition */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;/* Test on I2C1 EV5 and clear it */

    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);/* Send slave Address for write in pointer reg. */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ; /* Test on I2C1 EV8 and clear it */

    I2C_SendData(I2C1, addr_byte_hi);/* Send address for read - Hight Byte*/
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ; /* Test on I2C1 EV8 and clear it */

    I2C_SendData(I2C1, addr_byte_low);/* Send address for read - Low Byte */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ; /* Test on I2C1 EV8 and clear it */

    I2C_GenerateSTART(I2C1, ENABLE);/* Send START condition a second time (Re-Start) */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;/* Test on I2C1 EV5 and clear it */

    I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);/* Enable NACK bit on next read and read final byte*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);

    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Receiver);/* Change the channel to receive from slave *//* Send address for read */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_RECEIVED))
        ;/* Test on I2C1 EV4 and clear it */

    data = I2C1->DR;/* Receive data from I2C1 register */

    I2C_AcknowledgeConfig(I2C1, ENABLE);

    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */

    while (!I2C_CheckEvent(I2C1, 0x00))
        ;/* Test on I2C1 ??? and clear it */
    while (!I2C_CheckEvent(I2C1, 0x0))
        ;

//    for(long i=0; i<500000; i++);

    return data;
}/******************************* read_eeprom *******************************/

/**
 * @brief:   Writes "len" bytes to EEPROM from "data_ptr". A user is responsibile
 *           to provide a pointer to a data to be writen.
 * @param:
 *
 * @retval:
 **/
void eeprom_wr_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr)
{
    uint32_t i = 0;
    uint8_t temp;

    while (i < len)
    {
        temp = read_eeprom((mem_addr + i));
        if (temp != *(data_ptr + i))
            write_eeprom((mem_addr + i), *(data_ptr + i));
        i++;
    }/* while(i < len) */
}/**************************** eeprom_wr_buf ********************************/

/**
 * @brief:   Reads "len" bytes from EEPROM to "data_ptr". A user is responsibile
 *           to provide a pointer to appropriate memory space.
 * @param:
 *
 * @retval:
 **/
void eeprom_rd_buf(uint32_t mem_addr, uint32_t len, uint8_t *data_ptr)
{
    uint32_t i = 0;

    while (i < len)
    {
        *(data_ptr + i) = read_eeprom(mem_addr + i);
        i++;
    }/* while(i < len) */
}/**************************** eeprom_rd_buf ********************************/

/**
 * @brief: Inits I2C_1 to work with m24m02_dr EEPROM chip.
 *
 * @param:
 *
 * @retval:
 **/
void i2c_init(void)
{
    I2C_InitTypeDef I2C_InitStructure;

    /* I2C1 configuration */

    I2C_DeInit(I2C1);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); /* I2C1 and Periph clock enable */

    I2C_StructInit(&I2C_InitStructure); /* reset all structure's fields to default values */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE;
//    I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDR;
    I2C_InitStructure.I2C_OwnAddress1 = 0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;  //I2C_Ack_Disable;//
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
    I2C_Init(I2C1, &I2C_InitStructure);

    /* Enable I2C1 */
    I2C_Cmd(I2C1, ENABLE);
}/****************************** i2c_init ***********************************/

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
void delay_ms(uint32_t ms)
{
    volatile uint32_t nCount;
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    nCount = (RCC_Clocks.HCLK_Frequency / 10000) * ms;
    for (; nCount != 0; nCount--)
        ;
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

