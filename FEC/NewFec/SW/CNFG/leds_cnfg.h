/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : leds_cnfg.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\CNFG 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : Phantom at it's board. 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.07     Igor Natachanny   1.0.1   New LEDs names defined.
*                           Alive-LED blinking timeout in mSec definition added.
*******************************************************************************
      Copyright and Proprietary Statment.
******************************************************************************/

#ifndef _LEDS_CNFG_H_
#define _LEDS_CNFG_H_

//#ifdef __cplusplus
// extern "C" {
//#endif

/* --- INCLUDES ------------------------------------------------------------ */
#include <stm32f2xx.h>
/* --- EXPORTED TYPES ------------------------------------------------------ */

  
/* --- PRIVATE_DATA --------------------------------------------------------- */
 
//#ifdef  LEDS_CTRL_IMPL_HERE
//typedef enum
//{
//    TOP_LED1,
//    TOP_LED2,
//    TOP_LED3,
//    TOP_LED4,
//    LABL_RESET,
//    MBS_TX_LED,
//    MBS_RX_LED,
//    LABL_TCP,
//    LABL_UDP,
//    CAN_TX_LED,
//    CAN_RX_LED,
//    CPU_ACTIVITY_LED8,
//    LABL_SAFETY_LED9,  //SAFETY
//    DIP_LED,
//    ALIVE_LED,
//    NUMBER_OF_LEDS /* Keep this last */
//} LED_ID_ET;
  
/*                               MBS_TX_LED   MBS_RX_LED  CAN_TX_LED  CAN_RX_LED  LABL_LED1  LABL_LED2    LABL_LED3    LABL_LED4    LABL_LED5    LABL_LED6    LABL_LED7    LABL_LED8    LABL_LED9    DIP_LED   ALIVE_LED */
  
/*
  static GPIO_TypeDef* Led_PortOf[] = {GPIOG,      GPIOG,      GPIOG,      GPIOF,      GPIOD,      GPIOD,       GPIOE,       GPIOG,       GPIOG,       GPIOF,       GPIOF,       GPIOF,       GPIOF,       GPIOD,      GPIOC      };
                         
  static const uint16_t Led_PinOf[] = {GPIO_Pin_4, GPIO_Pin_3, GPIO_Pin_2, GPIO_Pin_11, GPIO_Pin_11, GPIO_Pin_10, GPIO_Pin_15, GPIO_Pin_1, GPIO_Pin_0, GPIO_Pin_15, GPIO_Pin_14, GPIO_Pin_13, GPIO_Pin_12, GPIO_Pin_7, GPIO_Pin_13};
*/

  static GPIO_TypeDef* Led_PortOf[] = {GPIOD,      GPIOD,       GPIOG,      GPIOG,      GPIOF,       GPIOF,       };

  static const uint16_t Led_PinOf[] = {GPIO_Pin_11, GPIO_Pin_10, GPIO_Pin_1, GPIO_Pin_0, GPIO_Pin_15, GPIO_Pin_14, };

//#endif
  
/* --- EXPORTED CONSTANTS -------------------------------------------------- */
  
    
  #define LED_ALIVE_TO  250            /* Alive-LED blinking timeout in mSec */

//#ifdef __cplusplus
//}
//#endif
#endif /* _LEDS_CNFG_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
