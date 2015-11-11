/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : FEC  
*
*  FILE        : test_board.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\FEC\SW\TEST 
*
*  PURPOSE     : The FEC board HW test support. 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  FEC at its own board.
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.10     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
*
******************************************************************************/

#ifndef _TEST_BOARD_H_
#define _TEST_BOARD_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */

  typedef enum {
    GP_OUT1,
    GP_OUT2,
    GP_OUT3,
    GP_OUT4,
    GP_OUT5,
    GP_OUT6,
    GP_OUT7,
    GP_OUT8,
    GP_OUT9,
    GP_OUT10,
    GP_OUT11,
    GP_OUT12,

    NUMBER_OF_GPs /* Keep this last */
  }GPIO_ID_ET;
  
/* --- PRIVATE_DATA --------------------------------------------------------- */
 
#ifdef  GPOUT_CTRL_IMPL_HERE
  
/*                                  GP_OUT1   GP_OUT2   GP_OUT3   GP_OUT4    GP_OUT5    GP_OUT6    GP_OUT7    GP_OUT8    GP_OUT9    GP_OUT10   GP_OUT11   GP_OUT12     */
  
  static GPIO_TypeDef* PortOf_[] = {GPIOE,      GPIOE,      GPIOE,      GPIOE,       GPIOE,       GPIOE,       GPIOE,       GPIOE,       GPIOC,       GPIOC,       GPIOC,       GPIOC      };
                         
  static const uint16_t PinOf_[] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_5,  GPIO_Pin_6,  GPIO_Pin_6,  GPIO_Pin_7,  GPIO_Pin_8,  GPIO_Pin_9};
  
#endif
void test_init_task(void * pvParameters);
/* --- EXPORTED CONSTANTS -------------------------------------------------- */
 
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _TEST_BOARD_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
