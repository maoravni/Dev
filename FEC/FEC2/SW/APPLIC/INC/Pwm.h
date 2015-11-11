/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM
*
*  FILE        : PWM.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom/applic
*
*  PURPOSE     :   
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.08     Yossi Sarusi     1.1.0   First version
*
*******************************************************************************
      Copyright and Proprietary Statment.

******************************************************************************/

#ifndef _PWM_H_
#define _PWM_H_

/* Includes -----------------------------------------------------------------*/
#include "ll_typedef.h"

/* Exported types -----------------------------------------------------------*/
typedef enum{
	HX_FRONT,
	HX_MEDIUM,
	HX_REAR
} CMBR_HX_ID;

/* Exported constants -------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/


 void PWM_init(int TaskPriority, int StackSize);
 STATUS Pwm_CreateElement(int ElementId, U32 DelayPeriod, U8 PercentDutyCycle, U32 TotalPeriod);
 STATUS Pwm_Enable(int ElementId, BOOL Enable);
 BOOL IsPwm_Enable(int ElementId);
 STATUS Pwm_DualPower(int ElementId, BOOL Enable);
 STATUS Pwm_SetDutyCycle(int ElementId, U8 PercentDutyCycle);
 int GetChamberPwmId(int ChamberId,CMBR_HX_ID HX_id );


#ifdef __cplusplus
}
#endif
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
  
#endif /* _MONITOR_H_ */

/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/





