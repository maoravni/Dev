/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : FEC_BOOT 
*
*  FILE        : crc_ccitt.h
*   
*  PATH        : C:\Landalabs\Spitfire\Branches_Embedded\FirstPrintFEC\FEC_BOOT\SW\APPLIC\inc 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.40.1.3812
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2013.03     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
*
******************************************************************************/

#ifndef _FILE_NAME_H_
#define _FILE_NAME_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */
#include "stm32f2xx.h"
/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  uint16_t crc_ccitt(unsigned short sum, uint8_t *p, uint32_t len);
//uint16_t crc_ccitt(uint8_t *data, uint32_t length, uint16_t seed, uint16_t final);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _FILE_NAME_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
