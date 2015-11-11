/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : modbus_exp.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS\
*
*  PURPOSE     : All declaration needed for using MODBUS services.
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

#ifndef _MODBUS_EXP_H_
#define _MODBUS_EXP_H_

//#ifdef __cplusplus
// extern "C" {
//#endif

/* --- INCLUDES ------------------------------------------------------------ */
  #include "ll_typedef.h"   
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif

  #include   "mb_cnfg.h"
  #include   ".\Common\mb_infra.h"
  #include   ".\RTU\rtu_msgs.h"
  #include   ".\RTU\rtu_master.h"
  #include   ".\RTU\rtu_slave.h"
  #include   ".\MbTcp\tcp_mb.h"   
/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







//#ifdef __cplusplus
//}
//#endif
#endif /* _MODBUS_EXP_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
