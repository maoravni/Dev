/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <cytypes.h>

#if !defined (`$INSTANCE_NAME`_H `$H`)
#define `$INSTANCE_NAME`_H `$H`
	
/**********/
/* Macros */
/**********/
	
#define ABS(X)			( (X>0) ? X:(-X) )
#define CHOP(X,LIMIT)	( (X>LIMIT) ? LIMIT:X )
	
/*********************************/	
/* System parameters definitions */	
/*********************************/	
#define DT		`$dt`

#define C_H		`$Material_Heat_Capacity`
#define M		`$Material_Mass`	
#define P		`$Heaters_Power`	
#define M_TAU	`$Material_Model_Tau`
#define K_FF	`$K_ff`	

#define K_P		`$Kp`
#define K_I		`$Ki`
#define K_D		`$Kd`
	
#define I_LIM	`$Integral_Limit`/`$Ki`
#define DC_LIM	`$Output_Limit`	
	
/***********************************/
/* Data need to be saved structure */
/***********************************/	
#pragma pack(1)
typedef struct
{
	float FiltSetPoint;		// 
	float ErrorIntegral;	//
	float Prev_Error;		//
	float Prev_SetPoint;	//
	float t;				//
    uint8 FF;				//
} T_`$INSTANCE_NAME`_Sys_Data;
#pragma pack()

/***********************/
/* Fanction prototypes */
/***********************/
void `$INSTANCE_NAME`_Init( float Init_MeasuredOutput );
uint8 `$INSTANCE_NAME`( float SetPoint, float MeasuredOutput );

#endif
/* [] END OF FILE */

