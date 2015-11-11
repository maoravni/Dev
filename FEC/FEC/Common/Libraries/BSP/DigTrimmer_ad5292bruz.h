/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : BSP_LIB 
*
*  FILE        : ad529bruz_20.h
*   
*  PATH        : C:\Landalabs\Spitfire\Branches_Embedded\RigDryerFirstPrintFEC-Branch\Common\Libraries\BSP 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR ARM Embedded Workbench 6.400 Evaluation
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.12     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _AD529BRUZ_20_H_
#define _AD529BRUZ_20_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif

/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */
#define TRIMMER1 0
#define TRIMMER2 1
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/
#define CS1_LOW()     GPIOD->BSRRH = GPIO_Pin_2
#define CS2_LOW()     GPIOD->BSRRH = GPIO_Pin_3

#define CS1_HIGH()    GPIOD->BSRRL = GPIO_Pin_2 
#define CS2_HIGH()    GPIOD->BSRRL = GPIO_Pin_3 

#define RDY1       (GPIOE->IDR & GPIO_Pin_7)
#define RDY2       (GPIOF->IDR & GPIO_Pin_2)
/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
   void Set_AO_mA(u8 trimmer_number,u16 current);
   u16 Get_AO_mA(u8 trimmer_number);
void DigitalTrimmersInit(void);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _AD529BRUZ_20_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
