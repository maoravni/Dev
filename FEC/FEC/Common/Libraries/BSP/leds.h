/******************************************************************************
 *                     (C) LandaLabs 2011                                      *
 *******************************************************************************
 *
 *  PROJECT     : HMI_LIB
 *
 *  FILE        : leds.h
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\HMI
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
 *  2012.07     Igor Natachanny   1.1.0   alive_led_task added.
 *
 *******************************************************************************
 Copyright and Proprietary Statment.

 ******************************************************************************/

#ifndef _LEDS_H_
#define _LEDS_H_

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes -----------------------------------------------------------------*/
#include "leds_cnfg.h"
#include "ll_typedef.h"

typedef enum
{
//    TOP_LED1,
//    TOP_LED2,
//    TOP_LED3,
//    TOP_LED4,
//    LABL_RESET,
    ALIVE_LED,
//    MBS_TX_LED,
    MODBUS_LED,
    LABL_TCP,
    LABL_UDP,
//    CAN_TX_LED,
//    CAN_RX_LED,
    CPU_ACTIVITY_LED8,
    LABL_SAFETY_LED9,  //SAFETY
//    DIP_LED,
//    ALIVE_LED,
    NUMBER_OF_LEDS /* Keep this last */
} LED_ID_ET;

/* Exported types -----------------------------------------------------------*/

/* Exported constants -------------------------------------------------------*/

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
void Turn_led_(LED_ID_ET led, BOOL State);
void led_light(LED_ID_ET Led);
void led_dark(LED_ID_ET Led);
void led_toggle(LED_ID_ET Led);
void LedAliveToggle();
void alive_led_task(void * pvParameters);

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* _LEDS_H_ */

/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/

