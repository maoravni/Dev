/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : pins_roles.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\BSP  
*
*  PURPOSE     : STM32F207ZG pins roles assignment. 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM 
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

#ifndef _PINS_ROLES_H_
#define _PINS_ROLES_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */


/* --- EXPORTED CONSTANTS -------------------------------------------------- */
   #define ETH_LINK_EXTI_LINE             EXTI_Line12
   #define ETH_LINK_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
   #define ETH_LINK_EXTI_PIN_SOURCE       EXTI_PinSource12
   #define ETH_LINK_EXTI_IRQn             EXTI15_10_IRQn 
/* PA12 */
   #define ETH_LINK_PIN                   GPIO_Pin_12
   #define ETH_LINK_GPIO_PORT             GPIOA
   #define ETH_LINK_GPIO_CLK              RCC_AHB1Periph_GPIOA

/*****************************************************************************/   
   
  #define  LED_A_PORT  GPIOG
  #define  LED_A_PIN   GPIO_Pin_9  
                                                    
  #define  LED_B_PORT  GPIOG
  #define  LED_B_PIN   GPIO_Pin_10
                                                     
  #define  LED_C_PORT  GPIOG
  #define  LED_C_PIN   GPIO_Pin_11
                          
  #define  LED_D_PORT  GPIOG                        
  #define  LED_D_PIN   GPIO_Pin_12
   
/*****************************************************************************/
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
/*****************************************************************************/





#ifdef __cplusplus
}
#endif
#endif /* _PINS_ROLES_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
