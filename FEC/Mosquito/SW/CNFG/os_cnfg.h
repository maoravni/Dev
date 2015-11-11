/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : Mosquito
*
*  FILE        : os.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Mosquito\SW\CNFG  
*
*  PURPOSE     : This project specific os Library configuration Data.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : Mosquito Board  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.07    Yossi Sarusi   1.0.0   Initial version
*
******************************************************************************/

#ifndef _OS_CNFG_H_
#define _OS_CNFG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */
 
  
  

/* --- EXPORTED CONSTANTS -------------------------------------------------- */

   //modbus slave task
#define MODBUS_TASK_PRIORITY   ( tskIDLE_PRIORITY + 1 )
#define MODBUS_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE + 350 )
   
   //CmdHnd  task
#define CMDHND_TASK_PRIORITY   ( tskIDLE_PRIORITY + 1 )
#define CMDHND_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE + 350 )
  
 
  
  

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

#endif /* _OS_CNFG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
