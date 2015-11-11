/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : board_setup.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\inc 
*
*  PURPOSE     : This file contains the non-run-time-changebale setup 
*                data for all peripherials of STM32F207SG according to                 
*                PHANTOM board requerements.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM_BOARD 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.01     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _BOARD_CONFIG_
#define _BOARD_CONFIG_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
#include "pins_roles.h"   
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
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              /*ETH_LINK_GPIO_PORT*/GPIOA,       /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */           
              /*ETH_LINK_GPIO_CLK*/RCC_AHB1Periph_GPIOA,        /* GPIO_Clock */
              { 
                /*ETH_LINK_PIN*/GPIO_Pin_12,            /* GPIO_Pin   */
                GPIO_Mode_IN,           /* GPIO_Mode  */
                GPIO_Speed_100MHz,      /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
            },
            
/* 129 */   {/* ETH_RMII_TXD1 (PG14) */
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource14,         /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource13,         /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource11,         /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource5,          /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource4,          /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource7,          /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource1,          /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource8,          /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource1,          /* PinSource  */
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
              GPIO_AF_ETH,              /* AltFunNum  */
              GPIO_PinSource2,          /* PinSource  */
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
//#endif
/*************************   MODBUS_2   (USART1)    ***************************/
/* 101 */   {/* USART3_TX */
              GPIO_AF_USART1,           /* AltFunNum  */
              GPIO_PinSource9,          /* PinSource  */
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
/* 102 */   {/* USART3_RX */
              GPIO_AF_USART1,           /* AltFunNum  */
              GPIO_PinSource10,         /* PinSource  */
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
/*************************   MODBUS_1   (USART3)    *************************/
/* 111 */   {/* USART3_TX */
              GPIO_AF_USART3,           /* AltFunNum  */
              GPIO_PinSource10,          /* PinSource  */
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
/* 112 */   {/* USART3_RX */
              GPIO_AF_USART3,           /* AltFunNum  */
              GPIO_PinSource11,         /* PinSource  */
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
/*************************   TERMINAL_COM  (USART6)  *************************/
/* 96 */   {/* USART6_TX */
              GPIO_AF_USART6,           /* AltFunNum  */
              GPIO_PinSource6,          /* PinSource  */
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
/* 97 */   {/* USART6_RX */
              GPIO_AF_USART6,           /* AltFunNum  */
              GPIO_PinSource7,          /* PinSource  */
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
/****************************************************************************/
/*  34 */  {/* RE */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              /*RE_PORT*/GPIOA,                  /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,/*RE_CLK_CMD,*/               /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOA,/*RE_CLK,*/                   /* GPIO_Clock */
              { 
                GPIO_Pin_0,                 /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*  132 */  {/* DE */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              DE_PORT,                  /* GPIO_Port  */
              DE_CLK_CMD,               /* ClkCmdFun  */
              DE_CLK,                   /* GPIO_Clock */
              { 
                DE_PIN,                 /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },           
/****************************************************************************/
/*  98 */  {/* RE2 */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              RE2_PORT,                 /* GPIO_Port  */
              RE2_CLK_CMD,              /* ClkCmdFun  */
              RE2_CLK,                  /* GPIO_Clock */
              { 
                RE2_PIN,                /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/*  99 */  {/* DE2 */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              DE2_PORT,                 /* GPIO_Port  */
              DE2_CLK_CMD,              /* ClkCmdFun  */
              DE2_CLK,                  /* GPIO_Clock */
              { 
                DE2_PIN,                /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_NOPULL        /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },           
/****************************************************************************/
/* 124 */  {/* LED_A */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              LED_A_PORT,               /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                LED_A_PIN,              /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 125 */  {/* LED_B */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              LED_B_PORT,               /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                LED_B_PIN,              /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 126 */  {/* LED_C */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              LED_C_PORT,               /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                LED_C_PIN,              /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/* 127 */  {/* LED_D */
              0xFF,                     /* AltFunNum  */
              0xFF,                     /* PinSource  */
              LED_D_PORT,               /* GPIO_Port  */
              RCC_AHB1PeriphClockCmd,   /* ClkCmdFun  */
              RCC_AHB1Periph_GPIOG,     /* GPIO_Clock */
              { 
                LED_D_PIN,              /* GPIO_Pin   */
                GPIO_Mode_OUT,          /* GPIO_Mode  */
                GPIO_Speed_50MHz,       /* GPIO_Speed */
                GPIO_OType_PP,          /* GPIO_OType */
                GPIO_PuPd_UP            /* GPIO_PuPd  */
              },
              {0,EXTI_Mode_Interrupt,EXTI_Trigger_Rising,DISABLE},
              {0,0,0,DISABLE}
           },
/****************************************************************************/           
    };/* PIN_CNFG_TS pins_cfg_data[] */
/****************************************************************************/
    
    USART_CNFG_ST usarts_cfng_data[] = {
      {
        USART1,                  /* *UsartName */
//        MB_2,                     /*  UsartRole; */
        RCC_APB2Periph_USART1,   /*  UsartClk  */
        RCC_APB2PeriphClockCmd,  /*  ClkCmdFun */
        {                        /*  USART_InitTypeDef CommParams */
          38400,                          /*  USART_BaudRate   */
          USART_WordLength_8b,           /*  USART_WordLength */          
          USART_StopBits_1,              /*  USART_StopBits   */            
          USART_Parity_No,               /*  USART_Parity     */             
          USART_Mode_Rx | USART_Mode_Tx, /*  USART_Mode       */               
          USART_HardwareFlowControl_None /*USART_HardwareFlowControl */         
        },
        {                        /* NVIC_InitTypeDef NvicParams */
          USART1_IRQn,                   /* NVIC_IRQChannel */                    
          0,                             /* NVIC_IRQChannelPreemptionPriority */  
          0,                             /* NVIC_IRQChannelSubPriority */         
          ENABLE,                        /* NVIC_IRQChannelCmd */            
        }
      }
      ,      
      {
        USART3,                  /* *UsartName */
        RCC_APB1Periph_USART3,   /*  UsartClk  */
        RCC_APB1PeriphClockCmd,  /*  ClkCmdFun */
        {                        /*  USART_InitTypeDef CommParams */
          38400,                          /*  USART_BaudRate   */
          USART_WordLength_8b,           /*  USART_WordLength */          
          USART_StopBits_1,              /*  USART_StopBits   */            
          USART_Parity_No,               /*  USART_Parity     */             
          USART_Mode_Rx | USART_Mode_Tx, /*  USART_Mode       */               
          USART_HardwareFlowControl_None /*USART_HardwareFlowControl */         
        },
        {                        /* NVIC_InitTypeDef NvicParams */
          USART3_IRQn,                   /* NVIC_IRQChannel */                    
          0,                             /* NVIC_IRQChannelPreemptionPriority */  
          0,                             /* NVIC_IRQChannelSubPriority */         
          ENABLE,                        /* NVIC_IRQChannelCmd */            
        }
      }
      ,
      {
        USART6,                  /* *UsartName */
        RCC_APB2Periph_USART6,   /*  UsartClk  */
        RCC_APB2PeriphClockCmd,  /*  ClkCmdFun */
        {                        /*  USART_InitTypeDef CommParams */
          115200,                        /*  USART_BaudRate   */
          USART_WordLength_8b,           /*  USART_WordLength */          
          USART_StopBits_1,              /*  USART_StopBits   */            
          USART_Parity_No,               /*  USART_Parity     */             
          USART_Mode_Rx | USART_Mode_Tx, /*  USART_Mode       */               
          USART_HardwareFlowControl_None /*USART_HardwareFlowControl */         
        },
        {                        /* NVIC_InitTypeDef NvicParams */
          USART6_IRQn,                   /* NVIC_IRQChannel */                    
          0,                             /* NVIC_IRQChannelPreemptionPriority */  
          0,                             /* NVIC_IRQChannelSubPriority */         
          ENABLE,                        /* NVIC_IRQChannelCmd */            
        }
        
      }
/****************************************************************************/           
    };/* USART_CNFG_TS usarts_cfng_data[] */
/****************************************************************************/    
  #endif /* #ifdef BOARD_INIT */




#ifdef __cplusplus
}
#endif
#endif /* _BOARD_CONFIG_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/
