/******************************************************************************
*                     (C) Igor Natachanny 2010                                *
*******************************************************************************
*
*  PROJECT     : RON3K 
*
*  FILE        : tim.h
*   
*  PATH        : C:\Igor\MyProjects\Eilon\RON3K\SW\WRK\BSP\ 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : RealView MDK-ARM Version 4.0
*
*  TARGET      : RON2K5 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2011.03     Igor Natachanny   1.0.0   Initial version.
*
*******************************************************************************
      Copyright and Proprietary Statment.
        
	This source code may be used by my legal customers only. It may not be used 
   copied or transferred in whatever form in whole or in part, to a third 
   party without a written consent. Derivative programs created by my customers 
   using this software in object code form only are not restricted in any way.  

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _TIM_TM_H_
#define _TIM_TM_H_

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
  #define   Sec     1
  #define   mSec    1000
  #define   uSec    1000000   
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/

  int32_t get_tim_status(TIM_TypeDef *TIMx);
  int32_t tim_start(TIM_TypeDef *TIMx, uint32_t time, uint32_t units);
  int32_t tim_init(TIM_TypeDef *TIMx);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/








#endif /* _SLEEP_TM_H_ */
/*============================================================================*/
/*                     (C) Copyright Igor Natachanny 2010                     */
/*=========================================================== end_of_file ====*/
