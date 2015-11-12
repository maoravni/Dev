/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : BSP_LIB 
*
*  FILE        : iwdg.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP 
*
*  PURPOSE     : 
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

#ifndef _IWDG_H_
#define _IWDG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */
#include  "iwdg_cnfg.h"
/* --- EXPORTED TYPES ------------------------------------------------------ */


/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  uint32_t is_force_iwdg_requested();
  uint32_t is_all_task_alive();
  void iwdg_force();
  void reset_task_alive_bits();
  void set_task_alive_bit(TASK_ALIVE_FLAGS_ET bit_pos);
  void iwdg_init();
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _IWDG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
