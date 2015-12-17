/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : COMMON_BSP
 *
 *  FILE        : board_setup.c
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP
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
 *  2012.08     Igor Natachanny   1.0.0   Generally version
 *
 *******************************************************************************
 Copyright and Proprietary Statment.

 *******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "ll_typedef.h"
#include <string.h>

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif

#include "usart.h"

uint32_t StayInBootLoader;

/* --- PRIVATE_DATA --------------------------------------------------------- */
#include "board_setup.h"

//void I2C1_init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStruct;
//    I2C_InitTypeDef I2C_InitStruct;
//
//    // enable APB1 peripheral clock for I2C1
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//    // enable clock for SCL and SDA pins
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//
//    /* setup SCL and SDA pins
//     * You can connect the I2C1 functions to two different
//     * pins:
//     * 1. SCL on PB6 or PB8
//     * 2. SDA on PB7 or PB9
//     */
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8; // we are going to use PB6 and PB8
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;       // set pins to alternate function
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;      // set GPIO speed
//    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; // set output to open drain --> the line has to be only pulled low, not driven high
//    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;           // Disable pull up resistors
//    GPIO_Init(GPIOB, &GPIO_InitStruct);                 // init GPIOB
//
//    // Connect I2C1 pins to AF
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1); // SCL
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA
//
//    // configure I2C1
//    I2C_InitStruct.I2C_ClockSpeed = 50000;         // 100kHz
//    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;         // I2C mode
//    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle --> standard
//    I2C_InitStruct.I2C_OwnAddress1 = 0x00;          // own address, not relevant in master mode
//    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;        // Enable acknowledge when reading (can be changed later on)
//    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
//    I2C_Init(I2C1, &I2C_InitStruct);                // init I2C1
//    //
//    // enable I2C1
//    I2C_Cmd(I2C1, ENABLE);
//    //I2C_StretchClockCmd(I2C1, ENABLE);
//    //I2C_Init(I2C1, &I2C_InitStruct);
//}

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*
 * Change the Boudrate of the desiried " UsartName " at run time.
 */
uint32_t ChangeUsartBaudrate(USART_TypeDef *UsartName, uint32_t Des_USART_BaudRate)
{
    return OK;
}/*  */

/*
 Configures a GPIO as output and holds it hi or low a time.
 */
void hold_gp_out_(GPIO_TypeDef* GPIO_Port, uint32_t GPIO_Pin, uint32_t out_val, uint32_t usec)
{
}

/*----------------------------------------------------------------------------
 Configures and initializes the Mosquito Board.
 *----------------------------------------------------------------------------*/
void prvSetupHardware()
{
}/* prvSetupHardware */

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/**
 * @brief:  Init a SPI interface as desiried.   
 *
 * @param:  A pointer to a SPI descriptor structure(SPI1 or SPI2).     
 *
 * @retval: zero if success, 1 - if the parameter has incorrect value.  
 **/
void stm32f2_spi_cnfg()
{
#ifdef USE_SPI    
    uint32_t i, cnfg_data_size = sizeof(spi_cfng_data)/sizeof(SPI_CNFG_ST);

    for(i = 0; i < cnfg_data_size; i++)
    {
        SPI_I2S_DeInit(spi_cfng_data[i].spi_name);
        spi_cfng_data[i].clk_cmd_fun(spi_cfng_data[i].spi_clk, ENABLE);
        SPI_Init(spi_cfng_data[i].spi_name, &(spi_cfng_data[i].spi_init_params));
        /* SPI interrupts init shoul be added here */
        SPI_Cmd(spi_cfng_data[i].spi_name, ENABLE); /* Keep this last! */
    }

#endif 

//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
//  SPI_Init(SPI2, &SPI_InitStructure);
    //------------------------------------------------------------------------------

}/*************** uint32_t SPIx__Init() ********************/

/*------------------------------------------------------------------------------
 Configures a USARTs.
 *-----------------------------------------------------------------------------*/
void stm32f2_usart_cnfg()
{
}/* stm32f2_usart_cnfg */
/*----------------------------------------------------------------------------
 Configures and initializes all used pins.
 *----------------------------------------------------------------------------*/
#ifdef STM32F2XX
void stm32f2xx_pins_cnfg()
{
    uint32_t i, cnfg_data_size = sizeof(pins_cfng_data) / sizeof(PIN_CNFG_ST);

    for (i = 0; i < cnfg_data_size; i++)
    {

        pins_cfng_data[i].ClkCmdFun(pins_cfng_data[i].GPIO_Clock, ENABLE);

        if (pins_cfng_data[i].pin.GPIO_Mode == GPIO_Mode_OUT)
        {
            if (pins_cfng_data[i].pre_out_val == 0)
//          pins_cfng_data[i].GPIO_Port->BSRRL = pins_cfng_data[i].pin.GPIO_Pin;
            pins_cfng_data[i].GPIO_Port->ODR &= ~(pins_cfng_data[i].pin.GPIO_Pin);
            else
//          pins_cfng_data[i].GPIO_Port->BSRRH = pins_cfng_data[i].pin.GPIO_Pin;
            pins_cfng_data[i].GPIO_Port->ODR |= pins_cfng_data[i].pin.GPIO_Pin;
        }/* if the pin in OUTPUT mode */

        if (pins_cfng_data[i].pin.GPIO_Mode == GPIO_Mode_IN)
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);/* Enable the Input Clock */
            if (pins_cfng_data[i].exin.EXTI_LineCmd == ENABLE)
            {
                SYSCFG_EXTILineConfig(pins_cfng_data[i].port_src, pins_cfng_data[i].pin_src);/* Connect the GPIO EXTI Line to the GPIO Pin */
                EXTI_Init(&(pins_cfng_data[i].exin));
                NVIC_Init(&(pins_cfng_data[i].nvic));
            }

        }/* if the pin in INPUT mode */

        GPIO_Init((pins_cfng_data[i].GPIO_Port), (&(pins_cfng_data[i].pin)));

        if (pins_cfng_data[i].pin.GPIO_Mode == GPIO_Mode_AF)
        {
            GPIO_PinAFConfig((pins_cfng_data[i].GPIO_Port), (pins_cfng_data[i].pin_src), (pins_cfng_data[i].AltFunNum));
        }

    }/* for */

}/* stm32f2xx_pins_cnfg */
#endif

#ifdef STM32F4XX
void stm32f4xx_pin_group_config(const PIN_CNFG_ST* pinGroup, int groupSize)
{
    uint32_t i;

    for (i = 0; i < groupSize; i++)
    {

        pinGroup[i].ClkCmdFun(pinGroup[i].GPIO_Clock, ENABLE);

        if (pinGroup[i].pin.GPIO_Mode == GPIO_Mode_OUT)
        {
            if (pinGroup[i].pre_out_val == 0)
//          pins_cfng_data[i].GPIO_Port->BSRRL = pins_cfng_data[i].pin.GPIO_Pin;
                pinGroup[i].GPIO_Port->ODR &= ~(pinGroup[i].pin.GPIO_Pin);
            else
//          pins_cfng_data[i].GPIO_Port->BSRRH = pins_cfng_data[i].pin.GPIO_Pin;
                pinGroup[i].GPIO_Port->ODR |= pinGroup[i].pin.GPIO_Pin;
        }/* if the pin in OUTPUT mode */

        if (pinGroup[i].pin.GPIO_Mode == GPIO_Mode_IN)
        {
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);/* Enable the Input Clock */
            if (pinGroup[i].exin.EXTI_LineCmd == ENABLE)
            {
                SYSCFG_EXTILineConfig(pinGroup[i].port_src, pinGroup[i].pin_src);/* Connect the GPIO EXTI Line to the GPIO Pin */
                EXTI_Init(&(pinGroup[i].exin));
                NVIC_Init(&(pinGroup[i].nvic));
            }

        }/* if the pin in INPUT mode */

        GPIO_Init((pinGroup[i].GPIO_Port), (&(pinGroup[i].pin)));

        if (pinGroup[i].pin.GPIO_Mode == GPIO_Mode_AF)
        {
            GPIO_PinAFConfig((pinGroup[i].GPIO_Port), (pinGroup[i].pin_src), (pinGroup[i].AltFunNum));
        }

    }/* for */

}

void stm32f4xx_pins_cnfg()
{
    //, cnfg_data_size = sizeof(pinGroup) / sizeof(PIN_CNFG_ST);
    stm32f4xx_pin_group_config(GPIO_ZeroCross, sizeof(GPIO_ZeroCross) / sizeof(PIN_CNFG_ST));
    stm32f4xx_pin_group_config(GPIO_Inputs, sizeof(GPIO_Inputs) / sizeof(PIN_CNFG_ST));
    stm32f4xx_pin_group_config(GPIO_Outputs, sizeof(GPIO_Outputs) / sizeof(PIN_CNFG_ST));
    stm32f4xx_pin_group_config(GPIO_Leds, sizeof(GPIO_Leds) / sizeof(PIN_CNFG_ST));
    stm32f4xx_pin_group_config(GPIO_ST_PSOC_IO, sizeof(GPIO_ST_PSOC_IO) / sizeof(PIN_CNFG_ST));
    stm32f4xx_pin_group_config(GPIO_A2D, sizeof(GPIO_A2D) / sizeof(PIN_CNFG_ST));
    stm32f4xx_pin_group_config(GPIO_Communications, sizeof(GPIO_Communications) / sizeof(PIN_CNFG_ST));
}
#endif

void resetBoard(int bootloader)
{
}

/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
