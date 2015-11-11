/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : FEC  
*
*  FILE        : eeprom_cnfg.h
*   
*  PATH        :  
*
*  PURPOSE     : Type of EEPROM chip select. 
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

#ifndef _EEPROM_CNFG_H_
#define _EEPROM_CNFG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */
#include "ll_typedef.h"

//  #define  _MC24xx1025_
  #define  _M24M02DR_
   
  #ifdef _MC24xx1025_   
    #include "mc24xx1025r.h"
  #endif
   
  #ifdef _M24M02DR_   
    #include "eeprom_m24m02_dr.h"
  #endif
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _EEPROM_CNFG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
