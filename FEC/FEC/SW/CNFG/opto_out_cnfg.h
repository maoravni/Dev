/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :BSP_LIB  
*
*  FILE        : opto_out_cnfg.h
*   
*  PATH        :  
*
*  PURPOSE     : Optically Isolated Outputs HAL.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.01     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
*
******************************************************************************/

#ifndef _OPTO_OUT_CNFG_H_
#define _OPTO_OUT_CNFG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */
#include "ll_typedef.h"
/* --- EXPORTED TYPES ------------------------------------------------------ */
  typedef enum _opto_out_id_et_{
    OPTO_OUT1 = 0,
    OPTO_OUT2,
    OPTO_OUT3,
    OPTO_OUT4,
    OPTO_OUT5,
    OPTO_OUT6,
    OPTO_OUT7,
    OPTO_OUT8,
    OPTO_OUT9,
    OPTO_OUT10,
    OPTO_OUT11,
    OPTO_OUT12,
  
    NUMBER_OF_OPTO_OUTS /* Keep this last */    
  } OPTO_OUT_ID_ET;
  
   typedef enum _opto_out_ctrl_val_{
    DEACTIVE = 0,
    ACTIVE,
    TOGGLE,
    NUMBER_OF_VALUES /* Keep this last */    
  } OPTO_OUT_CTRL_VAL_ET; 
/* --- PRIVATE_DATA --------------------------------------------------------- */

 
  
#ifdef  OPTO_OUTS_CTRL_IMPL_HERE
  
/*                                      OPTO_OUT1   OPTO_OUT2   OPTO_OUT3    OPTO_OUT4    OPTO_OUT5    OPTO_OUT6    OPTO_OUT7   OPTO_OUT8   OPTO_OUT9   OPTO_OUT10  OPTO_OUT11  OPTO_OUT12  SAFETY      */
  
  static GPIO_TypeDef*   DO_PortOf[] = {GPIOE,      GPIOE,      GPIOE,       GPIOE,       GPIOE,       GPIOE,       GPIOE,      GPIOE,      GPIOC,      GPIOC,      GPIOC,      GPIOC}; //, GPIOD       };
                         
  static const uint16_t   DO_PinOf[] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9};//, GPIO_Pin_12 };
  
  static GPIO_TypeDef* SAFETY_PortOf =GPIOD;
  static const uint16_t SAFETY_PinOf=GPIO_Pin_12 ;
  
  //static const uint16_t DO_InvrsOf[] = {false,      false,      false,       false,        false,       false,      false,       false,       false,       false,       false,       false,       false       };  
#endif
/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _OPTO_OUT_CNFG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
