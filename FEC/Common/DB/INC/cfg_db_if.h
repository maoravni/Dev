/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : CNFG_DB_LIB 
*
*  FILE        : cfg_db_if.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\DB\INC  
*
*  PURPOSE     : Services of Configuration Data Base Interface.
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

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _CFG_DB_IF_SRVCS_H_
#define _CFG_DB_IF_SRVCS_H_

#include "rtu_msgs.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */
#if 1
#pragma pack(1)
 typedef struct{
   uint8_t  instance;
   uint32_t  db_id;
   uint8_t  db_id_inx;
   uint16_t  mb_reg;
 }CFG_DB_IF_ENTRY_ST, *CFG_DB_IF_ENTRY_PS;
#pragma pack()
#endif
 
#if 0
 typedef struct{
   uint32_t  instance;
   uint32_t  db_id;
   uint32_t  db_id_inx;
   uint16_t  mb_reg;
 }CFG_DB_IF_ENTRY_ST, *CFG_DB_IF_ENTRY_PS;
#endif
/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  LL_STATUS db_req_hndr(uint8_t func_code, REQ_DATA_UT data, uint8_t *mb_tcp_resp, struct netconn *newconn);
  STATUS_ET cfg_db_if_init();
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _CFG_DB_IF_SRVCS_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
