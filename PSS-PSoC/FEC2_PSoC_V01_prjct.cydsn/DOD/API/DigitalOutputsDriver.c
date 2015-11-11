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

#if !defined (`$INSTANCE_NAME`_C `$C`)
	#define `$INSTANCE_NAME`_C `$C`
	T_`$INSTANCE_NAME`_Configuration `$INSTANCE_NAME`_Configuration;
#endif



/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start(void)
********************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start(void)
{
	`$INSTANCE_NAME`_Control_Reg_Write(0x3C);
	`$INSTANCE_NAME`_PrISM_Start();
	`$INSTANCE_NAME`_SPWM_Init();
	`$INSTANCE_NAME`_PWM_1_Start();
	`$INSTANCE_NAME`_PWM_2_Start();
	`$INSTANCE_NAME`_PWM_3_Start();
	`$INSTANCE_NAME`_PWM_4_Start();
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_OutConfig(uint8 Channeluint8 Config)
********************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_OutConfig(uint8 Channel, uint8 Config)
{
	uint8 Status = 0;
	switch(Channel)
	{
		/* Slow PWM 1*/
		case 0:
		{
			/* Configure Channel */		
			switch(Config)
			{
			// channel Mux select PrSIM
				case 0x00:
				{
					/* Disable the Channel also */
					`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()&(~0x41)); 
					`$INSTANCE_NAME`_Configuration.SlowPWM1 = 0x0;
					break;
				}
			// channel Mux select SPWM	
				case 0x01:
				{
					/* Duty cycle can be Zero */
					`$INSTANCE_NAME`_SPWM_SetMode(1);
					`$INSTANCE_NAME`_SPWM_Write_DC(0);
					/* Enable the Channel also */
					`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x41);
					`$INSTANCE_NAME`_Configuration.SlowPWM1 = 0x1;
					break;
				}
			// channel Mux select PWM	
				case 0x02:
				{
					`$INSTANCE_NAME`_SPWM_SetMode(0);
					/* Enable the Channel also */
					`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x41);
					`$INSTANCE_NAME`_Configuration.SlowPWM1 = 0x2;
					break;
				}
			// channel Mux select Digital Output (PWM)	
				case 0x03:
				{				
					/* Disable the Channel also */
					`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()&(~0x01));
					/* select the SPWM mux Channel*/
					`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x40);
					/* Set the SPWM to 100% */
					`$INSTANCE_NAME`_SPWM_SetMode(0);
					/* Set PWM Duty Cycle to 100% when channel Enabled ('HIGH')*/	
					`$INSTANCE_NAME`_SPWM_Write_DC(100);
					`$INSTANCE_NAME`_Configuration.SlowPWM1 = 0x3;
					break;
				}				
				default:
				{
					/* Disable the Channel */
					`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()&(~0x01));					
					// Non-relevant option
					Status = 2;
					break;
				}
			}
			break;
		}
		/* Slow PWM 2*/
		case 1:
		{
			/* Disable Channel */
			`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()&(~0x02));
			/* Configure Channel */	
		// 0. PrISM			
			if(Config == 0x00)
			{
				`$INSTANCE_NAME`_Configuration.SlowPWM2 = 0x0;
			}
		// 1. Digital Output.	
			else if(Config == 0x03)
			{
				`$INSTANCE_NAME`_PrISM_WritePulse1(0xFFFF);
				`$INSTANCE_NAME`_Configuration.SlowPWM2 = 0x1;
			}
			else
			{
				/* Disable the Channel */
				`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()&(~0x02));				
				Status = 2;
			}
			break;
		}
		/* PWM Output 1*/
		case 2:
		{
			/* Disable Channel */
			`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x04);
		// 0. PWM			
			if(Config == 0x02)
			{
				`$INSTANCE_NAME`_Configuration.PWM1 = 0x0;
			}
		// 1. Digital Output.	
			else if(Config == 0x03)
			{
				`$INSTANCE_NAME`_PWM_1_WriteCompare(99);
				`$INSTANCE_NAME`_Configuration.PWM1 = 0x1;
			}
			else
			{
				/* Kill the Channel */
				`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x04);
				Status = 2;
			}		
			break;
		}
		/* PWM Output 2*/
		case 3:
		{
			/* Disable Channel */
			`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x08);
		// 0. PWM			
			if(Config == 0x02)
			{
				`$INSTANCE_NAME`_Configuration.PWM2 = 0x0;
			}
		// 1. Digital Output.	
			else if(Config == 0x03)
			{
				`$INSTANCE_NAME`_PWM_2_WriteCompare(99);
				`$INSTANCE_NAME`_Configuration.PWM2 = 0x1;
			}
			else
			{
				/* Kill the Channel */
				`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x08);				
				Status = 2;
			}		
			break;
		}
		/* HB_PWM Output 1*/
		case 4:
		{
			/* Disable Channel */
			`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x10);
		// 0. PWM			
			if(Config == 0x02)
			{
				`$INSTANCE_NAME`_Configuration.HB_PWM1 = 0x0;
			}
		// 1. Digital Output.	
			else if(Config == 0x03)
			{
				`$INSTANCE_NAME`_PWM_3_WriteCompare(99);
				`$INSTANCE_NAME`_Configuration.HB_PWM1 = 0x1;
			}
			else
			{
				/* Kill the Channel */
				`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x10);
				Status = 2;
			}		
			break;
		}
		/* HB_PWM Output 2*/
		case 5:
		{
			/* Disable Channel */
			`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x20);
		// 0. PWM			
			if(Config == 0x02)
			{
				`$INSTANCE_NAME`_Configuration.HB_PWM2 = 0x0;
			}
		// 1. Digital Output.	
			else if(Config == 0x03)
			{
				`$INSTANCE_NAME`_PWM_4_WriteCompare(99);
				`$INSTANCE_NAME`_Configuration.HB_PWM2 = 0x1;
			}
			else
			{
				/* Kill the Channel */
				`$INSTANCE_NAME`_Control_Reg_Write(`$INSTANCE_NAME`_Control_Reg_Read()|0x20);				
				Status = 2;
			}		
			break;
		}		
		default:
		{
			Status = 1;
			break;
		}
	}
	return(Status);
}

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_SlowPWM1_Write(uint8 dc)
{
	switch(`$INSTANCE_NAME`_Configuration.SlowPWM1)
	{
	// 0. PrSIM
		case 0:
		{
			if(dc)
			{
				if(dc > 100)
				{
					dc = 100;	
				}
				`$INSTANCE_NAME`_PrISM_WritePulse0((uint16) (`$INSTANCE_NAME`_MAX_PULSE_DENSITY * dc));
				`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x01 );
			}
			else
			{
				`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x01) );
			}	
			break;
		}
	// 1. SPWM- Already configured as SPWM	
		case 1:
		{
			if(dc > 100)
			{
				dc = 100;	
			}
			`$INSTANCE_NAME`_SPWM_Write_DC(dc);	
			break;		
		}
	// 2. PWM- Already configured as UPWM
		case 2:
		{
			if(dc > 100)
			{
				dc = 100;	
			}
			`$INSTANCE_NAME`_SPWM_Write_DC(dc);		
			break;
		}
	// 3. Digital Output
		case 3:
		{
			if(dc)
			{
				`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x01 );
				dc = 1;
			}
			else
			{
				`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x01) );
			}
			break;
		}
		default:
		{
			/* Send an Error */
			dc = 0xFF;
			break;
		}		
	}
	return(dc);	
}

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_SlowPWM2_Write(uint8 dc)
{
	if(`$INSTANCE_NAME`_Configuration.SlowPWM2)
	{
		if(dc)
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x02 );
			dc = 1;
		}
		else
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x02) );
		}
	}
	else
	{
		if(dc)
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x02 );
			if(dc > 100)
			{
				dc = 100;	
			}
			`$INSTANCE_NAME`_PrISM_WritePulse1((uint16) (`$INSTANCE_NAME`_MAX_PULSE_DENSITY * dc));
		}
		else
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x02) );	
		}
	}
	return(dc);	
}

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_PWMOut1_Write(uint8 dc)
{
	if(`$INSTANCE_NAME`_Configuration.PWM1)
	{
		if(dc)
		{
		/* Don't kill Output*/
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x04) );
			dc = 1;
		}
		else
		{
		/* Kill Output*/
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x04 );
		}
	}
	else
	{
		if(dc)
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x04) );
			if(dc > 100)
			{
				dc = 100;	
			}
			`$INSTANCE_NAME`_PWM_1_WriteCompare(dc-1);
		}
		else
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x04 );	
		}
	}
	return(dc);		
}

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_PWMOut2_Write(uint8 dc)
{
	if(`$INSTANCE_NAME`_Configuration.PWM2)
	{
		if(dc)
		{
		/* Don't kill Output*/
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x08) );
			dc = 1;
		}
		else
		{
		/* Kill Output*/
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x08 );
		}
	}
	else
	{
		if(dc)
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x08) );
			if(dc > 100)
			{
				dc = 100;	
			}
			`$INSTANCE_NAME`_PWM_2_WriteCompare(dc-1);
		}
		else
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x08 );	
		}
	}
	return(dc);		
}

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_HB_PWMOut1_Write(uint8 dc)
{
	if(`$INSTANCE_NAME`_Configuration.HB_PWM1)
	{
		if(dc)
		{
		/* Don't kill Output*/
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x10) );
			dc = 1;
		}
		else
		{
		/* Kill Output*/
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x10 );
		}
	}
	else
	{
		if(dc)
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x10) );
			if(dc > 100)
			{
				dc = 100;	
			}
			`$INSTANCE_NAME`_PWM_3_WriteCompare(dc-1);
		}
		else
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x10 );	
		}
	}
	return(dc);			
}

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary:
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_HB_PWMOut2_Write(uint8 dc)
{
	if(`$INSTANCE_NAME`_Configuration.HB_PWM2)
	{
		if(dc)
		{
		/* Don't kill Output */
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x20) );
			dc = 1;
		}
		else
		{
		/* Kill Output*/
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x20 );
		}
	}
	else
	{
		if(dc)
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() & (~0x20) );
			if(dc > 100)
			{
				dc = 100;	
			}
			`$INSTANCE_NAME`_PWM_4_WriteCompare(dc-1);
		}
		else
		{
			`$INSTANCE_NAME`_Control_Reg_Write( `$INSTANCE_NAME`_Control_Reg_Read() | 0x20 );	
		}
	}
	return(dc);	
}

/* [] END OF FILE */
