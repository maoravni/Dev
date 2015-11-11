/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MOSQUITO 
*
*  FILE        : leds_cnfg.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Mosquito\SW\CNFG 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : Mosquito at it's board. 
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

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */

  typedef enum {
    LED_MBS_TX,  /* MCU_LED_1, */
    LED_MBS_RX,  /* MCU_LED_2, */
    LED_UART_RX, /* MCU_LED_3, */
    LED_UART_TX, /* MCU_LED_4, */
    LED_ALIVE,   /* MCU_LED_5, */
    MCU_TP_1,  
    NUMBER_OF_LEDS /* Keep this last    */
  }LED_ID_ET;
  
/* --- PRIVATE_DATA --------------------------------------------------------- */
  
#ifdef  LEDS_CTRL_IMPL_HERE
  
  /*                               MCU_LED_1   MCU_LED_2    MCU_LED_3    MCU_LED_4    MCU_LED_5   MCU_TP_1    */
  static GPIO_TypeDef* PortOf[] = {GPIOC,      GPIOB,       GPIOB,       GPIOB,       GPIOB,      GPIOC      };
                         
  static const uint16_t PinOf[] = {GPIO_Pin_9, GPIO_Pin_11, GPIO_Pin_13, GPIO_Pin_5,  GPIO_Pin_0, GPIO_Pin_12 };
  
#endif
  
/* --- EXPORTED CONSTANTS -------------------------------------------------- */
  #define LED_ALIVE_TO  250            /* Alive-LED blinking timeout in mSec */

#ifdef __cplusplus
}
#endif
#endif /* _LEDS_CNFG_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
