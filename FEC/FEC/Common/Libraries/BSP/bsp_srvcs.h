/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : BSP_LIB 
*
*  FILE        : bsp_srvcs.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP
*
*  PURPOSE     : Board Support Package Services.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : MOSQUITO 
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
#ifndef _BSP_SERVICES_H_
#define _BSP_SERVICES_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/

/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/


/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
//  #include "pwm.h"
#ifdef FEC_BOARD
  #include "a2d.h"
#endif   
  #include "opto_out.h"

  #include "eeprom_cnfg.h"
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
  void hold_gp_out_(GPIO_TypeDef* GPIO_Port, uint32_t GPIO_Pin, uint32_t out_val, uint32_t usec);
  void prvSetupHardware();
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _BSP_SERVICES_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/
