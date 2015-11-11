/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  FEC
*
*  FILE        : a2d_cnfg.h
*   
*  PATH        :  
*
*  PURPOSE     : STM32F207 internal A2D HAL.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.12     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
*
******************************************************************************/

#ifndef _A2D_CNFG_H_
#define _A2D_CNFG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */
  typedef enum {
    RTD_1 = 0,
    RTD_2,
    RTD_3,
    RTD_4,
    RTD_5,
    RTD_6,
    RTD_7,
    RTD_8,
    RTD_9,
    HUM_SENS_1, //9
    HUM_SENS_2,
    in_4_20mA_1,
    in_4_20mA_2,
    in_4_20mA_3,
    RTD_12,     //14
    RTD_10,
    RTD_11,
    A2D_NUM      /* Keep it last */
  }A2D_DATA_ET;
  
#ifdef RTD_RES_RETURN           /* RTD_1 RTD_2 RTD_3 RTD_4 RTD_5 RTD_6 RTD_7 RTD_8 RTD_9 RTD_10 RTD_11 RTD_12 */  
  const uint8_t  RtdId_2_Indx[] = {0,    1,    2,    3,    4,    5,    6,    7,    8,    15,    16,    14};  
#endif

/* --- EXPORTED CONSTANTS -------------------------------------------------- */
#define LAST_RTDs_INDX_OFFSET      RTD_12
#define HM_SENS_INDX_OFFSET         HUM_SENS_1   //9
#define AN_IN_4_20_mA_INDX_OFFSET  in_4_20mA_1  //11
  
  #define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)
  #define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)

  #define ADC_SAMPLE_TIME  ADC_SampleTime_480Cycles
  
  #define FIRST_IN_CONV_1                    RTD_1
  #define FIRST_IN_CONV_2                    HUM_SENS_1
  #define ADC_AVG_DIV_FACTOR                  6
  #define ADC_SMPLS_NUM                       1<<ADC_AVG_DIV_FACTOR
  #define A2D_CONV_FINISH_SEM_TICK_TO_WAIT   50   //100msec
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
#endif /* _A2D_CNFG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
