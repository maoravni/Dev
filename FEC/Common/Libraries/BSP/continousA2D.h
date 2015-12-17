/*
 * continousA2D.h
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#ifndef CONTINOUSA2D_H_
#define CONTINOUSA2D_H_

/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     :
 *
 *  FILE        : *.h
 *
 *  PATH        :
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
 *  2012.01     Igor Natachanny   1.0.0   Initial version
 *
 *******************************************************************************
 *      Copyright and Proprietary Statment.
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

#include <portmacro.h>
/* --- INCLUDES ------------------------------------------------------------ */
//   #include "ll_typedef.h"
#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif

//#include "free_rtos_exp.h"
#include "a2d_cnfg.h"
/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/

//#define MAX_MSG_SIZE   10
//#define USB_MCG_READY  0x0001
//#define ADC_Channels_number   17

#define ADC1_in0   0
#define ADC1_in3   1
#define ADC1_in5   2
#define ADC1_in6   3
#define ADC1_in8   4
#define ADC1_in9   5
#define ADC1_in10  6
#define ADC1_in12  7
#define ADC1_in13  8

#define ADC3_in4   9
#define ADC3_in5   10
#define ADC3_in6   11
#define ADC3_in7   12
#define ADC3_in8   13
#define ADC3_in9   14
#define ADC3_in14  15
#define ADC3_in15  16

//#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)
//#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)

//#define ADC_SAMPLE_TIME  ADC_SampleTime_112Cycles

typedef enum
{
    A2D_COMPLETE = 0, A2D_BUSY, WRONG_PARAM
} A2D_RES_ET;

extern float a2d_filtered_arr[A2D_NUM];
extern uint16_t a2d_samples_arr[A2D_NUM];

A2D_RES_ET a2d_read_dig(int param_id, uint16_t *val_ptr);
float a2d_read_filtered_data(int index);
void a2d_init_hw(void);
uint32_t a2d_start(unsigned portSHORT stack_depth, unsigned portBASE_TYPE priority);
void a2d_config_lpf_window(int window);

#ifdef __cplusplus
}
#endif
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/

#endif /* CONTINOUSA2D_H_ */
