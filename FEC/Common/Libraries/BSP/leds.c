/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : HMI_LIB  
*
*  FILE        : leds.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\HMI
*
*  PURPOSE     : LEDs control related thinks.
*
*  TOOLCHAIN:  : IAR ARM Embedded Workbench 6.303 
*
*  TARGET      : Phantom/Mosquito
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.07     Igor Natachanny   1.1.0   Doxygen-style function headers added.
*                                        'alive_led_task' implemented.
*******************************************************************************
      Copyright and Proprietary Statment.

*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
//  #define LEDS_CTRL_IMPL_HERE
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include <stdio.h>
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif

  //#include "free_rtos_exp.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
//  #include "..\BSP\bsp_srvcs.h"
  #include  "leds_cnfg.h"

  #include "leds.h"
//  #include "vcp_term.h"

/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */                          
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
/* --- PRIVATE_DATA --------------------------------------------------------- */





/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/


/**
  * @brief  Turns selected 'led' On/ff. 
  * @param  'led' going turn On.
    @param  'State' 1-On, 0-Off.
  * @retval None
  */
  void Turn_led_(LED_ID_ET led,BOOL State){

    if (State)
      Led_PortOf[led]->BSRRH = Led_PinOf[led];
    else
      Led_PortOf[led]->BSRRL = Led_PinOf[led];
    
  }/* led_light */


/**
  * @brief  Turns selected 'led' On. 
  * @param  'led' going turn On.
  * @retval None
  */
  void led_light(LED_ID_ET led){

    Led_PortOf[led]->BSRRH = Led_PinOf[led];
    
  }/* led_light */

/**
  * @brief  Turns selected 'led' Off. 
  * @param  'led' going turn Off.
  * @retval None
  */
  void led_dark(LED_ID_ET led){

    Led_PortOf[led]->BSRRL = Led_PinOf[led];
    
  }/* led_dark */


/**
  * @brief  Toggles the selected 'led'. 
  * @param  'led' going toggle.
  * @retval None
  */
  void led_toggle(LED_ID_ET led){
    
  Led_PortOf[led]->ODR ^= Led_PinOf[led];
  
  }/* led_toggle */


/**
  * @brief  Toggles the Alive led'. 
  * @param  none
  * @retval None
  */
  void LedAliveToggle()
  {
    led_toggle(ALIVE_LED);
  }

/*----------------------------------------------------------------------------
  * @brief  Toggle Led4 task
  * @param  pvParameters not used
  * @retval None
 *----------------------------------------------------------------------------*/

//  void alive_led_task(void * pvParameters){
//
//    while(1){
//      led_toggle(ALIVE_LED);
//      vTaskDelay(LED_ALIVE_TO);
//    }/* while(1) */
//
//  }/* alive_led_task */

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
#if 0    
   if( flars & USB_MCG_READY)
   {
     flars &= ~USB_MCG_READY;
     SendResponse (msg_buff, 10);
   }
#endif
