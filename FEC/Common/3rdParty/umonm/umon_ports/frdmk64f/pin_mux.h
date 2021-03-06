/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : pin_mux.h
**     Project     : twrk64f120m
**     Processor   : MK64FN1M0VLQ12
**     Component   : PinSettings
**     Version     : Component 01.002, Driver 1.1, CPU db: 3.00.000
**     Compiler    : IAR ARM C Compiler
**     Date/Time   : 2013-12-10, 14:58, # CodeGen: 7
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         CAN                 - void pin_mux_CAN(uint32_t instance);
**         ENET                - void pin_mux_ENET(uint32_t instance);
**         GPIO                - void pin_mux_GPIO(uint32_t instance);
**         I2C                 - void pin_mux_I2C(uint32_t instance);
**         RTC                 - void pin_mux_RTC(uint32_t instance);
**         SDHC                - void pin_mux_SDHC(uint32_t instance);
**         SPI                 - void pin_mux_SPI(uint32_t instance);
**         UART                - void pin_mux_UART(uint32_t instance);
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file pin_mux.h
** @version 1.1
** @brief
**
*/         
/*!
**  @addtogroup pin_mux_module pin_mux module documentation
**  @{
*/         

#ifndef pin_mux_H_
#define pin_mux_H_

/* MODULE pin_mux. */


/*
** ===================================================================
**     Method      :  pin_mux_CAN (component PinSettings)
*/
/*!
**     @brief
**         CAN method sets registers according routing settings. Call
**         this method code to route desired pins into CAN0 periphery.
**     @param
**         uint32_t instance - CAN instance number (0 is expected)
*/
/* ===================================================================*/
void pin_mux_CAN(uint32_t instance);
#define configure_can_pin_mux pin_mux_CAN
/*
** ===================================================================
**     Method      :  pin_mux_ENET (component PinSettings)
*/
/*!
**     @brief
**         ENET method sets registers according routing settings. Call
**         this method code to route desired pins into ENET periphery.
**     @param
**         uint32_t instance - ENET instance number (0 is expected)
*/
/* ===================================================================*/
void pin_mux_ENET(uint32_t instance);
#define configure_enet_pin_mux pin_mux_ENET
/*
** ===================================================================
**     Method      :  pin_mux_GPIO (component PinSettings)
*/
/*!
**     @brief
**         GPIO method sets registers according routing settings. Call
**         this method code to route desired pins into:
**         PTA, PTB, PTC, PTD, PTE
**         peripherals.
**     @param
**         uint32_t instance - GPIO instance number 0..4
*/
/* ===================================================================*/
void pin_mux_GPIO(uint32_t instance);
#define configure_gpio_pin_mux pin_mux_GPIO
/*
** ===================================================================
**     Method      :  pin_mux_I2C (component PinSettings)
*/
/*!
**     @brief
**         I2C method sets registers according routing settings. Call
**         this method code to route desired pins into:
**         I2C0, I2C1, I2C2
**         peripherals.
**     @param
**         uint32_t instance - I2C instance number 0..2
*/
/* ===================================================================*/
void pin_mux_I2C(uint32_t instance);
#define configure_i2c_pin_mux pin_mux_I2C
/*
** ===================================================================
**     Method      :  pin_mux_RTC (component PinSettings)
*/
/*!
**     @brief
**         RTC method sets registers according routing settings. Call
**         this method code to route desired pins into RTC periphery.
**     @param
**         uint32_t instance - RTC instance number (0 is expected)
*/
/* ===================================================================*/
void pin_mux_RTC(uint32_t instance);
#define configure_rtc_pin_mux pin_mux_RTC
/*
** ===================================================================
**     Method      :  pin_mux_SDHC (component PinSettings)
*/
/*!
**     @brief
**         SDHC method sets registers according routing settings. Call
**         this method code to route desired pins into SDHC periphery.
**     @param
**         uint32_t instance - SDHC instance number (0 is expected)
*/
/* ===================================================================*/
void pin_mux_SDHC(uint32_t instance);
#define configure_sdhc_pin_mux pin_mux_SDHC
/*
** ===================================================================
**     Method      :  pin_mux_SPI (component PinSettings)
*/
/*!
**     @brief
**         SPI method sets registers according routing settings. Call
**         this method code to route desired pins into:
**         SPI0, SPI1, SPI2
**         peripherals.
**     @param
**         uint32_t instance - SPI instance number 0..2
*/
/* ===================================================================*/
void pin_mux_SPI(uint32_t instance);
#define configure_spi_pin_mux pin_mux_SPI
/*
** ===================================================================
**     Method      :  pin_mux_UART (component PinSettings)
*/
/*!
**     @brief
**         UART method sets registers according routing settings. Call
**         this method code to route desired pins into:
**         UART0, UART1, UART2, UART3, UART4, UART5
**         peripherals.
**     @param
**         uint32_t instance - UART instance number 0..5
*/
/* ===================================================================*/
void pin_mux_UART(uint32_t instance);
#define configure_uart_pin_mux pin_mux_UART
/*
** ===================================================================
**     Method      :  pin_mux_I2S (component PinSettings)
*/
/*!
**     @brief
**         I2S method sets registers according routing settings. Call
**         this method code to route desired pins into:
**         I2S peripherals.
**     @param
**         uint32_t instance - I2S instance number 0
*/
/* ===================================================================*/
void pin_mux_I2S(uint32_t instance);
#define configure_i2s_pin_mux pin_mux_I2S

void pin_mux_CMP(uint32_t instance);
#define configure_cmp_pin_mux pin_mux_CMP

void pin_mux_SPI_CS0(uint32_t instance);
#define configure_spi_cs0_pin_mux pin_mux_SPI_CS0

void pin_mux_SPI_CS1(uint32_t instance);
#define configure_spi_cs1_pin_mux pin_mux_SPI_CS1

void pin_mux_FTM(uint32_t instance);
#define configure_ftm_pin_mux pin_mux_FTM

void pin_mux_GPIO_I2C(uint32_t instance);
#define configure_gpio_i2c_pin_mux pin_mux_GPIO_I2C

void pin_mux_GPIO_UART(uint32_t instance);
#define configure_gpio_uart_pin_mux pin_mux_GPIO_UART

/* END pin_mux. */
#endif /* #ifndef __pin_mux_H_ */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
