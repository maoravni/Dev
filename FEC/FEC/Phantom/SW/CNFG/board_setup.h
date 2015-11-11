/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : board_setup.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\CNFG
*
*  PURPOSE     : This file contains the non-run-time-changebale setup 
*                data for all peripherials of STM32F207SG according to                 
*                PHANTOM board requerements.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.07     Igor Natachanny   1.1.0   USART3 and it's GPIOs config. data added.
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
  void stm32f2_usart_cnfg();

#undef USE_SPI

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
   
/*===========================================================================*/
/*         ***** P I N S    C O N F I G U R A T I O N   D A T A   *****      */
/*===========================================================================*/
  #ifdef BOARD_CNFG
   
  // TODO: Move these arrays to a .c file.
  // TODO: Make sure these arrays are stored in the flash memory.
    PIN_CNFG_ST pins_cfng_data[] = {
#if 0      
 /*---------------------------------------------------------------------------*/
/*                              I2C   P I N S                              */
/*---------------------------------------------------------------------------*/
/* 139 */  {/* I2C1 SCL */
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
/* 140 */  {/* I2C1 SDA */
              1,                        /* pre_out_val */
              GPIO_AF_I2C1,             /* AltFunNum  */
              0,                        /* PinSource  */
              0,                        /* port_src   */
              GPIOB,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOB,     /* GPIO_Clock RCC_APB1Periph_I2C1*/
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
#endif           
/*---------------------------------------------------------------------------*/
/*                         T E S T   P O I N T S                             */
/*---------------------------------------------------------------------------*/
/* GP3? */  {/* MCU_TP_1 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_3,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP          /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
      /*************************     E T H E R N E T      ***************************/
     /*                                    Eval.    Phantom
        ETH_MDIO -------------------------> PA2     PA2  1  
        ETH_MDC --------------------------> PC1     PC1  2
        ETH_PPS_OUT ----------------------> PB5     N/A  
        ETH_MII_CRS ----------------------> PH2     N/A
        ETH_MII_COL ----------------------> PH3_____PA3 ?????
        ETH_MII_RX_ER --------------------> PI10    N/A
        ETH_MII_RXD2 ---------------------> PH6     N/A
        ETH_MII_RXD3 ---------------------> PH7     N/A
        ETH_MII_TX_CLK -------------------> PC3     N/A
        ETH_MII_TXD2 ---------------------> PC2     N/A
        ETH_MII_TXD3 ---------------------> PB8     N/A
        ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1     PA1   3
        ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7     PA7   4
        ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4     PC4   5
        ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5     PC5   6
        ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PG11____PB11  7
        ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13    PG13  8
        ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14    PG14  9
        ETH_PWR_DOWN/ETH_INT -------------> PB14____PA12  10
                                                  */
//#if 0 
 
/* 104 */   {/* ETH_PWR_DOWN/ETH_INT */
              1,                        /* pre_out_val */  
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              0,                        /* port_src   */
              GPIOA,       /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,        /* GPIO_Clock */
              { 
                GPIO_Pin_12,            /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
            
/* 129 */   {/* ETH_RMII_TXD1 (PG14) */
              1,                        /* pre_out_val */  
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource14,         /* PinSource  */
              0,                        /* port_src   */              
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_14,            /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
           
/* 128 */   {/* ETH_RMII_TXD0 (PG13) */
              1,                        /* pre_out_val */  
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource13,         /* PinSource  */
              0,                        /* port_src   */              
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
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
      
/* 70 */   {/* ETH_RMII_TX_EN (PB11) */
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
           
/* 45 */   {/* ETH_RMII_RXD1 (PC5) */
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
            
/* 44 */   {/* ETH_RMII_RXD0 (PC4) */
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
      
/* 43 */    {/* ETH_RMII_CRS_DV (PA7) */
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
            
/* 35 */   {/* ETH_RMII_REF_CLK (PA1) */
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
            
/* 100 */   {/* ETH_MCO1 (PA8) */
              1,                        /* pre_out_val */
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource8,          /* PinSource  */
              0,                        /* port_src   */              
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
              { 
                GPIO_Pin_8,             /* GPIO_Pin   */
                GPIO_Mode_AF,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
           
/* 27 */   {/* ETH_MDC (PC1) */
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
           
/* 36 */   {/* ETH_MDIO (PA2)*/
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
#if 0
        #define  RE_PORT      GPIOA
  #define  RE_PIN       GPIO_Pin_0
  #define  RE_CLK_CMD   RCC_AHB1PeriphClockCmd
  #define  RE_CLK       RCC_AHB1Periph_GPIOA
                                                    
  #define  DE_PORT       GPIOG
  #define  DE_PIN        GPIO_Pin_15
  #define  DE_CLK_CMD    RCC_AHB1PeriphClockCmd
  #define  DE_CLK        RCC_AHB1Periph_GPIOG
   
  #define  RE2_PORT      GPIOC
  #define  RE2_PIN       GPIO_Pin_8  
  #define  RE2_CLK_CMD   RCC_AHB1PeriphClockCmd
  #define  RE2_CLK       RCC_AHB1Periph_GPIOC
   
  #define  DE2_PORT      GPIOC
  #define  DE2_PIN       GPIO_Pin_9
  #define  DE2_CLK_CMD   RCC_AHB1PeriphClockCmd
  #define  DE2_CLK       RCC_AHB1Periph_GPIOC
#endif      
/*---------------------------------------------------------------------------*/
/*            M O D B U S   D A T A   D I R E C T I O N   P I N S            */
/*               M O D B U S   D A T A   D I R E C T I O N   P I N S               */
/*---------------------------------------------------------------------------*/
/*-------------------------   MOD_BUS_1    DE       -------------------------*/
/* 132 */ { /* DE PG15 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_15,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
          },
/*-------------------------   MOD_BUS_1    RE       -------------------------*/
/* 34 */  { /* RE PA0 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOA,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOA,     /* GPIO_Clock */
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
/*-------------------------   MOD_BUS_2    RE_2    ---------------------------*/
/* 98 */  { /* RE2 PC8 */
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
/* 99 */  { /* DE2 PC9 */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOC,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOC,     /* GPIO_Clock */
              { 
                GPIO_Pin_9,             /* GPIO_Pin   */
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
/*------------------------    RS_485_2   (USART3)    ------------------------*/
/* 78 */   {/* USART3_TX */
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
/* 79 */   {/* USART3_RX */
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
/*------------------------    AUX_TERM   (USART6)    ------------------------*/
/* 63 */   {/* USART6_TX */
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
/* 64 */    {/* USART6_RX */
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
/*------------------------ RS_485_1     (USART1)    -------------------------*/
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
/*                              L E D s                                      */
/*---------------------------------------------------------------------------*/
/* 124 */  {/* LED_A */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src  */
              0,                        /* port_src */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
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
/* 125 */   {/* LED_B */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                GPIO_Pin_10,            /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_OD,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },
/* 126 */   {/* LED_C */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
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
/* 127 */   {/* LED_D */
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOG,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
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
              
/* SSRs OUTPUTS*/
          {/* PID_SSR1 -PD#4*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOD,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOD,     /* GPIO_Clock */
              { 
                GPIO_Pin_4,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },  
              {/* PID_SSR2 -PD#5*/
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
              {/* PID_SSR3 -PD#6*/
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
            {/* PID_SSR4 -PD#7*/
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
            {/* PID_SSR5 -PD#8*/
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
            {/* PID_SSR6-PD#9*/
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
            {/* PID_SSR7 -PD#10*/
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
            {/* PID_SSR8 -PD#11*/
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
            {/* PID_SSR9 -PD#12*/
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
            {/* PID_SSR10 -PD#13*/
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
            {/* PID_SSR11 -PD#14*/
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
            {/* PID_SSR12 -PD#15*/
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
           
  {/* IBD1_FAN -PF#4*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
              { 
                GPIO_Pin_4,             /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE},
           },             
{/* IBD2_FAN -PF#5*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
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
{/* IBD3_FAN -PF#6*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
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
{/* IBD4_FAN -PF#7*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
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
{/* IBD5_FAN -PF#8*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
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
{/* IBD6_FAN -PF#9*/
              1,                        /* pre_out_val */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* pin_src    */
              0,                        /* port_src   */
              GPIOF,                    /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOF,     /* GPIO_Clock */
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
          9600,                           /*  USART_BaudRate               */
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
      {
        USART1,                            /* *UsartName- Modbus master    */
        RCC_APB2Periph_USART1,             /*  UsartClk                     */
        RCC_APB2PeriphClockCmd,            /*  ClkCmdFun                    */
        {                                  /*  USART_InitTypeDef CommParams */
          38400,                            /*  USART_BaudRate               */
          USART_WordLength_8b,             /*  USART_WordLength             */          
          USART_StopBits_1,                /*  USART_StopBits               */            
          USART_Parity_No,                 /*  USART_Parity                 */             
          USART_Mode_Rx | USART_Mode_Tx,   /*  USART_Mode                   */               
          USART_HardwareFlowControl_None   /*  USART_HardwareFlowControl    */         
        },
        {                                  /* NVIC_InitTypeDef NvicParams   */
          USART1_IRQn,                     /* NVIC_IRQChannel               */                    
          (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY>>4)+2,  /* NVIC_IRQChannelPreemptionPriority */  //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY
          0,                               /* NVIC_IRQChannelSubPriority    */         
          ENABLE,                          /* NVIC_IRQChannelCmd            */            
        }
      }
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
