/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  
*
*  FILE        :   tmp275.h 
*   
*  PATH        :  $PROJ_DIR$..\..\..\Common\Libraries\BSP
*
*  PURPOSE     :  // talk to tmp75/175/275 family of i2c temperature sensors
*                 // eric volpe (epvuc@limpoc.com) 3/18/2009 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.40.1.3812
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


#ifndef _TMP_275_H_
#define _TMP_275_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */
#define TMP75_ADDR   0x92
   
#define TMP75_OS 	(1<<7)
#define TMP75_12BIT	((1<<6)|(1<<5))
#define TMP75_11BIT	(1<<6)
#define TMP75_10BIT	(1<<5)
#define TMP75_9BIT	0
#define TMP75_FQ1	0
#define TMP75_FQ2	(1<<3)
#define TMP75_FQ4	(1<<4)
#define TMP75_FQ6	((1<<3)|(1<<4))
#define TMP75_POL	(1<<2)
#define TMP75_TM	(1<<1)
#define	TMP75_SD	(1<<0)


/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
int16_t tmp75_read_tempr(void);

//uint16_t tmp75_read_temp(void);
uint16_t tmp75_read_tlow(void);
uint16_t tmp75_read_thi(void);
uint8_t tmp75_read_config(void);
void tmp75_write_config(uint8_t conf);
void tmp75_write_tlow(uint16_t tlow);
void tmp75_write_thi(uint16_t thi);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _TMP_275_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
