/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MODBUS_LIB
*
*  FILE        : tcp_mb.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS\MbTcp 
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
*  2012.08     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.

******************************************************************************/

#ifndef _TCP_MB_H_
#define _TCP_MB_H_

#include <ll_typedef.h>

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */
  typedef struct mbap_hdr_ {
    U16   transaction_id;
    U16   protocol_id;
    U16   len;
    U8    unit_id;
  }MBAP_HEADER_ST, *MBAP_HEADER_PST;
  typedef struct mb_tcp_adu {
      U16   transaction_id;
      U16   protocol_id;
      U16   len;
    U8    unit_id;
    U8    func_code;
    REQ_DATA_UT data;
  }MB_TCP_ADU_ST, *MB_TCP_ADU_PST;

/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  void tcp_mb_init(void);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _TCP_MB_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
