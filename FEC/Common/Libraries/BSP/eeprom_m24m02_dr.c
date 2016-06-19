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

#include "eeprom_m24m02_dr.h"

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

#define I2CEE_LONG_TIMEOUT 0x100000
#define I2CEE_FLAG_TIMEOUT 0x10000

#define I2C_EEPROM_PAGESIZE 256

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

//void Delay(__IO uint32_t nCount)
//{
//  for(; nCount != 0; nCount--);
//}

/**
 * @brief: Writes 1 byte to EEPROM.
 *
 * @param:
 *
 * @retval:
 **/
void write_eeprom(uint32_t mem_addr, uint8_t data)
{
//    printf("Writing address %x\n", mem_addr);
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

    //I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current); /*enable NACK bit on next read and read final register*/
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
//    printf("Reading address %x\n", mem_addr);
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

    //I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);/* Enable NACK bit on next read and read final byte*/
    I2C_AcknowledgeConfig(I2C1, DISABLE);

    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Receiver);/* Change the channel to receive from slave *//* Send address for read */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_BYTE_RECEIVED))
        ;/* Test on I2C1 EV4 and clear it */

    data = I2C1->DR;/* Receive data from I2C1 register */

    I2C_GenerateSTOP(I2C1, ENABLE);/* Send I2C1 STOP Condition */

    // ready the acknowledge for the next transfer
    I2C_AcknowledgeConfig(I2C1, ENABLE);

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
//        temp = read_eeprom((mem_addr + i));
//        if (temp != *(data_ptr + i))
        //write_eeprom((mem_addr + i), *(data_ptr + i));
        I2C1_ByteWrite((data_ptr + i), (mem_addr + i));
        vTaskDelay(10);
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

#define U8_i2cee_chip_addr 0x0a

uint32_t eeprom_read_buffer_sequential(uint16_t u16_ReadAddr, uint16_t u16_NumByteToRead, uint8_t* pu8_Buffer)
{
    uint16_t u16_DataReadCnt;
    uint32_t u32_Timeout;
    DEV_SELECT_VAL_UT ctrl_byte_val;

    ctrl_byte_val.whole = 0;

    ctrl_byte_val.field.dev_type = U8_i2cee_chip_addr;
    ctrl_byte_val.field.addr_ms_bits = (u16_ReadAddr & 0x00030000) >> 16;
    ctrl_byte_val.field.e2 = 0;
    ctrl_byte_val.field.rd_wr = 1;

    u16_DataReadCnt = u16_NumByteToRead;

// ---------------- While the bus is busy -------------------------------------
    u32_Timeout = I2CEE_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {
        if ((u32_Timeout--) == 0)
            return 1/*I2CEE_TIMEOUT_UserCallback()*/;
    }

// ---------------- Send Start condition --------------------------------------
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    u32_Timeout = I2CEE_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((u32_Timeout--) == 0)
            return 2/*I2CEE_TIMEOUT_UserCallback()*/;
    }

    // ---------------- Send chip address for Write -------------------------------
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    u32_Timeout = I2CEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if ((u32_Timeout--) == 0)
            return 3/*I2CEE_TIMEOUT_UserCallback()*/;
    }

    // ---------------- Send address you wanna read from --------------------------
    /* Send the EEPROM's internal address to read from: MSB of the address first */
    I2C_SendData(I2C1, (uint8_t)((u16_ReadAddr & 0xFF00) >> 8));

    /* Test on EV8_2 and clear it */
    u32_Timeout = I2CEE_LONG_TIMEOUT;
    //while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if ((u32_Timeout--) == 0)
            return 4/*I2CEE_TIMEOUT_UserCallback()*/;
    }

    /* Send the EEPROM's internal address to read from: LSB of the address */
    I2C_SendData(I2C1, (uint8_t)(u16_ReadAddr & 0x00FF));

    /* Test on EV8? and clear it */
    u32_Timeout = I2CEE_LONG_TIMEOUT;
    //while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
    {
        if ((u32_Timeout--) == 0)
            return 5/*I2CEE_TIMEOUT_UserCallback()*/;
    }

    /* Clear AF flag if arised */
//I2C1->SR1 |= (uint16_t)0x0400;
// ---------------- Send Start condition --------------------------------------
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    u32_Timeout = I2CEE_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((u32_Timeout--) == 0)
            return 6/*I2CEE_TIMEOUT_UserCallback()*/;
    }

// ---------------- Send chip address for Read --------------------------------
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    u32_Timeout = I2CEE_LONG_TIMEOUT;
    //while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if ((u32_Timeout--) == 0)
            return 7/*I2CEE_TIMEOUT_UserCallback()*/;
    }

    // ---------------- Read all data from eeprom to Buffer -----------------------
    while (u16_DataReadCnt > 0)
    {
        // If only 1 byte (or last byte) has to be read
        if (u16_NumByteToRead < 2)
        {

            /* Prepare an NACK for the next data received */
            I2C_AcknowledgeConfig(I2C1, DISABLE);

            /* Have to be generated before reading DR register */
            I2C_GenerateSTOP(I2C1, ENABLE);

            /* Test on EV7 and clear it */
            u32_Timeout = I2CEE_LONG_TIMEOUT;
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
            {
                if ((u32_Timeout--) == 0)
                    return 8/*I2CEE_TIMEOUT_UserCallback()*/;
            }

            /* Read data and decrement counter */
            *pu8_Buffer = I2C_ReceiveData(I2C1);
            u16_DataReadCnt--;
        }
        /* If more then 1 byte has to be read */
        else
        {
            /* Test on EV7 and clear it */
            u32_Timeout = I2CEE_LONG_TIMEOUT;
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
            {
                if ((u32_Timeout--) == 0)
                    return 9/*I2CEE_TIMEOUT_UserCallback()*/;
            }

            /* Read data and decrement counter */
            *pu8_Buffer++ = I2C_ReceiveData(I2C1);
            u16_DataReadCnt--;
        }
    } // end while

    I2C_AcknowledgeConfig(I2C1, ENABLE);
    return 0;
}

/***************************************************************************/
/** * @brief      Writes one byte to the I2C EEPROM.
 * @param[in]  pBuffer   : pointer to the buffer  containing the data to be
 * written to the EEPROM.
 * @param[in]  WriteAddr : EEPROM's internal address to write to.
 * @return     None
 ******************************************************************************/

void I2C1_ByteWrite(uint8_t* pBuffer, uint16_t WriteAddr)
{
//    printf("I2C1_ByteWrite %x\n", WriteAddr);

    DEV_SELECT_VAL_UT ctrl_byte_val;

    ctrl_byte_val.whole = 0;

    ctrl_byte_val.field.dev_type = 0x0A;
    ctrl_byte_val.field.addr_ms_bits = (WriteAddr & 0x00030000) >> 16;
    ctrl_byte_val.field.e2 = 0;
    ctrl_byte_val.field.rd_wr = 1;

    /* Send STRAT condition */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    /* Send the EEPROM's internal address to write to : MSB of the address first */
    I2C_SendData(I2C1, (uint8_t)((WriteAddr & 0xFF00) >> 8));

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    /* Send the EEPROM's internal address to write to : LSB of the address */
    I2C_SendData(I2C1, (uint8_t)(WriteAddr & 0x00FF));

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    /* Send the byte to be written */
    I2C_SendData(I2C1, *pBuffer);

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    /* Send STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
}

/***************************************************************************/
/** * @brief      Reads a block of data from the EEPROM.
 * @param[in]  pBuffer : pointer to the buffer that receives the data read
 *                       from the EEPROM.
 * @param[in]  ReadAddr : EEPROM's internal address to read from.
 * @param[in]  NumByteToRead : number of bytes to read from the EEPROM.
 * @return     None
 ******************************************************************************/

void I2C1_BufferRead(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{
    DEV_SELECT_VAL_UT ctrl_byte_val;

    ctrl_byte_val.whole = 0;

    ctrl_byte_val.field.dev_type = 0x0A;
    ctrl_byte_val.field.addr_ms_bits = (ReadAddr & 0x00030000) >> 16;
    ctrl_byte_val.field.e2 = 0;
    ctrl_byte_val.field.rd_wr = 1;

    /* While the bus is busy */
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
        ;

    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    /* Send the EEPROM's internal address to read from: MSB of the address first */
    I2C_SendData(I2C1, (uint8_t)((ReadAddr & 0xFF00) >> 8));

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    /* Send the EEPROM's internal address to read from: LSB of the address */
    I2C_SendData(I2C1, (uint8_t)(ReadAddr & 0x00FF));

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;

    /* While there is data to be read */
    while (NumByteToRead)
    {
        if (NumByteToRead == 1)
        {

            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C1, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C1, ENABLE);
        }

        /* Test on EV7 and clear it */
        if (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the EEPROM */
            *pBuffer = I2C_ReceiveData(I2C1);

            /* Point to the next location where the byte read will be saved */
            pBuffer++;

            /* Decrement the read bytes counter */
            NumByteToRead--;
        }
    }
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}
/***************************************************************************/
/**
 * @brief      Writes buffer of data to the I2C EEPROM.
 * @param[in]  pBuffer : pointer to the buffer  containing the data to be
 *                       written to the EEPROM.
 * @param[in]  WriteAddr : EEPROM's internal address to write to.
 * @param[in]  NumByteToWrite : number of bytes to write to the EEPROM.
 * @return     None
 ******************************************************************************/
void I2C1_BufferWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
//    printf("I2C1_BufferWrite %x %d\n", WriteAddr, NumByteToWrite);

    uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
    uint16_t Addr = 0;
    Addr = WriteAddr % I2C_EEPROM_PAGESIZE;
    count = I2C_EEPROM_PAGESIZE - Addr;
    NumOfPage = NumByteToWrite / I2C_EEPROM_PAGESIZE;
    NumOfSingle = NumByteToWrite % I2C_EEPROM_PAGESIZE;

    /* If WriteAddr is I2C_FLASH_PAGESIZE aligned  */
    if (Addr == 0)
    {

        /* If NumByteToWrite < I2C_FLASH_PAGESIZE */
        if (NumOfPage == 0)
        {
            I2C1_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            I2C1_WaitEepromStandbyState();
        }

        /* If NumByteToWrite > I2C_FLASH_PAGESIZE */

        else
        {
            while (NumOfPage--)
            {
                I2C1_PageWrite(pBuffer, WriteAddr, I2C_EEPROM_PAGESIZE);
                I2C1_WaitEepromStandbyState();
                WriteAddr += I2C_EEPROM_PAGESIZE;
                pBuffer += I2C_EEPROM_PAGESIZE;
            }
            if (NumOfSingle != 0)
            {
                I2C1_PageWrite(pBuffer, WriteAddr, NumOfSingle);
                I2C1_WaitEepromStandbyState();
            }
        }
    }

    /* If WriteAddr is not I2C_FLASH_PAGESIZE aligned  */
    else
    {

        /* If NumByteToWrite < I2C_FLASH_PAGESIZE */
        if (NumOfPage == 0)
        {

            /* If the number of data to be written is more than the remaining space
             in the current page: */

            if (NumByteToWrite > count)
            {

                /* Write the data conained in same page */
                I2C1_PageWrite(pBuffer, WriteAddr, count);
                I2C1_WaitEepromStandbyState();

                /* Write the remaining data in the following page */
                I2C1_PageWrite((uint8_t*) (pBuffer + count), (WriteAddr + count), (NumByteToWrite - count));
                I2C1_WaitEepromStandbyState();
            }
            else
            {
                I2C1_PageWrite(pBuffer, WriteAddr, NumOfSingle);
                I2C1_WaitEepromStandbyState();
            }
        }

        /* If NumByteToWrite > I2C_FLASH_PAGESIZE */

        else
        {
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / I2C_EEPROM_PAGESIZE;
            NumOfSingle = NumByteToWrite % I2C_EEPROM_PAGESIZE;
            if (count != 0)
            {
                I2C1_PageWrite(pBuffer, WriteAddr, count);
                I2C1_WaitEepromStandbyState();
                WriteAddr += count;
                pBuffer += count;
            }
            while (NumOfPage--)
            {
                I2C1_PageWrite(pBuffer, WriteAddr, I2C_EEPROM_PAGESIZE);
                I2C1_WaitEepromStandbyState();
                WriteAddr += I2C_EEPROM_PAGESIZE;
                pBuffer += I2C_EEPROM_PAGESIZE;
            }

            if (NumOfSingle != 0)
            {
                I2C1_PageWrite(pBuffer, WriteAddr, NumOfSingle);
                I2C1_WaitEepromStandbyState();

            }
        }
    }
}

/***************************************************************************/
/** * @brief      Writes more than one byte to the EEPROM with a single WRITE cycle.
 *             Note: The number of byte can't exceed the EEPROM page size.
 * @param[in]  pBuffer : pointer to the buffer containing the data to be
 *                       written to the EEPROM.
 * @param[in]  WriteAddr : EEPROM's internal address to write to.
 * @param[in]  NumByteToWrite : number of bytes to write to the EEPROM.
 * @return     None
 ******************************************************************************/

void I2C1_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
//    printf("I2C1_PageWrite %x %d\n", WriteAddr, NumByteToWrite);

    DEV_SELECT_VAL_UT ctrl_byte_val;

    ctrl_byte_val.whole = 0;

    ctrl_byte_val.field.dev_type = 0x0A;
    ctrl_byte_val.field.addr_ms_bits = (WriteAddr & 0x00030000) >> 16;
    ctrl_byte_val.field.e2 = 0;
    ctrl_byte_val.field.rd_wr = 1;

    /* While the bus is busy */
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {
        /* Turn on LD2 and LD3 */
        //STM32vldiscovery_LEDOn(LED3);

        /* Insert delay */
        vTaskDelay(10);
        /* Turn off LD3 and LD4 */
        //STM32vldiscovery_LEDOff(LED3);

        /* Insert delay */
        //vTaskDelay(10);
        // at this point starts the led toggling because the BYSY FLAG is set
        //if uncomment the following line a software reset accours and the led
        //had switched down after one cycle

        //I2C_SoftwareResetCmd(I2C1, ENABLE);
    }

    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
        ;

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;

    /* Send the EEPROM's internal address to write to : MSB of the address first */
    I2C_SendData(I2C1, (uint8_t)((WriteAddr & 0xFF00) >> 8));

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    /* Send the EEPROM's internal address to write to : LSB of the address */
    I2C_SendData(I2C1, (uint8_t)(WriteAddr & 0x00FF));

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;

    /* While there is data to be written */
    while (NumByteToWrite--)
    {

        /* Send the current byte */
        I2C_SendData(I2C1, *pBuffer);

        /* Point to the next byte to be written */
        pBuffer++;

        /* Test on EV8 and clear it */
        while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
            ;
    }

    /* Send STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
}

/***************************************************************************/
/** * @brief  Wait for EEPROM Standby state
 ******************************************************************************/

void I2C1_WaitEepromStandbyState(void)
{
    DEV_SELECT_VAL_UT ctrl_byte_val;

    ctrl_byte_val.whole = 0;

    ctrl_byte_val.field.dev_type = 0x0A;
    ctrl_byte_val.field.addr_ms_bits = (0 & 0x00030000) >> 16;
    ctrl_byte_val.field.e2 = 0;
    ctrl_byte_val.field.rd_wr = 1;

    __IO uint16_t SR1_Tmp = 0;

    do
    {

        /* Send START condition */
        I2C_GenerateSTART(I2C1, ENABLE);

        /* Read I2C1 SR1 register to clear pending flags */
        SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);

        /* Send EEPROM address for write */
        I2C_Send7bitAddress(I2C1, ctrl_byte_val.whole, I2C_Direction_Transmitter);
    } while (!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

    /* Clear AF flag */
    I2C_ClearFlag(I2C1, I2C_FLAG_AF);

    /* STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
}

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

