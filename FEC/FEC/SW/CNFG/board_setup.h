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
 extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
//  #define USE_SPI

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
uint32_t ChangeUsartBoudrate(USART_TypeDef *UsartName,uint32_t Des_USART_BaudRate);  
void stm32f2_usart_cnfg();

/*===========================================================================*/
/*         ***** P I N S    C O N F I G U R A T I O N   D A T A   *****      */
/*===========================================================================*/
  #ifdef BOARD_CNFG
   
    PIN_CNFG_ST pins_cfng_data[] = {
#if 0    
/*---------------------------------------------------------------------------*/
/*                        S P I _ 3   R E L A T E D                          */
/*---------------------------------------------------------------------------*/
          {/* SPI3_RDY1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource7,          /* pin_src    */
              EXTI_PortSourceGPIOE,     /* port_src   */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_IN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* SPI3_RDY2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource2,          /* pin_src    */
              EXTI_PortSourceGPIOF,     /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_2,             /* GPIO_Pin   */
                GPIO_Mode_IN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* SPI3_CS1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource2,          /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_2,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* SPI3_CS2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource3,          /* pin_src    */
              EXTI_PortSourceGPIOD,     /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/*---------------------------------------------------------------------------*/
/*                          S P I _ 3   P I N S                              */
/*---------------------------------------------------------------------------*/
           {/* SPI3_SCK */
              1,                        /* pre_out_val */
              GPIO_AF_SPI3,             /* AltFunNum  */
              EXTI_PinSource10,         /* PinSource  */
              EXTI_PortSourceGPIOC,     /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,            /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
           {/* SPI3_MISO */
              1,                        /* pre_out_val */
              GPIO_AF_SPI3,             /* AltFunNum  */
              EXTI_PinSource11,         /* PinSource  */
              EXTI_PortSourceGPIOC,     /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
           {/* SPI3_MOSI */
              1,                        /* pre_out_val */
              GPIO_AF_SPI3,             /* AltFunNum  */
              EXTI_PinSource12,         /* PinSource  */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
#endif          
/*----------------------------------------------------------------------------*/
/*             S Y N C(Z E R O  -  C R O S S I N G)   P I N S                 */
/*----------------------------------------------------------------------------*/      
          {/* SYNC_1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum   */
              EXTI_PinSource7,          /* pin_src     */
              EXTI_PortSourceGPIOG,     /* port_src    */           
              GPIOG,                    /* GPIO_Port   */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun   */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock  */
              { 
                GPIO_Pin_7,             /* GPIO_Pin    */
                GPIO_Mode_IN,           /* GPIO_Mode   */
                GPIO_Speed_2MHz,        /* GPIO_Speed  */
                GPIO_OType_PP,          /* GPIO_OType  */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd   */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* SYNC_2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum   */
              EXTI_PinSource8,          /* pin_src     */
              EXTI_PortSourceGPIOG,     /* port_src    */           
              GPIOG,                    /* GPIO_Port   */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun   */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock  */
              { 
                GPIO_Pin_8,             /* GPIO_Pin    */
                GPIO_Mode_IN,           /* GPIO_Mode   */
                GPIO_Speed_2MHz,        /* GPIO_Speed  */
                GPIO_OType_PP,          /* GPIO_OType  */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd   */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* SYNC_3 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource0,          /* pin_src    */
              EXTI_PortSourceGPIOF,     /* port_src   */           
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_0,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* SYNC_4 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource15,          /* pin_src    */
              EXTI_PortSourceGPIOD,     /* port_src   */           
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_15,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },           
/*---------------------------------------------------------------------------*/
/*                            G P I O _ I N P U T s                          */
/*---------------------------------------------------------------------------*/
          {/* GPIO_IN1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource9,          /* pin_src    */
              EXTI_PortSourceGPIOB,     /* port_src   */           
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource10,          /* pin_src    */
              EXTI_PortSourceGPIOB,     /* port_src   */           
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN3 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource14,          /* pin_src    */
              EXTI_PortSourceGPIOB,     /* port_src   */           
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_14,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN4 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource15,         /* pin_src    */
              EXTI_PortSourceGPIOB,     /* port_src   */           
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_15,            /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN5 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource13,         /* pin_src    */
              EXTI_PortSourceGPIOD,     /* port_src   */           
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,            /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN6 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource14,         /* pin_src    */
              EXTI_PortSourceGPIOE,     /* port_src   */           
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_14,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },          
          {/* GPIO_IN7 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource14,          /* pin_src    */
              EXTI_PortSourceGPIOD,     /* port_src   */           
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_14,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN8 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource0,          /* pin_src    */
              EXTI_PortSourceGPIOE,     /* port_src   */           
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_0,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN9 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource1,          /* pin_src    */
              EXTI_PortSourceGPIOE,     /* port_src   */           
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_1,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN10 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource2,          /* pin_src    */
              EXTI_PortSourceGPIOE,     /* port_src   */           
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_2,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN11 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource3,          /* pin_src    */
              EXTI_PortSourceGPIOE,     /* port_src   */           
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* GPIO_IN12 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource4,          /* pin_src    */
              EXTI_PortSourceGPIOE,     /* port_src   */           
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_4,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
/*---------------------------------------------------------------------------*/
/*                              I 2 C   P I N S                              */
/*---------------------------------------------------------------------------*/
/* 139 */  {/* I2C1 SCL */
              1,                        /* pre_out_val */
              GPIO_AF_I2C1,             /* AltFunNum  */
              EXTI_PinSource8,          /* PinSource  */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 137 */  {/* I2C1 SDA */
              1,                        /* pre_out_val */
              GPIO_AF_I2C1,             /* AltFunNum  */
              EXTI_PinSource7,          /* PinSource  */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock RCC_APB1Periph_I2C1*/
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*---------------------------------------------------------------------------*/
/*                        B O A R D _ I D   P I N S                          */
/*---------------------------------------------------------------------------*/
/* 128 */ {/* BOARD_ID_0 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource13,          /* pin_src    */
              EXTI_PortSourceGPIOG,     /* port_src   */           
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },      
/* 11 */  {/* BOARD_ID_1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource1,          /* pin_src    */
              EXTI_PortSourceGPIOF,     /* port_src   */           
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_1,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
/* 132 */ {/* BOARD_ID_2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource15,         /* pin_src    */
              EXTI_PortSourceGPIOG,     /* port_src   */           
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_15,            /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },

/*---------------------------------------------------------------------------*/
/*                        N O D E _ I D   P I N S                            */
/*---------------------------------------------------------------------------*/
          {/* NODE_ID_0 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource10,          /* pin_src    */
              EXTI_PortSourceGPIOG,     /* port_src   */           
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },      
          {/* NODE_ID_1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource11,          /* pin_src    */
              EXTI_PortSourceGPIOG,     /* port_src   */           
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
          {/* NODE_ID_2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource12,         /* pin_src    */
              EXTI_PortSourceGPIOG,     /* port_src   */           
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,            /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
           },
           
/*---------------------------------------------------------------------------*/
/*                             G P I O _ O U T s                             */
/*---------------------------------------------------------------------------*/
           {/* GPIO_OUT1 SAFETY*/
              0,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              GPIO_PinSource12,         /* pin_src  */
              0,                        /* port_src */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,            /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN          /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },       
/*---------------------------------------------------------------------------*/
/*            M O D B U S   D A T A   D I R E C T I O N   P I N S            */
/*---------------------------------------------------------------------------*/         
/*------------------------   MOD_BUS_2    DE_2       ------------------------*/
          { /* DE2 PA10 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,            /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },      
/*---------------------------------------------------------------------------*/
/*                              O P T O _ O U T s                            */
/*---------------------------------------------------------------------------*/
           {/* OPTO_OUT1 PWM SW only */
              0,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              GPIO_PinSource8,          /* pin_src    */
              0,                        /* port_src   */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT2 */
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM1,*/                     /* AltFunNum  */
              GPIO_PinSource9,                     /* pin_src  */
              0,                        /* port_src */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/          /* GPIO_Mode  */
                GPIO_Speed_100MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT3 PWM SW only */
              0,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              GPIO_PinSource10,                     /* pin_src  */
              0,                        /* port_src */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/          /* GPIO_Mode  */
                GPIO_Speed_100MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT4 */
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM1,*/             /* AltFunNum  */
              GPIO_PinSource11,         /* pin_src  */
              0,                        /* port_src */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,            /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT5 PWM SW only */
              0,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              GPIO_PinSource12,         /* pin_src  */
              0,                        /* port_src */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,            /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT6 */
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM1,*/             /* AltFunNum  */
              GPIO_PinSource13,         /* pin_src  */
              0,                        /* port_src */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,            /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT7 */
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM9,*/             /* AltFunNum  */
              GPIO_PinSource5,          /* pin_src  */
              0,                        /* port_src */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT8 */
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM9,*/             /* AltFunNum  */
              GPIO_PinSource6,          /* pin_src  */
              0,                        /* port_src */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT9 */
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM8,*/             /* AltFunNum  */
              GPIO_PinSource6,          /* pin_src  */
              0,                        /* port_src */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT10 */
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM8,*/             /* AltFunNum  */
              GPIO_PinSource7,          /* pin_src  */
              0,                        /* port_src */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT11 */
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM8,*/             /* AltFunNum   */
              GPIO_PinSource8,         /* pin_src     */
              0,                        /* port_src    */
              GPIOC,                    /* GPIO_Port   */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun   */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock  */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* OPTO_OUT 12*/
              0,                        /* pre_out_val */
              0xFF,/*GPIO_AF_TIM8,*/             /* AltFunNum   */
              GPIO_PinSource9,          /* pin_src     */
              0,                        /* port_src    */
              GPIOC,                    /* GPIO_Port   */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun   */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock  */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_OUT,/*GPIO_Mode_AF,*/           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/*************************         A 2 D s          ***************************/
/* 1 */   {/* ADC1_IN0  (PA0) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_0,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 2 */   {/* ADC1_IN3  (PA3) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 3 */   {/* ADC1_IN5  (PA5) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 4 */   {/* ADC1_IN6  (PA6) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 5 */   {/* ADC1_IN8  (PB0) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_0,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 6 */   {/* ADC1_IN9  (PB1) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_1,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 7 */   {/* ADC1_IN10 (PC0) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_0,             /* GPIO_Pin   */
                GPIO_Mode_AN,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 8 */   {/* ADC1_IN12 (PC2) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_2,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 9 */   {/* ADC1_IN13 (PC3) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 10 */  {/* ADC3_IN9  (PF3) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 11 */  {/* ADC3_IN14 (PF4) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_4,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 12 */  {/* ADC3_IN15 (PF5) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 13 */  {/* ADC3_IN4  (PF6) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 14 */  {/* ADC3_IN5  (PF7) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 15 */  {/* ADC3_IN6  (PF8) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 16 */  {/* ADC3_IN7  (PF9) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 17 */  {/* ADC3_IN8  (PF10) */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_AN,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/*************************     E T H E R N E T      ***************************/

/* 0 */   {/* ETH_RMII_REF_CLK (PA1) */
              1,                        /* pre_out_val */
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource1,          /* PinSource  */
              0,                        /* port_src   */              
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_1,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
          },            
/* 1 */   {/* ETH_MDIO (PA2)*/
              1,                        /* pre_out_val */  
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource2,          /* PinSource  */
              0,                        /* port_src   */              
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_2,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
/* 2 */     {/* ETH_RMII_CRS_DV (PA7) */
              1,                        /* pre_out_val */
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource7,          /* PinSource  */
              0,                        /* port_src   */              
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },

/* 3 */     {/* ETH_PWR_DOWN/ETH_INT  RESET_PHY (PA8) */
              1,                        /* pre_out_val */  
              0xFF,                     /* AltFunNum   */
              GPIO_PinSource8,         /* PinSource   */
              0,                        /* port_src    */
              GPIOA,                    /* GPIO_Port   */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun   */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock  */
              { 
                GPIO_Pin_8,             /* GPIO_Pin    */
                GPIO_Mode_IN,           /* GPIO_Mode   */
                GPIO_Speed_100MHz,      /* GPIO_Speed  */
                GPIO_OType_OD,          /* GPIO_OType  */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd   */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
           
/* 4 */     {/* ETH_RMII_TX_EN (PB11) */
              1,                        /* pre_out_val */  
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource11,         /* PinSource  */
              0,                        /* port_src   */              
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,            /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
           
/* 5 */     {/* ETH_RMII_TXD0 (PB12) */
              1,                        /* pre_out_val */  
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource12,         /* PinSource  */
              0,                        /* port_src   */              
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,            /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
/* 6 */     {/* ETH_RMII_TXD1 (PB13) */
              1,                        /* pre_out_val */  
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource13,         /* PinSource  */
              0,                        /* port_src   */              
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,            /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },           
            
/* 7 */     {/* ETH_MDC (PC1) */
              1,                        /* pre_out_val */
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource1,          /* PinSource  */
              0,                        /* port_src   */              
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_1,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
/* 8 */     {/* ETH_RMII_RXD0 (PC4) */
              1,                        /* pre_out_val */  
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource4,          /* PinSource  */
              0,                        /* port_src   */              
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_4,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
/* 9 */      {/* ETH_RMII_RXD1 (PC5) */
              1,                        /* pre_out_val */  
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource5,          /* PinSource  */
              0,                        /* port_src   */              
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
           
           
/*---------------------------------------------------------------------------*/
/*                              U S A R T s   P I N S                        */
/*---------------------------------------------------------------------------*/
/*------------------------    RS_485_2   (USART3)    ------------------------*/
            {/* USART3_TX */
              1,                        /* pre_out_val */
              GPIO_AF_USART3,           /* AltFunNum  */
              GPIO_PinSource8,          /* PinSource  */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
           {/* USART3_RX */
              1,                        /* pre_out_val */
              GPIO_AF_USART3,           /* AltFunNum  */
              GPIO_PinSource9,         /* PinSource  */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*------------------------    AUX_TERM   (USART6)    ------------------------*/
           {/* USART6_TX */ 
              1,                        /* pre_out_val */
              GPIO_AF_USART6,           /* AltFunNum  */
              GPIO_PinSource14,          /* PinSource  */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_14,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
/* 144 */   {/* USART6_RX */
              1,                        /* pre_out_val */
              GPIO_AF_USART6,           /* AltFunNum  */
              GPIO_PinSource9,          /* PinSource  */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },

/*---------------------------------------------------------------------------*/
/*                              L E D s                                      */
/*---------------------------------------------------------------------------*/
           {/* MCU_LED1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_0,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },           
           {/* MCU_LED2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_1,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* MCU_LED3*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_2,            /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* MCU_LED4 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           {/* MCU_LED5 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_4,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED6 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED7 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED8 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED9 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_14,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED10 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_15,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED11 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED12 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED13 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED14 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* MCU_LED15 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_15,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
         
/******************************************************************************/           
    };/* PIN_CNFG_ST pins_cfng_data */
/******************************************************************************/

/******************************************************************************/
    SPI_CNFG_ST spi_cfng_data[] = {
/******************************************************************************/      
      {
        SPI3,
        RCC_APB2PeriphClockCmd,
        RCC_APB2Periph_SPI1,
        {
          SPI_Direction_2Lines_FullDuplex,
          
          SPI_Mode_Master,           /*!<  uint16_t SPI_Mode;                  Specifies the SPI operating mode. This parameter can be a value of @ref SPI_mode */

          SPI_DataSize_16b,          /*!<  uint16_t SPI_DataSize;            Specifies the SPI data size. This parameter can be a value of @ref SPI_data_size */

          SPI_CPOL_Low,              /*!<  uint16_t SPI_CPOL;                 Specifies the serial clock steady state. This parameter can be a value of @ref SPI_Clock_Polarity */

          SPI_CPHA_2Edge,            /*!<  uint16_t SPI_CPHA;                 Specifies the clock active edge for the bit capture. This parameter can be a value of @ref SPI_Clock_Phase */

          SPI_NSS_Soft,              /*!<  uint16_t SPI_NSS;                  Specifies whether the NSS signal is managed by hardware (NSS pin) or by software using the SSI bit. This parameter can be a value of @ref SPI_Slave_Select_management */
 
          SPI_BaudRatePrescaler_8,   /*!<  uint16_t SPI_BaudRatePrescaler;    Specifies the Baud Rate prescaler value which will be used to configure the transmit and receive SCK clock. 
                                                  This parameter can be a value of @ref SPI_BaudRate_Prescaler
                                                  @note The communication clock is derived from the master clock. The slave clock does not need to be set. */

          SPI_FirstBit_MSB,           /*!<  uint16_t SPI_FirstBit;             Specifies whether data transfers start from MSB or LSB bit. This parameter can be a value of @ref SPI_MSB_LSB_transmission */
          15                          /*!<  uint16_t SPI_CRCPolynomial;        Specifies the polynomial used for the CRC calculation. */
        }
      }      
/****************************************************************************/
    };/* SPI_CNFG_ST spi_cfng_data[] */
/****************************************************************************/

/******************************************************************************/
    USART_CNFG_ST usarts_cfng_data[] = {
/******************************************************************************/
      {
        USART3,                            /* *UsartName                    */
        RCC_APB1Periph_USART3,             /*  UsartClk                     */
        RCC_APB1PeriphClockCmd,            /*  ClkCmdFun                    */
        {                                  /*  USART_InitTypeDef CommParams */
          38400,                           /*  USART_BaudRate               */
          USART_WordLength_8b,             /*  USART_WordLength             */          
          USART_StopBits_1,                /*  USART_StopBits               */            
          USART_Parity_No,                 /*  USART_Parity                 */             
          USART_Mode_Rx | USART_Mode_Tx,   /*  USART_Mode                   */               
          USART_HardwareFlowControl_None   /*  USART_HardwareFlowControl    */         
        },
        {                                  /* NVIC_InitTypeDef NvicParams   */
          USART3_IRQn,                     /* NVIC_IRQChannel               */                    
          (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY>>4)+2,                               /* NVIC_IRQChannelPreemptionPriority */  //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY
          0,                               /* NVIC_IRQChannelSubPriority    */         
          ENABLE,                          /* NVIC_IRQChannelCmd            */            
        }
      },          
      {
        USART6,                            /* *UsartName                    */
        RCC_APB2Periph_USART6,             /*  UsartClk                     */
        RCC_APB2PeriphClockCmd,            /*  ClkCmdFun                    */
        {                                  /*  USART_InitTypeDef CommParams */
          115200,                           /*  USART_BaudRate               */
          USART_WordLength_8b,             /*  USART_WordLength             */          
          USART_StopBits_1,                /*  USART_StopBits               */            
          USART_Parity_No,                 /*  USART_Parity                 */             
          USART_Mode_Rx | USART_Mode_Tx,   /*  USART_Mode                   */               
          USART_HardwareFlowControl_None   /*  USART_HardwareFlowControl    */         
        },
        {                                  /* NVIC_InitTypeDef NvicParams   */
          USART6_IRQn,                     /* NVIC_IRQChannel               */                    
          (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY>>4)+3, /* NVIC_IRQChannelPreemptionPriority */  //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY
          0,                               /* NVIC_IRQChannelSubPriority    */         
          ENABLE,                          /* NVIC_IRQChannelCmd            */            
        }
      },
      
/****************************************************************************/
    };/* USART_CNFG_ST usarts_cfng_data[] */
/****************************************************************************/    
   
  #endif /* #ifdef BOARD_CNFG */




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
              0,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */              
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN          /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* WDI */
              0,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */              
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN          /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },         

#endif
/*---------------------------------------------------------------------------*/
/*                         S S R s   O U T P U T S                           */
/*---------------------------------------------------------------------------*/              
#if 0
              {/* PID_SSR1 -PD#5*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
              {/* PID_SSR2 -PD#6*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
            {/* PID_SSR3 -PD#7*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
            {/* PID_SSR4 -PD#8*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
            {/* PID_SSR5 -PD#9*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
            {/* PID_SSR6 -PD#10*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
            {/* PID_SSR7 -PD#11*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
            {/* PID_SSR8 -PD#12*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
            {/* PID_SSR9 -PD#13*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
            {/* PID_SSR10 -PD#14*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_14,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
            {/* PID_SSR11 -PD#15*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_15,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  

/* AIR VALVE OUTPUTS*/
            
           {/* VALVE-CR1-IN -PF#0*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_0,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
                     
          {/* VALVE-CR1-OUT -PF#1*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_1,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
          
           {/* VALVE-CR2-IN -PF#2*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_2,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
           
           {/* VALVE-CR2-OUT -PF#3*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
           

/*ALARM*/
          {/* ALARM#1 -PF#15*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_15,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           } 
#endif
#if 0      

#endif
#if 0
/*---------------------------------------------------------------------------*/
/*                        S P I _ 3   C S   P I N S                          */
/*---------------------------------------------------------------------------*/          
          {/* SPI3_CS3 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                     /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,      /* GPIO_Clock */
              { 
                GPIO_Pin_4,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* SPI3_CS4 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                     /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,      /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
          {/* SPI3_CS5 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },          
      
#endif
