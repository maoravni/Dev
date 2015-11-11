/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : FEC
 *
 *  FILE        : dg_input_cnfg.h
 *
 *  PATH        :
 *
 *  PURPOSE     : Digital Inputs HAL.
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

#ifndef _DIG_IN_CNFG_H_
#define _DIG_IN_CNFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* --- INCLUDES ------------------------------------------------------------ */
#include "ll_typedef.h"
/* --- EXPORTED TYPES ------------------------------------------------------ */
typedef enum _dg_in_id_et_
{
//    BOARD_ID_0 = 0,
//    BOARD_ID_1,
//    BOARD_ID_2,
//    NODE_ID_0,
//    NODE_ID_1,
//    NODE_ID_2,
    GLOBAL_IN_1_FEC2,
    GLOBAL_IN_1_FEC3,
    GLOBAL_IN_2,
    GLOBAL_IN_3,
    GLOBAL_IN_4,
    GLOBAL_IN_5_FEC2,
    GLOBAL_IN_5_FEC3,
    GLOBAL_IN_6,
    DIG_IN_1,
    DIG_IN_2,
    DIG_IN_3,
    DIG_IN_4,
    DIG_IN_5,
    DIG_IN_6,
    SAFETY_FEEDBACK,
//    DIG_IN_7,
//    DIG_IN_8,
//    DIG_IN_9,
//    DIG_IN_10,
//    DIG_IN_11,
//    DIG_IN_12,
//    SSID_1,
//    SSID_2,
//    SSID_3,
//    SSID_4,
    NUMBER_OF_DG_INs /* Keep this last */
} DIG_IN_ID_ET;

/* --- PRIVATE_DATA --------------------------------------------------------- */

#ifdef  DG_IN_CTRL_IMPL_HERE

/*                                   GLOB_IN_1   GLOB_IN_2    GLOB_IN_3   GLOB_IN_4   GLOB_IN_5   GLOB_IN_6    DIG_IN_1     DIG_IN_2     DIG_IN_3     DIG_IN_4     DIG_IN_5     DIG_IN_6     SAFETY_FEEDBACK*/

static GPIO_TypeDef* PortOf[] =
{   GPIOF, // Global Input 1 Fec 2
        GPIOE, // Global Input 1 Fec 3
        GPIOG, //Global Input 2
        GPIOG, // Global Input 3
        GPIOG, // Global Input 4
        GPIOA, // Global Input 5 Fec 2
        GPIOE, // Global Input 5 Fec 3
        GPIOG, // Global Input 6
        GPIOE, // Input 1
        GPIOB, // Input 2
        GPIOD, // Input 3
        GPIOD, // Input 4
        GPIOD, // Input 5
        GPIOE, // Input 6
        GPIOF, // Safety Feedback
};

static const uint16_t PinOf[] =
{   GPIO_Pin_1, // Global Input 1 Fec 2
        GPIO_Pin_4, // Global Input 1 Fec 3
        GPIO_Pin_15, // Global Input 2
        GPIO_Pin_7, // Global Input 3
        GPIO_Pin_8, // Global Input 4
        GPIO_Pin_6, // Global Input 5 Fec 2
        GPIO_Pin_9, // Global Input 5 Fec 3
        GPIO_Pin_13, // Global Input 6
        GPIO_Pin_11, // Input 1
        GPIO_Pin_10, // Input 2
        GPIO_Pin_4, // Input 3
        GPIO_Pin_5, // Input 4
        GPIO_Pin_13, // Input 5
        GPIO_Pin_14, // Input 6
        GPIO_Pin_5, // Safety Feedback
};

#endif
/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

#ifdef __cplusplus
}
#endif
#endif /* _DIG_IN_CNFG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
