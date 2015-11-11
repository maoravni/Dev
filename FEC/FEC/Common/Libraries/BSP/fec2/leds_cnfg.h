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
#include <stm32f4xx.h>
/* --- EXPORTED TYPES ------------------------------------------------------ */

  
/* --- PRIVATE_DATA --------------------------------------------------------- */
 
  
  static GPIO_TypeDef* Led_PortOf[] = {GPIOG,      GPIOG,      GPIOG,      GPIOG,      GPIOG,      GPIOF,       };
                         
  static const uint16_t Led_PinOf[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_11, };
  
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
