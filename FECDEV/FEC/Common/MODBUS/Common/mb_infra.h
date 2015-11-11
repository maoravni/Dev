/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : MB_LIB 
*
*  FILE        : mb_infra.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS\Common\inc  
*
*  PURPOSE     : Modbus node infrastructure related definition.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.08     Igor Natachanny   1.0.1   Bug fixed: Incorrect parameter value in
*                                        crc16_calc function - handling added.
*******************************************************************************
      Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _MODBUS_H_
#define _MODBUS_H_

//#include   "rtu_msg.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES --------------------------------------------------------------*/
  #include "mb_cnfg.h"
/* --- EXPORTED TYPES ------------------------------------------------------- */

/*----------------------------------------------------------------------------*/   
  typedef enum _modbus_work_mode_ {
    TX = 0,
    RX,
    NUM_OF_MB_NODE_WRK_MODES
  }MB_NODE_WRK__MODE_ET;
  
/*----------------------------------------------------------------------------*/
    
/* Exported constants -------------------------------------------------------*/


/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
    #define SWAP_16(value16) (((uint16_t)value16<<8) | ((uint16_t)value16>>8))  
    #define word(HI,LW) ((uint16_t) (((uint16_t)HI<<8)| LW)) 
/* Exported functions -------------------------------------------------------*/
  uint16_t crc16_calc(uint8_t *ptr, int32_t len);
  uint32_t mb_set_dir(MB_NODE_NAME_ET mb_node_name, uint32_t wrk_mode);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/
  






#ifdef __cplusplus
}
#endif
#endif /* _MODBUS_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/
