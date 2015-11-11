/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : Front End Controller
 *
 *  FILE        : board_setup.h
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\FEC\SW\CNFG
 *
 *  PURPOSE     : This file contains the non-run-time-changebale setup
 *                data for all peripherials of STM32F207SG according to
 *                FEC board requerements.
 *
 *  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
 *
 *  TARGET      :
 *
 *
 *  DATE        NAME              VER.    COMMENTS
 * -----------------------------------------------------------------------------
 *  2012.10     Igor Natachanny   1.0.0   Initial version.
 *
 *******************************************************************************
 Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
 ******************************************************************************/

#ifndef _BOARD_CONFIG_
#define _BOARD_CONFIG_

#include "FreeRtosConfig.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define BOOTLOADER_FLASH_FIRST_PAGE_ADDRESS 0x08000000
#define USER_FLASH_FIRST_PAGE_ADDRESS 0x08020000

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
//  #define USE_SPI
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
extern uint32_t StayInBootLoader;

uint32_t ChangeUsartBaudrate(USART_TypeDef *UsartName,uint32_t Des_USART_BaudRate);
void stm32f2_usart_cnfg();
void prvSetupHardware();
void resetBoard(int bootloader);
/*===========================================================================*/
/*         ***** P I N S    C O N F I G U R A T I O N   D A T A   *****      */
/*===========================================================================*/

#ifdef __cplusplus
}
#endif
#endif /* _BOARD_CONFIG_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/
#if 0            
/*---------------------------------------------------------------------------*/
/*            E X T E R N A L   W A T C H   D O G   C O N T R O L            */
/*---------------------------------------------------------------------------*/
{/* WD_EN */
0, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOG, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOG, /* GPIO_Clock */
{
    GPIO_Pin_6, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_DOWN /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* WDI */
0, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOG, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOG, /* GPIO_Clock */
{
    GPIO_Pin_5, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_DOWN /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},

#endif
/*---------------------------------------------------------------------------*/
/*                         S S R s   O U T P U T S                           */
/*---------------------------------------------------------------------------*/
#if 0
{/* PID_SSR1 -PD#5*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_5, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR2 -PD#6*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_6, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR3 -PD#7*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_7, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR4 -PD#8*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_8, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR5 -PD#9*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_9, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR6 -PD#10*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_10, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR7 -PD#11*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_11, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR8 -PD#12*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_12, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR9 -PD#13*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_13, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR10 -PD#14*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_14, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* PID_SSR11 -PD#15*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_15, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},

/* AIR VALVE OUTPUTS*/

{/* VALVE-CR1-IN -PF#0*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOF, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOF, /* GPIO_Clock */
{
    GPIO_Pin_0, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},

{/* VALVE-CR1-OUT -PF#1*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOF, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOF, /* GPIO_Clock */
{
    GPIO_Pin_1, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},

{/* VALVE-CR2-IN -PF#2*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOF, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOF, /* GPIO_Clock */
{
    GPIO_Pin_2, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},

{/* VALVE-CR2-OUT -PF#3*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOF, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOF, /* GPIO_Clock */
{
    GPIO_Pin_3, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},

/*ALARM*/
{/* ALARM#1 -PF#15*/
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOF, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOF, /* GPIO_Clock */
{
    GPIO_Pin_15, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
}
#endif
#if 0      

#endif
#if 0
/*---------------------------------------------------------------------------*/
/*                        S P I _ 3   C S   P I N S                          */
/*---------------------------------------------------------------------------*/
{/* SPI3_CS3 */
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_4, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* SPI3_CS4 */
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_5, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},
{/* SPI3_CS5 */
1, /* pre_out_val */
0xFF, /* AltFunNum  */
0xFF, /* pin_src    */
0, /* port_src   */
GPIOD, /* GPIO_Port  */
RCC_AHB1PeriphClockCmd, /* ClkCmdFun  */
RCC_AHB1Periph_GPIOD, /* GPIO_Clock */
{
    GPIO_Pin_6, /* GPIO_Pin   */
    GPIO_Mode_OUT, /* GPIO_Mode  */
    GPIO_Speed_50MHz, /* GPIO_Speed */
    GPIO_OType_PP, /* GPIO_OType */
    GPIO_PuPd_UP /* GPIO_PuPd  */
},
{   0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
{   0,0,0,DISABLE},
},

#endif
