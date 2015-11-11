/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : MOSQUITO 
*
*  FILE        : board_setup.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Mosquito\SW\BSP 
*
*  PURPOSE     : This file contains the non-run-time-changebale setup 
*                data for all peripherials of STM32F207SG according to                 
*                PHANTOM board requerements.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : MOSQUITO 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.07     Igor Natachanny   1.1.0   USART3 and it's GPIOs config. data added.
*
*******************************************************************************
      Copyright and Proprietary Statment.

******************************************************************************/

#ifndef _BOARD_CONFIG_
#define _BOARD_CONFIG_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/


/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
   
/*===========================================================================*/
/*         ***** P I N S    C O N F I G U R A T I O N   D A T A   *****      */
/*===========================================================================*/
  #ifdef BOARD_CNFG
   
    PIN_CNFG_ST pins_cfng_data[] = {
/*---------------------------------------------------------------------------*/
/*                         USB_OTG_FS   P I N S                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           NODE_ID   P I N S                               */
/*---------------------------------------------------------------------------*/
/* 83 */   {/* NODE_ID_0 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource2,          /* pin_src    */
              EXTI_PortSourceGPIOD,     /* port_src   */           
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
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
/* 87 */  {/* NODE_ID_1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource6,          /* pin_src    */
              EXTI_PortSourceGPIOD,     /* port_src   */           
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}             
          },
 /* 43 */ {/* NODE_ID_2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource12,         /* pin_src    */
              EXTI_PortSourceGPIOE,     /* port_src   */           
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
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
/*                           ADC123_IN ch11                                 */
/*---------------------------------------------------------------------------*/
/* 16 */   {/*ADC123_IN ch 11 */
              1,                        /* pre_out_val */
              ADC_Channel_11,           /* AltFunNum  */
              EXTI_PinSource2,          /* pin_src    */
              EXTI_PortSourceGPIOD,     /* port_src   */           
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_1,             /* GPIO_Pin   */
                GPIO_Mode_AN,           /* GPIO_Mode  */
                GPIO_Speed_2MHz,        /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              }, 
               {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
/*---------------------------------------------------------------------------*/
/*                              SPI_1   P I N S                              */
/*---------------------------------------------------------------------------*/
/* 29 */   {/* SPI1_NSS */
              1,                        /* pre_out_val */
              GPIO_AF_SPI1,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_4,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 30 */   {/* SPI1_SCK */
              1,                        /* pre_out_val */
              GPIO_AF_SPI1,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 31 */   {/* SPI1_MISO */
              1,                        /* pre_out_val */
              GPIO_AF_SPI1,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 32 */   {/* SPI1_MOSI */
              1,                        /* pre_out_val */
              GPIO_AF_SPI1,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*---------------------------------------------------------------------------*/
/*                              SPI_2   P I N S                              */
/*---------------------------------------------------------------------------*/
/* 51 */   {/* SPI2_NSS */
              1,                        /* pre_out_val */
              GPIO_AF_SPI2,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 47 */   {/* SPI2_SCK */
              1,                        /* pre_out_val */
              GPIO_AF_SPI2,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 17 */   {/* SPI2_MISO */
              1,                        /* pre_out_val */
              GPIO_AF_SPI2,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_2,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 18 */   {/* SPI2_MOSI */
              1,                        /* pre_out_val */
              GPIO_AF_SPI2,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },     
           
 /*---------------------------------------------------------------------------*/
/*                              I2C   P I N S                              */
/*---------------------------------------------------------------------------*/
/* 95 */   {/* I2C1 SCL */
              1,                        /* pre_out_val */
              GPIO_AF_I2C1,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 96 */   {/* I2C1 SDA */
              1,                        /* pre_out_val */
              GPIO_AF_I2C1,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*---------------------------------------------------------------------------*/
/*               M O D B U S   D A T A   E N A B L E   P I N S               */
/*---------------------------------------------------------------------------*/
/*------------------------   MOD_BUS_1    DE_1       ------------------------*/
/* 65 */  { /* RS485_DE1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/*------------------------   MOD_BUS_2    DE_2       ------------------------*/
/* 15 */  { /* RS485_DE2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_0,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/*---------------------------------------------------------------------------*/
/*                              U S A R T s   P I N S                        */
/*---------------------------------------------------------------------------*/
/*------------------------    RS_485_1   (USART3)    ------------------------*/
/* 78 */   {/* RS485_TX1 */
              1,                        /* pre_out_val */
              GPIO_AF_USART3,           /* AltFunNum  */
              GPIO_PinSource10,          /* PinSource  */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
/* 79 */   {/* RS485_RX1 */
              1,                        /* pre_out_val */
              GPIO_AF_USART3,           /* AltFunNum  */
              GPIO_PinSource11,         /* PinSource  */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*------------------------    RS_485_2   (USART6)    ------------------------*/
/* 63 */   {/* RS485_TX2 */
              1,                        /* pre_out_val */
              GPIO_AF_USART6,           /* AltFunNum  */
              GPIO_PinSource6,          /* PinSource  */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_6,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
/* 64 */   {/* RS485_RX2 */
              1,                        /* pre_out_val */
              GPIO_AF_USART6,           /* AltFunNum  */
              GPIO_PinSource7,         /* PinSource  */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_7,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*------------------------   AUX_TERM   (USART1)    -------------------------*/
/* 68 */   {/* USART1_TX */
              1,                        /* pre_out_val */
              GPIO_AF_USART1,           /* AltFunNum  */
              GPIO_PinSource9,          /* PinSource  */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
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
/* 69 */   {/* USART1_RX */
              1,                        /* pre_out_val */
              GPIO_AF_USART1,           /* AltFunNum  */
              GPIO_PinSource10,         /* PinSource  */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*---------------------------------------------------------------------------*/
/*            E X T E R N A L   W A T C H   D O G   C O N T R O L            */
/*---------------------------------------------------------------------------*/
/* 67 */  {/* WD_EN */
              0,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */              
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_DOWN          /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
           
/*---------------------------------------------------------------------------*/
/*                         T E S T   P O I N T S                             */
/*---------------------------------------------------------------------------*/
/* 80 */  {/* MCU_TP_1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,            /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/* 7 */   {/* MCU_TP_2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              EXTI_PinSource13,         /* pin_src    */
              EXTI_PortSourceGPIOC,     /* port_src   */           
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,            /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {
                EXTI_Line13,            /* EXTI_Line */
                EXTI_Mode_Interrupt,    /* EXTI_Mode */
                EXTI_Trigger_Falling,   /* EXTI_Trigger */
                ENABLE                  /* EXTI_LineCmd */
              },
              {
                EXTI15_10_IRQn,         /* NVIC_IRQChannel */
                0x0F,                   /* NVIC_IRQChannelPreemptionPriority */
                0x0F,                   /* NVIC_IRQChannelSubPriority */
                ENABLE                  /* NVIC_IRQChannelCmd */
              },             
          },
/*---------------------------------------------------------------------------*/
/*                              L E D s                                      */
/*---------------------------------------------------------------------------*/
/* 66 */  {/* MCU_LED_1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },           
/* 48 */   {/* MCU_LED_2 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,            /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 52 */   {/* MCU_LED_3 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_13,            /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 91 */   {/* MCU_LED_4 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
              { 
                GPIO_Pin_5,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 35 */   {/* MCU_LED_5 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock */
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
/*---------------------------------------------------------------------------*/
/*                         O P T O   O U T P U T S                           */
/*---------------------------------------------------------------------------*/
/* 38 */   {/* OPTO_OUT1 */
              1,                        /* pre_out_val */
              GPIO_AF_TIM1,             /* AltFunNum  */
              GPIO_PinSource12,          /* pin_src    */
              0,                        /* port_src   */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_12,             /* GPIO_Pin   */
                GPIO_Mode_AF,          /* GPIO_Mode  */
                GPIO_Speed_100MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 39 */   {/* OPTO_OUT2 */
              1,                        /* pre_out_val */
              GPIO_AF_TIM1,             /* AltFunNum  */
              GPIO_PinSource8,         /* pin_src    */
              0,                        /* port_src   */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_AF,          /* GPIO_Mode  */
                GPIO_Speed_100MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 40 */   {/* OPTO_OUT3 */
              1,                        /* pre_out_val */
              GPIO_AF_TIM1,             /* AltFunNum  */
              GPIO_PinSource14,          /* pin_src    */
              0,                       /* port_src   */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_14,             /* GPIO_Pin   */
                GPIO_Mode_AF,          /* GPIO_Mode  */
                GPIO_Speed_100MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 41 */   {/* OPTO_OUT4 */
              1,                        /* pre_out_val */
              GPIO_AF_TIM1,              /* AltFunNum  */
              GPIO_PinSource10,         /* pin_src    */
              0,                          /* port_src   */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,             /* GPIO_Pin   */
                GPIO_Mode_AF,          /* GPIO_Mode  */
                GPIO_Speed_100MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 42 */   {/* OPTO_OUT5 TEMP ALARM*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOE,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOE,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           }           
/******************************************************************************/           
    };/* PIN_CNFG_ST pins_cfg_data[] */
/******************************************************************************/

/******************************************************************************/
    SPI_CNFG_ST spi_cfng_data[] = {
/******************************************************************************/
      {
        SPI1,
        RCC_APB2PeriphClockCmd,
        RCC_APB2Periph_SPI1,
        {
          SPI_Direction_2Lines_FullDuplex,
          
          SPI_Mode_Master,           /*!<  uint16_t SPI_Mode;                 Specifies the SPI operating mode. This parameter can be a value of @ref SPI_mode */

          SPI_DataSize_8b,            /*!<  uint16_t SPI_DataSize;             Specifies the SPI data size. This parameter can be a value of @ref SPI_data_size */

          SPI_CPOL_High,              /*!<  uint16_t SPI_CPOL;                 Specifies the serial clock steady state. This parameter can be a value of @ref SPI_Clock_Polarity */

          SPI_CPHA_2Edge,             /*!<  uint16_t SPI_CPHA;                 Specifies the clock active edge for the bit capture. This parameter can be a value of @ref SPI_Clock_Phase */

          SPI_NSS_Soft,               /*!<  uint16_t SPI_NSS;                  Specifies whether the NSS signal is managed by hardware (NSS pin) or by software using the SSI bit. 
                                                  This parameter can be a value of @ref SPI_Slave_Select_management */
 
          SPI_BaudRatePrescaler_128,   /*!<  uint16_t SPI_BaudRatePrescaler;    Specifies the Baud Rate prescaler value which will be used to configure the transmit and receive SCK clock. 
                                                  This parameter can be a value of @ref SPI_BaudRate_Prescaler
                                                  @note The communication clock is derived from the master clock. The slave clock does not need to be set. */

          SPI_FirstBit_MSB,           /*!<  uint16_t SPI_FirstBit;             Specifies whether data transfers start from MSB or LSB bit. 
                                         This parameter can be a value of @ref SPI_MSB_LSB_transmission */
          7                          /*!<  uint16_t SPI_CRCPolynomial;        Specifies the polynomial used for the CRC calculation. */
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
          0,                               /* NVIC_IRQChannelPreemptionPriority */  
          0,                               /* NVIC_IRQChannelSubPriority    */         
          ENABLE,                          /* NVIC_IRQChannelCmd            */            
        }
      },          
      {
        USART6,                            /* *UsartName                    */
        RCC_APB2Periph_USART6,             /*  UsartClk                     */
        RCC_APB2PeriphClockCmd,            /*  ClkCmdFun                    */
        {                                  /*  USART_InitTypeDef CommParams */
          38400,                           /*  USART_BaudRate               */
          USART_WordLength_8b,             /*  USART_WordLength             */          
          USART_StopBits_1,                /*  USART_StopBits               */            
          USART_Parity_No,                 /*  USART_Parity                 */             
          USART_Mode_Rx | USART_Mode_Tx,   /*  USART_Mode                   */               
          USART_HardwareFlowControl_None   /*  USART_HardwareFlowControl    */         
        },
        {                                  /* NVIC_InitTypeDef NvicParams   */
          USART6_IRQn,                     /* NVIC_IRQChannel               */                    
          0,                               /* NVIC_IRQChannelPreemptionPriority */  
          0,                               /* NVIC_IRQChannelSubPriority    */         
          ENABLE,                          /* NVIC_IRQChannelCmd            */            
        }
      },      
      {
        USART1,                            /* *UsartName */
        RCC_APB2Periph_USART1,             /*  UsartClk                     */
        RCC_APB2PeriphClockCmd,            /*  ClkCmdFun                    */
        {                                  /*  USART_InitTypeDef CommParams */
          115200,                          /*  USART_BaudRate               */
          USART_WordLength_8b,             /*  USART_WordLength             */          
          USART_StopBits_1,                /*  USART_StopBits               */            
          USART_Parity_No,                 /*  USART_Parity                 */             
          USART_Mode_Rx | USART_Mode_Tx,   /*  USART_Mode                   */               
          USART_HardwareFlowControl_None   /*  USART_HardwareFlowControl    */         
        },
        {                                  /* NVIC_InitTypeDef NvicParams   */
          USART1_IRQn,                     /* NVIC_IRQChannel               */                    
          0,                               /* NVIC_IRQChannelPreemptionPriority */  
          0,                               /* NVIC_IRQChannelSubPriority    */         
          ENABLE,                          /* NVIC_IRQChannelCmd            */            
        }
      }

/****************************************************************************/
    };/* USART_CNFG_ST usarts_cfng_data[] */
/****************************************************************************/    
   
  #endif /* #ifdef BOARD_INIT */




#ifdef __cplusplus
}
#endif
#endif /* _BOARD_CONFIG_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/


#if 0      
/* 70 */   {/* USB_FS_DM */
              1,                        /* pre_out_val */
              GPIO_AF_OTG1_FS,          /* AltFunNum  */
              GPIO_PinSource11,         /* PinSource  */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_11,            /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}           
           },
/* 71 */   {/* USB_FS_DP */
              1,                        /* pre_out_val */
              GPIO_AF_OTG1_FS,          /* AltFunNum  */
              GPIO_PinSource12,         /* PinSource  */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
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
#endif
