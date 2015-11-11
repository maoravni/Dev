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
#include <device.h>

T_`$INSTANCE_NAME`_Sys_Data	`$INSTANCE_NAME`_Sys_Data;

void `$INSTANCE_NAME`_Init( float Init_MeasuredOutput)
{
	{
		memset(&`$INSTANCE_NAME`_Sys_Data,0,sizeof(`$INSTANCE_NAME`_Sys_Data));
		`$INSTANCE_NAME`_Sys_Data.FiltSetPoint = Init_MeasuredOutput;
		`$INSTANCE_NAME`_Sys_Data.FF = 1;
	}	
}
uint8 `$INSTANCE_NAME`( float SetPoint, float MeasuredOutput )
{
	float `$INSTANCE_NAME`_Error; 
	float `$INSTANCE_NAME`_ErrorDerivative;
	float `$INSTANCE_NAME`_ErrorAbsDerivative;
	float `$INSTANCE_NAME`_Out;
	
	/* Lowpass Filtering the system's reference input */
	if ( MeasuredOutput > SetPoint)
	{
		`$INSTANCE_NAME`_Sys_Data.FiltSetPoint = SetPoint;
	}
	else
	{
		/* Aluminium Model LPF */
		`$INSTANCE_NAME`_Sys_Data.FiltSetPoint = 
		`$INSTANCE_NAME`_Sys_Data.FiltSetPoint + DT*(SetPoint - `$INSTANCE_NAME`_Sys_Data.FiltSetPoint)/(M_TAU + DT);
	}
	
	/* Feedforard */
	if (`$INSTANCE_NAME`_Sys_Data.Prev_SetPoint != SetPoint)
	{
		`$INSTANCE_NAME`_Sys_Data.t = K_FF*(M*C_H*(SetPoint - MeasuredOutput)/(P*DC_LIM/100));
		`$INSTANCE_NAME`_Sys_Data.Prev_SetPoint = SetPoint;
	}
	else if (`$INSTANCE_NAME`_Sys_Data.t>0)
	{
		`$INSTANCE_NAME`_Sys_Data.t = `$INSTANCE_NAME`_Sys_Data.t-DT;
		`$INSTANCE_NAME`_Sys_Data.ErrorIntegral = 0;
	}
	else
	{
		`$INSTANCE_NAME`_Sys_Data.FF=0;
	}
	/*******/
	/* PID */
	/******/
	/* Error */
	`$INSTANCE_NAME`_Error = `$INSTANCE_NAME`_Sys_Data.FiltSetPoint - MeasuredOutput;
	/* Error derivative */
	`$INSTANCE_NAME`_ErrorDerivative = (`$INSTANCE_NAME`_Error - `$INSTANCE_NAME`_Sys_Data.Prev_Error) / DT;
	
	`$INSTANCE_NAME`_ErrorAbsDerivative = ( ABS(`$INSTANCE_NAME`_Error) - ABS(`$INSTANCE_NAME`_Sys_Data.Prev_Error) ) / DT;
	if (`$INSTANCE_NAME`_ErrorAbsDerivative > 0)
	{
		if( ABS(`$INSTANCE_NAME`_Sys_Data.ErrorIntegral) < I_LIM )
		{
	  		`$INSTANCE_NAME`_Sys_Data.ErrorIntegral += ( `$INSTANCE_NAME`_Error * DT );
		}
	}	
	
	/* Output Calculation */
	`$INSTANCE_NAME`_Out = (	K_P*`$INSTANCE_NAME`_Error + 
								K_I*`$INSTANCE_NAME`_Sys_Data.ErrorIntegral + 
								K_D*`$INSTANCE_NAME`_ErrorDerivative + 
								DC_LIM*`$INSTANCE_NAME`_Sys_Data.FF
							);
	if (`$INSTANCE_NAME`_Out < 0)
	{
		`$INSTANCE_NAME`_Out = 0;	
	}
	else
	{
		`$INSTANCE_NAME`_Out = CHOP(`$INSTANCE_NAME`_Out,DC_LIM);
	}
	/* Limit and output the new duty cycle value */
	return( (uint8) `$INSTANCE_NAME`_Out);
}




















/* [] END OF FILE */
