/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : HMI_LIB
*
*  FILE        : Monitor.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom/applic
*
*  PURPOSE     :   
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.08     Yossi Sarusi     1.1.0   First version
*
*******************************************************************************
      Copyright and Proprietary Statment.

******************************************************************************/

#ifndef _MONITOR_H_
#define _MONITOR_H_

/* Includes -----------------------------------------------------------------*/

/* Exported types -----------------------------------------------------------*/


/* Exported constants -------------------------------------------------------*/
#define MONITOR_PERIODIC_MSEC               250        /* 250 mSec tick                                  */
#define ALIVE_LED_CYCLE                      1        /* 250 msec ->> 1 X MONITOR_PERIDIC_MSEC          */
#define GENERAL_DB_MONITER_UPDATE_CYCLE      4        /* 1 sec  1000 msec ->> 4 X MONITOR_PERIDIC_MSEC  */ 
#define TEMPERATURE_DB_MONITER_UPDATE_CYCLE  4        /* 1 sec  1000 msec ->> 4 X MONITOR_PERIDIC_MSEC  */
#define TEMPERATURE_DB_MONITER_UPDATE_CYCLE_IN_ATP   2 /* 0,5 sec  500 msec ->> 4 X MONITOR_PERIDIC_MSEC */
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
  void EnableUpdateModbusDeviceStatusEnabe(BOOL Enable);  
  //void Monitor_init(int TaskPriority, int StackSize);
  //void Monitor_task(void * pvParameters);
  void monitor_function();
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
  
#endif /* _MONITOR_H_ */

/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/





