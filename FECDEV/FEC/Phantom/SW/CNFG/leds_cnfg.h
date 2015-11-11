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

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */

  typedef enum {
    MBS_RX_LED,
    MBS_TX_LED,
    LABL_TCP,
    ALIVE_LED,
    NUMBER_OF_LEDS /* Keep this last */
  }LED_ID_ET;
  
/* --- PRIVATE_DATA --------------------------------------------------------- */
 
#ifdef  LEDS_CTRL_IMPL_HERE
  
  /*                              MBS_TX_LED   MBS_TX_LED   LED_C        ALIVE_LED  */
  static GPIO_TypeDef* PortOf[] = {GPIOG,      GPIOG,       GPIOG,       GPIOG };
                         
  static const uint16_t PinOf[] = {GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12};
  
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
