/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : FEC  
*
*  FILE        : dg_input_cnfg.h
*   
*  PATH        :  
*
*  PURPOSE     : Digital Inputs HAL.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.11     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
*
******************************************************************************/

#ifndef _DIG_IN_CNFG_H_
#define _DIG_IN_CNFG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */
#include "ll_typedef.h"
/* --- EXPORTED TYPES ------------------------------------------------------ */
  typedef enum _dg_in_id_et_{
    BOARD_ID_0 = 0,
    BOARD_ID_1,
    BOARD_ID_2,
    NODE_ID_0,
    NODE_ID_1,
    NODE_ID_2,
    DIG_IN_1,
    DIG_IN_2,
    DIG_IN_3,
    DIG_IN_4,
    DIG_IN_5,
    DIG_IN_6,
    DIG_IN_7,
    DIG_IN_8,
    DIG_IN_9,
    DIG_IN_10,
    DIG_IN_11,
    DIG_IN_12,
    SSID_1,
    SSID_2,
    SSID_3,
    SSID_4,
    NUMBER_OF_DG_INs /* Keep this last */    
  } DIG_IN_ID_ET;
 
/* --- PRIVATE_DATA --------------------------------------------------------- */
 
#ifdef  DG_IN_CTRL_IMPL_HERE
  
/*                                   BOARD_ID_0   BOARD_ID_1   BOARD_ID_2    NODE_ID_0    NODE_ID_1    NODE_ID_2    DIG_IN_1    DIG_IN_2     DIG_IN_3     DIG_IN_4     DIG_IN_5     DIG_IN_6     DIG_IN_7     DIG_IN_8    DIG_IN_9    DIG_IN_10   DIG_IN_11   DIG_IN_12   SSID_1     SSID_2      SSID_3      SSID_4*/
  
  static GPIO_TypeDef*   PortOf[] = {GPIOG,       GPIOF,       GPIOG,        GPIOG,       GPIOG,       GPIOG,       GPIOB,      GPIOB,       GPIOD,       GPIOE,       GPIOB,       GPIOB,       GPIOD,       GPIOE,      GPIOE,      GPIOE,      GPIOE,      GPIOE,      GPIOG,      GPIOG,      GPIOF,      GPIOD};
                         
  static const uint16_t   PinOf[] = {GPIO_Pin_13, GPIO_Pin_1,  GPIO_Pin_15,  GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_14, GPIO_Pin_0, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_0, GPIO_Pin_15};
  
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
#endif /* _DIG_IN_CNFG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
