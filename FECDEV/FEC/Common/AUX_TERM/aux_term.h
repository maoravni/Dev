/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  
*
*  FILE        : aux_term.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\AUX_TERM  
*
*  PURPOSE     : Auxiliary Terminal Services.
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
      Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _AUX_TERM_H_
#define _AUX_TERM_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */
#include "ll_typedef.h"
#include "aux_trm_cnfg.h"
/* --- EXPORTED TYPES ------------------------------------------------------ */
  typedef struct _aux_term_queue_item_ {
    int8_t *ptr;
    uint8_t len;
  }AUX_TERM_QUE_ITM_ST, *AUX_TERM_QUE_ITM_PST;

/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  portBASE_TYPE aux_term_init(unsigned portBASE_TYPE in_queue_len, unsigned portSHORT stack_depth, unsigned portBASE_TYPE priority);
  portBASE_TYPE AuxTermSendMsg(int8_t *msg_txt_ptr, int8_t MsgLen);
  void AuxPrintMsg(char * Msg, uint8_t MsgLen);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/
//  #define AUX_TERM_QUEUE_SZ  20






#ifdef __cplusplus
}
#endif
#endif /* _AUX_TERM_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
