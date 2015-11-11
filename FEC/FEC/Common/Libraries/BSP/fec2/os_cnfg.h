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
//A2D task
#define A2D_TASK_PRIORITY   ( tskIDLE_PRIORITY ) // Low pri
#define A2D_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE  ) 

//Init task
#define INIT_TASK_PRIORITY   ( tskIDLE_PRIORITY ) // Low pri
#define INIT_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE*2  )
   
 //Terminal task
#define TERMINAL_TASK_PRIORITY   ( tskIDLE_PRIORITY +2 ) // low pri
#define TERMINAL_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE + 50 )
   
//USB task
#define USB_TASK_PRIORITY   ( tskIDLE_PRIORITY +2 ) // low pri
#define USB_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE + 50 )   
   
   //modbus task
#define MODBUS_TASK_PRIORITY   ( configMAX_PRIORITIES -2  ) //high priority
#define MODBUS_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE + 200 )
   
   //Monitor  task
#define MONITOR_TASK_PRIORITY   ( tskIDLE_PRIORITY+1) //low priority
#define MONITOR_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE + 350 )
  
//PWM  task
#define PWM_TASK_PRIORITY   ( configMAX_PRIORITIES-1) //high priority
#define PWM_TASK_STACK_SZ   ( configMINIMAL_STACK_SIZE + 250 )

//TCP MB task
#define TCP_MB_TASK_PRIORITY   ( tskIDLE_PRIORITY+5) //high priority
#define TCP_MB_TASK_STACK_SZ   ( DEFAULT_THREAD_STACKSIZE )

//BTC task
#define BTC_TASK_PRIORITY   ( tskIDLE_PRIORITY+3) //medium priority
#define BTC_TASK_STACK_SZ   ( DEFAULT_THREAD_STACKSIZE+500 )

   
// tcp_mp pri=2
//tcp_ip pri=5   
   
  
  
#ifdef __cplusplus
 }
#endif
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
