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
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif
//#include "stm32f2xx_rcc.h"
//#include "stm32f2xx_gpio.h"
//#include "stm32f2xx_usart.h"
//#include "stm32f2xx_spi.h"
//#include "stm32f2xx_exti.h"
//#include "stm32f2xx_syscfg.h"
//#include "stm32f2xx_adc.h"
//#include "stm32f2x7_eth_bsp.h"
#include "misc.h"
#include "usart.h"
//  #include "usb_conf.h"
//  #include "usb_core.h"
#include "DigTrimmer_ad5292bruz.h"
#include "dac.h"
//  #include "eeprom_m24m02_dr.h"
#include "eeprom_cnfg.h"
#ifndef PHANTOM_BOARD 
//#include "can.h"
#endif
#include <psocSpiWithDma.h>
#include <iwdg.h>
#include <rtc.h>

__no_init uint32_t StayInBootLoader @ 0x20000000;
//uint32_t checksum;

/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
//  __ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */

/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
typedef void (*rcc_prph_clk_cmd)(uint32_t prph_name, FunctionalState NewState);

/* --- PRIVATE TYPES -------------------------------------------------------- */
typedef struct
{
    uint32_t pre_out_val;
    uint8_t AltFunNum;
    uint8_t pin_src;
    uint8_t port_src;
    GPIO_TypeDef* GPIO_Port;
    rcc_prph_clk_cmd ClkCmdFun;
    uint32_t GPIO_Clock;
    GPIO_InitTypeDef pin;
    EXTI_InitTypeDef exin;
    NVIC_InitTypeDef nvic;
} PIN_CNFG_ST, *PIN_CNFG_PS;
/*----------------------------------------------------------------------------*/
typedef struct
{
    USART_TypeDef *UsartName;
//    uint32_t            UsartRole;
    uint32_t UsartClk;
    rcc_prph_clk_cmd ClkCmdFun;
    USART_InitTypeDef CommParams;
    NVIC_InitTypeDef NvicParams;
} USART_CNFG_ST, *USART_CNFG_PS;
/*----------------------------------------------------------------------------*/
typedef struct
{
    SPI_TypeDef *spi_name;
    rcc_prph_clk_cmd clk_cmd_fun;
    uint32_t spi_clk;
    SPI_InitTypeDef spi_init_params;

} SPI_CNFG_ST, *SPI_CNFG_PS;

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
//void stm32f2xx_pins_cnfg();
void stm32f4xx_pins_cnfg();
void stm32f2_usart_cnfg();
void stm32f2_spi_cnfg();
/* --- PRIVATE_DATA --------------------------------------------------------- */
#include "board_setup.h"
#ifdef STM32F4XX
#include "fec2_pins_config.h"
//#include "fec_pins_config.h"
#else
#include "fec_pins_config.h"
#endif

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
    uint32_t res = ERROR;
    uint32_t i, cnfg_data_size = sizeof(usarts_cfng_data) / sizeof(USART_CNFG_ST);
    USART_InitTypeDef NewCommParams;

    for (i = 0; i < cnfg_data_size; i++)
    {
        if (usarts_cfng_data[i].UsartName == UsartName)
        {
            break;
        }
    }

    if (i < cnfg_data_size)
    {
        memcpy(&(NewCommParams), &(usarts_cfng_data[i].CommParams), sizeof(USART_InitTypeDef));
        NewCommParams.USART_BaudRate = Des_USART_BaudRate;
        USART_Init(usarts_cfng_data[i].UsartName, &(NewCommParams));
        res = OK;
    }

    return res;
}/*  */

/*
 Configures a GPIO as output and holds it hi or low a time.
 */
void hold_gp_out_(GPIO_TypeDef* GPIO_Port, uint32_t GPIO_Pin, uint32_t out_val, uint32_t usec)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint32_t count = 0;
    const uint32_t utime = (120 * usec / 7);

    /* Enable GPIOs clocks */

    if (GPIO_Port == GPIOA)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    if (GPIO_Port == GPIOB)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    if (GPIO_Port == GPIOC)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    if (GPIO_Port == GPIOD)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    if (GPIO_Port == GPIOE)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    if (GPIO_Port == GPIOF)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    if (GPIO_Port == GPIOG)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    if (GPIO_Port == GPIOH)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

    if (GPIO_Port == GPIOI)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);

    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Configure MCO (PA8) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);
    if (out_val == 0)
        GPIO_Port->ODR &= ~(GPIO_Pin);
    else
        GPIO_Port->ODR |= GPIO_Pin;

    do
    {
        if (++count > utime)
        {
            return;
        }
    } while (1);

}

/*----------------------------------------------------------------------------
 Configures and initializes the Mosquito Board.
 *----------------------------------------------------------------------------*/
void prvSetupHardware()
{

    /* 4 bit for pre-emption priority, 0 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

#ifdef STM32F4XX
    stm32f4xx_pins_cnfg();/* Keep this first! */
#else
    stm32f2xx_pins_cnfg();/* Keep this first! */
#endif

#ifndef FEC_BOOTLOADER
    stm32f2_usart_cnfg();
#endif

#ifdef FEC_BOARD    

//    Init_CAN();

//    PSOC_setupSpiWithDma();

//    DigitalTrimmersInit();
//    while (1)
//    {
//    write_2_Trimmer(TRIMMER1, 'a', 0);
//    write_2_Trimmer(TRIMMER2, 'a', 0);
//    write_2_Trimmer(TRIMMER1, 'b', 0);
//    write_2_Trimmer(TRIMMER2, 'b', 0);
//    write_2_Trimmer(TRIMMER1, 'c', 0);
//    write_2_Trimmer(TRIMMER2, 'c', 0);
//    write_2_Trimmer(TRIMMER1, 'd', 0);
//    write_2_Trimmer(TRIMMER2, 'd', 0);
//    write_2_Trimmer(TRIMMER1, 'e', 0);
//    write_2_Trimmer(TRIMMER2, 'e', 0);
//    write_2_Trimmer(TRIMMER1, 'f', 0);
//    write_2_Trimmer(TRIMMER2, 'f', 0);
//    write_2_Trimmer(TRIMMER1, 'g', 0);
//    write_2_Trimmer(TRIMMER2, 'g', 0);
//    write_2_Trimmer(TRIMMER1, 'h', 0);
//    write_2_Trimmer(TRIMMER2, 'h', 0);
//    }

    init_DAC1();
    i2c_init();
#endif

#ifdef USE_IWDG
    iwdg_init();
#endif

#ifndef FEC_BOOTLOADER
    rtc_init();
    RCC_AHB2PeriphResetCmd(RCC_AHB2Periph_RNG, ENABLE);
    RNG_Cmd(ENABLE);
#endif

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
    uint32_t i, cnfg_data_size = sizeof(usarts_cfng_data) / sizeof(USART_CNFG_ST);

    for (i = 0; i < cnfg_data_size; i++)
    {
        init_usart(usarts_cfng_data[i].UsartName);
        usarts_cfng_data[i].ClkCmdFun(usarts_cfng_data[i].UsartClk, ENABLE);
        USART_Init(usarts_cfng_data[i].UsartName, &(usarts_cfng_data[i].CommParams));
        NVIC_Init(&(usarts_cfng_data[i].NvicParams));
        USART_Cmd(usarts_cfng_data[i].UsartName, ENABLE);
        USART_ITConfig(usarts_cfng_data[i].UsartName, USART_IT_RXNE, ENABLE);
//      USART_ITConfig(usarts_cfng_data[i].UsartName, USART_IT_TC, ENABLE);
    }/* for */

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
    typedef void (*pFunction)(void);

    switch (bootloader)
    {
    // this means normal boot.
    case 0:
        StayInBootLoader = 0;
        break;
    // this means stay in bootloader no matter what.
    case 1:
        StayInBootLoader = 0x11111111;
        break;
    // this means to burn the application from temporary area to main area.
    case 2:
        StayInBootLoader = 0x22222222;
        break;
    }

    NVIC_SystemReset();
}

/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
